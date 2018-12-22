
/*
core.h	核心算法函数头文件
*/
#ifndef CORE_H
#define CORE_H

/******************************************************************************
//宏定义
*******************************************************************************/
// 公用数据缓冲区大小
#define PUBLIC_DATA_BUFFER_MAX_LENGTH       4096        //默认为255


//空指令
#define Nop()                               {}


//发送POLL指令给服务器的时间间隔
#define SEND_POLL_PACKAGE_TIME_INTERVAL     (60) //默认是1h，单位是分钟

//发送传感器数据给服务器的时间间隔
#define SEND_SENSOR_DATA_TIME_INTERVAL      (60) //默认是1min，单位是秒

//发送自检完毕给服务器,请求初始化的时间间隔
#define SEND_STARTUP_RPT_TIME_INTERVAL      (1000 * 5)  //默认是5s

//没有接收到服务器的数据最大超时时间
#define SERVER_COMM_NO_DATA_REC_TIMEOUT     (1000UL * 60 * 2)   //没有接收到服务器的数据超时时间为2min

//向设备端查询状态的时间间隔
#define GET_PULLOUT_STATUS_TIME_INTERVAL    (1000 * 5)  //默认是5s

//没有接收到设备端的数据最大超时时间
#define DEVICE_COMM_NO_DATA_REC_TIMEOUT    (1000UL * 60 * 5)   //没有接收到设备端的数据超时时间为5min





/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
//是否需要应答的枚举
typedef enum
{
    DONT_RESPONSE = 0x00,       //不需要应答
    NEED_RESPONSE = 0x80        //需要应答
}CommResponseFlagEnum;





/******************************************************************************
//变量定义/声明
*******************************************************************************/
// 公用数据缓冲区，用于数据解析、数据发送、数据存储等
extern u8 g_PublicDataBuffer[];

// 系统出货使能标志
extern u8 g_SysPulloutEnableFlag;

//系统发送POLL数据包给服务器的计时
extern u32 g_SysPollTimeCnt;

//系统查询设备端状态的计时
extern u32 g_GetPulloutStatusTimeCnt;

// 系统初始化状态标志
extern u8 g_SysInitStatusFlag;

// 设备故障标志
extern u8 g_SystemDeviceErrorFlag;

// 发送传感器数据的计时
extern u32 g_SendSensorDataTimeCnt;

// 重新配置无线模块的间隔计时
extern u32 g_ReSetWireLessModuleTimeCnt;




















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








#endif

/**********************************END OF FILE*********************************/
