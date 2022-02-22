/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_SPI.c                                                     */
/* 内容摘要: SPI模块基础函数定义                                              */
/* 其它说明: **                                                               */
/* 当前版本: V1.0                                                             */
/* 作    者: SCM                                                              */
/* 完成日期: 2019-03-20                                                       */
/* 修改记录:                                                                  */
/*    修改日期          版本号        修改人        修改内容                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0          SCM        创建文件                     */
/*<FH->************************************************************************/


/******************************************************************************/
/*               #include（依次为标准库头文件、非标准库头文件）               */
/******************************************************************************/
#include "SCM62X.h"

/******************************************************************************/
/*                                外部引用定义                                */
/******************************************************************************/


/******************************************************************************/
/*                                  全局变量                                  */
/******************************************************************************/


/********************************************************
 Function:	SPI_ResetReg                                                                                                                                                                            
 input:     SPIx  
 output:     None                                        
 Returns:    None                                                                                      
 Description: reset spi register         
********************************************************/
void SPI_ResetReg(SGCC_SPI_TypeDef * SPIx )
{
    SPIx->SPCR0 = 0x0;
    SPIx->SPCR1 = 0x0;
    SPIx->SPCR2 = 0x80;
    SPIx->SPCR3 = 0xc;
    SPIx->SPCR3 = 0x21;
    SPIx->SPWAR = 0x0;
    SPIx->SPNUMR = 0x0;
}
/********************************************************
 Function:	SPI_ConfigMode                                                                                                                                                                            
 input:     SPIx
            mode: SPI_CR1_SCK_MOD0/SPI_CR1_SCK_MOD1/SPI_CR1_SCK_MOD2/SPI_CR1_SCK_MOD3	  
 output:     None                                        
 Returns:    None                                                                                      
 Description: 配置POL         
********************************************************/
void SPI_ConfigMode(SGCC_SPI_TypeDef * SPIx, UINT32 mode)
{
    UINT32 temp = 0;
	temp = SPIx->SPCR1;
    
    temp &= ~SPI_CR1_SCK_MOD;
    temp |= mode;
    
    SPIx->SPCR1 = temp;
}
/********************************************************
 Function:	SPI_InitMaster                                                                                                                                                                            
 input:     SPIx  
 output:     None                                        
 Returns:    None                                                                                      
 Description: spi init as master         
********************************************************/
void SPI_InitMaster(SGCC_SPI_TypeDef * SPIx, SPI_MASTER *initMaster )
{	
	SPIx->SPCR1 = initMaster->rxEn | initMaster->txEn | initMaster->mode | SPI_CR1_MSTR_MSTR;

    SPIx->SPCR2 = initMaster->txFifoLvl | initMaster->rxFifoLvl;
	SPIx->SPI_SCKDIV = initMaster->sckDiv;
    
    if(initMaster->DataNumEn == SPI_CR3_DNUM_EN)
    {
        SPIx->SPNUMR = initMaster->DataNum;
        SPIx->SPCR3 |= SPI_CR3_DNUM_EN;
    }
    else
        SPIx->SPCR3 &= ~SPI_CR3_DNUM_EN;
    
    SPIx->SPWAR = initMaster->waitNum;
}

/********************************************************
 Function:	SPI_InitSlave                                                                                                                                                                            
 input:     SPIx  
 output:     None                                        
 Returns:    None                                                                                      
 Description: spi init as slave         
********************************************************/
void SPI_InitSlave(SGCC_SPI_TypeDef * SPIx, SPI_SLAVE *initSlave )
{	
	SPIx->SPCR1 = initSlave->rxEn | initSlave->txEn | initSlave->mode | SPI_CR1_MSTR_SLAVE;

    SPIx->SPCR2 = initSlave->txFifoLvl | initSlave->rxFifoLvl;
}
/********************************************************
 Function:	SPI_StartTransferAsMaster                                                                                                                                                                            
 input:     SPIx	  
 output:     None                                        
 Returns:    None                                                                                      
 Description: select slave, enable SPI and start transfer  
********************************************************/
void SPI_StartTransferAsMaster(SGCC_SPI_TypeDef * SPIx)
{
    UINT32 temp = 0;
    temp = SPIx->SPCR3; 
    temp &= ~SPI_CR3_SSN_EN;
    temp |= (SPI_CR3_SPE_EN | SPI_CR3_STR_EN );
    
    SPIx->SPCR3 = temp;

}

/********************************************************
 Function:	SPI_StartTransferAsSlave                                                                                                                                                                            
 input:     SPIx	  
 output:     None                                        
 Returns:    None                                                                                      
 Description: select slave, enable SPI and start transfer  
********************************************************/
void SPI_StartTransferAsSlave(SGCC_SPI_TypeDef * SPIx)
{
    UINT32 temp = 0;
    temp = SPIx->SPCR3; 
    temp &= ~SPI_CR3_SSN_EN;
    temp |= (SPI_CR3_SPE_EN | SPI_CR3_STR_EN | SPI_CR3_DEBN_EN);
    
    SPIx->SPCR3 = temp;

}
/********************************************************
 Function:	SPI_StopTransferAsMaster                                                                                                                                                                            
 input:     SPIx	  
 output:     None                                        
 Returns:    None                                                                                      
 Description: disable tx and rx, set ssn high , disable SPI   
********************************************************/
void SPI_StopTransferAsMaster(SGCC_SPI_TypeDef * SPIx)
{
    SPIx->SPCR1 &= ~(SPI_CR1_R_EN | SPI_CR1_T_EN);
    
    SPIx->SPCR3 &= ~(SPI_CR3_SPE_EN | SPI_CR3_STR_EN);
    SPIx->SPCR3 |= SPI_CR3_SSN_EN;

}

/********************************************************
 Function:	SPI_DriveSSN                                                                                                                                                                            
 input:     SPIx
            opt--SSN_HIGH/SSN_LOW
 output:     None                                        
 Returns:    None                                                                                      
 Description: disable tx and rx, set ssn high , disable SPI   
********************************************************/
void SPI_DriveSSN(SGCC_SPI_TypeDef * SPIx, UINT8 opt)
{
    
    if(opt == SSN_HIGH)
    {
        SPIx->SPCR3 |= SPI_CR3_SSN_EN;
    }
    else
    {
        SPIx->SPCR3 &= ~SPI_CR3_SSN_EN;
    }

}

/********************************************************
 Function:	SPI_Enable                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
             opt-ENABLE/DISABLE
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_Enable(SGCC_SPI_TypeDef * SPIx, UINT8 opt)
{
    if(opt == ENABLE)
	    SPIx->SPCR3 |= SPI_CR3_SPE_EN;
    else
        SPIx->SPCR3 &= ~SPI_CR3_SPE_EN;
}

/********************************************************
 Function:	SPI_EnableIntr                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
             intrMask - which interrupt to enable , 
             SPI_INTR_TFE/SPI_INTR_RFF/SPI_INTR_SSNERR/SPI_INTR_TNF/SPI_INTR_RNE/SPI_INTR_BYTEDONE/SPI_INTR_TRANDONE
              SPI_CR0_RFF_EN/SPI_CR0_TFE_EN/SPI_CR0_SSN_EN/SPI_CR0_DUNM_EN/SPI_CR0_TNF_EN/SPI_CR0_RNE_EN/SPI_CR0_BYTEDONE_EN
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_EnableIntr(SGCC_SPI_TypeDef * SPIx, UINT8 intrType, UINT8 opt)
{
    if(opt == ENABLE)
        SPIx->SPCR0 |= intrType;
    else
        SPIx->SPCR0 &= ~intrType;
}

/********************************************************
 Function:	SPI_ClearIntr                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
             intrType - which interrupt to clear 
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_ClearIntr(SGCC_SPI_TypeDef * SPIx, UINT8 intrType)
{
    SPIx->SPINTSR = intrType;
}


/********************************************************
 Function:	SPI_ClearTxFIFO                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_ClearTxFIFO(SGCC_SPI_TypeDef * SPIx)
{
    SPIx->SPCR3 |= 0x4;
}

/********************************************************
 Function:	SPI_ClearRxFIFO                                                                                                                                                                            
 input:      SPIx -which SPI to enable or disable
 output:     None                                        
 Returns:    None                                                                                      
 Description:   
********************************************************/
void SPI_ClearRxFIFO(SGCC_SPI_TypeDef * SPIx)
{
    SPIx->SPCR3 |= 0x8;
}

/********************************************************
 Function:	SPI_WriteByte                                                                                                                                                                            
 input:     SPIx -- 
             dat -- data to send 
 output:     None                                        
 Returns:    None                                                                                      
 Description: spi write one data byte to txfifo       
********************************************************/
void SPI_WriteByte(SGCC_SPI_TypeDef * SPIx, UINT8 dat)
{
    SPIx->SPDR = dat;
}    
/********************************************************
 Function:	SPI_WriteBytes                                                                                                                                                                            
 input:     SPIx -- 
             dat -- data to send 
 output:     None                                        
 Returns:    None                                                                                      
 Description: spi write data bytes to txfifo       
********************************************************/
void SPI_WriteBytes(SGCC_SPI_TypeDef * SPIx, UINT8 *dat, UINT8 len)
{
    UINT8 i ;
    
    for(i = 0; i < len; i++)
    {
        while(((SPIx->SPSR & 0xF0) >> 4) == 8);  //wait until data in txfifo less than 8
        SPIx->SPDR = dat[i];
    }
}

/********************************************************
 Function:	SPI_RdByte                                                                                                                                                                            
 input:     SPIx --   
 output:     None                                        
 Returns:    读回的数据                                                                                      
 Description: SPI接收1字节数据        
********************************************************/
UINT8 SPI_RdByte (SGCC_SPI_TypeDef * SPIx)
{
    while(!(SPIx->SPINTSR & SPI_INTR_RNE));
    //while(!(SPIx->SPSR & 0xf));
	    return(SPIx->SPDR);
       
}

/********************************************************
 Function:	SPI_RxBytes                                                                                                                                                                            
 input:     SPIx --   
 output:     None                                        
 Returns:    读回的数据                                                                                      
 Description: SPI接收多字节数据        
********************************************************/
UINT8 SPI_RxBytes (SGCC_SPI_TypeDef * SPIx, UINT8 *dat)
{
	UINT8 len = 0;
   // UART_Printf(UARTx, "%x ", SPIx->SPSR);
    if((SPIx->SPSR & 0xF)  != 0)//wait until data is available in rxfifo 
    {
        dat[len] = SPIx->SPDR;
        len++;
    }
	return len;
}
