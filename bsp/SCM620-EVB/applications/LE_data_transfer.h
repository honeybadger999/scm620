#pragma pack(2)
struct DATA_TTU_EMS 
{
	uint16_t PCS_Fault;//100	R	PCSģ�����	UINT16		ϵͳ������	PCS_Fault
	uint16_t DCStore_Fault;//101	R	DC����ģ�����	UINT16		ϵͳ������	DCStore_Fault
	uint16_t DCPV_Fault;//102	R	DC���ģ�����	UINT16		ϵͳ������	DCPV_Fault
	uint16_t DCAC_Fault;//103	R	DCACģ�����	UINT16		ϵͳ������	DCAC_Fault
	uint16_t Battery_Fault;//104	R	��ع���	UINT16		ϵͳ������	Battery_Fault
	uint16_t AutoModeOn_Excep;//105	R	�Զ�ģʽ�����쳣��־	UINT16		70 ��FAULT/FAIL 80��PASS	AutoModeOn_Excep
	uint16_t SysVSVolt_Excep;//106	R	ϵͳVS��ѹ�쳣��־	UINT16		70 ��FAULT/FAIL 80��PASS	SysVSVolt_Excep
	uint16_t BusVDCVolt_Excep;//107	R	ĸ��VDC��ѹ�쳣��־	UINT16		70 ��FAULT/FAIL 80��PASS	BusVDCVolt_Excep
	uint16_t MasterDevCommu_Err;//108	R	����ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	MasterDevCommu_Err
	uint16_t PCSCommu_Err;//109	R	PCSͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	PCSCommu_Err
	uint16_t DC1Commu_Err;//110	R	DC1ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	DC1Commu_Err
	uint16_t DC2Commu_Err;//111	R	DC2ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	DC2Commu_Err
	uint16_t BatCommu_Err;//112	R	���ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	BatCommu_Err
	uint16_t DCACCommu_Err;//113	R	DCACͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	DCACCommu_Err
	uint16_t ChargingPileCommu_Err;//114	R	���׮ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	ChargingPileCommu_Err
	uint16_t rsvd1[5];					
	uint16_t AutoModeOn_Status;//120	R	�Զ�ģʽ����״̬	UINT16		1Ϊ����  0Ϊ�ػ�	AutoModeOn_Status
	uint16_t CurRunMode;//121	R	��ǰ����ģʽ	UINT16			CurRunMode
	uint16_t rsvd2[28];//-149		Ԥ��
	
	//write
	uint16_t EMS_Mode_Set;//150	R/W	�Զ�/�ֶ�ģʽ����	UINT16		A���Զ����ػ�ģʽ �������ֶ�ģʽ	EMS_Mode_Set
	uint16_t EMS_OnOff_Set;//151	R/W	���ػ�����	UINT16		AAAA�����������ػ�	EMS_OnOff_Set
	uint16_t EMS_OffConnectGrid_Set;//152	R/W	�벢������	UINT16		�ֶ������벢�����ƣ�PCSӦ�þ߱��������	EMS_OffConnectGrid_Set
	uint16_t rsvd3[48];//-199		Ԥ��								
};

struct DATA_TTU_PCS 
{
	uint16_t UAB;//200	R	����AB�ߵ�ѹ	UINT16	0.1V		UAB
	uint16_t UBC;//201	R	����BC�ߵ�ѹ	UINT16	0.1V		UBC
	uint16_t UAC;//202	R	����AC�ߵ�ѹ	UINT16	0.1V		UAC
	uint16_t PhV_phsA;//203	R	����A���ѹ	UINT16	0.1V		PhV_phsA
	uint16_t PhV_phsB;//204	R	����B���ѹ	UINT16	0.1V		PhV_phsB
	uint16_t PhV_phsC;//205	R	����C���ѹ	UINT16	0.1V		PhV_phsC
	uint16_t PhV_PhsN;//206	R	����N���ѹ	UINT16	0.1V		PhV_PhsN
	int16_t  A_phsA;//207	R	����A�����	INT16	0.1A		A_phsA
	int16_t A_phsB;//208	R	����B�����	INT16	0.1A		A_phsB
	int16_t A_phsC;//209	R	����C�����	INT16	0.1A		A_phsC
	int16_t A_phsN;//210	R	����N�����	INT16	0.1A		A_phsN
	uint16_t Hz;//211	R	����Ƶ��	UINT16	0.01Hz		Hz
	int16_t TotW;//212	R	�������й�����	UINT16	0.1kW	�人PCS�Ͱ�����¾��޵��๦�ʼ���������	TotW
	int16_t TotVAr;//213	R	�������޹�����	UINT16	0.1kVar		TotVAr
	uint16_t TotVA;//214	R	���������ڹ���	UINT16	0.1kVA		TotVA
	uint16_t Load_PhV_phsA;//215	R	����A���ѹ	UINT16	0.1V		Load_PhV_phsA
	uint16_t Load_PhV_phsB;//216	R	����B���ѹ	UINT16	0.1V		Load_PhV_phsB
	uint16_t Load_PhV_phsC;//217	R	����C���ѹ	UINT16	0.1V		Load_PhV_phsC
	uint16_t Load_PhV_phsN;//218	R	����N���ѹ	UINT16	0.1V		Load_PhV_phsN
	int16_t Load_A_phsA;//219	R	����A�����	INT16	0.1A		Load_A_phsA
	int16_t Load_A_phsB;//220	R	����B�����	INT16	0.1A		Load_A_phsB
	int16_t Load_A_phsC;//221	R	����C�����	INT16	0.1A		Load_A_phsC
	int16_t Load_A_phsN;//222	R	����N�����	INT16	0.1A		Load_A_phsN
	uint16_t Load_Hz;//223	R	����Ƶ��	UINT16	0.01Hz		Load_Hz
	uint16_t Load_TotW;//224	R	�������й�����	UINT16	0.1kW		Load_TotW
	uint16_t Load_TotVAr;//225	R	�������޹�����	UINT16	0.1kVar		Load_TotVAr
	uint16_t Load_TotVA;//226	R	���������ڹ���	UINT16	0.1kVA		Load_TotVA
	uint16_t Output_PhV_phsA;//227	R	���A���ѹ	UINT16	0.1V		Output_PhV_phsA
	uint16_t Output_PhV_phsB;//228	R	���B���ѹ	UINT16	0.1V		Output_PhV_phsB
	uint16_t Output_PhV_phsC;//229	R	���C���ѹ	UINT16	0.1V		Output_PhV_phsC
	uint16_t Output_PhV_phsN;//230	R	���N���ѹ	UINT16	0.1V		Output_PhV_phsN
	int16_t Output_A_phsA;//231	R	���A�����	INT16	0.1A		Output_A_phsA
	int16_t Output_A_phsB;//232	R	���B�����	INT16	0.1A		Output_A_phsB
	int16_t Output_A_phsC;//233	R	���C�����	INT16	0.1A		Output_A_phsC
	int16_t Output_A_phsN;//234	R	���N�����	INT16	0.1A		Output_A_phsN
	uint16_t Output_Hz;//235	R	���Ƶ��	UINT16	0.01Hz		Output_Hz
	uint16_t Output_TotW;//236	R	������й�����	UINT16	0.1kW		Output_TotW
	uint16_t Output_TotVAr;//237	R	������޹�����	UINT16	0.1kVar		Output_TotVAr
	uint16_t Output_TotVA;//238	R	��������ڹ���	UINT16	0.1kVA		Output_TotVA
	uint16_t DCTolBus_Volt;//239	R	ֱ������ĸ�ߵ�ѹ 	UINT16	0.1V		DCTolBus_Volt
	uint16_t DCUpBus_Volt;//240	R	ֱ����ĸ�ߵ�ѹ	UINT16	0.1V		DCUpBus_Volt
	uint16_t DCDownBus_Volt;//241	R	ֱ����ĸ�ߵ�ѹ	UINT16	0.1V		DCDownBus_Volt
	int16_t DC_Cur;//242	R	ֱ������� 	INT16	0.1A		DC_Cur
	uint16_t DC_ActPower;//243	R	ֱ���๦�� 	UINT16	0.1kW		DC_ActPower
	uint16_t IGBT_LowestTmp;//244	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��͵��¶�	IGBT_LowestTmp
	uint16_t IGBT_HighestTmp;//245	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��ߵ��¶�	IGBT_HighestTmp
	int16_t Env_Tmp;//246	R	�����¶�	INT16	0.1��C	�豸�϶����Ļ����¶�	Env_Tmp
	uint16_t PCS_Status1;//247	R	PCS״̬1	UINT16		"BIT7:������ѹ��ѹ/BIT6:������ѹǷѹ/BIT5:������ѹ�������/BIT4:DCĸ�ߵ�ѹ�쳣/BIT3:��������쳣
							//BIT2:���¹���/BIT1:ͨѶ����/BIT0:��ͣ��ť"	PCS_Status1
	uint16_t PCS_Status2;//248	R	PCS״̬2	UINT16			PCS_Status2
	uint16_t rsvd1[21];//-269		Ԥ��

	//write
	uint16_t PCS_RunMode_Set;//270	R/W	PCS����ģʽ����	UINT16		A��ѹģʽ��B����ģʽ��C�㹦��ģʽ,D��ѹ����ģʽ	PCS_RunMode_Set
	uint16_t PCS_DCSide_Volt_Set;//271	R/W	ֱ����ĸ�ߵ�ѹֵ����	UINT16	0.1V	ֱ����ѹĿ��ֵ���ã���700/750��	PCS_DCSide_Volt_Set
	int16_t PCS_DCSide_Cur_Set;//272	R/W	ֱ�������ֵ����	INT16	0.1A	ֱ�������ֵ���ã������ɱ�ʾ��������ʾ���룬����ʾ����	PCS_DCSide_Cur_Set
	int16_t PCS_DCSide_Power_Set;//273	R/W	ֱ���๦��ֵ���� 	INT16	0.1KW	ֱ�������������������ã������ɱ�ʾ�з���	PCS_DCSide_Power_Set
	uint16_t PCS_DCBusVoltProVal_Set;//274	R/W	ֱ��ĸ�ߵ�ѹ����ֵ����	UINT16	0.1V	ֱ����ĸ�ߵ�ѹ����ֵ����	PCS_DCBusVoltProVal_Set
	int16_t PCS_DCSide_CurLim_Set;//275	R/W	ֱ�����������ֵ����	INT16	0.1A	ֱ���������������ֵ��������ֵ�·�	PCS_DCSide_CurLim_Set
	int16_t PCS_DCSide_PowerLim_Set;//276	R/W	ֱ���๦������ֵ����	INT16	0.1KW		PCS_DCSide_PowerLim_Set
	uint16_t PCS_DCSideCurProVal_Set;//277	R/W	ֱ�����������ֵ����	UINT16	0.1A		PCS_DCSideCurProVal_Set
	uint16_t PCS_DCSidePowerProVal_Set;//278	R/W	ֱ���๦�ʱ���ֵ����	UINT16	0.1KW		PCS_DCSidePowerProVal_Set
	uint16_t rsvd2[22];//-300	R/W	Ԥ��				

};

struct DATA_TTU_STAC
{
	uint16_t STAC_BatSide_Volt;//301	R	��ز��ѹ	UINT16	0.1V	ģ���ز�ӿڵ�ѹ	STAC_BatSide_Volt
	int16_t STAC_BatSide_Cur;//302	R	��ز����	INT16 	0.1A	ģ���ز�ӿڵ�����Ӧ���з��ű�ʾ��/��	STAC_BatSide_Cur
	int16_t STAC_BatSide_ActPower;//303	R	��ز๦��	INT16 	0.1kW	ģ���ز�ӿڳ�/�ŵ繦��	STAC_BatSide_ActPower
	uint16_t STAC_BusSide_Volt;//304	R	ĸ�߲��ѹ	UINT16	0.1V	ģ��ֱ��ĸ�߲�ӿڵ�ѹ	STAC_BusSide_Volt
	int16_t STAC_BusSide_Cur;//305	R	ĸ�߲����	INT16 	0.1A	ģ��ֱ��ĸ�߲�ӿڵ���	STAC_BusSide_Cur
	int16_t STAC_BusSide_ActPower;//306	R	ĸ�߲๦��	INT16	0.1kW	ģ��ֱ��ĸ�߲�ӿڳ�/�ŵ繦��	STAC_BusSide_ActPower
	uint16_t STAC_CurDevStatus;//307	R	��ǰ���ػ�״̬	UINT16			STAC_CurDevStatus
	uint16_t STAC_CurRunMode;//308	R	��ǰ����ģʽ	UINT16			STAC_CurRunMode
	uint16_t STAC_IGBT_LowestTmp;//309	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��͵��¶�	STAC_IGBT_LowestTmp
	uint16_t STAC_IGBT_HighestTmp;//310	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��ߵ��¶�	STAC_IGBT_HighestTmp
	int16_t STAC_EnvTmp;//311	R	�����¶�	INT16	0.1��C	�豸�϶����Ļ����¶�	STAC_EnvTmp
	uint16_t STAC_FaultMsg;//312	R	������Ϣ	UINT16		BIT5:��ص�ѹ�߳���/BIT4��ص�ѹ�ͳ���/BIT3:��ط���/BIT2:��������/BIT1:ĸ���е㲻ƽ��/BIT0:���¹���	STAC_FaultMsg
	uint16_t rsvd1[8];//-320	R	Ԥ��				
	uint16_t STAC_OnOff_Set;//321	R/W	���ػ�����	UINT16		���ػ���дAAAA������д�����ػ�	STAC_OnOff_Set
	uint16_t STAC_RunMode_Set;//322	R/W	����ģʽ����	UINT16		��������ģʽ��1Ϊ��ѹ����ģʽ��4Ϊ��ѹģʽ	STAC_RunMode_Set
	uint16_t STAC_HVSideVolt_Set;//323	R/W	��ѹ���ѹ����	UINT16	0.1V	ĸ�߲�ĵ�ѹ���ã�һ������750V	STAC_HVSideVolt_Set
	int16_t STAC_LVSidecur_Set;//324	R/W	��ѹ���������	INT16	0.1A	��ز�ĵ������ã�����ѹ��������	STAC_LVSidecur_Set
	uint16_t STAC_325;//325	R/W	��ѹ������ѹ��ѹֵ	UINT16	0.1V		STAC_325
	uint16_t STAC_326;//326	R/W	��ѹ������ѹ��ѹֵ	UINT16	0.1V		STAC_326
	uint16_t STAC_327;//327	R/W	��ѹ���ϱ�����ѹֵ	UINT16	0.1V		STAC_327
	uint16_t STAC_328;//328	R/W	��ѹ���±�����ѹֵ	UINT16	0.1V		STAC_328
	uint16_t STAC_329;//329	R/W	��ѹ���ϱ�����ѹֵ	UINT16	0.1V		STAC_329
	uint16_t STAC_330;//330	R/W	��ѹ���±�����ѹֵ	UINT16	0.1V		STAC_330
	uint16_t STAC_331;//331	R/W	��ѹ���������ֵ	UINT16	0.1A		STAC_331
	uint16_t STAC_332;//332	R/W	��ѹ���������ֵ	UINT16	0.1A		STAC_332
	uint16_t rsvd2[8];//-340		Ԥ��				

};

struct DATA_TTU_PVAC
{
	uint16_t PVAC_PVSide_Volt;//341	R	������ѹ	UINT16	0.1V	ģ������ӿڵ�ѹ	PVAC_PVSide_Volt
	int16_t PVAC_PVSide_Cur;//342	R	��������	INT16 	0.1A	ģ������ӿڵ�����Ӧ���з��ű�ʾ��/��	PVAC_PVSide_Cur
	int16_t PVAC_PVSide_ActPower;//343	R	����๦��	INT16 	0.1kW	ģ������ӿڳ�/�ŵ繦��	PVAC_PVSide_ActPower
	uint16_t PVAC_BusSide_Volt;//344	R	ĸ�߲��ѹ	UINT16	0.1V	ģ��ֱ��ĸ�߲�ӿڵ�ѹ	PVAC_BusSide_Volt
	int16_t PVAC_BusSide_Cur;//345	R	ĸ�߲����	INT16 	0.1A	ģ��ֱ��ĸ�߲�ӿڵ���	PVAC_BusSide_Cur
	int16_t PVAC_BusSide_ActPower;//346	R	ĸ�߲๦��	INT16	0.1kW	ģ��ֱ��ĸ�߲�ӿڳ�/�ŵ繦��	PVAC_BusSide_ActPower
	uint16_t PVAC_CurDevStatus;//347	R	��ǰ���ػ�״̬	UINT16			PVAC_CurDevStatus
	uint16_t PVAC_CurRunMode;//348	R	��ǰ����ģʽ	UINT16			PVAC_CurRunMode
	uint16_t PVAC_IGBT_LowestTmp;//349	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��͵��¶�	PVAC_IGBT_LowestTmp
	uint16_t PVAC_IGBT_HighestTmp;//350	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��ߵ��¶�	PVAC_IGBT_HighestTmp
	int16_t PVAC_EnvTmp;//351	R	�����¶�	INT16	0.1��C	�豸�϶����Ļ����¶�	PVAC_EnvTmp
	uint16_t PVAC_FaultMsg;//352	R	������Ϣ	UINT16		BIT5:�����ѹ�߳���/BIT4�������ѹ�ͳ���/BIT3:�������/BIT2:��������/BIT1:ĸ���е㲻ƽ��/BIT0:���¹���	PVAC_FaultMsg
	uint32_t PVAC_DayWh;//353	R	�շ�����	UINT32	0.1kwh		PVAC_DayWh						
	uint32_t PVAC_MonWh;//355	R	�·�����	UINT32	0.1kwh		PVAC_MonWh						
	uint32_t PVAC_YearWh;//357	R	�귢����	UINT32	0.1kwh		PVAC_YearWh						
	uint16_t rsvd1[13];//-370		Ԥ��				
	uint16_t PVAC_371;//371	R/W	���ػ�����	UINT16		����ģ�鿪�ػ���дAAAA������д�����ػ�	
	uint16_t PVAC_372;//372	R/W	����ģʽ����	UINT16		A:���ģʽ��MPPTģʽ	
	uint16_t PVAC_373;//373	R/W	ĸ�߲��ѹ����	UINT16	0.1V	ĸ�߲�ĵ�ѹ����	
	int16_t PVAC_374;//374	R/W	������������	INT16	0.1A	�����ĵ������ã�����ѹ�������ã������޹�������	
	uint16_t PVAC_375;//375	R/W	ĸ�߲ౣ����ѹֵ	UINT16	0.1V		
	uint16_t PVAC_376;//376	R/W	����ౣ����ѹֵ	UINT16	0.1V		
	uint16_t PVAC_377;//377	R/W	������±�����ѹֵ	UINT16	0.1V		
	uint16_t PVAC_378;//378	R/W	ĸ�߲��������ֵ	UINT16	0.1A		
	uint16_t PVAC_379;//379	R/W	������������ֵ	UINT16	0.1A		
	uint16_t rsvd2[11];//-390	R/W	Ԥ��							
};

struct DATA_TTU_DCPW
{
	uint16_t DCPW_Runstatus;//391	R	����״̬	UINT16			DCPW_Runstatus
	uint16_t DCPW_OutputVolt;//392	R	�����ѹ	UINT16	0.1V		DCPW_OutputVolt
	uint16_t DCPW_OutputCur;//393	R	�������	UINT16	0.1A		DCPW_OutputCur
	uint16_t DCPW_HighestVolt_FixedVal;//394	R	��ߵ�ѹ�趨ֵ	UINT16	0.1V		DCPW_HighestVolt_FixedVal
	uint16_t DCPW_LowestVolt_FixedVal;//395	R	��͵�ѹ�趨ֵ	UINT16	0.1V		DCPW_LowestVolt_FixedVal
	uint16_t DCPW_MaximumCur_FixedVal;//396	R	�������趨ֵ	UINT16	0.1A		DCPW_MaximumCur_FixedVal
	uint16_t DCPW_Run;//397	R/W	���ػ�����	UINT16		AAAA�����������ػ�	
	uint16_t DCPW_VdcSet;//398	R/W	�����ѹ�趨ֵ	UINT16	0.1V	���400	
	uint16_t DCPW_ViSet;//399	R/W	��������趨ֵ	UINT16	0.1A	���30	
	uint16_t rsvd1[11];//-410	R/W	Ԥ��				
};

struct DATA_TTU_STBAT
{
	uint16_t STBAT_TolVolt;//411	R	�ܵ�ѹ	UINT16			STBAT_TolVolt
	int16_t STBAT_Cur;//412	R	����	INT16			STBAT_Cur
	int16_t STBAT_Tmp;//413	R	�¶�	INT16			STBAT_Tmp
	uint16_t STBAT_SOC;//414	R	SOC	UINT16			STBAT_SOC
	uint16_t STBAT_Loop;//415	R	ѭ������	UINT16			STBAT_Loop
	uint16_t STBAT_MaximumChargVolt;//416	R	����������ѹֵ	UINT16			STBAT_MaximumChargVolt
	uint16_t STBAT_MaximumChargCur;//417	R	������������ֵ	UINT16			STBAT_MaximumChargCur
	uint16_t STBAT_MinimumDischVolt;//418	R	������С�ŵ��ѹֵ	UINT16			STBAT_MinimumDischVolt
	uint16_t STBAT_MaximumDischCur;//419	R	�������ŵ����ֵ	UINT16			STBAT_MaximumDischCur
	uint16_t STBAT_BatHighestVolt;//420	R	�����ߵ�ѹ	UINT16			STBAT_BatHighestVolt
	uint16_t STBAT_BatLowestVolt;//421	R	�����͵�ѹ	UINT16			STBAT_BatLowestVolt
	int16_t STBAT_BatHighestTmp;//422	R	�������¶�	INT16			STBAT_BatHighestTmp
	int16_t STBAT_BatLowestTmp;//423	R	�������¶�	INT16			STBAT_BatLowestTmp
	uint16_t STBAT_SOH;//424	R	SOH	UINT16			STBAT_SOH
	uint32_t STBAT_ResCap;//425	R	ʣ������	UINT32			STBAT_ResCap						
	uint32_t STBAT_BatChargCap;//427	R	��س����	UINT32			STBAT_BatChargCap						
	uint32_t STBAT_BatDisChCap;//429	R	��طŵ���	UINT32			STBAT_BatDisChCap						
	uint32_t STBAT_DayBatChargSum;//431	R	���յ���ۻ������	UINT32			STBAT_DayBatChargSum						
	uint32_t STBAT_DayBatDisChSum;//433	R	���յ���ۻ��ŵ���	UINT32			STBAT_DayBatDisChSum						
	uint32_t STBAT_HisBatChargSum;//435	R	��ʷ�ۻ������	UINT32			STBAT_HisBatChargSum						
	uint32_t STBAT_HisBatDisChSum;//437	R	��ʷ�ۻ��ŵ���	UINT32			STBAT_HisBatDisChSum						
	uint16_t STBAT_BalaChargFlg;//439	R	�������־λ	UINT16			STBAT_BalaChargFlg
	uint16_t STBAT_BanChargFlg;//440	R	��ֹ����־λ	UINT16			STBAT_BanChargFlg
	uint16_t STBAT_BanDischFlg;//441	R	��ֹ�ŵ��־λ	UINT16			STBAT_BanDischFlg
	uint16_t STBAT_BasicStatus;//442		����״̬	UINT16		"Bit15���������Ƿѹ����  0-������ 1-����
	//Bit14��ģ���ѹ���� 0-������ 1-����Bit13��ģ���ѹ���� 0-������ 1-����Bit12��ģ���¶ȸ߱��� 0-������ 1-����Bit12��Ԥ��
	//Bit10����·���� 0-������ 1-����Bit9���ŵ�������� 0-������ 1-����Bit8������������ 0-������ 1-����Bit7���ŵ���±��� 0-������ 1-����Bit6���ŵ���±��� 0-������ 1-����
	//Bit5�������±��� 0-������ 1-����Bit4�������±��� 0-������ 1-����Bit3����ѹ��ѹ���� 0-������ 1-����Bit2����ѹ��ѹ���� 0-������ 1-����
	//Bit1�������ѹ���� 0-������ 1-����Bit0�������ѹ���� 0-������ 1-����"	STBAT_BasicStatus
	uint16_t STBAT_ProtectMsg;//443		������Ϣ	UINT16		"BIT15:����
	//Bit14�������쳣�澯 0-������ 1-���쳣Bit13�����������״̬ 0-�� 1-����Bit12�������ŵ�״̬ 0-�� 1-�ŵ�
	//Bit11���������״̬ 0-�� 1-���Bit10��ϵͳ����״̬ 0-null;1-����Bit9���¶ȸ澯 0-������ 1-�澯
	//Bit8�������澯 0-������ 1-�澯Bit7�� ��ѹ�澯 0-������ 1-�澯Bit6���¶ȱ��� 0-������ 1-����Bit5����ѹ���� 0-������ 1-����
	//Bit4���������� 0-������ 1-����Bit3��ϵͳ���ϱ��� 0-������ 1-����Bit2-BIT0:����״̬ 00�� ���� 01�� ��� 02�� �ŵ� 03�� ���� 04~07�� ����"	STBAT_ProtectMsg
	uint16_t STBAT_WarnMsg;//444		�澯��Ϣ	UINT16		"BIT15�������¶��쳣�澯 0-������ 1-�澯
	//Bit14��ģ���ѹ�澯 0-������ 1-�澯Bit13��ģ���ѹ�澯 0-������ 1-�澯Bit12��ģ���¶ȸ߸澯 0-������ 1-�澯
	//Bit11�������¶ȸ߸澯 0-������ 1-�澯Bit10��Ԥ��Bit9���ŵ�����澯 0-������ 1-�澯Bit8���������澯 0-������ 1-�澯
	//Bit7���ŵ���¸澯 0-������ 1-�澯Bit6���ŵ���¸澯 0-������ 1-�澯Bit5�������¸澯 0-������ 1-�澯
	//Bit4�������¸澯 0-������ 1-�澯Bit3����ѹ��ѹ�澯 0-������ 1-�澯Bit2����ѹ��ѹ�澯 0-������ 1-�澯
	//Bit1�������ѹ�澯  0-������ 1-�澯Bit0�������ѹ�澯  0-������ 1-�澯"	STBAT_WarnMsg
	uint16_t rsvd1[6];//-450		Ԥ��	UINT16			
};

struct DATA_TTU_ACPW
{
	uint16_t ACPW_Output_A_phsA;//451	R	���A�����	UINT16 	0.1A	���A�����	ACPW_Output_A_phsA
	uint16_t ACPW_Output_A_phsB;//452	R	���B�����	UINT16 	0.1A	���B�����	ACPW_Output_A_phsB
	uint16_t ACPW_Output_A_phsC;//453	R	���C�����	UINT16 	0.1A	���C�����	ACPW_Output_A_phsC
	uint16_t ACPW_Output_PhV_phsA;//454	R	���A���ѹ	UINT16 	0.1V	���A���ѹ	ACPW_Output_PhV_phsA
	uint16_t ACPW_Output_PhV_phsB;//455	R	���B���ѹ	UINT16 	0.1V	���B���ѹ	ACPW_Output_PhV_phsB
	uint16_t ACPW_Output_PhV_phsC;//456	R	���C���ѹ	UINT16 	0.1V	���C���ѹ	ACPW_Output_PhV_phsC
	uint16_t ACPW_Output_DCOutput;//457	R	ֱ����ѹ	UINT16 	0.1V	ֱ����ѹ	ACPW_Output_DCOutput
	uint16_t rsvd1;//458	R	����	UINT16 	0.1V	����	
	uint16_t ACPW_Output_PhW_phsA;//459	R	���A���й�����	UINT16 	0.1kW	���A���й�����	ACPW_Output_PhW_phsA
	uint16_t ACPW_Output_PhW_phsB;//460	R	���B���й�����	UINT16 	0.1kW	���B���й�����	ACPW_Output_PhW_phsB
	uint16_t ACPW_Output_PhW_phsC;//461	R	���C���й�����	UINT16 	0.1kW	���C���й�����	ACPW_Output_PhW_phsC
	uint16_t ACPW_Output_TotW;//462	R	����������й�	UINT16 	0.1kW	����������й�	ACPW_Output_TotW
	int16_t ACPW_Output_TotVar;//463	R	����������޹�	INT16 	0.1kVar	����������޹�	ACPW_Output_TotVar
	uint16_t ACPW_Output_TotVA;//464	R	������������ڹ���	UINT16 	0.001	������������ڹ���	ACPW_Output_TotVA
	uint16_t ACPW_TotPF;//465	R	����ƽ����������	UINT16 	0.001	����ƽ����������	ACPW_TotPF
	uint16_t ACPW_A_neut;//466	R	���ߵ���	UINT16 	0.1A	���ߵ���	ACPW_A_neut
	uint16_t ACPW_Status_And_Protect;//467	R	״̬�ͱ���	UINT16 		"bit8�������������/bit7���¶ȱ���/bit6��IGBT����bit5��VDC����/bit4: VS����/bit3��ϵͳ�Լ�״̬
					//bit2���Ӵ���״̬/bit1��PWM����״̬/bit0:����״̬ "	ACPW_Status_And_Protect
	uint16_t rsvd2[3];//-470	R		UINT16 			
	uint16_t ACPW_Flag;//471	R/W	ͨ���������ò�����ȷ�ϱ�־	UINT16 	1	ͨ���������ò�����ȷ�ϱ�־	
	uint16_t ACPW_LimitI;//472	R/W	��������޷�ֵ	UINT16 	0.1A	��������޷�ֵ	
	uint16_t ACPW_ProtectI;//473	R/W	�����������ֵ	UINT16 	0.1A	�����������ֵ	
	uint16_t ACPW_VS_ProtectHigh;//474	R/W	VS�ϱ���ֵ	UINT16 	0.1V	VS�ϱ���ֵ	
	uint16_t ACPW_VS_ProtectLow;//475	R/W	VS�±���ֵ	UINT16 	0.1V	VS�±���ֵ	
	uint16_t ACPW_RunOff;//476	R/W	���ػ�����	UINT16 	1	���ؼ�ָ�� HEX AAAA���� �����ػ�	
	uint16_t ACPW_Output;//477	R/W	VS�����ѹ����	UINT16 	0.1V	VS�����ѹ����	
	uint16_t rsvd3[3];//-480	R/W	Ԥ��				
};

struct DATA_TTU_CHARGE
{
	uint16_t Plug_Max_OutputVolt;//1000	R	ǹ��������ѹ	UINT16			Plug_Max_OutputVolt
	uint16_t Plug_Max_OutputCur;//1001	R	ǹ����������	UINT16			Plug_Max_OutputCur
	uint16_t Plug_Max_OutputPower;//1002	R	ǹ����������	UINT16			Plug_Max_OutputPower
	uint16_t Plug_ConnectStatus;//1003	R	ǹ����״̬	UINT16		0δ����/1����	Plug_ConnectStatus
	uint16_t Plug_ChargingStatus;//1004	R	ǹ��ǰ���״̬	UINT16		"0����ʼ��״̬1������״̬2���ȴ�����3��������
								//4�������5��ֹͣ�����6����������7��������δ��ǹ8������ʧ��9������״̬"	Plug_ChargingStatus
	uint16_t CarBMS_DemandVolt;//1005	R	��BMS�����ѹ	UINT16			CarBMS_DemandVolt
	uint16_t CarBMS_DemandCur;//1006	R	��BMS�������	UINT16			CarBMS_DemandCur
	uint16_t CarBMS_CurSOC;//1007	R	��BMS��ǰSOC	UINT16			CarBMS_CurSOC
	uint16_t ChargingVolt;//1008	R	��ǰ����ѹ	UINT16			ChargingVolt
	uint16_t ChargingCur;//1009	R	��ǰ������	UINT16			ChargingCur
	uint16_t ChargingPower;//1010	R	��ǰ��繦��	UINT16			ChargingPower
	uint16_t ChargedEnergy;//1011	R	���γ�����	UINT16			ChargedEnergy
	uint16_t ChargedTime;//1012	R	���γ��ʱ��	UINT16			ChargedTime
	uint16_t ChargedMoney;//1013	R	���γ����	UINT16			ChargedMoney
	uint16_t CurMeterEnergy;//1014	R	��ǰ������	UINT16			CurMeterEnergy
	uint16_t CauseOfChargedEnd;//1015	R	������ԭ��	UINT16		"0��δ֪ԭ��
								//1��APPֹͣ2��ˢ��ֹͣ3��BMS����ֹͣ8�������ж�9����ͣ����10���Ž�����"	CauseOfChargedEnd
	uint16_t ReadedPowerCtrl;//1016	R	��ȡ���ʿ���	UINT16			ReadedPowerCtrl
	uint16_t ReadedOnOffCtrl;//1017	R	��ȡ��ͣ����	UINT16			ReadedOnOffCtrl
	uint16_t FaultStatus1;//1018	R	����״̬01	UINT16		���ȷ��	FaultStatus1
	uint16_t FaultStatus2;//1019	R	����״̬02	UINT16		���ȷ��	FaultStatus2
	uint16_t rsvd1;//1020	R	Ԥ��	UINT16			
	uint16_t PowerCtrl_Set;//1021	R/W	���ʿ���	UINT16		����ֵ���� 60000����60kw	PowerCtrl_Set
	uint16_t StartOrStop_Set;//1022	R/W	��ͣ����	UINT16		01:������� 02��ֹͣ���	StartOrStop_Set
	uint16_t rsvd2;//1023	R/W	Ԥ��	UINT16			
};
struct DATA_TTU_METER
{
	uint32_t	Ua; //���ѹ
	uint32_t	Ub; 
	uint32_t	Uc;
	uint32_t	Ia; //�����
	uint32_t	Ib; 
	uint32_t	Ic;	
	uint32_t	Ps; 
	uint32_t	Qs;
	uint32_t	VAs;
	uint32_t	PFs;
	uint32_t	KWH[3]; //����	
	uint16_t    Harm_U[3]; //г��
	uint16_t    Harm_I[3]; //
	
};

struct DATA_TTU_PVClass {
	int32_t		P; //�й�����
	uint32_t	P_DC; //��ֱ���������
	uint32_t	Power_total;//�ܷ�����
	uint32_t	Power_month; //���·�����
	uint32_t	Power_lastmonth; //���·�����
	uint16_t	Power_day;   //���շ�����
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


