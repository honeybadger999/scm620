
/****************************SPI FLASH***********************************************/

typedef enum ERTYPE{Sec1,Sec8,Sec16,Chip} ErType;  
typedef enum IDTYPE{Manu_ID,Dev_ID,Jedec_ID} idtype;
#define MAX_ADDR		0x1FFFFF	
#define	SEC_MAX     	511         
#define SEC_SIZE		0x1000      


#define FLASHADD_INFO	 0x00001100//参数地址
#define FLASHQUAN_INFO      (11)   //参数数量
#define FLASHSECTOR_INFO     1  //存放的sector   一个sector 是 0x1000字节

#define FLASHADD_POWER	 0x00002100//参数地址
#define FLASHQUAN_POWER      (6)   //参数数量
#define FLASHSECTOR_POWER     2  //存放的sector   一个sector 是 0x1000字节

#define FLASHADD_ETH	 0x00003100//参数地址
#define FLASHQUAN_ETH      (14)   //参数数量
#define FLASHSECTOR_ETH     3  //存放的sector   一个sector 是 0x1000字节

extern void  SPI_FLASH_Init(void);
extern uint8_t SSTF016B_RD(uint32_t Dst, uint8_t* RcvBufPt ,uint32_t NByte);
//extern uint8_t SSTF016B_RdID(idtype IDType,uint32_t* RcvbufPt);
extern uint8_t SSTF016B_WR(uint32_t Dst, uint8_t* SndbufPt,uint32_t NByte);
extern uint8_t SSTF016B_Erase(uint32_t sec1, uint32_t sec2);
//extern void SPI_FLASH_Test(void);
//extern void df_write_open(uint32_t addr);
//extern void df_write(uint8_t *buf,uint16_t size);
//extern void df_read(uint8_t *buf,uint16_t size);
//extern void df_read_open(uint32_t addr);
//extern void df_write_close(void);
extern void Store_par_DataFlash(uint16_t *P_DATA_PAR,uint16_t len,uint16_t sector,uint32_t ParaFlashADD);

extern uint8_t	Buffer_DataFlash[256];
//extern void internal_flash_test(void);


/****************************SPI***********************************************/
