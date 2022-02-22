
#ifndef TYPES_H
#define TYPES_H

/* type define */
typedef	unsigned int			UINT32;
typedef	int						INT32;
typedef	unsigned short			UINT16;
typedef	short					INT16;
typedef unsigned char			UINT8;
typedef char					INT8;
typedef unsigned char			BOOL;

typedef	unsigned int			u32;
typedef	unsigned short			u16;
typedef unsigned char			u8;

typedef	unsigned int			uint32_t;
typedef	unsigned short			uint16_t;
typedef unsigned char			uint8_t;

union B16_B08
{
	UINT8 B08[2];
	UINT16  B16;
};

union B32_B08
{
	UINT8  B08[4];
	UINT32 B32;
};

	
#define BIT0                            0x00000001
#define BIT1                            0x00000002
#define BIT2                            0x00000004
#define BIT3                            0x00000008
#define BIT4                            0x00000010
#define BIT5                            0x00000020
#define BIT6                            0x00000040
#define BIT7                            0x00000080
#define BIT8                            0x00000100
#define BIT9                            0x00000200
#define BIT10                           0x00000400
#define BIT11                           0x00000800
#define BIT12                           0x00001000
#define BIT13                           0x00002000
#define BIT14                           0x00004000
#define BIT15                           0x00008000
#define BIT16                           0x00010000
#define BIT17                           0x00020000
#define BIT18                           0x00040000
#define BIT19                           0x00080000
#define BIT20                           0x00100000
#define BIT21                           0x00200000
#define BIT22                           0x00400000
#define BIT23                           0x00800000
#define BIT24                           0x01000000
#define BIT25                           0x02000000
#define BIT26                           0x04000000
#define BIT27                           0x08000000
#define BIT28                           0x10000000
#define BIT29                           0x20000000
#define BIT30                           0x40000000
#define BIT31                           0x80000000

typedef struct
{
	UINT32 smpWidth; //ADC_CON1_SMP_WIDTH_(n)
    UINT32 adcMode;  //ADC_CON1_ADC_MODE_0~ADC_CON1_ADC_MODE_3
    UINT32 waitFetch;   //ADC_CON1_NOTWAITFETCH/ADC_CON1_WAITFETCH
    UINT32 trigEn;   //ENABLE/DISABLE
    UINT32 trigSrc;   //trigSrc
    UINT32 avgTime; //ADC_CON1_AVGTIMES_1/ADC_CON1_AVGTIMES_2/ADC_CON1_AVGTIMES_4/ADC_CON1_AVGTIMES_8/
                   //ADC_CON1_AVGTIMES_16/ADC_CON1_AVGTIMES_32/ADC_CON1_AVGTIMES_64/ADC_CON1_AVGTIMES_128
    UINT32 chSel; //ADC_CON3_MCHS_(n)
    UINT32 chCombined; //ADC_CON3_CHNCOMBINED_1/ADC_CON3_CHNCOMBINED_0
} ADC_PARA ; 

#define PIN_144

#endif 
