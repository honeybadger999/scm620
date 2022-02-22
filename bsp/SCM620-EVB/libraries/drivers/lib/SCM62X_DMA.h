#ifndef __SCM62X_DMA_H 
#define __SCM62X_DMA_H 
#include "SCM62X.h"

typedef struct 
{
    UINT32 srcSaddr;
		UINT32 dstSaddr;
		UINT32 srcInc;
		UINT32 dstInc;
		UINT32 srcBlkOffset;
		UINT32 destBlkOffset;
		UINT32 transMode;
		UINT32 trUnit;
		UINT32 trCnt;
    UINT32 blocks;
    UINT32 intSrc;
}DMA_InitTypeDef;

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */

extern void DMA_Init (SGCC_DMA_TypeDef* DMAx, DMA_InitTypeDef* DMA_InitStruct);
extern void DMA_EnableIntr(SGCC_DMA_TypeDef* DMAx,UINT32 intMask);
extern void DMA_Enable(SGCC_DMA_TypeDef* DMAx,UINT32 opt);
extern UINT32 DMA_GetIntSts(SGCC_DMA_TypeDef* DMAx);
extern void DMA_ClearIntr(SGCC_DMA_TypeDef* DMAx);
void DMA_ClrIntStatus(SGCC_DMA_TypeDef* DMAx);
#endif 
                                                                                                   
