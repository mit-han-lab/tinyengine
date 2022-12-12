/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   JPEGDecoder.cpp
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

 #include "JPEGDecoder.h"
#include "picojpeg.h"
#include <string.h>

JPEGDecoder JpegDec;

JPEGDecoder::JPEGDecoder() {
  mcu_x = 0;
  mcu_y = 0;
  is_available = 0;
  thisPtr = this;
}

JPEGDecoder::~JPEGDecoder() {
  if (pImage)
    delete[] pImage;
  pImage = NULL;
}

uint8_t JPEGDecoder::pjpeg_callback(uint8_t *pBuf, uint8_t buf_size,
                                    uint8_t *pBytes_actually_read,
                                    void *pCallback_data) {
  JPEGDecoder *thisPtr = JpegDec.thisPtr;
  thisPtr->pjpeg_need_bytes_callback(pBuf, buf_size, pBytes_actually_read,
                                     pCallback_data);
  return 0;
}

uint8_t JPEGDecoder::pjpeg_need_bytes_callback(uint8_t *pBuf, uint8_t buf_size,
                                               uint8_t *pBytes_actually_read,
                                               void *pCallback_data) {
  uint n;

  n = jpg_min(g_nInFileSize - g_nInFileOfs, buf_size);

  if (jpg_source == JPEG_ARRAY) {
    for (int i = 0; i < n; i++) {
      pBuf[i] = *jpg_data++;
    }
  }

#ifdef LOAD_SPIFFS
  if (jpg_source == JPEG_FS_FILE)
    g_pInFileFs.read(pBuf, n);
#endif

#if defined(LOAD_SD_LIBRARY) || defined(LOAD_SDFAT_LIBRARY)
  if (jpg_source == JPEG_SD_FILE)
    g_pInFileSd.read(pBuf, n);
#endif

  *pBytes_actually_read = (uint8_t)(n);
  g_nInFileOfs += n;
  return 0;
}

int JPEGDecoder::decode_mcu(void) {

  status = pjpeg_decode_mcu();

  if (status) {
    is_available = 0;

    if (status != PJPG_NO_MORE_BLOCKS) {
#ifdef DEBUG
      Serial.print("pjpeg_decode_mcu() failed with status ");
      Serial.println(status);
#endif

      return -1;
    }
  }
  return 1;
}

int JPEGDecoder::read(void) {
  int y, x;
  uint16_t *pDst_row;

  if (is_available == 0 || mcu_y >= image_info.m_MCUSPerCol) {
    abort();
    return 0;
  }

  pDst_row = pImage;
  for (y = 0; y < image_info.m_MCUHeight; y += 8) {

    const int by_limit =
        jpg_min(8, image_info.m_height - (mcu_y * image_info.m_MCUHeight + y));

    for (x = 0; x < image_info.m_MCUWidth; x += 8) {
      uint16_t *pDst_block = pDst_row + x;

      uint src_ofs = (x * 8U) + (y * 16U);
      const uint8_t *pSrcR = image_info.m_pMCUBufR + src_ofs;
      const uint8_t *pSrcG = image_info.m_pMCUBufG + src_ofs;
      const uint8_t *pSrcB = image_info.m_pMCUBufB + src_ofs;

      const int bx_limit =
          jpg_min(8, image_info.m_width - (mcu_x * image_info.m_MCUWidth + x));

      if (image_info.m_scanType == PJPG_GRAYSCALE) {
        int bx, by;
        for (by = 0; by < by_limit; by++) {
          uint16_t *pDst = pDst_block;

          for (bx = 0; bx < bx_limit; bx++) {
#ifdef SWAP_BYTES
            *pDst++ = (*pSrcR & 0xF8) | (*pSrcR & 0xE0) >> 5 |
                      (*pSrcR & 0xF8) << 5 | (*pSrcR & 0x1C) << 11;
#else
            *pDst++ = (*pSrcR & 0xF8) << 8 | (*pSrcR & 0xFC) << 3 | *pSrcR >> 3;
#endif
            pSrcR++;
          }

          pSrcR += (8 - bx_limit);

          pDst_block += row_pitch;
        }
      } else {
        int bx, by;
        for (by = 0; by < by_limit; by++) {
          uint16_t *pDst = pDst_block;

          for (bx = 0; bx < bx_limit; bx++) {
#ifdef SWAP_BYTES
            *pDst++ = (*pSrcR & 0xF8) | (*pSrcG & 0xE0) >> 5 |
                      (*pSrcB & 0xF8) << 5 | (*pSrcG & 0x1C) << 11;
#else
            *pDst++ = (*pSrcR & 0xF8) << 8 | (*pSrcG & 0xFC) << 3 | *pSrcB >> 3;
#endif
            pSrcR++;
            pSrcG++;
            pSrcB++;
          }

          pSrcR += (8 - bx_limit);
          pSrcG += (8 - bx_limit);
          pSrcB += (8 - bx_limit);

          pDst_block += row_pitch;
        }
      }
    }
    pDst_row += (row_pitch * 8);
  }

  MCUx = mcu_x;
  MCUy = mcu_y;

  mcu_x++;
  if (mcu_x == image_info.m_MCUSPerRow) {
    mcu_x = 0;
    mcu_y++;
  }

  if (decode_mcu() == -1)
    is_available = 0;

  return 1;
}

int JPEGDecoder::readSwappedBytes(void) {
  int y, x;
  uint16_t *pDst_row;

  if (is_available == 0 || mcu_y >= image_info.m_MCUSPerCol) {
    abort();
    return 0;
  }

  pDst_row = pImage;
  for (y = 0; y < image_info.m_MCUHeight; y += 8) {

    const int by_limit =
        jpg_min(8, image_info.m_height - (mcu_y * image_info.m_MCUHeight + y));

    for (x = 0; x < image_info.m_MCUWidth; x += 8) {
      uint16_t *pDst_block = pDst_row + x;

      uint src_ofs = (x * 8U) + (y * 16U);
      const uint8_t *pSrcR = image_info.m_pMCUBufR + src_ofs;
      const uint8_t *pSrcG = image_info.m_pMCUBufG + src_ofs;
      const uint8_t *pSrcB = image_info.m_pMCUBufB + src_ofs;

      const int bx_limit =
          jpg_min(8, image_info.m_width - (mcu_x * image_info.m_MCUWidth + x));

      if (image_info.m_scanType == PJPG_GRAYSCALE) {
        int bx, by;
        for (by = 0; by < by_limit; by++) {
          uint16_t *pDst = pDst_block;

          for (bx = 0; bx < bx_limit; bx++) {

            *pDst++ = (*pSrcR & 0xF8) | (*pSrcR & 0xE0) >> 5 |
                      (*pSrcR & 0xF8) << 5 | (*pSrcR & 0x1C) << 11;

            pSrcR++;
          }
        }
      } else {
        int bx, by;
        for (by = 0; by < by_limit; by++) {
          uint16_t *pDst = pDst_block;

          for (bx = 0; bx < bx_limit; bx++) {

            *pDst++ = (*pSrcR & 0xF8) | (*pSrcG & 0xE0) >> 5 |
                      (*pSrcB & 0xF8) << 5 | (*pSrcG & 0x1C) << 11;

            pSrcR++;
            pSrcG++;
            pSrcB++;
          }

          pSrcR += (8 - bx_limit);
          pSrcG += (8 - bx_limit);
          pSrcB += (8 - bx_limit);

          pDst_block += row_pitch;
        }
      }
    }
    pDst_row += (row_pitch * 8);
  }

  MCUx = mcu_x;
  MCUy = mcu_y;

  mcu_x++;
  if (mcu_x == image_info.m_MCUSPerRow) {
    mcu_x = 0;
    mcu_y++;
  }

  if (decode_mcu() == -1)
    is_available = 0;

  return 1;
}

int JPEGDecoder::decodeFile(const char *pFilename) {

#if defined(ESP8266) || defined(ESP32)
#if defined(LOAD_SD_LIBRARY) || defined(LOAD_SDFAT_LIBRARY)
  if (*pFilename == '/')
#endif
    return decodeFsFile(pFilename);
#endif

#if defined(LOAD_SD_LIBRARY) || defined(LOAD_SDFAT_LIBRARY)
  return decodeSdFile(pFilename);
#endif

  return -1;
}

#ifdef LOAD_SPIFFS

int JPEGDecoder::decodeFsFile(const char *pFilename) {

  fs::File pInFile = SPIFFS.open(pFilename, "r");

  return decodeFsFile(pInFile);
}

int JPEGDecoder::decodeFsFile(const String &pFilename) {

  fs::File pInFile = SPIFFS.open(pFilename, "r");

  return decodeFsFile(pInFile);
}

int JPEGDecoder::decodeFsFile(fs::File jpgFile) {

  g_pInFileFs = jpgFile;

  jpg_source = JPEG_FS_FILE;

  if (!g_pInFileFs) {
#ifdef DEBUG
    Serial.println("ERROR: SPIFFS file not found!");
#endif

    return -1;
  }

  g_nInFileOfs = 0;

  g_nInFileSize = g_pInFileFs.size();

  return decodeCommon();
}
#endif

#if defined(LOAD_SD_LIBRARY) || defined(LOAD_SDFAT_LIBRARY)

int JPEGDecoder::decodeSdFile(const char *pFilename) {

  File pInFile = SD.open(pFilename, FILE_READ);

  return decodeSdFile(pInFile);
}

int JPEGDecoder::decodeSdFile(const String &pFilename) {
#if !defined(ARDUINO_ARCH_SAM)
  File pInFile = SD.open(pFilename, FILE_READ);

  return decodeSdFile(pInFile);
#else
  return -1;
#endif
}

int JPEGDecoder::decodeSdFile(File jpgFile) {

  g_pInFileSd = jpgFile;

  jpg_source = JPEG_SD_FILE;

  if (!g_pInFileSd) {
#ifdef DEBUG
    Serial.println("ERROR: SD file not found!");
#endif

    return -1;
  }

  g_nInFileOfs = 0;

  g_nInFileSize = g_pInFileSd.size();

  return decodeCommon();
}
#endif

int JPEGDecoder::decodeArray(const uint8_t array[], uint32_t array_size) {

  jpg_source = JPEG_ARRAY;

  g_nInFileOfs = 0;

  jpg_data = (uint8_t *)array;

  g_nInFileSize = array_size;

  return decodeCommon();
}

int JPEGDecoder::decodeCommon(void) {

  width = 0;
  height = 0;
  comps = 0;
  MCUSPerRow = 0;
  MCUSPerCol = 0;
  scanType = (pjpeg_scan_type_t)0;
  MCUWidth = 0;
  MCUHeight = 0;

  status = pjpeg_decode_init(&image_info, pjpeg_callback, NULL, 0);

  if (status) {
#ifdef DEBUG
    Serial.print("pjpeg_decode_init() failed with status ");
    Serial.println(status);

    if (status == PJPG_UNSUPPORTED_MODE) {
      Serial.println("Progressive JPEG files are not supported.");
    }
#endif

    return 0;
  }

  decoded_width = image_info.m_width;
  decoded_height = image_info.m_height;

  row_pitch = image_info.m_MCUWidth;
  pImage = new uint16_t[image_info.m_MCUWidth * image_info.m_MCUHeight];

  memset(pImage, 0,
         image_info.m_MCUWidth * image_info.m_MCUHeight * sizeof(*pImage));

  row_blocks_per_mcu = image_info.m_MCUWidth >> 3;
  col_blocks_per_mcu = image_info.m_MCUHeight >> 3;

  is_available = 1;

  width = decoded_width;
  height = decoded_height;
  comps = 1;
  MCUSPerRow = image_info.m_MCUSPerRow;
  MCUSPerCol = image_info.m_MCUSPerCol;
  scanType = image_info.m_scanType;
  MCUWidth = image_info.m_MCUWidth;
  MCUHeight = image_info.m_MCUHeight;

  return decode_mcu();
}

void JPEGDecoder::abort(void) {

  mcu_x = 0;
  mcu_y = 0;
  is_available = 0;
  if (pImage)
    delete[] pImage;
  pImage = NULL;

#ifdef LOAD_SPIFFS
  if (jpg_source == JPEG_FS_FILE)
    if (g_pInFileFs)
      g_pInFileFs.close();
#endif

#if defined(LOAD_SD_LIBRARY) || defined(LOAD_SDFAT_LIBRARY)
  if (jpg_source == JPEG_SD_FILE)
    if (g_pInFileSd)
      g_pInFileSd.close();
#endif
}
