/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子科技有限公司                                       */
/*                                                                            */
/* 文件名称: SCM62X_CLK.c                                                     */
/* 内容摘要: CLK模块基础函数定义                                              */
/* 其它说明: **                                                               */
/* 当前版本: V1.0                                                             */
/* 作    者: SCM                                                              */
/* 完成日期: 2019-03-20                                                       */
/* 修改记录:                                                                  */
/*    修改日期          版本号        修改人        修改内容                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0           SCM        创建文件                    */
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
uint32_t g_SystemClk = 60000000UL;

uint32_t g_SystemHclk = 30000000UL;

uint32_t g_SystemPclk = 150000000UL;

/*-----------------------------------------------------------------------------
  Function:		CLK_EnAllClk                                                                                                                                                                            
  input:     	None                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 使能所有模块时钟                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnAllClk() 
{
    SGCC_CLK_P->SYSCLK_CTRL |= (SYSCLK_CTRL_GMAC_PADCLK_EN | SYSCLK_CTRL_PHY_30M_EN);
	SGCC_CLK_P->HCLK_EN = 0x1fff;
	SGCC_CLK_P->PCLK0_EN |= 0x1fffbf;  
	SGCC_CLK_P->PCLK1_EN |= 0x7fffff;  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_DisAllPeriClk                                                                                                                                                                            
  input:     	None                  	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 不使能所油馍栊模块时钟                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_DisAllPeriClk() 
{
    SGCC_CLK_P->SYSCLK_CTRL &= ~(SYSCLK_CTRL_GMAC_PADCLK_EN | SYSCLK_CTRL_PHY_30M_EN);
	SGCC_CLK_P->HCLK_EN = 1;
	SGCC_CLK_P->PCLK0_EN = 0x0;  
	SGCC_CLK_P->PCLK1_EN = 0x6000;  
}

/*-----------------------------------------------------------------------------
  Function:		EnPeriClk                                                                                                                                                                           
  input:     	module-外设模块                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 使能某外设模块时钟 ,IOCTRL没有单独的时钟使能位                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnPeriClk(UINT8 module) 
{
	switch(module)
	{
	    case GPIO:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_GPIO_DB | PCLK0_EN_GPIO);
	    	break;
      case UART0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART0;
	    	break;
	    case UART1:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART1;
	    	break;
	    case UART2:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART2;
	    	break;
        case UART3:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_UART3;
	    	break;
	    case UART4:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART4;
	    	break;
	    case UART5:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART5;
	    	break;
	    case UART6:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART6;
	    	break;
        case UART7:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_UART7;
	    	break;	
      case DMA0:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA0;
	    	break;    
	    case DMA1:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA1;
	    	break;   	
	    case DMA2:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA2;
	    	break;    	
	    case DMA3:
	    	SGCC_CLK_P->HCLK_EN |= HCLK_EN_DMA3;
	    	break;   	
	    case I2C0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_I2C0;
	    	break;
	    case I2C1:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_I2C1;
	    	break;
	   case I2C2:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_I2C2;
	    	break; 	
     case U7816:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_7816;
	    	break;
     case PWMA:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_PWMA;
	    	break;
	   case PWMB:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_PWMB;
	    	break;
     case SPI0:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_SPI0 | PCLK0_EN_SPI_MST0);
	    	break;
	   case SPI1:
	    	SGCC_CLK_P->PCLK0_EN |= (PCLK0_EN_SPI1 | PCLK0_EN_SPI_MST1);
	    	break;    
     case SPI2:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST2);
	    	break;      
     case SPI3:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST3);
	    	break;      
	   case CRC:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_CRC;
	    	break;
     case ANACTRL0:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_ANA0 | PCLK1_EN_ANA0_W1);
	    	break;
     case ADC0:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_ADC0_CLK;
	    	break;
     case WDT:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_WDT;
	    	break;
	   case TIMERA:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_TIMERA;
	    	break;
	   case TIMERB:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_TIMERB;
	    	break;
	   case TIMERC:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_TIMERC;
	    	break;
	   case TIMERD:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_TIMERD;
	    	break;
	   case RTC:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_RTC;
	    	break;
	   case GMAC0:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_GMAC0;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case GMAC1:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_GMAC1;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case OTG:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_OTG;
	      SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PHY_30M_EN;
	    	break;
	   case FLASH:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_FLASH;
	    	break; 	
	   case XSRAM:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_XSRAM;
	    	break; 	
	   case NFC:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_NFC;
	    	break; 	 	
	   case AES:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_AES;
	    	break; 	 	
	   case SM4:
	      SGCC_CLK_P->HCLK_EN |= HCLK_EN_SM4;
	    	break; 	 	
	   case CAN0:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_CAN0;
	    	break; 	
	   case CAN1:
	    	SGCC_CLK_P->PCLK0_EN |= PCLK0_EN_CAN1;
	    	break; 	 	
		 case CAN2:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_CAN2;
	    	break; 	
		 
		 case ANACTRL1:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_ANA1 | PCLK1_EN_ANA1_W1);
	    	break;
     case ADC1:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_ADC1_CLK;
	    	break;
		 
		 case ANACTRL2:
	    	SGCC_CLK_P->PCLK1_EN |= (PCLK1_EN_ANA2 | PCLK1_EN_ANA2_W1);
	    	break;
     case ADC2:
	    	SGCC_CLK_P->PCLK1_EN |= PCLK1_EN_ADC2_CLK;
	    	break;
		 
	    default:
            break;
	    
	}
	
}

/*-----------------------------------------------------------------------------
  Function:		CLK_DisPeriClk                                                                                                                                                                           
  input:     	module-外设模块                	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 不使能某外设模块时钟                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CLK_DisPeriClk(UINT8 module) 
{	
	switch(module)
	{
	    case GPIO:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_GPIO_DB | PCLK0_EN_GPIO);
	    	break;
      case UART0:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART0;
	    	break;
	    case UART1:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART1;
	    	break;
	    case UART2:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART2;
	    	break;
        case UART3:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_UART3;
	    	break;
	    case UART4:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_UART4;
	    	break;
	    case UART5:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_UART5;
	    	break;
	    case UART6:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_UART6;
	    	break;
        case UART7:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_UART7;
	    	break;	
      case DMA0:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA0;
	    	break;    
	    case DMA1:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA1;
	    	break;   	
	    case DMA2:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA2;
	    	break;    	
	    case DMA3:
	    	SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_DMA3;
	    	break;   	
	    case I2C0:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_I2C0;
	    	break;
	    case I2C1:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_I2C1;
	    	break;
	   case I2C2:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_I2C2;
	    	break; 	
     case U7816:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_7816;
	    	break;
     case PWMA:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_PWMA;
	    	break;
	   case PWMB:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_PWMB;
	    	break;
     case SPI0:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_SPI0 | PCLK0_EN_SPI_MST0);
	    	break;
	   case SPI1:
	    	SGCC_CLK_P->PCLK0_EN &= ~(PCLK0_EN_SPI1 | PCLK0_EN_SPI_MST1);
	    	break;    
     case SPI2:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST2);
	    	break;      
     case SPI3:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_SPI2 | PCLK1_EN_SPI_MST3);
	    	break;      
	   case CRC:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_CRC;
	    	break;
     case ANACTRL0:
	    	SGCC_CLK_P->PCLK1_EN &= ~(PCLK1_EN_ANA0 | PCLK1_EN_ANA0_W1);
	    	break;
     case ADC0:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_ADC0_CLK;
	    	break;
     case WDT:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_WDT;
	    	break;
	   case TIMERA:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_TIMERA;
	    	break;
	   case TIMERB:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_TIMERB;
	    	break;
	   case TIMERC:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_TIMERC;
	    	break;
	   case TIMERD:
	    	SGCC_CLK_P->PCLK1_EN &= ~PCLK1_EN_TIMERD;
	    	break;
	   case RTC:
	    	SGCC_CLK_P->PCLK0_EN &= ~PCLK0_EN_RTC;
	    	break;
	   case GMAC0:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_GMAC0;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case GMAC1:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_GMAC1;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_GMAC_PADCLK_EN;
	    	break;
	   case OTG:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_OTG;
	      SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PHY_30M_EN;
	    	break;
	   case FLASH:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_FLASH;
	    	break; 	
		 
		 case XSRAM:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_XSRAM;
	    	break; 	
	   case NFC:
	      SGCC_CLK_P->HCLK_EN &= ~ HCLK_EN_NFC;
	    	break; 	 	
	   case AES:
	      SGCC_CLK_P->HCLK_EN &= ~ HCLK_EN_AES;
	    	break; 	 	
	   case SM4:
	      SGCC_CLK_P->HCLK_EN &= ~HCLK_EN_SM4;
	    	break; 	 	
	   case CAN0:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_CAN0;
	    	break; 	
	   case CAN1:
	    	SGCC_CLK_P->PCLK0_EN &= ~ PCLK0_EN_CAN1;
	    	break; 	 	
		 case CAN2:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_CAN2;
	    	break; 	
		 
		 case ANACTRL1:
	    	SGCC_CLK_P->PCLK1_EN &= ~ (PCLK1_EN_ANA1 | PCLK1_EN_ANA1_W1);
	    	break;
     case ADC1:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_ADC1_CLK;
	    	break;
		 
		 case ANACTRL2:
	    	SGCC_CLK_P->PCLK1_EN &= ~ (PCLK1_EN_ANA2 | PCLK1_EN_ANA2_W1);
	    	break;
     case ADC2:
	    	SGCC_CLK_P->PCLK1_EN &= ~ PCLK1_EN_ADC2_CLK;
	    	break;
		 
	    default:
            break;
	    
	}
	    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableMatrix                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable matrix hclk                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableMatrix(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= HCLK_EN_MTX;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~HCLK_EN_MTX;    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableHosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable HOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableHosc(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_HOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_HOSC_EN; 
        
    while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_HCLK_RD));   
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableLosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable LOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableLosc(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_LOSC_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_LOSC_EN;    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnablePll                                                                                                                                                                          
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable PLL                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnablePll(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_GPIO_P->DIR = 0x1 ;
        
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PLL_EN;
        SGCC_GPIO_P->DR = 0x1 ;
        //wait until PLL locked
        while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_PLL_LOCKED_O));  
        SGCC_GPIO_P->DR = 0x0 ;
    }
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_EN;  
        
    
    
}
/*-----------------------------------------------------------------------------
  Function:		CLK_EnableXhosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XHOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableXhosc(FunctionalState opt) 
{
    if(opt == ENABLE)
    {
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_XHOSC_EN;
        //wait until xhosc ready
        while(!(SGCC_CLK_P->SYSCLK_CTRL & SYSCLK_CTRL_XHOSC_RD));
    }
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_XHOSC_EN;    
        
    
    
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnableXlosc                                                                                                                                                                           
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable XLOSC                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnableXlosc(FunctionalState opt) 
{
	UINT32 delay;
    if(opt == ENABLE)
        SGCC_RTC_P->RTC_CLKRST_cfg |= SYSCLK_CTRL_XLOSC_EN;//
    else
        SGCC_RTC_P->RTC_CLKRST_cfg &= ~SYSCLK_CTRL_XLOSC_EN;    
    delay = 0x7fffff;
		while(delay--);
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectHclk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV1/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV2/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV4/SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV8
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC时钟为1M/2MHz                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectHclk(UINT32 clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_HCLK_SEL;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectPclk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_PCLK_SEL_HCLK_DIV2/SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC时钟为1M/2MHz                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectPclk(UINT32 clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_PCLK_SEL;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigAdcPllDiv                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ADC_PLL_DIV_32/SYSCLK_CTRL_ADC_PLL_DIV_64/SYSCLK_CTRL_ADC_PLL_DIV_128/SYSCLK_CTRL_ADC_PLL_DIV_256                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC使用PLL的分频系数                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_ConfigAdcPllDiv(UINT32 clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ADC_PLL_DIV;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigAdcHoscDiv                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ADC_HOSC_DIV_8/SYSCLK_CTRL_ADC_HOSC_DIV_16/SYSCLK_CTRL_ADC_HOSC_DIV_32/SYSCLK_CTRL_ADC_HOSC_DIV_64                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择ADC使用HOSC的分频系数                                                                                               	                                   
 *-----------------------------------------------------------------------------*/
void CLK_ConfigAdcHoscDiv(UINT32 clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ADC_HOSC_DIV;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}
/*-----------------------------------------------------------------------------
  Function:		CLK_SelectRtcClk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_RTC_CLK_SEL_XLOSC/SYSCLK_CTRL_RTC_CLK_SEL_EXTAL32
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择RTC时钟为XLOSC或者external 32K                                                                                              	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectRtcClk(UINT32 clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_RTC_CLK_SEL;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_SelectGpioDbClk                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_GPIO_DBCLK_SEL_LOSC/SYSCLK_CTRL_GPIO_DBCLK_SEL_PCLK
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择GPIO DBCLK为LOSC或者PCLK                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectGpioDbClk(UINT32 clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_GPIO_DBCLK_SEL;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}
/*-----------------------------------------------------------------------------
  Function:		CLK_SelectClkOut                                                                                                                                                                            
  input:     	clk: SYSCLK_CTRL_ANA_CLK_MUX_LOSC/SYSCLK_CTRL_ANA_CLK_MUX_XLOSC/
              SYSCLK_CTRL_ANA_CLK_MUX_XHOSC_DIV4/SYSCLK_CTRL_ANA_CLK_MUX_HOSC_DIV16/
              SYSCLK_CTRL_ANA_CLK_MUX_PLL_DIV64     
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择CLK_OUT时钟源                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CLK_SelectClkOut(UINT32 clk) 
{
	UINT32 temp;
	temp = SGCC_CLK_P->SYSCLK_CTRL;
	  
	temp &= ~SYSCLK_CTRL_ANA_CLK_MUX;
	temp |= clk ;
	  
	SGCC_CLK_P->SYSCLK_CTRL = temp;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_EnClkOut                                                                                                                                                                          
  input:     	opt: ENABLE/DISABLE                 	  
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: enable or disable clk_out                                                                                             	                                   
 *-----------------------------------------------------------------------------*/
void CLK_EnClkOut(FunctionalState opt) 
{
    if(opt == ENABLE)
        SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_ANA_CLK_EN;
    else
        SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_ANA_CLK_EN;    
    
}


/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigPll                                                                                                                                                                            
  input:     	pllConfig
 typedef struct
 {
    UINT32 refDiv;  //PLL_CTRL_PLL_REFDIV_1~4
    UINT32 fbDiv;   //PLL_CTRL_PLL_FBDIV_(n), n=0x0~0x7f
    UINT32 outDiv;  //PLL_CTRL_PLL_OUTDIV_1~4
    UINT32 refClk;  //PLL_CTRL_PLL_REF_XHOSC/PLL_CTRL_PLL_REF_HOSC/PLL_CTRL_PLL_REF_PAD50M
    UINT32 icpSelA; //PLL_CTRL_PLL_ICPSELA_0~1
    UINT32 icpSelB; //PLL_CTRL_PLL_ICPSELB_0~1
    UINT32 rSel;   //PLL_CTRL_PLL_RSEL_(n)
 } PLL_CONFIG ;    
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择CLK_OUT时钟源                                                                                            	                                   
 *-----------------------------------------------------------------------------*/
void CLK_ConfigPll(PLL_CONFIG pllConfig) 
{
	SGCC_CLK_P->PLL_CTRL = pllConfig.refDiv | pllConfig.fbDiv | pllConfig.outDiv | pllConfig.refClk | pllConfig.icpSelA | pllConfig.icpSelB | pllConfig.rSel;
	  
}

/*-----------------------------------------------------------------------------
  Function:		CLK_ConfigMainClk                                                                                                                                                                            
  input:     	clk--HOSC/LOSC/PLL    
  output:     None                                                                                    
  Returns:    None                                                                                      
  Description: 选择系统主时钟                                                                                                	                                   
 *-----------------------------------------------------------------------------*/
void CLK_ConfigMainClk(UINT8 clk) 
{
  
    switch(clk)
	{			
	  	case LOSC:
          CLK_EnableLosc(ENABLE);
          SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_OSC_SEL;
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_SEL;
	  	    break;
			
	  	case HOSC:
          CLK_EnableHosc(ENABLE);
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_OSC_SEL;
          SGCC_CLK_P->SYSCLK_CTRL &= ~SYSCLK_CTRL_PLL_SEL;
	  	    break;
	  	    
      case PLL:
      	  CLK_EnablePll(ENABLE) ;
          SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_OSC_SEL;
          SGCC_CLK_P->SYSCLK_CTRL |= SYSCLK_CTRL_PLL_SEL;
      
	  	    break;
	  	      
      default:
            break;
	  }
}

