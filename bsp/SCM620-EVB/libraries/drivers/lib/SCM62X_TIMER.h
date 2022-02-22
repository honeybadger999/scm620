#ifndef __SCM62X_TIMER_H 
#define __SCM62X_TIMER_H 

#include "types.h"
#include "SCM62X.h"

#define TC0    0
#define TC1    1
#define TC2    2
#define TC3    3


//工作于16位定时模式时的配置
typedef struct 
{
	UINT16 prescale; 
	UINT16 matchVal;
	UINT16 matchCtl; //匹配发生时是否清零或者停止TC
	UINT8 outMode;   //匹配发生时输出高、低电平脉冲或者高低电平翻转
	UINT16 outPulseW;	//脉冲宽度
}TC_16BitTimerCFG;

//工作于16位计数模式时的配置
typedef struct 
{
	UINT16 prescale; 
	UINT8 cntSrc; 
	UINT8 cntMode;  //上升沿、下降沿、上升/下降沿   
	UINT16 matchVal;
	UINT16 matchCtl; //匹配发生时是否清零或者停止TC
	UINT8 outMode;   //匹配发生时输出高、低电平脉冲或者高低电平翻转
	UINT16 outPulseW;	//脉冲宽度
}TC_16BitCNTCFG;

//工作于32位计数模式时的配置
typedef struct 
{
	UINT16 prescale; 
	UINT8 cntSrc; 
	UINT8 cntMode;  //上升沿、下降沿、上升/下降沿   
	UINT16 matchValL;  //低16位匹配值
	UINT16 matchValH;  //高16位匹配值
	UINT16 matchCtl; //匹配发生时是否清零或者停止TC
	UINT8 outMode;   //匹配发生时输出高、低电平脉冲或者高低电平翻转
	UINT16 outPulseW;	//脉冲宽度
}TC_32BitCNTCFG;

//工作于捕捉模式时的配置
typedef struct 
{
	UINT16 prescale; 
	UINT16 capSrc;    
	UINT16 capEdge;
}TC_CAPTCFG;


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
extern void TC_RestoreReg(SGCC_TC_TypeDef *TCx);
extern void TC_Enable(SGCC_TC_TypeDef *TCx, UINT8 opt) ;
extern void TC_EnableIntr(SGCC_TC_TypeDef *TCx, UINT8 intrType, UINT8 opt) ;
extern void TC_ClearIntr(SGCC_TC_TypeDef *TCx, UINT8 intrType) ;

extern UINT8 TC_GetIntrStatus(SGCC_TC_TypeDef *TCx, uint16_t flag);

extern void TC_Timer16Bit(SGCC_TC_TypeDef *TCx, BOOL matchEn, TC_16BitTimerCFG tcCfg, BOOL interEn) ;

extern void TC_Timer32Bit(SGCC_TC_TypeDef *TCx, UINT16 prescale, BOOL matchEn, UINT16 matchValL, UINT16 matchValH) ;

extern void TC_Counter16Bit(SGCC_TC_TypeDef *TCx, BOOL matchEn, TC_16BitCNTCFG timerCfg, BOOL interEn)  ;
extern void TC_Counter32Bit(SGCC_TC_TypeDef *TCx, TC_32BitCNTCFG tcCfg) ;

extern void TC_Capture16Bit(SGCC_TC_TypeDef *TCx, TC_CAPTCFG tcCfg, BOOL interEn) ;
extern void TC_Capture32Bit(SGCC_TC_TypeDef *TCx, TC_CAPTCFG tcCfg) ;

extern void Delay( UINT16 time );





#endif 
                                                                                                   
