/*
 * File      : uart.c
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
 * 2017-09-15     Haley        the first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <board.h>

#ifdef RT_USING_SERIAL
/**** UART0 and UART1 have two group config****/
/* UART0 */
#define UART0_GPIO_RX         UART0_PIN45RX
#define UART0_GPIO_TX         UART0_PIN44TX
//#define UART0_GPIO_RX         UART0_PIN97RX
//#define UART0_GPIO_TX         UART0_PIN96TX
/* UART1 */
#define UART1_GPIO_RX         UART1_PIN110RX
#define UART1_GPIO_TX         UART1_PIN109TX
//#define UART1_GPIO_RX         UART1_PIN116RX
//#define UART1_GPIO_TX         UART1_PIN113TX

#define UART_BUFFER_SIZE    256
uint8_t UartRxBuffer[UART_BUFFER_SIZE];
uint8_t UartTxBuffer[UART_BUFFER_SIZE];

/* uart driver */
struct scm620_uart
{
    SGCC_UART_TypeDef*  uart_device;
    IRQn_Type uart_interrupt;
    #ifdef RT_SERIAL_USING_DMA
    struct stm32_uart_dma
    {
        /* dma channel */
        DMA_Channel_TypeDef *rx_ch;
        /* dma global flag */
        uint32_t rx_gl_flag;
        /* dma irq channel */
        uint8_t rx_irq_ch;
        /* setting receive len */
        rt_size_t setting_recv_len;
        /* last receive index */
        rt_size_t last_recv_index;
    } dma;
    #endif /* RT_SERIAL_USING_DMA */
};

typedef struct
{
    uint32_t ui32BaudRate;  //baud rate

    uint32_t bTwoStopBits;  //stop bit, UART_CTRL_STOP_1BIT\UART_CTRL_STOP_2BIT

    uint32_t ui32Parity;  //PARITY_NONE/PARITY_ODD/PARITY_EVEN
}
scm620_uart_config_t;

#ifdef RT_SERIAL_USING_DMA
    static void DMA_Configuration(struct rt_serial_device *serial);
#endif /* RT_SERIAL_USING_DMA */

/**
 * @brief UART-based string print function.
 *
 * @param Send buff
 *
 * This function is used for printing a string via the UART, which for some
 * MCU devices may be multi-module.
 *
 * @return None.
 */
void rt_hw_uart_send_string(struct rt_serial_device *serial, char *pcString)
{
    struct scm620_uart* uart;

    RT_ASSERT(serial != RT_NULL);

    uart = (struct scm620_uart *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    UART_PutStr(uart->uart_device, (UINT8 *) pcString);
}

//connect scm620 drv to rt drv.
static rt_err_t scm620_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
    struct scm620_uart* uart;
    scm620_uart_config_t uart_cfg;

    RT_ASSERT(serial != RT_NULL);
    RT_ASSERT(cfg != RT_NULL);

    uart = (struct scm620_uart *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    /* Get the configure */
    uart_cfg.ui32BaudRate = cfg->baud_rate;

    if (cfg->stop_bits == STOP_BITS_1)
        uart_cfg.bTwoStopBits = UART_CTRL_STOP_1BIT;
    else if (cfg->stop_bits == STOP_BITS_2)
        uart_cfg.bTwoStopBits = UART_CTRL_STOP_2BIT;

    if (cfg->parity == PARITY_NONE)
        uart_cfg.ui32Parity = SCM_PARITY_NONE;
    else if (cfg->parity == PARITY_ODD)
        uart_cfg.ui32Parity = SCM_PARITY_ODD;
    else if (cfg->parity == PARITY_EVEN)
        uart_cfg.ui32Parity = SCM_PARITY_EVEN;

    /* UART Config and Enable */
    UART_Init(uart->uart_device, uart_cfg.ui32BaudRate, uart_cfg.ui32Parity, uart_cfg.bTwoStopBits);

    return RT_EOK;
}

static rt_err_t scm620_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct scm620_uart* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct scm620_uart *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    switch (cmd)
    {
        case RT_DEVICE_CTRL_CLR_INT:
            /* disable uart irq */
            NVIC_DisableIRQ(uart->uart_interrupt);
            /* disable interrupt */
            UART_ClearIntr(uart->uart_device, UART_INTSTATUS_RXINT);
            UART_EnableIntr(uart->uart_device, UART_CTRL_TXINTDIS | UART_CTRL_RXINTDIS);
            break;

        case RT_DEVICE_CTRL_SET_INT:
            /* enable uart irq */
            NVIC_EnableIRQ(uart->uart_interrupt);
            /* enable interrupt */
            UART_ClearIntr(uart->uart_device, UART_INTSTATUS_RXINT);
            UART_EnableIntr(uart->uart_device, UART_CTRL_RXINTEN);
            break;
            #ifdef RT_SERIAL_USING_DMA

        /* USART config */
        case RT_DEVICE_CTRL_CONFIG :
            if ((rt_uint32_t)(arg) == RT_DEVICE_FLAG_DMA_RX)
            {
                DMA_Configuration(serial);
            }

            break;
            #endif /* RT_SERIAL_USING_DMA */
    }

    return RT_EOK;
}

static int scm620_putc(struct rt_serial_device *serial, char c)
{
    struct scm620_uart* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct scm620_uart *)serial->parent.user_data;

    if (serial->parent.open_flag & RT_DEVICE_FLAG_INT_TX)
    {
        if ((uart->uart_device->STATUS & UART_STATUS_TXFULL))
        {
            UART_EnableIntr(uart->uart_device, UART_CTRL_TXINTEN);
            return -1;
        }

        UART_EnableIntr(uart->uart_device, UART_CTRL_TXINTEN);
        UART_PutChar(uart->uart_device, c);
    }
    else
    {
        UART_ClearIntr(uart->uart_device, UART_INTSTATUS_TXINT);
        UART_PutChar(uart->uart_device, c);

        while (!(uart->uart_device->STATUS & UART_STATUS_TXEMP));
    }

    return 1;
}

static int scm620_getc(struct rt_serial_device *serial)
{
    int ch = -1;
    struct scm620_uart* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct scm620_uart *)serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    if(((uart->uart_device->STATUS >> 1) & 0x01) != 0x01)
    {
        ch = UART_GetChar(uart->uart_device);
    }

    return ch;
}

/**
 * Uart common interrupt process. This need add to uart ISR.
 *
 * @param serial serial device
 */
static void uart_isr(struct rt_serial_device *serial)
{
    uint32_t intrStatus, intrEnSts;
    struct scm620_uart* uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct scm620_uart *) serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    /* Read the interrupt status */
    intrStatus = uart->uart_device->INTSTA;
    intrEnSts = uart->uart_device->CTRL;

    if(intrStatus & UART_INTSTATUS_stop_error)
    {
        UART_ClearIntr(uart->uart_device, UART_INTSTATUS_stop_error);
    }

    if(intrStatus & UART_INTSTATUS_parity_error)
    {
        UART_ClearIntr(uart->uart_device, UART_INTSTATUS_parity_error);
    }

    if((intrStatus & UART_INTSTATUS_RXINT) && (intrEnSts & UART_CTRL_RXINTEN) )
    {
        //clear interrupt
        UART_ClearIntr(uart->uart_device, UART_INTSTATUS_RXINT);
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_RX_IND);
    }

    if((intrStatus & UART_INTSTATUS_TXINT) && (intrEnSts & UART_CTRL_TXINTEN) )
    {
        //clear interrupt
        UART_ClearIntr(uart->uart_device, UART_INTSTATUS_TXINT);
        rt_hw_serial_isr(serial, RT_SERIAL_EVENT_TX_DONE);
        /* clear interrupt */
    }
}

static const struct rt_uart_ops scm620_uart_ops =
{
    scm620_configure,
    scm620_control,
    scm620_putc,
    scm620_getc,
};

#if defined(RT_USING_UART0)
/* UART0 device driver structure */
struct scm620_uart uart0 =
{
    //SGCC_UART0_BASE,
    SGCC_UART0_P,
    UART0_IRQn
};
static struct rt_serial_device serial0;
/*
void UART0_IRQHandler(void)
{
    
    rt_interrupt_enter();

    uart_isr(&serial0);


    rt_interrupt_leave();
}*/
#endif /* RT_USING_UART0 */

#if defined(RT_USING_UART1)
/* UART1 device driver structure */
struct scm620_uart uart1 =
{
    SGCC_UART1_BASE,
    UART1_IRQn
};
static struct rt_serial_device serial1;

void UART1_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial1);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART1 */

#if defined(RT_USING_UART2)
/* UART2 device driver structure */
struct scm620_uart uart2 =
{
    SGCC_UART2_P,
    UART2_IRQn
};
static struct rt_serial_device serial2;

void UART2_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial2);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART2 */

#if defined(RT_USING_UART3)
/* UART3 device driver structure */
struct scm620_uart uart3 =
{
    SGCC_UART3_P,
    UART3_IRQn
};
static struct rt_serial_device serial3;

void UART3_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial3);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART3 */

#if defined(RT_USING_UART4)
/* UART4 device driver structure */
struct scm620_uart uart4 =
{
    //SGCC_UART4_BASE,
    SGCC_UART4_P,
    UART4_IRQn
};
static struct rt_serial_device serial4;

void UART4_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial4);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART4 */

#if defined(RT_USING_UART5)
/* UART5 device driver structure */
struct scm620_uart uart5 =
{
    SGCC_UART5_BASE,
    UART5_IRQn
};
static struct rt_serial_device serial5;

void UART5_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial5);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART5 */

#if defined(RT_USING_UART6)
/* UART6 device driver structure */
struct scm620_uart uart6 =
{
    SGCC_UART6_BASE,
    UART6_IRQn
};
static struct rt_serial_device serial6;

void UART6_IRQHandler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    uart_isr(&serial6);

    /* leave interrupt */
    rt_interrupt_leave();
}
#endif /* RT_USING_UART6 */

#if defined(RT_USING_UART7)
/* UART7 device driver structure */
struct scm620_uart uart7 =
{
    //SGCC_UART7_BASE,
    SGCC_UART7_P,
    UART7_IRQn
};
static struct rt_serial_device serial7;
/*
void UART7_IRQHandler(void)
{

    rt_interrupt_enter();

    uart_isr(&serial7);


    rt_interrupt_leave();
}
*/
#endif /* RT_USING_UART7 */


static void GPIO_Configuration(void)
{
    #if defined(RT_USING_UART0)
    /* Make sure the UART RX and TX pins are enabled */
    UART0_InitPad(UART0_GPIO_RX, UART0_GPIO_TX);
    #endif /* RT_USING_UART0 */

    #if defined(RT_USING_UART1)
    /* Make sure the UART RX and TX pins are enabled */
    UART1_InitPad(UART1_GPIO_RX, UART1_GPIO_TX);
    #endif /* RT_USING_UART1 */

    #if defined(RT_USING_UART2)
    /* Make sure the UART RX and TX pins are enabled */
    UART2_InitPad();
    #endif /* RT_USING_UART2 */

    #if defined(RT_USING_UART3)
    /* Make sure the UART RX and TX pins are enabled */
    UART3_InitPad();
    #endif /* RT_USING_UART3 */

    #if defined(RT_USING_UART4)
    /* Make sure the UART RX and TX pins are enabled */
    UART4_InitPad();
    #endif /* RT_USING_UART4 */

    #if defined(RT_USING_UART5)
    /* Make sure the UART RX and TX pins are enabled */
    UART5_InitPad();
    #endif /* RT_USING_UART5 */

    #if defined(RT_USING_UART6)
    /* Make sure the UART RX and TX pins are enabled */
    UART6_InitPad();
    #endif /* RT_USING_UART6 */

    #if defined(RT_USING_UART7)
    /* Make sure the UART RX and TX pins are enabled */
    UART7_InitPad();
    #endif /* RT_USING_UART7 */
}

/**
 * @brief Initialize the UART
 *
 * This function initialize the UART
 *
 * @return None.
 */
int rt_hw_uart_init(void)
{
    struct scm620_uart* uart;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    GPIO_Configuration();

    #if defined(RT_USING_UART0)
    uart = &uart0;
    config.baud_rate = BAUD_RATE_115200;

    CLK_EnPeriClk(UART0);

    serial0.ops    = &scm620_uart_ops;
    serial0.config = config;

    /* register UART0 device */
    rt_hw_serial_register(&serial0, "uart0",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                          RT_DEVICE_FLAG_INT_TX, uart);
    #endif /* RT_USING_UART0 */

    #if defined(RT_USING_UART1)
    uart = &uart1;
    config.baud_rate = BAUD_RATE_115200;

    CLK_EnPeriClk(UART1);

    serial1.ops    = &scm620_uart_ops;
    serial1.config = config;

    /* register UART1 device */
    rt_hw_serial_register(&serial1, "uart1",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                          RT_DEVICE_FLAG_INT_TX, uart);
    #endif /* RT_USING_UART1 */

    #if defined(RT_USING_UART2)
    uart = &uart2;
    config.baud_rate = BAUD_RATE_115200;

    CLK_EnPeriClk(UART2);

    serial2.ops    = &scm620_uart_ops;
    serial2.config = config;

    /* register UART2 device */
    rt_hw_serial_register(&serial2, "uart2",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                          RT_DEVICE_FLAG_INT_TX, uart);
    #endif /* RT_USING_UART2 */

    #if defined(RT_USING_UART3)
    uart = &uart3;
    config.baud_rate = BAUD_RATE_115200;

    CLK_EnPeriClk(UART3);

    serial3.ops    = &scm620_uart_ops;
    serial3.config = config;

    /* register UART3 device */
    rt_hw_serial_register(&serial3, "uart3",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                          RT_DEVICE_FLAG_INT_TX, uart);
    #endif /* RT_USING_UART3 */

    #if defined(RT_USING_UART4)
    uart = &uart4;
    config.baud_rate = BAUD_RATE_115200;

    CLK_EnPeriClk(UART4);

    serial4.ops    = &scm620_uart_ops;
    serial4.config = config;

    /* register UART4 device */
    rt_hw_serial_register(&serial4, "uart4",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                          RT_DEVICE_FLAG_INT_TX, uart);
    #endif /* RT_USING_UART4 */

    #if defined(RT_USING_UART5)
    uart = &uart5;
    config.baud_rate = BAUD_RATE_115200;

    CLK_EnPeriClk(UART5);

    serial5.ops    = &scm620_uart_ops;
    serial5.config = config;

    /* register UART5 device */
    rt_hw_serial_register(&serial5, "uart5",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                          RT_DEVICE_FLAG_INT_TX, uart);
    #endif /* RT_USING_UART5 */

    #if defined(RT_USING_UART6)
    uart = &uart6;
    config.baud_rate = BAUD_RATE_115200;

    CLK_EnPeriClk(UART6);

    serial6.ops    = &scm620_uart_ops;
    serial6.config = config;

    /* register UART6 device */
    rt_hw_serial_register(&serial6, "uart6",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                          RT_DEVICE_FLAG_INT_TX, uart);
    #endif /* RT_USING_UART6 */

    #if defined(RT_USING_UART7)
    uart = &uart7;
    config.baud_rate = BAUD_RATE_115200;

    CLK_EnPeriClk(UART7);

    serial7.ops    = &scm620_uart_ops;
    serial7.config = config;

    /* register UART7 device */
    rt_hw_serial_register(&serial7, "uart7",
                          RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX |
                          RT_DEVICE_FLAG_INT_TX, uart);
    #endif /* RT_USING_UART7 */

    return 0;
}

INIT_BOARD_EXPORT(rt_hw_uart_init);

#endif /* RT_USING_SERIAL */
/*@}*/
