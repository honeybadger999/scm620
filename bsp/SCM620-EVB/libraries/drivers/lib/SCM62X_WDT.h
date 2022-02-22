#ifndef __SCM62X_WDT_H 
#define __SCM62X_WDT_H 

#include "types.h"
#include "SCM62X.h"

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void WDT_Start(UINT32 period, UINT8 wakAble, UINT8 rstAble, UINT8 intEn) ;
void WDT_Disable(void) ;
void WDT_Restart(void) ;
void WDT_StartAsTimer(UINT32 period) ;
void WDT_ClrWakeFlag(void) ;
void WDT_EnableIntr(UINT8 opt) ;


#endif  
