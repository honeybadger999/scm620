#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_can.h"
/* Private define-----------------------------------------------------------------------------*/

#define ERRTYPE     0xF0  //�޴���

#define SET_DC950 0x03
#define READ_DC950 0x01
/* Private typedef----------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables--------------------------------------------------------------------------*/
struct MD950_VALUE TRD_MD950_Value;

static CAN_FrameInfoTypeDef  ReceFrame;
static CAN_FrameInfoTypeDef  SendFrame;

/***********************************************************************************************
* Function		: BSP_CAN_Init
* Description	: can�ⲿ���ó�ʼ��
* Input			:
* Return		:
* Note(s)		: BSP_CAN_Set �����ðɲ����ʵ�����Ҳ���ϣ����ڲ�֪��can�ֳ�ʹ���Ƿ������
*				  ���ֲ�������ʱ�����ʹ̶��ķ�ʽ����CAN_Configuration��ֱ�����úá�
* Contributor	: hejian 2017/9/14
***********************************************************************************************/
void  BSP_CAN1_Init(_BSPCAN_PORT num)
{
    CAN_InitTypeDef CAN_InitStruct;

    CAN1_InitPad();

    CAN_InitStruct.CAN_Mode = CAN_MODE_FILTER_SIGLE;//CAN����
    CAN_InitStruct.CAN_BTR0 = CAN_BTR0_SJW3 | CAN_BTR0_BRP(2);
    CAN_InitStruct.CAN_BTR1 = CAN_BTR1_SAM_ONCE | CAN_BTR1_TSEG2(7) | CAN_BTR1_TSEG1(15);
    CAN_InitStruct.CAN_OCM  = CAN_OCR_OCM2;
    CAN_InitStruct.CAN_CDC  = CAN_ODR_CLKOUT_OFF;//CAN_ODR_CLKOUT_DIV6;
    CAN_InitStruct.CAN_ACR = 0xFF01FFFF; /* �Ӹ�λ����λ������ACR0,ACR1,ACR2,ACR3 */
    CAN_InitStruct.CAN_AMR = 0xFFFFFFFF; /* �Ӹ�λ����λ������AMR0,AMR1,AMR2,AMR3 */
    CAN_Init(SGCC_CAN1_P, &CAN_InitStruct);					//CAN��ʼ��

    NVIC_EnableIRQ(CAN1_IRQn);											//����ж�����
    NVIC_SetPriority(CAN1_IRQn, 0);

    //	CAN_ITConfig(SGCC_CAN1_P, 0xFF, ENABLE);				//CAN0ʹ���ж�
    CAN_ITConfig(SGCC_CAN1_P, 0x01, ENABLE); //ֻʹ�ܽ����ж�
}


/**
  * @brief  ����CAN1������ 
			20201209 ����Ϊ���н��գ������ԣ�δ������������
  * @param  none
  * @retval None
  */
void can1_rev_thread_entry(void *parameter)
{
//	DEBUG_PRINTF("%s\n",(char *)s);	
	//CAN0_init();	

	BSP_CAN1_Init(CAN_PORT0);
	for(;;)
	{
		
		rt_mq_recv(mq_can1_rev, &ReceFrame, sizeof(ReceFrame), RT_WAITING_FOREVER);

		
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
		
		CAN_SendData(SGCC_CAN1_P, &SendFrame, CAN_COMMAND_TRANSMISSION_REQUEST); 	//CAN0��������	
	}	
}

void can1_send_thread_entry(void *parameter)
{
	
	for(;;)
	{	
		rt_mq_recv(mq_can1_send, &ReceFrame, sizeof(ReceFrame), RT_WAITING_FOREVER);
		CAN_SendData(SGCC_CAN1_P, &ReceFrame, CAN_COMMAND_TRANSMISSION_REQUEST); 
		rt_thread_delay(10);
	}	
}

/**
  * @brief  ����CAN2�������ݵ����ϣ������ڴ桢���
  * @param  msgid  �� service_code ����PCSģ�� ��  DC400_add  ��DC400_data ����DCģ��
  *         20210802������MD950 DCģ���ͨѶ��ͬʱ����CAN2������mgrn PCSͨ�ţ������޸���mgrn��ص���Ϣ 
			msgid�޸�Ϊmsgtype ���൱�ڹ����롣service_code�޸�Ϊvaluetype �൱�ڵ�ַ
  * @retval None
  */

uint8_t CAN2_Post(enum MODULE module ,uint8_t msgtype,uint16_t valuetype,uint16_t DC400_add,uint32_t data)
{
		
	if(module == CAN_DC400){  //����DC400ģ������

	}
	else if(module == CAN_MD950){  //ֱ��ĸ��֮��dc-dcģ��
		
		SendFrame.FrameFormat = CAN_FRAME_FORMAT_EFF;
		SendFrame.FrameType = CAN_FRAME_TYPE_DATA;
		SendFrame.FrameID = 0x06B7F803;
		SendFrame.DataLength = 8;
		
		SendFrame.FrameData[0] = msgtype;
		SendFrame.FrameData[1] =  ERRTYPE;
		SendFrame.FrameData[2] =   (valuetype>>8) & 0xff;   //��ַ��;
		SendFrame.FrameData[3] =   valuetype & 0xff;         // ��ַ��;
		SendFrame.FrameData[4] =  (data>>24) & 0xff;  //����;
		SendFrame.FrameData[5] =  (data>>16) & 0xff;
		SendFrame.FrameData[6] =  (data>>8) & 0xff;
		SendFrame.FrameData[7] =  (data>>0) & 0xff;
		
	}	
	rt_mq_send (mq_can1_send,&SendFrame, sizeof(SendFrame));
}


/**
  * @brief  ��ʮ��������ת����IEEE754���������ݣ���������û������
  * @param  None
  * @retval None
 �����ȸ����� float��32 λ������λ S ռ 1 bit��ָ�� E ռ 8 bit��β�� M ռ 23 bit
 β�� M �ĵ�һλ���� 1(��Ϊ 1 <= M < 2)�������� 1 ����ʡ�Բ�д�����Ǹ�����λ������������ 23 λβ�����Ա�ʾ�� 24 λ��Ч����
  */
uint32_t data2hex(uint32_t data)  
{
	uint32_t temp,temp2;  
	uint8_t i=0;  
	
	temp = data;//ת����Ҫ�������ֵ  
	while(temp)  
	{         
		temp >>= 1;  
		i++;//���㵱ǰֵ��β��ռ�е�λ��  
	}  
	i--;//����������i����һ�Σ��������    
	temp = data;  //�ٴθ�ֵ��׼���ϲ�        
	temp = temp<<(23-1-i);//����23λβ��  ����23-i���ԣ�Ӧ����22-i 
	
	temp = temp&0x003FFFFF; //��β�������λ��������β����λ����
	temp = temp<<1; //�൱��ȥ����β�������λ
	
	temp = (i+127)<<23 | temp;//����ָ��������β���ϲ����� 
	temp = temp & ~(1<<31);//ȷ������  
	
	return temp;//������Ѿ����Ը������Ĵ洢��ʽ��ʾ�Ĵ������Ĳ�����
}


//float data2; 
//data��Һ���ϴ���16/32λ������data2Ϊ��������float���ݣ�data3Ϊ��Ҫ�·���DCģ������� ��float���ݵ�IEEE754 32λ��ʽ
void send_can2_MD950(uint16_t modbus_add,uint32_t data)
{
	uint16_t add;
	float data2;
	int *p = (int *)&data2;

	uint32_t data3;
	switch(modbus_add)
	{
		case 5000:  //���ػ�
			add = 0x0030;
			data3 = data<<16;
			break;
		case 5001://���ʷ���
			add = 0x002F;
			data3 = data<<16;
			break;
		case 5002://A�ڵ�ѹ
			add = 0x001D;
			data3 = data2hex(data);
			break;
		case 5003://A������
			add = 0x001A;
			data2 = data/15.789474;
			data3 = *p;
			break;
		case 5004://B�ڵ�ѹ
			add = 0x0021;
			data3 = data2hex(data);
			break;
		case 5005://B������
			add = 0x0022;
			data2 = data/16.666667;
			data3 = *p;
			break;
		case 5006:// p
			add = 0x0020;   //�ο�DT950N15BGģ�����CANͨѶЭ��.pdf 12ҳ���24  ���跶Χ0.1-1  ��������1��ʱ�����15kw �������Ҫ�������Ϊ1kw����Ҫ����1/15
			//data2 = data/15.001; //�������������15�� ��Ĭ���ǰ������������㣬���߽�dataǿ��ץ��Ϊfloat
			data2 = data/150.01; //�޸�Һ�����õ�λΪ0.1kw,������	
			data3 = *p;
			break;
		default:
			break;

	}
	CAN2_Post(CAN_MD950 ,SET_DC950,add,0,data3);
}


/*-----------------------------------------------------------------------------
  Function:    CAN2_IRQHandler
  input:       None
  Returns:     None
  Description: CAN2 interrupt handler
 *-----------------------------------------------------------------------------*/
void CAN1_IRQHandler(void)
{
    volatile uint8_t temp;
	
    temp = CAN_GetITFlag(SGCC_CAN1_P);							//��ȡ�жϱ�־λ

    /* �����ж�, ��λ��Ҫд����λCMR.2,�ͷŽ��ջ����� */
    if((temp & CAN_IT_RI) == CAN_IT_RI)						//���ձ�־λ
    {
        CAN_ReceData(SGCC_CAN1_P, &ReceFrame);				//�������ݣ�����ӡ�����Ϣ
		//���͵�����
		rt_mq_send (mq_can1_rev,&ReceFrame, sizeof(ReceFrame));

        /*if(BSPCAN_Control[num].Rxcount < BSPCAN_Control[num].pCanElem->RxMaxNum)
        {
        	memcpy(&BSPCAN_Control[num].pCanElem->RxMessage[BSPCAN_Control[num].Rxrear], &RxMessage, sizeof(CanRxMsg));
        	BSPCAN_Control[num].Rxrear = ((BSPCAN_Control[num].Rxrear + 1) %  BSPCAN_Control[num].pCanElem->RxMaxNum);
        	BSPCAN_Control[num].Rxcount++;
        }
        */
    }
}

