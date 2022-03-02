#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
//#include "LE_uart.h"
//#include "LE_can.h"

#include "SCM62X.h"
#include "hal_w25q64.h"

uint32_t  jedec_id = 0x00;
uint8_t status_reg = 0x00;
uint8_t wbuf[255];
uint8_t rbuf[255];

uint16_t i;

FLASH_STATUS_REG_TypeDef g_flash_reg;


FLASH_ID_TypeDef flash_id[] =
{
    {"W25X25",      0xEF3011},
    {"W25Q32D",     0xEF4016},
    {"W25Q16D",  		0xEF4015},
    {"W25Q64D",  		0xEF4017},
};

uint8_t flash_read_status_reg(void)
{
    uint8_t result = 0xff;
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_READ_STATUS_1_REG);
    result = hal_w25q64_rec_byte();
    hal_w25q64_stop();
    return result;
}


void flash_wait_ready(void)
{
    FLASH_STATUS_REG_TypeDef flash_reg;

    do
    {
        flash_reg.reg_val = flash_read_status_reg();
    }
    while(flash_reg.bits.BUSY);
}


void flash_write_status_reg(uint8_t reg_val)
{
    flash_wait_ready();
    flash_write_enable();
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_WRITE_STATUS_REG);
    hal_w25q64_send_byte(reg_val);
    hal_w25q64_stop();
    flash_write_disable();
}


void flash_write_enable(void)
{
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_WRITE_ENABLE);
    hal_w25q64_stop();
}

void flash_write_disable(void)
{
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_WRITE_DISABLE);
    hal_w25q64_stop();
}


void flash_protect(uint8_t protect_opt)
{
    flash_write_status_reg(protect_opt);
}

void flash_write_byte(uint32_t addr, uint8_t val)
{
    flash_wait_ready();
    flash_write_enable();
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_PAGE_PROGRAM);
    hal_w25q64_send_byte((addr & 0xff0000) >> 16);
    hal_w25q64_send_byte((addr & 0xff00) >> 8);
    hal_w25q64_send_byte(addr);
    hal_w25q64_send_byte(val);
    hal_w25q64_stop();
}

uint8_t flash_read_byte(uint32_t addr)
{
    uint8_t result;
    flash_wait_ready();
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_READ_DATA);
    hal_w25q64_send_byte((addr & 0xff0000) >> 16);
    hal_w25q64_send_byte((addr & 0xff00) >> 8);
    hal_w25q64_send_byte(addr);
    result = hal_w25q64_rec_byte();
    hal_w25q64_stop();
    return result;
}


uint8_t flash_write_page(uint32_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t i;

    if(len > FLASH_PAGE_BYTE_CNT)
    {
        return 0;
    }

    //sst25writeen();//去掉写保护
    flash_wait_ready();
    flash_write_enable();
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_PAGE_PROGRAM);
    hal_w25q64_send_byte((addr & 0xff0000) >> 16);
    hal_w25q64_send_byte((addr & 0xff00) >> 8);
    hal_w25q64_send_byte(addr);

    for(i = 0; i < len; i++)
    {
        hal_w25q64_send_byte((*buf + i));
    }

    hal_w25q64_stop();
    return i;
}


void flash_read_bytes(uint32_t addr, uint8_t *buf, uint32_t len)
{
    uint16_t i;
    flash_wait_ready();
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_READ_DATA);
    hal_w25q64_send_byte((addr & 0xff0000) >> 16);
    hal_w25q64_send_byte((addr & 0xff00) >> 8);
    hal_w25q64_send_byte(addr);

    for(i = 0; i < len; i++)
    {
        *(buf + i) = hal_w25q64_rec_byte();
    }

    hal_w25q64_stop();
}


void flash_erase(uint8_t cmd, ...)
{
    uint32_t addr;
    va_list arg_ptr;
    flash_wait_ready();
    flash_write_enable();
    hal_w25q64_start();
    hal_w25q64_send_byte(cmd);

    switch(cmd)
    {
        case FLASH_SECTOR_ERASE:
        case FLASH_BLOCK_32K_ERASE:
        case FLASH_BLOCK_64K_ERASE:
        {
            va_start(arg_ptr, cmd);
            addr = va_arg(arg_ptr, unsigned int);
            va_end(arg_ptr);
            hal_w25q64_send_byte((addr & 0xff0000) >> 16);
            hal_w25q64_send_byte((addr & 0xff00) >> 8);
            hal_w25q64_send_byte(addr);
        }
        break;

        case FLASH_CHIP_ERASE:
            break;

        default:
            break;
    }

    hal_w25q64_stop();
}


uint32_t flash_read_jedec_id(void)
{
    uint8_t i;
    uint32_t jedec_id = 0x00;
    flash_wait_ready();
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_READ_JEDEC_ID);

    for(i = 3; i > 0; i--)
    {
        jedec_id <<= 8;
        jedec_id |= hal_w25q64_rec_byte();
    }

    hal_w25q64_stop();
    return jedec_id;
}


void flash_read_device_id(uint8_t *device_id)
{
    uint8_t i;
    flash_wait_ready();
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_READ_UNIQUE_ID);

    for(i = 0; i < 4; i++)
    {
        hal_w25q64_send_byte(0xFF);
    }

    for(i = 8; i > 0; i--)
    {
        device_id[i - 1] = hal_w25q64_rec_byte();
    }

    hal_w25q64_stop();
}


void SST25VF080B_init(void)
{
    SPI1_InitPad();
    SPI_ResetReg(SGCC_SPI1_P);
}


void hal_w25q64_start(void)
{
    SPI_MASTER initMaster;
    initMaster.rxEn = SPI_CR1_R_EN;
    initMaster.txEn = SPI_CR1_T_EN;
    initMaster.sckDiv = 25;
    initMaster.mode = SPI_CR1_SCK_MOD3;
    initMaster.txFifoLvl = SPI_CR2_TTHD_8;
    initMaster.rxFifoLvl = SPI_CR2_RTHD_0;
    initMaster.DataNumEn = SPI_CR3_DNUM_DIS;
    initMaster.DataNum = 0;
    initMaster.waitNum = 0;
    SPI_InitMaster(SGCC_SPI1_P, &initMaster );
    SPI_StartTransferAsMaster(SGCC_SPI1_P);
}


void hal_w25q64_stop(void)
{
    SPI_StopTransferAsMaster(SGCC_SPI1_P);
}


void hal_w25q64_send_byte(uint8_t val)
{
    SPI_WriteByte(SGCC_SPI1_P, val);
    SPI_RdByte(SGCC_SPI1_P);
}


uint8_t hal_w25q64_rec_byte(void)
{
    uint8_t result;
    SPI_WriteByte(SGCC_SPI1_P, 0xff);
    result = SPI_RdByte(SGCC_SPI1_P);
    return result;
}


void flash_write_en(void)
{
    hal_w25q64_start();
    hal_w25q64_send_byte(FLASH_WRITE_STATUS_EN);
    hal_w25q64_stop();
}

//写保护
void sst25wpen(void)
{
    uint8_t sta;
    sta = flash_read_status_reg() | 0x1c; //读出寄存器并加入写保护位
    flash_write_en();//允许写状态寄存器

    flash_write_status_reg(sta);
}
//先消除保护位，再允许写位
void sst25writeen(void)
{
    uint8_t sta;
    sta = flash_read_status_reg() & (~0x1c); //读出寄存器并消除保护位
    flash_write_en();//允许写状态寄存器
    flash_write_status_reg(sta);//写寄存器
    flash_write_enable();//允许写
}

void SST25_test()
{
    for (i = 0; i < sizeof(wbuf); i++)
    {
        wbuf[i] = i;
    }

    jedec_id = flash_read_jedec_id();
    sst25writeen();
    status_reg	= flash_read_status_reg();
    flash_read_bytes(0x00, rbuf, sizeof(wbuf));
    memset(rbuf, 0x0, sizeof(rbuf));
    flash_erase(FLASH_BLOCK_32K_ERASE, 0x00);
    flash_read_bytes(0x00, rbuf, sizeof(wbuf));
    memset(rbuf, 0x0, sizeof(rbuf));

    for (i = 0; i < sizeof(wbuf); i++)
    {
        flash_write_byte(i, wbuf[i]);
    }

//	flash_write_byte(0x00,0x01);
//	flash_write_byte(0x01,0x02);
//	flash_write_byte(0x02,0x03);
//	flash_write_byte(0x03,0x04);
//	flash_write_byte(0x04,0x05);
//	flash_write_byte(0x05,0x06);

//	flash_write_page(0x00,wbuf,255);

    flash_read_bytes(0x00, rbuf, sizeof(wbuf));
    /*********************************************/
}

void flash_thread_entry(void *parameter)
{
	struct rt_spi_device *spi_dev_sst25;
	
	/* 查 找 spi 设 备 获 取 设 备 句 柄 */
	spi_dev_sst25 = (struct rt_spi_device *)rt_device_find("spi1");
	if (!spi_dev_sst25)
	{
		rt_kprintf("spi sample run failed! can't find %s device!\n", "spi1");
	}

	SST25VF080B_init();
	for(;;)
	{	
//		SST25_test();
//		rt_thread_delay(300);
		for (i = 0; i < sizeof(wbuf); i++)
		{
			wbuf[i] = i;
		}
		
		flash_erase(FLASH_SECTOR_ERASE, 0x00);		
		flash_write_page(0x00, wbuf, sizeof(wbuf));
		
		flash_erase(FLASH_SECTOR_ERASE, 0x01);
		flash_write_page(0x00001000, wbuf, sizeof(wbuf));
		
		flash_erase(FLASH_SECTOR_ERASE, 0x02);
		flash_write_page(0x00002000, wbuf, sizeof(wbuf));
	//	flash_write_page(0x0000100, wbuf, sizeof(wbuf));
	//	flash_write_page(0x0000200, wbuf, sizeof(wbuf));
		
		
		rt_thread_delay(300);
		flash_read_bytes(0x00, rbuf, sizeof(wbuf));
		memset(rbuf, 0x0, sizeof(rbuf));
		flash_read_bytes(0x00001000, rbuf, sizeof(wbuf));
		memset(rbuf, 0x0, sizeof(rbuf));
		flash_read_bytes(0x00002000, rbuf, sizeof(wbuf));
		memset(rbuf, 0x0, sizeof(rbuf));
		


	}	
}

