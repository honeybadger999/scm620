#ifndef __SCM62X_AES_H 
#define __SCM62X_AES_H 

#include "types.h"
#include "SCM62X.h"

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
void AES_Start(void) ;
void AES_SelFunc(UINT32 func) ;
void AES_SelKey(UINT32 key) ;
void AES_EnableRand(UINT8 opt) ;
void AES_EnableStep(UINT8 opt) ;
void AES_ConfigKey(UINT32 *key, UINT16 keyLen) ;
void AES_ConfigRand(UINT32 *rand) ;
void AES_WriteData(UINT32 *inputData  ) ;
void AES_ReadData(UINT32 *rdData) ;
void AES_WaitStop(void) ;

#endif 
