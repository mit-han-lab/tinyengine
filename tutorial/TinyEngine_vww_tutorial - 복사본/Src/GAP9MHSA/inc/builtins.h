/*
 * builtins.h
 *
 *  Created on: Apr 15, 2024
 *      Author: sm5pyo
 */

#ifndef GAP9MHSA_INC_BUILTINS_H_
#define GAP9MHSA_INC_BUILTINS_H_

#ifdef __cplusplus
 extern "C" {
#endif


/** @defgroup groupBuiltinsAPI GCC built-ins for GAP8 RISC-V ISA extensions
@ingroup groupBuiltins
@{ */

typedef   signed short v2s __attribute__((vector_size (4)));
typedef unsigned short v2u __attribute__((vector_size (4)));

typedef   signed char  v4s __attribute__((vector_size (4)));
typedef unsigned char  v4u __attribute__((vector_size (4)));

/** @defgroup BitManip BitManipulation
@{ */

/*
Bit manipulation builtins
*/

/** @brief Bit position of the last bit set in x starting from MSB.

Bit position of the last bit set in x starting from MSB. If bit 31 is set, returns 31. If only bit 0 is set returns 0.
If x is 0, returns 32.

Equivalent to pure gcc sequence (31 - __builtin_clz((x)))
*/

unsigned int __builtin_pulp_fl1(
	int x	/**< Argument */
	);

/** @brief Bit position of the first bit set in x, starting from LSB.

Bit position of the first bit set in x, starting from LSB. If bit 0 is set, returns 0. If only bit 31 is set, returns 31.
If x is 0, returns 32.

Equivalent to pure gcc __builtin_ctz((x))
*/
unsigned int __builtin_pulp_ff1(
	int x	/**< Argument */
	);
/** @brief Count leading bits of x

Count leading bits of x. This is the number of consecutive 1’s or 0’s from MSB.  If x0 is 0, returns 0.

Equivalent to pure gcc sequence __builtin_clrsb((x))
*/
unsigned int __builtin_pulp_clb(
	int x	/**< Argument */
	);
/** @brief Count number of bits at 1 in x.

Count number of bits at 1 in x.

Equivalent to pure gcc sequence __builtin_popcount((x))
*/
unsigned int __builtin_pulp_cnt(
	int x	/**< Argument */
	);
/** @brief Returns x's parity.

Returns x's parity.

Equivalent to pure gcc sequence __builtin_parity((x))
*/
unsigned int __builtin_pulp_parity(
	int x	/**< Argument */
	);

/** @brief Clears x's bit not at 1 in mask, mask = ~((2^size -1)<<pos), mask is immediate.

Clears x's bit not at 1 in mask, mask = ~((2^size -1)<<pos), mask is immediate.

Available macros:

        gap8_bitclr(x, size, off);
        __BITCLR(x, size, off);
*/
int __builtin_pulp_bclr (
	int x,			/**< Argument */
	unsigned int mask	/**< mask = ~((2^size -1)<<pos), immediate */
	);
/** @brief Clears size x's bit starting from off, mask[9..5] = size-1, mask[4..0] = off.

Clears size x's bit starting from off, mask[9..5] = size-1, mask[4..0] = off.

Available macros:

        gap8_bitclr_r(x, size, off)
        __BITCLR_R(x, size, off)
*/
int __builtin_pulp_bclr_r(
	int x,			/**< Argument */
	unsigned int mask	/**< mask[9..5] = size-1, mask[4..0] = off */
	);

/** @brief Sets x's at 1 in mask, mask = (2^size -1)<<pos, mask is immediate.

Sets x's at 1 in mask, mask = (2^size -1)<<pos, mask is immedidate.

Available macros:

        gap8_bitset(x, size, off)
        __BITSET(x, size, off)
*/
int __builtin_pulp_bset (
	int x,			/**< Argument */
	unsigned int mask	/**< mask = (2^size -1)<<pos, mask is immediate. */
	);
/** @brief Sets size x's starting at off, mask = (2^size -1)<<pos

Sets size x's starting at off, mask = (2^size -1)<<pos

Available macros:

        gap8_bitset_r(x, size, off)
        __BITSET_R(x, size, off)
*/
int __builtin_pulp_bset_r(
	int x,			/**< Argument */
	unsigned int mask	/**< mask[9..5] = size-1, mask[4..0] = off */
	);

/** @brief Extracts size bits from x starting at offset off, sign extended. size and off immediates and in [0..31], (off+size)<=32

Extracts size bits from x starting at offset off, sign extended. size and off immediates and in [0..31], (off+size)<=32

Available macros:

        gap8_bitextract(x, size, off)
        __BITEXTRACT(x, size, off)
*/
int __builtin_pulp_bextract (
	int x,			/**< Argument */
	unsigned int size,	/**< Extract size bits from x */
	unsigned int off	/**< Extract from x at position off, in [0..31] */
	);
/** @brief Extracts size bits from x starting at offset off, size-1 in mask[9..5], off in mask[4..0], sign extended.

Extracts size bits from x starting at offset off, size-1 in mask[9..5], off in mask[4..0], sign extended.

Available macros:

        gap8_bitextract_r(x, size, off)
        __BITEXTRACT_R(x, size, off)
*/
int __builtin_pulp_bextract_r(
	int x,			/**< Argument */
	unsigned int mask	/**< mask[9..5]: extract size, mask[4..0]: extract offset */
	);

/** @brief Extracts size bits from x starting at offset off, zero extended. size and off immediates and in [0..31], (off+size)<=32

Extracts size bits from x starting at offset off, zero extended. size and off immediates and in [0..31], (off+size)<=32

Available macros:

	gap8_bitextractu(x, size, off)
	__BITEXTRACTU(x, size, off)
*/
unsigned int __builtin_pulp_bextractu (
	int x,			/**< Argument */
	unsigned int size,	/**< Extract size bits from x */
	unsigned int off	/**< Extract from x at position off, in [0..31] */
	);
/** @brief Extracts size bits from x starting at offset off, size-1 in mask[9..5], off in mask[4..0], zero extended.

Extracts size bits from x starting at offset off, size-1 in mask[9..5], off in mask[4..0], zero extended.

Available macros:

	gap8_bitextractu_r(x, size, off)
	__BITEXTRACTU_R(x, size, off)
*/
unsigned int __builtin_pulp_bextractu_r(
	int x,			/**< Argument */
	unsigned int mask	/**< mask[9..5]: extract size, mask[4..0]: extract offset */
	);

/** @brief Inserts first size bits of src into dst at offset off. size and off given by 2 immediate masks: not_mask_imm, mask_imm.

Inserts first size bits of src into dst at offset off. size and off given by 2 immediate masks: not_mask_imm, mask_imm.
Returns dst after insertion of src.

        Given size and off:
                not_mask_imm = ~((2^size-1)<<off)
                mask_imm = ((2^size-1)<<off)
        Warning: if mask_imm != ~not_mask_imm an error wil be generated

Available macros:

	gap8_bitinsert(x, size, off)
	__BITINSERT(x, size, off)
*/
int __builtin_pulp_binsert(
	int dst,			/**< Insert into dst */
	unsigned int not_mask_imm,	/**< Not extraction mask: ~((2^size-1)<<off) */
	int src,			/**< Source. First size bits of src are inserted into dst at position off */
	unsigned int mask_imm,		/**< Extraction mask: (2^size-1)<<off */
	unsigned int off		/**< offset, in [0..31] */
	);
/** @brief Inserts first size bits of src into dst at offset off. size-1 in mask_extract_insert[9..5], off in mask_extract_insert[4..0]

Inserts first size bits of src into dst at offset off. size-1 in mask_extract_insert[9..5], off in mask_extract_insert[4..0].xi
Returns dst after insertion of src.

Available macros:

	gap8_bitinsert_r(x, size, off)
	__BITINSERT_R(x, size, off)
*/
int __builtin_pulp_binsert_r(
	int dst,				/**< Destination */
	int src,				/**< Source. First size bits of src are inserted into dst at position off */
	unsigned int mask_extract_insert,	/**< size-1 in mask_extract_insert[9..5], off in mask_extract_insert[4..0] */
	unsigned int off			/**< extraction offset, also present of mask_insert_extract */
	);

/** @brief One bit right rotation

One bit right rotation

Available macros:

	gap8_rotr(x, size, off)
	__ROTR(x, size, off)
*/
int __builtin_pulp_rotr(
	int x	/**< Argument */
	);

/** @} */ // End of bit manipulation
/*
Arithmetic
*/


/** @defgroup Arith ArithmeticOperations
@{ */

/* Monadic */
/** @brief Absolute value

Absolute value
*/
unsigned int __builtin_pulp_abs(
	int x	/**< Argument */
	);

/* Diadid */

/** @brief Maximum of two signed integers

aximum of two signed integers
*/
int __builtin_pulp_maxsi(
	int x,	/**< 1st Argument */
	int y	/**< 2nd Argument */
	);
/** @brief Minimum of two signed integers

Minimum of two signed integers
*/
int __builtin_pulp_minsi(
	int x,	/**< 1st Argument */
	int y	/**< 2nd Argument */
	);
/** @brief Unsigned maximum of two integers

Unsigned maximum of two integers
*/
unsigned int __builtin_pulp_maxusi(
	unsigned int x,	/**< 1st Argument */
	unsigned int y	/**< 2bd Argument */
	);
/** @brief Unsigned minimum of two integers

Unsigned minimum of two integers
*/
unsigned int __builtin_pulp_minusi(
	unsigned int x,	/**< 1st Argument */
	unsigned int y	/**< 2bd Argument */
	);

/** @brief Fixed point addition followed by normalization, normalization is an immediate [0..31]

Fixed point addition followed by normalization, normalization is an immediate [0..31].

returns (x+y)>>n

Available macros:

	gap8_addnorm(x, y, n);
	__ADDNORM(x, y, n);
*/
int __builtin_pulp_addN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, immediate, arithmetic shift right */
	);

/** @brief Fixed point addition followed by normalization

Fixed point addition followed by normalization

returns (x+y)>>n

Available macros:

	gap8_addnorm_reg(x, y, n);
	__ADDNORM_REG(x, y, n);
*/
int __builtin_pulp_addN_r(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, arithmetic shift right */
	);

/** @brief Fixed point addition followed by rounding and then by normalization, normalization is an immediate [0..31]

Fixed point addition followed by rounding and then by normalization, normalization is an immediate [0..31]

Returns ((x + y) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_addroundnorm(x, y, n);
	__ADDROUNDNORM(x, y, n);
*/
int __builtin_pulp_addRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n,	/**< Normalization factor, immediate in [0..31] */
	unsigned int r 	/**< Rounding factor, = 2^(n-1) */
	);

/** @brief Fixed point addition followed by rounding and then by normalization.

Fixed point addition followed by rounding and then by normalization.

Returns ((x + y) + 2^(n-1)) >> n

Available macros:

	gap8_addroundnorm_reg(x, y, n);
	__ADDROUNDNORM_REG(x, y, n);
*/
int __builtin_pulp_addRN_r(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization and rounding factor */
	);

/** @brief Fixed point unsigned addition followed by normalization, normalization is an immediate [0..31]

Fixed point unsigned addition followed by normalization, normalization is an immediate [0..31]

Returns (x + y) >> n

Available macros:

	gap8_addnormu(x, y, n);
	__ADDNORMU(x, y, n);
*/
unsigned int __builtin_pulp_adduN(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, immediate in [0..31] */
	);

/** @brief Fixed point unsigned addition followed by normalization, normalization is in [0..31]

Fixed point unsigned addition followed by normalization, normalization is in [0..31]

Returns ((x + y) + 2^(n-1)) >> n

Available macros:

	gap8_addnormu_reg(x, y, n);
	__ADDNORMU_REG(x, y, n);
*/
unsigned int __builtin_pulp_adduN_r(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n	/**< Normalization factor */
	);

/** @brief Fixed point unsigned addition followed by rounding and then by normalization, normalization is an immediate in [0..31]

Fixed point unsigned addition followed by rounding and then by normalization, normalization is an immediate in [0..31]

Returns ((x + y) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_addroundnormu(x, y, n);
	__ADDROUNDNORMU(x, y, n);
*/
unsigned int __builtin_pulp_adduRN(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n,	/**< Normalization factor, immediate in [0..31] */
	unsigned int r	/**< Rounding factor, = 2^(n-1) */
	);

/** @brief Fixed point unsigned addition followed by rounding and then by normalization, normalization is in [0..31]

Fixed point unsigned addition followed by rounding and then by normalization, normalization is in [0..31]

Returns ((x + y) + 2^(n-1)) >> n

Available macros:

	gap8_addroundnormu_reg(x, y, n);
	__ADDROUNDNORMU_REG(x, y, n);
*/
unsigned int __builtin_pulp_adduRN_r(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n	/**< Normalization and rounding factor */
	);


/** @brief Fixed point substraction followed by normalization, normalization is an immediate [0..31]

Fixed point substraction followed by normalization, normalization is an immediate [0..31].

returns (x-y)>>n

Available macros:

	gap8_subnorm(x, y, n);
	__SUBNORM(x, y, n);
*/
int __builtin_pulp_subN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, immediate in [0..31], arithmetic shift right */
	);

/** @brief Fixed point substraction followed by normalization

Fixed point substraction followed by normalization

returns (x-y)>>n

Available macros:

	gap8_subnorm_reg(x, y, n);
	__SUBNORM_REG(x, y, n);
*/
int __builtin_pulp_subN_r(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, arithmetic shift right */
	);


/** @brief Fixed point substraction followed by rounding and then by normalization, normalization is an immediate [0..31]

Fixed point substraction followed by rounding and then by normalization, normalization is an immediate [0..31]

Returns ((x - y) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_subroundnorm(x, y, n);
	__SUBROUNDNORM(x, y, n);
*/
int __builtin_pulp_subRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n,	/**< Normalization factor, immediate in [0..31] */
	unsigned int r	/**< Rounding factor, = 2^(n-1) */
	);

/** @brief Fixed point substraction followed by rounding and then by normalization.

Fixed point substraction followed by rounding and then by normalization.

Returns ((x - y) + 2^(n-1)) >> n

Available macros:

	gap8_subroundnorm_reg(x, y, n);
	__SUBROUNDNORM_REG(x, y, n);
*/
int __builtin_pulp_subRN_r(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization and rounding factor */
	);


/** @brief Fixed point unsigned substraction followed by normalization, normalization is an immediate [0..31]

Fixed point unsigned substraction followed by normalization, normalization is an immediate [0..31]

Returns (x - y) >> n

Available macros:

	gap8_subnormu(x, y, n);
	__SUBNORMU(x, y, n);
*/
unsigned int __builtin_pulp_subuN(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n	/**< Normalization factor */
	);

/** @brief Fixed point unsigned substraction followed by normalization.

Fixed point unsigned substraction followed by normalization.

Returns (x - y) >> n

Available macros:

	gap8_subnormu_reg(x, y, n);
	__SUBNORMU_REG(x, y, n);
*/
unsigned int __builtin_pulp_subuN_r(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n	/**< Normalization factor */
	);



/** @brief Fixed point unsigned substration followed by rounding and then by normalization, normalization is an immediate in [0..31]

Fixed point unsigned substration followed by rounding and then by normalization, normalization is an immediate in [0..31]

Returns: ((x - y) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_subroundnormu(x, y, n);
	__SUBROUNDNORMU(x, y, n);
*/
unsigned int __builtin_pulp_subuRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n,	/**< Normalization factor, immediate value in [0..31] */
	unsigned int r	/**< Rounding factor, should be equal to 2^(n-1) */
	);

/** @brief Fixed point unsigned substration followed by rounding and then by normalization.

Fixed point unsigned substration followed by rounding and then by normalization.

Returns: ((x - y) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_subroundnormu_reg(x, y, n);
	__SUBROUNDNORMU_REG(x, y, n);
*/
unsigned int __builtin_pulp_subuRN_r(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization and rounding factor */
	);

/** @brief LSP short int by LSP short int into int multiplication

LSP short int by LSP short int into int multiplication

Returns (((short int) (x) * (short int) (y)))

Available macros:

	gap8_muls(x, y, n);
	__MULS(x, y, n);
*/
int __builtin_pulp_muls(
	int x,	/**< 1st Argument */
	int y	/**< 2nd Argument */
	);

/** @brief LSP short int by LSP short int into int multiplication followed by normalization (immediate value)

LSP short int by LSP short int into int multiplication followed by normalization (immediate value)

Returns ((((short int) (x) * (short int) (y))) >> n)

Available macros:

	gap8_mulsN(x, y, n);
	__MULSN(x, y, n);
*/
int __builtin_pulp_mulsN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, immediate value in [0..31] */
	);


/** @brief LSP short int by LSP short int into int multiplication followed by rounding and then normalization (immediate value)

LSP short int by LSP short int into int multiplication followed by rounding and then normalization (immediate value)

Returns ((((short int) (x) * (short int) (y)) + r; r = (1<<((n)-1)))>>(n)

Available macros:

	gap8_mulsRN(x, y, n);
	__MULSRN(x, y, n);
*/
int __builtin_pulp_mulsRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n,	/**< Normalization factor, immediate value in [0..31] */
	unsigned int r	/**< Rounding factor, must be equal to 2^(n-1) */
	);

/** @brief MSP Short int by MSP short int into int multiplication

MSP short int by MSP short int into int multiplication

Returns  (((short int) ((x)>>16) * (short int) ((y)>>16)))

Available macros:

	gap8_mulhhs(x, y, n);
	__MULHHS(x, y, n);
*/
int __builtin_pulp_mulhhs(
	int x,	/**< 1st Argument */
	int y	/**< 2nd Argument */
	);

/** @brief MSP short int by MSP short int into int multiplication followed by normalization (immediate value)

MSP short int by MSP short int into int multiplication followed by normalization (immediate value)

Returns ((((short int) (x) * (short int) (y))) >> n)

Available macros:

	gap8_mulhhsN(x, y, n);
	__MULHHSN(x, y, n);
*/
int __builtin_pulp_mulhhsN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, immediate value in [0..31] */
	);


/** @brief MSP short int by MSP short int into int multiplication followed by rounding and then normalization (immediate value)

MSP short int by MSP short int into int multiplication followed by rounding and then normalization (immediate value)

Returns ((((short int) (x) * (short int) (y)) + r); r = (1<<((n)-1)))>>(n)

Available macros:

	gap8_mulhhsRN(x, y, n);
	__MULHHSRN(x, y, n);
*/
int __builtin_pulp_mulhhsRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	unsigned int n,	/**< Normalization factor, immediate value in [0..31] */
	unsigned int r	/**< Rounding factor, must be equal to 2^(n-1) */
	);

/** @brief LSP short int by LSP short int into int unsigned multiplication

LSP short int by LSP short int into unsigned int multiplication

Returns (((unsigned short int) (x) * (unsigned short int) (y)))

Available macros:

	gap8_mulu(x, y, n);
	__MULU(x, y, n);
*/
int __builtin_pulp_mulu(
	unsigned int x,	/**< 1st Argument */
	unsigned int y	/**< 2nd Argument */
	);

/** @brief LSP short int by LSP short int into int unsigned multiplication followed by normalization (immediate value)

LSP short int by LSP short int into int unsigned multiplication followed by normalization (immediate value)

Returns ((((unsigned short int) (x) * (unsigned short int) (y))) >> n)

Available macros:

	gap8_muluN(x, y, n);
	__MULUN(x, y, n);
*/
int __builtin_pulp_muluN(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, immediate value in [0..31] */
	);


/** @brief LSP short int by LSP short int into int unsigned multiplication followed by rounding and then normalization (immediate value)

LSP short int by LSP short int into int unsigned multiplication followed by rounding and then normalization (immediate value)

Returns ((((unsigned short int) (x) * (unsigned short int) (y)) + r)>>(n)); r = 2^(n-1)

Available macros:

	gap8_muluRN(x, y, n);
	__MULSUN(x, y, n);
*/
int __builtin_pulp_muluRN(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n,	/**< Normalization factor, immediate value in [0..31]. */
	unsigned int r  /**< Rounding factor, must be equal to 2^(n-1) */
	);

/** @brief MSP Short int by MSP short int into int unsigned multiplication

MSP short int by MSP short int into int unsigned multiplication

Returns  (((unsigned short int) ((x)>>16) * (unsigned short int) ((y)>>16)))

Available macros:

	gap8_mulhhu(x, y, n);
	__MULHHU(x, y, n);
*/
int __builtin_pulp_mulhhu(
	unsigned int x,	/**< 1st Argument */
	unsigned int y	/**< 2nd Argument */
	);

/** @brief MSP short int by MSP short int into int unsigned multiplication followed by normalization (immediate value)

MSP short int by MSP short int into int unsigned multiplication followed by normalization (immediate value)

Returns ((((unsigned short int) (x) * (unsigned short int) (y))) >> n)

Available macros:

	gap8_mulhhuN(x, y, n);
	__MULHHUN(x, y, n);
*/
int __builtin_pulp_mulhhuN(
	unsigned int x,	/**< 1st Argument */
	unsigned int y,	/**< 2nd Argument */
	unsigned int n	/**< Normalization factor, immediate value in [0..31] */
	);


/** @brief MSP short int by MSP short int into int unsigned multiplication followed by rounding and then normalization (immediate value)

MSP short int by MSP short int into int unsigned multiplication followed by rounding and then normalization (immediate value)

Returns ((((unsigned short int) (x) * (unsigned short int) (y)) + r)>>(n)); r = 2^(n-1)

Available macros:

	gap8_mulhhsRN(x, y, n);
	__MULHHSRN(x, y, n);
*/
unsigned int __builtin_pulp_mulhhuRN(
	short int x,		/**< 1st Argument */
	short int y,		/**< 1st Argument */
	unsigned int n,		/**< Normalization factor, immediate value in [0..31] */
	unsigned int r		/**< Rounding factor, must be equal to 2^(n-1) */
	);


/* Triadic */

/** @brief Multiply accumulate, int operands

Multiply accumulate, int operands

Returns Acc + x*y

Available macros:

	gap8_mac(Acc, x, y);
	__MAC(Acc, x, y);
*/
int __builtin_pulp_mac(
	int x,	/**< 1st Argument */
	int y,	/**< 2nd Argument */
	int Acc	/**< Accumulator */
	);
/** @brief Multiply MSP short int by MSP short int into int and Accumulate

Multiply MSP short int by MSP short int into int and Accumulate

Returns (Acc + ((short int) (x>>16) * (short int) (y>>16)))

Available macros:

	gap8_machhs(Acc, x, y);
	__MACHHS(Acc, x, y);
*/
int __builtin_pulp_machhs(
	int x,	/**< 1st Argument */
	int y,	/**< 2nd Argument */
	int Acc	/**< Accumulator */
	);

/** @brief unsigned multiply MSP short int by MSP short int into int and Accumulate

unsigned multiply MSP short int by MSP short int into int and Accumulate

Returns (Acc + ((unsigned short int) (x>>16) * (unsigned short int) (y>>16)))

Available macros:

	gap8_machhu(Acc, x, y);
	__MACHHU(Acc, x, y);
*/
int __builtin_pulp_machhu(
	int x,	/**< 1st Argument */
	int y,	/**< 2nd Argument */
	int Acc	/**< Accumulator */
	);

/** @brief Multiply substract, int operands

Multiply substract, int operands

Returns Acc - x*y

Available macros:

	gap8_msu(Acc, x, y);
	__MSU(Acc, x, y);
*/
int __builtin_pulp_msu(
	int x,	/**< 1st Argument */
	int y,	/**< 2nd Argument */
	int Acc	/**< Accumulator */
	);


/** @brief Multiply LSP short int by LSP short int into int and Accumulate

Multiply LSP short int by LSP short int into int and Accumulate

Returns (Acc + ((short int) x * (short int) y))

Available macros:

	gap8_macs(Acc, x, y);
	__MACS(Acc, x, y);
*/
int __builtin_pulp_macs(
	int x,	/**< 1st Argument */
	int y,	/**< 2nd Argument */
	int Acc	/**< Accumulator */
	);


/** @brief Unsigned multiply LSP short int by LSP short int into int and Accumulate

Unsigned multiply LSP short int by LSP short int into int and Accumulate

Returns (Acc + ((unsigned short int) x * (unsigned short int) y))

Available macros:

	gap8_macu(Acc, x, y);
	__MACU(Acc, x, y);
*/
unsigned int __builtin_pulp_macu(
	int x,	/**< 1st Argument */
	int y,	/**< 2nd Argument */
	int Acc	/**< Accumulator */
	);

/** @brief Multiply LSP short int by LSP short int into int and Accumulate then normalization

Multiply LSP short int by LSP short int into int and Accumulate then normalization

Returns (Acc + ((short int) x * (short int) y)) >> n

Available macros:

	gap8_macsN(Acc, x, y);
	__MACSN(Acc, x, y);
*/
int __builtin_pulp_macsN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	int Acc,	/**< Accumulator */
	unsigned int n	/**< Normalization factor, immediate in [0..31] */
	);

/** @brief Unsigned multiply LSP short int by LSP short int into int and Accumulate then normalization

Unsigned multiply LSP short int by LSP short int into int and Accumulate then normalization

Returns (Acc + ((unsigned short int) x * (unsigned short int) y)) >> n

Available macros:

	gap8_macuN(Acc, x, y);
	__MACUN(Acc, x, y);
*/
unsigned int __builtin_pulp_macuN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	int Acc,	/**< Accumulator */
	unsigned int n	/**< Normalization factor, immediate in [0..31] */
	);

/** @brief Multiply LSP short int by LSP short int into int and Accumulate then rounding followed by normalization

Multiply LSP short int by LSP short int into int and Accumulate then rounding followed by normalization

Returns ((Acc + ((short int) x * (short int) y)) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_macsRN(Acc, x, y);
	__MACSRN(Acc, x, y);
*/
int __builtin_pulp_macsRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	int Acc,	/**< Accumulator */
	unsigned int n,	/**< Normalization factor, immediate in [0..31] */
	unsigned int r	/**< Rounding factor, must be equal to 2^(n-1) */
	);

/** @brief Unsigned multiply LSP short int by LSP short int into int and Accumulate then rounding followed by normalization

Unsigned multiply LSP short int by LSP short int into int and Accumulate then rounding followed by normalization

Returns ((Acc + ((unsigned short int) x * (unsigned short int) y)) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_macuRN(Acc, x, y);
	__MACURN(Acc, x, y);
*/
unsigned int __builtin_pulp_macuRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	int Acc,	/**< Accumulator */
	unsigned int n,	/**< Normalization factor, immediate in [0..31] */
	unsigned int r	/**< Rounding factor, must be equal to 2^(n-1) */
	);

/** @brief Multiply MSP short int by MSP short int into int and Accumulate then rounding followed by normalization

Multiply MSP short int by MSP short int into int and Accumulate then rounding followed by normalization

Returns ((Acc + ((short int) (x>>16) * (short int) (y>>16))) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_machhsRN(Acc, x, y);
	__MACHHSRN(Acc, x, y);
*/
int __builtin_pulp_machhsN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	int Acc,	/**< Accumulator */
	unsigned int n	/**< Normalization factor, immediate in [0..31] */
	);

/** @brief Unsigned multiply MSP short int by MSP short int into int and Accumulate then normalization

Unsigned multiply MSP short int by MSP short int into int and Accumulate then normalization

Returns (Acc + ((unsigned short int) (x>>16) * (unsigned short int) (y>>16))) >> n

Available macros:

	gap8_machhuN(Acc, x, y);
	__MACHHUN(Acc, x, y);
*/
unsigned int __builtin_pulp_machhuN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	int Acc,	/**< Accumulator */
	unsigned int n	/**< Normalization factor, immediate in [0..31] */
	);

/** @brief Multiply MSP short int by MSP short int into int and Accumulate then rounding followed by normalization

Multiply MSP short int by MSP short int into int and Accumulate then rounding followed by normalization

Returns ((Acc + ((short int) (x>>16) * (short int) (y>>16))) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_machhsRN(Acc, x, y);
	__MACSHHRN(Acc, x, y);
*/
int __builtin_pulp_machhsRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	int Acc,	/**< Accumulator */
	unsigned int n,	/**< Normalization factor, immediate in [0..31] */
	unsigned int r	/**< Rounding factor, must be equal to 2^(n-1) */
	);

/** @brief Unsigned multiply MSP short int by MSP short int into int and Accumulate then rounding followed by normalization

Unsigned multiply MSP short int by MSP short int into int and Accumulate then rounding followed by normalization

Returns ((Acc + ((unsigned short int) (x>>16) * (unsigned short int) (y>>16))) + r) >> n; r = 2^(n-1)

Available macros:

	gap8_machhuRN(Acc, x, y);
	__MACHHURN(Acc, x, y);
*/
unsigned int __builtin_pulp_machhuRN(
	int x,		/**< 1st Argument */
	int y,		/**< 2nd Argument */
	int Acc,	/**< Accumulator */
	unsigned int n,	/**< Normalization factor, immediate in [0..31] */
	unsigned int r	/**< Rounding factor, must be equal to 2^(n-1) */
	);

/* Clipping */

/** @brief Clips input to [lb=-2^Precision .. ub=2^Precision-1], lb, ub immediates.

Clips input to [lb=-2^precision .. ub=2^precision-1], lb, ub immediates.

Returns: ((x)<(-(1<<(precision)))?(-(1<<(precision))):(((x)>((1<<(precision))-1))?((1<<(precision))-1):(x)))

Available macros:

	gap8_clip(x, precision);
	__CLIP(x, precision);
*/
int __builtin_pulp_clip(
	int x,	/**< Argument */
	int lb,	/**< Lower bound, Immediate: -2^precision     */
	int ub	/**< Upper bound, Immediate:  2^precision - 1 */
	);

/** @brief Clips input x to [-bound-1 .. bound]

Clips input x to [-bound-1 .. bound]

Returns (((x)<=-((bound)+1))?(-((bound)+1)):(((x)>=(bound))?(bound):(x)))

Available macros:

	gap8_clip_r(x, precision);
	__CLIP_R(x, precision);
*/
int __builtin_pulp_clip_r(
	int x,		/**< Argument */
	int bound	/**< Clipping bound: [-bound-1 .. bound] */
	);

/** @brief Clips input to [lb=0 .. ub=2^Precision-1], lb, ub immediates.

Clips input to [lb=0 .. ub=2^precision-1], lb, ub immediates.

Returns: ((x)<0)?0:(((x)>((1<<(precision))-1))?((1<<(precision))-1):(x))

Available macros:

	gap8_clipu(x, precision);
	__CLIPU(x, precision);
*/
unsigned int __builtin_pulp_clipu(
	int x,	/**< Argument */
	int lb,	/**< Lower bound, must be equal to 0 */
	int ub	/**< Upper bound, Immediate:  2^precision - 1 */
	);

/** @brief Clips input x to [0 .. bound]

Clips input x to [0 .. bound]

Returns (((x)<=0)?(0):(((x)>=(bound))?(bound):(x)))

Available macros:

	gap8_clipu_r(x, precision);
	__CLIPU_R(x, precision);
*/
unsigned int __builtin_pulp_clipu_r(
	int x,		/**< Argument */
	int bound	/**< Clipping bound: [0 .. bound] */
	);
/** @} */ // End of Arith

/** @defgroup Simd2and4 SIMD2_4_Arithmetic
@{ */

/* Simd 2 and 4 */

/* Simd pack */
/** @brief Pack 2 short int into a vector

Pack 2 short int into a vector

Returns ((v2s) {(signed short)   (x), (signed short)   (y)}). gcc vector notation.

Available macros:

	gap8_pack2(x, y);
	__PACK2(x, y);
*/
v2s __builtin_pulp_pack2(
	short int x,	/**< Argument going to vect[0] */
	short int y	/**< Argument going to vect[1] */
	);

/** @brief Pack 4 bytes into a vector

Pack 4 bytes into a vector

Returns ((v4s) {(signed char)   (x), (signed char)   (y), (signed char)   (z), (signed char)   (t)}). gcc vector notation.

Available macros:

	gap8_pack4(x, y, z, t);
	__PACK4(x, y, z, t);
*/
v4s __builtin_pulp_pack4(
	char x,	/**< Argument going to Vect[0] */
	char y,	/**< Argument going to Vect[1] */
	char z,	/**< Argument going to Vect[2] */
	char t	/**< Argument going to Vect[3] */
	);

/* Simd shuffle */
/** @brief Permutes vector elements of x according to positions given in mask (a vector).

Permutes vector elements of x according to positions given in mask (a vector).

Constructs a permutation of elements from input x vector and return a vector of the same type as the input vector.
The mask is an integral vector with the same width (W) and element count (N) as the output vector.

The elements of the input vector are numbered in memory ordering beginning at 0. The elements of mask are considered modulo N.

Available macros:

	__builtin_shuffle(x, mask)
*/
v2s __builtin_pulp_shuffleh(
	v2s x,		/**< A vector of 2 short ints */
	v2s mask	/**<  A vector of 2 shorts, member i, in [0..1], states that x[mask[i]] should be the ith member in the ouput vector */
	);

/** @brief Permutes vector elements of x according to positions given in mask (a vector).

Permutes vector elements of x according to positions given in mask (a vector).

Constructs a permutation of elements from input x vector and return a vector of the same type as the input vector.
The mask is an integral vector with the same width (W) and element count (N) as the output vector.

The elements of the input vector are numbered in memory ordering beginning at 0. The elements of mask are considered modulo N.

Available macros:

	__builtin_shuffle(x, mask)
*/
v4s __builtin_pulp_shuffleb(
	v4s x,		/**< A vector of 4 bytes */
	v4s mask	/**< A vector of 4 bytes, member i, in [0..3], states that x[mask[i]] should be the ith member in the ouput vector */
	);

/** @brief Picks 2 elements from vectors (x, y) according to positions given in mask (a vector).

Construct a permutation of elements from two vectors and return a vector of the same type as the input vector(s). The mask is an integral vector with the same width (W) and element count (N) as the output vector.

The elements of the input vectors are numbered in memory ordering of vec0 beginning at 0 and vec1 beginning at N. The elements of mask are considered modulo 2*N.

Available macros:

	__builtin_shuffle(x, y, mask)
*/
v2s __builtin_pulp_shuffle2h(
	v2s x,		/**< A vector of 2 short ints */
	v2s y,		/**< A vector of 2 short ints */
	v2s mask	/**< A vector of 2 shorts, member i, in [0..3], states that x[mask[i]] (if i<=1) or y[mask[i]] (if i>1) should be the ith member in the ouput vector */
	);

/** @brief Picks 4 elements from vectors (x, y) according to positions given in mask (a vector).

Construct a permutation of elements from two vectors and return a vector of the same type as the input vector(s). The mask is an integral vector with the same width (W) and element count (N) as the output vector.

The elements of the input vectors are numbered in memory ordering of vec0 beginning at 0 and vec1 beginning at N. The elements of mask are considered modulo 2*N.

Available macros:

	__builtin_shuffle(x, y, mask)
*/
v4s __builtin_pulp_shuffle4b(
	v4s x,		/**< A vector of 4 bytes */
	v4s y,		/**< A vector of 4 bytes */
	v4s mask	/**< A vector of 4 bytes, member i, in [0..7], states that x[mask[i]] (if i<=3) or y[mask[i]] (if i>3) should be the ith member in the ouput vector*/
	);

/* Simd 2/4 Monadic */

/** @brief Element wise abs on a vector of 2 shorts.

Element wise abs on a vector of 2 shorts.

Returns (v2u) {((x)[0]<0)?-(x)[0]:(x)[0], ((x)[1]<0)?-(x)[1]:(x)[1]}

Available macros:

        gap8_abs2(x)
        __ABS2(x)
*/
v2u	 __builtin_pulp_abs2(
	v2s x	/**< A vector of 2 shorts */
	);

/** @brief Element wise abs on a vector of 4 bytes.

Element wise abs on a vector of 4 bytes.

Returns (v4u) {((x)[0]<0)?-(x)[0]:(x)[0], ((x)[1]<0)?-(x)[1]:(x)[1], ((x)[2]<0)?-(x)[2]:(x)[2], ((x)[3]<0)?-(x)[3]:(x)[3]}

Available macros:

        gap8_abs4(x)
        __ABS4(x)
*/
v4u	 __builtin_pulp_abs4(
	v4s x	/**< A vector of 4 bytes */
	);

/** @brief Element wise unary minus on a vector of 2 shorts.

Element wise unary minus on a vector of 2 shorts.

Returns (v2s) {-(x)[0], -(x)[1]}

Available macros:

        gap8_neg2(x)
        __NEG2(x)
*/
v2u	 __builtin_pulp_neg2(
	v2s x	/**< A vector of 2 shorts */
	);

/** @brief Element wise unary minus on a vector of 4 bytes.

Element wise unary minus on a vector of 4 bytes.

Returns (v4s) {-(x)[0], -(x)[1], -(x)[2], -(x)[3])}

Available macros:

        gap8_neg4(x)
        __NEG4(x)
*/
v4u	 __builtin_pulp_neg4(
	v4s x	/**< A vector of 4 bytes */
	);

/* Simd 2/4 Diadic */

/** @brief Element wise addition on a vector of 2 shorts.

Element wise addition on a vector of shorts.

Returns (v2s) {(x)[0]+(y)[0], (x)[1]+(y)[1]}

Available macros:

        gap8_add2(x, y)
        __ADD2(x, y)
*/
v2s __builtin_pulp_add2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise addition on a vector of 4 bytes.

Element wise addition on a vector of 4 bytes.

Returns (v4s) {(x)[0]+(y)[0], (x)[1]+(y)[1], (x)[2]+(y)[2], (x)[3]+(y)[3]}

Available macros:

        gap8_add4(x, y)
        __ADD4(x, y)
*/
v4s __builtin_pulp_add4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise substraction on a vector of 2 shorts.

Element wise substraction on a vector of shorts.

Returns (v2s) {(x)[0]-(y)[0], (x)[1]-(y)[1]}

Available macros:

        gap8_sub2(x, y)
        __SUB2(x, y)
*/
v2s __builtin_pulp_sub2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise substraction on a vector of 4 bytes.

Element wise substraction on a vector of 4 bytes.

Returns (v4s) {(x)[0]-(y)[0], (x)[1]-(y)[1], (x)[2]-(y)[2], (x)[3]-(y)[3]}

Available macros:

        gap8_sub4(x, y)
        __SUB4(x, y)
*/
v4s __builtin_pulp_sub4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise average on a vector of 2 shorts.

Element wise average on a vector of 2 shorts.

Returns (v2s) {((x)[0]+(y)[0]))>>1, ((x)[1]+(y)[1])>>1}; Addition is performed on 16 bits.

Available macros:

        gap8_avg2(x, y)
        __AVG2(x, y)
*/
v2s __builtin_pulp_avg2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise average on a vector of 4 bytes.

Element wise average on a vector of 4 bytes.

Returns (v2s)  ((v4s) {((x)[0]+(y)[0])>>1, ((x)[1]+(y)[1])>>1, ((x)[2]+(y)[2])>>1, ((x)[3]+(y)[3])>>1}. Addition is performed on 8 bits.

Available macros:

        gap8_avg4(x, y)
        __AVG4(x, y)
*/
v4s __builtin_pulp_avg4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise average on a vector of 2 shorts.

Element wise average on a vector of 2 shorts.

Returns (v2u) {((unsigned short)(x)[0]+(unsigned short)(y)[0])>>1, ((unsigned short)(x)[1]+(unsigned short)(y)[1])>>1}; Addition is performed on 16 bits.

Available macros:

        gap8_avgu2(x, y)
        __AVGU2(x, y)
*/
v2u __builtin_pulp_avgu2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise unsigned average on a vector of 4 bytes.

Element wise unsigned average on a vector of 4 bytes.

Returns
        (v4u) {((unsigned char)(x)[0]+(unsigned char)(y)[0])>>1, ((unsigned char)(x)[1]+(unsigned char)(y)[1])>>1,
               ((unsigned char)(x)[2]+(unsigned char)(y)[2])>>1, ((unsigned char)(x)[3]+(unsigned char)(y)[3])>>1}; Addition is perfomed on 8 bits.

Available macros:

        gap8_avgu4(x, y)
        __AVGU4(x, y)
*/
v4u __builtin_pulp_avgu4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise min on a vector of 2 shorts.

Element wise min on a vector of 2 shorts.

Returns
        (v2s) {((signed short)(x)[0]<(signed short)(y)[0])?((signed short)(x)[0]):((signed short)(y)[0]),
               ((signed short)(x)[1]<(signed short)(y)[1])?((signed short)(x)[1]):((signed short)(y)[1])}

Available macros:

        gap8_min2(x, y)
        __MIN2(x, y)
*/
v2s __builtin_pulp_min2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise min on a vector of 4 bytes.

Element wise min on a vector of 4 bytes.

Returns
        (v4s) {((signed char)(x)[0]<(signed char)(y)[0])?(signed char)(x)[0]:(signed char)(y)[0],
               ((signed char)(x)[1]<(signed char)(y)[1])?(signed char)(x)[1]:(signed char)(y)[1],
               ((signed char)(x)[2]<(signed char)(y)[2])?(signed char)(x)[2]:(signed char)(y)[2],
               ((signed char)(x)[3]<(signed char)(y)[3])?(signed char)(x)[3]:(signed char)(y)[3]}

Available macros:

        gap8_min4(x, y)
        __MIN4(x, y)
*/
v4s __builtin_pulp_min4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise unsigned min on a vector of 2 shorts.

Element wise unsigned min on a vector of 2 shorts.

Returns
        (v2u) {((unsigned short)(x)[0]<(unsigned short)(y)[0])?(unsigned short)(x)[0]:(unsigned short)(y)[0],
               ((unsigned short)(x)[1]<(unsigned short)(y)[1])?(unsigned short)(x)[1]:(unsigned short)(y)[1]}

Available macros:

        gap8_minu2(x, y)
        __MINU2(x, y)
*/
v2u __builtin_pulp_minu2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise unsigned min on a vector of 4 bytes.

Element wise unsigned min on a vector of 4 bytes.

Returns
        (v4u) {((unsigned char)(x)[0]<(unsigned char)(y)[0])?(unsigned char)(x)[0]:(unsigned char)(y)[0],
               ((unsigned char)(x)[1]<(unsigned char)(y)[1])?(unsigned char)(x)[1]:(unsigned char)(y)[1],
               ((unsigned char)(x)[2]<(unsigned char)(y)[2])?(unsigned char)(x)[2]:(unsigned char)(y)[2],
               ((unsigned char)(x)[3]<(unsigned char)(y)[3])?(unsigned char)(x)[3]:(unsigned char)(y)[3]}

Available macros:

        gap8_minu4(x, y)
        __MINU4(x, y)
*/
v4u __builtin_pulp_minu4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise max on a vector of 2 shorts.

Element wise max on a vector of 2 shorts.

Returns
        (v2s) {((signed short)(x)[0]>(signed short)(y)[0])?((signed short)(x)[0]):((signed short)(y)[0]),
               ((signed short)(x)[1]>(signed short)(y)[1])?((signed short)(x)[1]):((signed short)(y)[1])}

Available macros:

        gap8_max2(x, y)
        __MAX2(x, y)
*/
v2s __builtin_pulp_max2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise max on a vector of 4 bytes.

Element wise max on a vector of 4 bytes.

Returns
        (v4s) {((signed char)(x)[0]>(signed char)(y)[0])?(signed char)(x)[0]:(signed char)(y)[0],
               ((signed char)(x)[1]>(signed char)(y)[1])?(signed char)(x)[1]:(signed char)(y)[1],
               ((signed char)(x)[2]>(signed char)(y)[2])?(signed char)(x)[2]:(signed char)(y)[2],
               ((signed char)(x)[3]>(signed char)(y)[3])?(signed char)(x)[3]:(signed char)(y)[3]}

Available macros:

        gap8_max4(x, y)
        __MAX4(x, y)
*/
v4s __builtin_pulp_max4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise unsigned max on a vector of 2 shorts.

Element wise unsigned max on a vector of 2 shorts.

Returns
        (v2u) {((unsigned short)(x)[0]>(unsigned short)(y)[0])?(unsigned short)(x)[0]:(unsigned short)(y)[0],
               ((unsigned short)(x)[1]>(unsigned short)(y)[1])?(unsigned short)(x)[1]:(unsigned short)(y)[1]}

Available macros:

        gap8_maxu2(x, y)
        __MAXU2(x, y)
*/
v2u __builtin_pulp_maxu2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise unsigned max on a vector of 4 bytes.

Element wise unsigned max on a vector of 4 bytes.

Returns
        (v4u) {((unsigned char)(x)[0]>(unsigned char)(y)[0])?(unsigned char)(x)[0]:(unsigned char)(y)[0],
               ((unsigned char)(x)[1]>(unsigned char)(y)[1])?(unsigned char)(x)[1]:(unsigned char)(y)[1],
               ((unsigned char)(x)[2]>(unsigned char)(y)[2])?(unsigned char)(x)[2]:(unsigned char)(y)[2],
               ((unsigned char)(x)[3]>(unsigned char)(y)[3])?(unsigned char)(x)[3]:(unsigned char)(y)[3]}

Available macros:

        gap8_maxu4(x, y)
        __MAXU4(x, y)
*/
v4u __builtin_pulp_maxu4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise bitwise and on a vector of 2 shorts.

Element wise bitwise and on a vector of 2 shorts.

Returns (v2s) {(x)[0]&(y)[0], (x)[1]&(y)[1]}

Available macros:

        gap8_and2(x, y)
        __AND2(x, y)
*/
v2s __builtin_pulp_and2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise bitwise and on a vector of 4 bytes.

Element wise bitwise and on a vector of 4 bytes.

Returns (v4s) {(x)[0]&(y)[0], (x)[1]&(y)[1], (x)[2]&(y)[2], (x)[3]&(y)[3]}

Available macros:

        gap8_and4(x, y)
        __AND4(x, y)
*/
v4s __builtin_pulp_and4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise bitwise or on a vector of 2 shorts.

Element wise bitwise or on a vector of 2 shorts.

Returns (v2s) {(x)[0]|(y)[0], (x)[1]|(y)[1]}

Available macros:

        gap8_or2(x, y)
        __OR2(x, y)
*/
v2s __builtin_pulp_or2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise bitwise or on a vector of 4 bytes.

Element wise bitwise or on a vector of 4 bytes.

Returns (v4s) {(x)[0]|(y)[0], (x)[1]|(y)[1], (x)[2]|(y)[2], (x)[3]|(y)[3]}

Available macros:

        gap8_or4(x, y)
        __OR4(x, y)
*/
v4s __builtin_pulp_or4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise bitwise exor on a vector of 2 shorts.

Element wise bitwise exor on a vector of 2 shorts.

Returns (v2s) {(x)[0]^(y)[0], (x)[1]^(y)[1]}

Available macros:

        gap8_exor2(x, y)
        __EXOR2(x, y)
*/
v2s __builtin_pulp_exor2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise bitwise exor on a vector of 4 bytes.

Element wise bitwise exor on a vector of 4 bytes.

Returns (v4s) {(x)[0]^(y)[0], (x)[1]^(y)[1], (x)[2]^(y)[2], (x)[3]^(y)[3]}

Available macros:

        gap8_exor4(x, y)
        __EXOR4(x, y)
*/
v4s __builtin_pulp_exor4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise logical shift right on a vector of 2 shorts.

Element wise logical shift right on a vector of 2 shorts.

Returns (v2u) {((unsigned short)(x)[0]>>(unsigned short)(y)[0]), ((unsigned short)(x)[1]>>(unsigned short)(y)[1])}

Available macros:

        gap8_srl2(x, y)
        __SRL2(x, y)
*/
v2u __builtin_pulp_srl2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise logical shift right on a vector of 4 bytes.

Element wise logical shift right on a vector of 4 bytes.

Returns (v4u) {((unsigned char)(x)[0]>>(unsigned char)(y)[0]), ((unsigned char)(x)[1]>>(unsigned char)(y)[1]),
               ((unsigned char)(x)[2]>>(unsigned char)(y)[2]), ((unsigned char)(x)[3]>>(unsigned char)(y)[3])}

Available macros:

        gap8_srl4(x, y)
        __SRL4(x, y)
*/
v4u __builtin_pulp_srl4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise arithmetic shift right on a vector of 2 shorts.

Element wise arithmetic shift right on a vector of 2 shorts.

Returns (v2s) {((signed short)(x)[0]>>(signed short)(y)[0]), ((signed short)(x)[1]>>(signed short)(y)[1])}

Available macros:

        gap8_sra2(x, y)
        __SRA2(x, y)
*/
v2s __builtin_pulp_sra2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise arithmetic shift right on a vector of 4 bytes.

Element wise arithmetic shift right on a vector of 4 bytes.

Returns (v4s) {((signed char)(x)[0]>>(signed char)(y)[0]), ((signed char)(x)[1]>>(signed char)(y)[1]),
               ((signed char)(x)[2]>>(signed char)(y)[2]), ((signed char)(x)[3]>>(signed char)(y)[3])}

Available macros:

        gap8_sra4(x, y)
        __SRA4(x, y)
*/
v4s __builtin_pulp_sra4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise logical shift left on a vector of 2 shorts.

Element wise logical shift left on a vector of 2 shorts.

Returns (v2s) {(x)[0]<<(y)[0], (x)[1]<<(y)[1]}

Available macros:

        gap8_sll2(x, y)
        __SLL2(x, y)
*/
v2s __builtin_pulp_sll2(
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise logical shift left on a vector of 4 bytes.

Element wise logical shift left on a vector of 4 bytes.

Returns (v4s) {(x)[0]<<(y)[0], (x)[1]<<(y)[1], (x)[2]<<(y)[2], (x)[3]<<(y)[3]}}

Available macros:

        gap8_sll4(x, y)
        __SLL4(x, y)
*/
v4s __builtin_pulp_sll4(
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/* Special simd add, sub */

/** @brief Element wise addition on a vector of 2 shorts followed by a division by 2.

Element wise addition on a vector of 2 shorts followed by a division by 2.

Returns ((x)+(y))>>(v2s) {1, 1}

Available macros:

        gap8_add2div2(x, y)
        __ADD2DIV2(x, y)
*/
v2s __builtin_pulp_add2div2 (
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise addition on a vector of 2 shorts followed by a division by 4.

Element wise addition on a vector of 2 shorts followed by a division by 4.

Returns ((x)+(y))>>(v2s) {2, 2}

Available macros:

        gap8_add2div4(x, y)
        __ADD2DIV4(x, y)
*/
v2s __builtin_pulp_add2div4 (
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise substraction on a vector of 2 shorts followed by a division by 2.

Element wise substraction on a vector of 2 shorts followed by a division by 2.

Returns ((x)-(y))>>(v2s) {1, 1}

Available macros:

        gap8_sub2div2(x, y)
        __SUB2DIV2(x, y)
*/
v2s __builtin_pulp_sub2div2 (
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise substraction on a vector of 2 shorts followed by a division by 4.

Element wise substraction on a vector of 2 shorts followed by a division by 4.

Returns ((x)-(y))>>(v2s) {2, 2}

Available macros:

        gap8_sub2div4(x, y)
        __SUB2DIV4(x, y)
*/
v2s __builtin_pulp_sub2div4 (
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Element wise addition on a vector of 4 bytes followed by a division by 2.

Element wise addition on a vector of 4 bytes followed by a division by 2.

Returns ((x)+(y))>>(v2s) {1, 1}

Available macros:

        gap8_add4div2(x, y)
        __ADD4DIV2(x, y)
*/
v4s __builtin_pulp_add4div2 (
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise addition on a vector of 4 bytes followed by a division by 4.

Element wise addition on a vector of 4 bytes followed by a division by 4.

Returns ((x)+(y))>>(v2s) {2, 2}

Available macros:

        gap8_add4div4(x, y)
        __ADD4DIV4(x, y)
*/
v4s __builtin_pulp_add4div4 (
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise substraction on a vector of 4 bytes followed by a division by 2.

Element wise substraction on a vector of 4 bytes followed by a division by 2.

Returns ((x)-(y))>>(v2s) {1, 1}

Available macros:

        gap8_sub4div2(x, y)
        __SUB4DIV2(x, y)
*/
v4s __builtin_pulp_sub4div2 (
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Element wise substraction on a vector of 4 bytes followed by a division by 4.

Element wise substraction on a vector of 4 bytes followed by a division by 4.

Returns ((x)-(y))>>(v2s) {2, 2}

Available macros:

        gap8_sub4div4(x, y)
        __SUB4DIV4(x, y)
*/
v4s __builtin_pulp_sub4div4 (
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @} */ // End of Simd2and4Arith


/** @defgroup Complex ComplexOperations
@{ */

/* Complex */

/** @brief Complex Multiplication, Q15x15 into Q15.

Complex Multiplication, Q15x15 into Q15.

Returns:
        (v2s) {(signed short) ((((int) (x)[0]*(int) (y)[0]) - ((int) (x)[1]*(int) (y)[1]))>>15),
               (signed short) ((((int) (x)[0]*(int) (y)[1]) + ((int) (x)[1]*(int) (y)[0]))>>15)}

Available macros:

        gap8_cplxmuls(x, x)
        __CPLXMULS(x, x)
*/
v2s __builtin_pulp_cplxmuls (
	v2s x,	/**< A complex vector, <I,Q>*/
	v2s y	/**< A complex vector, <I,Q>*/
	);

/** @brief Complex Multiplication, Q15x15 into Q15 followed by a division by 2.

Complex Multiplication, Q15x15 into Q15 followed by a division by 2.

Returns:
        (v2s) {((signed short) ((((int) (x)[0]*(int) (y)[0]) - ((int) (x)[1]*(int) (y)[1]))>>15))>>1,
               ((signed short) ((((int) (x)[0]*(int) (y)[1]) + ((int) (x)[1]*(int) (y)[0]))>>15))>>1}

Available macros:

        gap8_cplxmulsdiv2(x, x)
        __CPLXMULSDIV2(x, x)
*/
v2s __builtin_pulp_cplxmulsdiv2(
	v2s x,	/**< A complex vector, <I,Q>*/
	v2s y	/**< A complex vector, <I,Q>*/
	);

/** @brief Complex Multiplication, Q15x15 into Q15 followed by a division by 4.

Complex Multiplication, Q15x15 into Q15 followed by a division by 4.

Returns:
        (v2s) {((signed short) ((((int) (x)[0]*(int) (y)[0]) - ((int) (x)[1]*(int) (y)[1]))>>15))>>2,
               ((signed short) ((((int) (x)[0]*(int) (y)[1]) + ((int) (x)[1]*(int) (y)[0]))>>15))>>2}

Available macros:

        gap8_cplxmulsdiv4(x, x)
        __CPLXMULSDIV4(x, x)
*/
v2s __builtin_pulp_cplxmulsdiv4(
	v2s x,	/**< A complex vector, <I,Q>*/
	v2s y	/**< A complex vector, <I,Q>*/
	);

/** @brief Complex substration, result rotated by -pi/2.

Complex substration, result rotated by -pi/2.

Returns: (v2s) {(x)[1]-(y)[1], (y)[0]-(x)[0]}

Available macros:

        gap8_sub2rotmj(x, x)
        __SUB2ROTMJ(x, x)
*/
v2s __builtin_pulp_sub2rotmj (
	v2s x,	/**< A complex vector, <I,Q>*/
	v2s y	/**< A complex vector, <I,Q>*/
	);
/** @brief Complex conjugate.

Complex conjugate.

Returns: (v2s) {(x)[0], -(x)[1]}

Available macros:

        gap8_cplxconf(x, x)
        __CPLXCONJ(x, x)
*/
v2s __builtin_pulp_cplx_conj (
	v2s x	/**< A complex vector, <I,Q>*/
	);

/** @} */ // End of Complex


/** @defgroup Viterbi ViterbiSupport
@{ */

/* Viterbi */

/** @brief Viterbi maximum. Compare 2 pathes and update special flags accordingly.

Viterbi maximum. Compare 2 pathes and update special flags accordingly.

Returns:
        (_VitT1_Flag=((x)[1]<=(y)[1])?1:0, _VitT0_Flag=((x)[0]<=(y)[0])?1:0, (v2s) {((x)[0]>(y)[0])?(x)[0]:(y)[0], ((x)[1]>(y)[1])?(x)[1]:(y)[1]})

Available macros:

        gap8_vitmax(x, y)
        __VITMAX(x, y)

*/
v2s __builtin_pulp_vitmax2 (
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);

/** @brief Viterbi select. Using viterbi special flags compute the surviving pathes.

Viterbi select. Using viterbi special flags compute the surviving pathes.

Returns: (v2s) {(_VitT0_Flag?(y)[0]:(x)[0])<<1|_VitT0_Flag, (_VitT1_Flag?(y)[1]:(x)[1])<<1|_VitT1_Flag}

Available macros:

        gap8_vitsel(x, y)
        __VITSEL(x, y)
*/
v2s __builtin_pulp_vitsel2 (
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);
/** @} */ // End of Viterbi


/** @defgroup DotP DotProducts
@{ */

/* Dot Products */

/** @brief Dot product between 2 vectors of 2 shorts.

Dot product between 2 vectors of 2 shorts.

Returns: ((short)(x)[0]*(short)(y)[0] + (short)(x)[1]*(short)(y)[1])

Available macros:

        gap8_dotp2(x, y)
        __DOTP2(x, y)
*/
int __builtin_pulp_dotsp2 (
	v2s x,	/**< A vector of 2 shorts */
	v2s y	/**< A vector of 2 shorts */
	);


/** @brief Dot product between 1 vectors of 2 shorts and a replicated short scalar.

Dot product between 2 vectors of 2 shorts and a replicated short scalar.

Returns: ((short)(x)[0]*(short)(y) + (short)(x)[1]*(short)(y))

Available macros:

        gap8_dotpsc2(x, y)
        __DOTPSC2(x, y)
*/
int __builtin_pulp_dotspsc2(
	v2s x,		/**< A vector of 2 shorts */
	short int y	/**< A short scalar, will be replicated 2 times */
	);

/** @brief Dot product between 2 vectors of 4 bytes.

Dot product between 2 vectors of 4 bytes.

Returns: ((char)(x)[0]*(char)(y)[0] + (char)(x)[1]*(char)(y)[1] + (char)(x)[2]*(char)(y)[2] + (char)(x)[3]*(char)(y)[3])

Available macros:

        gap8_dotp4(x, y)
        __DOTP4(x, y)
*/
int __builtin_pulp_dotsp4 (
	v4s x,	/**< A vector of 4 bytes */
	v4s y	/**< A vector of 4 bytes */
	);

/** @brief Dot product  between a vector of 4 bytes and a replicated byte scalar.

Dot product  between a vector of 4 bytes and a replicated byte scalar.

Returns: ((char)(x)[0]*(char)(y) + (char)(x)[1]*(char)(y) + (char)(x)[2]*(char)(y) + (char)(x)[3]*(char)(y))

Available macros:

        gap8_dotpsc4(x, y)
        __DOTPSC4(x, y)
*/
int __builtin_pulp_dotspsc4(
	v4s x,		/**< a Vector of 4 bytes */
	signed char y	/**< A byte scalar, will be replicated 4 times */
	);

/** @brief Dot product between 2 vectors of 2 unsigned shorts.

Dot product between 2 vectors of 2 unsigned shorts.

Returns: ((unsigned short)(x)[0]*(unsigned short)(y)[0] + (unsigned short)(x)[1]*(unsigned short)(y)[1])

Available macros:

        gap8_dotup2(x, y)
        __DOTPU2(x, y)
*/
unsigned int __builtin_pulp_dotup2 (
	v2u x,	/**< A vector of 2 unsigned shorts */
	v2u y	/**< A vector of 2 unsigned shorts */
	);

/** @brief Dot product between 1 vector of 2 unsigned shorts and a replicated unsigned short scalar.

Dot product between 1 vector of 2 unsigned shorts and a replicated unsigned short scalar.

Returns: ((unsigned short)(x)[0]*(unsigned short)(y) + (unsigned short)(x)[1]*(unsigned short)(y))

Available macros:

        gap8_dotpusc2(x, y)
        __DOTPUSC2(x, y)
*/
unsigned int __builtin_pulp_dotupsc2(
	v2u x,			/**< A vector of 2 unsigned shorts */
	unsigned short int y	/**< An unsigned short scalar, will be replicated 2 times */
	);

/** @brief Dot product between 2 vectors of 4 unsigned bytes.

Dot product between 2 vectors of 4 unsigned bytes.

Returns:
        ((unsigned char)(x)[0]*(unsigned char)(y)[0] + (unsigned char)(x)[1]*(unsigned char)(y)[1] +
         (unsigned char)(x)[2]*(unsigned char)(y)[2] + (unsigned char)(x)[3]*(unsigned char)(y)[3])

Available macros:

        gap8_dotpu4(x, y)
        __DOTPU4(x, y)
*/
unsigned int __builtin_pulp_dotup4 (
	v4u x,	/**< A vector of 4 unsigned bytes */
	v4u y	/**< An unsigned byte scalar, will be replicated 4 times */
	);

/** @brief Dot product between 1 vector of 4 unsigned bytes and an unsigned scalar byte replicated 4 times.

Dot product between 1 vector of 4 unsigned bytes and an unsigned scalar byte replicated 4 times.

Returns:
        ((unsigned char)(x)[0]*(unsigned char)(y) + (unsigned char)(x)[1]*(unsigned char)(y) +
         (unsigned char)(x)[2]*(unsigned char)(y) + (unsigned char)(x)[3]*(unsigned char)(y))

Available macros:

        gap8_dotpusc4(x, y)
        __DOTPUSC4(x, y)
*/
unsigned int __builtin_pulp_dotupsc4(
	v4u x,		/**< A vector of 4 unsigned bytes */
	unsigned char y	/**< An unsigned byte scalar, will be replicated 4 times */
	);

/** @brief Dot product between a vector of 2 unsigned shorts and a vector of 2 signed shorts.

Dot product between a vector of 2 unsigned shorts and a vector of 2 signed shorts.

Returns: ((unsigned short)(x)[0]*(short)(y)[0] + (unsigned short)(x)[1]*(short)(y)[1])

Available macros:

        gap8_dotpus2(x, y)
        __DOTPUS2(x, y)
*/
int __builtin_pulp_dotusp2 (
	v2u x,	/**< A vector of 2 unsigned shorts */
	v2s y	/**< A vector of 2 signed shorts */
	);

/** @brief Dot product between a vector of 2 unsigned shorts and a signed short scalar replicated 2 times.

Dot product between a vector of 2 unsigned shorts and a signed short scalar replicated 2 times.

Returns: ((unsigned short)(x)[0]*(short)(y) + (unsigned short)(x)[1]*(short)(y))

Available macros:

        gap8_dotpussc2(x, y)
        __DOTPUSSC2(x, y)
*/
int __builtin_pulp_dotuspsc2(
	v2u x,		/**< A vector of 2 unsigned shorts */
	short int y	/**< A short scalar, will be replicated 2 times */
	);

/** @brief Dot product between 1 vector of 4 unsigned bytes and 1 vector of 4 signed bytes.

Dot product between 1 vector of 4 unsigned bytes and 1 vector of 4 signed bytes.

Returns:
        ((unsigned char)(x)[0]*(char)(y)[0] + (unsigned char)(x)[1]*(char)(y)[1] +
         (unsigned char)(x)[2]*(char)(y)[2] + (unsigned char)(x)[3]*(char)(y)[3])

Available macros:

        gap8_dotpus4(x, y)
        __DOTPUS4(x, y)
*/
int __builtin_pulp_dotusp4 (
	v4u x,	/**< A vector of 4 unsigned bytes */
	v4s y	/**< A vector of 4 signed bytes */
	);

/** @brief Dot product between 1 vector of 4 unsigned bytes and one scalar unsigned byte replicated 4 times.

Dot product between 1 vector of 4 unsigned bytes and one scalar unsigned byte replicated 4 times.

Returns:
        ((unsigned char)(x)[0]*(char)(y) + (unsigned char)(x)[1]*(char)(y) +
         (unsigned char)(x)[2]*(char)(y) + (unsigned char)(x)[3]*(char)(y))

Available macros:

        gap8_dotpussc4(x, y)
        __DOTPUSSC4(x, y)
*/
int __builtin_pulp_dotuspsc4(
	v4u x,		/**< A vector of 4 unsigned bytes */
	signed char y	/**< A signed byte scalar, will be replicated 4 times */
	);

/* Simd 2/4 Triadic */

/** @brief Dot products between 2 vectors of 2 shorts, accumulates result.

Dot products between 2 vectors of 2 shorts, accumulates result.

Returns: z + ((x)[0]*(y)[0] + (x)[1]*(y)[1])

Available macros:

        gap8_sumdotp2(x, y, z)
        __SUMDOTP2(x, y, z)
*/
int __builtin_pulp_sdotsp2 (
	v2s x,	/**< A vector of 2 shorts */
	v2s y,	/**< A vector of 2 shorts */
	int z	/**< The accumulator */
	);

/** @brief Dot products between 1 vectors of 2 shorts and a replicated short scalar, accumulates result.

Dot products between 2 vectors of 2 shorts and a replicated short scalar, accumulates result.

Returns: z + ((x)[0]*(y) + (x)[1]*(y))

Available macros:

        gap8_sumdotpsc2(x, y, z)
        __SUMDOTPSC2(x, y, z)
*/
int __builtin_pulp_sdotspsc2(
	v2s x,		/**< A vector of 2 shorts */
	short int y,	/**< A short scalar, will be replicated 2 times */
	int z		/**< The accumulator */
	);

/** @brief Dot product between 2 vectors of 4 bytes, accumulates result.

Dot product between 2 vectors of 4 bytes, accumulates result.

Returns: z + ((char)(x)[0]*(char)(y)[0] + (char)(x)[1]*(char)(y)[1] + (char)(x)[2]*(char)(y)[2] + (char)(x)[3]*(char)(y)[3])

Available macros:

        gap8_sumdotp4(x, y, z)
        __SUMDOTP4(x, y, z)
*/
int __builtin_pulp_sdotsp4 (
	v4s x,	/**< A vector of 4 bytes */
	v4s y,	/**< A vector of 4 bytes */
	int z	/**< The accumulator */
	);

/** @brief Dot product  between a vector of 4 bytes and a replicated byte scalar, accumulates the result.

Dot product  between a vector of 4 bytes and a replicated byte scalar, accumulates the result.

Returns: z + ((char)(x)[0]*(char)(y) + (char)(x)[1]*(char)(y) + (char)(x)[2]*(char)(y) + (char)(x)[3]*(char)(y))

Available macros:

        gap8_sumdotpsc4(x, y, z)
        __SUMDOTPSC4(x, y, z)
*/
int __builtin_pulp_sdotspsc4(
	v4s x,		/**< A vector of 4 bytes */
	char  y,	/**< A byte scalar, will be replicated 4 times */
	int z		/**< The accumulator */
	);

/** @brief Dot product between 2 vectors of 2 unsigned shorts, accumulates the result.

Dot product between 2 vectors of 2 unsigned shorts, accumulates the result.

Returns: z + ((unsigned short)(x)[0]*(unsigned short)(y)[0] + (unsigned short)(x)[1]*(unsigned short)(y)[1])

Available macros:

        gap8_sumdotup2(x, y, z)
        __SUMDOTPU2(x, y, z)
*/
unsigned int __builtin_pulp_sdotup2 (
	v2u x,		/**< A vector of 2 unsigned shorts */
	v2u y,		/**< A vector of 2 unsigned shorts */
	unsigned int z	/**< The accumulator */
	);

/** @brief Dot product between 1 vector of 2 unsigned shorts and a replicated unsigned short scalar, accumulates the result.

Dot product between 1 vector of 2 unsigned shorts and a replicated unsigned short scalar, accumulates the result.

Returns: z + ((unsigned short)(x)[0]*(unsigned short)(y) + (unsigned short)(x)[1]*(unsigned short)(y))

Available macros:

        gap8_sumdotpusc2(x, y, z)
        __SUMDOTPUSC2(x, y, z)
*/
unsigned int __builtin_pulp_sdotupsc2(
	v2u x,			/**< A vector of 2 unsigned shorts */
	unsigned short int y,	/**< An unsigned short scalar, will be replicated 2 times */
	unsigned int z		/**< The accumulator */
	);

/** @brief Dot product between 2 vectors of 4 unsigned bytes, accumulates the result.

Dot product between 2 vectors of 4 unsigned bytes, accumulates the result.

Returns:
        z + ((unsigned char)(x)[0]*(unsigned char)(y)[0] + (unsigned char)(x)[1]*(unsigned char)(y)[1] +
             (unsigned char)(x)[2]*(unsigned char)(y)[2] + (unsigned char)(x)[3]*(unsigned char)(y)[3])

Available macros:

        gap8_sumdotpu4(x, y, z)
        __SUMDOTPU4(x, y, z)
*/
unsigned int __builtin_pulp_sdotup4 (
	v4u x,		/**< A vector of 4 unsigned bytes */
	v4u y,		/**< A vector of 4 unsigned bytes */
	unsigned int z	/**< The accumulator */
	);

/** @brief Dot product between 1 vector of 4 unsigned bytes and an unsigned scalar byte replicated 4 times, accumulates the result.

Dot product between 1 vector of 4 unsigned bytes and an unsigned scalar byte replicated 4 times, accumulates the result.

Returns:
        z + ((unsigned char)(x)[0]*(unsigned char)(y) + (unsigned char)(x)[1]*(unsigned char)(y) +
             (unsigned char)(x)[2]*(unsigned char)(y) + (unsigned char)(x)[3]*(unsigned char)(y))

Available macros:

        gap8_sumdotpusc4(x, y, z)
        __SUMDOTPUSC4(x, y, z)
*/
unsigned int __builtin_pulp_sdotupsc4(
	v4u x,			/**< A vector of 4 unsigned bytes */
	unsigned short int y,	/**< An unsigned scalar byte, will be replicated 4 times */
	unsigned int z		/**< The accumulator */
	);

/** @brief Dot product between a vector of 2 unsigned shorts and a vector of 2 signed shorts, accumulates the result.

Dot product between a vector of 2 unsigned shorts and a vector of 2 signed shorts, accumulates the result.

Returns: z + ((unsigned short)(x)[0]*(short)(y)[0] + (unsigned short)(x)[1]*(short)(y)[1])

Available macros:

        gap8_sumdotpus2(x, y, z)
        __SUMDOTPUS2(x, y, z)
*/
int __builtin_pulp_sdotusp2 (
	v2u x,	/**< A vector of 2 unsigned shorts */
	v2s y,	/**< A vector of 2 signed shorts */
	int z	/**< The accumulator */
	);
/** @brief Dot product between a vector of 2 unsigned shorts and a signed short scalar replicated 2 times, accumulates the result.

Dot product between a vector of 2 unsigned shorts and a signed short scalar replicated 2 times, accumulates the result.

Returns: z + ((unsigned short)(x)[0]*(short)(y) + (unsigned short)(x)[1]*(short)(y))

Available macros:

        gap8_sumdotpussc2(x, y, z)
        __SUMDOTPUSSC2(x, y, z)
*/
int __builtin_pulp_sdotuspsc2(
	v2s x,		/**< A vector of 2 unsigned shorts */
	short int y,	/**< A signed scalar short, will be replicated 2 times */
	int z		/**< The accumulator */
	);

/** @brief Dot product between 1 vector of 4 unsigned bytes and 1 vector of 4 signed bytes, accumulates the result.

Dot product between 1 vector of 4 unsigned bytes and 1 vector of 4 signed bytes, accumulates the result.

Returns:
        z + ((unsigned char)(x)[0]*(char)(y)[0] + (unsigned char)(x)[1]*(char)(y)[1] +
             (unsigned char)(x)[2]*(char)(y)[2] + (unsigned char)(x)[3]*(char)(y)[3])

Available macros:

        gap8_sumdotpus4(x, y, z)
        __SUMDOTPUS4(x, y, z)
*/
int __builtin_pulp_sdotusp4 (
	v4u x,	/**< A vector of 4 unsigned bytes */
	v4s y,	/**< A vector of 4 signed bytes */
	int z	/**< The accumulator */
	);

/** @brief Dot product between 1 vector of 4 unsigned bytes and one scalar unsigned byte replicated 4 times, accumulates the result.

Dot product between 1 vector of 4 unsigned bytes and one scalar unsigned byte replicated 4 times, accumulates the result.

Returns:
        z + ((unsigned char)(x)[0]*(char)(y) + (unsigned char)(x)[1]*(char)(y) +
             (unsigned char)(x)[2]*(char)(y) + (unsigned char)(x)[3]*(char)(y))

Available macros:

        gap8_sumdotpussc4(x, y, z)
        __SUMDOTPUSSC4(x, y, z)
*/
int __builtin_pulp_sdotuspsc4(
	v4u x,		/**< A vector of 4 unsigned bytes */
	short int y,	/**< A signed scalar byte, will be replicated 4 times */
	int z		/**< The accumulator */
	);
/** @} */ // Endof DotP


/** @defgroup RWBaseOff ReadWriteBaseOffset
@{ */

/* Read/Write base + offset */

/** @brief Read one int at base + off, off is a byte offset.

Read one int at base + off, off is a byte offset.

Available macros:

        gap8_read_base_off_vol(base, off)
        __READ_BASE_OFF_VOL(base, off)
*/
int __builtin_pulp_OffsetedRead (
	int *base,	/**< A pointer */
	int off		/**< Byte offset to be added to base, no pointer type promotion */
	);

/** @brief Read one short int at base + off, off is a byte offset.

Read one short int at base + off, off is a byte offset.

Available macros:

        gap8_read_base_off_half_vol(base, off)
        __READ_BASE_OFF_HALF_VOL(base, off)
*/
short int __builtin_pulp_OffsetedReadHalf(
	short int *base,	/**< A pointer */
	int off			/**< Byte offset to be added to base, no pointer type promotion */
	);

/** @brief Read one byte at base + off, off is a byte offset.

Read one byte at base + off, off is a byte offset.

Available macros:

        gap8_read_base_off_byte_vol(base, off)
        __READ_BASE_OFF_BYTE_VOL(base, off)
*/
char __builtin_pulp_OffsetedReadbyte(
	char *base,	/**< A pointer */
	int off		/**< Byte offset to be added to base, no pointer type promotion */
	);

/* @brief Write one int, x, at base + off, off is a byte offset.

Write one int, x, at base + off, off is a byte offset.

Available macros:

        gap8_write_base_off_vol(x, base, off)
        __WRITE_BASE_OFF_VOL(x, base, off)
*/
void __builtin_pulp_OffsetedWrite (
	int x,		/**< Value to be written */
	int *base,	/**< A pointer */
	int off		/**< Byte offset to be added to base, no pointer type promotion */
	);

/** @brief Write one short int, x, at base + off, off is a byte offset.

Write one short int, x, at base + off, off is a byte offset.

Available macros:

        gap8_write_base_off_half_vol(x, base, off)
        __WRITE_BASE_OFF_HALF_VOL(x, base, off)
*/
void __builtin_pulp_OffsetedWriteHalf(
	int x,			/**< Value to be written */
	short int *base,	/**< A pointer */
	int off			/**< Byte offset to be added to base, no pointer type promotion */
	);

/** @brief Write one byte, x, at base + off, off is a byte offset.

Write one byte, x, at base + off, off is a byte offset.

Available macros:

        gap8_write_base_off_byte_vol(x, base, off)
        __WRITE_BASE_OFF_BYTE_VOL(x, base, off)
*/
void __builtin_pulp_OffsetedWritebyte(
	int x,		/**< Value to be written */
	char *base,	/**< A pointer */
	int off		/**< Byte offset to be added to base, no pointer type promotion */
	);
/** @} */ // Endof RWBaseOff

/** @defgroup RWSPR ReadWriteSpecialRegisters
@{ */

/* Special purpose registers */

/** @brief Reads special purpose register spr.

Reads special purpose register spr.

Available macros:

        gap8_sprread(spr)
        __SPRREAD(spr)
*/
int __builtin_pulp_spr_read(
	int spr		/**< Special purpose register number */
	);

/** @brief Reads special purpose register spr. Read is volatile.

Reads special purpose register spr. Read is volatile.

Available macros:

        gap8_sprread_vol(spr)
        __SPRREAD_VOL(spr)
*/
int __builtin_pulp_spr_read_vol(
	int spr		/**< Special purpose register number */
	);
/** @brief Writes x into special purpose register spr.

Writes x into special purpose register spr.

Available macros:

        gap8_sprwrite(spr, x)
        __SPRWRITE(spr, x)
*/
void __builtin_pulp_spr_write(
	int spr,	/**< Special purpose register number */
	int x		/**< Value to be written */
	);
/** @brief Sets bit off of special purpose register spr. off is immediare in [0..31]

Sets bit off of special purpose register spr. off is immediare in [0..31]

Available macros:

        gap8_sprbitset(spr, off)
        __SPRBITSET(spr, off)
*/
void __builtin_pulp_spr_bit_set(
	int spr,	/**< Special purpose register number */
	int off		/**< Bit offset, immediate in [0..31] */
	);

/** @brief Clears bit off of special purpose register spr. off is immediare in [0..31]

Clears bit off of special purpose register spr. off is immediare in [0..31]

Available macros:

        gap8_sprbitclr(spr, off)
        __SPRBITCLR(spr, off)
*/
void __builtin_pulp_spr_bit_clr(
	int spr,	/**< Special purpose register number */
	int off		/**< Bit offset, immediate in [0..31] */
	);

/** @brief Reads spr special purpose register and then writes x it. Returns the read content.

Reads spr special purpose register and then writes x it. Returns the read content.

Available macros:

        gap8_sprreadthenwrite(spr, x)
        __SPRREADTHENWRITE(spr, x)
*/
int __builtin_pulp_read_then_spr_write(
	int spr,	/**< Special purpose register number */
	int x		/**< Value to be written */
	);

/** @brief Reads spr special purpose register and then sets bit at position off of spr. off is immediate in [0..31].

Reads spr special purpose register and then sets bit at position off of spr. off is immediate in [0..31].

Available macros:

        gap8_sprreadthenbitset(spr, off)
        __SPRREADTHENBITSET(spr, off)
*/
int __builtin_pulp_read_then_spr_bit_set(
	int spr,	/**< Special purpose register number */
	int off		/**< Bit offset, immediate in [0..31] */
	);

/** @brief Reads spr special purpose register and then clears bit at position off of spr. off is immediate in [0..31].

Reads spr special purpose register and then clears bit at position off of spr. off is immediate in [0..31].

Available macros:

        gap8_sprreadthenbitclr(spr, off)
        __SPRREADTHENBITCLR(spr, off)
*/
int __builtin_pulp_read_then_spr_bit_clr(
	int spr,	/**< Special purpose register number */
	int off		/**< Bit offset, immediate in [0..31] */
	);
/** @} */ // End of RWSPR

/** @defgroup OthersG Others
@{ */

/* Others */

/** @brief Returns number of cores in the cluster

Returns number of cores in the cluster.

Default is 8 but can be overidden by the gcc option -mPE=<value>

Available macros:

        gap8_ncore()
        __NCORE()
*/
int __builtin_pulp_CoreCount();

/** @brief Returns the core number on which a program is currently executing.

Returns the core number on which a program is currently executing.

On Cluster: 0 to 7, on fabric controller: 0.

Available macros:

        gap8_coreid()
        __COREID()
*/
int __builtin_pulp_CoreId();

/** @brief Returns the cluster number on which a program is currently execution.

Returns the cluster number on which a program is currently execution.

For Cluster returns 0 and for fabric controller returns 31.

Available macros:

        gap8_clusterid()
        __CLUSTERID()
*/
int __builtin_pulp_ClusterId();

/** @brief Returns 1 is program currently runs on the fabric controller, 0 otherwise.

Returns 1 is program currently runs on the fabric controller, 0 otherwise.

Available macros:

        gap8_isfc()
        __ISFC()
*/
int __builtin_pulp_IsFc();

/** @}*/ // End of OthersG

/** @}*/

#ifdef __cplusplus
}
#endif

#endif /* GAP9MHSA_INC_BUILTINS_H_ */
