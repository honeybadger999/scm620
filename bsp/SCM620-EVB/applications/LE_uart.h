

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
#define PASS 80    // P��asc��
#define FAIL 70   //F ��asc��
#define FAULT 70

#define	POLY_CRC16	0xA001
#define MAX_REV_QUANT 255
#define MAX_SEND_QUANT 255

#define UART3_TIMEOUT_MS 500   //uart3 ѭ����ȡ����ĳ�ʱʱ��
#define UART3_QUEUE_LEN 30    //uart3  ���г���

#define MODBUS_WRITE 0x06
#define MODBUS_READ 0x03
#define MODBUS_WRITE10 0x10

/*****************************************************************************/
#define DC_READ_START_ADD 0
#define DC_READ_QUAN 66
#define DC_SET_START_ADD 641 //DCģ��RW�����׵�ַ
#define DC_SET_QUAN 13 //DCģ��RW��������


#define PCS_READ_START_ADD  0x1001
#define PCS_READ_QUAN  0x5F

#define PCS_READ_START_ADD1  0x1001
#define PCS_READ_QUAN1  0x1B
#define PCS_READ_START_ADD2  0x1038
#define PCS_READ_QUAN2  40

#define PCS_VDC_READ_ADD  0x1119

#define PCS_SET_START_ADD  0x2013
#define PCS_SET_QUAN 0x23

#define BROADCAST_SITE_NUMBER	245  //ͨ��վ��Ϊ245

#define DC2_485PORT 0x0C

#define PCS_485PORT  0x09

//a>>16 ���Ϊ0xffffxxxx   �ͱ���ϵͳ�й�
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
	uint32_t len;//д���У���Ҫд�ĸ��ֽڣ���Ϊ32bit
};
struct Uart_msg
{
	void (* function)(struct Uart_Msg_Msg *msg); //Ԥ������û�õ������պ�����ͬһ��Ϣ�Ĳ�ͬ����Ϣ������
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


//DC-DC ���ݽṹ
struct DC_Data{
	int16_t state0;//0	״̬��0	����״̬	2			
	int16_t state1;//1	״̬��1	���״̬1	2			
	int16_t state2;//2	״̬��2	���״̬	2			
	int16_t state3;//3	״̬��3		2			
	int16_t state4;//4	״̬��4	��·״̬	2			
	int16_t state5;//5	״̬��5		2			
	/*6	����0	����ģʽ	2			�������˵��
	7	����1	ֱ��2�ο���ѹ	2	0.1	V	
	8	����2	ֱ��1�ο���ѹ	2	0.1	V	
	9	����3		2			����
	10	����4		2			����
	11	����5	ֱ��2�ο�����	2	0.1	A	
	12	����6	ֱ��1�ο�����	2	0.1	A	
	13	����7		2			����
	14	����8		2			����
	15	����9	���ػ�����	2			�������˵��
	16	����10					����
	17	����11		2			����
	*/
	int16_t par[12];
	/*
	18	������0		2			����
	19	������1		2			����
	20	������2		2			����
	21	������3		2			����
	22	����ͨ��0		2			
	23	����ͨ��1		2			
	24	����ͨ��2		2			
	25	����ͨ��3		2
	*/
	int16_t rsvd[8];
	int16_t VDC1;//26	����0	ֱ��1��ѹ	2	0.1	V	           ĸ�߲�
	int16_t VDC1_bus_pos;//27	����1	ֱ��1bus����ѹ	2	0.1	V	
	int16_t VDC1_bus_neg;//28	����2	ֱ��1bus����ѹ	2	0.1	V	
	int16_t IDC1;//29	����3	ֱ��1�������	2	0.1	A	
	int16_t IL;//30	����4	��е���	2	0.1	A	
	int16_t IDC2;//31	����5	ֱ��2�������	2	0.1	A	
	int16_t VDC2;//32	����6	ֱ��2��ѹ	2	0.1	V	
	int16_t VDC2_bus_pos;//33	����7	ֱ��2bus����ѹ	2	0.1	V   ������ ���
	int16_t VDC2_bus_neg;//34	����8	ֱ��2bus����ѹ	2	0.1	V	
	int16_t rsvd2[12];
	int16_t T_IGBT1; //47	����21	IGBT�¶�1	2	0.1	��C	
	int16_t T_IGBT2; //48	����22	IGBT�¶�2	2	0.1	��C	
	int16_t rsvd3[7];		
	int16_t mode_hardware;//56	����30	Ӳ��ģʽ	2 
	int16_t stage_run;//57	����31	�����׶�	2
	int16_t V_BMS; //BMS��ص�ѹ
	int16_t I_BMS; //BMS��ص���
	int16_t T_BMS; //BMS����¶�
	int16_t SOC_SOH;//BMS���SOCSOH
	int16_t V_stop_charge;//BMS����ֹ��ѹ
	int16_t V_stop_discharge;//BMS�ŵ��ֹ��ѹ
	int16_t I_max_charge;//BMS���������
	int16_t I_max_discharge;//BMS�ŵ�������

	//�±�Ϊ����RW����
	int16_t rsvd4[4];
	int16_t add_485;//641		1	����0	2			ͨѶ��ַ
	int16_t IDC1_discharge_max;//642		2	����1	2			ֱ��1���ŵ����
	int16_t IDC1_charge_max;//643		3	����2	2			ֱ��1��������
	int16_t VDC1_max;//644		4	����3	2			ֱ��1��ߵ�ѹ
	int16_t VDC1_min;//645		5	����4	2			ֱ��1��͵�ѹ
	int16_t IDC2_discharge_max;//646		6	����5	2			ֱ��2���ŵ����
	int16_t IDC2_charge_max;//647		7	����6	2			ֱ��2��������
	int16_t VDC2_max; //648		8	����7	2			ֱ��2��ߵ�ѹ
	int16_t VDC2_min;//649		9	����8	2			ֱ��2��͵�ѹ
	int16_t rsvd5[4];
};


//���Ƴ������
struct EMS_DATA  //xx������
{
	uint16_t  LEUER_Mod;     //ģʽѡ��   1����  2����  3�Զ�
	uint16_t  P_A;    //Һ�����̨�����޹���ֵ����λKW
	uint16_t  Run;    //һ�����ػ���1����  0�ػ�
	uint16_t  rsvd[8];
	uint16_t  Run_FLAG;   //���ػ���־
	uint16_t  Mod_FLAG;   //ģʽ��־
	uint16_t  Status_FLAG;  //״̬��־����ʾ�����ػ�����̬�Ĳ�ͬ�֡�
	uint16_t  Run_Error_FLAG;  //�����쳣 1Ϊ����0����
	uint16_t  V_Error_FLAG;    //��ѹ�쳣 1Ϊ����0����
	uint16_t  VDC_Error_FLAG;   //ֱ��ĸ�ߵ�ѹ�쳣��־  1����0����
	uint16_t  BAT_Error_FLAG;   //��ص�ѹ�����־  1���� 0����
	uint16_t  COM_Error_FLAG;   //ͨѶ�����־  1���� 0����
	uint16_t  PCS_Com_State;	   //PCSģ��ͨѶ״̬
	uint16_t  DC1_Com_State;	   //dc1ģ��ͨѶ״̬
	uint16_t  DC2_Com_State;     //dc2ģ��ͨѶ״̬
	uint16_t  BAT_Com_State;     //���ģ��ͨѶ״̬
	uint16_t  DC_AC_Com_State;   //dc_acģ��ͨѶ״̬
	
	uint16_t  PCS_Fault;       //PCSģ�����״̬
	uint16_t  DC_ChuNeng_Fault; //dc_����ģ�����״̬
	uint16_t  DC_GuangFu_Fault;//dc_���ģ�����״̬
	uint16_t  DC_AC_Fault;      //dc_acģ�����״̬
};	

struct STRUCT_LEUER_Data{
//	struct WH_PCS_Data PCS;
//	struct DC_Data DC_DC1;
//	struct DC_Data DC_DC2;
	struct EMS_DATA EMS_Data;   //���Ƴ������
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


// ���ݱ�

#pragma pack(2)
struct STRUCT_PCS{
	uint16_t state;//�豸״̬	uint16	"1������ͣ��2������ͣ�� 3��������4���̾��ã�5��ͣ����6������8�����磬9��ָ�����10-����11-�������У�12-��ѹ����13-������14-�����������"
	uint16_t alarm;//�澯����	uint16	1�����1�澯 2�����2�澯
	uint16_t fault;//���ϴ���	uint16	����¼�����������
	int32_t DC_U;//0.001V	ֱ����ѹ	int32	"32λ���ݣ��ȸ�16λ�����16λ����0x1004Ϊֱ����ѹ��16λ��0x1005Ϊֱ����ѹ��16λ����ͬ"
	int32_t DC_I;//0.001A	ֱ������
	int32_t DC_P;//0.001W	ֱ������
	uint16_t rsvd[6];
	uint32_t leijiliang[6];
	/*	
	3015	R	0.001Ah	��ǰ����ģʽ�ۼ�Ah	uint32	
	3016					
	3017	R	0.001Wh	��ǰ����ģʽ�ۼ�Wh	uint32	
	3018					
	3019	R	0.001Ah	����ۼ�Ah	uint32	
	3020					
	3021	R	0.001Wh	����ۼ�Wh	uint32	
	3022					
	3023	R	0.001Ah	�ŵ��ۼ�Ah	uint32	
	3024	R				
	3025	R	0.001Wh	�ŵ��ۼ�Wh	uint32	
	3026	R	
*/
	uint16_t rsvd1[28];//3027-3054			����
	uint32_t sys_u[3];	
	/*
	3055	R	0.001V	����U���ѹ	uint32	
	3056	R				
	3057	R	0.001V	����V���ѹ	uint32	
	3058	R				
	3059	R	0.001V	����W���ѹ	uint32	
	3060	R	
	*/
	int32_t sys_i[3];	
	/*
	3061	R	0.001A	����U�����	int32	
	3062	R				
	3063	R	0.001A	����V�����	int32	
	3064					
	3065	R	0.001A	����W�����	int32	
	3066	
	*/
	int32_t sys_p[3];
	/*
	3067	R	0.001W	ϵͳ�й�����	int32	
	3068					
	3069	R	0.001Var	ϵͳ�޹�����	int32	
	3070					
	3071	R	0.001VA	ϵͳ���ڹ���	int32	
	3072	
	*/
	uint32_t load_u[3];
	/*
	3073	R	0.001V	����U���ѹ	uint32	
	3074	R				
	3075	R	0.001V	����V���ѹ	uint32	
	3076	R				
	3077	R	0.001V	����W���ѹ	uint32	
	3078	W	
	*/
	int32_t load_i[3];
	/*
	3079	R	0.001A	����U�����	int32	
	3080					
	3081	R	0.001A	����V�����	int32	
	3082					
	3083	R	0.001A	����W�����	int32	
	3084		
	*/
	int32_t load_p[3];
	/*
	3085	R	0.001W	�����й�����	int32	
	3086					
	3087	R	0.001Var	�����޹�����	int32	
	3088					
	3089	R	0.001VA	�������ڹ���	int32	
	3090	
	*/
	int16_t PF;//3091	R	0.001	PF	int16
	uint16_t freq;//3092	R	0.01Hz	Ƶ��	uint16
	int16_t temprature_in;//3093	R	0.1��	ģ�������¶�	int16
	int16_t temprature_out;//3094	R	0.1��	ģ�������¶�	int16
	uint16_t VDC; //20210430���ӣ�ֻ��
	int16_t rsvd2[4];
	//ǰ��100����
	uint32_t mode;//3100	����ģʽ		����ģʽ	uint32	����¼һ������ģʽ������				
	int32_t parameter[4];
	/*
	3101					
	3102	���ò���1		���ò���1	int32	ͬ��
	3103					
	3104	���ò���2		���ò���2	int32	ͬ��
	3105					
	3106	���ò���3		���ò���3	int32	ͬ��
	3107					
	3108	���ò���4		���ò���4	int32	ͬ��
	3109
	*/
	int16_t runstop;//3110	����/ͣ������		����/ͣ������	uint16	0���ر�  1������
	uint16_t clearfault;//3111	����״̬�������		����״̬�������	uint16	1���������
	uint16_t autostart;//3112	������������־		������������־	uint16	0���������� 1��������

	uint32_t set[10];
	/*
	3113	ֱ������ѹ	0.001V	ֱ������ѹ	uint32	0.000V~1500.000V
	3114					
	3115	ֱ����С��ѹ	0.001V	ֱ����С��ѹ	uint32	0.000V~1500.000V
	3116					
	3117	��������	0.001A	��������	uint32	0~1500.000A
	3118					
	3119	���ŵ����	0.001A	���ŵ����	uint32	0~1500.000A
	3120					
	3121	����繦��	0.001W	����繦��	uint32	-1500000.0~1500000.0w
	3122					
	3123	���ŵ繦��	0.001W	���ŵ繦��	uint32	-1500000.0~1500000.0w
	3124					
	3125	������ѹ����	0.001V	������ѹ����	uint32	0.000~300.000V
	3126					
	3127	������ѹ����	0.001V	������ѹ����	uint32	0.000~300.000V
	3128					
	3129	������������	0.001A	������������	uint32	
	3130					
	3131	������������	0.001A	������������	uint32	
	3132
	*/
	uint16_t set2[2];
	/*
	3133	����Ƶ������	0.01Hz	����Ƶ������	uint16	40.00~120.00Hz
	3134	����Ƶ������	0.01Hz	����Ƶ������	uint16	40.00~120.00Hz
	*/
	uint16_t rsvd3[5];//3135-3139
	uint32_t bus_V;	//3140	ĸ�ߵ�ѹ	0.001V	ĸ�ߵ�ѹ	uint32				
	uint32_t var;//3142	�޹�����	0.001Var	�޹�����	uint32	
	
	uint16_t rsvd4[6];//�±����������������
	//uint16_t bote;	//3144	����RS485-1������		����RS485-1������	uint16	1��4800��2��9600��3��14400��4��19200��5��38400��6��56000��7��57600��8��115200��9��128000��Ĭ��38400
	//uint16_t timeout;//3145	����RS485-1��ʱ����ʱ��	S	����RS485-1��ʱ����ʱ��	uint16	��Χ��0~65534��0���������ϲ�ͣ��������ֵ��ͨѶ��ʱ����ͣ����Ĭ��Ϊ0
	//uint16_t bingji_mode;//3146	ͨ������ģʽ		ͨ������ģʽ	uint16	0������ 1���������� 2�������ӻ�
	//uint16_t bingji_way;//3147	ͨ��������ʽ		ͨ��������ʽ	uint16	1����ģ�鲢�� 2����ģ�鲢��
	//uint16_t bingji_quan;//3148	ͨ����������		ͨ����������	uint16	
	//uint16_t bingji_scale;//3149	ͨ�������������		ͨ�������������	uint16	
};
#pragma pack()


#pragma pack(2)
struct Hour_Meter {
	uint32_t	Ua; //���ѹ
	uint32_t	Ub; 
	uint32_t	Uc;
	uint32_t	Uab; //�ߵ�ѹ
	uint32_t	Ubc; 
	uint32_t	Uca; 
	uint32_t	Ia; //�����
	uint32_t	Ib; 
	uint32_t	Ic;	
	uint32_t	Pa; //�й�����
	uint32_t	Pb; 
	uint32_t	Pc;
	uint32_t	Ps; 
	uint32_t	Qa; //�޹�����
	uint32_t	Qb; 
	uint32_t	Qc;
	uint32_t	Qs;
	uint32_t	VAa; //���ڹ���
	uint32_t	VAb; 
	uint32_t	VAc;
	uint32_t	VAs;
	uint32_t	PFa; //��������
	uint32_t	PFb; 
	uint32_t	PFc;
	uint32_t	PFs;
	uint32_t	Frequency;
	uint32_t	KWH[6]; //����	
	uint16_t    Harm_U[3]; //г��
	uint16_t    Harm_I[3]; //
};
#pragma pack()

#pragma pack(2)
struct PV_GLASS {
	int32_t		P; //�й�����
	uint32_t	P_DC; //��ֱ���������
	uint32_t	Power_total;//�ܷ�����
	uint32_t	Power_month; //���·�����
	uint32_t	Power_lastmonth; //���·�����
	uint16_t	Power_day;   //���շ�����
};
#pragma pack()
/*
*/
#endif /* __LE_UART_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
