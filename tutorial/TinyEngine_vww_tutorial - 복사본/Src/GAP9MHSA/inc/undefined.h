/*
 * undefined.h
 *
 *  Created on: Apr 15, 2024
 *      Author: sm5pyo
 */

//#include "builtins.h"

#ifndef GAP9MHSA_INC_UNDEFINED_H_
#define GAP9MHSA_INC_UNDEFINED_H_


#ifdef __cplusplus
 extern "C" {
#endif


 void __attribute__ ((noinline)) linearQK_4x2_H(
   const int8_t *  pInBuffer,
   const int8_t *  pWeight,
   const int16_t * pBiasBuffer,
   int8_t *        pOutBuffer,
   const uint16_t  dim_sequence,
   const uint16_t  dim_embedding,
   const uint16_t  projections,
   const uint16_t  heads,
   const int16_t   requant_div,
   const int16_t   requant_mul
 );
 void __attribute__ ((noinline)) linearV_4x2_H(
   const int8_t *  pInBuffer,
   const int8_t *  pWeight,
   const int16_t * pBiasBuffer,
   int8_t *        pOutBuffer,
   const uint16_t  dim_sequence,
   const uint16_t  dim_embedding,
   const uint16_t  projections,
   const uint16_t  heads,
   const int16_t   requant_div,
   const int16_t   requant_mul
 );
 void __attribute__ ((noinline)) matmulSoftmax_4x2_S(
   const int8_t *  pInBuffer,
   const int8_t *  pWeight,
   int8_t *        pOutBuffer,
   const uint16_t  dim_sequence,
   const uint16_t  projections,
   const uint16_t  heads,
   const int16_t   requant_div,
   const int16_t   requant_mul,
   const int32_t   coeffA,
   const int32_t   coeffB,
   const int32_t   coeffC,
   const int32_t   log2,
   const uint32_t  n_levels
 );
 void __attribute__ ((noinline)) matmul_4x2_S(
   const int8_t *  pInBuffer,
   const int8_t *  pWeight,
   int8_t *        pOutBuffer,
   const uint16_t  dim_sequence,
   const uint16_t  projections,
   const uint16_t  heads,
   const int16_t   requant_div,
   const int16_t   requant_mul
 );
 void __attribute__ ((noinline)) linearO_4x2_H(
   const int8_t *  pInBuffer,
   const int8_t *  pWeight,
   const int16_t * pBiasBuffer,
   int8_t *        pOutBuffer,
   const uint16_t  dim_sequence,
   const uint16_t  dim_embedding,
   const uint16_t  projections,
   const uint16_t  heads,
   const int16_t   requant_div,
   const int16_t   requant_mul
 );

int submain();
void cluster_fork(void *args);

#define NUM_CORES 1
typedef   signed short v2s __attribute__((vector_size (4)));
typedef unsigned short v2u __attribute__((vector_size (4)));

typedef   signed char  v4s __attribute__((vector_size (4)));
typedef unsigned char  v4u __attribute__((vector_size (4)));

#define min(a,b) ((a)<(b)?(a):(b))
//#define clip8(x) __builtin_pulp_clip_r(x, 127)

//int __builtin_pulp_sdotsp4 (
//	v4s x,	/**< A vector of 4 bytes */
//	v4s y,	/**< A vector of 4 bytes */
//	int z	/**< The accumulator */
//	);
//
//void arm_dot_prod_q7(
//    const q7_t * pSrcA,
//    const q7_t * pSrcB,
//    uint32_t blockSize,
//    q31_t * result)
//{

#define SumDotp(pSrcA, pSrcB, result)                                       arm_dot_prod_q7(pSrcA, pSrcB, 4, result)
#define SumDotp4(a, b, c)                                       arm_dot_prod_q7(a, b, 4, c)
//#define SumDotp8(a, b, c)                                       __builtin_pulp_sdotusp8(a, b, c)
//#define SumDotp16(a, b, c)                                      __builtin_pulp_sdotusp16(a, b, c)
//#define SumDotps4(a, b, c)                                      __builtin_pulp_sdotsp4(a, b, c)
//#define SumDotps8(a, b, c)                                      __builtin_pulp_sdotsp8(a, b, c)
//#define SumDotps16(a, b, c)                                     __builtin_pulp_sdotsp16(a, b, c)
//#define clip4(x)                                                __builtin_pulp_clipu_r(x, 15)
//#define clip2(x)                                                __builtin_pulp_clipu_r(x, 3)

//unsigned int __builtin_pulp_clipu_r(
//	int x,		/**< Argument */
//	int bound	/**< Clipping bound: [0 .. bound] */
//	);
//
//void arm_clip_q7(const q7_t * pSrc,
//  q7_t * pDst,
//  q7_t low,
//  q7_t high,
//  uint32_t numSamples)

#define clip8(pSrc, pDst, low, high, numSamples)                   arm_clip_q7(pSrc, pDst, low, high, numSamples)


//unsigned int __function__(int x); /**< Argument */
//	/** @brief Bit position of the first bit set in x, starting from LSB. */


typedef struct pi_task pi_task_t;

struct pi_device;
struct pi_device_api;
// pmsis device minimal api: used for basic inheritance
typedef struct pi_device_api
{
    int (*open)(struct pi_device *device);
    int (*close)(struct pi_device *device);
    int (*open_async)(struct pi_device *device, pi_task_t *async);
    int (*close_async)(struct pi_device *device, pi_task_t *async);
    ssize_t (*read)(struct pi_device *device, uint32_t ext_addr,
                    void *buffer, uint32_t size, pi_task_t *async);
    ssize_t (*write)(struct pi_device *device, uint32_t ext_addr,
                     const void *buffer, uint32_t size, pi_task_t *async);
    int (*ioctl)(struct pi_device *device, uint32_t func_id, void *arg);
    int (*ioctl_async)(struct pi_device *device, uint32_t func_id,
                       void *arg, pi_task_t *async);
    void *specific_api;
} pi_device_api_t;


// device struct, it wont stay here
typedef struct pi_device {
    struct pi_device_api *api; // function pointers
    void *config; // driver conf: might be filled either using device tree or manually
    void *data; // driver data
} pi_device_t;


struct pi_task_implem
{
  struct pi_task *next;
  struct rt_thread_s *thread;
  int pending;
  int keep;
  void (*saved_callback)(void *);
  void *saved_arg;
  int saved_pending;

  union {
//    rt_periph_copy_t copy;
    struct {
      unsigned int data[8];
    };
    struct {
      unsigned int time;
    };
//    rt_bridge_req_t bridge_req;
  };
} __attribute__((packed));

#define PI_TASK_IMPLEM struct pi_task_implem implem

typedef struct pi_task{
    struct pi_task *next;
    // Warning, might be accessed inline in asm, and thus can not be moved
    uintptr_t arg[4];
    int8_t done;
    int id;

    PI_TASK_IMPLEM;

} pi_task_t;

typedef struct __pi_flash_api_t {
  int (*open)(struct pi_device *device);
  void (*close)(struct pi_device *device);
  int32_t (*ioctl)(struct pi_device *device, uint32_t cmd, void *arg);
  void (*read_async)(struct pi_device *device, uint32_t pi_flash_addr, void *data, uint32_t size, pi_task_t *task);
  void (*program_async)(struct pi_device *device, uint32_t pi_flash_addr, const void *data, uint32_t size, pi_task_t *task);
  void (*erase_chip_async)(struct pi_device *device, pi_task_t *task);
  void (*erase_sector_async)(struct pi_device *device, uint32_t pi_flash_addr, pi_task_t *task);
  void (*erase_async)(struct pi_device *device, uint32_t pi_flash_addr, int size, pi_task_t *task);
  void (*reg_set_async)(struct pi_device *device, uint32_t pi_flash_addr, uint8_t *value, pi_task_t *task);
  void (*reg_get_async)(struct pi_device *device, uint32_t pi_flash_addr, uint8_t *value, pi_task_t *task);
  int (*copy_async)(struct pi_device *device, uint32_t pi_flash_addr, void *buffer, uint32_t size, int ext2loc, pi_task_t *task);
  int (*copy_2d_async)(struct pi_device *device, uint32_t pi_flash_addr, void *buffer, uint32_t size, uint32_t stride, uint32_t length, int ext2loc, pi_task_t *task);
  // synchronous implems
  int (*read)(struct pi_device *device, uint32_t pi_flash_addr, void *data, uint32_t size);
  int (*program)(struct pi_device *device, uint32_t pi_flash_addr, const void *data, uint32_t size);
  int (*erase_chip)(struct pi_device *device);
  int (*erase_sector)(struct pi_device *device, uint32_t pi_flash_addr);
  int (*erase)(struct pi_device *device, uint32_t pi_flash_addr, int size);
  int (*reg_set)(struct pi_device *device, uint32_t pi_flash_addr, uint8_t *value);
  int (*reg_get)(struct pi_device *device, uint32_t pi_flash_addr, uint8_t *value);
  int (*copy)(struct pi_device *device, uint32_t pi_flash_addr, void *buffer, uint32_t size, int ext2loc);
  int (*copy_2d)(struct pi_device *device, uint32_t pi_flash_addr, void *buffer, uint32_t size, uint32_t stride, uint32_t length, int ext2loc);
} pi_flash_api_t;


struct pi_flash_conf {
  pi_flash_api_t *api;    /*!< Pointer to specific flash methods. Reserved for
    internal runtime usage. */
#if defined(__GAP9__)
    pi_aes_utils_conf_t aes_conf; /*!< AES configuration for on-the-fly
                                       encryption/decryption */
#endif
};

struct pi_mx25u51245g_conf
{
  struct pi_flash_conf flash;  /*!< Generic flash configuration. */
    int spi_itf;           /*!< SPI interface where the RAM is
        connected. */
    int spi_cs;            /*!< Chip select where the RAM is connected. */
    int xip_en;
    uint32_t baudrate;     /*!< Baudrate (in bytes/second). */
    uint32_t mba;          /*!< Mapping base address for the sub IP to
        auto choose CS based on input address */
};

#ifdef __cplusplus
}
#endif

#endif /* GAP9MHSA_INC_UNDEFINED_H_ */
