/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: ������о΢����                                                   */
/*                                                                            */
/* �ļ�����: SCM62X_UART.c                                                    */
/* ����ժҪ: UARTģ�������������                                             */
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
  Function:		UART_RestoreReg                                                                                                                                              
  input:     UARTx: select the UART 
  Returns:    None                                                                                      
  Description:                                                                                      
               	Initialize UARTx, configure baud rate, parity, stop bit, etc..                                    
 *-----------------------------------------------------------------------------*/
void UART_RestoreReg (SGCC_UART_TypeDef* UARTx)
{
    UARTx->CTRL = 0;
    
    UARTx->BAUDDIV = 0;
}

/*-----------------------------------------------------------------------------
  Function:		UART_Init                                                                         
                                                                                                         
  input:     UARTx: select the UART 
             baudrate: baud rate
             parity: PARITY_NONE/PARITY_ODD/PARITY_EVEN
             stopNum: stop bit, UART_CTRL_STOP_1BIT\UART_CTRL_STOP_2BIT
	        	                                     
  Returns:    None                                                                                      
  Description:                                                                                      
               	Initialize UARTx, configure baud rate, parity, stop bit, etc..                                    
 *-----------------------------------------------------------------------------*/
void UART_Init (SGCC_UART_TypeDef* UARTx, UINT32 baudrate, UINT32 parity, UINT32 stopNum)
{
    UARTx->CTRL = 0;
	//configure baudrate register
    UARTx->BAUDDIV = (UINT32)(UART_CLK/baudrate);//;
    
    //configure data frame parameters
    if(parity == PARITY_NONE)
    {
	    UARTx->CTRL = UART_CTRL_STOPBIT_CHECK | stopNum | UART_CTRL_RXEN | UART_CTRL_TXEN ; 
    }
    else
    {
        UARTx->CTRL = parity | UART_CTRL_PARITY_EN | UART_CTRL_STOPBIT_CHECK | stopNum | UART_CTRL_RXEN | UART_CTRL_TXEN; 
    }
}

/*-----------------------------------------------------------------------------
  Function:		UART_EnableIntr                                                                                                                                                                                
  input:    UARTx: select the UART 
            IntMask:      UART_CTRL_RXINTEN or UART_CTRL_TXINTEN                  
  Returns:    None                                                                                      
  Description:                                                                                            
               	                                  
 *-----------------------------------------------------------------------------*/
void UART_EnableIntr(SGCC_UART_TypeDef* UARTx,UINT32 intMask)
{
    if(intMask & UART_CTRL_TXINTEN)
    {
	    UARTx->CTRL |= UART_CTRL_TXINTEN;
    }
    else
    {
        UARTx->CTRL &= ~UART_CTRL_TXINTEN;
    }
    if(intMask & UART_CTRL_RXINTEN)
    {
	    UARTx->CTRL |= UART_CTRL_RXINTEN;
    }
    else
    {
        UARTx->CTRL &= ~UART_CTRL_RXINTEN;
    }
    
}

/*-----------------------------------------------------------------------------
  Function:		UART_ClearIntr                                                                                                                                                                                
  input:    UARTx: select the UART 
            intType:      UART_INTSTATUS_RXINT or UART_INTSTATUS_TXINT                  
  Returns:    None                                                                                      
  Description:                                                                                            
               	                                  
 *-----------------------------------------------------------------------------*/
void UART_ClearIntr(SGCC_UART_TypeDef* UARTx,UINT32 intType)
{
	UARTx->INTSTA = intType;
}
/*-----------------------------------------------------------------------------
  Function:		UART_GetChar                                                                       
  input:    UARTx: select the UART                                  
  Returns:    ch                                                                                      
  Description:  get received data                                                                                          
               	                                  
 *-----------------------------------------------------------------------------*/
UINT8 UART_GetChar(SGCC_UART_TypeDef* UARTx)
{   
    UINT8 ch;    
   
    while(UARTx->STATUS & UART_STATUS_RXEMP); //wait until rxfifo not empty
    
    ch = UARTx->RXDATA;
    return (ch);
}				

/*-----------------------------------------------------------------------------
  Function:		UART_PutChar                                                                       
  input:    UARTx: select the UART        
            ch:character to output                             
  Returns:  None                                                                                        
  Description:  transmit a byte                                                                                          
               	                                  
 *-----------------------------------------------------------------------------*/
void UART_PutChar(SGCC_UART_TypeDef* UARTx, UINT8 ch)
{
    while(UARTx->STATUS & UART_STATUS_TXFULL); //wait until TXFIFO not full
    UARTx->TXDATA = ch;
}

/*-----------------------------------------------------------------------------
  Function:		UART_GetFlagStatus                                                                       
  input:    UARTx: select the UART        
            flag:which flag to get, can be UART_STATUS_TXFULL/UART_STATUS_RXEMP/
                                           UART_STATUS_TXING/UART_STATUS_RXING/
                                           UART_STATUS_RXFULL/UART_STATUS_TXEMP/
                                           UART_STATUS_RXFIFO_OVERRUN/UART_STATUS_TXFIFO_OVERRUN      
  Returns:  None                                                                                        
  Description:  get UART status                                                                                         
               	                                  
 *-----------------------------------------------------------------------------*/
UINT8 UART_GetFlagStatus(SGCC_UART_TypeDef* UARTx, uint16_t flag)
{
    if(UARTx->STATUS & flag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

/*-----------------------------------------------------------------------------
  Function:		UART_PutStr                                                                      
  input:    UARTx: select the UART         
            str:string to output                             
  Returns:  None                                                                                        
  Description:  transmit a string                                                                                          
               	                                  
 *-----------------------------------------------------------------------------*/
void UART_PutStr(SGCC_UART_TypeDef* UARTx, UINT8 *Str)
 {
  	UINT8 *cp;
   
 	for(cp = Str; *cp != '\0'; cp++)       
   		UART_PutChar(UARTx, *cp);	
}

/*-----------------------------------------------------------------------------
  Function:		UART_GetStr                                                                       
  input:    UARTx: select the UART         
            buf:buffer to save received data                             
  Returns:                                                                                          
  Description:  get received data                                                                                          
               	                                  
 *-----------------------------------------------------------------------------*/
int UART_GetStr(SGCC_UART_TypeDef* UARTx, UINT8 *buf)
{
    UINT8    *cp;
    UINT8    data;
    UINT32  count;
    count = 0;
    cp = buf;
    
    do
    {
        data = UART_GetChar(UARTx);

        switch(data)
        {
            case RETURN_KEY:
                *cp = '\0';
                UART_PutChar(UARTx,'\n');   
                break;
            case BACKSP_KEY:
            case DELETE_KEY:
                if(count)
                {
                    count--;
                    *(--cp) = '\0';
                    UART_PutStr(UARTx, "\b \b");
                }         
                break;
            default:
			    if(count < 1024)
				{         
                    if( data > 0x1F && data < 0x7F)
                    {
                        *cp = (UINT8)data;
                        cp++;
                        count++;
                        UART_PutChar(UARTx, data);
                    }
				}
				else
				{
				     return (count);
				}
                break;
        }
    } while(data != RETURN_KEY);  
    
  return (count);
}

/*-----------------------------------------------------------------------------
  Function:		UART_Printf                                                                       
  input:    UARTx: select the UART      
            buf:buffer to save received data                             
  Returns:                                                                                          
  Description:  get received data                                                                                          
               	                                  
 *-----------------------------------------------------------------------------*/
/**/
INT32 UART_Printf(SGCC_UART_TypeDef* UARTx, const char *f, ...)	// variable arguments 
{
    INT32 i;
    __va_list arg_ptr;    
    char buffer[256];

   	//put the character to buffer
   	va_start(arg_ptr, f);
   	vsprintf(&buffer[0], f, arg_ptr);
   	va_end(arg_ptr);   
	
   	//output the buffer
    i=0;
    while(buffer[i])	
    {
    	UART_PutChar(UARTx, buffer[i]);
    	i++;
    }

	
    return i;
}

