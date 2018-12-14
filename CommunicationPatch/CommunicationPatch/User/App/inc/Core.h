
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


//求整形的高低字节
#define LSB(a)                              ((a).cV[0])
#define MSB(a)   	                        ((a).cV[1])

//求字节型的高低4位
#define HALF_LSB(a)	                        ((a).HalfByte.Lsb)
#define HALF_MSB(a)	                        ((a).HalfByte.Msb)

//空指令
#define Nop()                               {}

////最大货道数目
//#define DEVICE_ALL_TUNNEL_NUMBER            (5 * 10)

//最大展示位数目，即面板上的按键个数
#define DEVICE_ALL_POSITION_NUMBER          4

//设备的机型标识号
#define DEVICE_MACHINE_TYPE_ID              1

//本机设备号
#define LOCAL_DEVICE_ID                     1

//判断货道是否正常
#define JUDGE_TUNNEL_RUNNING_STATUS         0x40        //

//判断货道内商品剩余数
#define JUDGE_TUNNEL_RESIDUE_NUMBER         0x3F

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

// 温度控制的时间间隔
#define SYS_TEMP_CTR_TIME_INTERVAL          (1000 * 3)  //默认是3s

// 系统工作时间判断的时间间隔
#define SYS_WORK_TIME_JUDGE_INTERVAL        (1000 * 1)  //默认是1s


// 系统制冷时温度控制的温差定义
#define COLD_TEMP_DIFFERIENCE_UPPER_LIMIT    2           //2°的上限温差
#define COLD_TEMP_DIFFERIENCE_LOWER_LIMIT    2           //2°的下限温差

// 系统加热时温度控制的温差定义
#define HEAT_TEMP_DIFFERIENCE_UPPER_LIMIT    2           //2°的上限温差
#define HEAT_TEMP_DIFFERIENCE_LOWER_LIMIT    2           //2°的下限温差

// 系统电磁锁打开后关闭的延时
#define EMLOCK_OPEN_TIMEOUT                 (1000 * 5)



//货物放置的行列数和分组数默认值
#define GOODS_COORDS_MAX_ROW_NUM            8               //系统支持最多的行数为16
#define GOODS_COORDS_MAX_LEFT_COLUMN_NUM    8               //系统支持最多的左侧列数为8
#define GOODS_COORDS_MAX_RIGHT_COLUMN_NUM   8               //系统支持最多的右侧列数为8

#define GOODS_COORDS_BLOCK_NUM              2               //货物分为2块，即左侧和右侧
#define GOODS_COORDS_ROW_NUM                1               //货物分为5层
#define GOODS_COORDS_LEFT_COLUMN_NUM        1               //货物左侧列数为6
#define GOODS_COORDS_RIGHT_COLUMN_NUM       1               //货物右侧列数为5

// 出货货道的范围
#define PULLOUT_TUNNEL_MIN_VALUE            0x11            //最小的货道是1-1
#define PULLOUT_TUNNEL_MAX_VALUE            (GOODS_COORDS_ROW_NUM * 16 + \
                                            (GOODS_COORDS_LEFT_COLUMN_NUM + GOODS_COORDS_RIGHT_COLUMN_NUM))    //最大的货道是

// 系统带程序的电路板数量
#define VMC_ALL_HAVE_FIRMVARE_PCBS_NUM      3           //总共有3块带程序的PCB板
// 通信板硬件版本号
#define SYSTEM_MC_HARDWARE_VERSION          0x0100      //硬件版本是V1.0.0
// 通信板固件版本号
#define SYSTEM_MC_SOFTWARE_VERSION          0x0140      //固件版本是V1.4.0

// 设备端硬件版本号
#define SYSTEM_GG_HARDWARE_VERSION          0x1100      //硬件版本是V1.0.0
// 设备端固件版本号
#define SYSTEM_GG_SOFTWARE_VERSION          0x1100      //固件版本是V1.0.0

// 光栅板硬件版本号
#define SYSTEM_RD_HARDWARE_VERSION          0x2100      //硬件版本是V1.0.0
// 光栅板固件版本号
#define SYSTEM_RD_SOFTWARE_VERSION          0x2100      //固件版本是V1.0.0

// 设备名称
#define SYSTEM_DEVICE_NAME                  "H11"       //康创聚联一代寻址机

//最大支持的一次性出货货道数量
#define ONCE_PULLOUT_MAX_CHANNEL_NUMBER     8







/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
//是否需要应答的枚举
typedef enum
{
    DONT_RESPONSE = 0x00,       //不需要应答
    NEED_RESPONSE = 0x80        //需要应答
}CommResponseFlagEnum;

// vmc_st即售货机状态的枚举
typedef enum
{
    DEVICE_WORK_NORMAL = 0,     //工作正常
    DEVICE_ERROR,               //故障
    DEVICE_BUSY,                //忙碌中
//    DEVICE_MAINTAIN             //设备维护
}VMCStatusEnum;

// 维护模式状态的枚举
typedef enum
{
    EXIT_MAINTAIN = 0,              //退出维护模式
    ACCESS_MAINTAIN                 //进入维护模式
}MaintainModeEnum;

// VMC开门控制的选择枚举
typedef enum
{
    DEVICE_DOOR = 0,                //设备大门
    GET_GOODS_DOOR                  //取物门
}OpenDoorSelectEnum;

// VMC开灯控制的选择枚举
typedef enum
{
    OUTLINE_LED = 0,                //示廓灯
    POSITION_LED                    //面板按键灯
}OpenLEDSelectEnum;

// 货道是否正常的枚举
typedef enum
{
    TUNNEL_NORMAL = 0x00,           //货道正常
    TUNNEL_ERROR  = 0x40            //货道故障
}TunnelRunStatusEnum;

//星期的枚举
typedef enum
{
    MONDAY = 1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
}WeekDayEnum;

//工作时间段枚举
typedef enum
{
    ENERGY_SAVE_TIME = 0,       //节能工作时间段
    SALE_WORK_TIME,             //售卖工作时间段
    OTHER_WORK_TIME             //其他时间段
}SysWorkTimeEnum;

//系统温度控制枚举
typedef enum
{
    HOLD_NORMAL_TEMP = 0,       //保持常温
    REFRIGERATION_TEMP,         //制冷
    WARMMING_TEMP               //加热
}SysTempCtrStatusEnum;

// 系统温度控制模式枚举
typedef enum
{
    ACCORD_TO_TIME_FRAME = 0,   //根据时间段控制
    ACCORD_TO_AUTO,             //自动控制
    ACCORD_TO_NO_TEMP           //没有下发温度，不控制
}SysTempCtrModeEnum;

// 系统运行状态变化标志枚举
typedef enum
{
    SYSTEM_RUN_STATUS_CHANGED = 0,      //有变化
    SYSTEM_RUN_STATUS_STAY              //没有变化
}SysRunStatusChangeEnum;


//出货状态相关类
//出货结果的状态枚举
typedef enum
{
    GET_GOODS_SUCCEED = 0,              //出货成功
    GET_GOODS_FAILURE = 1,              //出货失败，具体待讨论
    GET_GOODS_NO_GOODS_FAILURE = 2,     //该货道无商品出货失败
    GET_GOODS_ING = 0xFF                //正在出货
}GetGoodsResultEnum;

//补货状态枚举定义
typedef enum
{
    NO_REPLENISH = 0,   //没有补货动作
    REPLENISH_OVER      //补货完毕
}ReplenishStatusEnum;

//取货门开关状态的枚举
typedef enum
{
    DOOR_PULLOUT_OPEN = 0,  //取货门打开了
    DOOR_PULLOUT_CLOSE,     //取货门关闭
//    DOOR_PULLOUT_UNKNOWN    //取货门非开非关状态
}PulloutDoorSwitchEnum;

//定位的步进电机状态枚举
typedef enum
{
    ALL_STEP_MOTOR_NORMAL = 0,      //所有的步进电机都正常
    HORIZONTAL_MOTOR_FAULT,         //水平的步进电机故障
    VERTICAL_MOTOR_FAULT,           //垂直的步进电机故障
    ALL_STEP_MOTOR_FAULT,           //所有的步进电机故障
}LocateStepMotorStatusEnum;


//货斗处是否有货物的枚举
typedef enum
{
    BOX_NO_GOODS = 0,
    BOX_HAVE_GOODS          //货斗处有货物
}BoxGoodsStatusEnum;

//出货红外传感器状态枚举
typedef enum
{
    INFRARED_SENSOR_NORMAL = 0, //传感器正常
    INFRARED_SENSOR_FAULT
}PulloutInfraredSensorEnum;

//出货直流电机状态枚举
typedef enum
{
    ALL_PULLOUT_MOTOR_NORMAL = 0,   //所有的出货直流电机正常
    LEFT_PULLOUT_MOTOR_FAULT,       //左侧取货电机故障
    LEFT_ROTATE_MOTOR_FAULT,        //左侧旋转电机故障
    RIGHT_PULLOUT_MOTOR_FAULT,      //右侧取货电机故障
    RIGHT_ROTATE_MOTOR_FAULT,       //右侧旋转电机故障
    PULLOUT_DOOR_MOTOR_FAULT        //取货门电机故障
}PulloutMotorStatusEnum;

//出货过程的标志枚举
typedef enum
{
    GET_GOODS_IDLE = 0,     //没有出货，空闲中
    LOCATE_RUNING,          //定位货道中
    PULLOUT_RUNING,        //从货道取货中
    LOCATE_DOOR,            //定位取物门中
    WAIT_TAKE_GOODS,        //等待取走货物
    BACK_TO_ORIGIN,         //回原点中

    GET_GOODS_OVER,         //出货结束
    GET_GOODS_ERROR = 0xFF  //出货错误
}GetGoodsProcessEnum;


//控制线圈类
//系统复位/重启状态枚举
typedef enum
{
    SYSTEM_RST = 0,     //已复位成功
    SYSTEM_NORMAL       //没有启动
}SystemResetStatusEnum;

//状态指示灯状态枚举
typedef enum
{
    LED_TURN_OFF = 0,
    LED_TURN_ON         //指示灯打开
}IndicateLEDEnum;

//蜂鸣器状态枚举
typedef enum
{
    BUZZER_OPEN = 1,        //蜂鸣器打开
    BUZZER_CLOSE
}BuzzerStatusEnum;


//数据包续传标志枚举
typedef enum
{
    PACKAGE_UNCONTINUE_TRANS = 0,
    PACKAGE_CONTINUE_TRANS = 1
}PackageContinueTransStatusEnum;

//动作类型枚举
typedef enum
{
    ACTION_BUTTON = 0,      //按键动作
    ACTION_DEVICE_DOOR,      //设备大门动作
    ACTION_HUMANBODY_INFRARED//人体红外感应
}ActionTypeEnum;

//电机状态枚举
typedef enum
{
    MOTOR_NORMAL = 0,      //电机正常
    MOTOR_FAULT,            //电机故障
}SysMotorStatusEnum;

//取货门状态枚举
typedef enum
{
    DOOR_CLOSE = 0,     //取货门关闭
    DOOR_OPEN
}PulloutDoorStatusEnum;

// 货道剩余货物状态
typedef enum
{
    REMAIN_NO_GOODS             = 0,    //没有货物了
    REMAIN_HAVE_GOODS           = 1,    //还有货物
    REMAIN_UNKNOWN_GOODS_STATUS = 0xFF  //不能检测
}ChannelInnageStatus;

// 出货控制应答枚举
typedef enum
{
    PULLOUT_RIGHTNOW = 0,       //立即出货
    PULLOUT_BUSY,               //出货中
    PULLOUT_CHANNEL_WRONG       //出货货道号错误
}PulloutCtrReplyEnum;




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

//系统温度控制的计时
extern u16 g_SysTempCtrTimeCnt;

//系统工作时间判断的计时
extern u16 g_SysWorkTimeJudgeCnt;

//系统节能时间段标志
extern u8 g_SysEnergySaveTimeFlag;

//系统售卖时间段标志
extern u8 g_SysSaleTimeFlag;

// 系统节能时间段设置标志
extern u8 g_SysEnergySaveTimeSetFlag;

// 系统售卖时间设置标志
extern u8 g_SysSaleTimeSetFlag;

// 系统运行状态变化标志
extern u8 g_SysRunStatusChangeFlag;

// 设备端复位标志
extern u8 g_PulloutBoardReseted;

// 系统校时标志
extern u8 g_SysCorrectDateTimeFlag;

// 系统初始化状态标志
extern u8 g_SysInitStatusFlag;

// 系统进入/退出故障状态时保存整机状态值的变量
extern u8 g_SysAccessExitErrorVMCStatus;

//// 电磁锁打开后关闭的计时
//extern u16 g_SysEMLockOpenCnt;

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
