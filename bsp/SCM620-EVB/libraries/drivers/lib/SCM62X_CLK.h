#ifndef __SCM62X_CLK_H 
#define __SCM62X_CLK_H 

#include "types.h"
#include "SCM62X.h"

extern uint32_t g_SystemClk;
extern uint32_t g_SystemHclk;
extern uint32_t g_SystemPclk;


// ±÷”‘¥
#define LOSC   0
#define XLOSC  1
#define HOSC   2
#define XHOSC  3
#define PLL    4
#define PLL150M    5
typedef struct
{
    UINT32 refDiv;  //PLL_CTRL_PLL_REFDIV_1~4
    UINT32 fbDiv;   //PLL_CTRL_PLL_FBDIV_(n), n=0x0~0x7f
    UINT32 outDiv;  //PLL_CTRL_PLL_OUTDIV_1~4
    UINT32 refClk;  //PLL_CTRL_PLL_REF_XHOSC/PLL_CTRL_PLL_REF_HOSC/PLL_CTRL_PLL_REF_PAD50M
    UINT32 icpSelA; //PLL_CTRL_PLL_ICPSELA_0~1
    UINT32 icpSelB; //PLL_CTRL_PLL_ICPSELB_0~1
    UINT32 rSel;   //PLL_CTRL_PLL_RSEL_(n)
} PLL_CONFIG ;

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void CLK_EnAllClk(void) ;
void CLK_DisAllPeriClk(void) ;

void CLK_EnPeriClk(UINT8 module) ;
void CLK_DisPeriClk(UINT8 module) ;
void CLK_EnableMatrix(FunctionalState opt) ;
void CLK_EnableHosc(FunctionalState opt) ;
void CLK_EnableLosc(FunctionalState opt) ;
void CLK_EnablePll(FunctionalState opt) ;
void CLK_EnableXhosc(FunctionalState opt) ;
void CLK_EnableXlosc(FunctionalState opt) ;
void CLK_SelectHclk(UINT32 clk) ;
void CLK_SelectPclk(UINT32 clk) ;
void CLK_ConfigAdcPllDiv(UINT32 clk) ;
void CLK_ConfigAdcHoscDiv(UINT32 clk) ;
void CLK_SelectRtcClk(UINT32 clk) ;
void CLK_SelectGpioDbClk(UINT32 clk) ;
void CLK_SelectClkOut(UINT32 clk) ;
void CLK_ConfigPll(PLL_CONFIG pllConfig) ;
void CLK_EnClkOut(FunctionalState opt) ;
void CLK_ConfigMainClk(UINT8 clk) ;


#endif  
