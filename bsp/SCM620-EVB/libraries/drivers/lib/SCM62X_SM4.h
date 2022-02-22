#ifndef __SCM62X_SM4_H 
#define __SCM62X_SM4_H 

#include "types.h"
#include "SCM62X.h"

typedef struct 
{
    UINT32 mode;  //SM4_MR_WM_SEL_ECB/SM4_MR_WM_SEL_CBC
    UINT32 encyMode; //SM4_MR_ENC_SEL_ENCRYPT/SM4_MR_ENC_SEL_DECRYPT
    UINT32 dpaw;
    UINT32 dpawr;
    
}SM4_InitTypeDef;

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void SM4_RstKey(UINT8 opt) ;
void SM4_RstIV(UINT8 opt) ;
void SM4_Start(void) ;
void SM4_Enable(UINT8 opt) ;
void SM4_SelEncMode(UINT32 mode) ;
void SM4_SelMode(UINT32 mode) ;
void SM4_ConfigDPAWR(UINT32 dpawr); 
void SM4_EnableDpaw(UINT8 opt) ;
void SM4_EnableIntr(UINT8 opt) ;
void SM4_ClearIntr(void) ;
UINT8 SM4_GetStatus(void);
void SM4_ConfigKey(UINT32 k1, UINT32 k2, UINT32 k3, UINT32 k4) ;
void SM4_ConfigIV(UINT32 iv1, UINT32 iv2, UINT32 iv3, UINT32 iv4); 
void SM4_WriteData(UINT32 d1, UINT32 d2, UINT32 d3, UINT32 d4) ;
void SM4_ReadData(UINT32 *rdData) ;

#endif 
