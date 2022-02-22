

#ifndef __SCM62X_CAN_H 
#define __SCM62X_CAN_H 

#include "SCM62X.h"

#define CAN_FRAME_FORMAT_EFF      0x01   // 扩展格式
#define CAN_FRAME_FORMAT_SFF      0x00   // 标准格式
#define CAN_FRAME_TYPE_DATA       0x00   // 数据帧
#define CAN_FRAME_TYPE_REMOTE     0x01   // 远程帧

#define CAN_MODE_RESET                      0x01
#define CAN_MODE_NORMAL_OPERATION           0x00

#define CAN_MODE_LISTEN_ONLY                0x02
#define CAN_MODE_SELF_TEST                  0x04

#define CAN_MODE_FILTER_SIGLE               0x08
#define CAN_MODE_FILTER_DUAL                0x00

#define CAN_MODE_SLEEP                      0x10
#define CAN_MODE_WAKE_UP                    0x00

#define CAN_COMMAND_TRANSMISSION_REQUEST         0x01
#define CAN_COMMAND_ABORT_TRANSMISSION           0x02
#define CAN_COMMAND_RELEASE_RECEIVE_BUFFER       0x04
#define CAN_COMMAND_CLEAR_DATA_OVERRUN           0x08
#define CAN_COMMAND_SELF_RECEPTION_REQUEST       0x10

#define CAN_STATUS_RECEIVE_BUFFER                0x01
#define CAN_STATUS_DATA_OVERRUN                  0x02
#define CAN_STATUS_TRANSMIT_BUFFER               0x04
#define CAN_STATUS_TRANSMISSION_COMPLETE         0x08
#define CAN_STATUS_RECEIVE                       0x10
#define CAN_STATUS_TRANSMIT                      0x20
#define CAN_STATUS_ERROR                         0x40
#define CAN_STATUS_BUS                           0x80

#define CAN_IT_RI                 							 0x01   /* 接收中断 */
#define CAN_IT_TI                                0x02   /* 发送中断 */
#define CAN_IT_EI                                0x04   /* 错误报警中断 */
#define CAN_IT_DOI                               0x08   /* 数据溢出中断 */
#define CAN_IT_WUI                               0x10   /* 唤醒中断 */
#define CAN_IT_EPI                               0x20   /* 错误被动中断 */
#define CAN_IT_ALI                               0x40   /* 仲裁丢失中断 */
#define CAN_IT_BEI                               0x80   /* 总线错误中断 */

#define CAN_ALC_ID28              							0
#define CAN_ALC_ID27              							1
#define CAN_ALC_ID26              							2
#define CAN_ALC_ID25              							3
#define CAN_ALC_ID24              							4
#define CAN_ALC_ID23                            5
#define CAN_ALC_ID22                            6
#define CAN_ALC_ID21                            7
#define CAN_ALC_ID20                            8
#define CAN_ALC_ID19                            9
#define CAN_ALC_ID18                            10
#define CAN_ALC_IDSRTR                          11
#define CAN_ALC_IDIDE                           12
#define CAN_ALC_ID17                            13
#define CAN_ALC_ID16                            14
#define CAN_ALC_ID15                            15
#define CAN_ALC_ID14                            16
#define CAN_ALC_ID13                            17
#define CAN_ALC_ID12                            18
#define CAN_ALC_ID11                            19
#define CAN_ALC_ID10                            20
#define CAN_ALC_ID09                            21
#define CAN_ALC_ID08                            22
#define CAN_ALC_ID07                            23
#define CAN_ALC_ID06                            24
#define CAN_ALC_ID05                            25
#define CAN_ALC_ID04                            26
#define CAN_ALC_ID03                            27
#define CAN_ALC_ID02                            28
#define CAN_ALC_ID01                            29
#define CAN_ALC_ID00                            30
#define CAN_ALC_IDRTR                           31

#define CAN_ECC_DIRECTION_RECEPTION             0x20
#define CAN_ECC_DIRECTION_TRANSMISSION          0x00
#define CAN_ECC_ERROR_BIT                       0x00
#define CAN_ECC_ERROR_FORM                      0x40
#define CAN_ECC_ERROR_STUFF                     0x80
#define CAN_ECC_ERROR_CODE_OTHER                0xC0
#define CAN_ECC_START_OF_FRAME                  0x03
#define CAN_ECC_ID28_21                         0x02
#define CAN_ECC_ID20_18                         0x06
#define CAN_ECC_SRTR                            0x04
#define CAN_ECC_IDE                             0x05
#define CAN_ECC_ID17_13                         0x07
#define CAN_ECC_ID12_5                          0x0F
#define CAN_ECC_ID4_0                           0x0E
#define CAN_ECC_RTR                             0x0C
//#define CAN_ECC_RESERVED1                     0x0D
//#define CAN_ECC_RESERVED0                     0x09
#define CAN_ECC_DATA_LENGTH_CODE                0x0B
#define CAN_ECC_DATA_FIELD                      0x0A
#define CAN_ECC_CRC_SEQUENCE                    0x08
#define CAN_ECC_CRC_DELIMITER                   0x18
#define CAN_ECC_ACK                             0x19
#define CAN_ECC_ACK_DELIMITER                   0x1B
#define CAN_ECC_END_OF_FRAME                    0x1A
#define CAN_ECC_INTERMISSION                    0x12
#define CAN_ECC_ACTIVE_ERROR_FLAG               0x11
#define CAN_ECC_PASSIVE_ERROR_FLAG              0x16
#define CAN_ECC_TOLERATE_DOMINANT               0x13
#define CAN_ECC_ERROR_DELIMITER                 0x17
#define CAN_ECC_OVERLOAD_FLAG                   0x1C

#define CAN_BTR0_SJW0                           (0<<6)
#define CAN_BTR0_SJW1                           (1<<6)
#define CAN_BTR0_SJW2                           (2<<6)
#define CAN_BTR0_SJW3                           (3<<6)
#define CAN_BTR0_BRP(n)                         ((n)&0x3F)

#define CAN_BTR1_SAM_ONCE                       (0<<7)
#define CAN_BTR1_SAM_THREE                      (1<<7)
#define CAN_BTR1_TSEG1(n)                       ((n)&0x0F)
#define CAN_BTR1_TSEG2(n)                       (((n)&0x07)<<4)

#define CAN_OCR_OCM0                            0 //Reserved
#define CAN_OCR_OCM1                            1 //Reserved
#define CAN_OCR_OCM2                            2
#define CAN_OCR_OCM3                            3
 
#define CAN_ODR_CLKOUT_OFF                      0xC8
#define CAN_ODR_CLKOUT_DIV1                     0xC7
#define CAN_ODR_CLKOUT_DIV2                     0xC0
#define CAN_ODR_CLKOUT_DIV4                     0xC1
#define CAN_ODR_CLKOUT_DIV6                     0xC2
#define CAN_ODR_CLKOUT_DIV8                     0xC3
#define CAN_ODR_CLKOUT_DIV10                    0xC4
#define CAN_ODR_CLKOUT_DIV12                    0xC5
#define CAN_ODR_CLKOUT_DIV14                    0xC6


typedef struct
{
    uint32_t CAN_Mode;      /* 模式 ,*/

    uint32_t CAN_BTR0;      /* |  2bit  |       6bit      | */ 
                            /* |  SJW   |        BRP      | */
                            /* BRP: 0~63,TQ = 2 x Tclk x CAN_BRP,, Tclk = 1/fxtal1 */
                            /* SJW: 0~3, 同步跳转位宽 = CAN_SJW + 1 */
    
    uint32_t CAN_BTR1;      /* |1bit|   3bit  |   4bit    | */ 
                            /* |SAM |   TSEG2 |   TSEG1   | */
                            /* SAN: 采样模式位, 0 --- 总线被采样一次, 1 --- 总线被采样三次 */
                            /* TSEG1: 0~15,定义PROP_SEG + PHASE_SEG1的时间长度 = CAN_TSEG1 + 1, 并且指出了采样点的位置 */
                            /* TSEG2: 0~7, 定义PHASE_SEG2的时间长度 =  CAN_TSEG2 + 1 */
    
    uint32_t CAN_OCM;       /* 输出控制模式 0,1保留; 2 正常输出模式, 3 时钟输出模式 
                               在正常输出模式下, TX1的输出时TX0取反;在时钟输出模式下, TX1输出TXCLK;
                               TXCLK的上升沿标志着一个bit传输的开始 */
    
    uint32_t CAN_CDC;       /* 时钟输出和分频控制, 1.CLKOUT输出分频,2.CLKOUT输出关闭 */
    
    uint32_t CAN_ACR;      /* 验收代码,从高位到低位依次是ACR0,ACR1,ACR2,ACR3 */
    uint32_t CAN_AMR;      /* 验收代码屏蔽 从高位到低位依次是AMR0,AMR1,AMR2,AMR3 */
    
} CAN_InitTypeDef;


typedef struct
{
    uint32_t FrameFormat;     /* 帧格式, 标准格式11位识别符; 扩展模式29位识别符 */
    uint32_t FrameID;         /* 帧识别符,根据帧格式可能是11位,也可能是29位*/
    uint32_t FrameType;       /* 帧类型, 数据帧,远程帧 */
    uint32_t DataLength;      /* 数据字段长度 */
    uint8_t  FrameData[8];    /* 数据字段的内容, 最大8个字节 */
    
} CAN_FrameInfoTypeDef;

void CAN_Init(SGCC_CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct);
void CAN_ITConfig(SGCC_CAN_TypeDef* CANx, uint32_t CAN_IT, uint32_t NewState);
uint32_t CAN_GetStatus(SGCC_CAN_TypeDef* CANx);
uint32_t CAN_GetITFlag(SGCC_CAN_TypeDef* CANx);
uint32_t CAN_GetALC(SGCC_CAN_TypeDef* CANx);
void CAN_SetCMD(SGCC_CAN_TypeDef* CANx,uint32_t Command);
void CAN_Sleep(SGCC_CAN_TypeDef* CANx);
void CAN_ConfigError(SGCC_CAN_TypeDef* CANx, uint32_t ErrorWL, uint32_t RxErrorCounter, uint32_t TxErrorCounter);
void CAN_SendData(SGCC_CAN_TypeDef* CANx,CAN_FrameInfoTypeDef *CAN_FrameInfoStruct, uint32_t TransmissionCMD);
void CAN_ReceData(SGCC_CAN_TypeDef* CANx,CAN_FrameInfoTypeDef *CAN_FrameInfoStruct);

#endif  
