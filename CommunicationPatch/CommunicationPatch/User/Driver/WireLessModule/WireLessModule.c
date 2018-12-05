
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
// ����ģ���ʼ����ɱ�־
u8  g_WireLessModuleInitFlag = FALSE;









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

//����: ����ģ��ATָ����ƺ���
u8 WireLess_AT_Command_Ctr(u8 cmd);

//����: ����ģ���ʼ������
u8 WireLess_Initial(void);















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

#if (SERVER_AT_PRINTF_EN)
    printf("\r\n����ATָ�������ģ��:");
#endif

    while(*str != '\0')  //����AT����
    {

#if (SERVER_AT_PRINTF_EN)
        printf("%c", *str);
#endif

        USART_SendData(SERVER_COMM_USART, *str++);
        while(USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_TXE) == RESET);//�ȴ��������
    }

#if (SERVER_AT_PRINTF_EN)
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
                //������й��ƶ���
                case 0x00:
                case 0x02:
                case 0x04:
                case 0x07:
                {
                    sprintf(&at_array[index], "\"%s\",", WIRELESS_APN_CMNET);
                }
                break;
                //������й����ŵ�
                case 0x03:
                case 0x05:
                case 0x11:
                {
                    sprintf(&at_array[index], "\"%s\",", WIRELESS_APN_CTNET);
                }
                break;
                //������й���ͨ��
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

    switch(cmd)
    {
        case AT_COMMAND_QPWOD:  //�������������
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

        case AT_COMMAND_CPIN:   //���SIM��״̬
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CREG:   //��⿨������ע��״̬
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == '0') && (buf[i + 2] == '1') && (buf[i + 7] == 'O') && (buf[i + 8] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CGREG:  //��⿨��GPRS����ע��״̬
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == '0') && (buf[i + 2] == '1') && (buf[i + 7] == 'O') && (buf[i + 8] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QICSGP:  //����APN��������ID
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CGQMIN:  //���ÿɽ��ܵ���С��������
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QIDEACT:  //ʧ��PDP������
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QIACT_EN:  //����PDP������
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QIACT_DE:  //���PDP������
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CGQMIN_DE:  //���ɽ��ܵ���С��������
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QIOPEN:  //���÷�����IP��ַ�Ͷ˿ںš���������
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
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QICLOSE:  //�ر�TCP����
        {
            for(i = 0; i < len; i++)
            {
                if((buf[i] == 'O') && (buf[i + 1] == 'K'))
                {
                    return SUCCEED;
                }
            }
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_SWITCH_CMD:  //�л�������ģʽ
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

        case AT_COMMAND_ATO:  //�л���͸��ģʽ
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
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_QCCID:  //��ȡSIM����CCID��
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
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CIMI:  //��ȡSIM����IMSI��
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
            if(repeat_sta == TRUE)  //�ظ�AT
            {
                return RECEIVE_AT_COMMAND_REPEAT;
            }
        }
        break;

        case AT_COMMAND_CGSN:  //��ȡSIM����IMEI��
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
            if(repeat_sta == TRUE)  //�ظ�AT
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
    u8  temp_sta = FAILURE; //��������������ɹ���ʧ�ܡ�ֻ�ǽ��յ�����ģ�������ָ����ظ�������û�н���ģ�
    u8  repeat_sta = TRUE;  //����ģ�������ָ����ظ�����־

    s_ServerCommRx.Status = FALSE;
    s_ServerCommRx.Index = 0;
    s_ServerCommRx.Timeout_Count = 0;
    while(9)
    {
        if(s_ServerCommRx.Status == TRUE)   //�н���
        {
            if(s_ServerCommRx.Timeout_Count >= WIRELESS_RX_AT_DATA_TIMEOUT)    //�Ƿ�ʱ��
            {
                if((s_ServerCommRx.Index < 3))   //�������Чֵ������ && (s_ServerCommRx.Buffer[0] == '\0')
                {
                    s_ServerCommRx.Status = FALSE;
                    s_ServerCommRx.Index = 0;
                    s_ServerCommRx.Timeout_Count = 0;

                    continue;
                }
                temp_l = s_ServerCommRx.Index;    //���������ݳ���
                //�����ݿ�������������������ֹ���µ�������û
                memcpy(g_PublicDataBuffer, s_ServerCommRx.Buffer, temp_l);

                s_ServerCommRx.Status = FALSE;
                s_ServerCommRx.Index = 0;
                s_ServerCommRx.Timeout_Count = 0;

#if (SERVER_AT_PRINTF_EN)
                printf("���յ�����ģ��Ӧ��AT��");
                for(u16 i = 0; i < temp_l; i++)
                {
                    printf("%c", g_PublicDataBuffer[i]);
                }
#endif

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

                case AT_COMMAND_QICLOSE:  //�ر�TCP����
                {
                    temp_time_count = WIRELESS_WAIT_QICLOSE_TIMEOUT;
                }
                break;

                case AT_COMMAND_SWITCH_CMD:  //�л�������ģʽ��+++��
                {
                    temp_time_count = WIRELESS_WAIT_SWITCH_CMD_TIMEOUT;
                }
                break;

                case AT_COMMAND_ATO:  //�л���͸��ģʽ
                {
                    temp_time_count = WIRELESS_WAIT_ATO_TIMEOUT;
                }
                break;

                case AT_COMMAND_QCCID:  //��ȡSIM����CCID��
                {
                    temp_time_count = WIRELESS_WAIT_QCCID_TIMEOUT;
                }
                break;

                case AT_COMMAND_CIMI:  //��ȡSIM����ISMI��
                {
                    temp_time_count = WIRELESS_WAIT_CIMI_TIMEOUT;
                }
                break;

                case AT_COMMAND_CGSN:  //��ȡSIM����IMEI��
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
//����: WireLess_AT_Command_Ctr
//����: ����ģ��ATָ����ƺ���
//���: u8 cmd��������
//����: u8�����ؿ��ƽ��
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
        if(temp_sta == FAILURE) //���ʧ����
        {
            index++;
            if(index > 1)   //��ʱ1�Σ����˳�
            {
                return FAILURE;
            }
            else
            {
                //�ط�һ��
                WireLess_Send_AT_Command(cmd);
            }
        }
        else    //����ɹ��ˣ����˳�
        {
            break;
        }
    }

    return SUCCEED;
}

/*****************************************
//����: WireLess_Initial
//����: ����ģ���ʼ������
//���: ��
//����: u8�����س�ʼ��״̬
******************************************/
u8 WireLess_Initial(void)
{
    u8  index = 0;
//    u8  temp_sta = FALSE;

#if (SERVER_AT_PRINTF_EN)
    printf("�ȴ�����ģ���˳�͸��ģʽ\r\n");
#endif

    //�Ƚ�������ģʽ
//    Delay_ms(1500);
    if(WireLess_AT_Command_Ctr(AT_COMMAND_SWITCH_CMD) == FAILURE)
    {

#if (SERVER_AT_PRINTF_EN)
        printf("�ȴ�����ģ���������\"RDY\"\r\n");
#endif

//        //�ȵȴ���������ģ��������ġ�RDY��
//        if(WireLess_AT_Command_Ctr(AT_COMMAND_QPWOD) == FAILURE)
//        {
//            return FAILURE;
//        }
    }

#if (SERVER_AT_PRINTF_EN)
    printf("���SIM��״̬\r\n");
#endif

    //���SIM��״̬
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CPIN) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("��ȡSIM����CCID��\r\n");
#endif

    //��ȡSIM����CCID��
    if(WireLess_AT_Command_Ctr(AT_COMMAND_QCCID) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("��ȡSIM����IMSI��\r\n");
#endif

    //��ȡSIM����IMSI��
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CIMI) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("��ȡSIM����IMEI��\r\n");
#endif

    //��ȡSIM����IMEI��
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CGSN) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("��⿨������ע��״̬\r\n");
#endif

    //��⿨������ע��״̬
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CREG) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("��⿨��GPRS����ע��״̬\r\n");
#endif

    //��⿨��GPRS����ע��״̬
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CGREG) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("����APN��������ID\r\n");
#endif

    //����APN��������ID
    if(WireLess_AT_Command_Ctr(AT_COMMAND_QICSGP) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("���ÿɽ��ܵ���С��������\r\n");
#endif

    //���ÿɽ��ܵ���С��������
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CGQMIN) == FAILURE)
    {
        return FAILURE;
    }

    ForceReconnect:
//    if(temp_sta == TRUE)
    {

#if (SERVER_AT_PRINTF_EN)
        printf("ʧ��PDP������\r\n");
#endif

        //ʧ��PDP������
        if(WireLess_AT_Command_Ctr(AT_COMMAND_QIDEACT) == FAILURE)
        {
            return FAILURE;
        }
    }

#if (SERVER_AT_PRINTF_EN)
    printf("����PDP������\r\n");
#endif

    //����PDP������
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
    printf("���PDP������\r\n");
#endif

    //���PDP������
    if(WireLess_AT_Command_Ctr(AT_COMMAND_QIACT_DE) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("���ɽ��ܵ���С��������\r\n");
#endif

    //���ɽ��ܵ���С��������
    if(WireLess_AT_Command_Ctr(AT_COMMAND_CGQMIN_DE) == FAILURE)
    {
        return FAILURE;
    }

#if (SERVER_AT_PRINTF_EN)
    printf("���÷�����IP��ַ�Ͷ˿ںţ���������\r\n");
#endif

    //���÷�����IP��ַ�Ͷ˿ںţ���������
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

    g_WireLessModuleInitFlag = TRUE;    //����ģ���ʼ�����

#if (SERVER_AT_PRINTF_EN)
    printf("����ģ���ʼ���ɹ���\r\n");
#endif

    return SUCCEED;
}






//////////////////////////////END OF C�ļ�////////////////////////////////////
