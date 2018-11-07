
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:��Ŀ���̵�MCU�����ʼ��Դ�ļ�����Ҫ��������ʹ���˵�����ĳ�ʼ��(����)��
    �Լ�һЩ�򵥵ĳ�ʼ��ԭ������
*******************************************************************************/


/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/



/******************************************************************************
//��������
*******************************************************************************/
//��������: ��Ƭ����ʼ������
void  MCU_Init(void);

//��������: ��Ƭ��ʱ�����ú���
void RCC_Configuration(void);

//��������: �ر�JTAG���ܺ���
void Close_JTAG_Function(void);

//��������: ��Ƭ��ʱ�Ӳ��Ժ���
void RCC_Test(void);





/******************************************************************************
//��������
*******************************************************************************/
//********************************************************
//��������: MCU_Init
//��������: ��Ƭ����ʼ������
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void  MCU_Init(void)
{ 
    SygProgramUpgradeFlag = FALSE;
    Internal_Flash_Upgrade_StoreIn();   //�����������ı�־�洢��Ƭ��Flash
    
//    //�������ʹ���ⲿ������Ҫ���ò�����ϵͳʱ��
//    RCC_Configuration();
    
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);    //����ӳ���ж�������ַ

//    Close_JTAG_Function();              //��JTAG�ڹر�
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//ʹ�����ȼ�����2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //����AFIOʱ��

    Debug_Init();                       //���Կڳ�ʼ��
    
    Server_Comm_Init();                     //�������ͨ����صĳ�ʼ��
    
    Device_Comm_Init();                //���豸��ͨ����صĳ�ʼ��

//    SDcard_Comm_Init();                 //SD��ͨ����صĳ�ʼ��
    
    SysTick_Init_Delay(DELAY_MS);       //��ʼ��SysTick�жϼ��Ϊ1ms
    
#if (SERVER_PRINTF_EN)
printf("------------------��Ƭ�����ӿڳ�ʼ�����------------------\r\n");
#endif   
  
}

//********************************************************
//��������: RCC_Configuration
//��������: ��Ƭ��ʱ�����ú���
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void RCC_Configuration(void)
{
    //ʹ��HSI
    RCC->CR |= (uint32_t)0x00000001;
    
    //����HSIΪϵͳʱ��Դ
    RCC->CFGR |= (uint32_t)RCC_CFGR_PLLSRC_HSI_Div2;
    //PLLCLK=8/2*12=48M
    RCC->CFGR |= (uint32_t)RCC_CFGR_PLLMULL12;
    
    //HCLK = SYSCLK/2 
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHBʱ��Ϊϵͳʱ��SYSCLK
//    RCC_PCLK2Config(RCC_HCLK_Div1);  //APB2ʱ��ΪHCLK
    RCC_PCLK1Config(RCC_HCLK_Div2);  //APB1ʱ��ΪHCLK/2������HCLKΪAHBʱ��     

    //ʹ��PLLʱ��
    RCC->CR |= RCC_CR_PLLON;
    
    //�ȴ�PLLʱ��׼����
    while((RCC->CR & RCC_CR_PLLRDY) == 0);
    
    //��תPLLʱ��Ϊϵͳʱ��
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
    
    //�ȴ�PLLʱ��׼����
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); 
}

//********************************************************
//��������: Close_JTAG_Function
//��������: �ر�JTAG���ܺ���
//��    ��: ��
//��    ��: ��
//��    ע: ��JTAG������Ϊ��ͨIO��
//********************************************************
void Close_JTAG_Function(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //����AFIOʱ��
    
    // �ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_JTAGDisable ��JTAG-DP ���� + SW-DP ʹ�� 
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    // �ı�ָ���ܽŵ�ӳ�� GPIO_Remap_SWJ_Disable SWJ ��ȫ���ã�JTAG+SW-DP��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  
    // �ı�ָ���ܽŵ�ӳ�� GPIO_Remap_PD01 ��OSC_IN��OSC_OUT ���� + PD0��PD1 ʹ�� 
    GPIO_PinRemapConfig(GPIO_Remap_PD01 , ENABLE);
}

//********************************************************
//��������: RCC_Test
//��������: ��Ƭ��ʱ�Ӳ��Ժ���
//��    ��: ��
//��    ��: ��
//��    ע: ��PA8�����ϵͳʱ��
//********************************************************
void RCC_Test(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    /* Output HSE clock on MCO pin */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    RCC_MCOConfig(RCC_MCO_SYSCLK); 
}

//////////////////////////////END OF C�ļ�////////////////////////////////////
