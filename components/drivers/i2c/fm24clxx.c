/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-14     aubr.cool    1st version
 */
#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/i2c.h>
#include <drivers/i2c_dev.h>
#include <board.h>

#ifdef RT_USING_FM24CLXX

struct fm24clxx_device
{
    struct rt_device         parent;
    struct rt_i2c_bus_device *bus;
};

/* RT-Thread device interface */

static rt_err_t fm24clxx_init(rt_device_t dev)
{
    return RT_EOK;
}
static rt_err_t fm24clxx_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t fm24clxx_close(rt_device_t dev)
{
    return RT_EOK;
}

static rt_err_t fm24clxx_control(rt_device_t dev, int cmd, void *args)
{
    return RT_EOK;
}

static rt_size_t fm24clxx_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    struct fm24clxx_device *fm24clxx;
    const struct fm24clxx_config *cfg;
    struct rt_i2c_msg msg[2];
    rt_uint8_t mem_addr[2] = {0,};
    rt_size_t ret = 0;
    RT_ASSERT(dev != 0);

    fm24clxx = (struct fm24clxx_device *) dev;

    RT_ASSERT(fm24clxx->parent.user_data != 0);
    cfg = (const struct fm24clxx_config *) fm24clxx->parent.user_data;

    if(pos > cfg->size)
    {
         return 0;
    }

    if(pos + size > cfg->size)
    {
         size = cfg->size - pos;
    }

    msg[0].addr     = cfg->addr;
    msg[0].flags    = cfg->flags | RT_I2C_WR;
    mem_addr[0]     = (rt_uint8_t) pos;
    mem_addr[1]     = (pos >> 8);
    msg[0].buf      = (rt_uint8_t *) mem_addr;
    msg[0].len      =  1;

    msg[1].addr     = cfg->addr;
    msg[1].flags    = cfg->flags | RT_I2C_RD;
    msg[1].buf      = (rt_uint8_t *) buffer;
    msg[1].len      = size;

    ret = rt_i2c_transfer(fm24clxx->bus, msg, 2);
    return (ret == 2) ? size : 0;
}

static rt_size_t fm24clxx_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
    struct fm24clxx_device *fm24clxx;
    const struct fm24clxx_config *cfg;
    struct rt_i2c_msg msg[2];
    rt_uint8_t mem_addr[2] = {0,};
    rt_size_t ret = 0;
    RT_ASSERT(dev != 0);

    fm24clxx = (struct fm24clxx_device *) dev;

    RT_ASSERT(fm24clxx->parent.user_data != 0);
    cfg = (const struct fm24clxx_config *) fm24clxx->parent.user_data;

    if(pos > cfg->size)
    {
         return 0;
    }

    if(pos + size > cfg->size)
    {
         size = cfg->size - pos;
    }

    msg[0].addr     = cfg->addr;
    msg[0].flags    = cfg->flags | RT_I2C_WR | RT_I2C_NO_STOP;
    mem_addr[0]     = (rt_uint8_t) pos;
    mem_addr[1]     = (pos >> 8);
    msg[0].buf      = (rt_uint8_t *) mem_addr;
    msg[0].len      =  1;

    msg[1].addr     = cfg->addr;
    msg[1].flags    = cfg->flags | RT_I2C_WR | RT_I2C_NO_START;
    msg[1].buf      = (rt_uint8_t *) buffer;
    msg[1].len      = size;

    ret = rt_i2c_transfer(fm24clxx->bus, msg, 2);
    return (ret == 2) ? size : 0;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device fm24clxx_ops =
{
    fm24clxx_init,
    fm24clxx_open,
    fm24clxx_close,
    fm24clxx_read,
    fm24clxx_write,
    fm24clxx_control
};
#endif

rt_err_t fm24clxx_register(const char *fm_device_name, const char *i2c_bus, void *user_data)
{
    static struct fm24clxx_device fm24clxx_drv;
    struct rt_i2c_bus_device *bus;

    bus = rt_i2c_bus_device_find(i2c_bus);
    if (bus == RT_NULL)
    {
        return RT_ENOSYS;
    }

    fm24clxx_drv.bus = bus;
    fm24clxx_drv.parent.type      = RT_Device_Class_Block;
#ifdef RT_USING_DEVICE_OPS
    fm24clxx_drv.parent.ops       = &fm24clxx_ops;
#else
    fm24clxx_drv.parent.init      = fm24clxx_init;
    fm24clxx_drv.parent.open      = fm24clxx_open;
    fm24clxx_drv.parent.close     = fm24clxx_close;
    fm24clxx_drv.parent.read      = fm24clxx_read;
    fm24clxx_drv.parent.write     = fm24clxx_write;
    fm24clxx_drv.parent.control   = fm24clxx_control;
#endif

    fm24clxx_drv.parent.user_data = user_data;

    return rt_device_register(&fm24clxx_drv.parent, fm_device_name, RT_DEVICE_FLAG_RDWR);
}

void fm24c08_test(void)
{
		static rt_device_t fm24c08_device = RT_NULL;
	
		u8 Send_Data[10] = {10,11,12,13,14,15,16,17,18,19};
		u8 Rcev_Data[10] = {0};
		
		fm24c08_device = rt_device_find("fm24c08");
	  rt_device_open(fm24c08_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM);
		
		rt_device_write(fm24c08_device,0,Send_Data,10);
		rt_device_read(fm24c08_device,0,Rcev_Data,10);
		rt_thread_delay(100);
		rt_kprintf("fm24c08 write data:10,11,12,13,14,15,16,17,18,19\n");
		rt_kprintf("fm24c08 read  data:%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",Rcev_Data[0],Rcev_Data[1],Rcev_Data[2],Rcev_Data[3],Rcev_Data[4],Rcev_Data[5],Rcev_Data[6],Rcev_Data[7],Rcev_Data[8],Rcev_Data[9]);
    
}
MSH_CMD_EXPORT(fm24c08_test, test fm24c08 write and read);

#endif


