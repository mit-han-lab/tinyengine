/* ----------------------------------------------------------------------
 * Project: TinyEngine
 * Title:   stubs.c
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

#pragma GCC diagnostic ignored "-Wunused-parameter"

extern int errno;
extern int __io_putchar(int ch) __attribute__((weak));
extern int __io_getchar(void) __attribute__((weak));

int _write(int fd, const void *buffer, unsigned int count) {
  unsigned int i;

  for (i = 0; i < count; i++) {
    __io_putchar(((char *)buffer)[i]);
  }

  return count;
}

int _read(int fd, void *buffer, unsigned int count) {
  unsigned int i;

  for (i = 0; i < count; i++) {
    ((char *)buffer)[i] = __io_getchar();
  }

  return count;
}

int _close(int fd) { return -1; }

int _fstat(int fd, void *buffer) { return -1; }

int _isatty(int fd) { return -1; }

long _lseek(int fd, long offset, int origin) { return -1; }

void _exit(int status) {}
