/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_SM4.c                                                     */
/* 内容摘要: SM4模块基础函数定义                                              */
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
  Function:		SM4_RstKey                                                                                                                                                                           
  input:    opt- ENABLE or DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: clear or save key after calculation                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void SM4_RstKey(UINT8 opt) 
{
    if(opt == ENABLE)
    {
	    SGCC_SM4_P->CR |= SM4_CR_KEY_RST;
    }
    else
    {
        SGCC_SM4_P->CR &= ~SM4_CR_KEY_RST;
    }

}

/*-----------------------------------------------------------------------------
  Function:		SM4_RstIV                                                                                                                                                                           
  input:    opt- ENABLE or DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: use the current of initial IV                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void SM4_RstIV(UINT8 opt) 
{
    if(opt == ENABLE)
    {
	    SGCC_SM4_P->CR &= ~SM4_CR_IV_RST;
    }
    else
    {
        SGCC_SM4_P->CR |= SM4_CR_IV_RST;
    }
}


/*-----------------------------------------------------------------------------
  Function:		SM4_Start                                                                                                                                                                           
  input:     None	
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: start SM4                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void SM4_Start() 
{
    SGCC_SM4_P->CR |= SM4_CR_AE;
}

/*-----------------------------------------------------------------------------
  Function:		SM4_Enable                                                                                                                                                                          
  input:     opt- ENABLE or DISABLE	
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: enable or disable SM4                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void SM4_Enable(UINT8 opt) 
{
    if(opt == ENABLE)
    {
	    SGCC_SM4_P->CR |= SM4_CR_CS;
    }
    else
    {
        SGCC_SM4_P->CR &= ~SM4_CR_CS;
    }
}

/*-----------------------------------------------------------------------------
  Function:		SM4_SelEncMode                                                                                                                                                                          
  input:     mode- SM4_MR_ENC_SEL_ENCRYPT/SM4_MR_ENC_SEL_DECRYPT
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: select encrypt or deencrypt mode                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void SM4_SelEncMode(UINT32 mode) 
{
    SGCC_SM4_P->MR &= ~SM4_MR_ENC_SEL;
    SGCC_SM4_P->MR |= mode;
}

/*-----------------------------------------------------------------------------
  Function:		SM4_SelMode                                                                                                                                                                          
  input:     mode- SM4_MR_WM_SEL_ECB/SM4_MR_WM_SEL_CBC
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: select CBC or ECB mode                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void SM4_SelMode(UINT32 mode) 
{
    SGCC_SM4_P->MR &= ~SM4_MR_WM_SEL;
    SGCC_SM4_P->MR |= mode;
}

/*-----------------------------------------------------------------------------
  Function:		SM4_ConfigDPAWR                                                                                                                                                                          
  input:     dpawr- SM4_MR_DPAWR_25_PER/SM4_MR_DPAWR_50_PER/SM4_MR_DPAWR_75_PER
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: configure DPAWR                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void SM4_ConfigDPAWR(UINT32 dpawr) 
{
    SGCC_SM4_P->MR &= ~SM4_MR_DPAWR;
    SGCC_SM4_P->MR |= dpawr;
}

/*-----------------------------------------------------------------------------
  Function:		SM4_EnableDpaw                                                                                                                                                                          
  input:     opt- ENABLE or DISABLE	
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: enable or disable DPAW                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void SM4_EnableDpaw(UINT8 opt) 
{
    if(opt == ENABLE)
    {
	    SGCC_SM4_P->MR |= SM4_MR_DPAW;
    }
    else
    {
        SGCC_SM4_P->MR &= ~SM4_MR_DPAW;
    }
}

/*-----------------------------------------------------------------------------
  Function:		SM4_EnableIntr                                                                                                                                                                           
  input:    opt- ENABLE or DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: enable interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void SM4_EnableIntr(UINT8 opt) 
{
    if(opt == ENABLE)
    {
	    SGCC_SM4_P->IER |= SM4_IER_DONE;
    }
    else
    {
        SGCC_SM4_P->IER &= ~SM4_IER_DONE;
    }
	 
	
}

/*-----------------------------------------------------------------------------
  Function:		SM4_ClearIntr                                                                                                                                                                           
  input:      None                    	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: clear interrupt                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void SM4_ClearIntr() 
{
    SGCC_SM4_P->SR = 0;
}

/*-----------------------------------------------------------------------------
  Function:		SM4_GetStatus                                                                       
  input:    None     
  Returns:  None                                                                                        
  Description:  get SM4 status                                                                                         
               	                                  
 *-----------------------------------------------------------------------------*/
UINT8 SM4_GetStatus()
{
    if(SGCC_SM4_P->SR & SM4_SR_DONE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

/*-----------------------------------------------------------------------------
  Function:		SM4_ConfigKey                                                                                                                                                                           
  input:      None                    	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: configure key                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void SM4_ConfigKey(UINT32 k1, UINT32 k2, UINT32 k3, UINT32 k4) 
{
    SGCC_SM4_P->K1 = k1;
    SGCC_SM4_P->K2 = k2;
    SGCC_SM4_P->K3 = k3;
    SGCC_SM4_P->K4 = k4;
}

/*-----------------------------------------------------------------------------
  Function:		SM4_ConfigIV                                                                                                                                                                          
  input:      None                    	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: configure IV                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void SM4_ConfigIV(UINT32 iv1, UINT32 iv2, UINT32 iv3, UINT32 iv4) 
{
    SGCC_SM4_P->IV1 = iv1;
    SGCC_SM4_P->IV2 = iv2;
    SGCC_SM4_P->IV3 = iv3;
    SGCC_SM4_P->IV4 = iv4;
}

/*-----------------------------------------------------------------------------
  Function:		SM4_WriteData                                                                                                                                                                          
  input:      None                    	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: input data                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void SM4_WriteData(UINT32 d1, UINT32 d2, UINT32 d3, UINT32 d4) 
{
    SGCC_SM4_P->DR1 = d1;
    SGCC_SM4_P->DR2 = d2;
    SGCC_SM4_P->DR3 = d3;
    SGCC_SM4_P->DR4 = d4;
}

/*-----------------------------------------------------------------------------
  Function:		SM4_ReadData                                                                                                                                                                          
  input:      None                    	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: read data                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void SM4_ReadData(UINT32 *rdData) 
{
    rdData[0] = SGCC_SM4_P->DR1 ;
    rdData[1] = SGCC_SM4_P->DR2 ;
    rdData[2] = SGCC_SM4_P->DR3 ;
    rdData[3] = SGCC_SM4_P->DR4 ;
}

