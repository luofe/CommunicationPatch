
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
void Device_Comm_Package_Process(u8 cmd, u8* resp_str);

//��������: ��ͨ�Ű�ͨ�����ݰ���������
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l);

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
    
	USART_ITConfig(DEVICE_COMM_USART, USART_IT_RXNE, ENABLE);	//ʹ��USART�����ж�    
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
        printf("%02X ", *buffer);
#endif	
        
        USART_SendData(DEVICE_COMM_USART, *buffer++); 
        while(USART_GetFlagStatus(DEVICE_COMM_USART, USART_FLAG_TXE) == RESET);//�ȴ��������
    } 
    
#if (DEVICE_PRINTF_EN)
    printf("\r\n");
#endif	
    
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
    
    data_str[0] = '$';
    switch(cmd)
    {
        case DEVICE_SET_SENSOR_ADJUST_CMD:   //�����У׼������
        {
            index = 1;
            strcpy(data_str, "S_SSL:");
            index += strlen("S_SSL:");
            //����type
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����multiple
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.multiple, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����K1
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.K1, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����B1
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.B1, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����K2
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.K2, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����B2
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.B2, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:   //����Ƕ�ȡ������У׼����
        {
            index = 1;
            strcpy(data_str, "Q_SSL:");
            index += strlen("Q_SSL:");
            //����type
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_SensorAdj.type, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_SET_RTC_CMD:   //�������RTC 
        {
            index = 1;
            strcpy(data_str, "S_TIME:");
            index += strlen("S_TIME:");
            //����year
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.year, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����month
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.month, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����day
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.day, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����hour
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.hour, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����min
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.min, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = '-';
            //����sec
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_RTC.sec, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_RTC_CMD:   //����Ƕ�ȡRTC
        {
            index = 1;
            strcpy(data_str, "Q_TIME");
            index += strlen("Q_TIME");
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:   //�������IP��ַ
        {
            index = 1;
            strcpy(data_str, "S_IP:");
            index += strlen("S_IP:");
            strcpy(&data_str[index], (char const*)s_IPAddrPort.ip_port);
            index += strlen(temp_str);
//            //����ip1
//            memset(temp_str, 0, sizeof(temp_str));  //�����
//            SysInt2Str(s_IPAddrPort.ip1, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //����ip2
//            memset(temp_str, 0, sizeof(temp_str));  //�����
//            SysInt2Str(s_IPAddrPort.ip2, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //����ip3
//            memset(temp_str, 0, sizeof(temp_str));  //�����
//            SysInt2Str(s_IPAddrPort.ip3, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = '.';
//            //����ip4
//            memset(temp_str, 0, sizeof(temp_str));  //�����
//            SysInt2Str(s_IPAddrPort.ip4, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
//            data_str[index++] = ',';
//            //����˿ں�
//            memset(temp_str, 0, sizeof(temp_str));  //�����
//            SysInt2Str(s_IPAddrPort.port, temp_str, 10);
//            strcpy(&data_str[index], temp_str);
//            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:   //����Ƕ�ȡIP��ַ�Ͷ˿ں�
        {
            index = 1;
            strcpy(data_str, "Q_IP");
            index += strlen("Q_IP");
        }
        break;
        
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:   //������������ϴ�ʱ����
        {
            index = 1;
            strcpy(data_str, "S_DATARPT:");
            index += strlen("S_DATARPT:");
            //����time1
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_UploadInterval.time1, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
            data_str[index++] = ',';
            //����time2
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_UploadInterval.time2, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:   //�����ȡ�����ϴ�ʱ����
        {
            index = 1;
            strcpy(data_str, "Q_DATARPT");
            index += strlen("Q_DATARPT");
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:   //�������������ʱ����
        {
            index = 1;
            strcpy(data_str, "S_HEALTHRPT:");
            index += strlen("S_HEALTHRPT:");
            //����heartbeat
            memset(temp_str, 0, sizeof(temp_str));  //�����
            SysInt2Str(s_UploadInterval.heartbeat, temp_str, 10);
            strcpy(&data_str[index], temp_str);
            index += strlen(temp_str);
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:   //�����ȡ������ʱ����
        {
            index = 1;
            strcpy(data_str, "Q_HEALTHRPT");
            index += strlen("Q_HEALTHRPT");
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:   //�������������λ
        {
            index = 1;
            strcpy(data_str, "C_RESET");
            index += strlen("C_RESET");
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:   //������ƴ�������Ϣ��ӡ
        {
            index = 1;
            strcpy(data_str, "SHOW_ALL");
            index += strlen("SHOW_ALL");
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:   //�����������������Ϣ
        {
            index = 1;
            strcpy(data_str, "HIDE_ALL");
            index += strlen("HIDE_ALL");
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:   //������ƴ�������Ϣ��ӡ
        {
            index = 1;
            strcpy(data_str, "SHOW_DATA");
            index += strlen("SHOW_DATA");
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:   //����������ش�������Ϣ
        {
            index = 1;
            strcpy(data_str, "HIDE_DATA");
            index += strlen("HIDE_DATA");
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:   //�������GPS��Ϣ��ӡ
        {
            index = 1;
            strcpy(data_str, "SHOW_GPS");
            index += strlen("SHOW_GPS");
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:   //�����������GPS��Ϣ
        {
            index = 1;
            strcpy(data_str, "HIDE_GPS");
            index += strlen("HIDE_GPS");
        }
        break;
        
        default :
        
        break;
    }
    //����س�����
    strcpy(&data_str[index], "\r\n");
    index += 2;
    //����
    Device_Comm_Send_Data((u8*)data_str, index);
}

//********************************************************
//��������: Device_Comm_Package_Process
//��������: ���豸��ͨ�ŵ����ݰ�������
//��    ��: u8 cmd���������룬u8* resp_str����Ӧ���ַ���
//��    ��: ��
//��    ע: ��
//********************************************************
void Device_Comm_Package_Process(u8 cmd, u8* resp_str)
{
    u16 i;
    u16 index = 0;
    char temp_str[100];

    switch(cmd)
    {
        case DEVICE_SET_SENSOR_ADJUST_CMD:   //�����У׼������
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //���û���óɹ�
            {
                Device_Comm_Package_Bale(DEVICE_SET_SENSOR_ADJUST_CMD);
            }
        }
        break;
        
        case DEVICE_READ_SENSOR_ADJUST_CMD:   //����Ƕ�ȡ������У׼����
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
            {
                //��ȡtype
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.type = atoi(temp_str);
                index += 1;
                //��ȡmultiple
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.multiple = atoi(temp_str);
                index += 1;
                //��ȡK1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.K1 = atoi(temp_str);
                index += 1;
                //��ȡB1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.B1 = atoi(temp_str);
                index += 1;
                //��ȡK2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.K2 = atoi(temp_str);
                index += 1;
                //��ȡB2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while((resp_str[index] != '(') && (resp_str[index + 1] != 'O'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_SensorAdj.B2 = atoi(temp_str);
                index += 2;
            }
            else    //���ʧ�ܣ����ٶ�ȡһ��
            {
                Device_Comm_Package_Bale(DEVICE_READ_SENSOR_ADJUST_CMD);
            }
        }
        break;
        
        case DEVICE_SET_RTC_CMD:   //�������RTC 
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_SET_RTC_CMD);
            }
        }
        break;
        
        case DEVICE_READ_RTC_CMD:   //����Ƕ�ȡRTC
        {
//            for(i = 0; i < 3; i++)
//            {
//                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
//            }
//            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
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
                index += 2;
            }
//            else
//            {
//                Device_Comm_Package_Bale(DEVICE_READ_RTC_CMD);
//            }
        }
        break;
        
        case DEVICE_SET_IP_ADDR_CMD:   //�������IP��ַ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_SET_IP_ADDR_CMD);
            }
        }
        break;
        
        case DEVICE_READ_IP_ADDR_CMD:   //����Ƕ�ȡIP��ַ�Ͷ˿ں�
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
            {
                memcpy(s_IPAddrPort.ip_port, resp_str, strlen((char const*)resp_str));
//                //��ȡip1
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while(resp_str[index] != '.');
//                temp_str[i++] = '\0';   //�ӽ�����
//                s_IPAddrPort.ip1 = atoi(temp_str);
//                index += 1;
//                //��ȡip2
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while(resp_str[index] != '.');
//                temp_str[i++] = '\0';   //�ӽ�����
//                s_IPAddrPort.ip2 = atoi(temp_str);
//                index += 1;
//                //��ȡip3
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while(resp_str[index] != '.');
//                temp_str[i++] = '\0';   //�ӽ�����
//                s_IPAddrPort.ip3 = atoi(temp_str);
//                index += 1;
//                //��ȡip4
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while(resp_str[index] != ',');
//                temp_str[i++] = '\0';   //�ӽ�����
//                s_IPAddrPort.ip4 = atoi(temp_str); 
//                index += 1;               
//                //��ȡport
//                i = 0;
//                do
//                {
//                    temp_str[i++] = resp_str[index++];
//                }while((resp_str[index] != '(') && (resp_str[index + 1] != 'O'));
//                temp_str[i++] = '\0';   //�ӽ�����
//                s_IPAddrPort.port = atoi(temp_str);
//                index += 2;
            }
            else    //���ʧ�ܣ����ٶ�ȡһ��
            {
                Device_Comm_Package_Bale(DEVICE_READ_IP_ADDR_CMD);
            }
        }
        break;
        
        case DEVICE_SET_UPLOAD_INTERVAL_CMD:   //������������ϴ�ʱ����
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //�����ȡ���ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_SET_UPLOAD_INTERVAL_CMD);
            }
        }
        break;
        
        case DEVICE_READ_UPLOAD_INTERVAL_CMD:   //�����ȡ�����ϴ�ʱ����
        {
//            for(i = 0; i < 3; i++)
//            {
//                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
//            }
//            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
            {
                //��ȡtime1
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while(resp_str[index] != ',');
                temp_str[i++] = '\0';   //�ӽ�����
                s_UploadInterval.time1 = atoi(temp_str);
                index += 1;
                //��ȡtime2
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_UploadInterval.time2 = atoi(temp_str);
                index += 2;
            }
//            else    //���ʧ�ܣ����ٶ�ȡһ��
//            {
//                Device_Comm_Package_Bale(DEVICE_READ_IP_ADDR_CMD);
//            }
        }
        break;
        
        case DEVICE_SET_HEARTBEAT_INTERVAL_CMD:   //�������������ʱ����
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_SET_HEARTBEAT_INTERVAL_CMD);
            }
        }
        break;
        
        case DEVICE_READ_HEARTBEAT_INTERVAL_CMD:   //�����ȡ������ʱ����
        {
//            for(i = 0; i < 3; i++)
//            {
//                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
//            }
//            if(strcmp("OK!", temp_str) == SUCCEED)  //�����ȡ�ɹ�
            {
                //��ȡheartbeat
                i = 0;
                do
                {
                    temp_str[i++] = resp_str[index++];
                }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
                temp_str[i++] = '\0';   //�ӽ�����
                s_UploadInterval.heartbeat = atoi(temp_str);
                index += 2;
            }
//            else    //���ʧ�ܣ����ٶ�ȡһ��
//            {
//                Device_Comm_Package_Bale(DEVICE_READ_HEARTBEAT_INTERVAL_CMD);
//            }
        }
        break;
        
        case DEVICE_CTR_RESET_CMD:   //�������������λ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_CTR_RESET_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_ALL_PRINTF_CMD:   //������ƴ�������Ϣ��ӡ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_CTR_ALL_PRINTF_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_ALL_HIDE_CMD:   //�����������������Ϣ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_CTR_ALL_HIDE_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_PRINTF_CMD:   //������ƴ�������Ϣ��ӡ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_CTR_SENSOR_PRINTF_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_SENSOR_HIDE_CMD:   //����������ش�������Ϣ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_CTR_SENSOR_HIDE_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_GPS_PRINTF_CMD:   //�������GPS��Ϣ��ӡ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_CTR_GPS_PRINTF_CMD);
            }
        }
        break;
        
        case DEVICE_CTR_GPS_HIDE_CMD:   //�����������GPS��Ϣ
        {
            for(i = 0; i < 3; i++)
            {
                temp_str[i] = resp_str[strlen((const char*)resp_str) - (6 - i)];
            }
            if(strcmp("OK!", temp_str) != SUCCEED)  //������ò��ɹ�
            {
                Device_Comm_Package_Bale(DEVICE_CTR_GPS_HIDE_CMD);
            }
        }
        break;
        
        case DEVICE_GET_SENSOR_DATA_CMD:   //����Ǵ����������ϱ�
        {
            //[SS-Temp  ]=
            //��ȡSS-Temp
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
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
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.TRH.humi = atoi(temp_str);
            index += 2;
            
            //��ȡSS-PM2.5
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.PM2_5.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.PM2_5.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.PM2_5.app_val = atoi(temp_str);
            index += 2;
            
            //��ȡSS-PM10
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.PM10.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.PM10.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.PM10.app_val = atoi(temp_str);
            index += 2;
            
            //��ȡSS-CO(3)
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.CO.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.CO.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.CO.app_val = atoi(temp_str);
            index += 2;
            
            //��ȡSS-NO2(4)
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.NO2.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.NO2.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.NO2.app_val = atoi(temp_str);
            index += 2;
            
            //��ȡSS-O3(5) 
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.O3.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.O3.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.O3.app_val = atoi(temp_str);
            index += 2;
            
            //��ȡSS-SO2(6) 
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.SO2.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.SO2.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.SO2.app_val = atoi(temp_str);
            index += 2;
            
            //��ȡSS-NO(7)
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.NO.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.NO.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.NO.app_val = atoi(temp_str);
            index += 2;
            
            //��ȡSS-TVOC(9
            //real_val
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.TVOC.real_val = atoi(temp_str);
            index += 1;
            //label_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while(resp_str[index] != ',');
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.TVOC.label_val = atoi(temp_str);
            index += 1;
            //app_val
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.TVOC.app_val = atoi(temp_str);
            index += 2;
            
            //��ȡSS-Temp-W
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
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
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.ExtSensor.pa = atoi(temp_str);
            index += 2;
            
            //��ȡFUN-main
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '/') && (resp_str[index + 1] != 'm'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.Fan.m_freq = atoi(temp_str);
            index += 2;
            
            //��ȡFUN-pm10
            index += 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '/') && (resp_str[index + 1] != 'm'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_SensorData.Fan.pm10_freq = atoi(temp_str);
            index += 2;
        }
        break;
        
        case DEVICE_GET_GPS_DATA_CMD:       //�����GPS��Ϣ�ϱ�
        {
            //[GPS-STA  ]=
            //��ȡGPS-STA
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.status = atoi(temp_str);
            index += 2;
            
            //��ȡGPS-FS
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.fs = atoi(temp_str);
            index += 2;
            
            //��ȡGPS-LAT
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.latitude = atoi(temp_str);
            index += 2;
            
            //��ȡGPS-LON
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.longitude = atoi(temp_str);
            index += 2;
            
            //��ȡGPS-HIGH
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.high = atoi(temp_str);
            index += 2;
            
            //��ȡGPS-SPEED
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.speed = atoi(temp_str);
            index += 2;
            
            //��ȡGPS-HEAD
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.heading = atoi(temp_str);
            index += 2;
            
            //��ȡGPS-NOSV
            index = 9 + 3;
            i = 0;
            do
            {
                temp_str[i++] = resp_str[index++];
            }while((resp_str[index] != '\r') && (resp_str[index + 1] != '\n'));
            temp_str[i++] = '\0';   //�ӽ�����
            s_GPSInfo.noSV = atoi(temp_str);
            index += 2;
        }
        break;
        
        default :
        
        break;
    }
}

//********************************************************
//��������: Device_Comm_Package_Analysis
//��������: ��ͨ�Ű�ͨ�����ݰ���������
//��    ��: u8 *data�����ṹ��ָ��, u16 data_l�������ݳ���
//��    ��: u8��������״̬��0�����ɹ���
//��    ע: ��
//********************************************************
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l)
{
    u16  index;
    u16  i;
    char temp_str[1024];
    
    //���жϵ�һ���ַ��ǲ��ǡ�$��
    if(data[0] == '$')
    {
        //����ǡ�S_SSL=��
        if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'S') && (data[4] == 'S') && (data[5] == 'L') && (data[6] == '='))
        {
            index = 7;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_SET_SENSOR_ADJUST_CMD, (u8*)temp_str);
        }
        //����ǡ�Q_SSL=��
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'S') && (data[4] == 'S') && (data[5] == 'L') && (data[6] == '='))
        {
            index = 7;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_READ_SENSOR_ADJUST_CMD, (u8*)temp_str);
        }
        //����ǡ�S_TIME=��
        else if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'T') && (data[4] == 'I') && (data[5] == 'M') && 
                (data[6] == 'E') && (data[7] == '='))
        {
            index = 8;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_SET_RTC_CMD, (u8*)temp_str);
        }
        //����ǡ�Q_TIME=��
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'T') && (data[4] == 'I') && (data[5] == 'M') && 
                (data[6] == 'E') && (data[7] == '='))
        {
            index = 8;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_READ_RTC_CMD, (u8*)temp_str);
        }
        //����ǡ�S_IP=��
        else if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'I') && (data[4] == 'P') && (data[5] == '='))
        {
            index = 6;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_SET_IP_ADDR_CMD, (u8*)temp_str);
        }
        //����ǡ�Q_IP=��
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'I') && (data[4] == 'P') && (data[5] == '='))
        {
            index = 6;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_READ_IP_ADDR_CMD, (u8*)temp_str);
        }
        //����ǡ�S_DATARPT=��
        else if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'D') && (data[4] == 'A') && (data[5] == 'T') && 
                (data[6] == 'A') && (data[7] == 'R') && (data[8] == 'P') && (data[9] == 'T') && (data[10] == '='))
        {
            index = 11;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_SET_UPLOAD_INTERVAL_CMD, (u8*)temp_str);
        }
        //����ǡ�Q_DATARPT=��
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'D') && (data[4] == 'A') && (data[5] == 'T') && 
                (data[6] == 'A') && (data[7] == 'R') && (data[8] == 'P') && (data[9] == 'T') && (data[10] == '='))
        {
            index = 11;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_READ_UPLOAD_INTERVAL_CMD, (u8*)temp_str);
        }
        //����ǡ�S_HEALTHRPT=��
        else if((data[1] == 'S') && (data[2] == '_') && (data[3] == 'H') && (data[4] == 'E') && (data[5] == 'A') && 
                (data[6] == 'L') && (data[7] == 'T') && (data[8] == 'H') && (data[9] == 'R') && (data[10] == 'P') && 
                    (data[11] == 'T') && (data[12] == '='))
        {
            index = 13;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_SET_HEARTBEAT_INTERVAL_CMD, (u8*)temp_str);
        }
        //����ǡ�Q_HEALTHRPT=��
        else if((data[1] == 'Q') && (data[2] == '_') && (data[3] == 'H') && (data[4] == 'E') && (data[5] == 'A') && 
                (data[6] == 'L') && (data[7] == 'T') && (data[8] == 'H') && (data[9] == 'R') && (data[10] == 'P') && 
                    (data[11] == 'T') && (data[12] == '='))
        {
            index = 13;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_READ_HEARTBEAT_INTERVAL_CMD, (u8*)temp_str);
        }
        //����ǡ�C_RESET=��
        else if((data[1] == 'C') && (data[2] == '_') && (data[3] == 'R') && (data[4] == 'E') && (data[5] == 'S') && 
                (data[6] == 'E') && (data[7] == 'T') && (data[8] == '='))
        {
            index = 9;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_CTR_RESET_CMD, (u8*)temp_str);
        }
        //����ǡ�SHOW_ALL=��
        else if((data[1] == 'S') && (data[2] == 'H') && (data[3] == 'O') && (data[4] == 'W') && (data[5] == '_') && 
                (data[6] == 'A') && (data[7] == 'L') && (data[8] == 'L') && (data[9] == '='))
        {
            index = 10;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_CTR_ALL_PRINTF_CMD, (u8*)temp_str);
        }
        //����ǡ�HIDE_ALL=��
        else if((data[1] == 'H') && (data[2] == 'I') && (data[3] == 'D') && (data[4] == 'E') && (data[5] == '_') && 
                (data[6] == 'A') && (data[7] == 'L') && (data[8] == 'L') && (data[9] == '='))
        {
            index = 10;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_CTR_ALL_HIDE_CMD, (u8*)temp_str);
        }
        //����ǡ�SHOW_DATA=��
        else if((data[1] == 'S') && (data[2] == 'H') && (data[3] == 'O') && (data[4] == 'W') && (data[5] == '_') && 
                (data[6] == 'D') && (data[7] == 'A') && (data[8] == 'T') && (data[9] == 'A') && (data[10] == '='))
        {
            index = 11;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_CTR_SENSOR_PRINTF_CMD, (u8*)temp_str);
        }
        //����ǡ�HIDE_DATA=��
        else if((data[1] == 'H') && (data[2] == 'I') && (data[3] == 'D') && (data[4] == 'E') && (data[5] == '_') && 
                (data[6] == 'D') && (data[7] == 'A') && (data[8] == 'T') && (data[9] == 'A') && (data[10] == '='))
        {
            index = 11;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_CTR_SENSOR_HIDE_CMD, (u8*)temp_str);
        }
        //����ǡ�SHOW_GPS=��
        else if((data[1] == 'S') && (data[2] == 'H') && (data[3] == 'O') && (data[4] == 'W') && (data[5] == '_') && 
                (data[6] == 'G') && (data[7] == 'P') && (data[8] == 'S') && (data[9] == '='))
        {
            index = 10;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_CTR_GPS_PRINTF_CMD, (u8*)temp_str);
        }
        //����ǡ�HIDE_GPS=��
        else if((data[1] == 'H') && (data[2] == 'I') && (data[3] == 'D') && (data[4] == 'E') && (data[5] == '_') && 
                (data[6] == 'G') && (data[7] == 'P') && (data[8] == 'S') && (data[9] == '='))
        {
            index = 10;
            i = 0;
            do
            {
                temp_str[i++] = data[index++];
            }while((data[index] == '\r') && (data[index + 1] == '\n'));
            temp_str[i++] = '\0'; //���Ͻ�����
            Device_Comm_Package_Process(DEVICE_CTR_GPS_HIDE_CMD, (u8*)temp_str);
        }
    }
    else
    {
        for(index = 0; index < (data_l / 2); index++)
        {
            //\r\n[SS-Temp......����������
            if((data[index] == '[') && (data[index + 1] == 'S') && (data[index + 2] == 'S') && (data[index + 3] == '-') && 
               (data[index + 4] == 'T') && (data[index + 5] == 'e') && (data[index + 6] == 'm') && (data[index + 7] == 'p'))
            {
                index = 0;
                i = 0;
                do
                {
                    temp_str[i++] = data[index++];
                }while((data[index] == '\r') && (data[index + 1] == '\n'));
                temp_str[i++] = '\0'; //���Ͻ�����
                Device_Comm_Package_Process(DEVICE_GET_SENSOR_DATA_CMD, (u8*)temp_str);
            }
            //\r\n[GPS-STA......GPS����
            else if((data[index] == '[') && (data[index + 1] == 'G') && (data[index + 2] == 'P') && (data[index + 3] == 'S') && 
               (data[index + 4] == '-') && (data[index + 5] == 'S') && (data[index + 6] == 'T') && (data[index + 7] == 'A'))
            {
                index = 0;
                i = 0;
                do
                {
                    temp_str[i++] = data[index++];
                }while((data[index] == '\r') && (data[index + 1] == '\n'));
                temp_str[i++] = '\0'; //���Ͻ�����
                Device_Comm_Package_Process(DEVICE_GET_GPS_DATA_CMD, (u8*)temp_str);
            }
        }
    }
    
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
            printf("���յ��豸�˵�����: ");
            for(u8 i = 0; i < temp_l; i++)
            {
                printf("%c", s_DeviceCommRx.Buffer[i]);
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
