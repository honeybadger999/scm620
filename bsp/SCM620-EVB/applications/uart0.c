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
** �������� ��Crc_Make()
** �������� ������CRCУ���롣
** ��ڲ���1��buff��������CRCУ������ָ�룻
** ��ڲ���2��size����buffָ����׵�ַ��ʼ���ֽ���
** ���ڲ��� ���ޣ�����ֵΪָ��crc��	
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
static uint16_t	Uart0_crc_check( uint16_t data_len, uint8_t *data )
{
	uint16_t	Crc_Code;
 	uint16_t	Crc_LowCode, Crc_HighCode;
	Crc_Code = Uart0_Crc_Make( data_len, data);	//�Խ������ݼ���CRCУ����
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
*	�������ƣ�Send_Uart1	
*	������*buff	���������ڴ�  len�������ݳ���
*	
*	û������DMA������ɵ��жϣ����ǲ��ò�ѯ��ʽ��ͬʱȥ������������ָ��������ڵȴ�ʱ�ڿ����л�����
*   ����Ŀ�������ִ��Ч��
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

	Uart0_Delay_nus(5000);  //��������  ʱ������̫�̻�Ӱ�췢��  5ms ����50���ֽ�û����
	USART0_RX_EN;
//	xTaskResumeAll();//�ָ�������
}

/****************************************************
�����ú�����Uart3_REV_thread
�������ܣ�������յ���DC���ݣ�������ؽṹ�壬give�ź���
���ú�������
*****************************************************/
void handle_DC_DC2()
{	
	uint16_t i,quantity;
	if(Uart0_rev[1] == 3)
	{
		quantity = (Uart0_rev[2]>>1); //�Ĵ�������
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
				*( (&DC2data.add_485) +i) = (Uart0_rev[3+2*i]<<8)+Uart0_rev[4+2*i];//add_485Ϊ��һ��RW����
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
�����ú�����Uart3_REV_thread
�������ܣ�������յ���PCS���ݣ�������ؽṹ�壬give�ź���
���ú�������
*****************************************************/
static void  handle_PCS()
{
	uint16_t i,quantity;
	if(Uart0_rev[1] == 3)
	{
		quantity = (Uart0_rev[2]>>1); //�Ĵ�������
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
				//��������mode��u32����Ҫ��ת����u16��ַ������ÿ�μ�1���ǰ���u32����
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
					//��������bus_V��u32����Ҫ��ת����u16��ַ������ÿ�μ�1���ǰ���u32����
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
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* �� ʼ �� �� �� �� �� */
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

    /* step2�� �� �� �� �� �� �� �� �� */
    config.baud_rate = BAUD_RATE_9600; //�� �� �� �� �� Ϊ 9600
    config.data_bits = DATA_BITS_8; //�� �� λ 8
    config.stop_bits = STOP_BITS_1; //ͣ ֹ λ 1
    config.bufsz = 255; //�� �� �� �� �� buff size Ϊ 128
    config.parity = PARITY_NONE; //�� �� ż У �� λ
    /* step3�� �� �� �� �� �� ���� ͨ �� �� �� �� �� �� �� �� �� �� �� �֣� �� �� �� �� �� */
    rt_device_control(uart0_device, RT_DEVICE_CTRL_CONFIG, &config);
    /* step4�� �� �� �� �� �� ���� �� �� �� �� �� �� �� ѯ �� �� ģ ʽ �� �� �� �� �� �� */
    rt_device_open(uart0_device, RT_DEVICE_FLAG_INT_RX);
    /* �� �� �� �� �� �� �� �� */
//    rt_device_set_rx_indicate(uart0_device, uart0_input);
	
    while (1)
    {
			/* �� �� �� �� ȡ �� ��*/
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
			Uart0_Send_Buf[i++]  = msg->msg.port; //վ��
			Uart0_Send_Buf[i++] =  msg->msg.command; //������
			Uart0_Send_Buf[i++]  = (msg->msg.add>>8) & 0x00ff; 	//�Ĵ�����ַ��
			Uart0_Send_Buf[i++]  = msg->msg.add & 0x00ff; 			//�Ĵ�����ַ��
			Uart0_Send_Buf[i++]  = (msg->msg.len>>8) & 0x00ff;; //�Ĵ���������/ д����ֵ��
			Uart0_Send_Buf[i++]  = msg->msg.len  & 0x00ff;  //�Ĵ���������/ д����ֵ��
		}
		else if( msg->msg.command == MODBUS_WRITE10){
			if(msg->msg.port == PCS_485PORT){ //pcsģ����Ҫ�����ĸ��ֽ�
				Uart0_Send_Buf[i++]  = msg->msg.port; //վ��
				Uart0_Send_Buf[i++] =  msg->msg.command; //������
				Uart0_Send_Buf[i++]  = (msg->msg.add>>8) & 0x00ff; 	//�Ĵ�����ַ��
				Uart0_Send_Buf[i++]  = msg->msg.add & 0x00ff; 			//�Ĵ�����ַ��
				Uart0_Send_Buf[i++]  = 0;  //�Ĵ��������� �̶�Ϊ0��
				Uart0_Send_Buf[i++]  = 2;   //�Ĵ��������� �̶�Ϊ1��
				Uart0_Send_Buf[i++]  = 4;  //�ֽ����� �̶�Ϊ2��
				Uart0_Send_Buf[i++]  = (msg->msg.len>>24) & 0x00ff; //д����ֵ��
				Uart0_Send_Buf[i++]  = (msg->msg.len>>16) & 0x00ff; 
				Uart0_Send_Buf[i++]  = (msg->msg.len>>8) & 0x00ff; 
				Uart0_Send_Buf[i++]  = msg->msg.len  & 0x00ff;  //д����ֵ��
			}
			else{ //dc��Ҫ���������ֽ�
				Uart0_Send_Buf[i++]  = msg->msg.port; //վ��
				Uart0_Send_Buf[i++] =  msg->msg.command; //������
				Uart0_Send_Buf[i++]  = (msg->msg.add>>8) & 0x00ff; 	//�Ĵ�����ַ��
				Uart0_Send_Buf[i++]  = msg->msg.add & 0x00ff; 			//�Ĵ�����ַ��
				Uart0_Send_Buf[i++]  = 0;  //�Ĵ��������� �̶�Ϊ0��
				Uart0_Send_Buf[i++]  = 1;   //�Ĵ��������� �̶�Ϊ1��
				Uart0_Send_Buf[i++]  = 2;  //�ֽ����� �̶�Ϊ2��
				Uart0_Send_Buf[i++]  = (msg->msg.len>>8) & 0x00ff;; //�Ĵ���������/ д����ֵ��
				Uart0_Send_Buf[i++]  = msg->msg.len  & 0x00ff;  //�Ĵ���������/ д����ֵ��
			}
		}
		crc_code = Uart0_Crc_Make(i,Uart0_Send_Buf);
		Uart0_Send_Buf[i++] = (crc_code>>8) & 0x0FF;
		Uart0_Send_Buf[i++] = crc_code & 0x0FF;
		len = i; 	

		Uart0_Send(Uart0_Send_Buf,len);		
		rt_free(msg); //�ͷ��ڴ�,����ǿ�ָ�룬�򲻻�ִ���κβ���	
/*
		��ʱ,�ȴ������ϴ� �����ڶ����п��ܴ��ڶ������ݵȴ��ϴ��������������ʱ��Uart3_read_thread
		�����еĵȴ�����Ϊ��ͬ�����еģ�������ӹ�ϵ����������Ϊ400ms
		���ҵ�ǰ��������ʽ��������ʱʱ����Ƿ��ͼ��		
*/
		rt_thread_delay(30);
	}
}

uint8_t Uart0_Post(uint8_t port,uint8_t command,uint16_t add,uint32_t data)
{
	static struct Uart_msg *Uart0_msg;	
	Uart0_msg = (struct Uart_msg *)rt_malloc(sizeof(struct Uart_msg));
	if(Uart0_msg ==NULL)//����ʧ��
	{
	//	DEBUG_PRINTF("uart3_msg �����ڴ�ʧ��\n");
	//	return ERRMEM;
	}
	
    Uart0_msg->msg.port = port;; //վ��
	Uart0_msg->msg.command = command; //������
	Uart0_msg->msg.add  = add; 	//�Ĵ�����ַ
	Uart0_msg->msg.len = data; //�Ĵ�������/ д����ֵ
	 
//	UBaseType_t temp;	
	//temp = uxQueueSpacesAvailable( xQueue_Uart3 );
//	DEBUG_PRINTF("xQueue_Uart3 : %d\n",temp);
	
	if(command == MODBUS_READ){
		rt_mb_send(mb_uart0, (rt_uint32_t)(Uart0_msg));
	}
	else if((command == MODBUS_WRITE)||(command == MODBUS_WRITE10)){ //�ɿ��ǻ��ɶ��У������ȷ���
		rt_mb_send(mb_uart0, (rt_uint32_t)(Uart0_msg));
//		DEBUG_PRINTF("xQueue_Uart3 write\n");
	}
//	return ERROK;
}

/********************************************************************
�����ú�����������
�������ܣ�ѭ������uart3���ڵ�����ָ�timeout 200ms��delay 2S
				ͨ���ź����ж��Ƿ�ʱ�������ݱ���ȷ���պ���ź���ִ��give����
���ú�����Uart3_Read_module
***********************************************************************/
void uart0_read_thread_entry(void* parameter)
{        
	rt_err_t xReturn = RT_EOK;
//	DEBUG_PRINTF("%s\n",(char *)s);
	for(;;)
	{		

		Uart0_Post(DC2_485PORT,MODBUS_READ,DC_READ_START_ADD, DC_READ_QUAN);
		xReturn = rt_sem_take(sem_com_dc, 20);// ��ȡ�ź��� ��ʱ�ȴ�
		if(xReturn == RT_EOK)
			LEUER_Data.EMS_Data.DC2_Com_State = PASS;
		else
			LEUER_Data.EMS_Data.DC2_Com_State = FAIL;
		rt_thread_delay(20);
		//��ȡDC2�� �ɱ������
		Uart0_Post(DC2_485PORT,MODBUS_READ,DC_SET_START_ADD, DC_SET_QUAN);
		rt_sem_take(sem_com_dc, 20);// ��ȡ�ź��� ��ʱ�ȴ�
		rt_thread_delay(20);
		

		PCS_Read_Block = 1;
		Uart0_Post(PCS_485PORT,MODBUS_READ,PCS_READ_START_ADD1, PCS_READ_QUAN1);
		xReturn = rt_sem_take(sem_com_pcs, 20);// ��ȡ�ź��� ��ʱ�ȴ�
		rt_thread_delay(20);
		if(xReturn == RT_EOK)
			LEUER_Data.EMS_Data.PCS_Com_State = PASS;
		else
			LEUER_Data.EMS_Data.PCS_Com_State = FAIL;
		
		PCS_Read_Block = 6;
		Uart0_Post(PCS_485PORT,MODBUS_READ,PCS_READ_START_ADD2, PCS_READ_QUAN2);
		rt_sem_take(sem_com_pcs, 20);// ��ȡ�ź��� ��ʱ�ȴ�
		rt_thread_delay(20);
		//��ȡPCS�� �ɱ������
		PCS_Read_Block = 2;
		Uart0_Post(PCS_485PORT,MODBUS_READ,PCS_SET_START_ADD, PCS_SET_QUAN);
		rt_sem_take(sem_com_pcs, 20);// ��ȡ�ź��� ��ʱ�ȴ�	
		rt_thread_delay(20);
		
		PCS_Read_Block = 3;
		Uart0_Post(PCS_485PORT,MODBUS_READ,0x2800, 4);
		rt_sem_take(sem_com_pcs, 20);// ��ȡ�ź��� ��ʱ�ȴ�	
		rt_thread_delay(20);
				
		PCS_Read_Block = 4;
		Uart0_Post(PCS_485PORT,MODBUS_READ,PCS_VDC_READ_ADD, 1);
		rt_sem_take(sem_com_pcs, 20);// ��ȡ�ź��� ��ʱ�ȴ�
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


		// ��ʱ��
		 rt_thread_delay(100);
	}

}

/* �� ʱ �� 1 �� ʱ �� �� */
void timeout_uart0(void *parameter)
{
//    rt_kprintf("periodic timer is timeout \n");
    static uint16_t com0_rx_sta = 0, last_com0_rx_sta = 0, last_com0_rxlen = 0; //com״̬

    //�����¼����������ж��ڿ�ʼ���պ� �����¼�
//	rt_event_recv(&com_event,EVENT_FLAG_RV_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);

	if(com0_rxlen > last_com0_rxlen )
	{
		//���ڽ���״̬
		com0_rx_sta = 1;
	}
	else if(com0_rxlen == last_com0_rxlen )
	{
		//�������״̬
		com0_rx_sta = 0;
	}
	else
	{
		//������� ��Uart7_rev_counter����0,Uart7_rev_counterӦ�ڽ�������������
	}

	if((com0_rx_sta == 0) && (last_com0_rx_sta == 1))
	{
		Uart0_rev_counter = com0_rxlen;
		rt_event_send(&com_event, EVENT_FLAG_PS_UART0);
		com0_rxlen=0;
		//�����ź���
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

    // �޸����س���ʧ������ 2020.11.26
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
			
			if(com0_rxlen == 1) //��ʼ����
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


