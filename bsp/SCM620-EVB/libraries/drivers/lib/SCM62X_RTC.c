/*<FH+>************************************************************************/
/*                                                                            */
/* 版权所有: 北京智芯微电子                                                   */
/*                                                                            */
/* 文件名称: SCM62X_RTC.c                                                     */
/* 内容摘要: RTC模块基础函数定义                                              */
/* 其它说明: **                                                               */
/* 当前版本: V1.0                                                             */
/* 作    者: SCM                                                              */
/* 完成日期: 2019-03-20                                                       */
/* 修改记录:                                                                  */
/*    修改日期          版本号        修改人        修改内容                  */
/* -------------------------------------------------------------------------- */
/*    2019-03-20        V1.0          SCM        创建文件                     */
/*<FH->************************************************************************/


/******************************************************************************/
/*               #include（依次为标准库头文件、非标准库头文件）               */
/******************************************************************************/
#include "SCM62X.h"

/******************************************************************************/
/*                                外部引用定义                                */
/******************************************************************************/


/******************************************************************************/
/*                                  全局变量                                  */
/******************************************************************************/


/******************************************************************************/
/*                                 函数的实现                                 */
/******************************************************************************/
/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_Enable                                                       */
/* 功能描述: rtc使能                                                          */
/* 输入参数: 无                                                               */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-05        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
void RTC_Enable(void) 
{
    SGCC_RTC_P->Control |= RTC_CTL_RTC_EN;
}

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_Disable                                                      */
/* 功能描述: RTC禁止                                                          */
/* 输入参数: 无                                                               */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
void RTC_Disable(void) 
{
    SGCC_RTC_P->Control &= ~RTC_CTL_RTC_EN;
}

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_GetTime                                                      */
/* 功能描述: RTC中读取时间信息                                                */
/* 输入参数: dateTime --- 指向时间信息存储结构的指针                          */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
void RTC_GetTime(DateTimeTypeDef *dateTime) 
{
    //read current time register
    dateTime->timer[0] = SGCC_RTC_P->CurTime0;
    dateTime->timer[1] = SGCC_RTC_P->CurTime1;
}

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_SetTime                                                      */
/* 功能描述: 设置RTC时间信息                                                  */
/* 输入参数: dateTime --- 时间信息值                                          */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
void RTC_SetTime(DateTimeTypeDef dateTime) 
{
		
    /* clear RTC_EN bit */
    SGCC_RTC_P->Control &= ~RTC_CTL_RTC_EN;
    
    /* check and read the RTC_EN_STATUS bit until 0 */
    while(SGCC_RTC_P->Status & RTC_EN_STATUS);
    
    /* clear all RTC interrupts */
    SGCC_RTC_P->Status = RTC_ALARM_INT_STATUS|RTC_PRD_INT_STATUS;
    
    /* set new time to register Time0 */
    SGCC_RTC_P->Time0 = dateTime.timer[0];
    
    /* set new time to register Time1 */
    SGCC_RTC_P->Time1 = dateTime.timer[1];
    
    /* set RTC_EN bit */
    SGCC_RTC_P->Control |= RTC_CTL_RTC_EN;

    /* wait until RTC_EN_STATUS bit is changed to '1' */
    while(!(SGCC_RTC_P->Status & RTC_EN_STATUS));
}

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_EnablePeriodIntr                                             */
/* 功能描述: 使能RTC时间中断                                                  */
/* 输入参数: intrType --- 使能的时间中断选择                                  */
/*           可设置的值如下:(它们之间可以'|')                                 */
/*                   RTC_PINT_SECOND --- 使能秒周期中断                       */
/*                   RTC_PINT_MINUTE --- 使能分周期中断                       */
/*                   RTC_PINT_HOUR   --- 使能时周期中断                       */
/*                   RTC_PINT_DAY    --- 使能日周期中断                       */
/*                   RTC_PINT_MONTH  --- 使能月周期中断                       */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/

void RTC_EnablePeriodIntr(uint8_t intrType) 
{
    uint32_t temp = 0;

    temp  = SGCC_RTC_P->Control ;  
    temp |= RTC_CTL_PRD_INT_EN;   //enable period interrupt

    temp &= ~RTC_CTL_PERINT_SEL;

    switch(intrType)
    {
        case RTC_PINT_SECOND:
        temp |= RTC_CTL_PERINT_SEL_SECOND;  //select second interrupt
        break;
    
        case RTC_PINT_MINUTE:
        temp |= RTC_CTL_PERINT_SEL_MINUTE;  //select minute interrupt
        break;
    
        case RTC_PINT_HOUR:
        temp |= RTC_CTL_PERINT_SEL_HOUR;  //select hour interrupt
        break;
    
        case RTC_PINT_DAY:
        temp |= RTC_CTL_PERINT_SEL_DAY;  //select day interrupt
        break;
    
        case RTC_PINT_MONTH:
        temp |= RTC_CTL_PERINT_SEL_MONTH;  //select month interrupt
        break;
    
        default:
        break;
    } 
    SGCC_RTC_P->Control = temp;  
}

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_DisablePeriodIntr                                            */
/* 功能描述: RTC时间周期中断禁止                                              */
/* 输入参数: 无                                                               */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
void RTC_DisablePeriodIntr(void) 
{
    SGCC_RTC_P->Control &= ~(RTC_CTL_PERINT_SEL | RTC_CTL_PRD_INT_EN);
}

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_EnableAlarmIntr                                              */
/* 功能描述: 设置并使能闹钟中断                                               */
/* 输入参数: alarmDateTime --- 闹钟时间信息值                                 */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
void RTC_EnableAlarmIntr(DateTimeTypeDef alarmDateTime) 
{
    // uint32_t temp = 0;
    
    /* disable ALARM                                                          */
    SGCC_RTC_P->Control &= ~RTC_CTL_ALARM_EN;
    /* disable alarm interrupt                                                */
    SGCC_RTC_P->Control &= ~RTC_CTL_ALARM_INT_EN;
    
    /* set new time to alarm registers Timer0                                 */
    SGCC_RTC_P->AlarmTime0 = alarmDateTime.timer[0];

    /* set new time to alarm registers Timer1                                 */
    SGCC_RTC_P->AlarmTime1 = alarmDateTime.timer[1];

    /* enable ALARM                                                           */
    SGCC_RTC_P->Control |= RTC_CTL_ALARM_EN;

    /* enable alarm interrupt                                                 */
    SGCC_RTC_P->Control |= RTC_CTL_ALARM_INT_EN;
}

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_DisableAlarmIntr                                             */
/* 功能描述: 禁止RTC闹钟中断                                                  */
/* 输入参数: 无                                                               */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
void RTC_DisableAlarmIntr(void) 
{
    SGCC_RTC_P->Control &= ~RTC_CTL_ALARM_INT_EN;
}

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_ClearIntr                                                    */
/* 功能描述: 清除中断标志                                                     */
/* 输入参数: intrType --- 中断类型                                            */
/*                        中断类型包括: RTC_PERIOD_INTR、RTC_ALARM_INTR       */
/* 输出参数: 无                                                               */
/* 返 回 值: 无                                                               */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
void RTC_ClearIntr(uint8_t intrType) 
{
    SGCC_RTC_P->Status = intrType;
} 

/*<FUNC+>**********************************************************************/
/* 函数名称: RTC_GetFlagStatus                                                */
/* 功能描述: 获取状态信息,(或,判断是否与输入的状态一致                        */
/* 输入参数: flag --- 状态信息                                                */
/*              RTC_PRD_INT_STATUS     当前是否产生了周期中断                 */
/*              RTC_ALARM_INT_STATUS   当前是否产生了闹钟中断                 */
/*              RTC_EN_STATUS          当前RTC是否使能                        */
/* 输出参数: 无                                                               */
/* 返 回 值: 1---产生了此中断或此项使能                                       */
/*           0---未产生此中断或此项未使能                                     */
/* 操作流程:                                                                  */
/* 其它说明: 无                                                               */
/* 修改记录:                                                                  */
/* -------------------------------------------------------------------------- */
/*    2018-01-08        V1.00         SCM           创建函数                  */
/*<FUNC->**********************************************************************/
uint8_t RTC_GetFlagStatus(uint8_t flag)
{
    if(SGCC_RTC_P->Status & flag)
    {
        return 1;
    }
    else
    {
        return 0;
    } 
}

//flag:   SELPCLK OR VBAT32K
void RTC_Pclk_sel(UINT8 flag)
{
	if(flag==SELPCLK)
		SGCC_RTC_P->RTC_CLKRST_cfg |=1<<12;
	else if (flag==VBAT32K)
		SGCC_RTC_P->RTC_CLKRST_cfg &=~(1<<12);
}
void RTC_Module_Reset()
{
	SGCC_RTC_P->BDRST =1;
}
