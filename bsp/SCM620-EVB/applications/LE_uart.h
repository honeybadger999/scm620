

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LE_UART_H
#define __LE_UART_H
/* Exported macro ------------------------------------------------------------*/

#define USART0_RX_EN 	GPIO_WrDataOut(56, GPIO_HIGH);
#define USART0_TX_EN 	GPIO_WrDataOut(56, GPIO_LOW);

#define USART3_RX_EN 	GPIO_WrDataOut(57, GPIO_HIGH);
#define USART3_TX_EN 	GPIO_WrDataOut(57, GPIO_LOW);

#define USART4_RX_EN 	GPIO_WrDataOut(59, GPIO_HIGH);
#define USART4_TX_EN 	GPIO_WrDataOut(59, GPIO_LOW);

#define USART7_RX_EN 	GPIO_WrDataOut(58, GPIO_HIGH);
#define USART7_TX_EN 	GPIO_WrDataOut(58, GPIO_LOW);

#define DMAxSTREAMx_UART1_REV  DMA2_Stream5
#define DMA_CHANNEL_UART1_REV  DMA_Channel_4
#define DMAxSTREAMx_UART1_SEND DMA2_Stream7
#define DMA_CHANNEL_UART1_SEND DMA_Channel_4
#define DMA_FLAG_TCIF_UART1    DMA_FLAG_TCIF7

#define DMAxSTREAMx_UART3_REV  DMA1_Stream1
#define DMA_CHANNEL_UART3_REV  DMA_Channel_4
#define DMAxSTREAMx_UART3_SEND DMA1_Stream4
#define DMA_CHANNEL_UART3_SEND DMA_Channel_7
#define DMA_FLAG_TCIF_UART3    DMA_FLAG_TCIF4

#define DMAxSTREAMx_UART6_REV  DMA2_Stream1
#define DMA_CHANNEL_UART6_REV  DMA_Channel_5
#define DMAxSTREAMx_UART6_SEND DMA2_Stream6
#define DMA_CHANNEL_UART6_SEND DMA_Channel_5
#define DMA_FLAG_TCIF_UART6    DMA_FLAG_TCIF6


#define TRUE 1
#define FALSE 0
#define PASS 80    // P的asc码
#define FAIL 70   //F 的asc码
#define FAULT 70

#define	POLY_CRC16	0xA001
#define MAX_REV_QUANT 255
#define MAX_SEND_QUANT 255

#define UART3_TIMEOUT_MS 500   //uart3 循环读取命令的超时时间
#define UART3_QUEUE_LEN 30    //uart3  队列长度

#define MODBUS_WRITE 0x06
#define MODBUS_READ 0x03
#define MODBUS_WRITE10 0x10

/*****************************************************************************/
#define DC_READ_START_ADD 0
#define DC_READ_QUAN 66
#define DC_SET_START_ADD 641 //DC模块RW数据首地址
#define DC_SET_QUAN 13 //DC模块RW数据数量


#define PCS_READ_START_ADD  0x1001
#define PCS_READ_QUAN  0x5F

#define PCS_READ_START_ADD1  0x1001
#define PCS_READ_QUAN1  0x1B
#define PCS_READ_START_ADD2  0x1038
#define PCS_READ_QUAN2  40

#define PCS_VDC_READ_ADD  0x1119

#define PCS_SET_START_ADD  0x2013
#define PCS_SET_QUAN 0x23

#define BROADCAST_SITE_NUMBER	245  //通用站号为245

#define DC2_485PORT 0x0C

#define PCS_485PORT  0x09

//a>>16 结果为0xffffxxxx   和编译系统有关
#define CHANGE_H16_L16(x) (((x<<16)&0xffff0000) + ((x>>16)&0x0000ffff))

/* Exported types ------------------------------------------------------------*/
extern	uint8_t	Uart1_rev[MAX_REV_QUANT];
extern	uint8_t	Uart3_rev[MAX_REV_QUANT];
extern	uint8_t	Uart6_rev[MAX_REV_QUANT];
extern	uint8_t Uart1_Send_Buf[MAX_SEND_QUANT];
extern	uint8_t Uart3_Send_Buf[MAX_SEND_QUANT];
extern	uint8_t Uart6_Send_Buf[MAX_SEND_QUANT];
extern uint16_t Uart1_rev_counter;
extern uint16_t Uart3_rev_counter;
extern uint16_t Uart6_rev_counter;
extern uint16_t Uart2_rev_counter;
extern uint8_t Stm32_par_SCI[50];
extern uint16_t TIM2_Timer;
extern  uint16_t *P_WH_PCSdata ;
extern  uint16_t *P_WH_DC1data ;
extern  uint16_t *P_DC2data ;
extern  uint16_t *P_EMS ;

extern rt_uint8_t uart7_stack[256];

void timeout_uart7(void *parameter);
void timeout_uart0(void *parameter);
void timeout_uart3(void *parameter);




extern struct DC_Data DC2data;

extern struct STRUCT_PCS PCSdata;
extern struct PV_GLASS PV_Glass;

extern uint16_t *P_DC2data;	
extern uint16_t *P_PCSdata ;

extern struct Hour_Meter hour_meter;
/* Exported constants --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
//extern void DmaSendDataProc(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr) ;
extern void send_can1(uint16_t modbus_add,int16_t data);
extern void send_uart3(uint16_t module,uint16_t add,uint32_t data,uint16_t fuccode);
extern void send_can2_MD950(uint16_t modbus_add,uint32_t data);
extern uint8_t Uart0_Post(uint8_t port,uint8_t command,uint16_t add,uint32_t data);
/***********************************UART**********************************************/
struct CRC_TABLE{
 	uint16_t      TABLE1[256];
 	uint16_t      TABLE2[256];
};                              
extern struct	CRC_TABLE crc_tab;

struct Uart_Msg_Msg
{
	uint8_t port;
	uint8_t command;
	uint16_t add;
	uint32_t len;//写入中，需要写四个字节，改为32bit
};
struct Uart_msg
{
	void (* function)(struct Uart_Msg_Msg *msg); //预留，暂没用到，可日后用于同一消息的不同的消息处理函数
	struct Uart_Msg_Msg msg;
};


/***********************************UART**********************************************/

/*********************************DC_DC *******************************************/
enum MODEL_EMUM{
	MODULE_DC1 = 100,
	MODULE_DC2,
	
	
	MODULE_PCS,
} ;

struct COM_Info{
	uint16_t timeout;
	uint16_t offline;
	uint16_t TEMP;
};
extern struct COM_Info Pcs_ComInfo;

extern struct COM_Info DC2_ComInfo;
/*******************************DC DC**********************************************/

/*******************************PCS**********************************************/


//DC-DC 数据结构
struct DC_Data{
	int16_t state0;//0	状态字0	运行状态	2			
	int16_t state1;//1	状态字1	逆变状态1	2			
	int16_t state2;//2	状态字2	电池状态	2			
	int16_t state3;//3	状态字3		2			
	int16_t state4;//4	状态字4	旁路状态	2			
	int16_t state5;//5	状态字5		2			
	/*6	参数0	工作模式	2			详见设置说明
	7	参数1	直流2参考电压	2	0.1	V	
	8	参数2	直流1参考电压	2	0.1	V	
	9	参数3		2			保留
	10	参数4		2			保留
	11	参数5	直流2参考电流	2	0.1	A	
	12	参数6	直流1参考电流	2	0.1	A	
	13	参数7		2			保留
	14	参数8		2			保留
	15	参数9	开关机命令	2			详见设置说明
	16	参数10					保留
	17	参数11		2			保留
	*/
	int16_t par[12];
	/*
	18	命令字0		2			保留
	19	命令字1		2			保留
	20	命令字2		2			保留
	21	命令字3		2			保留
	22	波形通道0		2			
	23	波形通道1		2			
	24	波形通道2		2			
	25	波形通道3		2
	*/
	int16_t rsvd[8];
	int16_t VDC1;//26	数据0	直流1电压	2	0.1	V	           母线侧
	int16_t VDC1_bus_pos;//27	数据1	直流1bus正电压	2	0.1	V	
	int16_t VDC1_bus_neg;//28	数据2	直流1bus负电压	2	0.1	V	
	int16_t IDC1;//29	数据3	直流1输出电流	2	0.1	A	
	int16_t IL;//30	数据4	电感电流	2	0.1	A	
	int16_t IDC2;//31	数据5	直流2输出电流	2	0.1	A	
	int16_t VDC2;//32	数据6	直流2电压	2	0.1	V	
	int16_t VDC2_bus_pos;//33	数据7	直流2bus正电压	2	0.1	V   整流侧 电池
	int16_t VDC2_bus_neg;//34	数据8	直流2bus负电压	2	0.1	V	
	int16_t rsvd2[12];
	int16_t T_IGBT1; //47	数据21	IGBT温度1	2	0.1	°C	
	int16_t T_IGBT2; //48	数据22	IGBT温度2	2	0.1	°C	
	int16_t rsvd3[7];		
	int16_t mode_hardware;//56	数据30	硬件模式	2 
	int16_t stage_run;//57	数据31	工作阶段	2
	int16_t V_BMS; //BMS电池电压
	int16_t I_BMS; //BMS电池电流
	int16_t T_BMS; //BMS电池温度
	int16_t SOC_SOH;//BMS电池SOCSOH
	int16_t V_stop_charge;//BMS充电截止电压
	int16_t V_stop_discharge;//BMS放电截止电压
	int16_t I_max_charge;//BMS充电最大电流
	int16_t I_max_discharge;//BMS放电最大电流

	//下边为参数RW数据
	int16_t rsvd4[4];
	int16_t add_485;//641		1	参数0	2			通讯地址
	int16_t IDC1_discharge_max;//642		2	参数1	2			直流1最大放电电流
	int16_t IDC1_charge_max;//643		3	参数2	2			直流1最大充电电流
	int16_t VDC1_max;//644		4	参数3	2			直流1最高电压
	int16_t VDC1_min;//645		5	参数4	2			直流1最低电压
	int16_t IDC2_discharge_max;//646		6	参数5	2			直流2最大放电电流
	int16_t IDC2_charge_max;//647		7	参数6	2			直流2最大充电电流
	int16_t VDC2_max; //648		8	参数7	2			直流2最高电压
	int16_t VDC2_min;//649		9	参数8	2			直流2最低电压
	int16_t rsvd5[4];
};


//控制程序参数
struct EMS_DATA  //xx个参数
{
	uint16_t  LEUER_Mod;     //模式选择   1离网  2并网  3自动
	uint16_t  P_A;    //液晶或后台设置限功率值，单位KW
	uint16_t  Run;    //一键开关机，1开机  0关机
	uint16_t  rsvd[8];
	uint16_t  Run_FLAG;   //开关机标志
	uint16_t  Mod_FLAG;   //模式标志
	uint16_t  Status_FLAG;  //状态标志，显示开机关机故障态的不同字。
	uint16_t  Run_Error_FLAG;  //开机异常 1为错误，0正常
	uint16_t  V_Error_FLAG;    //电压异常 1为错误，0正常
	uint16_t  VDC_Error_FLAG;   //直流母线电压异常标志  1错误，0正常
	uint16_t  BAT_Error_FLAG;   //电池电压错误标志  1错误 0正常
	uint16_t  COM_Error_FLAG;   //通讯错误标志  1错误 0正常
	uint16_t  PCS_Com_State;	   //PCS模块通讯状态
	uint16_t  DC1_Com_State;	   //dc1模块通讯状态
	uint16_t  DC2_Com_State;     //dc2模块通讯状态
	uint16_t  BAT_Com_State;     //电池模块通讯状态
	uint16_t  DC_AC_Com_State;   //dc_ac模块通讯状态
	
	uint16_t  PCS_Fault;       //PCS模块故障状态
	uint16_t  DC_ChuNeng_Fault; //dc_储能模块故障状态
	uint16_t  DC_GuangFu_Fault;//dc_光伏模块故障状态
	uint16_t  DC_AC_Fault;      //dc_ac模块故障状态
};	

struct STRUCT_LEUER_Data{
//	struct WH_PCS_Data PCS;
//	struct DC_Data DC_DC1;
//	struct DC_Data DC_DC2;
	struct EMS_DATA EMS_Data;   //控制程序参数
};
extern struct STRUCT_LEUER_Data LEUER_Data;

enum PCS_DIV_EMUM{
	PCS_BLO1_ALARM = 0,
	PCS_BLO2_SET,
	PCS_BLO3_LOAD,
	PCS_BLO4_SYS,
	PCS_BLO5_PCS,
	PCS_BLO6_PAR_READ,
	PCS_BLO7_QIANSUAN,
	PCS_BLO8_LIDIAN,
} ;


// 数据表

#pragma pack(2)
struct STRUCT_PCS{
	uint16_t state;//设备状态	uint16	"1—长暂停，2—短暂停， 3—长静置4—短静置，5—停机，6—故障8—掉电，9—指令错误10-软起11-恒流运行，12-恒压运行13-待机，14-离网逆变运行"
	uint16_t alarm;//告警代码	uint16	1、风机1告警 2、风机2告警
	uint16_t fault;//故障代码	uint16	见附录二，故障码表
	int32_t DC_U;//0.001V	直流电压	int32	"32位数据，先高16位，后低16位，即0x1004为直流电压高16位，0x1005为直流电压低16位，下同"
	int32_t DC_I;//0.001A	直流电流
	int32_t DC_P;//0.001W	直流功率
	uint16_t rsvd[6];
	uint32_t leijiliang[6];
	/*	
	3015	R	0.001Ah	当前工作模式累计Ah	uint32	
	3016					
	3017	R	0.001Wh	当前工作模式累计Wh	uint32	
	3018					
	3019	R	0.001Ah	充电累计Ah	uint32	
	3020					
	3021	R	0.001Wh	充电累计Wh	uint32	
	3022					
	3023	R	0.001Ah	放电累计Ah	uint32	
	3024	R				
	3025	R	0.001Wh	放电累计Wh	uint32	
	3026	R	
*/
	uint16_t rsvd1[28];//3027-3054			备用
	uint32_t sys_u[3];	
	/*
	3055	R	0.001V	电网U相电压	uint32	
	3056	R				
	3057	R	0.001V	电网V相电压	uint32	
	3058	R				
	3059	R	0.001V	电网W相电压	uint32	
	3060	R	
	*/
	int32_t sys_i[3];	
	/*
	3061	R	0.001A	电网U相电流	int32	
	3062	R				
	3063	R	0.001A	电网V相电流	int32	
	3064					
	3065	R	0.001A	电网W相电流	int32	
	3066	
	*/
	int32_t sys_p[3];
	/*
	3067	R	0.001W	系统有功功率	int32	
	3068					
	3069	R	0.001Var	系统无功功率	int32	
	3070					
	3071	R	0.001VA	系统视在功率	int32	
	3072	
	*/
	uint32_t load_u[3];
	/*
	3073	R	0.001V	负载U相电压	uint32	
	3074	R				
	3075	R	0.001V	负载V相电压	uint32	
	3076	R				
	3077	R	0.001V	负载W相电压	uint32	
	3078	W	
	*/
	int32_t load_i[3];
	/*
	3079	R	0.001A	负载U相电流	int32	
	3080					
	3081	R	0.001A	负载V相电流	int32	
	3082					
	3083	R	0.001A	负载W相电流	int32	
	3084		
	*/
	int32_t load_p[3];
	/*
	3085	R	0.001W	负载有功功率	int32	
	3086					
	3087	R	0.001Var	负载无功功率	int32	
	3088					
	3089	R	0.001VA	负载视在功率	int32	
	3090	
	*/
	int16_t PF;//3091	R	0.001	PF	int16
	uint16_t freq;//3092	R	0.01Hz	频率	uint16
	int16_t temprature_in;//3093	R	0.1℃	模块进风口温度	int16
	int16_t temprature_out;//3094	R	0.1℃	模块出风口温度	int16
	uint16_t VDC; //20210430增加，只读
	int16_t rsvd2[4];
	//前边100个字
	uint32_t mode;//3100	工作模式		工作模式	uint32	见附录一，工作模式及参数				
	int32_t parameter[4];
	/*
	3101					
	3102	设置参数1		设置参数1	int32	同上
	3103					
	3104	设置参数2		设置参数2	int32	同上
	3105					
	3106	设置参数3		设置参数3	int32	同上
	3107					
	3108	设置参数4		设置参数4	int32	同上
	3109
	*/
	int16_t runstop;//3110	启动/停机命令		启动/停机命令	uint16	0：关闭  1：开启
	uint16_t clearfault;//3111	故障状态清除命令		故障状态清除命令	uint16	1：清除故障
	uint16_t autostart;//3112	开机自启动标志		开机自启动标志	uint16	0：不自启动 1：自启动

	uint32_t set[10];
	/*
	3113	直流最大电压	0.001V	直流最大电压	uint32	0.000V~1500.000V
	3114					
	3115	直流最小电压	0.001V	直流最小电压	uint32	0.000V~1500.000V
	3116					
	3117	最大充电电流	0.001A	最大充电电流	uint32	0~1500.000A
	3118					
	3119	最大放电电流	0.001A	最大放电电流	uint32	0~1500.000A
	3120					
	3121	最大充电功率	0.001W	最大充电功率	uint32	-1500000.0~1500000.0w
	3122					
	3123	最大放电功率	0.001W	最大放电功率	uint32	-1500000.0~1500000.0w
	3124					
	3125	交流电压上限	0.001V	交流电压上限	uint32	0.000~300.000V
	3126					
	3127	交流电压下限	0.001V	交流电压下限	uint32	0.000~300.000V
	3128					
	3129	交流电流上限	0.001A	交流电流上限	uint32	
	3130					
	3131	交流电流下限	0.001A	交流电流下限	uint32	
	3132
	*/
	uint16_t set2[2];
	/*
	3133	交流频率上限	0.01Hz	交流频率上限	uint16	40.00~120.00Hz
	3134	交流频率下限	0.01Hz	交流频率下限	uint16	40.00~120.00Hz
	*/
	uint16_t rsvd3[5];//3135-3139
	uint32_t bus_V;	//3140	母线电压	0.001V	母线电压	uint32				
	uint32_t var;//3142	无功功率	0.001Var	无功功率	uint32	
	
	uint16_t rsvd4[6];//下边六个在软件中设置
	//uint16_t bote;	//3144	设置RS485-1波特率		设置RS485-1波特率	uint16	1：4800；2：9600；3：14400；4：19200；5：38400；6：56000；7：57600；8：115200；9：128000；默认38400
	//uint16_t timeout;//3145	设置RS485-1超时故障时间	S	设置RS485-1超时故障时间	uint16	范围：0~65534。0：不报故障不停机，其他值：通讯超时故障停机。默认为0
	//uint16_t bingji_mode;//3146	通道并机模式		通道并机模式	uint16	0：单机 1：并机主机 2：并机从机
	//uint16_t bingji_way;//3147	通道并机方式		通道并机方式	uint16	1：单模块并机 2：多模块并机
	//uint16_t bingji_quan;//3148	通道并机数量		通道并机数量	uint16	
	//uint16_t bingji_scale;//3149	通道并机霍尔变比		通道并机霍尔变比	uint16	
};
#pragma pack()


#pragma pack(2)
struct Hour_Meter {
	uint32_t	Ua; //相电压
	uint32_t	Ub; 
	uint32_t	Uc;
	uint32_t	Uab; //线电压
	uint32_t	Ubc; 
	uint32_t	Uca; 
	uint32_t	Ia; //相电流
	uint32_t	Ib; 
	uint32_t	Ic;	
	uint32_t	Pa; //有功功率
	uint32_t	Pb; 
	uint32_t	Pc;
	uint32_t	Ps; 
	uint32_t	Qa; //无功功率
	uint32_t	Qb; 
	uint32_t	Qc;
	uint32_t	Qs;
	uint32_t	VAa; //视在功率
	uint32_t	VAb; 
	uint32_t	VAc;
	uint32_t	VAs;
	uint32_t	PFa; //功率引述
	uint32_t	PFb; 
	uint32_t	PFc;
	uint32_t	PFs;
	uint32_t	Frequency;
	uint32_t	KWH[6]; //电能	
	uint16_t    Harm_U[3]; //谐波
	uint16_t    Harm_I[3]; //
};
#pragma pack()

#pragma pack(2)
struct PV_GLASS {
	int32_t		P; //有功功率
	uint32_t	P_DC; //总直流输出功率
	uint32_t	Power_total;//总发电量
	uint32_t	Power_month; //当月发电量
	uint32_t	Power_lastmonth; //上月发电量
	uint16_t	Power_day;   //当日发电量
};
#pragma pack()
/*
*/
#endif /* __LE_UART_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
