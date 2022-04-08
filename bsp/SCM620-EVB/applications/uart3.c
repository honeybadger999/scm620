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
#define METER_485PORT 2//�ݶ�
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
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT; /* �� ʼ �� �� �� �� �� */
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

    /* step2�� �� �� �� �� �� �� �� �� */
    config.baud_rate = BAUD_RATE_9600; //�� �� �� �� �� Ϊ 9600
    config.data_bits = DATA_BITS_8; //�� �� λ 8
    config.stop_bits = STOP_BITS_1; //ͣ ֹ λ 1
    config.bufsz = 255; //�� �� �� �� �� buff size Ϊ 128
    config.parity = PARITY_NONE; //�� �� ż У �� λ
    /* step3�� �� �� �� �� �� ���� ͨ �� �� �� �� �� �� �� �� �� �� �� �֣� �� �� �� �� �� */
    rt_device_control(uart3_device, RT_DEVICE_CTRL_CONFIG, &config);
    /* step4�� �� �� �� �� �� ���� �� �� �� �� �� �� �� ѯ �� �� ģ ʽ �� �� �� �� �� �� */
    rt_device_open(uart3_device, RT_DEVICE_FLAG_INT_RX);
    /* �� �� �� �� �� �� �� �� */
//    rt_device_set_rx_indicate(uart0_device, uart0_input);
	
    while (1)
    {
			/* �� �� �� �� ȡ �� ��*/
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
				hour_meter.Ua = CHANGE_H16_L16(hour_meter.Ua); //���ѹ
				hour_meter.Ub = CHANGE_H16_L16(hour_meter.Ub); 
				hour_meter.Uc = CHANGE_H16_L16(hour_meter.Uc);
				hour_meter.Uab = CHANGE_H16_L16(hour_meter.Uab); //�ߵ�ѹ
				hour_meter.Ubc = CHANGE_H16_L16(hour_meter.Ubc); 
				hour_meter.Uca = CHANGE_H16_L16(hour_meter.Uca); 
				hour_meter.Ia = CHANGE_H16_L16(hour_meter.Ia); //�����
				hour_meter.Ib = CHANGE_H16_L16(hour_meter.Ib); 
				hour_meter.Ic = CHANGE_H16_L16(hour_meter.Ic);	
				hour_meter.Pa = CHANGE_H16_L16(hour_meter.Pa); //�й�����
				hour_meter.Pb = CHANGE_H16_L16(hour_meter.Pb); 
				hour_meter.Pc = CHANGE_H16_L16(hour_meter.Pc);
				hour_meter.Ps = CHANGE_H16_L16(hour_meter.Ps); 
				hour_meter.Qa = CHANGE_H16_L16(hour_meter.Qa); //�޹�����
				hour_meter.Qb = CHANGE_H16_L16(hour_meter.Qb); 
				hour_meter.Qc = CHANGE_H16_L16(hour_meter.Qc);
				hour_meter.Qs = CHANGE_H16_L16(hour_meter.Qs);
				hour_meter.VAa = CHANGE_H16_L16(hour_meter.VAa); //���ڹ���
				hour_meter.VAb = CHANGE_H16_L16(hour_meter.VAb); 
				hour_meter.VAc = CHANGE_H16_L16(hour_meter.VAc);
				hour_meter.VAs = CHANGE_H16_L16(hour_meter.VAs);
				hour_meter.PFa = CHANGE_H16_L16(hour_meter.PFa); //��������
				hour_meter.PFb = CHANGE_H16_L16(hour_meter.PFb); 
				hour_meter.PFc = CHANGE_H16_L16(hour_meter.PFc);
				hour_meter.PFs = CHANGE_H16_L16(hour_meter.PFs);
				hour_meter.Frequency = CHANGE_H16_L16(hour_meter.Frequency);
				hour_meter.KWH[0] = CHANGE_H16_L16(hour_meter.KWH[0]); //����
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
		
		//�������
		else if(Uart3_rev[0] == 1){
			for(i=0;i<(Uart3_rev[2]>>1);i++)
			{
				*(P_PV_Glass+i) = (Uart3_rev[3+2*i]<<8)+Uart3_rev[4+2*i]; //������������ݿ���Ҫ�����ߵ�16λ
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
*	�������ƣ�Send_Uart3	
*	������*buff	���������ڴ�  len�������ݳ���
*	
*	û������DMA������ɵ��жϣ����ǲ��ò�ѯ��ʽ��ͬʱȥ������������ָ��������ڵȴ�ʱ�ڿ����л�����
*   ����Ŀ�������ִ��Ч��
*
*    ������̫���� Uart3_SendDMA�޷�ʹ�ã� ��Ҫ����sct�ļ���ȷ��Uart3_Send_Buf��IRAM1��
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

	Uart3_Delay_nus(5000);  //��������  ʱ������̫�̻�Ӱ�췢��  5ms ����50���ֽ�û����
	USART3_RX_EN;
//	xTaskResumeAll();//�ָ�������
}

void Uart3_MSG_send(uint8_t port,uint8_t command,uint16_t add, uint16_t quant)
{
	uint16_t i,crc_code,len;

	i=0;		
	Uart3_Send_Buf[i++]  = port; //վ��
	Uart3_Send_Buf[i++] =  command; //������
	Uart3_Send_Buf[i++]  = (add>>8) & 0x00ff; 	//
	Uart3_Send_Buf[i++]  = add & 0x00ff; 			//�Ĵ�����ַ��
	Uart3_Send_Buf[i++]  = (quant>>8) & 0x00ff;; //�Ĵ���������/ д����ֵ��
	Uart3_Send_Buf[i++]  = quant  & 0x00ff;  		//�Ĵ���������/ д����ֵ��
	 
	crc_code = Uart3_Crc_Make(i,Uart3_Send_Buf);
	Uart3_Send_Buf[i++] = (crc_code>>8) & 0x0FF;
	Uart3_Send_Buf[i++] = crc_code & 0x0FF;

	len = i; 			
	Uart3_Send(Uart3_Send_Buf,len);	
}

/*******************************************************************************************************
** �������� ��Crc_Make()
** �������� ������CRCУ���롣
** ��ڲ���1��buff��������CRCУ������ָ�룻
** ��ڲ���2��size����buffָ����׵�ַ��ʼ���ֽ���
** ���ڲ��� ���ޣ�����ֵΪָ��crc��	
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
			
		if(car>256)   //20190916 ����������С���ʻ����������ֱ�ӷ���
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
	Crc_Code = Uart3_Crc_Make( data_len, data);	//�Խ������ݼ���CRCУ����
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
		Uart3_MSG_send(METER_485PORT,MODBUS_READ,METER_Read_Start_ADD, METER_Read_Quan); //���Ϳ�������	
		xReturn = rt_sem_take(sem_com_meter, 20);// ��ȡ�ź��� ��ʱ�ȴ�
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
		
		//��ȡ������� 04������ 3004��ַ��ȡ11������
		Uart3_MSG_send(1,4,3004, 11);
		
		xReturn = rt_sem_take(sem_com_meter, 20);// ��ȡ�ź��� ��ʱ�ȴ�
		if(xReturn == RT_EOK)
			LEUER_Data.EMS_Data.BAT_Com_State = PASS;
		else
			LEUER_Data.EMS_Data.BAT_Com_State = FAIL;
		rt_thread_delay(2000/ portTICK_RATE_MS);
	}
}

/* �� ʱ �� 1 �� ʱ �� �� */
void timeout_uart3(void *parameter)
{
//    rt_kprintf("periodic timer is timeout \n");
    static uint16_t com3_rx_sta = 0, last_com3_rx_sta = 0, last_com3_rxlen = 0; //com״̬

    //�����¼����������ж��ڿ�ʼ���պ� �����¼�
//	rt_event_recv(&com_event,EVENT_FLAG_RV_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);

	if(com3_rxlen > last_com3_rxlen )
	{
		//���ڽ���״̬
		com3_rx_sta = 1;
	}
	else if(com3_rxlen == last_com3_rxlen )
	{
		//�������״̬
		com3_rx_sta = 0;
	}
	else
	{
		//������� ��Uart7_rev_counter����0,Uart7_rev_counterӦ�ڽ�������������
	}

	if((com3_rx_sta == 0) && (last_com3_rx_sta == 1))
	{
		Uart3_rev_counter = com3_rxlen;
		rt_event_send(&com_event, EVENT_FLAG_PS_UART3);
		com3_rxlen=0;
		//�����ź���
	}

	last_com3_rx_sta = com3_rx_sta;
	last_com3_rxlen = com3_rxlen;
//	rt_thread_delay(10);
}

/****************************************************************
  * @brief  uart3�жϽ��պ���
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

    // �޸����س���ʧ������ 2020.11.26
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
			
			if(com3_rxlen == 1) //��ʼ����
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
