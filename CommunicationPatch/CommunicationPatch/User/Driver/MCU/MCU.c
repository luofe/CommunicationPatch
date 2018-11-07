
/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
// Functiom:项目工程的MCU外设初始化源文件，主要包括各个使用了的外设的初始化(调用)，
    以及一些简单的初始化原函数。
*******************************************************************************/


/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//变量定义
*******************************************************************************/



/******************************************************************************
//函数声明
*******************************************************************************/
//函数功能: 单片机初始化函数
void  MCU_Init(void);

//函数功能: 单片机时钟配置函数
void RCC_Configuration(void);

//函数功能: 关闭JTAG功能函数
void Close_JTAG_Function(void);

//函数功能: 单片机时钟测试函数
void RCC_Test(void);





/******************************************************************************
//函数定义
*******************************************************************************/
//********************************************************
//函数名称: MCU_Init
//函数功能: 单片机初始化函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void  MCU_Init(void)
{ 
    SygProgramUpgradeFlag = FALSE;
    Internal_Flash_Upgrade_StoreIn();   //将程序升级的标志存储到片内Flash
    
//    //如果不是使用外部晶振，需要启用并配置系统时钟
//    RCC_Configuration();
    
//    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x2000);    //重新映射中断向量地址

//    Close_JTAG_Function();              //将JTAG口关闭
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//使用优先级分组2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //开启AFIO时钟

    Debug_Init();                       //调试口初始化
    
    Server_Comm_Init();                     //与服务器通信相关的初始化
    
    Device_Comm_Init();                //与设备端通信相关的初始化

//    SDcard_Comm_Init();                 //SD卡通信相关的初始化
    
    SysTick_Init_Delay(DELAY_MS);       //初始化SysTick中断间隔为1ms
    
#if (SERVER_PRINTF_EN)
printf("------------------单片机各接口初始化完毕------------------\r\n");
#endif   
  
}

//********************************************************
//函数名称: RCC_Configuration
//函数功能: 单片机时钟配置函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void RCC_Configuration(void)
{
    //使能HSI
    RCC->CR |= (uint32_t)0x00000001;
    
    //设置HSI为系统时钟源
    RCC->CFGR |= (uint32_t)RCC_CFGR_PLLSRC_HSI_Div2;
    //PLLCLK=8/2*12=48M
    RCC->CFGR |= (uint32_t)RCC_CFGR_PLLMULL12;
    
    //HCLK = SYSCLK/2 
    RCC_HCLKConfig(RCC_SYSCLK_Div1); //AHB时钟为系统时钟SYSCLK
//    RCC_PCLK2Config(RCC_HCLK_Div1);  //APB2时钟为HCLK
    RCC_PCLK1Config(RCC_HCLK_Div2);  //APB1时钟为HCLK/2，其中HCLK为AHB时钟     

    //使能PLL时钟
    RCC->CR |= RCC_CR_PLLON;
    
    //等待PLL时钟准备好
    while((RCC->CR & RCC_CR_PLLRDY) == 0);
    
    //旋转PLL时钟为系统时钟
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
    
    //等待PLL时钟准备好
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); 
}

//********************************************************
//函数名称: Close_JTAG_Function
//函数功能: 关闭JTAG功能函数
//输    入: 无
//输    出: 无
//备    注: 将JTAG口配置为普通IO口
//********************************************************
void Close_JTAG_Function(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //开启AFIO时钟
    
    // 改变指定管脚的映射 GPIO_Remap_SWJ_JTAGDisable ，JTAG-DP 禁用 + SW-DP 使能 
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
    // 改变指定管脚的映射 GPIO_Remap_SWJ_Disable SWJ 完全禁用（JTAG+SW-DP）
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);  
    // 改变指定管脚的映射 GPIO_Remap_PD01 ，OSC_IN和OSC_OUT 禁用 + PD0和PD1 使能 
    GPIO_PinRemapConfig(GPIO_Remap_PD01 , ENABLE);
}

//********************************************************
//函数名称: RCC_Test
//函数功能: 单片机时钟测试函数
//输    入: 无
//输    出: 无
//备    注: 在PA8上输出系统时钟
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

//////////////////////////////END OF C文件////////////////////////////////////
