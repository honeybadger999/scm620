/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_SCU.c                                                     */
/* 内容摘要: SCU模块基础函数定义                                              */
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
  Function:		SCU_EnableXhfdt                                                                                                                                                                          
  input:      opt -- ENABLE/DISABLE	
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XHOSC stop detection                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void SCU_EnableXhfdt(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_XHFDT_EN;
    }
    else if(opt == DISABLE)
    {
        SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_XHFDT_EN;
    }
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnableXlfdt                                                                                                                                                                          
  input:      opt -- ENABLE/DISABLE	
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XLOSC stop detection                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void SCU_EnableXlfdt(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_XLFDT_EN;
    }
    else if(opt == DISABLE)
    {
        SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_XLFDT_EN;
    }
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnableVD                                                                                                                                                                          
  input:      opt -- ENABLE/DISABLE	
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable VD                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void SCU_EnableVD(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_VD_EN;
    }
    else if(opt == DISABLE)
    {
        SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_VD_EN;
    }
}

/*-----------------------------------------------------------------------------
  Function:		SCU_ConfigGmacMode                                                                                                                                                                          
  input:      macMode -- configure GMAC mode,
                        SYS_CTRL_GMAC_MODE_MII / SYS_CTRL_GMAC_MODE_RMII
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: configure GMAC mode                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void SCU_ConfigGmacMode(UINT8 macMode) 
{
    SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_GMAC_MODE;
    SGCC_SCU_P->SYS_CTRL |= macMode;
     
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnFlashStdby                                                                                                                                                                          
  input:      opt -- ENABLE or DISABLE,
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable flash standby mode                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void SCU_EnFlashStdby(UINT8 opt) 
{
    if(opt == ENABLE)
    {
        SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_FLASH_STDBY_EN;
    }
    else if(opt == DISABLE)
    {
        SGCC_SCU_P->SYS_CTRL &= ~SYS_CTRL_FLASH_STDBY_EN;
    }
     
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnableWakeSrc                                                                                                                                                                          
  input:     	src:  WAKEUP_SRC_OTG_SUP/WAKEUP_SRC_UART0_RX/  WAKEUP_SRC_M4_SW  /            
                     WAKEUP_SRC_WDT_INT/ WAKEUP_SRC_TIMERA1_INT/WAKEUP_SRC_RTC_PRD /            
                    WAKEUP_SRC_RTC_ALM / WAKEUP_SRC_GPIO9 /  WAKEUP_SRC_GPIO8/            
                    WAKEUP_SRC_GPIO7~WAKEUP_SRC_GPIO0
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable  wakeup source                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void SCU_EnableWakeSrc(UINT32 src) 
{
   SGCC_SCU_P->WAKEUP_EN = src;
}

/*-----------------------------------------------------------------------------
  Function:		SCU_DisableWakeSrc                                                                                                                                                                          
  input:     	src:  WAKEUP_SRC_OTG_SUP/WAKEUP_SRC_UART0_RX/  WAKEUP_SRC_M4_SW  /            
                     WAKEUP_SRC_WDT_INT/ WAKEUP_SRC_TIMERA1_INT/WAKEUP_SRC_RTC_PRD /            
                    WAKEUP_SRC_RTC_ALM / WAKEUP_SRC_GPIO9 /  WAKEUP_SRC_GPIO8/            
                    WAKEUP_SRC_GPIO7~WAKEUP_SRC_GPIO0
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable  wakeup source                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void SCU_DisableWakeSrc(UINT32 src) 
{
   SGCC_SCU_P->WAKEUP_EN &= ~(src);
}

/*-----------------------------------------------------------------------------
  Function:		SCU_GetWakeEnSts                                                                                                                                                                          
  input:     	src:  WAKEUP_SRC_OTG_SUP/WAKEUP_SRC_UART0_RX/  WAKEUP_SRC_M4_SW  /            
                     WAKEUP_SRC_WDT_INT/ WAKEUP_SRC_TIMERA1_INT/WAKEUP_SRC_RTC_PRD /            
                    WAKEUP_SRC_RTC_ALM / WAKEUP_SRC_GPIO9 /  WAKEUP_SRC_GPIO8/            
                    WAKEUP_SRC_GPIO7~WAKEUP_SRC_GPIO0
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get WAKE_EN status                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT8 SCU_GetWakeEnSts(UINT32 src) 
{
   UINT32 temp;
   
   temp = SGCC_SCU_P->WAKEUP_EN;
   if(temp & src)
       return 1;
   else
       return 0;
   
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnableIntr                                                                                                                                                                          
  input:     	intrType:  SCU_INT_SRC_WAKEUP/SCU_INT_SRC_VD/SCU_INT_SRC_XLFDT/SCU_INT_SRC_XHFDT
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable interupt                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_EnableIntr(UINT32 intrType) 
{
   SGCC_SCU_P->INT_EN |= intrType;
}


/*-----------------------------------------------------------------------------
  Function:		SCU_DisableIntr                                                                                                                                                                          
  input:     	intrType:  SCU_INT_SRC_WAKEUP/SCU_INT_SRC_VD/SCU_INT_SRC_XLFDT/SCU_INT_SRC_XHFDT
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: disable interupt                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_DisableIntr(UINT32 intrType) 
{
   SGCC_SCU_P->INT_EN &= ~intrType;
}



/*-----------------------------------------------------------------------------
  Function:		SCU_GetSysSts                                                                                                                                                                          
  input:     	stsType:  SYS_STS_CPU_LOCKUP_STS/SYS_STS_VD_STS/SYS_STS_XLFDT_STS/SYS_STS_XHFDT_STS
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get system status                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT8 SCU_GetSysSts(UINT32 stsType) 
{
   UINT32 temp;
   
   temp = SGCC_SCU_P->SYS_STS;
   if(temp & stsType)
       return 1;
   else
       return 0;
   
}

/*-----------------------------------------------------------------------------
  Function:		SCU_ClearSysSts                                                                                                                                                                          
  input:     	stsType:  SYS_STS_CPU_LOCKUP_STS/SYS_STS_VD_STS/SYS_STS_XLFDT_STS/SYS_STS_XHFDT_STS
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: clear status                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_ClearSysSts(UINT32 stsType) 
{
   SGCC_SCU_P->SYS_STS = stsType;
}


/*-----------------------------------------------------------------------------
  Function:		SCU_GetIntrSts                                                                                                                                                                          
  input:     	intrType:  SCU_INT_SRC_WAKEUP/SCU_INT_SRC_VD/SCU_INT_SRC_XLFDT/SCU_INT_SRC_XHFDT
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: get interrupt status                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT8 SCU_GetIntrSts(UINT32 intrType) 
{
   UINT32 temp;
   
   temp = SGCC_SCU_P->INT_STS;
   if(temp & intrType)
       return 1;
   else
       return 0;
   
}

/*-----------------------------------------------------------------------------
  Function:		SCU_ClearIntr                                                                                                                                                                          
  input:     	intrType:  SCU_INT_SRC_WAKEUP/SCU_INT_SRC_VD/SCU_INT_SRC_XLFDT/SCU_INT_SRC_XHFDT
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable interupt                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_ClearIntr(UINT32 intrType) 
{
   SGCC_SCU_P->INT_STS = intrType;
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_EnableHitCnt                                                                                                                                                                         
  input:     	opt -- ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable hit counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CACHE_EnableHitCnt(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
		    SGCC_SCU_P->CACHE_HIT |= CACHE_HIT_EN;
		}
		else
		{ 
			  SGCC_SCU_P->CACHE_HIT &= ~CACHE_HIT_EN;
		}
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_RstHitCnt                                                                                                                                                                         
  input:     	opt -- ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: reset hit related counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CACHE_RstHitCnt(FunctionalState opt) 
{
    SGCC_SCU_P->CACHE_HIT |= CACHE_HIT_RST;
	  
	  //wait the bit clears
	  while(SGCC_SCU_P->CACHE_HIT & CACHE_HIT_RST);
		
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_ReadHitCnt                                                                                                                                                                         
  input:     	None
  output:     None                                                                                    
  Returns:    hitCnt                                                                                      
  Description: read hit counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT32 CACHE_ReadHitCnt(void) 
{
	  UINT32 temp;
	  
	  temp = SGCC_SCU_P->CACHE_HIT & CACHE_HIT_CNT;
	  
	  return temp;
	  		
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_ReadHitOvFlow                                                                                                                                                                         
  input:     	None
  output:     None                                                                                    
  Returns:    hitCnt                                                                                      
  Description: read hit overflow flag                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT32 CACHE_ReadHitOvFlow(void) 
{
	  UINT32 temp;
	  
	  temp = SGCC_SCU_P->CACHE_HIT & CACHE_HIT_OVF;
	  
	  return temp;
	  		
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_EnableMissCnt                                                                                                                                                                         
  input:     	opt -- ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable miss counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CACHE_EnableMissCnt(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
		    SGCC_SCU_P->CACHE_MISS |= CACHE_MISS_EN;
		}
		else
		{ 
			  SGCC_SCU_P->CACHE_MISS &= ~CACHE_MISS_EN;
		}
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_RstMissCnt                                                                                                                                                                         
  input:     	opt -- ENABLE/DISABLE
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: reset Miss related counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CACHE_RstMissCnt(FunctionalState opt) 
{
    SGCC_SCU_P->CACHE_MISS |= CACHE_MISS_RST;
	  
	  //wait the bit clears
	  while(SGCC_SCU_P->CACHE_MISS & CACHE_MISS_RST);
		
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_ReadMissCnt                                                                                                                                                                         
  input:     	None
  output:     None                                                                                    
  Returns:    hitCnt                                                                                      
  Description: read Miss counter                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT32 CACHE_ReadMissCnt(void) 
{
	  UINT32 temp;
	  
	  temp = SGCC_SCU_P->CACHE_MISS & CACHE_MISS_CNT;
	  
	  return temp;
	  		
}

/*-----------------------------------------------------------------------------
  Function:		CACHE_ReadMissOvFlow                                                                                                                                                                         
  input:     	None
  output:     None                                                                                    
  Returns:    hitCnt                                                                                      
  Description: read Miss overflow flag                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT32 CACHE_ReadMissOvFlow(void) 
{
	  UINT32 temp;
	  
	  temp = SGCC_SCU_P->CACHE_MISS & CACHE_MISS_OVF;
	  
	  return temp;
	  		
}

/*-----------------------------------------------------------------------------
  Function:		SCU_EnTrngSamp                                                                                                                                                                          
  input:      None
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable Trng sample                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void SCU_EnTrngSamp() 
{
    SGCC_SCU_P->SYS_CTRL |= SYS_CTRL_TRNG_SAMP_EN;
    
}

/*-----------------------------------------------------------------------------
  Function:		SCU_RdTrngData                                                                                                                                                                          
  input:      None
  output:     None                                                                                    
  Returns:    TRNG data                                                                                      
  Description: read Trng data                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
UINT32 SCU_RdTrngData() 
{
    UINT32 temp;
    
    while(SGCC_SCU_P->SYS_CTRL & SYS_CTRL_TRNG_SAMP_EN);  //wait while finish
    
    temp = SGCC_SCU_P->TRNG_DA;
    return(temp);
    
}
