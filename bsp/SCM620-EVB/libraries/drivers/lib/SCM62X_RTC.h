
#ifndef _SCM62X_RTC_H
#define _SCM62X_RTC_H


#ifdef __cplusplus
extern "C" {
#endif


//interrupt type
#define RTC_PERIOD_INTR  0x20
#define RTC_ALARM_INTR   0x10

//period interrupt type
#define RTC_PINT_SECOND  1
#define RTC_PINT_MINUTE  2
#define RTC_PINT_HOUR    3
#define RTC_PINT_DAY     4
#define RTC_PINT_MONTH   5

//for RTC PCLK Sel
#define SELPCLK (UINT8)1
#define VBAT32K (UINT8)2
/******************************************************************************/
/*                              全局数据类型声明                              */
/******************************************************************************/
/*<UNION+>*********************************************************************/
/* 结构: _DATETIMETYPEDEF                                                     */
/* 注释: rtc时间(或闹钟)寄存器timer0和timer1与其相应的位段的联合体            */
/*<UNION->*********************************************************************/
typedef union _DateTimeTypeDef
{
    uint32_t timer[2];
    struct {
        uint32_t secondU:       4;  /* 秒的个位              timer0: bit03~00 */
        uint32_t secondD:       3;  /* 秒的十位              timer0: bit06~04 */
        uint32_t reserved07:    1;  /* 保留位                timer0: bit07    */
        uint32_t minuteU:       4;  /* 分的个位              timer0: bit11~08 */
        uint32_t minuteD:       3;  /* 分的十位              timer0: bit14~12 */
        uint32_t reserved15:    1;  /* 保留位                timer0: bit15    */
        uint32_t hourU:         4;  /* 时的个位              timer0: bit19~16 */
        uint32_t hourD:         2;  /* 时的十位              timer0: bit21~20 */
        uint32_t reserved22_23: 2;  /* 保留位                timer0: bit23~22 */
        uint32_t weekDay:       3;  /* 0~6分别对应周日,周一...周六   bit26~24 */
        uint32_t reserved27_31: 5;  /* 保留位                timer0: bit31~27 */ 
 
        uint32_t dayU:          4;  /* 日的个位              timer1: bit03~00 */
        uint32_t dayD:          2;  /* 日的十位              timer1: bit05~04 */
        uint32_t reserved06_07: 2;  /* 保留位                timer1: bit07~06 */
        uint32_t monthU:        4;  /* 月的个位              timer1: bit11~08 */
        uint32_t monthD:        1;  /* 月的十位              timer1: bit12    */
        uint32_t reserved13_15: 3;  /* 保留位                timer1: bit15~13 */
        uint32_t yearU:         4;  /* 年的个位              timer1: bit19~16 */
        uint32_t yearD:         4;  /* 年的十位              timer1: bit23~20 */
        uint32_t reserved24_31: 8;  /* 保留位                timer1: bit31~24 */
    } b;

}DateTimeTypeDef;

/******************************************************************************/
/*                                全局变量声明                                */
/******************************************************************************/


/******************************************************************************/
/*                                外部引用定义                                */
/******************************************************************************/


/******************************************************************************/
/*                                全局函数原型                                */
/******************************************************************************/
void RTC_Enable(void);
void RTC_Disable(void);

void RTC_GetTime(DateTimeTypeDef *dateTime);
void RTC_SetTime(DateTimeTypeDef dateTime);

void RTC_EnablePeriodIntr(uint8_t intrType);
void RTC_DisablePeriodIntr(void);

void RTC_EnableAlarmIntr(DateTimeTypeDef alarmDateTime);
void RTC_DisableAlarmIntr(void);

void RTC_ClearIntr(uint8_t intrType);
uint8_t RTC_GetFlagStatus(uint8_t flag);
void RTC_Pclk_sel(uint8_t flag);
void RTC_Module_Reset(void);
#ifdef __cplusplus
    }
#endif

#endif
                                                                                                
