#ifndef __SCM62X_PWM_H 
#define __SCM62X_PWM_H 

#include "types.h"
#include "SCM62X.h"
/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
extern void PWM_Start(SGCC_PWM_TypeDef* pwmx, uint8_t period, uint8_t width, uint8_t div, uint8_t outRev) ;

#endif  
                                                                                                   
