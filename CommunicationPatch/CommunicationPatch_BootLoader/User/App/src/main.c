/*******************************************************************************
// Copyright(c)2017, 天津新众聚联信息科技股份有限公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language：C)
// Date   : 2017-04-01
// Author : 刘锋
// Functiom:项目工程的main源文件
*******************************************************************************/


/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//变量定义
*******************************************************************************/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;


/******************************************************************************
//函数声明
*******************************************************************************/
void ButtonInit(void);                                    //按键初始化函数
void UsartConfig(void);                                   //USART配置函数


/******************************************************************************
//函数定义
*******************************************************************************/
//********************************************************
//函数名称: main
//函数功能: 系统主函数
//输    入: 无
//输    出: 无
//备    注: 
//********************************************************
int main(void)
{
    /*
     * 解锁Flash
     */
    FLASH_Unlock();


    /*
     * 按键初始化
     */ 
    ButtonInit();
    Internal_Flash_Upgrade_ReadOut();               //读取Flash中是否升级的标志  

    /*
     * 判断是否按下手动键
     */
    if((SygProgramUpgradeFlag != 0x0000))
    {
        //如果手动键被按下，触发IAP驱动来重新对Flash编程
        UsartConfig();                                      //串口配置
        SerialPutString("\r\n==========================================================");
        SerialPutString("\r\n=                                                        =");
        SerialPutString("\r\n=                    BootLoader                          =");
        SerialPutString("\r\n=                                                        =");
        SerialPutString("\r\n=                    烧写应用程序                         =");
        SerialPutString("\r\n=                                                        =");
        SerialPutString("\r\n==========================================================");
        SerialPutString("\r\n\r\n");

        Main_Menu();                                        //启动IAP程序
    }
    else
    {                                                       //运行应用程序
        /*
         * 判断应用程序地址的代码空间是否被编程过
         */
        if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
        { 
            /*
             * 设置用户应用程序地址
             */
            JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
            Jump_To_Application = (pFunction) JumpAddress;

            /*
             * 初始化用户应用程序的堆栈指针
             */
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);

            /*
             * 跳转到用户应用程序
             */
            Jump_To_Application();
        }//end of if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    }//end of if(GetButtonStatus() == Bit_RESET)

    while(1);
}


//********************************************************
//函数名称: ButtonInit
//函数功能: Button端口初始化函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void ButtonInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*
     * USE_BUTTON端口初始化: 浮空输入
     */
    RCC_APB2PeriphClockCmd(USE_USART_RX_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = USE_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;      //下拉输入
    GPIO_Init(USE_USART_RX_GPIO_PORT, &GPIO_InitStructure);
}


//********************************************************
//函数名称: UsartConfig
//函数功能: Usart模块配置函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void UsartConfig(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /*
     * 复位USE_USART模块
     */
    USART_DeInit(USE_USART);

    /*
     * 使能USE_USART_TX引脚模块时钟
     */
    RCC_APB2PeriphClockCmd(USE_USART_TX_GPIO_CLK, ENABLE);

    /*
     * 使能USE_USART_RX引脚模块时钟
     */
    RCC_APB2PeriphClockCmd(USE_USART_RX_GPIO_CLK, ENABLE);

    /*
     * 使能USART模块时钟
     */
    if(USE_USART == USART1)
    {
        RCC_APB2PeriphClockCmd(USE_USART_CLK, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(USE_USART_CLK, ENABLE);
    }//end of if(USE_USART == USART1)


    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*
     * USE_USART的GPIO配置
     */
    //USE_USART_TX: 推挽复用输出
    GPIO_InitStructure.GPIO_Pin   = USE_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //推挽复用输出
    GPIO_Init(USE_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    //USE_USART_RX: 浮空输入(或带上拉输入)
    GPIO_InitStructure.GPIO_Pin   = USE_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(USE_USART_RX_GPIO_PORT, &GPIO_InitStructure);


    /*
     * USE_USART模块参数配置
     * 波特率: USART1_BAUDRATE；8个数据位；1个停止位；无校验位；无硬件流控制；使能发送和接收；
     */
    USART_InitStructure.USART_BaudRate            = USE_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USE_USART, &USART_InitStructure); 

    USART_Cmd(USE_USART, ENABLE);                         //使能USE_USART模块
}


#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

//////////////////////////////END OF C文件////////////////////////////////////
