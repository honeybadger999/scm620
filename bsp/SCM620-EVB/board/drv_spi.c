/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 *
 * Change Logs:
 * Date           Author            Notes
 * 2017-10-20     ZYH            the first version
 * 2017-11-35     ZYH            update to 3.0.0
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>
#include <drivers/spi.h>
#include <board.h>

#ifdef RT_USING_SPI

#define SPIRXEVENT 0x01
#define SPITXEVENT 0x02
#define SPITIMEOUT 2
#define SPICRCEN 0

struct scm620_spi
{
    SGCC_SPI_TypeDef *Instance;
    struct rt_spi_configuration *cfg;
};

static rt_err_t scm620_spi_init(SGCC_SPI_TypeDef *spix, struct rt_spi_configuration *cfg)
{
		SPI_MASTER initMaster;
	  SPI_SLAVE initSlave;

    if (cfg->mode & RT_SPI_SLAVE)
    {		//SLAVE
			if (cfg->mode & RT_SPI_MODE_0)
			{
				initSlave.mode = SPI_CR1_SCK_MOD0;  
			}
			else if (cfg->mode & RT_SPI_MODE_1)
			{
				initSlave.mode = SPI_CR1_SCK_MOD1;  
			}
			else if (cfg->mode & RT_SPI_MODE_2)
			{
				initSlave.mode = SPI_CR1_SCK_MOD2;  
			}
			else if (cfg->mode & RT_SPI_MODE_3)
			{
				initSlave.mode = SPI_CR1_SCK_MOD3;  
			}
			initSlave.rxEn = SPI_CR1_R_EN;
			initSlave.txEn = SPI_CR1_T_EN;
			initSlave.txFifoLvl = cfg->tthd;
			initSlave.rxFifoLvl = cfg->rthd;
			SPI_InitSlave(spix, &initSlave);
			SPI_StartTransferAsSlave(spix);
    }
    else
    {
			if ((cfg->mode & 0x03) == RT_SPI_MODE_0)
			{
				initMaster.mode = SPI_CR1_SCK_MOD0;  
			}
			else if ((cfg->mode & 0x03) == RT_SPI_MODE_1)
			{
				initMaster.mode = SPI_CR1_SCK_MOD1;  
			}
			else if ((cfg->mode & 0x03) == RT_SPI_MODE_2)
			{
				initMaster.mode = SPI_CR1_SCK_MOD2;  
			}
			else if ((cfg->mode & 0x03) == RT_SPI_MODE_3)
			{
				initMaster.mode = SPI_CR1_SCK_MOD3;  
			}
			
			initMaster.rxEn = SPI_CR1_R_EN;
			initMaster.txEn = SPI_CR1_T_EN;
			initMaster.sckDiv = cfg->set_hz;
			initMaster.txFifoLvl = cfg->tthd;
			initMaster.rxFifoLvl = cfg->rthd;
			initMaster.DataNumEn = SPI_CR3_DNUM_DIS;
			initMaster.DataNum = 0;
			initMaster.waitNum = 0;
			SPI_InitMaster(spix, &initMaster ); 
			SPI_StartTransferAsMaster(spix);
    }
    return RT_EOK;
}

#define SPISTEP(datalen) (((datalen) == 8) ? 1 : 2)
#define SPISEND_1(reg, ptr, datalen)       \
    do                                     \
    {                                      \
        if (datalen == 8)                  \
        {                                  \
            (reg) = *(rt_uint8_t *)(ptr);  \
        }                                  \
        else                               \
        {                                  \
            (reg) = *(rt_uint16_t *)(ptr); \
        }                                  \
    } while (0)
#define SPIRECV_1(reg, ptr, datalen)      \
    do                                    \
    {                                     \
        if (datalen == 8)                 \
        {                                 \
            *(rt_uint8_t *)(ptr) = (reg); \
        }                                 \
        else                              \
        {                                 \
            *(rt_uint16_t *)(ptr) = reg;  \
        }                                 \
    } while (0)

static rt_err_t spitxrx1b(struct scm620_spi *hspi, void *rcvb, const void *sndb)
{
    rt_uint32_t padrcv = 0;
    rt_uint32_t padsnd = 0xFF;

    if (!rcvb && !sndb)
    {
        return RT_ERROR;
    }
    if (!rcvb)
    {
        rcvb = &padrcv;
    }
    if (!sndb)
    {
        sndb = &padsnd;
    }
		
//    while (!(hspi->Instance->SPINTSR & SPI_INTR_TNF));
    SPISEND_1(hspi->Instance->SPDR, sndb, hspi->cfg->data_width);
    while (!(hspi->Instance->SPINTSR & SPI_INTR_RNE));
    SPIRECV_1(hspi->Instance->SPDR, rcvb, hspi->cfg->data_width);
    return RT_EOK;
}

static rt_uint32_t spixfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
    rt_err_t res;
    RT_ASSERT(device != RT_NULL);
    RT_ASSERT(device->bus != RT_NULL);
    RT_ASSERT(device->bus->parent.user_data != RT_NULL);
    struct scm620_spi *hspi = (struct scm620_spi *)device->bus->parent.user_data;
    if (message->cs_take)
    {
			//SSN控制-使能
    }
    const rt_uint8_t *sndb = message->send_buf;
    rt_uint8_t *rcvb = message->recv_buf;
    rt_int32_t length = message->length;
    while (length)
    {
        res = spitxrx1b(hspi, rcvb, sndb);
        if (rcvb)
        {
            rcvb += SPISTEP(hspi->cfg->data_width);
        }
        if (sndb)
        {
            sndb += SPISTEP(hspi->cfg->data_width);
        }
        if (res != RT_EOK)
        {
            break;
        }
        length--;
    }

    if (message->cs_release)
    {
        //SSN控制-失能
    }
    return message->length - length;
}

rt_err_t spi_configure(struct rt_spi_device *device,
                       struct rt_spi_configuration *configuration)
{
    struct scm620_spi *hspi = (struct scm620_spi *)device->bus->parent.user_data;
    hspi->cfg = configuration;
    return scm620_spi_init(hspi->Instance, configuration);
}

void spi_stop(struct rt_spi_device *device)
{
    struct scm620_spi *hspi = (struct scm620_spi *)device->bus->parent.user_data;
		SPI_StopTransferAsMaster(hspi->Instance);
}

const struct rt_spi_ops scm_spi_ops =
{
    .configure = spi_configure,
    .xfer = spixfer,
};

struct rt_spi_bus _spi_bus0, _spi_bus1, _spi_bus2, _spi_bus3;
struct scm620_spi _spi0, _spi1, _spi2, _spi3;
int scm620_spi_register_bus(SGCC_SPI_TypeDef *SPIx, const char *name)
{
    struct rt_spi_bus *spi_bus;
    struct scm620_spi *spi;
    if (SPIx == SGCC_SPI0_P)
    {
        spi_bus = &_spi_bus0;
        spi = &_spi0;
    }
    else if (SPIx == SGCC_SPI1_P)
    {
        spi_bus = &_spi_bus1;
        spi = &_spi1;
    }
		else if (SPIx == SGCC_SPI2_P)
    {
        spi_bus = &_spi_bus2;
        spi = &_spi2;
    }
		else if (SPIx == SGCC_SPI3_P)
    {
        spi_bus = &_spi_bus3;
        spi = &_spi3;
    }
    else
    {
        return -1;
    }
    spi->Instance = SPIx;
    spi_bus->parent.user_data = spi;
    return rt_spi_bus_register(spi_bus, name, &scm_spi_ops);
}

int rt_hw_spi_init(void)
{
    int result = 0;
#ifdef RT_USING_SPI0
		SPI0_InitPad();
    result = scm620_spi_register_bus(SGCC_SPI0_P, "spi0");
#endif
#ifdef RT_USING_SPI1
		SPI1_InitPad();
    result = scm620_spi_register_bus(SGCC_SPI1_P, "spi1");
#endif
#ifdef RT_USING_SPI2
	  SPI2_InitPad();
    result = scm620_spi_register_bus(SGCC_SPI2_P, "spi2");
#endif
#ifdef RT_USING_SPI3
		SPI3_InitPad();
    result = scm620_spi_register_bus(SGCC_SPI3_P, "spi3");
#endif
    return result;
}
INIT_BOARD_EXPORT(rt_hw_spi_init);
#endif /* RT_USING_SPI */

