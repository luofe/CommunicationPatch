
/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
//系统精确延时文件
*******************************************************************************/


/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//变量定义
*******************************************************************************/
//延时计数值
__IO u32 g_ms_Timing_Count = 0;	//ms计时

u32 s_Timing_Count;	//s计时


/******************************************************************************
//函数声明
*******************************************************************************/
//功能:	系统滴答延时初始化
void SysTick_Init_Delay(__IO uint32_t delay_l);

//功能:	系统精确ms延时函数
void Delay_ms(__IO uint16_t nTime);

//功能:	精确的us延时函数
void Delay_us(__IO uint16_t nTime);



/******************************************************************************
//函数定义
*******************************************************************************/
/**********************************
//名称:	SysTick_Init_Delay
//功能:	系统滴答延时初始化
//入口参数:	无
//出口参数:	无
**********************************/
void SysTick_Init_Delay(__IO uint32_t delay_l)
{
	if (SysTick_Config(SystemCoreClock / delay_l))	//为1ms
  	{ 
    	while (1);	//等待稳定
  	}
	NVIC_SetPriority(SysTick_IRQn, 0x0);	//设置SysTick中断优先级为0
}

/**********************************
//名称:	Delay_ms
//功能:	系统精确ms延时函数
//入口参数:	无
//出口参数:	无
//备注:    
**********************************/
void Delay_ms(__IO uint16_t nTime)
{
//	uint16_t x;
//	for(x = nTime;x > 0;x--)
//	{
//		g_ms_Timing_Count = 0;
//		while(!g_ms_Timing_Count);
//	}
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    if(nTime > 32767)                                             //参数过滤，最大定时长度为32767ms
    {
        nTime = 32767;
    }
    //计算TIM的时钟频率 TIMnCLK= SystemCoreClock / (TIM_Prescaler + 1) = 0.2 MHZ
    //计算输出频率 = TIMnCLK / (TIM_Period + 1)
    RCC_APB1PeriphClockCmd(DELAY_TIMER_CLK, ENABLE);                //使能TIMx模块时钟
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //定义时钟分频因子为1，即1分频
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Down; //计数方向为: 向下计数
    TIM_TimeBaseStructure.TIM_Prescaler     = (u16)(SystemCoreClock / 2000) - 1;
                                                                    //设置预分频值,分频为1KHz，1ms
    TIM_TimeBaseStructure.TIM_Period        = nTime * 2 - 1;       //设置自动重装载值
    TIM_TimeBaseInit(DELAY_TIMER, &TIM_TimeBaseStructure);          //初始化TIMx
    TIM_GenerateEvent(DELAY_TIMER, TIM_EventSource_Update);         //产生一次更新事件
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update);                    //清除更新中断标志
    TIM_Cmd(DELAY_TIMER, ENABLE);                                   //使能TIMx
    
    //
    //等待定时到
    //
    while(TIM_GetFlagStatus(DELAY_TIMER, TIM_FLAG_Update) == RESET);//等待TIMx更新标志
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update);                    //清除更新中断标志
}

/**********************************
//名称:	Delay_us
//功能:	精确的us延时函数
//入口参数:	无
//出口参数:	无
**********************************/
void Delay_us(__IO uint16_t nTime)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(DELAY_TIMER_CLK, ENABLE);                //使能TIMx模块时钟
    //计算TIM的时钟频率 TIMnCLK= SystemCoreClock / (TIM_Prescaler + 1) = 1 MHZ
    //计算输出频率 = TIMnCLK / (TIM_Period + 1)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //定义时钟分频因子为0，不分频
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Down; //计数方向为: 向下计数
    TIM_TimeBaseStructure.TIM_Prescaler     = (u16)(SystemCoreClock / 1000000) - 1;
                                                                    //设置预分频值,分频为1MHz，1us
    TIM_TimeBaseStructure.TIM_Period        = nTime;                //设置自动重装载值
    TIM_TimeBaseInit(DELAY_TIMER, &TIM_TimeBaseStructure);          //初始化TIMx
    TIM_GenerateEvent(DELAY_TIMER, TIM_EventSource_Update);         //产生一次更新事件
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update);                    //清除更新中断标志
    TIM_Cmd(DELAY_TIMER, ENABLE);                                   //使能TIMx
    
    //
    //等待定时到
    //
    while(TIM_GetFlagStatus(DELAY_TIMER, TIM_FLAG_Update) == RESET);//等待TIMx更新标志
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update);                    //清除更新中断标志
}


//////////////////////////////END OF C文件////////////////////////////////////
