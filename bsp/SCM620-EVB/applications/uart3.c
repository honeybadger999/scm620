/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_uart.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define METER_485PORT 2//暂定
#define METER_Read_Start_ADD 0x0000
#define METER_Read_Quan 64
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
uint8_t	Uart3_rev[MAX_REV_QUANT];	
uint16_t 	Uart3_rev_counter,com3_rxlen;
uint8_t 	Uart3_Send_Buf[MAX_SEND_QUANT];
static uint8_t TxCount = 0;

struct Hour_Meter hour_meter;
struct PV_GLASS PV_Glass;
uint16_t *P_hourmeter = ( uint16_t *)&hour_meter;
uint16_t *P_PV_Glass = ( uint16_t *)&PV_Glass;
uint16_t Read_Block;
/* Private function prototypes -----------------------------------------------*/
static void Uart3_Delay_nus(uint16_t time_us) ;
static uint16_t Uart3_crc_check( uint16_t data_len, uint8_t *data );
static uint16_t Uart3_Crc_Make(uint16_t size, uint8_t *buff);
/* Private functions ---------------------------------------------------------*/


void uart3_rev_thread_entry(void* parameter)
{

    static rt_device_t uart3_device = RT_NULL;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* 初 始 化 配 置 参 数 */
	rt_err_t result;
	rt_uint16_t i;
	rt_uint32_t e;

//	static char rx_buffer[RT_SERIAL_RB_BUFSZ + 1];
	
    GPIO_InitPad(57); //485_con3
    GPIO_ConfigDir(57, GPIO_OUTPUT);
    USART3_RX_EN;

	uart3_device = rt_device_find("uart3");

    if (!uart3_device)
    {
        rt_kprintf("find %s failed!\n", "uart3_device");
        //return RT_ERROR;
    }

    /* step2： 修 改 串 口 配 置 参 数 */
    config.baud_rate = BAUD_RATE_9600; //修 改 波 特 率 为 9600
    config.data_bits = DATA_BITS_8; //数 据 位 8
    config.stop_bits = STOP_BITS_1; //停 止 位 1
    config.bufsz = 255; //修 改 缓 冲 区 buff size 为 128
    config.parity = PARITY_NONE; //无 奇 偶 校 验 位
    /* step3： 控 制 串 口 设 备。 通 过 控 制 接 口 传 入 命 令 控 制 字， 与 控 制 参 数 */
    rt_device_control(uart3_device, RT_DEVICE_CTRL_CONFIG, &config);
    /* step4： 打 开 串 口 设 备。 以 中 断 接 收 及 轮 询 发 送 模 式 打 开 串 口 设 备 */
    rt_device_open(uart3_device, RT_DEVICE_FLAG_INT_RX);
    /* 设 置 接 收 回 调 函 数 */
//    rt_device_set_rx_indicate(uart0_device, uart0_input);
	
    while (1)
    {
			/* 从 串 口 读 取 数 据*/
			//rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
		rt_event_recv(&com_event, EVENT_FLAG_PS_UART3,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);	

		if(Uart3_crc_check(Uart3_rev_counter-2,Uart3_rev)!=TRUE) return;			
		if(Uart3_rev[2]>128) return;
		
		if(Uart3_rev[0] == METER_485PORT){

			if(Read_Block ==1){
				for(i=0;i<(Uart3_rev[2]>>1);i++)
				{
					*(P_hourmeter+i) = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i];
				}
				hour_meter.Frequency= CHANGE_H16_L16(hour_meter.Frequency);
				hour_meter.Ua = CHANGE_H16_L16(hour_meter.Ua); //相电压
				hour_meter.Ub = CHANGE_H16_L16(hour_meter.Ub); 
				hour_meter.Uc = CHANGE_H16_L16(hour_meter.Uc);
				hour_meter.Uab = CHANGE_H16_L16(hour_meter.Uab); //线电压
				hour_meter.Ubc = CHANGE_H16_L16(hour_meter.Ubc); 
				hour_meter.Uca = CHANGE_H16_L16(hour_meter.Uca); 
				hour_meter.Ia = CHANGE_H16_L16(hour_meter.Ia); //相电流
				hour_meter.Ib = CHANGE_H16_L16(hour_meter.Ib); 
				hour_meter.Ic = CHANGE_H16_L16(hour_meter.Ic);	
				hour_meter.Pa = CHANGE_H16_L16(hour_meter.Pa); //有功功率
				hour_meter.Pb = CHANGE_H16_L16(hour_meter.Pb); 
				hour_meter.Pc = CHANGE_H16_L16(hour_meter.Pc);
				hour_meter.Ps = CHANGE_H16_L16(hour_meter.Ps); 
				hour_meter.Qa = CHANGE_H16_L16(hour_meter.Qa); //无功功率
				hour_meter.Qb = CHANGE_H16_L16(hour_meter.Qb); 
				hour_meter.Qc = CHANGE_H16_L16(hour_meter.Qc);
				hour_meter.Qs = CHANGE_H16_L16(hour_meter.Qs);
				hour_meter.VAa = CHANGE_H16_L16(hour_meter.VAa); //视在功率
				hour_meter.VAb = CHANGE_H16_L16(hour_meter.VAb); 
				hour_meter.VAc = CHANGE_H16_L16(hour_meter.VAc);
				hour_meter.VAs = CHANGE_H16_L16(hour_meter.VAs);
				hour_meter.PFa = CHANGE_H16_L16(hour_meter.PFa); //功率因数
				hour_meter.PFb = CHANGE_H16_L16(hour_meter.PFb); 
				hour_meter.PFc = CHANGE_H16_L16(hour_meter.PFc);
				hour_meter.PFs = CHANGE_H16_L16(hour_meter.PFs);
				hour_meter.Frequency = CHANGE_H16_L16(hour_meter.Frequency);
				hour_meter.KWH[0] = CHANGE_H16_L16(hour_meter.KWH[0]); //电能
				hour_meter.KWH[1] = CHANGE_H16_L16(hour_meter.KWH[1]);
				hour_meter.KWH[2] = CHANGE_H16_L16(hour_meter.KWH[2]);
				hour_meter.KWH[3] = CHANGE_H16_L16(hour_meter.KWH[3]);
				hour_meter.KWH[4] = CHANGE_H16_L16(hour_meter.KWH[4]);
				hour_meter.KWH[5] = CHANGE_H16_L16(hour_meter.KWH[5]);
			}
			else if(Read_Block ==2){
				for(i=0;i<(Uart3_rev[2]>>1);i++){	
					hour_meter.Harm_U[0] = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i];
				}
			}
			else if(Read_Block ==3){
				for(i=0;i<(Uart3_rev[2]>>1);i++){
					hour_meter.Harm_U[1] = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i];
				}
			}
			else if(Read_Block ==4){
				for(i=0;i<(Uart3_rev[2]>>1);i++){
					hour_meter.Harm_U[2] = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i];
				}
			}
			
			else if(Read_Block ==5){
				for(i=0;i<(Uart3_rev[2]>>1);i++){
					hour_meter.Harm_I[0] = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i];
				}
			}
			else if(Read_Block ==6){
				for(i=0;i<(Uart3_rev[2]>>1);i++){
					hour_meter.Harm_I[1] = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i];
				}
			}
			else if(Read_Block ==7){
				for(i=0;i<(Uart3_rev[2]>>1);i++){
					hour_meter.Harm_I[2] = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i];
				}
			}	
		}
		
		//玻璃光伏
		else if(Uart3_rev[0] == 1){
			for(i=0;i<(Uart3_rev[2]>>1);i++)
			{
				*(P_PV_Glass+i) = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i]; //玻璃光伏的数据可能要交换高低16位
			}
			PV_Glass.P= CHANGE_H16_L16(PV_Glass.P);
			PV_Glass.P_DC= CHANGE_H16_L16(PV_Glass.P_DC);
			PV_Glass.Power_total= CHANGE_H16_L16(PV_Glass.Power_total);
			PV_Glass.Power_month= CHANGE_H16_L16(PV_Glass.Power_month);
			PV_Glass.Power_lastmonth= CHANGE_H16_L16(PV_Glass.Power_lastmonth);
		}
    }
}
/**************************************************************************************************
*	函数名称：Send_Uart3	
*	参数：*buff	发送数据内存  len发送数据长度
*	
*	没有设置DMA发送完成的中断，而是采用查询方式，同时去掉调度器挂起恢复操作，在等待时期可以切换其它
*   任务，目的是提高执行效率
*
*    开启以太网后 Uart3_SendDMA无法使用， 需要增加sct文件，确保Uart3_Send_Buf在IRAM1中
*    
**************************************************************************************************/
static void Uart3_Send(const uint8_t *buff,uint8_t len)
{
	uint8_t i;
	//	vTaskSuspendAll();

	USART3_TX_EN;
	Uart3_Delay_nus(2000);

	for (i = 0; i < len; i++)
	{
		UART_PutChar(SGCC_UART3_P, buff[i]);
	}

	Uart3_Delay_nus(5000);  //经过测试  时间设置太短会影响发送  5ms 发送50个字节没问题
	USART3_RX_EN;
//	xTaskResumeAll();//恢复调度器
}

void Uart3_MSG_send(uint8_t port,uint8_t command,uint16_t add, uint16_t quant)
{
	uint16_t i,crc_code,len;

	i=0;		
	Uart3_Send_Buf[i++]  = port; //站号
	Uart3_Send_Buf[i++] =  command; //功能码
	Uart3_Send_Buf[i++]  = (add>>8) & 0x00ff; 	//
	Uart3_Send_Buf[i++]  = add & 0x00ff; 			//寄存器地址低
	Uart3_Send_Buf[i++]  = (quant>>8) & 0x00ff;; //寄存器数量高/ 写入数值高
	Uart3_Send_Buf[i++]  = quant  & 0x00ff;  		//寄存器数量低/ 写入数值低
	 
	crc_code = Uart3_Crc_Make(i,Uart3_Send_Buf);
	Uart3_Send_Buf[i++] = (crc_code>>8) & 0x0FF;
	Uart3_Send_Buf[i++] = crc_code & 0x0FF;

	len = i; 			
	Uart3_Send(Uart3_Send_Buf,len);	
}

/*******************************************************************************************************
** 函数名称 ：Crc_Make()
** 函数功能 ：计算CRC校验码。
** 入口参数1：buff：待计算CRC校验代码的指针；
** 入口参数2：size：从buff指针的首地址开始的字节数
** 出口参数 ：无（返回值为指针crc）	
*******************************************************************************************************
*/
static uint16_t Uart3_Crc_Make(uint16_t size, uint8_t *buff)
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
	CrcCodeTmp = (crc[0]<<8)+crc[1];
	return CrcCodeTmp;
}

/***********************************************************/ 
static uint16_t	Uart3_crc_check( uint16_t data_len, uint8_t *data )
{
	uint16_t	Crc_Code;
 	uint16_t	Crc_LowCode, Crc_HighCode;
	Crc_Code = Uart3_Crc_Make( data_len, data);	//对接收数据计算CRC校验码
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
static void Uart3_Delay_nus(uint16_t time_us)  
{
	uint16_t k1,k2;
	for(k1=time_us;k1>0;k1--)  
	{
		for(k2=33;k2>0;k2--)
		{
		}
	}
}

void uart3_read_thread_entry(void *parameter)
{	
	rt_err_t xReturn = RT_EOK;
	//DEBUG_PRINTF("%s\n",(char *)s);
	
	for(;;)
	{				
		Read_Block=1;
		Uart3_MSG_send(METER_485PORT,MODBUS_READ,METER_Read_Start_ADD, METER_Read_Quan); //发送开出数据	
		xReturn = rt_sem_take(sem_com_meter, 20);// 获取信号量 超时等待
		if(xReturn == RT_EOK)
			LEUER_Data.EMS_Data.BAT_Com_State = PASS;
		else
			LEUER_Data.EMS_Data.BAT_Com_State = FAIL;
		rt_thread_delay(2000/ portTICK_RATE_MS);
		
		Read_Block=2;
		Uart3_MSG_send(METER_485PORT,MODBUS_READ,0x00D8, 2);
		rt_thread_delay(2000/ portTICK_RATE_MS);
		Read_Block=3;
		Uart3_MSG_send(METER_485PORT,MODBUS_READ,0x013C, 2);
		rt_thread_delay(2000/ portTICK_RATE_MS);
		Read_Block=4;
		Uart3_MSG_send(METER_485PORT,MODBUS_READ,0x01A0, 2);
		rt_thread_delay(2000/ portTICK_RATE_MS);
		
		
		Read_Block=5;
		Uart3_MSG_send(METER_485PORT,MODBUS_READ,0x0204, 2);
		rt_thread_delay(2000/ portTICK_RATE_MS);
		Read_Block=6;
		Uart3_MSG_send(METER_485PORT,MODBUS_READ,0x0268, 2);
		rt_thread_delay(2000/ portTICK_RATE_MS);
		Read_Block=7;
		Uart3_MSG_send(METER_485PORT,MODBUS_READ,0x02C8, 2);
		rt_thread_delay(2000/ portTICK_RATE_MS);
		
		//读取玻璃光伏 04功能码 3004地址读取11个数据
		Uart3_MSG_send(1,4,3004, 11);
		
		xReturn = rt_sem_take(sem_com_meter, 20);// 获取信号量 超时等待
		if(xReturn == RT_EOK)
			LEUER_Data.EMS_Data.BAT_Com_State = PASS;
		else
			LEUER_Data.EMS_Data.BAT_Com_State = FAIL;
		rt_thread_delay(2000/ portTICK_RATE_MS);
	}
}

/* 定 时 器 1 超 时 函 数 */
void timeout_uart3(void *parameter)
{
//    rt_kprintf("periodic timer is timeout \n");
    static uint16_t com3_rx_sta = 0, last_com3_rx_sta = 0, last_com3_rxlen = 0; //com状态

    //接收事件，阻塞，中断在开始接收后 发送事件
//	rt_event_recv(&com_event,EVENT_FLAG_RV_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);

	if(com3_rxlen > last_com3_rxlen )
	{
		//正在接收状态
		com3_rx_sta = 1;
	}
	else if(com3_rxlen == last_com3_rxlen )
	{
		//脱离接收状态
		com3_rx_sta = 0;
	}
	else
	{
		//接收完毕 ，Uart7_rev_counter被置0,Uart7_rev_counter应在解析函数中置零
	}

	if((com3_rx_sta == 0) && (last_com3_rx_sta == 1))
	{
		Uart3_rev_counter = com3_rxlen;
		rt_event_send(&com_event, EVENT_FLAG_PS_UART3);
		com3_rxlen=0;
		//发送信号量
	}

	last_com3_rx_sta = com3_rx_sta;
	last_com3_rxlen = com3_rxlen;
//	rt_thread_delay(10);
}

/****************************************************************
  * @brief  uart3中断接收函数
  * @param  None
  * @retval None
  ****************************************************************/

void UART3_IRQHandler(void)
{
    UINT32 intrStatus, intrEnSts;
    UINT8 rxData;
    SGCC_UART_TypeDef *UARTx;
    UARTx = (SGCC_UART_TypeDef *)SGCC_UART3_BASE;
    intrStatus = UARTx->INTSTA;
    intrEnSts = UARTx->CTRL;

    if (intrStatus & UART_INTSTATUS_stop_error)
    {
        UART_ClearIntr(UARTx, UART_INTSTATUS_stop_error); //clr int bit
    }

    // 修改潼关抄表失败问题 2020.11.26
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

			if (com3_rxlen < MAX_REV_QUANT)
                Uart3_rev[com3_rxlen++] = rxData;
			
			if(com3_rxlen == 1) //开始接收
				rt_event_send(&com_event, EVENT_FLAG_RV_UART3);
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
