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
uint16_t PV_Power_21;//21号设备的光伏发电功率

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
	static uint8_t task_exist=0;//记录任务是否存在标志位
	static uint8_t counter = 0;
	rt_thread_t tid1 = RT_NULL;
//	DEBUG_PRINTF("%s\n",(char *)parameter);
	for(;;)
	{		

		if(link_status_global && !status){
		//	 DEBUG_PRINTF(">>网线恢复正常，正在配置网卡...\n");
//			 netif_set_link_up(&xnetif);
		//	 DEBUG_PRINTF(">>网卡配置成功...OK\n");
			 status = 1;
			rt_kprintf("linkup\n");
//			if(task_exist!=1){ //如果任务不存在，则建立
//			
//				tid1 = rt_thread_create("eth",eth_thread_entry,RT_NULL,1024,7, 5);
//				if (tid1 != RT_NULL)
//					rt_thread_startup(tid1);
//				task_exist=1;			
//				DEBUG_PRINTF(">>SERVER2任务建立\n");
//			}
		}
		//如果网线被拔掉，并且已经配置好了，就需要删除网卡
		if(!link_status_global && status){		
			//检测连续三次断开状态，才认为断线
			if(counter++ >= 5)
			{
				counter = 0;
				rt_kprintf("linkdown\n");
		//	DEBUG_PRINTF(">>网线被拔出，请检查...\n");
//			netif_set_link_down(&xnetif); 
			status = 0;

//			if(task_exist==1){ //如果任务存在，则删除
//				vTaskDelete(xHandle_Server2);
//				rt_thread_delete(tid1);
//				
//				closesocket(socket_connect_modbus);
//				closesocket(sock_modbus);
//				task_exist=0;
//				flag_modbustcp = 0;
//				counter_modbustcp = 0;
//				DEBUG_PRINTF(">>SERVER2任务删除\n");
//				}
			}
		}	
		else
		{
			counter = 0;
		}
		
		
		
/*		
		if(flag_modbustcp==1) //说明已经有客户端连接上了
		{
			if(counter_modbustcp++ == 60)
			{
				if(task_exist==1){ //如果任务存在，则删除
					closesocket(sock_modbus);
					closesocket(socket_connect_modbus);
					//vTaskDelete(xHandle_Server2);
					rt_thread_delete(tid1);
					task_exist=0;
				//	DEBUG_PRINTF(">>SERVER2任务删除\n");
				}
			}
			if(counter_modbustcp == 65)
			{
				if(task_exist!=1){ //如果任务不存在，则建立
					//xTaskCreate(ETH_AS_Server2_thread,"SERVER2",256,(void *)"TCPserver2 task start",3,&xHandle_Server2);
					tid1 = rt_thread_create("eth",eth_thread_entry,RT_NULL,1024,7, 5);
					if (tid1 != RT_NULL)
						rt_thread_startup(tid1);
					task_exist=1;			
					//DEBUG_PRINTF(">>SERVER2任务建立\n");
					counter_modbustcp=0;
					flag_modbustcp=0;
				}	
			}
		}
*/
		rt_thread_delay(1000/ portTICK_RATE_MS);//延时2000ms
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
	
	if((identifier!=0) || (P_modbusTCP->station_add>10)) return; //如果不是modbus tcp 或者站号不对 ，站号需要确认
	if((add>2000)|| (PDU_len>100))  return;

// 确认点表是否需要转换  	
	if(P_modbusTCP->function_code==3)  //读命令
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
		 
		//可对len_or_data增加限值
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
	else if(P_modbusTCP->function_code==6) //写命令
	{	
		if((add>=100)&&(add<=1100)) {  //pcs par
		//	Data_TTU[add-100] = len_or_data;
		}
		else
			return;
		
		memcpy(P_modbusTCP_send,P_modbusTCP,12); //06指令固定12字节
		Send_modbusTCP_Len = 12;
	
		//write(socket,Send_Buf_modbus,Send_modbusTCP_Len); 
		
		send(socket,Send_Buf_modbus,Send_modbusTCP_Len,0);
	}
	else if(P_modbusTCP->function_code==0x10) //连续写命令
	{	
		if((Rev_Buf_modbus[11]*2 != Rev_Buf_modbus[12] ) || (Rev_Buf_modbus[11]>50)) 
			return;
		
		/*if((add<=152)||(add>=150)){		
			for(i=0;i<(Rev_Buf_modbus[12]>>1);i++){
				P_ttu_ems[i+add-100] = ((Rev_Buf_modbus[i*2+13] <<8)&0xff00) +Rev_Buf_modbus[i*2+14] ;
			}	
		}
		*/
		if(add==152){	//测试参数下发功能，将参数放到当前模式来显示	  20211130,此参数为21设备的光伏发电功率
			DATA_ttu.DATA_TTU_ems.Battery_Fault= ((Rev_Buf_modbus[13] <<8)&0xff00) +Rev_Buf_modbus[14] ; //用于反写回融合终端，方便润和看
		
			PV_Power_21 =DATA_ttu.DATA_TTU_ems.Battery_Fault;//21的光伏功率
			LEUER_Data.EMS_Data.DC_AC_Com_State =PV_Power_21;//用于液晶显示，方便查看，也上传到了融合终端
			LEUER_Data.EMS_Data.P_A = ((Rev_Buf_modbus[i*2+13] <<8)&0xff00) +Rev_Buf_modbus[i*2+14] ; //实际写入值，融合终端会把写入的值读取，不确定有没有用，先保留
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
		memcpy(P_modbusTCP_send,P_modbusTCP,12); //10指令固定12字节
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
	
//	rt_kprintf(10000/ portTICK_RATE_MS); //加延时，等待socket完全释放，否则bind错误
	__start:
	//tcp的 协议族、类型、协议 调用 netconn_new_with_callback函数完成接口初始化
	sock_modbus = socket(AF_INET, SOCK_STREAM, 0);  
	if (sock_modbus < 0)
	{
		rt_kprintf("Socket error\n");
		goto __exit;
	}
	//—sockaddr_in结构体本质上与 sockaddr结构体一样，只是定义的更加详细
	server_addr.sin_family = AF_INET; //协议族
	server_addr.sin_addr.s_addr = INADDR_ANY; //ip 
	server_addr.sin_port = htons(PROTOCLO_MODBUS_PORT); //端口号   htons交换高低8位
	memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero)); //zero为保留的8字节

	//如果要已经处于连接状态的soket在调用closesocket()后强制关闭，不经历TIME_WAIT的过程,测试结果差异不明显
	bDontLinger = FALSE;
	setsockopt( sock_modbus, SOL_SOCKET, SO_DONTLINGER, ( const char* )&bDontLinger, sizeof( char ) );
	
	//解决在close之后会有一个WAIT_TIME，导致bind失败的问题 ,测试没有效果
	int val = 1;
	setsockopt(sock_modbus,SOL_SOCKET,SO_REUSEADDR,(void *)&val,sizeof(int));
	/*参数 server_addr 是一个指向 sockaddr 结构体的指针，其中包含了网卡的 IP 地址、端口号等
	重要的信息，LwIP 为了更好描述这些信息，使用了 sockaddr 结构体来定义了必要的信息的
	字段，它常被用于 Socket API 的很多函数中，我们在使用 bind()的时候，只需要直接填写
	相关字段即可，参数 namelen 指定了 name 结构体的长度。*/
	if (bind(sock_modbus, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
	{
		rt_kprintf("Unable to bind\n");
	    goto __exit;
	}

	if (listen(sock_modbus, 2) == -1)  //让服务器进入监听状态，等待远端的连接请求，LwIP 中可以接收多个客户端的连接，因此参数 backlog 指定了请求队列的大小，
	{
		rt_kprintf("Listen error\n");
	    goto __exit;
	}

	while (1)
	{
		//vTaskSuspend(NULL);//挂起，防止有连接时再次进入netconn_accept，此函数内会将TCP_conn_LINK1置NULL
		sin_size = sizeof(struct sockaddr_in);
		//经过测试，accept是堵塞的  socket_connect可以理解为是当前的第几个client 
		socket_connect_modbus = accept(sock_modbus, (struct sockaddr *)&client_addr, &sin_size); //客户端信息保存在client_addr中
		if(socket_connect_modbus!= -1){
			rt_kprintf("new client connected from (%s, %d)\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));//函数转换为IP字符串
		
			
			/* set option at TCP level *//* the cast is historical cruft *//* name of option *//* length of option value */
			//设置socket选项。connected：socket名称。IPPROTO_TCP是一个选项，TCP_NODELAY是选项IPPROTO_TCP内部的一个子选项（是不使用 Nagle 算法的意思 ）
			bDontLinger = TRUE;
			setsockopt(socket_connect_modbus,IPPROTO_TCP, TCP_NODELAY, ( const char* )&bDontLinger, sizeof( char ));			
		}
		
		while (1)
		{
			flag_modbustcp=1;
			//当客户端主动断开连接时，recv也会解除阻塞，//连接断开返回0，错误返回-1
			recv_data_len = recv(socket_connect_modbus, Rev_Buf_modbus, TCP_BUFSIZE, 0x08);
			 
			//检测到断线直接跳出进行close操作。
			if(link_status_global==0)//断线
				break;
			
			//如果发生有连接收不到数据情况，这个break打开试试。
			if (recv_data_len < 0)  // 没数据
			{
				rt_thread_delay(100/ portTICK_RATE_MS);
				if(cnt++ >= 300)
				{
					//连续30秒没有数据，判定为断线了 。
					cnt=0;
					recv_data_len = 0;
			//		break;
				}
				continue;
			}
			 //手动断开连接  断线无法检测
			else if (recv_data_len == 0) 
				break;
			
			cnt = 0;
			#if(1)
				rt_kprintf("recv %d len data\n",recv_data_len);
				//write(socket_connect_modbus,Rev_Buf_modbus,recv_data_len);  //回显，用于测试
			//	send(socket_connect_modbus,Rev_Buf_modbus,recv_data_len,0);
				handle_TCP_MODBUS(socket_connect_modbus);
			#endif
			counter_modbustcp=0;
			//如果2分钟没进入到这里，说明连接上了没有数据发送过来，说明有其他连接，要在其它地方closesocket.
		}
		if (socket_connect_modbus >= 0){
			closesocket(socket_connect_modbus);
		}
		socket_connect_modbus = -1;
		rt_kprintf("delete tcp_pcb\n");
		tcp_pcbs = tcp_active_pcbs;		

		//直接对pcb进行删除操作，相当于对tcp层直接操作，比较大的概率assert错误
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
