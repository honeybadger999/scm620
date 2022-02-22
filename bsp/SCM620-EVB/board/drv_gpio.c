/*
 * File      : gpio.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2017, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-09-16     Haley        the first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <rthw.h>

#include <board.h>

#ifdef RT_USING_PIN

#define SCM620_PIN_NUMBERS    144
struct rt_pin_irq_hdr scm620_pin_irq_hdr_tab[144];

void scm620_pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    if (mode == PIN_MODE_OUTPUT)
    {
        /* output setting */
        GPIO_InitPad(pin);
        GPIO_ConfigDir(pin, GPIO_OUTPUT);
    }
    else if (mode == PIN_MODE_INPUT)
    {
        /* input setting: not pull. */
        GPIO_InitPad(pin);
        GPIO_ConfigDir(pin, GPIO_INPUT);
        IO_ConfigPullUp(pin, DISABLE);
        IO_ConfigPullDown(pin, DISABLE);
    }
    else if (mode == PIN_MODE_INPUT_PULLUP)
    {
        /* input setting: pull up. */
        GPIO_ConfigDir(pin, GPIO_INPUT);
        IO_ConfigPullDown(pin, DISABLE);
        IO_ConfigPullUp(pin, ENABLE);
    }
    else if (mode == PIN_MODE_INPUT_PULLDOWN)
    {
        /* input setting: pull down. */
        GPIO_ConfigDir(pin, GPIO_INPUT);
        IO_ConfigPullUp(pin, DISABLE);
        IO_ConfigPullDown(pin, ENABLE);
    }
    else
    {
        /* input setting:default. */
        GPIO_ConfigDir(pin, GPIO_INPUT);
    }
}

void scm620_pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    if (value == PIN_LOW)
    {
        GPIO_WrDataOut(pin, GPIO_LOW);
    }
    else if (value == PIN_HIGH)
    {
        GPIO_WrDataOut(pin, GPIO_HIGH);
    }
}

int scm620_pin_read(rt_device_t dev, rt_base_t pin)
{
    int value = PIN_LOW;

    value = GPIO_RdDataIn(pin);

    return value;
}

rt_err_t scm620_pin_attach_irq(struct rt_device *device, rt_int32_t pin,
                               rt_uint32_t mode, void (*hdr)(void *args), void *args)
{
    rt_base_t level;
    rt_int32_t irqindex = -1;

    irqindex = pin;

    level = rt_hw_interrupt_disable();

    if(scm620_pin_irq_hdr_tab[irqindex].pin == pin   &&
            scm620_pin_irq_hdr_tab[irqindex].hdr == hdr   &&
            scm620_pin_irq_hdr_tab[irqindex].mode == mode &&
            scm620_pin_irq_hdr_tab[irqindex].args == args
      )
    {
        rt_hw_interrupt_enable(level);
        return RT_EOK;
    }

    if(scm620_pin_irq_hdr_tab[irqindex].pin != -1)
    {
        rt_hw_interrupt_enable(level);
        return -RT_EBUSY;
    }

    scm620_pin_irq_hdr_tab[irqindex].pin = pin;
    scm620_pin_irq_hdr_tab[irqindex].hdr = hdr;
    scm620_pin_irq_hdr_tab[irqindex].mode = mode;
    scm620_pin_irq_hdr_tab[irqindex].args = args;
    rt_hw_interrupt_enable(level);

    return RT_EOK;
}

rt_err_t scm620_pin_dettach_irq(struct rt_device *device, rt_int32_t pin)
{
    rt_base_t level;
    rt_int32_t irqindex = -1;

    irqindex = pin;

    level = rt_hw_interrupt_disable();

    if(scm620_pin_irq_hdr_tab[irqindex].pin == -1)
    {
        rt_hw_interrupt_enable(level);
        return RT_EOK;
    }

    scm620_pin_irq_hdr_tab[irqindex].pin = -1;
    scm620_pin_irq_hdr_tab[irqindex].hdr = RT_NULL;
    scm620_pin_irq_hdr_tab[irqindex].mode = 0;
    scm620_pin_irq_hdr_tab[irqindex].args = RT_NULL;
    rt_hw_interrupt_enable(level);

    return RT_EOK;
}

rt_err_t scm620_pin_irq_enable(struct rt_device *device, rt_base_t pin, rt_uint32_t enabled)
{
    rt_base_t level;
    rt_int32_t irqindex = -1;

    irqindex = pin;

    if (enabled == PIN_IRQ_ENABLE)
    {
        level = rt_hw_interrupt_disable();

        /* Configure the GPIO/button interrupt polarity */
        if (scm620_pin_irq_hdr_tab[irqindex].mode == PIN_IRQ_MODE_RISING)
        {
            GPIO_ConfigIntrMode(scm620_pin_irq_hdr_tab[irqindex].pin, GPIO_INTR_RISINGEDGE);
        }
        else if (scm620_pin_irq_hdr_tab[irqindex].mode == PIN_IRQ_MODE_FALLING)
        {
            GPIO_ConfigIntrMode(scm620_pin_irq_hdr_tab[irqindex].pin, GPIO_INTR_FALLINGEDGE);
        }
        else if (scm620_pin_irq_hdr_tab[irqindex].mode == PIN_IRQ_MODE_HIGH_LEVEL)
        {
            GPIO_ConfigIntrMode(scm620_pin_irq_hdr_tab[irqindex].pin, GPIO_INTR_HIGHLVEL);
        }
        else if (scm620_pin_irq_hdr_tab[irqindex].mode == PIN_IRQ_MODE_LOW_LEVEL)
        {
            GPIO_ConfigIntrMode(scm620_pin_irq_hdr_tab[irqindex].pin, GPIO_INTR_LOWLEVEL);
        }
        else if (scm620_pin_irq_hdr_tab[irqindex].mode == PIN_IRQ_MODE_RISING_FALLING)
        {
            GPIO_ConfigIntrMode(scm620_pin_irq_hdr_tab[irqindex].pin, GPIO_INTR_BOTHEDGE);
        }

        /* Clear the GPIO Interrupt (write to clear) */
        GPIO_ClrIntr(scm620_pin_irq_hdr_tab[irqindex].pin);

        /* Enable the GPIO/button interrupt */
        GPIO_EnIntr(scm620_pin_irq_hdr_tab[irqindex].pin, ENABLE);

        GPIO_EnSyn(scm620_pin_irq_hdr_tab[irqindex].pin, ENABLE);
        GPIO_ConfigDebounce(scm620_pin_irq_hdr_tab[irqindex].pin, GPIO_DEBOUNCE_BOTHPULSE, GPIO_DEBOUNCE_CYCLE_4);

        NVIC_EnableIRQ(GPIO_IRQn);

        rt_hw_interrupt_enable(level);
    }
    else if (enabled == PIN_IRQ_DISABLE)
    {
        /* Disable the GPIO/button interrupt */
        GPIO_EnIntr(scm620_pin_irq_hdr_tab[irqindex].pin, DISABLE);
        NVIC_DisableIRQ(GPIO_IRQn);
    }
    else
    {
        return RT_ENOSYS;
    }

    return RT_EOK;
}

const static struct rt_pin_ops scm620_pin_ops =
{
    scm620_pin_mode,
    scm620_pin_write,
    scm620_pin_read,
    scm620_pin_attach_irq,
    scm620_pin_dettach_irq,
    scm620_pin_irq_enable,
};

int rt_hw_pin_init(void)
{
    unsigned char i = 0;

    for(i = 0; i < SCM620_PIN_NUMBERS; i++)
    {
        scm620_pin_irq_hdr_tab[i].pin = -1;
        scm620_pin_irq_hdr_tab[i].hdr = RT_NULL;
        scm620_pin_irq_hdr_tab[i].mode = 0;
        scm620_pin_irq_hdr_tab[i].args = RT_NULL;
    }

    rt_device_pin_register("pin", &scm620_pin_ops, RT_NULL);

    return 0;
}

INIT_BOARD_EXPORT(rt_hw_pin_init);

rt_inline void pin_irq_hdr(int irqno)
{
    GPIO_ClrIntr(irqno);

    if(scm620_pin_irq_hdr_tab[irqno].hdr)
    {
        scm620_pin_irq_hdr_tab[irqno].hdr(scm620_pin_irq_hdr_tab[irqno].args);
    }
}

void GPIO_IRQHandler(void)
{
    volatile unsigned char temp, i;
    volatile SGCC_GPIO_TypeDef * GPIO_P;

    /* enter interrupt */
    rt_interrupt_enter();

    GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO0_BASE);
    temp = GPIO_P->INTSR;

    for(i = 0; i < 32; i++)
    {
        if((temp & (1 << i)) != 0)
        {
            pin_irq_hdr(i);
        }
    }

    GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO1_BASE);
    temp = GPIO_P->INTSR;

    for(i = 0; i < 32; i++)
    {
        if((temp & (1 << i)) != 0)
        {
            pin_irq_hdr(i + 32);
        }
    }

    GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO2_BASE);
    temp = GPIO_P->INTSR;

    for(i = 0; i < 32; i++)
    {
        if((temp & (1 << i)) != 0)
        {
            pin_irq_hdr(i + 64);
        }
    }

    GPIO_P = (SGCC_GPIO_TypeDef *)(SGCC_GPIO3_BASE);
    temp = GPIO_P->INTSR;

    for(i = 0; i < 32; i++)
    {
        if((temp & (1 << i)) != 0)
        {
            pin_irq_hdr(i + 96);
        }
    }

    /* leave interrupt */
    rt_interrupt_leave();
}


#endif

/*@}*/
