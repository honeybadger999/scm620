/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-06-05     tanek        first implementation.
 * 2018-03-08     ZYH          Porting for stm32f4xx
 */
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/i2c.h>
#include <drivers/i2c_dev.h>
#include <board.h>

#ifdef RT_USING_I2C

struct scm620_i2c
{
    SGCC_I2C_TypeDef *Instance;
};

static rt_err_t scm620_i2c_init(SGCC_I2C_TypeDef *I2C,uint32_t addr)
{
		I2C_InitTypeDef i2cInit;
	
		i2cInit.mode = I2C_MASTER;
    i2cInit.scl = 400 * 1000;
    i2cInit.prescale = I2C_CFGR_DB0;
		i2cInit.addrMode = addr;
    I2C_Init(I2C,i2cInit) ;

    I2C_Enable(I2C);
    return RT_EOK;
}

static int scm620_i2c_read(SGCC_I2C_TypeDef *I2C, rt_uint16_t slave_address, rt_uint8_t* p_buffer, rt_uint16_t data_byte)
{
		uint8_t  result;

    /* send a start condition to I2C bus */
		result = I2C_Start(I2C);
		if(result == 0)
		{
			return RT_ERROR;
		}
		
    /* send slave address to I2C bus */
    result = I2C_SendByteTillDone(I2C, slave_address|1);
		if(result == 0)
		{
			return RT_ERROR;
		}
    /* while there is data to be read */
    while(data_byte-1)
    {
      /* read a byte from the EEPROM */
				result = I2C_ReceiveByte(I2C, p_buffer);
				if(result == 0)
				{
					return RT_ERROR;
				}
			
				/* point to the next location where the byte read will be saved */
				p_buffer++; 

				/* decrement the read bytes counter */
				data_byte--;
    }
		result = I2C_ReceivelastByte(I2C, p_buffer);
		if(result == 0)
		{
			return RT_ERROR;
		}
		I2C_Stop(I2C);
    return RT_EOK;
}

static int scm620_i2c_write(SGCC_I2C_TypeDef *I2C, uint16_t slave_address, uint8_t* p_buffer, uint16_t data_byte,uint8_t flags)
{
		uint8_t  result;
    /* send a start condition to I2C bus */
	
		rt_thread_delay(2);
		if((flags & RT_I2C_NO_START) != RT_I2C_NO_START)
		{
			result = I2C_Start(I2C);
			if(result == 0)
			{
				return RT_ERROR;
			}
			
			/* send slave address to I2C bus */
			result = I2C_SendByteTillDone(I2C, slave_address);
			if(result == 0)
			{
				return RT_ERROR;
			}
		}
    /* while there is data to be read */
    while(data_byte)
    {
        result = I2C_SendByteTillDone(I2C, *p_buffer);

				if(result == 0)
				{
					return RT_ERROR;
				}
        /* point to the next byte to be written */
        p_buffer++;

        /* decrement the write bytes counter */
        data_byte --;
    }

    /* send a stop condition to I2C bus */
		if((flags & RT_I2C_NO_STOP) == 0)
		{
			I2C_Stop(I2C);
		}
    return 0;
}

static rt_size_t i2cmxfer(struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[], rt_uint32_t num)
{
		struct rt_i2c_msg *msg;
		rt_uint32_t i;
    rt_err_t ret = RT_ERROR;
	
    RT_ASSERT(bus != RT_NULL);
    RT_ASSERT(bus->parent.user_data != RT_NULL);
		struct scm620_i2c *hi2c = (struct scm620_i2c *)bus->priv;	

		for (i = 0; i < num; i++)
    {
        msg = &msgs[i];

        if (msg->flags & RT_I2C_ADDR_10BIT)
        {
            scm620_i2c_init(hi2c->Instance,I2C_ADR_ADSE_10BIT);
        }
        else
        {
						scm620_i2c_init(hi2c->Instance,I2C_ADR_ADSE_7BIT);
        }
        if (msg->flags & RT_I2C_RD)
        {
            if (scm620_i2c_read(hi2c->Instance, msg->addr, msg->buf, msg->len) != 0)
            {
                return ret;
            }
        }
        else
        {
            if (scm620_i2c_write(hi2c->Instance, msg->addr, msg->buf, msg->len, msg->flags) != 0)
            {
                return ret;
            }
        }
    }
    
    ret = i;

    return ret;
}
//static rt_size_t i2csxfer(struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[], rt_uint32_t num)
//{

//}
//static rt_err_t i2ccontrol(struct rt_i2c_bus_device *bus,rt_uint32_t a,rt_uint32_t b)
//{

//}

static const struct rt_i2c_bus_device_ops scm620_i2c_ops =
{
		.master_xfer = i2cmxfer,
//    .slave_xfer = i2csxfer,
//		.i2c_bus_control = i2ccontrol,
};

struct rt_i2c_bus_device _i2c_bus0, _i2c_bus1, _i2c_bus2;
struct scm620_i2c _i2c0, _i2c1, _i2c2, _i2c3;
int scm620_i2c_register_bus(SGCC_I2C_TypeDef *I2C, const char *name)
{
    struct rt_i2c_bus_device *i2c_bus;
	  struct scm620_i2c *i2c;
	
    if (I2C == SGCC_I2C0_P)
    {
        i2c_bus = &_i2c_bus0;
				i2c = &_i2c0;
    }
    else if (I2C == SGCC_I2C1_P)
    {
        i2c_bus = &_i2c_bus1;
				i2c = &_i2c0;
		}
		else if (I2C == SGCC_I2C2_P)
    {
        i2c_bus = &_i2c_bus2;
				i2c = &_i2c0;
		}
    else
    {
        return -1;
    }
		i2c->Instance = I2C;
		i2c_bus->priv = i2c;
		i2c_bus->ops = &scm620_i2c_ops;
    return rt_i2c_bus_device_register(i2c_bus, name);
}

int rt_hw_i2c_init(void)
{
		int result = 0;
		#ifdef RT_USING_I2C0
			I2C0_InitPad(I2C0_PIN136SCL,I2C0_PIN137SDA);

			result = scm620_i2c_register_bus(SGCC_I2C0_P, "i2c0");
		#endif
		#ifdef RT_USING_I2C1
			I2C1_InitPad();

			result = scm620_i2c_register_bus(SGCC_I2C1_P, "i2c1");
		#endif
    #ifdef RT_USING_I2C2
			I2C2_InitPad();

			result = scm620_i2c_register_bus(SGCC_I2C2_P, "i2c2");
		#endif

    return result;
}
INIT_BOARD_EXPORT(rt_hw_i2c_init);
/* end of i2c driver */

#endif /* RT_USING_I2C */
