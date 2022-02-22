#ifndef __SCM62X_CRC_H 
#define __SCM62X_CRC_H 

#include "types.h"
#include "SCM62X.h"


typedef struct 
{
	UINT16 polyType;
    UINT16 inRev;
    UINT16 outRev;
    UINT16 xorOut;
    UINT16 crcMode;    
    UINT32 initVal;
}CRC_InitTypeDef;


/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */

void CRC_Init(CRC_InitTypeDef crcInit) ;
void CRC_Enable(UINT8 opt) ;
void CRC_InputByte(UINT8* buf, UINT16 len) ;
void CRC_InputWord(UINT32* buf, UINT16 len) ;
void CRC_InputHalfWord(UINT16* buf, UINT16 len) ;
void CRC_GetData(UINT32* buf) ;
void CRC_GetWord(UINT32* buf) ;
void CRC_GetByte(UINT8* buf) ;
void CRC_GetHalfWord(UINT16* buf) ;
BOOL CRC_GetVerifyResult(void) ;
#endif  
                                                                                                   
