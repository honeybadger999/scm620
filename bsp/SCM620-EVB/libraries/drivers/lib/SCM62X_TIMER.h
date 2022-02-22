#ifndef __SCM62X_TIMER_H 
#define __SCM62X_TIMER_H 

#include "types.h"
#include "SCM62X.h"

#define TC0    0
#define TC1    1
#define TC2    2
#define TC3    3


//������16λ��ʱģʽʱ������
typedef struct 
{
	UINT16 prescale; 
	UINT16 matchVal;
	UINT16 matchCtl; //ƥ�䷢��ʱ�Ƿ��������ֹͣTC
	UINT8 outMode;   //ƥ�䷢��ʱ����ߡ��͵�ƽ������߸ߵ͵�ƽ��ת
	UINT16 outPulseW;	//������
}TC_16BitTimerCFG;

//������16λ����ģʽʱ������
typedef struct 
{
	UINT16 prescale; 
	UINT8 cntSrc; 
	UINT8 cntMode;  //�����ء��½��ء�����/�½���   
	UINT16 matchVal;
	UINT16 matchCtl; //ƥ�䷢��ʱ�Ƿ��������ֹͣTC
	UINT8 outMode;   //ƥ�䷢��ʱ����ߡ��͵�ƽ������߸ߵ͵�ƽ��ת
	UINT16 outPulseW;	//������
}TC_16BitCNTCFG;

//������32λ����ģʽʱ������
typedef struct 
{
	UINT16 prescale; 
	UINT8 cntSrc; 
	UINT8 cntMode;  //�����ء��½��ء�����/�½���   
	UINT16 matchValL;  //��16λƥ��ֵ
	UINT16 matchValH;  //��16λƥ��ֵ
	UINT16 matchCtl; //ƥ�䷢��ʱ�Ƿ��������ֹͣTC
	UINT8 outMode;   //ƥ�䷢��ʱ����ߡ��͵�ƽ������߸ߵ͵�ƽ��ת
	UINT16 outPulseW;	//������
}TC_32BitCNTCFG;

//�����ڲ�׽ģʽʱ������
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
                                                                                                   
