
#ifndef _SCM62X_DRIVERS_H_
#define _SCM62X_DRIVERS_H_


#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "types.h"
#include "SCM62X_REGBIT.h"
#include "SCM62X_IOCTRL.h"
#include "SCM62X_CLK.h"
#include "SCM62X_TIMER.h"
#include "SCM62X_UART.h"
#include "SCM62X_RTC.h"
#include "SCM62X_DMA.h"
#include "SCM62X_SPI.h"
#include "SCM62X_WDT.h"
#include "SCM62X_FLASH.h"
#include "SCM62X_SCU.h"
#include "SCM62X_I2C.h"
#include "SCM62X_GPIO.h"
#include "SCM62X_PWM.h"
#include "SCM62X_CAN.h"
#include "SCM62X_CRC.h"
#include "SCM62X_7816.h"
#include "SCM62X_SM4.h"
#include "SCM62X_AES.h"
#include "SCM62X_IOCTRL.h"
#include "SCM62X_NFC.h"
#include "SCM62X_ANACTRL.h"
extern SGCC_UART_TypeDef*  PrintPort;
#define PCLK  40000  //40000以K（兆）为单位

//for modules
#define DMA0    0
#define DMA1    1
#define DMA2    2
#define DMA3    3
#define GMAC0   4
#define GMAC1   5
#define OTG     6
#define FLASH   7
#define U7816   8
#define PWMA    9
#define UART0   10
#define UART1   11
#define UART2   12
#define UART3   13
#define I2C0    14
#define I2C1    15
#define SPI0    16
#define SPI1    17
#define GPIO    18
#define RTC     19
#define TIMERA  20
#define TIMERB  21
#define WDT     22
#define ANACTRL0  23
#define PWMB     24
#define UART4    25
#define UART5    26
#define UART6    27
#define UART7    28
#define I2C2     29
#define SPI2     30
#define SPI3     31
#define CRC      32
#define TIMERC   33
#define TIMERD   34
#define CAN0     35
#define CAN1     36
#define CAN2     37
#define ANACTRL1  38
#define ANACTRL2  39
#define SM4      40
#define AES      41
#define NFC      42
#define XSRAM    43
#define ADC0     44
#define ADC1     45
#define ADC2     46



/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */


#endif 
