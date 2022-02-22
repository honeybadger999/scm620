
#include "bsp.h"

SGCC_UART_TypeDef*  PrintPort = SGCC_UART3_P;

void LED_GPIO_Init(void)
{
	GPIO_InitPad(LED0);                                                              
	GPIO_InitPad(LED1);                                                              
	GPIO_InitPad(LED2);                                                              
	GPIO_InitPad(LED3);                                                

	GPIO_ConfigDir(LED0,GPIO_OUTPUT);
	GPIO_ConfigDir(LED1,GPIO_OUTPUT);
	GPIO_ConfigDir(LED2,GPIO_OUTPUT);
	GPIO_ConfigDir(LED3,GPIO_OUTPUT);
}

void UART_Config_Init(void)
{
	UART3_InitPad();         //默认开发板串口3打印
	UART_Init(SGCC_UART3_P, 115200, SCM_PARITY_NONE, UART_CTRL_STOP_1BIT);               //注意pclk SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4 为18.75M				
}

//重映射串口打印
void rt_hw_console_output( const char *str )
{    
	/* 进入临界段 */    
	rt_enter_critical(); 

	/* 直到字符串结束 */
	while ( *str != '\0' )
	{
			/* 换行 */
			//RT-Thread 系统中已有的打印均以 \n 结尾，而并非 \r\n，所以在字符输出时，需要在输出 \n 之前输出 \r，完成回车与换行，否则系统打印出来的信息将只有换行
			if ( *str == '\n' )
			{
					UART_PutChar( PrintPort, '\r' );
					while ( UART_GetFlagStatus( PrintPort, UART_STATUS_TXEMP ) == RESET );
			}

			UART_PutChar( PrintPort, *str++ );
			while ( UART_GetFlagStatus( PrintPort, UART_STATUS_TXEMP ) == RESET );
	}
		
	/* 退出临界段 */    
	rt_exit_critical();
}

char rt_hw_console_getchar(void)
{
	 //查询方式实现，记得将UART初始化中的中断接收配置相关代码注释掉    
	int ch = -1;   
	
  if( UART_GetFlagStatus(PrintPort, UART_STATUS_RXEMP) == RESET )    
  {        
		ch = ( int )UART_GetChar(PrintPort);   
//    PrintPort->STATUS = UART_STATUS_RXFULL;		 
	}
  else    
  {        
		if( UART_GetFlagStatus(PrintPort, UART_STATUS_RXFIFO_OVERRUN) != RESET )       
    {            
			PrintPort->STATUS = UART_STATUS_RXFIFO_OVERRUN;       
    } 
    rt_thread_mdelay( 10 );    
  }  
  
  return ch;
}

//void UART3_IRQHandler(void)
//{
//    UINT32 intrStatus, intrEnSts;
//    UINT8 rxData;
//	  SGCC_UART_TypeDef* UARTx ;
//    UARTx  = (SGCC_UART_TypeDef*)SGCC_UART3_BASE;
//    intrStatus = UARTx->INTSTA;
//    intrEnSts = UARTx->CTRL;      
//		
//		  
//	if(intrStatus & UART_INTSTATUS_stop_error)
//	{
//	  UART_ClearIntr(UARTx,UART_INTSTATUS_stop_error); //clr int bit
//	}
//	if(intrStatus & UART_INTSTATUS_parity_error)
//	{
//		UART_ClearIntr(UARTx,UART_INTSTATUS_parity_error); //clr int bit
//	}	
//  if((intrStatus & UART_INTSTATUS_RXINT) && (intrEnSts & UART_CTRL_RXINTEN) )
//  {
//		//clear interrupt
//		UART_ClearIntr(UARTx,UART_INTSTATUS_RXINT);
//		rxData = UART_GetChar(UARTx);
//	}
//	if((intrStatus & UART_INTSTATUS_TXINT) && (intrEnSts & UART_CTRL_TXINTEN) )	
//    {
//		//clear interrupt
//    UART_ClearIntr(UARTx,UART_INTSTATUS_TXINT);
////		TxCount++;
////		if(TxCount==10)
////		UART_EnableIntr(UARTx,UART_CTRL_RXINTEN|UART_CTRL_TXINTDIS);
//	}
//}
