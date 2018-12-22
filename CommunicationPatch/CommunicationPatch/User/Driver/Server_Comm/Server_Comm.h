
/*******************************************************************************
//Server_Comm.h
//与服务器通信相关的驱动头文件
*******************************************************************************/

#ifndef	SERVER_COMM_H
#define	SERVER_COMM_H

/******************************************************************************
//宏定义
*******************************************************************************/
//#define SERVER_COMM_USART_BAUDRATE                  9600                       //定义SERVER_COMM_USART通信波特率
//#define SERVER_COMM_USART                           USART1                       //定义SERVER_COMM_USART使用的串口号
//#define SERVER_COMM_USART_CLK                       RCC_APB2Periph_USART1        //定义SERVER_COMM_USART使用的模块时钟
//
//#define SERVER_COMM_USART_IRQn                      USART1_IRQn
//#define SERVER_COMM_USART_IRQHandler                USART1_IRQHandler
//
////SERVER_COMM_USART_TX -----------------------------------------------
//#define SERVER_COMM_USART_TX_GPIO_CLK               RCC_APB2Periph_GPIOA         //定义SERVER_COMM_USART_TX端口的时钟
//#define SERVER_COMM_USART_TX_GPIO_PORT              GPIOA                        //定义SERVER_COMM_USART_TX占用的GPIO端口
//#define SERVER_COMM_USART_TX_GPIO_PIN               GPIO_Pin_9                   //定义SERVER_COMM_USART_TX的GPIO引脚号
//
////SERVER_COMM_USART_RX -----------------------------------------------
//#define SERVER_COMM_USART_RX_GPIO_CLK               RCC_APB2Periph_GPIOA         //定义SERVER_COMM_USART_RX端口的时钟
//#define SERVER_COMM_USART_RX_GPIO_PORT              GPIOA                        //定义SERVER_COMM_USART_RX占用的GPIO端口
//#define SERVER_COMM_USART_RX_GPIO_PIN               GPIO_Pin_10                  //定义SERVER_COMM_USART_RX的GPIO引脚号

#define SERVER_COMM_USART_BAUDRATE              115200                       //定义SERVER_COMM_USART通信波特率
#define SERVER_COMM_USART                       USART2                       //定义SERVER_COMM_USART使用的串口号
#define SERVER_COMM_USART_CLK                   RCC_APB1Periph_USART2        //定义SERVER_COMM_USART使用的模块时钟

#define SERVER_COMM_USART_IRQn                  USART2_IRQn
#define SERVER_COMM_USART_IRQHandler            USART2_IRQHandler

//SERVER_COMM_USART_TX -----------------------------------------------
#define SERVER_COMM_USART_TX_GPIO_CLK           RCC_APB2Periph_GPIOA         //定义SERVER_COMM_USART_TX端口的时钟
#define SERVER_COMM_USART_TX_GPIO_PORT          GPIOA                        //定义SERVER_COMM_USART_TX占用的GPIO端口
#define SERVER_COMM_USART_TX_GPIO_PIN           GPIO_Pin_2                   //定义SERVER_COMM_USART_TX的GPIO引脚号

//SERVER_COMM_USART_RX -----------------------------------------------
#define SERVER_COMM_USART_RX_GPIO_CLK           RCC_APB2Periph_GPIOA         //定义SERVER_COMM_USART_RX端口的时钟
#define SERVER_COMM_USART_RX_GPIO_PORT          GPIOA                        //定义SERVER_COMM_USART_RX占用的GPIO端口
#define SERVER_COMM_USART_RX_GPIO_PIN           GPIO_Pin_3                  //定义SERVER_COMM_USART_RX的GPIO引脚号





//通信数据包定义
#define SERVER_COMM_PACKAGE_HEAD                    0x7E    //包头
#define SERVER_COMM_PACKAGE_IDENTIFY                0xAA    //标识符
#define SERVER_COMM_PACKAGE_VERSION                 0x01    //版本号1
#define SERVER_COMM_PACKAGE_DT                      0x00    //设备类型是寻址的
#define SERVER_COMM_PACKAGE_DT_ALL                  0xFF    //支持所有的类型(用于自检握手时判断)
#define SERVER_COMM_PACKAGE_DID                     0x01    //设备编号1
#define SERVER_COMM_PACKAGE_BROADCAST_DID           0x00    //广播设备编号
#define SERVER_COMM_PACKAGE_RESERVED                0x00    //预留，备用字节
#define SERVER_COMM_PACKAGE_CONTINUE_TRANS          0x80    //续传包
#define SERVER_COMM_PACKAGE_UNCONTINUE_TRANS        0x00    //非续传包，或单包
#define SERVER_COMM_PACKAGE_TAIL                    0x54    //包尾


//数据包命令码列表
//通用应答
#define SERVER_COMM_PACKAGE_CMD_RESPONSE            0x6001    //
//设置类
#define SERVER_COMM_PACKAGE_CMD_SET_PARA            0x0B00    //设置参数
//查询类
#define SERVER_COMM_PACKAGE_CMD_READ_PARA           0x0B01    //读取信息
//控制类
#define SERVER_COMM_PACKAGE_CMD_CTR_RESET           0x0C00    //控制复位
//上报类
#define SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE    0x0D80    //报告握手
#define SERVER_COMM_PACKAGE_CMD_REPORT_HEARTBEAT    0x0D81    //报告心跳
#define SERVER_COMM_PACKAGE_CMD_REPORT_DATA         0x0D83    //报告传感器数据
#define SERVER_COMM_PACKAGE_CMD_REPORT_PARA         0x0D87    //报告参数数据
#define SERVER_COMM_PACKAGE_CMD_REPORT_GPS          0x0D8B    //报告GPS信息
#define SERVER_COMM_PACKAGE_CMD_REPORT_FLASH        0x0E01    //上报片外Flash的数据包
#define SERVER_COMM_PACKAGE_CMD_RPT_RESET           0x0D82    //上报复位

// 参数类型定义
#define SYSTEM_PARA_TYPE_IP_ADDR_PORT               0x01        //IP地址、端口号
#define SYSTEM_PARA_TYPE_TIME_SYNCH                 0x02        //系统时间同步
#define SYSTEM_PARA_TYPE_UPLOAD_INTERVAL            0x04        //通用数据上传间隔
#define SYSTEM_PARA_TYPE_HEARTBEAT_INTERVAL         0x06        //心跳间隔
#define SYSTEM_PARA_TYPE_LAB_ADJUST                 0x07        //实验室校准
#define SYSTEM_PARA_TYPE_APP_ADJUST                 0x08        //应用校准
#define SYSTEM_PARA_TYPE_BATCH_APP_ADJUST           0xA0        //批量应用校准
#define SYSTEM_PARA_TYPE_BATCH_LAB_ADJUST           0xB0        //批量实验室校准

// 外部存储的数据包类型定义
#define EXTFLASH_PACKAGE_TYPE_DEVICE_RESET          0x01        //设备复位
#define EXTFLASH_PACKAGE_TYPE_REPORT_DATA           0x02        //传感器数据包
#define EXTFLASH_PACKAGE_TYPE_HEARTBEAT             0x03        //心跳数据包




// 通信数据包解析状态
//完整数据包结构
#define SERVER_COMM_PACKAGE_ANALYSIS_HEAD       0       //包头
#define SERVER_COMM_PACKAGE_ANALYSIS_IDENT      1       //标识符
#define SERVER_COMM_PACKAGE_ANALYSIS_LEN        2       //数据长度
//#define SERVER_COMM_PACKAGE_ANALYSIS_ADF        3       //数据体
//数据体
#define SERVER_COMM_PACKAGE_ANALYSIS_SN         4       //流水号
#define SERVER_COMM_PACKAGE_ANALYSIS_CMD        5       //命令码
#define SERVER_COMM_PACKAGE_ANALYSIS_DATA       6      //数据内容
#define SERVER_COMM_PACKAGE_ANALYSIS_CRC        7       //校验码


// 通信数据包解析错误码
//完整数据包结构
#define PACKAGE_ANALYSIS_SUCCEED                0       //解析正确
#define PACKAGE_ANALYSIS_HEAD_ERROR             1       //包头错误
#define PACKAGE_ANALYSIS_IDENT_ERROR            2       //标识符错误
//本公司结构
#define PACKAGE_ANALYSIS_SN_ERROR               3       //序列号错误
#define PACKAGE_ANALYSIS_CMD_ERROR              4       //命令码错误
#define PACKAGE_ANALYSIS_DATA_ERROR             5       //数据内容错误
#define PACKAGE_ANALYSIS_CRC_ERROR              6       //校验码错误

#define PACKAGE_ANALYSIS_NOT_RESPONSE_ERROR     7      //非应答包错误
#define PACKAGE_ANALYSIS_UNKNOWN_ERROR          0xFF    //未知错误



//与服务器通信的数据最大长度
#define PC_COM_MAX_DATA_LENGTH                  (512)     //是对设备端进行升级时的数据包大小

//USART接收超时
#define SERVER_COMM_RX_DATA_TIMEOUT             5	    //没有接收超时5ms就是接收完毕
//与服务器通信最大重发次数
#define SERVER_COMM_REPEAT_SEND_TIME            3       //重发次数2次
//与服务器通信最大等待间隔时间
#define SERVER_COMM_WAIT_RESPONSE_TIMEOUT       (1000 * 5)  //等待服务器应答超时

//发送握手指令给服务器的时间间隔
#define SERVER_COMM_HANDSHAKE_INTERVAL          (1000 * 30) //

// 重新配置无线模块的时间间隔
#define RESET_WIRELESSMODULE_INTERVAL           (1000UL * 60 * 2) //默认1分钟








/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
//与服务器通信的接收数据的结构体定义
typedef struct
{
    u8  Status;                         //接收数据状态标志
    u32 Timeout_Count;                  //接收超时计数,单位ms
    u8  Buffer[PC_COM_MAX_DATA_LENGTH]; //接收数据缓冲区
    u16 Index;                          //接收数据计数
}ServerCommRxStruct;

//与服务器通信的发送数据的结构体定义
typedef struct
{
    u8  Buffer[PC_COM_MAX_DATA_LENGTH]; //发送数据缓冲区
    u16 Index;                          //发送数据计数

    u8  RepeatNum;                      //重发次数，0—不用重发

    u8  WaitResponse;                   //等待服务器应答标志
    u16 WaitResponseTimeout;            //等待应答的超时时间
}ServerCommTxStruct;

// 与服务器通信的数据结构体
typedef struct
{
    u8  Head;           //包头
    u8  Identify;       //标识符
    u16 Length;         //长度
    union
    {
        u8 Buff[PC_COM_MAX_DATA_LENGTH];//消息体
        struct
        {
            u16 SN;     //流水号
            u16 CMD;    //命令符
            u8  Data[PC_COM_MAX_DATA_LENGTH - 4 - 2];//数据内容
            u16 Crc;    //CRC校验码
        };
    }ADF;
}ServerCommPackageStruct;

// 传感器参数结构体
typedef struct
{
    u8  para_type;       //参数类型
//    //实验室校准参数
//    struct
//    {
//        u8  sensor_type;    //传感器类型
//        u16 shrink;     //缩小系数
//        u16 K1;
//        u16 B1;
//        u16 K2;
//        u16 B2;
//    }Lab;
//    //应用校准参数
//    struct
//    {
//        u8  sensor_type;    //传感器类型
//        u16 shrink;     //缩小系数
//        u16 K3;
//        u16 B3;
//        u16 K4;
//        u16 B4;
//    }App;
//    //IP地址和端口号
//    u8  IPAddr[25];
//    // 通用数据上传间隔(s)
//    u32 Upload_interval;
//    // 心跳上传间隔(min)
//    u16 Heartbeat_interval;
    //协议类型
    u8  proc_type;
    //厂商
    u8  manu_type;
    //设备类型
    u8  device_type;
    //设备编号
    u8  deviceID[4];
    //密文序列
    u8  Code[8];
    // 软件版本
    u8  Ver[5];
}SystemParaStruct;

// SIM卡相关参数
typedef struct
{
    u8  num_len;    //sim卡号长度，中国是11位
    u8  num[2 + 11];//前面加86

    u8  IMSI_len;   //imsi码长度
    u8  IMSI[15];    //一般都是15位

    u8  IMEI_len;   //imei码长度
    u8  IMEI[15];    //一般都是15位

    u8  CCID_len;   //ccid码长度
    u8  CCID[20];    //一般都是20位
}SIMCardParaStruct;


// 通用应答的结果枚举
typedef enum
{
    RES_SUCCEED = 0,
    RES_FAILURE,
    RES_BUSY,
    RES_UPGRADE,
    RES_PARA_ERROR,
    RES_READ_WRITE_FAILURE,
    RES_UNKNOWN,
    RES_AUTHEN_ERROR,
}ComResponseResultEnum;

/******************************************************************************
//变量定义/声明
*******************************************************************************/
//用于与服务器通信的接收数据结构体
extern ServerCommRxStruct s_ServerCommRx;

//与服务器通信的数据包结构体声明
extern ServerCommPackageStruct s_ServerCommPackage;

//与服务器通信的发送数据的结构体声明
extern ServerCommTxStruct s_ServerCommTx;

// 传感器参数设置
extern SystemParaStruct s_SystemPara;

// SIM卡参数结构体声明
extern SIMCardParaStruct s_SIMCardPara;

// 最新一次发送的命令码
extern u16 g_LastSendServerCmd;



/******************************************************************************
//函数声明
*******************************************************************************/
//功能:	与服务器通信相关的初始化函数
void Server_Comm_Init(void);

//功能: 与服务器通信的发送数据函数
void Server_Comm_Send_Data(u8 *buffer, u16 data_l);

//函数功能: 与服务器通信的数据包打包函数
void Server_Comm_Package_Bale(u16 cmd);

//函数功能: 与服务器通信的监测函数
void Server_Comm_Rec_Monitor(void);












//功能: 与服务器通信的测试函数
void Server_Comm_Test(void);


#endif