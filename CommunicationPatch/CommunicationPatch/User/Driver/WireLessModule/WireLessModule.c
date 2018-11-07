
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:�������úͿ�������ͨ��ģ���Դ����
*******************************************************************************/

/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//��������
*******************************************************************************/









/******************************************************************************
//��������
*******************************************************************************/
//����: �����ַ���������ģ��ĺ���
void WireLess_Send_Str(char *str);

//����: ����ATָ��ĺ���
void WireLess_Send_AT_Command(u8 cmd);

//��������: ����ģ����յ���ATָ�����
u8 WireLess_AT_Command_Analysis(u8 cmd, u8* buf, u16 len, u8 repeat_sta);

//��������: ����ģ�����ATָ��Ӧ��ļ�⺯��
u8 WireLess_Rec_AT_Command_Monitor(u8 cmd);















/******************************************************************************
//��������
*******************************************************************************/

/*****************************************
//����: WireLess_Send_Str
//����: �����ַ���������ģ��ĺ���
//���: char *str������Ҫ���͵�����
//����: ��
******************************************/
void WireLess_Send_Str(char *str)
{  
        
#if (SERVER_PRINTF_EN)
printf("\r\n����ATָ���WL:");
#endif
    
    while(*str != '\0')  //����AT����
    {
        
#if (SERVER_PRINTF_EN)
printf("%c ", *str);
#endif
            
        USART_SendData(SERVER_COMM_USART, *str++); 
        while(USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_TXE) == RESET);//�ȴ��������
    } 
        
#if (SERVER_PRINTF_EN)
printf("\r\n");
#endif
              
}

/*****************************************
//����: WireLess_Send_AT_Command
//����: ����ATָ��ĺ���
//���: u8 cmd����������
//����: ��
******************************************/
void WireLess_Send_AT_Command(u8 cmd)
{
    char at_array[64];
    u16 index = 0;
    
    memset(at_array, '\0', sizeof(at_array));
    
    switch(cmd)
    {
        case AT_COMMAND_CPIN:   //���SIM��״̬
        {
            strcpy(at_array, "AT+CPIN?");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_CREG:   //��⿨����ע��״̬
        {
            strcpy(at_array, "AT+CREG?");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_CGREG:   //���GPRS����ע��״̬
        {
            strcpy(at_array, "AT+CGREG?");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_QICSGP:   //����APN��������ID
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
        
        case AT_COMMAND_CGQMIN:   //���ÿɽ��ܵ���С��������
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
        
        case AT_COMMAND_QIDEACT:   //ʧ��PDP������
        {
            //AT+QIDEACT=<contextID>
            strcpy(at_array, "AT+QIDEACT=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d", WIRELESS_CONTEXT_ID);
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_QIACT_EN:   //����PDP������
        {
            //AT+QIACT=<contextID>
            strcpy(at_array, "AT+QIACT=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d", WIRELESS_CONTEXT_ID);
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_QIACT_DE:   //���PDP������
        {
            strcpy(at_array, "AT+QIACT?");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_CGQMIN_DE:   //���ɽ��ܵ���С��������
        {
            strcpy(at_array, "AT+CGQMIN?");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_QIOPEN:   //���÷�����IP��ַ�Ͷ˿ںţ���������
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
        
        case AT_COMMAND_QICLOSE:   //�ر�����
        {
            //AT+QICLOSE=<connectID>
            //AT+QICLOSE=0
            strcpy(at_array, "AT+QICLOSE=");
            index = strlen(at_array);
            sprintf(&at_array[index], "%d", WIRELESS_CONNECT_ID);
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_SWITCH_CMD:   //�л�������ģʽ
        {
            strcpy(at_array, "+++");    //ע�⣺û�лس����з�������
        }
        break;
        
        case AT_COMMAND_ATO:   //�л���͸��ģʽ
        {
            strcpy(at_array, "ATO");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_QCCID:   //��ȡSIM����CCID��
        {
            strcpy(at_array, "AT+QCCID");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_CIMI:   //��ȡSIM����ISMI��
        {
            strcpy(at_array, "AT+CIMI");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_CGSN:   //��ȡSIM����IMEI��
        {
            strcpy(at_array, "AT+CGSN");
            strcat(at_array, "\r\n");
        }
        break;
        
        case AT_COMMAND_QPWOD:   //����ģ��ػ������ڸ�λ��
        {
            strcpy(at_array, "AT+QPOWD=1"); //1�������ػ���0��Ӳ�ػ�
            strcat(at_array, "\r\n");
        }
        break;
    }
    WireLess_Send_Str(at_array);
}

//********************************************************
//��������: WireLess_AT_Command_Analysis
//��������: ����ģ����յ���ATָ�����
//��    ��: u8 cmd����������, u8* buf�����յ����ݣ�, u16 len�������ݳ��ȣ�u8 repeat_sta��ATָ���ظ�״̬���ظ�ATָ�������Ӧ�����
//��    ��: u8�������ؽ���������ɹ�����ʧ��
//��    ע: ��
//********************************************************
u8 WireLess_AT_Command_Analysis(u8 cmd, u8* buf, u16 len, u8 repeat_sta)
{
    u16 i = 0;
    u16 str_len = 0;
    
    switch(cmd)
    {
        case AT_COMMAND_QPWOD:  //�������������
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
        
        case AT_COMMAND_CPIN:   //���SIM��״̬
        {
            str_len = strlen("AT+CPIN?\r\n\r\n+CPIN: READY\r\n\r\nOK\r\n");
            if(len < str_len)
            {
                if(repeat_sta == TRUE)  //�ظ�AT
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
                else    //Ӧ��
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
        
        case AT_COMMAND_CREG:   //��⿨������ע��״̬
        {
            str_len = strlen("AT+CREG?\r\n\r\n+CREG: 0,1\r\n\r\nOK\r\n");
            if(len < str_len)
            {
                if(repeat_sta == TRUE)  //�ظ�AT
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
        
        case AT_COMMAND_CGREG:  //��⿨��GPRS����ע��״̬
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
        
        case WIRELESS_WAIT_QICSGP_TIMEOUT:  //����APN��������ID
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
        
        case AT_COMMAND_CGQMIN:  //���ÿɽ��ܵ���С��������
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
        
        case AT_COMMAND_QIDEACT:  //ʧ��PDP������
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
        
        case AT_COMMAND_QIACT_EN:  //����PDP������
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
        
        case AT_COMMAND_QIACT_DE:  //���PDP������
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
        
        case AT_COMMAND_CGQMIN_DE:  //���ɽ��ܵ���С��������
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
        
        case AT_COMMAND_QIOPEN:  //���÷�����IP��ַ�Ͷ˿ںš���������
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
//��������: WireLess_Rec_AT_Command_Monitor
//��������: ����ģ�����ATָ��Ӧ��ļ�⺯��
//��    ��: u8 cmd����������
//��    ��: u8�������ؽ���������ɹ�����ʧ��
//��    ע: ��
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
        if(s_ServerCommRx.Status == TRUE)   //�н���
        {
            if(s_ServerCommRx.Timeout_Count >= SERVER_COMM_RX_DATA_TIMEOUT)    //�Ƿ�ʱ��
            {
                temp_l = s_ServerCommRx.Index;    //���������ݳ���
                //�����ݿ�������������������ֹ���µ�������û
                memcpy(g_PublicDataBuffer, s_ServerCommRx.Buffer, temp_l); 
                
                s_ServerCommRx.Status = FALSE;
                s_ServerCommRx.Index = 0;
                s_ServerCommRx.Timeout_Count = 0;
                
                //�������յ���ATָ��
                temp_sta = WireLess_AT_Command_Analysis(cmd, g_PublicDataBuffer, temp_l, repeat_sta);
                if(temp_sta == FAILURE)  //���Ӧ��ʧ��
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
        //���糬ʱû�н��յ���Ҫ��Ӧ�����˳�
        else 
        {
            switch(cmd)
            {
                case AT_COMMAND_QPWOD://�������������
                {
                    temp_time_count = WIRELESS_WAIT_RDY_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_CPIN://���SIM��״̬
                {
                    temp_time_count = WIRELESS_WAIT_CPIN_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_CREG:   //��⿨������ע��״̬
                {
                    temp_time_count = WIRELESS_WAIT_CREG_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_CGREG:  //��⿨��GPRS����ע��״̬
                {
                    temp_time_count = WIRELESS_WAIT_CGREG_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_QICSGP:  //����APN��������ID
                {
                    temp_time_count = WIRELESS_WAIT_QICSGP_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_CGQMIN:  //���ÿɽ��ܵ���С��������
                {
                    temp_time_count = WIRELESS_WAIT_CGQMIN_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_QIDEACT:  //ʧ��PDP������
                {
                    temp_time_count = WIRELESS_WAIT_QIDEACT_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_QIACT_EN:  //����PDP������
                {
                    temp_time_count = WIRELESS_WAIT_QIACT_EN_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_QIACT_DE:  //���PDP������
                {
                    temp_time_count = WIRELESS_WAIT_QIACT_DE_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_CGQMIN_DE:  //���ɽ��ܵ���С��������
                {
                    temp_time_count = WIRELESS_WAIT_CGQMIN_DE_TIMEOUT;
                }
                break;
                
                case AT_COMMAND_QIOPEN:  //���÷�����IP��ַ�Ͷ˿ںţ���������
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
//����: WireLess_Initial
//����: ����ģ���ʼ������
//���: ��
//����: u8�����س�ʼ��״̬
******************************************/
u8 WireLess_Initial(void)
{
    u8 index;
    u8 temp_sta = FAILURE;
    
    //1���ȵȴ���������ģ��������ġ�RDY��
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QPWOD);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_QPWOD);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    //2�����SIM��״̬
    WireLess_Send_AT_Command(AT_COMMAND_CPIN);
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CPIN);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_CPIN);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //3����⿨������ע��״̬
    WireLess_Send_AT_Command(AT_COMMAND_CREG); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CREG);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_CREG);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //4����⿨��GPRS����ע��״̬
    WireLess_Send_AT_Command(AT_COMMAND_CGREG); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CGREG);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_CGREG);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //5������APN��������ID
    WireLess_Send_AT_Command(AT_COMMAND_QICSGP); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QICSGP);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_QICSGP);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //6�����ÿɽ��ܵ���С��������
    WireLess_Send_AT_Command(AT_COMMAND_CGQMIN); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CGQMIN);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_CGQMIN);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //7��ʧ��PDP������
    WireLess_Send_AT_Command(AT_COMMAND_QIDEACT); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QIDEACT);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_QIDEACT);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //8������PDP������
    WireLess_Send_AT_Command(AT_COMMAND_QIACT_EN); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QIACT_EN);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_QIACT_EN);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //9�����PDP������
    WireLess_Send_AT_Command(AT_COMMAND_QIACT_DE); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QIACT_DE);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_QIACT_DE);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //10�����ɽ��ܵ���С��������
    WireLess_Send_AT_Command(AT_COMMAND_CGQMIN_DE); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_CGQMIN_DE);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_CGQMIN_DE);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    //11�����÷�����IP��ַ�Ͷ˿ںţ���������
    WireLess_Send_AT_Command(AT_COMMAND_QIOPEN); 
    index = 0;
    while(9)
    {
        temp_sta = WireLess_Rec_AT_Command_Monitor(AT_COMMAND_QIOPEN);
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //��������ģ��
                WireLess_Send_AT_Command(AT_COMMAND_QIOPEN);
            }
        }
        else    //����ɹ��ˣ��������һ��
        {
            break;
        }
    }
    
    return SUCCEED;
}






//////////////////////////////END OF C�ļ�////////////////////////////////////
