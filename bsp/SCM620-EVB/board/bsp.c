
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
	UART3_InitPad();         //Ĭ�Ͽ����崮��3��ӡ
	UART_Init(SGCC_UART3_P, 115200, SCM_PARITY_NONE, UART_CTRL_STOP_1BIT);               //ע��pclk SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4 Ϊ18.75M				
}

//��ӳ�䴮�ڴ�ӡ
void rt_hw_console_output( const char *str )
{    
	/* �����ٽ�� */    
	rt_enter_critical(); 

	/* ֱ���ַ������� */
	while ( *str != '\0' )
	{
			/* ���� */
			//RT-Thread ϵͳ�����еĴ�ӡ���� \n ��β�������� \r\n���������ַ����ʱ����Ҫ����� \n ֮ǰ��� \r����ɻس��뻻�У�����ϵͳ��ӡ��������Ϣ��ֻ�л���
			if ( *str == '\n' )
			{
					UART_PutChar( PrintPort, '\r' );
					while ( UART_GetFlagStatus( PrintPort, UART_STATUS_TXEMP ) == RESET );
			}

			UART_PutChar( PrintPort, *str++ );
			while ( UART_GetFlagStatus( PrintPort, UART_STATUS_TXEMP ) == RESET );
	}
		
	/* �˳��ٽ�� */    
	rt_exit_critical();
}

char rt_hw_console_getchar(void)
{
	 //��ѯ��ʽʵ�֣��ǵý�UART��ʼ���е��жϽ���������ش���ע�͵�    
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
