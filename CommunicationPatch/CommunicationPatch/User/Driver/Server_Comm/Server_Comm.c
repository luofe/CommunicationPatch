
/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
// Functiom:项目工程负责与服务器通信的源程序。
*******************************************************************************/

/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//变量定义
*******************************************************************************/
//用于与服务器通信的接收数据结构体
ServerCommRxStruct s_ServerCommRx;

//与服务器通信的数据包结构体
ServerCommPackageStruct s_ServerCommPackage;

//与服务器通信的发送数据的结构体定义
ServerCommTxStruct s_ServerCommTx;

// 传感器参数设置
SystemParaStruct s_SystemPara;

// SIM卡参数结构体声明
SIMCardParaStruct s_SIMCardPara;

// 接收报文序列号
u8 g_RecServerPackageSN[2] = {0x00,0x01};

// 接收报文解析处理结果
u8  g_RecServerPackageResult = SUCCEED;

// 最新一次发送的命令码
u16 g_LastSendServerCmd = SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE; //默认是握手包


uc16 CRC_TABLE_XW[256] =
{
    /* CRC查找表 */
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040,
};








/******************************************************************************
//函数声明
*******************************************************************************/
//功能:	与服务器通信相关的初始化函数
void Server_Comm_Init(void);

//功能:	与服务器通信的USART的初始化函数
void Server_Comm_Usart_Init(void);

//功能: 与服务器通信的发送数据函数
void Server_Comm_Send_Data(u8 *buffer, u16 data_l);

//函数功能: 与服务器通信的数据包CRC16校验码生成
u16 GetCheckCRC_XW(u8 *pData, u16 len);

//函数功能: 与服务器通信的数据包发送函数
void Server_Comm_Package_Send(void);

//函数功能: 与服务器通信的数据包打包函数
void Server_Comm_Package_Bale(u16 cmd);

//函数功能: 与服务器通信的数据包处理函数
void Server_Comm_Package_Process(u16 cmd, u8* data, u16 len);

//函数功能: 与服务器通信的数据包解析函数
u8 Server_Comm_Package_Analysis(u8 *rec_array, u16 rec_length);

//函数功能: 与服务器通信的监测函数
void Server_Comm_Rec_Monitor(void);

//函数功能: 与服务器通信的错误处理函数
void Server_Comm_Error_Process(void);

//功能: 服务器对设备端的固件升级的函数
u8 Server_Comm_Upgrade_Pullout_Board(void);
















//功能: 与服务器通信的测试函数
void Server_Comm_Test(void);

/******************************************************************************
//函数定义
*******************************************************************************/
/**********************************
//名称:	Server_Comm_Init
//功能:	与服务器通信相关的初始化函数
//入口参数:	无
//出口参数:	无
**********************************/
void Server_Comm_Init(void)
{
    Server_Comm_Usart_Init();
}

/**********************************
//名称:	Server_Comm_Usart_Init
//功能:	与服务器通信的USART的初始化函数
//入口参数:	无
//出口参数:	无
**********************************/
void Server_Comm_Usart_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	//初始化NVIC结构体
    
    //复位SERVER_COMM_USART模块
    USART_DeInit(SERVER_COMM_USART);
    
    //使能SERVER_COMM_USART_TX引脚模块时钟
    RCC_APB2PeriphClockCmd(SERVER_COMM_USART_TX_GPIO_CLK, ENABLE);
    
    //使能SERVER_COMM_USART_RX引脚模块时钟
    RCC_APB2PeriphClockCmd(SERVER_COMM_USART_RX_GPIO_CLK, ENABLE);
    
    //使能USART模块时钟
    if(SERVER_COMM_USART == USART1)
    {
        RCC_APB2PeriphClockCmd(SERVER_COMM_USART_CLK, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(SERVER_COMM_USART_CLK, ENABLE);
    }
    
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    //SERVER_COMM_USART的GPIO配置
    //SERVER_COMM_USART_TX: 推挽复用输出
    GPIO_InitStructure.GPIO_Pin   = SERVER_COMM_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //推挽复用输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//选择50MHz
    GPIO_Init(SERVER_COMM_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    //SERVER_COMM_USART_RX: 浮空输入(或带上拉输入)
    GPIO_InitStructure.GPIO_Pin   = SERVER_COMM_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(SERVER_COMM_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    
    //SERVER_COMM_USART模块参数配置
    //波特率: USART1_BAUDRATE；8个数据位；1个停止位；无校验位；无硬件流控制；使能发送和接收；
    USART_InitStructure.USART_BaudRate            = SERVER_COMM_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(SERVER_COMM_USART, &USART_InitStructure); 
    
    USART_Cmd(SERVER_COMM_USART, ENABLE);                         //使能SERVER_COMM_USART模块
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//使用优先级分组2	
    NVIC_InitStructure.NVIC_IRQChannel                      = SERVER_COMM_USART_IRQn; //使能USART中断	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;                //抢断优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;                //响应优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	//配置USART的嵌套向量中断
    
	USART_ITConfig(SERVER_COMM_USART, USART_IT_RXNE, DISABLE);	//先禁止接收中断     
}

/*****************************************
//名称: Server_Comm_Send_Data
//功能: 与服务器通信的发送数据函数
//入口: u8 *buffer——需要发送的数据;u16 data_l——数据长度
//出口: 无
******************************************/
void Server_Comm_Send_Data(u8 *buffer, u16 data_l)
{  
    
#if (SERVER_PRINTF_EN)
    printf("\r\n发送数据给服务器:");
#endif
    
    while(data_l--)  
    {
        
#if (SERVER_PRINTF_EN)
        printf("%02X ", *buffer);
#endif
        
        USART_SendData(SERVER_COMM_USART, *buffer++); 
        while(USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_TXE) == RESET);//等待发送完成
    } 
    
#if (SERVER_PRINTF_EN)
    printf("\r\n");
#endif
    
}

//********************************************************
//函数名称: GetCheckCRC_XW
//函数功能: 与服务器通信的数据包CRC16校验码生成
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
u16 GetCheckCRC_XW(u8 *pData, u16 len)
{
    u16 i=0;
    u16 crc = 0xffff;
    
    for ( i = 0; i < len; i++ )
    {
        crc = ( crc >> 8 ) ^ CRC_TABLE_XW[( crc ^ pData[i] ) & 0xff];
    }
    return __REV16(crc);	// 取反操作
}

//********************************************************
//函数名称: Server_Comm_Package_Send
//函数功能: 与服务器通信的数据包发送函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void Server_Comm_Package_Send(void)
{
    u16 temp_crc;
    
    s_ServerCommTx.Index = 0;
    
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = s_ServerCommPackage.Head;
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = s_ServerCommPackage.Identify;
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)(s_ServerCommPackage.Length >> 8);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)s_ServerCommPackage.Length;
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)(s_ServerCommPackage.ADF.SN >> 8);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)s_ServerCommPackage.ADF.SN;
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)(s_ServerCommPackage.ADF.CMD >> 8);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)s_ServerCommPackage.ADF.CMD;
    g_LastSendServerCmd = s_ServerCommPackage.ADF.CMD;  //保留本次发送的命令码
    memcpy(&s_ServerCommTx.Buffer[s_ServerCommTx.Index], s_ServerCommPackage.ADF.Data, (s_ServerCommPackage.Length - 6));
    s_ServerCommTx.Index += s_ServerCommPackage.Length - 6;
    temp_crc = GetCheckCRC_XW(&s_ServerCommTx.Buffer[4], s_ServerCommTx.Index - 4);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)(temp_crc >> 8);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)temp_crc;
    
    Server_Comm_Send_Data(s_ServerCommTx.Buffer, s_ServerCommTx.Index);
    
    if(s_ServerCommPackage.ADF.SN == 0xFFFF)    //
    {
        s_ServerCommPackage.ADF.SN = 1;     //流水号清零
    }
    else
    {
        s_ServerCommPackage.ADF.SN++;                                                   //流水号递增
    }
}

//********************************************************
//函数名称: Server_Comm_Package_Bale
//函数功能: 与服务器通信的数据包打包函数
//输    入: u16 cmd——命令码
//输    出: 无
//备    注: 无
//********************************************************
void Server_Comm_Package_Bale(u16 cmd)
{
    u16 i;
    s_ServerCommPackage.ADF.CMD = cmd;
    switch(cmd)
    {
        case SERVER_COMM_PACKAGE_CMD_RESPONSE:   //通用应答
        {
            i = 0;
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageSN[0];
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageSN[1];
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageResult;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE:   //握手
        {
            i = 0;
            //放入RTC时间
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //协议类型
            s_ServerCommPackage.ADF.Data[i++] = s_SystemPara.proc_type;
            //协议厂商
            s_ServerCommPackage.ADF.Data[i++] = s_SystemPara.manu_type;
            //设备类型
            s_ServerCommPackage.ADF.Data[i++] = s_SystemPara.device_type;
            //设备编号
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SystemPara.deviceID, 4);
            i += 4; 
            //CODE码
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SystemPara.Code, 8);
            i += 8;  
            //软件版本
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SystemPara.Ver, 5);
            i += 5;
            // SIM卡号
            s_ServerCommPackage.ADF.Data[i++] = s_SIMCardPara.num_len;
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SIMCardPara.num, s_SIMCardPara.num_len);
            i += s_SIMCardPara.num_len;
            //SIM卡的IMSI码
            s_ServerCommPackage.ADF.Data[i++] = s_SIMCardPara.IMSI_len;
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SIMCardPara.IMSI, s_SIMCardPara.IMSI_len);
            i += s_SIMCardPara.IMSI_len;
            //SIM卡的IMEI码
            s_ServerCommPackage.ADF.Data[i++] = s_SIMCardPara.IMEI_len;
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SIMCardPara.IMEI, s_SIMCardPara.IMEI_len);
            i += s_SIMCardPara.IMEI_len;
            //SIM卡的CCID码
            s_ServerCommPackage.ADF.Data[i++] = s_SIMCardPara.CCID_len;
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SIMCardPara.CCID, s_SIMCardPara.CCID_len);
            i += s_SIMCardPara.CCID_len;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_HEARTBEAT:  //心跳
        {
            i = 0;
            //放入RTC时间
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_DATA:   //上报传感器数据
        {
            u8  sen_num_index;
            i = 0;
            //放入RTC时间
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //设备状态字
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SensorData.device_sta, 4);
            i += 4;
            //电池电压
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_DevicePower.vol, 2);
            i += 2;
            //传感器个数
            sen_num_index = i;
            s_SensorData.sensor_num = 0;
            i++;
            //以下都是传感器数据
            //PM2.5
            s_ServerCommPackage.ADF.Data[i++] = 0x00;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM2_5.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM2_5.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM2_5.app_val;
            s_SensorData.sensor_num++;
            //PM10
            s_ServerCommPackage.ADF.Data[i++] = 0x02;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM10.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM10.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM10.app_val;
            s_SensorData.sensor_num++;
            //CO
            s_ServerCommPackage.ADF.Data[i++] = 0x03;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.CO.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.CO.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.CO.app_val;
            s_SensorData.sensor_num++;
            //NO2
            s_ServerCommPackage.ADF.Data[i++] = 0x04;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO2.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO2.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO2.app_val;
            s_SensorData.sensor_num++;
            //O3
            s_ServerCommPackage.ADF.Data[i++] = 0x05;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.O3.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.O3.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.O3.app_val;
            s_SensorData.sensor_num++;
            //SO2
            s_ServerCommPackage.ADF.Data[i++] = 0x06;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.SO2.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.SO2.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.SO2.app_val;
            s_SensorData.sensor_num++;
            //NO
            s_ServerCommPackage.ADF.Data[i++] = 0x07;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO.app_val;
            s_SensorData.sensor_num++;
            
            // 新版本的设备没有TVOC。。。2018-11-10
//            //TVOC
//            s_ServerCommPackage.ADF.Data[i++] = 0x09;  
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.real_val >> 24);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.real_val >> 16);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.real_val >> 8);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TVOC.real_val;
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.label_val >> 24);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.label_val >> 16);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.label_val >> 8);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TVOC.label_val;
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.app_val >> 24);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.app_val >> 16);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.app_val >> 8);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TVOC.app_val;
//            s_SensorData.sensor_num++;
            //温湿度（内）
            s_ServerCommPackage.ADF.Data[i++] = 0x60;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.temp >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.temp >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.temp >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TRH.temp;
            for(u8 j = 0; j < 8; j++)   //实验室校准值和应用校准值填充0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x61; 
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.humi >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.humi >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.humi >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TRH.humi;
            for(u8 j = 0; j < 8; j++)   //实验室校准值和应用校准值填充0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            //外部传感器
            s_ServerCommPackage.ADF.Data[i++] = 0x62;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.wd >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.wd >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.wd >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.wd;
            for(u8 j = 0; j < 8; j++)   //实验室校准值和应用校准值填充0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x63; 
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.ws >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.ws >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.ws >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.ws;
            for(u8 j = 0; j < 8; j++)   //实验室校准值和应用校准值填充0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x64;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.temp >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.temp >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.temp >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.temp;
            for(u8 j = 0; j < 8; j++)   //实验室校准值和应用校准值填充0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x65; 
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.humi >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.humi >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.humi >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.humi;
            for(u8 j = 0; j < 8; j++)   //实验室校准值和应用校准值填充0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x66; 
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.pa >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.pa >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.pa >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.pa;
            for(u8 j = 0; j < 8; j++)   //实验室校准值和应用校准值填充0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            //最后放入传感器个数
            s_ServerCommPackage.ADF.Data[sen_num_index] = s_SensorData.sensor_num;
            
            if((g_SysInitStatusFlag == TRUE) && (g_WireLessModuleInitFlag == TRUE))   //初始化完毕,无线模块也初始化完成了
            {
                //数据长度
                s_ServerCommPackage.Length = i + 6;
                Server_Comm_Package_Send();
            }
            else
            {
                                        
#if (SERVER_PRINTF_EN)
        printf("将传感器上传数据包存储到Flash中\r\n");
#endif
        
                u8  temp_array[256];
                temp_array[0] = 0x02;                       //数据包类型
                temp_array[1] = (u8)s_ServerCommPackage.Length; //数据包长度
                memcpy(&temp_array[2], s_ServerCommPackage.ADF.Data, i);
                
                Data_Storge_Process(s_ServerCommPackage.ADF.Data, (i + 2));
            }
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_PARA:   //上报参数信息
        {
            i = 0;
            //放入RTC时间
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime; 
            //放入之前的查询包的流水号
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageSN[0];
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageSN[1];
            // 放入参数类型
            s_ServerCommPackage.ADF.Data[i++] = s_SystemPara.para_type;
            switch(s_SystemPara.para_type)
            {
                case SYSTEM_PARA_TYPE_IP_ADDR_PORT: //IP地址设置
                {  
                    // 放入参数长度
                    s_ServerCommPackage.ADF.Data[i++] = strlen((const char*)s_IPAddrPort.ip_port); 
                    //放入IP地址
                    memcpy(&s_ServerCommPackage.ADF.Data[i], s_IPAddrPort.ip_port, strlen((const char*)s_IPAddrPort.ip_port));
                    i += strlen((const char*)s_IPAddrPort.ip_port);
                }
                break;
                
                case SYSTEM_PARA_TYPE_UPLOAD_INTERVAL:  //通用数据上传间隔
                {
                    // 放入参数长度
                    s_ServerCommPackage.ADF.Data[i++] = 4; 
                    //放入通用数据上传间隔
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_UploadInterval.time1 >> 24);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_UploadInterval.time1 >> 16);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_UploadInterval.time1 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_UploadInterval.time1;
                }
                break;
                
                case SYSTEM_PARA_TYPE_HEARTBEAT_INTERVAL:   //心跳间隔
                {
                    // 放入参数长度
                    s_ServerCommPackage.ADF.Data[i++] = 2; 
                    //放入心跳间隔
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_UploadInterval.heartbeat >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_UploadInterval.heartbeat;
                }
                break;
                
                case SYSTEM_PARA_TYPE_LAB_ADJUST:   //实验室校准
                {
                    // 放入参数长度
                    s_ServerCommPackage.ADF.Data[i++] = 11; 
                    // 放入传感器类型
                    s_ServerCommPackage.ADF.Data[i++] = s_SensorAdj.type;
                    // 放入缩小系数
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_SensorAdj.multiple >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.multiple;
                    // 放入K1
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.K1 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.K1;
                    // 放入B1
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.B1 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.B1;
                    // 放入K2
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.K2 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.K2;
                    // 放入B2
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.B2 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.B2;
                }
                break;
                
                case SYSTEM_PARA_TYPE_APP_ADJUST:   //应用校准
                {
                    // 放入参数长度
                    s_ServerCommPackage.ADF.Data[i++] = 11; 
                    // 放入传感器类型
                    s_ServerCommPackage.ADF.Data[i++] = s_SensorAdj.type;
                    // 放入缩小系数
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_SensorAdj.multiple >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.multiple;
                    // 放入K3
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.K3 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.K3;
                    // 放入B3
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.B3 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.B3;
                    // 放入K4
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.K4 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.K4;
                    // 放入B4
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.B4 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.B4;
                }
                break;
                
                default:
                break;
            }
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_GPS:   //上报GPS
        {
            i = 0;
            //放入帧数 
            s_ServerCommPackage.ADF.Data[i++] = 1;
            //时间
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //经度
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.longitude >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.longitude >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.longitude >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.longitude;
            //纬度
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.latitude >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.latitude >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.latitude >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.latitude;
            //速度
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.speed >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.speed;
            //方向
            s_ServerCommPackage.ADF.Data[i++] = s_GPSInfo.heading;
            //高度
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.high >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.high >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.high >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.high;
            //绑定状态
            s_ServerCommPackage.ADF.Data[i++] = s_GPSInfo.fs;
            //星数
            s_ServerCommPackage.ADF.Data[i++] = s_GPSInfo.noSV;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_FLASH:   //上报片外Flash的数据包
        {
            u8  Rx_Buffer[256];
            
//            if(g_DataPageNum > 4)   //太多了会超过可发送缓存
//            {
//                                        
//#if (SERVER_PRINTF_EN)
//        printf("Flash中存储的帧数=%d\r\n", g_DataPageNum);
//#endif
//        
//                return;
//            }
            
            i = 0;
            //时间
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //放入帧数     
            s_ServerCommPackage.ADF.Data[i++] = 1;
            //读取数据包
            SPI_FLASH_BufferRead(Rx_Buffer, (g_DataPageNum * 256), sizeof(Rx_Buffer));
            
            //放入数据包类型 
            s_ServerCommPackage.ADF.Data[i++] = Rx_Buffer[0];
            //放入数据包长度
            s_ServerCommPackage.ADF.Data[i++] = Rx_Buffer[1];
            //放入流水号
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_ServerCommPackage.ADF.SN >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_ServerCommPackage.ADF.SN >> 0);
            //放入命令码
            switch(Rx_Buffer[0])
            {
                case EXTFLASH_PACKAGE_TYPE_REPORT_DATA: //如果存储的数据包是传感器数据上传
                {
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(SERVER_COMM_PACKAGE_CMD_REPORT_DATA >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(SERVER_COMM_PACKAGE_CMD_REPORT_DATA >> 0);
                }
                break;
            }
            //放入数据内容
            for(u16 j = 0; j < Rx_Buffer[1]; j++)
            {
                s_ServerCommPackage.ADF.Data[i++] = Rx_Buffer[j + 2];
            }
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
            
            s_ServerCommTx.WaitResponse = NEED_RESPONSE;    //等待应答
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_RPT_RESET:   //上报复位
        {
            i = 0;
            //时间
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //复位原因
            s_ServerCommPackage.ADF.Data[i++] = 0x04;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        default :
        
        break;
    }
}

//********************************************************
//函数名称: Server_Comm_Package_Process
//函数功能: 与服务器通信的数据包处理函数
//输    入: u8 cmd——命令码, u8* data——数据内容, u16 len——数据长度
//输    出: 无
//备    注: 无
//********************************************************
void Server_Comm_Package_Process(u16 cmd, u8* data, u16 len)
{
    u16 i;
    
    switch(cmd)
    {
        case SERVER_COMM_PACKAGE_CMD_RESPONSE:   //通用应答
        {
            i = 0;
            //获取流水号
            u16  temp_sn = data[i++];
            temp_sn <<= 8;
            temp_sn += data[i++];
            //判断应答结果
            if(data[i++] == SUCCEED)
            {
                if(g_SysInitStatusFlag == FALSE) //如果还没有握手
                {
                    
#if (SERVER_PRINTF_EN)
                    printf("握手完毕\r\n");
#endif   
                    
                    g_SysInitStatusFlag = TRUE;
                }
                if(g_SysInitStatusFlag == TRUE)    //如果已经握手完毕
                {      
                    if(s_ServerCommPackage.ADF.CMD == SERVER_COMM_PACKAGE_CMD_REPORT_FLASH)
                    {
                        s_ServerCommTx.WaitResponse = DONT_RESPONSE;    //等待应答标志复位
                        s_ServerCommTx.RepeatNum = 0;   //重发次数复位
                        g_DataPageNum--;
                    }
                }
                
                
                //如果有数据包存储到片外Flash
                if((g_DataPageNum >= SENSOR_DATA_MIN_PAGE_NUM) && (g_DataPageNum < 0xFFFF))
                {
                    
#if (SERVER_PRINTF_EN)
                    printf("g_DataPageNum=%d,共%d帧\r\n", g_DataPageNum, ((g_DataPageNum - SENSOR_DATA_MIN_PAGE_NUM) + 1));
#endif   
                    
                    Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_FLASH);
                }
            }
            else
            {
                //判断是否要重发
            }
        }
        break;
        case SERVER_COMM_PACKAGE_CMD_SET_PARA:   //设置参数
        {        
            u8 temp_len;    //参数长度    
            i = 0;
            //获取参数类型
            s_SystemPara.para_type = data[i++];
            temp_len = data[i++];
            switch(s_SystemPara.para_type)
            {
                case SYSTEM_PARA_TYPE_IP_ADDR_PORT: //IP地址设置
                {
                    memset(s_IPAddrPort.ip_port, 0, sizeof(s_IPAddrPort.ip_port));
                    memcpy(s_IPAddrPort.ip_port, &data[i], temp_len); //获取IP地址
                    
                    
#if (SERVER_PRINTF_EN)
                    printf("设置的IP地址=%s\r\n", s_IPAddrPort.ip_port);
#endif   
                    
                    //给设备下发IP地址
                    Device_Printf_Ctr(DEVICE_SET_IP_ADDR_CMD);
                    Delay_ms(500);
                    // 获取IP端口
                    Device_Printf_Ctr(DEVICE_READ_IP_ADDR_CMD);
                    
                    //复位无线模块初始化状态和整机初始化状态，以让无线模块重新初始化
                    g_WireLessModuleInitFlag = FALSE;
                    g_SysInitStatusFlag = FALSE;
                    
                    //无线模块重新初始化
//                    WireLess_Initial();
                    
//                    //设置IP和端口到无线模块里
//                    //先退出透传模式进入命令模式
//                    if(WireLess_AT_Command_Ctr(AT_COMMAND_SWITCH_CMD) == FAILURE)
//                    {
//                        g_RecServerPackageResult = RES_FAILURE;
//                        break;
//                    }
//                    //关闭连接
//                    if(WireLess_AT_Command_Ctr(AT_COMMAND_QICLOSE) == FAILURE)
//                    {
//                        g_RecServerPackageResult = RES_FAILURE;
//                        break;
//                    }
//                    //建立连接
//                    if(WireLess_AT_Command_Ctr(AT_COMMAND_QIOPEN) == FAILURE)
//                    {
//                        g_RecServerPackageResult = RES_FAILURE;
//                        break;
//                    }
                    g_RecServerPackageResult = RES_SUCCEED;
                }
                break;
                
                case SYSTEM_PARA_TYPE_TIME_SYNCH: //系统时间同步
                {
                    s_GPSInfo.gmtTime = 0;
                    while(temp_len--)
                    {
                        s_GPSInfo.gmtTime <<= 8;
                        s_GPSInfo.gmtTime += data[i++];
                    }
                    g_RecServerPackageResult = RES_SUCCEED;
                }
                break;
                
                case SYSTEM_PARA_TYPE_UPLOAD_INTERVAL:  //通用数据上传间隔
                {
                    s_UploadInterval.time1 = data[i++];
                    s_UploadInterval.time1 <<= 8;
                    s_UploadInterval.time1 += data[i++];
                    s_UploadInterval.time1 <<= 8;
                    s_UploadInterval.time1 += data[i++];
                    s_UploadInterval.time1 <<= 8;
                    s_UploadInterval.time1 += data[i++];
                    
#if (SERVER_PRINTF_EN)
                    printf("s_UploadInterval.time1=%d\r\n", s_UploadInterval.time1);
#endif   
                    
                    if(Device_Printf_Ctr(DEVICE_SET_UPLOAD_INTERVAL_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_HEARTBEAT_INTERVAL:   //心跳间隔
                {
                    s_UploadInterval.heartbeat = data[i++];
                    s_UploadInterval.heartbeat <<= 8;
                    s_UploadInterval.heartbeat += data[i++];
                    
                    if(Device_Printf_Ctr(DEVICE_SET_HEARTBEAT_INTERVAL_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_LAB_ADJUST:   //实验室校准
                {
                    u16 temp_data = 0;
                    
                    s_SensorAdj.type = data[i++];
                    
                    s_SensorAdj.multiple = data[i++]; 
                    s_SensorAdj.multiple <<= 8;
                    s_SensorAdj.multiple += data[i++];
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K1 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B1 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K2 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B2 = (s16)temp_data;
                    
                    if(Device_Printf_Ctr(DEVICE_SET_SENSOR_LAB_ADJUST_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_APP_ADJUST:   //应用校准
                {
                    u16 temp_data = 0;
                    
                    s_SensorAdj.type = data[i++];
                    
                    s_SensorAdj.multiple = data[i++]; 
                    s_SensorAdj.multiple <<= 8;
                    s_SensorAdj.multiple += data[i++];
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K3 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B3 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K4 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B4 = (s16)temp_data;
                    
                    if(Device_Printf_Ctr(DEVICE_SET_SENSOR_APP_ADJUST_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_BATCH_APP_ADJUST:   //批量应用校准
                {
                    u16 temp_data = 0;
                    
                    s_SensorAdj.type = data[i++];
                    
                    s_SensorAdj.multiple = data[i++]; 
                    s_SensorAdj.multiple <<= 8;
                    s_SensorAdj.multiple += data[i++];
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K3 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B3 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K4 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B4 = (s16)temp_data;
                    
                    if(Device_Printf_Ctr(DEVICE_SET_SENSOR_APP_ADJUST_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_BATCH_LAB_ADJUST:   //批量实验室校准
                {
                    u16 temp_data = 0;
                    
                    s_SensorAdj.type = data[i++];
                    
                    s_SensorAdj.multiple = data[i++]; 
                    s_SensorAdj.multiple <<= 8;
                    s_SensorAdj.multiple += data[i++];
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K1 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B1 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K2 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B2 = (s16)temp_data;
                    
                    if(Device_Printf_Ctr(DEVICE_SET_SENSOR_LAB_ADJUST_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                default:
                break;
            }
            //通用应答
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RESPONSE);
            Delay_ms(500);  //延时一段时间，避免重新配置无线模块导致发送失败
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_READ_PARA:   //读取信息
        {
            //先通用应答
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RESPONSE);
            i = 0;
            //获取参数类型
            s_SystemPara.para_type = data[i++];
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_PARA);
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_CTR_RESET:   //控制复位
        {            
            if(Device_Printf_Ctr(DEVICE_CTR_RESET_CMD) == SUCCEED)
            {
                g_RecServerPackageResult = RES_SUCCEED;
                //通用应答
                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RESPONSE);
                Delay_ms(500);  //防止连包
                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RPT_RESET);
                Delay_ms(500);  //延时一段时间，避免重新配置无线模块导致发送失败
                
                __set_FAULTMASK(1);      // 关闭所有中断
                NVIC_SystemReset();     // 复位
            }
            else
            {
                g_RecServerPackageResult = RES_FAILURE;
                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RESPONSE);
            }
        }
        break;
        
        default :
        
        break;
    }
}

//********************************************************
//函数名称: Server_Comm_Package_Analysis
//函数功能: 与服务器通信的数据包解析函数
//输    入: u8 *rec_array——数据内容, u16 rec_length——数据长度
//输    出: 返回解析状态
//备    注: 无
//********************************************************
u8 Server_Comm_Package_Analysis(u8 *rec_array, u16 rec_length)
{
	u16 i = 0;
    u8  temp_data = 0;
    u16 temp_index = 0;        //数据索引
	u8  data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_HEAD;	    //数据分析状态
    u8  packet_analysis_error_status = PACKAGE_ANALYSIS_SUCCEED;    //数据包解析错误状态
    
    ServerCommPackageStruct *p_Package = (ServerCommPackageStruct*)malloc(sizeof(ServerCommPackageStruct));
    if(p_Package == NULL) 
    {
                 
#if (SERVER_PRINTF_EN)
        printf("内存空间不足，无法解析！！！\r\n");
#endif	
        
        return PACKAGE_ANALYSIS_UNKNOWN_ERROR;
    }
         
#if (SERVER_PRINTF_EN)
    printf("接收到服务器数据: ");
#endif	
    
	for(temp_index = 0; temp_index < rec_length; temp_index++)        //
	{
		temp_data = rec_array[temp_index];  //取出接收缓存区数据
        
#if (SERVER_PRINTF_EN)
        printf("%02X ",temp_data);  // 
#endif

		switch(data_analysis_status)    //根据查找状态处理
		{
			case SERVER_COMM_PACKAGE_ANALYSIS_HEAD:	    //如果是包头
            {
                if(temp_data == SERVER_COMM_PACKAGE_HEAD)   //如果是包头
                {   
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_IDENT; //接收状态转为接收标识符
                }
                else
                {
                    packet_analysis_error_status = PACKAGE_ANALYSIS_HEAD_ERROR; //包头错误
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_IDENT:	        //如果是标识符
            {
                if(temp_data == SERVER_COMM_PACKAGE_IDENTIFY)   //如果是标识符
                {   
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_LEN; //接收状态转为接收数据长度
                    i = 0;
                    p_Package->Length = 0;
                }
                else
                {
                    packet_analysis_error_status = PACKAGE_ANALYSIS_IDENT_ERROR; //标识符错误
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_LEN:    //如果是数据长度
            {
                p_Package->Length += temp_data; 
                i++;
                if(i == 2)
                {     
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_SN; //接收状态转为接收数据体流水号
                    i = 0;
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_SN:          //如果是流水号
            {
                g_RecServerPackageSN[i++] = temp_data; 
                if(i == 2)
                {  
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_CMD; //接收状态转为接收数据体命令码
                    i = 0;
                    p_Package->ADF.CMD = 0;
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_CMD:          //如果是命令码
            {
                p_Package->ADF.CMD <<= (i * 8);
                p_Package->ADF.CMD += temp_data; 
                i++;
                if(i == 2)
                {     
                    if(p_Package->Length == 6)      //没有数据内容的数据包
                    {
                        data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_CRC; //接收状态转为校验码
                        p_Package->ADF.Crc = 0;
                    }
                    else
                    {
                        data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_DATA; //接收状态转为接收数据体数据内容
                    }
                    i = 0;
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_DATA:          //如果是数据内容
            {
                p_Package->ADF.Data[i++] = temp_data;      //获取数据内容  
                if(i == (p_Package->Length - 6)) //接收长度够了
                {   
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_CRC; //接收状态转为校验码
                    p_Package->ADF.Crc = 0;
                    i = 0;
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_CRC:          //如果是校验码
            {
                p_Package->ADF.Crc <<= (i * 8);
                p_Package->ADF.Crc += temp_data; 
                i++;
                if(i == 2)
                {
                    i = 0;
                    if(p_Package->ADF.Crc == GetCheckCRC_XW(&rec_array[temp_index - p_Package->Length + 1], (p_Package->Length - 2)))  //假如校验码相等
                    {      
                        
#if (SERVER_PRINTF_EN)
                        printf("数据包解析成功\r\n");
#endif
                        
                        g_RecServerPackageResult = SUCCEED; //接收的数据包解析成功
                        //开始处理接收到的数据包
                        Server_Comm_Package_Process(p_Package->ADF.CMD, p_Package->ADF.Data, (p_Package->Length - 6));
                        
                        data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_HEAD;	//状态转为查找包头
                    }
                    else        //否则校验码错误
                    {
                        packet_analysis_error_status = PACKAGE_ANALYSIS_CRC_ERROR; //校验码错误
                        //释放空间
                        free(p_Package); 
                        p_Package = NULL;
                        
                        return packet_analysis_error_status;
                    }
                }
            }
            break;
            
			default:
            {
                data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_HEAD;	//状态转为查找包头
                packet_analysis_error_status = PACKAGE_ANALYSIS_UNKNOWN_ERROR;
            }
            break;
		}
	}
    
    //如果解析失败
    if(packet_analysis_error_status != PACKAGE_ANALYSIS_SUCCEED)
    {
        char com_str[50];  //公共字符串
        char const temp_str[] = "NO CARRIE";
        
        for(temp_index = 0; temp_index < rec_length; temp_index++)
        {
            //"NO CARRIER"
            memcpy(com_str, &rec_array[temp_index], strlen(temp_str));
            com_str[strlen(temp_str)] = '\0';  //放入结束符
            if(strcmp(com_str, temp_str) == SUCCEED)
            {
                //无线模块复位为未初始化
                g_WireLessModuleInitFlag = FALSE;
                //系统初始化状态复位为未初始化
                g_SysInitStatusFlag = FALSE;
            }
        }
    }
    //释放空间
    free(p_Package); 
    p_Package = NULL;
                        
    return packet_analysis_error_status;
}

//********************************************************
//函数名称: Server_Comm_Rec_Monitor
//函数功能: 与服务器通信的监测函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void Server_Comm_Rec_Monitor(void)
{
    if(s_ServerCommRx.Status == TRUE)   //有接收
    {
        if(s_ServerCommRx.Timeout_Count >= SERVER_COMM_RX_DATA_TIMEOUT)    //是否超时了
        {
            u16 temp_l = s_ServerCommRx.Index;    //拷贝出数据长度
            //将数据拷贝至公共缓冲区，防止被新的数据淹没
            memcpy(g_PublicDataBuffer, s_ServerCommRx.Buffer, temp_l); 
            
            s_ServerCommRx.Status = FALSE;
            s_ServerCommRx.Index = 0;
            s_ServerCommRx.Timeout_Count = 0;
            
            //解析数据包如果不成功，则返回对方错误
            if(Server_Comm_Package_Analysis(g_PublicDataBuffer, temp_l) != PACKAGE_ANALYSIS_SUCCEED)   //u8 i = 
            {
                //                  Server_Comm_Error_Process();
            }
            
            //#if (SERVER_PRINTF_EN)
            //printf("\r\n");
            //printf("数据包解析 %d\r\n", i);
            //#endif
            
        }
    }
    //假如很长时间没有接收到服务器的数据，说明服务器要么掉电关机了，要么串口线出故障了，则重新进入初始化流程
    else if(s_ServerCommRx.Timeout_Count >= SERVER_COMM_NO_DATA_REC_TIMEOUT)
    {
        s_ServerCommRx.Timeout_Count = 0;
        g_SysInitStatusFlag = FALSE;
    }
}

//********************************************************
//函数名称: Server_Comm_Error_Process
//函数功能: 与服务器通信的错误处理函数
//输    入: 无
//输    出: 无
//备    注: 无
//********************************************************
void Server_Comm_Error_Process(void)
{
    u8 temp_status = s_ServerCommTx.WaitResponse;   //保存等待应答状态
    
    s_ServerCommTx.WaitResponse = DONT_RESPONSE;    //等待应答标志复位
    
    s_ServerCommPackage.ADF.Data[0] = FAILURE;
    s_ServerCommPackage.Length = 9;     //放入数据长度
    
    Server_Comm_Package_Send();
    
    s_ServerCommTx.WaitResponse = temp_status;   //恢复等待应答状态
}

/*****************************************
//名称: Server_Comm_Test
//功能: 与服务器通信的测试函数
//入口: 无
//出口: 无
******************************************/
void Server_Comm_Test(void)
{  
    if(s_ServerCommRx.Status == TRUE) //如果有接收
    {
        if(s_ServerCommRx.Timeout_Count > SERVER_COMM_RX_DATA_TIMEOUT)        //如果超时没有收到数据了，说明接收完毕
        {
            Server_Comm_Send_Data(s_ServerCommRx.Buffer, s_ServerCommRx.Index);
            
            s_ServerCommRx.Status = FALSE;
            s_ServerCommRx.Timeout_Count = 0;
            s_ServerCommRx.Index = 0;
            
            //            Delay_ms(500);
        }
    }
}


//////////////////////////////END OF C文件////////////////////////////////////
