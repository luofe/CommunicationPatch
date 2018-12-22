
/*******************************************************************************
//Print.h
//调试打印数据通过USART1的驱动头文件
*******************************************************************************/

#ifndef	PRINT_H
#define	PRINT_H

/******************************************************************************
//宏定义
*******************************************************************************/

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

#define SERVER_PRINTF_EN		            1	//定义是否打印输出 TRUE/FALSE
#define DEVICE_PRINTF_EN                    1	//定义是否打印输出 TRUE/FALSE
#define SERVER_AT_PRINTF_EN                 1	//定义是否打印输出 TRUE/FALSE
#define FLASH_PRINTF_EN                     1	//定义是否打印输出 TRUE/FALSE

//#define DEBUG_USART_BAUDRATE            115200                       //定义DEBUG_USART通信波特率
//#define DEBUG_USART                     USART3                       //定义DEBUG_USART使用的串口号
//#define DEBUG_USART_CLK                 RCC_APB1Periph_USART3        //定义DEBUG_USART使用的模块时钟
//
//#define DEBUG_USART_IRQn                USART3_IRQn
//#define DEBUG_USART_IRQHandler          USART3_IRQHandler
//
////DEBUG_USART_TX -----------------------------------------------
//#define DEBUG_USART_TX_GPIO_CLK         RCC_APB2Periph_GPIOB         //定义DEBUG_USART_TX端口的时钟
//#define DEBUG_USART_TX_GPIO_PORT        GPIOB                        //定义DEBUG_USART_TX占用的GPIO端口
//#define DEBUG_USART_TX_GPIO_PIN         GPIO_Pin_10                   //定义DEBUG_USART_TX的GPIO引脚号
//
////DEBUG_USART_RX -----------------------------------------------
//#define DEBUG_USART_RX_GPIO_CLK         RCC_APB2Periph_GPIOB         //定义DEBUG_USART_RX端口的时钟
//#define DEBUG_USART_RX_GPIO_PORT        GPIOB                        //定义DEBUG_USART_RX占用的GPIO端口
//#define DEBUG_USART_RX_GPIO_PIN         GPIO_Pin_11                  //定义DEBUG_USART_RX的GPIO引脚号

#define DEBUG_USART_BAUDRATE            115200                       //定义DEBUG_USART通信波特率
#define DEBUG_USART                     USART1                       //定义DEBUG_USART使用的串口号
#define DEBUG_USART_CLK                 RCC_APB2Periph_USART1        //定义DEBUG_USART使用的模块时钟

#define DEBUG_USART_IRQn                USART1_IRQn
#define DEBUG_USART_IRQHandler          USART1_IRQHandler

//DEBUG_USART_TX -----------------------------------------------
#define DEBUG_USART_TX_GPIO_CLK         RCC_APB2Periph_GPIOA         //定义DEBUG_USART_TX端口的时钟
#define DEBUG_USART_TX_GPIO_PORT        GPIOA                        //定义DEBUG_USART_TX占用的GPIO端口
#define DEBUG_USART_TX_GPIO_PIN         GPIO_Pin_9                   //定义DEBUG_USART_TX的GPIO引脚号

//DEBUG_USART_RX -----------------------------------------------
#define DEBUG_USART_RX_GPIO_CLK         RCC_APB2Periph_GPIOA         //定义DEBUG_USART_RX端口的时钟
#define DEBUG_USART_RX_GPIO_PORT        GPIOA                        //定义DEBUG_USART_RX占用的GPIO端口
#define DEBUG_USART_RX_GPIO_PIN         GPIO_Pin_10                  //定义DEBUG_USART_RX的GPIO引脚号







//USART接收超时
#define DEBUG_COMM_RX_DATA_TIMEOUT              5	    //没有接收超时5ms就是接收完毕






/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
//调试用的USART接收数据的结构体定义
typedef struct
{
    u8  RxStatus;                   //接收数据状态标志
    u16 RxTimeout_Count;            //接收超时计数,单位ms
    u8  RxBuffer[250];              //接收数据缓冲区
    u16 RxIndex;                    //接收数据计数

    u8  TxBuffer[250];              //发送数据缓冲区
    u16 TxIndex;                    //发送数据计数
}DebugCommStruct;








/******************************************************************************
//变量定义/声明
*******************************************************************************/
//用于调试的USART的数据结构体
extern DebugCommStruct s_DebugComm;















/******************************************************************************
//函数声明
*******************************************************************************/
//功能:	程序调试口的初始化函数
void Debug_Init(void);

int putchar(int ch);	//重写putchar 函数

//函数功能: 与调试口通信的监测函数
void Debug_Comm_Rec_Monitor(void);










#endif