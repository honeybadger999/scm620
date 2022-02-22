/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_AES.c                                                   */
/* 内容摘要: AES模块基础函数定义                                             */
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
  Function:		AES_Start                                                                                                                                                                           
  input:     None	
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: start AES or not                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void AES_Start() 
{
    SGCC_AES_P->CNTRL |= AES_CNTRL_START;   
}

/*-----------------------------------------------------------------------------
  Function:		AES_WaitStop                                                                                                                                                                           
  input:     None	
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: wait start bit to be 0                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void AES_WaitStop() 
{
    while(SGCC_AES_P->CNTRL & AES_CNTRL_START);
}

/*-----------------------------------------------------------------------------
  Function:		AES_SelFunc                                                                                                                                                                          
  input:      func- AES_CNTRL_FUNC_SEL_ENC/AES_CNTRL_FUNC_SEL_STDEC/AES_CNTRL_FUNC_SEL_KEYEXP/AES_CNTRL_FUNC_SEL_SIMDEC
  output:      None                                                                                    
  Returns:     None                                                                                       
  Description: select AES functions                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void AES_SelFunc(UINT32 func) 
{
    SGCC_AES_P->CNTRL &= ~AES_CNTRL_FUNC_SEL;
    SGCC_AES_P->CNTRL |= func;
}

/*-----------------------------------------------------------------------------
  Function:		AES_SelKey                                                                                                                                                                          
  input:     key- AES_CNTRL_KEY_SEL_128B/AES_CNTRL_KEY_SEL_192B/AES_CNTRL_KEY_SEL_256B
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: select key                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void AES_SelKey(UINT32 key) 
{
    SGCC_AES_P->CNTRL &= ~AES_CNTRL_KEY_SEL;
    SGCC_AES_P->CNTRL |= key;
}

/*-----------------------------------------------------------------------------
  Function:		 AES_EnableRand                                                                                                                                                                         
  input:       opt- ENABLE or DISABLE	
  output:      None                                                                                    
  Returns:     None                                                                                       
  Description: enable or disable rand                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void AES_EnableRand(UINT8 opt) 
{
    if(opt == ENABLE)
    {
	    SGCC_AES_P->CNTRL |= AES_CNTRL_RAND_EN;
    }
    else
    {
        SGCC_AES_P->CNTRL &= ~AES_CNTRL_RAND_EN;
    }
}

/*-----------------------------------------------------------------------------
  Function:		 AES_EnableStep                                                                                                                                                                          
  input:       opt- ENABLE or DISABLE                                                                             
  Returns:     None                                                                                       
  Description: enable step or not                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void AES_EnableStep(UINT8 opt) 
{
    if(opt == ENABLE)
    {
	    SGCC_AES_P->CNTRL |= SM4_IER_DONE;
    }
    else
    {
        SGCC_AES_P->CNTRL &= ~SM4_IER_DONE;
    } 
}

/*-----------------------------------------------------------------------------
  Function:		 AES_ConfigKey                                                                                                                                                                           
  input:       key
               keyLen  
  output:      None                                                                                    
  Returns:     None                                                                                       
  Description: configure key                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void AES_ConfigKey(UINT32 *key, UINT16 keyLen) 
{
    UINT8 i;
    for(i = 0; i < keyLen/32; i++)
    {
        SGCC_AES_P->KEY[i] = key[i];     
    }
}

/*-----------------------------------------------------------------------------
  Function:		AES_ConfigRand                                                                                                                                                                          
  input:      None                    	  
  output:     None                                                                                    
  Returns:    None                                                                                       
  Description: configure rand                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void AES_ConfigRand(UINT32 *rand) 
{
    UINT8 i;
    for(i = 0; i < 2; i++)
    {
        SGCC_AES_P->RAND[i] = rand[i];
      
    }
}

/*-----------------------------------------------------------------------------
  Function:		 AES_WriteData                                                                                                                                                                          
  input:       inputData                    	  
  output:      None                                                                                    
  Returns:     None                                                                                       
  Description: input data                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void AES_WriteData(UINT32 *inputData  ) 
{
    UINT8 i;
    
    for(i = 0; i < 4; i++)
    {
        SGCC_AES_P->DATA[i] = inputData[i];
    }    
}

/*-----------------------------------------------------------------------------
  Function:		 AES_ReadData                                                                                                                                                                          
  input:       None                    	  
  output:      None                                                                                    
  Returns:     None                                                                                       
  Description: read data                                                                                                  	                                   
 *-----------------------------------------------------------------------------*/
void AES_ReadData(UINT32 *rdData) 
{
    UINT8 i;
    
    for(i = 0; i < 4; i++)
    {
        rdData[i] = SGCC_AES_P->DATA[i];
    }
}

