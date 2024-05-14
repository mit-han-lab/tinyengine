/*
 * mchan_test.h
 * Alessio Burrello <alessio.burrello@unibo.it>
 *
 * Copyright (C) 2019-2020 University of Bologna
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */
#if __PLATFORM__ != ARCHI_PLATFORM_FPGA && (PULP_SDK == 1)
#ifndef MCHAN_H
#define MCHAN_H
// INLINE --> SPEED UP
// always no inline --> reduce code size dramatically
#include "pmsis.h"

#define TX 0
#define RX 1
#define INC 1
#define FIX 0
#define LIN 0
#define TWD 1

#define PLATFORM 'FPGA'

#define MCHAN_COMMAND_QUEUE (ARCHI_MCHAN_DEMUX_ADDR)
#if (MCHAN_VERSION < 7)
#define MCHAN_STATUS_REGISTER (ARCHI_MCHAN_DEMUX_ADDR + PLP_DMA_STATUS_OFFSET)
#elif (MCHAN_VERSION == 7) 
#define MCHAN_STATUS_REGISTER (ARCHI_MCHAN_DEMUX_ADDR + MCHAN_STATUS_OFFSET)
#endif //MCHAN_VERSION

// TEMPORARY FIX DAVIDE
#define PE_MCHAN_COMMAND_QUEUE 0x10201800
#define PE_MCHAN_STATUS_REGISTER 0x10201804

#ifdef ARCHI_MCHAN_EXT_OFFSET
#if (MCHAN_VERSION < 7)
#define PE_MCHAN_COMMAND_QUEUE (ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(0) + ARCHI_MCHAN_EXT_OFFSET + PLP_DMA_QUEUE_OFFSET)
#define PE_MCHAN_STATUS_REGISTER (ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(0) + ARCHI_MCHAN_EXT_OFFSET + PLP_DMA_STATUS_OFFSET)
#elif (MCHAN_VERSION == 7) 
#define PE_MCHAN_COMMAND_QUEUE (ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(0) + ARCHI_MCHAN_EXT_OFFSET + MCHAN_CMD_OFFSET)
#define PE_MCHAN_STATUS_REGISTER (ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(0) + ARCHI_MCHAN_EXT_OFFSET + MCHAN_STATUS_OFFSET)
#endif //MCHAN_VERSION 
#endif

#define FC_DMA_EVENT 8
#define CL_DMA_EVENT 22

static  __attribute__ ((noinline))  int mchan_alloc(){
  return *(volatile int*) MCHAN_COMMAND_QUEUE;
}

#if PLATFORM == 'GAP8'
static void mchan_transfer(
  unsigned int len,
  char type,
  char incr, 
  char twd,
  char ele, 
  char ile, 
  char ble, 
  unsigned int ext_addr, 
  unsigned int tcdm_addr, 
  unsigned short int count, 
  unsigned short int stride)
{
#if (MCHAN_VERSION < 7)
  *(volatile int*) MCHAN_COMMAND_QUEUE = len | (type<<PLP_DMA_TYPE_BIT) | ( incr<<PLP_DMA_INCR_BIT) | (twd <<PLP_DMA_2D_BIT) | (ele<<PLP_DMA_ELE_BIT) | (ile <<PLP_DMA_ILE_BIT) | (ble <<PLP_DMA_BLE_BIT);
#elif (MCHAN_VERSION == 7) 
 *(volatile int*) MCHAN_COMMAND_QUEUE = len | (type<<MCHAN_CMD_CMD_TYPE_BIT) | ( incr<<MCHAN_CMD_CMD_INC_BIT) | (twd <<MCHAN_CMD_CMD__2D_EXT_BIT) | (ele<<MCHAN_CMD_CMD_ELE_BIT) | (ile <<MCHAN_CMD_CMD_ILE_BIT) | (ble <<MCHAN_CMD_CMD_BLE_BIT);
#endif //MCHAN_VERSION 
 *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_addr;
 *(volatile int*) MCHAN_COMMAND_QUEUE = ext_addr;

 if (twd == 1) *(volatile int*) MCHAN_COMMAND_QUEUE = count | (stride << 16);
}
#elif PLATFORM == 'FPGA'
static inline void mchan_transfer(
  unsigned int len, // lenght of transfer
  char type,   // direction of transfer 1. L2 to L1  0. L1 to L2
  char incr,   //  incre transfer 1. incremental 0. not incremental
  char twd_ext,  // 2-D transf
  char twd_tcdm,  // 2-D transf
  char ele,  // event line enable (set to 1)
  char ile,  // interrupt line enable (set to 0)
  char ble,  //  broadcast line enable (set to 0)
  unsigned int ext_addr, // L2 address  
  unsigned int tcdm_addr,  // L1 address
  unsigned int ext_count, // ext count (for single bit transfer set to 1) (maybe it is the length of 1-d transfer)
  unsigned int ext_stride, // ext stride(stride of the transfer)
  unsigned int tcdm_count,  // tcdm count (for single bit transfer set to 1) (maybe it is the length of 1-d transfer)
  unsigned int tcdm_stride) // tcdm stride (stride of the transfer)
{
 //*(volatile int*) MCHAN_COMMAND_QUEUE = len | (type<<PLP_DMA_TYPE_BIT) | ( incr<<PLP_DMA_INCR_BIT) | (twd_ext <<PLP_DMA_2D_BIT) | (ele<<PLP_DMA_ELE_BIT) | (ile <<PLP_DMA_ILE_BIT) | (ble <<PLP_DMA_BLE_BIT) | (twd_tcdm << PLP_DMA_2D_TCDM_BIT);
 *(volatile int*) MCHAN_COMMAND_QUEUE = len | (type<<MCHAN_CMD_CMD_TYPE_BIT) | ( incr<<MCHAN_CMD_CMD_INC_BIT) | (twd_ext <<MCHAN_CMD_CMD__2D_EXT_BIT) | (ele<<MCHAN_CMD_CMD_ELE_BIT) | (ile <<MCHAN_CMD_CMD_ILE_BIT) | (ble <<MCHAN_CMD_CMD_BLE_BIT) | (twd_tcdm << MCHAN_CMD_CMD__2D_TCDM_BIT);
 *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_addr;
 *(volatile int*) MCHAN_COMMAND_QUEUE = ext_addr;
 
 if (twd_ext  == 1)
   {
     *(volatile int*) MCHAN_COMMAND_QUEUE = ext_count;
     *(volatile int*) MCHAN_COMMAND_QUEUE = ext_stride;
   }
 
 if (twd_tcdm == 1)
   { 
     *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_count; 
     *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_stride;
   }
 
}
#endif


static void mchan_barrier(int id) {
  while(((*(volatile int*)(MCHAN_STATUS_REGISTER)) >> id ) & 0x1 ) {
    eu_evt_maskWaitAndClr(1<<FC_DMA_EVENT);
 }
}

static void mchan_free(int id) {
  *(volatile int*) MCHAN_STATUS_REGISTER = 0x1 << id;
}

#ifdef PE_MCHAN_COMMAND_QUEUE
static  __attribute__ ((noinline))  int pe_mchan_alloc(){
  return *(volatile int*) PE_MCHAN_COMMAND_QUEUE;
}

static  void pe_mchan_transfer(unsigned int len, char type, char incr, char twd, char ele, char ile, char ble, unsigned int ext_addr, unsigned int tcdm_addr, unsigned short int count, unsigned short int stride)
{
#if (MCHAN_VERSION < 7)
 *(volatile int*) PE_MCHAN_COMMAND_QUEUE = len | (type<<PLP_DMA_TYPE_BIT) | ( incr<<PLP_DMA_INCR_BIT) | (twd <<PLP_DMA_2D_BIT) | (ele<<PLP_DMA_ELE_BIT) | (ile <<PLP_DMA_ILE_BIT) | (ble <<PLP_DMA_BLE_BIT);
#elif (MCHAN_VERSION == 7) 
 *(volatile int*) MCHAN_COMMAND_QUEUE = len | (type<<MCHAN_CMD_CMD_TYPE_BIT) | ( incr<<MCHAN_CMD_CMD_INC_BIT) | (twd <<MCHAN_CMD_CMD__2D_EXT_BIT) | (ele<<MCHAN_CMD_CMD_ELE_BIT) | (ile <<MCHAN_CMD_CMD_ILE_BIT) | (ble <<MCHAN_CMD_CMD_BLE_BIT);
#endif //MCHAN_VERSION  
 *(volatile int*) PE_MCHAN_COMMAND_QUEUE = tcdm_addr;
 *(volatile int*) PE_MCHAN_COMMAND_QUEUE = ext_addr;

 if (twd == 1) *(volatile int*) PE_MCHAN_COMMAND_QUEUE = count | (stride << 16);
}

static  __attribute__ ((noinline))  void pe_mchan_barrier(int id) {
  while(((*(volatile int*)(PE_MCHAN_STATUS_REGISTER)) >> id ) & 0x1 ) {
    if (plp_isFc()) {
#if PULP_CHIP != CHIP_GAP
      hal_itc_wait_for_event_noirq(1<<FC_DMA_EVENT);
#else
      eu_evt_maskWaitAndClr(1<<FC_DMA_EVENT);
#endif
    }
    else eu_evt_maskWaitAndClr(1<<CL_DMA_EVENT);
  };
}

static  __attribute__ ((noinline))  void pe_mchan_free(int id) {
  *(volatile int*) PE_MCHAN_STATUS_REGISTER = 0x1 << id;
}
#endif
#endif
// ON FPGA / GAP8
#else
#ifndef MCHAN_H
#define MCHAN_H
// INLINE --> SPEED UP
// always no inline --> reduce code size dramatically
#if (GAP_SDK == 1)
#include "pulp.h"
#define PLATFORM 'GAP8'
#else
#include "pmsis.h"
#define PLATFORM 'FPGA'
#endif

#define TX 0
#define RX 1
#define INC 1
#define FIX 0
#define LIN 0
#define TWD 1


#if (GAP_SDK == 1)
#define MCHAN_COMMAND_QUEUE (ARCHI_MCHAN_DEMUX_ADDR)
#if (MCHAN_VERSION < 7)
#define MCHAN_STATUS_REGISTER (ARCHI_MCHAN_DEMUX_ADDR + PLP_DMA_STATUS_OFFSET)
#elif (MCHAN_VERSION == 7) 
#define MCHAN_STATUS_REGISTER (ARCHI_MCHAN_DEMUX_ADDR + MCHAN_STATUS_OFFSET)
#endif //MCHAN_VERSION

// TEMPORARY FIX DAVIDE
#define PE_MCHAN_COMMAND_QUEUE 0x10201800
#define PE_MCHAN_STATUS_REGISTER 0x10201804

#ifdef ARCHI_MCHAN_EXT_OFFSET
#if (MCHAN_VERSION < 7)
#define PE_MCHAN_COMMAND_QUEUE (ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(0) + ARCHI_MCHAN_EXT_OFFSET + PLP_DMA_QUEUE_OFFSET)
#define PE_MCHAN_STATUS_REGISTER (ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(0) + ARCHI_MCHAN_EXT_OFFSET + PLP_DMA_STATUS_OFFSET)
#elif (MCHAN_VERSION == 7) 
#define PE_MCHAN_COMMAND_QUEUE (ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(0) + ARCHI_MCHAN_EXT_OFFSET + MCHAN_CMD_OFFSET)
#define PE_MCHAN_STATUS_REGISTER (ARCHI_CLUSTER_PERIPHERALS_GLOBAL_ADDR(0) + ARCHI_MCHAN_EXT_OFFSET + MCHAN_STATUS_OFFSET)
#endif //MCHAN_VERSION 
#endif
#else
#define MCHAN_COMMAND_QUEUE    0x10204400
#define MCHAN_STATUS_REGISTER  0x10204404

// TEMPORARY FIX DAVIDE
#define PLP_DMA_2D_TCDM_BIT 22

#define PE_MCHAN_COMMAND_QUEUE   0x10201C00
#define PE_MCHAN_STATUS_REGISTER 0x10201C04

#define PLP_DMA_TYPE_BIT  0x00000011
#define PLP_DMA_INCR_BIT 0x00000012
#define PLP_DMA_2D_BIT 0x00000013
#define PLP_DMA_ELE_BIT 0x00000014
#define PLP_DMA_ILE_BIT 0x00000015
#define PLP_DMA_BLE_BIT 0x00000016
#define PLP_DMA_2D_TCDM_BIT 0x0000017
#endif



#define FC_DMA_EVENT 8
#define CL_DMA_EVENT 22

static  __attribute__ ((noinline))  int mchan_alloc(){
  return *(volatile int*) MCHAN_COMMAND_QUEUE;
}

#if PLATFORM == 'GAP8'
static void mchan_transfer(unsigned int len, char type, char incr, char twd, char ele, char ile, char ble, unsigned int ext_addr, unsigned int tcdm_addr, unsigned short int count, unsigned short int stride)
{
 *(volatile int*) MCHAN_COMMAND_QUEUE = len | (type<<PLP_DMA_TYPE_BIT) | ( incr<<PLP_DMA_INCR_BIT) | (twd <<PLP_DMA_2D_BIT) | (ele<<PLP_DMA_ELE_BIT) | (ile <<PLP_DMA_ILE_BIT) | (ble <<PLP_DMA_BLE_BIT);
 *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_addr;
 *(volatile int*) MCHAN_COMMAND_QUEUE = ext_addr;

 if (twd == 1) *(volatile int*) MCHAN_COMMAND_QUEUE = count | (stride << 16);
}
#elif PLATFORM == 'FPGA'
static inline void mchan_transfer(
  unsigned int len, // lenght of transfer
  char type,   // direction of transfer 1. L2 to L1  0. L1 to L2
  char incr,   //  incre transfer 1. incremental 0. not incremental
  char twd_ext,  // 2-D transf
  char twd_tcdm,  // 2-D transf
  char ele,  // event line enable (set to 1)
  char ile,  // interrupt line enable (set to 0)
  char ble,  //  broadcast line enable (set to 0)
  unsigned int ext_addr, // L2 address  
  unsigned int tcdm_addr,  // L1 address
  unsigned int ext_count, // ext count (for single bit transfer set to 1) (maybe it is the length of 1-d transfer)
  unsigned int ext_stride, // ext stride(stride of the transfer)
  unsigned int tcdm_count,  // tcdm count (for single bit transfer set to 1) (maybe it is the length of 1-d transfer)
  unsigned int tcdm_stride) // tcdm stride (stride of the transfer)
{
  *(volatile int*) MCHAN_COMMAND_QUEUE = len | (type<<PLP_DMA_TYPE_BIT) | ( incr<<PLP_DMA_INCR_BIT) | (twd_ext <<PLP_DMA_2D_BIT) | (ele<<PLP_DMA_ELE_BIT) | (ile <<PLP_DMA_ILE_BIT) | (ble <<PLP_DMA_BLE_BIT) | (twd_tcdm << PLP_DMA_2D_TCDM_BIT);
 *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_addr;
 *(volatile int*) MCHAN_COMMAND_QUEUE = ext_addr;
 
 if (twd_ext  == 1)
   {
     *(volatile int*) MCHAN_COMMAND_QUEUE = ext_count;
     *(volatile int*) MCHAN_COMMAND_QUEUE = ext_stride;
   }
 
 if (twd_tcdm == 1)
   { 
     *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_count; 
     *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_stride;
   }
 
}
#endif


static void mchan_barrier(int id) {
  while(((*(volatile int*)(MCHAN_STATUS_REGISTER)) >> id ) & 0x1 ) {
    eu_evt_maskWaitAndClr(1<<FC_DMA_EVENT);
 }
}

static void mchan_free(int id) {
  *(volatile int*) MCHAN_STATUS_REGISTER = 0x1 << id;
}

#if (GAP_SDK == 1)
#ifdef PE_MCHAN_COMMAND_QUEUE
static  __attribute__ ((noinline))  int pe_mchan_alloc(){
  return *(volatile int*) PE_MCHAN_COMMAND_QUEUE;
}

static  void pe_mchan_transfer(unsigned int len, char type, char incr, char twd, char ele, char ile, char ble, unsigned int ext_addr, unsigned int tcdm_addr, unsigned short int count, unsigned short int stride)
{
#if (MCHAN_VERSION < 7)
  *(volatile int*) PE_MCHAN_COMMAND_QUEUE = len | (type<<PLP_DMA_TYPE_BIT) | ( incr<<PLP_DMA_INCR_BIT) | (twd <<PLP_DMA_2D_BIT) | (ele<<PLP_DMA_ELE_BIT) | (ile <<PLP_DMA_ILE_BIT) | (ble <<PLP_DMA_BLE_BIT);
#elif (MCHAN_VERSION == 7) 
  *(volatile int*) MCHAN_COMMAND_QUEUE = len | (type<<MCHAN_CMD_CMD_TYPE_BIT) | ( incr<<MCHAN_CMD_CMD_INC_BIT) | (twd <<MCHAN_CMD_CMD__2D_EXT_BIT) | (ele<<MCHAN_CMD_CMD_ELE_BIT) | (ile <<MCHAN_CMD_CMD_ILE_BIT) | (ble <<MCHAN_CMD_CMD_BLE_BIT);
#endif //MCHAN_VERSION  
  *(volatile int*) PE_MCHAN_COMMAND_QUEUE = tcdm_addr;
  *(volatile int*) PE_MCHAN_COMMAND_QUEUE = ext_addr;
  if (twd == 1) *(volatile int*) PE_MCHAN_COMMAND_QUEUE = count | (stride << 16);
}

static  __attribute__ ((noinline))  void pe_mchan_barrier(int id) {
  while(((*(volatile int*)(PE_MCHAN_STATUS_REGISTER)) >> id ) & 0x1 ) {
    if (plp_isFc()) {
#if PULP_CHIP != CHIP_GAP
      hal_itc_wait_for_event_noirq(1<<FC_DMA_EVENT);
#else
      eu_evt_maskWaitAndClr(1<<FC_DMA_EVENT);
#endif
    }
    else eu_evt_maskWaitAndClr(1<<CL_DMA_EVENT);
  };
}

static  __attribute__ ((noinline))  void pe_mchan_free(int id) {
  *(volatile int*) PE_MCHAN_STATUS_REGISTER = 0x1 << id;
}
#endif
#else
static inline int fc_mchan_alloc(){
  return *(volatile int*) PE_MCHAN_COMMAND_QUEUE;
}

static inline void fc_mchan_transfer(unsigned int len, char type, char incr, char twd_ext, char twd_tcdm, char ele, char ile, char ble, unsigned int ext_addr, unsigned int tcdm_addr, unsigned short int ext_count, unsigned short int ext_stride, unsigned short int tcdm_count, unsigned short int tcdm_stride)
{
 *(volatile int*) PE_MCHAN_COMMAND_QUEUE = len | (type<<PLP_DMA_TYPE_BIT) | ( incr<<PLP_DMA_INCR_BIT) | (twd_ext <<PLP_DMA_2D_BIT) | (ele<<PLP_DMA_ELE_BIT) | (ile <<PLP_DMA_ILE_BIT) | (ble <<PLP_DMA_BLE_BIT)| (twd_tcdm << PLP_DMA_2D_TCDM_BIT);
 *(volatile int*) PE_MCHAN_COMMAND_QUEUE = tcdm_addr;
 *(volatile int*) PE_MCHAN_COMMAND_QUEUE = ext_addr;
 
 if (twd_ext  == 1)
   {
     *(volatile int*) MCHAN_COMMAND_QUEUE = ext_count;
     *(volatile int*) MCHAN_COMMAND_QUEUE = ext_stride;
   }
 
 if (twd_tcdm == 1)
   { 
     *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_count; 
     *(volatile int*) MCHAN_COMMAND_QUEUE = tcdm_stride;
   }
 
}

static inline void fc_mchan_barrier(int id) {
  while(((*(volatile int*)(PE_MCHAN_STATUS_REGISTER)) >> id ) & 0x1 ) {
    if (hal_is_fc()) {
#if PULP_CHIP != CHIP_GAP     
      hal_itc_wait_for_event_noirq(1<<FC_DMA_EVENT);
#else 
      eu_evt_maskWaitAndClr(1<<FC_DMA_EVENT);
#endif      
    }
    else eu_evt_maskWaitAndClr(1<<CL_DMA_EVENT);
  };
}

static inline void fc_mchan_free(int id) {
  *(volatile int*) PE_MCHAN_STATUS_REGISTER = 0x1 << id;
}
#endif
#endif
#endif