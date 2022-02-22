#ifndef __SCM62X_GPIO_H 
#define __SCM62X_GPIO_H 

#include "types.h"

//GPIO0 port number
#define GPIO0_PORTNUM 32

/*direction*/
#define GPIO_INPUT 0
#define GPIO_OUTPUT 1

/*high or low level*/
#define GPIO_HIGH  1
#define GPIO_LOW   0


/*interrupt mask or unmask*/
#define GPIO_INTR_MASK    1
#define GPIO_INTR_UNMASK  0

/*interrupt trigger edge*/
#define GPIO_INTR_HIGHLVEL  0
#define GPIO_INTR_LOWLEVEL  1
#define GPIO_INTR_RISINGEDGE  2
#define GPIO_INTR_FALLINGEDGE  3
#define GPIO_INTR_BOTHEDGE  4


/*debounce mode*/
#define GPIO_DEBOUNCE_NONE  0
#define GPIO_DEBOUNCE_HIGHPULSE  1
#define GPIO_DEBOUNCE_LOWPULSE  2
#define GPIO_DEBOUNCE_BOTHPULSE  3


/*debounce cycle*/
#define GPIO_DEBOUNCE_CYCLE_1  0
#define GPIO_DEBOUNCE_CYCLE_2  1
#define GPIO_DEBOUNCE_CYCLE_3  2
#define GPIO_DEBOUNCE_CYCLE_4  3



/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void GPIO_RestoreVal(void) ;
void GPIO_ConfigDir(UINT8 portNum, UINT8 dir) ;
void GPIO_WrDataOut(UINT8 portNum, UINT8 level) ;
UINT8 GPIO_RdDataIn(UINT8 portNum) ;
void GPIO_EnIntr(UINT8 portNum, UINT8 opt) ;
void GPIO_MaskIntr(UINT8 portNum, UINT8 intrMask) ;
void GPIO_ClrIntr(UINT8 portNum) ;
void GPIO_ConfigIntrMode(UINT8 portNum, UINT8 trigMode) ;
void GPIO_ConfigDebounce(UINT8 portNum, UINT8 debounceMode, UINT8 debounceCycle) ;
void GPIO_EnSyn(UINT8 portNum, UINT8 synEn) ;




#endif   
                                                                                                   
