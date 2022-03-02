#ifndef __HAL_W25Q64_H__
#define __HAL_W25Q64_H__

#include "stdint.h"

typedef struct
{
    char *pStr;
    unsigned long ulFramID;
} FLASH_ID_TypeDef;


typedef union
{
    struct
    {
        uint8_t BUSY: 1;
        uint8_t WEL: 1;
        uint8_t BP0: 1;
        uint8_t BP1: 1;
        uint8_t BP2: 1;
        uint8_t TB: 1;
        uint8_t SEC: 1;
        uint8_t SRP0: 1;
    } bits;
    uint8_t reg_val;
} FLASH_STATUS_REG_TypeDef;





/************************ W25Q64 ÃüÁî×Ö**********************************/
#define FLASH_WRITE_ENABLE	         		                0x06	//Ð´Ê¹ÄÜ
#define FLASH_WRITE_DISABLE				                      0x04	//Ð´½ûÖ¹
#define FLASH_READ_STATUS_1_REG   				              0x05	//¶Á×´Ì¬¼Ä´æÆ÷1
#define FLASH_READ_STATUS_2_REG   				              0x35	//¶Á×´Ì¬¼Ä´æÆ÷2
#define FLASH_WRITE_STATUS_REG				                  0x01	//Ð´×´Ì¬¼Ä´æÆ÷
#define FLASH_WRITE_STATUS_EN				                    0x50	//ÔÊÐíÐ´×´Ì¬¼Ä´æÆ÷




#define FLASH_READ_DATA				                          0x03	//¶ÁÊý¾Ý
#define FLASH_FAST_READ_DATA        			              0x0B	//¿ìËÙ¶ÁÊý¾Ý
#define FLASH_FAST_READ_DUAL_OUTPUT                     0x3B  //
#define FLASH_FAST_READ_QUAD_OUTPUT                     0x6B  //
#define FLASH_FAST_READ_DUAL_IO                         0xBB  //
#define FLASH_FAST_READ_QUAD_IO                         0xEB  //
#define FLASH_OCTAL_WORD_READ_QUAD_IO                   0xE3  //
#define FLASH_PAGE_PROGRAM         					            0x02	//Ò³±à³Ì
#define FLASH_SECTOR_ERASE			                        0x20	//ÉÈÇø²Á³ý
#define FLASH_BLOCK_32K_ERASE 	                        0x52  //32K¿é²Á³ý
#define FLASH_BLOCK_64K_ERASE 	                        0xD8  //64K¿é²Á³ý
#define FLASH_CHIP_ERASE 				                        0xC7  //ÕûÆ¬²Á³ý
#define FLASH_ERASE_SUSPEND                             0x75  //
#define FLASH_ERASE_RESUME                              0x75  //
#define FLASH_POWER_DOWN					                      0xB9  //
#define FLASH_HIGH_PERFORMANCCE_MODE                    0xA3  //
#define FLASH_RELEASE_POWER_DOWN_OR_HIGH_PERFORMANCCE   0xAB  //
#define FLASH_READ_MANUFACTURER_DEVICE_ID               0x90  //
#define FLASH_READ_UNIQUE_ID                            0x4B  //¶ÁÎ¨Ò»ID
#define FLASH_READ_JEDEC_ID				                      0x9F	//¶ÁJEDEC ID
#define FLASH_CONTINUOUS_READ_MODE_RESET                0xFF






#define FLASH_STATUS_BUSY			             0x01	//FLAHS×´Ì¬Ã¦ 
#define FLASH_STATUS_WRITE_ENABLE			     0x02	//FLASH×´Ì¬Ð´Ê¹ÄÜ


#define FLASH_PROTECT_LOW_HALF	           0x38	//
#define FLASH_PROTECT_ALL		               0x1C	//
#define FLASH_PROTECT_NONE		             0x00	//



#define FLASH_WRITE_PROTECT_IO             7
#define FLASH_CS_IO                        6

#define FLASH_PAGE_BYTE_CNT                256


void flash_write_enable(void);
void flash_write_disable(void);
uint8_t flash_read_status_reg(void);
void flash_write_status_reg(uint8_t reg_val);
uint32_t flash_read_jedec_id(void);
void flash_read_device_id(uint8_t *device_id);

void flash_erase(uint8_t cmd, ...);
void flash_protect(uint8_t protect_opt);
void flash_write_byte(uint32_t addr, uint8_t val);
uint8_t flash_read_byte(uint32_t addr);
uint8_t flash_write_page(uint32_t addr, uint8_t *buf, uint16_t len);
void flash_read_bytes(uint32_t addr, uint8_t *buf, uint32_t len);

void SST25VF080B_init(void);
void hal_w25q64_send_byte(uint8_t val);
uint8_t hal_w25q64_rec_byte(void);
void hal_w25q64_start(void);
void hal_w25q64_stop(void);


void sst25wpen(void);
void sst25writeen(void);
extern void flash_thread_entry(void *parameter);


#endif /*__HAL_W25Q64_H__*/


