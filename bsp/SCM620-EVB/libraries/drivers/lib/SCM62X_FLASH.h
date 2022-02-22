#ifndef __SCM62X_FLASH_H__
#define __SCM62X_FLASH_H__

#include "types.h"
#include "SCM62X.h"

#define FLASH_CLK     40   //100MHz

#define FLASH_PAGE_SIZE      256
#define FLASH_SECTOR_SIZE    512

#define MAIN_BLK_SIZE        0x100000
#define NVR_BLK_SIZE         0x1000
#define REDUND_BLK_SIZE      0x1000


#define SGCC_FLASH_BASE      (0x100000UL)
#define MAIN_BLK_BASE        (SGCC_FLASH_BASE)
#define NVR_BLK_BASE         (MAIN_BLK_BASE + MAIN_BLK_SIZE)
#define REDUND_BLK_BASE      (NVR_BLK_BASE + NVR_BLK_SIZE)



#define NVR_KEY           0x10F0C105
#define BOOT_STATUS       0x55AAAA55
#define UPS_KEY           0x0A10C105


#define NVR_KEY_ADDR      0x100200
#define UPS_KEY_ADDR      0x7FFF8
#define BOOT_LOCATION     0x7FFFC

#define MAX_T_1USREF      1000   //1us
#define MIN_T_RW          100    //100ns
#define MIN_T_AEPH         10   //(寄存器值+1)*时钟周期不能小于10ns
#define MIN_T_AA          35   //(寄存器值+2)*时钟周期不能小于35ns
#define MIN_T_APH         15   //15ns
#define MIN_T_DS          15   //15ns
#define MAX_T_HV_US       3000000UL   //3ms
#define MIN_T_NVSP_US     4000   //4us
#define MIN_T_PGS_US      20000UL  //20us  
#define MIN_T_PROG_US     16000UL  //16us
#define MAX_T_PROG_US     24000UL  //24us
#define MIN_T_RCVP_US     5000     //5us
#define MIN_T_SERASE_US   3200000UL    //3.2ms
#define MAX_T_SERASE_US   4800000UL    //4.8ms
#define MIN_T_CERASE_US   8000000UL    //8ms
#define MAX_T_CERASE_US   12000000UL   //12ms
#define MIN_T_NVSC_US     10000     //10us
#define MIN_T_RCVE_US     50000     //50us


typedef struct
{
    UINT32 t_1usRef;
    UINT32 t_rwCnt;
    UINT32 t_aephCnt;
    UINT32 t_aaCnt;
    UINT32 t_aphCnt;
    UINT32 t_dsCnt;
    UINT32 t_hvUsCnt;
    UINT32 t_nvspUsCnt;
    UINT32 t_pgsUsCnt;
    UINT32 t_progUsCnt;
    UINT32 t_rcvpUsCnt;
    UINT32 t_seraseUsCnt;
    UINT32 t_ceraseUsCnt;
    UINT32 t_nvscUsCnt;
    UINT32 t_rcveUsCnt;
} FLASH_TIMPARA ;


//#define Write32(x,y)   *(volatile UINT32 *)((UINT8 * )x)=(y)  
//#define Read32(x)      *((volatile UINT32 *)((UINT8 * )x)) 
#define Write32(x,y)   *(volatile UINT32 *)(x)=(y)  
#define Read32(x)      *((volatile UINT32 *)(x)) 
#define Write8(x,y)   *((UINT8 * )x)=(y)  


/*API*/
void FLASH_EnableSupervisor(UINT8 option ) ;
UINT8 FLASH_SetNvrKey(void) ;
UINT8 FLASH_SetUpsKey(void) ;
void FLASH_ConfigTimPara(FLASH_TIMPARA timPara ) ;
UINT8 FLASH_ProgSingleWord(UINT32 *addr, UINT32 dataWord) ;
UINT8 FLASH_ProgWordsInPage(UINT32 *addr, UINT32 *buf, UINT32 len); 
UINT8 FLASH_ProgMultiWords(UINT32 *addr, UINT32 *buf, UINT32 len); 
UINT8 FLASH_SectorErase(UINT32 *eraseAdr) ;
UINT8 FLASH_ChipErase(UINT32 *eraseAdr) ;
void EFLASH_Read(UINT32 *addr,  UINT32 *buf, UINT32 len);

void EFLASH_EnStdby(UINT8 opt);
void EFLASH_EnDeepPowerDown(UINT8 opt);


#endif   
