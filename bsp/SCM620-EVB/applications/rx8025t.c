
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


//��ʼ�ź�//
void rx_i2c_start(void)
{
    SCL_H();
    nrf_delay_us(1);
    //�½��أ���ʼ�ź�//
    SDA_L();
    nrf_delay_us(1);
    SCL_L();
    nrf_delay_us(1);
}

//����һ���ֽ�8λ������ACKֵ
u8 rx_I2C_WriteByte(u8 putData)
{
    u8 i;

    //��λ���ݴ���
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
        SCL_H();  //���豸����ߵ�ƽ��ȡһλ
        nrf_delay_us(1);
        SCL_L();
        nrf_delay_us(1);
    }

    //Ӧ��λ�ж�//
    SDA_H();//�ͷ�����
    //nrf_delay_us(1);
    SCL_H();
    nrf_delay_us(1);	    		//������
    SDAA_Read(); //ACK ���豸������
    //���豸�ͷ����ߣ�SDAӦ��Ϊ��//
    SCL_L();
    nrf_delay_us(1);

    return 0;
}

//�����ź�//��Ϊ���˳�//
void rx_i2c_stop(void)
{
    //��SDA����//
    SDA_L();
    nrf_delay_us(1);			//������
    SCL_H();
    nrf_delay_us(1);			//������
    //�����أ������ź�//
    SDA_H();
    nrf_delay_us(1);
}



/*******************************************************************
*
* u8 Is_Leap_Year(u16 year) -�ж��Ƿ������꺯��
* ���������year�����
* ����ֵ���жϽ����-0��������
*										-1������
***********************************************************************/
UINT8 Is_Leap_Year(UINT16 year)
{
    if(year % 4 == 0) //�����ܱ�4����
    {
        if(year % 100 == 0)
        {
            if(year % 400 == 0)return 1; //�����00��β,��Ҫ�ܱ�400����
            else return 0;
        }
        else return 1;
    }
    else return 0;
}

/*******************************************************************
*
* u32 DateToUSec(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec) -��������ʱ����ת��ΪUTC������ʱ
* ���������syear����ݣ���01 02 19
*						smon���·�
*						sday����
*						hour��ʱ
*						min����
*						sec����
* ����ֵ��UTC������-0����ʾ����
*
***********************************************************************/
UINT32 DateToUSec(RTC_TIME time)
{
    UINT16 t, syear;
    UINT32 seccount = 0;
    //ƽ����·����ڱ�
    UINT16 syear1 = time.tm_year;
    UINT8 smon = time.tm_mon;
    UINT8 sday = time.tm_mday;
    UINT8 hour = time.tm_hour;
    UINT8 min = time.tm_min;
    UINT8 sec = time.tm_sec;
    const UINT8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    syear = syear1; 			// �������

    if(syear < 1970 || syear > RtcTimeBais + 99)
        return 0;

    if(smon < 1 || smon > 12)
        return 0;

    if(sday < 1 || sday > 31)
        return 0;

    for(t = 1970; t < syear; t++)	//��������ݵ��������
    {
        if(Is_Leap_Year(t))
            seccount += 31622400; //�����������
        else
            seccount += 31536000;			 //ƽ���������
    }

    smon -= 1;

    for(t = 0; t < smon; t++)	 //��ǰ���·ݵ����������
    {
        seccount += (u32)mon_table[t] * 86400; //�·����������

        if(Is_Leap_Year(syear) && t == 1)
            seccount += 86400; //����2�·�����һ���������
    }

    seccount += (u32)(sday - 1) * 86400; //��ǰ�����ڵ����������
    seccount += (u32)hour * 3600; //Сʱ������
    seccount += (u32)min * 60;	 //����������
    seccount += sec; //�������Ӽ���ȥ

    // seccount -= 8*3600; // ��������ת������������ʱ�䡣 su @ 20190121

    return seccount;
}

/*******************************************************************
*
* u8 GetWeek(u16 year,u8 month,u8 day) -�������
* ���������year����ݣ���2001
*						mon���·�
*						day����
*	����ֵ����������1-7��ʾ����һ��������
***********************************************************************/
UINT8 GetWeek(UINT16 year, UINT8 month, UINT8 day)
{
    UINT16 temp2;
    UINT8 yearH, yearL;
    //����������
    const UINT8 table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

    yearH = year / 100;
    yearL = year % 100;

    // ���Ϊ21����,�������100
    if (yearH > 19)yearL += 100;

    // ����������ֻ��1900��֮���
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
* void SecToDate(uint32 usec, calendar_obj* pCalendar) -��UTC����ת��Ϊ����
* ���������USec��UTC����
*						pCalendar����õ����ڷ���ֵָ��
*
***********************************************************************/
void SecToDate(UINT32 usec, RTC_TIME *pCalendar)
{
    UINT16 daycnt = 0, temp_year;;
    UINT32 temp = 0;
    UINT16 temp1 = 0;
    //ƽ����·����ڱ�
    const UINT8 mon_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    usec += 8 * 3600; // ����������ʱ��ת������������
    temp = usec / 86400; //�õ�����(��������Ӧ��)

    if(daycnt != temp) //����һ����
    {
        daycnt = temp;
        temp1 = 1970;	//��1970�꿪ʼ

        while(temp >= 365)
        {
            if(Is_Leap_Year(temp1))//������
            {
                if(temp >= 366)temp -= 366; //�����������
                else
                {
                    temp1++;
                    break;
                }
            }
            else temp -= 365;	 //ƽ��

            temp1++;
        }

        temp_year = temp1;//�õ����
        temp1 = 0;

        while(temp >= 28) //������һ����
        {
            if(Is_Leap_Year(temp_year) && temp1 == 1) //�����ǲ�������/2�·�
            {
                if(temp >= 29)temp -= 29; //�����������
                else break;
            }
            else
            {
                if(temp >= mon_table[temp1])temp -= mon_table[temp1]; //ƽ��
                else break;
            }

            temp1++;
        }

        pCalendar->tm_mon = temp1 + 1;	//�õ��·�
        pCalendar->tm_mday = temp + 1;  	//�õ�����
    }
    else
    {
        temp_year = 1970;
        pCalendar->tm_mon = 1;
        pCalendar->tm_mday = 1;
    }

    temp = usec % 86400;     		//�õ�������
    pCalendar->tm_hour = temp / 3600;     	//Сʱ
    pCalendar->tm_min = (temp % 3600) / 60; 	//����
    pCalendar->tm_sec = (temp % 3600) % 60; 	//����
    pCalendar->tm_wday = GetWeek(temp_year, pCalendar->tm_mon, pCalendar->tm_mday); //��ȡ����

    pCalendar->tm_year = (temp_year > RtcTimeBais) ? temp_year : 0; //�õ����
}

//Ӧ���ź�//
void rx_i2c_ack(void)
{
    //���豸��Ӧ��ACK//
    SDA_L();
    nrf_delay_us(1);
    SCL_H();
    nrf_delay_us(1);
    SCL_L();
    nrf_delay_us(1);
    //��Ӧ��ACK������SDA���øߣ�����Ȩ������//
    SDA_H();
    //nrf_delay_us(1);
}
//��ȡһ���ֽڣ����ض�������//
u8 rx_I2C_ReadByte(void)
{
    u8 i, Temp;

    Temp = 0;

    //�����������ɴ��豸����//
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
  * @brief  ��оƬ��ȡʱ�Ӻ���������
  * @param  pdt	tm�ṹ�������ַ
  * @retval ��
  */
void RX8025T_read(RTC_TIME *pdt)
{
    u8 tmp1;
    u8 TempAck;
    u8 tempaddr;

    //��ʼ�ź�//
    rx_i2c_start();
    //д����
    tempaddr = 0x64;
    TempAck = rx_I2C_WriteByte(tempaddr);
    //��ַ��8λ//
    TempAck = rx_I2C_WriteByte(0);

    //��ʼ�ź�
    rx_i2c_start();
    //��������Ϊ��1//
    tempaddr = 0x65;
    TempAck = rx_I2C_WriteByte(tempaddr);

    GPIO_ConfigDir(3, GPIO_INPUT);
    nrf_delay_us(1);
    //1��ʼ��������
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
    pdt->tm_wday = (tmp1 >> 4) * 10 + (tmp1 & 0x0F); // ����Ϊ0x00ת����7

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
    //���һ���ֽڲ���ӦACK//
    tmp1 = rx_I2C_ReadByte();
    nrf_delay_us(1);
    GPIO_ConfigDir(3, GPIO_OUTPUT);
    pdt->tm_year = (tmp1 >> 4) * 10 + (tmp1 & 0x0F) + RtcTimeBais;

    //i2c_nack();//SDAӦ�þ��Ǹߣ�NACK����SDAΪ��//
    SCL_H();
    nrf_delay_us(1);
    SCL_L();
    nrf_delay_us(1);
    //ֹͣ�ź�//
    rx_i2c_stop();
    nrf_delay_us(1);
    nrf_delay_us(1);

}

/**
  * @brief  ��оƬ����ʱ�Ӻ���������
  * @param  pdt	tm�ṹ�������ַ
  * @retval ��
  */
void RX8025T_write(RTC_TIME *pdt)
{
    u8 TempAck;
    u8 tempaddr;
    u8 year;

    if(pdt->tm_year < 1970 || pdt->tm_year > RtcTimeBais + 99)
        return;

    year = pdt->tm_year - RtcTimeBais;

    //��ʼ�ź�//
    rx_i2c_start();
    //д����
    tempaddr = 0x64;
    TempAck = rx_I2C_WriteByte(tempaddr);
    //��ַ//
    TempAck = rx_I2C_WriteByte(0);
    //��������//
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

    //ֹͣ�ź�//
    rx_i2c_stop();
    nrf_delay_us(1);
    nrf_delay_us(1);
}



void RX8025_test(void)
{
    RTC_TIME WRtcTime;		//����ʱ��

    WRtcTime.tm_sec = 0;
    WRtcTime.tm_min = 6;
    WRtcTime.tm_hour = 15;
    WRtcTime.tm_wday = 5;
    WRtcTime.tm_mday = 11;
    WRtcTime.tm_mon = 1;
    WRtcTime.tm_year = 2022;

    RX8025T_write(&WRtcTime);//��ʼ��ʱ��
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
		//�洢��������,1Сʱ�洢һ�Ρ��ڿ���ʱ��ȡ��
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
