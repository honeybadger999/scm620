#ifndef __SCM62X_SPI_H 
#define __SCM62X_SPI_H 

#include "SCM62X.h"


#define SSN_HIGH   1
#define SSN_LOW  0

typedef struct 
{
	 UINT8 sckDiv;
	 UINT8 txEn;
	 UINT8 rxEn;
	 UINT8 mode;
	 UINT8 txFifoLvl;
	 UINT8 rxFifoLvl;
	 UINT8 DataNumEn;
	 UINT8 DataNum;
	 UINT8 waitNum;
}SPI_MASTER;

typedef struct 
{
	 UINT8 txEn;
	 UINT8 rxEn;
	 UINT8 mode;
	 UINT8 txFifoLvl;
	 UINT8 rxFifoLvl;
}SPI_SLAVE;


//#define SPI_RdRxByte(SPIx,rdByte)             (rdByte = SPIx->SPDR)
//#define SPI_WrTxByte(SPIx, wrByte)       (SPIx->SPDR = (wrByte))

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */

void SPI_InitMaster(SGCC_SPI_TypeDef * SPIx, SPI_MASTER *initMaster );
void SPI_InitSlave(SGCC_SPI_TypeDef * SPIx, SPI_SLAVE *initSlave );

void SPI_StartTransferAsMaster(SGCC_SPI_TypeDef * SPIx);
void SPI_StopTransferAsMaster(SGCC_SPI_TypeDef * SPIx);
void SPI_PullSsnHigh(SGCC_SPI_TypeDef * SPIx);
void SPI_PullSsnLow(SGCC_SPI_TypeDef * SPIx);
void SPI_Enable(SGCC_SPI_TypeDef * SPIx, UINT8 opt);
void SPI_EnableIntr(SGCC_SPI_TypeDef * SPIx, UINT8 intrType, UINT8 opt);
void SPI_ClearIntr(SGCC_SPI_TypeDef * SPIx, UINT8 intrType);
void SPI_WriteByte(SGCC_SPI_TypeDef * SPIx, UINT8 dat);
void SPI_WriteBytes(SGCC_SPI_TypeDef * SPIx, UINT8 *dat, UINT8 len);
UINT8 SPI_RdByte (SGCC_SPI_TypeDef * SPIx);
void SPI_ResetReg(SGCC_SPI_TypeDef * SPIx );
void SPI_DriveSSN(SGCC_SPI_TypeDef * SPIx, UINT8 opt);

void SPI_StartTransferAsSlave(SGCC_SPI_TypeDef * SPIx);
void SPI_ClearTxFIFO(SGCC_SPI_TypeDef * SPIx);
#endif 
