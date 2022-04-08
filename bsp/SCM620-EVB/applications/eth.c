#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>

#include "misc.h"

#include <netif/ethernetif.h>
#include "lwipopts.h"
#include "drv_eth.h"
#include "misc.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "eth.h"
#include "LE_data_transfer.h"
#include "LE_uart.h"

#define PROTOCLO_MODBUS_PORT 502
#define TCP_BUFSIZE 1024  


ModbusTCP_Send_Struct *P_modbusTCP_send;
unsigned char Send_Buf_modbus[300];
u16_t Send_modbusTCP_Len;
ModbusTCP_Struct *P_modbusTCP;
uint16_t PV_Power_21;//21���豸�Ĺ�����繦��

uint16_t *P_ttu_ems =( uint16_t *)&(DATA_ttu.DATA_TTU_ems);
uint16_t *P_ttu_pcs = ( uint16_t *)&(DATA_ttu.DATA_TTU_pcs);
uint16_t *P_ttu_stac = ( uint16_t *)&(DATA_ttu.DATA_TTU_stac);
uint16_t *P_ttu_pvac = ( uint16_t *)&(DATA_ttu.DATA_TTU_pvac) ;
uint16_t *P_ttu_stbat = ( uint16_t *)&(DATA_ttu.DATA_TTU_stbat); 
uint16_t *P_ttu_charge = ( uint16_t *)&(DATA_ttu.DATA_TTU_charge); 
uint16_t *P_ttu_meter = ( uint16_t *)&(DATA_ttu.DATA_TTU_meter);
uint16_t *P_ttu_pvclass = ( uint16_t *)&(DATA_ttu.DATA_TTU_pvclass);


struct struct_Ethernet_Par Ethernet_Par;
uint16_t *P_ethernet = ( uint16_t *) &Ethernet_Par;
unsigned char Rev_Buf_modbus[1024];

int sock_modbus = -1;
int socket_connect_modbus=0;

volatile uint16_t link_status_global;
uint16_t counter_modbustcp=0;
uint16_t flag_modbustcp=0;

void vEthLinkChkTask( void * parameter )
{
	uint8_t status = 0;
	uint32_t  t;	
	static uint8_t task_exist=0;//��¼�����Ƿ���ڱ�־λ
	static uint8_t counter = 0;
	rt_thread_t tid1 = RT_NULL;
//	DEBUG_PRINTF("%s\n",(char *)parameter);
	for(;;)
	{		

		if(link_status_global && !status){
		//	 DEBUG_PRINTF(">>���߻ָ�������������������...\n");
//			 netif_set_link_up(&xnetif);
		//	 DEBUG_PRINTF(">>�������óɹ�...OK\n");
			 status = 1;
			rt_kprintf("linkup\n");
//			if(task_exist!=1){ //������񲻴��ڣ�����
//			
//				tid1 = rt_thread_create("eth",eth_thread_entry,RT_NULL,1024,7, 5);
//				if (tid1 != RT_NULL)
//					rt_thread_startup(tid1);
//				task_exist=1;			
//				DEBUG_PRINTF(">>SERVER2������\n");
//			}
		}
		//������߱��ε��������Ѿ����ú��ˣ�����Ҫɾ������
		if(!link_status_global && status){		
			//����������ζϿ�״̬������Ϊ����
			if(counter++ >= 5)
			{
				counter = 0;
				rt_kprintf("linkdown\n");
		//	DEBUG_PRINTF(">>���߱��γ�������...\n");
//			netif_set_link_down(&xnetif); 
			status = 0;

//			if(task_exist==1){ //���������ڣ���ɾ��
//				vTaskDelete(xHandle_Server2);
//				rt_thread_delete(tid1);
//				
//				closesocket(socket_connect_modbus);
//				closesocket(sock_modbus);
//				task_exist=0;
//				flag_modbustcp = 0;
//				counter_modbustcp = 0;
//				DEBUG_PRINTF(">>SERVER2����ɾ��\n");
//				}
			}
		}	
		else
		{
			counter = 0;
		}
		
		
		
/*		
		if(flag_modbustcp==1) //˵���Ѿ��пͻ�����������
		{
			if(counter_modbustcp++ == 60)
			{
				if(task_exist==1){ //���������ڣ���ɾ��
					closesocket(sock_modbus);
					closesocket(socket_connect_modbus);
					//vTaskDelete(xHandle_Server2);
					rt_thread_delete(tid1);
					task_exist=0;
				//	DEBUG_PRINTF(">>SERVER2����ɾ��\n");
				}
			}
			if(counter_modbustcp == 65)
			{
				if(task_exist!=1){ //������񲻴��ڣ�����
					//xTaskCreate(ETH_AS_Server2_thread,"SERVER2",256,(void *)"TCPserver2 task start",3,&xHandle_Server2);
					tid1 = rt_thread_create("eth",eth_thread_entry,RT_NULL,1024,7, 5);
					if (tid1 != RT_NULL)
						rt_thread_startup(tid1);
					task_exist=1;			
					//DEBUG_PRINTF(">>SERVER2������\n");
					counter_modbustcp=0;
					flag_modbustcp=0;
				}	
			}
		}
*/
		rt_thread_delay(1000/ portTICK_RATE_MS);//��ʱ2000ms
	}
}

void handle_TCP_MODBUS(int socket)
{
	uint16_t i,serial_number,identifier,PDU_len,add,len_or_data;

	P_modbusTCP = (ModbusTCP_Struct *)Rev_Buf_modbus;
	P_modbusTCP_send = (ModbusTCP_Send_Struct *)Send_Buf_modbus;
	
	serial_number = ((uint16_t)(P_modbusTCP->serial_number_H)<<8)+P_modbusTCP->serial_number_L;
	identifier= ((uint16_t)(P_modbusTCP->identifier_H)<<8)+P_modbusTCP->identifier_L;
	PDU_len= ((uint16_t)(P_modbusTCP->PDU_len_H)<<8)+P_modbusTCP->PDU_len_L;
	add= ((uint16_t)(P_modbusTCP->add_H)<<8)+P_modbusTCP->add_L;
	len_or_data= ((uint16_t)(P_modbusTCP->len_or_data_H)<<8)+P_modbusTCP->len_or_data_L;
	
	if((identifier!=0) || (P_modbusTCP->station_add>10)) return; //�������modbus tcp ����վ�Ų��� ��վ����Ҫȷ��
	if((add>2000)|| (PDU_len>100))  return;

// ȷ�ϵ���Ƿ���Ҫת��  	
	if(P_modbusTCP->function_code==3)  //������
	{				
		if(len_or_data>128) return;
		
		P_modbusTCP_send->serial_number_L = P_modbusTCP->serial_number_L;
		P_modbusTCP_send->serial_number_H = P_modbusTCP->serial_number_H;
		P_modbusTCP_send->identifier_L = P_modbusTCP->identifier_L;
		P_modbusTCP_send->identifier_H = P_modbusTCP->identifier_H;
		P_modbusTCP_send->PDU_len_L = (len_or_data *2 + 3)&0xFF;
		P_modbusTCP_send->PDU_len_H = ((len_or_data *2 + 3)>>8)&0xFF;
		P_modbusTCP_send->station_add = P_modbusTCP->station_add;
		P_modbusTCP_send->function_code = P_modbusTCP->function_code;
		P_modbusTCP_send->quant = len_or_data *2;
		 
		//�ɶ�len_or_data������ֵ
		if((add>=100)&&((add+len_or_data)<=199)){				
			for(i=0;i<len_or_data;i++)				
			{
				P_modbusTCP_send->data[0+i*2] = (P_ttu_ems[i+add-100]>>8) &0xff; 
				P_modbusTCP_send->data[1+i*2] = P_ttu_ems[i+add-100] &0xff;		
			}
		}
		else if((add>=200)&&((add+len_or_data)<=300)){					
			for(i=0;i<len_or_data;i++)				
			{
				P_modbusTCP_send->data[0+i*2] = (P_ttu_pcs[i+add-200]>>8) &0xff; 
				P_modbusTCP_send->data[1+i*2] = P_ttu_pcs[i+add-200] &0xff;		
			}
		}
		else if((add>=301)&&((add+len_or_data)<=340)){					
			for(i=0;i<len_or_data;i++)				
			{
				P_modbusTCP_send->data[0+i*2] = (P_ttu_stac[i+add-301]>>8) &0xff; 
				P_modbusTCP_send->data[1+i*2] = P_ttu_stac[i+add-301] &0xff;		
			}
		}
		else if((add>=341)&&((add+len_or_data)<=390)){					
			for(i=0;i<len_or_data;i++)				
			{
				P_modbusTCP_send->data[0+i*2] = (P_ttu_pvac[i+add-341]>>8) &0xff; 
				P_modbusTCP_send->data[1+i*2] = P_ttu_pvac[i+add-341] &0xff;		
			}
		}
		else if((add>=411)&&((add+len_or_data)<=450)){					
			for(i=0;i<len_or_data;i++)				
			{
				P_modbusTCP_send->data[0+i*2] = (P_ttu_stbat[i+add-411]>>8) &0xff; 
				P_modbusTCP_send->data[1+i*2] = P_ttu_stbat[i+add-411] &0xff;		
			}
		}
		else if((add>=500)&&((add+len_or_data)<=550)){					
			for(i=0;i<len_or_data;i++)				
			{
				P_modbusTCP_send->data[0+i*2] = (P_ttu_meter[i+add-500]>>8) &0xff; 
				P_modbusTCP_send->data[1+i*2] = P_ttu_meter[i+add-500] &0xff;		
			}
		}
		else if((add>=551)&&((add+len_or_data)<=580)){					
			for(i=0;i<len_or_data;i++)				
			{
				P_modbusTCP_send->data[0+i*2] = (P_ttu_pvclass[i+add-551]>>8) &0xff; 
				P_modbusTCP_send->data[1+i*2] = P_ttu_pvclass[i+add-551] &0xff;		
			}
		}
		else if((add>=1000)&&((add+len_or_data)<=1030)){					
			for(i=0;i<len_or_data;i++)				
			{
				P_modbusTCP_send->data[0+i*2] = (P_ttu_charge[i+add-1000]>>8) &0xff; 
				P_modbusTCP_send->data[1+i*2] = P_ttu_charge[i+add-1000] &0xff;		
			}
		}
		else
			return;
		Send_modbusTCP_Len = len_or_data *2 +9;

		//write(socket,Send_Buf_modbus,Send_modbusTCP_Len); 
		
		send(socket,Send_Buf_modbus,Send_modbusTCP_Len,0);
	}
	else if(P_modbusTCP->function_code==6) //д����
	{	
		if((add>=100)&&(add<=1100)) {  //pcs par
		//	Data_TTU[add-100] = len_or_data;
		}
		else
			return;
		
		memcpy(P_modbusTCP_send,P_modbusTCP,12); //06ָ��̶�12�ֽ�
		Send_modbusTCP_Len = 12;
	
		//write(socket,Send_Buf_modbus,Send_modbusTCP_Len); 
		
		send(socket,Send_Buf_modbus,Send_modbusTCP_Len,0);
	}
	else if(P_modbusTCP->function_code==0x10) //����д����
	{	
		if((Rev_Buf_modbus[11]*2 != Rev_Buf_modbus[12] ) || (Rev_Buf_modbus[11]>50)) 
			return;
		
		/*if((add<=152)||(add>=150)){		
			for(i=0;i<(Rev_Buf_modbus[12]>>1);i++){
				P_ttu_ems[i+add-100] = ((Rev_Buf_modbus[i*2+13] <<8)&0xff00) +Rev_Buf_modbus[i*2+14] ;
			}	
		}
		*/
		if(add==152){	//���Բ����·����ܣ��������ŵ���ǰģʽ����ʾ	  20211130,�˲���Ϊ21�豸�Ĺ�����繦��
			DATA_ttu.DATA_TTU_ems.Battery_Fault= ((Rev_Buf_modbus[13] <<8)&0xff00) +Rev_Buf_modbus[14] ; //���ڷ�д���ں��նˣ�������Ϳ�
		
			PV_Power_21 =DATA_ttu.DATA_TTU_ems.Battery_Fault;//21�Ĺ������
			LEUER_Data.EMS_Data.DC_AC_Com_State =PV_Power_21;//����Һ����ʾ������鿴��Ҳ�ϴ������ں��ն�
			LEUER_Data.EMS_Data.P_A = ((Rev_Buf_modbus[i*2+13] <<8)&0xff00) +Rev_Buf_modbus[i*2+14] ; //ʵ��д��ֵ���ں��ն˻��д���ֵ��ȡ����ȷ����û���ã��ȱ���
		}
		else if((add<=300)&&(add>=270)){		
			for(i=0;i<(Rev_Buf_modbus[12]>>1);i++){
				P_ttu_pcs[i+add-200] =  ((Rev_Buf_modbus[i*2+13] <<8)&0xff00) +Rev_Buf_modbus[i*2+14] ;
			}	
		}
		else if((add<=340)&&(add>=321)){		
			for(i=0;i<(Rev_Buf_modbus[12]>>1);i++){
				P_ttu_stac[i+add-301] =  ((Rev_Buf_modbus[i*2+13] <<8)&0xff00) +Rev_Buf_modbus[i*2+14] ;
			}	
		}
		else if((add<=390)&&(add>=371)){		
			for(i=0;i<(Rev_Buf_modbus[12]>>1);i++){
				P_ttu_pvac[i+add-341] =  ((Rev_Buf_modbus[i*2+13] <<8)&0xff00) +Rev_Buf_modbus[i*2+14] ;
			}	
		}
		else if((add<=1023)&&(add>=1021)){		
			for(i=0;i<(Rev_Buf_modbus[12]>>1);i++){
				P_ttu_charge[i+add-1000] =  ((Rev_Buf_modbus[i*2+13] <<8)&0xff00) +Rev_Buf_modbus[i*2+14] ;
			}	
		}
		else
			return;
		memcpy(P_modbusTCP_send,P_modbusTCP,12); //10ָ��̶�12�ֽ�
		Send_modbusTCP_Len = 12;

		//write(socket,Send_Buf_modbus,Send_modbusTCP_Len); 
		send(socket,Send_Buf_modbus,Send_modbusTCP_Len,0);
	}	
}


void eth_thread_entry(void* parameter)
{
    static rt_device_t eth_device = RT_NULL;

	char *argv = "test";
	char *ip = "192.168.1.11";
	char *port = "502";	
	char *p[3];
	uint16_t cnt=0;
	extern struct tcp_pcb *tcp_active_pcbs;
	struct tcp_pcb *tcp_pcbs=NULL;
	
	p[0] = argv;
	p[1] = ip;
	p[2] = port;

    eth_device = rt_device_find("eth0");
    rt_device_open(eth_device, RT_DEVICE_OFLAG_RDWR);
//	tcpclient(3,p);
		
	rt_kprintf("%s\n",(char *)parameter);

	int recv_data_len;

	struct sockaddr_in server_addr,client_addr;
	socklen_t sin_size;
	char bDontLinger = FALSE;
	
//	rt_kprintf(10000/ portTICK_RATE_MS); //����ʱ���ȴ�socket��ȫ�ͷţ�����bind����
	__start:
	//tcp�� Э���塢���͡�Э�� ���� netconn_new_with_callback������ɽӿڳ�ʼ��
	sock_modbus = socket(AF_INET, SOCK_STREAM, 0);  
	if (sock_modbus < 0)
	{
		rt_kprintf("Socket error\n");
		goto __exit;
	}
	//��sockaddr_in�ṹ�屾������ sockaddr�ṹ��һ����ֻ�Ƕ���ĸ�����ϸ
	server_addr.sin_family = AF_INET; //Э����
	server_addr.sin_addr.s_addr = INADDR_ANY; //ip 
	server_addr.sin_port = htons(PROTOCLO_MODBUS_PORT); //�˿ں�   htons�����ߵ�8λ
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero)); //zeroΪ������8�ֽ�

	//���Ҫ�Ѿ���������״̬��soket�ڵ���closesocket()��ǿ�ƹرգ�������TIME_WAIT�Ĺ���,���Խ�����첻����
	bDontLinger = FALSE;
	setsockopt( sock_modbus, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( char ) );
	
	//�����close֮�����һ��WAIT_TIME������bindʧ�ܵ����� ,����û��Ч��
	int val = 1;
	setsockopt(sock_modbus,SOL_SOCKET,SO_REUSEADDR,(void *)&val,sizeof(int));
	/*���� server_addr ��һ��ָ�� sockaddr �ṹ���ָ�룬���а����������� IP ��ַ���˿ںŵ�
	��Ҫ����Ϣ��LwIP Ϊ�˸���������Щ��Ϣ��ʹ���� sockaddr �ṹ���������˱�Ҫ����Ϣ��
	�ֶΣ����������� Socket API �ĺܶຯ���У�������ʹ�� bind()��ʱ��ֻ��Ҫֱ����д
	����ֶμ��ɣ����� namelen ָ���� name �ṹ��ĳ��ȡ�*/
	if (bind(sock_modbus, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		rt_kprintf("Unable to bind\n");
	    goto __exit;
	}

	if (listen(sock_modbus, 2) == -1)  //�÷������������״̬���ȴ�Զ�˵���������LwIP �п��Խ��ն���ͻ��˵����ӣ���˲��� backlog ָ����������еĴ�С��
	{
		rt_kprintf("Listen error\n");
	    goto __exit;
	}

	while (1)
	{
		//vTaskSuspend(NULL);//���𣬷�ֹ������ʱ�ٴν���netconn_accept���˺����ڻὫTCP_conn_LINK1��NULL
		sin_size = sizeof(struct sockaddr_in);
		//�������ԣ�accept�Ƕ�����  socket_connect�������Ϊ�ǵ�ǰ�ĵڼ���client 
		socket_connect_modbus = accept(sock_modbus, (struct sockaddr *)&client_addr, &sin_size); //�ͻ�����Ϣ������client_addr��
		if(socket_connect_modbus!= -1){
			rt_kprintf("new client connected from (%s, %d)\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));//����ת��ΪIP�ַ���
		
			
			/* set option at TCP level *//* the cast is historical cruft *//* name of option *//* length of option value */
			//����socketѡ�connected��socket���ơ�IPPROTO_TCP��һ��ѡ�TCP_NODELAY��ѡ��IPPROTO_TCP�ڲ���һ����ѡ��ǲ�ʹ�� Nagle �㷨����˼ ��
			bDontLinger = TRUE;
			setsockopt(socket_connect_modbus,IPPROTO_TCP, TCP_NODELAY, ( const char* )&bDontLinger, sizeof( char ));			
		}
		
		while (1)
		{
			flag_modbustcp=1;
			//���ͻ��������Ͽ�����ʱ��recvҲ����������//���ӶϿ�����0�����󷵻�-1
			recv_data_len = recv(socket_connect_modbus, Rev_Buf_modbus, TCP_BUFSIZE, 0x08);
			 
			//��⵽����ֱ����������close������
			if(link_status_global==0)//����
				break;
			
			//��������������ղ���������������break�����ԡ�
			if (recv_data_len < 0)  // û����
			{
				rt_thread_delay(100/ portTICK_RATE_MS);
				if(cnt++ >= 300)
				{
					//����30��û�����ݣ��ж�Ϊ������ ��
					cnt=0;
					recv_data_len = 0;
			//		break;
				}
				continue;
			}
			 //�ֶ��Ͽ�����  �����޷����
			else if (recv_data_len == 0) 
				break;
			
			cnt = 0;
			#if(1)
				rt_kprintf("recv %d len data\n",recv_data_len);
				//write(socket_connect_modbus,Rev_Buf_modbus,recv_data_len);  //���ԣ����ڲ���
			//	send(socket_connect_modbus,Rev_Buf_modbus,recv_data_len,0);
				handle_TCP_MODBUS(socket_connect_modbus);
			#endif
			counter_modbustcp=0;
			//���2����û���뵽���˵����������û�����ݷ��͹�����˵�����������ӣ�Ҫ�������ط�closesocket.
		}
		if (socket_connect_modbus >= 0){
			closesocket(socket_connect_modbus);
		}
		socket_connect_modbus = -1;
		rt_kprintf("delete tcp_pcb\n");
		tcp_pcbs = tcp_active_pcbs;		

		//ֱ�Ӷ�pcb����ɾ���������൱�ڶ�tcp��ֱ�Ӳ������Ƚϴ�ĸ���assert����
//		tcp_abandon(tcp_active_pcbs,1);
//		while(tcp_pcbs)
//		{
//			tcp_abandon(tcp_pcbs,1);
//			tcp_pcbs = tcp_pcbs->next;
//		}
			
		
	//	break;
	}
	__exit:
	if (sock_modbus >= 0){
		closesocket(sock_modbus); 
	}
	rt_thread_delay(2000/ portTICK_RATE_MS);
	
	goto __start;

}
