#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_uart.h"


static uint8_t TxCount = 0;
struct rt_thread uart0_read_thread;
struct rt_thread uart0_rev_thread;
uint8_t Uart0_rev[MAX_REV_QUANT];
uint16_t Uart0_rev_counter,com0_rxlen;

uint8_t  Uart0_Send_Buf[MAX_SEND_QUANT];
static uint16_t modbus_len,modbus_crc_code;

uint16_t PCS_Read_Block;

/*******************************************************************************************************
** 函数名称 ：Crc_Make()
** 函数功能 ：计算CRC校验码。
** 入口参数1：buff：待计算CRC校验代码的指针；
** 入口参数2：size：从buff指针的首地址开始的字节数
** 出口参数 ：无（返回值为指针crc）	
*******************************************************************************************************
*/
static uint16_t Uart0_Crc_Make(uint16_t size, uint8_t *buff)
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
static uint16_t	Uart0_crc_check( uint16_t data_len, uint8_t *data )
{
	uint16_t	Crc_Code;
 	uint16_t	Crc_LowCode, Crc_HighCode;
	Crc_Code = Uart0_Crc_Make( data_len, data);	//对接收数据计算CRC校验码
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
static void Uart0_Delay_nus(uint16_t time_us)  
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
static void Uart0_Send(const uint8_t *buff,uint8_t len)
{
	//	vTaskSuspendAll();
//    usart_write(7, Uart7_Send_Buf, modbus_len);
	uint16_t i;
	USART0_TX_EN;
	Uart0_Delay_nus(2000);

	for (i = 0; i < len; i++)
	{
		UART_PutChar(SGCC_UART0_P, buff[i]);
	}

	Uart0_Delay_nus(5000);  //经过测试  时间设置太短会影响发送  5ms 发送50个字节没问题
	USART0_RX_EN;
//	xTaskResumeAll();//恢复调度器
}

/****************************************************
被调用函数：Uart3_REV_thread
函数功能：处理接收到的DC数据，放入相关结构体，give信号量
调用函数：无
*****************************************************/
void handle_DC_DC2()
{	
	uint16_t i,quantity;
	if(Uart0_rev[1] == 3)
	{
		quantity = (Uart0_rev[2]>>1); //寄存器数量
		if(quantity>DC_READ_QUAN)
			return;
		else if(quantity==DC_READ_QUAN){
			for(i=0;i<quantity;i++)
			{
				*(P_DC2data+i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];
			}	
		}
		else if(quantity==DC_SET_QUAN){
			for(i=0;i<quantity;i++)
			{
				*( (&DC2data.add_485) +i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];//add_485为第一个RW数据
			}
		}	
		rt_sem_release(sem_com_dc);
	}

	else if(Uart0_rev[1] == 0x10)
	{
		i=0;
	}
}

/****************************************************
被调用函数：Uart3_REV_thread
函数功能：处理接收到的PCS数据，放入相关结构体，give信号量
调用函数：无
*****************************************************/
static void  handle_PCS()
{
	uint16_t i,quantity;
	if(Uart0_rev[1] == 3)
	{
		quantity = (Uart0_rev[2]>>1); //寄存器数量
		if(quantity>PCS_READ_QUAN)
			return;
		else if(quantity==PCS_READ_QUAN1){
			for(i=0;i<quantity;i++)
			{
				*(P_PCSdata+i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];
			}
			
			rt_sem_release(sem_com_pcs);
			
			PCSdata.DC_U= CHANGE_H16_L16(PCSdata.DC_U);
			PCSdata.DC_I= CHANGE_H16_L16(PCSdata.DC_I);
			PCSdata.DC_P= CHANGE_H16_L16(PCSdata.DC_P);
			for(i=0;i<6;i++){
				PCSdata.leijiliang[i]= CHANGE_H16_L16(PCSdata.leijiliang[i]);
			}
		}
		else if(quantity==PCS_READ_QUAN2){
			for(i=0;i<quantity;i++)
			{
				*((uint16_t *)&(PCSdata.sys_u[0])+i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];
			}	
			for(i=0;i<3;i++){
				PCSdata.sys_u[i] = CHANGE_H16_L16(PCSdata.sys_u[i]);
				PCSdata.sys_i[i] = CHANGE_H16_L16(PCSdata.sys_i[i]); 
				PCSdata.sys_p[i] = CHANGE_H16_L16(PCSdata.sys_p[i]);
				PCSdata.load_u[i] = CHANGE_H16_L16(PCSdata.load_u[i]);
				PCSdata.load_i[i] = CHANGE_H16_L16(PCSdata.load_i[i]); 
				PCSdata.load_p[i] = CHANGE_H16_L16(PCSdata.load_p[i]);
			}
			
			PCSdata.sys_i[0] = PCSdata.sys_i[0] *(-1);
			PCSdata.sys_i[1] = PCSdata.sys_i[1] *(-1);
			PCSdata.sys_i[2] = PCSdata.sys_i[2] *(-1);
			PCSdata.sys_p[0] = PCSdata.sys_p[0] *(-1);

		}
		else if(quantity==PCS_SET_QUAN){
			for(i=0;i<quantity;i++){
				//这里由于mode是u32，需要先转换成u16地址，否则每次加1都是按照u32来加
				*( ((uint16_t *)&PCSdata.mode) +i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];
			}
			
			PCSdata.mode = CHANGE_H16_L16(PCSdata.mode);
			for(i=0;i<4;i++){
				PCSdata.parameter[i] = CHANGE_H16_L16(PCSdata.parameter[i]);	
			}
			for(i=0;i<10;i++){
				PCSdata.set[i] = CHANGE_H16_L16(PCSdata.set[i]);	
			}	
		}	
		else if(quantity == 4){
			if(PCS_Read_Block == 3){
				for(i=0;i<quantity;i++){
					//这里由于bus_V是u32，需要先转换成u16地址，否则每次加1都是按照u32来加
					*( ((uint16_t *)&PCSdata.bus_V) +i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];
				}
				
				PCSdata.bus_V = CHANGE_H16_L16(PCSdata.bus_V);
				PCSdata.var = CHANGE_H16_L16(PCSdata.var);
			}
			/*else if(PCS_Read_Block == 5){
				for(i=0;i<quantity;i++){
					*( (&PCSdata.bingji_mode) +i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];
				}	
			}
			*/
		}
		/*else if(quantity == 2){
			for(i=0;i<quantity;i++){
				*( (&PCSdata.bote) +i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];
			}	
		}
		*/
		else if(quantity == 1){
			for(i=0;i<quantity;i++){
				*( (&PCSdata.VDC) +i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];
			}	
		}
		
	}

	else if(Uart0_rev[1] == 0x10){
		i=0;
	}
}

void uart0_rev_thread_entry(void* parameter)
{

    static rt_device_t uart0_device = RT_NULL;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初 始 化 配 置 参 数 */
	rt_err_t result;
	rt_uint16_t rx_length;
	rt_uint32_t e;
	uint16_t Uart0_modbus_address;		
	uint16_t Uart0_modbus_quant;
//	static char rx_buffer[RT_SERIAL_RB_BUFSZ + 1];
	
    GPIO_InitPad(56); //485_con7
    GPIO_ConfigDir(56, GPIO_OUTPUT);
    USART0_RX_EN;

	uart0_device = rt_device_find("uart0");

    if (!uart0_device)
    {
        rt_kprintf("find %s failed!\n", "uart0_device");
        //return RT_ERROR;
    }

    /* step2： 修 改 串 口 配 置 参 数 */
    config.baud_rate = BAUD_RATE_9600; //修 改 波 特 率 为 9600
    config.data_bits = DATA_BITS_8; //数 据 位 8
    config.stop_bits = STOP_BITS_1; //停 止 位 1
    config.bufsz = 255; //修 改 缓 冲 区 buff size 为 128
    config.parity = PARITY_NONE; //无 奇 偶 校 验 位
    /* step3： 控 制 串 口 设 备。 通 过 控 制 接 口 传 入 命 令 控 制 字， 与 控 制 参 数 */
    rt_device_control(uart0_device, RT_DEVICE_CTRL_CONFIG, &config);
    /* step4： 打 开 串 口 设 备。 以 中 断 接 收 及 轮 询 发 送 模 式 打 开 串 口 设 备 */
    rt_device_open(uart0_device, RT_DEVICE_FLAG_INT_RX);
    /* 设 置 接 收 回 调 函 数 */
//    rt_device_set_rx_indicate(uart0_device, uart0_input);
	
    while (1)
    {
			/* 从 串 口 读 取 数 据*/
			//rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
		rt_event_recv(&com_event, EVENT_FLAG_PS_UART0,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);	

		if(Uart0_crc_check(Uart0_rev_counter-2,Uart0_rev)==TRUE)
		{
	
			switch(Uart0_rev[0])
			{
				case DC2_485PORT:
					handle_DC_DC2();
				break;
				case PCS_485PORT:
					handle_PCS();
				break;
				default:
				break;
			}
		}
    }
}

void Uart0_MSG_thread_entry(void* parameter)
{
	struct Uart_msg *msg;	
	uint16_t i,len,crc_code;
	//DEBUG_PRINTF("%s\n",(char *)s);
	for(;;)
	{		
		rt_mb_recv(mb_uart0, (rt_uint32_t *)&msg, RT_WAITING_FOREVER);
		
		i=0;	
		if(( msg->msg.command == MODBUS_READ)||( msg->msg.command == MODBUS_WRITE)){
			Uart0_Send_Buf[i++]  = msg->msg.port; //站号
			Uart0_Send_Buf[i++] =  msg->msg.command; //功能码
			Uart0_Send_Buf[i++]  = (msg->msg.add>>8) & 0x00ff; 	//寄存器地址高
			Uart0_Send_Buf[i++]  = msg->msg.add & 0x00ff; 			//寄存器地址低
			Uart0_Send_Buf[i++]  = (msg->msg.len>>8) & 0x00ff;; //寄存器数量高/ 写入数值高
			Uart0_Send_Buf[i++]  = msg->msg.len  & 0x00ff;  //寄存器数量低/ 写入数值低
		}
		else if( msg->msg.command == MODBUS_WRITE10){
			if(msg->msg.port == PCS_485PORT){ //pcs模块需要发送四个字节
				Uart0_Send_Buf[i++]  = msg->msg.port; //站号
				Uart0_Send_Buf[i++] =  msg->msg.command; //功能码
				Uart0_Send_Buf[i++]  = (msg->msg.add>>8) & 0x00ff; 	//寄存器地址高
				Uart0_Send_Buf[i++]  = msg->msg.add & 0x00ff; 			//寄存器地址低
				Uart0_Send_Buf[i++]  = 0;  //寄存器数量高 固定为0；
				Uart0_Send_Buf[i++]  = 2;   //寄存器数量低 固定为1；
				Uart0_Send_Buf[i++]  = 4;  //字节数量 固定为2；
				Uart0_Send_Buf[i++]  = (msg->msg.len>>24) & 0x00ff; //写入数值高
				Uart0_Send_Buf[i++]  = (msg->msg.len>>16) & 0x00ff; 
				Uart0_Send_Buf[i++]  = (msg->msg.len>>8) & 0x00ff; 
				Uart0_Send_Buf[i++]  = msg->msg.len  & 0x00ff;  //写入数值低
			}
			else{ //dc需要发送两个字节
				Uart0_Send_Buf[i++]  = msg->msg.port; //站号
				Uart0_Send_Buf[i++] =  msg->msg.command; //功能码
				Uart0_Send_Buf[i++]  = (msg->msg.add>>8) & 0x00ff; 	//寄存器地址高
				Uart0_Send_Buf[i++]  = msg->msg.add & 0x00ff; 			//寄存器地址低
				Uart0_Send_Buf[i++]  = 0;  //寄存器数量高 固定为0；
				Uart0_Send_Buf[i++]  = 1;   //寄存器数量低 固定为1；
				Uart0_Send_Buf[i++]  = 2;  //字节数量 固定为2；
				Uart0_Send_Buf[i++]  = (msg->msg.len>>8) & 0x00ff;; //寄存器数量高/ 写入数值高
				Uart0_Send_Buf[i++]  = msg->msg.len  & 0x00ff;  //寄存器数量低/ 写入数值低
			}
		}
		crc_code = Uart0_Crc_Make(i,Uart0_Send_Buf);
		Uart0_Send_Buf[i++] = (crc_code>>8) & 0x0FF;
		Uart0_Send_Buf[i++] = crc_code & 0x0FF;
		len = i; 	

		Uart0_Send(Uart0_Send_Buf,len);		
		rt_free(msg); //释放内存,如果是空指针，则不会执行任何操作	
/*
		延时,等待数据上传 ，由于队列中可能存在多组数据等待上传，而且这里的延时与Uart3_read_thread
		任务中的等待可认为是同步进行的，不是相加关系，所以设置为400ms
		而且当前采用任务方式，所以延时时间就是发送间隔		
*/
		rt_thread_delay(30);
	}
}

uint8_t Uart0_Post(uint8_t port,uint8_t command,uint16_t add,uint32_t data)
{
	static struct Uart_msg *Uart0_msg;	
	Uart0_msg = (struct Uart_msg *)rt_malloc(sizeof(struct Uart_msg));
	if(Uart0_msg ==NULL)//分配失败
	{
	//	DEBUG_PRINTF("uart3_msg 分配内存失败\n");
	//	return ERRMEM;
	}
	
    Uart0_msg->msg.port = port;; //站号
	Uart0_msg->msg.command = command; //功能码
	Uart0_msg->msg.add  = add; 	//寄存器地址
	Uart0_msg->msg.len = data; //寄存器数量/ 写入数值
	 
//	UBaseType_t temp;	
	//temp = uxQueueSpacesAvailable( xQueue_Uart3 );
//	DEBUG_PRINTF("xQueue_Uart3 : %d\n",temp);
	
	if(command == MODBUS_READ){
		rt_mb_send(mb_uart0, (rt_uint32_t)(Uart0_msg));
	}
	else if((command == MODBUS_WRITE)||(command == MODBUS_WRITE10)){ //可考虑换成队列，可优先发送
		rt_mb_send(mb_uart0, (rt_uint32_t)(Uart0_msg));
//		DEBUG_PRINTF("xQueue_Uart3 write\n");
	}
//	return ERROK;
}

/********************************************************************
被调用函数：调度器
函数功能：循环发送uart3串口的请求指令，timeout 200ms。delay 2S
				通过信号量判断是否超时，当数据被正确接收后对信号量执行give操作
调用函数：Uart3_Read_module
***********************************************************************/
void uart0_read_thread_entry(void* parameter)
{        
	rt_err_t xReturn = RT_EOK;
//	DEBUG_PRINTF("%s\n",(char *)s);
	for(;;)
	{		

		Uart0_Post(DC2_485PORT,MODBUS_READ,DC_READ_START_ADD, DC_READ_QUAN);
		xReturn = rt_sem_take(sem_com_dc, 20);// 获取信号量 超时等待
		if(xReturn == RT_EOK)
			LEUER_Data.EMS_Data.DC2_Com_State = PASS;
		else
			LEUER_Data.EMS_Data.DC2_Com_State = FAIL;
		rt_thread_delay(20);
		//读取DC2的 可保存参数
		Uart0_Post(DC2_485PORT,MODBUS_READ,DC_SET_START_ADD, DC_SET_QUAN);
		rt_sem_take(sem_com_dc, 20);// 获取信号量 超时等待
		rt_thread_delay(20);
		

		PCS_Read_Block = 1;
		Uart0_Post(PCS_485PORT,MODBUS_READ,PCS_READ_START_ADD1, PCS_READ_QUAN1);
		xReturn = rt_sem_take(sem_com_pcs, 20);// 获取信号量 超时等待
		rt_thread_delay(20);
		if(xReturn == RT_EOK)
			LEUER_Data.EMS_Data.PCS_Com_State = PASS;
		else
			LEUER_Data.EMS_Data.PCS_Com_State = FAIL;
		
		PCS_Read_Block = 6;
		Uart0_Post(PCS_485PORT,MODBUS_READ,PCS_READ_START_ADD2, PCS_READ_QUAN2);
		rt_sem_take(sem_com_pcs, 20);// 获取信号量 超时等待
		rt_thread_delay(20);
		//读取PCS的 可保存参数
		PCS_Read_Block = 2;
		Uart0_Post(PCS_485PORT,MODBUS_READ,PCS_SET_START_ADD, PCS_SET_QUAN);
		rt_sem_take(sem_com_pcs, 20);// 获取信号量 超时等待	
		rt_thread_delay(20);
		
		PCS_Read_Block = 3;
		Uart0_Post(PCS_485PORT,MODBUS_READ,0x2800, 4);
		rt_sem_take(sem_com_pcs, 20);// 获取信号量 超时等待	
		rt_thread_delay(20);
				
		PCS_Read_Block = 4;
		Uart0_Post(PCS_485PORT,MODBUS_READ,PCS_VDC_READ_ADD, 1);
		rt_sem_take(sem_com_pcs, 20);// 获取信号量 超时等待
		rt_thread_delay(20);
		
		/*			
		Uart3_Post(PCS_485PORT,MODBUS_READ,0x2900, 2);
		PCS_Read_Block = 4;	
		vTaskDelay( UART3_TIMEOUT_MS / portTICK_RATE_MS );
		
		Uart3_Post(PCS_485PORT,MODBUS_READ,0x2A03, 4);
		PCS_Read_Block = 5;
		vTaskDelay( UART3_TIMEOUT_MS / portTICK_RATE_MS );
		
		PCS_Read_Block = 0;
		*/	


		// 延时钟
		 rt_thread_delay(100);
	}

}

/* 定 时 器 1 超 时 函 数 */
void timeout_uart0(void *parameter)
{
//    rt_kprintf("periodic timer is timeout \n");
    static uint16_t com0_rx_sta = 0, last_com0_rx_sta = 0, last_com0_rxlen = 0; //com状态

    //接收事件，阻塞，中断在开始接收后 发送事件
//	rt_event_recv(&com_event,EVENT_FLAG_RV_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);

	if(com0_rxlen > last_com0_rxlen )
	{
		//正在接收状态
		com0_rx_sta = 1;
	}
	else if(com0_rxlen == last_com0_rxlen )
	{
		//脱离接收状态
		com0_rx_sta = 0;
	}
	else
	{
		//接收完毕 ，Uart7_rev_counter被置0,Uart7_rev_counter应在解析函数中置零
	}

	if((com0_rx_sta == 0) && (last_com0_rx_sta == 1))
	{
		Uart0_rev_counter = com0_rxlen;
		rt_event_send(&com_event, EVENT_FLAG_PS_UART0);
		com0_rxlen=0;
		//发送信号量
	}

	last_com0_rx_sta = com0_rx_sta;
	last_com0_rxlen = com0_rxlen;
//	rt_thread_delay(10);
}

/*-----------------------------------------------------------------------------
  Function:		UART0_IRQHandler
  input:    None
  Returns:  None
  Description:  UART0 interrupt handler
 *-----------------------------------------------------------------------------*/
void UART0_IRQHandler(void)
{
    UINT32 intrStatus, intrEnSts;
    UINT8 rxData;
    SGCC_UART_TypeDef *UARTx;
    UARTx = (SGCC_UART_TypeDef *)SGCC_UART0_BASE;
    intrStatus = UARTx->INTSTA;
    intrEnSts = UARTx->CTRL;

    if (intrStatus & UART_INTSTATUS_stop_error)
    {
        UART_ClearIntr(UARTx, UART_INTSTATUS_stop_error); //clr int bit
        //UART_Printf(printx, "Uart0 intr,INTSTA reg: bit%d UART_INTSTATUS_stop_error\n" ,UART_INTSTATUS_stop_errorB);
    }

    // 修改潼关抄表失败问题 2020.11.26
    if(intrStatus & 0x04)
    {
        //dis_string(NULL, 0, "uart0 stop error");
        UARTx->INTSTA |= 0x04;
    }

    if (intrStatus & UART_INTSTATUS_parity_error)
    {
        UART_ClearIntr(UARTx, UART_INTSTATUS_parity_error); //clr int bit
        //UART_Printf(printx, "Uart0 intr,INTSTA reg: bit%d UART_INTSTATUS_parity_error\n" ,UART_INTSTATUS_parity_errorB);
    }

    if (intrStatus & UART_INTSTATUS_RXINT)
    {
        //clear interrupt
        UART_ClearIntr(UARTx, UART_INTSTATUS_RXINT);

        if ((UARTx->STATUS & UART_STATUS_RXEMP) == 0)
        {
            rxData = UARTx->RXDATA;
			
			if (com0_rxlen < MAX_REV_QUANT)
                Uart0_rev[com0_rxlen++] = rxData;
			
			if(com0_rxlen == 1) //开始接收
				rt_event_send(&com_event, EVENT_FLAG_RV_UART0);
        }
    }

    if ((intrStatus & UART_INTSTATUS_TXINT) && (intrEnSts & UART_CTRL_TXINTEN))
    {
        //clear interrupt
        UART_ClearIntr(UARTx, UART_INTSTATUS_TXINT);
        TxCount++;

        if (TxCount == 10)
            UART_EnableIntr(UARTx, UART_CTRL_RXINTEN | UART_CTRL_TXINTDIS);
    }
}


