
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
        
#if (SERVER_PRINTF_EN)
printf("\r\n发送AT指令给WL:");
#endif
    
    while(*str != '\0')  //发送AT命令
    {
        
#if (SERVER_PRINTF_EN)
printf("%c ", *str);
#endif
            
        USART_SendData(SERVER_COMM_USART, *str++); 
        while(USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_TXE) == RESET);//等待发送完成
    } 
        
#if (SERVER_PRINTF_EN)
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
            //AT+QICSGP=<contextID>,<context_type>,[<APN>[,<username>,<password>)[,<authentication>]]]
            //AT+QICSGP=1,1,"CMNET","","",1
            strcpy(at_array, "AT+QICSGP=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d,", WIRELESS_CONTEXT_ID);
            strcat(at_array, "1,");
            index = strlen(at_array);
            sprintf(&at_array[index], "\"%s\",", WIRELESS_APN);
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
            sprintf(&at_array[index], "\"%s\",", WIRELESS_SERVER_IP);
            index = strlen(at_array);
            sprintf(&at_array[index], "%s,", WIRELESS_SERVER_REMOTE_PORT);
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
    u16 str_len = 0;
    
    switch(cmd)
    {
        case AT_COMMAND_QPWOD:  //如果是重启命令
        {
            str_len = strlen("RDY");
            for(i = 0; i < len - (str_len - 1); i++)
            {
                if((g_PublicDataBuffer[i] == 'R') && (g_PublicDataBuffer[i + 1] == 'D') && (g_PublicDataBuffer[i + 2] == 'Y'))
                {
                    return SUCCEED;
                }
            }
        }
        break;
        
        case AT_COMMAND_CPIN:   //检测SIM卡状态
        {
            str_len = strlen("AT+CPIN?\r\n\r\n+CPIN: READY\r\n\r\nOK\r\n");
            if(len < str_len)
            {
                if(repeat_sta == TRUE)  //重复AT
                {
                    for(i = 0; i < len - (str_len - 1); i++)
                    {
                        if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                           && (g_PublicDataBuffer[i + 3] == 'C') && (g_PublicDataBuffer[i + 4] == 'P') && (g_PublicDataBuffer[i + 5] == 'I')
                               && (g_PublicDataBuffer[i + 6] == 'N') && (g_PublicDataBuffer[i + 7] == '?'))
                        {
//                        if((g_PublicDataBuffer[i + 19] == 'R') && (g_PublicDataBuffer[i + 20] == 'E') && (g_PublicDataBuffer[i + 21] == 'A')
//                           && (g_PublicDataBuffer[i + 22] == 'D') && (g_PublicDataBuffer[i + 23] == 'Y'))
                            {
                                return RECEIVE_AT_COMMAND_REPEAT;
                            }
                        }
                    }
                }
                else    //应答
                {
                    str_len = strlen("\r\nOK\r\n");
                    for(i = 0; i < len - (str_len - 1); i++)
                    {
                        if((g_PublicDataBuffer[i] == 'O') && (g_PublicDataBuffer[i + 1] == 'K'))
                        {
                            return SUCCEED;
                        }
                    }
                }
            }
            else
            {
                for(i = 0; i < len - (str_len - 1); i++)
                {
                    if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                       && (g_PublicDataBuffer[i + 3] == 'C') && (g_PublicDataBuffer[i + 4] == 'P') && (g_PublicDataBuffer[i + 5] == 'I')
                           && (g_PublicDataBuffer[i + 6] == 'N') && (g_PublicDataBuffer[i + 7] == '?'))
                    {
                        if((g_PublicDataBuffer[i + 19] == 'R') && (g_PublicDataBuffer[i + 20] == 'E') && (g_PublicDataBuffer[i + 21] == 'A')
                           && (g_PublicDataBuffer[i + 22] == 'D') && (g_PublicDataBuffer[i + 23] == 'Y') 
                           && (g_PublicDataBuffer[i + 28] == 'O') && (g_PublicDataBuffer[i + 29] == 'K'))
                        {
                            return SUCCEED;
                        }
                    }
                }
            }
            
        }
        break;
        
        case AT_COMMAND_CREG:   //检测卡的网络注册状态
        {
            str_len = strlen("AT+CREG?\r\n\r\n+CREG: 0,1\r\n\r\nOK\r\n");
            if(len < str_len)
            {
                if(repeat_sta == TRUE)  //重复AT
                {
                    for(i = 0; i < len - (str_len - 1); i++)
                    {
                        if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                           && (g_PublicDataBuffer[i + 3] == 'C') && (g_PublicDataBuffer[i + 4] == 'R') && (g_PublicDataBuffer[i + 5] == 'E')
                               && (g_PublicDataBuffer[i + 6] == 'G') && (g_PublicDataBuffer[i + 7] == '?'))
                        {
//                            if((g_PublicDataBuffer[i + 19] == '0') && (g_PublicDataBuffer[i + 21] == '1')
//                               && (g_PublicDataBuffer[i + 26] == 'O') && (g_PublicDataBuffer[i + 27] == 'K'))
                            {
                                return RECEIVE_AT_COMMAND_REPEAT;
                            }
                        }
                    }
                }
                else
                {
                    str_len = strlen("\r\nOK\r\n");
                    for(i = 0; i < len - (str_len - 1); i++)
                    {
                        if((g_PublicDataBuffer[i] == 'O') && (g_PublicDataBuffer[i + 1] == 'K'))
                        {
                            return SUCCEED;
                        }
                    }
                }
            }
            else
            {
                for(i = 0; i < len - (str_len - 1); i++)
                {
                    if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                       && (g_PublicDataBuffer[i + 3] == 'C') && (g_PublicDataBuffer[i + 4] == 'R') && (g_PublicDataBuffer[i + 5] == 'E')
                           && (g_PublicDataBuffer[i + 6] == 'G') && (g_PublicDataBuffer[i + 7] == '?'))
                    {
                        if((g_PublicDataBuffer[i + 19] == '0') && (g_PublicDataBuffer[i + 21] == '1')
                           && (g_PublicDataBuffer[i + 26] == 'O') && (g_PublicDataBuffer[i + 27] == 'K'))
                        {
                            return SUCCEED;
                        }
                    }
                }
            }
        }
        break;
        
        case AT_COMMAND_CGREG:  //检测卡的GPRS网络注册状态
        {
            str_len = strlen("AT+CGREG?\r\n\r\n+CGREG: 0,1\r\n\r\nOK\r\n");
            for(i = 0; i < len - (str_len - 1); i++)
            {
                if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                    && (g_PublicDataBuffer[i + 3] == 'C') && (g_PublicDataBuffer[i + 4] == 'G') && (g_PublicDataBuffer[i + 5] == 'R') 
                    && (g_PublicDataBuffer[i + 6] == 'E') && (g_PublicDataBuffer[i + 7] == 'G') && (g_PublicDataBuffer[i + 8] == '?'))
                {
                    if((g_PublicDataBuffer[i + 21] == '0') && (g_PublicDataBuffer[i + 23] == '1')
                        && (g_PublicDataBuffer[i + 28] == 'O') && (g_PublicDataBuffer[i + 29] == 'K'))
                    {
                        return SUCCEED;
                    }
                }
            }
        }
        break;
        
        case WIRELESS_WAIT_QICSGP_TIMEOUT:  //配置APN和上下文ID
        {
            str_len = strlen("AT+QICSGP=1,1,\"CMNET\",\"\",\"\",1\r\n\r\nOK\r\n");
            for(i = 0; i < len - (str_len - 1); i++)
            {
                if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                    && (g_PublicDataBuffer[i + 3] == 'Q') && (g_PublicDataBuffer[i + 4] == 'I') && (g_PublicDataBuffer[i + 5] == 'C') 
                    && (g_PublicDataBuffer[i + 6] == 'S') && (g_PublicDataBuffer[i + 7] == 'G') && (g_PublicDataBuffer[i + 8] == 'P'))
                {
                    if((g_PublicDataBuffer[i + 33] == 'O') && (g_PublicDataBuffer[i + 34] == 'K'))
                    {
                        return SUCCEED;
                    }
                }
            }
        }
        break;
        
        case AT_COMMAND_CGQMIN:  //配置可接受的最小服务质量
        {
            str_len = strlen("AT+CGQMIN=1,1,4,5,2,31\r\n\r\nOK\r\n");
            for(i = 0; i < len - (str_len - 1); i++)
            {
                if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                    && (g_PublicDataBuffer[i + 3] == 'C') && (g_PublicDataBuffer[i + 4] == 'G') && (g_PublicDataBuffer[i + 5] == 'Q') 
                    && (g_PublicDataBuffer[i + 6] == 'M') && (g_PublicDataBuffer[i + 7] == 'I') && (g_PublicDataBuffer[i + 8] == 'N'))
                {
                    if((g_PublicDataBuffer[i + 26] == 'O') && (g_PublicDataBuffer[i + 27] == 'K'))
                    {
                        return SUCCEED;
                    }
                }
            }
        }
        break;
        
        case AT_COMMAND_QIDEACT:  //失能PDP上下文
        {
            str_len = strlen("AT+QIDEACT=1\r\n\r\nOK\r\n");
            for(i = 0; i < len - (str_len - 1); i++)
            {
                if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                    && (g_PublicDataBuffer[i + 3] == 'Q') && (g_PublicDataBuffer[i + 4] == 'I') && (g_PublicDataBuffer[i + 5] == 'D') 
                    && (g_PublicDataBuffer[i + 6] == 'E') && (g_PublicDataBuffer[i + 7] == 'A') && (g_PublicDataBuffer[i + 8] == 'C') 
                    && (g_PublicDataBuffer[i + 9] == 'T'))
                {
                    if((g_PublicDataBuffer[i + 16] == 'O') && (g_PublicDataBuffer[i + 17] == 'K'))
                    {
                        return SUCCEED;
                    }
                }
            }
        }
        break;
        
        case AT_COMMAND_QIACT_EN:  //激活PDP上下文
        {
            str_len = strlen("AT+QIACT=1\r\n\r\nOK\r\n");
            for(i = 0; i < len - (str_len - 1); i++)
            {
                if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                    && (g_PublicDataBuffer[i + 3] == 'Q') && (g_PublicDataBuffer[i + 4] == 'I') && (g_PublicDataBuffer[i + 5] == 'A') 
                    && (g_PublicDataBuffer[i + 6] == 'C') && (g_PublicDataBuffer[i + 7] == 'T'))
                {
                    if((g_PublicDataBuffer[i + 14] == 'O') && (g_PublicDataBuffer[i + 15] == 'K'))
                    {
                        return SUCCEED;
                    }
                }
            }
        }
        break;
        
        case AT_COMMAND_QIACT_DE:  //检测PDP上下文
        {
            str_len = strlen("AT+QIACT?\r\n\r\n+QIACT: 1,1,1,\"10.219.117.219\"\r\n\r\nOK\r\n");
            for(i = 0; i < len - (str_len - 1); i++)
            {
                if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                    && (g_PublicDataBuffer[i + 3] == 'Q') && (g_PublicDataBuffer[i + 4] == 'I') && (g_PublicDataBuffer[i + 5] == 'A') 
                    && (g_PublicDataBuffer[i + 6] == 'C') && (g_PublicDataBuffer[i + 7] == 'T') && (g_PublicDataBuffer[i + 8] == '?'))
                {
                    if((g_PublicDataBuffer[i + 21] == '1') && (g_PublicDataBuffer[i + 23] == '1') 
                       && (g_PublicDataBuffer[i + 25] == '1'))
                    {
                        return SUCCEED;
                    }
                }
            }
        }
        break;
        
        case AT_COMMAND_CGQMIN_DE:  //检测可接受的最小服务质量
        {
            str_len = strlen("AT+CGQMIN?\r\n\r\n+CGQMIN: 1,1,4,5,2,31\r\n\r\nOK\r\n");
            for(i = 0; i < len - (str_len - 1); i++)
            {
                if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                    && (g_PublicDataBuffer[i + 3] == 'C') && (g_PublicDataBuffer[i + 4] == 'G') && (g_PublicDataBuffer[i + 5] == 'Q') 
                    && (g_PublicDataBuffer[i + 6] == 'M') && (g_PublicDataBuffer[i + 7] == 'I') && (g_PublicDataBuffer[i + 8] == 'N'))
                {
                    if((g_PublicDataBuffer[i + 39] == 'O') && (g_PublicDataBuffer[i + 40] == 'K'))
                    {
                        return SUCCEED;
                    }
                }
            }
        }
        break;
        
        case AT_COMMAND_QIOPEN:  //配置服务器IP地址和端口号、建立连接
        {
            if(repeat_sta == TRUE)
            {
                str_len = strlen("AT+QIOPEN=1,0,\"TCP\",\"58.83.189.147\",21009,0,2\r\n");
                for(i = 0; i < len - (str_len - 1); i++)
                {
                    if((g_PublicDataBuffer[i] == 'A') && (g_PublicDataBuffer[i + 1] == 'T') && (g_PublicDataBuffer[i + 2] == '+') 
                       && (g_PublicDataBuffer[i + 3] == 'Q') && (g_PublicDataBuffer[i + 4] == 'I') && (g_PublicDataBuffer[i + 5] == 'O') 
                           && (g_PublicDataBuffer[i + 6] == 'P') && (g_PublicDataBuffer[i + 7] == 'E') && (g_PublicDataBuffer[i + 8] == 'N'))
                    {
                        if((g_PublicDataBuffer[i + 10] == '1') && (g_PublicDataBuffer[i + 12] == '0'))
                        {
                            return RECEIVE_AT_COMMAND_REPEAT;
                        }
                    }
                }
            }
            else
            {
                str_len = strlen("\r\nCONNECT\r\n");
                for(i = 0; i < len - (str_len - 1); i++)
                {
                    if((g_PublicDataBuffer[i] == 'C') && (g_PublicDataBuffer[i + 1] == 'O') && (g_PublicDataBuffer[i + 2] == 'N') 
                       && (g_PublicDataBuffer[i + 3] == 'N') && (g_PublicDataBuffer[i + 4] == 'E') && (g_PublicDataBuffer[i + 5] == 'C') 
                       && (g_PublicDataBuffer[i + 6] == 'T'))
                    {
                        return SUCCEED;
                    }
                }
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
    u8  temp_sta = FAILURE;
    u8  repeat_sta = TRUE;
    
    s_ServerCommRx.Timeout_Count = 0;
    while(9)
    {
        if(s_ServerCommRx.Status == TRUE)   //有接收
        {
            if(s_ServerCommRx.Timeout_Count >= SERVER_COMM_RX_DATA_TIMEOUT)    //是否超时了
            {
                temp_l = s_ServerCommRx.Index;    //拷贝出数据长度
                //将数据拷贝至公共缓冲区，防止被新的数据淹没
                memcpy(g_PublicDataBuffer, s_ServerCommRx.Buffer, temp_l); 
                
                s_ServerCommRx.Status = FALSE;
                s_ServerCommRx.Index = 0;
                s_ServerCommRx.Timeout_Count = 0;
                
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
//名称: WireLess_Initial
//功能: 无线模块初始化函数
//入口: 无
//出口: u8—返回初始化状态
******************************************/
u8 WireLess_Initial(void)
{
    u8 index;
    u8 temp_sta = FAILURE;
    
    //1、先等待接收无线模块启动后的“RDY”
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QPWOD);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_QPWOD);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    //2、检测SIM卡状态
    WireLess_Send_AT_Command(AT_COMMAND_CPIN);
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CPIN);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_CPIN);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //3、检测卡的网络注册状态
    WireLess_Send_AT_Command(AT_COMMAND_CREG); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CREG);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_CREG);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //4、检测卡的GPRS网络注册状态
    WireLess_Send_AT_Command(AT_COMMAND_CGREG); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CGREG);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_CGREG);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //5、配置APN和上下文ID
    WireLess_Send_AT_Command(AT_COMMAND_QICSGP); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QICSGP);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_QICSGP);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //6、配置可接受的最小服务质量
    WireLess_Send_AT_Command(AT_COMMAND_CGQMIN); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CGQMIN);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_CGQMIN);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //7、失能PDP上下文
    WireLess_Send_AT_Command(AT_COMMAND_QIDEACT); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QIDEACT);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_QIDEACT);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //8、激活PDP上下文
    WireLess_Send_AT_Command(AT_COMMAND_QIACT_EN); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QIACT_EN);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_QIACT_EN);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //9、检测PDP上下文
    WireLess_Send_AT_Command(AT_COMMAND_QIACT_DE); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QIACT_DE);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_QIACT_DE);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //10、检测可接受的最小服务质量
    WireLess_Send_AT_Command(AT_COMMAND_CGQMIN_DE); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CGQMIN_DE);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_CGQMIN_DE);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    //11、配置服务器IP地址和端口号，建立连接
    WireLess_Send_AT_Command(AT_COMMAND_QIOPEN); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QIOPEN);
        if(temp_sta == FAILURE) //如果失败了
        {
            index++;
            if(index > 1)   //超时1次，就退出
            {
                return FAILURE;
            }
            else
            {
                //重启无线模块
                WireLess_Send_AT_Command(AT_COMMAND_QIOPEN);
            }
        }
        else    //如果成功了，则进入下一步
        {
            break;
        }
    }
    
    return SUCCEED;
}






//////////////////////////////END OF C文件////////////////////////////////////
