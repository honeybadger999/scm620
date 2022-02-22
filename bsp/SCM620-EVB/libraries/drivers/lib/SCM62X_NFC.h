
#ifndef _SCM62X_NFC_H_
#define _SCM62X_NFC_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "types.h"


#define FC_CE_CHIP0 0
#define FC_CE_CHIP1 1
#define FC_CE_CHIP2 2
#define FC_CE_CHIP3 3

#define FC_CMD_TYPE_COMMAND  1
#define FC_CMD_TYPE_ADDRESS  2
#define FC_CMD_TYPE_FDMA     3
#define FC_CMD_TYPE_STATUS   4
#define FC_CMD_TYPE_WAITR_B  5
#define FC_CMD_TYPE_DELAY    6
#define FC_CMD_TYPE_RUN      7

#define FC_CMD_ADDR_LOW_1BYTE 1
#define FC_CMD_ADDR_LOW_2BYTE 2
#define FC_CMD_ADDR_3BYTE     3

#define FC_CMD_FDMA_DATA_WRITE 0
#define FC_CMD_FDMA_DATA_READ  2

#define FC_CMD_FDMA_MODE0 0
#define FC_CMD_FDMA_MODE1 1

//NFC_CTRL¼Ä´æÆ÷
#define Ch_fifo_clrB 16
#define Ch_rstB      8
#define Ch_haltB     4
#define Ch_enB       0

//NFC_INT_MASK¼Ä´æÆ÷
#define Int_fifo_mskB    4
#define Success_Int_mskB 2
#define Status_Err_mskB  1
#define Rb_err_mskB      0

//NFC_INT_STATUS¼Ä´æÆ÷ R
#define Int_sys_fullB           1
#define Int_sys_not_emptyB      0

//FLASH_CONFIG¼Ä´æÆ÷
#define Flash_typeB     0

//FLASH STATUS MASK¼Ä´æÆ÷
#define Status_rpt_time2B      16
#define Wait_timesB            8
#define Flash_status_maskB     0

//FLASH_TIMING0¼Ä´æÆ÷
#define Delay_selB            4
#define tPWTB                 0

//SYS_INT_FIFO¼Ä´æÆ÷ R
#define Success_intB          7
#define Status_errB           6
#define Rb_errB               5
#define Op_idB                0

//FATAL_ERR_MAP_REG¼Ä´æÆ÷

//bch unit
#define MODE512 1
#define MODE1K  0

#define BCH_unit_B 8


/******************************************************************************/
/*                                Íâ²¿ÒýÓÃ¶¨Òå                                */
/******************************************************************************/
extern void fc_send_cmd(unsigned char ce, // 0:chip 0; 1:chip 1; 2:chip 2; 3:chip 3;
                 unsigned char cmd_type, //1:command; 2:address; 3:fdma; 4:status; 5:wait R/#B; 6:delay; 7:run
                 unsigned char opt, //when address: 1: low 1byte; 
                                    //              2: low 2byte; 
                                    //              3 or 0: 3byte 
                                    //when fdma:   option[1]=0: data write; 
                                    //             option[1]=1: data read; 
                                    //             option[0]=0: mode 0; 
                                    //             option[0]=1: mode 1
                 unsigned int CodeIn);

extern void wait_cmd_success(void);





#endif

