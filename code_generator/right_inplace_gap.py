# ----------------------------------------------------------------------
# Project: Right In-Place Convolution
# Title:   right_inplace_gap.py
#
# Reference papers:
#  - Right In-Place (RiP) Convolution: A Simple and General Strategy for Memory-Efficient CNN Inference, arXiv 2026
#    https://arxiv.org/abs/2610.00586
# Contact authors:
#  - Opegbemi Matthias Busoye, matthias@powerlabstech.com
#  - Tolulope Matthew Busoye, matthew@powerlabstech.com
#  - Eghonghon-aye Eigbe, eghonghon@powerlabstech.com
#
# Target ISA:  ARMv7E-M
# ----------------------------------------------------------------------
"""
The gap a right in-place convolution needs, under the dense layout the kernels use: padding is not
stored, so only the real input pixels occupy the workspace. Taken verbatim from the paper's reference
implementation, right_inplace.py, with only the functions right_inplace_gap_dense_ext reaches.
"""

import math


def k_eff(k, d):
    """Return the effective kernel size"""
    return (k - 1) * d + 1

def out_dims(hin, win, k_h, k_w, s_h, s_w, d_h, d_w, p_h, p_w):
    """Return (H_p, W_p, H_out, W_out) for the given convolution parameters."""
    hin_p = hin + 2 * p_h
    win_p = win + 2 * p_w
    k_eff_h = k_eff(k_h, d_h)
    k_eff_w = k_eff(k_w, d_w)
    hout = (hin_p - k_eff_h) // s_h + 1
    wout = (win_p - k_eff_w) // s_w + 1
    return hin_p, win_p, hout, wout


def index_end_exclusive(r, c, w, ch):
    """The last channel index of a pixel, exclusive"""
    return (r * w + c+1) * ch

def index_start_inclusive(r, c, w, ch):
    """The first channel index of a pixel, inclusive"""
    return (r * w + c) * ch


def next_live_input(
    r, c, hin, win, k_h, k_w,
    s_h, s_w, d_h, d_w, p_h, p_w
):
    """
    Returns the coordinate of the first still-live input after writing output
    (r, c), under the dense layout (padding not materialized).
    (None, None) when nothing is live, i.e. at the last output pixel.
    """
    _, _, hout, wout = out_dims(
        hin, win, k_h, k_w,
        s_h, s_w, d_h, d_w, p_h, p_w
    )
    ROW = "row"
    COL = "col"

    def get_axis_params(axis):
        if axis == ROW:
            k, s, d, p, l, out = k_h, s_h, d_h, p_h, hin, hout
        elif axis == COL:
            k, s, d, p, l, out = k_w, s_w, d_w, p_w, win, wout
        else:
            raise RuntimeError(
                f"Received an invalid axis {axis}, expect one of ('{ROW}' or '{COL}')"
            )
        return k, s, d, p, l, out

    def _tap(o, axis):
        """
        Returns the index of the next valid tap in the input
        real region of a given axis
        """
        k, s, d, p, l, _ = get_axis_params(axis)
        # the index of the first tap in the relative to the first real image
        ft_idx = o * s - p
        # the index of taps that behind (fall in the padded region) the first real
        # tap, it is zero if ft >= 0
        n_pt = max(0, math.ceil(-ft_idx / d))
        # the index of the first tap that is the real input space
        rft_idx = ft_idx + n_pt * d
        if n_pt <= k-1 and rft_idx < l:
            # does this output have at least one tap in the real image space
            return rft_idx
        return None

    def _next(o, axis):
        "return the next output with a valid tap in a given axis"
        _, s, _, p, l, out = get_axis_params(axis)
        # the valid nli range for the given axis
        # the integer transition point between real and padded regions
        v_lo, v_hi = math.ceil(p/s), min(out-1, math.floor((l - 1 + p) / s))

        # NOTE: If padding us zero, all taps are valid so this loop would not
        #       be needed, it is fun to assert that
        if p == 0:
            assert len(range(max(0, o), min(v_lo, out))) == 0,(
                f"Monotone validity for zero padded input breaks, this should not "
                "be possible, check the maths."
            )

        # check the left side of the vertex if there is a valid tap
        for j in range(max(0, o), min(v_lo, out)):
            i = _tap(j, axis)
            if i is not None: return j

        # this then becomes the first valid tap
        j = max(o, v_lo)
        if j <= v_hi:
            return j
        return None

    # we have the next out if they are both valid
    rn = r if _tap(r, ROW) is not None else None
    cn = _next(c+1, COL)

    if rn is None or cn is None:
        # we have no remaining valid output on the row, we advance
        rn = _next(r+1, ROW)
        cn = _next(0, COL)

    # no more next live input
    if rn is None or cn is None:
        return None, None

    return _tap(rn, ROW), _tap(cn, COL)



def right_inplace_gap_dense_ext(
    cin, cout, k_h, k_w, hin, win, s_h=1, s_w=1,
    d_h=1, d_w=1, p_h=0, p_w=0, load_input_receptive_field=False
):
    """
    Safe gap for right-inplace convolution under a DENSE layout: padding is
    not materialized, so only the hin*win*cin real input pixels occupy
    workspace (packed at gap + (r*win+c)*cin), and kernel taps landing in the
    padding region read/write nothing.

    Same debt as right_inplace_gap_ext, but a different candidate set. However,
    in there padding is stored like any other pixel, so the next live input
    advances smoothly and a fixed 3x3 grid of extremes suffices. Here the padding
    boundary makes D(r, c) piecewise-linear: it kinks where the receptive
    field crosses from padding into real input, and again where it leaves real
    input for padding. Those kinks land at rows/columns determined by
    (p - j*d)/s, so every one is enumerated below and added to the extremes.

    Requires p_h < k_eff_h and p_w < k_eff_w (matches
    validate_right_inplace_dense_ext's precondition): with padding at least
    as wide as the dilated kernel, some output would read no real input at all
    and the dense layout could not represent it.
    """

    hin_p, win_p, hout, wout = out_dims(
        hin, win, k_h, k_w,
        s_h, s_w, d_h, d_w, p_h, p_w
    )
    if hout <= 0 or wout <= 0:
        return 0

    k_eff_h = k_eff(k_h, d_h)
    k_eff_w = k_eff(k_w, d_w)
    assert p_h < k_eff_h, (
        f"padding p_h={p_h} must be < k_eff_h={k_eff_h}, "
        "this prevents out entirely from pad value which contains no information"
    )
    assert p_w < k_eff_w, (
        f"padding p_w={p_w} must be < k_eff_w={k_eff_w}, "
        "this prevents out entirely from pad value which contains no information"
    )

    # See right_inplace_gap_ext: C_out slots act as a swap buffer only when the
    # receptive field is re-read per output channel.
    swap_buffer = 0 if load_input_receptive_field else cout

    def debt(r, c):
        # debt = out_abs_end(r, c) - NLI(r, c). Padding is NOT materialized, so
        # addresses are in the dense space and next_live_input needs the real p.
        nli_r, nli_c = next_live_input(
            r=r, c=c, hin=hin, win=win, k_h=k_h, k_w=k_w,
            s_h=s_h, s_w=s_w, d_h=d_h, d_w=d_w, p_h=p_h, p_w=p_w
        )
        if nli_r is None and nli_c is None:
            return swap_buffer
        return (index_end_exclusive(r, c, wout, cout)
                - index_start_inclusive(nli_r, nli_c, win, cin)
                + swap_buffer)

    # Candidate rows/columns: the extremes, plus every kink. A kink sits where
    # tap j crosses into the real region, (p - j*d)/s, and where it leaves at
    # the far edge, (p - j*d + len - 1)/s. Both sides of each are taken, since
    # ceil() lands on the first output past the crossing.
    c_c = {0, max(0, wout-2), wout-1}
    for i in range(k_w):
        for v in ((p_w - i*d_w) / s_w, (p_w - i*d_w + win - 1) / s_w):
            c_c.add(int(math.ceil(v)))
            c_c.add(int(math.ceil(v)) - 1)

    r_c = {0, max(0, hout-2), hout-1}
    for j in range(k_h):
        for v in ((p_h - j*d_h) / s_h, (p_h - j*d_h + hin - 1) / s_h):
            r_c.add(int(math.ceil(v)))
            r_c.add(int(math.ceil(v)) - 1)

    # Kink formulas can land outside the grid; keep only real output indices.
    r_c = [r for r in r_c if 0 <= r <= hout - 1]
    c_c = [c for c in c_c if 0 <= c <= wout - 1]

    candidates = [debt(r, c) for r in r_c for c in c_c]

    return max(
        candidates +
        # in case gap + input size cannot accommodate the output size
        [hout*wout*cout - hin * win * cin]
    )
