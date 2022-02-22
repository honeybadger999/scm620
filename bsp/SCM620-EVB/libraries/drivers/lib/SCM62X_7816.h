#ifndef __SCM62X_7816_H 
#define __SCM62X_7816_H 

#include "types.h"

//7816时钟 4000K
#define U7816_CLK  4000    
#define U7816_RTRIG 0

//通道定义
#define U7816_CH0       0
#define U7816_CH1       1

//ESAM通路电源控制
#define	ESAMPOWERON  	1
#define	ESAMPOWEROFF 	0


#define KEY_N	 0
#define KEY_P	 1


//U7816通讯缓冲区定义
#define u7816_buf_size	30


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
extern void U7816_RestoreReg(void);
extern void U7816_ConfigBaudRate(UINT16 baudRate) ;
extern void U7816_ConfigDataFrame(UINT8 tmod, UINT8 diconv, UINT8 par, UINT8 txegt) ;
extern void U7816_Init(UINT16 baudRate) ;
extern void U7816_PrepareRx(UINT8 rxLvl, UINT8 errWid, UINT8 ch) ;
extern void U7816_GetChars(UINT8 *data, UINT16 len);
extern UINT8 U7816_GetChar(UINT8 *data);
extern void U7816_PrepareTx(UINT8 rpt, UINT8 ch) ;
extern UINT8  U7816_SendByte(UINT8 data );
extern void U7816_EnableIntr(UINT32 intMask);
extern void U7816_ClearIntr(UINT32 intType);
extern UINT8 U7816_TxRxFrame( UINT8 ch, UINT8 Lc, UINT8 Le );
extern void U7816_RxTxCommhead( UINT8 cla, UINT8 ins, UINT8 p1, UINT8 p2, UINT8 len);
extern void U7816_OpenEsam( void );
extern void U7816_CloseEsam( void );
extern UINT8 U7816_JudgeEdge( unsigned int time );
extern UINT8 U7816_ColdReset( UINT8 ch );
extern UINT8 U7816_ReadRandom( UINT8 ch, UINT8 len );
extern UINT8 U7816_EsamProc( UINT8 ch );








#endif 
