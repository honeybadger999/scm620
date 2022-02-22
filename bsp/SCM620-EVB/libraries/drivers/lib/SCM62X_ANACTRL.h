#ifndef __SCM62X_ANACTRL_H__
#define __SCM62X_ANACTRL_H__

#include "types.h"
#include "SCM62X.h"


//typedef struct
//{
//	UINT32 smpWidth; //ADC_CON1_SMP_WIDTH_(n)
//    UINT32 adcMode;  //ADC_CON1_ADC_MODE_0~ADC_CON1_ADC_MODE_3
//    UINT32 trigEn;   //ENABLE/DISABLE
//    UINT32 trigSrc;   //trigSrc
//    UINT32 avgTime; //ADC_CON1_AVGTIMES_1/ADC_CON1_AVGTIMES_2/ADC_CON1_AVGTIMES_4/ADC_CON1_AVGTIMES_8
//    UINT32 chSel; //ADC_CON3_MCHS_(n)
//    UINT32 chCombined; //ADC_CON3_CHNCOMBINED_1/ADC_CON3_CHNCOMBINED_0
//} ADC_PARA ;

void ADC_EnableChnInt(SGCC_ANACTL_TypeDef *anactl, UINT8 opt);
void ADC_EnableCalibInt(SGCC_ANACTL_TypeDef *anactl, UINT8 opt);
void ADC_EnableSampInt(SGCC_ANACTL_TypeDef *anactl, UINT8 opt);
UINT8 ADC_GetFlagSts (SGCC_ANACTL_TypeDef *anactl, UINT32 flag);
void ADC_ClrIntSts(SGCC_ANACTL_TypeDef *anactl, UINT32 intSts);
UINT8 ADC_GetRdySts (SGCC_ANACTL_TypeDef *anactl, UINT32 flag);
void ADC_ConfigTrim(SGCC_ANACTL_TypeDef *anactl, UINT32 calibA0, UINT32 calibA1, UINT32 cdacTrim, UINT32 ib);
void ADC_Config(SGCC_ANACTL_TypeDef *anactl, ADC_PARA adcPara);
void ADC_SelectCh(SGCC_ANACTL_TypeDef *anactl, UINT32 chNo);
UINT32 ADC_GetData(SGCC_ANACTL_TypeDef *anactl, UINT8 chNo);
void ADC_Enable(SGCC_ANACTL_TypeDef *anactl, UINT8 opt);
void ADC_EnableAutoCal(SGCC_ANACTL_TypeDef *anactl, UINT8 opt);
void ADC_AutoCal(SGCC_ANACTL_TypeDef *adcCtrl,UINT32 ib, UINT32 a0,  UINT32 a1);
void ADC_DisableAutoCal(SGCC_ANACTL_TypeDef *adcCtrl);

void ADC_Reset(SGCC_ANACTL_TypeDef *anactl, UINT8 opt);
void ADC_SetCs(SGCC_ANACTL_TypeDef *anactl, UINT8 opt);
void TRNG_CfgFopt(UINT32 opt);
void TRNG_CfgSopt(UINT32 opt);
void TRNG_SelF(UINT32 opt);
void TRNG_EnSamp(FunctionalState opt);


#endif   
