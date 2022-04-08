#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "LE_uart.h"
//#include "LE_can.h"
//#include "LE_sst_flash.h"
//#include "RX8025T.h"



//const struct dfs_mount_tbl mount_table[] =
//{
//    {"nand0", "/", "uffs", 0, 0},
//    {0}
//};

#ifdef RT_USING_FINSH
    #include <finsh.h>
    #include <shell.h>
#endif

struct rt_event com_event;
//static struct rt_mailbox mb_uart0;
rt_mailbox_t mb_uart0 = RT_NULL;
rt_sem_t sem_com_pcs = RT_NULL;
rt_sem_t sem_com_dc = RT_NULL;
rt_sem_t sem_com_meter = RT_NULL;
rt_sem_t sem_com_pv = RT_NULL;
rt_mq_t mq_can0 = RT_NULL;
rt_mq_t mq_can1_rev = RT_NULL;
rt_mq_t mq_can1_send = RT_NULL;

extern CAN_FrameInfoTypeDef  ReceFrame;

void LED_Display(void *args)
{
    static unsigned char Led_Flag = 0;

    if(Led_Flag == 0)
    {
        Led_Flag = 1;
        rt_pin_write(36, PIN_HIGH);
    }
    else
    {
        Led_Flag = 0;
        rt_pin_write(36, PIN_LOW);
    }
}

static rt_uint8_t pin_stack[256];
static struct rt_thread pin_thread;
static rt_device_t pin_device = RT_NULL;

static void pin_thread_entry(void* parameter)
{
    unsigned int Delay_Count = 0;

    //查询PIN设备并打开
    pin_device = rt_device_find("pin");
    rt_device_open(pin_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM);

    //初始化LED引脚

    rt_pin_mode(55, PIN_MODE_OUTPUT);
//   rt_pin_mode(43, PIN_MODE_OUTPUT);
//   rt_pin_mode(36, PIN_MODE_OUTPUT);
//   rt_pin_mode(35, PIN_MODE_OUTPUT);

    //初始化按键引脚
//   rt_pin_mode(108, PIN_MODE_INPUT);

//   rt_pin_mode(0, PIN_MODE_INPUT);
//   rt_pin_attach_irq(0, PIN_IRQ_MODE_FALLING, LED_Display, RT_NULL);
//   rt_pin_irq_enable(0, PIN_IRQ_ENABLE);

    //  rt_pin_mode(1, PIN_MODE_INPUT);
    //  rt_pin_attach_irq(1, PIN_IRQ_MODE_FALLING, LED_Display, RT_NULL);
    //  rt_pin_irq_enable(1, PIN_IRQ_ENABLE);

    while (1)
    {

        rt_pin_write(55, PIN_HIGH);

        rt_thread_delay(1000/ portTICK_RATE_MS);
        rt_pin_write(55, PIN_LOW);


        //读取KEY4，并跟随状态亮灭LED
        /*       if(rt_pin_read(108) == PIN_HIGH)
               {
                   rt_pin_write(43, PIN_HIGH);
               }
               else
               {
                   rt_pin_write(43, PIN_LOW);
               }
        */
        rt_thread_delay(1000/ portTICK_RATE_MS);
    }
}


static rt_uint8_t wdt_stack[200];
static struct rt_thread wdt_thread;

static void wdt_thread_entry(void* parameter)
{
    static rt_device_t wdt_device = RT_NULL;

    /* 根据设备名称查找看门狗设备，获取设备句柄 */
    wdt_device = rt_device_find("wdt");
    rt_device_open(wdt_device, RT_DEVICE_OFLAG_RDWR);

    rt_device_control(wdt_device, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, (void *)SYSWDT_CTRL_OTSET_1s);

    while (1)
    {
        /* 在空闲线程的回调函数里喂狗 */
        rt_device_control(wdt_device, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
        rt_thread_delay(1000/ portTICK_RATE_MS);
    }
}


int main(void)
{
    rt_err_t result;
    static rt_thread_t tid1 = RT_NULL;
    static struct fm24clxx_config fm24c08_config;
    /* 定 时 器 的 控 制 块 */
    static rt_timer_t timer1, timer2, timer3;

    /* init pin thread */

    sem_com_pcs =  rt_sem_create("sem_com_pcs", 0, RT_IPC_FLAG_FIFO);
    sem_com_dc =  rt_sem_create("sem_com_dc", 0, RT_IPC_FLAG_FIFO);
    sem_com_meter = rt_sem_create("sem_com_meter", 0, RT_IPC_FLAG_FIFO);
    sem_com_pv = rt_sem_create("sem_com_pv", 0, RT_IPC_FLAG_FIFO);

    mq_can0 = rt_mq_create("can0_rev", sizeof(ReceFrame), 30, RT_IPC_FLAG_FIFO);
    mq_can1_rev = rt_mq_create("can1_rev", sizeof(ReceFrame), 30, RT_IPC_FLAG_FIFO);
    mq_can1_send = rt_mq_create("can1_send", sizeof(ReceFrame), 30, RT_IPC_FLAG_FIFO);

    result = rt_event_init(&com_event, "event", RT_IPC_FLAG_PRIO);
    mb_uart0 = rt_mb_create ("mailbox_uart0", 256, RT_IPC_FLAG_FIFO);

    if (mb_uart0 == RT_NULL)
    {
        rt_kprintf("init mailbox failed.\n");
        return -1;
    }

    result = rt_thread_init(&pin_thread,
                            "pin",
                            pin_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&pin_stack[0],
                            sizeof(pin_stack),
                            RT_THREAD_PRIORITY_MAX / 3,
                            5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&pin_thread);
    }

    /* init wdt thread */
    /*    result = rt_thread_init(&wdt_thread,
                                "wdt",
                                wdt_thread_entry,
                                RT_NULL,
                                (rt_uint8_t*)&wdt_stack[0],
                                sizeof(wdt_stack),
                                RT_THREAD_PRIORITY_MAX / 3,
                                5);

        if (result == RT_EOK)
        {
            rt_thread_startup(&wdt_thread);
        }
    */

    /* init uart thread */
    result = rt_thread_init(&uart7_thread,
                            "uart7",
                            uart7_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&uart7_stack[0],
                            sizeof(uart7_stack),
                            8,  //0 优先级代表最高优先级，最低优先级留给空闲线程使用；
                            5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&uart7_thread);
    }

  //优先级；数字越大，优先级越低    0最高优先级
    tid1 = rt_thread_create("uart0_rev",uart0_rev_thread_entry,RT_NULL,512,8, 5);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid1 = rt_thread_create("uart0_read",uart0_read_thread_entry,RT_NULL,512,15, 5);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid1 = rt_thread_create("Uart0_MSG_thread",Uart0_MSG_thread_entry,RT_NULL, 512, 10, 5);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid1 = rt_thread_create("uart3_rev", uart3_rev_thread_entry, RT_NULL, 512, 8, 5);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid1 = rt_thread_create("uart3_read",uart3_read_thread_entry,RT_NULL,512, 15, 5);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid1 = rt_thread_create("can0_rev",can0_rev_thread_entry, RT_NULL,512,8, 5);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid1 = rt_thread_create("can1_rev",can1_rev_thread_entry,RT_NULL,512,8, 5);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid1 = rt_thread_create("can1_send",can1_send_thread_entry,RT_NULL,512, 13, 5);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);


    tid1 = rt_thread_create("flash",flash_thread_entry, RT_NULL,512,8, 5);

    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    tid1 = rt_thread_create("rtc", rtc_thread_entry, RT_NULL, 512, 6, 5);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

	tid1 = rt_thread_create("eth",eth_thread_entry,RT_NULL,1024,7, 5);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
	
	tid1 = rt_thread_create("data",Data_Transfer_entry,RT_NULL,512,17, 5);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
	
	tid1 = rt_thread_create("ems",EMS_thread_YGY_entry,RT_NULL,1024,18, 5);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
	
	tid1 = rt_thread_create("ethlink",vEthLinkChkTask,RT_NULL,256,18, 5);
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);
	
    /*    result = rt_thread_init(&uart7_timer_thread,
                                "timer7",
                                uart7_timer_thread_entry,
                                RT_NULL,
                                (rt_uint8_t*)&uart7_timer_stack[0],
                                sizeof(uart7_timer_stack),
                                RT_THREAD_PRIORITY_MAX / 3,
                                5);

        if (result == RT_EOK)
        {
            rt_thread_startup(&uart7_timer_thread);
        }
    */
    timer1 = rt_timer_create("timer_uart7", timeout_uart7,
                             RT_NULL, 10,
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);

    if (timer1 != RT_NULL)
        rt_timer_start(timer1);

    timer2 = rt_timer_create("timer_uart0", timeout_uart0,
                             RT_NULL, 10,
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);

    if (timer2 != RT_NULL)
        rt_timer_start(timer2);

    timer3 = rt_timer_create("timer_uart3", timeout_uart3,
                             RT_NULL, 10,
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);

    if (timer3 != RT_NULL)
        rt_timer_start(timer3);


    InitGlobalVariables();

    return 0;
}

/*
int fputc(int ch, FILE *f)
{
	ITM_SendChar(ch);
	return ch;
}*/