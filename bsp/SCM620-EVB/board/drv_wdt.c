#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


#ifdef RT_USING_WDT

static rt_err_t drv_init(rt_watchdog_t *wdt)
{
		WDT_Start(SYSWDT_CTRL_OTSET_4s, DISABLE, ENABLE, DISABLE);			
    return RT_EOK;
}

static rt_err_t drv_control(rt_watchdog_t *wdt, int cmd, void *arg)
{
    switch (cmd)
    {
    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
        WDT_Start((rt_uint32_t)arg, DISABLE, ENABLE, DISABLE);		
        break;
    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
        WDT_Restart();
        break;
    default:
        return RT_ERROR;
    }
    return RT_EOK;
}

static struct rt_watchdog_ops _ops =
    {
        drv_init,
        drv_control
    };

static rt_watchdog_t _wdt =
    {
        .ops = &_ops
    };

int rt_wdt_init(void)
{
    return rt_hw_watchdog_register(&_wdt, "wdt", RT_DEVICE_FLAG_DEACTIVATE, RT_NULL);
}
INIT_BOARD_EXPORT(rt_wdt_init);
#endif
