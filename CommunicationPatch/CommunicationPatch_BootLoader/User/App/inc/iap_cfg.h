/*******************************************************************************
// Copyright(c)2017, 天津新众聚联信息科技股份有限公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language：C)
// Date   : 2017-04-01
// Author : 刘锋
// Functiom:项目工程的主要配置文件
*******************************************************************************/

#ifndef IAP_CFG_H
#define IAP_TCFG_H




/******************************************************************************
//宏定义
*******************************************************************************/

#define BOOTLOADER_SIZE         0x2000                      //Bootloader引导程序的空间，即应用程序实际运行地址偏向量
/*
 * 配置地址相关信息
 */
#define ApplicationAddress      (0x8000000 + BOOTLOADER_SIZE) //定义用户应用程序的起始地址

#define PAGE_SIZE                (0x400)                      //定义FLASH页大小：1 Kbytes(小容量和中容量是1K)
#define FLASH_SIZE               (0x20000)                    //定义FLASH空间大小：128 KBytes


// 临时存放应用程序
#define ApplicationTempAddress  (0x8000000 + ((FLASH_SIZE - (PAGE_SIZE * 2) + BOOTLOADER_SIZE) / 2))  //定义用户应用程序的起始地址0x8004000
// 临时存放应用程序的空间大小
#define ApplicationTempSize     ((FLASH_SIZE - BOOTLOADER_SIZE - (PAGE_SIZE * 2)) / 2)

/*
 * 配置Ymodem通信协议超时间隔和最大错误次数信息(或降低系统主频)
 */
#define NAK_TIMEOUT             (0x200000)
#define MAX_ERRORS              (5)

#define WAIT_UPGRADE_TIMEOUT_MAX_COUNT      30              //等待升级发送字符‘C’超过30次就自动跳到应用程序

/*
 * 配置触发按键
 */
//USE_BUTTON-------------------------------------------------- PB2
//#define USE_BUTTON_GPIO_PORT     GPIOA                        //定义USE_BUTTON使用的的GPIO端口
//#define USE_BUTTON_GPIO_CLK      RCC_APB2Periph_GPIOA         //定义USE_BUTTON端口的时钟
//#define USE_BUTTON_GPIO_PIN      GPIO_Pin_10                   //定义USE_BUTTON的GPIO引脚号
//
//#define GetButtonStatus()        GPIO_ReadInputDataBit(USE_BUTTON_GPIO_PORT, USE_BUTTON_GPIO_PIN)
                                                              //获取按键的状态


/*
 * 配置使用的串口
 */
#define USE_USART_BAUDRATE       9600                       //定义USE_USART通信波特率
#define USE_USART                USART3                       //定义USE_USART使用的串口号
#define USE_USART_CLK            RCC_APB1Periph_USART3        //定义USE_USART使用的模块时钟

//USE_USART_TX ----------------------------------------------- (USART1_TX：PA9)
#define USE_USART_TX_GPIO_CLK    RCC_APB2Periph_GPIOB         //定义USE_USART_TX端口的时钟
#define USE_USART_TX_GPIO_PORT   GPIOB                        //定义USE_USART_TX占用的GPIO端口
#define USE_USART_TX_GPIO_PIN    GPIO_Pin_10                   //定义USE_USART_TX的GPIO引脚号

//USE_USART_RX ----------------------------------------------- (USART1_RX：PA10)
#define USE_USART_RX_GPIO_CLK    RCC_APB2Periph_GPIOB         //定义USE_USART_RX端口的时钟
#define USE_USART_RX_GPIO_PORT   GPIOB                        //定义USE_USART_RX占用的GPIO端口
#define USE_USART_RX_GPIO_PIN    GPIO_Pin_11                  //定义USE_USART_RX的GPIO引脚号

#define GetButtonStatus()        GPIO_ReadInputDataBit(USE_USART_RX_GPIO_PORT, USE_USART_RX_GPIO_PIN)

/******************************************************************************
//变量定义/声明
*******************************************************************************/
// 升级文件已经写入Flash内的标志
extern u8  g_ProgramFlashFileRecFlag;



/******************************************************************************
//函数声明
*******************************************************************************/





#endif
