#ifndef __SCM62X_H__
#define __SCM62X_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WRITE( addr, data ) *( (volatile unsigned int *) ( addr ) ) = data                          
#define READ( addr )  *( (volatile unsigned int *) ( addr ) ) 



/* -------------------------  Interrupt Number Definition  ------------------------ */

typedef enum IRQn
{
/* -------------------  Cortex-M0 Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn           = -14,      /*!<  2 Non Maskable Interrupt          */
  HardFault_IRQn                = -13,      /*!<  3 HardFault Interrupt             */



  SVCall_IRQn                   =  -5,      /*!< 11 SV Call Interrupt               */

  PendSV_IRQn                   =  -2,      /*!< 14 Pend SV Interrupt               */
  SysTick_IRQn                  =  -1,      /*!< 15 System Tick Interrupt           */

/* ----------------------  SM1226A01 Specific Interrupt Numbers  ------------------ */

  SCU_IRQn                      =   0,      /*!<                                    */
  WDT_IRQn                      =   1,      /*!<                                    */
  IIC0_IRQn                     =   2,      /*!<                                    */ 
  IIC1_IRQn                     =   3,      /*!<                                    */
  IIC2_IRQn                     =   4,      /*!<                                    */
  UART0_IRQn                    =   5,      /*!<                                    */
  UART1_IRQn                    =   6,      /*!<                                    */
  UART2_IRQn                    =   7,      /*!<                                    */
  UART3_IRQn                    =   8,      /*!<                                    */
  UART4_IRQn                    =   9,      /*!<                                    */
  UART5_IRQn                    =   10,      /*!<                                    */
  UART6_IRQn                    =   11,      /*!<                                    */
  UART7_IRQn                    =   12,      /*!<                                    */
  SPI0_IRQn                     =   13,      /*!<                                    */
  SPI1_IRQn                     =   14,      /*!<                                    */
  SPI2_IRQn                     =  15,      /*!<                                    */
  SPI3_IRQn                     =  16,      /*!<                                    */ 
  TIMERA0_IRQn                      =  17,      /*!<                                    */
  TIMERA1_IRQn                      =  18,      /*!<                                    */
  TIMERB0_IRQn                      =  19,      /*!<                                    */
  TIMERB1_IRQn                      =  20,      /*!<                                    */
  TIMERC0_IRQn                      =  21,      /*!<                                    */
  TIMERC1_IRQn                      =  22,      /*!<                                    */
  TIMERD0_IRQn                      =  23,      /*!<                                    */
  TIMERD1_IRQn                      =  24,      /*!<                                    */
  DMA00_IRQn                     =  25,      /*!<                                    */
  DMA01_IRQn                     =  26,      /*!<                                    */
  DMA02_IRQn                     =  27,      /*!<                                    */
  DMA03_IRQn                     =  28,      /*!<                                    */
  DMA10_IRQn                     =  29,      /*!<                                    */
  DMA11_IRQn                     =  30,      /*!<                                    */
  DMA12_IRQn                     =  31,      /*!<                                    */
  DMA13_IRQn                     =  32,      /*!<                                    */
  DMA20_IRQn                     =  33,      /*!<                                    */
  DMA21_IRQn                     =  34,      /*!<                                    */
  DMA22_IRQn                     =  35,      /*!<                                    */
  DMA23_IRQn                     =  36,      /*!<                                    */
  DMA30_IRQn                     =  37,      /*!<                                    */
  DMA31_IRQn                     =  38,      /*!<                                    */
  DMA32_IRQn                     =  39,      /*!<                                    */
  DMA33_IRQn                     =  40,      /*!<                                    */
  ALM_IRQn                      =  41,      /*!<                                    */
  PRD_IRQn                      =  42,      /*!<                                    */
  ANACTL0_IRQn                   =  43,      /*!<                                    */
  GPIO_IRQn                     =  44,      /*!<                                    */
  OTG_IRQn                      =  45,      /*!<                                    */
  GMAC0_IRQn                    =  46,      /*!<                                    */
  GMAC1_IRQn                    =  47,      /*!<                                    */
  U7816_IRQn                    =  48,      /*!<                                    */
  CAN0_IRQn                     =  49,      /*!<                                    */
  CAN1_IRQn                     =  50,      /*!<                                    */
  CAN2_IRQn                     =  51,      /*!<                                    */
  ANACTL1_IRQn                  =  52,      /*!<                                    */
  ANACTL2_IRQn                  =  53,      /*!<                                    */
  NFC_IRQn                      =  55,      /*!<                                    */     
  SM4_IRQn                      =  56,      /*!<                                    */     
} IRQn_Type;


/* ================================================================================ */
/* ================      Processor and Core Peripheral Section     ================ */
/* ================================================================================ */

/* --------  Configuration of the Cortex-M0 Processor and Core Peripherals  ------- */
#define __CM4_REV                 0x0000    /*!< Core revision r0p0                              */
#define __MPU_PRESENT             0         /*!< MPU present or not                              */
#define __NVIC_PRIO_BITS          2         /*!< Number of Bits used for Priority Levels         */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used    */

#include <core_cm4.h>                       /* Processor and core peripherals                    */

#ifndef __cplusplus
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#endif

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/* ================================================================================ */
/* ================       Device Specific Peripheral Section       ================ */
/* ================================================================================ */

/* -------------------  Start of section using anonymous unions  ------------------ */
#if defined(__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined(__ICCARM__)
  #pragma language=extended
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
/* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning 586
#else
  #warning Not supported compiler type
#endif



/* ================================================================================ */
/* ================            IOCTRL UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t IO_CTRL0;
  __IO uint32_t IO_CTRL1;
  __IO uint32_t IO_CTRL2;
  __IO uint32_t IO_CTRL3;
  __IO uint32_t IO_CTRL4;
  __IO uint32_t IO_CTRL5;
  __IO uint32_t IO_CTRL6;
  __IO uint32_t IO_CTRL7;
  __IO uint32_t IO_CTRL8;
  __IO uint32_t IO_CTRL9;
  __IO uint32_t IO_CTRLA;
  __IO uint32_t IO_CTRLB;
  __IO uint32_t IO_CTRLC;
  __IO uint32_t IO_CTRLD;
  __IO uint32_t IO_CTRLE;
  __IO uint32_t IO_CTRLF;
  __IO uint32_t IO_CTRL10;
  __IO uint32_t IO_CTRL11;
  __IO uint32_t IO_CTRL12;
  __IO uint32_t IO_CTRL13; //0x4c
  
  __I uint32_t RESERVED0[12]; 
   
  __IO uint32_t IO_PU0;   //0x80
  __IO uint32_t IO_PU1;
  __IO uint32_t IO_PU2;
  __IO uint32_t IO_PU3;
  __IO uint32_t IO_PU4;  //0x90
  
  __I uint32_t RESERVED1[11]; 
  
  __IO uint32_t IO_PD0; //0xC0
  __IO uint32_t IO_PD1;
  __IO uint32_t IO_PD2;
  __IO uint32_t IO_PD3;
  __IO uint32_t IO_PD4; //0xD0 
  
  __I uint32_t RESERVED2[11]; 
  
  __IO uint32_t IO_DS0;  //0x100
  __IO uint32_t IO_DS1;
  __IO uint32_t IO_DS2;
  __IO uint32_t IO_DS3;
  __IO uint32_t IO_DS4;
  __IO uint32_t IO_DS5;
  __IO uint32_t IO_DS6;
  __IO uint32_t IO_DS7;
  __IO uint32_t IO_DS8;
} SGCC_IOCTRL_TypeDef;


/* ================================================================================ */
/* ================            FLASH UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __O uint32_t PROG_START; //0x0
  __O uint32_t SECTOR_ERASE_START; //0x4
  __O uint32_t CHIP_ERASE_START; //0x8
  __I  uint32_t RESERVED0[5];
  __IO uint32_t STANDBY_EN; //0x20
  __I  uint32_t RESERVED1[7];
  __IO uint32_t PROG_ERASE_FAIL;//0x40
  __IO uint32_t REDUNDANCY_INFO; //0x44
  __I uint32_t FMC_STATUS;  //0x48
  __I  uint32_t RESERVED2[13];
  __IO uint32_t T_1US_REF; //0x80
  __IO uint32_t T_RW_CNT; //0x84
  __IO uint32_t T_AEPH_CNT; //0x88
  __IO uint32_t T_AA_CNT; //0x8c
  __IO uint32_t T_APH_CNT; //0x90
  __IO uint32_t T_DS_CNT; //0x94
  __IO uint32_t T_HV_US_CNT; //0x98
  __IO uint32_t T_NVSP_US_CNT; //0x9c
  __IO  uint32_t T_PGS_US_CNT; //0xA0
  __IO uint32_t T_PROG_US_CNT; //0xA4
  __IO uint32_t T_RCVP_US_CNT; //0xA8
  __IO uint32_t T_SERASE_US_CNT; //0xAC
  __IO uint32_t T_CERASE_US_CNT; //0xB0
  __IO uint32_t T_NVSC_US_CNT; //0xB4
  __IO uint32_t T_RCVE_US_CNT; //0xB8
  __I  uint32_t RESERVED3[113];
  __O uint32_t FMC_PROTECT; //0x280
  
} SGCC_FMC_TypeDef;

/* ================================================================================ */
/* ================            ANALOGCTROL UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t ANA_CFG_REG; //0x0
  __IO uint32_t ANA_STAT_REG; //0x4
  __IO uint32_t ADC_TRIM_REG; //0x8
  __IO uint32_t ADC_RDY_REG;//0xc
  __IO uint32_t ADC_CON1_REG;//0x10
  __IO uint32_t ADC_CON2_REG;//0x14
  __IO uint32_t ADC_CON3_REG;//0x18
  __IO uint32_t ADC_CON4_REG;//0x1c
  __I  uint32_t RESERVED1[4]; 
  __IO uint32_t BGA01_REG;  //0x30  
  __IO uint32_t RESERVED2 ;//0x34
  __IO uint32_t BGB01_REG;  //0x38  
  __IO uint32_t RESERVED3 ;//0x3c
  __IO uint32_t VR12_1_REG ;//0x40
  __IO uint32_t VR12_2_REG ;//0x44
  __IO uint32_t VR16_REG ;//0x48
  __I  uint32_t RESERVED4;//0x4c
  __IO uint32_t VD33_REG ;//0x50
  __IO uint32_t LOSC_REG ;//0x54
  __IO uint32_t HOSC_REG ;//0x58
  __IO uint32_t XLOSC_REG ;//0x5c
  __IO uint32_t XHOSC_REG ;//0x60
  __IO uint32_t ANATEST_REG ;//0x64
	__IO uint32_t TRNG_CFG ;//0x68
  __I  uint32_t RESERVED5[5];//0x6C~0x7c
  __I  uint32_t ADC_DATA_REG0; //0x80
  __I  uint32_t ADC_DATA_REG1; //0x84
  __I  uint32_t ADC_DATA_REG2; //0x88
  __I  uint32_t ADC_DATA_REG3; //0x8c
  __I  uint32_t ADC_DATA_REG4; //0x90
  __I  uint32_t ADC_DATA_REG5; //0x94
  __I  uint32_t ADC_DATA_REG6; //0x98
  __I  uint32_t ADC_DATA_REG7; //0x9c
  __I  uint32_t ADC_DATA_REG8; //0xA0
  __I  uint32_t ADC_DATA_REG9; //0xA4
  __I  uint32_t ADC_DATA_REG10; //0xA8
  __I  uint32_t ADC_DATA_REG11; //0xAC
  __I  uint32_t RESERVED6[19];//0xB0~0xF8
  __IO uint32_t TRIM_PROTECT;
} SGCC_ANACTL_TypeDef;

/* ================================================================================ */
/* ================            RTC UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __O uint32_t Time0;
  __O uint32_t Time1;
  __I uint32_t CurTime0;
  __I uint32_t CurTime1;
  __IO uint32_t AlarmTime0;
  __IO uint32_t AlarmTime1;
  __I  uint32_t RESERVED0[2];
  __IO uint32_t Control;
  __IO uint32_t Status;//0x24
	__IO uint32_t RESERVED1[6];//
	
	__IO uint32_t RTC_CLKRST_cfg;//0x40
	__IO uint32_t RTC_PAD_cfg;//0x44
	__I  uint32_t RESERVED2[2];
	__IO uint32_t BKP[8];//0x50-6C
	__I  uint32_t RESERVED3[4];
	__IO uint32_t BDRST;//0x80
	
} SGCC_RTC_TypeDef;

/* ================================================================================ */
/* ================    SYS CONTROL UNIT (OSC PMU and RESET)        ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t SYS_CTRL;
  __IO uint32_t SYS_STS;
  __IO uint32_t INT_EN;
  __IO uint32_t INT_STS;
  __IO uint32_t CACHE_HIT;
  __IO uint32_t CACHE_MISS;
  __IO uint32_t WAKEUP_EN;  
  __I  uint32_t TRNG_DA;
  __IO uint32_t CAN_CTRL; 
} SGCC_SCU_TypeDef;

/* ================================================================================ */
/* ================    CLK CONTROL UNIT        ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t SYSCLK_CTRL;
  __IO uint32_t HCLK_EN;
  __IO uint32_t PCLK0_EN;
  __IO uint32_t PCLK1_EN;
  __IO uint32_t PLL_CTRL;
} SGCC_CLK_TypeDef;

/* ================================================================================ */
/* ================    RESET CONTROL UNIT        ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t SOFT_RST0;
  __IO uint32_t SOFT_RST1;
  __IO uint32_t SOFT_RST2;
  __IO uint32_t PER_RST;
  __IO uint32_t RST_STS;
} SGCC_RESET_TypeDef;




/*--------------------- OTGPHY ---------------------*/
typedef struct {                            
   __IO  uint32_t OTGPHY_CTRL;
   __IO  uint32_t OTGC_CTRL;
} SGCC_OTGPHY_TypeDef;


/*--------------------- UART ---------------------*/
typedef struct {                            
   __O  uint32_t TXDATA;
   __I  uint32_t RXDATA;
   __IO uint32_t STATUS;
   __IO uint32_t CTRL;
   __IO uint32_t INTSTA;
   __IO uint32_t BAUDDIV;
	__IO uint32_t RTS_CTRL;
} SGCC_UART_TypeDef;

/* ================================================================================ */
/* ================            U7816 UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t BDR;
  __IO uint32_t CHNL;
  __IO uint32_t DFRM;
  __IO uint32_t DATA;
  __IO uint32_t FIFO;
  __IO  uint32_t SR;
  __IO uint32_t INTEN;
  __IO  uint32_t INTFLAG;
} SGCC_U7816_TypeDef;

/*--------------------- DMA ---------------------*/
typedef struct {                         
   __IO  uint32_t TRANS_CTRL;
   __IO  uint32_t TRANS_FMT1;
   __IO uint32_t TRANS_FMT2_STAT2;
   __IO  uint32_t TRANS_STAT1;
   __IO uint32_t TRANS_SRC_ADDR;
   __IO uint32_t TRANS_DST_ADDR;
   __IO uint32_t TRIGEN0_REG;
   __IO uint32_t TRIGEN1_REG;
} SGCC_DMA_TypeDef;



/*--------------------- WDT ---------------------*/
typedef struct {                            /*!< (@ 0x50500000) WDT Structure          */
  __IO uint32_t CTRL;                       /*!< (@ 0x50500000) WDT Control Register.  */
  __O  uint32_t CLR;                        /*!< (@ 0x50500004) WDT Clear Register.  */
} SGCC_WDT_TypeDef;

/*--------------------- T/C ---------------------*/
typedef struct {                            /*!< (@ 0x50600000/ 0x50600040/ 0x50600080/0x506000c0) T/C Structure          */
  __IO uint32_t FSR;                        /*!< (@ 0x50600000) T/C Function Select Register.  */
  __IO uint32_t CR;                         /*!< (@ 0x50600004) T/C Control Register.  */
  __IO uint32_t TC;                         /*!< (@ 0x50600008) T/C Timer Counter Register.  */
  __IO uint32_t PR;                         /*!< (@ 0x5060000C) T/C Prescale Register.  */
  __IO uint32_t PC;                         /*!< (@ 0x50600010) T/C Prescale CountervRegister.  */
  __IO uint32_t MR;                         /*!< (@ 0x50600014) T/C Match Register.  */
  __IO uint32_t MCR;                        /*!< (@ 0x50600018) T/C Match Control Register.  */
  __IO uint32_t EMR;                        /*!< (@ 0x5060001C) T/C Match Out Control Register.  */
  __IO uint32_t EWCR;                       /*!< (@ 0x50600020) T/C Match Out Pulse Width Register.  */
  __IO uint32_t CAPTR  ;                    /*!< (@ 0x50600024) T/C Capture Register.  */
  __IO uint32_t CCR;                        /*!< (@ 0x50600028) T/C Capture Control Register.  */
  __IO uint32_t INTIF;                      /*!< (@ 0x5060002C) T/C Interrupt Flag Register.  */
  __IO uint32_t INTIE;                      /*!< (@ 0x50600030) T/C Interrupt Enable Register.  */
} SGCC_TC_TypeDef;



/*--------------------- PWM ---------------------*/
typedef struct {                            /*!< (@ 0x50F00000/ 0x50F00010/ 0x50F00020) PWM Structure          */
  __IO uint32_t DIV;                        /*!< (@ 0x50500000) PWM Division Register.  */
  __IO uint32_t PERIOD;                     /*!< (@ 0x50500004) PWM Period Register.  */
  __IO uint32_t WIDTH;                      /*!< (@ 0x50500008) PWM Width Register.  */
  __IO uint32_t CONTROL;                    /*!< (@ 0x5050000C) PWM Control Register.  */
} SGCC_PWM_TypeDef;

/* ================================================================================ */
/* ================            SPI UNIT                            ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t SPCR0;
  __IO uint32_t SPCR1;
  __IO uint32_t SPCR2;
  __IO uint32_t SPCR3;
  __I  uint32_t SPSR;
  __IO uint32_t SPWAR;
  __IO uint32_t SPDR;
  __IO uint32_t SPNUMR;
  __IO uint32_t SPINTSR;
  __IO uint32_t SPI_SCKDIV;
} SGCC_SPI_TypeDef;

/* ================================================================================ */
/* ================            I2C UNIT                            ================ */
/* ================================================================================ */
/*--------------------- I2C ---------------------*/
typedef struct {                            /*!<          */
  __IO uint32_t CFGR;                         /*!< (@ 0x50100000) I2C ????? */
  __IO  uint32_t CLKR;                         /*!< (@ 0x50100004) I2C ????? */
  __IO uint32_t ADR;                        /*!< (@ 0x50100008) I2C ?????? */
  __IO uint32_t CMDR;                         /*!< (@ 0x5010000C) I2C ?????  */
  __IO uint32_t SR;                       /*!< (@ 0x50100014) I2C ?????????? */
  __IO  uint32_t BUFR;                        /*!< (@ 0x50100018) ??????? */
} SGCC_I2C_TypeDef;

/*--------------------- GPIO ---------------------*/
typedef struct {                            /*  GPIO Structure          */
  __IO uint32_t DR;                    /*offset[0x0] Port A Data Register  */
  __IO uint32_t DIR;                    /*offset[0x4] Port A Data Direction Register  */
  __IO uint32_t INTER;                  /*offset[0x8] Interrupt Enable Register  */
  __IO  uint32_t INTMR;                 /*offset[0xC] Interrupt Mask Register */
  __IO  uint32_t INTSR;                 /*offset[0x10] Interrupt status register  */
  __IO uint32_t DBSYR;                 /*offset[0x14] Debounce and Syn Register  */
  __IO  uint32_t INTCR;                 /*offset[0x18] Interrupt Clear Register  */
  __IO  uint32_t ETPR;                  /*offset[0x1c] External Port A Register  */
  __IO uint32_t PULLEN;                  /*offset[0x20] Pullenable Regsiter  */
  __IO uint32_t PULLTYPE;                  /*offset[0x24] Pulletype Regsiter  */

} SGCC_GPIO_TypeDef;


/* ================================================================================ */
/* ================            CAN UNIT                            ================ */

/* ================   注意，以下CAN的寄存器均是低8位有效           ================ */
/* ================================================================================ */
typedef struct
{
	__IO uint32_t MOD;                    // 0x00 // 0x00
	__IO uint32_t CMR;                    // 0x04 // 0x01
	__IO uint32_t SR;                     // 0x08 // 0x02
	__IO uint32_t IR;                     // 0x0C // 0x03
	__IO uint32_t IER;                    // 0x10 // 0x04
	__IO uint32_t Reserved05;             // 0x14 // 0x05
	__IO uint32_t BTR0;                   // 0x18 // 0x06
	__IO uint32_t BTR1;                   // 0x1C // 0x07
	__IO uint32_t OCR;                    // 0x20 // 0x08
	__IO uint32_t Reserved09;             // 0x24 // 0x09
	__IO uint32_t Reserved0A;             // 0x28 // 0x0A

    __IO uint32_t ALC;                    // 0x2C // 0x0B
    __IO uint32_t ECC;                    // 0x30 // 0x0C
    __IO uint32_t EWLR;                   // 0x34 // 0x0D
    __IO uint32_t RXERR;                  // 0x38 // 0x0E
    __IO uint32_t TXERR;                  // 0x3C // 0x0F

    __IO uint32_t TFINFO_RFINFO_ACR0;     // 0x40 // 0x10
    __IO uint32_t TDINFO_RDINFO1_ACR1;    // 0x44 // 0x11
    __IO uint32_t TDINFO_RDINFO2_ACR2;    // 0x48 // 0x12
    __IO uint32_t TDINFO_RDINFO3_ACR3;    // 0x4C // 0x13
    __IO uint32_t TDINFO_RDINFO4_AMR0;    // 0x50 // 0x14
    __IO uint32_t TDINFO_RDINFO5_AMR1;    // 0x54 // 0x15
    __IO uint32_t TDINFO_RDINFO6_AMR2;    // 0x58 // 0x16
    __IO uint32_t TDINFO_RDINFO7_AMR3;    // 0x5C // 0x17
    __IO uint32_t TDINFO_RDINFO8;         // 0x60 // 0x18
	__IO uint32_t TDINFO_RDINFO9;         // 0x64 // 0x19
	__IO uint32_t TDINFO_RDINFO10;        // 0x68 // 0x1A
	__IO uint32_t TDINFO_RDINFO11;        // 0x6C // 0x1B
	__IO uint32_t TDINFO_RDINFO12;        // 0x70 // 0x1C
    __IO uint32_t RMC;       			  // 0x74 // 0x1D
    __IO uint32_t RBSA;       			  // 0x78 // 0x1E
    __IO uint32_t CDR;  				  // 0x7C // 0x1F
    __IO uint32_t Receive_FIFO[64];       // 0x80  // 0x20~5F 
    __IO uint32_t Transmit_Buffer[13];    // 0x180 // 0x60~6C
 
} SGCC_CAN_TypeDef;


/*--------------------- CRC ---------------------*/
typedef struct {                            /*!< (@ 0x50300000) CRC Structure          */
  __IO uint32_t CTRL;                       /*!< (@ 0x50300000) CRC 运算控制寄存器.  */
  __I  uint32_t STAT;                        /*!< (@ 0x50300004) CRC 运算状态寄存器.  */
  __I uint32_t RESU;                       /*!< (@ 0x50300008)  CRC 生成结果 Register.  */
  union
  {
      __IO  uint32_t DATA_WORD;                        /*!< (@ 0x5030000C) CRC ????? Register.  */
      __IO  uint16_t DATA_HALFW[2];
      __IO  uint8_t DATA_BYTE[4];
      
  }DATA;
} SGCC_CRC_TypeDef;


/* ================================================================================ */
/* ================    AES CONTROL UNIT        ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t CNTRL;
  __IO uint32_t RESEV[3];
  __IO uint32_t DATA[4];
  __IO uint32_t KEY[8];
  __IO uint32_t RAND[2];
} SGCC_AES_TypeDef;

/* ================================================================================ */
/* ================    SM4 CONTROL UNIT        ================ */
/* ================================================================================ */
typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t MR;
  __IO uint32_t IER;
  __IO uint32_t SR;
  __O uint32_t K1;
  __O uint32_t K2;
  __O uint32_t K3;
  __O uint32_t K4;
  __O uint32_t IV1;
  __O uint32_t IV2;
  __O uint32_t IV3;
  __O uint32_t IV4;
  __IO uint32_t DR1;    
  __IO uint32_t DR2;
  __IO uint32_t DR3;
  __IO uint32_t DR4;
} SGCC_SM4_TypeDef;

/** 
  * @brief Ethernet MAC
  */

typedef struct
{
  __IO uint32_t MACCR;  //0x0
  __IO uint32_t MACFFR; //0x4
  __IO uint32_t MACHTHR; //0x8
  __IO uint32_t MACHTLR; //0xC
  __IO uint32_t MACMIIAR; //0x10
  __IO uint32_t MACMIIDR; //0x14
  __IO uint32_t MACFCR;   //0x18
  __IO uint32_t MACVLANTR;      //0x1C       /*    8 */
       uint32_t RESERVED0[2];
  __IO uint32_t MACRWUFFR;      //0x28       /*   11 */
  __IO uint32_t MACPMTCSR;      //0x2C
  __IO uint32_t MACLPICSR;      //0x30
       uint32_t MACLPITCR;      //0x34
  __IO uint32_t MACSR;          //0x38       /*   15 */
  __IO uint32_t MACIMR;         //0x3C
  __IO uint32_t MACA0HR;        //0x40
  __IO uint32_t MACA0LR;   //0x44
  __IO uint32_t MACA1HR;   //0x48
  __IO uint32_t MACA1LR;   //0x4C
  __IO uint32_t MACA2HR;   //0x50
  __IO uint32_t MACA2LR;   //0x54
  __IO uint32_t MACA3HR;   //0x58
  __IO uint32_t MACA3LR;   //0x5C            /*   24 */
  __IO uint32_t MACA4HR;   //0x60
  __IO uint32_t MACA4LR;   //0x64
  __IO uint32_t MACA5HR;   //0x68
  __IO uint32_t MACA5LR;   //0x6C
  __IO uint32_t MACA6HR;   //0x70
  __IO uint32_t MACA6LR;   //0x74
  __IO uint32_t MACA7HR;   //0x78
  __IO uint32_t MACA7LR;   //0x7C          
  __IO uint32_t MACA8HR;   //0x80
  __IO uint32_t MACA8LR;   //0x84
  __IO uint32_t MACA9HR;   //0x88
  __IO uint32_t MACA9LR;   //0x8C
  __IO uint32_t MACA10HR;  //0x90
  __IO uint32_t MACA10LR;  //0x94
  __IO uint32_t MACA11HR;  //0x98
  __IO uint32_t MACA11LR;  //0x9C           
  __IO uint32_t MACA12HR;  //0xA0
  __IO uint32_t MACA12LR;  //0xA4
  __IO uint32_t MACA13HR;  //0xA8
  __IO uint32_t MACA13R;   //0xAC
  __IO uint32_t MACA14HR;  //0xB0
  __IO uint32_t MACA14LR;  //0xB4
  __IO uint32_t MACA15HR;  //0xB8
  __IO uint32_t MACA15LR;  //0xBC           
  __IO uint32_t MACANCR;   //0xC0
  __IO uint32_t MACANSR;   //0xC4
  __IO uint32_t MACANAR;   //0xC8
  __IO uint32_t MACANLPAR; //0xCC
  __IO uint32_t MACANER;  //0xD0
  uint32_t RESERVED2[11];
  __IO uint32_t MMCCR;     //0x100            /*   65 */
  __IO uint32_t MMCRIR;    //0x104
  __IO uint32_t MMCTIR;    //0x108
  __IO uint32_t MMCRIMR;   //0x10C
  __IO uint32_t MMCTIMR;   //0x110            /*   69 */
       uint32_t RESERVED3[14];
  __IO uint32_t MMCTGFSCCR;            /*   84 */
  __IO uint32_t MMCTGFMSCCR;
       uint32_t RESERVED4[5];
  __IO uint32_t MMCTGFCR;
       uint32_t RESERVED5[10];
  __IO uint32_t MMCRFCECR;
  __IO uint32_t MMCRFAECR;
       uint32_t RESERVED6[10];
  __IO uint32_t MMCRGUFCR;
       uint32_t RESERVED7[334];
  __IO uint32_t PTPTSCR;
  __IO uint32_t PTPSSIR;
  __IO uint32_t PTPTSHR;
  __IO uint32_t PTPTSLR;
  __IO uint32_t PTPTSHUR;
  __IO uint32_t PTPTSLUR;
  __IO uint32_t PTPTSAR;
  __IO uint32_t PTPTTHR;
  __IO uint32_t PTPTTLR;
       uint32_t RESERVED8[567];
  __IO uint32_t DMABMR;
  __IO uint32_t DMATPDR;
  __IO uint32_t DMARPDR;
  __IO uint32_t DMARDLAR;
  __IO uint32_t DMATDLAR;
  __IO uint32_t DMASR;
  __IO uint32_t DMAOMR;
  __IO uint32_t DMAIER;
  __IO uint32_t DMAMFBOCR;
       uint32_t RESERVED9[9];
  __IO uint32_t DMACHTDR;
  __IO uint32_t DMACHRDR;
  __IO uint32_t DMACHTBAR;
  __IO uint32_t DMACHRBAR;
} ETH_TypeDef;

typedef struct {
  __IO  uint32_t ctrl;              // 0x0
  __IO  uint32_t int_msk;
  __IO  uint32_t int_status;
  __IO  uint32_t err_clr;
  __IO  uint32_t flash_config;      // 0x10
  __IO  uint32_t flash_status_mask;
  __IO  uint32_t flash_timing0;      // 0x18
  __IO  uint32_t sys_int_fifo;       // 0x1c
  __IO  uint32_t fatal_err_map_reg;  // 0x28
  __IO  uint32_t nofatal_err_map_reg;
  __IO  uint32_t err_rpt_level;      // 0x30
  __IO  uint32_t bch_dec_done_flag;
  __IO  uint32_t rb0_during_tim;
  __IO  uint32_t rb1_during_tim;
  __IO  uint32_t status_endure_c0;   // 0x40
  __IO  uint32_t status_endure_c1;   
  __IO  uint32_t status_endure_c2;   
  __IO  uint32_t status_endure_c3;   
  __IO  uint32_t ch0_flsh_status_value; // 0x50
        uint32_t reserv0[45]; // 0x54 ~ 0xFC
  __IO  uint32_t ch0_cmdfifo; // 0x100
  __IO  uint32_t ch0_cmdfifo_status; 
  __IO  uint32_t ch0_config; // 0x108
  __IO  uint32_t ch0_scrambler;
  __IO  uint32_t ch0_scrb_init_val; // 0x110
  __IO  uint32_t ch0_bch;
  __IO  uint32_t ch0_bch_oob_byte_len; // 0x118
        uint32_t reserv1; // 0x11C 
  __IO  uint32_t ch0_bscan_ctrl; // 0x120
  __IO  uint32_t ch0_bscan_threshold; // 0x124
        uint32_t reserv2[2]; // 0x128 ~ 0x12C
  __IO  uint32_t ch0_bscan_length; // 0x130
  __IO  uint32_t ch0_num1_in_dma; // 0x134
  __IO  uint32_t ch0_num0_in_dma; // 0x138
        uint32_t reserv3;
  __IO  uint32_t ch0_powr_cycle_flg; // 0x140
  __IO  uint32_t ch0_powr_cycle_flg_feedback; 
  __IO  uint32_t ch0_fw_1B_wr_data0; // 0x148
} SGCC_NFCREG_TypeDef;

typedef struct {
  uint32_t resv00;              // 0x0
  uint32_t resv04;              // 0x4
  uint32_t resv08;              // 0x8
  uint32_t resv0c;              // 0xc
  uint32_t resv10;              // 0x10
  __IO  uint32_t clrfulemp;           // 0x14 块0~31的清空
  __IO  uint32_t setfulemp;           // 0x18 块0~31的置满
  uint32_t resv1C;
  uint32_t resv20;
  __IO  uint32_t fifofulemp;           // 0x24 含义与Ch_fc_dsr_out[1:0]一致 在ful_emp_sel==1时有效。
  __IO  uint32_t fulempsel;           // 0x28 1表示使用软件管理空满  0表示硬件参与处理空满
} SGCC_NFC_fifoREG_TypeDef;

/* --------------------  End of section using anonymous unions  ------------------- */
#if defined(__CC_ARM)
  #pragma pop
#elif defined(__ICCARM__)
  /* leave anonymous unions enabled */
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined(__TASKING__)
  #pragma warning restore
#else
  #warning Not supported compiler type
#endif




/* ================================================================================ */
/* ================              Peripheral memory map             ================ */
/* ================================================================================ */



#define SGCC_CLK_BASE             (0x40000000UL)
#define SGCC_I2C0_BASE            (0x40200000UL)
#define SGCC_I2C1_BASE            (0x40210000UL)
#define SGCC_UART0_BASE           (0x40300000UL)
#define SGCC_UART1_BASE           (0x40310000UL)
#define SGCC_UART2_BASE           (0x40320000UL)
#define SGCC_UART3_BASE           (0x40330000UL)
#define SGCC_PWM0_BASE            (0x40400000UL)
#define SGCC_TIMERA0_BASE          (0x40500000UL)
#define SGCC_TIMERA1_BASE          (0x40500040UL)
#define SGCC_TIMERB0_BASE          (0x40510000UL)
#define SGCC_TIMERB1_BASE          (0x40510040UL)
#define SGCC_SPI0_BASE            (0x40600000UL)
#define SGCC_SPI1_BASE            (0x40610000UL)
#define SGCC_WDT_BASE             (0x40700000UL)
#define SGCC_CAN0_BASE            (0x40800000UL)
#define SGCC_CAN1_BASE            (0x40810000UL)
#define SGCC_RTC_BASE             (0x40900000UL)

#define SGCC_GPIO_BASE            (0x40A00000UL)
#define SGCC_GPIO0_BASE            (0x40A00000UL)
#define SGCC_GPIO1_BASE            (0x40A10000UL)
#define SGCC_GPIO2_BASE            (0x40A20000UL)
#define SGCC_GPIO3_BASE            (0x40A30000UL)

#define SGCC_TEST_BASE            (0x40B00000UL)
#define SGCC_IOCTRL_BASE          (0x40C00000UL)
#define SGCC_U7816_BASE           (0x40D00000UL)

#define SGCC_RESET_BASE           (0x41100000UL)
#define SGCC_I2C2_BASE            (0x41220000UL)
#define SGCC_UART4_BASE           (0x41340000UL)
#define SGCC_UART5_BASE           (0x41350000UL)
#define SGCC_UART6_BASE           (0x41360000UL)
#define SGCC_UART7_BASE           (0x41370000UL)
#define SGCC_PWM1_BASE            (0x41410000UL)
#define SGCC_TIMERC0_BASE          (0x41520000UL)
#define SGCC_TIMERC1_BASE          (0x41520040UL)
#define SGCC_TIMERD0_BASE          (0x41530000UL)
#define SGCC_TIMERD1_BASE          (0x41530040UL)

#define SGCC_SPI2_BASE            (0x41620000UL)
#define SGCC_SPI3_BASE            (0x41630000UL)
#define SGCC_CRC_BASE             (0x41800000UL)
#define SGCC_CAN2_BASE            (0x41820000UL)

#define SGCC_ANACTRL0_BASE        (0x41C00000UL)
#define SGCC_ANACTRL1_BASE        (0x41C10000UL)
#define SGCC_ANACTRL2_BASE        (0x41C20000UL)

#define SGCC_SCU_BASE             (0x41d00000UL)
#define SGCC_CACHE_BASE           (0x41f00000UL)

#define SGCC_OTG_BASE             (0x50000000UL)
#define SGCC_DMA0_BASE            (0x50100000UL)
#define SGCC_DMA1_BASE            (0x50110000UL)
#define SGCC_DMA2_BASE            (0x50120000UL)
#define SGCC_DMA3_BASE            (0x50130000UL)

#define SGCC_GMAC0_BASE           (0x50200000UL)
#define SGCC_GMAC1_BASE           (0x50210000UL)

#define SGCC_AES_BASE             (0x50600000UL)
#define SGCC_SM4_BASE             (0x50610000UL)


#define SGCC_XSRAM_REG_BASE           (0x5FFF0000UL)
#define SGCC_XSRAM_MEM_BASE           (0x60000000UL)


#define ETH_MAC_BASE          (SGCC_GMAC1_BASE)
#define ETH_MMC_BASE          (SGCC_GMAC1_BASE + 0x0100)
#define ETH_PTP_BASE          (SGCC_GMAC1_BASE + 0x0700)
#define ETH_DMA_BASE          (SGCC_GMAC1_BASE + 0x1000)


#define SGCC_FMC_BASE        (0x208000UL)

#define NFC_BASE             (0x50700000UL)   
#define NFC_FIFO_REG_BASE    (0x50750000UL)  //0x0000~0x057F
#define NFC_FIFO_BASE        (0x50750580UL)  //0x0580~0xFFFF 16K的SRAM



/* ================================================================================ */
/* ================             Peripheral declaration             ================ */
/* ================================================================================ */

#define SGCC_FMC_P            ((     SGCC_FMC_TypeDef *)  SGCC_FMC_BASE)
#define SGCC_CRC_P            ((     SGCC_CRC_TypeDef *)  SGCC_CRC_BASE)

#define SGCC_CLK_P            ((     SGCC_CLK_TypeDef *)  SGCC_CLK_BASE)
#define SGCC_I2C0_P           ((     SGCC_I2C_TypeDef *)  SGCC_I2C0_BASE)
#define SGCC_I2C1_P           ((     SGCC_I2C_TypeDef *)  SGCC_I2C1_BASE)
#define SGCC_UART0_P          ((     SGCC_UART_TypeDef *)  SGCC_UART0_BASE)
#define SGCC_UART1_P          ((     SGCC_UART_TypeDef *)  SGCC_UART1_BASE)
#define SGCC_UART2_P          ((     SGCC_UART_TypeDef *)  SGCC_UART2_BASE)
#define SGCC_UART3_P          ((     SGCC_UART_TypeDef *)  SGCC_UART3_BASE)
#define SGCC_PWM00_P           ((     SGCC_PWM_TypeDef *)  (SGCC_PWM0_BASE))
#define SGCC_PWM01_P           ((     SGCC_PWM_TypeDef *)  (SGCC_PWM0_BASE + 0x10))
#define SGCC_PWM02_P           ((     SGCC_PWM_TypeDef *)  (SGCC_PWM0_BASE + 0x20))

#define SGCC_TIMERA0_P            ((     SGCC_TC_TypeDef *)  SGCC_TIMERA0_BASE)
#define SGCC_TIMERA1_P            ((     SGCC_TC_TypeDef *)  SGCC_TIMERA1_BASE)
#define SGCC_TIMERB0_P            ((     SGCC_TC_TypeDef *)  SGCC_TIMERB0_BASE)
#define SGCC_TIMERB1_P            ((     SGCC_TC_TypeDef *)  SGCC_TIMERB1_BASE)
#define SGCC_SPI0_P           ((     SGCC_SPI_TypeDef *)  SGCC_SPI0_BASE)
#define SGCC_SPI1_P           ((     SGCC_SPI_TypeDef *)  SGCC_SPI1_BASE)
#define SGCC_WDT_P            ((     SGCC_WDT_TypeDef *)  SGCC_WDT_BASE)
#define SGCC_CAN0_P            ((     SGCC_CAN_TypeDef *)  SGCC_CAN0_BASE)
#define SGCC_CAN1_P            ((     SGCC_CAN_TypeDef *)  SGCC_CAN1_BASE)
#define SGCC_RTC_P            ((     SGCC_RTC_TypeDef *)  SGCC_RTC_BASE)

#define SGCC_GPIO_P           ((     SGCC_GPIO_TypeDef *)  SGCC_GPIO0_BASE)

#define SGCC_GPIO0_P           ((     SGCC_GPIO_TypeDef *)  SGCC_GPIO0_BASE)
#define SGCC_GPIO1_P           ((     SGCC_GPIO_TypeDef *)  SGCC_GPIO1_BASE)
#define SGCC_GPIO2_P           ((     SGCC_GPIO_TypeDef *)  SGCC_GPIO2_BASE)
#define SGCC_GPIO3_P           ((     SGCC_GPIO_TypeDef *)  SGCC_GPIO3_BASE)

#define SGCC_IOCTRL_P         ((     SGCC_IOCTRL_TypeDef *)  SGCC_IOCTRL_BASE)
#define SGCC_TEST_P           ((     SGCC_TEST_TypeDef *)  SGCC_TEST_BASE)

#define SGCC_RESET_P          ((     SGCC_RESET_TypeDef *)  SGCC_RESET_BASE)
#define SGCC_I2C2_P           ((     SGCC_I2C_TypeDef *)  SGCC_I2C2_BASE)
#define SGCC_UART4_P          ((     SGCC_UART_TypeDef *)  SGCC_UART4_BASE)
#define SGCC_UART5_P          ((     SGCC_UART_TypeDef *)  SGCC_UART5_BASE)
#define SGCC_UART6_P          ((     SGCC_UART_TypeDef *)  SGCC_UART6_BASE)
#define SGCC_UART7_P          ((     SGCC_UART_TypeDef *)  SGCC_UART7_BASE)
#define SGCC_PWM10_P           ((     SGCC_PWM_TypeDef *)  SGCC_PWM1_BASE)
#define SGCC_PWM11_P           ((     SGCC_PWM_TypeDef *)  (SGCC_PWM1_BASE + 0x10))
#define SGCC_PWM12_P           ((     SGCC_PWM_TypeDef *)  (SGCC_PWM1_BASE + 0x20))

#define SGCC_TIMERC0_P         ((     SGCC_TC_TypeDef *)  SGCC_TIMERC0_BASE)
#define SGCC_TIMERC1_P         ((     SGCC_TC_TypeDef *)  SGCC_TIMERC1_BASE)
#define SGCC_TIMERD0_P         ((     SGCC_TC_TypeDef *)  SGCC_TIMERD0_BASE)
#define SGCC_TIMERD1_P         ((     SGCC_TC_TypeDef *)  SGCC_TIMERD1_BASE)

#define SGCC_SPI2_P           ((     SGCC_SPI_TypeDef *)  SGCC_SPI2_BASE)
#define SGCC_SPI3_P           ((     SGCC_SPI_TypeDef *)  SGCC_SPI3_BASE)
#define SGCC_CRC_P            ((     SGCC_CRC_TypeDef *)  SGCC_CRC_BASE)
#define SGCC_CAN2_P           ((     SGCC_CAN_TypeDef *)  SGCC_CAN2_BASE)
#define SGCC_ANACTRL0_P         ((     SGCC_ANACTL_TypeDef *)  SGCC_ANACTRL0_BASE)
#define SGCC_ANACTRL1_P         ((     SGCC_ANACTL_TypeDef *)  SGCC_ANACTRL1_BASE)
#define SGCC_ANACTRL2_P         ((     SGCC_ANACTL_TypeDef *)  SGCC_ANACTRL2_BASE)


#define SGCC_SCU_P            ((     SGCC_SCU_TypeDef *)  SGCC_SCU_BASE)
#define SGCC_CACHE_P           ((UINT32*)(SGCC_CACHE_BASE))

#define SGCC_OTG_P            ((     SGCC_OTG_TypeDef *)  SGCC_OTG_BASE)
#define SGCC_OTGPHY_P         ((     SGCC_OTGPHY_TypeDef *)  (SGCC_OTG_BASE + 0xF00))

#define SGCC_DMA0_P           ((     SGCC_DMA_TypeDef *)  SGCC_DMA0_BASE)
#define SGCC_DMA1_P           ((     SGCC_DMA_TypeDef *)  SGCC_DMA1_BASE)
#define SGCC_DMA2_P           ((     SGCC_DMA_TypeDef *)  SGCC_DMA2_BASE)
#define SGCC_DMA3_P           ((     SGCC_DMA_TypeDef *)  SGCC_DMA3_BASE)

#define SGCC_GMAC0_P          ((     ETH_TypeDef *)  SGCC_GMAC0_BASE)
#define SGCC_GMAC1_P          ((     ETH_TypeDef *)  SGCC_GMAC1_BASE)

#define SGCC_U7816_P              ((   SGCC_U7816_TypeDef *)  SGCC_U7816_BASE)

#define SGCC_FMC_P           ((     SGCC_FMC_TypeDef *)  SGCC_FMC_BASE)

#define SGCC_NFC_FIFO_REG_P   ((     SGCC_NFC_fifoREG_TypeDef *)  NFC_FIFO_REG_BASE)
#define SGCC_NFC_REG_P        ((     SGCC_NFCREG_TypeDef *)  NFC_BASE)

#define SGCC_SM4_P           ((     SGCC_SM4_TypeDef *)  SGCC_SM4_BASE)
#define SGCC_AES_P           ((     SGCC_AES_TypeDef *)  SGCC_AES_BASE)

#include "SCM62X_drivers.h"


#ifdef __cplusplus
}
#endif

#endif  
