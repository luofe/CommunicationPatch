
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
2、设备上电默认只打印各传感器初始化状态，不打印其它信息；


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

// 设备初始化完毕标志
u8  g_DeviceInitialFlag = FALSE;










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
u8 Device_Comm_Package_Process(u8 cmd, u8* resp_str, u16 len);

//函数功能: 与通信板通信数据包解析函数
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l);

//函数功能: 接收设备端命令类数据解析函数
u8 Device_Rec_Command_Analysis(u8 cmd, u8* buf, u16 len);

//功能: 设备接收应答命令的管理函数
u8 Device_Rec_Response_Cmd_Monitor(u8 cmd);

//功能: 设备打印输出控制函数
u8 Device_Printf_Ctr(u8 cmd);

//功能: 设备端初始化函数
u8 Device_Initial(void);

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
    
	USART_ITConfig(DEVICE_COMM_USART, USART_IT_RXNE, DISABLE);	//使能USART接收中断    
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
        printf("%c", *buffer);
#endif	
        
        USART_SendData(DEVICE_COMM_USART, *buffer++); 
        while(USART_GetFlagStatus(DEVICE_COMM_USART, USART_FLAG_TXE) == RESET);//等待发送完成
    } 
    
//#if (DEVICE_PRINTF_EN)
//    printf("\r\n");
//#endif	
    
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
    
    memset(data_str, '\0', sizeof(data_str));
    memset(temp_str, '\0', sizeof(temp_str));
    
    data_str[0] = '$';
    switch(cmd)
    {
        case DEVICE_SET_SENSOR_ADJUST_CMD:   //如果是校准传感器
        {
            index = 1;
            strcat(data_str, "S_SSL:");
            index += strlen("S_SSL:");
            //放入type
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入multiple
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.multiple, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入K1
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.K1, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入B1
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.B1, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入K2
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.K2, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入B2
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.B2, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:   //如果是读取传感器校准数据
        {
            index = 1;
            strcat(data_str, "Q_SSL:");
            index += strlen("Q_SSL:");
            //放入type
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_SET_RTC_CMD:   //如果设置RTC 
        {
            index = 1;
            strcat(data_str, "S_TIME:");
            index += strlen("S_TIME:");
            //放入year
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.year, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入month
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.month, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入day
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.day, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入hour
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.hour, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入min
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.min, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //放入sec
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_RTC.sec, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_RTC_CMD:   //如果是读取RTC
        {
            index = 1;
            strcat(data_str, "Q_TIME");
            index += strlen("Q_TIME");
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:   //如果设置IP地址
        {
            index = 1;
            strcat(data_str, "S_IP:");
            index += strlen("S_IP:");
            strcat(data_str, (char const*)s_IPAddrPort.ip_port);
            index += strlen(temp_str);
//            //放入ip1
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.ip1, temp_str, 10);
//            strcat(data_str, temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //放入ip2
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.ip2, temp_str, 10);
//            strcat(data_str, temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //放入ip3
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.ip3, temp_str, 10);
//            strcat(data_str, temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //放入ip4
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.ip4, temp_str, 10);
//            strcat(data_str, temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = ',';
//            //放入端口号
//            memset(temp_str, 0, sizeof(temp_str));  //先清空
//            SysInt2Str(s_IPAddrPort.port, temp_str, 10);
//            strcat(data_str, temp_str);
//            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:   //如果是读取IP地址和端口号
        {
            index = 1;
            strcat(data_str, "Q_IP");
            index += strlen("Q_IP");
        }
        break;
        
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:   //如果设置数据上传时间间隔
        {
            index = 1;
            strcat(data_str, "S_DATARPT:");
            index += strlen("S_DATARPT:");
            //放入time1
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_UploadInterval.time1, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //放入time2
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_UploadInterval.time2, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:   //如果读取数据上传时间间隔
        {
            index = 1;
            strcat(data_str, "Q_DATARPT");
            index += strlen("Q_DATARPT");
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:   //如果设置心跳包时间间隔
        {
            index = 1;
            strcat(data_str, "S_HEALTHRPT:");
            index += strlen("S_HEALTHRPT:");
            //放入heartbeat
            memset(temp_str, 0, sizeof(temp_str));  //先清空
            SysInt2Str(s_UploadInterval.heartbeat, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:   //如果读取心跳包时间间隔
        {
            index = 1;
            strcat(data_str, "Q_HEALTHRPT");
            index += strlen("Q_HEALTHRPT");
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:   //如果控制重启复位
        {
            index = 1;
            strcat(data_str, "C_RESET");
            index += strlen("C_RESET");
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:   //如果控制打开所有信息打印
        {
            index = 1;
            strcat(data_str, "SHOW_ALL");
            index += strlen("SHOW_ALL");
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:   //如果控制隐藏所有信息
        {
            index = 1;
            strcat(data_str, "HIDE_ALL");
            index += strlen("HIDE_ALL");
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:   //如果控制传感器信息打印
        {
            index = 1;
            strcat(data_str, "SHOW_DATA");
            index += strlen("SHOW_DATA");
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:   //如果控制隐藏传感器信息
        {
            index = 1;
            strcat(data_str, "HIDE_DATA");
            index += strlen("HIDE_DATA");
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:   //如果控制GPS信息打印
        {
            index = 1;
            strcat(data_str, "SHOW_GPS");
            index += strlen("SHOW_GPS");
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:   //如果控制隐藏GPS信息
        {
            index = 1;
            strcat(data_str, "HIDE_GPS");
            index += strlen("HIDE_GPS");
        }
        break;
        
        default :
        
        break;
    }
    //放入回车换行
    strcat(data_str, "\r\n");
    index += 2;
    //发送
    Device_Comm_Send_Data((u8*)data_str, index);
}

//********************************************************
//函数名称: Device_Comm_Package_Process
//函数功能: 与设备端通信的数据包处理函数
//输    入: u8 cmd——命令码，u8* resp_str——应答字符串, u16 len——数据长度
//输    出: u8——返回解析处理结果
//备    注: 无
//********************************************************
u8 Device_Comm_Package_Process(u8 cmd, u8* resp_str, u16 len)
{
    u16 i;
    u16 index = 0;
    char temp_str[100];
    memset(temp_str, '\0', sizeof(temp_str));

    switch(cmd)
    {
        case DEVICE_SET_SENSOR_ADJUST_CMD:   //如果是校准传感器
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果没设置成功
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:   //如果是读取传感器校准数据
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
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
            else    //如果失败
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_SET_RTC_CMD:   //如果设置RTC 
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_RTC_CMD:   //如果是读取RTC
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //如果读取成功
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
            else
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:   //如果设置IP地址
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:   //如果是读取IP地址和端口号
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
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
            else    //如果失败
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:   //如果设置数据上传时间间隔
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果读取不成功
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:   //如果读取数据上传时间间隔
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //如果读取成功
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
            else    //如果失败，则再读取一次
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:   //如果设置心跳包时间间隔
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:   //如果读取心跳包时间间隔
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //如果读取成功
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
            else    //如果失败
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:   //如果控制重启复位
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
//            memcpy(temp_str, &resp_str[1], 3);
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                                
#if (DEVICE_PRINTF_EN)
printf("没有找到“OK”\r\n");
#endif   
  
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:   //如果控制打开所有信息打印
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:   //如果控制隐藏所有信息
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:   //如果控制传感器信息打印
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
                        
#if (DEVICE_PRINTF_EN)
            printf("%s", temp_str);
            printf("\r\n");
#endif	
            
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                                
#if (DEVICE_PRINTF_EN)
printf("没有找到“OK”\r\n");
#endif   
  
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:   //如果控制隐藏传感器信息
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                                
#if (DEVICE_PRINTF_EN)
printf("没有找到“OK”\r\n");
#endif   
  
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:   //如果控制GPS信息打印
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:   //如果控制隐藏GPS信息
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //如果设置不成功
            {
                return FAILURE;
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM2_5.real_val = atoi(temp_str);    
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM2_5.label_val = atoi(temp_str);   
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM10.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.PM10.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.CO.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.CO.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO2.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO2.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.O3.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.O3.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.SO2.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.SO2.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.NO.app_val = atoi(temp_str);
            index += 2;
            
            //新版的设备没有TVOC打印！！！！！！！！！！！！2018-11-10
//            //获取SS-TVOC(9
//            //real_val
//            index += 9 + 3;
//            i = 0;
//            do
//            {
//                temp_str[i++] = resp_str[index++];
//                if(index >= len)
//                {
//                    return FAILURE;
//                }
//            }while(resp_str[index] != ',');
//            temp_str[i++] = '\0';   //加结束符
//            s_SensorData.TVOC.real_val = atoi(temp_str);
//            index += 1;
//            //label_val
//            i = 0;
//            do
//            {
//                temp_str[i++] = resp_str[index++];
//                if(index >= len)
//                {
//                    return FAILURE;
//                }
//            }while(resp_str[index] != ',');
//            temp_str[i++] = '\0';   //加结束符
//            s_SensorData.TVOC.label_val = atoi(temp_str);
//            index += 1;
//            //app_val
//            i = 0;
//            do
//            {
//                temp_str[i++] = resp_str[index++];
//                if(index >= len)
//                {
//                    return FAILURE;
//                }
//            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
//            temp_str[i++] = '\0';   //加结束符
//            s_SensorData.TVOC.app_val = atoi(temp_str);
//            index += 2;
            
            //获取SS-Temp-W
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while((resp_str[index] != '/') && (resp_str[index + 1] != 'm'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.Fan.m_freq = atoi(temp_str);
            index += 6;     //[FUN-main ]=0/min\r\n
            
            //获取FUN-pm10
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while((resp_str[index] != '/') && (resp_str[index + 1] != 'm'));
            temp_str[i++] = '\0';   //加结束符
            s_SensorData.Fan.pm10_freq = atoi(temp_str);
            index += 6;     //[FUN-pm10 ]=0/min\r\n
            
            s_SensorData.got_status = TRUE; //获得了传感器数据
            //获取日期和时间
            //示例：2018-9-28	3:26:1
            //year
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];  
                if(index >= len)
                {
                    return FAILURE;
                }
            }while((resp_str[index] != '-'));
            temp_str[i++] = '\0';   //加结束符
            s_RTC.year = atoi(temp_str);
            index += 1;  
            //month
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }    
            }while((resp_str[index] != '-'));
            temp_str[i++] = '\0';   //加结束符
            s_RTC.month = atoi(temp_str);
            index += 1;      
            //day
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }   
            }while((resp_str[index] != 0x09));  //制表位
            temp_str[i++] = '\0';   //加结束符
            s_RTC.day = atoi(temp_str);
            index += 1;   
            //hour
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];  
                if(index >= len)
                {
                    return FAILURE;
                }
            }while((resp_str[index] != ':'));
            temp_str[i++] = '\0';   //加结束符
            s_RTC.hour = atoi(temp_str);
            index += 1;
            //min
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];  
                if(index >= len)
                {
                    return FAILURE;
                }  
            }while((resp_str[index] != ':'));
            temp_str[i++] = '\0';   //加结束符
            s_RTC.min = atoi(temp_str);
            index += 1;      
            //sec
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++]; 
                if(index >= len)
                {
                    return FAILURE;
                }     
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_RTC.sec = atoi(temp_str);
                                
#if (DEVICE_PRINTF_EN)
            printf("传感器数据获取完毕！\r\n");
#endif	
            
            // 控制隐藏传感器数据的打印
            Device_Printf_Ctr(DEVICE_CTR_SENSOR_HIDE_CMD);
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
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
                if(index >= len)
                {
                    return FAILURE;
                }
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //加结束符
            s_GPSInfo.noSV = atoi(temp_str);
            index += 2;
        }
        break;
        
        default :
        
        break;
    }
    
    return SUCCEED;
}

//********************************************************
//函数名称: Device_Comm_Package_Analysis
//函数功能: 设备端数据解析函数
//输    入: u8 *data——数据, u16 data_l——数据长度
//输    出: u8——解析状态，0——成功，
//备    注: 无
//********************************************************
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l)
{
    u16  index;
    char com_str[100];  //公共字符串
    char temp_str[100];
    
    memset(com_str, '\0', sizeof(com_str));
    memset(temp_str, '\0', sizeof(com_str));
    
    for(index = 0; index < (data_l / 2); index++)
    {
        //\r\n[SS-Temp......传感器数据
        if((data[index] == '[') && (data[index + 1] == 'S') && (data[index + 2] == 'S') && (data[index + 3] == '-') && 
           (data[index + 4] == 'T') && (data[index + 5] == 'e') && (data[index + 6] == 'm') && (data[index + 7] == 'p'))
        {
            if(data_l >= 350)   //1、设备端是带操作系统的  有的数据会被其他任务切断，导致分成若干包接收到；2、外部传感器启动慢
            {
                Device_Comm_Package_Process(DEVICE_GET_SENSOR_DATA_CMD, &data[index], (data_l - index));
            }
        }
        //\r\n[GPS-STA......GPS数据
        else if((data[index] == '[') && (data[index + 1] == 'G') && (data[index + 2] == 'P') && (data[index + 3] == 'S') && 
                (data[index + 4] == '-') && (data[index + 5] == 'S') && (data[index + 6] == 'T') && (data[index + 7] == 'A'))
        {
            Device_Comm_Package_Process(DEVICE_GET_SENSOR_DATA_CMD, &data[index], (data_l - index));
        }
        //\r\n[EVENT]: Sampling start!......设备启动完毕
        else 
        {
            strcpy(temp_str, "[EVENT]: Sampling start!");
            memcpy(com_str, &data[index], strlen(temp_str));
            if(strcmp(com_str, temp_str) == SUCCEED)
            {
                g_DeviceInitialFlag = TRUE;
            }
        }
    }
    
    return SUCCEED;
}

//********************************************************
//函数名称: Device_Rec_Command_Analysis
//函数功能: 接收设备端命令类数据解析函数
//输    入: u8 cmd——命令码, u8* buf——数据内容, u16 len——数据长度
//输    出: u8——解析状态，0——成功，1——失败，2——非命令数据
//备    注: 无
//********************************************************
u8 Device_Rec_Command_Analysis(u8 cmd, u8* buf, u16 len)
{
    u16  index;
    u16  i;
    char temp_str[1024];
    u8  ana_sta = FAILURE;
   
    memset(temp_str, '\0', sizeof(temp_str));
    
    //先找到“\r\n”头
    for(index = 0; index < (len / 2); index++)
    {
        if((buf[index] == '\r') && (buf[index + 1] == '\n'))
        {
            index += 2;
            break;
        }
    }

    //等待的是什么命令
    switch(cmd)
    {
        case DEVICE_SET_SENSOR_ADJUST_CMD:
        {
            //如果是“S_SSL=”
            char cmp_str[] = "S_SSL=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:
        {
            //如果是“Q_SSL=”
            char cmp_str[] = "Q_SSL=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_SET_RTC_CMD:
        {
            //如果是“S_TIME=”
            char cmp_str[] = "S_TIME=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_RTC_CMD:
        {
            //如果是“Q_TIME=”
            char cmp_str[] = "Q_TIME=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:
        {
            //如果是“S_IP=”
            char cmp_str[] = "S_IP=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:
        {
            //如果是“Q_IP=”
            char cmp_str[] = "Q_IP=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:
        {
            //如果是“S_DATARPT=”
            char cmp_str[] = "S_DATARPT=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:
        {
            //如果是“Q_DATARPT=”
            char cmp_str[] = "Q_DATARPT=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:
        {
            //如果是“S_HEALTHRPT=”
            char cmp_str[] = "S_HEALTHRPT=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:
        {
            //如果是“Q_HEALTHRPT=”
            char cmp_str[] = "Q_HEALTHRPT=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:
        {
            //如果是“C_RESET=”
            char cmp_str[] = "C_RESET=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:
        {
            //如果是“SHOW_ALL=”
            char cmp_str[] = "SHOW_ALL=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:
        {
            //如果是“HIDE_ALL=”
            char cmp_str[] = "HIDE_ALL=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:
        {
            //如果是“SHOW_DATA”
            char cmp_str[] = "SHOW_DATA";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:
        {
            //如果是“HIDE_DATA”
            char cmp_str[] = "HIDE_DATA";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:
        {
            //如果是“SHOW_GPS=”
            char cmp_str[] = "SHOW_GPS=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:
        {
            //如果是“HIDE_GPS=”
            char cmp_str[] = "HIDE_GPS=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        default:
        break;
    }  
    
    if(ana_sta == SUCCEED)
    {
        
#if (DEVICE_PRINTF_EN)
        printf("找到了应答的命令头\r\n");
#endif   
        
        i = 0;
        do
        {
            temp_str[i++] = buf[index++];
        }while((buf[index] != '\r') && (buf[index + 1] != '\n'));
            
        return Device_Comm_Package_Process(cmd, (u8*)temp_str, (len - index));
    }
    else
    {
        
#if (DEVICE_PRINTF_EN)
        printf("没找到应答的命令头\r\n");
#endif   
        
        return 2;
    }
        
}

/*****************************************
//名称: Device_Rec_Response_Cmd_Monitor
//功能: 设备接收应答命令的管理函数
//入口: u8 cmd—命令码
//出口: u8—返回控制结果
******************************************/
u8 Device_Rec_Response_Cmd_Monitor(u8 cmd)
{
    u16 temp_l = 0;
    u8  temp_sta = FAILURE; //解析结果：成功、失败、非命令数据包（正常打印的其它数据）
    u8  times_cnt = 0;  //次数
    
    s_ServerCommRx.Timeout_Count = 0;
    while(9)
    {
        if(s_DeviceCommRx.Status == TRUE)   //有接收
        {
            if(s_DeviceCommRx.Timeout_Count >= DEVICE_COMM_RX_DATA_TIMEOUT)    //是否超时了
            {
                if((s_DeviceCommRx.Index < 5))   //如果是无效值，抛弃
                {
                    s_DeviceCommRx.Status = FALSE;
                    s_DeviceCommRx.Index = 0;
                    s_DeviceCommRx.Timeout_Count = 0;
                    
                    continue; 
                }
                temp_l = s_DeviceCommRx.Index;    //拷贝出数据长度
                //将数据拷贝至公共缓冲区，防止被新的数据淹没
                memcpy(g_PublicDataBuffer, s_DeviceCommRx.Buffer, temp_l); 
                
                s_DeviceCommRx.Status = FALSE;
                s_DeviceCommRx.Index = 0;
                s_DeviceCommRx.Timeout_Count = 0;
                
#if (DEVICE_PRINTF_EN)
                printf("接收到设备端返回命令: ");
                for(u16 i = 0; i < temp_l; i++)
                {
                    printf("%c", g_PublicDataBuffer[i]);
                }
                printf("\r\n");
#endif	
                
                //解析接收到的AT指令
                temp_sta = Device_Rec_Command_Analysis(cmd, g_PublicDataBuffer, temp_l);
                if(temp_sta == FAILURE)  //如果应答失败
                {
                    return FAILURE;
                }
                else if(temp_sta == SUCCEED)    //如果是成功了
                {
                    return SUCCEED;
                }
                else
                {
                    times_cnt++;
                    if(times_cnt >= 2)
                    {
                        return FAILURE;
                    }
                }
            }
        }
        //假如超时没有接收到想要的应答，则退出
        else 
        {
            if(s_DeviceCommRx.Timeout_Count >= DEVICE_COMM_WAIT_RESPONSE_TIMEOUT)
            {
                s_DeviceCommRx.Timeout_Count = 0;
                
                return FAILURE;
            }
        }
    }
}

/*****************************************
//名称: Device_Printf_Ctr
//功能: 设备打印输出控制函数
//入口: u8 cmd—命令码
//出口: u8—返回控制结果
******************************************/
u8 Device_Printf_Ctr(u8 cmd)
{
    u8 index;
    u8 temp_sta = FAILURE;
    
    //发送控制命令
    Device_Comm_Package_Bale(cmd);
    
    index = 0;
    while(9)
    {
        temp_sta = Device_Rec_Response_Cmd_Monitor(cmd);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重发一次
                Device_Comm_Package_Bale(cmd);
            }
        }
        else    //如果成功了，则退出
        {
            break;
        }
    }
    
    return SUCCEED;
}

/*****************************************
//名称: Device_Initial
//功能: 设备端初始化函数
//入口: 无
//出口: u8—返回控制结果
******************************************/
u8 Device_Initial(void)
{
    u32 temp_timing_count = 0;
    // 清除一次设备端的接收数据，防止初始化无线模块的时候接收太多
    s_DeviceCommRx.Status = FALSE;
    s_DeviceCommRx.Index = 0;
    s_DeviceCommRx.Timeout_Count = 0;
    
    // 先复位设备
    Device_Printf_Ctr(DEVICE_CTR_RESET_CMD);
    
    // 判断设备启动状态，检测“[EVENT]: Sampling start!”
    temp_timing_count = g_ms_Timing_Count;
    while(g_DeviceInitialFlag == FALSE)
    {
        Device_Comm_Rec_Monitor();
        
        if(abs(g_ms_Timing_Count - temp_timing_count) > DEVICE_INITIAL_OVER_TIMEOUT)
        {
            return FAILURE;
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
            printf("接收到设备端%d个数据: ", temp_l);
            for(u16 i = 0; i < temp_l; i++)
            {
                printf("%c", g_PublicDataBuffer[i]);
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
