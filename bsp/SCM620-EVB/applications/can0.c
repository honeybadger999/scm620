#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_can.h"
/* Private define-----------------------------------------------------------------------------*/
#define MAX_MAIL_NUM        3  // 邮箱个数
#define MAX_CAN_NUM         2  // CAN的个数

/* Private typedef----------------------------------------------------------------------------*/
/* Private macro------------------------------------------------------------------------------*/
/* Private variables--------------------------------------------------------------------------*/

static CAN_FrameInfoTypeDef  ReceFrame;
static CAN_FrameInfoTypeDef  SendFrame;

CAN_InitTypeDef CAN_InitStruct;

/***********************************************************************************************
* Function		: BSP_CAN_Init
* Description	: can外部调用初始化
* Input			:
* Return		:
* Note(s)		: BSP_CAN_Set 里或许该吧波特率的配置也做上，由于不知道can现场使用是否会碰见
*				  多种波特率暂时波特率固定的方式。在CAN_Configuration里直接配置好。
* Contributor	: hejian 2017/9/14
***********************************************************************************************/
void  BSP_CAN0_Init(_BSPCAN_PORT num)
{
    CAN_InitTypeDef CAN_InitStruct;

    CAN0_InitPad(CAN0_PIN103RXD, CAN0_PIN104TXD0);		//初始化CAN引脚

    CAN_InitStruct.CAN_Mode = CAN_MODE_FILTER_SIGLE;//CAN设置  单过滤器。 使用一个 4 字节过滤器过滤所接收数据

//    CAN_InitStruct.CAN_BTR0 = CAN_BTR0_SJW3 | CAN_BTR0_BRP(5); //总线时序0  总线时许1  P329   62.5m
	CAN_InitStruct.CAN_BTR0 = CAN_BTR0_SJW3 | CAN_BTR0_BRP(2); //125k
    CAN_InitStruct.CAN_BTR1 = CAN_BTR1_SAM_ONCE | CAN_BTR1_TSEG2(7) | CAN_BTR1_TSEG1(15);

    CAN_InitStruct.CAN_OCM  = CAN_OCR_OCM2; //输出控制模式：正常
    CAN_InitStruct.CAN_CDC  = CAN_ODR_CLKOUT_OFF;//CAN_ODR_CLKOUT_DIV6; 输出时钟和分频控制 ：输出关闭
    CAN_InitStruct.CAN_ACR = 0xFF01FFFF; /* 从高位到低位依次是ACR0,ACR1,ACR2,ACR3 */ //验收代码
    CAN_InitStruct.CAN_AMR = 0xFFFFFFFF; /* 从高位到低位依次是AMR0,AMR1,AMR2,AMR3 */  //验收代码屏蔽
    CAN_Init(SGCC_CAN0_P, &CAN_InitStruct);					//CAN初始化


    NVIC_EnableIRQ(CAN0_IRQn);//添加中断向量
    NVIC_SetPriority(CAN0_IRQn, 0);
//	CAN_ITConfig(SGCC_CAN0_P, 0xFF, ENABLE);//CAN0使能中断   0xFF:使能所有的中断
    CAN_ITConfig(SGCC_CAN0_P, 0x01, ENABLE); //只使能接收中断

//    BSPCAN_Control[num].pCanElem = pbxCAN;

}


/**
  * @brief  接收CAN1的任务 
			20201209 调整为队列接收，经测试，未发生丢包现象
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

		//云谷园项目 没有电池，为pcs做假通讯
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
		
		CAN_SendData(SGCC_CAN0_P, &SendFrame, CAN_COMMAND_TRANSMISSION_REQUEST); 	//CAN0发送数据	
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

    temp = CAN_GetITFlag(SGCC_CAN0_P);							//获取中断标志位

    /* 接收中断, 此位需要写命令位CMR.2,释放接收缓冲区 */
    if((temp & CAN_IT_RI) == CAN_IT_RI)						//接收标志位
    {
        CAN_ReceData(SGCC_CAN0_P, &ReceFrame);				//接收数据，并打印相关信息

		//发送到队列
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

