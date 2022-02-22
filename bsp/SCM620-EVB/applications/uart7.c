#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>

#define UART_LEN_MAX 200

UINT8 TxCount = 0;
rt_uint8_t uart7_stack[256];
struct rt_thread uart7_thread;
UINT8 Uart7_rev[UART_LEN_MAX];
UINT16 Uart7_rev_counter,com7_rxlen;

uint8_t  Uart7_Send_Buf[256];
struct CRC_TABLE crc_tab;
/* �� �� �� �� �� Ϣ �� ��*/
struct rx_msg
{
	rt_device_t dev;
	rt_size_t size;
};
/* �� Ϣ �� �� �� �� �� */
static struct rt_messagequeue rx_mq;
/* �� �� �� �� �� �� �� �� */
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
        //�� Ϣ �� �� �� 
        rt_kprintf("message queue full�� \n");
    }

    return result;
}
*/


/*******************************************************************************************************
** �������� ��Crc_Make()
** �������� ������CRCУ���롣
** ��ڲ���1��buff��������CRCУ������ָ�룻
** ��ڲ���2��size����buffָ����׵�ַ��ʼ���ֽ���
** ���ڲ��� ���ޣ�����ֵΪָ��crc��	
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
		
	if(car>256)   //20190916 ����������С���ʻ����������ֱ�ӷ���
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
	Crc_Code = Uart7_Crc_Make( data_len, data);	//�Խ������ݼ���CRCУ����
 	Crc_HighCode = data[ data_len ];
 	Crc_LowCode = data[ data_len + 1 ]; 	
 	if( Crc_Code == ( Crc_HighCode * 0x100 + Crc_LowCode ) )		//У������ȷ
		return	1;
 		else
 			return	0; 	
}


/***************************************
	us��ʱ����
	��ȷ����168m��Ƶ time_us=1 ��ʱ1us
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
*	�������ƣ�Send_Uart1	
*	������*buff	���������ڴ�  len�������ݳ���
*	
*	û������DMA������ɵ��жϣ����ǲ��ò�ѯ��ʽ��ͬʱȥ������������ָ��������ڵȴ�ʱ�ڿ����л�����
*   ����Ŀ�������ִ��Ч��
**************************************************************************************************/
static void Uart7_Send(const uint8_t *buff,uint8_t len)
{
	//	vTaskSuspendAll();
//    usart_write(7, Uart7_Send_Buf, modbus_len);
	uint16_t i;
	GPIO_WrDataOut(58, GPIO_LOW);
	Uart7_Delay_nus(2000);

	for (i = 0; i < len; i++)
	{
		UART_PutChar(SGCC_UART7_P, buff[i]);
	}

	Uart7_Delay_nus(5000);  //��������  ʱ������̫�̻�Ӱ�췢��  5ms ����50���ֽ�û����
	GPIO_WrDataOut(58, GPIO_HIGH);
//	xTaskResumeAll();//�ָ�������
}

/*
*******************************************************************************************************
** �������� ��handle_modbus_03h()
** �������� ������modbus��03H���ܡ�
**  �����ĵ�ַΪʵ�ʵ�ַ���ڴ˺�������Ҫ��ȥ1  Һ������Ҫ�ڵ���ַ�����ϼ�1
*******************************************************************************************************
*/
static void handle_modbus_03h(uint16_t modbus_add,uint16_t modbus_dataNum)
{
	uint16_t i;
	uint16_t modbus_len,modbus_crc_code;

	Uart7_Send_Buf[0] = Uart7_rev[0];
	Uart7_Send_Buf[1] = 0x03;
	Uart7_Send_Buf[2] = modbus_dataNum<<1;

	for(i=0;i<modbus_dataNum;i++)				
	{
		if((modbus_add>=100)&&(modbus_add<=150))//EMS
		{
			Uart7_Send_Buf[3+i*2] = i;//(*(P_EMS-100+modbus_add+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = i;//*(P_EMS-100+modbus_add+i) & 0x0FF;
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

void uart7_thread_entry(void* parameter)
{

    static rt_device_t uart7_device = RT_NULL;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* �� ʼ �� �� �� �� �� */
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
        UART_Init(SGCC_UART7_P, 9600, PARITY_NONE, UART_CTRL_STOP_1BIT); //עӢpclk SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4 Ϊ18.75M
        UART_EnableIntr(SGCC_UART7_P, UART_CTRL_RXINTEN | UART_CTRL_TXINTDIS);
        NVIC_EnableIRQ(UART7_IRQn);
        NVIC_SetPriority(UART7_IRQn, 7); //0-7
    */

	/* �� ʼ �� �� Ϣ �� �� */
//	rt_mq_init(&rx_mq, "rx_mq",
//	msg_pool, /* �� �� �� Ϣ �� �� �� �� */
//	sizeof(struct rx_msg), /* һ �� �� Ϣ �� �� �� �� �� */
//	sizeof(msg_pool), /* �� �� �� Ϣ �� �� �� �� �� С */
//	RT_IPC_FLAG_FIFO); /* �� �� �� �� �� �� �� �� ���� �� �� �� �� �� �� �� �� �� ���� �� �� Ϣ */

	uart7_device = rt_device_find("uart7");

    if (!uart7_device)
    {
        rt_kprintf("find %s failed!\n", "uart7_device");
        //return RT_ERROR;
    }

    /* step2�� �� �� �� �� �� �� �� �� */
    config.baud_rate = BAUD_RATE_9600; //�� �� �� �� �� Ϊ 9600
    config.data_bits = DATA_BITS_8; //�� �� λ 8
    config.stop_bits = STOP_BITS_1; //ͣ ֹ λ 1
    config.bufsz = 255; //�� �� �� �� �� buff size Ϊ 128
    config.parity = PARITY_NONE; //�� �� ż У �� λ
    /* step3�� �� �� �� �� �� ���� ͨ �� �� �� �� �� �� �� �� �� �� �� �֣� �� �� �� �� �� */
    rt_device_control(uart7_device, RT_DEVICE_CTRL_CONFIG, &config);
    /* step4�� �� �� �� �� �� ���� �� �� �� �� �� �� �� ѯ �� �� ģ ʽ �� �� �� �� �� �� */
    rt_device_open(uart7_device, RT_DEVICE_FLAG_INT_RX);
//	rt_device_open(uart7_device, RT_DEVICE_FLAG_DMA_RX);
    /* �� �� �� �� �� �� �� �� */
//    rt_device_set_rx_indicate(uart7_device, uart7_input);
	
    while (1)
    {
		//rt_memset(&msg, 0, sizeof(msg));
		/* �� �� Ϣ �� �� �� �� ȡ �� Ϣ*/

			/* �� �� �� �� ȡ �� ��*/
			//rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
		rt_event_recv(&com_event, EVENT_FLAG_PS_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);	
		if((Uart7_rev[0]==20) || (Uart7_rev[0]==238))   //ͨ��վ�š�
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
						//20200420 ֮ǰ����Ƕ�׹��࣬���в��:
						////1 д��ṹ�岢���ݵ�ַ�ж�ģ�� ��Ӧ������
					//	handle_modbus_06h(Uart7_modbus_address,Uart7_modbus_quant);
						//2 �ж�վ�Ų�д�룬ͬʱ��module��ֵΪ����վ�ţ��������жϣ�
						//judge_portnumber(Uart1_modbus_address,&Module); 
						//3 //����ַת���ɸ���ģ���ͨѶ��ַ��ͨ��Uart3_Postд�뵽���Ͷ���
						//send_uart3(Module,Uart1_modbus_address,Uart1_modbus_quant);
					break;
					case 0x01:
					//	handle_modbus_01h(Uart7_modbus_address,Uart7_modbus_quant);
					break;
					case 0x05:
					//	handle_modbus_05h(Uart7_modbus_address,Uart7_modbus_quant);
					break;
					case 0x10:
					//	handle_modbus_10h(Uart7_modbus_address,Uart7_modbus_quant);
					break;
					default:
					break;
				}
			}
		}
		/* �� ӡ �� �� */
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

            if (com7_rxlen < UART_LEN_MAX)
                Uart7_rev[com7_rxlen++] = rxData;
			
			if(com7_rxlen == 1) //��ʼ����
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

#define	POLY_CRC16	0xA001
void Crc_Init( void )
{
	uint16_t	mask, bit, crc, mem;
	for( mask = 0; mask < 0x100; mask++ ) 
	{
		crc = mask;
		for( bit = 0; bit < 8; bit++ ) 
		{
			mem = crc & 0x0001 ;
			crc /= 2;
			if( mem != 0 ) 
				crc ^= POLY_CRC16 ;
		}
		crc_tab.TABLE2[ mask ] = crc & 0xff;
		crc_tab.TABLE1[ mask ] = crc >> 8;
	}
}
