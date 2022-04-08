/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "LE_uart.h"
#include "LE_can.h"
#include "RX8025T.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

//20210422新增
#define PCS_RUN_SUCCESS1 0x0C    //PCS 运行时状态 C 恒流运行
#define PCS_RUN_SUCCESS2 0x0B    //PCS 运行时状态 B 恒压运行
#define PCS_RUN_ADD  3110 //PCS 块开关机地址
#define PCS_RUN_COMMAND 1   //PCS 模块开机命令
#define PCS_STOP_COMMAND 0   //PCS 模块关机命令
#define PCS_MOD_ADD 3100
#define PCS_FAULT_CLEAR_ADD 3111
#define PCS_POW_ADD 3102  //功率设置

//PCS电网侧 ABC线电压判断：在310-450内 正常，为1 。 频率：在49-51内 为1
#define PCS_VA_CHECK ( ((PCSdata.sys_u[0]>250000) ? 0:1 ) && ((PCSdata.sys_u[0]<190000) ? 0:1 ) ) 
#define PCS_VB_CHECK ( ((PCSdata.sys_u[1]>250000) ? 0:1 ) && ((PCSdata.sys_u[1]<190000) ? 0:1 ) )
#define PCS_VC_CHECK ( ((PCSdata.sys_u[2]>250000) ? 0:1 ) && ((PCSdata.sys_u[2]<190000) ? 0:1 ) )
//#define PCS_F_CHECK  ( ((WH_PCSdata.Sys_Freq>5100) ? 0:1 ) && ((WH_PCSdata.Sys_Freq<4900) ? 0:1 ) )  //PCS电网侧频率
#define PCS_SYSTEM_CHECK 	(PCS_VA_CHECK && PCS_VB_CHECK && PCS_VC_CHECK) //&& PCS_F_CHECK)   //PCS电网侧电压  正常是1；非正常是0，


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


#define DC_RUN_SUCCESS 0x3F01   //DC模块运行成功的状态
#define DC_RUN_COMMAND 1   //DC模块开机命令
#define DC_STOP_COMMAND 2   //DC模块关机命令

#define DC2_MOD_ADD  2206 //DC 模块模式地址
#define DC2_RUN_ADD  2215 //DC 模块开关机地址

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
extern uint16_t PV_Power_21;//21号设备的光伏发电功率
/* Private function prototypes -----------------------------------------------*/
static void ShutDown(void);
static void StartUp(void);
static void auto_start_stop(void);
static uint16_t fault_check(void);
/* Private functions ---------------------------------------------------------*/

/**********************************
**	函数名称：ShutDown
**  函数功能: //关机，在收到关机命令时或者严重故障时
     如果需要及时相应关机指令，比如在启动DC模块是马上关机，可以将关机单独列一个优先级较高的任务，
	 收到关机指令后task delete 开机任务 再执行关机，执行完毕后在新建开机任务 。
***********************************/
static void ShutDown(void)
{
    uint16_t time=0;
	//为防止因为某个模块无法关机，导致其它模块也没办法执行关机，所以关机指令一起执行。
	do{
		rt_kprintf("关机执行中..\n");
		//send_uart3(MODULE_DCAC,DC_AC_RUN_ADD,0); //设置DC-AC关机
		send_uart3(MODULE_PCS,PCS_RUN_ADD,PCS_STOP_COMMAND,MODBUS_WRITE); //设置PCS模块关机
		rt_thread_delay( 2000 / portTICK_RATE_MS );
		send_uart3(MODULE_DC2,DC2_RUN_ADD,DC_STOP_COMMAND,NULL); //设置DC2模块关机
		rt_thread_delay( 2000 / portTICK_RATE_MS );
		if(time++ ==10){ //如果次数大于10次
			LEUER_Data.EMS_Data.Status_FLAG = 7;
			rt_kprintf("关机失败\n");
			break;
		}				
	}while((PCSdata.state == PCS_RUN_SUCCESS1)|| (PCSdata.state == PCS_RUN_SUCCESS2)||(DC2data.state5 == DC_RUN_SUCCESS ));
		
	LEUER_Data.EMS_Data.Run_FLAG = 0;//关机标志
	//vTaskSuspend(xHandle_EMS);

}
/**********************************
**	函数名称：StartUp
**  函数功能: //开机，在控制板启动时或者收到开机命令时
    20210918 优化开机
      1. 当检测交流电压和通讯故障时不会清除开机指令，会一直尝试开机
      2. 如果是设备无法开启的故障，为了安全，会清除开机指令。要想再次开机只能重启控制板。
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
	

	//检测PCS网侧电压频率是否正常,当前只考虑了并网情况下
	do{
		rt_kprintf("PCS进线交流电压检测中..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 10;
		if(PCS_SYSTEM_CHECK ==0 ){
			LEUER_Data.EMS_Data.V_Error_FLAG = 1; 
		}
		else{
			LEUER_Data.EMS_Data.V_Error_FLAG = 0;
		}
		rt_thread_delay( 2000 / portTICK_RATE_MS );	
		if(time++ ==10){ //如果次数大于10次，认为故障 ，且跳出,并执行关机
			LEUER_Data.EMS_Data.V_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			//LEUER_Data.EMS_Data.Run = 0;  进线电压没有时会一直检测 并尝试开机
			return;      //直接退出，下次任务还会尝试检测
		}		
	}while(LEUER_Data.EMS_Data.V_Error_FLAG); 
	
	//可能是液晶屏操作关机，所以要加上关机后推出此流程。
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //如果关机马上退出此函数
	
	//检测通讯,当前检测  pcs 
	do{
		rt_kprintf("通讯检测中..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 11;
		//if((LEUER_Data.EMS_Data.BAT_Com_State == FAIL) || (LEUER_Data.EMS_Data.PCS_Com_State == FAIL))	
		if(LEUER_Data.EMS_Data.PCS_Com_State == FAIL)			
			LEUER_Data.EMS_Data.COM_Error_FLAG = 1;//通讯状态错误
		else
			LEUER_Data.EMS_Data.COM_Error_FLAG = 0;
		
		rt_thread_delay( 2000 / portTICK_RATE_MS );
		if(time++ ==10){ //如果次数大于10次，认为通讯故障 ，且跳出,并执行关机
			LEUER_Data.EMS_Data.COM_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			//LEUER_Data.EMS_Data.Run = 0; 通讯没有时会一直检测 并尝试开机
			return;
		}
	}while(LEUER_Data.EMS_Data.COM_Error_FLAG );
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //如果关机马上退出此函数

	//PCS故障清除
	do{
		rt_kprintf("PCS故障清除中..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 14;

		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_PCS,PCS_FAULT_CLEAR_ADD,1,MODBUS_WRITE); //故障清除

		rt_thread_delay( 5000 / portTICK_RATE_MS ); //储能模块开机
		if(time++ ==10){ //如果次数大于10次，认为故障 ，且跳出,并执行关机
			LEUER_Data.EMS_Data.PCS_Fault = 1;
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run = 0;
			break;
		}
	}while(PCSdata.fault != 0);
	
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //如果关机马上退出此函数
	
	//启动PCS开机
	do{
		rt_kprintf("pma模块开机中..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 15;
		send_uart3(MODULE_PCS,PCS_MOD_ADD,0x02,MODBUS_WRITE10);  //设置PCS模块运行模式为0X02 恒压模式.
		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_PCS,PCS_POW_ADD,100000,MODBUS_WRITE10);  //设置PCS 电池口电压100V
		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_PCS,PCS_FAULT_CLEAR_ADD,1,MODBUS_WRITE); //故障清除
		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_PCS,PCS_RUN_ADD,PCS_RUN_COMMAND,MODBUS_WRITE); //设置PCS模块开机运行
		rt_thread_delay( 8000 / portTICK_RATE_MS ); //模块开机
		if(time++ ==10){ //如果次数大于10次，认为故障 ，且跳出,并执行关机
			LEUER_Data.EMS_Data.PCS_Fault = 1;
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run = 0;
			break;
		}
	}while((PCSdata.state != PCS_RUN_SUCCESS1) && (PCSdata.state != PCS_RUN_SUCCESS2));
	
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //如果关机马上退出此函数
	
	//检测模块输出母线电压连续10S在xx和xx之间，增加超过多长时间报故障，且关机
	do{
		rt_kprintf("PCS模块VDC母线电压检测中..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 16;
		counter=0;
		for(i=0;i<10;i++){
			if((PCSdata.VDC >6500 ) && (PCSdata.VDC <8000 )){	 
				counter++;	
			}
			rt_thread_delay( 1000 / portTICK_RATE_MS );
		}
		if(counter<10){
			LEUER_Data.EMS_Data.VDC_Error_FLAG = 1;//VDC状态错误
			LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			LEUER_Data.EMS_Data.Run = 0;
			break;
		}
		else
			LEUER_Data.EMS_Data.VDC_Error_FLAG = 0;
	}while(LEUER_Data.EMS_Data.VDC_Error_FLAG );
	
	
	//启动光伏模块开机  模式为7光伏模式，后边拨码开关 bit1-5  00010 光伏输入：端口2  输出口：端口1 
	//光伏输入正常电压为400-850V ，具有最大功率跟踪功能。
	do{
		rt_kprintf("光伏DC模块开机中..\n");
		LEUER_Data.EMS_Data.Status_FLAG = 18;
		send_uart3(MODULE_DC2,DC2_MOD_ADD,7,NULL);  //设置DC2模块运行模式为7.
		rt_thread_delay( 1000 / portTICK_RATE_MS );
		send_uart3(MODULE_DC2,DC2_RUN_ADD,DC_RUN_COMMAND,NULL); //设置DC2模块开机运行
		rt_thread_delay( 5000 / portTICK_RATE_MS ); //储能模块开机
		if(time++ ==10){ //如果次数大于10次，认为DC2故障 ，且跳出,并执行关机
			//LEUER_Data.EMS_Data.DC_GuangFu_Fault = 1;  //20211019 因为晚上光伏无法开机，所以流程走到这里会报错并关机。屏蔽
			//LEUER_Data.EMS_Data.Run_Error_FLAG = 1;
			//LEUER_Data.EMS_Data.Run = 0;
			break;
		}
	}while(DC2data.state5 != DC_RUN_SUCCESS );//开机成功显示3F01，点表没有写具体意义		
		
		
	time = 0;
	if(LEUER_Data.EMS_Data.Run != 0xAAAA) 
		return; //如果关机马上退出此函数
	
	LEUER_Data.EMS_Data.Run_FLAG = 1;//开机标志
	LEUER_Data.EMS_Data.Status_FLAG = 5;
	LEUER_Data.EMS_Data.Run_Error_FLAG = 0;
	rt_kprintf("开机执行完毕\n");
			
	//vTaskResume(xHandle_EMS);
}

/**********************************************************************************************************
**	函数名称：fault_check
**  函数功能：循环检查设备故障
************************************************************************************************************/
static uint16_t fault_check(void)
{		
	//PCS模块故障检测
	if(PCSdata.fault!=0){
		LEUER_Data.EMS_Data.PCS_Fault = 1;
		rt_kprintf("PCS故障并关机\n");
		LEUER_Data.EMS_Data.Status_FLAG = 21;
		return 1;
	}
	else
		LEUER_Data.EMS_Data.PCS_Fault = 0;
		
	//保护   直流母线电压大于xxV
	if((PCSdata.VDC >8000 )||(PCSdata.VDC <6000 )){
		LEUER_Data.EMS_Data.VDC_Error_FLAG = 1;//VDC状态错误
		rt_kprintf("VDC报警并关机\n");
		LEUER_Data.EMS_Data.Status_FLAG = 23;
		return 1;
	}
	else
		LEUER_Data.EMS_Data.VDC_Error_FLAG = 0;
	return 0;
}

/**********************************************************************************************************
**	函数名称：auto_start_stop
**  函数功能：检测并执行一键开机关机
************************************************************************************************************/
static void auto_start_stop(void)
{	
	if((LEUER_Data.EMS_Data.Run_FLAG == 0)&&(LEUER_Data.EMS_Data.Run == 0xAAAA)){//如果没有开机，则执行startup,如果是开机后看门狗重启，则正常执行开机流程
		StartUp();	
		if(LEUER_Data.EMS_Data.Run_FLAG == 0){
			rt_kprintf("开机失败或中途退出\n");
		}
	}
	else if(LEUER_Data.EMS_Data.Run != 0xAAAA){//关机，如果检测到任何一个模块在开启状态则执行关机。
		if((PCSdata.state == PCS_RUN_SUCCESS1)|| (PCSdata.state == PCS_RUN_SUCCESS2)){  //PCS运行成功返回值待确认
			ShutDown();
			LEUER_Data.EMS_Data.Status_FLAG = 6;	
			rt_kprintf("关机成功\n");
		}
	}	
}

/**********************************************************************************************************
**	函数名称：EMS_thread_XA(void *s)
**  函数功能：每s执行一次程序，内部离并网子程序每5秒执行一次，开关机指令和对PCS离并网操作指令为即时
    发送，只要有命令就发送；离并网子程序的模式改变通过对比函数发送，会有一个5秒的等待间隔
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
		if(LEUER_Data.EMS_Data.LEUER_Mod == 0x0A){ //如果设置自动模式
			auto_start_stop();
		}	
		else{ //手动模式，手动操作各个模块开启
		}
		//只有自动模式Run_FLAG才会为1.所以手动模式不会故障后主动开机。
		if(LEUER_Data.EMS_Data.Run_FLAG == 1){ //完整执行了开机流程，说明已经开机，没有关机，或者是开机后的故障状态。
			if(fault_check()){ //发生故障
				ShutDown(); //直接执行关机
				
				//清除开机流程，这句在shutdown函数内已执行过，为清晰流程，再次执行。
				//执行完后满足将开机触发条件。在下次任务中开机。
				LEUER_Data.EMS_Data.Run_FLAG = 0;
				
			}
			
			//20211028 新发来光伏模块是老程序，不会自动开机，当检测到开机时，并且开口电压大于650*0.9V时，下发开机指令
			
			if((timePC[3]<17) && (timePC[3]>=4)){  //在4-18点时才会主动开机
				if(DC2data.VDC2 > 6000){  //实际运行时还会有590V的情况，所以改成600V.
					send_uart3(MODULE_DC2,DC2_MOD_ADD,7,NULL);  //设置DC2模块运行模式为7.
					rt_thread_delay( 1000 / portTICK_RATE_MS );
					send_uart3(MODULE_DC2,DC2_RUN_ADD,DC_RUN_COMMAND,NULL); //设置DC2模块开机运行
				}
			}
			else if((timePC[3]>=18) || (timePC[3]<4)){  //在18点到凌晨4点时，如果检测到DC仍在开机，则发关机
				if(DC2data.VDC2 > 5000){  //
					send_uart3(MODULE_DC2,DC2_RUN_ADD,DC_STOP_COMMAND,NULL); 
				}	
			}
			
			//隔离DC-DC固定时间开关机
			if((timePC[3]==6) && (timePC[4]==01)){
				TRD_MD950_Value.power_drection = 2;
				TRD_MD950_Value.v_portA = 780;
				TRD_MD950_Value.v_portB = 720;
				TRD_MD950_Value.p_dalta = Power_MD950; 
				TRD_MD950_Value.run = 0; //开机
				
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
				TRD_MD950_Value.run = 1; //关机
				send_can2_MD950( 5000, 1);
				
				Power_MD950=0;
				Power_PV21=0; //如果长时间不更新，关机时确保赋值0一次。
			}
			
			
			//计算当前发电功率
			Power_PV21 = abs(6553-PV_Power_21); //单位1kw
			//增加根据PV_Power_21 计算的程序  且send_can2_MD950内3已经修改为了以0.1kw为单位
			Power_MD950 = Power_PV21*10/3; //计算每个模块的输出功率，0.1kw   13表示1.3kw
			if(Power_MD950>70) Power_MD950 = 0; //每台大于7kw 说明是刚开机或者异常
			
			LEUER_Data.EMS_Data.DC1_Com_State = Power_MD950*3; //用于将实际发电功率发到融合终端 单位0.1kw  因为传过来的整数损失了0.-0.9kw的功率
			if(LEUER_Data.EMS_Data.DC1_Com_State<5) LEUER_Data.EMS_Data.DC1_Com_State=0; 
			
			if(Power_MD950!=lastPower_MD950){ //数据有变化就发送
				TRD_MD950_Value.p_dalta = Power_MD950; 
				send_can2_MD950( 5006, Power_MD950);
				
			}			
			lastPower_MD950 = Power_MD950;
		}	
	}		
}
