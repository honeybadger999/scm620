/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: ������о΢����                                                   */
/*                                                                            */
/* �ļ�����: SCM62X_WDT.c                                                     */
/* ����ժҪ: WDTģ�������������                                              */
/* ����˵��: **                                                               */
/* ��ǰ�汾: V1.0                                                             */
/* ��    ��: SCM                                                              */
/* �������: 2019-03-20                                                       */
/* �޸ļ�¼:                                                                  */
/*    �޸�����          �汾��        �޸���        �޸�����                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0          SCM        �����ļ�                     */
/*<FH->************************************************************************/


/******************************************************************************/
/*               #include������Ϊ��׼��ͷ�ļ����Ǳ�׼��ͷ�ļ���               */
/******************************************************************************/
#include "SCM62X.h"

/******************************************************************************/
/*                                �ⲿ���ö���                                */
/******************************************************************************/


/******************************************************************************/
/*                                  ȫ�ֱ���                                  */
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
  Description: ����WDT��������ڲ�����                                                                                                 	                                   
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
  Description: ��ʹ��WDT                                                                                                 	                                   
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
  Description: ����WDT                                                                                                 	                                   
 *-----------------------------------------------------------------------------*/
void WDT_Restart() 
{
	  UINT32 temp;
	  
	  //������WDT������
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
  Description: �廽�ѱ�־                                                                                                 	                                   
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
