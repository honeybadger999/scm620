#ifndef __SCM62X_SCU_H 
#define __SCM62X_SCU_H 

#include "types.h"
#include "SCM62X.h"



// ±÷”‘¥
#define RCLF  0
#define XTLF  1
#define RCHF  2
#define XTHF  3

/*interrupt source*/ 
#define SCU_INT_SRC_WAKEUP       (1 << 3)        
#define SCU_INT_SRC_VD           (1 << 2)        
#define SCU_INT_SRC_XLFDT        (1 << 1)        
#define SCU_INT_SRC_XHFDT        (1 << 0)        



/*wake source*/        
#define WAKEUP_SRC_NONE                 (0)
#define WAKEUP_SRC_OTG_SUP              (1 << 16)
#define WAKEUP_SRC_UART0_RX             (1 << 15)
#define WAKEUP_SRC_M4_SW                (1 << 14)
#define WAKEUP_SRC_WDT_INT              (1 << 13)
#define WAKEUP_SRC_TIMERA0_INT          (1 << 12)
#define WAKEUP_SRC_RTC_PRD              (1 << 11)
#define WAKEUP_SRC_RTC_ALM              (1 << 10)
#define WAKEUP_SRC_GPIO9              (1 << 9)
#define WAKEUP_SRC_GPIO8             (1 << 8)
#define WAKEUP_SRC_GPIO7              (1 << 7)
#define WAKEUP_SRC_GPIO6             (1 << 6)
#define WAKEUP_SRC_GPIO5              (1 << 5)
#define WAKEUP_SRC_GPIO4             (1 << 4)
#define WAKEUP_SRC_GPIO3              (1 << 3)
#define WAKEUP_SRC_GPIO2            (1 << 2)
#define WAKEUP_SRC_GPIO1              (1 << 1)
#define WAKEUP_SRC_GPIO0            (1 << 0)
  

/*sleep mode*/
#define SLEEP_MODE       0
#define DEEP_SLEEP_MODE  1


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void SCU_EnableXhfdt(FunctionalState opt) ;
void SCU_EnableVD(FunctionalState opt) ;
void SCU_ConfigGmacMode(UINT8 macMode) ;
void SCU_EnFlashStdby(UINT8 opt) ;
void SCU_EnableWakeSrc(UINT32 src) ;
void SCU_DisableWakeSrc(UINT32 src); 
void SCU_EnableIntr(UINT32 intrType) ;
void SCU_DisableIntr(UINT32 intrType) ;
UINT8 SCU_GetIntrSts(UINT32 intrType) ;
void SCU_ClearIntr(UINT32 intrType) ;
UINT8 SCU_GetWakeEnSts(UINT32 src) ;
void SCU_ClearSysSts(UINT32 stsType) ;

void SCU_EnTrngSamp(void) ;
UINT32 SCU_RdTrngData(void) ;

#endif  
