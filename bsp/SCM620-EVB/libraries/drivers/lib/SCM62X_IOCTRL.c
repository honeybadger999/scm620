/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_IOCTRL.c                                                  */
/* 内容摘要: IOCTRL模块基础函数定义                                           */
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

/********************************************************
 Function:		RMII0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为MAC0  
********************************************************/
void RMII0_InitPad()
{	  
	  SGCC_IOCTRL_P->IO_CTRL0 &= 0xfff0fff0;
	  SGCC_IOCTRL_P->IO_CTRL0 |= 0x20002;
	
	  SGCC_IOCTRL_P->IO_CTRL1 &= 0xff00000f;
	  SGCC_IOCTRL_P->IO_CTRL1 |= 0x222220;
		  
	  SGCC_IOCTRL_P->IO_CTRL7 &= 0xffff00ff;
	  SGCC_IOCTRL_P->IO_CTRL7 |= 0x00002200;
	
}

/********************************************************
 Function:		MII0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为MAC0  
********************************************************/
void MII0_InitPad()
{	  
	  SGCC_IOCTRL_P->IO_CTRL0 &= 0xfff00000;
	  SGCC_IOCTRL_P->IO_CTRL0 |= 0x22222;
	
	  SGCC_IOCTRL_P->IO_CTRL1 &= 0xff00000f;
	  SGCC_IOCTRL_P->IO_CTRL1 |= 0x222220;
	
	  SGCC_IOCTRL_P->IO_CTRL6 &= 0x0000ffff;
	  SGCC_IOCTRL_P->IO_CTRL6 |= 0x22220000;
	  
	  SGCC_IOCTRL_P->IO_CTRL7 &= 0xffff0000;
	  SGCC_IOCTRL_P->IO_CTRL7 |= 0x00002222;
	
}


/********************************************************
 Function:		RMII1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为MAC1 RMII1  
********************************************************/
void RMII1_InitPad()
{	  
	  SGCC_IOCTRL_P->IO_CTRL9 &= 0xfff0ffff;
	  SGCC_IOCTRL_P->IO_CTRL9 |= 0x00020000;
	
	  SGCC_IOCTRL_P->IO_CTRLA &= 0x0000ff00;
	  SGCC_IOCTRL_P->IO_CTRLA |= 0x22220022;
		  
	  SGCC_IOCTRL_P->IO_CTRLF &= 0xfff00fff;
	  SGCC_IOCTRL_P->IO_CTRLF |= 0x00022000;
}

/********************************************************
 Function:		MII1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为MAC1 MII1  
********************************************************/
void MII1_InitPad()
{	  
	  SGCC_IOCTRL_P->IO_CTRL9 &= 0x0000ffff;
	  SGCC_IOCTRL_P->IO_CTRL9 |= 0x22220000;
	
	  SGCC_IOCTRL_P->IO_CTRLA &= 0x0000ff00;
	  SGCC_IOCTRL_P->IO_CTRLA |= 0x22220022;
	
	  SGCC_IOCTRL_P->IO_CTRLB &= 0xffff0000;
	  SGCC_IOCTRL_P->IO_CTRLB |= 0x00002222;
	  
	  SGCC_IOCTRL_P->IO_CTRLF &= 0xfff0000f;
	  SGCC_IOCTRL_P->IO_CTRLF |= 0x00022220;
}


/********************************************************
 Function:		OTG_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为OTG 
********************************************************/
void OTG_InitPad()
{	  
	 
}

/********************************************************
 Function:		XSRAM_InitPad                                                                                                                                                                            
 input:      None             
 output:     None                                        
 Returns:    None                                                                                      
 Description: XSRAM管脚功能使能/不使能  
********************************************************/
void XSRAM_EnPad()
{	  
	  SGCC_IOCTRL_P->IO_CTRL0 &= 0xfff00000;
	  SGCC_IOCTRL_P->IO_CTRL0 |= 0x11111;
	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= 0xf000000f;
	  SGCC_IOCTRL_P->IO_CTRL1 |= 0x1111110;
	  
	  SGCC_IOCTRL_P->IO_CTRL2 &= 0xffffff0f;
	  SGCC_IOCTRL_P->IO_CTRL2 |= 0x10;
	  
	  SGCC_IOCTRL_P->IO_CTRL6 &= 0x0000ff0f;
	  SGCC_IOCTRL_P->IO_CTRL6 |= 0x11110010;
	
	  SGCC_IOCTRL_P->IO_CTRL7 &= 0x00FF0000;
	  SGCC_IOCTRL_P->IO_CTRL7 |= 0x11001111;
	
	  SGCC_IOCTRL_P->IO_CTRL8 &= 0xFFFF0000;
	  SGCC_IOCTRL_P->IO_CTRL8 |= 0x1111;
	
	  SGCC_IOCTRL_P->IO_CTRL9 &= 0x11001111;
	  SGCC_IOCTRL_P->IO_CTRL9 |= 0x11110000;
	
	  SGCC_IOCTRL_P->IO_CTRLA &= 0x0000FF00;
	  SGCC_IOCTRL_P->IO_CTRLA |= 0x11110011;
		
		SGCC_IOCTRL_P->IO_CTRLB &= 0xFFFFFF00;
	  SGCC_IOCTRL_P->IO_CTRLB |= 0x11;
		
		SGCC_IOCTRL_P->IO_CTRLE &= 0xF00FF00F;
	  SGCC_IOCTRL_P->IO_CTRLE |= 0x1100110;
		
		SGCC_IOCTRL_P->IO_CTRLF &= 0x00F0000F;
	  SGCC_IOCTRL_P->IO_CTRLF |= 0x11011110;
		
		SGCC_IOCTRL_P->IO_CTRL10 &= 0xFFFFFFF0;
	  SGCC_IOCTRL_P->IO_CTRL10 |= 0x1;
		
		SGCC_IOCTRL_P->IO_CTRL11 &= 0xFF00FFFF;
    SGCC_IOCTRL_P->IO_CTRL11 |= 0x110000;
}


/********************************************************
 Function:		MCO_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为MCO 
********************************************************/
void MCO_InitPad(void)
{	  
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((UINT32)7<<12));
	tmp |= ((UINT32)3<<12);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;  
}
/********************************************************
 Function:		TIMER_EXIN_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为TIMER_EXIN
********************************************************/
void TIMERA_EXIN2_InitPad_34(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<4));
	tmp |= ((uint32_t)2<<4);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void TIMERB_EXIN2_InitPad_35(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)2<<8);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

/********************************************************
 Function:		TIMER_EXOUT_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为TIMER_EXOUT
********************************************************/
void TIMERA_EXOUT1_InitPad_42(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~((uint32_t)7<<4));
	tmp |= ((uint32_t)5<<4);
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

void TIMERA_EXOUT1_InitPad_96(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLB;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)5<<28);
	SGCC_IOCTRL_P->IO_CTRLB = tmp;
}

void TIMERA_EXOUT1_InitPad_100(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<12));
	tmp |= ((uint32_t)5<<12);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void TIMERA_EXOUT2_InitPad_34(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<4));
	tmp |= ((uint32_t)5<<4);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void TIMERA_EXOUT2_InitPad_104(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)5<<28);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void TIMERA_EXOUT2_InitPad_136(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL10;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)5<<28);
	SGCC_IOCTRL_P->IO_CTRL10 = tmp;
}

void TIMERB_EXOUT1_InitPad_43(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)5<<8);
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

void TIMERB_EXOUT1_InitPad_97(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<0));
	tmp |= ((uint32_t)5<<0);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void TIMERB_EXOUT1_InitPad_101(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<16));
	tmp |= ((uint32_t)5<<16);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void TIMERB_EXOUT2_InitPad_35(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)5<<8);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void TIMERB_EXOUT2_InitPad_137(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL11;
	tmp &= (~((uint32_t)7<<0));
	tmp |= ((uint32_t)5<<0);
	SGCC_IOCTRL_P->IO_CTRL11 = tmp;
}

void TIMERC_EXOUT1_InitPad_46(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~((uint32_t)7<<20));
	tmp |= ((uint32_t)5<<20);
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

void TIMERC_EXOUT1_InitPad_98(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<4));
	tmp |= ((uint32_t)5<<4);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void TIMERC_EXOUT1_InitPad_102(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<20));
	tmp |= ((uint32_t)5<<20);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void TIMERC_EXOUT2_InitPad_36(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<12));
	tmp |= ((uint32_t)5<<12);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void TIMERC_EXOUT2_InitPad_139(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL11;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)6<<8);
	SGCC_IOCTRL_P->IO_CTRL11 = tmp;
}

void TIMERD_EXOUT1_InitPad_47(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~((uint32_t)7<<24));
	tmp |= ((uint32_t)5<<24);
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

void TIMERD_EXOUT1_InitPad_99(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)5<<8);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void TIMERD_EXOUT1_InitPad_103(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<24));
	tmp |= ((uint32_t)5<<24);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void TIMERD_EXOUT2_InitPad_37(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<16));
	tmp |= ((uint32_t)5<<16);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void TIMERD_EXOUT2_InitPad_140(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL11;
	tmp &= (~((uint32_t)7<<12));
	tmp |= ((uint32_t)6<<12);
	SGCC_IOCTRL_P->IO_CTRL11 = tmp;
}
/********************************************************
 Function:		PWM_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为PWM
********************************************************/
void PWM00_InitPad_42(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~((uint32_t)7<<4));
	tmp |= ((uint32_t)4<<4);
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

void PWM00_InitPad_96(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLB;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)2<<28);
	SGCC_IOCTRL_P->IO_CTRLB = tmp;
}

void PWM00_InitPad_100(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<12));
	tmp |= ((uint32_t)2<<12);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void PWM01_InitPad_43(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)4<<8);
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

void PWM01_InitPad_97(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<0));
	tmp |= ((uint32_t)2<<0);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void PWM01_InitPad_101(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<16));
	tmp |= ((uint32_t)2<<16);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void PWM01_InitPad_140(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL11;
	tmp &= (~((uint32_t)7<<12));
	tmp |= ((uint32_t)1<<12);
	SGCC_IOCTRL_P->IO_CTRL11 = tmp;
}

void PWM02_InitPad_40(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)4<<28);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void PWM02_InitPad_46(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~((uint32_t)7<<20));
	tmp |= ((uint32_t)2<<20);
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

void PWM02_InitPad_98(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<4));
	tmp |= ((uint32_t)2<<4);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void PWM02_InitPad_102(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<20));
	tmp |= ((uint32_t)2<<20);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void PWM10_InitPad_35(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)3<<8);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void PWM10_InitPad_103(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<24));
	tmp |= ((uint32_t)2<<24);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void PWM10_InitPad_111(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLD;
	tmp &= (~((uint32_t)7<<24));
	tmp |= ((uint32_t)4<<24);
	SGCC_IOCTRL_P->IO_CTRLD = tmp;
}

void PWM10_InitPad_136(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL10;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)1<<28);
	SGCC_IOCTRL_P->IO_CTRL10 = tmp;
}

void PWM11_InitPad_47(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~((uint32_t)7<<24));
	tmp |= ((uint32_t)2<<24);
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

void PWM11_InitPad_36(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<12));
	tmp |= ((uint32_t)3<<12);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void PWM11_InitPad_99(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)2<<8);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void PWM11_InitPad_104(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)2<<28);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

void PWM11_InitPad_112(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLD;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)4<<28);
	SGCC_IOCTRL_P->IO_CTRLD = tmp;
}

void PWM11_InitPad_137(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL11;
	tmp &= (~((uint32_t)7<<0));
	tmp |= ((uint32_t)1<<0);
	SGCC_IOCTRL_P->IO_CTRL11 = tmp;
}

void PWM12_InitPad_34(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<4));
	tmp |= ((uint32_t)3<<4);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void PWM12_InitPad_37(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<16));
	tmp |= ((uint32_t)3<<16);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

void PWM12_InitPad_110(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLD;
	tmp &= (~((uint32_t)7<<20));
	tmp |= ((uint32_t)4<<20);
	SGCC_IOCTRL_P->IO_CTRLD = tmp;
}

void PWM12_InitPad_113(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLE;
	tmp &= (~((uint32_t)7<<0));
	tmp |= ((uint32_t)4<<0);
	SGCC_IOCTRL_P->IO_CTRLE = tmp;
}

void PWM12_InitPad_139(void)
{
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL11;
	tmp &= (~((uint32_t)7<<8));
	tmp |= ((uint32_t)1<<8);
	SGCC_IOCTRL_P->IO_CTRL11 = tmp;
}

/********************************************************
 Function:		UART0_EnIrda                                                                                                                                                                            
 input:      opt -- ENABLE/DISABLE              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 开启/不开启红外功能 
********************************************************/
void UART0_EnIrda(UINT8 opt)
{	  
	 if(ENABLE == opt)
	 {
		 SGCC_IOCTRL_P->IO_CTRL13 |= 1;
	 }
	 else
	 {
		 SGCC_IOCTRL_P->IO_CTRL13 &= ((UINT32)(~1));
	 }
}

/********************************************************
 Function:		UART0_InitPad                                                                                                                                                                            
 input:      UART0_PIN44TX/UART0_PIN97RX/UART0_PIN45RX/UART0_PIN96TX              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART0 TX RX引脚传入顺序自适应
********************************************************/
typedef struct{
	volatile UINT32 *p_ioctrl;
	UINT8  _offsetbit;
	UINT8  _option;
}padinit_typ;
padinit_typ uart0_pad[]={
	[UART0_PIN44TX] = {&SGCC_IOCTRL_P->IO_CTRL5, 12, 3},
	[UART0_PIN97RX] = {&SGCC_IOCTRL_P->IO_CTRLC, 0, 4},
	[UART0_PIN45RX] = {&SGCC_IOCTRL_P->IO_CTRL5, 16, 3},
	[UART0_PIN96TX] = {&SGCC_IOCTRL_P->IO_CTRLB, 28, 4}
};
void UART0_InitPad(UINT8 usepinrx, UINT8 usepintx)
{
	UINT32 tmp;
	UINT8 i;
	if((usepinrx<=3)&&(usepintx<=3))
	{
		i=usepinrx;
		do
		{
			tmp = *uart0_pad[i].p_ioctrl;
			tmp &= (~((uint32_t)7<<uart0_pad[i]._offsetbit));
			tmp |= ((uint32_t)uart0_pad[i]._option<<uart0_pad[i]._offsetbit);
			*uart0_pad[i].p_ioctrl = tmp;
			if(i == usepinrx)
			{
				i = usepintx;
			}
			else
			{
				return;
			}
		}while(1);
	}
}

/********************************************************
 Function:		UART1_InitPad                                                                                                                                                                            
 input:      UART1_PIN109TX/UART1_PIN110RX/UART1_PIN113TX/UART1_PIN116RX             
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART1 TX RX引脚传入顺序自适应
********************************************************/
padinit_typ uart1_pad[]={
	[UART1_PIN109TX] = {&SGCC_IOCTRL_P->IO_CTRLD, 16, 6},
	[UART1_PIN110RX] = {&SGCC_IOCTRL_P->IO_CTRLD, 20, 6},
	[UART1_PIN113TX] = {&SGCC_IOCTRL_P->IO_CTRLE, 0, 1},
	[UART1_PIN116RX] = {&SGCC_IOCTRL_P->IO_CTRLE, 12, 1}
};
void UART1_InitPad(UINT8 usepinrx, UINT8 usepintx)
{
	UINT32 tmp;
	UINT8 i;
	if((usepinrx<=3)&&(usepintx<=3))
	{
		i=usepinrx;
		do
		{
			tmp = *uart1_pad[i].p_ioctrl;
			tmp &= (~((uint32_t)7<<uart1_pad[i]._offsetbit));
			tmp |= ((uint32_t)uart1_pad[i]._option<<uart1_pad[i]._offsetbit);
			*uart1_pad[i].p_ioctrl = tmp;
			if(i == usepinrx)
			{
				i = usepintx;
			}
			else
			{
				return;
			}
		}while(1);
	}
}

/********************************************************
 Function:		UART2_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART2
********************************************************/
void UART2_InitPad(void)
{
	uint32_t tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL8;
	tmp &= (~(((uint32_t)7<<16)|((uint32_t)7<<20)));
	tmp |= (((uint32_t)2<<16)|((uint32_t)2<<20));
	SGCC_IOCTRL_P->IO_CTRL8 = tmp;	  
}

/********************************************************
 Function:		UART2_RTSCTS_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART4 
********************************************************/
void UART2_RTSCTS_InitPad(void)
{	  
	uint32_t tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL9;
	tmp &= (~(((uint32_t)7<<4)|((uint32_t)7<<8)));
	tmp |= (((uint32_t)2<<4)|((uint32_t)2<<8));
	SGCC_IOCTRL_P->IO_CTRL9 = tmp;
}

/********************************************************
 Function:		UART3_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART3 
********************************************************/
void UART3_InitPad()
{	  
	  SGCC_IOCTRL_P->IO_CTRL4 &= (~(((uint32_t)7<<12)|((uint32_t)7<<16)));
}

/********************************************************
 Function:		UART3_RTSCTS_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART3 cts rts 
********************************************************/
void UART3_RTSCTS_InitPad(void)
{	  
	SGCC_IOCTRL_P->IO_CTRL4 &= (~(((uint32_t)7<<4)|((uint32_t)7<<8)));
}

/********************************************************
 Function:		UART4_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART4 
********************************************************/
void UART4_InitPad(void)
{	  
	uint32_t tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~(((uint32_t)7<<24)|((uint32_t)7<<20)));
	tmp |= (((uint32_t)3<<24)|((uint32_t)3<<20));
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
}

/********************************************************
 Function:		UART5_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART5 
********************************************************/
void UART5_InitPad(void)
{	  
	uint32_t tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLD;
	tmp &= (~(((uint32_t)7<<24)|((uint32_t)7<<28)));
	tmp |= (((uint32_t)1<<24)|((uint32_t)1<<28));
	SGCC_IOCTRL_P->IO_CTRLD = tmp;
}

/********************************************************
 Function:		UART6_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART6 
********************************************************/
void UART6_InitPad(void)
{	  
	uint32_t tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL10;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)3<<28);
	SGCC_IOCTRL_P->IO_CTRL10 = tmp;
	
	tmp = SGCC_IOCTRL_P->IO_CTRL11;
	tmp &= (~((uint32_t)7<<0));
	tmp |= ((uint32_t)3<<0);
	SGCC_IOCTRL_P->IO_CTRL11 = tmp;
}

/********************************************************
 Function:		UART6_RTSCTS_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART4 
********************************************************/
void UART6_RTSCTS_InitPad(void)
{	  
	uint32_t tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL11;
	tmp &= (~(((uint32_t)7<<12)|((uint32_t)7<<8)));
	tmp |= (((uint32_t)5<<12)|((uint32_t)5<<8));
	SGCC_IOCTRL_P->IO_CTRL11 = tmp;
}

/********************************************************
 Function:		UART7_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART7 
********************************************************/
void UART7_InitPad(void)
{	  
	 SGCC_IOCTRL_P->IO_CTRLC &= (~(((uint32_t)7<<16)|((uint32_t)7<<20))); 
}

/********************************************************
 Function:		UART7_RTSCTS_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为UART4 
********************************************************/
void UART7_RTSCTS_InitPad(void)
{	  
	uint32_t tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~(((uint32_t)7<<24)|((uint32_t)7<<28)));
	tmp |= (((uint32_t)4<<24)|((uint32_t)4<<28));
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
}

/********************************************************
 Function:		SPI0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为SPI0 
********************************************************/
void SPI0_InitPad(void)
{	  
	UINT32 tmp;

	tmp = SGCC_IOCTRL_P->IO_CTRL5;
	tmp &= (~(((uint32_t)7<<0)|((uint32_t)7<<4)|((uint32_t)7<<8)));
	tmp |= (((uint32_t)1<<0)|((uint32_t)1<<4)|((uint32_t)1<<8));
	SGCC_IOCTRL_P->IO_CTRL5 = tmp;
	
	tmp = SGCC_IOCTRL_P->IO_CTRL4;
	tmp &= (~((uint32_t)7<<28));
	tmp |= ((uint32_t)1<<28);
	SGCC_IOCTRL_P->IO_CTRL4 = tmp;
}

/********************************************************
 Function:		SPI1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为SPI1 
********************************************************/
void SPI1_InitPad(void)
{	  
	  SGCC_IOCTRL_P->IO_CTRL9 &= (~(((uint32_t)7<<8)|((uint32_t)7<<12)|((uint32_t)7<<4)|((uint32_t)7<<0)));
}

/********************************************************
 Function:		SPI2_InitPad                                                                                                                                                                            
 input:      SPI2_PIN111SCK/SPI2_PIN112MISO/SPI2_PIN113MOSI/SPI2_PIN133SCK/SPI2_PIN134MISO/SPI2_PIN135MOSI
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为SPI2 
********************************************************/
padinit_typ spi2_pad[]={
	[SPI2_PIN111SCK] = {&SGCC_IOCTRL_P->IO_CTRLD, 24, 5},
	[SPI2_PIN112MISO] = {&SGCC_IOCTRL_P->IO_CTRLD, 28, 5},
	[SPI2_PIN113MOSI] = {&SGCC_IOCTRL_P->IO_CTRLE, 0, 5},
	[SPI2_PIN133SCK] = {&SGCC_IOCTRL_P->IO_CTRL10, 16, 1},
	[SPI2_PIN134MISO] = {&SGCC_IOCTRL_P->IO_CTRL10, 20, 1},
	[SPI2_PIN135MOSI] = {&SGCC_IOCTRL_P->IO_CTRL10, 24, 1}
};
void SPI2_InitPad(UINT8 usepinsck, UINT8 usepinmiso, UINT8 usepinmosi)
{	  
	UINT32 tmp;
	UINT8 i;
	if((usepinsck<=5)&&(usepinmiso<=5) &&(usepinmosi<=5))
	{
		i=usepinmiso;
		do
		{
			tmp = *spi2_pad[i].p_ioctrl;
			tmp &= (~((uint32_t)7<<spi2_pad[i]._offsetbit));
			tmp |= ((uint32_t)spi2_pad[i]._option<<spi2_pad[i]._offsetbit);
			*spi2_pad[i].p_ioctrl = tmp;
			if(i == usepinmiso)
			{
				i = usepinmosi;
			}
			else if(i == usepinmosi)
			{
				i = usepinsck;
			}
			else
			{
				break;
			}
		}while(1);
	}
	SGCC_IOCTRL_P->IO_CTRLD &= (~((uint32_t)7<<20));
}

/********************************************************
 Function:		SPI3_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为SPI3 
********************************************************/
void SPI3_InitPad(void)
{	  
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL2;
	tmp &= (~(((uint32_t)7<<8)|((uint32_t)7<<12)|((uint32_t)7<<16)|((uint32_t)7<<20)));
	tmp |= (((uint32_t)1<<8)|((uint32_t)1<<12)|((uint32_t)1<<16)|((uint32_t)1<<20));
	SGCC_IOCTRL_P->IO_CTRL2 = tmp;
}

/********************************************************
 Function:		I2C0_InitPad                                                                                                                                                                            
 input:      I2C0_PIN109SDA/I2C0_PIN137SDA/I2C0_PIN140SDA/I2C0_PIN110SCL/I2C0_PIN136SCL/I2C0_PIN139SCL
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为I2C0  SCL SDA引脚传入顺序自适应
********************************************************/
padinit_typ i2c0_pad[]={
	[I2C0_PIN109SDA] = {&SGCC_IOCTRL_P->IO_CTRLD, 16, 5},
	[I2C0_PIN137SDA] = {&SGCC_IOCTRL_P->IO_CTRL11, 0, 2},
	[I2C0_PIN140SDA] = {&SGCC_IOCTRL_P->IO_CTRL11, 12, 2},
	[I2C0_PIN110SCL] = {&SGCC_IOCTRL_P->IO_CTRLD, 20, 5},
	[I2C0_PIN136SCL] = {&SGCC_IOCTRL_P->IO_CTRL10, 28, 2},
	[I2C0_PIN139SCL] = {&SGCC_IOCTRL_P->IO_CTRL11, 8, 2}
};
void I2C0_InitPad(UINT8 usepinscl, UINT8 usepinsda)
{
	UINT32 tmp;
	UINT8 i;
	if((usepinscl<=5)&&(usepinsda<=5))
	{
		i=usepinscl;
		do
		{
			tmp = *i2c0_pad[i].p_ioctrl;
			tmp &= (~((uint32_t)7<<i2c0_pad[i]._offsetbit));
			tmp |= ((uint32_t)i2c0_pad[i]._option<<i2c0_pad[i]._offsetbit);
			*i2c0_pad[i].p_ioctrl = tmp;
			if(i == usepinscl)
			{
				i = usepinsda;
			}
			else
			{
				return;
			}
		}while(1);
	}	
}

/********************************************************
 Function:		I2C1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为I2C1 
********************************************************/
void I2C1_InitPad(void)
{	 
	uint32_t tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL8;
	tmp &= (~(((uint32_t)7<<20)|((uint32_t)7<<16)));
	tmp |= (((uint32_t)1<<20)|((uint32_t)1<<16));
	SGCC_IOCTRL_P->IO_CTRL8 = tmp;
}

/********************************************************
 Function:		I2C2_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为I2C2 
********************************************************/
void I2C2_InitPad(void)
{	
	SGCC_IOCTRL_P->IO_CTRLD &= (~((uint32_t)7<<28));
	SGCC_IOCTRL_P->IO_CTRLE &= (~((uint32_t)7<<0));
}

/********************************************************
 Function:		SCI0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为SCI0 
********************************************************/
void SCI0_InitPad(void)
{	  
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRLC;
	tmp &= (~(((UINT32)7<<8)|((UINT32)7<<12)));
	tmp |= ((UINT32)3<<8);
	SGCC_IOCTRL_P->IO_CTRLC = tmp;
	SGCC_IOCTRL_P->IO_CTRLE &= (~((UINT32)7<<12));
}


/********************************************************
 Function:		SCI1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为SCI1 
********************************************************/
void SCI1_InitPad(void)
{	  
	UINT32 tmp;
	tmp = SGCC_IOCTRL_P->IO_CTRL10;
	tmp &= (~(((UINT32)7<<16)|((UINT32)7<<20)|((UINT32)7<<24)));
	tmp |= (((UINT32)3<<16) | ((UINT32)3<<20) | ((UINT32)3<<24) );
	SGCC_IOCTRL_P->IO_CTRL10 = tmp;
}


/********************************************************
 Function:		CAN0_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为CAN0_TX/CAN0_RX
********************************************************/
padinit_typ can0_pad[]={
	[CAN0_PIN28RXD]   = {&SGCC_IOCTRL_P->IO_CTRL3, 12, 2},
	[CAN0_PIN103RXD]  = {&SGCC_IOCTRL_P->IO_CTRLC, 24, 6},
    [CAN0_PIN29TXD0]  = {&SGCC_IOCTRL_P->IO_CTRL3, 16, 2},
	[CAN0_PIN104TXD0] = {&SGCC_IOCTRL_P->IO_CTRLC,28, 6},
};
void CAN0_InitPad(UINT8 usepinscl, UINT8 usepinsda)
{
	UINT32 tmp;
	UINT8 i;
	if((usepinscl<=3)&&(usepinsda<=3))
	{
		i=usepinscl;
		do
		{
			tmp = *can0_pad[i].p_ioctrl;
			tmp &= (~((uint32_t)7<<can0_pad[i]._offsetbit));
			tmp |= ((uint32_t)can0_pad[i]._option<<can0_pad[i]._offsetbit);
			*can0_pad[i].p_ioctrl = tmp;
			if(i == usepinscl)
			{
				i = usepinsda;
			}
			else
			{
				return;
			}
		}while(1);
	}	
}

/********************************************************
 Function:		CAN1_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为CAN1_TX/CAN1_RX
********************************************************/
void CAN1_InitPad()
{	
    SGCC_IOCTRL_P->IO_CTRLB &= ~(7<<28);
    SGCC_IOCTRL_P->IO_CTRLC &= ~(7<<0);
    
    SGCC_IOCTRL_P->IO_CTRLB |= 3<<28;
    SGCC_IOCTRL_P->IO_CTRLC |= 3<<0;
    
    SGCC_IOCTRL_P->IO_PD2    &= ~(1UL<<31);
    SGCC_IOCTRL_P->IO_PD3    &= ~(1<<0);
    SGCC_IOCTRL_P->IO_PU2    &= ~(1UL<<31);
    SGCC_IOCTRL_P->IO_PU3    &= ~(1<<0); 
}

/********************************************************
 Function:		CAN2_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为CAN1_TX/CAN1_RX
********************************************************/
void CAN2_InitPad()
{	
    SGCC_IOCTRL_P->IO_CTRL11 &= ~(7<<8);
    SGCC_IOCTRL_P->IO_CTRL11 &= ~(7<<12);
    
    SGCC_IOCTRL_P->IO_CTRL11 |= 4<<8;
    SGCC_IOCTRL_P->IO_CTRL11 |= 4<<12;
    
    SGCC_IOCTRL_P->IO_PD4    &= ~(3<<10);
    SGCC_IOCTRL_P->IO_PU4    &= ~(3<<10);
}

/********************************************************
 Function:		EXTAL0_InitPad                                                                                                                                                                            
=======
 Function:		XHOSC_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为XHOSC
********************************************************/
void XHOSC_InitPad()
{	  
    SGCC_IOCTRL_P->IO_CTRL2 &= 0x00FFFFFF;
}

/********************************************************
 Function:		EXTAL32_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为EXTAL32
********************************************************/
void EXTAL32_InitPad()
{	  
}

/********************************************************
 Function:		XTAL32_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为XTAL32
********************************************************/
void XTAL32_InitPad()
{	  
}

/********************************************************
 Function:		CLKIN_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为CLKIN
********************************************************/
void CLKIN_InitPad()
{	  
	
}

/********************************************************
 Function:		CLKIN32K_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为32KCLKIN
********************************************************/
void CLKIN32K_InitPad()
{	  
	
}

/********************************************************
 Function:		JTAG_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为JTAG
********************************************************/
void JTAG_InitPad()
{	  
	  SGCC_IOCTRL_P->IO_CTRL1 &= ~IO_CTRL1_JTAG_GPIO346;
}


/********************************************************
 Function:		CLKOUT_InitPad()                                                                                                                                                                            
 input:      None            
 output:     None                                        
 Returns:    None                                                                                      
 Description: 其它模拟信号测试使能
********************************************************/
void CLKOUT_InitPad()
{	  
	  SGCC_IOCTRL_P->IO_CTRL5 &= 0xffff0fff;
	  SGCC_IOCTRL_P->IO_CTRL5 |= 0x00006000; 
}

/********************************************************
 Function:		ADC_EnTest                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: ADC测试使能
********************************************************/
void ADC_EnTest(FunctionalState opt)
{	  
	
}

/********************************************************
 Function:		SPI0_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_SPI0_4P5/IO_DS_SPI0_9/IO_DS_SPI0_13P5/IO_DS_SPI0_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置SPI0相关管脚的输出电流大小
********************************************************/
void SPI0_ConfigDS(UINT32 ds)
{
 
}

/********************************************************
 Function:		SPI1_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_SPI1_4P5/IO_DS_SPI1_9/IO_DS_SPI1_13P5/IO_DS_SPI1_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置SPI0相关管脚的输出电流大小
********************************************************/
void SPI1_ConfigDS(UINT32 ds)
{
    
}

/********************************************************
 Function:		SPI2_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_SPI2_4P5/IO_DS_SPI2_9/IO_DS_SPI2_13P5/IO_DS_SPI2_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置SPI2相关管脚的输出电流大小
********************************************************/
void SPI2_ConfigDS(UINT32 ds)
{
  
}

/********************************************************
 Function:		SPI3_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_SPI3_4P5/IO_DS_SPI3_9/IO_DS_SPI3_13P5/IO_DS_SPI3_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置SPI3相关管脚的输出电流大小
********************************************************/
void SPI3_ConfigDS(UINT32 ds)
{
   
}

/********************************************************
 Function:		MII0_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_MII0_4P5/IO_DS_MII0_9/IO_DS_MII0_13P5/IO_DS_MII0_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置MII0/RMII0相关管脚的输出电流大小
********************************************************/
void MII0_ConfigDS(UINT32 ds)
{
  
}

/********************************************************
 Function:		MII1_ConfigDS                                                                                                                                                                            
 input:      ds - IO_DS_MII1_4P5/IO_DS_MII1_9/IO_DS_MII1_13P5/IO_DS_MII1_18              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置MII1/RMII1相关管脚的输出电流大小
********************************************************/
void MII1_ConfigDS(UINT32 ds)
{
    
}


/********************************************************
 Function:		IO_ConfigPullUp                                                                                                                                                                            
 input:      ioNo - P_GPIO0~P_UART5_RX
             opt - ENABLE/DISABLE            
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置IO管脚的上拉电阻使能/不使能
********************************************************/
u8 Pin_Change[142] = 
{
	33 ,//GPIO0
	255,//GPIO1
	68 ,//GPIO2
	69 ,//GPIO3
	95 ,//GPIO4
	96 ,//GPIO5
	97 ,//GPIO6
	98 ,//GPIO7
	110,//GPIO8
	131,//GPIO9
	0  ,//GPIO10
	1  ,//GPIO11
	2  ,//GPIO12
	3  ,//GPIO13
	4  ,//GPIO14
	255,//GPIO15
	255,//GPIO16
	9  ,//GPIO17
	10 ,//GPIO18
	11 ,//GPIO19
	12 ,//GPIO20
	13 ,//GPIO21
	14 ,//GPIO22
	17 ,//GPIO23
	18 ,//GPIO24
	19 ,//GPIO25
	20 ,//GPIO26
	21 ,//GPIO27
	22 ,//GPIO28
	23 ,//GPIO29
	25 ,//GPIO30
	26 ,//GPIO31
	27 ,//GPIO32
	28 ,//GPIO33
	34 ,//GPIO34
	35 ,//GPIO35
	36 ,//GPIO36
	39 ,//GPIO37
	40 ,//GPIO38
	41 ,//GPIO39
	42 ,//GPIO40
	43 ,//GPIO41
	44 ,//GPIO42
	45 ,//GPIO43
	46 ,//GPIO44
	48 ,//GPIO45
	49 ,//GPIO46
	52 ,//GPIO47
	53 ,//GPIO48
	54 ,//GPIO49
	55 ,//GPIO50
	56 ,//GPIO51
	57 ,//GPIO52
	58 ,//GPIO53
	69 ,//GPIO54
	62 ,//GPIO55
	63 ,//GPIO56
	64 ,//GPIO57
	65 ,//GPIO58
	66 ,//GPIO59
	67 ,//GPIO60
	72 ,//GPIO61
	73 ,//GPIO62
	74 ,//GPIO63
	75 ,//GPIO64
	76 ,//GPIO65
	77 ,//GPIO66
	78 ,//GPIO67
	79 ,//GPIO68
	80 ,//GPIO69
	81 ,//GPIO70
	84 ,//GPIO71
	85 ,//GPIO72
	86 ,//GPIO73
	87 ,//GPIO74
	88 ,//GPIO75
	89 ,//GPIO76
	90 ,//GPIO77
	91 ,//GPIO78
	92 ,//GPIO79
	99 ,//GPIO80
	100,//GPIO81
	101,//GPIO82
	102,//GPIO83
	103,//GPIO84
	109,//GPIO85
	111,//GPIO86
	112,//GPIO87
	113,//GPIO88
	114,//GPIO89
	115,//GPIO90
	116,//GPIO91
	117,//GPIO92
	118,//GPIO93
	121,//GPIO94
	122,//GPIO95
	123,//GPIO96
	124,//GPIO97
	125,//GPIO98
	126,//GPIO99
	127,//GPIO100
	128,//GPIO101
	132,//GPIO102
	133,//GPIO103
	134,//GPIO104
	135,//GPIO105
	136,//GPIO106
	138,//GPIO107
	139,//GPIO108
	140,//GPIO109
	141,//GPIO110
};

void IO_ConfigPullUp(UINT8 ioNo, FunctionalState opt)
{
  UINT32 temp;
	 ioNo = Pin_Change[ioNo];
	 if(ioNo < 32)
	 {
	     temp = SGCC_IOCTRL_P->IO_PU0;
	     temp &= ~(1 << ioNo);	 
	     if(opt == ENABLE)
	     temp |=  (1 << ioNo);
	     SGCC_IOCTRL_P->IO_PU0 = temp;
	
   } else if((ioNo <64)&&(ioNo >=32))
	 {
	     temp = SGCC_IOCTRL_P->IO_PU1;
	     temp &= ~(1 << (ioNo - 32));	 
	     if(opt == ENABLE)
	     temp |=  (1 << (ioNo - 32));	 
	     SGCC_IOCTRL_P->IO_PU1 = temp;
			 
	 } else if((ioNo <96)&&(ioNo >=64))
   {
	     temp = SGCC_IOCTRL_P->IO_PU2;
	     temp &= ~(1 << (ioNo - 64));	 
	     if(opt == ENABLE)
	     temp |=  (1 << (ioNo - 64));	 
	     SGCC_IOCTRL_P->IO_PU2 = temp;
			 
	 } else if((ioNo <128)&&(ioNo >=96))		 
	 {
	     temp = SGCC_IOCTRL_P->IO_PU3;
	     temp &= ~(1 << (ioNo - 96));	 
	     if(opt == ENABLE)
	     temp |=  (1 << (ioNo - 96));	 
	     SGCC_IOCTRL_P->IO_PU3 = temp;		 
   }else{
		 	 temp = SGCC_IOCTRL_P->IO_PU4;
	     temp &= ~(1 << (ioNo - 128));	 
	     if(opt == ENABLE)
	     temp |=  (1 << (ioNo - 128));	 
	     SGCC_IOCTRL_P->IO_PU4 = temp;
	 }
}

/********************************************************
 Function:		IO_ConfigPullDown                                                                                                                                                                            
 input:      ioNo - P_GPIO0~P_UART5_RX
             opt - ENABLE/DISABLE            
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置IO管脚的下拉电阻使能/不使能
********************************************************/
void IO_ConfigPullDown(UINT8 ioNo, FunctionalState opt)
{
  UINT32 temp;
	 ioNo = Pin_Change[ioNo];
	 if(ioNo < 32)
	 {
	     temp = SGCC_IOCTRL_P->IO_PD0;
	     temp &= ~(1 << ioNo);	 
	     if(opt == ENABLE)
	     temp |=  (1 << ioNo);
	     SGCC_IOCTRL_P->IO_PD0 = temp;
	
  } else if((ioNo <64)&&(ioNo >=32))
	 {
	     temp = SGCC_IOCTRL_P->IO_PD1;
	     temp &= ~(1 << (ioNo - 32));
	     if(opt == ENABLE)
	     temp |=  (1 << (ioNo - 32));
	     SGCC_IOCTRL_P->IO_PD1 = temp;
			 
	 } else if((ioNo <96)&&(ioNo >=64))
   {
	     temp = SGCC_IOCTRL_P->IO_PD2;
	     temp &= ~(1 << (ioNo - 64));
	     if(opt == ENABLE)
	     temp |=  (1 << (ioNo - 64));
	     SGCC_IOCTRL_P->IO_PD2 = temp;
			 
	 } else if((ioNo <128)&&(ioNo >=96))		 
	 {
	     temp = SGCC_IOCTRL_P->IO_PD3;
	     temp &= ~(1 << (ioNo - 96));
	     if(opt == ENABLE)
	     temp |=  (1 << (ioNo - 96));
	     SGCC_IOCTRL_P->IO_PD3 = temp;		 
   }else{
		   temp = SGCC_IOCTRL_P->IO_PD4;
	     temp &= ~(1 << (ioNo - 128));
	     if(opt == ENABLE)
	     temp |=  (1 << (ioNo - 128));
	     SGCC_IOCTRL_P->IO_PD4 = temp;
	 } 
}

/********************************************************
 Function:		ADC_Select                                                                                                                                                                            
input:      adcOut: 0-ADC0/ADC1/ADC2
 output:     None                                        
 Returns:    None                                                                                      
 Description: 选择哪一路ADC输出
********************************************************/
void ADC_Select(UINT8 adcS)
{
   SGCC_IOCTRL_P->IO_CTRL12 = adcS;   
		  
}

/********************************************************
 Function:		ADC_InitOutPad                                                                                                                                                                            
 input:      None
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置ADC 输出管脚
********************************************************/
void ADC_InitOutPad()
{
    SGCC_IOCTRL_P->IO_CTRL8 &= 0xff00ffff;
		SGCC_IOCTRL_P->IO_CTRL8 |= 0x00330000;
		
		SGCC_IOCTRL_P->IO_CTRL9 &= 0xffff0000;
		SGCC_IOCTRL_P->IO_CTRL9 |= 0x3333;
  
    SGCC_IOCTRL_P->IO_CTRLB &= 0x0fffffff;
		SGCC_IOCTRL_P->IO_CTRLB |= 0x60000000;
		
		SGCC_IOCTRL_P->IO_CTRLC &= 0xff000000;
		SGCC_IOCTRL_P->IO_CTRLC |= 0x00666666;
}

/********************************************************
 Function:		ADC_InitInPad                                                                                                                                                                            
 input:      adcIn:1~16
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置ADC 输入管脚
********************************************************/
void ADC_InitInPad(UINT8 adcIn)
{
	  switch(adcIn)
	  {
	  	  case 1:
	      SGCC_IOCTRL_P->IO_CTRL3 &= 0xffffff0f;   //IO_CTRL3[6:4], for ADC0/1
		    break;
		    
		    case 2:
	      SGCC_IOCTRL_P->IO_CTRL3 &= 0xfffff0ff;   //IO_CTRL3[10:8], for ADC0/1
		    break;
		    
		    case 3:
	      SGCC_IOCTRL_P->IO_CTRL3 &= 0xffff0fff;   //IO_CTRL3[14:12], for ADC0/1
		    break;
		    
		    case 4:
	      SGCC_IOCTRL_P->IO_CTRL3 &= 0xfff0ffff;   //IO_CTRL3[18:16], for ADC0/1
		    break;
		    
		    case 5:
	      SGCC_IOCTRL_P->IO_CTRL5 &= 0xffff0fff;   //IO_CTRL5[14:12], for ADC2
	      SGCC_IOCTRL_P->IO_CTRL5 |= 0x00004000;   
		    break;
		    
		    case 6:
	      SGCC_IOCTRL_P->IO_CTRL5 &= 0xfff0ffff;   //IO_CTRL5[18:16], for ADC2
	      SGCC_IOCTRL_P->IO_CTRL5 |= 0x00040000;   
		    break;
		    
		    case 7:
	      SGCC_IOCTRL_P->IO_CTRL5 &= 0xff0fffff;   //IO_CTRL5[22:20], for ADC2
	      SGCC_IOCTRL_P->IO_CTRL5 |= 0x00400000;   
		    break;
		    
		    case 8:
	      SGCC_IOCTRL_P->IO_CTRL5 &= 0xf0ffffff;   //IO_CTRL5[26:24], for ADC2
	      SGCC_IOCTRL_P->IO_CTRL5 |= 0x04000000;   
		    break;
		    
		    case 9:
	      SGCC_IOCTRL_P->IO_CTRL4 &= 0x0fffffff;   //IO_CTRL4[30:28], for ADC1/2
	      SGCC_IOCTRL_P->IO_CTRL4 |= 0x02000000;   
		    break;
		    
		    case 10:
	      SGCC_IOCTRL_P->IO_CTRL5 &= 0xfffffff0;   //IO_CTRL5[2:0], for ADC1/2
	      SGCC_IOCTRL_P->IO_CTRL5 |= 0x00000002;   
		    break;
		    
		    case 11:
	      SGCC_IOCTRL_P->IO_CTRL5 &= 0xffffff0f;   //IO_CTRL5[6:4], for ADC1/2
	      SGCC_IOCTRL_P->IO_CTRL5 |= 0x00000020;   
		    break;
		    
		    case 12:
	      SGCC_IOCTRL_P->IO_CTRL5 &= 0xfffff0ff;   //IO_CTRL5[10:8], for ADC1/2
	      SGCC_IOCTRL_P->IO_CTRL5 |= 0x00000200;   
		    break;
		    
		    case 13:
	      SGCC_IOCTRL_P->IO_CTRL4 &= 0xffffff0f;   //IO_CTRL4[6:4], for ADC0
	      SGCC_IOCTRL_P->IO_CTRL4 |= 0x00000010;   
		    break;
		    
		    case 14:
	      SGCC_IOCTRL_P->IO_CTRL4 &= 0xfffff0ff;   //IO_CTRL4[10:8], for ADC0
	      SGCC_IOCTRL_P->IO_CTRL4 |= 0x00000010;   
		    break;
		    
		    case 15:
	      SGCC_IOCTRL_P->IO_CTRL4 &= 0xffff0fff;   //IO_CTRL4[14:12], for ADC0
	      SGCC_IOCTRL_P->IO_CTRL4 |= 0x00001000;   
		    break;
		    
		    case 16:
	      SGCC_IOCTRL_P->IO_CTRL4 &= 0xfff0ffff;   //IO_CTRL4[18:16], for ADC0
	      SGCC_IOCTRL_P->IO_CTRL4 |= 0x00010000;   
		    break;
		    
		    default:
		    break;
	  }
    
		
		
}

/********************************************************
 Function:		GPIO_InitPad                                                                                                                                                                            
 input:      ioNo - P_GPIO0~P_UART5_RX
                     
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置GPIO PAD 复合为GPIO功能
********************************************************/
void GPIO_InitPad(UINT8 ioNo)
{
  switch(ioNo)
	{
		case 0:
			SGCC_IOCTRL_P->IO_CTRL4 &= ~((UINT32)7<<4);
			SGCC_IOCTRL_P->IO_CTRL4 |= (UINT32)4<<4;
			break;
		case 1: //rtc wrapper控制
			SGCC_RTC_P->RTC_PAD_cfg &=~((UINT32)7<<4);
			break;
		case 2: 
			SGCC_IOCTRL_P->IO_CTRL8 &= ~((UINT32)0x7<<16);
			break;
		case 3:
			SGCC_IOCTRL_P->IO_CTRL8 &= ~((UINT32)0x7<<20);
			break;
		case 4:
			SGCC_IOCTRL_P->IO_CTRLB &= ~((UINT32)0x7<<28);
			break;
		case 5:
			SGCC_IOCTRL_P->IO_CTRLC &= ~((UINT32)0x7<<0);
			break;
		case 6:
			SGCC_IOCTRL_P->IO_CTRLC &= ~((UINT32)0x7<<4);
			break;
		case 7:
			SGCC_IOCTRL_P->IO_CTRLC &= ~((UINT32)0x7<<8);
			break;
		case 8:
			SGCC_IOCTRL_P->IO_CTRLD &= ~((UINT32)0x7<<24);
			break;
		case 9:
			SGCC_IOCTRL_P->IO_CTRL10 &= ~((UINT32)0x7<<12);
			break;
		case 10:
			SGCC_IOCTRL_P->IO_CTRL0 &= ~((UINT32)0x7<<0);
			break;
		case 11:
			SGCC_IOCTRL_P->IO_CTRL0 &= ~((UINT32)0x7<<4);
			break;
		case 12:
			SGCC_IOCTRL_P->IO_CTRL0 &= ~((UINT32)0x7<<8);
			break;
		case 13:
			SGCC_IOCTRL_P->IO_CTRL0 &= ~((UINT32)0x7<<12);
			break;
		case 14:
			SGCC_IOCTRL_P->IO_CTRL0 &= ~((UINT32)0x7<<16);
			break;
		case 15://rtc wrapper控制
			SGCC_RTC_P->RTC_PAD_cfg &=~((UINT32)7<<12);
			SGCC_RTC_P->RTC_PAD_cfg |= ((UINT32)1<<12);
			break;
		case 16://rtc wrapper控制
			SGCC_RTC_P->RTC_PAD_cfg &=~((UINT32)7<<20);
			SGCC_RTC_P->RTC_PAD_cfg |= ((UINT32)1<<20);
			break;
		case 17:
			SGCC_IOCTRL_P->IO_CTRL1 &= ~((UINT32)0x7<<4);
			break;
		case 18:
			SGCC_IOCTRL_P->IO_CTRL1 &= ~((UINT32)0x7<<8);
			break;
		case 19:
			SGCC_IOCTRL_P->IO_CTRL1 &= ~((UINT32)0x7<<12);
			break;
		case 20:
			SGCC_IOCTRL_P->IO_CTRL1 &= ~((UINT32)0x7<<16);
			break;
		case 21:
			SGCC_IOCTRL_P->IO_CTRL1 &= ~((UINT32)0x7<<20);
			break;
		case 22:
			SGCC_IOCTRL_P->IO_CTRL1 &= ~((UINT32)0x7<<24);
			break;
		case 23:
			SGCC_IOCTRL_P->IO_CTRL2 &= ~((UINT32)0x7<<4);
			break;
		case 24:
			SGCC_IOCTRL_P->IO_CTRL2 &= ~((UINT32)0x7<<8);
			break;
		case 25:
			SGCC_IOCTRL_P->IO_CTRL2 &= ~((UINT32)0x7<<12);
			break;
		case 26:
			SGCC_IOCTRL_P->IO_CTRL2 &= ~((UINT32)0x7<<16);
			break;
		case 27:
			SGCC_IOCTRL_P->IO_CTRL2 &= ~((UINT32)0x7<<20);
			break;
		case 28:
			SGCC_IOCTRL_P->IO_CTRL2 &= ~((UINT32)0x7<<24);
			SGCC_IOCTRL_P->IO_CTRL2 |= ((UINT32)0x1<<24);
			break;
		case 29:
			SGCC_IOCTRL_P->IO_CTRL2 &= ~((UINT32)0x7<<28);
			SGCC_IOCTRL_P->IO_CTRL2 |= ((UINT32)0x1<<28);
			break;
		case 30:
			SGCC_IOCTRL_P->IO_CTRL3 &= ~((UINT32)0x7<<4);
			SGCC_IOCTRL_P->IO_CTRL3 |= ((UINT32)0x1<<4);
			break;
		case 31:
			SGCC_IOCTRL_P->IO_CTRL3 &= ~((UINT32)0x7<<8);
			SGCC_IOCTRL_P->IO_CTRL3 |= ((UINT32)0x1<<8);
			break;
		case 32:
			SGCC_IOCTRL_P->IO_CTRL3 &= ~((UINT32)0x7<<12);
			SGCC_IOCTRL_P->IO_CTRL3 |= ((UINT32)0x1<<12);
			break;
		case 33:
			SGCC_IOCTRL_P->IO_CTRL3 &= ~((UINT32)0x7<<16);
			SGCC_IOCTRL_P->IO_CTRL3 |= ((UINT32)0x1<<16);
			break;
		case 34:
			SGCC_IOCTRL_P->IO_CTRL4 &= ~((UINT32)0x7<<8);
			SGCC_IOCTRL_P->IO_CTRL4 |= ((UINT32)0x4<<8);
			break;
		case 35:
			SGCC_IOCTRL_P->IO_CTRL4 &= ~((UINT32)0x7<<12);
			SGCC_IOCTRL_P->IO_CTRL4 |= ((UINT32)0x4<<12);
			break;
		case 36:
			SGCC_IOCTRL_P->IO_CTRL4 &= ~((UINT32)0x7<<16);
			SGCC_IOCTRL_P->IO_CTRL4 |= ((UINT32)0x4<<16);
			break;
		case 37:
			SGCC_IOCTRL_P->IO_CTRL4 &= ~((UINT32)0x7<<28);
			break;	
		case 38:
			SGCC_IOCTRL_P->IO_CTRL5 &= ~((UINT32)0x7<<0);
			break;		
		case 39:
			SGCC_IOCTRL_P->IO_CTRL5 &= ~((UINT32)0x7<<4);
			break;		
		case 40:
			SGCC_IOCTRL_P->IO_CTRL5 &= ~((UINT32)0x7<<8);
			break;		
		case 41:
			SGCC_IOCTRL_P->IO_CTRL5 &= ~((UINT32)0x7<<12);
			break;			
		case 42:
			SGCC_IOCTRL_P->IO_CTRL5 &= ~((UINT32)0x7<<16);
			break;		
		case 43:
			SGCC_IOCTRL_P->IO_CTRL5 &= ~((UINT32)0x7<<20);
			break;		
		case 44:
			SGCC_IOCTRL_P->IO_CTRL5 &= ~((UINT32)0x7<<24);
			break;		
			
		case 45:
			SGCC_IOCTRL_P->IO_CTRL6 &= ~((UINT32)0x7<<0);
			break;
			
		case 46:
			SGCC_IOCTRL_P->IO_CTRL6 &= ~((UINT32)0x7<<4);
			break;
		
		case 47:
			SGCC_IOCTRL_P->IO_CTRL6 &= ~((UINT32)0x7<<16);
			break;
		case 48:
			SGCC_IOCTRL_P->IO_CTRL6 &= ~((UINT32)0x7<<20);
			break;
		case 49:
			SGCC_IOCTRL_P->IO_CTRL6 &= ~((UINT32)0x7<<24);
			break;
		case 50:
			SGCC_IOCTRL_P->IO_CTRL6 &= ~((UINT32)0x7<<28);
			break;
		case 51:
			SGCC_IOCTRL_P->IO_CTRL7 &= ~((UINT32)0x7<< 0);
			break;
		case 52:
			SGCC_IOCTRL_P->IO_CTRL7 &= ~((UINT32)0x7<< 4);
			break;
		case 53:
			SGCC_IOCTRL_P->IO_CTRL7 &= ~((UINT32)0x7<<8);
			break;
		case 54:
			SGCC_IOCTRL_P->IO_CTRL7 &= ~((UINT32)0x7<<12);
			break;		
		case 55:
			SGCC_IOCTRL_P->IO_CTRL7 &= ~((UINT32)0x7<<24);
			break;
		case 56:
			SGCC_IOCTRL_P->IO_CTRL7 &= ~((UINT32)0x7<< 28);
			break;
		case 57:
			SGCC_IOCTRL_P->IO_CTRL8 &= ~((UINT32)0x7<< 0);
			break;
		case 58:
			SGCC_IOCTRL_P->IO_CTRL8 &= ~((UINT32)0x7<<4);
			break;
		case 59:
			SGCC_IOCTRL_P->IO_CTRL8 &= ~((UINT32)0x7<<8);
			break;			
		case 60:
			SGCC_IOCTRL_P->IO_CTRL8 &= ~((UINT32)0x7<<12);
			break;		
		case 61:
			SGCC_IOCTRL_P->IO_CTRL9 &= ~((UINT32)0x7<<0);
			SGCC_IOCTRL_P->IO_CTRL9 |= ((UINT32)0x1<<0);
			break;	
		case 62:
			SGCC_IOCTRL_P->IO_CTRL9 &= ~((UINT32)0x7<<4);
			SGCC_IOCTRL_P->IO_CTRL9 |= ((UINT32)0x1<<4);
			break;		
		case 63:
			SGCC_IOCTRL_P->IO_CTRL9 &= ~((UINT32)0x7<<8);
			SGCC_IOCTRL_P->IO_CTRL9 |= ((UINT32)0x1<<8);
			break;		
		case 64:
			SGCC_IOCTRL_P->IO_CTRL9 &= ~((UINT32)0x7<<12);
			SGCC_IOCTRL_P->IO_CTRL9 |= ((UINT32)0x1<<12);
			break;	
		case 65:
			SGCC_IOCTRL_P->IO_CTRL9 &= ~((UINT32)0x7<<16);
			break;
		
		case 66:
			SGCC_IOCTRL_P->IO_CTRL9 &= ~((UINT32)0x7<<20);
			break;
		case 67:
			SGCC_IOCTRL_P->IO_CTRL9 &= ~((UINT32)0x7<<24);
			break;
		case 68:
			SGCC_IOCTRL_P->IO_CTRL9 &= ~((UINT32)0x7<<28);
			break;
		case 69:
			SGCC_IOCTRL_P->IO_CTRLA &= ~((UINT32)0x7<<0);
			break;
		case 70:
			SGCC_IOCTRL_P->IO_CTRLA &= ~((UINT32)0x7<< 4);
			break;
		case 71:
			SGCC_IOCTRL_P->IO_CTRLA &= ~((UINT32)0x7<<16);
			break;
		
		case 72:
			SGCC_IOCTRL_P->IO_CTRLA &= ~((UINT32)0x7<<20);
			break;
		case 73:
			SGCC_IOCTRL_P->IO_CTRLA &= ~((UINT32)0x7<<24);
			break;
		case 74:
			SGCC_IOCTRL_P->IO_CTRLA &= ~((UINT32)0x7<<28);
			break;
		case 75:
			SGCC_IOCTRL_P->IO_CTRLB &= ~((UINT32)0x7<<0);
			break;
		case 76:
			SGCC_IOCTRL_P->IO_CTRLB &= ~((UINT32)0x7<< 4);
			break;	
		case 77:
			SGCC_IOCTRL_P->IO_CTRLB &= ~((UINT32)0x7<<8);
			break;
		case 78:
			SGCC_IOCTRL_P->IO_CTRLB &= ~((UINT32)0x7<<12);
			break;
		case 79:
			SGCC_IOCTRL_P->IO_CTRLB &= ~((UINT32)0x7<< 16);
			break;		
		case 80:
			SGCC_IOCTRL_P->IO_CTRLC &= ~((UINT32)0x7<<12);
			SGCC_IOCTRL_P->IO_CTRLC |= ((UINT32)0x4<<12);
			break;		
		case 81:
			SGCC_IOCTRL_P->IO_CTRLC &= ~((UINT32)0x7<<16);
			SGCC_IOCTRL_P->IO_CTRLC |= ((UINT32)0x3<<16);
			break;			
		case 82:
			SGCC_IOCTRL_P->IO_CTRLC &= ~((UINT32)0x7<<20);
			SGCC_IOCTRL_P->IO_CTRLC |= ((UINT32)0x3<<20);
			break;		
		case 83:
			SGCC_IOCTRL_P->IO_CTRLC &= ~((UINT32)0x7<< 24);
			break;			
			
		case 84:
			SGCC_IOCTRL_P->IO_CTRLC &= ~((UINT32)0x7<< 28);
			break;	
		case 85:
			SGCC_IOCTRL_P->IO_CTRLD &= ~((UINT32)0x7<< 20);
			SGCC_IOCTRL_P->IO_CTRLD |= ((UINT32)0x2<< 20);
			break;		
		case 86:
			SGCC_IOCTRL_P->IO_CTRLD &= ~((UINT32)0x7<< 28);
			SGCC_IOCTRL_P->IO_CTRLD |= ((UINT32)0x2<< 28);
			break;		
		case 87:
			SGCC_IOCTRL_P->IO_CTRLE &= ~((UINT32)0x7<< 0);
			SGCC_IOCTRL_P->IO_CTRLE |= ((UINT32)0x2<< 0);
			break;	
		
	  case 88:
			SGCC_IOCTRL_P->IO_CTRLE &= ~((UINT32)0x7<< 4);
			break;			
			
		case 89:
			SGCC_IOCTRL_P->IO_CTRLE &= ~((UINT32)0x7<< 8);
			break;	
			
		case 90:
			SGCC_IOCTRL_P->IO_CTRLE &= ~((UINT32)0x7<< 12);
			SGCC_IOCTRL_P->IO_CTRLE |= ((UINT32)0x2<< 12);
			break;	
		
		case 91:
			SGCC_IOCTRL_P->IO_CTRLE &= ~((UINT32)0x7<< 16);
			break;	
			
		case 92:
			SGCC_IOCTRL_P->IO_CTRLE &= ~((UINT32)0x7<< 20);
			break;	
		
		case 93:
			SGCC_IOCTRL_P->IO_CTRLE &= ~((UINT32)0x7<< 24);
			break;		
		
		case 94:
			SGCC_IOCTRL_P->IO_CTRLF &= ~((UINT32)0x7<< 4);
			break;
		
		case 95:
			SGCC_IOCTRL_P->IO_CTRLF &= ~((UINT32)0x7<< 8);
			break;
	  case 96:
			SGCC_IOCTRL_P->IO_CTRLF &= ~((UINT32)0x7<< 12);
			break;
		
		case 97:
			SGCC_IOCTRL_P->IO_CTRLF &= ~((UINT32)0x7<< 16);
			break;								
		case 98:
			SGCC_IOCTRL_P->IO_CTRLF &= ~((UINT32)0x7<<20);
			break;
			
		case 99:
			SGCC_IOCTRL_P->IO_CTRLF &= ~((UINT32)0x7<< 24);
			break;								
		case 100:
			SGCC_IOCTRL_P->IO_CTRLF &= ~((UINT32)0x7<<28);
			break;	
		case 101:
			SGCC_IOCTRL_P->IO_CTRL10 &= ~((UINT32)0x7<<0);
			break;		
		case 102:
			SGCC_IOCTRL_P->IO_CTRL10 &= ~((UINT32)0x7<< 16);
			SGCC_IOCTRL_P->IO_CTRL10 |= ((UINT32)0x2<< 16);
			break;		
		
		case 103:
			SGCC_IOCTRL_P->IO_CTRL10 &= ~((UINT32)0x7<< 20);
			SGCC_IOCTRL_P->IO_CTRL10 |= ((UINT32)0x2<< 20);
			break;		
			
		case 104:
			SGCC_IOCTRL_P->IO_CTRL10 &= ~((UINT32)0x7<<24);
			break;			
			
		case 105:
			SGCC_IOCTRL_P->IO_CTRL10 &= ~((UINT32)0x7<<28);
			SGCC_IOCTRL_P->IO_CTRL10 |= ((UINT32)0x4<< 28);
			break;			
		case 106:
			SGCC_IOCTRL_P->IO_CTRL11 &= ~((UINT32)0x7<<0);
			SGCC_IOCTRL_P->IO_CTRL11 |= ((UINT32)0x4<< 0);
			break;	
		
		case 107:
			SGCC_IOCTRL_P->IO_CTRL11 &= ~((UINT32)0x7<<8);
			SGCC_IOCTRL_P->IO_CTRL11 |= ((UINT32)0x3<< 8);
			break;	
			
		case 108:
			SGCC_IOCTRL_P->IO_CTRL11 &= ~((UINT32)0x7<<12);
			SGCC_IOCTRL_P->IO_CTRL11 |= ((UINT32)0x3<< 12);
			break;	
		
		case 109:
			SGCC_IOCTRL_P->IO_CTRL11 &= ~((UINT32)0x7<<16);
			break;	
		case 110:
			SGCC_IOCTRL_P->IO_CTRL11 &= ~((UINT32)0x7<<20);
			break;		
		
		case 111:
			SGCC_IOCTRL_P->IO_CTRLD &= ~((UINT32)0x7<<4);
			break;
			
		default:
			break;
					
	}
}

void NandFlashCtrl_InitPad(void)
{
	SGCC_IOCTRL_P->IO_CTRL7 &= ~(0xFFF<<4); // D4_NFC、D5_NFC、D6_NFC
	SGCC_IOCTRL_P->IO_CTRL7 |= (0x333<<4);
	
	SGCC_IOCTRL_P->IO_CTRL7 &= ~(0xF<<24); // FSMC_D7_NFC
	SGCC_IOCTRL_P->IO_CTRL7 |= (0x3<<24);
	
	SGCC_IOCTRL_P->IO_CTRLF &= ~(0xFF<<4); //RB2 NCE2
	SGCC_IOCTRL_P->IO_CTRLF |= (0x31<<4);	
	
//		SGCC_IOCTRL_P->IO_CTRLF &= ~(0xFFFF<<4); //RB2 NCE2 NCE3 NCE4
//	SGCC_IOCTRL_P->IO_CTRLF |= (0x3331<<4);
	
	SGCC_IOCTRL_P->IO_CTRL9 &= ~(0xFUL<<28); //FSMC_CLE_NFC
	SGCC_IOCTRL_P->IO_CTRL9 |= (0x3<<28);
	
	SGCC_IOCTRL_P->IO_CTRLA &= ~(0xF<<0); // FSMC_ALE_NFC
	SGCC_IOCTRL_P->IO_CTRLA |= (0x3<<0);
	
	SGCC_IOCTRL_P->IO_CTRLA &= ~(0xFF<<16); //D0_NFC、D1_NFC 
	SGCC_IOCTRL_P->IO_CTRLA |= (0x33<<16);
	
	SGCC_IOCTRL_P->IO_CTRL2 &=~(0X07<<4);//FSMC_NIORD(out, FMC_WP_NFC)
	SGCC_IOCTRL_P->IO_CTRL2 |=(0X01<<4);
	
	SGCC_IOCTRL_P->IO_CTRLB &= ~(0xFF<<8); // RB3 RB4
	SGCC_IOCTRL_P->IO_CTRLB |= (0x33<<8);
	
	SGCC_IOCTRL_P->IO_CTRLE &= ~(0xFF<<4); // D2_NFC D3_NFC
	SGCC_IOCTRL_P->IO_CTRLE |= (0x22<<4);
	
	SGCC_IOCTRL_P->IO_CTRLE &= ~(0xFF<<20); // FSMC_NRE_NFC(out) FSMC_NWE_NFC(out)
	SGCC_IOCTRL_P->IO_CTRLE |= (0x22<<20);
	


	
	
}
/********************************************************
 Function:		RTCTM_InitPad                                                                                                                                                                            
 input:      None              
 output:     None                                        
 Returns:    None                                                                                      
 Description: 相关管脚配置为RTC_TM
********************************************************/
void RTCTM_InitPad()
{	  
	  UINT32 temp;
	  temp = SGCC_RTC_P->RTC_PAD_cfg;
	  
	  temp |= (0x3 << 4);
	  
	  SGCC_RTC_P->RTC_PAD_cfg = temp;
}
