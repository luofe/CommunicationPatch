
/*
  bsp.h  单片机初始化函数定义头文件
*/
#ifndef MCU_H
#define MCU_H

/******************************************************************************
//宏定义
*******************************************************************************/


/******************************************************************************
//变量定义/声明
*******************************************************************************/
// USB使能控制口
#define USB_ENABLE_CTR_GPIO_PIN             GPIO_Pin_12         //定义的GPIO引脚号
#define USB_ENABLE_CTR_GPIO_PORT            GPIOB               //定义使用的GPIO端口
#define USB_ENABLE_CTR_GPIO_CLK             RCC_APB2Periph_GPIOB//定义端口的时钟
#define USB_ENABLE_CTR_GPIO_SOURCE          GPIO_PinSource12    //定义引脚号
// USB使能控制
#define SetUSBEnable()                      GPIO_SetBits(USB_ENABLE_CTR_GPIO_PORT, USB_ENABLE_CTR_GPIO_PIN)
#define SetUSBDisable()                     GPIO_ResetBits(USB_ENABLE_CTR_GPIO_PORT, USB_ENABLE_CTR_GPIO_PIN)


/******************************************************************************
//函数声明
*******************************************************************************/

void MCU_Init(void);             //板级初始化函数

//函数功能: 单片机时钟测试函数
void RCC_Test(void);







#endif

/**********************************END OF FILE*********************************/

