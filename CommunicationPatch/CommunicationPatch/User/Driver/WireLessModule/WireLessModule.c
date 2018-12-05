
/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
// Functiom:负责配置和控制无线通信模块的源程序。
*******************************************************************************/

/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//变量定义
*******************************************************************************/
// 无线模块初始化完成标志
u8  g_WireLessModuleInitFlag = FALSE;









/******************************************************************************
//函数声明
*******************************************************************************/
//功能: 发送字符串给无线模块的函数
void WireLess_Send_Str(char *str);

//功能: 发送AT指令的函数
void WireLess_Send_AT_Command(u8 cmd);

//函数功能: 无线模块接收到的AT指令解析
u8 WireLess_AT_Command_Analysis(u8 cmd, u8* buf, u16 len, u8 repeat_sta);

//函数功能: 无线模块接收AT指令应答的监测函数
u8 WireLess_Rec_AT_Command_Monitor(u8 cmd);

//功能: 无线模块AT指令控制函数
u8 WireLess_AT_Command_Ctr(u8 cmd);

//功能: 无线模块初始化函数
u8 WireLess_Initial(void);















/******************************************************************************
//函数定义
*******************************************************************************/

/*****************************************
//名称: WireLess_Send_Str
//功能: 发送字符串给无线模块的函数
//入口: char *str——需要发送的数据
//出口: 无
******************************************/
void WireLess_Send_Str(char *str)
{

#if (SERVER_AT_PRINTF_EN)
    printf("\r\n发送AT指令给无线模块:");
#endif

    while(*str != '\0')  //发送AT命令
    {

#if (SERVER_AT_PRINTF_EN)
        printf("%c", *str);
#endif

        USART_SendData(SERVER_COMM_USART, *str++);
        while(USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_TXE) == RESET);//等待发送完成
    }

#if (SERVER_AT_PRINTF_EN)
    printf("\r\n");
#endif

}

/*****************************************
//名称: WireLess_Send_AT_Command
//功能: 发送AT指令的函数
//入口: u8 cmd——命令码
//出口: 无
******************************************/
void WireLess_Send_AT_Command(u8 cmd)
{
    char at_array[64];
    u16 index = 0;

    memset(at_array, '\0', sizeof(at_array));

    switch(cmd)
    {
        case AT_COMMAND_CPIN:   //检测SIM卡状态
        {
            strcpy(at_array, "AT+CPIN?");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_CREG:   //检测卡网络注册状态
        {
            strcpy(at_array, "AT+CREG?");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_CGREG:   //检测GPRS网络注册状态
        {
            strcpy(at_array, "AT+CGREG?");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_QICSGP:   //配置APN和上下文ID
        {
            u8 temp_mnc = ((s_SIMCardPara.IMSI[3] - 0x30) << 4);
            temp_mnc += (s_SIMCardPara.IMSI[4] - 0x30);
            //AT+QICSGP=<contextID>,<context_type>,[<APN>[,<username>,<password>)[,<authentication>]]]
            //AT+QICSGP=1,1,"CMNET","","",1
            strcpy(at_array, "AT+QICSGP=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d,", WIRELESS_CONTEXT_ID);
            strcat(at_array, "1,");
            index = strlen(at_array);
            switch(temp_mnc)
            {
                //如果是中国移动的
                case 0x00:
                case 0x02:
                case 0x04:
                case 0x07:
                {
                    sprintf(&at_array[index], "\"%s\",", WIRELESS_APN_CMNET);
                }
                break;
                //如果是中国电信的
                case 0x03:
                case 0x05:
                case 0x11:
                {
                    sprintf(&at_array[index], "\"%s\",", WIRELESS_APN_CTNET);
                }
                break;
                //如果是中国联通的
                case 0x01:
                case 0x06:
                case 0x09:
                {
                    sprintf(&at_array[index], "\"%s\",", WIRELESS_APN_3GNET);
                }
                break;

                default:
                {
                    sprintf(&at_array[index], "\"%s\",", WIRELESS_APN_CMNET);
                }
                break;
            }
            strcat(at_array, "\"\",");
            strcat(at_array, "\"\",");
            strcat(at_array, "1");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_CGQMIN:   //配置可接受的最小服务质量
        {
            //AT+CGQMIN=<contextID>[,<precedence>[,<delay>[,<reliability>[,<peak>[,<mean>]]]]]
            //AT+CGQMIN=1,1,4,5,2,31
            strcpy(at_array, "AT+CGQMIN=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d,", WIRELESS_CONTEXT_ID);
            strcat(at_array, "1,");
            strcat(at_array, "4,");
            strcat(at_array, "5,");
            strcat(at_array, "2,");
            strcat(at_array, "31");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_QIDEACT:   //失能PDP上下文
        {
            //AT+QIDEACT=<contextID>
            strcpy(at_array, "AT+QIDEACT=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d", WIRELESS_CONTEXT_ID);
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_QIACT_EN:   //激活PDP上下文
        {
            //AT+QIACT=<contextID>
            strcpy(at_array, "AT+QIACT=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d", WIRELESS_CONTEXT_ID);
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_QIACT_DE:   //检测PDP上下文
        {
            strcpy(at_array, "AT+QIACT?");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_CGQMIN_DE:   //检测可接受的最小服务质量
        {
            strcpy(at_array, "AT+CGQMIN?");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_QIOPEN:   //配置服务器IP地址和端口号，建立连接
        {
            //AT+QIOPEN=<contextID>,<connectID>,<service_type>,<IP_address>/<domain_name>,<remote_port>[,<local_port>[,<access_mode>]]
            //AT+QIOPEN=1,0,"TCP","58.83.189.147",21009,0,2
            strcpy(at_array, "AT+QIOPEN=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d,", WIRELESS_CONTEXT_ID);
            index = strlen(at_array);
            sprintf(&at_array[index], "%d,", WIRELESS_CONNECT_ID);
            index = strlen(at_array);
            sprintf(&at_array[index], "\"%s\",", WIRELESS_SOCKET_TYPE);
            index = strlen(at_array);
            strcat(at_array, (const char*)s_IPAddrPort.ip_port);
            strcat(at_array, ",");
            index = strlen(at_array);
//            sprintf(&at_array[index], "\"%s\",", WIRELESS_SERVER_IP);
//            index = strlen(at_array);
//            sprintf(&at_array[index], "%s,", WIRELESS_SERVER_REMOTE_PORT);
            strcat(at_array, "0,");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d", WIRELESS_SERVER_ACCESS_MODE);
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_QICLOSE:   //关闭连接
        {
            //AT+QICLOSE=<connectID>
            //AT+QICLOSE=0
            strcpy(at_array, "AT+QICLOSE=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d", WIRELESS_CONNECT_ID);
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_SWITCH_CMD:   //切换到命令模式
        {
            strcpy(at_array, "+++");    //注意：没有回车换行符！！！
        }
        break;

        case AT_COMMAND_ATO:   //切换回透传模式
        {
            strcpy(at_array, "ATO");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_QCCID:   //获取SIM卡的CCID号
        {
            strcpy(at_array, "AT+QCCID");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_CIMI:   //获取SIM卡的ISMI号
        {
            strcpy(at_array, "AT+CIMI");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_CGSN:   //获取SIM卡的IMEI号
        {
            strcpy(at_array, "AT+CGSN");
            strcat(at_array, "\r\n");
        }
        break;

        case AT_COMMAND_QPWOD:   //控制模块关机（用于复位）
        {
            strcpy(at_array, "AT+QPOWD=1"); //1—正常关机；0—硬关机
            strcat(at_array, "\r\n");
        }
        break;
    }
    WireLess_Send_Str(at_array);
}

//********************************************************
//函数名称: WireLess_AT_Command_Analysis
//函数功能: 无线模块接收到的AT指令解析
//输    入: u8 cmd——命令码, u8* buf—接收的数据，, u16 len——数据长度，u8 repeat_sta—AT指令重复状态（重复AT指令或者是应答包）
//输    出: u8——返回解析结果，成功或者失败
//备    注: 无
//********************************************************
u8 WireLess_AT_Command_Analysis(u8 cmd, u8* buf, u16 len, u8 repeat_sta)
{
    u16 i = 0;

    switch(cmd)
    {
        case AT_COMMAND_QPWOD:  //如果是重启命令
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'R') && (buf[i + 1] == 'D') && (buf[i + 2] == 'Y'))
                {
                    return SUCCEED;
                }
                else if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
                else if((buf[i] == 'P') && (buf[i + 1] == 'O') && (buf[i + 2] == 'W') && (buf[i + 3] == 'E')
                        && (buf[i + 4] == 'R') && (buf[i + 5] == 'E') && (buf[i + 6] == 'D'))
                {
                    return RECEIVE_AT_COMMAND_REPEAT;
                }
            }
        }
        break;

        case AT_COMMAND_CPIN:   //检测SIM卡状态
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CREG:   //检测卡的网络注册状态
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == '0') && (buf[i + 2] == '1') && (buf[i + 7] == 'O') && (buf[i + 8] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CGREG:  //检测卡的GPRS网络注册状态
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == '0') && (buf[i + 2] == '1') && (buf[i + 7] == 'O') && (buf[i + 8] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QICSGP:  //配置APN和上下文ID
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CGQMIN:  //配置可接受的最小服务质量
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QIDEACT:  //失能PDP上下文
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QIACT_EN:  //激活PDP上下文
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QIACT_DE:  //检测PDP上下文
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CGQMIN_DE:  //检测可接受的最小服务质量
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QIOPEN:  //配置服务器IP地址和端口号、建立连接
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'C') && (buf[i + 1] == 'O') && (buf[i + 2] == 'N')
                   && (buf[i + 3] == 'N') && (buf[i + 4] == 'E') && (buf[i + 5] == 'C')
                       && (buf[i + 6] == 'T'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QICLOSE:  //关闭TCP连接
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_SWITCH_CMD:  //切换到命令模式
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
                else if((buf[i] == 'R') && (buf[i + 1] == 'D') && (buf[i + 2] == 'Y'))
                {
                    return SUCCEED;
                }
            }
        }
        break;

        case AT_COMMAND_ATO:  //切换回透传模式
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'C') && (buf[i + 1] == 'O') && (buf[i + 2] == 'N')
                   && (buf[i + 3] == 'N') && (buf[i + 4] == 'E') && (buf[i + 5] == 'C')
                       && (buf[i + 6] == 'T'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QCCID:  //获取SIM卡的CCID号
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    s_SIMCardPara.CCID_len = 20;
                    memcpy(s_SIMCardPara.CCID, &buf[i - 4 - s_SIMCardPara.CCID_len], s_SIMCardPara.CCID_len);

                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CIMI:  //获取SIM卡的IMSI号
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    s_SIMCardPara.IMSI_len = 15;
                    memcpy(s_SIMCardPara.IMSI, &buf[i - 4 - s_SIMCardPara.IMSI_len], s_SIMCardPara.IMSI_len);

                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CGSN:  //获取SIM卡的IMEI号
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    s_SIMCardPara.IMEI_len = 15;
                    memcpy(s_SIMCardPara.IMEI, &buf[i - 4 - s_SIMCardPara.IMEI_len], s_SIMCardPara.IMEI_len);

                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //重复AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        default:
        break;
    }

    return FAILURE;
}

//********************************************************
//函数名称: WireLess_Rec_AT_Command_Monitor
//函数功能: 无线模块接收AT指令应答的监测函数
//输    入: u8 cmd——命令码
//输    出: u8——返回解析结果，成功或者失败
//备    注: 无
//********************************************************
u8 WireLess_Rec_AT_Command_Monitor(u8 cmd)
{
    u32 temp_time_count = 0;
    u16 temp_l = 0;
    u8  temp_sta = FAILURE; //解析结果：包括成功、失败、只是接收到无线模块对请求指令的重复包（还没有结果的）
    u8  repeat_sta = TRUE;  //无线模块对请求指令的重复包标志

    s_ServerCommRx.Status = FALSE;
    s_ServerCommRx.Index = 0;
    s_ServerCommRx.Timeout_Count = 0;
    while(9)
    {
        if(s_ServerCommRx.Status == TRUE)   //有接收
        {
            if(s_ServerCommRx.Timeout_Count >= WIRELESS_RX_AT_DATA_TIMEOUT)    //是否超时了
            {
                if((s_ServerCommRx.Index < 3))   //如果是无效值，抛弃 && (s_ServerCommRx.Buffer[0] == '\0')
                {
                    s_ServerCommRx.Status = FALSE;
                    s_ServerCommRx.Index = 0;
                    s_ServerCommRx.Timeout_Count = 0;

                    continue;
                }
                temp_l = s_ServerCommRx.Index;    //拷贝出数据长度
                //将数据拷贝至公共缓冲区，防止被新的数据淹没
                memcpy(g_PublicDataBuffer, s_ServerCommRx.Buffer, temp_l);

                s_ServerCommRx.Status = FALSE;
                s_ServerCommRx.Index = 0;
                s_ServerCommRx.Timeout_Count = 0;

#if (SERVER_AT_PRINTF_EN)
                printf("接收到无线模块应答AT：");
                for(u16 i = 0; i < temp_l; i++)
                {
                    printf("%c", g_PublicDataBuffer[i]);
                }
#endif

                //解析接收到的AT指令
                temp_sta = WireLess_AT_Command_Analysis(cmd, g_PublicDataBuffer, temp_l, repeat_sta);
                if(temp_sta == FAILURE)  //如果应答失败
                {
                    return FAILURE;
                }
                else if(temp_sta == SUCCEED)
                {
                    return SUCCEED;
                }
                else
                {
                    repeat_sta = FALSE;
                }
            }
        }
        //假如超时没有接收到想要的应答，则退出
        else
        {
            switch(cmd)
            {
                case AT_COMMAND_QPWOD://如果是重启命令
                {
                    temp_time_count = WIRELESS_WAIT_RDY_TIMEOUT;
                }
                break;

                case AT_COMMAND_CPIN://检测SIM卡状态
                {
                    temp_time_count = WIRELESS_WAIT_CPIN_TIMEOUT;
                }
                break;

                case AT_COMMAND_CREG:   //检测卡的网络注册状态
                {
                    temp_time_count = WIRELESS_WAIT_CREG_TIMEOUT;
                }
                break;

                case AT_COMMAND_CGREG:  //检测卡的GPRS网络注册状态
                {
                    temp_time_count = WIRELESS_WAIT_CGREG_TIMEOUT;
                }
                break;

                case AT_COMMAND_QICSGP:  //配置APN和上下文ID
                {
                    temp_time_count = WIRELESS_WAIT_QICSGP_TIMEOUT;
                }
                break;

                case AT_COMMAND_CGQMIN:  //配置可接受的最小服务质量
                {
                    temp_time_count = WIRELESS_WAIT_CGQMIN_TIMEOUT;
                }
                break;

                case AT_COMMAND_QIDEACT:  //失能PDP上下文
                {
                    temp_time_count = WIRELESS_WAIT_QIDEACT_TIMEOUT;
                }
                break;

                case AT_COMMAND_QIACT_EN:  //激活PDP上下文
                {
                    temp_time_count = WIRELESS_WAIT_QIACT_EN_TIMEOUT;
                }
                break;

                case AT_COMMAND_QIACT_DE:  //检测PDP上下文
                {
                    temp_time_count = WIRELESS_WAIT_QIACT_DE_TIMEOUT;
                }
                break;

                case AT_COMMAND_CGQMIN_DE:  //检测可接受的最小服务质量
                {
                    temp_time_count = WIRELESS_WAIT_CGQMIN_DE_TIMEOUT;
                }
                break;

                case AT_COMMAND_QIOPEN:  //配置服务器IP地址和端口号，建立连接
                {
                    temp_time_count = WIRELESS_WAIT_QIOPEN_TIMEOUT;
                }
                break;

                case AT_COMMAND_QICLOSE:  //关闭TCP连接
                {
                    temp_time_count = WIRELESS_WAIT_QICLOSE_TIMEOUT;
                }
                break;

                case AT_COMMAND_SWITCH_CMD:  //切换到命令模式“+++”
                {
                    temp_time_count = WIRELESS_WAIT_SWITCH_CMD_TIMEOUT;
                }
                break;

                case AT_COMMAND_ATO:  //切换回透传模式
                {
                    temp_time_count = WIRELESS_WAIT_ATO_TIMEOUT;
                }
                break;

                case AT_COMMAND_QCCID:  //获取SIM卡的CCID号
                {
                    temp_time_count = WIRELESS_WAIT_QCCID_TIMEOUT;
                }
                break;

                case AT_COMMAND_CIMI:  //获取SIM卡的ISMI号
                {
                    temp_time_count = WIRELESS_WAIT_CIMI_TIMEOUT;
                }
                break;

                case AT_COMMAND_CGSN:  //获取SIM卡的IMEI号
                {
                    temp_time_count = WIRELESS_WAIT_CGSN_TIMEOUT;
                }
                break;

                default:
                break;
            }

            if(s_ServerCommRx.Timeout_Count >= temp_time_count)
            {
                s_ServerCommRx.Timeout_Count = 0;

                return FAILURE;
            }
        }
    }
}

/*****************************************
//名称: WireLess_AT_Command_Ctr
//功能: 无线模块AT指令控制函数
//入口: u8 cmd—命令码
//出口: u8—返回控制结果
******************************************/
u8 WireLess_AT_Command_Ctr(u8 cmd)
{
    u8 index;
    u8 temp_sta = FAILURE;

    WireLess_Send_AT_Command(cmd);

    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(cmd);
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
                WireLess_Send_AT_Command(cmd);
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
//名称: WireLess_Initial
//功能: 无线模块初始化函数
//入口: 无
//出口: u8—返回初始化状态
******************************************/
u8 WireLess_Initial(void)
{
    u8  index = 0;
//    u8  temp_sta = FALSE;

#if (SERVER_AT_PRINTF_EN)
    printf("等待无线模块退出透传模式\r\n");
#endif

    //先进入命令模式
//    Delay_ms(1500);
    if(WireLess_AT_Command_Ctr(AT_COMMAND_SWITCH_CMD) == FAILURE)
    {

#if (SERVER_AT_PRINTF_EN)
        printf("等待无线模块启动后的\"RDY\"\r\n");
#endif

//        //先等待接收无线模块启动后的“RDY”
//        if(WireLess_AT_Command_Ctr(AT_COMMAND_QPWOD) == FAILURE)
//        {
//            return FAILURE;
//        }
    }

#if (SERVER_AT_PRINTF_EN)
    printf("检测SIM卡状态\r\n");
#endif

    //检测SIM卡状态
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CPIN) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("获取SIM卡的CCID号\r\n");
#endif

    //获取SIM卡的CCID号
    if(WireLess_AT_Command_Ctr(AT_COMMAND_QCCID) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("获取SIM卡的IMSI号\r\n");
#endif

    //获取SIM卡的IMSI号
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CIMI) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("获取SIM卡的IMEI号\r\n");
#endif

    //获取SIM卡的IMEI号
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CGSN) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("检测卡的网络注册状态\r\n");
#endif

    //检测卡的网络注册状态
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CREG) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("检测卡的GPRS网络注册状态\r\n");
#endif

    //检测卡的GPRS网络注册状态
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CGREG) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("配置APN和上下文ID\r\n");
#endif

    //配置APN和上下文ID
    if(WireLess_AT_Command_Ctr(AT_COMMAND_QICSGP) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("配置可接受的最小服务质量\r\n");
#endif

    //配置可接受的最小服务质量
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CGQMIN) == FAILURE)
    {
        return FAILURE;
    }

    ForceReconnect:
//    if(temp_sta == TRUE)
    {

#if (SERVER_AT_PRINTF_EN)
        printf("失能PDP上下文\r\n");
#endif

        //失能PDP上下文
        if(WireLess_AT_Command_Ctr(AT_COMMAND_QIDEACT) == FAILURE)
        {
            return FAILURE;
        }
    }

#if (SERVER_AT_PRINTF_EN)
    printf("激活PDP上下文\r\n");
#endif

    //激活PDP上下文
    index = 0;
    if(WireLess_AT_Command_Ctr(AT_COMMAND_QIACT_EN) == FAILURE)
    {
        index++;
        if(index >= 2)
        {
            return FAILURE;
        }
        else
        {
//            temp_sta = TRUE;
            goto ForceReconnect;
        }
    }
    index = 0;
//    temp_sta = FALSE;

#if (SERVER_AT_PRINTF_EN)
    printf("检测PDP上下文\r\n");
#endif

    //检测PDP上下文
    if(WireLess_AT_Command_Ctr(AT_COMMAND_QIACT_DE) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("检测可接受的最小服务质量\r\n");
#endif

    //检测可接受的最小服务质量
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CGQMIN_DE) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("配置服务器IP地址和端口号，建立连接\r\n");
#endif

    //配置服务器IP地址和端口号，建立连接
    index = 0;
    if(WireLess_AT_Command_Ctr(AT_COMMAND_QIOPEN) == FAILURE)
    {
        index++;
        if(index >= 2)
        {
            return FAILURE;
        }
        else
        {
//            temp_sta = TRUE;
            goto ForceReconnect;
        }
    }

    //    Delay_ms(100);

    g_WireLessModuleInitFlag = TRUE;    //无线模块初始化完成

#if (SERVER_AT_PRINTF_EN)
    printf("无线模块初始化成功！\r\n");
#endif

    return SUCCEED;
}






//////////////////////////////END OF C文件////////////////////////////////////
