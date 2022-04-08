/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "LE_uart.h"
#include "LE_can.h"
#include "RX8025T.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//20210422����
#define PCS_RUN_SUCCESS1 0x0C    //PCS ����ʱ״̬ C ��������
#define PCS_RUN_SUCCESS2 0x0B    //PCS ����ʱ״̬ B ��ѹ����
#define PCS_RUN_ADD  3110 //PCS �鿪�ػ���ַ
#define PCS_RUN_COMMAND 1   //PCS ģ�鿪������
#define PCS_STOP_COMMAND 0   //PCS ģ��ػ�����
#define PCS_MOD_ADD 3100
#define PCS_FAULT_CLEAR_ADD 3111
#define PCS_POW_ADD 3102  //��������

//PCS������ ABC�ߵ�ѹ�жϣ���310-450�� ������Ϊ1 �� Ƶ�ʣ���49-51�� Ϊ1
#define PCS_VA_CHECK ( ((PCSdata.sys_u[0]>250000) ? 0:1 ) && ((PCSdata.sys_u[0]<190000) ? 0:1 ) ) 
#define PCS_VB_CHECK ( ((PCSdata.sys_u[1]>250000) ? 0:1 ) && ((PCSdata.sys_u[1]<190000) ? 0:1 ) )
#define PCS_VC_CHECK ( ((PCSdata.sys_u[2]>250000) ? 0:1 ) && ((PCSdata.sys_u[2]<190000) ? 0:1 ) )
//#define PCS_F_CHECK  ( ((WH_PCSdata.Sys_Freq>5100) ? 0:1 ) && ((WH_PCSdata.Sys_Freq<4900) ? 0:1 ) )  //PCS������Ƶ��
#define PCS_SYSTEM_CHECK 	(PCS_VA_CHECK && PCS_VB_CHECK && PCS_VC_CHECK) //&& PCS_F_CHECK)   //PCS�������ѹ  ������1����������0��


#define PILE_RUN_COMMAND 0
#define PILE1GUNA_RUN_ADD 2437
#define PILE1GUNA_POW_ADD 2436
#define PILE1GUNB_RUN_ADD 2537
#define PILE1GUNB_POW_ADD 2536
#define PILE2GUNA_RUN_ADD 2637
#define PILE2GUNA_POW_ADD 2636
#define PILE2GUNB_RUN_ADD 2737
#define PILE2GUNB_POW_ADD 2736
#define PILE3GUNA_RUN_ADD 2837
#define PILE3GUNA_POW_ADD 2836
#define PILE3GUNB_RUN_ADD 2937
#define PILE3GUNB_POW_ADD 2936


#define DC_RUN_SUCCESS 0x3F01   //DCģ�����гɹ���״̬
#define DC_RUN_COMMAND 1   //DCģ�鿪������
#define DC_STOP_COMMAND 2   //DCģ��ػ�����

#define DC2_MOD_ADD  2206 //DC ģ��ģʽ��ַ
#define DC2_RUN_ADD  2215 //DC ģ�鿪�ػ���ַ

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern uint16_t PV_Power_21;//21���豸�Ĺ�����繦��
/* Private function prototypes -----------------------------------------------*/
static void ShutDown(void);
static void StartUp(void);
static void auto_start_stop(void);
static uint16_t fault_check(void);
/* Private functions ---------------------------------------------------------*/

/**********************************
**	�������ƣ�ShutDown
**  ��������: //�ػ������յ��ػ�����ʱ�������ع���ʱ
     �����Ҫ��ʱ��Ӧ�ػ�ָ�����������DCģ�������Ϲػ������Խ��ػ�������һ�����ȼ��ϸߵ�����
	 �յ��ػ�ָ���task delete �������� ��ִ�йػ���ִ����Ϻ����½��������� ��
***********************************/
static void ShutDown(void)
{
    uint16_t time=0;
	//Ϊ��ֹ��Ϊĳ��ģ���޷��ػ�����������ģ��Ҳû�취ִ�йػ������Թػ�ָ��һ��ִ�С�
	do{
		rt_kprintf("�ػ�ִ����..\n");
		//send_uart3(MODULE_DCAC,DC_AC_RUN_ADD,0); //����DC-AC�ػ�
		send_uart3(MODULE_PCS,PCS_RUN_ADD,PCS_STOP_COMMAND,MODBUS_WRITE); //����PCSģ��ػ�
		rt_thread_delay( 2000 / portTICK_RATE_MS );
		send_uart3(MODULE_DC2,DC2_RUN_ADD,DC_STOP_COMMAND,NULL); //����DC2ģ��ػ�
		rt_thread_delay( 2000 / portTICK_RATE_MS );
		if(time++ ==10){ //�����������10��
			LEUER_Data.EMS_Data.Status_FLAG = 7;
			rt_kprintf("�ػ�ʧ��\n");
			break;
		}				
	}while((PCSdata.state == PCS_RUN_SUCCESS1)|| (PCSdata.state == PCS_RUN_SUCCESS2)||(DC2data.state5 == DC_RUN_SUCCESS ));
		
	LEUER_Data.EMS_Data.Run_FLAG = 0;//�ػ���־
	//vTaskSuspend(xHandle_EMS);

}
/**********************************
**	�������ƣ�StartUp
**  ��������: //�������ڿ��ư�����ʱ�����յ���������ʱ
    20210918 �Ż�����
      1. ����⽻����ѹ��ͨѶ����ʱ�����������ָ���һֱ���Կ���
      2. ������豸�޷������Ĺ��ϣ�Ϊ�˰�ȫ�����������ָ�Ҫ���ٴο���ֻ���������ư塣
***********************************/
static void StartUp(void)
{
	uint8_t i,counter,time=0;

	LEUER_Data.EMS_Data.PCS_Fault = 0;
	LEUER_Data.EMS_Data.DC_ChuNeng_Fault = 0;
	LEUER_Data.EMS_Data.DC_GuangFu_Fault = 0;

	LEUER_Data.EMS_Data.PCS_Com_State = 0;
//	LEUER_Data.EMS_Data.DC_AC_Com_State = 0;
	LEUER_Data.EMS_Data.BAT_Com_State = 0;
	LEUER_Data.EMS_Data.DC2_Com_State = 0;
//	LEUER_Data.EMS_Data.DC1_Com_State = 0;
	LEUER_Data.EMS_Data.COM_Error_FLAG = 0;
	LEUER_Data.EMS_Data.BAT_Error_FLAG = 0;
	LEUER_Data.EMS_Data.VDC_Error_FLAG = 0;
	LEUER_Data.EMS_Data.V_Error_FLAG = 0;
	LEUER_Data.EMS_Data.Run_Error_FLAG = 0;
	LEUER_Data.EMS_Data.Run_FLAG = 0;
	

	//���PCS�����ѹƵ���Ƿ�����,��ǰֻ�����˲��������
	do{
		rt_kprintf("PCS���߽�����ѹ�����..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 10;
		if(PCS_SYSTEM_CHECK ==0 ){
			LEUER_Data.EMS_Data.V_Error_FLAG = 1; 
		}
		else{
			LEUER_Data.EMS_Data.V_Error_FLAG = 0;
		}
		rt_thread_delay( 2000 / portTICK_RATE_MS );	
		if(time++ ==10){ //�����������10�Σ���Ϊ���� ��������,��ִ�йػ�
			LEUER_Data.EMS_Data.V_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			//LEUER_Data.EMS_Data.Run = 0;  ���ߵ�ѹû��ʱ��һֱ��� �����Կ���
			return;      //ֱ���˳����´����񻹻᳢�Լ��
		}		
	}while(LEUER_Data.EMS_Data.V_Error_FLAG); 
	
	//������Һ���������ػ�������Ҫ���Ϲػ����Ƴ������̡�
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //����ػ������˳��˺���
	
	//���ͨѶ,��ǰ���  pcs 
	do{
		rt_kprintf("ͨѶ�����..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 11;
		//if((LEUER_Data.EMS_Data.BAT_Com_State == FAIL) || (LEUER_Data.EMS_Data.PCS_Com_State == FAIL))	
		if(LEUER_Data.EMS_Data.PCS_Com_State == FAIL)			
			LEUER_Data.EMS_Data.COM_Error_FLAG = 1;//ͨѶ״̬����
		else
			LEUER_Data.EMS_Data.COM_Error_FLAG = 0;
		
		rt_thread_delay( 2000 / portTICK_RATE_MS );
		if(time++ ==10){ //�����������10�Σ���ΪͨѶ���� ��������,��ִ�йػ�
			LEUER_Data.EMS_Data.COM_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			//LEUER_Data.EMS_Data.Run = 0; ͨѶû��ʱ��һֱ��� �����Կ���
			return;
		}
	}while(LEUER_Data.EMS_Data.COM_Error_FLAG );
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //����ػ������˳��˺���

	//PCS�������
	do{
		rt_kprintf("PCS���������..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 14;

		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_PCS,PCS_FAULT_CLEAR_ADD,1,MODBUS_WRITE); //�������

		rt_thread_delay( 5000 / portTICK_RATE_MS ); //����ģ�鿪��
		if(time++ ==10){ //�����������10�Σ���Ϊ���� ��������,��ִ�йػ�
			LEUER_Data.EMS_Data.PCS_Fault = 1;
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run = 0;
			break;
		}
	}while(PCSdata.fault != 0);
	
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //����ػ������˳��˺���
	
	//����PCS����
	do{
		rt_kprintf("pmaģ�鿪����..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 15;
		send_uart3(MODULE_PCS,PCS_MOD_ADD,0x02,MODBUS_WRITE10);  //����PCSģ������ģʽΪ0X02 ��ѹģʽ.
		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_PCS,PCS_POW_ADD,100000,MODBUS_WRITE10);  //����PCS ��ؿڵ�ѹ100V
		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_PCS,PCS_FAULT_CLEAR_ADD,1,MODBUS_WRITE); //�������
		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_PCS,PCS_RUN_ADD,PCS_RUN_COMMAND,MODBUS_WRITE); //����PCSģ�鿪������
		rt_thread_delay( 8000 / portTICK_RATE_MS ); //ģ�鿪��
		if(time++ ==10){ //�����������10�Σ���Ϊ���� ��������,��ִ�йػ�
			LEUER_Data.EMS_Data.PCS_Fault = 1;
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run = 0;
			break;
		}
	}while((PCSdata.state != PCS_RUN_SUCCESS1) && (PCSdata.state != PCS_RUN_SUCCESS2));
	
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //����ػ������˳��˺���
	
	//���ģ�����ĸ�ߵ�ѹ����10S��xx��xx֮�䣬���ӳ����೤ʱ�䱨���ϣ��ҹػ�
	do{
		rt_kprintf("PCSģ��VDCĸ�ߵ�ѹ�����..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 16;
		counter=0;
		for(i=0;i<10;i++){
			if((PCSdata.VDC >6500 ) && (PCSdata.VDC <8000 )){	 
				counter++;	
			}
			rt_thread_delay( 1000 / portTICK_RATE_MS );
		}
		if(counter<10){
			LEUER_Data.EMS_Data.VDC_Error_FLAG = 1;//VDC״̬����
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run = 0;
			break;
		}
		else
			LEUER_Data.EMS_Data.VDC_Error_FLAG = 0;
	}while(LEUER_Data.EMS_Data.VDC_Error_FLAG );
	
	
	//�������ģ�鿪��  ģʽΪ7���ģʽ����߲��뿪�� bit1-5  00010 ������룺�˿�2  ����ڣ��˿�1 
	//�������������ѹΪ400-850V ����������ʸ��ٹ��ܡ�
	do{
		rt_kprintf("���DCģ�鿪����..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 18;
		send_uart3(MODULE_DC2,DC2_MOD_ADD,7,NULL);  //����DC2ģ������ģʽΪ7.
		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_DC2,DC2_RUN_ADD,DC_RUN_COMMAND,NULL); //����DC2ģ�鿪������
		rt_thread_delay( 5000 / portTICK_RATE_MS ); //����ģ�鿪��
		if(time++ ==10){ //�����������10�Σ���ΪDC2���� ��������,��ִ�йػ�
			//LEUER_Data.EMS_Data.DC_GuangFu_Fault = 1;  //20211019 ��Ϊ���Ϲ���޷����������������ߵ�����ᱨ���ػ�������
			//LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			//LEUER_Data.EMS_Data.Run = 0;
			break;
		}
	}while(DC2data.state5 != DC_RUN_SUCCESS );//�����ɹ���ʾ3F01�����û��д��������		
		
		
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //����ػ������˳��˺���
	
	LEUER_Data.EMS_Data.Run_FLAG = 1;//������־
	LEUER_Data.EMS_Data.Status_FLAG = 5;
	LEUER_Data.EMS_Data.Run_Error_FLAG = 0;
	rt_kprintf("����ִ�����\n");
			
	//vTaskResume(xHandle_EMS);
}

/**********************************************************************************************************
**	�������ƣ�fault_check
**  �������ܣ�ѭ������豸����
************************************************************************************************************/
static uint16_t fault_check(void)
{		
	//PCSģ����ϼ��
	if(PCSdata.fault!=0){
		LEUER_Data.EMS_Data.PCS_Fault = 1;
		rt_kprintf("PCS���ϲ��ػ�\n");
		LEUER_Data.EMS_Data.Status_FLAG = 21;
		return 1;
	}
	else
		LEUER_Data.EMS_Data.PCS_Fault = 0;
		
	//����   ֱ��ĸ�ߵ�ѹ����xxV
	if((PCSdata.VDC >8000 )||(PCSdata.VDC <6000 )){
		LEUER_Data.EMS_Data.VDC_Error_FLAG = 1;//VDC״̬����
		rt_kprintf("VDC�������ػ�\n");
		LEUER_Data.EMS_Data.Status_FLAG = 23;
		return 1;
	}
	else
		LEUER_Data.EMS_Data.VDC_Error_FLAG = 0;
	return 0;
}

/**********************************************************************************************************
**	�������ƣ�auto_start_stop
**  �������ܣ���Ⲣִ��һ�������ػ�
************************************************************************************************************/
static void auto_start_stop(void)
{	
	if((LEUER_Data.EMS_Data.Run_FLAG == 0)&&(LEUER_Data.EMS_Data.Run == 0xAAAA)){//���û�п�������ִ��startup,����ǿ������Ź�������������ִ�п�������
		StartUp();	
		if(LEUER_Data.EMS_Data.Run_FLAG == 0){
			rt_kprintf("����ʧ�ܻ���;�˳�\n");
		}
	}
	else if(LEUER_Data.EMS_Data.Run != 0xAAAA){//�ػ��������⵽�κ�һ��ģ���ڿ���״̬��ִ�йػ���
		if((PCSdata.state == PCS_RUN_SUCCESS1)|| (PCSdata.state == PCS_RUN_SUCCESS2)){  //PCS���гɹ�����ֵ��ȷ��
			ShutDown();
			LEUER_Data.EMS_Data.Status_FLAG = 6;	
			rt_kprintf("�ػ��ɹ�\n");
		}
	}	
}

/**********************************************************************************************************
**	�������ƣ�EMS_thread_XA(void *s)
**  �������ܣ�ÿsִ��һ�γ����ڲ��벢���ӳ���ÿ5��ִ��һ�Σ����ػ�ָ��Ͷ�PCS�벢������ָ��Ϊ��ʱ
    ���ͣ�ֻҪ������ͷ��ͣ��벢���ӳ����ģʽ�ı�ͨ���ԱȺ������ͣ�����һ��5��ĵȴ����
************************************************************************************************************/	

void EMS_thread_YGY_entry(void *parameter)
{	
	//uint32_t total_pow,average_pow;
	//uint8_t counter,flag_1A,flag_1B,flag_2A,flag_2B,flag_3A,flag_3B;
	uint16_t Power_PV21;
	uint16_t Power_MD950,lastPower_MD950=0;
	rt_kprintf("%s\n",(char *)parameter);
	for(;;)
	{
		rt_thread_delay( 5000 / portTICK_RATE_MS ); 
		if(LEUER_Data.EMS_Data.LEUER_Mod == 0x0A){ //��������Զ�ģʽ
			auto_start_stop();
		}	
		else{ //�ֶ�ģʽ���ֶ���������ģ�鿪��
		}
		//ֻ���Զ�ģʽRun_FLAG�Ż�Ϊ1.�����ֶ�ģʽ������Ϻ�����������
		if(LEUER_Data.EMS_Data.Run_FLAG == 1){ //����ִ���˿������̣�˵���Ѿ�������û�йػ��������ǿ�����Ĺ���״̬��
			if(fault_check()){ //��������
				ShutDown(); //ֱ��ִ�йػ�
				
				//����������̣������shutdown��������ִ�й���Ϊ�������̣��ٴ�ִ�С�
				//ִ��������㽫�����������������´������п�����
				LEUER_Data.EMS_Data.Run_FLAG = 0;
				
			}
			
			//20211028 �·������ģ�����ϳ��򣬲����Զ�����������⵽����ʱ�����ҿ��ڵ�ѹ����650*0.9Vʱ���·�����ָ��
			
			if((timePC[3]<17) && (timePC[3]>=4)){  //��4-18��ʱ�Ż���������
				if(DC2data.VDC2 > 6000){  //ʵ������ʱ������590V����������Ըĳ�600V.
					send_uart3(MODULE_DC2,DC2_MOD_ADD,7,NULL);  //����DC2ģ������ģʽΪ7.
					rt_thread_delay( 1000 / portTICK_RATE_MS );
					send_uart3(MODULE_DC2,DC2_RUN_ADD,DC_RUN_COMMAND,NULL); //����DC2ģ�鿪������
				}
			}
			else if((timePC[3]>=18) || (timePC[3]<4)){  //��18�㵽�賿4��ʱ�������⵽DC���ڿ������򷢹ػ�
				if(DC2data.VDC2 > 5000){  //
					send_uart3(MODULE_DC2,DC2_RUN_ADD,DC_STOP_COMMAND,NULL); 
				}	
			}
			
			//����DC-DC�̶�ʱ�俪�ػ�
			if((timePC[3]==6) && (timePC[4]==01)){
				TRD_MD950_Value.power_drection = 2;
				TRD_MD950_Value.v_portA = 780;
				TRD_MD950_Value.v_portB = 720;
				TRD_MD950_Value.p_dalta = Power_MD950; 
				TRD_MD950_Value.run = 0; //����
				
				send_can2_MD950( 5001, 2);
				rt_thread_delay( 1000 / portTICK_RATE_MS );
				send_can2_MD950( 5002, 780);
				rt_thread_delay( 1000 / portTICK_RATE_MS );
				send_can2_MD950( 5004, 720);
				rt_thread_delay( 1000 / portTICK_RATE_MS );
				send_can2_MD950( 5006, Power_MD950);
				rt_thread_delay( 1000 / portTICK_RATE_MS );
				send_can2_MD950( 5000, 0);	
			}	

			else if((timePC[3]==19) && (timePC[4]==01)){
				TRD_MD950_Value.p_dalta =0;
				TRD_MD950_Value.run = 1; //�ػ�
				send_can2_MD950( 5000, 1);
				
				Power_MD950=0;
				Power_PV21=0; //�����ʱ�䲻���£��ػ�ʱȷ����ֵ0һ�Ρ�
			}
			
			
			//���㵱ǰ���繦��
			Power_PV21 = abs(6553-PV_Power_21); //��λ1kw
			//���Ӹ���PV_Power_21 ����ĳ���  ��send_can2_MD950��3�Ѿ��޸�Ϊ����0.1kwΪ��λ
			Power_MD950 = Power_PV21*10/3; //����ÿ��ģ���������ʣ�0.1kw   13��ʾ1.3kw
			if(Power_MD950>70) Power_MD950 = 0; //ÿ̨����7kw ˵���Ǹտ��������쳣
			
			LEUER_Data.EMS_Data.DC1_Com_State = Power_MD950*3; //���ڽ�ʵ�ʷ��繦�ʷ����ں��ն� ��λ0.1kw  ��Ϊ��������������ʧ��0.-0.9kw�Ĺ���
			if(LEUER_Data.EMS_Data.DC1_Com_State<5) LEUER_Data.EMS_Data.DC1_Com_State=0; 
			
			if(Power_MD950!=lastPower_MD950){ //�����б仯�ͷ���
				TRD_MD950_Value.p_dalta = Power_MD950; 
				send_can2_MD950( 5006, Power_MD950);
				
			}			
			lastPower_MD950 = Power_MD950;
		}	
	}		
}
