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
float PVAC_Power[3]; //���ܣ��ڿ�����ʼ��ʱ��flash��������rtc�����б��档
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  ����EMS����
  * @param  None
  * @retval None
  */
void updataEMS(void)
{
	DATA_ttu.DATA_TTU_ems.PCS_Fault = LEUER_Data.EMS_Data.PCS_Fault;//100	R	PCSģ�����	UINT16	
	DATA_ttu.DATA_TTU_ems.DCStore_Fault = LEUER_Data.EMS_Data.DC_ChuNeng_Fault;//101	R	DC����ģ�����	UINT16		
		
	DATA_ttu.DATA_TTU_ems.DCPV_Fault =LEUER_Data.EMS_Data.DC_GuangFu_Fault ;//102	R	DC���ģ�����	UINT16		ϵͳ������	DCPV_Fault
	DATA_ttu.DATA_TTU_ems.DCAC_Fault=LEUER_Data.EMS_Data.DC_AC_Fault ;//103	R	DCACģ�����	UINT16		ϵͳ������	DCAC_Fault
//	DATA_ttu.DATA_TTU_ems.Battery_Fault=LEUER_Data.EMS_Data.BAT_Error_FLAG ;//104	R	��ع���	UINT16		ϵͳ������	Battery_Fault
	DATA_ttu.DATA_TTU_ems.AutoModeOn_Excep=LEUER_Data.EMS_Data.Run_Error_FLAG ;//105	R	�Զ�ģʽ�����쳣��־	UINT16		70 ��FAULT/FAIL 80��PASS	AutoModeOn_Excep
	DATA_ttu.DATA_TTU_ems.SysVSVolt_Excep=LEUER_Data.EMS_Data.V_Error_FLAG ;//106	R	ϵͳVS��ѹ�쳣��־	UINT16		70 ��FAULT/FAIL 80��PASS	SysVSVolt_Excep
	DATA_ttu.DATA_TTU_ems.BusVDCVolt_Excep=LEUER_Data.EMS_Data.VDC_Error_FLAG ;//107	R	ĸ��VDC��ѹ�쳣��־	UINT16		70 ��FAULT/FAIL 80��PASS	BusVDCVolt_Excep
	DATA_ttu.DATA_TTU_ems.MasterDevCommu_Err=LEUER_Data.EMS_Data.COM_Error_FLAG ;//108	R	����ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	MasterDevCommu_Err
	DATA_ttu.DATA_TTU_ems.PCSCommu_Err=LEUER_Data.EMS_Data.PCS_Com_State ;//109	R	PCSͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	PCSCommu_Err
	DATA_ttu.DATA_TTU_ems.DC1Commu_Err=LEUER_Data.EMS_Data.DC1_Com_State ;//110	R	DC1ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	DC1Commu_Err
	DATA_ttu.DATA_TTU_ems.DC2Commu_Err=LEUER_Data.EMS_Data.DC2_Com_State ;//111	R	DC2ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	DC2Commu_Err
	DATA_ttu.DATA_TTU_ems.BatCommu_Err=LEUER_Data.EMS_Data.BAT_Com_State ;//112	R	���ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	BatCommu_Err
	DATA_ttu.DATA_TTU_ems.DCACCommu_Err=LEUER_Data.EMS_Data.DC_AC_Com_State ;//113	R	DCACͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	DCACCommu_Err
	//	DATA_ttu.DATA_TTU_ems.ChargingPileCommu_Err=LEUER_Data.EMS_Data. ;//114	R	���׮ͨѶ�����־	UINT16		70 ��FAULT/FAIL 80��PASS	ChargingPileCommu_Err
						
	DATA_ttu.DATA_TTU_ems.AutoModeOn_Status=LEUER_Data.EMS_Data.Run_FLAG ;//120	R	�Զ�ģʽ����״̬	UINT16		1Ϊ����  0Ϊ�ػ�	AutoModeOn_Status
	DATA_ttu.DATA_TTU_ems.CurRunMode=LEUER_Data.EMS_Data.Mod_FLAG;//121	R	��ǰ����ģʽ	UINT16			CurRunMode

	DATA_ttu.DATA_TTU_ems.EMS_Mode_Set = LEUER_Data.EMS_Data.LEUER_Mod ;//150	R/W	�Զ�/�ֶ�ģʽ����	UINT16		A���Զ����ػ�ģʽ �������ֶ�ģʽ	EMS_Mode_Set
	DATA_ttu.DATA_TTU_ems.EMS_OnOff_Set = LEUER_Data.EMS_Data.Run ;//151	R/W	���ػ�����	UINT16		AAAA�����������ػ�	EMS_OnOff_Set
	DATA_ttu.DATA_TTU_ems.EMS_OffConnectGrid_Set = LEUER_Data.EMS_Data.P_A ;//152	R/W	�벢������	UINT16		�ֶ������벢�����ƣ�PCSӦ�þ߱��������	EMS_OffConnectGrid_Set
}

/****************************************************************
	����PCS���ݵ� �ں��ն�ͨѶ�����ݱ�
*****************************************************************/	
void updataPCS(void)
{
	uint8_t temp;
	uint32_t temp2;
	//PCS
	
	DATA_ttu.DATA_TTU_pcs.UAB= (uint16_t)(PCSdata.sys_u[0]*1.732/100);//200	R	����AB�ߵ�ѹ	UINT16	0.1V		UAB
	DATA_ttu.DATA_TTU_pcs.UBC= (uint16_t)(PCSdata.sys_u[1]*1.732/100);//201	R	����BC�ߵ�ѹ	UINT16	0.1V		UBC
	DATA_ttu.DATA_TTU_pcs.UAC= (uint16_t)(PCSdata.sys_u[2]*1.732/100);//202	R	����AC�ߵ�ѹ	UINT16	0.1V		UAC
	
	DATA_ttu.DATA_TTU_pcs.PhV_phsA= (uint16_t)(PCSdata.sys_u[0]/100);//;//203	R	����A���ѹ	UINT16	0.1V		PhV_phsA
	DATA_ttu.DATA_TTU_pcs.PhV_phsB= (uint16_t)(PCSdata.sys_u[1]/100);//;//204	R	����B���ѹ	UINT16	0.1V		PhV_phsB
	DATA_ttu.DATA_TTU_pcs.PhV_phsC= (uint16_t)(PCSdata.sys_u[2]/100);//;//205	R	����C���ѹ	UINT16	0.1V		PhV_phsC
//	DATA_ttu.DATA_TTU_pcs.PhV_PhsN= PCSdata.PhV_PhsN;//;//206	R	����N���ѹ	UINT16	0.1V		PhV_PhsN

	
	DATA_ttu.DATA_TTU_pcs.A_phsA= (int16_t)(PCSdata.sys_i[0]/100);//;//207	R	����A�����	INT16	0.1A		A_phsA
	DATA_ttu.DATA_TTU_pcs.A_phsB= (int16_t)(PCSdata.sys_i[1]/100);//;//208	R	����B�����	INT16	0.1A		A_phsB
	DATA_ttu.DATA_TTU_pcs.A_phsC= (int16_t)(PCSdata.sys_i[2]/100);//;//209	R	����C�����	INT16	0.1A		A_phsC
//	DATA_ttu.DATA_TTU_pcs.A_phsN= PCSdata.A_phsN;//;//210	R	����N�����	INT16	0.1A		A_phsN
	DATA_ttu.DATA_TTU_pcs.Hz= PCSdata.freq;//;//211	R	����Ƶ��	UINT16	0.01Hz		Hz
	DATA_ttu.DATA_TTU_pcs.TotW= (int16_t)(PCSdata.sys_p[0]/100);//;//212	R	�������й�����	UINT16	0.1kW	�人PCS�Ͱ�����¾��޵��๦�ʼ���������	TotW
	DATA_ttu.DATA_TTU_pcs.TotVAr=(int16_t)(PCSdata.sys_p[1]/100);//;//213	R	�������޹�����	UINT16	0.1kVar		TotVAr
	DATA_ttu.DATA_TTU_pcs.TotVA= (int16_t)(PCSdata.sys_p[2]/100);//;//214	R	���������ڹ���	UINT16	0.1kVA		TotVA

	// ģ���ϴ�������Ƶ����ϴ����¼���
	DATA_ttu.DATA_TTU_pcs.TotW= ((int32_t)DATA_ttu.DATA_TTU_pcs.PhV_phsA  * DATA_ttu.DATA_TTU_pcs.A_phsA+
								 (int32_t)DATA_ttu.DATA_TTU_pcs.PhV_phsB  * DATA_ttu.DATA_TTU_pcs.A_phsB+
								 (int32_t)DATA_ttu.DATA_TTU_pcs.PhV_phsC  * DATA_ttu.DATA_TTU_pcs.A_phsC)/100/100;

	temp2= (int32_t)DATA_ttu.DATA_TTU_pcs.TotW  *DATA_ttu.DATA_TTU_pcs.TotW +
		   (int32_t)DATA_ttu.DATA_TTU_pcs.TotVAr*DATA_ttu.DATA_TTU_pcs.TotVAr;
	
	temp2 = data2hex(temp2);
	temp2 = sqrtf(temp2);

	temp2 = Float32(temp2);
	DATA_ttu.DATA_TTU_pcs.TotVA = (uint16_t)(temp2);

    DATA_ttu.DATA_TTU_pcs.Load_PhV_phsA= (uint16_t)(PCSdata.load_u[0]/100);//;//215	R	����A���ѹ	UINT16	0.1V		Load_PhV_phsA
	DATA_ttu.DATA_TTU_pcs.Load_PhV_phsB= (uint16_t)(PCSdata.load_u[1]/100);//;//216	R	����B���ѹ	UINT16	0.1V		Load_PhV_phsB
	DATA_ttu.DATA_TTU_pcs.Load_PhV_phsC= (uint16_t)(PCSdata.load_u[2]/100);//;//217	R	����C���ѹ	UINT16	0.1V		Load_PhV_phsC
//	DATA_ttu.DATA_TTU_pcs.Load_PhV_phsN= PCSdata.Load_PhV_phsN;//;//218	R	����N���ѹ	UINT16	0.1V		Load_PhV_phsN
	DATA_ttu.DATA_TTU_pcs.Load_A_phsA= (int16_t)(PCSdata.load_i[0]/100);//;//219	R	����A�����	INT16	0.1A		Load_A_phsA
	DATA_ttu.DATA_TTU_pcs.Load_A_phsB= (int16_t)(PCSdata.load_i[1]/100);//;//220	R	����B�����	INT16	0.1A		Load_A_phsB
	DATA_ttu.DATA_TTU_pcs.Load_A_phsC= (int16_t)(PCSdata.load_i[2]/100);//;//221	R	����C�����	INT16	0.1A		Load_A_phsC
//	DATA_ttu.DATA_TTU_pcs.Load_A_phsN= PCSdata.Load_A_phsN;//;//222	R	����N�����	INT16	0.1A		Load_A_phsN
//	DATA_ttu.DATA_TTU_pcs.Load_Hz= PCSdata.Load_Hz;//;//223	R	����Ƶ��	UINT16	0.01Hz		Load_Hz
	DATA_ttu.DATA_TTU_pcs.Load_TotW= (int16_t)(PCSdata.load_p[0]/100);//;//224	R	�������й�����	UINT16	0.1kW		Load_TotW
	DATA_ttu.DATA_TTU_pcs.Load_TotVAr= (int16_t)(PCSdata.load_p[1]/100);//;//225	R	�������޹�����	UINT16	0.1kVar		Load_TotVAr
	DATA_ttu.DATA_TTU_pcs.Load_TotVA= (int16_t)(PCSdata.load_p[2]/100);//226	R	���������ڹ���	UINT16	0.1kVA		Load_TotVA
	
//	DATA_ttu.DATA_TTU_pcs.Output_PhV_phsA= PCSdata.Output_PhV_phsA;//;//227	R	���A���ѹ	UINT16	0.1V		Output_PhV_phsA
//	DATA_ttu.DATA_TTU_pcs.Output_PhV_phsB= PCSdata.Output_PhV_phsB;//;//228	R	���B���ѹ	UINT16	0.1V		Output_PhV_phsB
//	DATA_ttu.DATA_TTU_pcs.Output_PhV_phsC= PCSdata.Output_PhV_phsC;//;//229	R	���C���ѹ	UINT16	0.1V		Output_PhV_phsC
//	DATA_ttu.DATA_TTU_pcs.Output_PhV_phsN= PCSdata.Output_PhV_phsN;//;//230	R	���N���ѹ	UINT16	0.1V		Output_PhV_phsN
//	DATA_ttu.DATA_TTU_pcs.Output_A_phsA= PCSdata.Output_A_phsA;//;//231	R	���A�����	INT16	0.1A		Output_A_phsA
//	DATA_ttu.DATA_TTU_pcs.Output_A_phsB= PCSdata.Output_A_phsB;//;//232	R	���B�����	INT16	0.1A		Output_A_phsB
//	DATA_ttu.DATA_TTU_pcs.Output_A_phsC= PCSdata.Output_A_phsC;//;//233	R	���C�����	INT16	0.1A		Output_A_phsC
//	DATA_ttu.DATA_TTU_pcs.Output_A_phsN= PCSdata.Output_A_phsN;//;//234	R	���N�����	INT16	0.1A		Output_A_phsN
//	DATA_ttu.DATA_TTU_pcs.Output_Hz= PCSdata.Output_Hz;//;//235	R	���Ƶ��	UINT16	0.01Hz		Output_Hz
//	DATA_ttu.DATA_TTU_pcs.Output_TotW= PCSdata.Output_TotW;//;//236	R	������й�����	UINT16	0.1kW		Output_TotW
//	DATA_ttu.DATA_TTU_pcs.Output_TotVAr= PCSdata.Output_TotVAr;//;//237	R	������޹�����	UINT16	0.1kVar		Output_TotVAr
//	DATA_ttu.DATA_TTU_pcs.Output_TotVA= PCSdata.Output_TotVA;//;//238	R	��������ڹ���	UINT16	0.1kVA		Output_TotVA
	
	DATA_ttu.DATA_TTU_pcs.DCTolBus_Volt= PCSdata.VDC;//;//239	R	ֱ������ĸ�ߵ�ѹ 	UINT16	0.1V		DCTolBus_Volt
	//DATA_ttu.DATA_TTU_pcs.DCUpBus_Volt= PCSdata.DCUpBus_Volt;//;//240	R	ֱ����ĸ�ߵ�ѹ	UINT16	0.1V		DCUpBus_Volt
	//DATA_ttu.DATA_TTU_pcs.DCDownBus_Volt= PCSdata.DCDownBus_Volt;//;//241	R	ֱ����ĸ�ߵ�ѹ	UINT16	0.1V		DCDownBus_Volt
//	DATA_ttu.DATA_TTU_pcs.DC_Cur= PCSdata.DC_Cur;//;//242	R	ֱ������� 	INT16	0.1A		DC_Cur
//	DATA_ttu.DATA_TTU_pcs.DC_ActPower= PCSdata.DC_ActPower;//;//243	R	ֱ���๦�� 	UINT16	0.1kW		DC_ActPower
//	DATA_ttu.DATA_TTU_pcs.IGBT_LowestTmp= PCSdata.IGBT_LowestTmp;//;//244	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��͵��¶�	IGBT_LowestTmp
//	DATA_ttu.DATA_TTU_pcs.IGBT_HighestTmp= PCSdata.IGBT_HighestTmp;//;//245	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��ߵ��¶�	IGBT_HighestTmp
	DATA_ttu.DATA_TTU_pcs.Env_Tmp= PCSdata.temprature_in;//;//246	R	�����¶�	INT16	0.1��C	�豸�϶����Ļ����¶�	Env_Tmp


	//BIT7:������ѹ��ѹ/BIT6:������ѹǷѹ/BIT5:������ѹ�������/BIT4:DCĸ�ߵ�ѹ�쳣/BIT3:��������쳣
	//BIT2:���¹���/BIT1:ͨѶ����/BIT0:��ͣ��ť

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
	DATA_ttu.DATA_TTU_pcs.PCS_Status1= temp;//;//247	R	PCS״̬1	UINT16		"BIT7:������ѹ��ѹ/BIT6:������ѹǷѹ/BIT5:������ѹ�������/BIT4:DCĸ�ߵ�ѹ�쳣/BIT3:��������쳣
							//BIT2:���¹���/BIT1:ͨѶ����/BIT0:��ͣ��ť"	PCS_Status1
//	DATA_ttu.DATA_TTU_pcs.PCS_Status2= PCSdata.PCS_Status2;//;//248	R	PCS״̬2	UINT16			PCS_Status2

//0x02	DC��ѹ	��ѹ�趨(0.001V) //0x21	DC����	�����趨(0.001A)  //0x22	DC�㹦��	�����趨(0.001W)
	if(PCSdata.mode == 0x02)  	    temp = 0x0A;  //DC��ѹģʽʱ�����õĲ���1��BAT�ڵĵ�ѹ��
	else if(PCSdata.mode == 0x21)    temp = 0x0B;
	else if(PCSdata.mode == 0x22)    temp = 0x0C;
	else   								temp = 0x0;
	
	//write
	DATA_ttu.DATA_TTU_pcs.PCS_RunMode_Set= temp;//270	R/W	PCS����ģʽ����	UINT16		A��ѹģʽ��B����ģʽ��C�㹦��ģʽ,D��ѹ����ģʽ	PCS_RunMode_Set
	DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Volt_Set= (uint16_t)(PCSdata.bus_V/100);//;//271	R/W	ֱ����ĸ�ߵ�ѹֵ����	UINT16	0.1V	ֱ����ѹĿ��ֵ���ã���700/750��	PCS_DCSide_Volt_Set

			
	if(DATA_ttu.DATA_TTU_pcs.PCS_RunMode_Set== 0x0B) 
		DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Cur_Set= (uint16_t)(PCSdata.parameter[0]/100);//;//272	R/W	ֱ�������ֵ����	INT16	0.1A	ֱ�������ֵ���ã������ɱ�ʾ��������ʾ���룬����ʾ����	PCS_DCSide_Cur_Set
	else DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Cur_Set=0;
	
	if(DATA_ttu.DATA_TTU_pcs.PCS_RunMode_Set== 0x0C) 
		DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Power_Set= (uint16_t)(PCSdata.parameter[0]/100);//273	R/W	ֱ���๦��ֵ���� 	INT16	0.1KW	ֱ�������������������ã������ɱ�ʾ�з���	PCS_DCSide_Power_Set
	else DATA_ttu.DATA_TTU_pcs.PCS_DCSide_Power_Set=0;
	
//	DATA_ttu.DATA_TTU_pcs.PCS_DCBusVoltProVal_Set= PCSdata.PCS_DCBusVoltProVal_Set;//;//274	R/W	ֱ��ĸ�ߵ�ѹ����ֵ����	UINT16	0.1V	ֱ����ĸ�ߵ�ѹ����ֵ����	PCS_DCBusVoltProVal_Set


	//�±߶��ǵ�ز�
	DATA_ttu.DATA_TTU_pcs.PCS_DCSide_CurLim_Set= (uint16_t)(PCSdata.set[2]/100);//275	R/W	ֱ�����������ֵ����	INT16	0.1A	ֱ���������������ֵ��������ֵ�·�	PCS_DCSide_CurLim_Set
	DATA_ttu.DATA_TTU_pcs.PCS_DCSide_PowerLim_Set= (uint16_t)(PCSdata.set[4]/100);//276	R/W	ֱ���๦������ֵ����	INT16	0.1KW		PCS_DCSide_PowerLim_Set
	DATA_ttu.DATA_TTU_pcs.PCS_DCSideCurProVal_Set= (uint16_t)(PCSdata.set[2]/100);//277	R/W	ֱ�����������ֵ����	UINT16	0.1A		PCS_DCSideCurProVal_Set
	DATA_ttu.DATA_TTU_pcs.PCS_DCSidePowerProVal_Set= (uint16_t)(PCSdata.set[4]/100);//278 R/W	ֱ���๦�ʱ���ֵ����	UINT16	0.1KW		PCS_DCSidePowerProVal_Set

}


/****************************************************************
	���´������ݵ� �ں��ն�ͨѶ�����ݱ� 
*****************************************************************/
void updataSTAC(void)
{
	
	DATA_ttu.DATA_TTU_stac.STAC_BatSide_Volt = (uint16_t)(PCSdata.DC_U/100);//301	R	��ز��ѹ	UINT16	0.1V	ģ���ز�ӿڵ�ѹ	STAC_BatSide_Volt
	DATA_ttu.DATA_TTU_stac.STAC_BatSide_Cur = (uint16_t)(PCSdata.DC_I/100);//302	R	��ز����	INT16 	0.1A	ģ���ز�ӿڵ�����Ӧ���з��ű�ʾ��/��	STAC_BatSide_Cur
	DATA_ttu.DATA_TTU_stac.STAC_BatSide_ActPower= (uint16_t)(PCSdata.DC_P/100);//303	R	��ز๦��	INT16 	0.1kW	ģ���ز�ӿڳ�/�ŵ繦��	STAC_BatSide_ActPower

	DATA_ttu.DATA_TTU_stac.STAC_BusSide_Volt=PCSdata.VDC;//304	R	ĸ�߲��ѹ	UINT16	0.1V	ģ��ֱ��ĸ�߲�ӿڵ�ѹ	STAC_BusSide_Volt
//	DATA_ttu.DATA_TTU_stac.STAC_BusSide_Cur;//305	R	ĸ�߲����	INT16 	0.1A	ģ��ֱ��ĸ�߲�ӿڵ���	STAC_BusSide_Cur
//	DATA_ttu.DATA_TTU_stac.STAC_BusSide_ActPower;//306	R	ĸ�߲๦��	INT16	0.1kW	ģ��ֱ��ĸ�߲�ӿڳ�/�ŵ繦��	STAC_BusSide_ActPower
//	DATA_ttu.DATA_TTU_stac.STAC_CurDevStatus;//307	R	��ǰ���ػ�״̬	UINT16			STAC_CurDevStatus
//	DATA_ttu.DATA_TTU_stac.STAC_CurRunMode;//308	R	��ǰ����ģʽ	UINT16			STAC_CurRunMode
//	DATA_ttu.DATA_TTU_stac.STAC_IGBT_LowestTmp;//309	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��͵��¶�	STAC_IGBT_LowestTmp
//	DATA_ttu.DATA_TTU_stac.STAC_IGBT_HighestTmp;//310	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��ߵ��¶�	STAC_IGBT_HighestTmp
//	DATA_ttu.DATA_TTU_stac.STAC_EnvTmp;//311	R	�����¶�	INT16	0.1��C	�豸�϶����Ļ����¶�	STAC_EnvTmp
//	DATA_ttu.DATA_TTU_stac.STAC_FaultMsg;//312	R	������Ϣ	UINT16		BIT5:��ص�ѹ�߳���/BIT4��ص�ѹ�ͳ���/BIT3:��ط���/BIT2:��������/BIT1:ĸ���е㲻ƽ��/BIT0:���¹���	STAC_FaultMsg

	
//	DATA_ttu.DATA_TTU_stac.STAC_OnOff_Set;//321	R/W	���ػ�����	UINT16		���ػ���дAAAA������д�����ػ�	STAC_OnOff_Set
//	DATA_ttu.DATA_TTU_stac.STAC_RunMode_Set;//322	R/W	����ģʽ����	UINT16		��������ģʽ��1Ϊ��ѹ����ģʽ��4Ϊ��ѹģʽ	STAC_RunMode_Set
	DATA_ttu.DATA_TTU_stac.STAC_HVSideVolt_Set=(uint16_t)(PCSdata.bus_V/100);//323	R/W	��ѹ���ѹ����	UINT16	0.1V	ĸ�߲�ĵ�ѹ���ã�һ������750V	STAC_HVSideVolt_Set
	
		
//	DATA_ttu.DATA_TTU_stac.STAC_LVSidecur_Set;//324	R/W	��ѹ���������	INT16	0.1A	��ز�ĵ������ã�����ѹ��������	STAC_LVSidecur_Set
//	DATA_ttu.DATA_TTU_stac.STAC_325;//325	R/W	��ѹ������ѹ��ѹֵ	UINT16	0.1V		STAC_325
//	DATA_ttu.DATA_TTU_stac.STAC_326;//326	R/W	��ѹ������ѹ��ѹֵ	UINT16	0.1V		STAC_326
//	DATA_ttu.DATA_TTU_stac.STAC_327;//327	R/W	��ѹ���ϱ�����ѹֵ	UINT16	0.1V		STAC_327
//	DATA_ttu.DATA_TTU_stac.STAC_328;//328	R/W	��ѹ���±�����ѹֵ	UINT16	0.1V		STAC_328
	

	
	DATA_ttu.DATA_TTU_stac.STAC_329= (uint16_t)(PCSdata.set[0]/100);//329	R/W	��ѹ���ϱ�����ѹֵ	UINT16	0.1V		STAC_329
	DATA_ttu.DATA_TTU_stac.STAC_330= (uint16_t)(PCSdata.set[1]/100);//330	R/W	��ѹ���±�����ѹֵ	UINT16	0.1V		STAC_330
//	DATA_ttu.DATA_TTU_stac.STAC_331;//331	R/W	��ѹ���������ֵ	UINT16	0.1A		STAC_331
	DATA_ttu.DATA_TTU_stac.STAC_332=(uint16_t)(PCSdata.set[2]/100);//332	R/W	��ѹ���������ֵ	UINT16	0.1A		STAC_332
	
}

/****************************************************************
	���¹��ģ�����ݵ� �ں��ն�ͨѶ�����ݱ�
*****************************************************************/	
void updataPVAC(void)
{
	uint32_t temp_32;
	uint16_t temp_16,temp2_16;
	static uint16_t lasthour,lastmonth,lastday;
	uint16_t temp_power;
	static float energy=0;
	
	DATA_ttu.DATA_TTU_pvac.PVAC_PVSide_Volt = DC2data.VDC2;//341	R	������ѹ	UINT16	0.1V	ģ������ӿڵ�ѹ	PVAC_PVSide_Volt
	
	if((DC2data.IDC2<5) && (DC2data.IDC2>(-5))) 
		DC2data.IDC2 = 0;//��Ϊ�ֳ�����������ⲻ�ÿ�����С��0.5Aʱֱ�Ӹ�0.
	
	DATA_ttu.DATA_TTU_pvac.PVAC_PVSide_Cur= DC2data.IDC2;//342	R	��������	INT16 	0.1A	ģ������ӿڵ�����Ӧ���з��ű�ʾ��/��	PVAC_PVSide_Cur
	
	temp_32 = (int32_t)DC2data.VDC2 * DC2data.IDC2; 
	DATA_ttu.DATA_TTU_pvac.PVAC_PVSide_ActPower= (int16_t)((int32_t)temp_32/10000);//343	R	����๦��	INT16 	0.1kW	ģ������ӿڳ�/�ŵ繦��	PVAC_PVSide_ActPower
	
	//ȡ����ֵ
	if(DC2data.IDC2<0) temp_16 = 0-DC2data.IDC2;
	else temp_16 = DC2data.IDC2;
	if(DC2data.VDC2<0) temp2_16 = 0-DC2data.VDC2;
	else temp2_16 = DC2data.VDC2;
	
	temp_32 = (uint32_t)temp_16 * temp2_16;  //��ȷ����������ֵ���Ǹ�ֵ��ȡ����ֵ��
	temp_power= (uint16_t)(temp_32/10000);//343	R	����๦��	ȡ����ֵ�ˣ����ڼ�����ܡ�
	
	DATA_ttu.DATA_TTU_pvac.PVAC_BusSide_Volt= DC2data.VDC1;//344	R	ĸ�߲��ѹ	UINT16	0.1V	ģ��ֱ��ĸ�߲�ӿڵ�ѹ	PVAC_BusSide_Volt
	DATA_ttu.DATA_TTU_pvac.PVAC_BusSide_Cur= DC2data.IDC1;//345	R	ĸ�߲����	INT16 	0.1A	ģ��ֱ��ĸ�߲�ӿڵ���	PVAC_BusSide_Cur
	
	
	temp_32 = (uint32_t)DC2data.VDC1*DC2data.IDC1;	
	
	//������뽫temp32ת�� int32 ��������㲻��
	DATA_ttu.DATA_TTU_pvac.PVAC_BusSide_ActPower= (int32_t)temp_32/10000;//346	R	ĸ�߲๦��	INT16	0.1kW	ģ��ֱ��ĸ�߲�ӿڳ�/�ŵ繦��	PVAC_BusSide_ActPower

	if(DC2data.par[0] ==7) //���ģʽ���ÿ��ػ�ָ�7ģʽ���Զ����ػ�
		DATA_ttu.DATA_TTU_pvac.PVAC_CurDevStatus= 1;//347	R	��ǰ���ػ�״̬	UINT16			PVAC_CurDevStatus
	else
		DATA_ttu.DATA_TTU_pvac.PVAC_CurDevStatus= 0;
	
	DATA_ttu.DATA_TTU_pvac.PVAC_CurRunMode= DC2data.par[0];//348	R	��ǰ����ģʽ	UINT16			PVAC_CurRunMode
	
	if(DC2data.T_IGBT1 >DC2data.T_IGBT2 )
	{
		DATA_ttu.DATA_TTU_pvac.PVAC_IGBT_LowestTmp= DC2data.T_IGBT2;//349	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��͵��¶�	PVAC_IGBT_LowestTmp
		DATA_ttu.DATA_TTU_pvac.PVAC_IGBT_HighestTmp= DC2data.T_IGBT1;//350	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��ߵ��¶�	PVAC_IGBT_HighestTmp
	}
	else
	{
		DATA_ttu.DATA_TTU_pvac.PVAC_IGBT_LowestTmp= DC2data.T_IGBT1;//349	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��͵��¶�	PVAC_IGBT_LowestTmp
		DATA_ttu.DATA_TTU_pvac.PVAC_IGBT_HighestTmp= DC2data.T_IGBT2;//350	R	IGBT����¶�	UINT16	0.1��C	���IGBT�����¶ȣ���ʾ��ߵ��¶�	PVAC_IGBT_HighestTmp	
	}

//	DATA_ttu.DATA_TTU_pvac.PVAC_EnvTmp= DC2data.;//351	R	�����¶�	INT16	0.1��C	�豸�϶����Ļ����¶�	PVAC_EnvTmp

	//BIT5:�����ѹ�߳���/BIT4�������ѹ�ͳ���/BIT3:�������/BIT2:��������/BIT1:ĸ���е㲻ƽ��/BIT0:���¹���

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
	DATA_ttu.DATA_TTU_pvac.PVAC_FaultMsg= temp_16;//352	R	������Ϣ	UINT16			PVAC_FaultMsg
			
		
	//������ܡ�
	////��Ϊ��ǰ����3������һ�Σ����Ի��㵽Сʱ���ǵ�ǰ�ĵ��� ��λ0.1kwh.
	//Ҫ���ǵ����ȣ�����������60��6kw����ô����1200���ͱ�Ϊ0�ˡ�����һ�ǿ��Խ�energy��Ϊfloat���������ȸ��ߣ��ں�߸�ֵ���Զ�ת��Ϊu32

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
	
	//�������ԣ�float���Զ�ת��Ϊu32���ͣ�ȥ��С���㡣
	DATA_ttu.DATA_TTU_pvac.PVAC_DayWh= PVAC_Power[0];//353	R	�շ�����	UINT32	0.1kwh		PVAC_DayWh
	DATA_ttu.DATA_TTU_pvac.PVAC_MonWh= PVAC_Power[1];//355	R	�·�����	UINT32	0.1kwh		PVAC_MonWh
	DATA_ttu.DATA_TTU_pvac.PVAC_YearWh= PVAC_Power[2];//355	R	�·�����	UINT32	0.1kwh		PVAC_YearWh

	DATA_ttu.DATA_TTU_pvac.PVAC_DayWh=CHANGE_H16_L16(DATA_ttu.DATA_TTU_pvac.PVAC_DayWh);
	DATA_ttu.DATA_TTU_pvac.PVAC_MonWh=CHANGE_H16_L16(DATA_ttu.DATA_TTU_pvac.PVAC_MonWh);
	DATA_ttu.DATA_TTU_pvac.PVAC_YearWh=CHANGE_H16_L16(DATA_ttu.DATA_TTU_pvac.PVAC_YearWh);
		
	lasthour = timePC[3];   //Сʱ
	lastday= timePC[2];     //��
	lastmonth= timePC[1];	//��				

	
	if(DC2data.par[9] ==1)
		DATA_ttu.DATA_TTU_pvac.PVAC_371= 0xAAAA;//371	R/W	���ػ�����	UINT16		����ģ�鿪�ػ���дAAAA������д�����ػ�	
	else
		DATA_ttu.DATA_TTU_pvac.PVAC_371= 0;
	DATA_ttu.DATA_TTU_pvac.PVAC_372= 0x0A;//372	R/W	����ģʽ����	UINT16		A:���ģʽ��MPPTģʽ	
	DATA_ttu.DATA_TTU_pvac.PVAC_373= DC2data.par[2];//373	R/W	ĸ�߲��ѹ����	UINT16	0.1V	ĸ�߲�ĵ�ѹ����	
	DATA_ttu.DATA_TTU_pvac.PVAC_374= DC2data.par[5];//374	R/W	������������	INT16	0.1A	�����ĵ������ã�����ѹ�������ã������޹�������	
	DATA_ttu.DATA_TTU_pvac.PVAC_375= DC2data.VDC1_max;//375	R/W	ĸ�߲ౣ����ѹֵ	UINT16	0.1V		
	DATA_ttu.DATA_TTU_pvac.PVAC_376= DC2data.VDC2_max;//376	R/W	����ౣ����ѹֵ	UINT16	0.1V		
	DATA_ttu.DATA_TTU_pvac.PVAC_377= DC2data.VDC2_min;//377	R/W	������±�����ѹֵ	UINT16	0.1V		
	DATA_ttu.DATA_TTU_pvac.PVAC_378= DC2data.IDC1_discharge_max;//378	R/W	ĸ�߲��������ֵ	UINT16	0.1A		
	DATA_ttu.DATA_TTU_pvac.PVAC_379= DC2data.IDC2_charge_max;//379	R/W	������������ֵ	UINT16	0.1A		

}

/****************************************************************
	�����������ݵ� �ں��ն�ͨѶ�����ݱ�
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
	
		//���thd�Ż�
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
	�����������ݵ� �ں��ն�ͨѶ�����ݱ�
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
		updataPVAC(); //���
	//	updataBAT(); //���
	//	updataCHG(); //���׮
		updataOTHER();
		
	}	
	/* �������ľ���ʵ�ֻ������������ѭ���������������ں���������֮ǰɾ��������NULL������ʾɾ��
	���ǵ�ǰ���� */

}
