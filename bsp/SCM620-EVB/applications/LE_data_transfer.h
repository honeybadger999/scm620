#pragma pack(2)
struct DATA_TTU_EMS 
{
	uint16_t PCS_Fault;//100	R	PCS模块故障	UINT16		系统监测故障	PCS_Fault
	uint16_t DCStore_Fault;//101	R	DC储能模块故障	UINT16		系统监测故障	DCStore_Fault
	uint16_t DCPV_Fault;//102	R	DC光伏模块故障	UINT16		系统监测故障	DCPV_Fault
	uint16_t DCAC_Fault;//103	R	DCAC模块故障	UINT16		系统监测故障	DCAC_Fault
	uint16_t Battery_Fault;//104	R	电池故障	UINT16		系统监测故障	Battery_Fault
	uint16_t AutoModeOn_Excep;//105	R	自动模式开机异常标志	UINT16		70 ：FAULT/FAIL 80：PASS	AutoModeOn_Excep
	uint16_t SysVSVolt_Excep;//106	R	系统VS电压异常标志	UINT16		70 ：FAULT/FAIL 80：PASS	SysVSVolt_Excep
	uint16_t BusVDCVolt_Excep;//107	R	母线VDC电压异常标志	UINT16		70 ：FAULT/FAIL 80：PASS	BusVDCVolt_Excep
	uint16_t MasterDevCommu_Err;//108	R	整体通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	MasterDevCommu_Err
	uint16_t PCSCommu_Err;//109	R	PCS通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	PCSCommu_Err
	uint16_t DC1Commu_Err;//110	R	DC1通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	DC1Commu_Err
	uint16_t DC2Commu_Err;//111	R	DC2通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	DC2Commu_Err
	uint16_t BatCommu_Err;//112	R	电池通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	BatCommu_Err
	uint16_t DCACCommu_Err;//113	R	DCAC通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	DCACCommu_Err
	uint16_t ChargingPileCommu_Err;//114	R	充电桩通讯错误标志	UINT16		70 ：FAULT/FAIL 80：PASS	ChargingPileCommu_Err
	uint16_t rsvd1[5];					
	uint16_t AutoModeOn_Status;//120	R	自动模式开机状态	UINT16		1为开机  0为关机	AutoModeOn_Status
	uint16_t CurRunMode;//121	R	当前运行模式	UINT16			CurRunMode
	uint16_t rsvd2[28];//-149		预留
	
	//write
	uint16_t EMS_Mode_Set;//150	R/W	自动/手动模式设置	UINT16		A：自动开关机模式 其它：手动模式	EMS_Mode_Set
	uint16_t EMS_OnOff_Set;//151	R/W	开关机设置	UINT16		AAAA开机，其它关机	EMS_OnOff_Set
	uint16_t EMS_OffConnectGrid_Set;//152	R/W	离并网设置	UINT16		手动进行离并网控制，PCS应该具备这个功能	EMS_OffConnectGrid_Set
	uint16_t rsvd3[48];//-199		预留								
};

struct DATA_TTU_PCS 
{
	uint16_t UAB;//200	R	电网AB线电压	UINT16	0.1V		UAB
	uint16_t UBC;//201	R	电网BC线电压	UINT16	0.1V		UBC
	uint16_t UAC;//202	R	电网AC线电压	UINT16	0.1V		UAC
	uint16_t PhV_phsA;//203	R	电网A相电压	UINT16	0.1V		PhV_phsA
	uint16_t PhV_phsB;//204	R	电网B相电压	UINT16	0.1V		PhV_phsB
	uint16_t PhV_phsC;//205	R	电网C相电压	UINT16	0.1V		PhV_phsC
	uint16_t PhV_PhsN;//206	R	电网N相电压	UINT16	0.1V		PhV_PhsN
	int16_t  A_phsA;//207	R	电网A相电流	INT16	0.1A		A_phsA
	int16_t A_phsB;//208	R	电网B相电流	INT16	0.1A		A_phsB
	int16_t A_phsC;//209	R	电网C相电流	INT16	0.1A		A_phsC
	int16_t A_phsN;//210	R	电网N相电流	INT16	0.1A		A_phsN
	uint16_t Hz;//211	R	电网频率	UINT16	0.01Hz		Hz
	int16_t TotW;//212	R	电网总有功功率	UINT16	0.1kW	武汉PCS和班特里奥均无单相功率及功率因数	TotW
	int16_t TotVAr;//213	R	电网总无功功率	UINT16	0.1kVar		TotVAr
	uint16_t TotVA;//214	R	电网总视在功率	UINT16	0.1kVA		TotVA
	uint16_t Load_PhV_phsA;//215	R	负载A相电压	UINT16	0.1V		Load_PhV_phsA
	uint16_t Load_PhV_phsB;//216	R	负载B相电压	UINT16	0.1V		Load_PhV_phsB
	uint16_t Load_PhV_phsC;//217	R	负载C相电压	UINT16	0.1V		Load_PhV_phsC
	uint16_t Load_PhV_phsN;//218	R	负载N相电压	UINT16	0.1V		Load_PhV_phsN
	int16_t Load_A_phsA;//219	R	负载A相电流	INT16	0.1A		Load_A_phsA
	int16_t Load_A_phsB;//220	R	负载B相电流	INT16	0.1A		Load_A_phsB
	int16_t Load_A_phsC;//221	R	负载C相电流	INT16	0.1A		Load_A_phsC
	int16_t Load_A_phsN;//222	R	负载N相电流	INT16	0.1A		Load_A_phsN
	uint16_t Load_Hz;//223	R	负载频率	UINT16	0.01Hz		Load_Hz
	uint16_t Load_TotW;//224	R	负载总有功功率	UINT16	0.1kW		Load_TotW
	uint16_t Load_TotVAr;//225	R	负载总无功功率	UINT16	0.1kVar		Load_TotVAr
	uint16_t Load_TotVA;//226	R	负载总视在功率	UINT16	0.1kVA		Load_TotVA
	uint16_t Output_PhV_phsA;//227	R	输出A相电压	UINT16	0.1V		Output_PhV_phsA
	uint16_t Output_PhV_phsB;//228	R	输出B相电压	UINT16	0.1V		Output_PhV_phsB
	uint16_t Output_PhV_phsC;//229	R	输出C相电压	UINT16	0.1V		Output_PhV_phsC
	uint16_t Output_PhV_phsN;//230	R	输出N相电压	UINT16	0.1V		Output_PhV_phsN
	int16_t Output_A_phsA;//231	R	输出A相电流	INT16	0.1A		Output_A_phsA
	int16_t Output_A_phsB;//232	R	输出B相电流	INT16	0.1A		Output_A_phsB
	int16_t Output_A_phsC;//233	R	输出C相电流	INT16	0.1A		Output_A_phsC
	int16_t Output_A_phsN;//234	R	输出N相电流	INT16	0.1A		Output_A_phsN
	uint16_t Output_Hz;//235	R	输出频率	UINT16	0.01Hz		Output_Hz
	uint16_t Output_TotW;//236	R	输出总有功功率	UINT16	0.1kW		Output_TotW
	uint16_t Output_TotVAr;//237	R	输出总无功功率	UINT16	0.1kVar		Output_TotVAr
	uint16_t Output_TotVA;//238	R	输出总视在功率	UINT16	0.1kVA		Output_TotVA
	uint16_t DCTolBus_Volt;//239	R	直流侧总母线电压 	UINT16	0.1V		DCTolBus_Volt
	uint16_t DCUpBus_Volt;//240	R	直流上母线电压	UINT16	0.1V		DCUpBus_Volt
	uint16_t DCDownBus_Volt;//241	R	直流下母线电压	UINT16	0.1V		DCDownBus_Volt
	int16_t DC_Cur;//242	R	直流侧电流 	INT16	0.1A		DC_Cur
	uint16_t DC_ActPower;//243	R	直流侧功率 	UINT16	0.1kW		DC_ActPower
	uint16_t IGBT_LowestTmp;//244	R	IGBT最低温度	UINT16	0.1°C	多个IGBT器件温度，显示最低的温度	IGBT_LowestTmp
	uint16_t IGBT_HighestTmp;//245	R	IGBT最高温度	UINT16	0.1°C	多个IGBT器件温度，显示最高的温度	IGBT_HighestTmp
	int16_t Env_Tmp;//246	R	环境温度	INT16	0.1°C	设备上读到的环境温度	Env_Tmp
	uint16_t PCS_Status1;//247	R	PCS状态1	UINT16		"BIT7:电网电压过压/BIT6:电网电压欠压/BIT5:电网电压相序错误/BIT4:DC母线电压异常/BIT3:输出电流异常
							//BIT2:过温故障/BIT1:通讯故障/BIT0:急停按钮"	PCS_Status1
	uint16_t PCS_Status2;//248	R	PCS状态2	UINT16			PCS_Status2
	uint16_t rsvd1[21];//-269		预留

	//write
	uint16_t PCS_RunMode_Set;//270	R/W	PCS运行模式设置	UINT16		A恒压模式，B恒流模式，C恒功率模式,D稳压限流模式	PCS_RunMode_Set
	uint16_t PCS_DCSide_Volt_Set;//271	R/W	直流侧母线电压值设置	UINT16	0.1V	直流电压目标值设置，如700/750等	PCS_DCSide_Volt_Set
	int16_t PCS_DCSide_Cur_Set;//272	R/W	直流侧电流值设置	INT16	0.1A	直流侧电流值设置，正负可表示方向，正表示输入，负表示返送	PCS_DCSide_Cur_Set
	int16_t PCS_DCSide_Power_Set;//273	R/W	直流侧功率值设置 	INT16	0.1KW	直流侧输入或输出功率设置，正负可表示有方向	PCS_DCSide_Power_Set
	uint16_t PCS_DCBusVoltProVal_Set;//274	R/W	直流母线电压保护值设置	UINT16	0.1V	直流侧母线电压保护值上限	PCS_DCBusVoltProVal_Set
	int16_t PCS_DCSide_CurLim_Set;//275	R/W	直流侧电流限流值设置	INT16	0.1A	直流侧电流超过限流值则按照限流值下发	PCS_DCSide_CurLim_Set
	int16_t PCS_DCSide_PowerLim_Set;//276	R/W	直流侧功率限流值设置	INT16	0.1KW		PCS_DCSide_PowerLim_Set
	uint16_t PCS_DCSideCurProVal_Set;//277	R/W	直流侧电流保护值设置	UINT16	0.1A		PCS_DCSideCurProVal_Set
	uint16_t PCS_DCSidePowerProVal_Set;//278	R/W	直流侧功率保护值设置	UINT16	0.1KW		PCS_DCSidePowerProVal_Set
	uint16_t rsvd2[22];//-300	R/W	预留				

};

struct DATA_TTU_STAC
{
	uint16_t STAC_BatSide_Volt;//301	R	电池侧电压	UINT16	0.1V	模块电池侧接口电压	STAC_BatSide_Volt
	int16_t STAC_BatSide_Cur;//302	R	电池侧电流	INT16 	0.1A	模块电池侧接口电流，应具有符号表示充/放	STAC_BatSide_Cur
	int16_t STAC_BatSide_ActPower;//303	R	电池侧功率	INT16 	0.1kW	模块电池侧接口充/放电功率	STAC_BatSide_ActPower
	uint16_t STAC_BusSide_Volt;//304	R	母线侧电压	UINT16	0.1V	模块直流母线侧接口电压	STAC_BusSide_Volt
	int16_t STAC_BusSide_Cur;//305	R	母线侧电流	INT16 	0.1A	模块直流母线侧接口电流	STAC_BusSide_Cur
	int16_t STAC_BusSide_ActPower;//306	R	母线侧功率	INT16	0.1kW	模块直流母线侧接口充/放电功率	STAC_BusSide_ActPower
	uint16_t STAC_CurDevStatus;//307	R	当前开关机状态	UINT16			STAC_CurDevStatus
	uint16_t STAC_CurRunMode;//308	R	当前运行模式	UINT16			STAC_CurRunMode
	uint16_t STAC_IGBT_LowestTmp;//309	R	IGBT最低温度	UINT16	0.1°C	多个IGBT器件温度，显示最低的温度	STAC_IGBT_LowestTmp
	uint16_t STAC_IGBT_HighestTmp;//310	R	IGBT最高温度	UINT16	0.1°C	多个IGBT器件温度，显示最高的温度	STAC_IGBT_HighestTmp
	int16_t STAC_EnvTmp;//311	R	环境温度	INT16	0.1°C	设备上读到的环境温度	STAC_EnvTmp
	uint16_t STAC_FaultMsg;//312	R	故障信息	UINT16		BIT5:电池电压高超限/BIT4电池电压低超限/BIT3:电池反接/BIT2:电流超限/BIT1:母线中点不平衡/BIT0:过温故障	STAC_FaultMsg
	uint16_t rsvd1[8];//-320	R	预留				
	uint16_t STAC_OnOff_Set;//321	R/W	开关机设置	UINT16		开关机，写AAAA开机，写其它关机	STAC_OnOff_Set
	uint16_t STAC_RunMode_Set;//322	R/W	运行模式设置	UINT16		更改运行模式，1为恒压限流模式，4为恒压模式	STAC_RunMode_Set
	uint16_t STAC_HVSideVolt_Set;//323	R/W	高压侧电压设置	UINT16	0.1V	母线侧的电压设置，一般设置750V	STAC_HVSideVolt_Set
	int16_t STAC_LVSidecur_Set;//324	R/W	低压侧电流设置	INT16	0.1A	电池侧的电流设置，即低压电流设置	STAC_LVSidecur_Set
	uint16_t STAC_325;//325	R/W	高压侧上稳压电压值	UINT16	0.1V		STAC_325
	uint16_t STAC_326;//326	R/W	高压侧下稳压电压值	UINT16	0.1V		STAC_326
	uint16_t STAC_327;//327	R/W	高压侧上保护电压值	UINT16	0.1V		STAC_327
	uint16_t STAC_328;//328	R/W	高压侧下保护电压值	UINT16	0.1V		STAC_328
	uint16_t STAC_329;//329	R/W	低压侧上保护电压值	UINT16	0.1V		STAC_329
	uint16_t STAC_330;//330	R/W	低压侧下保护电压值	UINT16	0.1V		STAC_330
	uint16_t STAC_331;//331	R/W	高压侧电流保护值	UINT16	0.1A		STAC_331
	uint16_t STAC_332;//332	R/W	低压侧电流保护值	UINT16	0.1A		STAC_332
	uint16_t rsvd2[8];//-340		预留				

};

struct DATA_TTU_PVAC
{
	uint16_t PVAC_PVSide_Volt;//341	R	光伏侧电压	UINT16	0.1V	模块光伏侧接口电压	PVAC_PVSide_Volt
	int16_t PVAC_PVSide_Cur;//342	R	光伏侧电流	INT16 	0.1A	模块光伏侧接口电流，应具有符号表示充/放	PVAC_PVSide_Cur
	int16_t PVAC_PVSide_ActPower;//343	R	光伏侧功率	INT16 	0.1kW	模块光伏侧接口充/放电功率	PVAC_PVSide_ActPower
	uint16_t PVAC_BusSide_Volt;//344	R	母线侧电压	UINT16	0.1V	模块直流母线侧接口电压	PVAC_BusSide_Volt
	int16_t PVAC_BusSide_Cur;//345	R	母线侧电流	INT16 	0.1A	模块直流母线侧接口电流	PVAC_BusSide_Cur
	int16_t PVAC_BusSide_ActPower;//346	R	母线侧功率	INT16	0.1kW	模块直流母线侧接口充/放电功率	PVAC_BusSide_ActPower
	uint16_t PVAC_CurDevStatus;//347	R	当前开关机状态	UINT16			PVAC_CurDevStatus
	uint16_t PVAC_CurRunMode;//348	R	当前运行模式	UINT16			PVAC_CurRunMode
	uint16_t PVAC_IGBT_LowestTmp;//349	R	IGBT最低温度	UINT16	0.1°C	多个IGBT器件温度，显示最低的温度	PVAC_IGBT_LowestTmp
	uint16_t PVAC_IGBT_HighestTmp;//350	R	IGBT最高温度	UINT16	0.1°C	多个IGBT器件温度，显示最高的温度	PVAC_IGBT_HighestTmp
	int16_t PVAC_EnvTmp;//351	R	环境温度	INT16	0.1°C	设备上读到的环境温度	PVAC_EnvTmp
	uint16_t PVAC_FaultMsg;//352	R	故障信息	UINT16		BIT5:光伏电压高超限/BIT4：光伏电压低超限/BIT3:光伏反接/BIT2:电流超限/BIT1:母线中点不平衡/BIT0:过温故障	PVAC_FaultMsg
	uint32_t PVAC_DayWh;//353	R	日发电量	UINT32	0.1kwh		PVAC_DayWh						
	uint32_t PVAC_MonWh;//355	R	月发电量	UINT32	0.1kwh		PVAC_MonWh						
	uint32_t PVAC_YearWh;//357	R	年发电量	UINT32	0.1kwh		PVAC_YearWh						
	uint16_t rsvd1[13];//-370		预留				
	uint16_t PVAC_371;//371	R/W	开关机设置	UINT16		操作模块开关机，写AAAA开机，写其它关机	
	uint16_t PVAC_372;//372	R/W	运行模式设置	UINT16		A:光伏模式、MPPT模式	
	uint16_t PVAC_373;//373	R/W	母线侧电压设置	UINT16	0.1V	母线侧的电压设置	
	int16_t PVAC_374;//374	R/W	光伏侧电流设置	INT16	0.1A	光伏侧的电流设置，即低压电流设置，用于限功率设置	
	uint16_t PVAC_375;//375	R/W	母线侧保护电压值	UINT16	0.1V		
	uint16_t PVAC_376;//376	R/W	光伏侧保护电压值	UINT16	0.1V		
	uint16_t PVAC_377;//377	R/W	光伏侧下保护电压值	UINT16	0.1V		
	uint16_t PVAC_378;//378	R/W	母线侧电流保护值	UINT16	0.1A		
	uint16_t PVAC_379;//379	R/W	光伏侧电流保护值	UINT16	0.1A		
	uint16_t rsvd2[11];//-390	R/W	预留							
};

struct DATA_TTU_DCPW
{
	uint16_t DCPW_Runstatus;//391	R	运行状态	UINT16			DCPW_Runstatus
	uint16_t DCPW_OutputVolt;//392	R	输出电压	UINT16	0.1V		DCPW_OutputVolt
	uint16_t DCPW_OutputCur;//393	R	输出电流	UINT16	0.1A		DCPW_OutputCur
	uint16_t DCPW_HighestVolt_FixedVal;//394	R	最高电压设定值	UINT16	0.1V		DCPW_HighestVolt_FixedVal
	uint16_t DCPW_LowestVolt_FixedVal;//395	R	最低电压设定值	UINT16	0.1V		DCPW_LowestVolt_FixedVal
	uint16_t DCPW_MaximumCur_FixedVal;//396	R	最大电流设定值	UINT16	0.1A		DCPW_MaximumCur_FixedVal
	uint16_t DCPW_Run;//397	R/W	开关机设置	UINT16		AAAA开机，其它关机	
	uint16_t DCPW_VdcSet;//398	R/W	输出电压设定值	UINT16	0.1V	最高400	
	uint16_t DCPW_ViSet;//399	R/W	输入电流设定值	UINT16	0.1A	最大30	
	uint16_t rsvd1[11];//-410	R/W	预留				
};

struct DATA_TTU_STBAT
{
	uint16_t STBAT_TolVolt;//411	R	总电压	UINT16			STBAT_TolVolt
	int16_t STBAT_Cur;//412	R	电流	INT16			STBAT_Cur
	int16_t STBAT_Tmp;//413	R	温度	INT16			STBAT_Tmp
	uint16_t STBAT_SOC;//414	R	SOC	UINT16			STBAT_SOC
	uint16_t STBAT_Loop;//415	R	循环次数	UINT16			STBAT_Loop
	uint16_t STBAT_MaximumChargVolt;//416	R	允许最大充电电压值	UINT16			STBAT_MaximumChargVolt
	uint16_t STBAT_MaximumChargCur;//417	R	允许最大充电电流值	UINT16			STBAT_MaximumChargCur
	uint16_t STBAT_MinimumDischVolt;//418	R	允许最小放电电压值	UINT16			STBAT_MinimumDischVolt
	uint16_t STBAT_MaximumDischCur;//419	R	允许最大放电电流值	UINT16			STBAT_MaximumDischCur
	uint16_t STBAT_BatHighestVolt;//420	R	电池最高电压	UINT16			STBAT_BatHighestVolt
	uint16_t STBAT_BatLowestVolt;//421	R	电池最低电压	UINT16			STBAT_BatLowestVolt
	int16_t STBAT_BatHighestTmp;//422	R	电池最高温度	INT16			STBAT_BatHighestTmp
	int16_t STBAT_BatLowestTmp;//423	R	电池最低温度	INT16			STBAT_BatLowestTmp
	uint16_t STBAT_SOH;//424	R	SOH	UINT16			STBAT_SOH
	uint32_t STBAT_ResCap;//425	R	剩余容量	UINT32			STBAT_ResCap						
	uint32_t STBAT_BatChargCap;//427	R	电池充电量	UINT32			STBAT_BatChargCap						
	uint32_t STBAT_BatDisChCap;//429	R	电池放电量	UINT32			STBAT_BatDisChCap						
	uint32_t STBAT_DayBatChargSum;//431	R	当日电池累积充电量	UINT32			STBAT_DayBatChargSum						
	uint32_t STBAT_DayBatDisChSum;//433	R	当日电池累积放电量	UINT32			STBAT_DayBatDisChSum						
	uint32_t STBAT_HisBatChargSum;//435	R	历史累积充电量	UINT32			STBAT_HisBatChargSum						
	uint32_t STBAT_HisBatDisChSum;//437	R	历史累积放电量	UINT32			STBAT_HisBatDisChSum						
	uint16_t STBAT_BalaChargFlg;//439	R	均衡充电标志位	UINT16			STBAT_BalaChargFlg
	uint16_t STBAT_BanChargFlg;//440	R	禁止充电标志位	UINT16			STBAT_BanChargFlg
	uint16_t STBAT_BanDischFlg;//441	R	禁止放电标志位	UINT16			STBAT_BanDischFlg
	uint16_t STBAT_BasicStatus;//442		基本状态	UINT16		"Bit15：单体二级欠压保护  0-正常； 1-保护
	//Bit14：模块高压保护 0-正常； 1-保护Bit13：模块低压保护 0-正常； 1-保护Bit12：模块温度高保护 0-正常； 1-保护Bit12：预留
	//Bit10：短路保护 0-正常； 1-保护Bit9：放电过流保护 0-正常； 1-保护Bit8：充电过流保护 0-正常； 1-保护Bit7：放电高温保护 0-正常； 1-保护Bit6：放电低温保护 0-正常； 1-保护
	//Bit5：充电高温保护 0-正常； 1-保护Bit4：充电低温保护 0-正常； 1-保护Bit3：总压高压保护 0-正常； 1-保护Bit2：总压低压保护 0-正常； 1-保护
	//Bit1：单体高压保护 0-正常； 1-保护Bit0：单体低压保护 0-正常； 1-保护"	STBAT_BasicStatus
	uint16_t STBAT_ProtectMsg;//443		保护信息	UINT16		"BIT15:保留
	//Bit14：风扇异常告警 0-正常， 1-有异常Bit13：电池组休眠状态 0-否， 1-休眠Bit12：电池组放电状态 0-否， 1-放电
	//Bit11：电池组充电状态 0-否， 1-充电Bit10：系统空闲状态 0-null;1-空闲Bit9：温度告警 0-正常； 1-告警
	//Bit8：电流告警 0-正常； 1-告警Bit7： 电压告警 0-正常； 1-告警Bit6：温度保护 0-正常； 1-保护Bit5：电压保护 0-正常； 1-保护
	//Bit4：电流保护 0-正常； 1-保护Bit3：系统故障保护 0-正常； 1-保护Bit2-BIT0:基本状态 00： 休眠 01： 充电 02： 放电 03： 搁置 04~07： 保留"	STBAT_ProtectMsg
	uint16_t STBAT_WarnMsg;//444		告警信息	UINT16		"BIT15：端子温度异常告警 0-正常； 1-告警
	//Bit14：模块高压告警 0-正常； 1-告警Bit13：模块低压告警 0-正常； 1-告警Bit12：模块温度高告警 0-正常； 1-告警
	//Bit11：主控温度高告警 0-正常； 1-告警Bit10：预留Bit9：放电过流告警 0-正常； 1-告警Bit8：充电过流告警 0-正常； 1-告警
	//Bit7：放电高温告警 0-正常； 1-告警Bit6：放电低温告警 0-正常； 1-告警Bit5：充电高温告警 0-正常； 1-告警
	//Bit4：充电低温告警 0-正常； 1-告警Bit3：总压高压告警 0-正常； 1-告警Bit2：总压低压告警 0-正常； 1-告警
	//Bit1：单体高压告警  0-正常； 1-告警Bit0：单体低压告警  0-正常； 1-告警"	STBAT_WarnMsg
	uint16_t rsvd1[6];//-450		预留	UINT16			
};

struct DATA_TTU_ACPW
{
	uint16_t ACPW_Output_A_phsA;//451	R	输出A相电流	UINT16 	0.1A	输出A相电流	ACPW_Output_A_phsA
	uint16_t ACPW_Output_A_phsB;//452	R	输出B相电流	UINT16 	0.1A	输出B相电流	ACPW_Output_A_phsB
	uint16_t ACPW_Output_A_phsC;//453	R	输出C相电流	UINT16 	0.1A	输出C相电流	ACPW_Output_A_phsC
	uint16_t ACPW_Output_PhV_phsA;//454	R	输出A相电压	UINT16 	0.1V	输出A相电压	ACPW_Output_PhV_phsA
	uint16_t ACPW_Output_PhV_phsB;//455	R	输出B相电压	UINT16 	0.1V	输出B相电压	ACPW_Output_PhV_phsB
	uint16_t ACPW_Output_PhV_phsC;//456	R	输出C相电压	UINT16 	0.1V	输出C相电压	ACPW_Output_PhV_phsC
	uint16_t ACPW_Output_DCOutput;//457	R	直流电压	UINT16 	0.1V	直流电压	ACPW_Output_DCOutput
	uint16_t rsvd1;//458	R	保留	UINT16 	0.1V	保留	
	uint16_t ACPW_Output_PhW_phsA;//459	R	输出A相有功功率	UINT16 	0.1kW	输出A相有功功率	ACPW_Output_PhW_phsA
	uint16_t ACPW_Output_PhW_phsB;//460	R	输出B相有功功率	UINT16 	0.1kW	输出B相有功功率	ACPW_Output_PhW_phsB
	uint16_t ACPW_Output_PhW_phsC;//461	R	输出C相有功功率	UINT16 	0.1kW	输出C相有功功率	ACPW_Output_PhW_phsC
	uint16_t ACPW_Output_TotW;//462	R	输出三相总有功	UINT16 	0.1kW	输出三相总有功	ACPW_Output_TotW
	int16_t ACPW_Output_TotVar;//463	R	输出三相总无功	INT16 	0.1kVar	输出三相总无功	ACPW_Output_TotVar
	uint16_t ACPW_Output_TotVA;//464	R	输出三相总视在功率	UINT16 	0.001	输出三相总视在功率	ACPW_Output_TotVA
	uint16_t ACPW_TotPF;//465	R	三相平均功率因数	UINT16 	0.001	三相平均功率因数	ACPW_TotPF
	uint16_t ACPW_A_neut;//466	R	零线电流	UINT16 	0.1A	零线电流	ACPW_A_neut
	uint16_t ACPW_Status_And_Protect;//467	R	状态和保护	UINT16 		"bit8：输出电流保护/bit7：温度保护/bit6：IGBT保护bit5：VDC保护/bit4: VS保护/bit3：系统自检状态
					//bit2：接触器状态/bit1：PWM控制状态/bit0:开机状态 "	ACPW_Status_And_Protect
	uint16_t rsvd2[3];//-470	R		UINT16 			
	uint16_t ACPW_Flag;//471	R/W	通过串口设置参数的确认标志	UINT16 	1	通过串口设置参数的确认标志	
	uint16_t ACPW_LimitI;//472	R/W	输出电流限幅值	UINT16 	0.1A	输出电流限幅值	
	uint16_t ACPW_ProtectI;//473	R/W	输出电流保护值	UINT16 	0.1A	输出电流保护值	
	uint16_t ACPW_VS_ProtectHigh;//474	R/W	VS上保护值	UINT16 	0.1V	VS上保护值	
	uint16_t ACPW_VS_ProtectLow;//475	R/W	VS下保护值	UINT16 	0.1V	VS下保护值	
	uint16_t ACPW_RunOff;//476	R/W	开关机命令	UINT16 	1	开关及指令 HEX AAAA开机 其它关机	
	uint16_t ACPW_Output;//477	R/W	VS输出电压设置	UINT16 	0.1V	VS输出电压设置	
	uint16_t rsvd3[3];//-480	R/W	预留				
};

struct DATA_TTU_CHARGE
{
	uint16_t Plug_Max_OutputVolt;//1000	R	枪最大输出电压	UINT16			Plug_Max_OutputVolt
	uint16_t Plug_Max_OutputCur;//1001	R	枪最大输出电流	UINT16			Plug_Max_OutputCur
	uint16_t Plug_Max_OutputPower;//1002	R	枪最大输出功率	UINT16			Plug_Max_OutputPower
	uint16_t Plug_ConnectStatus;//1003	R	枪连接状态	UINT16		0未连接/1连接	Plug_ConnectStatus
	uint16_t Plug_ChargingStatus;//1004	R	枪当前充电状态	UINT16		"0：初始化状态1：空闲状态2：等待启动3：启动中
								//4：充电中5：停止充电中6：充电结算中7：充电完成未拔枪8：启动失败9：故障状态"	Plug_ChargingStatus
	uint16_t CarBMS_DemandVolt;//1005	R	车BMS需求电压	UINT16			CarBMS_DemandVolt
	uint16_t CarBMS_DemandCur;//1006	R	车BMS需求电流	UINT16			CarBMS_DemandCur
	uint16_t CarBMS_CurSOC;//1007	R	车BMS当前SOC	UINT16			CarBMS_CurSOC
	uint16_t ChargingVolt;//1008	R	当前充电电压	UINT16			ChargingVolt
	uint16_t ChargingCur;//1009	R	当前充电电流	UINT16			ChargingCur
	uint16_t ChargingPower;//1010	R	当前充电功率	UINT16			ChargingPower
	uint16_t ChargedEnergy;//1011	R	本次充电电量	UINT16			ChargedEnergy
	uint16_t ChargedTime;//1012	R	本次充电时长	UINT16			ChargedTime
	uint16_t ChargedMoney;//1013	R	本次充电金额	UINT16			ChargedMoney
	uint16_t CurMeterEnergy;//1014	R	当前电表读数	UINT16			CurMeterEnergy
	uint16_t CauseOfChargedEnd;//1015	R	充电结束原因	UINT16		"0：未知原因
								//1：APP停止2：刷卡停止3：BMS正常停止8：网络中断9：急停故障10：门禁故障"	CauseOfChargedEnd
	uint16_t ReadedPowerCtrl;//1016	R	读取功率控制	UINT16			ReadedPowerCtrl
	uint16_t ReadedOnOffCtrl;//1017	R	读取启停控制	UINT16			ReadedOnOffCtrl
	uint16_t FaultStatus1;//1018	R	故障状态01	UINT16		后边确定	FaultStatus1
	uint16_t FaultStatus2;//1019	R	故障状态02	UINT16		后边确定	FaultStatus2
	uint16_t rsvd1;//1020	R	预留	UINT16			
	uint16_t PowerCtrl_Set;//1021	R/W	功率控制	UINT16		绝对值控制 60000代表60kw	PowerCtrl_Set
	uint16_t StartOrStop_Set;//1022	R/W	启停控制	UINT16		01:启动充电 02：停止充电	StartOrStop_Set
	uint16_t rsvd2;//1023	R/W	预留	UINT16			
};
struct DATA_TTU_METER
{
	uint32_t	Ua; //相电压
	uint32_t	Ub; 
	uint32_t	Uc;
	uint32_t	Ia; //相电流
	uint32_t	Ib; 
	uint32_t	Ic;	
	uint32_t	Ps; 
	uint32_t	Qs;
	uint32_t	VAs;
	uint32_t	PFs;
	uint32_t	KWH[3]; //电能	
	uint16_t    Harm_U[3]; //谐波
	uint16_t    Harm_I[3]; //
	
};

struct DATA_TTU_PVClass {
	int32_t		P; //有功功率
	uint32_t	P_DC; //总直流输出功率
	uint32_t	Power_total;//总发电量
	uint32_t	Power_month; //当月发电量
	uint32_t	Power_lastmonth; //上月发电量
	uint16_t	Power_day;   //当日发电量
};

struct DATA_TTU
{
	struct DATA_TTU_EMS     DATA_TTU_ems;	
	struct DATA_TTU_PCS     DATA_TTU_pcs;
	struct DATA_TTU_STAC    DATA_TTU_stac;
	struct DATA_TTU_PVAC    DATA_TTU_pvac;
	struct DATA_TTU_DCPW    DATA_TTU_dcpw;
	struct DATA_TTU_STBAT   DATA_TTU_stbat;
	struct DATA_TTU_ACPW    DATA_TTU_acpw;
	struct DATA_TTU_CHARGE  DATA_TTU_charge;
	struct DATA_TTU_METER   DATA_TTU_meter;
	struct DATA_TTU_PVClass   DATA_TTU_pvclass;
};
#pragma pack()

extern struct DATA_TTU DATA_ttu;
extern float PVAC_Power[3];


