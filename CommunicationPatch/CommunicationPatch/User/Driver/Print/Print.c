
/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
// Functiom:项目工程负责调试打印信息和与服务器（不是服务器）通信的源程序。
*******************************************************************************/

/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//变量定义
*******************************************************************************/
//用于调试的USART的数据结构体
DebugCommStruct s_DebugComm;



/******************************************************************************
//函数声明
*******************************************************************************/
//功能:	程序调试口的初始化函数
void Debug_Init(void);

// 程序调试USART的初始化函数
void Debug_Usart_Init(void);

//函数功能: 调试用的GPIO口初始化函数
void Debug_GPIO_Init(void);

//函数功能: 与调试口通信的数据包解析函数
u8 Debug_Comm_Package_Analysis(u8 *rec_array, u16 rec_length);

//函数功能: 与调试口通信的监测函数
void Debug_Comm_Rec_Monitor(void);


/******************************************************************************
//函数定义
*******************************************************************************/
/**********************************
//名称:	Debug_Init
//功能:	程序调试口的初始化函数
//入口参数:	无
//出口参数:	无
**********************************/
void Debug_Init(void)
{
    Debug_Usart_Init();
}

/**********************************
//名称:	Debug_Usart_Init
//功能:	程序调试USART的初始化函数
//入口参数:	无
//出口参数:	无
**********************************/
void Debug_Usart_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	//初始化NVIC结构体
    
    //复位DEBUG_USART模块
    USART_DeInit(DEBUG_USART);
    
    //使能DEBUG_USART_TX引脚模块时钟
    RCC_APB2PeriphClockCmd(DEBUG_USART_TX_GPIO_CLK, ENABLE);
    
    //使能DEBUG_USART_RX引脚模块时钟
    RCC_APB2PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK, ENABLE);
    
    //使能USART模块时钟
    if(DEBUG_USART == USART1)
    {
        RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
    }
    
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    //DEBUG_USART的GPIO配置
    //DEBUG_USART_TX: 推挽复用输出
    GPIO_InitStructure.GPIO_Pin   = DEBUG_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//选择50MHz
    GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    //DEBUG_USART_RX: 浮空输入(或带上拉输入)
    GPIO_InitStructure.GPIO_Pin   = DEBUG_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    
    //DEBUG_USART模块参数配置
    //波特率: USART1_BAUDRATE；8个数据位；1个停止位；无校验位；无硬件流控制；使能发送和接收；
    USART_InitStructure.USART_BaudRate            = DEBUG_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEBUG_USART, &USART_InitStructure); 
    
    USART_Cmd(DEBUG_USART, ENABLE);                         //使能DEBUG_USART模块
    
    NVIC_InitStructure.NVIC_IRQChannel                      = DEBUG_USART_IRQn; //使能USART中断	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;                //抢断优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 2;                //响应优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	//配置USART的嵌套向量中断
    
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, DISABLE);	//先禁止USART接收中断
}

/**********************************
//名称:	putchar
//功能:	重写putchar函数，供系统库函数调用
//入口参数:	ch——int，要打印数据
//出口参数:	int,返回值
//备    注: 如果要使用仿真模式下的TerminalI/O显示打印，则需要把此映射函数去掉！！！
**********************************/
//PUTCHAR_PROTOTYPE
//{
//    USART_SendData(DEBUG_USART, (u8)ch);
//    while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
//    
//    return ch;
//}
int putchar(int ch)
{
    USART_SendData(DEBUG_USART, ch); //通过USART输出一个字符
    while(USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
    //等待发送完成
    return ch;
}

//********************************************************
//函数名称: Debug_Comm_Package_Analysis
//函数功能: 与调试口通信的数据包解析函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
u8 Debug_Comm_Package_Analysis(u8 *rec_array, u16 rec_length)
{
    u8      temp_data = 0;
    u16     temp_index = 0;        //数据索引
    u8      packet_analysis_error_status = PACKAGE_ANALYSIS_SUCCEED;    //数据包解析错误状态
    
#if (SERVER_PRINTF_EN)
    printf("接收到调试口的数据: ");
#endif	
    
	for(temp_index = 0; temp_index < rec_length; temp_index++)        //
	{
		temp_data = rec_array[temp_index];  //取出接收缓存区数据
        
#if (SERVER_PRINTF_EN)
        printf("%02X ",temp_data);  // 
#endif
        
	}
    
    return packet_analysis_error_status;
}

//********************************************************
//函数名称: Debug_Comm_Rec_Monitor
//函数功能: 与调试口通信的监测函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void Debug_Comm_Rec_Monitor(void)      
{
    if(s_DebugComm.RxStatus == TRUE)   //有接收
    {
        if(s_DebugComm.RxTimeout_Count >= DEBUG_COMM_RX_DATA_TIMEOUT)    //是否超时了
        {
            if(s_ServerCommRx.Status == FALSE)
            {
                char com_str[50];  //公共字符串
                char com_str2[50];  //公共字符串
                char const temp_str[]      = "/*擦除整片W25Q128*/";
                char const temp_str2[]     = "/*退出透传模式*/";
                
                u8  temp_sta = FAILURE;
                u8  temp_sta2 = FAILURE;
                
                memcpy(com_str, s_DebugComm.RxBuffer, strlen(temp_str));
                com_str[strlen(temp_str)] = '\0';  //放入结束符
                temp_sta = strcmp(com_str, temp_str);
 
                memcpy(com_str2, s_DebugComm.RxBuffer, strlen(temp_str2));
                com_str[strlen(temp_str2)] = '\0';  //放入结束符
                temp_sta2 = strcmp(com_str2, temp_str2);

                if(temp_sta == SUCCEED)
                {
                    
#if (SERVER_PRINTF_EN)
                    printf("擦除整片W25Q128中......稍等一会儿......\r\n");
#endif	
                    
                    SPI_FLASH_BulkErase();  //擦除整片
                    g_DataPageNum = SENSOR_DATA_MIN_PAGE_NUM - 1;
                    
#if (SERVER_PRINTF_EN)
                    printf("擦除完毕！\r\n");
#endif	
                    
                }
                else if(temp_sta2 == SUCCEED)
                {
                    g_DebugInterfaceTransmitFlag = FALSE;   //透传结束
                    //禁止所有信息的打印
                    Device_Printf_Ctr(DEVICE_CTR_ALL_HIDE_CMD);
                    
#if (SERVER_PRINTF_EN)
                    printf("\r\n退出透传模式\r\n");
#endif	
                    
                }
                else
                {
                    
#if (SERVER_PRINTF_EN)
                    printf("\r\n转发Debug口数据\r\n");
#endif	
                    
//                    // 转发给无线模块端
//                    for(u16 i = 0; i < s_DebugComm.RxIndex; i++)
//                    {
//                        USART_SendData(SERVER_COMM_USART, s_DebugComm.RxBuffer[i]); 
//                        while(USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_TXE) == RESET);//等待发送完成
//                    }
                    
                    // 转发给设备端
                    for(u16 i = 0; i < s_DebugComm.RxIndex; i++)
                    {
                        USART_SendData(DEVICE_COMM_USART, s_DebugComm.RxBuffer[i]); 
                        while(USART_GetFlagStatus(DEVICE_COMM_USART, USART_FLAG_TXE) == RESET);//等待发送完成
                    }
                    
                    g_DebugInterfaceTransmitFlag = TRUE;    //透传进行中
                }
                
                s_DebugComm.RxStatus = FALSE;
                s_DebugComm.RxIndex = 0;
                s_DebugComm.RxTimeout_Count = 0;
            }
        }
    }
    //假如很长时间没有接收到Debug口数据，则说明透传结束了
    else if(s_DeviceCommRx.Timeout_Count >= (1000UL * 60 * 3))
    {
        s_DebugComm.RxTimeout_Count = 0;
        g_DebugInterfaceTransmitFlag = FALSE;   //透传结束
        //禁止所有信息的打印
        Device_Printf_Ctr(DEVICE_CTR_ALL_HIDE_CMD);
        
#if (SERVER_PRINTF_EN)
        printf("\r\n超时退出透传模式\r\n");
#endif	
        
    }
}




//////////////////////////////END OF C文件////////////////////////////////////
