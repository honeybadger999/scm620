/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_WDT.c                                                     */
/* 内容摘要: WDT模块基础函数定义                                              */
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
  Function:		WDT_Start                                                                                                                                                                            
  input:     	period--overflow period, can be
                SYSWDT_CTRL_OTSET_3906us/SYSWDT_CTRL_OTSET_7812us/
                SYSWDT_CTRL_OTSET_15625us/SYSWDT_CTRL_OTSET_31250us/
                SYSWDT_CTRL_OTSET_62500us/SYSWDT_CTRL_OTSET_125ms/
                SYSWDT_CTRL_OTSET_250ms/SYSWDT_CTRL_OTSET_500ms/
                SYSWDT_CTRL_OTSET_1s/SYSWDT_CTRL_OTSET_2s   /  SYSWDT_CTRL_OTSET_4s   /
                SYSWDT_CTRL_OTSET_8s/SYSWDT_CTRL_OTSET_12s/SYSWDT_CTRL_OTSET_16s/
                SYSWDT_CTRL_OTSET_24s/SYSWDT_CTRL_OTSET_32s
                
               wakAble--DISABLE/ENABLE
               rstAble--DISABLE/ENABLE
               intEn--DISABLE/ENABLE
  output:      None                                                                                    
  Returns:     None                                                                                      
  Description: 配置WDT的溢出周期并启动                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Start(UINT32 period, UINT8 wakAble, UINT8 rstAble, UINT8 intEn) 
{
	  UINT32 temp = 0;
	  
	  temp |= (period | 0x1);
	  
	  if(wakAble)
	      temp |= SYSWDT_CTRL_WAKABLE ;
	  if(rstAble)
	  	  temp |= SYSWDT_CTRL_RSTABLE;
	  if(intEn)
	  	  temp |= SYSWDT_CTRL_INT_EN;
	  
	  SGCC_WDT_P->CTRL = temp;
}

/*-----------------------------------------------------------------------------
  Function:		WDT_Disable                                                                                                                                                                            
  input:     	None                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 不使能WDT                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Disable() 
{
	  SGCC_WDT_P->CTRL = 0x0;
}

/*-----------------------------------------------------------------------------
  Function:		WDT_Restart                                                                                                                                                                            
  input:     	None                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 重启WDT                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Restart() 
{
	  UINT32 temp;
	  
	  //输入清WDT的密码
	  temp = SGCC_WDT_P->CTRL;
	  temp |= SYSWDT_CTRL_PASSWORD_WDTCLR;
	  SGCC_WDT_P->CTRL = temp;
	  
	  SGCC_WDT_P->CLR = 0xa5;
}


/*-----------------------------------------------------------------------------
  Function:		WDT_ClrWakeFlag                                                                                                                                                                            
  input:     	None                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 清唤醒标志                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_ClrWakeFlag() 
{
	  SGCC_WDT_P->CTRL |= SYSWDT_CTRL_CLRWAKE;
}


/*-----------------------------------------------------------------------------
  Function:		WDT_EnableIntr                                                                                                                                                                            
input:        opt:ENABLE/DISABLE                   	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable interrupt                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_EnableIntr(UINT8 opt) 
{
    if(opt == ENABLE)
	    SGCC_WDT_P->CTRL |= SYSWDT_CTRL_INT_EN;
    else
        SGCC_WDT_P->CTRL &= ~SYSWDT_CTRL_INT_EN;
}
