/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   picojpeg.h
 *
 * Reference papers:
 *  - MCUNet: Tiny Deep Learning on IoT Device, NeurIPS 2020
 *  - MCUNetV2: Memory-Efficient Patch-based Inference for Tiny Deep Learning, NeurIPS 2021
 *  - MCUNetV3: On-Device Training Under 256KB Memory, NeurIPS 2022
 * Contact authors:
 *  - Wei-Ming Chen, wmchen@mit.edu
 *  - Wei-Chen Wang, wweichen@mit.edu
 *  - Ji Lin, jilin@mit.edu
 *  - Ligeng Zhu, ligeng@mit.edu
 *  - Song Han, songhan@mit.edu
 *
 * Target ISA:  ARMv7E-M
 * -------------------------------------------------------------------- */

#ifndef PICOJPEG_H
#define PICOJPEG_H

#ifdef __cplusplus
extern "C" {
#endif

enum {
  PJPG_NO_MORE_BLOCKS = 1,
  PJPG_BAD_DHT_COUNTS,
  PJPG_BAD_DHT_INDEX,
  PJPG_BAD_DHT_MARKER,
  PJPG_BAD_DQT_MARKER,
  PJPG_BAD_DQT_TABLE,
  PJPG_BAD_PRECISION,
  PJPG_BAD_HEIGHT,
  PJPG_BAD_WIDTH,
  PJPG_TOO_MANY_COMPONENTS,
  PJPG_BAD_SOF_LENGTH,
  PJPG_BAD_VARIABLE_MARKER,
  PJPG_BAD_DRI_LENGTH,
  PJPG_BAD_SOS_LENGTH,
  PJPG_BAD_SOS_COMP_ID,
  PJPG_W_EXTRA_BYTES_BEFORE_MARKER,
  PJPG_NO_ARITHMITIC_SUPPORT,
  PJPG_UNEXPECTED_MARKER,
  PJPG_NOT_JPEG,
  PJPG_UNSUPPORTED_MARKER,
  PJPG_BAD_DQT_LENGTH,
  PJPG_TOO_MANY_BLOCKS,
  PJPG_UNDEFINED_QUANT_TABLE,
  PJPG_UNDEFINED_HUFF_TABLE,
  PJPG_NOT_SINGLE_SCAN,
  PJPG_UNSUPPORTED_COLORSPACE,
  PJPG_UNSUPPORTED_SAMP_FACTORS,
  PJPG_DECODE_ERROR,
  PJPG_BAD_RESTART_MARKER,
  PJPG_ASSERTION_ERROR,
  PJPG_BAD_SOS_SPECTRAL,
  PJPG_BAD_SOS_SUCCESSIVE,
  PJPG_STREAM_READ_ERROR,
  PJPG_NOTENOUGHMEM,
  PJPG_UNSUPPORTED_COMP_IDENT,
  PJPG_UNSUPPORTED_QUANT_TABLE,
  PJPG_UNSUPPORTED_MODE,
};

typedef enum {
  PJPG_GRAYSCALE,
  PJPG_YH1V1,
  PJPG_YH2V1,
  PJPG_YH1V2,
  PJPG_YH2V2
} pjpeg_scan_type_t;

typedef struct {

  int m_width;
  int m_height;

  int m_comps;

  int m_MCUSPerRow;
  int m_MCUSPerCol;

  pjpeg_scan_type_t m_scanType;

  int m_MCUWidth;
  int m_MCUHeight;
  unsigned char *m_pMCUBufR;
  unsigned char *m_pMCUBufG;
  unsigned char *m_pMCUBufB;
} pjpeg_image_info_t;

typedef unsigned char (*pjpeg_need_bytes_callback_t)(
    unsigned char *pBuf, unsigned char buf_size,
    unsigned char *pBytes_actually_read, void *pCallback_data);

unsigned char
pjpeg_decode_init(pjpeg_image_info_t *pInfo,
                  pjpeg_need_bytes_callback_t pNeed_bytes_callback,
                  void *pCallback_data, unsigned char reduce);

unsigned char pjpeg_decode_mcu(void);

#ifdef __cplusplus
}
#endif

#endif
