/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-07-24     Tanek        the first version
 * 2018-11-12     Ernest Chen  modify copyright
 */

#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>

#include <board.h>

#define SYSCLK_FREQ_200MHz          200000000    /*校准的200M*/
#define SYSCLK_FREQ_150MHz          150000000    /*校准的200M*/
#define SYSCLK_FREQ_60MHz           60000000 
uint32_t SystemCoreClock = SYSCLK_FREQ_150MHz;

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
#define RT_HEAP_SIZE 1024 * 40    //FINSH 占用4 * 1024
static uint32_t rt_heap[RT_HEAP_SIZE];     // heap default size: 4K(1024 * 4)
RT_WEAK void *rt_heap_begin_get(void)
{
    return rt_heap;
}

RT_WEAK void *rt_heap_end_get(void)
{
    return rt_heap + RT_HEAP_SIZE;
}
#endif

/** System Clock Configuration
*/
void SystemClock_Config(void)
{  
/*	PLL_CONFIG pllConfig;

	XHOSC_InitPad();
	CLK_EnableXhosc(ENABLE) ;
	CLK_EnAllClk();	//使能所有时钟
	CLK_EnPeriClk(RTC);
	CLK_EnableXlosc(ENABLE);
	CLK_SelectRtcClk(XLOSC);
	RTC_Pclk_sel(SELPCLK);
		
	//configure pll output   
	pllConfig.refDiv = PLL_CTRL_PLL_REFDIV_1; 
	pllConfig.fbDiv = PLL_CTRL_PLL_FBDIV_(0x32);  
	pllConfig.outDiv = PLL_CTRL_PLL_OUTDIV_2;  
	pllConfig.refClk = PLL_CTRL_PLL_REF_XHOSC;  
	pllConfig.icpSelA = PLL_CTRL_PLL_ICPSELA_0; 
	pllConfig.icpSelB = PLL_CTRL_PLL_ICPSELB_1; 
	pllConfig.rSel = PLL_CTRL_PLL_RSEL_(4); 
	CLK_ConfigPll(pllConfig);
	CLK_EnablePll(ENABLE);
	
  //配置主时钟
	CLK_ConfigMainClk(PLL);
	CLK_SelectHclk(SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV2) ;  //75000000
	CLK_SelectPclk(SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4) ;		//1875000
//	CLK_EnAllClk(); 
*/

  
      PLL_CONFIG pllConfig;

    CLK_EnableXhosc(ENABLE) ;  //这俩行不能屏蔽，否则会卡在CLK_EnablePll里 原因未知
    CLK_EnableXlosc(ENABLE);

    //configure pll output
    pllConfig.refDiv = PLL_CTRL_PLL_REFDIV_1;  //PLL_CTRL_PLL_REFDIV_1~4  计算M = REFDIV_+1
//	pllConfig.fbDiv = PLL_CTRL_PLL_FBDIV_(0x32);   //PLL_CTRL_PLL_FBDIV_(n), n=0x0~0x7f
    pllConfig.fbDiv = PLL_CTRL_PLL_FBDIV_(0x4B);   //PLL_CTRL_PLL_FBDIV_(n), n=0x0~0x7f

    //Fout=（Fin×N） /（M×O）PLL 输 出 频 率 分 频 比 选 择 ，O=OUTDIV+1  M = REFDIV_+1
    // Fout = 8m * 0x4B /2/2 = 150M
    pllConfig.outDiv = PLL_CTRL_PLL_OUTDIV_2;  //PLL_CTRL_PLL_OUTDIV_1~4
    pllConfig.refClk = PLL_CTRL_PLL_REF_XHOSC;  //PLL_输入时钟源：外部晶振
    pllConfig.icpSelA = PLL_CTRL_PLL_ICPSELA_0; //PLL_CTRL_PLL_ICPSELA_0~1  CP 电流选择， 推荐用 1
    pllConfig.icpSelB = PLL_CTRL_PLL_ICPSELB_1; //PLL_CTRL_PLL_ICPSELB_0~1  CP 电流选择， 推荐用 0；
    pllConfig.rSel = PLL_CTRL_PLL_RSEL_(4);   //PLL_CTRL_PLL_RSEL_(n) 滤波电阻大小选择？
    CLK_ConfigPll(pllConfig) ;
    CLK_EnablePll(ENABLE);

    //configure main_clk
    CLK_ConfigMainClk(PLL);
    CLK_SelectHclk(SYSCLK_CTRL_HCLK_SEL_SYSCLK_DIV2) ; //HCLK = 150/2 为系统时钟2分频
    CLK_SelectPclk(SYSCLK_CTRL_PCLK_SEL_HCLK_DIV4) ;  //PCLK = 150/4 = 37.5M 为hclk4分频
    CLK_EnAllClk();
	
	  //初始化SysTick
  SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND / 2);
  NVIC_SetPriority(SysTick_IRQn, 0);
	
}

/**
 * This function will initial your board.
 */
void rt_hw_board_init()
{	
	SystemClock_Config();

    /* Call components board initial (use INIT_BOARD_EXPORT()) */
#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init(rt_heap_begin_get(), rt_heap_end_get());
#endif

#ifdef RT_USING_CONSOLE
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif

#ifdef BSP_USING_SDRAM
    rt_system_heap_init((void *)EXT_SDRAM_BEGIN, (void *)EXT_SDRAM_END);
#else
//    rt_system_heap_init((void *)HEAP_BEGIN, (void *)HEAP_END);
#endif
}

void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler */
}


