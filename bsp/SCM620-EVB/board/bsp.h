#ifndef __BSP_H__
#define __BSP_H__

#include "SCM62X.h"
#include <rtthread.h>

extern SGCC_UART_TypeDef*  PrintPort;

#define LED0   38
#define LED1   39
#define LED2   40
#define LED3   45

void LED_GPIO_Init(void);
void UART_Config_Init(void);
char rt_hw_console_getchar(void);
	
#endif
