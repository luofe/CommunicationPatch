
/*******************************************************************************
//Device_Comm.h
//���豸ͨ�ŵ�����ͷ�ļ�
��ע: 
*******************************************************************************/

#ifndef	DEVICE_COMM_H
#define	DEVICE_COMM_H

/******************************************************************************
//�궨��
*******************************************************************************/
#define DEVICE_COMM_USART_BAUDRATE             115200                       //���岨����
#define DEVICE_COMM_USART                      USART3                       //����ʹ�õĴ��ں�
#define DEVICE_COMM_USART_CLK                  RCC_APB1Periph_USART3        //����ʹ�õ�ģ��ʱ��

#define DEVICE_COMM_USART_IRQn                 USART3_IRQn
#define DEVICE_COMM_USART_IRQHandler           USART3_IRQHandler

//USART_TX -----------------------------------------------
#define DEVICE_COMM_USART_TX_GPIO_CLK          RCC_APB2Periph_GPIOB         //����USART_TX�˿ڵ�ʱ��
#define DEVICE_COMM_USART_TX_GPIO_PORT         GPIOB                        //����USART_TXռ�õ�GPIO�˿�
#define DEVICE_COMM_USART_TX_GPIO_PIN          GPIO_Pin_10                   //����USART_TX��GPIO���ź�

//USART_RX -----------------------------------------------
#define DEVICE_COMM_USART_RX_GPIO_CLK          RCC_APB2Periph_GPIOB         //����USART_RX�˿ڵ�ʱ��
#define DEVICE_COMM_USART_RX_GPIO_PORT         GPIOB                        //����USART_RXռ�õ�GPIO�˿�
#define DEVICE_COMM_USART_RX_GPIO_PIN          GPIO_Pin_11                   //����USART_RX��GPIO���ź�


//Modbusͨ��������󳤶�
#define DEVICE_COMM_DATA_MAX_LENGTH            1024  


//���ݰ��������б�
#define DEVICE_SET_SENSOR_ADJUST_CMD            0   //У׼������
#define DEVICE_READ_SENSOR_ADJUST_CMD           1   //��ȡ������У׼������
#define DEVICE_READ_DEVICE_ID                   2   //��ȡ�豸ID��
#define DEVICE_SET_RTC_CMD                      3   //����RTCʱ��
#define DEVICE_READ_RTC_CMD                     4   //��ȡRTCʱ��
#define DEVICE_SET_IP_ADDR_CMD                  5   //����IP��ַ
#define DEVICE_READ_IP_ADDR_CMD                 6   //��ȡIP��ַ
#define DEVICE_SET_UPLOAD_INTERVAL_CMD          7   //���������ϴ����
#define DEVICE_READ_UPLOAD_INTERVAL_CMD         8   //��ȡ�����ϴ����
#define DEVICE_SET_HEARTBEAT_INTERVAL_CMD       9   //�������������
#define DEVICE_READ_HEARTBEAT_INTERVAL_CMD      10  //��ȡ���������
#define DEVICE_CTR_RESET_CMD                    11  //�����豸�˸�λ

#define DEVICE_CTR_ALL_PRINTF_CMD               12  //�����豸��ӡ������Ϣ
#define DEVICE_CTR_ALL_HIDE_CMD                 13  //�����豸�������д�ӡ��Ϣ
#define DEVICE_CTR_SENSOR_PRINTF_CMD            14  //�����豸��ӡ��������Ϣ
#define DEVICE_CTR_SENSOR_HIDE_CMD              15  //�����豸���ش�������Ϣ
#define DEVICE_CTR_GPS_PRINTF_CMD               16  //�����豸��ӡGPS��Ϣ
#define DEVICE_CTR_GPS_HIDE_CMD                 17  //�����豸����GPS��Ϣ

#define DEVICE_GET_SENSOR_DATA_CMD              18  //��ȡ����������
#define DEVICE_GET_GPS_DATA_CMD                 19  //��ȡGPS����

#define DEVICE_CTR_BATTERY_PRINTF_CMD           20  //�����豸��ӡ��Դ��ѹ��Ϣ
#define DEVICE_CTR_BATTERY_HIDE_CMD             21  //�����豸���ص�Դ��ѹ��Ϣ
#define DEVICE_GET_BATTERY_VOL_CMD              22  //��ȡ��Դ��ѹ




//���豸��ͨ������ط�����
#define DEVICE_COMM_REPEAT_SENDTIME            3       //�ط�����3��
//���豸��ͨ�����ȴ����ʱ��
#define DEVICE_COMM_WAIT_RESPONSE_TIMEOUT      (1000 * 1)    //�ȴ��豸��Ӧ��ʱΪ1s


//USART���ճ�ʱ
#define DEVICE_COMM_RX_DATA_TIMEOUT            10	    //û�����ݽ��ճ�ʱ�ȴ�ʱ����5����λ��Systick���ڣ�



// ��ȡ�豸�˴��������ݵ�ʱ����
#define DEVICE_COMM_GET_SENSOR_DATA_INTERVAL    (1000 * 10) 

// �豸��������������ʱ��
#define DEVICE_INITIAL_OVER_TIMEOUT             (1000 * 20)

// �豸�������ȡGPS���ݵ����ʱ��
#define DEVICE_GET_GPS_DATA_TIMEOUT             (1000 * 10)

// �豸�������ȡ�豸��ŵ����ʱ��
#define DEVICE_GET_DEVICE_ID_TIMEOUT            (1000 * 2)

// �豸�������ȡ��Դ��ѹ�����ʱ��
#define DEVICE_GET_POWER_DATA_TIMEOUT           (1000 * 2)




/******************************************************************************
//ö�١��ṹ�����Ͷ���
*******************************************************************************/
//���豸��ͨ�ŵĽ������ݵĽṹ�嶨��
typedef struct
{
    u8  Status;                         //��������״̬��־
    u32 Timeout_Count;                  //���ճ�ʱ����,��λms
    u8  Buffer[DEVICE_COMM_DATA_MAX_LENGTH]; //�������ݻ�����
    u16 Index;                          //�������ݼ���
}DeviceCommRxStruct;

//���豸��ͨ�ŵķ������ݵĽṹ�嶨��
typedef struct
{
    struct
    {
        u8  Buffer[DEVICE_COMM_DATA_MAX_LENGTH]; //�������ݻ�����
        u16 Index;                          //�������ݼ���
    }LineUp[2];
    
    u8  RepeatNum;                      //�ط�������0�������ط�
    
    u8  WaitResponse;                   //�ȴ��豸��Ӧ���־
    u16 WaitResponseTimeout;            //�ȴ�Ӧ��ĳ�ʱʱ��
}DeviceCommTxStruct;

// ������У׼�Ĳ����ṹ��
typedef struct
{
    u8  type;
    u16 multiple;
    s16 K1;
    s16 B1;
    s16 K2;
    s16 B2;
    s16 K3;
    s16 B3;
    s16 K4;
    s16 B4;
}SensorAdjStruct;

// RTC�ṹ��
typedef struct
{
    u16 year;
    u8  month;
    u8  day;
    u8  hour;
    u8  min;
    u8  sec;
}RTCStruct;

// IP��ַ�Ͷ˿ںŽṹ��
typedef struct
{
//    u8  ip1;
//    u8  ip2;
//    u8  ip3;
//    u8  ip4;
//    u16 port;
    u8  ip_port[25];
}IPAddrPortStruct;

// �����ϴ�ʱ�����ṹ��
typedef struct
{
    u16 time1;
    u16 time2;
    u16 heartbeat;
}UploadIntervalStruct;

// ͨ���������ݽṹ��
typedef struct
{
    u8  got_status;     //��ȡ״̬��TRUE�������
    u8  device_sta[4];  //�豸״̬��
    u8  sensor_num;     //����������
    
    struct
    {
        float temp;     //�¶�
        float humi;     //ʪ��
    }TRH;
    struct
    {
        s32 real_val;   //��ʵֵ
        s32 label_val;  //ʵ��������ֵ
        s32 app_val;    //Ӧ��ֵ
    }PM2_5;
    struct
    {
        s32 real_val;   //��ʵֵ
        s32 label_val;  //ʵ��������ֵ
        s32 app_val;    //Ӧ��ֵ
    }PM10;
    struct
    {
        s32 real_val;   //��ʵֵ
        s32 label_val;  //ʵ��������ֵ
        s32 app_val;    //Ӧ��ֵ
    }CO;
    struct
    {
        s32 real_val;   //��ʵֵ
        s32 label_val;  //ʵ��������ֵ
        s32 app_val;    //Ӧ��ֵ
    }NO2;
    struct
    {
        s32 real_val;   //��ʵֵ
        s32 label_val;  //ʵ��������ֵ
        s32 app_val;    //Ӧ��ֵ
    }O3;
    struct
    {
        s32 real_val;   //��ʵֵ
        s32 label_val;  //ʵ��������ֵ
        s32 app_val;    //Ӧ��ֵ
    }SO2;
    struct
    {
        s32 real_val;   //��ʵֵ
        s32 label_val;  //ʵ��������ֵ
        s32 app_val;    //Ӧ��ֵ
    }NO;
    struct
    {
        s32 real_val;   //��ʵֵ
        s32 label_val;  //ʵ��������ֵ
        s32 app_val;    //Ӧ��ֵ
    }TVOC;
    struct
    {
        float temp;     //�¶�
        float humi;     //ʪ��
        float wd;       //����
        float ws;       //����
        float pa;       //��ѹ
    }ExtSensor;
    struct
    {
        u16 m_freq;     //������Ƶ��
        u16 pm10_freq;  //PM10����Ƶ��
    }Fan;
}SensorDataStruct;

// GPS��λ��Ϣ
typedef struct 
{
    u8  got_status;     //��ȡ״̬
    u8  status;         // ��λ״̬'A'��λ��'V'����λ
    u8  fs;             // ��״̬'1'����λ��'2'2D��λ��'3'3D��λ
    s32 latitude;       // �Զ�Ϊ��λ��γ��ֵ����10��7�η�����ȷ�������֮һ��
    s32 longitude;      // �Զ�Ϊ��λ�ľ���ֵ����10��7�η�����ȷ�������֮һ��
    s16 high;           // ���θ߶ȣ���mΪ��λ
    u16 speed;          // ��km/hΪ��λ����10����ֵ
    u8  heading;        // 0-359,����Ϊ0
    u8  noSV;           // ���ڶ�λ����������
    u32 gmtTime;        // ��1970�������
}GPSInfoStruct;

// ��Դ��ѹ��Ϣ
typedef struct
{
    u8  adc[2];
    u8  vol[2];     //��ص�ѹ
}DevicePowerStruct;



/******************************************************************************
//��������/����
*******************************************************************************/
//�������豸��ͨ�ŵĽ������ݽṹ��
extern DeviceCommRxStruct s_DeviceCommRx;

//���豸��ͨ�ŵķ������ݵĽṹ�嶨��
extern DeviceCommTxStruct s_DeviceCommTx;

// ������У׼�Ĳ����ṹ������
extern SensorAdjStruct s_SensorAdj;

// RTC�ṹ������
extern RTCStruct s_RTC;

// IP��ַ�Ͷ˿ںŽṹ������
extern IPAddrPortStruct s_IPAddrPort;

// �����ϱ�ʱ�����ṹ������
extern UploadIntervalStruct s_UploadInterval;

// ͨ�ô��������ݽṹ������
extern SensorDataStruct s_SensorData;

// GPS��λ��Ϣ
extern GPSInfoStruct s_GPSInfo;

// �豸��ʼ����ϱ�־
extern u8  g_DeviceInitialFlag;

// ��Դ��ѹ��Ϣ
extern DevicePowerStruct s_DevicePower;

// �豸��ʼ��״̬
extern u8  g_DeviceInitFlag;







/******************************************************************************
//��������
*******************************************************************************/
//��������: ���豸��ͨ����صĳ�ʼ������
void Device_Comm_Init(void);

//����: ���豸�˰�ͨ�ŵķ�������
void Device_Comm_Send_Data(u8 *buffer, u16 data_l);

//��������: ���豸��ͨ�ŵ����ݰ��������
void Device_Comm_Package_Bale(u8 cmd);

//��������: ���豸��ͨ�ŵ����ݰ�������
u8 Device_Comm_Package_Process(u8 cmd, u8* resp_str, u16 len);

//��������: ��ͨ�Ű�ͨ�����ݰ���������
u8 Device_Comm_Package_Analysis(u8 *data, u16 data_l);

//����: �豸��ӡ������ƺ���
u8 Device_Printf_Ctr(u8 cmd);

//����: �豸�˳�ʼ������
u8 Device_Initial(void);

//��������: ���豸��ͨ�ŵļ�⺯��
void Device_Comm_Rec_Monitor(void);
























#endif