/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子科技有限公司                                       */
/*                                                                            */
/* 文件名称: SCM62X_CAN.c                                                     */
/* 内容摘要: CAN模块基础函数定义                                              */
/* 其它说明: **                                                               */
/* 当前版本: V1.0                                                             */
/* 作    者: SCM                                                              */
/* 完成日期: 2019-03-20                                                       */
/* 修改记录:                                                                  */
/*    修改日期          版本号        修改人        修改内容                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0           SCM        创建文件                    */
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
/*-----------------------------------------------------------------------------
  Function:		 CAN_ITConfig                                                                                                                                                                                 
  input:       CANx: select the CAN 
               CAN_IT
							 NewState
  Returns:     None                                                                                      
  Description: CAN IT Config                                    
 *-----------------------------------------------------------------------------*/
void CAN_ITConfig(SGCC_CAN_TypeDef* CANx, uint32_t CAN_IT, uint32_t NewState)
{
    // 使能相关中断
    if (NewState == ENABLE)
    {
        CANx->IER |= CAN_IT;
    }
    // 关闭相关中断
    else
    {
        CANx->IER &= ~CAN_IT;
    }
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_GetStatus                                                                                                                                                                                 
  input:       CANx: select the CAN 
  Returns:     None                                                                                      
  Description: Get CAN status                                   
 *-----------------------------------------------------------------------------*/
uint32_t CAN_GetStatus(SGCC_CAN_TypeDef* CANx)
{
    return CANx->SR;
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_GetITFlag                                                                                                                                                                                 
  input:       CANx: select the CAN 
  Returns:     None                                                                                      
  Description: Get CAN IT flag                                   
 *-----------------------------------------------------------------------------*/
uint32_t CAN_GetITFlag(SGCC_CAN_TypeDef* CANx)
{
    return CANx->IR;
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_GetALC                                                                                                                                                                                 
  input:       CANx: select the CAN 
  Returns:     None                                                                                      
  Description: Get CAN IT flag                                   
 *-----------------------------------------------------------------------------*/
uint32_t CAN_GetALC(SGCC_CAN_TypeDef* CANx)
{
    return CANx->ALC;
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_SetCMD                                                                                                                                                                                 
  input:       CANx: select the CAN 
  Returns:     None                                                                                      
  Description: Set CAN CMD                                   
 *-----------------------------------------------------------------------------*/
void CAN_SetCMD(SGCC_CAN_TypeDef* CANx,uint32_t Command)
{
    CANx->CMR = Command;
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_ConfigError                                                                                                                                                                                 
  input:       CANx: select the CAN 
							 ErrorWL, RxErrorCounter, TxErrorCounter
  Returns:     None                                                                                      
  Description: CAN config error                                  
 *-----------------------------------------------------------------------------*/
void CAN_ConfigError(SGCC_CAN_TypeDef* CANx, uint32_t ErrorWL, uint32_t RxErrorCounter, uint32_t TxErrorCounter)
{
    volatile uint8_t Judge;
    
    // 进入复位模式
    do
    {
        CANx->MOD = 0x01;
		Judge = CANx->MOD ;
    }
    while( !(Judge & 0x01) );
    
    CANx->EWLR  = ErrorWL;
    CANx->RXERR = RxErrorCounter;
    CANx->TXERR = TxErrorCounter;
    
    // 退出Reset模式 
    do
    {
        CANx->MOD = 0x00;
		Judge = CANx->MOD ;
    }
    while(Judge & 0x01);
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_Sleep                                                                                                                                                                                 
  input:       CANx: select the CAN 
  Returns:     None                                                                                      
  Description: CAN sleep                                  
 *-----------------------------------------------------------------------------*/
void CAN_Sleep(SGCC_CAN_TypeDef* CANx)
{
     do
     {
         CANx->MOD |= 0x10;
     }while((CANx->MOD & 0x10) != 0x10);
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_Init                                                                                                                                                                                 
  input:       CANx: select the CAN 
  Returns:     None                                                                                      
  Description: Init CAN                               
 *-----------------------------------------------------------------------------*/
void CAN_Init(SGCC_CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct)
{
    volatile uint8_t Judge;
    
    // 进入复位模式
    do
    {//  .0=1---reset MODR,进入复位模式，以便设置相应的寄存器
     //防止未进入复位模式，重复写入
        CANx->MOD = 0x01;
		Judge = CANx->MOD ;
    }
    while( !(Judge & 0x01) );
    
    // 波特率
    // 设置TQ大小, 定义SJW大小
    CANx->BTR0 = CAN_InitStruct->CAN_BTR0;
    
    // 设置bit周期内的各段所用的TQ多少, 指定采样点, 指定采样次数
    CANx->BTR1 = CAN_InitStruct->CAN_BTR1;

    // 输出控制模式, 仅低两位有效
    CANx->OCR  = CAN_InitStruct->CAN_OCM;
    
    // CAN_CDR 输出分频控制,
    CANx->CDR  = CAN_InitStruct->CAN_CDC;
    
    // 释放接收缓冲器
    CAN_SetCMD(CANx,CAN_COMMAND_RELEASE_RECEIVE_BUFFER);
    
    // 初始化 验收代码, 初始化 验收屏蔽代码
    CANx->TFINFO_RFINFO_ACR0  = 0xFF&((CAN_InitStruct->CAN_ACR)>>24); //
    CANx->TDINFO_RDINFO1_ACR1 = 0xFF&((CAN_InitStruct->CAN_ACR)>>16); //
    CANx->TDINFO_RDINFO2_ACR2 = 0xFF&((CAN_InitStruct->CAN_ACR)>>8); //
    CANx->TDINFO_RDINFO3_ACR3 = 0xFF&((CAN_InitStruct->CAN_ACR)>>0); //接收代码（0x16_0x19）；接收过滤位的选择*******复位模式

    CANx->TDINFO_RDINFO4_AMR0 = 0xFF&((CAN_InitStruct->CAN_AMR)>>24); //
    CANx->TDINFO_RDINFO5_AMR1 = 0xFF&((CAN_InitStruct->CAN_AMR)>>16); //
    CANx->TDINFO_RDINFO6_AMR2 = 0xFF&((CAN_InitStruct->CAN_AMR)>>8); //
    CANx->TDINFO_RDINFO7_AMR3 = 0xFF&((CAN_InitStruct->CAN_AMR)>>0); // 掩码（0x20_0x23）; 接收过滤位的选择*******复位模式

    // 退出Reset模式
    do
    {//  .0=1---reset MODR,进入复位模式，以便设置相应的寄存器
     //防止未进入复位模式，重复写入
        CANx->MOD = 0x00;
		Judge = CANx->MOD;
    }
    while(Judge & 0x01);

    // 设置模式
    CANx->MOD = CAN_InitStruct->CAN_Mode;
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_SendData                                                                                                                                                                                 
  input:       CANx: select the CAN 
							 CAN_FrameInfoStruct, TransmissionCMD
  Returns:     None                                                                                      
  Description: CAN send data                                  
 *-----------------------------------------------------------------------------*/
void CAN_SendData(SGCC_CAN_TypeDef* CANx,CAN_FrameInfoTypeDef *CAN_FrameInfoStruct, uint32_t TransmissionCMD)
{
    volatile uint8_t Judge;
    volatile uint32_t *p_StartDataAddr;
    uint32_t i;
    
    do
    {
        Judge = CANx->SR;
    }
    //while(((Judge & 0x0C) != 0x0C)||(Judge & 0x10)); // 发送缓冲器被锁。等待
    //while(((Judge & 0x04) != 0x04)||(Judge & 0x10)); // 发送缓冲器被锁。等待
    while((Judge & 0x04) != 0x04);
    
    CANx->TFINFO_RFINFO_ACR0  = (CAN_FrameInfoStruct->FrameFormat<<7)|(CAN_FrameInfoStruct->FrameType<<6) 
                              | (CAN_FrameInfoStruct->DataLength & 0x0F) ;    // 最后的0x0F保证BIT5,BIT4为"0"
    
    // 扩展格式
    if (CAN_FrameInfoStruct->FrameFormat == CAN_FRAME_FORMAT_EFF)
    {
        CANx->TDINFO_RDINFO1_ACR1 = 0xFF&(CAN_FrameInfoStruct->FrameID>>21);
        CANx->TDINFO_RDINFO2_ACR2 = 0xFF&(CAN_FrameInfoStruct->FrameID>>13);
        CANx->TDINFO_RDINFO3_ACR3 = 0xFF&(CAN_FrameInfoStruct->FrameID>>5);
        CANx->TDINFO_RDINFO4_AMR0 = 0xFC&(CAN_FrameInfoStruct->FrameID<<3);    // 按照建议,保证了BIT1,BIT0为"0"
        
        p_StartDataAddr = &CANx->TDINFO_RDINFO5_AMR1;
    }
    // 标准格式
    else
    {
        CANx->TDINFO_RDINFO1_ACR1 = 0xFF&(CAN_FrameInfoStruct->FrameID>>3);
        CANx->TDINFO_RDINFO2_ACR2 = 0xF0&(CAN_FrameInfoStruct->FrameID<<5);
        
        p_StartDataAddr = &CANx->TDINFO_RDINFO3_ACR3;
    }
    
    // 数据载入寄存器中
    for (i=0; i<CAN_FrameInfoStruct->DataLength; i++)
    {
        p_StartDataAddr[i] = CAN_FrameInfoStruct->FrameData[i];
    }

    // 设置发送命令
    CAN_SetCMD(CANx,TransmissionCMD);
}

/*-----------------------------------------------------------------------------
  Function:		 CAN_ReceData                                                                                                                                                                                 
  input:       CANx: select the CAN 
							 CAN_FrameInfoStruct
  Returns:     None                                                                                      
  Description: CAN received data                                   
 *-----------------------------------------------------------------------------*/
void CAN_ReceData(SGCC_CAN_TypeDef* CANx,CAN_FrameInfoTypeDef *CAN_FrameInfoStruct)
{
    
    volatile uint8_t Judge;
    volatile uint32_t *p_StartDataAddr;
    uint32_t i;
    
    do
	{
		Judge = CANx->SR;
	}while(!(Judge & 0x01));
    
    CAN_FrameInfoStruct->DataLength  = 0x0F & CANx->TFINFO_RFINFO_ACR0;
    CAN_FrameInfoStruct->FrameFormat = (0x80 & CANx->TFINFO_RFINFO_ACR0)>>7;
    CAN_FrameInfoStruct->FrameType   = (0x40 & CANx->TFINFO_RFINFO_ACR0)>>6;
    
    if (CAN_FrameInfoStruct->FrameFormat == CAN_FRAME_FORMAT_EFF)
    {
        CAN_FrameInfoStruct->FrameID = (CANx->TDINFO_RDINFO1_ACR1 <<21)
                                     | (CANx->TDINFO_RDINFO2_ACR2 <<13)
                                     | (CANx->TDINFO_RDINFO3_ACR3 <<5)
                                     | (CANx->TDINFO_RDINFO4_AMR0 >>3);
        p_StartDataAddr = &CANx->TDINFO_RDINFO5_AMR1;
        
    }
    else
    {
        CAN_FrameInfoStruct->FrameID = (CANx->TDINFO_RDINFO1_ACR1 <<3)
                                     | (CANx->TDINFO_RDINFO2_ACR2 >>5);
        p_StartDataAddr = &CANx->TDINFO_RDINFO3_ACR3;
    }
    
    for (i=0; i<CAN_FrameInfoStruct->DataLength; i++)
    {
        CAN_FrameInfoStruct->FrameData[i] = p_StartDataAddr[i];
    }
 
    // 释放接收缓冲区
    CAN_SetCMD(CANx,CAN_COMMAND_RELEASE_RECEIVE_BUFFER);
    
    // 释放ECC错误
    // 释放仲裁失败
    //释放仲裁丢失捕捉寄存器
    Judge = CANx->ALC;
    //释放错误代码捕捉寄存器
    Judge = CANx->ECC;
}

