#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_uart.h"
#include "LE_sst_flash.h"
#include "RX8025T.h"
#include "eth.h"
#include "LE_can.h"

static uint8_t TxCount = 0;
rt_uint8_t uart7_stack[256];
struct rt_thread uart7_thread;
uint8_t Uart7_rev[MAX_REV_QUANT];
uint16_t Uart7_rev_counter,com7_rxlen;

uint8_t  Uart7_Send_Buf[MAX_SEND_QUANT];
struct CRC_TABLE crc_tab;
uint16_t modbus_len,modbus_crc_code;

struct STRUCT_LEUER_Data LEUER_Data;
struct COM_Info Pcs_ComInfo,DC1_ComInfo,DC2_ComInfo;
struct DC_Data DC2data;

struct STRUCT_PCS PCSdata;
uint16_t  time_uart[6];
uint16_t *P_DC2data = ( uint16_t *)&DC2data;	
uint16_t *P_EMS = ( uint16_t *)&LEUER_Data.EMS_Data;
uint16_t *P_PCSdata = ( uint16_t *)&PCSdata;
uint16_t *P_MD950 = ( uint16_t *)&TRD_MD950_Value;

float PVAC_Power[3];

/* 串 口 接 收 消 息 结 构*/
struct rx_msg
{
	rt_device_t dev;
	rt_size_t size;
};
/* 消 息 队 列 控 制 块 */
static struct rt_messagequeue rx_mq;
/* 接 收 数 据 回 调 函 数 */
/*
static rt_err_t uart7_input(rt_device_t dev, rt_size_t size)
{
    struct rx_msg msg;
    rt_err_t result;
    msg.dev = dev;
    msg.size = size;
    result = rt_mq_send(&rx_mq, &msg, sizeof(msg));

    if ( result == -RT_EFULL)
    {
        //消 息 队 列 满 
        rt_kprintf("message queue full！ \n");
    }

    return result;
}
*/


/*******************************************************************************************************
** 函数名称 ：Crc_Make()
** 函数功能 ：计算CRC校验码。
** 入口参数1：buff：待计算CRC校验代码的指针；
** 入口参数2：size：从buff指针的首地址开始的字节数
** 出口参数 ：无（返回值为指针crc）	
*******************************************************************************************************
*/
static uint16_t Uart7_Crc_Make(uint16_t size, uint8_t *buff)
{
	uint16_t car, i;
	uint16_t crc[2];
	uint16_t CrcCodeTmp;

	crc[0] = 0xff;
	crc[1] = 0xff;
	for ( i = 0; i < size; i++ ) {
	car = buff[i];
	car ^= crc[0];
		
	if(car>256)   //20190916 发现这里有小概率会溢出，增加直接返回
		return 0;
		
	crc[0] = crc[1] ^ crc_tab.TABLE2[car];
	crc[1] = crc_tab.TABLE1[car];
	}
	/*
	CrcCodeTmp=crc[0];
	crc[0]=crc[1];
	crc[1]=CrcCodeTmp;
	return (*(uint16_t*)(&crc[0]));
	*/

	CrcCodeTmp = (crc[0]<<8)+crc[1];
	return CrcCodeTmp;

}

/***********************************************************/ 
static uint16_t	Uart7_crc_check( uint16_t data_len, uint8_t *data )
{
	uint16_t	Crc_Code;
 	uint16_t	Crc_LowCode, Crc_HighCode;
	Crc_Code = Uart7_Crc_Make( data_len, data);	//对接收数据计算CRC校验码
 	Crc_HighCode = data[ data_len ];
 	Crc_LowCode = data[ data_len + 1 ]; 	
 	if( Crc_Code == ( Crc_HighCode * 0x100 + Crc_LowCode ) )		//校验码正确
		return	1;
 		else
 			return	0; 	
}


/***************************************
	us延时函数
	精确测量168m主频 time_us=1 延时1us
**************************************/
static void Uart7_Delay_nus(uint16_t time_us)  
{
	uint16_t k1,k2;
	for(k1=time_us;k1>0;k1--)  
	{
		for(k2=33;k2>0;k2--)
		{
		}
	}
}

/**************************************************************************************************
*	函数名称：Send_Uart1	
*	参数：*buff	发送数据内存  len发送数据长度
*	
*	没有设置DMA发送完成的中断，而是采用查询方式，同时去掉调度器挂起恢复操作，在等待时期可以切换其它
*   任务，目的是提高执行效率
**************************************************************************************************/
static void Uart7_Send(const uint8_t *buff,uint8_t len)
{
	//	vTaskSuspendAll();
//    usart_write(7, Uart7_Send_Buf, modbus_len);
	uint16_t i;
	USART7_TX_EN;
	Uart7_Delay_nus(2000);

	for (i = 0; i < len; i++)
	{
		UART_PutChar(SGCC_UART7_P, buff[i]);
	}

	Uart7_Delay_nus(5000);  //经过测试  时间设置太短会影响发送  5ms 发送50个字节没问题
	USART7_RX_EN;
//	xTaskResumeAll();//恢复调度器
}

/*
*******************************************************************************************************
** 函数名称 ：handle_modbus_03h()
** 函数功能 ：处理modbus的03H功能。
**  点表里的地址为实际地址，在此函数不需要减去1  液晶屏需要在点表地址基础上加1
*******************************************************************************************************
*/
static void handle_modbus_03h(uint16_t modbus_add,uint16_t modbus_dataNum)
{
	uint16_t i;

	Uart7_Send_Buf[0] = Uart7_rev[0];
	Uart7_Send_Buf[1] = 0x03;
	Uart7_Send_Buf[2] = modbus_dataNum<<1;

	for(i=0;i<modbus_dataNum;i++)				
	{	
		if((modbus_add>=100)&&(modbus_add<=150))//EMS
		{
			Uart7_Send_Buf[3+i*2] = (*(P_EMS-100+modbus_add+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = *(P_EMS-100+modbus_add+i) & 0x0FF;
		}
		else if((modbus_add>=151)&&(modbus_add<=153))//
		{
			Uart7_Send_Buf[3+i*2] =((uint16_t)PVAC_Power[modbus_add-151+i]>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = ((uint16_t)PVAC_Power[modbus_add-151+i]) & 0x0FF;
		}
		else if((modbus_add>=160)&&(modbus_add<=166))  //time
		{
			Uart7_Send_Buf[3+i*2] =(time_uart[modbus_add-160+i]>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = (time_uart[modbus_add-160+i]) & 0x0FF;
		}
		else if((modbus_add>=170)&&(modbus_add<=183))  //以太网参数 mac ip
		{
			Uart7_Send_Buf[3+i*2] =(*(P_ethernet+modbus_add-170+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = *(P_ethernet+modbus_add-170+i) & 0x0FF;
		}
		else if((modbus_add>=2200)&&(modbus_add<=2282))    // DC2 所有数据读取 
		{
			Uart7_Send_Buf[3+i*2] = (*(P_DC2data+modbus_add-2200+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = *(P_DC2data+modbus_add-2200+i) & 0x0FF;	
		}
		else if((modbus_add>=3000)&&(modbus_add<=3149))    // PCS所有数据读取
		{
			Uart7_Send_Buf[3+i*2] = (*(P_PCSdata+modbus_add-3000+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = *(P_PCSdata+modbus_add-3000+i) & 0x0FF;	
		}
		else if((modbus_add>=5000)&&(modbus_add<=5030))    //母线DC-DC 
		{
			Uart7_Send_Buf[3+i*2] = (*(P_MD950+modbus_add-5000+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = *(P_MD950+modbus_add-5000+i) & 0x0FF;	
		}

		else
			return;
	}

	modbus_len = 3 + modbus_dataNum*2;
	modbus_crc_code = Uart7_Crc_Make(modbus_len,Uart7_Send_Buf);
	Uart7_Send_Buf[modbus_len++] = (modbus_crc_code>>8) & 0x0FF;
	Uart7_Send_Buf[modbus_len++] = modbus_crc_code & 0x0FF;

	Uart7_Send(Uart7_Send_Buf,modbus_len);
}

static void handle_modbus_06h(uint16_t modbus_add,uint16_t data)
{
	uint16_t i;
	if(modbus_add ==0)
	{
		send_uart3(BROADCAST_SITE_NUMBER,0,data,NULL); // 当从触摸屏写入公共站号，直接通过uart3转发，在这个函数内写入地址641
	}
		//ems参数
	else if((modbus_add>=100)&&(modbus_add<=110)) {  //ems
		*(P_EMS+modbus_add-100) = data;
		Store_par_DataFlash(P_EMS,FLASHQUAN_INFO,FLASHSECTOR_INFO,FLASHADD_INFO);	 //存储参数
	}
	else if((modbus_add>=151)&&(modbus_add<=153)) {  //电能初值设置，不保存，在rtc中保存
		PVAC_Power[modbus_add-151] = (float)data;
	}
	else if((modbus_add>=160)&&(modbus_add<=164))    //time
	{
		time_uart[modbus_add-160] = data;
	}
	else if(modbus_add==166) // time写入
	{
		if(data== 1111){	
			timePC[0] = time_uart[0];timePC[1] = time_uart[1];timePC[2] = time_uart[2];
			timePC[3] = time_uart[3];timePC[4] = time_uart[4];timePC[5] = time_uart[5];
			rt_event_send(&com_event, EVENT_FLAG_RTC_WRITE);
		}
		else if(data== 2222){	
			time_uart[0] = timePC[0];time_uart[1] = timePC[1];time_uart[2] = timePC[2];
			time_uart[3] = timePC[3];time_uart[4] = timePC[4];time_uart[5] = timePC[5];
		}
	}
	else if((modbus_add>=170)&&(modbus_add<=183))  //以太网参数 mac ip
	{
		*(P_ethernet+modbus_add-170) = data;
	}
	else if(modbus_add==184) // 以太网写入
	{
		if(data== 1111){	
			Store_par_DataFlash(P_ethernet,FLASHQUAN_ETH,FLASHSECTOR_ETH,FLASHADD_ETH);//写入flash
		}
	}
	
	//DC2 2206-2217为不可保存参数 2270-2282为可保存参数
	else if(((modbus_add>=2270)&&(modbus_add<=2282))||((modbus_add>=2206)&&(modbus_add<=2217))) { 
		*(P_DC2data+modbus_add-2200) = data;  //写入到结构体以供显示
		send_uart3(MODULE_DC2,modbus_add,data,NULL);
	}
	else if((modbus_add>=3100)&&(modbus_add<=3143)) {  // pcs
		*(P_PCSdata+modbus_add-3100+100) = data;
		send_uart3(MODULE_PCS,modbus_add,data,MODBUS_WRITE);
	}
	else if((modbus_add>=5000)&&(modbus_add<=5010))    //母线DC-DC 
	{
		*(P_MD950+modbus_add-5000) = data;
		send_can2_MD950( modbus_add, data);	
	}
		
	else
		return;

	for(i=0;i<8;i++) //应答主机
	{
		Uart7_Send_Buf[i]= Uart7_rev[i];	
	}
	Uart7_Send(Uart7_Send_Buf,modbus_len);

}

/**
  * @brief  uart1 液晶屏 麦格米特400VDC模块参数写入，uint32格式，10功能。
  * @param  地址 数量
  * @retval None
  */
static void handle_modbus_10h(uint16_t modbus_add,int16_t quant)
{
	uint16_t i,modbus_crc_code;
	uint32_t data;
	
	if((modbus_add>=3100)&&(modbus_add<=3149) && (quant == 2)) //PCS模块
	{

		*(P_PCSdata+modbus_add-3100+100) = (Uart7_rev[7]<<8)+Uart7_rev[8]; 
		*(P_PCSdata+modbus_add-3100+100+1) = (Uart7_rev[9]<<8)+Uart7_rev[10]; 
		data = *(P_PCSdata+modbus_add-3100+100+1) ; 
		data = (data<<16) + *(P_PCSdata+modbus_add-3100+100);
		send_uart3(MODULE_PCS,modbus_add,data,MODBUS_WRITE10);

	}
	else
		return;

		//应答主机
	for(i=0;i<6;i++) 
	{
		Uart7_Send_Buf[i]= Uart7_rev[i];	
	}

	modbus_crc_code = Uart7_Crc_Make(6,Uart7_Send_Buf);
	Uart7_Send_Buf[6] = (modbus_crc_code>>8) & 0x0FF;
	Uart7_Send_Buf[7] = modbus_crc_code & 0x0FF;
	Uart7_Send(Uart7_Send_Buf,8);
	
}

void uart7_thread_entry(void* parameter)
{

    static rt_device_t uart7_device = RT_NULL;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初 始 化 配 置 参 数 */
//	static char msg_pool[256];
	struct rx_msg msg;
	rt_err_t result;
	rt_uint16_t rx_length;
	rt_uint32_t e;
	uint16_t Uart7_modbus_address;		
	uint16_t Uart7_modbus_quant;
//	static char rx_buffer[RT_SERIAL_RB_BUFSZ + 1];
	
    GPIO_InitPad(58); //485_con7
    GPIO_ConfigDir(58, GPIO_OUTPUT);
    GPIO_WrDataOut(58, GPIO_HIGH);

    /*	UART7_InitPad();												   //232  tiaoshi
        UART_Init(SGCC_UART7_P, 9600, PARITY_NONE, UART_CTRL_STOP_1BIT); //注英pclk SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4 为18.75M
        UART_EnableIntr(SGCC_UART7_P, UART_CTRL_RXINTEN | UART_CTRL_TXINTDIS);
        NVIC_EnableIRQ(UART7_IRQn);
        NVIC_SetPriority(UART7_IRQn, 7); //0-7
    */

	/* 初 始 化 消 息 队 列 */
//	rt_mq_init(&rx_mq, "rx_mq",
//	msg_pool, /* 存 放 消 息 的 缓 冲 区 */
//	sizeof(struct rx_msg), /* 一 条 消 息 的 最 大 长 度 */
//	sizeof(msg_pool), /* 存 放 消 息 的 缓 冲 区 大 小 */
//	RT_IPC_FLAG_FIFO); /* 如 果 有 多 个 线 程 等 待， 按 照 先 来 先 得 到 的 方 法分 配 消 息 */

	uart7_device = rt_device_find("uart7");

    if (!uart7_device)
    {
        rt_kprintf("find %s failed!\n", "uart7_device");
        //return RT_ERROR;
    }

    /* step2： 修 改 串 口 配 置 参 数 */
    config.baud_rate = BAUD_RATE_19200; //修 改 波 特 率 为 9600
    config.data_bits = DATA_BITS_8; //数 据 位 8
    config.stop_bits = STOP_BITS_1; //停 止 位 1
    config.bufsz = 255; //修 改 缓 冲 区 buff size 为 128
    config.parity = PARITY_NONE; //无 奇 偶 校 验 位
    /* step3： 控 制 串 口 设 备。 通 过 控 制 接 口 传 入 命 令 控 制 字， 与 控 制 参 数 */
    rt_device_control(uart7_device, RT_DEVICE_CTRL_CONFIG, &config);
    /* step4： 打 开 串 口 设 备。 以 中 断 接 收 及 轮 询 发 送 模 式 打 开 串 口 设 备 */
    rt_device_open(uart7_device, RT_DEVICE_FLAG_INT_RX);
//	rt_device_open(uart7_device, RT_DEVICE_FLAG_DMA_RX);
    /* 设 置 接 收 回 调 函 数 */
//    rt_device_set_rx_indicate(uart7_device, uart7_input);
	
    while (1)
    {
		//rt_memset(&msg, 0, sizeof(msg));
		/* 从 消 息 队 列 中 读 取 消 息*/

			/* 从 串 口 读 取 数 据*/
			//rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
		rt_event_recv(&com_event, EVENT_FLAG_PS_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);	
		if((Uart7_rev[0]==20) || (Uart7_rev[0]==238))   //通用站号。
		{
			if(Uart7_crc_check(Uart7_rev_counter-2,Uart7_rev)==TRUE)
			{
				Uart7_modbus_address = (Uart7_rev[2]<<8)+Uart7_rev[3];
				Uart7_modbus_quant = (Uart7_rev[4]<<8)+Uart7_rev[5]; //quan or data
				
				switch(Uart7_rev[1])
				{
					case MODBUS_READ:
						handle_modbus_03h(Uart7_modbus_address,Uart7_modbus_quant);
					break;
	
					case MODBUS_WRITE:
						//20200420 之前函数嵌套过多，进行拆分:
						////1 写入结构体并根据地址判断模块 ，应答触摸屏
						handle_modbus_06h(Uart7_modbus_address,Uart7_modbus_quant);
						//2 判断站号并写入，同时将module赋值为公共站号（仅用于判断）
						//judge_portnumber(Uart1_modbus_address,&Module); 
						//3 //将地址转换成各个模块的通讯地址并通过Uart3_Post写入到发送队列
						//send_uart3(Module,Uart1_modbus_address,Uart1_modbus_quant);
					break;
					case 0x01:
					//	handle_modbus_01h(Uart7_modbus_address,Uart7_modbus_quant);
					break;
					case 0x05:
					//	handle_modbus_05h(Uart7_modbus_address,Uart7_modbus_quant);
					break;
					case 0x10:
						handle_modbus_10h(Uart7_modbus_address,Uart7_modbus_quant);
					break;
					default:
					break;
				}
			}
		}
		/* 打 印 数 据 */
		//rt_kprintf("%s\n",Uart7_rev);	
    }
}

void UART7_IRQHandler(void)
{
    UINT32 intrStatus, intrEnSts;
    UINT8 rxData;
    SGCC_UART_TypeDef *UARTx;
    UARTx = (SGCC_UART_TypeDef *)SGCC_UART7_BASE;
    intrStatus = UARTx->INTSTA;
    intrEnSts = UARTx->CTRL;

    if (intrStatus & UART_INTSTATUS_stop_error)
    {
        UART_ClearIntr(UARTx, UART_INTSTATUS_stop_error); //clr int bit
    }

    if(intrStatus & 0x04)
    {
        UARTx->INTSTA |= 0x04;
    }

    if (intrStatus & UART_INTSTATUS_parity_error)
    {
        UART_ClearIntr(UARTx, UART_INTSTATUS_parity_error); //clr int bit
    }

    if (intrStatus & UART_INTSTATUS_RXINT)
    {
        //clear interrupt
        UART_ClearIntr(UARTx, UART_INTSTATUS_RXINT);

        if ((UARTx->STATUS & UART_STATUS_RXEMP) == 0)
        {
            rxData = UARTx->RXDATA;

            if (com7_rxlen < MAX_REV_QUANT)
                Uart7_rev[com7_rxlen++] = rxData;
			
			if(com7_rxlen == 1) //开始接收
				rt_event_send(&com_event, EVENT_FLAG_RV_UART7);
        }
    }

    if ((intrStatus & UART_INTSTATUS_TXINT) && (intrEnSts & UART_CTRL_TXINTEN))
    {
        //clear interrupt
        UART_ClearIntr(UARTx, UART_INTSTATUS_TXINT);
        TxCount++;

        if (TxCount == 10)
        {
            UART_EnableIntr(UARTx, UART_CTRL_RXINTEN | UART_CTRL_TXINTDIS);
        }
    }
}

/* 定 时 器 1 超 时 函 数 */
void timeout_uart7(void *parameter)
{
//    rt_kprintf("periodic timer is timeout \n");
    static uint16_t com7_rx_sta = 0, last_com7_rx_sta = 0, last_com7_rxlen = 0; //com状态
    //接收事件，阻塞，中断在开始接收后 发送事件
//	rt_event_recv(&com_event,EVENT_FLAG_RV_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);

	if(com7_rxlen > last_com7_rxlen )
	{
		//正在接收状态
		com7_rx_sta = 1;
	}
	else if(com7_rxlen == last_com7_rxlen )
	{
		//脱离接收状态
		com7_rx_sta = 0;
	}
	else
	{
		//接收完毕 ，Uart7_rev_counter被置0,Uart7_rev_counter应在解析函数中置零
	}

	if((com7_rx_sta == 0) && (last_com7_rx_sta == 1))
	{
		Uart7_rev_counter = com7_rxlen;
		rt_event_send(&com_event, EVENT_FLAG_PS_UART7);
		com7_rxlen=0;
		//发送信号量
	}

	last_com7_rx_sta = com7_rx_sta;
	last_com7_rxlen = com7_rxlen;
//	rt_thread_delay(10);
}


//将地址转换成各个模块的通讯地址并通过Uart3_Post写入到发送队列
void send_uart3(uint16_t module,uint16_t add,uint32_t data,uint16_t fuccode)
{
	uint16_t add_Module = 0;
	switch(module)
	{
	
		case MODULE_DC2: 
		{
			if((add>=2270)&&(add<=2282)){ 
				add_Module = add+DC_SET_START_ADD-2270; 			
				Uart0_Post(DC2_485PORT,MODBUS_WRITE10,add_Module,data);
			}
			else if((add>=2206)&&(add<=2217)){ 
				add_Module = add-2200; 		
				Uart0_Post(DC2_485PORT,MODBUS_WRITE10,add_Module,data);
			}
			add_Module = 0;
		}
		break;
		
		case MODULE_PCS: //16bit和32bit的数据是混在一起的 所以funcode为06或者10
			if(add<=3134){
				add_Module = add-3000-100+PCS_SET_START_ADD;
			}
			else if((add==3140)||(add==3142)){
				add_Module = add-3140+0x2800;
			}
			else if((add>=3144)&&(add<=3145)){
				add_Module = add-3144+0x2900;
			}
			else if((add>=3146)&&(add<=3149)){
				add_Module = add-3146+0x2A03;
			}
			Uart0_Post(PCS_485PORT,fuccode,add_Module,data);				
		break;

		case BROADCAST_SITE_NUMBER:  
		{
			add_Module = 641;
			Uart0_Post(BROADCAST_SITE_NUMBER,MODBUS_WRITE,add_Module,data);
			add_Module = 0;
		}
		break;	
		default:
		break;
	}
}
