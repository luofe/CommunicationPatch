
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

//ϵͳ�¶ȿ��Ƶļ�ʱ
u16 g_SysTempCtrTimeCnt = 0;

//ϵͳ����ʱ���жϵļ�ʱ
u16 g_SysWorkTimeJudgeCnt = 0;

//ϵͳ����ʱ��α�־
u8 g_SysEnergySaveTimeFlag = OTHER_WORK_TIME;  //Ĭ���Ƿǽ���ʱ���

//ϵͳ����ʱ��α�־
u8 g_SysSaleTimeFlag = SALE_WORK_TIME;  //Ĭ��������ʱ���

// ϵͳ����ʱ������ñ�־
u8 g_SysEnergySaveTimeSetFlag = ACCORD_TO_NO_TEMP;    //Ĭ����û���·��¶ȣ�������ѹ������������ֻ������

// ϵͳ����ʱ������ñ�־
u8 g_SysSaleTimeSetFlag = FALSE;      //Ĭ����û������

// ϵͳ����״̬�仯��־
u8 g_SysRunStatusChangeFlag = SYSTEM_RUN_STATUS_STAY;    //Ĭ��û�б仯

// �豸�˸�λ��־
u8 g_PulloutBoardReseted = FALSE;    //Ĭ���豸��δ��λ���

// ϵͳУʱ��־
u8 g_SysCorrectDateTimeFlag = FALSE;    //Ĭ��û��Уʱ

// ϵͳ��ʼ��״̬��־
u8 g_SysInitStatusFlag = FALSE;         //Ĭ��δ���

// ϵͳ����/�˳�����״̬ʱ��������״ֵ̬�ı���
u8 g_SysAccessExitErrorVMCStatus;

//// ������򿪺�رյļ�ʱ
//u16 g_SysEMLockOpenCnt = 0;

// �豸���ϱ�־
u8 g_SystemDeviceErrorFlag = FALSE;

// ���ʹ��������ݵļ�ʱ
u32 g_SendSensorDataTimeCnt = 0;

// ��ȡ���������ݵļ�ʱ
u32 g_GetDeviceSensorDataTimeCnt = 0;







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
//����: SysStr2Int
//����: �ַ���ת�������ĺ���
//���: char* dst�����ַ���
//����: int����ת�����������
//��ע: ��atoi()������ԭ��
******************************************/
int SysStr2Int(char* str)
{
    int data = 0;  
    const char *ptr = str;
    
    if(*str == '-'){  
        str ++;  
    }  
    while(*str != '\0'){  
        if(*str < '0' || *str > '9'){  
            break;  
        }  
        data = data * 10 + (*str - '0');  
        str ++;  
    }  
    if(*ptr == '-'){  
        data =-data;  
    }  
    
    return data; 
}   

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

/*****************************************
//����: SysCharDateAscend
//����: �ַ����͵����ڵ�������
//���: const TCHAR* c_date�����ַ����͵�����
//����: char*�������ص�������ַ���
//��ע: �������ԡ������ա��ĸ�ʽ���ַ���
******************************************/
char* SysCharDateAscend(char* c_date)
{
    char *pr = c_date;
    
    char c_year[5] = "";
    char c_month[3]= "";
    char c_day[3]  = "";
    
    u16   year;
    u8    month;
    u8    day;
    u8    leap_year_day;
    
    memcpy(c_year, &pr[0], 4);
    c_year[4] = '\0';
    memcpy(c_month, &pr[4], 2);
    c_month[2] = '\0';
    memcpy(c_day, &pr[6], 2);
    c_day[2] = '\0';
    
    year    = atoi(c_year);
    month   = atoi(c_month);
    day     = atoi(c_day);
    
    switch(month)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
        {
            //�������31��
            if(day == 31)
            {
                day = 1;
                if(month == 12)
                {
                    month = 1;
                    year++;
                }
                else
                {
                    month++;
                }
            }
            else
            {
                day++;
            }
        }
        break;
        
        case 2:
        {
            //�ж��Ƿ�Ϊ����
            if((year%4 == 0) && (year%100 != 0) || (year%400 == 0))
            {
                leap_year_day = 29;
            }
            else
            {
                leap_year_day = 28;
            }
            //�������29��
            if(day == leap_year_day)
            {
                day = 1;
                month++;
            }
            else
            {
                day++;
            }
        }
        break;
        
        case 4:
        case 6:
        case 9:
        case 11:
        {
            //�������30��
            if(day == 30)
            {
                day = 1;
                month++;
            }
            else
            {
                day++;
            }
        }
        break;
    }
    
    SysInt2Str(year, c_year, 10);
    SysInt2Str(month, c_month, 10);
    if(month < 10)  //�����һλ��
    {
        c_month[1] = c_month[0];
        c_month[0] = '0';
    }
    SysInt2Str(day, c_day, 10);
    if(day < 10)  //�����һλ��
    {
        c_day[1] = c_day[0];
        c_day[0] = '0';
    }
    
    memcpy(&pr[0], c_year, 4);
    memcpy(&pr[4], c_month, 2);
    memcpy(&pr[6], c_day, 2);
    
    return c_date;
}

/*****************************************
//����: SysCharArrayCmp
//����: ϵͳ�ַ����ȽϺ���
//���: const char *data1�����ַ���1, const char *data2�����ַ���2
//����: u8�������صıȽ�״̬��1����ȣ�0�������
******************************************/
u8 SysCharArrayCmp(const char *data1, const char *data2)
{
    while((*data1 != '\0') && (*data2 != '\0'))
    {
        if(*data1++ != *data2++)
        {
            return FALSE;
        }
    }
    
    return TRUE;
}

/*****************************************
//����: SysU8ArrayCmp
//����: ϵͳu8������ȽϺ���
//���: u8 *data1��������1, u8 *data2��������2, u16 datal�����Ƚϵĳ���
//����: u8�������صıȽ�״̬��0����ȣ�1������1������2��2������1������2С
******************************************/
u8 SysU8ArrayCmp(u8 *data1, u8 *data2, u16 datal)
{
    u16 i = 0;
    
    for(; i < datal; i++)
    {
        if(data1[i] > data2[i]) //�����������1��������2�����
        {
            return 1;
        }
        else if(data1[i] < data2[i])//�����������1С������2�����
        {
            return 2;
        }
    }
    
    if(i == datal)  //������Ƚ���ϣ�˵�������
    {
        return 0;
    }
    
    return 0;
}

//********************************************************
//��������: U16_Change_Order
//��������: ����U16���ݵĸߵ��ֽڵĺ���
//��    ��: u16 data������Ҫ����������
//��    ��: u16�������ؽ������ֵ
//��    ע: 
//********************************************************
u16 U16_Change_Order(u16 data)	
{	
	return ((data << 8) + (data >> 8));	
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
    //����ģ��ĳ�ʼ��
    WireLess_Initial();
    
	USART_ITConfig(DEVICE_COMM_USART, USART_IT_RXNE, ENABLE);	//ʹ��USART�����ж�
    //�豸�˳�ʼ��
    Device_Initial();

    Ext_Flash_Detect();     //���Ƭ��flash�Ƿ����
    
	USART_ITConfig(DEBUG_USART, USART_IT_RXNE, ENABLE);	//ʹ��USART�����ж�
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
    //��ȡƬ��Flash������
    Internal_Flash_ReadOut();
    
    //���������ͨ����صı���
    s_ServerCommRx.Status = FALSE;
    s_ServerCommRx.Timeout_Count = 0;
    s_ServerCommRx.Index = 0;
    memset(s_ServerCommRx.Buffer, 0, sizeof(s_ServerCommRx.Buffer));
    
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
    
    
    // �豸����ز���
    s_UploadInterval.time1      = SEND_SENSOR_DATA_TIME_INTERVAL;
    s_UploadInterval.heartbeat  = SEND_POLL_PACKAGE_TIME_INTERVAL;
    
    memset(&s_SensorData, 0, sizeof(s_SensorData));
    memset(s_SensorData.device_sta, 0xFF, sizeof(s_SensorData.device_sta));
    s_SensorData.got_status     = FALSE;
    s_DevicePower.vol[0]        = 0;
    s_DevicePower.vol[1]        = 0;
    s_SensorData.sensor_num     = 0;
    
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
        if(abs(g_ms_Timing_Count - g_SysPollTimeCnt) >= (s_UploadInterval.heartbeat * 60 * 1000))
        {
            g_SysPollTimeCnt = g_ms_Timing_Count;
            
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_HEARTBEAT);
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
            //����ģ��ĳ�ʼ��
            WireLess_Initial();
        }
    }
    //��������ʹ��������ݵ�ʱ����
    if(abs(g_ms_Timing_Count - g_SendSensorDataTimeCnt) >= (s_UploadInterval.time1 * 1000))//30
    {
        //ֻ�еȻ�����豸�˵Ĵ��������ݲ��ϱ�
        if(s_SensorData.got_status == TRUE)
        {
            g_SendSensorDataTimeCnt = g_ms_Timing_Count;
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_DATA);
        }
        else    //������ϱ�����������ֹ�������
        {
            g_SendSensorDataTimeCnt = g_ms_Timing_Count - (s_UploadInterval.heartbeat * 60 * 1000);
        }
    }
    
    // ����ȡ�豸�˴��������ݵ�ʱ����
    if(abs(g_ms_Timing_Count - g_GetDeviceSensorDataTimeCnt) >= DEVICE_COMM_GET_SENSOR_DATA_INTERVAL)
    {
        g_GetDeviceSensorDataTimeCnt = g_ms_Timing_Count;
        // ������ʾ���������ݵĴ�ӡ
        Device_Printf_Ctr(DEVICE_CTR_SENSOR_PRINTF_CMD);
    }
    
    // RTC���ʱ
    if(abs(g_ms_Timing_Count - s_Timing_Count) >= 1000)
    {
        s_Timing_Count = g_ms_Timing_Count;
        s_GPSInfo.gmtTime++;
    }
}








//////////////////////////////END OF C�ļ�////////////////////////////////////

