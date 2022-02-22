#ifndef __SCM62X_I2C_H 
#define __SCM62X_I2C_H 

#include "SCM62X.h"

#define SYS_FREQ  8000000UL   //以M（兆）为单位

#define I2C_MASTER 1
#define I2C_SLAVE 0 

typedef struct 
{
    uint8_t mode; //I2C_MASTER/I2C_SLAVE
    uint32_t scl;  //working frequency, for master and slave
    uint32_t prescale; //I2C_CFGR_DB0/I2C_CFGR_DB2/I2C_CFGR_DB4/I2C_CFGR_DB6,debounce , for master and slave
    uint32_t stretchEn; //I2C_CFGR_STR_EN/I2C_CFGR_STR_DIS, stretch or not, for slave only
    uint32_t timeoutEn; //I2C_CFGR_TO_EN/I2C_CFGR_TO_DIS, enable timeout  or not, for slave only
    uint16_t addrMode;//I2C_ADR_ADSE_10BIT/I2C_ADR_ADSE_7BIT,7-bit or 10-bit address, for slave only
    uint16_t addrMask;//I2C_ADR_ADMA_0~I2C_ADR_ADMA_7, for slave only
    uint16_t gCallEn;//I2C_ADR_GCA_EN/I2C_ADR_GCA_DIS, for slave only
    uint16_t slaveAddr;//slave address, for slave only
}I2C_InitTypeDef;

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */

extern void I2C_Init( SGCC_I2C_TypeDef* i2cx,  I2C_InitTypeDef i2cInit);
extern void I2C_Enable(SGCC_I2C_TypeDef* i2cx);
extern void I2C_Disable(SGCC_I2C_TypeDef* i2cx);
extern void I2C_EnableIntr(SGCC_I2C_TypeDef* i2cx);
extern void I2C_DisableIntr(SGCC_I2C_TypeDef* i2cx);
extern void I2C_ITConfig(SGCC_I2C_TypeDef* i2cx, uint32_t I2C_IT, 
                  FunctionalState NewState);
extern void I2C_WrCmd( SGCC_I2C_TypeDef* i2cx, uint32_t cmd );
extern FlagStatus I2C_GetFlagStatus(SGCC_I2C_TypeDef* i2cx, uint32_t I2C_FLAG);
extern void I2C_ClrSts(SGCC_I2C_TypeDef* i2cx, uint32_t I2C_FLAG);
extern void I2C_SendData(SGCC_I2C_TypeDef* i2cx, uint8_t data);
extern uint8_t I2C_ReceiveData(SGCC_I2C_TypeDef* i2cx);
extern void I2C_SendByteOnly(SGCC_I2C_TypeDef* i2cx, uint8_t data, uint8_t mode);
extern ErrorStatus I2C_SendByteTillDone(SGCC_I2C_TypeDef* i2cx, uint8_t data);
extern ErrorStatus I2C_ReceiveByte(SGCC_I2C_TypeDef* i2cx, uint8_t *data);
extern ErrorStatus I2C_ReceivelastByte(SGCC_I2C_TypeDef* i2cx, uint8_t *data );
extern ErrorStatus I2C_Start(SGCC_I2C_TypeDef* i2cx);
extern ErrorStatus I2C_Stop(SGCC_I2C_TypeDef* i2cx);
extern ErrorStatus I2C_ReStart(SGCC_I2C_TypeDef* i2cx);



#endif 
