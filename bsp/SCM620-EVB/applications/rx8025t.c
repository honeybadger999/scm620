
#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "SCM62X.h"
#include "RX8025T.h"
#include "LE_sst_flash.h"
#include "LE_uart.h"

#define SDAA_Read()   GPIO_RdDataIn(3)

#define SCL_H()		GPIO_WrDataOut(2,GPIO_HIGH)
#define SCL_L()		GPIO_WrDataOut(2,GPIO_LOW)
#define SDA_H()		GPIO_WrDataOut(3,GPIO_HIGH)
#define SDA_L()		GPIO_WrDataOut(3,GPIO_LOW)

RTC_TIME RtcTime;
uint16_t  timePC[6];

void nrf_delay_us(unsigned int time)
{
    unsigned int  b = 0;

    for(b = 0; b < 300; b++); //10uS
}

void RX8025T_Init(void)
{
    GPIO_InitPad(2);	//SCL
    GPIO_ConfigDir(2, GPIO_OUTPUT);
    GPIO_WrDataOut(2, GPIO_HIGH);
    GPIO_InitPad(3);	//SDA
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    GPIO_WrDataOut(3, GPIO_HIGH);
}


//起始信号//
void rx_i2c_start(void)
{
    SCL_H();
    nrf_delay_us(1);
    //下降沿，起始信号//
    SDA_L();
    nrf_delay_us(1);
    SCL_L();
    nrf_delay_us(1);
}

//发送一个字节8位，返回ACK值
u8 rx_I2C_WriteByte(u8 putData)
{
    u8 i;

    //八位数据传输
    for(i = 0; i < 8; i++)
    {
        if((putData & 0x80) > 0)
        {
            SDA_H();
        }
        else
        {
            SDA_L();
        }

        putData <<= 1;
        nrf_delay_us(1);
        SCL_H();  //从设备这个高电平读取一位
        nrf_delay_us(1);
        SCL_L();
        nrf_delay_us(1);
    }

    //应答位判断//
    SDA_H();//释放总线
    //nrf_delay_us(1);
    SCL_H();
    nrf_delay_us(1);	    		//必须有
    SDAA_Read(); //ACK 从设备给拉低
    //从设备释放总线，SDA应该为高//
    SCL_L();
    nrf_delay_us(1);

    return 0;
}

//结束信号//都为高退出//
void rx_i2c_stop(void)
{
    //将SDA拉低//
    SDA_L();
    nrf_delay_us(1);			//必须有
    SCL_H();
    nrf_delay_us(1);			//必须有
    //上跳沿，结束信号//
    SDA_H();
    nrf_delay_us(1);
}



/*******************************************************************
*
* u8 Is_Leap_Year(u16 year) -判断是否是闰年函数
* 输入参数：year：年份
* 返回值：判断结果：-0：非闰年
*										-1：闰年
***********************************************************************/
UINT8 Is_Leap_Year(UINT16 year)
{
    if(year % 4 == 0) //必须能被4整除
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return 1; //如果以00结尾,还要能被400整除
            else return 0;
        }
        else return 1;
    }
    else return 0;
}

/*******************************************************************
*
* u32 DateToUSec(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec) -将年月日时分秒转换为UTC秒数计时
* 输入参数：syear：年份，如01 02 19
*						smon：月份
*						sday：日
*						hour：时
*						min：分
*						sec：秒
* 返回值：UTC秒数：-0：表示错误
*
***********************************************************************/
UINT32 DateToUSec(RTC_TIME time)
{
    UINT16 t, syear;
    UINT32 seccount = 0;
    //平年的月份日期表
    UINT16 syear1 = time.tm_year;
    UINT8 smon = time.tm_mon;
    UINT8 sday = time.tm_mday;
    UINT8 hour = time.tm_hour;
    UINT8 min = time.tm_min;
    UINT8 sec = time.tm_sec;
    const UINT8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    syear = syear1; 			// 年份修正

    if(syear < 1970 || syear > RtcTimeBais + 99)
        return 0;

    if(smon < 1 || smon > 12)
        return 0;

    if(sday < 1 || sday > 31)
        return 0;

    for(t = 1970; t < syear; t++)	//把所有年份的秒钟相加
    {
        if(Is_Leap_Year(t))
            seccount += 31622400; //闰年的秒钟数
        else
            seccount += 31536000;			 //平年的秒钟数
    }

    smon -= 1;

    for(t = 0; t < smon; t++)	 //把前面月份的秒钟数相加
    {
        seccount += (u32)mon_table[t] * 86400; //月份秒钟数相加

        if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400; //闰年2月份增加一天的秒钟数
    }

    seccount += (u32)(sday - 1) * 86400; //把前面日期的秒钟数相加
    seccount += (u32)hour * 3600; //小时秒钟数
    seccount += (u32)min * 60;	 //分钟秒钟数
    seccount += sec; //最后的秒钟加上去

    // seccount -= 8*3600; // 将东八区转换到格林威治时间。 su @ 20190121

    return seccount;
}

/*******************************************************************
*
* u8 GetWeek(u16 year,u8 month,u8 day) -获得星期
* 输入参数：year：年份，如2001
*						mon：月份
*						day：日
*	返回值：星期数：1-7表示星期一至星期日
***********************************************************************/
UINT8 GetWeek(UINT16 year, UINT8 month, UINT8 day)
{
    UINT16 temp2;
    UINT8 yearH, yearL;
    //星期修正表
    const UINT8 table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

    yearH = year / 100;
    yearL = year % 100;

    // 如果为21世纪,年份数加100
    if (yearH > 19)yearL += 100;

    // 所过闰年数只算1900年之后的
    temp2 = yearL + yearL / 4;
    temp2 = temp2 % 7;
    temp2 = temp2 + day + table_week[month - 1];

    if (yearL % 4 == 0 && month < 3)temp2--;

    temp2 = temp2 % 7;

    if(temp2 == 0)temp2 = 7;

    return temp2;

}
/*******************************************************************
*
* void SecToDate(uint32 usec, calendar_obj* pCalendar) -将UTC秒数转换为日期
* 输入参数：USec：UTC秒数
*						pCalendar：获得的日期返回值指针
*
***********************************************************************/
void SecToDate(UINT32 usec, RTC_TIME *pCalendar)
{
    UINT16 daycnt = 0, temp_year;;
    UINT32 temp = 0;
    UINT16 temp1 = 0;
    //平年的月份日期表
    const UINT8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    usec += 8 * 3600; // 将格林威治时间转换到东八区。
    temp = usec / 86400; //得到天数(秒钟数对应的)

    if(daycnt != temp) //超过一天了
    {
        daycnt = temp;
        temp1 = 1970;	//从1970年开始

        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))//是闰年
            {
                if(temp >= 366)temp -= 366; //闰年的秒钟数
                else
                {
                    temp1++;
                    break;
                }
            }
            else temp -= 365;	 //平年

            temp1++;
        }

        temp_year = temp1;//得到年份
        temp1 = 0;

        while(temp >= 28) //超过了一个月
        {
            if(Is_Leap_Year(temp_year) && temp1 == 1) //当年是不是闰年/2月份
            {
                if(temp >= 29)temp -= 29; //闰年的秒钟数
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1]; //平年
                else break;
            }

            temp1++;
        }

        pCalendar->tm_mon = temp1 + 1;	//得到月份
        pCalendar->tm_mday = temp + 1;  	//得到日期
    }
    else
    {
        temp_year = 1970;
        pCalendar->tm_mon = 1;
        pCalendar->tm_mday = 1;
    }

    temp = usec % 86400;     		//得到秒钟数
    pCalendar->tm_hour = temp / 3600;     	//小时
    pCalendar->tm_min = (temp % 3600) / 60; 	//分钟
    pCalendar->tm_sec = (temp % 3600) % 60; 	//秒钟
    pCalendar->tm_wday = GetWeek(temp_year, pCalendar->tm_mon, pCalendar->tm_mday); //获取星期

    pCalendar->tm_year = (temp_year > RtcTimeBais) ? temp_year : 0; //得到年份
}

//应答信号//
void rx_i2c_ack(void)
{
    //主设备回应个ACK//
    SDA_L();
    nrf_delay_us(1);
    SCL_H();
    nrf_delay_us(1);
    SCL_L();
    nrf_delay_us(1);
    //回应完ACK后，主将SDA线置高，控制权交给从//
    SDA_H();
    //nrf_delay_us(1);
}
//读取一个字节，返回读到的数//
u8 rx_I2C_ReadByte(void)
{
    u8 i, Temp;

    Temp = 0;

    //这里数据线由从设备控制//
    for(i = 0; i < 8; i++)
    {
        SCL_H();
        Temp <<= 1;
        Temp |= SDAA_Read();
        nrf_delay_us(1);
        SCL_L();
        nrf_delay_us(1);
    }

    return Temp;
}



/**
  * @brief  从芯片读取时钟和日期数据
  * @param  pdt	tm结构体变量地址
  * @retval 无
  */
void RX8025T_read(RTC_TIME *pdt)
{
    u8 tmp1;
    u8 TempAck;
    u8 tempaddr;

    //起始信号//
    rx_i2c_start();
    //写命令
    tempaddr = 0x64;
    TempAck = rx_I2C_WriteByte(tempaddr);
    //地址低8位//
    TempAck = rx_I2C_WriteByte(0);

    //起始信号
    rx_i2c_start();
    //读命令，最低为置1//
    tempaddr = 0x65;
    TempAck = rx_I2C_WriteByte(tempaddr);

    GPIO_ConfigDir(3, GPIO_INPUT);
    nrf_delay_us(1);
    //1开始接收数据
    tmp1 = rx_I2C_ReadByte();
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    rx_i2c_ack();
    GPIO_ConfigDir(3, GPIO_INPUT);
    pdt->tm_sec = (tmp1 >> 4) * 10 + (tmp1 & 0x0F);

    //2
    tmp1 = rx_I2C_ReadByte();
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    rx_i2c_ack();
    GPIO_ConfigDir(3, GPIO_INPUT);
    pdt->tm_min = (tmp1 >> 4) * 10 + (tmp1 & 0x0F);

    //3
    tmp1 = rx_I2C_ReadByte();
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    rx_i2c_ack();
    GPIO_ConfigDir(3, GPIO_INPUT);
    pdt->tm_hour = (tmp1 >> 4) * 10 + (tmp1 & 0x0F);

    //4
    tmp1 = rx_I2C_ReadByte();
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    rx_i2c_ack();
    GPIO_ConfigDir(3, GPIO_INPUT);
    pdt->tm_wday = (tmp1 >> 4) * 10 + (tmp1 & 0x0F); // 周日为0x00转换成7

    //5
    tmp1 = rx_I2C_ReadByte();
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    rx_i2c_ack();
    GPIO_ConfigDir(3, GPIO_INPUT);
    pdt->tm_mday = (tmp1 >> 4) * 10 + (tmp1 & 0x0F);

    //6
    tmp1 = rx_I2C_ReadByte();
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    rx_i2c_ack();
    GPIO_ConfigDir(3, GPIO_INPUT);
    pdt->tm_mon = (tmp1 >> 4) * 10 + (tmp1 & 0x0F);

    //7
    //最后一个字节不回应ACK//
    tmp1 = rx_I2C_ReadByte();
    nrf_delay_us(1);
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    pdt->tm_year = (tmp1 >> 4) * 10 + (tmp1 & 0x0F) + RtcTimeBais;

    //i2c_nack();//SDA应该就是高，NACK就是SDA为高//
    SCL_H();
    nrf_delay_us(1);
    SCL_L();
    nrf_delay_us(1);
    //停止信号//
    rx_i2c_stop();
    nrf_delay_us(1);
    nrf_delay_us(1);

}

/**
  * @brief  向芯片设置时钟和日期数据
  * @param  pdt	tm结构体变量地址
  * @retval 无
  */
void RX8025T_write(RTC_TIME *pdt)
{
    u8 TempAck;
    u8 tempaddr;
    u8 year;

    if(pdt->tm_year < 1970 || pdt->tm_year > RtcTimeBais + 99)
        return;

    year = pdt->tm_year - RtcTimeBais;

    //起始信号//
    rx_i2c_start();
    //写命令
    tempaddr = 0x64;
    TempAck = rx_I2C_WriteByte(tempaddr);
    //地址//
    TempAck = rx_I2C_WriteByte(0);
    //发送数据//
    rx_I2C_WriteByte((((pdt->tm_sec) / 10) << 4) | ((pdt->tm_sec) % 10));
    rx_I2C_WriteByte((((pdt->tm_min) / 10) << 4) | ((pdt->tm_min) % 10));
    rx_I2C_WriteByte((((pdt->tm_hour) / 10) << 4) | ((pdt->tm_hour) % 10));

    if(pdt->tm_wday == 1)
        rx_I2C_WriteByte(0x02);

    if(pdt->tm_wday == 2)
        rx_I2C_WriteByte(0x04);

    if(pdt->tm_wday == 3)
        rx_I2C_WriteByte(0x08);

    if(pdt->tm_wday == 4)
        rx_I2C_WriteByte(0x10);

    if(pdt->tm_wday == 5)
        rx_I2C_WriteByte(0x20);

    if(pdt->tm_wday == 6)
        rx_I2C_WriteByte(0x40);

    if(pdt->tm_wday == 7)
        rx_I2C_WriteByte(0x01);

    rx_I2C_WriteByte((((pdt->tm_mday) / 10) << 4) | ((pdt->tm_mday) % 10));
    rx_I2C_WriteByte((((pdt->tm_mon) / 10) << 4) | ((pdt->tm_mon) % 10));
    rx_I2C_WriteByte((((year) / 10) << 4) | ((year) % 10));

    //停止信号//
    rx_i2c_stop();
    nrf_delay_us(1);
    nrf_delay_us(1);
}



void RX8025_test(void)
{
    RTC_TIME WRtcTime;		//设置时间

    WRtcTime.tm_sec = 0;
    WRtcTime.tm_min = 6;
    WRtcTime.tm_hour = 15;
    WRtcTime.tm_wday = 5;
    WRtcTime.tm_mday = 11;
    WRtcTime.tm_mon = 1;
    WRtcTime.tm_year = 2022;

    RX8025T_write(&WRtcTime);//初始化时间
}

void rtc_thread_entry(void *parameter)
{
	rt_uint32_t e;
	static uint16_t timer=0,timer_1hour=0;
	uint16_t temp=0;
	uint16_t *P_power = ( uint16_t *)&(PVAC_Power[0]);
	rt_err_t val;
	RX8025T_Init();
	

    RtcTime.tm_sec = 0;
    RtcTime.tm_min = 6;
    RtcTime.tm_hour = 15;
    RtcTime.tm_wday = 5;
    RtcTime.tm_mday = 11;
    RtcTime.tm_mon = 1;
    RtcTime.tm_year = 2022;
	
	for(;;)
	{
		rt_thread_delay(300);
			
		if(timer++ == 5)
		{
			RX8025T_read(&RtcTime);
			
			timePC[0] = RtcTime.tm_year;
			timePC[1] = RtcTime.tm_mon;
			timePC[2] = RtcTime.tm_mday;
			timePC[3] = RtcTime.tm_hour;
			timePC[4] = RtcTime.tm_min;
			timePC[5] = RtcTime.tm_sec;
			
			timer=0;									
		}
		//存储三个电能,1小时存储一次。在开机时读取。
		if(timer_1hour++ >= 1200)
		{	
			Store_par_DataFlash(P_power,FLASHQUAN_POWER,FLASHSECTOR_POWER,FLASHADD_POWER);
			timer_1hour=0;
		}
		
		val = rt_event_recv(&com_event, EVENT_FLAG_RTC_WRITE,(RT_EVENT_FLAG_OR|RT_EVENT_FLAG_CLEAR),10, &e);
		if( val == RT_EOK)
		{
			RtcTime.tm_year = timePC[0];
			RtcTime.tm_mon  = timePC[1];
			RtcTime.tm_mday = timePC[2];
			RtcTime.tm_hour = timePC[3];
			RtcTime.tm_min  = timePC[4];
			RtcTime.tm_sec  = 0;
			RtcTime.tm_wday = 1;
				
			RX8025T_write(&RtcTime);
		}
		
	}
}
