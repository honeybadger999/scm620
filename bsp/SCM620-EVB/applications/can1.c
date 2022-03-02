#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_can.h"
/* Private define-----------------------------------------------------------------------------*/

#define ERRTYPE     0xF0  //无错误

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
* Description	: can外部调用初始化
* Input			:
* Return		:
* Note(s)		: BSP_CAN_Set 里或许该吧波特率的配置也做上，由于不知道can现场使用是否会碰见
*				  多种波特率暂时波特率固定的方式。在CAN_Configuration里直接配置好。
* Contributor	: hejian 2017/9/14
***********************************************************************************************/
void  BSP_CAN1_Init(_BSPCAN_PORT num)
{
    CAN_InitTypeDef CAN_InitStruct;

    CAN1_InitPad();

    CAN_InitStruct.CAN_Mode = CAN_MODE_FILTER_SIGLE;//CAN设置
    CAN_InitStruct.CAN_BTR0 = CAN_BTR0_SJW3 | CAN_BTR0_BRP(2);
    CAN_InitStruct.CAN_BTR1 = CAN_BTR1_SAM_ONCE | CAN_BTR1_TSEG2(7) | CAN_BTR1_TSEG1(15);
    CAN_InitStruct.CAN_OCM  = CAN_OCR_OCM2;
    CAN_InitStruct.CAN_CDC  = CAN_ODR_CLKOUT_OFF;//CAN_ODR_CLKOUT_DIV6;
    CAN_InitStruct.CAN_ACR = 0xFF01FFFF; /* 从高位到低位依次是ACR0,ACR1,ACR2,ACR3 */
    CAN_InitStruct.CAN_AMR = 0xFFFFFFFF; /* 从高位到低位依次是AMR0,AMR1,AMR2,AMR3 */
    CAN_Init(SGCC_CAN1_P, &CAN_InitStruct);					//CAN初始化

    NVIC_EnableIRQ(CAN1_IRQn);											//添加中断向量
    NVIC_SetPriority(CAN1_IRQn, 0);

    //	CAN_ITConfig(SGCC_CAN1_P, 0xFF, ENABLE);				//CAN0使能中断
    CAN_ITConfig(SGCC_CAN1_P, 0x01, ENABLE); //只使能接收中断
}


/**
  * @brief  接收CAN1的任务 
			20201209 调整为队列接收，经测试，未发生丢包现象
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
		
		CAN_SendData(SGCC_CAN1_P, &SendFrame, CAN_COMMAND_TRANSMISSION_REQUEST); 	//CAN0发送数据	
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
  * @brief  用于CAN2发送数据的整合，开辟内存、入队
  * @param  msgid  、 service_code 用于PCS模块 ；  DC400_add  、DC400_data 用于DC模块
  *         20210802增加了MD950 DC模块的通讯。同时由于CAN2不再与mgrn PCS通信，所以修改了mgrn相关的信息 
			msgid修改为msgtype ，相当于功能码。service_code修改为valuetype 相当于地址
  * @retval None
  */

uint8_t CAN2_Post(enum MODULE module ,uint8_t msgtype,uint16_t valuetype,uint16_t DC400_add,uint32_t data)
{
		
	if(module == CAN_DC400){  //发送DC400模块数据

	}
	else if(module == CAN_MD950){  //直流母线之间dc-dc模块
		
		SendFrame.FrameFormat = CAN_FRAME_FORMAT_EFF;
		SendFrame.FrameType = CAN_FRAME_TYPE_DATA;
		SendFrame.FrameID = 0x06B7F803;
		SendFrame.DataLength = 8;
		
		SendFrame.FrameData[0] = msgtype;
		SendFrame.FrameData[1] =  ERRTYPE;
		SendFrame.FrameData[2] =   (valuetype>>8) & 0xff;   //地址高;
		SendFrame.FrameData[3] =   valuetype & 0xff;         // 地址低;
		SendFrame.FrameData[4] =  (data>>24) & 0xff;  //数据;
		SendFrame.FrameData[5] =  (data>>16) & 0xff;
		SendFrame.FrameData[6] =  (data>>8) & 0xff;
		SendFrame.FrameData[7] =  (data>>0) & 0xff;
		
	}	
	rt_mq_send (mq_can1_send,&SendFrame, sizeof(SendFrame));
}


/**
  * @brief  将十进制数据转换成IEEE754浮点型数据，初步测试没有问题
  * @param  None
  * @retval None
 单精度浮点数 float：32 位，符号位 S 占 1 bit，指数 E 占 8 bit，尾数 M 占 23 bit
 尾数 M 的第一位总是 1(因为 1 <= M < 2)，因此这个 1 可以省略不写，它是个隐藏位，这样单精度 23 位尾数可以表示了 24 位有效数字
  */
uint32_t data2hex(uint32_t data)  
{
	uint32_t temp,temp2;  
	uint8_t i=0;  
	
	temp = data;//转存需要计算的数值  
	while(temp)  
	{         
		temp >>= 1;  
		i++;//计算当前值的尾数占有的位数  
	}  
	i--;//计算下来，i会多加一次，这儿减掉    
	temp = data;  //再次赋值，准备合并        
	temp = temp<<(23-1-i);//补足23位尾数  这里23-i不对，应该是22-i 
	
	temp = temp&0x003FFFFF; //将尾数的最高位和其它非尾数的位清零
	temp = temp<<1; //相当于去掉了尾数的最高位
	
	temp = (i+127)<<23 | temp;//计算指数，并与尾数合并起来 
	temp = temp & ~(1<<31);//确定正负  
	
	return temp;//这里就已经是以浮点数的存储方式表示的传进来的参数了
}


//float data2; 
//data是液晶上传的16/32位整数，data2为计算完后的float数据，data3为需要下发给DC模块的数据 是float数据的IEEE754 32位格式
void send_can2_MD950(uint16_t modbus_add,uint32_t data)
{
	uint16_t add;
	float data2;
	int *p = (int *)&data2;

	uint32_t data3;
	switch(modbus_add)
	{
		case 5000:  //开关机
			add = 0x0030;
			data3 = data<<16;
			break;
		case 5001://功率方向
			add = 0x002F;
			data3 = data<<16;
			break;
		case 5002://A口电压
			add = 0x001D;
			data3 = data2hex(data);
			break;
		case 5003://A口限流
			add = 0x001A;
			data2 = data/15.789474;
			data3 = *p;
			break;
		case 5004://B口电压
			add = 0x0021;
			data3 = data2hex(data);
			break;
		case 5005://B口限流
			add = 0x0022;
			data2 = data/16.666667;
			data3 = *p;
			break;
		case 5006:// p
			add = 0x0020;   //参考DT950N15BG模块对外CAN通讯协议.pdf 12页序号24  可设范围0.1-1  所以设置1的时候代表15kw 所以如果要设置输出为1kw，需要设置1/15
			//data2 = data/15.001; //如果处于整数（15） 则默认是按照整数来运算，或者将data强制抓换为float
			data2 = data/150.01; //修改液晶设置单位为0.1kw,待测试	
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
	
    temp = CAN_GetITFlag(SGCC_CAN1_P);							//获取中断标志位

    /* 接收中断, 此位需要写命令位CMR.2,释放接收缓冲区 */
    if((temp & CAN_IT_RI) == CAN_IT_RI)						//接收标志位
    {
        CAN_ReceData(SGCC_CAN1_P, &ReceFrame);				//接收数据，并打印相关信息
		//发送到队列
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

