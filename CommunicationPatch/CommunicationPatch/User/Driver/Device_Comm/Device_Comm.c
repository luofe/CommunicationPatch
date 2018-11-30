
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:��Ŀ�������豸��ͨ����صĳ���
//��ע:
1��������豸����ͨ�Ű��ͨ��Э����ο������Ķ�Ӧ�ĵ���
2���豸�ϵ�Ĭ��ֻ��ӡ����������ʼ��״̬������ӡ������Ϣ��


*******************************************************************************/

/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/
//�������豸��ͨ�ŵĽ������ݽṹ��
DeviceCommRxStruct s_DeviceCommRx;

//���豸��ͨ�ŵķ������ݵĽṹ�嶨��
DeviceCommTxStruct s_DeviceCommTx;

// ������У׼�Ĳ����ṹ��
SensorAdjStruct s_SensorAdj;

// RTC�ṹ��
RTCStruct s_RTC;

// IP��ַ�Ͷ˿ںŽṹ��
IPAddrPortStruct s_IPAddrPort;

// �����ϱ�ʱ�����ṹ��
UploadIntervalStruct s_UploadInterval;

// ͨ�ô��������ݽṹ��
SensorDataStruct s_SensorData;

// GPS��λ��Ϣ
GPSInfoStruct s_GPSInfo;

// �豸��ʼ����ϱ�־
u8  g_DeviceStartFlag = FALSE;

// ��Դ��ѹ��Ϣ
DevicePowerStruct s_DevicePower;

// �豸��ʼ��״̬
u8  g_DeviceInitFlag = FALSE;










/******************************************************************************
//��������
*******************************************************************************/
//��������: ���豸��ͨ����صĳ�ʼ������
void Device_Comm_Init(void);

//��������: ���豸��ͨ���õ�USART��ʼ������
void Device_Comm_USART_Init(void);

//��������: ���豸��ͨ���õ�GPIO�ڳ�ʼ������
void Device_Comm_GPIO_Init(void);

//����: ���豸�˰�ͨ�ŵķ�������
void Device_Comm_Send_Data(u8 *buffer, u16 data_l);

//��������: ���豸��ͨ�ŵ����ݰ��������
void Device_Comm_Package_Bale(u8 cmd);

//��������: ���豸��ͨ�ŵ����ݰ�������
u8 Device_Comm_Package_Process(u8 cmd, u8* resp_str, u16 len);

//��������: ��ͨ�Ű�ͨ�����ݰ���������
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l);

//��������: �����豸�����������ݽ�������
u8 Device_Rec_Command_Analysis(u8 cmd, u8* buf, u16 len);

//����: �豸����Ӧ������Ĺ�����
u8 Device_Rec_Response_Cmd_Monitor(u8 cmd);

//����: �豸��ӡ������ƺ���
u8 Device_Printf_Ctr(u8 cmd);

//����: �豸�˳�ʼ������
u8 Device_Initial(void);

//��������: ���豸��ͨ�ŵļ�⺯��
void Device_Comm_Rec_Monitor(void);


























/******************************************************************************
//��������
*******************************************************************************/
/********************************************************
//��������: Device_Comm_Init
//��������: ���豸��ͨ����صĳ�ʼ������
//��    ��: ��
//��    ��: ��
//��    ע: ��
********************************************************/
void Device_Comm_Init(void)
{
    Device_Comm_USART_Init();
    
    //    Device_Comm_GPIO_Init();
}

/********************************************************
//��������: Device_Comm_USART_Init
//��������: ���豸��ͨ���õ�USART��ʼ������
//��    ��: ��
//��    ��: ��
//��    ע: ��
********************************************************/
void Device_Comm_USART_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	//��ʼ��NVIC�ṹ��
    
    //��λDEVICE_COMM_USARTģ��
    USART_DeInit(DEVICE_COMM_USART);
    
    //ʹ��DEVICE_COMM_USART_TX����ģ��ʱ��
    RCC_APB2PeriphClockCmd(DEVICE_COMM_USART_TX_GPIO_CLK, ENABLE);
    
    //ʹ��DEVICE_COMM_USART_RX����ģ��ʱ��
    RCC_APB2PeriphClockCmd(DEVICE_COMM_USART_RX_GPIO_CLK, ENABLE);
    
    //ʹ��USARTģ��ʱ��
    if(DEVICE_COMM_USART == USART1)
    {
        RCC_APB2PeriphClockCmd(DEVICE_COMM_USART_CLK, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(DEVICE_COMM_USART_CLK, ENABLE);
    }
    
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    //DEVICE_COMM_USART��GPIO����
    //DEVICE_COMM_USART_TX: ���츴�����
    GPIO_InitStructure.GPIO_Pin   = DEVICE_COMM_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //���츴�����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//ѡ��50MHz
    GPIO_Init(DEVICE_COMM_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    //DEVICE_COMM_USART_RX: ��������(�����������)
    GPIO_InitStructure.GPIO_Pin   = DEVICE_COMM_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(DEVICE_COMM_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    
    //DEVICE_COMM_USARTģ���������
    //������: USART1_BAUDRATE��8������λ��1��ֹͣλ����У��λ����Ӳ�������ƣ�ʹ�ܷ��ͺͽ��գ�
    USART_InitStructure.USART_BaudRate            = DEVICE_COMM_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(DEVICE_COMM_USART, &USART_InitStructure); 
    
    USART_Cmd(DEVICE_COMM_USART, ENABLE);                         //ʹ��DEVICE_COMM_USARTģ��
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//ʹ�����ȼ�����2	
    NVIC_InitStructure.NVIC_IRQChannel                      = DEVICE_COMM_USART_IRQn; //ʹ��USART�ж�	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;                //�������ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 1;                //��Ӧ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	//����USART��Ƕ�������ж�
    
	USART_ITConfig(DEVICE_COMM_USART, USART_IT_RXNE, DISABLE);	//ʹ��USART�����ж�    
}

/*****************************************
//����: Device_Comm_Send_Data
//����: ���豸�˰�ͨ�ŵķ�������
//���: u8 *buffer������Ҫ���͵�����;u16 data_l�������ݳ���
//����: ��
******************************************/
void Device_Comm_Send_Data(u8 *buffer, u16 data_l)
{ 
#if (DEVICE_PRINTF_EN)
    printf("�������ݸ��豸��: ");
#endif	
    
    while(data_l--)  //����AT����
    {
        
#if (DEVICE_PRINTF_EN)
        printf("%c", *buffer);
#endif	
        
        USART_SendData(DEVICE_COMM_USART, *buffer++); 
        while(USART_GetFlagStatus(DEVICE_COMM_USART, USART_FLAG_TXE) == RESET);//�ȴ��������
    } 
    
//#if (DEVICE_PRINTF_EN)
//    printf("\r\n");
//#endif	
    
}

//********************************************************
//��������: Device_Comm_Package_Bale
//��������: ���豸��ͨ�ŵ����ݰ��������
//��    ��: u8 cmd����������
//��    ��: ��
//��    ע: ��
//********************************************************
void Device_Comm_Package_Bale(u8 cmd)
{
    u8 index;
    char data_str[255];
    char temp_str[100];
    
    memset(data_str, '\0', sizeof(data_str));
    memset(temp_str, '\0', sizeof(temp_str));
    
    data_str[0] = '$';
    switch(cmd)
    {
        case DEVICE_SET_SENSOR_LAB_ADJUST_CMD:   //�����У׼������ʵ���Ҳ���
        {
            index = 1;
            strcat(data_str, "S_SSL:");
            index += strlen("S_SSL:");
            //����type
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����multiple
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.multiple, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����K1
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.K1, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����B1
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.B1, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����K2
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.K2, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����B2
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.B2, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        case DEVICE_SET_SENSOR_APP_ADJUST_CMD:   //�����У׼������Ӧ�ò���
        {
            index = 1;
            strcat(data_str, "S_SSA:");
            index += strlen("S_SSA:");
            //����type
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����multiple
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.multiple, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����K1
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.K1, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����B1
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.B1, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����K2
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.K2, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����B2
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.B2, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:   //����Ƕ�ȡ������У׼����
        {
            index = 1;
            strcat(data_str, "Q_SSL:");
            index += strlen("Q_SSL:");
            //����type
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_DEVICE_ID:   //����Ƕ�ȡ�豸ID
        {
            index = 1;
            strcat(data_str, "Q_ID");
            index += strlen("Q_ID");
        }
        break;
        
        case DEVICE_SET_RTC_CMD:   //�������RTC 
        {
            index = 1;
            strcat(data_str, "S_TIME:");
            index += strlen("S_TIME:");
            //����year
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.year, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����month
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.month, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����day
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.day, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����hour
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.hour, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����min
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.min, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����sec
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.sec, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_RTC_CMD:   //����Ƕ�ȡRTC
        {
            index = 1;
            strcat(data_str, "Q_TIME");
            index += strlen("Q_TIME");
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:   //�������IP��ַ
        {
            index = 1;
            strcat(data_str, "S_IP:");
            index += strlen("S_IP:");
            strcat(data_str, (char const*)s_IPAddrPort.ip_port);
            index += strlen(data_str);
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:   //����Ƕ�ȡIP��ַ�Ͷ˿ں�
        {
            index = 1;
            strcat(data_str, "Q_IP");
            index += strlen("Q_IP");
        }
        break;
        
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:   //������������ϴ�ʱ����
        {
            index = 1;
            strcat(data_str, "S_DATARPT:");
            index += strlen("S_DATARPT:");
            //����time1
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_UploadInterval.time1, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����time2
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_UploadInterval.time2, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:   //�����ȡ�����ϴ�ʱ����
        {
            index = 1;
            strcat(data_str, "Q_DATARPT");
            index += strlen("Q_DATARPT");
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:   //�������������ʱ����
        {
            index = 1;
            strcat(data_str, "S_HEALTHRPT:");
            index += strlen("S_HEALTHRPT:");
            //����heartbeat
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_UploadInterval.heartbeat, temp_str, 10);
            strcat(data_str, temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:   //�����ȡ������ʱ����
        {
            index = 1;
            strcat(data_str, "Q_HEALTHRPT");
            index += strlen("Q_HEALTHRPT");
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:   //�������������λ
        {
            index = 1;
            strcat(data_str, "C_RESET");
            index += strlen("C_RESET");
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:   //������ƴ�������Ϣ��ӡ
        {
            index = 1;
            strcat(data_str, "SHOW_ALL");
            index += strlen("SHOW_ALL");
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:   //�����������������Ϣ
        {
            index = 1;
            strcat(data_str, "HIDE_ALL");
            index += strlen("HIDE_ALL");
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:   //������ƴ�������Ϣ��ӡ
        {
            index = 1;
            strcat(data_str, "SHOW_DATA");
            index += strlen("SHOW_DATA");
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:   //����������ش�������Ϣ
        {
            index = 1;
            strcat(data_str, "HIDE_DATA");
            index += strlen("HIDE_DATA");
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:   //�������GPS��Ϣ��ӡ
        {
            index = 1;
            strcat(data_str, "SHOW_GPS");
            index += strlen("SHOW_GPS");
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:   //�����������GPS��Ϣ
        {
            index = 1;
            strcat(data_str, "HIDE_GPS");
            index += strlen("HIDE_GPS");
        }
        break;
        
        case DEVICE_CTR_BATTERY_PRINTF_CMD:   //������ƴ�ӡ��Դ��ѹ��Ϣ
        {
            index = 1;
            strcat(data_str, "SHOW_ADC");
            index += strlen("SHOW_ADC");
        }
        break;
        
        case DEVICE_CTR_BATTERY_HIDE_CMD:   //����������ص�Դ��ѹ��Ϣ
        {
            index = 1;
            strcat(data_str, "HIDE_ADC");
            index += strlen("HIDE_ADC");
        }
        break;
        
        default :
        
        break;
    }
    //����س�����
    strcat(data_str, "\r\n");
    index += 2;
    //����
    Device_Comm_Send_Data((u8*)data_str, index);
}

//********************************************************
//��������: Device_Comm_Package_Process
//��������: ���豸��ͨ�ŵ����ݰ�������
//��    ��: u8 cmd���������룬u8* resp_str����Ӧ���ַ���, u16 len�������ݳ���
//��    ��: u8�������ؽ���������
//��    ע: ��
//********************************************************
u8 Device_Comm_Package_Process(u8 cmd, u8* resp_str, u16 len)
{
    u16 i;
    u16 index = 0;
    char temp_str[100];
    
    memset(temp_str, '\0', sizeof(temp_str));

    switch(cmd)
    {
        case DEVICE_SET_SENSOR_LAB_ADJUST_CMD:   //�����У׼������ʵ���Ҳ���
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //���û���óɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_SET_SENSOR_APP_ADJUST_CMD:   //�����У׼������Ӧ�ò���
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //���û���óɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:   //����Ƕ�ȡ������У׼����
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
            {
                //��ȡtype
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.type = atoi(temp_str);
                index += 1;
                //��ȡmultiple
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.multiple = atoi(temp_str);
                index += 1;
                //��ȡK1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.K1 = atoi(temp_str);
                index += 1;
                //��ȡB1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.B1 = atoi(temp_str);
                index += 1;
                //��ȡK2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.K2 = atoi(temp_str);
                index += 1;
                //��ȡB2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '(') && (resp_str[index + 1] != 'O'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.B2 = atoi(temp_str);
            }
            else    //���ʧ��
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_DEVICE_ID:   //����Ƕ�ȡ�豸ID
        {  
            u8 j;
            for(i = 0, j = 0; i < len; (i += 2), j++)
            {
                s_SystemPara.deviceID[j] = (resp_str[i] - 0x30) * 16;
                s_SystemPara.deviceID[j] += (resp_str[i + 1] - 0x30);
            } 
        }
        break;
        
        case DEVICE_SET_RTC_CMD:   //�������RTC 
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_RTC_CMD:   //����Ƕ�ȡRTC
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
            {
                //��ȡyear
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //�ӽ�����
                s_RTC.year = atoi(temp_str);
                index += 1;
                //��ȡmonth
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //�ӽ�����
                s_RTC.month = atoi(temp_str);
                index += 1;
                //��ȡday
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //�ӽ�����
                s_RTC.day = atoi(temp_str);
                index += 1;
                //��ȡhour
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //�ӽ�����
                s_RTC.hour = atoi(temp_str);
                index += 1;
                //��ȡmin
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != '-');
                temp_str[i++] = '\0';   //�ӽ�����
                s_RTC.min = atoi(temp_str);
                index += 1;
                //��ȡsec
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_RTC.sec = atoi(temp_str);
            }
            else
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:   //�������IP��ַ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:   //����Ƕ�ȡIP��ַ�Ͷ˿ں�
        {
            u8  temp_addr;
            //ܳ����ģ���������Ӧ��û�С�\r\n�����ɣ�����
            //��ȡIP
            i = 0;
            temp_str[i++] = '"';   //��"
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }while(resp_str[index] != ',');
            temp_str[i++] = '"';   //��"
            temp_str[i] = '\0';   //�ӽ�����
            memcpy(s_IPAddrPort.ip_port, temp_str, i);
            s_IPAddrPort.ip_port[i++] = ',';
            temp_addr = i;
            index += 1;
            //��ȡ�˿�
            memcpy(&s_IPAddrPort.ip_port[temp_addr], &resp_str[index], (len - index));
            
            s_IPAddrPort.got_status = TRUE;
        }
        break;
        
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:   //������������ϴ�ʱ����
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //�����ȡ���ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:   //�����ȡ�����ϴ�ʱ����
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
            {
                //��ȡtime1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_UploadInterval.time1 = atoi(temp_str);
                index += 1;
                //��ȡtime2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '(') && (resp_str[index + 1] != 'O'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_UploadInterval.time2 = atoi(temp_str);
            }
            else    //���ʧ�ܣ����ٶ�ȡһ��
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:   //�������������ʱ����
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:   //�����ȡ������ʱ����
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
            {
                //��ȡheartbeat
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '(') && (resp_str[index + 1] != 'O'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_UploadInterval.heartbeat = atoi(temp_str);
            }
            else    //���ʧ��
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:   //�������������λ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:   //������ƴ�������Ϣ��ӡ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:   //�����������������Ϣ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:   //������ƴ�������Ϣ��ӡ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:   //����������ش�������Ϣ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:   //�������GPS��Ϣ��ӡ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:   //�����������GPS��Ϣ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_BATTERY_PRINTF_CMD:   //������Ƶ�Դ��ѹ��Ϣ��ӡ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_CTR_BATTERY_HIDE_CMD:   //������ص�Դ��ѹ��Ϣ��ӡ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (4 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                return FAILURE;
            }
        }
        break;
        
        case DEVICE_GET_SENSOR_DATA_CMD:   //����Ǵ����������ϱ�
        {
            char com_str[12];
            char sensor_name[12];
            
            //[SS-Temp  ]=
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-Temp");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //��ȡSS-Temp
                index = 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.TRH.temp = atoi(temp_str);
                index += 2;

                //[SS-RH    ]=
                //��ȡSS-RH
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.TRH.humi = atoi(temp_str);
                s_SensorData.TRH.status = TRUE;
                index += 2;
            }
            else
            {
                s_SensorData.TRH.status = FALSE;
            }
            
            //��ȡSS-PM2.5
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-PM2.5");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //real_val
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.PM2_5.real_val = atoi(temp_str);    
                index += 1;
                //label_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.PM2_5.label_val = atoi(temp_str);   
                index += 1;
                //app_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.PM2_5.app_val = atoi(temp_str);
                s_SensorData.PM2_5.status = TRUE;    
                index += 2;
            }
            else
            {
                s_SensorData.PM2_5.status = FALSE;
            }
            
            //��ȡSS-PM10
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-PM10");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //real_val
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.PM10.real_val = atoi(temp_str);
                index += 1;
                //label_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.PM10.label_val = atoi(temp_str);
                index += 1;
                //app_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.PM10.app_val = atoi(temp_str);
                s_SensorData.PM10.status = TRUE;
                index += 2;
            }
            else
            {
                s_SensorData.PM10.status = FALSE;
            }
            
            //��ȡSS-CO(3)
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-CO");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //real_val
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.CO.real_val = atoi(temp_str);
                index += 1;
                //label_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.CO.label_val = atoi(temp_str);
                index += 1;
                //app_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.CO.app_val = atoi(temp_str);
                s_SensorData.CO.status = TRUE;
                index += 2;
            }
            else
            {
                s_SensorData.CO.status = FALSE;
            }
            
            //��ȡSS-NO2(4)
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-NO2");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //real_val
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.NO2.real_val = atoi(temp_str);
                index += 1;
                //label_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.NO2.label_val = atoi(temp_str);
                index += 1;
                //app_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.NO2.app_val = atoi(temp_str);
                s_SensorData.NO2.status = TRUE;
                index += 2;
            }
            else
            {
                s_SensorData.NO2.status = FALSE;
            }
            
            //��ȡSS-O3(5) 
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-O3");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //real_val
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.O3.real_val = atoi(temp_str);
                index += 1;
                //label_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.O3.label_val = atoi(temp_str);
                index += 1;
                //app_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.O3.app_val = atoi(temp_str);
                s_SensorData.O3.status = TRUE;
                index += 2;
            }
            else
            {
                s_SensorData.O3.status = FALSE;
            }
            
            //��ȡSS-SO2(6) 
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-SO2");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //real_val
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.SO2.real_val = atoi(temp_str);
                index += 1;
                //label_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.SO2.label_val = atoi(temp_str);
                index += 1;
                //app_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.SO2.app_val = atoi(temp_str);
                s_SensorData.SO2.status = TRUE;
                index += 2;
            }
            else
            {
                s_SensorData.SO2.status = FALSE;
            }
            
            //��ȡSS-NO(7)
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-NO");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //real_val
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.NO.real_val = atoi(temp_str);
                index += 1;
                //label_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.NO.label_val = atoi(temp_str);
                index += 1;
                //app_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.NO.app_val = atoi(temp_str);
                s_SensorData.NO.status = TRUE;
                index += 2;
            }
            else
            {
                s_SensorData.NO.status = FALSE;
            }
            
            //��ȡSS-TVOC(9
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-TVOC");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //�°���豸û��TVOC��ӡ������������������������2018-11-10
                //��ȡSS-TVOC(9
                //real_val
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.TVOC.real_val = atoi(temp_str);
                index += 1;
                //label_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.TVOC.label_val = atoi(temp_str);
                index += 1;
                //app_val
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.TVOC.app_val = atoi(temp_str);
                s_SensorData.TVOC.status = TRUE;
                index += 2;
            }
            else
            {
                s_SensorData.TVOC.status = FALSE;
            }
            
            //��ȡSS-Temp-W
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[SS-Temp-W");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //��ȡSS-Temp-W
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.ExtSensor.temp = atoi(temp_str);
                index += 2;
                
                //��ȡSS-RH-W
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.ExtSensor.humi = atoi(temp_str);
                index += 2;
                
                //��ȡSS-WD
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.ExtSensor.wd = atoi(temp_str);
                index += 2;
                
                //��ȡSS-WS
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.ExtSensor.ws = atoi(temp_str);
                index += 2;
                
                //��ȡSS-PA
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.ExtSensor.pa = atoi(temp_str);
                s_SensorData.ExtSensor.status = TRUE;  
                index += 2;
            }
            else
            {
                s_SensorData.ExtSensor.status = FALSE;
            }
            
            //��ȡFUN-main
            memset(sensor_name, '\0', sizeof(sensor_name));
            strcpy(sensor_name, "[FUN-main");
            memset(com_str, '\0', sizeof(com_str));
            memcpy(com_str, &resp_str[index], strlen(sensor_name));
            if(strcmp(com_str, sensor_name) == SUCCEED)
            {
                //��ȡFUN-main
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '/') && (resp_str[index + 1] != 'm'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.Fan.m_freq = atoi(temp_str);
                index += 6;     //[FUN-main ]=0/min\r\n
                
                //��ȡFUN-pm10
                index += 9 + 3;
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                    if(index >= len)
                    {
                        return FAILURE;
                    }
                }while((resp_str[index] != '/') && (resp_str[index + 1] != 'm'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorData.Fan.pm10_freq = atoi(temp_str);
                s_SensorData.Fan.status = TRUE;
                index += 6;     
            }
            else
            {
                s_SensorData.Fan.status = FALSE;
            }
            
            s_SensorData.got_status = TRUE; //����˴���������
            //��ȡ���ں�ʱ��
            //ʾ����2018-9-28	3:26:1
            //year
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];  
                if(index >= len)
                {
                    return FAILURE;
                }
            }while((resp_str[index] != '-'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.year = atoi(temp_str);
            index += 1;  
            //month
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }    
            }while((resp_str[index] != '-'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.month = atoi(temp_str);
            index += 1;      
            //day
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }   
            }while((resp_str[index] != 0x09));  //�Ʊ�λ
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.day = atoi(temp_str);
            index += 1;   
            //hour
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];  
                if(index >= len)
                {
                    return FAILURE;
                }
            }while((resp_str[index] != ':'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.hour = atoi(temp_str);
            index += 1;
            //min
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];  
                if(index >= len)
                {
                    return FAILURE;
                }  
            }while((resp_str[index] != ':'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.min = atoi(temp_str);
            index += 1;      
            //sec
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++]; 
                if(index >= len)
                {
                    return FAILURE;
                }     
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.sec = atoi(temp_str);

            // �������ش��������ݵĴ�ӡ
            Device_Printf_Ctr(DEVICE_CTR_SENSOR_HIDE_CMD);
            
            // ������ʾ��Դ��ѹ���ݵĴ�ӡ
            Device_Printf_Ctr(DEVICE_CTR_BATTERY_PRINTF_CMD);
        }
        break;
        
        case DEVICE_GET_GPS_DATA_CMD:       //�����GPS��Ϣ�ϱ�
        {
//            float temp_lat = 0.0;   //��Ÿ�������γ��
//            float temp_lon = 0.0;   //��Ÿ������ľ���
//            u16 temp_dir = 0;       //����汱����ֵ
            //$GPGGA,
            //��ȡUTCʱ��
            index = 7;
            //Сʱ
            i = 0;
            temp_str[i++] = resp_str[index++];
            temp_str[i++] = resp_str[index++];
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.hour = atoi(temp_str);
            //����
            i = 0;
            temp_str[i++] = resp_str[index++];
            temp_str[i++] = resp_str[index++];
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.min = atoi(temp_str);
            //��
            i = 0;
            temp_str[i++] = resp_str[index++];
            temp_str[i++] = resp_str[index++];
            temp_str[i++] = '\0';   //�ӽ�����
            s_RTC.sec = atoi(temp_str);
            //��ҪС��������С��
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }   
            index += 1;
            
            //��ȡγ��
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.latitude = (s32)(atof(temp_str) * pow(10, 7));
            index += 1;
            //�˵�γ�Ȱ���
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //��ȡ����
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.longitude = (s32)(atof(temp_str) * pow(10, 7));
            index += 1;
            //�˵����Ȱ���
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵�GPS״̬
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //��ȡʹ��������
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.noSV = atoi(temp_str);
            index += 1;
            
            //�˵�HDOPˮƽ��������
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //��ȡ���θ߶ȣ�m��
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.high = atoi(temp_str);
            index += 1;
            
            //�˵��߶ȵ�λM
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵����ˮ׼��߶��쳣��ֵ
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵����ˮ׼��߶��쳣��ֵ�ĵ�λM
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵����GPS��������
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵���ֲο���վ���
            i = 0;
            while((resp_str[index] != '*'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵�У����
            i = 0;
            while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //�˵�GPGll���
            i = 0;
            while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //�˵���$GPGSA����
            i = 0;
            while((resp_str[index] != 'A') && (resp_str[index + 1] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //�˵���λģʽ
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //��ȡ��λ����
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.fs = atoi(temp_str);
            index += 1;
            
            //�˵�$GPGSA����������
            i = 0;
            while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //�˵���$GPGSV����
            i = 0;
            while((resp_str[index] != 'V') && (resp_str[index + 1] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //�˵�$GPGSV����������
            i = 0;
            while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //�˵���$GPRMC����
            i = 0;
            while((resp_str[index] != 'C') && (resp_str[index + 1] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //�˵�utcʱ��
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //��ȡ��λ״̬
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            s_GPSInfo.status = temp_str[0];
            index += 1;
            
            //�˵�γ��
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            //�˵�γ�Ȱ���
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵�����
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            //�˵����Ȱ���
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵�$GPRMC����������
            i = 0;
            while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //�˵���$GPVTG����
            i = 0;
            while((resp_str[index] != 'G') && (resp_str[index + 1] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 2;
            
            //��ȡ�汱�ƶ�����
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.heading = (u8)(atoi(temp_str) / 2);
            index += 1;
            
            //�˵��ű�����
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //�˵��������ʣ��ڣ�
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            index += 1;
            
            //��ȡ�����ٶȣ�����/Сʱ��
            i = 0;
            while((resp_str[index] != ','))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.speed = atoi(temp_str) * 10;  //����ʮ��
            index += 1;
            
            //�˵�$GPVTG����������
            i = 0;
            while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }  
            
            s_GPSInfo.got_status = TRUE;    //��ȡ�ɹ�
            
            // ��������GPS���ݵĴ�ӡ
            Device_Printf_Ctr(DEVICE_CTR_GPS_HIDE_CMD);
        }
        break;
        
        case DEVICE_GET_BATTERY_VOL_CMD:       //����ǵ�Դ��ѹ�ϱ�
        {
            u16 temp_data;
            //[ADC]: VBat=2759/1522V
            //��ȡADֵ
            index = 12;
            i = 0;
            while((resp_str[index] != '/'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_data = atoi(temp_str);

            s_DevicePower.adc[0] = (u8)(temp_data >> 8);
            s_DevicePower.adc[1] = (u8)temp_data;
            index += 1;
            //��ȡ��ѹֵ
            i = 0;
            while((resp_str[index] != 'V'))
            {
                temp_str[i++] = resp_str[index++];
                if(index >= len)
                {
                    return FAILURE;
                }
            }
            temp_data = atoi(temp_str);
            s_DevicePower.vol[0] = (u8)(temp_data >> 8);
            s_DevicePower.vol[1] = (u8)temp_data;
            index += 1;

            // �������ص�Դ��ѹ�Ĵ�ӡ
            Device_Printf_Ctr(DEVICE_CTR_BATTERY_HIDE_CMD);
        }
        break;
        
        default :
        
        break;
    }
    
    return SUCCEED;
}

//********************************************************
//��������: Device_Comm_Package_Analysis
//��������: �豸�����ݽ�������
//��    ��: u8 *data��������, u16 data_l�������ݳ���
//��    ��: u8��������״̬��0�����ɹ���
//��    ע: �˺������������豸������������ӡ����Ϣ������
//********************************************************
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l)
{
    u16  index;
    char com_str[100];  //�����ַ���
    char const sam_start_str[]      = "[EVENT]: Sampling start!";
    char const sensor_data_str[]    = "[SS-Temp";
    char const gps_data_str[]       = "$GPGGA,";
    char const power_data_str[]     = "[ADC]: VBat=";
    
    memset(com_str, '\0', sizeof(com_str));
    
    for(index = 0; index < (data_l / 2); index++)
    {
        //\r\n[SS-Temp......����������
        memcpy(com_str, &data[index], strlen(sensor_data_str));
        com_str[strlen(sensor_data_str)] = '\0';  //���������
        if(strcmp(com_str, sensor_data_str) == SUCCEED)
        {
            if(data_l >= 350)   //1���豸���Ǵ�����ϵͳ��  �е����ݻᱻ���������жϣ����·ֳ����ɰ����յ���2���ⲿ������������
            {
                Device_Comm_Package_Process(DEVICE_GET_SENSOR_DATA_CMD, &data[index], (data_l - index));
                
                break;
            }
            else
            {
                break;
            }
        }
        
        //$GPGGA......GPS����
        memcpy(com_str, &data[index], strlen(gps_data_str));
        com_str[strlen(gps_data_str)] = '\0';  //���������
        if(strcmp(com_str, gps_data_str) == SUCCEED)
        {
            Device_Comm_Package_Process(DEVICE_GET_GPS_DATA_CMD, &data[index], (data_l - index));
            
            break;
        }
        
        //[ADC]: VBat=2759/1522V
        memcpy(com_str, &data[index], strlen(power_data_str));
        com_str[strlen(power_data_str)] = '\0';  //���������
        if(strcmp(com_str, power_data_str) == SUCCEED)
        {
            Device_Comm_Package_Process(DEVICE_GET_BATTERY_VOL_CMD, &data[index], (data_l - index));
            
            break;
        }
        
        //\r\n[EVENT]: Sampling start!......�豸�������
        memcpy(com_str, &data[index], strlen(sam_start_str));
        com_str[strlen(sam_start_str)] = '\0';  //���������
        if(strcmp(com_str, sam_start_str) == SUCCEED)
        {
            g_DeviceStartFlag = TRUE;
            
            break;
        }
    }
    
    return SUCCEED;
}

//********************************************************
//��������: Device_Rec_Command_Analysis
//��������: �����豸�����������ݽ�������
//��    ��: u8 cmd����������, u8* buf������������, u16 len�������ݳ���
//��    ��: u8��������״̬��0�����ɹ���1����ʧ�ܣ�2��������������
//��    ע: �˺������������豸�����б�����ӡ����Ӧ���������Ϣ������
//********************************************************
u8 Device_Rec_Command_Analysis(u8 cmd, u8* buf, u16 len)
{
    u16  index;
    u16  i;
    char temp_str[512];
    u8  ana_sta = FAILURE;
   
    memset(temp_str, '\0', sizeof(temp_str));
    
    //���ҵ���\r\n��ͷ
    for(index = 0; index < (len / 2); index++)
    {
        if((buf[index] == '\r') && (buf[index + 1] == '\n'))
        {
            index += 2;
            break;
        }
    }

    //�ȴ�����ʲô����
    switch(cmd)
    {
        case DEVICE_SET_SENSOR_LAB_ADJUST_CMD:
        {
            //����ǡ�S_SSL=��
            char cmp_str[] = "S_SSL=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_SET_SENSOR_APP_ADJUST_CMD:
        {
            //����ǡ�S_SSL=��
            char cmp_str[] = "S_SSA=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:
        {
            //����ǡ�Q_SSL=��
            char cmp_str[] = "Q_SSL=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_DEVICE_ID:   //����Ƕ�ȡ�豸ID
        {
            char cmp_str[] = "Q_ID=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_SET_RTC_CMD:
        {
            //����ǡ�S_TIME=��
            char cmp_str[] = "S_TIME=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_RTC_CMD:
        {
            //����ǡ�Q_TIME=��
            char cmp_str[] = "Q_TIME=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:
        {
            //����ǡ�S_IP=��
            char cmp_str[] = "S_IP=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:
        {
            //����ǡ�Q_IP=��
            char cmp_str[] = "Q_IP=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:
        {
            //����ǡ�S_DATARPT=��
            char cmp_str[] = "S_DATARPT=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:
        {
            //����ǡ�Q_DATARPT=��
            char cmp_str[] = "Q_DATARPT=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:
        {
            //����ǡ�S_HEALTHRPT=��
            char cmp_str[] = "S_HEALTHRPT=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:
        {
            //����ǡ�Q_HEALTHRPT=��
            char cmp_str[] = "Q_HEALTHRPT=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:
        {
            //����ǡ�C_RESET=��
            char cmp_str[] = "C_RESET=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:
        {
            //����ǡ�SHOW_ALL=��
            char cmp_str[] = "SHOW_ALL=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:
        {
            //����ǡ�HIDE_ALL=��
            char cmp_str[] = "HIDE_ALL=";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:
        {
            //����ǡ�SHOW_DATA��
            char cmp_str[] = "SHOW_DATA";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:
        {
            //����ǡ�HIDE_DATA��
            char cmp_str[] = "HIDE_DATA";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:
        {
            //����ǡ�SHOW_GPS��
            char cmp_str[] = "SHOW_GPS";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:
        {
            //����ǡ�HIDE_GPS��
            char cmp_str[] = "HIDE_GPS";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_BATTERY_PRINTF_CMD:
        {
            //����ǡ�SHOW_ADC��
            char cmp_str[] = "SHOW_ADC";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        case DEVICE_CTR_BATTERY_HIDE_CMD:
        {
            //����ǡ�HIDE_ADC��
            char cmp_str[] = "HIDE_ADC";
            memcpy(temp_str, &buf[index], strlen(cmp_str));
            temp_str[index + strlen(cmp_str)] = '\0';
            if(strcmp(temp_str, cmp_str) == SUCCEED)
            {
                index += strlen(cmp_str);
                ana_sta = SUCCEED;
            }
        }
        break;
        
        default:
        break;
    }  
    
    if(ana_sta == SUCCEED)
    {
        
#if (DEVICE_PRINTF_EN)
        printf("�ҵ���Ӧ�������ͷ\r\n");
#endif   
        
        i = 0;
        do
        {
            temp_str[i++] = buf[index++];      
        }while((buf[index] != '\r') && (buf[index + 1] != '\n'));
        temp_str[i] = '\0';

        return Device_Comm_Package_Process(cmd, (u8*)temp_str, i);
    }
    else
    {
        
#if (DEVICE_PRINTF_EN)
        printf("û�ҵ�Ӧ�������ͷ\r\n");
#endif   
        
        return 2;
    }
        
}

/*****************************************
//����: Device_Rec_Response_Cmd_Monitor
//����: �豸����Ӧ������Ĺ�����
//���: u8 cmd��������
//����: u8�����ؿ��ƽ��
******************************************/
u8 Device_Rec_Response_Cmd_Monitor(u8 cmd)
{
    u16 temp_l = 0;
    u8  temp_sta = FAILURE; //����������ɹ���ʧ�ܡ����������ݰ���������ӡ���������ݣ�
    u8  times_cnt = 0;  //����
    
    s_DeviceCommRx.Status = FALSE;
    s_DeviceCommRx.Index = 0;
    s_DeviceCommRx.Timeout_Count = 0;
    while(9)
    {
        if(s_DeviceCommRx.Status == TRUE)   //�н���
        {
            if(s_DeviceCommRx.Timeout_Count >= DEVICE_COMM_RX_DATA_TIMEOUT)    //�Ƿ�ʱ��
            {
                if((s_DeviceCommRx.Index < 5))   //�������Чֵ������
                {
                    s_DeviceCommRx.Status = FALSE;
                    s_DeviceCommRx.Index = 0;
                    s_DeviceCommRx.Timeout_Count = 0;
                    
                    continue; 
                }
                temp_l = s_DeviceCommRx.Index;    //���������ݳ���
                //�����ݿ�������������������ֹ���µ�������û
                memcpy(g_PublicDataBuffer, s_DeviceCommRx.Buffer, temp_l); 
                
                s_DeviceCommRx.Status = FALSE;
                s_DeviceCommRx.Index = 0;
                s_DeviceCommRx.Timeout_Count = 0;
                
#if (DEVICE_PRINTF_EN)
                printf("���յ��豸�˷�������: ");
                for(u16 i = 0; i < temp_l; i++)
                {
                    printf("%c", g_PublicDataBuffer[i]);
                }
                printf("\r\n");
#endif	
                
                //�������յ���ATָ��
                temp_sta = Device_Rec_Command_Analysis(cmd, g_PublicDataBuffer, temp_l);
                if(temp_sta == FAILURE)  //���Ӧ��ʧ��
                {
                    return FAILURE;
                }
                else if(temp_sta == SUCCEED)    //����ǳɹ���
                {
                    return SUCCEED;
                }
                else
                {
                    times_cnt++;
                    if(times_cnt >= 2)
                    {
                        return FAILURE;
                    }
                }
            }
        }
        //���糬ʱû�н��յ���Ҫ��Ӧ�����˳�
        else 
        {
            if(s_DeviceCommRx.Timeout_Count >= DEVICE_COMM_WAIT_RESPONSE_TIMEOUT)
            {    
                s_DeviceCommRx.Timeout_Count = 0;
                
                return FAILURE;
            }
        }
    }
}

/*****************************************
//����: Device_Printf_Ctr
//����: �豸��ӡ������ƺ���
//���: u8 cmd��������
//����: u8�����ؿ��ƽ��
******************************************/
u8 Device_Printf_Ctr(u8 cmd)
{
    u8 index;
    u8 temp_sta = FAILURE;
    
    //���Ϳ�������
    Device_Comm_Package_Bale(cmd);
    
    index = 0;
    while(9)
    {
        temp_sta = Device_Rec_Response_Cmd_Monitor(cmd);
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
                Device_Comm_Package_Bale(cmd);
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
//����: Device_Initial
//����: �豸�˳�ʼ������
//���: ��
//����: u8�����ؿ��ƽ��
******************************************/
u8 Device_Initial(void)
{
    u32 temp_timing_count = 0;
    // ���һ���豸�˵Ľ������ݣ���ֹ��ʼ������ģ���ʱ�����̫��
    s_DeviceCommRx.Status = FALSE;
    s_DeviceCommRx.Index = 0;
    s_DeviceCommRx.Timeout_Count = 0;
        
#if (SERVER_AT_PRINTF_EN)
    printf("��λ�豸��\r\n");
#endif	
    
    // �ȸ�λ�豸
    Delay_ms(3000);
    Device_Printf_Ctr(DEVICE_CTR_RESET_CMD);
    // �ж��豸����״̬����⡰[EVENT]: Sampling start!��
    temp_timing_count = g_ms_Timing_Count;
    while(g_DeviceStartFlag == FALSE)
    {
        Device_Comm_Rec_Monitor();
        
        if(abs(g_ms_Timing_Count - temp_timing_count) > DEVICE_INITIAL_OVER_TIMEOUT)
        {
            return FAILURE;
        }
    }
    
#if (SERVER_AT_PRINTF_EN)
    printf("��ȡ�豸��GPS\r\n");
#endif	
    
    // ��ȡGPS��Ϣ
    Device_Printf_Ctr(DEVICE_CTR_GPS_PRINTF_CMD);
    temp_timing_count = g_ms_Timing_Count;
    while(s_GPSInfo.got_status == FALSE)
    {
        Device_Comm_Rec_Monitor();
        
        if(abs(g_ms_Timing_Count - temp_timing_count) > DEVICE_GET_GPS_DATA_TIMEOUT)
        {
            return FAILURE;
        }
    }
            
#if (SERVER_AT_PRINTF_EN)
    printf("��ȡ�豸���豸���\r\n");
#endif	
    
    //��ȡ�豸���
    Device_Printf_Ctr(DEVICE_READ_DEVICE_ID); 
            
#if (SERVER_AT_PRINTF_EN)
    printf("��ȡ�����������ϴ����\r\n");
#endif	
    
    //��ѯͨ�������ϴ����
    Device_Printf_Ctr(DEVICE_READ_UPLOAD_INTERVAL_CMD);
                
#if (SERVER_AT_PRINTF_EN)
    printf("��ȡ�����ϴ����\r\n");
#endif	
    
    //��ѯ�������ϴ����
    Device_Printf_Ctr(DEVICE_READ_HEARTBEAT_INTERVAL_CMD);
            
#if (SERVER_AT_PRINTF_EN)
    printf("��ȡ�豸��IP�Ͷ˿�\r\n");
#endif	
    
    // ��ȡIP�˿�
    Device_Printf_Ctr(DEVICE_READ_IP_ADDR_CMD);
                
#if (SERVER_AT_PRINTF_EN)
    printf("��ȡ�豸�˵�ص�ѹ\r\n");
#endif	
    
    //��ȡ��ص�ѹ
    Device_Printf_Ctr(DEVICE_CTR_BATTERY_PRINTF_CMD); 
    // �����Դ��ѹ��Ϊ0��˵����ȡ���˵�Դ��ѹ
    temp_timing_count = g_ms_Timing_Count;
    while((s_DevicePower.vol[0] == 0) && (s_DevicePower.vol[1] == 0))
    {
        Device_Comm_Rec_Monitor();
        
        if(abs(g_ms_Timing_Count - temp_timing_count) > DEVICE_GET_POWER_DATA_TIMEOUT)
        {
            return FAILURE;
        }
    }
      
    g_DeviceInitFlag = TRUE;
    
#if (DEVICE_PRINTF_EN)
    printf("�豸��ʼ���ɹ���\r\n");
#endif	
    
    return SUCCEED;
}

//********************************************************
//��������: Device_Comm_Rec_Monitor
//��������: ���豸��ͨ�ŵļ�⺯��
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void Device_Comm_Rec_Monitor(void)      
{
    if(s_DeviceCommRx.Status == TRUE)   //�н���
    {
        if(s_DeviceCommRx.Timeout_Count >= DEVICE_COMM_RX_DATA_TIMEOUT)    //�Ƿ�ʱ��
        {
            u16 temp_l = s_DeviceCommRx.Index;    //���������ݳ���
            //�����ݿ�������������������ֹ���µ�������û
            memcpy(g_PublicDataBuffer, s_DeviceCommRx.Buffer, temp_l); 
            
            s_DeviceCommRx.Status = FALSE;
            s_DeviceCommRx.Index = 0;
            s_DeviceCommRx.Timeout_Count = 0;
            
#if (DEVICE_PRINTF_EN)
            printf("���յ��豸��%d������: ", temp_l);
            for(u16 i = 0; i < temp_l; i++)
            {
                printf("%c", g_PublicDataBuffer[i]);
            }
            printf("\r\n");
#endif	
            
            Device_Comm_Package_Analysis(g_PublicDataBuffer, temp_l);
        }
    }
    //����ܳ�ʱ��û�н��յ��豸�˵����ݣ�˵���豸��Ҫô�����ˣ�ҪôRS485�߳������ˣ��������豸��
    else if(s_DeviceCommRx.Timeout_Count >= DEVICE_COMM_NO_DATA_REC_TIMEOUT)
    {
        s_DeviceCommRx.Timeout_Count = 0;
        
        s_DeviceCommRx.Status = FALSE;
        s_DeviceCommRx.Index = 0;
        s_DeviceCommRx.Timeout_Count = 0;
    }
}





//////////////////////////////END OF C�ļ�////////////////////////////////////
