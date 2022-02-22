/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2017-10-25     ZYH               first implementation
 * 2018-10-23     XXXXzzzz000       first implementation,referance:scmf4xx-HAL/drv_rtc.c
 */
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

#include <string.h>
#include <time.h>

#ifdef RT_USING_RTC

static rt_err_t scm_rtc_control(struct rt_device *dev,
                                  int cmd,
                                  void *args)
{
		DateTimeTypeDef dateTimeW;
    struct tm *tm_now;
    struct tm now;
	
		uint32_t year;
		uint32_t month;
		uint32_t day;
		uint32_t hour;
		uint32_t minute;
		uint32_t second;
	
    switch (cmd)
    {
    case RT_DEVICE_CTRL_RTC_GET_TIME:
				RTC_GetTime(&dateTimeW);																									
				year    = dateTimeW.b.yearD * 10 + dateTimeW.b.yearU;
        month   = dateTimeW.b.monthD * 10 + dateTimeW.b.monthU;
        day     = dateTimeW.b.dayD * 10 + dateTimeW.b.dayU;
        hour    = dateTimeW.b.hourD * 10 + dateTimeW.b.hourU;    
        minute  = dateTimeW.b.minuteD * 10 + dateTimeW.b.minuteU;
        second  = dateTimeW.b.secondD * 10 + dateTimeW.b.secondU;
        now.tm_hour = hour;
        now.tm_min = minute;
        now.tm_sec = second;
        now.tm_year = year + 100;
        now.tm_mon = month - 1;
        now.tm_mday = day;
        *((time_t *)args) = mktime(&now);
        break;
    case RT_DEVICE_CTRL_RTC_SET_TIME:
			  
        rt_enter_critical();
        /* converts calendar time time into local time. */
        tm_now = localtime((const time_t *)args);
        /* copy the statically located variable */
        memcpy(&now, tm_now, sizeof(struct tm));
        /* unlock scheduler. */
        rt_exit_critical();
		
				//设置当前日期和时间
				dateTimeW.b.yearD = (now.tm_year - 100)/10; 
				dateTimeW.b.yearU = (now.tm_year - 100)%10;  
				dateTimeW.b.monthD = (now.tm_mon + 1)/10; 
				dateTimeW.b.monthU = (now.tm_mon + 1)%10; 
				dateTimeW.b.dayD = now.tm_mday/10; 
				dateTimeW.b.dayU = now.tm_mday%10;
				dateTimeW.b.weekDay = 6; 
				dateTimeW.b.hourD = now.tm_hour/10;  
				dateTimeW.b.hourU = now.tm_hour%10; 
				dateTimeW.b.minuteD = now.tm_min/10;  
				dateTimeW.b.minuteU = now.tm_min%10; 
				dateTimeW.b.secondD = now.tm_sec/10;  
				dateTimeW.b.secondU = now.tm_sec%10;   
				RTC_SetTime(dateTimeW); 																										//设定时间
        break;
    }
    return RT_EOK;
}

static rt_err_t scm_rtc_init(struct rt_device *dev)
{
    return RT_EOK;
}

static rt_err_t scm_rtc_open(struct rt_device *dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

static rt_err_t scm_rtc_close(struct rt_device *dev)
{
    return RT_EOK;
}

static rt_size_t scm_rtc_read(struct rt_device *dev,
                                rt_off_t pos,
                                void *buffer,
                                rt_size_t size)
{
    scm_rtc_control(dev, RT_DEVICE_CTRL_RTC_GET_TIME, buffer);
    return size;
}

static rt_size_t scm_rtc_write(struct rt_device *dev,
                                 rt_off_t pos,
                                 const void *buffer,
                                 rt_size_t size)
{
    scm_rtc_control(dev, RT_DEVICE_CTRL_RTC_SET_TIME, (void *)buffer);
    return size;
}

struct rt_device rtc_device;
int rt_hw_rtc_init(void)
{
    rtc_device.type = RT_Device_Class_RTC;
    rtc_device.rx_indicate = RT_NULL;
    rtc_device.tx_complete = RT_NULL;
    rtc_device.init = scm_rtc_init;
    rtc_device.open = scm_rtc_open;
    rtc_device.close = scm_rtc_close;
    rtc_device.read = scm_rtc_read;
    rtc_device.write = scm_rtc_write;
    rtc_device.control = scm_rtc_control;
    rtc_device.user_data = RT_NULL;
    /* register a character device */
    return rt_device_register(&rtc_device, "rtc", RT_DEVICE_FLAG_RDWR);
}
INIT_BOARD_EXPORT(rt_hw_rtc_init);

static void rtc_set(uint8_t argc, char **argv)
{
		time_t now;     /* 保存获取的当前时间值 */
	
		set_date(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
		set_time(atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
		/* 获取时间 */
		now = time(RT_NULL);
		/* 打印输出时间信息 */
		rt_kprintf("%s\n", ctime(&now));
}
MSH_CMD_EXPORT(rtc_set, rtc_set yyyy mm dd hh mm ss);

static void rtc_get(void)
{
		time_t now;     /* 保存获取的当前时间值 */
		/* 获取时间 */
		now = time(RT_NULL);
		/* 打印输出时间信息 */
		rt_kprintf("%s\n", ctime(&now));
}
MSH_CMD_EXPORT(rtc_get, rtc get);
#endif /* RT_USING_RTC */

