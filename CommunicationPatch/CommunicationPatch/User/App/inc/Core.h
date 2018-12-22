
/*
core.h	�����㷨����ͷ�ļ�
*/
#ifndef CORE_H
#define CORE_H

/******************************************************************************
//�궨��
*******************************************************************************/
// �������ݻ�������С
#define PUBLIC_DATA_BUFFER_MAX_LENGTH       4096        //Ĭ��Ϊ255


//��ָ��
#define Nop()                               {}


//����POLLָ�����������ʱ����
#define SEND_POLL_PACKAGE_TIME_INTERVAL     (60) //Ĭ����1h����λ�Ƿ���

//���ʹ��������ݸ���������ʱ����
#define SEND_SENSOR_DATA_TIME_INTERVAL      (60) //Ĭ����1min����λ����

//�����Լ���ϸ�������,�����ʼ����ʱ����
#define SEND_STARTUP_RPT_TIME_INTERVAL      (1000 * 5)  //Ĭ����5s

//û�н��յ����������������ʱʱ��
#define SERVER_COMM_NO_DATA_REC_TIMEOUT     (1000UL * 60 * 2)   //û�н��յ������������ݳ�ʱʱ��Ϊ2min

//���豸�˲�ѯ״̬��ʱ����
#define GET_PULLOUT_STATUS_TIME_INTERVAL    (1000 * 5)  //Ĭ����5s

//û�н��յ��豸�˵��������ʱʱ��
#define DEVICE_COMM_NO_DATA_REC_TIMEOUT    (1000UL * 60 * 5)   //û�н��յ��豸�˵����ݳ�ʱʱ��Ϊ5min





/******************************************************************************
//ö�١��ṹ�����Ͷ���
*******************************************************************************/
//�Ƿ���ҪӦ���ö��
typedef enum
{
    DONT_RESPONSE = 0x00,       //����ҪӦ��
    NEED_RESPONSE = 0x80        //��ҪӦ��
}CommResponseFlagEnum;





/******************************************************************************
//��������/����
*******************************************************************************/
// �������ݻ��������������ݽ��������ݷ��͡����ݴ洢��
extern u8 g_PublicDataBuffer[];

// ϵͳ����ʹ�ܱ�־
extern u8 g_SysPulloutEnableFlag;

//ϵͳ����POLL���ݰ����������ļ�ʱ
extern u32 g_SysPollTimeCnt;

//ϵͳ��ѯ�豸��״̬�ļ�ʱ
extern u32 g_GetPulloutStatusTimeCnt;

// ϵͳ��ʼ��״̬��־
extern u8 g_SysInitStatusFlag;

// �豸���ϱ�־
extern u8 g_SystemDeviceErrorFlag;

// ���ʹ��������ݵļ�ʱ
extern u32 g_SendSensorDataTimeCnt;

// ������������ģ��ļ����ʱ
extern u32 g_ReSetWireLessModuleTimeCnt;




















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








#endif

/**********************************END OF FILE*********************************/
