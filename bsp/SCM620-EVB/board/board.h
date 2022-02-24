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
extern rt_mailbox_t mb_uart0;
extern rt_sem_t sem_com_pcs ;
extern rt_sem_t sem_com_dc ;

void SystemClock_Config(void);
void rt_hw_board_init(void);
void SysTick_Handler(void);

extern struct rt_event com_event;
#ifdef __cplusplus
}
#endif

#endif

