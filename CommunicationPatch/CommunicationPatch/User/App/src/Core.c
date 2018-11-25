
/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
// Functiom:项目工程的核心处理源文件
*******************************************************************************/


/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//变量定义
*******************************************************************************/
// 公用数据缓冲区，用于数据解析、数据发送、数据存储等
u8 g_PublicDataBuffer[PUBLIC_DATA_BUFFER_MAX_LENGTH] = {0};

// 系统出货使能标志
u8 g_SysPulloutEnableFlag = TRUE;       //

//系统发送POLL数据包给服务器的计时
u32 g_SysPollTimeCnt = 0;     //默认启动后发送一次POLL包  

//系统查询设备端状态的计时
u32 g_GetPulloutStatusTimeCnt = 0;

//系统温度控制的计时
u16 g_SysTempCtrTimeCnt = 0;

//系统工作时间判断的计时
u16 g_SysWorkTimeJudgeCnt = 0;

//系统节能时间段标志
u8 g_SysEnergySaveTimeFlag = OTHER_WORK_TIME;  //默认是非节能时间段

//系统售卖时间段标志
u8 g_SysSaleTimeFlag = SALE_WORK_TIME;  //默认是售卖时间段

// 系统节能时间段设置标志
u8 g_SysEnergySaveTimeSetFlag = ACCORD_TO_NO_TEMP;    //默认是没有下发温度，不控制压缩机加热器，只开风扇

// 系统售卖时间段设置标志
u8 g_SysSaleTimeSetFlag = FALSE;      //默认是没有设置

// 系统运行状态变化标志
u8 g_SysRunStatusChangeFlag = SYSTEM_RUN_STATUS_STAY;    //默认没有变化

// 设备端复位标志
u8 g_PulloutBoardReseted = FALSE;    //默认设备端未复位完毕

// 系统校时标志
u8 g_SysCorrectDateTimeFlag = FALSE;    //默认没有校时

// 系统初始化状态标志
u8 g_SysInitStatusFlag = FALSE;         //默认未完成

// 系统进入/退出故障状态时保存整机状态值的变量
u8 g_SysAccessExitErrorVMCStatus;

//// 电磁锁打开后关闭的计时
//u16 g_SysEMLockOpenCnt = 0;

// 设备故障标志
u8 g_SystemDeviceErrorFlag = FALSE;

// 发送传感器数据的计时
u32 g_SendSensorDataTimeCnt = 0;

// 读取传感器数据的计时
u32 g_GetDeviceSensorDataTimeCnt = 0;







/******************************************************************************
//函数声明
*******************************************************************************/
//功能: 字符串转整形数的函数
int SysStr2Int(char* str);

//功能: 整形数转字符串的函数
char* SysInt2Str(int val,char* dst,int radix);

//功能: 字符串型的日期递增函数
char* SysCharDateAscend(char* c_date);

//功能: 系统字符串比较函数
u8 SysCharArrayCmp(const char *data1, const char *data2);

//功能: 系统u8型数组比较函数
u8 SysU8ArrayCmp(u8 *data1, u8 *data2, u16 datal);

//函数功能: 交换U16数据的高低字节的函数
u16 U16_Change_Order(u16 data);

//函数功能: 系统外设初始化函数
void SysPeripheralInit(void);

//函数功能: 系统所有的全局变量初始化
void SysGlobalVariableInit(void);

//函数功能: 系统各种功能控制
void System_Function_Control(void);

//函数功能: 系统各种状态采集
void System_Status_Collection(void);
















/******************************************************************************
//函数定义
*******************************************************************************/
/*****************************************
//名称: SysStr2Int
//功能: 字符串转整形数的函数
//入口: char* dst——字符串
//出口: int——转换后的整形数
//备注: 即atoi()函数的原型
******************************************/
int SysStr2Int(char* str)
{
    int data = 0;  
    const char *ptr = str;
    
    if(*str == '-'){  
        str ++;  
    }  
    while(*str != '\0'){  
        if(*str < '0' || *str > '9'){  
            break;  
        }  
        data = data * 10 + (*str - '0');  
        str ++;  
    }  
    if(*ptr == '-'){  
        data =-data;  
    }  
    
    return data; 
}   

/*****************************************
//名称: SysInt2Str
//功能: 整形数转字符串的函数
//入口: int val——整形数, char* dst——字符串, int radix——转换进制
//出口: 无
//备注: 即itoa()函数的原型
******************************************/
char* SysInt2Str(int val, char* dst, int radix)
{
    char *_pdst = dst; 
    char *_first = _pdst;     
    char _cov;           
    unsigned int _rem;
    
    if (!val)//允许val等于0 
    {
        *_pdst = '0';
        *++_pdst = '\0';
        
        return dst;
    }           
    if(val <0)
    {
        *_pdst++ = '-';
        val = -val;
    }
    
    while(val > 0)
    {
        _rem = (unsigned int)(val % radix);
        val /= radix;//每次计算一位 ，从低到高
        if  (_rem > 9)//16进制
            *_pdst++ = (char)(_rem - 10 + 'a'); 
        else
            *_pdst++ = (char)(_rem + '0');      
    }      
    *_pdst-- = '\0';
    do{ //由于数据是地位到高位储存的，需要转换位置
        _cov = *_pdst;
        *_pdst = *_first;
        *_first = _cov;
        _pdst--;
        _first++;        
    }while(_first < _pdst); 
    
    return dst;
}

/*****************************************
//名称: SysCharDateAscend
//功能: 字符串型的日期递增函数
//入口: const TCHAR* c_date——字符串型的日期
//出口: char*——返回递增后的字符串
//备注: 必须是以“年月日”的格式的字符串
******************************************/
char* SysCharDateAscend(char* c_date)
{
    char *pr = c_date;
    
    char c_year[5] = "";
    char c_month[3]= "";
    char c_day[3]  = "";
    
    u16   year;
    u8    month;
    u8    day;
    u8    leap_year_day;
    
    memcpy(c_year, &pr[0], 4);
    c_year[4] = '\0';
    memcpy(c_month, &pr[4], 2);
    c_month[2] = '\0';
    memcpy(c_day, &pr[6], 2);
    c_day[2] = '\0';
    
    year    = atoi(c_year);
    month   = atoi(c_month);
    day     = atoi(c_day);
    
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        {
            //如果到了31号
            if(day == 31)
            {
                day = 1;
                if(month == 12)
                {
                    month = 1;
                    year++;
                }
                else
                {
                    month++;
                }
            }
            else
            {
                day++;
            }
        }
        break;
        
        case 2:
        {
            //判断是否为闰年
            if((year%4 == 0) && (year%100 != 0) || (year%400 == 0))
            {
                leap_year_day = 29;
            }
            else
            {
                leap_year_day = 28;
            }
            //如果到了29号
            if(day == leap_year_day)
            {
                day = 1;
                month++;
            }
            else
            {
                day++;
            }
        }
        break;
        
        case 4:
        case 6:
        case 9:
        case 11:
        {
            //如果到了30号
            if(day == 30)
            {
                day = 1;
                month++;
            }
            else
            {
                day++;
            }
        }
        break;
    }
    
    SysInt2Str(year, c_year, 10);
    SysInt2Str(month, c_month, 10);
    if(month < 10)  //如果是一位数
    {
        c_month[1] = c_month[0];
        c_month[0] = '0';
    }
    SysInt2Str(day, c_day, 10);
    if(day < 10)  //如果是一位数
    {
        c_day[1] = c_day[0];
        c_day[0] = '0';
    }
    
    memcpy(&pr[0], c_year, 4);
    memcpy(&pr[4], c_month, 2);
    memcpy(&pr[6], c_day, 2);
    
    return c_date;
}

/*****************************************
//名称: SysCharArrayCmp
//功能: 系统字符串比较函数
//入口: const char *data1——字符串1, const char *data2——字符串2
//出口: u8——返回的比较状态，1—相等，0—不相等
******************************************/
u8 SysCharArrayCmp(const char *data1, const char *data2)
{
    while((*data1 != '\0') && (*data2 != '\0'))
    {
        if(*data1++ != *data2++)
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

/*****************************************
//名称: SysU8ArrayCmp
//功能: 系统u8型数组比较函数
//入口: u8 *data1——数组1, u8 *data2——数组2, u16 datal——比较的长度
//出口: u8——返回的比较状态，0—相等，1—数组1比数组2大，2—数组1比数组2小
******************************************/
u8 SysU8ArrayCmp(u8 *data1, u8 *data2, u16 datal)
{
    u16 i = 0;
    
    for(; i < datal; i++)
    {
        if(data1[i] > data2[i]) //如果出现数组1大于数组2的情况
        {
            return 1;
        }
        else if(data1[i] < data2[i])//如果出现数组1小于数组2的情况
        {
            return 2;
        }
    }
    
    if(i == datal)  //如果都比较完毕，说明都相等
    {
        return 0;
    }
    
    return 0;
}

//********************************************************
//函数名称: U16_Change_Order
//函数功能: 交换U16数据的高低字节的函数
//输    入: u16 data——需要交换的数据
//输    出: u16——返回交换后的值
//备    注: 
//********************************************************
u16 U16_Change_Order(u16 data)	
{	
	return ((data << 8) + (data >> 8));	
}

//********************************************************
//函数名称: SysPeripheralInit
//函数功能: 系统外设初始化函数
//输    入: 无
//输    出: 无
//备    注: 
//********************************************************
void SysPeripheralInit(void)
{
	USART_ITConfig(SERVER_COMM_USART, USART_IT_RXNE, ENABLE);	//使能USART接收中断
    //无线模块的初始化
    WireLess_Initial();
    
	USART_ITConfig(DEVICE_COMM_USART, USART_IT_RXNE, ENABLE);	//使能USART接收中断
    //设备端初始化
    Device_Initial();

    Ext_Flash_Detect();     //检测片外flash是否存在
    
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);	//使能USART接收中断
}

//********************************************************
//函数名称: SysGlobalVariableInit
//函数功能: 系统所有的全局变量初始化
//输    入: 无
//输    出: 无
//备    注: 
//********************************************************
void SysGlobalVariableInit(void)
{
    u8  i = 0;
    //读取片内Flash的数据
    Internal_Flash_ReadOut();
    
    //与服务器端通信相关的变量
    s_ServerCommRx.Status = FALSE;
    s_ServerCommRx.Timeout_Count = 0;
    s_ServerCommRx.Index = 0;
    memset(s_ServerCommRx.Buffer, 0, sizeof(s_ServerCommRx.Buffer));
    
    // 与服务器通信的数据包结构体
    s_ServerCommPackage.Head        = SERVER_COMM_PACKAGE_HEAD;
    s_ServerCommPackage.Identify    = SERVER_COMM_PACKAGE_IDENTIFY;
    s_ServerCommPackage.Length      = 0;
    s_ServerCommPackage.ADF.SN      = 1;    //序列号从1开始
    s_ServerCommPackage.ADF.CMD     = SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE;
    
    //系统参数
    s_SystemPara.proc_type      = 0x10;
    s_SystemPara.manu_type      = 0x01;
    s_SystemPara.device_type    = 0x00;
    memset(s_SystemPara.deviceID, 0, sizeof(s_SystemPara.deviceID));
    memset(s_SystemPara.Code, 0, 8);
    memcpy(s_SystemPara.Ver, "01.01", 5);
    s_SIMCardPara.num_len       = 13;
    memcpy(s_SIMCardPara.num, "8615810510449", s_SIMCardPara.num_len);
    memset(s_SIMCardPara.CCID, 0, s_SIMCardPara.CCID_len);
    memset(s_SIMCardPara.IMEI, 0, s_SIMCardPara.IMEI_len);
    memset(s_SIMCardPara.IMSI, 0, s_SIMCardPara.IMSI_len);
    
    i = 0;
    s_IPAddrPort.ip_port[i++] = '"';
    memcpy(&s_IPAddrPort.ip_port[i], WIRELESS_SERVER_IP, strlen(WIRELESS_SERVER_IP));
    i += strlen(WIRELESS_SERVER_IP);
    s_IPAddrPort.ip_port[i++] = '"';
    s_IPAddrPort.ip_port[i++] = ',';
    memcpy(&s_IPAddrPort.ip_port[i], WIRELESS_SERVER_REMOTE_PORT, strlen(WIRELESS_SERVER_REMOTE_PORT));
    
    
    // 设备端相关参数
    s_UploadInterval.time1      = SEND_SENSOR_DATA_TIME_INTERVAL;
    s_UploadInterval.heartbeat  = SEND_POLL_PACKAGE_TIME_INTERVAL;
    
    memset(&s_SensorData, 0, sizeof(s_SensorData));
    memset(s_SensorData.device_sta, 0xFF, sizeof(s_SensorData.device_sta));
    s_SensorData.got_status     = FALSE;
    s_DevicePower.vol[0]        = 0;
    s_DevicePower.vol[1]        = 0;
    s_SensorData.sensor_num     = 0;
    
    s_GPSInfo.got_status = FALSE;
    memset(&s_GPSInfo, 0, sizeof(s_GPSInfo));
}

//********************************************************
//函数名称: System_Function_Control
//函数功能: 系统各种功能控制
//输    入: 无
//输    出: 无
//备    注: 
//********************************************************
void System_Function_Control(void)
{
    if(g_SysInitStatusFlag == TRUE) //初始化完毕
    {
        //如果到发送心跳的时间了
        if(abs(g_ms_Timing_Count - g_SysPollTimeCnt) >= (s_UploadInterval.heartbeat * 60 * 1000))
        {
            g_SysPollTimeCnt = g_ms_Timing_Count;
            
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_HEARTBEAT);
        }
    }
    else 
    {
        if(g_WireLessModuleInitFlag == TRUE)   //假如无线模块也初始化完成了
        {
            //与服务器握手
            if(abs(g_ms_Timing_Count - g_SysPollTimeCnt) >= SERVER_COMM_HANDSHAKE_INTERVAL)
            {
                g_SysPollTimeCnt = g_ms_Timing_Count;
                
                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE);
            }
        }
        else
        {
            //无线模块的初始化
            WireLess_Initial();
        }
    }
    //如果到发送传感器数据的时间了
    if(abs(g_ms_Timing_Count - g_SendSensorDataTimeCnt) >= (s_UploadInterval.time1 * 1000))//30
    {
        //只有等获得了设备端的传感器数据才上报
        if(s_SensorData.got_status == TRUE)
        {
            g_SendSensorDataTimeCnt = g_ms_Timing_Count;
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_DATA);
        }
        else    //否则得上报心跳包，防止网络断联
        {
            g_SendSensorDataTimeCnt = g_ms_Timing_Count - (s_UploadInterval.heartbeat * 60 * 1000);
        }
    }
    
    // 到读取设备端传感器数据的时候了
    if(abs(g_ms_Timing_Count - g_GetDeviceSensorDataTimeCnt) >= DEVICE_COMM_GET_SENSOR_DATA_INTERVAL)
    {
        g_GetDeviceSensorDataTimeCnt = g_ms_Timing_Count;
        // 控制显示传感器数据的打印
        Device_Printf_Ctr(DEVICE_CTR_SENSOR_PRINTF_CMD);
    }
    
    // RTC秒计时
    if(abs(g_ms_Timing_Count - s_Timing_Count) >= 1000)
    {
        s_Timing_Count = g_ms_Timing_Count;
        s_GPSInfo.gmtTime++;
    }
}








//////////////////////////////END OF C文件////////////////////////////////////

