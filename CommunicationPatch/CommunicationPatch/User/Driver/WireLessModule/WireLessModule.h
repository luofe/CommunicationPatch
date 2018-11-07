
/*******************************************************************************
//WireLessModule.h
//配置无线模块的驱动头文件
备注: 
*******************************************************************************/

#ifndef	WIRELESSMODULE_H
#define	WIRELESSMODULE_H

/******************************************************************************
//宏定义
*******************************************************************************/
//AT指令的命令列表
#define AT_COMMAND_CPIN             0       //检测SIM卡状态
#define AT_COMMAND_CREG             1       //检测卡网络注册状态
#define AT_COMMAND_CGREG            2       //检测GPRS网络注册状态
#define AT_COMMAND_QICSGP           3       //配置APN和上下文ID
#define AT_COMMAND_CGQMIN           4       //配置可接受的最小服务质量
#define AT_COMMAND_QIDEACT          5       //失能PDP上下文
#define AT_COMMAND_QIACT_EN         6       //激活PDP上下文
#define AT_COMMAND_QIACT_DE         7       //检测PDP上下文
#define AT_COMMAND_CGQMIN_DE        8       //检测可接受的最小服务质量
#define AT_COMMAND_QIOPEN           9       //配置服务器IP地址和端口号，建立连接
#define AT_COMMAND_QICLOSE          10      //关闭连接
#define AT_COMMAND_SWITCH_CMD       11      //切换到命令模式“+++”，注意：没有回车换行！！！
#define AT_COMMAND_ATO              12      //切换回透传模式
#define AT_COMMAND_QCCID            13      //获取SIM卡的CCID号
#define AT_COMMAND_CIMI             14      //获取SIM卡的ISMI号
#define AT_COMMAND_CGSN             15      //获取SIM卡的IMEI号
#define AT_COMMAND_QPWOD            16      //控制模块关机（用于复位）
#define AT_COMMAND_CFUN             17      //设置模块功能模式（可用于复位）
//#define AT_COMMAND_READY            18      //等待模块启动后发送RDY



//上下文ID
#define WIRELESS_CONTEXT_ID         1       //1~16
//连接ID（Socket编号）
#define WIRELESS_CONNECT_ID         0       //0~11
//接入点APN
#define WIRELESS_APN                "CMNET" //中国移动、中国电信、中国联通等的英文缩写
//服务器类型
#define WIRELESS_SOCKET_TYPE        "TCP"   //有TCP、UDP、TCP-Listener等
//服务器IP地址
#define WIRELESS_SERVER_IP          "58.83.189.147"  
//服务器外网端口号
#define WIRELESS_SERVER_REMOTE_PORT "21009" 
//数据通道模式
#define WIRELESS_SERVER_ACCESS_MODE 2       //有0-Buffer模式、1-Direct模式 和 2-透传模式




// 等待无线模块重启完毕的最大等待时间
#define WIRELESS_WAIT_RDY_TIMEOUT       (1000 * 20)     //20s
// 等待SIM卡状态的最大等待时间
#define WIRELESS_WAIT_CPIN_TIMEOUT      (1000 * 3)      //20s
// 等待SIM卡的网络注册状态最大等待时间
#define WIRELESS_WAIT_CREG_TIMEOUT      (1000 * 90)     //90s
// 等待SIM卡的GPRS网络注册状态最大等待时间
#define WIRELESS_WAIT_CGREG_TIMEOUT     (1000 * 60)     //60s
// 等待配置APN和上下文ID的最大等待时间
#define WIRELESS_WAIT_QICSGP_TIMEOUT    (1000 * 1)     //1s
// 等待配置可接受的最小服务质量的最大等待时间
#define WIRELESS_WAIT_CGQMIN_TIMEOUT    (1000 * 1)     //1s
// 等待失能PDP上下文的最大等待时间
#define WIRELESS_WAIT_QIDEACT_TIMEOUT   (1000 * 10)    //10s
// 等待激活PDP上下文的最大等待时间
#define WIRELESS_WAIT_QIACT_EN_TIMEOUT  (1000 * 10)    //10s
// 等待检测PDP上下文的最大等待时间
#define WIRELESS_WAIT_QIACT_DE_TIMEOUT  (1000 * 10)    //10s 
// 等待检测可接受的最小服务质量的最大等待时间
#define WIRELESS_WAIT_CGQMIN_DE_TIMEOUT (1000 * 10)    //10s 
// 等待配置服务器IP地址和端口号、建立连接的最大等待时间
#define WIRELESS_WAIT_QIOPEN_TIMEOUT    (1000 * 10)    //10s

// AT指令的应答包含2部分，分别是重复一遍指令和应答指令
// 与SUCCEED、FAILURE同步使用，所以要避开前面2个值
#define RECEIVE_AT_COMMAND_REPEAT       2













/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/












/******************************************************************************
//变量定义/声明
*******************************************************************************/










/******************************************************************************
//函数声明
*******************************************************************************/
//功能: 发送AT指令的函数
void WireLess_Send_AT_Command(u8 cmd);



























#endif