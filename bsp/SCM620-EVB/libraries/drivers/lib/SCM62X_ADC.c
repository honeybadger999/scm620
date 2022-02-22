/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_ADC.c                                                   */
/* 内容摘要: ANA模块基础函数定义                                             */
/* 其它说明: **                                                               */
/* 当前版本: V1.0                                                             */
/* 作    者: SCM                                                              */
/* 完成日期: 2019-03-20                                                       */
/* 修改记录:                                                                  */
/*    修改日期          版本号        修改人        修改内容                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0          SCM        创建文件                     */
/*<FH->************************************************************************/


/******************************************************************************/
/*               #include（依次为标准库头文件、非标准库头文件）               */
/******************************************************************************/
#include "SCM62X.h"

/******************************************************************************/
/*                                外部引用定义                                */
/******************************************************************************/


/******************************************************************************/
/*                                  全局变量                                  */
/******************************************************************************/

/*-----------------------------------------------------------------------------
  Function:		ADC_EnableChnInt                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable channel-change interrupt                                   
 *-----------------------------------------------------------------------------*/
void ADC_EnableChnInt(SGCC_ANACTL_TypeDef *adcCtrl, UINT8 opt)
{
    if(opt == ENABLE)
        adcCtrl->ANA_CFG_REG |= ANA_CFG_REG_ADC_CHN_INTEN;
    else
        adcCtrl->ANA_CFG_REG &= ~ANA_CFG_REG_ADC_CHN_INTEN;   
}

/*-----------------------------------------------------------------------------
  Function:		ADC_EnableCalibInt                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable channel calibration interrupt                                   
 *-----------------------------------------------------------------------------*/
void ADC_EnableCalibInt(SGCC_ANACTL_TypeDef *adcCtrl, UINT8 opt)
{
    if(opt == ENABLE)
        adcCtrl->ANA_CFG_REG |= ANA_CFG_REG_CALIB_INTEN;
    else
        adcCtrl->ANA_CFG_REG &= ~ANA_CFG_REG_CALIB_INTEN;   
}

/*-----------------------------------------------------------------------------
  Function:		ADC_EnableSampInt                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable ADC sampling interrupt                                   
 *-----------------------------------------------------------------------------*/
void ADC_EnableSampInt(SGCC_ANACTL_TypeDef *adcCtrl, UINT8 opt)
{
    if(opt == ENABLE)
        adcCtrl->ANA_CFG_REG |= ANA_CFG_REG_ADC_INTEN;
    else
        adcCtrl->ANA_CFG_REG &= ~ANA_CFG_REG_ADC_INTEN;   
}


/*-----------------------------------------------------------------------------
  Function:		ADC_GetFlagSts                                                                                                                                                                                 
  input:      flag-ANA_STAT_REG_CHS/ANA_STAT_REG_CALIBRE/ANA_STAT_REG_SMP_PULSE/ANA_STAT_REG_CONVSTART/ANA_STAT_REG_RSTN/ANA_STAT_REG_CS/ANA_STAT_REG_CHN_UPDATED/ANA_STAT_REG_CALIB_FAIL
                  /ANA_STAT_REG_ADC_CHN_INT/ANA_STAT_REG_CALIB_INT/ANA_STAT_REG_ADC11_INT~ANA_STAT_REG_ADC0_INT
  Returns:    None                                                                                      
  Description:get status                                    
 *-----------------------------------------------------------------------------*/
UINT8 ADC_GetFlagSts (SGCC_ANACTL_TypeDef *adcCtrl, UINT32 flag)
{
    if(adcCtrl->ANA_STAT_REG & flag)
        return 1;
    else
        return 0;
}

/*-----------------------------------------------------------------------------
  Function:		ADC_ClrIntSts                                                                                                                                                                                 
  input:      flag-ANA_STAT_REG_ADC_CHN_INT/ANA_STAT_REG_CALIB_INT/ANA_STAT_REG_ADC11_INT~ANA_STAT_REG_ADC0_INT
  Returns:    None                                                                                      
  Description:clear interrupt status                                    
 *-----------------------------------------------------------------------------*/
void ADC_ClrIntSts(SGCC_ANACTL_TypeDef *adcCtrl, UINT32 intSts)
{
    adcCtrl->ANA_STAT_REG = intSts;
}

/*-----------------------------------------------------------------------------
  Function:		ADC_GetRdySts                                                                                                                                                                                 
  input:      flag-ADC_RDY_REG_CH0~ADC_RDY_REG_CH11
  Returns:    None                                                                                      
  Description:get ADC data ready status                                    
 *-----------------------------------------------------------------------------*/
UINT8 ADC_GetRdySts (SGCC_ANACTL_TypeDef *adcCtrl, UINT32 flag)
{
    if(adcCtrl->ADC_RDY_REG & flag)
        return 1;
    else
        return 0;
}

/*-----------------------------------------------------------------------------
  Function:		ADC_ConfigTrim                                                                                                                                                                                 
  input:      calibA0 - ADC_TRIM_CALIB_A0_M3~ADC_TRIM_CALIB_A0_M1/ADC_TRIM_CALIB_A0_0/ADC_TRIM_CALIB_A0_P1~ADC_TRIM_CALIB_A0_P4
              calibA1- ADC_TRIM_CALIB_A1_M3~ADC_TRIM_CALIB_A1_M1/ADC_TRIM_CALIB_A1_0/ADC_TRIM_CALIB_A1_P1~ADC_TRIM_CALIB_A1_P4
              cdacTrim - ADC_TRIM_TRIM_(n), n can be 0x0~0x7F
              ib - ADC_TRIM_IB_SMALLI/ADC_TRIM_IB_BIGI
  output:     None
  Returns:    None                                                                                      
  Description:configure ADC trim value                                
 *-----------------------------------------------------------------------------*/
void ADC_ConfigTrim(SGCC_ANACTL_TypeDef *adcCtrl, UINT32 calibA0, UINT32 calibA1, UINT32 cdacTrim, UINT32 ib)
{
	UINT32 temp;
	  
    temp = calibA0 | calibA1 | cdacTrim | ib;
    adcCtrl->ADC_TRIM_REG = temp;
    
}



/*-----------------------------------------------------------------------------
  Function:		ADC_Config                                                                                                                                                                                 
  input:      adcPara
typedef struct
{
    UINT32 smpWidth; //ADC_CON1_SMP_WIDTH_(n)
    UINT32 adcMode;  //ADC_CON1_ADC_MODE_0~ADC_CON1_ADC_MODE_3
    UINT32 trigEn;   //ENABLE/DISABLE
    UINT32 avgTime; //ADC_CON1_AVGTIMES_1/ADC_CON1_AVGTIMES_2/ADC_CON1_AVGTIMES_4/ADC_CON1_AVGTIMES_8
    UINT32 chSel; //ADC_CON3_MCHS_(n)
    UINT32 chCombined; //ADC_CON3_CHNCOMBINED_1/ADC_CON3_CHNCOMBINED_0
} ADC_PARA ;  
  Returns:    None                                                                                      
  Description:configure ADC work in auto mode                                    
 *-----------------------------------------------------------------------------*/
void ADC_Config(SGCC_ANACTL_TypeDef *adcCtrl, ADC_PARA adcPara)
{
    UINT32 temp;
    
    //step 1
    //disable ADC and auto-calibration
    ADC_Enable(adcCtrl, DISABLE);
    ADC_EnableAutoCal(adcCtrl, DISABLE);
    
    //clear all adc interrupt
    ADC_ClrIntSts(adcCtrl, ANA_STAT_REG_ADC_ALLINT);
    
    
    //step 2
    //enable interrupt
    ADC_EnableSampInt(adcCtrl, ENABLE);
    
    
    //step 3
    temp = adcPara.smpWidth | adcPara.adcMode ;
    
    if(adcPara.trigEn == ENABLE)
    {
        temp |= ADC_CON1_TRIG_EN ;
			  temp &= ~ADC_CON1_ADC_TRIGGER_SEL ;
        temp |= adcPara.trigSrc ;
    }
    else
    {
        temp &= ~ADC_CON1_TRIG_EN ;
    }
    
    
       
    if(adcPara.adcMode == ADC_CON1_ADC_MODE_0)
    {
        temp |= adcPara.avgTime;
    }
    
    //config ADC_CON1
    adcCtrl->ADC_CON1_REG = temp;
    
    //step 4, configure ADC_CON2:CS_B==0, RST_N==1，CALIBRE==0
    adcCtrl->ADC_CON2_REG = ADC_CON2_CS_LOW | ADC_CON2_RSTN_HIGH | ADC_CON2_CALIBRE_0;
    
    //step 5, config ADC_CON3
    adcCtrl->ADC_CON3_REG = (adcPara.chSel);  
    if((adcPara.adcMode == ADC_CON1_ADC_MODE_2) || (adcPara.adcMode == ADC_CON1_ADC_MODE_3))
    {
        adcCtrl->ADC_CON3_REG |= adcPara.chCombined;
    }
    
    //step 6, enable ADC
    adcCtrl->ADC_CON4_REG |= ADC_CON4_ADCEN;       
}

/*-----------------------------------------------------------------------------
  Function:		ADC_Reset                                                                                                                                                                                 
  input:      opt  - ADC_CON2_RSTN_LOW / ADC_CON2_RSTN_HIGH
  output:     None
  Returns:    None                                                                                      
  Description:reset ADC or not                                   
 *-----------------------------------------------------------------------------*/
void ADC_Reset(SGCC_ANACTL_TypeDef *adcCtrl, UINT8 opt)
{
    adcCtrl->ADC_CON2_REG &= ~ADC_CON2_RSTN;
    adcCtrl->ADC_CON2_REG |= opt;
   
}

/*-----------------------------------------------------------------------------
  Function:		ADC_SetCs                                                                                                                                                                                 
  input:      opt  - ADC_CON2_CS_LOW / ADC_CON2_CS_HIGH
  output:     None
  Returns:    None                                                                                      
  Description:set cs high or low                                   
 *-----------------------------------------------------------------------------*/
void ADC_SetCs(SGCC_ANACTL_TypeDef *adcCtrl, UINT8 opt)
{
    adcCtrl->ADC_CON2_REG &= ~ADC_CON2_CS;  
    adcCtrl->ADC_CON2_REG |= opt;
   
}


/*-----------------------------------------------------------------------------
  Function:		ADC_SelectCh                                                                                                                                                                                 
  input:      chNo - ADC_CON3_MCHS_(n)
  output:     None
  Returns:    None                                                                                      
  Description:select ADC channel                                   
 *-----------------------------------------------------------------------------*/
void ADC_SelectCh(SGCC_ANACTL_TypeDef *adcCtrl, UINT32 chNo)
{
    adcCtrl->ADC_CON3_REG = chNo;  
}

/*-----------------------------------------------------------------------------
  Function:		ADC_GetData                                                                                                                                                                                 
  input:      chNo - 0~11
  output:     None
  Returns:    None                                                                                      
  Description:read ADC data                                    
 *-----------------------------------------------------------------------------*/
UINT32 ADC_GetData(SGCC_ANACTL_TypeDef *adcCtrl, UINT8 chNo)
{
    UINT32 *ptr;
    ptr = (UINT32*)((UINT32)adcCtrl + 0x80 + 4 * chNo);
 
    return (*ptr); 
}


/*-----------------------------------------------------------------------------
  Function:		ADC_Enable                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:disable ADC                                    
 *-----------------------------------------------------------------------------*/
void ADC_Enable(SGCC_ANACTL_TypeDef *adcCtrl, UINT8 opt)
{
    if(opt == ENABLE)
        adcCtrl->ADC_CON4_REG |= ADC_CON4_ADCEN;
    else
        adcCtrl->ADC_CON4_REG &= ~ADC_CON4_ADCEN;   
}

/*-----------------------------------------------------------------------------
  Function:		ADC_EnableAutoCal                                                                                                                                                                                 
  input:      opt -- ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable ADC auto calibration                                  
 *-----------------------------------------------------------------------------*/
void ADC_EnableAutoCal(SGCC_ANACTL_TypeDef *adcCtrl, UINT8 opt)
{
    if(opt == ENABLE)
        adcCtrl->ADC_CON4_REG |= ADC_CON4_CALIB_EN;
    else
        adcCtrl->ADC_CON4_REG &= ~ADC_CON4_CALIB_EN;   
}




/*-----------------------------------------------------------------------------
  Function:		ADC_AutoCal                                                                                                                                                                                 
  input:      ib -- ADC_TRIM_IB_SMALLI/ADC_TRIM_IB_BIGI
              a0 -- ADC_TRIM_CALIB_A0_M3 /ADC_TRIM_CALIB_A0_M2 /ADC_TRIM_CALIB_A0_M1 /ADC_TRIM_CALIB_A0_0  /ADC_TRIM_CALIB_A0_P1        
                     /ADC_TRIM_CALIB_A0_P2 / ADC_TRIM_CALIB_A0_P3 /ADC_TRIM_CALIB_A0_P4 
              a1 -- ADC_TRIM_CALIB_A1_M3 /  ADC_TRIM_CALIB_A1_M2    /ADC_TRIM_CALIB_A1_M1 /ADC_TRIM_CALIB_A1_0 /ADC_TRIM_CALIB_A1_P1 
                    /ADC_TRIM_CALIB_A1_P2/ ADC_TRIM_CALIB_A1_P3 /ADC_TRIM_CALIB_A1_P4      
  output:     None
  Returns:    None                                                                                      
  Description:enable or disable ADC auto calibration                                  
 *-----------------------------------------------------------------------------*/
void ADC_AutoCal(SGCC_ANACTL_TypeDef *adcCtrl,UINT32 ib, UINT32 a0,  UINT32 a1)
{
    UINT16 i;
    adcCtrl->ADC_TRIM_REG &= ~(ADC_TRIM_CALIB_A1 | ADC_TRIM_CALIB_A0 | ADC_TRIM_IB);
    adcCtrl->ADC_TRIM_REG |= (ib | a0 | a1);
    adcCtrl->ADC_CON2_REG = 0x6;
    
    for(i = 0; i < 0x1000; i++);
    adcCtrl->ADC_CON4_REG |= ADC_CON4_CALIB_EN;
    
    
    //wait
    while(!ADC_GetFlagSts (adcCtrl, ANA_STAT_REG_CALIB_INT));
    ADC_ClrIntSts(adcCtrl, ANA_STAT_REG_CALIB_INT);
}


/*-----------------------------------------------------------------------------
  Function:		ADC_DisableAutoCal                                                                                                                                                                                 
  input:      opt --  ENABLE/DISABLE
  output:     None
  Returns:    None                                                                                      
  Description:disable ADC auto calibration                                  
 *-----------------------------------------------------------------------------*/
void ADC_DisableAutoCal(SGCC_ANACTL_TypeDef *adcCtrl)
{
    adcCtrl->ADC_CON4_REG &= ~ADC_CON4_CALIB_EN;
    adcCtrl->ADC_CON2_REG = 0x1;
}
