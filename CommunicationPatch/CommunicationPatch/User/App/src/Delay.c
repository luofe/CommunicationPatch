
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
//ϵͳ��ȷ��ʱ�ļ�
*******************************************************************************/


/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/
//��ʱ����ֵ
__IO u32 g_ms_Timing_Count = 0;	//ms��ʱ

u32 s_Timing_Count;	//s��ʱ


/******************************************************************************
//��������
*******************************************************************************/
//����:	ϵͳ�δ���ʱ��ʼ��
void SysTick_Init_Delay(__IO uint32_t delay_l);

//����:	ϵͳ��ȷms��ʱ����
void Delay_ms(__IO uint16_t nTime);

//����:	��ȷ��us��ʱ����
void Delay_us(__IO uint16_t nTime);



/******************************************************************************
//��������
*******************************************************************************/
/**********************************
//����:	SysTick_Init_Delay
//����:	ϵͳ�δ���ʱ��ʼ��
//��ڲ���:	��
//���ڲ���:	��
**********************************/
void SysTick_Init_Delay(__IO uint32_t delay_l)
{
	if (SysTick_Config(SystemCoreClock / delay_l))	//Ϊ1ms
  	{ 
    	while (1);	//�ȴ��ȶ�
  	}
	NVIC_SetPriority(SysTick_IRQn, 0x0);	//����SysTick�ж����ȼ�Ϊ0
}

/**********************************
//����:	Delay_ms
//����:	ϵͳ��ȷms��ʱ����
//��ڲ���:	��
//���ڲ���:	��
//��ע:    
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
    if(nTime > 32767)                                             //�������ˣ����ʱ����Ϊ32767ms
    {
        nTime = 32767;
    }
    //����TIM��ʱ��Ƶ�� TIMnCLK= SystemCoreClock / (TIM_Prescaler + 1) = 0.2 MHZ
    //�������Ƶ�� = TIMnCLK / (TIM_Period + 1)
    RCC_APB1PeriphClockCmd(DELAY_TIMER_CLK, ENABLE);                //ʹ��TIMxģ��ʱ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷ�Ƶ����Ϊ1����1��Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Down; //��������Ϊ: ���¼���
    TIM_TimeBaseStructure.TIM_Prescaler     = (u16)(SystemCoreClock / 2000) - 1;
                                                                    //����Ԥ��Ƶֵ,��ƵΪ1KHz��1ms
    TIM_TimeBaseStructure.TIM_Period        = nTime * 2 - 1;       //�����Զ���װ��ֵ
    TIM_TimeBaseInit(DELAY_TIMER, &TIM_TimeBaseStructure);          //��ʼ��TIMx
    TIM_GenerateEvent(DELAY_TIMER, TIM_EventSource_Update);         //����һ�θ����¼�
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update);                    //��������жϱ�־
    TIM_Cmd(DELAY_TIMER, ENABLE);                                   //ʹ��TIMx
    
    //
    //�ȴ���ʱ��
    //
    while(TIM_GetFlagStatus(DELAY_TIMER, TIM_FLAG_Update) == RESET);//�ȴ�TIMx���±�־
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update);                    //��������жϱ�־
}

/**********************************
//����:	Delay_us
//����:	��ȷ��us��ʱ����
//��ڲ���:	��
//���ڲ���:	��
**********************************/
void Delay_us(__IO uint16_t nTime)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(DELAY_TIMER_CLK, ENABLE);                //ʹ��TIMxģ��ʱ��
    //����TIM��ʱ��Ƶ�� TIMnCLK= SystemCoreClock / (TIM_Prescaler + 1) = 1 MHZ
    //�������Ƶ�� = TIMnCLK / (TIM_Period + 1)
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷ�Ƶ����Ϊ0������Ƶ
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Down; //��������Ϊ: ���¼���
    TIM_TimeBaseStructure.TIM_Prescaler     = (u16)(SystemCoreClock / 1000000) - 1;
                                                                    //����Ԥ��Ƶֵ,��ƵΪ1MHz��1us
    TIM_TimeBaseStructure.TIM_Period        = nTime;                //�����Զ���װ��ֵ
    TIM_TimeBaseInit(DELAY_TIMER, &TIM_TimeBaseStructure);          //��ʼ��TIMx
    TIM_GenerateEvent(DELAY_TIMER, TIM_EventSource_Update);         //����һ�θ����¼�
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update);                    //��������жϱ�־
    TIM_Cmd(DELAY_TIMER, ENABLE);                                   //ʹ��TIMx
    
    //
    //�ȴ���ʱ��
    //
    while(TIM_GetFlagStatus(DELAY_TIMER, TIM_FLAG_Update) == RESET);//�ȴ�TIMx���±�־
    TIM_ClearFlag(DELAY_TIMER, TIM_FLAG_Update);                    //��������жϱ�־
}


//////////////////////////////END OF C�ļ�////////////////////////////////////
