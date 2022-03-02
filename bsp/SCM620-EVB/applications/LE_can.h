/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LE_CAN_H
#define __LE_CAN_H

/* Includes ------------------------------------------------------------------*/
//#include "LE_struct_def.c"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/



#ifndef BIT
    #define BIT(x)				(1uL << (x))
#endif


#define	FLAG_PCS_SEND_PAR 0x01
#define FLAG_PCS_READ_VALUE	0x02 
#define CAN_BAUD_NUM    18		//�������ò����ʸ���

#define CAN0_RX_SIZE		64
#define CAN0_TX_SIZE		32
#define BSPCANTxMsg     CAN_FrameInfoTypeDef	// ���¶���Can�ڲ����Ϳ��ƽṹ�壬�ⲿ�û����ô�
#define BSPCANRxMsg		CAN_FrameInfoTypeDef 	// ���¶���Can�ڲ����տ��ƽṹ�壬�ⲿ�û����ô�
/* Exported macro ------------------------------------------------------------*/
enum MODULE{CAN_PCS=100,CAN_DC400,CAN_DC1,CAN_DC2,CAN_MD950,PMA_BATTERY};
/* Exported functions ------------------------------------------------------- */
//extern void CAN1_Configuration(void);
//extern void can1_tx(unsigned char Data1,unsigned char  Data2); 
//extern void CAN1_WriteData(unsigned int ID);
//extern void CAN_Baud_Process(unsigned int Baud,CAN_InitTypeDef *CAN_InitStructure);
//extern void CAN_Config(void);
//extern void Init_RxMes(CanRxMsg *RxMessage);
//extern uint8_t CAN1_Post(enum MODULE module,uint8_t msgid,uint8_t service_code,uint16_t data,uint32_t pcsid);
//extern uint8_t CAN2_Post(enum MODULE module ,uint8_t msgtype,uint16_t valuetype,uint16_t DC400_add,uint32_t data);

//PCSȫ�ֽṹ��

struct MD950_VALUE 
{
	//���õ�����
	uint16_t run;//���ػ�
	uint16_t power_drection;//��ͣ��ȡ
	uint16_t v_portA;   //A�ڵ�ѹ
	uint16_t i_portA;  //A������
	uint16_t v_portB;   //B�ڵ�ѹ
	uint16_t i_portB;  //B������
	uint16_t p_dalta;  //�´�����delta p
	
	uint16_t rsvd[10];
	//��ʾ������
	uint16_t R_u_portB; //B�ڵ�ѹ
	uint16_t R_i_portA;  //A�ڵ���
	uint16_t R_limitB;   //B�˿�������
	uint16_t R_tempr;    //�����¶�
	uint16_t R_u_portA;  //A�˿ڵ�ѹ
	uint16_t R_limit_u_portB; //B�˿ڵ�ѹ����
	uint16_t R_i_portB;     //B�˿ڵ���
	uint16_t R_limitA;     //A�˿�������
	uint16_t R_limit_u_portA; //A�˿ڵ�ѹ����
	uint16_t R_limit_p; //ģ���޹��ʵ�
	uint16_t R_alarm; //״̬�澯
};

// ���ſ��ƽṹ
typedef struct
{
    SGCC_CAN_TypeDef *CANx;
    //  GPIO_TypeDef *GPIOx;
    uint16_t Port_Pin_Tx;
    uint16_t Port_Pin_Rx;
} _BSPCAN_HD;
//CAN��ѡ��
typedef enum
{
    CAN_PORT0 = 0,
    CAN_PORT1 = 1,
} _BSPCAN_PORT;


//����������
typedef enum
{
    CAN1MBaud   = 3,		// 1MBit/sec
    CAN500kBaud = 4,	 	// 500kBit/sec
    CAN250kBaud = 9, 		// 250kBit/sec
    CAN125kBaud = 18, 		// 125kBit/sec
    CAN100kBaud = 30, 		// 100kBit/sec
    CAN50kBaud  = 45,  		// 50 kBit/sec
    CAN20kBaud  = 100,  	// 20kBit/sec
    CAN10kBaud  = 200   	// 10kBit/sec
} _BSPCAN_Baund;

typedef struct STRUCT_CAN_SET
{
    _BSPCAN_Baund BaundRate;           // ������
    BSPCANTxMsg  *TxMessage;           // ��������
    BSPCANRxMsg  *RxMessage;           // �������ã�CanRxMsg�ṹ������ʽ
    uint16_t     	 RxMaxNum;             // ����Ԫ����������CanRxMsg�����Сֵ
    uint16_t       TxMaxNum;
} BSP_CAN_Set;

typedef struct CANCONTROL
{
    BSP_CAN_Set *pCanElem;             // ����Ԫ�ؿ�����
    uint16_t Rxfront;					   // ���մ���ͷ
    uint16_t Rxrear;					   // ���մ���β
    uint16_t Rxcount;					   // ���ռ���ֵ
    uint16_t Txfront;
    uint16_t Txrear;
    uint16_t Txcount;
} _BSPCAN_Control;


extern struct MD950_VALUE TRD_MD950_Value;
extern void send_can2_MD950(uint16_t modbus_add,uint32_t data);

#endif /* __LE_GLOBAL_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
