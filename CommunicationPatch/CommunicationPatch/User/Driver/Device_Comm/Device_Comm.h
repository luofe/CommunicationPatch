
/*******************************************************************************
//Device_Comm.h
//与设备通信的驱动头文件
备注: 
*******************************************************************************/

#ifndef	DEVICE_COMM_H
#define	DEVICE_COMM_H

/******************************************************************************
//宏定义
*******************************************************************************/
#define DEVICE_COMM_USART_BAUDRATE             115200                       //定义波特率
#define DEVICE_COMM_USART                      USART3                       //定义使用的串口号
#define DEVICE_COMM_USART_CLK                  RCC_APB1Periph_USART3        //定义使用的模块时钟

#define DEVICE_COMM_USART_IRQn                 USART3_IRQn
#define DEVICE_COMM_USART_IRQHandler           USART3_IRQHandler

//USART_TX -----------------------------------------------
#define DEVICE_COMM_USART_TX_GPIO_CLK          RCC_APB2Periph_GPIOB         //定义USART_TX端口的时钟
#define DEVICE_COMM_USART_TX_GPIO_PORT         GPIOB                        //定义USART_TX占用的GPIO端口
#define DEVICE_COMM_USART_TX_GPIO_PIN          GPIO_Pin_10                   //定义USART_TX的GPIO引脚号

//USART_RX -----------------------------------------------
#define DEVICE_COMM_USART_RX_GPIO_CLK          RCC_APB2Periph_GPIOB         //定义USART_RX端口的时钟
#define DEVICE_COMM_USART_RX_GPIO_PORT         GPIOB                        //定义USART_RX占用的GPIO端口
#define DEVICE_COMM_USART_RX_GPIO_PIN          GPIO_Pin_11                   //定义USART_RX的GPIO引脚号


//Modbus通信数据最大长度
#define DEVICE_COMM_DATA_MAX_LENGTH            1024  


//数据包命令码列表
#define DEVICE_SET_SENSOR_ADJUST_CMD            0   //校准传感器
#define DEVICE_READ_SENSOR_ADJUST_CMD           1   //读取传感器校准传数据
#define DEVICE_READ_DEVICE_ID                   2   //读取设备ID号
#define DEVICE_SET_RTC_CMD                      3   //设置RTC时间
#define DEVICE_READ_RTC_CMD                     4   //读取RTC时间
#define DEVICE_SET_IP_ADDR_CMD                  5   //设置IP地址
#define DEVICE_READ_IP_ADDR_CMD                 6   //读取IP地址
#define DEVICE_SET_UPLOAD_INTERVAL_CMD          7   //设置数据上传间隔
#define DEVICE_READ_UPLOAD_INTERVAL_CMD         8   //读取数据上传间隔
#define DEVICE_SET_HEARTBEAT_INTERVAL_CMD       9   //设置心跳包间隔
#define DEVICE_READ_HEARTBEAT_INTERVAL_CMD      10  //读取心跳包间隔
#define DEVICE_CTR_RESET_CMD                    11  //控制设备端复位

#define DEVICE_CTR_ALL_PRINTF_CMD               12  //控制设备打印所有信息
#define DEVICE_CTR_ALL_HIDE_CMD                 13  //控制设备隐藏所有打印信息
#define DEVICE_CTR_SENSOR_PRINTF_CMD            14  //控制设备打印传感器信息
#define DEVICE_CTR_SENSOR_HIDE_CMD              15  //控制设备隐藏传感器信息
#define DEVICE_CTR_GPS_PRINTF_CMD               16  //控制设备打印GPS信息
#define DEVICE_CTR_GPS_HIDE_CMD                 17  //控制设备隐藏GPS信息

#define DEVICE_GET_SENSOR_DATA_CMD              18  //获取传感器数据
#define DEVICE_GET_GPS_DATA_CMD                 19  //获取GPS数据

#define DEVICE_CTR_BATTERY_PRINTF_CMD           20  //控制设备打印电源电压信息
#define DEVICE_CTR_BATTERY_HIDE_CMD             21  //控制设备隐藏电源电压信息
#define DEVICE_GET_BATTERY_VOL_CMD              22  //获取电源电压




//与设备端通信最大重发次数
#define DEVICE_COMM_REPEAT_SENDTIME            3       //重发次数3次
//与设备端通信最大等待间隔时间
#define DEVICE_COMM_WAIT_RESPONSE_TIMEOUT      (1000 * 1)    //等待设备端应答超时为1s


//USART接收超时
#define DEVICE_COMM_RX_DATA_TIMEOUT            10	    //没有数据接收超时等待时长（5个单位的Systick周期）



// 读取设备端传感器数据的时间间隔
#define DEVICE_COMM_GET_SENSOR_DATA_INTERVAL    (1000 * 10) 

// 设备启动完毕所需最大时间
#define DEVICE_INITIAL_OVER_TIMEOUT             (1000 * 20)

// 设备启动后读取GPS数据的最大时间
#define DEVICE_GET_GPS_DATA_TIMEOUT             (1000 * 10)

// 设备启动后读取设备编号的最大时间
#define DEVICE_GET_DEVICE_ID_TIMEOUT            (1000 * 2)

// 设备启动后读取电源电压的最大时间
#define DEVICE_GET_POWER_DATA_TIMEOUT           (1000 * 2)




/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
//与设备端通信的接收数据的结构体定义
typedef struct
{
    u8  Status;                         //接收数据状态标志
    u32 Timeout_Count;                  //接收超时计数,单位ms
    u8  Buffer[DEVICE_COMM_DATA_MAX_LENGTH]; //接收数据缓冲区
    u16 Index;                          //接收数据计数
}DeviceCommRxStruct;

//与设备端通信的发送数据的结构体定义
typedef struct
{
    struct
    {
        u8  Buffer[DEVICE_COMM_DATA_MAX_LENGTH]; //发送数据缓冲区
        u16 Index;                          //发送数据计数
    }LineUp[2];
    
    u8  RepeatNum;                      //重发次数，0—不用重发
    
    u8  WaitResponse;                   //等待设备端应答标志
    u16 WaitResponseTimeout;            //等待应答的超时时间
}DeviceCommTxStruct;

// 传感器校准的参数结构体
typedef struct
{
    u8  type;
    u16 multiple;
    s16 K1;
    s16 B1;
    s16 K2;
    s16 B2;
    s16 K3;
    s16 B3;
    s16 K4;
    s16 B4;
}SensorAdjStruct;

// RTC结构体
typedef struct
{
    u16 year;
    u8  month;
    u8  day;
    u8  hour;
    u8  min;
    u8  sec;
}RTCStruct;

// IP地址和端口号结构体
typedef struct
{
//    u8  ip1;
//    u8  ip2;
//    u8  ip3;
//    u8  ip4;
//    u16 port;
    u8  ip_port[25];
}IPAddrPortStruct;

// 数据上传时间间隔结构体
typedef struct
{
    u16 time1;
    u16 time2;
    u16 heartbeat;
}UploadIntervalStruct;

// 通传感器数据结构体
typedef struct
{
    u8  got_status;     //获取状态，TRUE—获得了
    u8  device_sta[4];  //设备状态字
    u8  sensor_num;     //传感器个数
    
    struct
    {
        float temp;     //温度
        float humi;     //湿度
    }TRH;
    struct
    {
        s32 real_val;   //真实值
        s32 label_val;  //实验室修正值
        s32 app_val;    //应用值
    }PM2_5;
    struct
    {
        s32 real_val;   //真实值
        s32 label_val;  //实验室修正值
        s32 app_val;    //应用值
    }PM10;
    struct
    {
        s32 real_val;   //真实值
        s32 label_val;  //实验室修正值
        s32 app_val;    //应用值
    }CO;
    struct
    {
        s32 real_val;   //真实值
        s32 label_val;  //实验室修正值
        s32 app_val;    //应用值
    }NO2;
    struct
    {
        s32 real_val;   //真实值
        s32 label_val;  //实验室修正值
        s32 app_val;    //应用值
    }O3;
    struct
    {
        s32 real_val;   //真实值
        s32 label_val;  //实验室修正值
        s32 app_val;    //应用值
    }SO2;
    struct
    {
        s32 real_val;   //真实值
        s32 label_val;  //实验室修正值
        s32 app_val;    //应用值
    }NO;
    struct
    {
        s32 real_val;   //真实值
        s32 label_val;  //实验室修正值
        s32 app_val;    //应用值
    }TVOC;
    struct
    {
        float temp;     //温度
        float humi;     //湿度
        float wd;       //风向
        float ws;       //风速
        float pa;       //气压
    }ExtSensor;
    struct
    {
        u16 m_freq;     //主风扇频率
        u16 pm10_freq;  //PM10风扇频率
    }Fan;
}SensorDataStruct;

// GPS定位信息
typedef struct 
{
    u8  got_status;     //获取状态
    u8  status;         // 定位状态'A'定位，'V'不定位
    u8  fs;             // 绑定状态'1'不定位，'2'2D定位，'3'3D定位
    s32 latitude;       // 以度为单位的纬度值乘以10的7次方，精确到百万分之一度
    s32 longitude;      // 以度为单位的经度值乘以10的7次方，精确到百万分之一度
    s16 high;           // 海拔高度，以m为单位
    u16 speed;          // 以km/h为单位扩大10倍赋值
    u8  heading;        // 0-359,正北为0
    u8  noSV;           // 用于定位的卫星数量
    u32 gmtTime;        // 从1970年的秒数
}GPSInfoStruct;

// 电源电压信息
typedef struct
{
    u8  adc[2];
    u8  vol[2];     //电池电压
}DevicePowerStruct;



/******************************************************************************
//变量定义/声明
*******************************************************************************/
//用于与设备端通信的接收数据结构体
extern DeviceCommRxStruct s_DeviceCommRx;

//与设备端通信的发送数据的结构体定义
extern DeviceCommTxStruct s_DeviceCommTx;

// 传感器校准的参数结构体声明
extern SensorAdjStruct s_SensorAdj;

// RTC结构体声明
extern RTCStruct s_RTC;

// IP地址和端口号结构体声明
extern IPAddrPortStruct s_IPAddrPort;

// 数据上报时间间隔结构体声明
extern UploadIntervalStruct s_UploadInterval;

// 通用传感器数据结构体声明
extern SensorDataStruct s_SensorData;

// GPS定位信息
extern GPSInfoStruct s_GPSInfo;

// 设备初始化完毕标志
extern u8  g_DeviceInitialFlag;

// 电源电压信息
extern DevicePowerStruct s_DevicePower;

// 设备初始化状态
extern u8  g_DeviceInitFlag;







/******************************************************************************
//函数声明
*******************************************************************************/
//函数功能: 与设备端通信相关的初始化函数
void Device_Comm_Init(void);

//功能: 与设备端板通信的发送数据
void Device_Comm_Send_Data(u8 *buffer, u16 data_l);

//函数功能: 与设备端通信的数据包打包函数
void Device_Comm_Package_Bale(u8 cmd);

//函数功能: 与设备端通信的数据包处理函数
u8 Device_Comm_Package_Process(u8 cmd, u8* resp_str, u16 len);

//函数功能: 与通信板通信数据包解析函数
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l);

//功能: 设备打印输出控制函数
u8 Device_Printf_Ctr(u8 cmd);

//功能: 设备端初始化函数
u8 Device_Initial(void);

//函数功能: 与设备端通信的监测函数
void Device_Comm_Rec_Monitor(void);
























#endif