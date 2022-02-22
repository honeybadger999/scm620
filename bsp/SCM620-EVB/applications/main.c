#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>


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

extern rt_uint8_t uart7_stack[256];
extern struct rt_thread uart7_thread;
extern void uart7_thread_entry(void* parameter);
extern UINT16 Uart7_rev_counter,com7_rxlen;

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

    //��ѯPIN�豸����
    pin_device = rt_device_find("pin");
    rt_device_open(pin_device, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_STREAM);

    //��ʼ��LED����

    rt_pin_mode(55, PIN_MODE_OUTPUT);
//   rt_pin_mode(43, PIN_MODE_OUTPUT);
//   rt_pin_mode(36, PIN_MODE_OUTPUT);
//   rt_pin_mode(35, PIN_MODE_OUTPUT);

    //��ʼ����������
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

        rt_thread_delay(100);
        rt_pin_write(55, PIN_LOW);


        //��ȡKEY4��������״̬����LED
        /*       if(rt_pin_read(108) == PIN_HIGH)
               {
                   rt_pin_write(43, PIN_HIGH);
               }
               else
               {
                   rt_pin_write(43, PIN_LOW);
               }
        */
        rt_thread_delay(100);
    }
}


static rt_uint8_t wdt_stack[200];
static struct rt_thread wdt_thread;

static void wdt_thread_entry(void* parameter)
{
    static rt_device_t wdt_device = RT_NULL;

    /* �����豸���Ʋ��ҿ��Ź��豸����ȡ�豸��� */
    wdt_device = rt_device_find("wdt");
    rt_device_open(wdt_device, RT_DEVICE_OFLAG_RDWR);

    rt_device_control(wdt_device, RT_DEVICE_CTRL_WDT_SET_TIMEOUT, (void *)SYSWDT_CTRL_OTSET_1s);

    while (1)
    {
        /* �ڿ����̵߳Ļص�������ι�� */
        rt_device_control(wdt_device, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
        rt_thread_delay(100);
    }
}

//static rt_uint8_t eth_stack[512];
//static struct rt_thread eth_thread;

//static void eth_thread_entry(void* parameter)
//{
//		static rt_device_t eth_device = RT_NULL;
//
//    eth_device = rt_device_find("eth0");
//		rt_device_open(eth_device, RT_DEVICE_OFLAG_RDWR);
//
//		while (1)
//    {
//      rt_thread_delay(10);
//    }
//}

/* �� ʱ �� 1 �� ʱ �� �� */
static void timeout_uart(void *parameter)
{
    rt_kprintf("periodic timer is timeout \n");
    static uint16_t com7_rx_sta = 0, last_com7_rx_sta = 0, last_com7_rxlen = 0; //com״̬
    rt_uint32_t e;

    //�����¼����������ж��ڿ�ʼ���պ� �����¼�
//	rt_event_recv(&com_event,EVENT_FLAG_RV_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);

	if(com7_rxlen > last_com7_rxlen )
	{
		//���ڽ���״̬
		com7_rx_sta = 1;
	}
	else if(com7_rxlen == last_com7_rxlen )
	{
		//�������״̬
		com7_rx_sta = 0;
	}
	else
	{
		//������� ��Uart7_rev_counter����0,Uart7_rev_counterӦ�ڽ�������������
	}

	if((com7_rx_sta == 0) && (last_com7_rx_sta == 1))
	{
		Uart7_rev_counter = com7_rxlen;
		rt_event_send(&com_event, EVENT_FLAG_PS_UART7);
		com7_rxlen=0;
		//�����ź���
	}

	last_com7_rx_sta = com7_rx_sta;
	last_com7_rxlen = com7_rxlen;
//	rt_thread_delay(10);
}


static rt_uint8_t uart7_timer_stack[200];
static struct rt_thread uart7_timer_thread;
void uart7_timer_thread_entry(void* parameter)
{
    //�����¼����������ж��ڿ�ʼ���պ� �����¼�
//	rt_event_recv(&com_event,EVENT_FLAG_RV_UART7,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),RT_WAITING_FOREVER, &e);
    while(1)
    {

    }
}

int main(void)
{
    rt_err_t result;
    static struct fm24clxx_config fm24c08_config;
    /* �� ʱ �� �� �� �� �� */
    static rt_timer_t timer1;
    /* �� �� �� �� �� */
	Crc_Init();	
    /*
        w25qxx_init("w25q64", "spi1");
        fm24c08_config.addr = 0xA0;
        fm24c08_config.flags = 0;
        fm24c08_config.size = 1024 * 8;
        fm24clxx_register("fm24c08", "i2c0", &fm24c08_config);
    */
    /* init pin thread */
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

    /* init pwm thread */
    /*   result = rt_thread_init(&pwm_thread,
                               "pwm",
                               pwm_thread_entry,
                               RT_NULL,
                               (rt_uint8_t*)&pwm_stack[0],
                               sizeof(pwm_stack),
                               RT_THREAD_PRIORITY_MAX / 3,
                               5);

       if (result == RT_EOK)
       {
           rt_thread_startup(&pwm_thread);
       }
    */
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
//		/* init eth thread */
//    result = rt_thread_init(&eth_thread,
//                            "eth",
//                            eth_thread_entry,
//                            RT_NULL,
//                            (rt_uint8_t*)&eth_stack[0],
//                            sizeof(eth_stack),
//                            RT_THREAD_PRIORITY_MAX/3,
//                            5);
//    if (result == RT_EOK)
//    {
//        rt_thread_startup(&eth_thread);
//    }

    /* init uart thread */
    result = rt_thread_init(&uart7_thread,
                            "uart7",
                            uart7_thread_entry,
                            RT_NULL,
                            (rt_uint8_t*)&uart7_stack[0],
                            sizeof(uart7_stack),
                            RT_THREAD_PRIORITY_MAX / 3,  //0 ���ȼ�����������ȼ���������ȼ����������߳�ʹ�ã�
                            5);

    if (result == RT_EOK)
    {
        rt_thread_startup(&uart7_thread);
    }

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
    timer1 = rt_timer_create("timer_uart", timeout_uart,
                             RT_NULL, 5,
                             RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);

    if (timer1 != RT_NULL)
        rt_timer_start(timer1);

    result = rt_event_init(&com_event, "event", RT_IPC_FLAG_PRIO);
    return 0;
}


