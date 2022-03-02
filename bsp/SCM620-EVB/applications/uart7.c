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
	USART7_TX_EN;
	Uart7_Delay_nus(2000);

	for (i = 0; i < len; i++)
	{
		UART_PutChar(SGCC_UART7_P, buff[i]);
	}

	Uart7_Delay_nus(5000);  //��������  ʱ������̫�̻�Ӱ�췢��  5ms ����50���ֽ�û����
	USART7_RX_EN;
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
		else if((modbus_add>=170)&&(modbus_add<=183))  //��̫������ mac ip
		{
			Uart7_Send_Buf[3+i*2] =(*(P_ethernet+modbus_add-170+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = *(P_ethernet+modbus_add-170+i) & 0x0FF;
		}
		else if((modbus_add>=2200)&&(modbus_add<=2282))    // DC2 �������ݶ�ȡ 
		{
			Uart7_Send_Buf[3+i*2] = (*(P_DC2data+modbus_add-2200+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = *(P_DC2data+modbus_add-2200+i) & 0x0FF;	
		}
		else if((modbus_add>=3000)&&(modbus_add<=3149))    // PCS�������ݶ�ȡ
		{
			Uart7_Send_Buf[3+i*2] = (*(P_PCSdata+modbus_add-3000+i)>>8) & 0x0FF;
			Uart7_Send_Buf[4+i*2] = *(P_PCSdata+modbus_add-3000+i) & 0x0FF;	
		}
		else if((modbus_add>=5000)&&(modbus_add<=5030))    //ĸ��DC-DC 
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
		send_uart3(BROADCAST_SITE_NUMBER,0,data,NULL); // ���Ӵ�����д�빫��վ�ţ�ֱ��ͨ��uart3ת���������������д���ַ641
	}
		//ems����
	else if((modbus_add>=100)&&(modbus_add<=110)) {  //ems
		*(P_EMS+modbus_add-100) = data;
		Store_par_DataFlash(P_EMS,FLASHQUAN_INFO,FLASHSECTOR_INFO,FLASHADD_INFO);	 //�洢����
	}
	else if((modbus_add>=151)&&(modbus_add<=153)) {  //���ܳ�ֵ���ã������棬��rtc�б���
		PVAC_Power[modbus_add-151] = (float)data;
	}
	else if((modbus_add>=160)&&(modbus_add<=164))    //time
	{
		time_uart[modbus_add-160] = data;
	}
	else if(modbus_add==166) // timeд��
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
	else if((modbus_add>=170)&&(modbus_add<=183))  //��̫������ mac ip
	{
		*(P_ethernet+modbus_add-170) = data;
	}
	else if(modbus_add==184) // ��̫��д��
	{
		if(data== 1111){	
			Store_par_DataFlash(P_ethernet,FLASHQUAN_ETH,FLASHSECTOR_ETH,FLASHADD_ETH);//д��flash
		}
	}
	
	//DC2 2206-2217Ϊ���ɱ������ 2270-2282Ϊ�ɱ������
	else if(((modbus_add>=2270)&&(modbus_add<=2282))||((modbus_add>=2206)&&(modbus_add<=2217))) { 
		*(P_DC2data+modbus_add-2200) = data;  //д�뵽�ṹ���Թ���ʾ
		send_uart3(MODULE_DC2,modbus_add,data,NULL);
	}
	else if((modbus_add>=3100)&&(modbus_add<=3143)) {  // pcs
		*(P_PCSdata+modbus_add-3100+100) = data;
		send_uart3(MODULE_PCS,modbus_add,data,MODBUS_WRITE);
	}
	else if((modbus_add>=5000)&&(modbus_add<=5010))    //ĸ��DC-DC 
	{
		*(P_MD950+modbus_add-5000) = data;
		send_can2_MD950( modbus_add, data);	
	}
		
	else
		return;

	for(i=0;i<8;i++) //Ӧ������
	{
		Uart7_Send_Buf[i]= Uart7_rev[i];	
	}
	Uart7_Send(Uart7_Send_Buf,modbus_len);

}

/**
  * @brief  uart1 Һ���� �������400VDCģ�����д�룬uint32��ʽ��10���ܡ�
  * @param  ��ַ ����
  * @retval None
  */
static void handle_modbus_10h(uint16_t modbus_add,int16_t quant)
{
	uint16_t i,modbus_crc_code;
	uint32_t data;
	
	if((modbus_add>=3100)&&(modbus_add<=3149) && (quant == 2)) //PCSģ��
	{

		*(P_PCSdata+modbus_add-3100+100) = (Uart7_rev[7]<<8)+Uart7_rev[8]; 
		*(P_PCSdata+modbus_add-3100+100+1) = (Uart7_rev[9]<<8)+Uart7_rev[10]; 
		data = *(P_PCSdata+modbus_add-3100+100+1) ; 
		data = (data<<16) + *(P_PCSdata+modbus_add-3100+100);
		send_uart3(MODULE_PCS,modbus_add,data,MODBUS_WRITE10);

	}
	else
		return;

		//Ӧ������
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
    config.baud_rate = BAUD_RATE_19200; //�� �� �� �� �� Ϊ 9600
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
						handle_modbus_06h(Uart7_modbus_address,Uart7_modbus_quant);
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
						handle_modbus_10h(Uart7_modbus_address,Uart7_modbus_quant);
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

            if (com7_rxlen < MAX_REV_QUANT)
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

/* �� ʱ �� 1 �� ʱ �� �� */
void timeout_uart7(void *parameter)
{
//    rt_kprintf("periodic timer is timeout \n");
    static uint16_t com7_rx_sta = 0, last_com7_rx_sta = 0, last_com7_rxlen = 0; //com״̬
    //�����¼����������ж��ڿ�ʼ���պ� �����¼�
//	rt_event_recv(&com_event,EVENT_FLAG_RV_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);

	if(com7_rxlen > last_com7_rxlen )
	{
		//���ڽ���״̬
		com7_rx_sta = 1;
	}
	else if(com7_rxlen == last_com7_rxlen )
	{
		//�������״̬
		com7_rx_sta = 0;
	}
	else
	{
		//������� ��Uart7_rev_counter����0,Uart7_rev_counterӦ�ڽ�������������
	}

	if((com7_rx_sta == 0) && (last_com7_rx_sta == 1))
	{
		Uart7_rev_counter = com7_rxlen;
		rt_event_send(&com_event, EVENT_FLAG_PS_UART7);
		com7_rxlen=0;
		//�����ź���
	}

	last_com7_rx_sta = com7_rx_sta;
	last_com7_rxlen = com7_rxlen;
//	rt_thread_delay(10);
}


//����ַת���ɸ���ģ���ͨѶ��ַ��ͨ��Uart3_Postд�뵽���Ͷ���
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
		
		case MODULE_PCS: //16bit��32bit�������ǻ���һ��� ����funcodeΪ06����10
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
