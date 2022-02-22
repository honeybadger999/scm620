/*
 * Copyright (c) 2019 Winner Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-19     fanwenl      1st version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <drivers/rt_drv_pwm.h>
#include <board.h>

#ifdef RT_USING_PWM

struct scm_pwm_Type
{
		SGCC_PWM_TypeDef *Instance;
};

static rt_err_t drv_pwm_set(SGCC_PWM_TypeDef* pwmx,struct rt_pwm_configuration *configuration)
{
	u16 div;

	if(pwmx == SGCC_PWM00_P)
	{
		if(configuration->channel == 0)
		{
			PWM00_InitPad_42();
		}
		else if(configuration->channel == 1)
		{	
			PWM00_InitPad_96();
		}
		else if(configuration->channel == 2)
		{	
			PWM00_InitPad_100();
		}
		else
		{
			return RT_ERROR;
		}
	}
	else if(pwmx == SGCC_PWM01_P)
	{
		if(configuration->channel == 0)
		{
			PWM01_InitPad_43();
		}
		else if(configuration->channel == 1)
		{	
			PWM01_InitPad_97();
		}
		else if(configuration->channel == 2)
		{	
			PWM01_InitPad_101();
		}
		else if(configuration->channel == 3)
		{	
			PWM01_InitPad_140();
		}
		else
		{
			return RT_ERROR;
		}
	}
	else if(pwmx == SGCC_PWM02_P)
	{
		if(configuration->channel == 0)
		{
			PWM02_InitPad_40();
		}
		else if(configuration->channel == 1)
		{	
			PWM02_InitPad_46();
		}
		else if(configuration->channel == 2)
		{	
			PWM02_InitPad_98();
		}
		else if(configuration->channel == 3)
		{	
			PWM02_InitPad_102();
		}
		else
		{
			return RT_ERROR;
		}
	}
	else if(pwmx == SGCC_PWM10_P)
	{
		if(configuration->channel == 0)
		{
			PWM10_InitPad_35();
		}
		else if(configuration->channel == 1)
		{	
			PWM10_InitPad_103();
		}
		else if(configuration->channel == 2)
		{	
			PWM10_InitPad_111();
		}
		else if(configuration->channel == 2)
		{	
			PWM10_InitPad_136();
		}
		else
		{
			return RT_ERROR;
		}
	}
	else if(pwmx == SGCC_PWM11_P)
	{
		if(configuration->channel == 0)
		{
			PWM11_InitPad_36();
		}
		else if(configuration->channel == 1)
		{	
			PWM11_InitPad_47();
		}
		else if(configuration->channel == 2)
		{	
			PWM11_InitPad_99();
		}
		else if(configuration->channel == 3)
		{	
			PWM11_InitPad_104();
		}
		else if(configuration->channel == 3)
		{	
			PWM11_InitPad_112();
		}
		else if(configuration->channel == 4)
		{	
			PWM11_InitPad_137();
		}
		else
		{
			return RT_ERROR;
		}
	}
	else if(pwmx == SGCC_PWM12_P)
	{
		if(configuration->channel == 0)
		{
			PWM12_InitPad_34();
		}
		else if(configuration->channel == 1)
		{	
			PWM12_InitPad_37();
		}
		else if(configuration->channel == 2)
		{	
			PWM12_InitPad_110();
		}
		else if(configuration->channel == 3)
		{	
			PWM12_InitPad_113();
		}
		else if(configuration->channel == 3)
		{	
			PWM12_InitPad_139();
		}
		else
		{
			return RT_ERROR;
		}
	}
	div = 250-1;

	PWM_Start(pwmx, 18750000 / div / (1.0/(configuration->period/1000000000.0)) - 1,18750000 / div /(1.0/(configuration->pulse/1000000000.0)) - 1,  div,  0) ; //Ê¹ÄÜPWMÊä³ö
	
	return RT_EOK;
}

static rt_err_t drv_pwm_control(struct rt_device_pwm *device, int cmd, void *arg)
{
    struct rt_pwm_configuration *configuration = (struct rt_pwm_configuration *)arg;
		struct scm_pwm_Type *scm_pwm = (struct scm_pwm_Type *)device->parent.user_data;
	
    switch (cmd)
    {
    case PWM_CMD_ENABLE:
				scm_pwm->Instance->CONTROL |= 0x5;
        return RT_EOK;
    case PWM_CMD_DISABLE:
				scm_pwm->Instance->CONTROL &= ~0x5;
        return RT_EOK;
    case PWM_CMD_SET:
        return drv_pwm_set(scm_pwm->Instance, configuration);
    case PWM_CMD_GET:
        return RT_EOK;
    default:
        return RT_EINVAL;
    }
}

static struct rt_pwm_ops drv_ops =
{
  drv_pwm_control
};

#ifdef RT_USING_PWM00
static struct rt_device_pwm _pwm00;
static struct scm_pwm_Type scm_pwm00;
#endif
#ifdef RT_USING_PWM01
static struct rt_device_pwm _pwm01;
static struct scm_pwm_Type scm_pwm01;
#endif
#ifdef RT_USING_PWM02
static struct rt_device_pwm _pwm02;
static struct scm_pwm_Type scm_pwm02;
#endif
#ifdef RT_USING_PWM10
static struct rt_device_pwm _pwm10;
static struct scm_pwm_Type scm_pwm10;
#endif
#ifdef RT_USING_PWM11
static struct rt_device_pwm _pwm11;
static struct scm_pwm_Type scm_pwm11;
#endif
#ifdef RT_USING_PWM12
static struct rt_device_pwm _pwm12;
static struct scm_pwm_Type scm_pwm12;
#endif
int scm620_hw_pwm_init(void)
{
	
#ifdef RT_USING_PWM00
		
    scm_pwm00.Instance = SGCC_PWM00_P;
    rt_device_pwm_register(&_pwm00, "pwm00",&drv_ops, &scm_pwm00);
#endif	
#ifdef RT_USING_PWM01
		
    scm_pwm01.Instance = SGCC_PWM01_P;
    rt_device_pwm_register(&_pwm01, "pwm01",&drv_ops, &scm_pwm01);
#endif	
#ifdef RT_USING_PWM02
		
    scm_pwm02.Instance = SGCC_PWM02_P;
    rt_device_pwm_register(&_pwm02, "pwm02",&drv_ops, &scm_pwm02);
#endif	
#ifdef RT_USING_PWM10
		
    scm_pwm10.Instance = SGCC_PWM10_P;
    rt_device_pwm_register(&_pwm10, "pwm10",&drv_ops, &scm_pwm10);
#endif	
#ifdef RT_USING_PWM11
		
    scm_pwm11.Instance = SGCC_PWM11_P;
    rt_device_pwm_register(&_pwm11, "pwm11",&drv_ops, &scm_pwm11);
#endif	
#ifdef RT_USING_PWM12
		
    scm_pwm12.Instance = SGCC_PWM12_P;
    rt_device_pwm_register(&_pwm12, "pwm12",&drv_ops, &scm_pwm12);
#endif	
    return 0;
}

INIT_BOARD_EXPORT(scm620_hw_pwm_init);

#endif
