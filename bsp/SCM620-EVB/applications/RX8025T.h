#ifndef _RX8025_H_
#define _RX8025_H_

#define RtcTimeBais 2000 //千年

typedef struct
{
    UINT8 tm_sec;
    UINT8 tm_min;
    UINT8 tm_hour;
    UINT8 tm_wday;
    UINT8 tm_mday;
    UINT8 tm_mon;
    UINT16 tm_year;
} RTC_TIME;



void RX8025T_Init(void);
void RX8025T_read(RTC_TIME *pdt);
void RX8025T_write(RTC_TIME *pdt);


UINT8  Is_Leap_Year(UINT16 year);
UINT32 DateToUSec(RTC_TIME time);
UINT8  GetWeek(UINT16 year, UINT8 month, UINT8 day);
void   SecToDate(UINT32 usec, RTC_TIME *pCalendar);

extern RTC_TIME RtcTime;
extern uint16_t  timePC[6];

#endif
