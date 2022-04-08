#ifndef __SCM62X_IOCTRL_H 
#define __SCM62X_IOCTRL_H 

#ifndef _IOCTRL_C_
#define EXT_IOCTRL extern
#else
#define EXT_IOCTRL
#endif

#include "types.h"
#include "SCM62X.h"
/********************************************************
 Function:		PWM_InitPad                                                                                                                                                                            
 input:      
							PWM_X -- PWM0/PWM1   
							PWM_CH -- 0/1/2
							PINNUM -- 引脚编号                                                                                    
 Description: PWM管脚功能初始化
********************************************************/
#define PWM_InitPad(PWM_X, PWM_CH, PINNUM) PWM_X##PWM_CH##_InitPad_##PINNUM()

/********************************************************
 Function:		TIMER_EXOUT_InitPad                                                                                                                                                                            
 input:      
							TIMER_X -- TIMERA/TIMERB/TIMERC/TIMERD   
							OUT_CH -- 1/2
							PINNUM -- 引脚编号                                                                                    
 Description: PWM管脚功能初始化
********************************************************/
#define TIMER_EXOUT_InitPad(TIMER_X, OUT_CH, PINNUM) TIMER_X##_EXOUT##OUT_CH##_InitPad_##PINNUM()

/********************************************************
 Function:		TIMER_EXIN_InitPad                                                                                                                                                                            
 input:      
							TIMER_X -- TIMERA/TIMERB/TIMERC/TIMERD   
							IN_CH   -- 1/2/3/4
							PINNUM -- 引脚编号                                                                                    
 Description: PWM管脚功能初始化
********************************************************/
#define TIMER_EXIN_InitPad(TIMER_X,IN_CH,PINNUM) \
do{\
	UINT32 tmp;\
	tmp = (TIMER_X##IN_CH##_IN_##PINNUM##_IOCTRL);\
	tmp &= (~((uint32_t)7<<(TIMER_X##IN_CH##_IN_##PINNUM##_OFFSET)));\
	tmp |= ((uint32_t)(TIMER_X##IN_CH##_IN_##PINNUM##_OPTION)<<(TIMER_X##IN_CH##_IN_##PINNUM##_OFFSET));\
	(TIMER_X##IN_CH##_IN_##PINNUM##_IOCTRL) = tmp;\
}while(0);

#define UART0_PIN44TX 0
#define UART0_PIN45RX 1
#define UART0_PIN96TX 2
#define UART0_PIN97RX 3

#define UART1_PIN109TX 0
#define UART1_PIN110RX 1
#define UART1_PIN113TX 2
#define UART1_PIN116RX 3

#define I2C0_PIN109SDA 0
#define I2C0_PIN137SDA 1
#define I2C0_PIN140SDA 2
#define I2C0_PIN110SCL 3
#define I2C0_PIN136SCL 4
#define I2C0_PIN139SCL 5

#define SPI2_PIN111SCK	0
#define SPI2_PIN112MISO	1
#define SPI2_PIN113MOSI	2
#define SPI2_PIN133SCK	3
#define SPI2_PIN134MISO	4
#define SPI2_PIN135MOSI	5

#define TIMERA2_IN_34_IOCTRL SGCC_IOCTRL_P->IO_CTRL4
#define TIMERA2_IN_34_OFFSET	4
#define TIMERA2_IN_34_OPTION 2
#define TIMERB2_IN_35_IOCTRL SGCC_IOCTRL_P->IO_CTRL4
#define TIMERB2_IN_35_OFFSET	8
#define TIMERB2_IN_35_OPTION 2
#define TIMERC2_IN_36_IOCTRL SGCC_IOCTRL_P->IO_CTRL4
#define TIMERC2_IN_36_OFFSET	12
#define TIMERC2_IN_36_OPTION 2
#define TIMERD2_IN_37_IOCTRL SGCC_IOCTRL_P->IO_CTRL4
#define TIMERD2_IN_37_OFFSET	16
#define TIMERD2_IN_37_OPTION 2
#define TIMERC3_IN_40_IOCTRL SGCC_IOCTRL_P->IO_CTRL4
#define TIMERC3_IN_40_OFFSET	28
#define TIMERC3_IN_40_OPTION 3
#define TIMERA3_IN_42_IOCTRL SGCC_IOCTRL_P->IO_CTRL5
#define TIMERA3_IN_42_OFFSET	4
#define TIMERA3_IN_42_OPTION 3
#define TIMERB3_IN_43_IOCTRL SGCC_IOCTRL_P->IO_CTRL5
#define TIMERB3_IN_43_OFFSET	8
#define TIMERB3_IN_43_OPTION 3
#define TIMERC3_IN_46_IOCTRL SGCC_IOCTRL_P->IO_CTRL5
#define TIMERC3_IN_46_OFFSET	20
#define TIMERC3_IN_46_OPTION 1
#define TIMERD3_IN_47_IOCTRL SGCC_IOCTRL_P->IO_CTRL5
#define TIMERD3_IN_47_OFFSET	24
#define TIMERD3_IN_47_OPTION 1
#define TIMERA3_IN_96_IOCTRL SGCC_IOCTRL_P->IO_CTRLB
#define TIMERA3_IN_96_OFFSET	28
#define TIMERA3_IN_96_OPTION 1
#define TIMERB3_IN_97_IOCTRL SGCC_IOCTRL_P->IO_CTRLC
#define TIMERB3_IN_97_OFFSET	0
#define TIMERB3_IN_97_OPTION 1
#define TIMERC3_IN_98_IOCTRL SGCC_IOCTRL_P->IO_CTRLC
#define TIMERC3_IN_98_OFFSET	4
#define TIMERC3_IN_98_OPTION 1
#define TIMERD3_IN_99_IOCTRL SGCC_IOCTRL_P->IO_CTRLC
#define TIMERD3_IN_99_OFFSET	8
#define TIMERD3_IN_99_OPTION 1
#define TIMERA1_IN_100_IOCTRL SGCC_IOCTRL_P->IO_CTRLC
#define TIMERA1_IN_100_OFFSET	12
#define TIMERA1_IN_100_OPTION 1
#define TIMERB1_IN_101_IOCTRL SGCC_IOCTRL_P->IO_CTRLC
#define TIMERB1_IN_101_OFFSET	16
#define TIMERB1_IN_101_OPTION 1
#define TIMERC1_IN_102_IOCTRL SGCC_IOCTRL_P->IO_CTRLC
#define TIMERC1_IN_102_OFFSET	20
#define TIMERC1_IN_102_OPTION 1
#define TIMERD1_IN_103_IOCTRL SGCC_IOCTRL_P->IO_CTRLC
#define TIMERD1_IN_103_OFFSET	24
#define TIMERD1_IN_103_OPTION 1
#define TIMERA2_IN_104_IOCTRL SGCC_IOCTRL_P->IO_CTRLC
#define TIMERA2_IN_104_OFFSET	28
#define TIMERA2_IN_104_OPTION 1
#define TIMERA2_IN_110_IOCTRL SGCC_IOCTRL_P->IO_CTRLD
#define TIMERA2_IN_110_OFFSET	20
#define TIMERA2_IN_110_OPTION 3
#define TIMERA4_IN_111_IOCTRL SGCC_IOCTRL_P->IO_CTRLD
#define TIMERA4_IN_111_OFFSET	24
#define TIMERA4_IN_111_OPTION 3
#define TIMERB4_IN_112_IOCTRL SGCC_IOCTRL_P->IO_CTRLD
#define TIMERB4_IN_112_OFFSET	28
#define TIMERB4_IN_112_OPTION 3
#define TIMERC4_IN_113_IOCTRL SGCC_IOCTRL_P->IO_CTRLE
#define TIMERC4_IN_113_OFFSET	0
#define TIMERC4_IN_113_OPTION 3
#define TIMERA4_IN_136_IOCTRL SGCC_IOCTRL_P->IO_CTRL10
#define TIMERA4_IN_136_OFFSET	28
#define TIMERA4_IN_136_OPTION 0
#define TIMERB4_IN_137_IOCTRL SGCC_IOCTRL_P->IO_CTRL11
#define TIMERB4_IN_137_OFFSET	0
#define TIMERB4_IN_137_OPTION 0
#define TIMERC4_IN_139_IOCTRL SGCC_IOCTRL_P->IO_CTRL11
#define TIMERC4_IN_139_OFFSET	8
#define TIMERC4_IN_139_OPTION 0
#define TIMERD4_IN_140_IOCTRL SGCC_IOCTRL_P->IO_CTRL11
#define TIMERD4_IN_140_OFFSET	12
#define TIMERD4_IN_140_OPTION 0
#define TIMERB2_IN_141_IOCTRL SGCC_IOCTRL_P->IO_CTRL11
#define TIMERB2_IN_141_OFFSET	16
#define TIMERB2_IN_141_OPTION 2

#define CAN0_PIN28RXD   0
#define CAN0_PIN103RXD  1
#define CAN0_PIN29TXD0  2
#define CAN0_PIN104TXD0 3

/*  -------------------------------------------------------------------------------
 *   API
 *  -------------------------------------------------------------------------------
 */
EXT_IOCTRL void SPI0_InitPad(void);
EXT_IOCTRL void SPI1_InitPad(void);
EXT_IOCTRL void SPI2_InitPad(UINT8 usepinsck, UINT8 usepinmiso, UINT8 usepinmosi);
EXT_IOCTRL void SPI3_InitPad(void);

EXT_IOCTRL void UART0_EnIrda(UINT8 opt);
EXT_IOCTRL void UART0_InitPad(UINT8 usepinrx, UINT8 usepintx);
EXT_IOCTRL void UART1_InitPad(UINT8 usepinrx, UINT8 usepintx);
EXT_IOCTRL void UART2_InitPad(void);
EXT_IOCTRL void UART3_InitPad(void);
EXT_IOCTRL void UART4_InitPad(void);
EXT_IOCTRL void UART5_InitPad(void);
EXT_IOCTRL void UART6_InitPad(void);
EXT_IOCTRL void UART7_InitPad(void);
EXT_IOCTRL void UART2_RTSCTS_InitPad(void);
EXT_IOCTRL void UART3_RTSCTS_InitPad(void);
EXT_IOCTRL void UART6_RTSCTS_InitPad(void);
EXT_IOCTRL void UART7_RTSCTS_InitPad(void);

EXT_IOCTRL void I2C0_InitPad(UINT8 usepinscl, UINT8 usepinsda);
EXT_IOCTRL void I2C1_InitPad(void);
EXT_IOCTRL void I2C2_InitPad(void);

EXT_IOCTRL void SCI0_InitPad(void);
EXT_IOCTRL void SCI1_InitPad(void);

EXT_IOCTRL void CAN0_InitPad(UINT8 usepinscl, UINT8 usepinsda);
EXT_IOCTRL void CAN1_InitPad(void);
EXT_IOCTRL void CAN2_InitPad(void);

EXT_IOCTRL void RTCTM_InitPad(void);

EXT_IOCTRL void PWM00_InitPad_42(void);
EXT_IOCTRL void PWM00_InitPad_96(void);
EXT_IOCTRL void PWM00_InitPad_100(void);
EXT_IOCTRL void PWM01_InitPad_43(void);
EXT_IOCTRL void PWM01_InitPad_97(void);
EXT_IOCTRL void PWM01_InitPad_101(void);
EXT_IOCTRL void PWM01_InitPad_140(void);
EXT_IOCTRL void PWM02_InitPad_40(void);
EXT_IOCTRL void PWM02_InitPad_46(void);
EXT_IOCTRL void PWM02_InitPad_98(void);
EXT_IOCTRL void PWM02_InitPad_102(void);
EXT_IOCTRL void PWM10_InitPad_35(void);
EXT_IOCTRL void PWM10_InitPad_103(void);
EXT_IOCTRL void PWM10_InitPad_111(void);
EXT_IOCTRL void PWM10_InitPad_136(void);
EXT_IOCTRL void PWM11_InitPad_47(void);
EXT_IOCTRL void PWM11_InitPad_36(void);
EXT_IOCTRL void PWM11_InitPad_99(void);
EXT_IOCTRL void PWM11_InitPad_104(void);
EXT_IOCTRL void PWM11_InitPad_112(void);
EXT_IOCTRL void PWM11_InitPad_137(void);
EXT_IOCTRL void PWM12_InitPad_34(void);
EXT_IOCTRL void PWM12_InitPad_37(void);
EXT_IOCTRL void PWM12_InitPad_110(void);
EXT_IOCTRL void PWM12_InitPad_113(void);
EXT_IOCTRL void PWM12_InitPad_139(void);

EXT_IOCTRL void TIMERA_EXOUT1_InitPad_42(void);
EXT_IOCTRL void TIMERA_EXOUT1_InitPad_96(void);
EXT_IOCTRL void TIMERA_EXOUT1_InitPad_100(void);
EXT_IOCTRL void TIMERA_EXOUT2_InitPad_34(void);
EXT_IOCTRL void TIMERA_EXOUT2_InitPad_104(void);
EXT_IOCTRL void TIMERA_EXOUT2_InitPad_136(void);
EXT_IOCTRL void TIMERB_EXOUT1_InitPad_43(void);
EXT_IOCTRL void TIMERB_EXOUT1_InitPad_97(void);
EXT_IOCTRL void TIMERB_EXOUT1_InitPad_101(void);
EXT_IOCTRL void TIMERB_EXOUT2_InitPad_35(void);
EXT_IOCTRL void TIMERB_EXOUT2_InitPad_137(void);
EXT_IOCTRL void TIMERC_EXOUT1_InitPad_46(void);
EXT_IOCTRL void TIMERC_EXOUT1_InitPad_98(void);
EXT_IOCTRL void TIMERC_EXOUT1_InitPad_102(void);
EXT_IOCTRL void TIMERC_EXOUT2_InitPad_36(void);
EXT_IOCTRL void TIMERC_EXOUT2_InitPad_139(void);
EXT_IOCTRL void TIMERD_EXOUT1_InitPad_47(void);
EXT_IOCTRL void TIMERD_EXOUT1_InitPad_99(void);
EXT_IOCTRL void TIMERD_EXOUT1_InitPad_103(void);
EXT_IOCTRL void TIMERD_EXOUT2_InitPad_37(void);
EXT_IOCTRL void TIMERD_EXOUT2_InitPad_140(void);

EXT_IOCTRL void EXIN0_GPIO1_InitPad(void);
EXT_IOCTRL void EXIN1_InitPad(void);
EXT_IOCTRL void EXIN2_InitPad(void);
EXT_IOCTRL void EXIN3_InitPad(void);

EXT_IOCTRL void CMPIN0_InitPad(UINT8 opt);
EXT_IOCTRL void CMPIN1_InitPad(UINT8 opt);
EXT_IOCTRL void CMPIN2_InitPad(UINT8 opt);
EXT_IOCTRL void CMPIN3_InitPad(UINT8 opt);

EXT_IOCTRL void C1OUT_InitPad(void);
EXT_IOCTRL void C2OUT_InitPad(void);

EXT_IOCTRL void POUT0_InitPad(void);
EXT_IOCTRL void POUT1_InitPad(void);
EXT_IOCTRL void POUT2_InitPad(void);
EXT_IOCTRL void POUT3_POUT4_SYNCCLK_InitPad(void);
EXT_IOCTRL void POUT5_InitPad(void);
EXT_IOCTRL void POUT6_InitPad(void);
EXT_IOCTRL void POUT7_InitPad(void);
EXT_IOCTRL void POUT8_InitPad(void);
EXT_IOCTRL void POUT9_InitPad(void);
EXT_IOCTRL void POUT10_InitPad(void);
EXT_IOCTRL void POUT11_InitPad(void);
EXT_IOCTRL void POUT13_InitPad(void);
EXT_IOCTRL void POUT14_POUT15_InitPad(void);

EXT_IOCTRL void XTALI_InitPad(void);
EXT_IOCTRL void XTALO_InitPad(void);
EXT_IOCTRL void XHOSC_InitPad(void);
EXT_IOCTRL void XTHF_InitPad(void);
EXT_IOCTRL void FDET_RTC_InitPad(void);
EXT_IOCTRL void XTHFDET_CLK_InitPad(void);
EXT_IOCTRL void VOSC_VC_InitPad(void);
EXT_IOCTRL void ATEST_InitPad(void);
EXT_IOCTRL void IBP0P4U_InitPad(void);
EXT_IOCTRL void HAFV_InitPad(void);
EXT_IOCTRL void TESTIN1_InitPad(void);
EXT_IOCTRL void TESTIN2_InitPad(void);

EXT_IOCTRL void VDOUT_InitPad(void);
EXT_IOCTRL void AIN0_InitPad(void);
EXT_IOCTRL void AIN1_InitPad(void);

EXT_IOCTRL void AIN2_InitPad(void);
EXT_IOCTRL void AIN3_InitPad(void);
EXT_IOCTRL void AIN4_InitPad(void);
EXT_IOCTRL void AIN5_InitPad(void);
EXT_IOCTRL void AIN6_InitPad(void);
EXT_IOCTRL void AIN7_InitPad(void);

EXT_IOCTRL void SPI3_ConfigDs(UINT8 opt);
EXT_IOCTRL void SPI2_ConfigDs(UINT8 opt);
EXT_IOCTRL void SPI1_ConfigDs(UINT8 opt);
EXT_IOCTRL void SPI0_ConfigDs(UINT8 opt);

EXT_IOCTRL void PIN_CfgPullAbility(UINT8 pinCfg, UINT8 opt);

EXT_IOCTRL void GPIO_InitPad(UINT8 ioNo);
EXT_IOCTRL void NandFlashCtrl_InitPad(void);
EXT_IOCTRL void MCO_InitPad(void);

EXT_IOCTRL void RMII1_InitPad(void);
EXT_IOCTRL void MII0_InitPad(void);

void RTCTM_InitPad(void);
void IO_ConfigPullUp(UINT8 ioNo, FunctionalState opt);
void IO_ConfigPullDown(UINT8 ioNo, FunctionalState opt);
#endif  
                                                                                                   
