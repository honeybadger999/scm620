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
#include <drivers/hwtimer.h>
#include <board.h>

#ifdef RT_USING_HWTIMER

struct scm_timer_Type
{
		SGCC_TC_TypeDef *Instance;
};

TC_16BitTimerCFG tcCfg;

static void scm_timer_init(rt_hwtimer_t *timer, rt_uint32_t state)
{
		struct scm_timer_Type *scm_timer = (struct scm_timer_Type *)timer->parent.user_data;
	
		if(scm_timer->Instance == SGCC_TIMERA0_P)
		{
			NVIC_EnableIRQ(TIMERA0_IRQn);    
		}
		else if(scm_timer->Instance == SGCC_TIMERA1_P)
		{
			NVIC_EnableIRQ(TIMERA1_IRQn);    
		}
		else if(scm_timer->Instance == SGCC_TIMERB0_P)
		{
			NVIC_EnableIRQ(TIMERB0_IRQn);    
		}
		else if(scm_timer->Instance == SGCC_TIMERB1_P)
		{
			NVIC_EnableIRQ(TIMERB1_IRQn);    
		}
		else if(scm_timer->Instance == SGCC_TIMERC0_P)
		{
			NVIC_EnableIRQ(TIMERC0_IRQn);    
		}
		else if(scm_timer->Instance == SGCC_TIMERC1_P)
		{
			NVIC_EnableIRQ(TIMERC1_IRQn);    
		}
		else if(scm_timer->Instance == SGCC_TIMERD0_P)
		{
			NVIC_EnableIRQ(TIMERD0_IRQn);    
		}
		else if(scm_timer->Instance == SGCC_TIMERD1_P)
		{
			NVIC_EnableIRQ(TIMERD1_IRQn);    
		}
}

static rt_err_t scm_timer_start(rt_hwtimer_t *timer, rt_uint32_t t, rt_hwtimer_mode_t opmode)
{
		struct scm_timer_Type *scm_timer = (struct scm_timer_Type *)timer->parent.user_data;
		uint8_t matchEn;
	
		matchEn = 1;    
		if(matchEn)
		{
				tcCfg.matchVal = t - 1;  
		}
	
		TC_Timer16Bit(scm_timer->Instance, matchEn, tcCfg, 1) ;
	
   	scm_timer->Instance->CR = 0x2;
   	scm_timer->Instance->CR = 0x1;
		
    return RT_EOK;
}

static void scm_timer_stop(rt_hwtimer_t *timer)
{
    struct scm_timer_Type *scm_timer = (struct scm_timer_Type *)timer->parent.user_data;
		
		scm_timer->Instance->CR = 0;
}

static rt_err_t scm_timer_ctrl(rt_hwtimer_t *timer, rt_uint32_t cmd, void *arg)
{
		
    struct scm_timer_Type *scm_timer = (struct scm_timer_Type *)timer->parent.user_data;
		
    /* The frequency value is an immutable value. */
    if (cmd == HWTIMER_CTRL_FREQ_SET)
    {
			
				TC_RestoreReg(scm_timer->Instance);
		
				//PCLK 25000000               
				
				tcCfg.outMode = 0;                         // PIN输出翻转
				tcCfg.outPulseW = 0;                       // PIN翻转，所以脉冲宽度没有用，此处设置为0
				tcCfg.prescale = g_SystemPclk/(*(rt_uint32_t*)arg) - 1;                     // PC计数到预分频值之后，在下一个周期TC加1，PC清零，故这里是100个PCLK会使TC加1;
																									 // 当前PCLK是32MHz，使用的是其二分频，故一个TC增加的周期是1/(16MHz/100) s
				TC_ClearIntr(scm_timer->Instance, TC_INTR_MR);
				TC_ClearIntr(scm_timer->Instance, TC_INTR_TCOV);
				TC_RestoreReg(scm_timer->Instance); 

        return RT_EOK;
    }
		if (cmd == HWTIMER_CTRL_MODE_SET)
    {
			if((*(rt_uint32_t*)arg) == HWTIMER_MODE_PERIOD)
			{
				tcCfg.matchCtl = TC_MCR_CLEARTC;
			}
			else
			{
				tcCfg.matchCtl = TC_MCR_NOTCLEARTC;
			}
		}
		return -RT_ENOSYS;
}

static const struct rt_hwtimer_info _info =
{
    1000000,            /* the maximum count frequency can be set */
          1,            /* the minimum count frequency can be set */
    0xFFFFFFFF,         /* the maximum counter value */
    HWTIMER_CNTMODE_DW, /* Increment or Decreasing count mode */
};

static const struct rt_hwtimer_ops _ops =
{
    scm_timer_init,
    scm_timer_start,
    scm_timer_stop,
    RT_NULL,
    scm_timer_ctrl,
};

#ifdef RT_USING_HWTIMERA0
static rt_hwtimer_t _timera0;
static struct scm_timer_Type scm_timera0;
#endif
#ifdef RT_USING_HWTIMERA1
static rt_hwtimer_t _timera1;
static struct scm_timer_Type scm_timera1;
#endif
#ifdef RT_USING_HWTIMERB0
static rt_hwtimer_t _timerb0;
static struct scm_timer_Type scm_timerb0;
#endif
#ifdef RT_USING_HWTIMERB1
static rt_hwtimer_t _timerb1;
static struct scm_timer_Type scm_timerb1;
#endif
#ifdef RT_USING_HWTIMERC0
static rt_hwtimer_t _timerc0;
static struct scm_timer_Type scm_timerc0;
#endif
#ifdef RT_USING_HWTIMERC1
static rt_hwtimer_t _timerc1;
static struct scm_timer_Type scm_timerc1;
#endif
#ifdef RT_USING_HWTIMERD0
static rt_hwtimer_t _timerd0;
static struct scm_timer_Type scm_timerd0;
#endif
#ifdef RT_USING_HWTIMERD1
static rt_hwtimer_t _timerd1;
static struct scm_timer_Type scm_timerd1;
#endif


int scm620_hw_timer_init(void)
{
	
#ifdef RT_USING_HWTIMERA0
    scm_timera0.Instance = SGCC_TIMERA0_P;

    _timera0.info = &_info;
    _timera0.ops = &_ops;

    rt_device_hwtimer_register(&_timera0, "timera0", &scm_timera0);
#endif	
#ifdef RT_USING_HWTIMERA1
    scm_timera1.Instance = SGCC_TIMERA1_P;

    _timera1.info = &_info;
    _timera1.ops = &_ops;

    rt_device_hwtimer_register(&_timera1, "timera1", &scm_timera1);
#endif	
#ifdef RT_USING_HWTIMERB0
    scm_timerb0.Instance = SGCC_TIMERB0_P;

    _timerb0.info = &_info;
    _timerb0.ops = &_ops;

    rt_device_hwtimer_register(&_timerb0, "timerb0", &scm_timerb0);
#endif	
#ifdef RT_USING_HWTIMERB1
    scm_timerb1.Instance = SGCC_TIMERB1_P;

    _timerb1.info = &_info;
    _timerb1.ops = &_ops;

    rt_device_hwtimer_register(&_timerb1, "timerb1", &scm_timerb1);
#endif	
#ifdef RT_USING_HWTIMERC0
    scm_timerc0.Instance = SGCC_TIMERC0_P;

    _timerc0.info = &_info;
    _timerc0.ops = &_ops;

    rt_device_hwtimer_register(&_timerc0, "timerc0", &scm_timerc0);
#endif	
#ifdef RT_USING_HWTIMERC1
    scm_timerc1.Instance = SGCC_TIMERC1_P;

    _timerc1.info = &_info;
    _timerc1.ops = &_ops;

    rt_device_hwtimer_register(&_timerc1, "timerc1", &scm_timerc1);
#endif	
#ifdef RT_USING_HWTIMERD0
    scm_timerd0.Instance = SGCC_TIMERD0_P;

    _timerd0.info = &_info;
    _timerd0.ops = &_ops;

    rt_device_hwtimer_register(&_timerd0, "timerd0", &scm_timerd0);
#endif	
#ifdef RT_USING_HWTIMERD1
    scm_timerd1.Instance = SGCC_TIMERD1_P;

    _timerd1.info = &_info;
    _timerd1.ops = &_ops;

    rt_device_hwtimer_register(&_timerd1, "timerd1", &scm_timerd1);
#endif	
    return 0;
}
INIT_BOARD_EXPORT(scm620_hw_timer_init);

void TIMERA0_IRQHandler(void)
{
		/* enter interrupt */
    rt_interrupt_enter();
#ifdef RT_USING_HWTIMERA0
    TC_ClearIntr(SGCC_TIMERA0_P, 0X0F);
    rt_device_hwtimer_isr(&_timera0);
#endif
		/* leave interrupt */
    rt_interrupt_leave();
}
void TIMERA1_IRQHandler(void)
{
		/* enter interrupt */
    rt_interrupt_enter();
#ifdef RT_USING_HWTIMERA1
    TC_ClearIntr(SGCC_TIMERA1_P, 0X0F);
    rt_device_hwtimer_isr(&_timera1);
#endif
		/* leave interrupt */
    rt_interrupt_leave();
}void TIMERB0_IRQHandler(void)
{
		/* enter interrupt */
    rt_interrupt_enter();
#ifdef RT_USING_HWTIMERB0
    TC_ClearIntr(SGCC_TIMERB0_P, 0X0F);
    rt_device_hwtimer_isr(&_timerb0);
#endif
		/* leave interrupt */
    rt_interrupt_leave();
}void TIMERB1_IRQHandler(void)
{
		/* enter interrupt */
    rt_interrupt_enter();
#ifdef RT_USING_HWTIMERB1
    TC_ClearIntr(SGCC_TIMERB1_P, 0X0F);
    rt_device_hwtimer_isr(&_timerb1);
#endif
		/* leave interrupt */
    rt_interrupt_leave();
}void TIMERC0_IRQHandler(void)
{
		/* enter interrupt */
    rt_interrupt_enter();
#ifdef RT_USING_HWTIMERC0
    TC_ClearIntr(SGCC_TIMERC0_P, 0X0F);
    rt_device_hwtimer_isr(&_timerc0);
#endif
		/* leave interrupt */
    rt_interrupt_leave();
}void TIMERC1_IRQHandler(void)
{
		/* enter interrupt */
    rt_interrupt_enter();
#ifdef RT_USING_HWTIMERC1
    TC_ClearIntr(SGCC_TIMERC1_P, 0X0F);
    rt_device_hwtimer_isr(&_timerc1);
#endif
		/* leave interrupt */
    rt_interrupt_leave();
}void TIMERD0_IRQHandler(void)
{
		/* enter interrupt */
    rt_interrupt_enter();
#ifdef RT_USING_HWTIMERD0
    TC_ClearIntr(SGCC_TIMERD0_P, 0X0F);
    rt_device_hwtimer_isr(&_timerd0);
#endif
		/* leave interrupt */
    rt_interrupt_leave();
}void TIMERD1_IRQHandler(void)
{
		/* enter interrupt */
    rt_interrupt_enter();
#ifdef RT_USING_HWTIMERD1
    TC_ClearIntr(SGCC_TIMERD1_P, 0X0F);
    rt_device_hwtimer_isr(&_timerd1);
#endif
		/* leave interrupt */
    rt_interrupt_leave();
}

/* 定时器超时回调函数 */
static rt_err_t timeout_cb(rt_device_t dev, rt_size_t size)
{
    rt_kprintf("this is hwtimer timeout callback fucntion!\n");
    rt_kprintf("tick is :%d !\n", rt_tick_get());
    return 0;
}

void hwtimer_test(void)
{
		static rt_device_t timer_device = RT_NULL;
		rt_hwtimerval_t timeout_s;      /* 定时器超时值 */
		rt_hwtimer_mode_t mode;         /* 定时器模式 */
		rt_uint32_t freq = 1000;        /* 计数频率 */
	
		timer_device = rt_device_find("timera0");
	  rt_device_open(timer_device, RT_DEVICE_OFLAG_RDWR);
		rt_device_set_rx_indicate(timer_device, timeout_cb);
	
		/* 设置计数频率(默认1Mhz或支持的最小计数频率) */
    rt_device_control(timer_device, HWTIMER_CTRL_FREQ_SET, &freq);
    /* 设置模式为非周期性定时器 */
    mode = HWTIMER_MODE_ONESHOT;
    rt_device_control(timer_device, HWTIMER_CTRL_MODE_SET, &mode);
	
		/* 设置定时器超时值为5s并启动定时器 */
    timeout_s.sec = 5;      /* 秒 */
    timeout_s.usec = 0;     /* 微秒 */
    rt_device_write(timer_device, 0, &timeout_s, sizeof(timeout_s));
		rt_kprintf("start hwtimer fucntion!\n");
    rt_kprintf("tick is :%d !\n", rt_tick_get());
}
MSH_CMD_EXPORT(hwtimer_test, test hwtimer timeout callback);

#endif


