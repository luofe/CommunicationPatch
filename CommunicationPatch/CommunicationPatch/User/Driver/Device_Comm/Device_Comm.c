
/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
// Functiom:项目工程与设备端通信相关的程序。
//备注:
1、具体的设备端与通信板的通信协议请参考附带的对应文档；


*******************************************************************************/

/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//变量定义
*******************************************************************************/
//用于与设备端通信的接收数据结构体
DeviceCommRxStruct s_DeviceCommRx;

//与设备端通信的发送数据的结构体定义
DeviceCommTxStruct s_DeviceCommTx;

// 传感器校准的参数结构体
SensorAdjStruct s_SensorAdj;

// RTC结构体
RTCStruct s_RTC;

// IP地址和端口号结构体
IPAddrPortStruct s_IPAddrPort;

// 数据上报时间间隔结构体
UploadIntervalStruct s_UploadInterval;

// 通用传感器数据结构体
SensorDataStruct s_SensorData;

// GPS定位信息
GPSInfoStruct s_GPSInfo;










/******************************************************************************
//函数声明
*******************************************************************************/
//函数功能: 与设备端通信相关的初始化函数
void Device_Comm_Init(void);

//函数功能: 与设备端通信用的USART初始化函数
void Device_Comm_USART_Init(void);

//函数功能: 与设备端通信用的GPIO口初始化函数
void Device_Comm_GPIO_Init(void);

//功能: 与设备端板通信的发送数据
void Device_Comm_Send_Data(u8 *buffer, u16 data_l);

//函数功能: 与设备端通信的数据包打包函数
void Device_Comm_Package_Bale(u8 cmd);

//函数功能: 与设备端通信的数据包处理函数
void Device_Comm_Package_Process(u8 cmd, u8* resp_str);

//函数功能: 与通信板通信数据包解析函数
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l);

//函数功能: 与设备端通信的监测函数
void Device_Comm_Rec_Monitor(void);


























/******************************************************************************
//函数定义
*******************************************************************************/
/********************************************************
//函数名称: Device_Comm_Init
//函数功能: 与设备端通信相关的初始化函数
//输    入: 无
//输    出: 无
//备    注: 无
********************************************************/
void Device_Comm_Init(void)
{
    Device_Comm_USART_Init();
    
    //    Device_Comm_GPIO_Init();
}

/********************************************************
//函数名称: Device_Comm_USART_Init
//函数功能: 与设备端通信用的USART初始化函数
//输    入: 无
//输    出: 无
//备    注: 无
********************************************************/
void Device_Comm_USART_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	//初始化NVIC结构体
    
    //复位DEVICE_COMM_USART模块
    USART_DeInit(DEVICE_COMM_USART);
    
    //使能DEVICE_COMM_USART_TX引脚模块时钟
    RCC_APB2PeriphClockCmd(DEVICE_COMM_USART_TX_GPIO_CLK, ENABLE);
    
    //使能DEVICE_COMM_USART_RX引脚模块时钟
    RCC_APB2PeriphClockCmd(DEVICE_COMM_USART_RX_GPIO_CLK, ENABLE);
    
    //使能USART模块时钟
    if(DEVICE_COMM_USART == USART1)
    {
        RCC_APB2PeriphClockCmd(DEVICE_COMM_USART_CLK, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(DEVICE_COMM_USART_CLK, ENABLE);
    }
    
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    //DEVICE_COMM_USART的GPIO配置
    //DEVICE_COMM_USART_TX: 推挽复用输出
    GPIO_InitStructure.GPIO_Pin   = DEVICE_COMM_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//选择50MHz
    GPIO_Init(DEVICE_COMM_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    //DEVICE_COMM_USART_RX: 浮空输入(或带上拉输入)
    GPIO_InitStructure.GPIO_Pin   = DEVICE_COMM_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(DEVICE_COMM_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    
    //DEVICE_COMM_USART模块参数配置
    //波特率: USART1_BAUDRATE；8个数据位；1个停止位；无校验位；无硬件流控制；使能发送和接收；
    USART_InitStructure.USART_BaudRate            = DEVICE_COMM_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEVICE_COMM_USART, &USART_InitStructure); 
    
    USART_Cmd(DEVICE_COMM_USART, ENABLE);                         //使能DEVICE_COMM_USART模块
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//使用优先级分组2	
    NVIC_InitStructure.NVIC_IRQChannel                      = DEVICE_COMM_USART_IRQn; //使能USART中断	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;                //抢断优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 1;                //响应优先级为1
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	//配置USART的嵌套向量中断
    
	USART_ITConfig(DEVICE_COMM_USART, USART_IT_RXNE, ENABLE);	//使能USART接收中断    
}

/*****************************************
//名称: Device_Comm_Send_Data
//功能: 与设备端板通信的发送数据
//入口: u8 *buffer——需要发送的数据;u16 data_l——数据长度
//出口: 无
******************************************/
void Device_Comm_Send_Data(u8 *buffer, u16 data_l)
{ 
#if (DEVICE_PRINTF_EN)
    printf("发送数据给设备端: ");
#endif	
    
    while(data_l--)  //发送AT命令
    {
        
#if (DEVICE_PRINTF_EN)
        printf("%02X ", *buffer);
#endif	
        
        USART_SendData(DEVICE_COMM_USART, *buffer++); 
        while(USART_GetFlagStatus(DEVICE_COMM_USART, USART_FLAG_TXE) == RESET);//等待发送完成
    } 
    
#if (DEVICE_PRINTF_EN)
    printf("\r\n");
#endif	
    
}

//********************************************************
//函数名称: Device_Comm_Package_Bale
//函数功能: 与设备端通信的数据包打包函数
//输    入: u8 cmd——命令码
//输    出: 无
//备    注: 无
//********************************************************
void Device_Comm_Package_Bale(u8 cmd)
{
    u8 index;
    char data_str[255];
    char temp_str[100];
    
    data_str[0] = '$';
    switch(cmd)
    {
        case DEVICE_SET_SENSOR_ADJUST_CMD:   //如果是校准传感器
        {
            index = 1;
            strcpy(data_str, "S_SSL:");
            index += strlen("S_SSL:");
            //放入type
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入multiple
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.multiple, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入K1
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.K1, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入B1
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.B1, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入K2
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.K2, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入B2
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.B2, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:   //如果是读取传感器校准数据
        {
            index = 1;
            strcpy(data_str, "Q_SSL:");
            index += strlen("Q_SSL:");
            //放入type
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_SET_RTC_CMD:   //如果设置RTC 
        {
            index = 1;
            strcpy(data_str, "S_TIME:");
            index += strlen("S_TIME:");
            //放入year
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.year, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入month
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.month, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入day
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.day, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入hour
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.hour, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入min
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.min, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入sec
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.sec, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_RTC_CMD:   //如果是读取RTC
        {
            index = 1;
            strcpy(data_str, "Q_TIME");
            index += strlen("Q_TIME");
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:   //如果设置IP地址
        {
            index = 1;
            strcpy(data_str, "S_IP:");
            index += strlen("S_IP:");
            strcpy(&data_str[index], (char const*)s_IPAddrPort.ip_port);
            index += strlen(temp_str);
//            //放入ip1
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.ip1, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //放入ip2
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.ip2, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //放入ip3
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.ip3, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //放入ip4
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.ip4, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = ',';
//            //放入端口号
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.port, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:   //如果是读取IP地址和端口号
        {
            index = 1;
            strcpy(data_str, "Q_IP");
            index += strlen("Q_IP");
        }
        break;
        
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:   //如果设置数据上传时间间隔
        {
            index = 1;
            strcpy(data_str, "S_DATARPT:");
            index += strlen("S_DATARPT:");
            //放入time1
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_UploadInterval.time1, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入time2
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_UploadInterval.time2, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:   //如果读取数据上传时间间隔
        {
            index = 1;
            strcpy(data_str, "Q_DATARPT");
            index += strlen("Q_DATARPT");
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:   //如果设置心跳包时间间隔
        {
            index = 1;
            strcpy(data_str, "S_HEALTHRPT:");
            index += strlen("S_HEALTHRPT:");
            //放入heartbeat
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_UploadInterval.heartbeat, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:   //如果读取心跳包时间间隔
        {
            index = 1;
            strcpy(data_str, "Q_HEALTHRPT");
            index += strlen("Q_HEALTHRPT");
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:   //如果控制重启复位
        {
            index = 1;
            strcpy(data_str, "C_RESET");
            index += strlen("C_RESET");
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:   //如果控制打开所有信息打印
        {
            index = 1;
            strcpy(data_str, "SHOW_ALL");
            index += strlen("SHOW_ALL");
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:   //如果控制隐藏所有信息
        {
            index = 1;
            strcpy(data_str, "HIDE_ALL");
            index += strlen("HIDE_ALL");
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:   //如果控制传感器信息打印
        {
            index = 1;
            strcpy(data_str, "SHOW_DATA");
            index += strlen("SHOW_DATA");
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:   //如果控制隐藏传感器信息
        {
            index = 1;
            strcpy(data_str, "HIDE_DATA");
            index += strlen("HIDE_DATA");
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:   //如果控制GPS信息打印
        {
            index = 1;
            strcpy(data_str, "SHOW_GPS");
            index += strlen("SHOW_GPS");
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:   //如果控制隐藏GPS信息
        {
            index = 1;
            strcpy(data_str, "HIDE_GPS");
            index += strlen("HIDE_GPS");
        }
        break;
        
        default :
        
        break;
    }
    //放入回车换行
    strcpy(&data_str[index], "\r\n");
    index += 2;
    //发送
    Device_Comm_Send_Data((u8*)data_str, index);
}

//********************************************************
//函数名称: Device_Comm_Package_Process
//函数功能: 与设备端通信的数据包处理函数
//输    入: u8 cmd——命令码，u8* resp_str——应答字符串
//输    出: 无
//备    注: 无
//********************************************************
void Device_Comm_Package_Process(u8 cmd, u8* resp_str)
{
    u16 i;
    u16 index = 0;
    char temp_str[100];

    switch(cmd)
    {
        case DEVICE_SET_SENSOR_ADJUST_CMD:   //如果是校准传感器
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果没设置成功
            {
                Device_Comm_Package_Bale(DEVICE_SET_SENSOR_ADJUST_CMD);
            }
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:   //如果是读取传感器校准数据
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //如果读取成功
            {
                //获取type
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //加结束符
                s_SensorAdj.type = atoi(temp_str);
                index += 1;
                //获取multiple
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //加结束符
                s_SensorAdj.multiple = atoi(temp_str);
                index += 1;
                //获取K1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //加结束符
                s_SensorAdj.K1 = atoi(temp_str);
                index += 1;
                //获取B1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //加结束符
                s_SensorAdj.B1 = atoi(temp_str);
                index += 1;
                //获取K2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //加结束符
                s_SensorAdj.K2 = atoi(temp_str);
                index += 1;
                //获取B2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while((resp_str[index] != '(') && (resp_str[index + 1] != 'O'));
                temp_str[i++] = '\0';   //加结束符
                s_SensorAdj.B2 = atoi(temp_str);
                index += 2;
            }
            else    //如果失败，则再读取一次
            {
                Device_Comm_Package_Bale(DEVICE_READ_SENSOR_ADJUST_CMD);
            }
        }
        break;
        
        case DEVICE_SET_RTC_CMD:   //如果设置RTC 
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_SET_RTC_CMD);
            }
        }
        break;
        
        case DEVICE_READ_RTC_CMD:   //如果是读取RTC
        {
//            for(i = 0; i < 3; i++)
//            {
//                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
//            }
//            if(strcmp("OK!", temp_str) == SUCCEED)  //如果读取成功
            {
                //获取year
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //加结束符
                s_RTC.year = atoi(temp_str);
                index += 1;
                //获取month
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //加结束符
                s_RTC.month = atoi(temp_str);
                index += 1;
                //获取day
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //加结束符
                s_RTC.day = atoi(temp_str);
                index += 1;
                //获取hour
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //加结束符
                s_RTC.hour = atoi(temp_str);
                index += 1;
                //获取min
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //加结束符
                s_RTC.min = atoi(temp_str);
                index += 1;
                //获取sec
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //加结束符
                s_RTC.sec = atoi(temp_str);
                index += 2;
            }
//            else
//            {
//                Device_Comm_Package_Bale(DEVICE_READ_RTC_CMD);
//            }
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:   //如果设置IP地址
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_SET_IP_ADDR_CMD);
            }
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:   //如果是读取IP地址和端口号
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //如果读取成功
            {
                memcpy(s_IPAddrPort.ip_port, resp_str, strlen((char const*)resp_str));
//                //获取ip1
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while(resp_str[index] != '.');
//                temp_str[i++] = '\0';   //加结束符
//                s_IPAddrPort.ip1 = atoi(temp_str);
//                index += 1;
//                //获取ip2
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while(resp_str[index] != '.');
//                temp_str[i++] = '\0';   //加结束符
//                s_IPAddrPort.ip2 = atoi(temp_str);
//                index += 1;
//                //获取ip3
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while(resp_str[index] != '.');
//                temp_str[i++] = '\0';   //加结束符
//                s_IPAddrPort.ip3 = atoi(temp_str);
//                index += 1;
//                //获取ip4
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while(resp_str[index] != ',');
//                temp_str[i++] = '\0';   //加结束符
//                s_IPAddrPort.ip4 = atoi(temp_str); 
//                index += 1;               
//                //获取port
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while((resp_str[index] != '(') && (resp_str[index + 1] != 'O'));
//                temp_str[i++] = '\0';   //加结束符
//                s_IPAddrPort.port = atoi(temp_str);
//                index += 2;
            }
            else    //如果失败，则再读取一次
            {
                Device_Comm_Package_Bale(DEVICE_READ_IP_ADDR_CMD);
            }
        }
        break;
        
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:   //如果设置数据上传时间间隔
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果读取不成功
            {
                Device_Comm_Package_Bale(DEVICE_SET_UPLOAD_INTERVAL_CMD);
            }
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:   //如果读取数据上传时间间隔
        {
//            for(i = 0; i < 3; i++)
//            {
//                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
//            }
//            if(strcmp("OK!", temp_str) == SUCCEED)  //如果读取成功
            {
                //获取time1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //加结束符
                s_UploadInterval.time1 = atoi(temp_str);
                index += 1;
                //获取time2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //加结束符
                s_UploadInterval.time2 = atoi(temp_str);
                index += 2;
            }
//            else    //如果失败，则再读取一次
//            {
//                Device_Comm_Package_Bale(DEVICE_READ_IP_ADDR_CMD);
//            }
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:   //如果设置心跳包时间间隔
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_SET_HEARTBEAT_INTERVAL_CMD);
            }
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:   //如果读取心跳包时间间隔
        {
//            for(i = 0; i < 3; i++)
//            {
//                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
//            }
//            if(strcmp("OK!", temp_str) == SUCCEED)  //如果读取成功
            {
                //获取heartbeat
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //加结束符
                s_UploadInterval.heartbeat = atoi(temp_str);
                index += 2;
            }
//            else    //如果失败，则再读取一次
//            {
//                Device_Comm_Package_Bale(DEVICE_READ_HEARTBEAT_INTERVAL_CMD);
//            }
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:   //如果控制重启复位
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_CTR_RESET_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:   //如果控制打开所有信息打印
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_CTR_ALL_PRINTF_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:   //如果控制隐藏所有信息
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_CTR_ALL_HIDE_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:   //如果控制传感器信息打印
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_CTR_SENSOR_PRINTF_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:   //如果控制隐藏传感器信息
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_CTR_SENSOR_HIDE_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:   //如果控制GPS信息打印
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_CTR_GPS_PRINTF_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:   //如果控制隐藏GPS信息
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                Device_Comm_Package_Bale(DEVICE_CTR_GPS_HIDE_CMD);
            }
        }
        break;
        
        case DEVICE_GET_SENSOR_DATA_CMD:   //如果是传感器数据上报
        {
            //[SS-Temp  ]=
            //获取SS-Temp
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.TRH.temp = atoi(temp_str);
            index += 2;

            //[SS-RH    ]=
            //获取SS-RH
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.TRH.humi = atoi(temp_str);
            index += 2;
            
            //获取SS-PM2.5
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM2_5.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM2_5.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM2_5.app_val = atoi(temp_str);
            index += 2;
            
            //获取SS-PM10
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM10.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM10.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM10.app_val = atoi(temp_str);
            index += 2;
            
            //获取SS-CO(3)
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.CO.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.CO.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.CO.app_val = atoi(temp_str);
            index += 2;
            
            //获取SS-NO2(4)
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO2.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO2.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO2.app_val = atoi(temp_str);
            index += 2;
            
            //获取SS-O3(5) 
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.O3.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.O3.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.O3.app_val = atoi(temp_str);
            index += 2;
            
            //获取SS-SO2(6) 
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.SO2.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.SO2.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.SO2.app_val = atoi(temp_str);
            index += 2;
            
            //获取SS-NO(7)
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO.app_val = atoi(temp_str);
            index += 2;
            
            //获取SS-TVOC(9
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.TVOC.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.TVOC.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.TVOC.app_val = atoi(temp_str);
            index += 2;
            
            //获取SS-Temp-W
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.ExtSensor.temp = atoi(temp_str);
            index += 2;
            
            //获取SS-RH-W
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.ExtSensor.humi = atoi(temp_str);
            index += 2;
            
            //获取SS-WD
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.ExtSensor.wd = atoi(temp_str);
            index += 2;
            
            //获取SS-WS
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.ExtSensor.ws = atoi(temp_str);
            index += 2;
            
            //获取SS-PA
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.ExtSensor.pa = atoi(temp_str);
            index += 2;
            
            //获取FUN-main
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '/') && (resp_str[index + 1] != 'm'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.Fan.m_freq = atoi(temp_str);
            index += 2;
            
            //获取FUN-pm10
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '/') && (resp_str[index + 1] != 'm'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.Fan.pm10_freq = atoi(temp_str);
            index += 2;
        }
        break;
        
        case DEVICE_GET_GPS_DATA_CMD:       //如果是GPS信息上报
        {
            //[GPS-STA  ]=
            //获取GPS-STA
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.status = atoi(temp_str);
            index += 2;
            
            //获取GPS-FS
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.fs = atoi(temp_str);
            index += 2;
            
            //获取GPS-LAT
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.latitude = atoi(temp_str);
            index += 2;
            
            //获取GPS-LON
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.longitude = atoi(temp_str);
            index += 2;
            
            //获取GPS-HIGH
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.high = atoi(temp_str);
            index += 2;
            
            //获取GPS-SPEED
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.speed = atoi(temp_str);
            index += 2;
            
            //获取GPS-HEAD
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.heading = atoi(temp_str);
            index += 2;
            
            //获取GPS-NOSV
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.noSV = atoi(temp_str);
            index += 2;
        }
        break;
        
        default :
        
        break;
    }
}

//********************************************************
//函数名称: Device_Comm_Package_Analysis
//函数功能: 与通信板通信数据包解析函数
//输    入: u8 *data——结构体指针, u16 data_l——数据长度
//输    出: u8——解析状态，0——成功，
//备    注: 无
//********************************************************
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l)
{
    u16  index;
    u16  i;
    char temp_str[1024];
    
    //先判断第一个字符是不是“$”
    if(data[0] == '$')
    {
        //如果是“S_SSL=”
        if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'S') && (data[4] == 'S') && (data[5] == 'L') && (data[6] == '='))
        {
            index = 7;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_SET_SENSOR_ADJUST_CMD, (u8*)temp_str);
        }
        //如果是“Q_SSL=”
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'S') && (data[4] == 'S') && (data[5] == 'L') && (data[6] == '='))
        {
            index = 7;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_READ_SENSOR_ADJUST_CMD, (u8*)temp_str);
        }
        //如果是“S_TIME=”
        else if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'T') && (data[4] == 'I') && (data[5] == 'M') && 
                (data[6] == 'E') && (data[7] == '='))
        {
            index = 8;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_SET_RTC_CMD, (u8*)temp_str);
        }
        //如果是“Q_TIME=”
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'T') && (data[4] == 'I') && (data[5] == 'M') && 
                (data[6] == 'E') && (data[7] == '='))
        {
            index = 8;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_READ_RTC_CMD, (u8*)temp_str);
        }
        //如果是“S_IP=”
        else if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'I') && (data[4] == 'P') && (data[5] == '='))
        {
            index = 6;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_SET_IP_ADDR_CMD, (u8*)temp_str);
        }
        //如果是“Q_IP=”
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'I') && (data[4] == 'P') && (data[5] == '='))
        {
            index = 6;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_READ_IP_ADDR_CMD, (u8*)temp_str);
        }
        //如果是“S_DATARPT=”
        else if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'D') && (data[4] == 'A') && (data[5] == 'T') && 
                (data[6] == 'A') && (data[7] == 'R') && (data[8] == 'P') && (data[9] == 'T') && (data[10] == '='))
        {
            index = 11;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_SET_UPLOAD_INTERVAL_CMD, (u8*)temp_str);
        }
        //如果是“Q_DATARPT=”
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'D') && (data[4] == 'A') && (data[5] == 'T') && 
                (data[6] == 'A') && (data[7] == 'R') && (data[8] == 'P') && (data[9] == 'T') && (data[10] == '='))
        {
            index = 11;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_READ_UPLOAD_INTERVAL_CMD, (u8*)temp_str);
        }
        //如果是“S_HEALTHRPT=”
        else if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'H') && (data[4] == 'E') && (data[5] == 'A') && 
                (data[6] == 'L') && (data[7] == 'T') && (data[8] == 'H') && (data[9] == 'R') && (data[10] == 'P') && 
                    (data[11] == 'T') && (data[12] == '='))
        {
            index = 13;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_SET_HEARTBEAT_INTERVAL_CMD, (u8*)temp_str);
        }
        //如果是“Q_HEALTHRPT=”
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'H') && (data[4] == 'E') && (data[5] == 'A') && 
                (data[6] == 'L') && (data[7] == 'T') && (data[8] == 'H') && (data[9] == 'R') && (data[10] == 'P') && 
                    (data[11] == 'T') && (data[12] == '='))
        {
            index = 13;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_READ_HEARTBEAT_INTERVAL_CMD, (u8*)temp_str);
        }
        //如果是“C_RESET=”
        else if((data[1] == 'C') && (data[2] == '_') && (data[3] == 'R') && (data[4] == 'E') && (data[5] == 'S') && 
                (data[6] == 'E') && (data[7] == 'T') && (data[8] == '='))
        {
            index = 9;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_CTR_RESET_CMD, (u8*)temp_str);
        }
        //如果是“SHOW_ALL=”
        else if((data[1] == 'S') && (data[2] == 'H') && (data[3] == 'O') && (data[4] == 'W') && (data[5] == '_') && 
                (data[6] == 'A') && (data[7] == 'L') && (data[8] == 'L') && (data[9] == '='))
        {
            index = 10;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_CTR_ALL_PRINTF_CMD, (u8*)temp_str);
        }
        //如果是“HIDE_ALL=”
        else if((data[1] == 'H') && (data[2] == 'I') && (data[3] == 'D') && (data[4] == 'E') && (data[5] == '_') && 
                (data[6] == 'A') && (data[7] == 'L') && (data[8] == 'L') && (data[9] == '='))
        {
            index = 10;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_CTR_ALL_HIDE_CMD, (u8*)temp_str);
        }
        //如果是“SHOW_DATA=”
        else if((data[1] == 'S') && (data[2] == 'H') && (data[3] == 'O') && (data[4] == 'W') && (data[5] == '_') && 
                (data[6] == 'D') && (data[7] == 'A') && (data[8] == 'T') && (data[9] == 'A') && (data[10] == '='))
        {
            index = 11;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_CTR_SENSOR_PRINTF_CMD, (u8*)temp_str);
        }
        //如果是“HIDE_DATA=”
        else if((data[1] == 'H') && (data[2] == 'I') && (data[3] == 'D') && (data[4] == 'E') && (data[5] == '_') && 
                (data[6] == 'D') && (data[7] == 'A') && (data[8] == 'T') && (data[9] == 'A') && (data[10] == '='))
        {
            index = 11;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_CTR_SENSOR_HIDE_CMD, (u8*)temp_str);
        }
        //如果是“SHOW_GPS=”
        else if((data[1] == 'S') && (data[2] == 'H') && (data[3] == 'O') && (data[4] == 'W') && (data[5] == '_') && 
                (data[6] == 'G') && (data[7] == 'P') && (data[8] == 'S') && (data[9] == '='))
        {
            index = 10;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_CTR_GPS_PRINTF_CMD, (u8*)temp_str);
        }
        //如果是“HIDE_GPS=”
        else if((data[1] == 'H') && (data[2] == 'I') && (data[3] == 'D') && (data[4] == 'E') && (data[5] == '_') && 
                (data[6] == 'G') && (data[7] == 'P') && (data[8] == 'S') && (data[9] == '='))
        {
            index = 10;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //加上结束符
            Device_Comm_Package_Process(DEVICE_CTR_GPS_HIDE_CMD, (u8*)temp_str);
        }
    }
    else
    {
        for(index = 0; index < (data_l / 2); index++)
        {
            //\r\n[SS-Temp......传感器数据
            if((data[index] == '[') && (data[index + 1] == 'S') && (data[index + 2] == 'S') && (data[index + 3] == '-') && 
               (data[index + 4] == 'T') && (data[index + 5] == 'e') && (data[index + 6] == 'm') && (data[index + 7] == 'p'))
            {
                index = 0;
                i = 0;
                do
                {
                    temp_str[i++] = data[index++];
                }while((data[index] == '\r') && (data[index + 1] == '\n'));
                temp_str[i++] = '\0'; //加上结束符
                Device_Comm_Package_Process(DEVICE_GET_SENSOR_DATA_CMD, (u8*)temp_str);
            }
            //\r\n[GPS-STA......GPS数据
            else if((data[index] == '[') && (data[index + 1] == 'G') && (data[index + 2] == 'P') && (data[index + 3] == 'S') && 
               (data[index + 4] == '-') && (data[index + 5] == 'S') && (data[index + 6] == 'T') && (data[index + 7] == 'A'))
            {
                index = 0;
                i = 0;
                do
                {
                    temp_str[i++] = data[index++];
                }while((data[index] == '\r') && (data[index + 1] == '\n'));
                temp_str[i++] = '\0'; //加上结束符
                Device_Comm_Package_Process(DEVICE_GET_GPS_DATA_CMD, (u8*)temp_str);
            }
        }
    }
    
    return SUCCEED;
}

//********************************************************
//函数名称: Device_Comm_Rec_Monitor
//函数功能: 与设备端通信的监测函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void Device_Comm_Rec_Monitor(void)      
{
    if(s_DeviceCommRx.Status == TRUE)   //有接收
    {
        if(s_DeviceCommRx.Timeout_Count >= DEVICE_COMM_RX_DATA_TIMEOUT)    //是否超时了
        {
            u16 temp_l = s_DeviceCommRx.Index;    //拷贝出数据长度
            //将数据拷贝至公共缓冲区，防止被新的数据淹没
            memcpy(g_PublicDataBuffer, s_DeviceCommRx.Buffer, temp_l); 
            
            s_DeviceCommRx.Status = FALSE;
            s_DeviceCommRx.Index = 0;
            s_DeviceCommRx.Timeout_Count = 0;
            
#if (DEVICE_PRINTF_EN)
            printf("接收到设备端的数据: ");
            for(u8 i = 0; i < temp_l; i++)
            {
                printf("%c", s_DeviceCommRx.Buffer[i]);
            }
            printf("\r\n");
#endif	
            
            Device_Comm_Package_Analysis(g_PublicDataBuffer, temp_l);
        }
    }
    //假如很长时间没有接收到设备端的数据，说明设备端要么掉电了，要么RS485线出故障了，则重启设备端
    else if(s_DeviceCommRx.Timeout_Count >= DEVICE_COMM_NO_DATA_REC_TIMEOUT)
    {
        s_DeviceCommRx.Timeout_Count = 0;
        
        s_DeviceCommRx.Status = FALSE;
        s_DeviceCommRx.Index = 0;
        s_DeviceCommRx.Timeout_Count = 0;
    }
}





//////////////////////////////END OF C文件////////////////////////////////////
