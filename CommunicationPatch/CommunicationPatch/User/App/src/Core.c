
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:��Ŀ���̵ĺ��Ĵ���Դ�ļ�
*******************************************************************************/


/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/
// �������ݻ��������������ݽ��������ݷ��͡����ݴ洢��
u8 g_PublicDataBuffer[PUBLIC_DATA_BUFFER_MAX_LENGTH] = {0};

// ϵͳ����ʹ�ܱ�־
u8 g_SysPulloutEnableFlag = TRUE;       //

//ϵͳ����POLL���ݰ����������ļ�ʱ
u32 g_SysPollTimeCnt = 0;     //Ĭ����������һ��POLL��

//ϵͳ��ѯ�豸��״̬�ļ�ʱ
u32 g_GetPulloutStatusTimeCnt = 0;

// ϵͳ��ʼ��״̬��־
u8 g_SysInitStatusFlag = FALSE;         //Ĭ��δ���

// �豸���ϱ�־
u8 g_SystemDeviceErrorFlag = FALSE;

// ���ʹ��������ݵļ�ʱ
u32 g_SendSensorDataTimeCnt = 0;

// ��ȡ���������ݵļ�ʱ
u32 g_GetDeviceSensorDataTimeCnt = 0;

// ������������ģ��ļ����ʱ
u32 g_ReSetWireLessModuleTimeCnt = 0;







/******************************************************************************
//��������
*******************************************************************************/
//����: �ַ���ת�������ĺ���
int SysStr2Int(char* str);

//����: ������ת�ַ����ĺ���
char* SysInt2Str(int val,char* dst,int radix);

//����: �ַ����͵����ڵ�������
char* SysCharDateAscend(char* c_date);

//����: ϵͳ�ַ����ȽϺ���
u8 SysCharArrayCmp(const char *data1, const char *data2);

//����: ϵͳu8������ȽϺ���
u8 SysU8ArrayCmp(u8 *data1, u8 *data2, u16 datal);

//��������: ����U16���ݵĸߵ��ֽڵĺ���
u16 U16_Change_Order(u16 data);

//��������: ϵͳ�����ʼ������
void SysPeripheralInit(void);

//��������: ϵͳ���е�ȫ�ֱ�����ʼ��
void SysGlobalVariableInit(void);

//��������: ϵͳ���ֹ��ܿ���
void System_Function_Control(void);

//��������: ϵͳ����״̬�ɼ�
void System_Status_Collection(void);
















/******************************************************************************
//��������
*******************************************************************************/
/*****************************************
//����: SysInt2Str
//����: ������ת�ַ����ĺ���
//���: int val����������, char* dst�����ַ���, int radix����ת������
//����: ��
//��ע: ��itoa()������ԭ��
******************************************/
char* SysInt2Str(int val, char* dst, int radix)
{
    char *_pdst = dst;
    char *_first = _pdst;
    char _cov;
    unsigned int _rem;

    if (!val)//����val����0
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
        val /= radix;//ÿ�μ���һλ ���ӵ͵���
        if  (_rem > 9)//16����
            *_pdst++ = (char)(_rem - 10 + 'a');
        else
            *_pdst++ = (char)(_rem + '0');
    }
    *_pdst-- = '\0';
    do{ //���������ǵ�λ����λ����ģ���Ҫת��λ��
        _cov = *_pdst;
        *_pdst = *_first;
        *_first = _cov;
        _pdst--;
        _first++;
    }while(_first < _pdst);

    return dst;
}

//********************************************************
//��������: SysPeripheralInit
//��������: ϵͳ�����ʼ������
//��    ��: ��
//��    ��: ��
//��    ע:
//********************************************************
void SysPeripheralInit(void)
{
	USART_ITConfig(SERVER_COMM_USART, USART_IT_RXNE, ENABLE);	//ʹ��USART�����ж�
	USART_ITConfig(DEVICE_COMM_USART, USART_IT_RXNE, ENABLE);	//ʹ��USART�����ж�
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);	//ʹ��USART�����ж�

    //�豸�˳�ʼ��
    Device_Initial();

    //����ģ��ĳ�ʼ��
    WireLess_Initial();

    Ext_Flash_Detect();     //���Ƭ��flash�Ƿ����

    g_SendSensorDataTimeCnt = g_ms_Timing_Count;
    g_ReSetWireLessModuleTimeCnt = g_ms_Timing_Count;
    g_SysPollTimeCnt = g_ms_Timing_Count - SERVER_COMM_HANDSHAKE_INTERVAL;
}

//********************************************************
//��������: SysGlobalVariableInit
//��������: ϵͳ���е�ȫ�ֱ�����ʼ��
//��    ��: ��
//��    ��: ��
//��    ע:
//********************************************************
void SysGlobalVariableInit(void)
{
    u8  i = 0;

    //���������ͨ����صı���
    s_ServerCommRx.Status = FALSE;
    s_ServerCommRx.Timeout_Count = 0;
    s_ServerCommRx.Index = 0;
    memset(s_ServerCommRx.Buffer, 0, sizeof(s_ServerCommRx.Buffer));
    s_ServerCommTx.Index = 0;
    s_ServerCommTx.RepeatNum = 0;
    s_ServerCommTx.WaitResponse = DONT_RESPONSE;
    s_ServerCommTx.WaitResponseTimeout = 0;

    // �������ͨ�ŵ����ݰ��ṹ��
    s_ServerCommPackage.Head        = SERVER_COMM_PACKAGE_HEAD;
    s_ServerCommPackage.Identify    = SERVER_COMM_PACKAGE_IDENTIFY;
    s_ServerCommPackage.Length      = 0;
    s_ServerCommPackage.ADF.SN      = 1;    //���кŴ�1��ʼ
    s_ServerCommPackage.ADF.CMD     = SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE;

    //ϵͳ����
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
    s_IPAddrPort.got_status   = FALSE;

    // �豸����ز���
    s_UploadInterval.time1      = SEND_SENSOR_DATA_TIME_INTERVAL;
    s_UploadInterval.heartbeat  = SEND_POLL_PACKAGE_TIME_INTERVAL;

    memset(&s_SensorData, 0, sizeof(s_SensorData));
    memset(s_SensorData.device_sta, 0xFF, sizeof(s_SensorData.device_sta));
    s_SensorData.got_status     = FALSE;
    s_SensorData.sensor_num     = 0;
    s_SensorData.PM2_5.status   = FALSE;
    s_SensorData.PM10.status    = FALSE;
    s_SensorData.CO.status      = FALSE;
    s_SensorData.NO2.status     = FALSE;
    s_SensorData.O3.status      = FALSE;
    s_SensorData.SO2.status     = FALSE;
    s_SensorData.NO.status      = FALSE;
    s_SensorData.TVOC.status    = FALSE;
    s_SensorData.TRH.status     = FALSE;
    s_SensorData.Fan.status     = FALSE;
    s_SensorData.ExtSensor.status= FALSE;
    s_DevicePower.vol[0]        = 0;
    s_DevicePower.vol[1]        = 0;

    s_GPSInfo.got_status = FALSE;
    memset(&s_GPSInfo, 0, sizeof(s_GPSInfo));
}

//********************************************************
//��������: System_Function_Control
//��������: ϵͳ���ֹ��ܿ���
//��    ��: ��
//��    ��: ��
//��    ע:
//********************************************************
void System_Function_Control(void)
{
    if(g_SysInitStatusFlag == TRUE) //��ʼ�����
    {
        //���������������ʱ����
        if(abs(g_ms_Timing_Count - g_SysPollTimeCnt) >= (s_UploadInterval.heartbeat * 60 * 1000))   //����Ҫת����ms
        {
            g_SysPollTimeCnt = g_ms_Timing_Count;

            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_HEARTBEAT);
        }
        else if(g_ExtFlashHaveData == TRUE) //�洢��������
        {
            if((s_ServerCommTx.WaitResponse == DONT_RESPONSE) || (s_ServerCommTx.WaitResponseTimeout >= SERVER_COMM_WAIT_RESPONSE_TIMEOUT))
            {

#if (SERVER_PRINTF_EN)
                printf("g_DataPageNum=%d,��ʣ%d��\r\n", g_DataPageNum, (g_DataPageNum - SENSOR_DATA_MIN_PAGE_NUM));
#endif

                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_FLASH);
                s_ServerCommTx.RepeatNum++;
                if(s_ServerCommTx.RepeatNum >= SERVER_COMM_REPEAT_SEND_TIME)        //�����ط������򲻷���
                {
                    s_ServerCommTx.RepeatNum = 0;
                    s_ServerCommTx.WaitResponse = 0;
                    g_ExtFlashHaveData = FALSE;
                    //�ж��������
                    g_SysInitStatusFlag = FALSE;
                    g_WireLessModuleInitFlag = FALSE;
                }
                s_ServerCommTx.WaitResponseTimeout = 0;
            }
        }
    }
    else
    {
        if(g_WireLessModuleInitFlag == TRUE)   //��������ģ��Ҳ��ʼ�������
        {
            //�����������
            if(abs(g_ms_Timing_Count - g_SysPollTimeCnt) >= SERVER_COMM_HANDSHAKE_INTERVAL)
            {
                g_SysPollTimeCnt = g_ms_Timing_Count;

                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE);
            }
        }
        else
        {
            //��������������ģ���ʱ����
            if(abs(g_ms_Timing_Count - g_ReSetWireLessModuleTimeCnt) >= RESET_WIRELESSMODULE_INTERVAL)
            {
                // ����ǲ�SIM���ˣ����Ҳ�����͸��������
                if((g_WireLessSIMCardInsertFlag == TRUE) && (g_DebugInterfaceTransmitFlag == FALSE))   //Debug͸��ģʽ�¼�ʹ���������쳣Ҳ����������
                {
                    //����ģ��ĳ�ʼ��
                    WireLess_Initial();
                }
                g_ReSetWireLessModuleTimeCnt = g_ms_Timing_Count;
            }
        }
    }
    //���˷��ʹ��������ݰ���ʱ����
    if(abs(g_ms_Timing_Count - g_SendSensorDataTimeCnt) >= (s_UploadInterval.time1 * 1000))//��Ҫת��ms
    {
        //ֻ�еȻ�����豸�˵Ĵ��������ݲ��ϱ�
        if(s_SensorData.got_status == TRUE)
        {
            g_SendSensorDataTimeCnt = g_ms_Timing_Count;
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_DATA);
        }
    }

    // �����ʱû��Ӧ��
    if((s_ServerCommTx.WaitResponse == NEED_RESPONSE) && (g_LastSendServerCmd == SERVER_COMM_PACKAGE_CMD_REPORT_DATA)
       && (s_ServerCommTx.WaitResponseTimeout >= SERVER_COMM_WAIT_RESPONSE_TIMEOUT))
    {

#if (SERVER_PRINTF_EN)
            printf("���������ݰ���ʱû��Ӧ��\r\n");
#endif

        Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_DATA);  //��ʱ��洢��flash��

        s_ServerCommTx.WaitResponseTimeout = 0;
        s_ServerCommTx.WaitResponse = DONT_RESPONSE;
    }

    // ���������͸��������
    if(g_DebugInterfaceTransmitFlag == FALSE)
    {
        if(g_DeviceInitFlag == TRUE)    //�豸��ʼ�����
        {
            // ����ȡ�豸�˴��������ݵ�ʱ����
            if(abs(g_ms_Timing_Count - g_GetDeviceSensorDataTimeCnt) >= DEVICE_COMM_GET_SENSOR_DATA_INTERVAL)
            {
                g_GetDeviceSensorDataTimeCnt = g_ms_Timing_Count;
                // ������ʾ���������ݵĴ�ӡ
                Device_Printf_Ctr(DEVICE_CTR_SENSOR_PRINTF_CMD);
            }
        }
        else if(abs(g_ms_Timing_Count - g_ReSetDeviceTimeCnt) >= RESET_DEVICE_INTERVAL)
        {
            //�豸�˳�ʼ��
            Device_Initial();
            g_ReSetDeviceTimeCnt = g_ms_Timing_Count;
        }
    }
}








//////////////////////////////END OF C�ļ�////////////////////////////////////

