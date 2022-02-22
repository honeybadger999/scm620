/*<FH+>************************************************************************/
/*                                                                            */
/* ��Ȩ����: ������о΢���ӿƼ����޹�˾                                       */
/*                                                                            */
/* �ļ�����: SCM62X_CAN.c                                                     */
/* ����ժҪ: CANģ�������������                                              */
/* ����˵��: **                                                               */
/* ��ǰ�汾: V1.0                                                             */
/* ��    ��: SCM                                                              */
/* �������: 2019-03-20                                                       */
/* �޸ļ�¼:                                                                  */
/*    �޸�����          �汾��        �޸���        �޸�����                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0           SCM        �����ļ�                    */
/*<FH->************************************************************************/


/******************************************************************************/
/*               #include������Ϊ��׼��ͷ�ļ����Ǳ�׼��ͷ�ļ���               */
/******************************************************************************/
#include "SCM62X.h"

/******************************************************************************/
/*                                �ⲿ���ö���                                */
/******************************************************************************/

/******************************************************************************/
/*                                  ȫ�ֱ���                                  */
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
    // ʹ������ж�
    if (NewState == ENABLE)
    {
        CANx->IER |= CAN_IT;
    }
    // �ر�����ж�
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
    
    // ���븴λģʽ
    do
    {
        CANx->MOD = 0x01;
		Judge = CANx->MOD ;
    }
    while( !(Judge & 0x01) );
    
    CANx->EWLR  = ErrorWL;
    CANx->RXERR = RxErrorCounter;
    CANx->TXERR = TxErrorCounter;
    
    // �˳�Resetģʽ 
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
    
    // ���븴λģʽ
    do
    {//  .0=1---reset MODR,���븴λģʽ���Ա�������Ӧ�ļĴ���
     //��ֹδ���븴λģʽ���ظ�д��
        CANx->MOD = 0x01;
		Judge = CANx->MOD ;
    }
    while( !(Judge & 0x01) );
    
    // ������
    // ����TQ��С, ����SJW��С
    CANx->BTR0 = CAN_InitStruct->CAN_BTR0;
    
    // ����bit�����ڵĸ������õ�TQ����, ָ��������, ָ����������
    CANx->BTR1 = CAN_InitStruct->CAN_BTR1;

    // �������ģʽ, ������λ��Ч
    CANx->OCR  = CAN_InitStruct->CAN_OCM;
    
    // CAN_CDR �����Ƶ����,
    CANx->CDR  = CAN_InitStruct->CAN_CDC;
    
    // �ͷŽ��ջ�����
    CAN_SetCMD(CANx,CAN_COMMAND_RELEASE_RECEIVE_BUFFER);
    
    // ��ʼ�� ���մ���, ��ʼ�� �������δ���
    CANx->TFINFO_RFINFO_ACR0  = 0xFF&((CAN_InitStruct->CAN_ACR)>>24); //
    CANx->TDINFO_RDINFO1_ACR1 = 0xFF&((CAN_InitStruct->CAN_ACR)>>16); //
    CANx->TDINFO_RDINFO2_ACR2 = 0xFF&((CAN_InitStruct->CAN_ACR)>>8); //
    CANx->TDINFO_RDINFO3_ACR3 = 0xFF&((CAN_InitStruct->CAN_ACR)>>0); //���մ��루0x16_0x19�������չ���λ��ѡ��*******��λģʽ

    CANx->TDINFO_RDINFO4_AMR0 = 0xFF&((CAN_InitStruct->CAN_AMR)>>24); //
    CANx->TDINFO_RDINFO5_AMR1 = 0xFF&((CAN_InitStruct->CAN_AMR)>>16); //
    CANx->TDINFO_RDINFO6_AMR2 = 0xFF&((CAN_InitStruct->CAN_AMR)>>8); //
    CANx->TDINFO_RDINFO7_AMR3 = 0xFF&((CAN_InitStruct->CAN_AMR)>>0); // ���루0x20_0x23��; ���չ���λ��ѡ��*******��λģʽ

    // �˳�Resetģʽ
    do
    {//  .0=1---reset MODR,���븴λģʽ���Ա�������Ӧ�ļĴ���
     //��ֹδ���븴λģʽ���ظ�д��
        CANx->MOD = 0x00;
		Judge = CANx->MOD;
    }
    while(Judge & 0x01);

    // ����ģʽ
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
    //while(((Judge & 0x0C) != 0x0C)||(Judge & 0x10)); // ���ͻ������������ȴ�
    //while(((Judge & 0x04) != 0x04)||(Judge & 0x10)); // ���ͻ������������ȴ�
    while((Judge & 0x04) != 0x04);
    
    CANx->TFINFO_RFINFO_ACR0  = (CAN_FrameInfoStruct->FrameFormat<<7)|(CAN_FrameInfoStruct->FrameType<<6) 
                              | (CAN_FrameInfoStruct->DataLength & 0x0F) ;    // ����0x0F��֤BIT5,BIT4Ϊ"0"
    
    // ��չ��ʽ
    if (CAN_FrameInfoStruct->FrameFormat == CAN_FRAME_FORMAT_EFF)
    {
        CANx->TDINFO_RDINFO1_ACR1 = 0xFF&(CAN_FrameInfoStruct->FrameID>>21);
        CANx->TDINFO_RDINFO2_ACR2 = 0xFF&(CAN_FrameInfoStruct->FrameID>>13);
        CANx->TDINFO_RDINFO3_ACR3 = 0xFF&(CAN_FrameInfoStruct->FrameID>>5);
        CANx->TDINFO_RDINFO4_AMR0 = 0xFC&(CAN_FrameInfoStruct->FrameID<<3);    // ���ս���,��֤��BIT1,BIT0Ϊ"0"
        
        p_StartDataAddr = &CANx->TDINFO_RDINFO5_AMR1;
    }
    // ��׼��ʽ
    else
    {
        CANx->TDINFO_RDINFO1_ACR1 = 0xFF&(CAN_FrameInfoStruct->FrameID>>3);
        CANx->TDINFO_RDINFO2_ACR2 = 0xF0&(CAN_FrameInfoStruct->FrameID<<5);
        
        p_StartDataAddr = &CANx->TDINFO_RDINFO3_ACR3;
    }
    
    // ��������Ĵ�����
    for (i=0; i<CAN_FrameInfoStruct->DataLength; i++)
    {
        p_StartDataAddr[i] = CAN_FrameInfoStruct->FrameData[i];
    }

    // ���÷�������
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
 
    // �ͷŽ��ջ�����
    CAN_SetCMD(CANx,CAN_COMMAND_RELEASE_RECEIVE_BUFFER);
    
    // �ͷ�ECC����
    // �ͷ��ٲ�ʧ��
    //�ͷ��ٲö�ʧ��׽�Ĵ���
    Judge = CANx->ALC;
    //�ͷŴ�����벶׽�Ĵ���
    Judge = CANx->ECC;
}

