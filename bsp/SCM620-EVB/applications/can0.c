#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_can.h"
/* Private define-----------------------------------------------------------------------------*/
#define MAX_MAIL_NUM        3  // �������
#define MAX_CAN_NUM         2  // CAN�ĸ���

/* Private typedef----------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables--------------------------------------------------------------------------*/

static CAN_FrameInfoTypeDef  ReceFrame;
static CAN_FrameInfoTypeDef  SendFrame;

CAN_InitTypeDef CAN_InitStruct;

/***********************************************************************************************
* Function		: BSP_CAN_Init
* Description	: can�ⲿ���ó�ʼ��
* Input			:
* Return		:
* Note(s)		: BSP_CAN_Set �����ðɲ����ʵ�����Ҳ���ϣ����ڲ�֪��can�ֳ�ʹ���Ƿ������
*				  ���ֲ�������ʱ�����ʹ̶��ķ�ʽ����CAN_Configuration��ֱ�����úá�
* Contributor	: hejian 2017/9/14
***********************************************************************************************/
void  BSP_CAN0_Init(_BSPCAN_PORT num)
{
    CAN_InitTypeDef CAN_InitStruct;

    CAN0_InitPad(CAN0_PIN103RXD, CAN0_PIN104TXD0);		//��ʼ��CAN����

    CAN_InitStruct.CAN_Mode = CAN_MODE_FILTER_SIGLE;//CAN����  ���������� ʹ��һ�� 4 �ֽڹ�������������������

//    CAN_InitStruct.CAN_BTR0 = CAN_BTR0_SJW3 | CAN_BTR0_BRP(5); //����ʱ��0  ����ʱ��1  P329   62.5m
	CAN_InitStruct.CAN_BTR0 = CAN_BTR0_SJW3 | CAN_BTR0_BRP(2); //125k
    CAN_InitStruct.CAN_BTR1 = CAN_BTR1_SAM_ONCE | CAN_BTR1_TSEG2(7) | CAN_BTR1_TSEG1(15);

    CAN_InitStruct.CAN_OCM  = CAN_OCR_OCM2; //�������ģʽ������
    CAN_InitStruct.CAN_CDC  = CAN_ODR_CLKOUT_OFF;//CAN_ODR_CLKOUT_DIV6; ���ʱ�Ӻͷ�Ƶ���� ������ر�
    CAN_InitStruct.CAN_ACR = 0xFF01FFFF; /* �Ӹ�λ����λ������ACR0,ACR1,ACR2,ACR3 */ //���մ���
    CAN_InitStruct.CAN_AMR = 0xFFFFFFFF; /* �Ӹ�λ����λ������AMR0,AMR1,AMR2,AMR3 */  //���մ�������
    CAN_Init(SGCC_CAN0_P, &CAN_InitStruct);					//CAN��ʼ��


    NVIC_EnableIRQ(CAN0_IRQn);//����ж�����
    NVIC_SetPriority(CAN0_IRQn, 0);
//	CAN_ITConfig(SGCC_CAN0_P, 0xFF, ENABLE);//CAN0ʹ���ж�   0xFF:ʹ�����е��ж�
    CAN_ITConfig(SGCC_CAN0_P, 0x01, ENABLE); //ֻʹ�ܽ����ж�

//    BSPCAN_Control[num].pCanElem = pbxCAN;

}


/**
  * @brief  ����CAN1������ 
			20201209 ����Ϊ���н��գ������ԣ�δ������������
  * @param  none
  * @retval None
  */
void can0_rev_thread_entry(void *parameter)
{
//	DEBUG_PRINTF("%s\n",(char *)s);	
	//CAN0_init();	

	BSP_CAN0_Init(CAN_PORT0);
	for(;;)
	{
		//rt_thread_delay(100);
		rt_mq_recv(mq_can0, &ReceFrame, sizeof(ReceFrame), RT_WAITING_FOREVER);

		//�ƹ�԰��Ŀ û�е�أ�Ϊpcs����ͨѶ
		//CAN1_Post(PMA_BATTERY,0,0,0,0);
		
		SendFrame.FrameFormat = CAN_FRAME_FORMAT_EFF;
		SendFrame.FrameType = CAN_FRAME_TYPE_DATA;
		SendFrame.FrameID = 0x00004231;
		SendFrame.DataLength = 8;
		SendFrame.FrameData[0] = 0xE5;
		SendFrame.FrameData[1] =  0x0C;
		SendFrame.FrameData[2] =  0xE4;
		SendFrame.FrameData[3] =  0x0C;
		SendFrame.FrameData[4] =  0x0B;
		SendFrame.FrameData[5] =  0;
		SendFrame.FrameData[6] =  0;
		SendFrame.FrameData[7] =  0;
		
		CAN_SendData(SGCC_CAN0_P, &SendFrame, CAN_COMMAND_TRANSMISSION_REQUEST); 	//CAN0��������	
	}	
}


/*-----------------------------------------------------------------------------
  Function:    CAN0_IRQHandler
  input:       None
  Returns:     None
  Description: CAN1 interrupt handler
 *-----------------------------------------------------------------------------*/
void CAN0_IRQHandler(void)
{
    volatile uint8_t temp;

 //   CAN_FrameInfoTypeDef RxMessage;
 //   _BSPCAN_PORT num;
 //   num = CAN_PORT0;

    temp = CAN_GetITFlag(SGCC_CAN0_P);							//��ȡ�жϱ�־λ

    /* �����ж�, ��λ��Ҫд����λCMR.2,�ͷŽ��ջ����� */
    if((temp & CAN_IT_RI) == CAN_IT_RI)						//���ձ�־λ
    {
        CAN_ReceData(SGCC_CAN0_P, &ReceFrame);				//�������ݣ�����ӡ�����Ϣ

		//���͵�����
		rt_mq_send (mq_can0,&ReceFrame, sizeof(ReceFrame));
		
        /*if(BSPCAN_Control[num].Rxcount < BSPCAN_Control[num].pCanElem->RxMaxNum)
        {
        	memcpy(&BSPCAN_Control[num].pCanElem->RxMessage[BSPCAN_Control[num].Rxrear], &RxMessage, sizeof(CanRxMsg));
        	BSPCAN_Control[num].Rxrear = ((BSPCAN_Control[num].Rxrear + 1) %  BSPCAN_Control[num].pCanElem->RxMaxNum);
        	BSPCAN_Control[num].Rxcount++;
        }
        */
    }
}

