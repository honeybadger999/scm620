#ifndef __SCM62X_UART_H 
#define __SCM62X_UART_H 

#include "types.h"
#include "SCM62X.h"

#define PARITY_NONE                  0
//#define PARITY_ODD                  (UART_CTRL_PARITY_EN | UART_CTRL_PARITY_ODD)
//#define PARITY_EVEN                 (UART_CTRL_PARITY_EN | UART_CTRL_PARITY_EVEN)

#define SCM_PARITY_NONE                  0
#define SCM_PARITY_ODD                  (UART_CTRL_PARITY_EN | UART_CTRL_PARITY_ODD)
#define SCM_PARITY_EVEN                 (UART_CTRL_PARITY_EN | UART_CTRL_PARITY_EVEN)

#ifndef UART_CLK
    #define UART_CLK 18750000UL
#endif

#define DEFAULT_BARDRATE   2400
   

#define BACKSP_KEY 0x08
#define RETURN_KEY 0x0D
#define DELETE_KEY 0x7F
#define ESC_KEY 0x1B

extern UINT8 UART_DataReady[3];
extern UINT8 UART_ThrEmpty[3] ;

struct COMMUNICATION_FRM
{
	UINT8 Buf[30];			//接收缓冲/发送缓冲
	UINT8 Len;					//接收长度字节/发生长度
	UINT8 Total_Len;				//接收总长度/发送总长度
	UINT8 Step;  					//接收步骤
};

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */

extern void UART_Init (SGCC_UART_TypeDef* UARTx, UINT32 baudrate, UINT32 parity, UINT32 stopNum);
extern void UART_EnableIntr(SGCC_UART_TypeDef* UARTx,UINT32 intMask);
extern void UART_ClearIntr(SGCC_UART_TypeDef* UARTx,UINT32 intType);
extern UINT8 UART_GetChar(SGCC_UART_TypeDef* UARTx);
extern void UART_PutChar(SGCC_UART_TypeDef* UARTx, UINT8 ch);
extern UINT8 UART_GetFlagStatus(SGCC_UART_TypeDef* UARTx, uint16_t flag);
extern void UART_PutStr(SGCC_UART_TypeDef* UARTx, UINT8 *Str);
extern int UART_GetStr(SGCC_UART_TypeDef* UARTx, UINT8 *buf);
extern INT32 UART_Printf(SGCC_UART_TypeDef* UARTx, const char *f, ...)	;/* variable arguments */







#endif 
