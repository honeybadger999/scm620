/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-5      SummerGift   first version
 */

#ifndef __BOARD_H__
#define __BOARD_H__

#include <rtthread.h>

#include "SCM62X.h"
#include "bsp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SCM630_FLASH_START_ADRESS     ((uint32_t)0x00000000)
#define SCM630_FLASH_SIZE             (2048 * 1024)
#define SCM630_FLASH_END_ADDRESS      ((uint32_t)(SCM630_FLASH_START_ADRESS + SCM630_FLASH_SIZE))

#define SCM630_SRAM_SIZE           512
#define SCM630_SRAM_END            (0x20000000 + SCM630_SRAM_SIZE * 1024)

#if defined(__CC_ARM) || defined(__CLANG_ARM)
extern int Image$$RW_IRAM1$$ZI$$Limit;
#define HEAP_BEGIN      (&Image$$RW_IRAM1$$ZI$$Limit)
#elif __ICCARM__
#pragma section="CSTACK"
#define HEAP_BEGIN      (__segment_end("CSTACK"))
#else
extern int __bss_end;
#define HEAP_BEGIN      (&__bss_end)
#endif

#define HEAP_END        SCM630_SRAM_END


#define EVENT_FLAG_PS_UART0 (1 << 0)
#define EVENT_FLAG_RV_UART0 (1 << 1)
#define EVENT_FLAG_PS_UART3 (1 << 2)
#define EVENT_FLAG_RV_UART3 (1 << 3)
#define EVENT_FLAG_PS_UART7 (1 << 4)
#define EVENT_FLAG_RV_UART7 (1 << 5)
#define EVENT_FLAG_RTC_WRITE (1 << 6)

extern rt_mailbox_t mb_uart0;
extern rt_sem_t sem_com_pcs ;
extern rt_sem_t sem_com_dc ;
extern rt_sem_t sem_com_meter ;
extern rt_sem_t sem_com_pv ;
extern rt_mq_t mq_can0;
extern rt_mq_t mq_can1_rev;
extern rt_mq_t mq_can1_send;

extern void rtc_thread_entry(void *parameter);
extern void flash_thread_entry(void *parameter);
extern void can0_rev_thread_entry(void *parameter);
extern void can1_rev_thread_entry(void *parameter);
extern void can1_send_thread_entry(void *parameter);
extern void uart7_thread_entry(void* parameter);
extern void uart0_rev_thread_entry(void* parameter);
extern void uart0_read_thread_entry(void* parameter);
extern void Uart0_MSG_thread_entry(void* parameter);
extern struct rt_thread uart7_thread;

extern void uart3_rev_thread_entry(void* parameter);
extern void uart3_read_thread_entry(void *parameter);

void SystemClock_Config(void);
void rt_hw_board_init(void);
void SysTick_Handler(void);
extern void InitGlobalVariables(void);

extern struct rt_event com_event;
#ifdef __cplusplus
}
#endif

#endif

