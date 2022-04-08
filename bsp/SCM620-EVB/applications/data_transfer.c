/* Includes ------------------------------------------------------------------*/
//#include "main.h"
#include "board.h"
#include "eth.h"
#include "LE_data_transfer.h"
#include "LE_uart.h"
#include "RX8025T.h"
#include "LE_uart.h"
#include <math.h>
/** @addtogroup Template_Project
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BIT(x) (0x01<<x)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct DATA_TTU DATA_ttu;
float PVAC_Power[3]; //电能，在开机初始化时从flash读出，在rtc任务中保存。
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  更新EMS数据
  * @param  None
  * @retval None
  */
void updataEMS(void)
{
	DATA_ttu.DATA_TTU_ems.PCS_Fault = LEUER_Data.EMS_Data.PCS_Fault;//100	R	PCS模块故障	UINT16	
	DATA_ttu.DATA_TTU_ems.DCStore_Fault = LEUER_Data.EMS_Data.DC_ChuNeng_Fault;//101	R	DC储能模块故障	UINT16		
		
	DATA_ttu.DATA_TTU_ems.DCPV_Fault =LEUER_Data.EMS_Data.DC_GuangFu_Fault ;//102	R	DC光伏模块故障	UINT16		系统监测故障	DCPV_Fault
	DATA_ttu.DATA_TTU_ems.DCAC_Fault=LEUER_Data.EMS_Data.DC_AC_Fault ;//103	R	DCAC模块故障	UINT16		系统监测故障	DCAC_Fault
//	DATA_ttu.DATA_TTU_ems.Battery_Fault=LEUER_Data.EMS_Data.BAT_Error_FLAG ;//104	R	电池故障	UINT16		系统监测故障	Battery_Fault
	DATA_ttu.DATA_TTU_ems.AutoModeOn_Excep=LEUER_Data.EMS_Data.Run_Error_FLAG ;//105	R	自动模式开机异常标志	UINT16		70 ：FAULT/FAIL 80：PASS	AutoModeOn_Excep
	DATA_ttu.DATA_TTU_ems.SysVSVolt_Excep=LEUER_Data.EMS_Data.V_Error_FLAG ;//106	R	系统VS电压异常标志	UINT16		70 ：FAULT/FAIL 80：PASS	SysVSVolt_Excep
	DATA_ttu.DATA_TTU_ems.BusVDCVolt_Excep=LEUER_Data.EMS_Data.VDC_Error_FLAG ;//107	R	母线VDC电压异常标志	UINT16		70 ：FAULT/FAIL 80：PASS	BusVDCVolt_Excep
	DATA_ttu.DATA_TTU_ems.MasterDevCommu_Err=LEUER_Data.EMS_Data.COM_Error_FLAG ;//108	R	整体通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	MasterDevCommu_Err
	DATA_ttu.DATA_TTU_ems.PCSCommu_Err=LEUER_Data.EMS_Data.PCS_Com_State ;//109	R	PCS通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	PCSCommu_Err
	DATA_ttu.DATA_TTU_ems.DC1Commu_Err=LEUER_Data.EMS_Data.DC1_Com_State ;//110	R	DC1通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	DC1Commu_Err
	DATA_ttu.DATA_TTU_ems.DC2Commu_Err=LEUER_Data.EMS_Data.DC2_Com_State ;//111	R	DC2通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	DC2Commu_Err
	DATA_ttu.DATA_TTU_ems.BatCommu_Err=LEUER_Data.EMS_Data.BAT_Com_State ;//112	R	电池通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	BatCommu_Err
	DATA_ttu.DATA_TTU_ems.DCACCommu_Err=LEUER_Data.EMS_Data.DC_AC_Com_State ;//113	R	DCAC通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	DCACCommu_Err
	//	DATA_ttu.DATA_TTU_ems.ChargingPileCommu_Err=LEUER_Data.EMS_Data. ;//114	R	充电桩通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	ChargingPileCommu_Err
						
	DATA_ttu.DATA_TTU_ems.AutoModeOn_Status=LEUER_Data.EMS_Data.Run_FLAG ;//120	R	自动模式开机状态	UINT16		1为开机  0为关机	AutoModeOn_Status
	DATA_ttu.DATA_TTU_ems.CurRunMode=LEUER_Data.EMS_Data.Mod_FLAG;//121	R	当前运行模式	UINT16			CurRunMode

	DATA_ttu.DATA_TTU_ems.EMS_Mode_Set = LEUER_Data.EMS_Data.LEUER_Mod ;//150	R/W	自动/手动模式设置	UINT16		A：自动开关机模式 其它：手动模式	EMS_Mode_Set
	DATA_ttu.DATA_TTU_ems.EMS_OnOff_Set = LEUER_Data.EMS_Data.Run ;//151	R/W	开关机设置	UINT16		AAAA开机，其它关机	EMS_OnOff_Set
	DATA_ttu.DATA_TTU_ems.EMS_OffConnectGrid_Set = LEUER_Data.EMS_Data.P_A ;//152	R/W	离并网设置	UINT16		手动进行离并网控制，PCS应该具备这个功能	EMS_OffConnectGrid_Set
}

/****************************************************************
	更新PCS数据到 融合终端通讯的数据表
*****************************************************************/	
void updataPCS(void)
{
	uint8_t temp;
	uint32_t temp2;
	//PCS
	
	DATA_ttu.DATA_TTU_pcs.UAB= (uint16_t)(PCSdata.sys_u[0]*1.732/100);//200	R	电网AB线电压	UINT16	0.1V		UAB
	DATA_ttu.DATA_TTU_pcs.UBC= (uint16_t)(PCSdata.sys_u[1]*1.732/100);//201	R	电网BC线电压	UINT16	0.1V		UBC
	DATA_ttu.DATA_TTU_pcs.UAC= (uint16_t)(PCSdata.sys_u[2]*1.732/100);//202	R	电网AC线电压	UINT16	0.1V		UAC
	
	DATA_ttu.DATA_TTU_pcs.PhV_phsA= (uint16_t)(PCSdata.sys_u[0]/100);//;//203	R	电网A相电压	UINT16	0.1V		PhV_phsA
	DATA_ttu.DATA_TTU_pcs.PhV_phsB= (uint16_t)(PCSdata.sys_u[1]/100);//;//204	R	电网B相电压	UINT16	0.1V		PhV_phsB
	DATA_ttu.DATA_TTU_pcs.PhV_phsC= (uint16_t)(PCSdata.sys_u[2]/100);//;//205	R	电网C相电压	UINT16	0.1V		PhV_phsC
//	DATA_ttu.DATA_TTU_pcs.PhV_PhsN= PCSdata.PhV_PhsN;//;//206	R	电网N相电压	UINT16	0.1V		PhV_PhsN

	
	DATA_ttu.DATA_TTU_pcs.A_phsA= (int16_t)(PCSdata.sys_i[0]/100);//;//207	R	电网A相电流	INT16	0.1A		A_phsA
	DATA_ttu.DATA_TTU_pcs.A_phsB= (int16_t)(PCSdata.sys_i[1]/100);//;//208	R	电网B相电流	INT16	0.1A		A_phsB
	DATA_ttu.DATA_TTU_pcs.A_phsC= (int16_t)(PCSdata.sys_i[2]/100);//;//209	R	电网C相电流	INT16	0.1A		A_phsC
//	DATA_ttu.DATA_TTU_pcs.A_phsN= PCSdata.A_phsN;//;//210	R	电网N相电流	INT16	0.1A		A_phsN
	DATA_ttu.DATA_TTU_pcs.Hz= PCSdata.freq;//;//211	R	电网频率	UINT16	0.01Hz		Hz
	DATA_ttu.DATA_TTU_pcs.TotW= (int16_t)(PCSdata.sys_p[0]/100);//;//212	R	电网总有功功率	UINT16	0.1kW	武汉PCS和班特里奥均无单相功率及功率因数	TotW
	DATA_ttu.DATA_TTU_pcs.TotVAr=(int16_t)(PCSdata.sys_p[1]/100);//;//213	R	电网总无功功率	UINT16	0.1kVar		TotVAr
	DATA_ttu.DATA_TTU_pcs.TotVA= (int16_t)(PCSdata.sys_p[2]/100);//;//214	R	电网总视在功率	UINT16	0.1kVA		TotVA

	// 模块上传的与设计的相差较大，重新计算
	DATA_ttu.DATA_TTU_pcs.TotW= ((int32_t)DATA_ttu.DATA_TTU_pcs.PhV_phsA  * DATA_ttu.DATA_TTU_pcs.A_phsA+
								 (int32_t)DATA_ttu.DATA_TTU_pcs.PhV_phsB  * DATA_ttu.DATA_TTU_pcs.A_phsB+
								 (int32_t)DATA_ttu.DATA_TTU_pcs.PhV_phsC  * DATA_ttu.DATA_TTU_pcs.A_phsC)/100/100;

	temp2= (int32_t)DATA_ttu.DATA_TTU_pcs.TotW  *DATA_ttu.DATA_TTU_pcs.TotW +
		   (int32_t)DATA_ttu.DATA_TTU_pcs.TotVAr*DATA_ttu.DATA_TTU_pcs.TotVAr;
	
	temp2 = data2hex(temp2);
	temp2 = sqrtf(temp2);

	temp2 = Float32(temp2);
	DATA_ttu.DATA_TTU_pcs.TotVA = (uint16_t)(temp2);

    DATA_ttu.DATA_TTU_pcs.Load_PhV_phsA= (uint16_t)(PCSdata.load_u[0]/100);//;//215	R	负载A相电压	UINT16	0.1V		Load_PhV_phsA
	DATA_ttu.DATA_TTU_pcs.Load_PhV_phsB= (uint16_t)(PCSdata.load_u[1]/100);//;//216	R	负载B相电压	UINT16	0.1V		Load_PhV_phsB
	DATA_ttu.DATA_TTU_pcs.Load_PhV_phsC= (uint16_t)(PCSdata.load_u[2]/100);//;//217	R	负载C相电压	UINT16	0.1V		Load_PhV_phsC
//	DATA_ttu.DATA_TTU_pcs.Load_PhV_phsN= PCSdata.Load_PhV_phsN;//;//218	R	负载N相电压	UINT16	0.1V		Load_PhV_phsN
	DATA_ttu.DATA_TTU_pcs.Load_A_phsA= (int16_t)(PCSdata.load_i[0]/100);//;//219	R	负载A相电流	INT16	0.1A		Load_A_phsA
	DATA_ttu.DATA_TTU_pcs.Load_A_phsB= (int16_t)(PCSdata.load_i[1]/100);//;//220	R	负载B相电流	INT16	0.1A		Load_A_phsB
	DATA_ttu.DATA_TTU_pcs.Load_A_phsC= (int16_t)(PCSdata.load_i[2]/100);//;//221	R	负载C相电流	INT16	0.1A		Load_A_phsC
//	DATA_ttu.DATA_TTU_pcs.Load_A_phsN= PCSdata.Load_A_phsN;//;//222	R	负载N相电流	INT16	0.1A		Load_A_phsN
//	DATA_ttu.DATA_TTU_pcs.Load_Hz= PCSdata.Load_Hz;//;//223	R	负载频率	UINT16	0.01Hz		Load_Hz
	DATA_ttu.DATA_TTU_pcs.Load_TotW= (int16_t)(PCSdata.load_p[0]/100);//;//224	R	负载总有功功率	UINT16	0.1kW		Load_TotW
	DATA_ttu.DATA_TTU_pcs.Load_TotVAr= (int16_t)(PCSdata.load_p[1]/100);//;//225	R	负载总无功功率	UINT16	0.1kVar		Load_TotVAr
	DATA_ttu.DATA_TTU_pcs.Load_TotVA= (int16_t)(PCSdata.load_p[2]/100);//226	R	负载总视在功率	UINT16	0.1kVA		Load_TotVA
	
//	DATA_ttu.DATA_TTU_pcs.Output_PhV_phsA= PCSdata.Output_PhV_phsA;//;//227	R	输出A相电压	UINT16	0.1V		Output_PhV_phsA
//	DATA_ttu.DATA_TTU_pcs.Output_PhV_phsB= PCSdata.Output_PhV_phsB;//;//228	R	输出B相电压	UINT16	0.1V		Output_PhV_phsB
//	DATA_ttu.DATA_TTU_pcs.Output_PhV_phsC= PCSdata.Output_PhV_phsC;//;//229	R	输出C相电压	UINT16	0.1V		Output_PhV_phsC
//	DATA_ttu.DATA_TTU_pcs.Output_PhV_phsN= PCSdata.Output_PhV_phsN;//;//230	R	输出N相电压	UINT16	0.1V		Output_PhV_phsN
//	DATA_ttu.DATA_TTU_pcs.Output_A_phsA= PCSdata.Output_A_phsA;//;//231	R	输出A相电流	INT16	0.1A		Output_A_phsA
//	DATA_ttu.DATA_TTU_pcs.Output_A_phsB= PCSdata.Output_A_phsB;//;//232	R	输出B相电流	INT16	0.1A		Output_A_phsB
//	DATA_ttu.DATA_TTU_pcs.Output_A_phsC= PCSdata.Output_A_phsC;//;//233	R	输出C相电流	INT16	0.1A		Output_A_phsC
//	DATA_ttu.DATA_TTU_pcs.Output_A_phsN= PCSdata.Output_A_phsN;//;//234	R	输出N相电流	INT16	0.1A		Output_A_phsN
//	DATA_ttu.DATA_TTU_pcs.Output_Hz= PCSdata.Output_Hz;//;//235	R	输出频率	UINT16	0.01Hz		Output_Hz
//	DATA_ttu.DATA_TTU_pcs.Output_TotW= PCSdata.Output_TotW;//;//236	R	输出总有功功率	UINT16	0.1kW		Output_TotW
//	DATA_ttu.DATA_TTU_pcs.Output_TotVAr= PCSdata.Output_TotVAr;//;//237	R	输出总无功功率	UINT16	0.1kVar		Output_TotVAr
//	DATA_ttu.DATA_TTU_pcs.Output_TotVA= PCSdata.Output_TotVA;//;//238	R	输出总视在功率	UINT16	0.1kVA		Output_TotVA
	
	DATA_ttu.DATA_TTU_pcs.DCTolBus_Volt= PCSdata.VDC;//;//239	R	直流侧总母线电压 	UINT16	0.1V		DCTolBus_Volt
	//DATA_ttu.DATA_TTU_pcs.DCUpBus_Volt= PCSdata.DCUpBus_Volt;//;//240	R	直流上母线电压	UINT16	0.1V		DCUpBus_Volt
	//DATA_ttu.DATA_TTU_pcs.DCDownBus_Volt= PCSdata.DCDownBus_Volt;//;//241	R	直流下母线电压	UINT16	0.1V		DCDownBus_Volt
//	DATA_ttu.DATA_TTU_pcs.DC_Cur= PCSdata.DC_Cur;//;//242	R	直流侧电流 	INT16	0.1A		DC_Cur
//	DATA_ttu.DATA_TTU_pcs.DC_ActPower= PCSdata.DC_ActPower;//;//243	R	直流侧功率 	UINT16	0.1kW		DC_ActPower
//	DATA_ttu.DATA_TTU_pcs.IGBT_LowestTmp= PCSdata.IGBT_LowestTmp;//;//244	R	IGBT最低温度	UINT16	0.1°C	多个IGBT器件温度，显示最低的温度	IGBT_LowestTmp
//	DATA_ttu.DATA_TTU_pcs.IGBT_HighestTmp= PCSdata.IGBT_HighestTmp;//;//245	R	IGBT最高温度	UINT16	0.1°C	多个IGBT器件温度，显示最高的温度	IGBT_HighestTmp
	DATA_ttu.DATA_TTU_pcs.Env_Tmp= PCSdata.temprature_in;//;//246	R	环境温度	INT16	0.1°C	设备上读到的环境温度	Env_Tmp


	//BIT7:电网电压过压/BIT6:电网电压欠压/BIT5:电网电压相序错误/BIT4:DC母线电压异常/BIT3:输出电流异常
	//BIT2:过温故障/BIT1:通讯故障/BIT0:急停按钮

	if(PCSdata.fault == 18) 		temp |= BIT(0);
	else 							temp &= (~BIT(0));
	if(PCSdata.fault == 32781) 	temp |= BIT(1);
	else 							temp &= (~BIT(1));
	if(PCSdata.fault == 5)	 	temp |= BIT(2);
	else	 						temp &= (~BIT(2));
	if(PCSdata.fault == 4) 		temp |= BIT(3);
	else 							temp &= (~BIT(3));
	if(PCSdata.fault == 265) 	temp |= BIT(4);
	else 							temp &= (~BIT(4));
	if(PCSdata.fault == 258) 	temp |= BIT(5);
	else 							temp &= (~BIT(5));
	if(PCSdata.fault == 257) 	temp |= BIT(6);
	else 							temp &= (~BIT(6));
	DATA_ttu.DATA_TTU_pcs.PCS_Status1= temp;//;//247	R	PCS状态1	UINT16		"BIT7:电网电压过压/BIT6:电网电压欠压/BIT5:电网电压相序错误/BIT4:DC母线电压异常/BIT3:输出电流异常
							//BIT2:过温故障/BIT1:通讯故障/BIT0:急停按钮"	PCS_Status1
//	DATA_ttu.DATA_TTU_pcs.PCS_Status2= PCSdata.PCS_Status2;//;//248	R	PCS状态2	UINT16			PCS_Status2

//0x02	DC恒压	电压设定(0.001V) //0x21	DC恒流	电流设定(0.001A)  //0x22	DC恒功率	功率设定(0.001W)
	if(PCSdata.mode == 0x02)  	    temp = 0x0A;  //DC恒压模式时，设置的参数1是BAT口的电压。
	else if(PCSdata.mode == 0x21)    temp = 0x0B;
	else if(PCSdata.mode == 0x22)    temp = 0x0C;
	else   								temp = 0x0;
	
	//write
	DATA_ttu.DATA_TTU_pcs.PCS_RunMode_Set= temp;//270	R/W	PCS运行模式设置	UINT16		A恒压模式，B恒流模式，C恒功率模式,D稳压限流模式	PCS_RunMode_Set
	DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Volt_Set= (uint16_t)(PCSdata.bus_V/100);//;//271	R/W	直流侧母线电压值设置	UINT16	0.1V	直流电压目标值设置，如700/750等	PCS_DCSide_Volt_Set

			
	if(DATA_ttu.DATA_TTU_pcs.PCS_RunMode_Set== 0x0B) 
		DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Cur_Set= (uint16_t)(PCSdata.parameter[0]/100);//;//272	R/W	直流侧电流值设置	INT16	0.1A	直流侧电流值设置，正负可表示方向，正表示输入，负表示返送	PCS_DCSide_Cur_Set
	else DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Cur_Set=0;
	
	if(DATA_ttu.DATA_TTU_pcs.PCS_RunMode_Set== 0x0C) 
		DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Power_Set= (uint16_t)(PCSdata.parameter[0]/100);//273	R/W	直流侧功率值设置 	INT16	0.1KW	直流侧输入或输出功率设置，正负可表示有方向	PCS_DCSide_Power_Set
	else DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Power_Set=0;
	
//	DATA_ttu.DATA_TTU_pcs.PCS_DCBusVoltProVal_Set= PCSdata.PCS_DCBusVoltProVal_Set;//;//274	R/W	直流母线电压保护值设置	UINT16	0.1V	直流侧母线电压保护值上限	PCS_DCBusVoltProVal_Set


	//下边都是电池侧
	DATA_ttu.DATA_TTU_pcs.PCS_DCSide_CurLim_Set= (uint16_t)(PCSdata.set[2]/100);//275	R/W	直流侧电流限流值设置	INT16	0.1A	直流侧电流超过限流值则按照限流值下发	PCS_DCSide_CurLim_Set
	DATA_ttu.DATA_TTU_pcs.PCS_DCSide_PowerLim_Set= (uint16_t)(PCSdata.set[4]/100);//276	R/W	直流侧功率限流值设置	INT16	0.1KW		PCS_DCSide_PowerLim_Set
	DATA_ttu.DATA_TTU_pcs.PCS_DCSideCurProVal_Set= (uint16_t)(PCSdata.set[2]/100);//277	R/W	直流侧电流保护值设置	UINT16	0.1A		PCS_DCSideCurProVal_Set
	DATA_ttu.DATA_TTU_pcs.PCS_DCSidePowerProVal_Set= (uint16_t)(PCSdata.set[4]/100);//278 R/W	直流侧功率保护值设置	UINT16	0.1KW		PCS_DCSidePowerProVal_Set

}


/****************************************************************
	更新储能数据到 融合终端通讯的数据表 
*****************************************************************/
void updataSTAC(void)
{
	
	DATA_ttu.DATA_TTU_stac.STAC_BatSide_Volt = (uint16_t)(PCSdata.DC_U/100);//301	R	电池侧电压	UINT16	0.1V	模块电池侧接口电压	STAC_BatSide_Volt
	DATA_ttu.DATA_TTU_stac.STAC_BatSide_Cur = (uint16_t)(PCSdata.DC_I/100);//302	R	电池侧电流	INT16 	0.1A	模块电池侧接口电流，应具有符号表示充/放	STAC_BatSide_Cur
	DATA_ttu.DATA_TTU_stac.STAC_BatSide_ActPower= (uint16_t)(PCSdata.DC_P/100);//303	R	电池侧功率	INT16 	0.1kW	模块电池侧接口充/放电功率	STAC_BatSide_ActPower

	DATA_ttu.DATA_TTU_stac.STAC_BusSide_Volt=PCSdata.VDC;//304	R	母线侧电压	UINT16	0.1V	模块直流母线侧接口电压	STAC_BusSide_Volt
//	DATA_ttu.DATA_TTU_stac.STAC_BusSide_Cur;//305	R	母线侧电流	INT16 	0.1A	模块直流母线侧接口电流	STAC_BusSide_Cur
//	DATA_ttu.DATA_TTU_stac.STAC_BusSide_ActPower;//306	R	母线侧功率	INT16	0.1kW	模块直流母线侧接口充/放电功率	STAC_BusSide_ActPower
//	DATA_ttu.DATA_TTU_stac.STAC_CurDevStatus;//307	R	当前开关机状态	UINT16			STAC_CurDevStatus
//	DATA_ttu.DATA_TTU_stac.STAC_CurRunMode;//308	R	当前运行模式	UINT16			STAC_CurRunMode
//	DATA_ttu.DATA_TTU_stac.STAC_IGBT_LowestTmp;//309	R	IGBT最低温度	UINT16	0.1°C	多个IGBT器件温度，显示最低的温度	STAC_IGBT_LowestTmp
//	DATA_ttu.DATA_TTU_stac.STAC_IGBT_HighestTmp;//310	R	IGBT最高温度	UINT16	0.1°C	多个IGBT器件温度，显示最高的温度	STAC_IGBT_HighestTmp
//	DATA_ttu.DATA_TTU_stac.STAC_EnvTmp;//311	R	环境温度	INT16	0.1°C	设备上读到的环境温度	STAC_EnvTmp
//	DATA_ttu.DATA_TTU_stac.STAC_FaultMsg;//312	R	故障信息	UINT16		BIT5:电池电压高超限/BIT4电池电压低超限/BIT3:电池反接/BIT2:电流超限/BIT1:母线中点不平衡/BIT0:过温故障	STAC_FaultMsg

	
//	DATA_ttu.DATA_TTU_stac.STAC_OnOff_Set;//321	R/W	开关机设置	UINT16		开关机，写AAAA开机，写其它关机	STAC_OnOff_Set
//	DATA_ttu.DATA_TTU_stac.STAC_RunMode_Set;//322	R/W	运行模式设置	UINT16		更改运行模式，1为恒压限流模式，4为恒压模式	STAC_RunMode_Set
	DATA_ttu.DATA_TTU_stac.STAC_HVSideVolt_Set=(uint16_t)(PCSdata.bus_V/100);//323	R/W	高压侧电压设置	UINT16	0.1V	母线侧的电压设置，一般设置750V	STAC_HVSideVolt_Set
	
		
//	DATA_ttu.DATA_TTU_stac.STAC_LVSidecur_Set;//324	R/W	低压侧电流设置	INT16	0.1A	电池侧的电流设置，即低压电流设置	STAC_LVSidecur_Set
//	DATA_ttu.DATA_TTU_stac.STAC_325;//325	R/W	高压侧上稳压电压值	UINT16	0.1V		STAC_325
//	DATA_ttu.DATA_TTU_stac.STAC_326;//326	R/W	高压侧下稳压电压值	UINT16	0.1V		STAC_326
//	DATA_ttu.DATA_TTU_stac.STAC_327;//327	R/W	高压侧上保护电压值	UINT16	0.1V		STAC_327
//	DATA_ttu.DATA_TTU_stac.STAC_328;//328	R/W	高压侧下保护电压值	UINT16	0.1V		STAC_328
	

	
	DATA_ttu.DATA_TTU_stac.STAC_329= (uint16_t)(PCSdata.set[0]/100);//329	R/W	低压侧上保护电压值	UINT16	0.1V		STAC_329
	DATA_ttu.DATA_TTU_stac.STAC_330= (uint16_t)(PCSdata.set[1]/100);//330	R/W	低压侧下保护电压值	UINT16	0.1V		STAC_330
//	DATA_ttu.DATA_TTU_stac.STAC_331;//331	R/W	高压侧电流保护值	UINT16	0.1A		STAC_331
	DATA_ttu.DATA_TTU_stac.STAC_332=(uint16_t)(PCSdata.set[2]/100);//332	R/W	低压侧电流保护值	UINT16	0.1A		STAC_332
	
}

/****************************************************************
	更新光伏模块数据到 融合终端通讯的数据表
*****************************************************************/	
void updataPVAC(void)
{
	uint32_t temp_32;
	uint16_t temp_16,temp2_16;
	static uint16_t lasthour,lastmonth,lastday;
	uint16_t temp_power;
	static float energy=0;
	
	DATA_ttu.DATA_TTU_pvac.PVAC_PVSide_Volt = DC2data.VDC2;//341	R	光伏侧电压	UINT16	0.1V	模块光伏侧接口电压	PVAC_PVSide_Volt
	
	if((DC2data.IDC2<5) && (DC2data.IDC2>(-5))) 
		DC2data.IDC2 = 0;//因为现场纠结误差问题不好看，当小于0.5A时直接赋0.
	
	DATA_ttu.DATA_TTU_pvac.PVAC_PVSide_Cur= DC2data.IDC2;//342	R	光伏侧电流	INT16 	0.1A	模块光伏侧接口电流，应具有符号表示充/放	PVAC_PVSide_Cur
	
	temp_32 = (int32_t)DC2data.VDC2 * DC2data.IDC2; 
	DATA_ttu.DATA_TTU_pvac.PVAC_PVSide_ActPower= (int16_t)((int32_t)temp_32/10000);//343	R	光伏侧功率	INT16 	0.1kW	模块光伏侧接口充/放电功率	PVAC_PVSide_ActPower
	
	//取绝对值
	if(DC2data.IDC2<0) temp_16 = 0-DC2data.IDC2;
	else temp_16 = DC2data.IDC2;
	if(DC2data.VDC2<0) temp2_16 = 0-DC2data.VDC2;
	else temp2_16 = DC2data.VDC2;
	
	temp_32 = (uint32_t)temp_16 * temp2_16;  //不确定电流是正值还是负值，取绝对值。
	temp_power= (uint16_t)(temp_32/10000);//343	R	光伏侧功率	取绝对值了，用于计算电能。
	
	DATA_ttu.DATA_TTU_pvac.PVAC_BusSide_Volt= DC2data.VDC1;//344	R	母线侧电压	UINT16	0.1V	模块直流母线侧接口电压	PVAC_BusSide_Volt
	DATA_ttu.DATA_TTU_pvac.PVAC_BusSide_Cur= DC2data.IDC1;//345	R	母线侧电流	INT16 	0.1A	模块直流母线侧接口电流	PVAC_BusSide_Cur
	
	
	temp_32 = (uint32_t)DC2data.VDC1*DC2data.IDC1;	
	
	//这里必须将temp32转成 int32 。否则计算不对
	DATA_ttu.DATA_TTU_pvac.PVAC_BusSide_ActPower= (int32_t)temp_32/10000;//346	R	母线侧功率	INT16	0.1kW	模块直流母线侧接口充/放电功率	PVAC_BusSide_ActPower

	if(DC2data.par[0] ==7) //光伏模式不用开关机指令，7模式会自动开关机
		DATA_ttu.DATA_TTU_pvac.PVAC_CurDevStatus= 1;//347	R	当前开关机状态	UINT16			PVAC_CurDevStatus
	else
		DATA_ttu.DATA_TTU_pvac.PVAC_CurDevStatus= 0;
	
	DATA_ttu.DATA_TTU_pvac.PVAC_CurRunMode= DC2data.par[0];//348	R	当前运行模式	UINT16			PVAC_CurRunMode
	
	if(DC2data.T_IGBT1 >DC2data.T_IGBT2 )
	{
		DATA_ttu.DATA_TTU_pvac.PVAC_IGBT_LowestTmp= DC2data.T_IGBT2;//349	R	IGBT最低温度	UINT16	0.1°C	多个IGBT器件温度，显示最低的温度	PVAC_IGBT_LowestTmp
		DATA_ttu.DATA_TTU_pvac.PVAC_IGBT_HighestTmp= DC2data.T_IGBT1;//350	R	IGBT最高温度	UINT16	0.1°C	多个IGBT器件温度，显示最高的温度	PVAC_IGBT_HighestTmp
	}
	else
	{
		DATA_ttu.DATA_TTU_pvac.PVAC_IGBT_LowestTmp= DC2data.T_IGBT1;//349	R	IGBT最低温度	UINT16	0.1°C	多个IGBT器件温度，显示最低的温度	PVAC_IGBT_LowestTmp
		DATA_ttu.DATA_TTU_pvac.PVAC_IGBT_HighestTmp= DC2data.T_IGBT2;//350	R	IGBT最高温度	UINT16	0.1°C	多个IGBT器件温度，显示最高的温度	PVAC_IGBT_HighestTmp	
	}

//	DATA_ttu.DATA_TTU_pvac.PVAC_EnvTmp= DC2data.;//351	R	环境温度	INT16	0.1°C	设备上读到的环境温度	PVAC_EnvTmp

	//BIT5:光伏电压高超限/BIT4：光伏电压低超限/BIT3:光伏反接/BIT2:电流超限/BIT1:母线中点不平衡/BIT0:过温故障

	if((DC2data.state0 & BIT(4)) == BIT(4)) 		temp_16 |= BIT(0);
	else 											temp_16 &= (~BIT(0));
	if((DC2data.state2 & BIT(3)) == BIT(3)) 	 	temp_16 |= BIT(1);
	else 											temp_16 &= (~BIT(1));
	if((DC2data.state0 & BIT(3)) == BIT(3)) 	 	temp_16 |= BIT(2);
	else 											temp_16 &= (~BIT(2));
	

	if((DC2data.state2 & BIT(10)) == BIT(10)) 	temp_16 |= BIT(3);
	else	 										temp_16 &= (~BIT(3));
	if((DC2data.state2 & BIT(8)) == BIT(8))  	temp_16 |= BIT(4);
	else 											temp_16 &= (~BIT(4));
	if((DC2data.state2 & BIT(9)) == BIT(9))  	temp_16 |= BIT(5);
	else 											temp_16 &= (~BIT(5));
	DATA_ttu.DATA_TTU_pvac.PVAC_FaultMsg= temp_16;//352	R	故障信息	UINT16			PVAC_FaultMsg
			
		
	//计算电能。
	////因为当前任务3秒运行一次，所以换算到小时就是当前的电能 单位0.1kwh.
	//要考虑到精度，比如现在是60，6kw。那么除以1200，就变为0了。现在一是可以将energy改为float，这样精度更高，在后边赋值会自动转换为u32

	energy = (float)temp_power/1200; 
	
	if((timePC[3] == 0)&&(lasthour == 23)){ //new day
		PVAC_Power[0] = 0;	
	}
	else{
		PVAC_Power[0] += energy;
	}	
	
	if((timePC[2] == 1)&&  ((lastday == 28) ||(lastday == 29)||(lastday == 30)||(lastday == 31)) )  {//new month
		PVAC_Power[1] = 0;
	}
	else{
		PVAC_Power[1] += energy;
	}
		
	if((timePC[1] ==1) && (lastmonth == 12)){  //new year
		PVAC_Power[2] = 0;
		energy=0;
	}
	else{
		PVAC_Power[2] += energy;
	}
	
	//经过测试，float会自动转换为u32类型，去掉小数点。
	DATA_ttu.DATA_TTU_pvac.PVAC_DayWh= PVAC_Power[0];//353	R	日发电量	UINT32	0.1kwh		PVAC_DayWh
	DATA_ttu.DATA_TTU_pvac.PVAC_MonWh= PVAC_Power[1];//355	R	月发电量	UINT32	0.1kwh		PVAC_MonWh
	DATA_ttu.DATA_TTU_pvac.PVAC_YearWh= PVAC_Power[2];//355	R	月发电量	UINT32	0.1kwh		PVAC_YearWh

	DATA_ttu.DATA_TTU_pvac.PVAC_DayWh=CHANGE_H16_L16(DATA_ttu.DATA_TTU_pvac.PVAC_DayWh);
	DATA_ttu.DATA_TTU_pvac.PVAC_MonWh=CHANGE_H16_L16(DATA_ttu.DATA_TTU_pvac.PVAC_MonWh);
	DATA_ttu.DATA_TTU_pvac.PVAC_YearWh=CHANGE_H16_L16(DATA_ttu.DATA_TTU_pvac.PVAC_YearWh);
		
	lasthour = timePC[3];   //小时
	lastday= timePC[2];     //日
	lastmonth= timePC[1];	//月				

	
	if(DC2data.par[9] ==1)
		DATA_ttu.DATA_TTU_pvac.PVAC_371= 0xAAAA;//371	R/W	开关机设置	UINT16		操作模块开关机，写AAAA开机，写其它关机	
	else
		DATA_ttu.DATA_TTU_pvac.PVAC_371= 0;
	DATA_ttu.DATA_TTU_pvac.PVAC_372= 0x0A;//372	R/W	运行模式设置	UINT16		A:光伏模式、MPPT模式	
	DATA_ttu.DATA_TTU_pvac.PVAC_373= DC2data.par[2];//373	R/W	母线侧电压设置	UINT16	0.1V	母线侧的电压设置	
	DATA_ttu.DATA_TTU_pvac.PVAC_374= DC2data.par[5];//374	R/W	光伏侧电流设置	INT16	0.1A	光伏侧的电流设置，即低压电流设置，用于限功率设置	
	DATA_ttu.DATA_TTU_pvac.PVAC_375= DC2data.VDC1_max;//375	R/W	母线侧保护电压值	UINT16	0.1V		
	DATA_ttu.DATA_TTU_pvac.PVAC_376= DC2data.VDC2_max;//376	R/W	光伏侧保护电压值	UINT16	0.1V		
	DATA_ttu.DATA_TTU_pvac.PVAC_377= DC2data.VDC2_min;//377	R/W	光伏侧下保护电压值	UINT16	0.1V		
	DATA_ttu.DATA_TTU_pvac.PVAC_378= DC2data.IDC1_discharge_max;//378	R/W	母线侧电流保护值	UINT16	0.1A		
	DATA_ttu.DATA_TTU_pvac.PVAC_379= DC2data.IDC2_charge_max;//379	R/W	光伏侧电流保护值	UINT16	0.1A		

}

/****************************************************************
	更新所有数据到 融合终端通讯的数据表
*****************************************************************/

void updataOTHER(void)
{
	DATA_ttu.DATA_TTU_meter.Ua= CHANGE_H16_L16(hour_meter.Ua);
	DATA_ttu.DATA_TTU_meter.Ub= CHANGE_H16_L16(hour_meter.Ub);
	DATA_ttu.DATA_TTU_meter.Uc= CHANGE_H16_L16(hour_meter.Uc);
	DATA_ttu.DATA_TTU_meter.Ia= CHANGE_H16_L16(hour_meter.Ia);
	DATA_ttu.DATA_TTU_meter.Ib= CHANGE_H16_L16(hour_meter.Ib);
	DATA_ttu.DATA_TTU_meter.Ic= CHANGE_H16_L16(hour_meter.Ic);
	

	DATA_ttu.DATA_TTU_meter.Ps=  CHANGE_H16_L16(hour_meter.Ps);
	DATA_ttu.DATA_TTU_meter.Qs=  CHANGE_H16_L16(hour_meter.Qs);
	DATA_ttu.DATA_TTU_meter.VAs= CHANGE_H16_L16(hour_meter.VAs);
	DATA_ttu.DATA_TTU_meter.PFs= CHANGE_H16_L16(hour_meter.PFs);
	
	DATA_ttu.DATA_TTU_meter.KWH[0]=  CHANGE_H16_L16(hour_meter.KWH[0]);
	DATA_ttu.DATA_TTU_meter.KWH[1]=  CHANGE_H16_L16(hour_meter.KWH[1]);
	DATA_ttu.DATA_TTU_meter.KWH[2]=  CHANGE_H16_L16(hour_meter.KWH[2]);
	
	DATA_ttu.DATA_TTU_meter.Harm_U[0] = hour_meter.Harm_U[0];
	DATA_ttu.DATA_TTU_meter.Harm_U[1] = hour_meter.Harm_U[1];
	DATA_ttu.DATA_TTU_meter.Harm_U[2] = hour_meter.Harm_U[2];
	DATA_ttu.DATA_TTU_meter.Harm_I[0] = hour_meter.Harm_I[0];
	DATA_ttu.DATA_TTU_meter.Harm_I[1] = hour_meter.Harm_I[1];
	DATA_ttu.DATA_TTU_meter.Harm_I[2] = hour_meter.Harm_I[2];
	
		//添加thd优化
	if(DATA_ttu.DATA_TTU_meter.Harm_I[0] >2000)
		DATA_ttu.DATA_TTU_meter.Harm_I[0] = DATA_ttu.DATA_TTU_meter.Harm_I[0]-2000;
	if(DATA_ttu.DATA_TTU_meter.Harm_I[0]>1000) 
		DATA_ttu.DATA_TTU_meter.Harm_I[0] = DATA_ttu.DATA_TTU_meter.Harm_I[0]-1000;
	if(DATA_ttu.DATA_TTU_meter.Harm_I[0]>500) 
		DATA_ttu.DATA_TTU_meter.Harm_I[0] = DATA_ttu.DATA_TTU_meter.Harm_I[0]-500;
	
	if(DATA_ttu.DATA_TTU_meter.Harm_I[1] >2000)
		DATA_ttu.DATA_TTU_meter.Harm_I[1] = DATA_ttu.DATA_TTU_meter.Harm_I[1]-2000;
	if(DATA_ttu.DATA_TTU_meter.Harm_I[1]>1000) 
		DATA_ttu.DATA_TTU_meter.Harm_I[1] = DATA_ttu.DATA_TTU_meter.Harm_I[1]-1000;
	if(DATA_ttu.DATA_TTU_meter.Harm_I[1]>500) 
		DATA_ttu.DATA_TTU_meter.Harm_I[1] = DATA_ttu.DATA_TTU_meter.Harm_I[1]-500;
	
	if(DATA_ttu.DATA_TTU_meter.Harm_I[2] >2000)
		DATA_ttu.DATA_TTU_meter.Harm_I[2] = DATA_ttu.DATA_TTU_meter.Harm_I[2]-2000;
	if(DATA_ttu.DATA_TTU_meter.Harm_I[2]>1000) 
		DATA_ttu.DATA_TTU_meter.Harm_I[2] = DATA_ttu.DATA_TTU_meter.Harm_I[2]-1000;
	if(DATA_ttu.DATA_TTU_meter.Harm_I[2]>500) 
		DATA_ttu.DATA_TTU_meter.Harm_I[2] = DATA_ttu.DATA_TTU_meter.Harm_I[2]-500;
	
	
	DATA_ttu.DATA_TTU_pvclass.P =  				CHANGE_H16_L16(PV_Glass.P);
	DATA_ttu.DATA_TTU_pvclass.P_DC =  			CHANGE_H16_L16(PV_Glass.P_DC);
	DATA_ttu.DATA_TTU_pvclass.Power_total =  	CHANGE_H16_L16(PV_Glass.Power_total);
	DATA_ttu.DATA_TTU_pvclass.Power_month =  	CHANGE_H16_L16(PV_Glass.Power_month);
	DATA_ttu.DATA_TTU_pvclass.Power_lastmonth = CHANGE_H16_L16(PV_Glass.Power_lastmonth);
	DATA_ttu.DATA_TTU_pvclass.Power_day = 		PV_Glass.Power_day;
}

/****************************************************************
	更新所有数据到 融合终端通讯的数据表
*****************************************************************/
void Data_Transfer_entry(void *parameter)
{
	//TickType_t xLastWakeTime;
	//DEBUG_PRINTF("%s\n",(char *)parameter);
	
	// Initialise the xLastWakeTime variable with the current time.
   // xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		//vTaskDelayUntil( &xLastWakeTime, 3000 / portTICK_RATE_MS );
		rt_thread_delay(3000/ portTICK_RATE_MS);
		updataEMS();
		updataPCS();
		//updataSTAC();
		updataPVAC(); //光伏
	//	updataBAT(); //电池
	//	updataCHG(); //充电桩
		updataOTHER();
		
	}	
	/* 如果任务的具体实现会跳出上面的死循环，则此任务必须在函数运行完之前删除。传入NULL参数表示删除
	的是当前任务 */

}
