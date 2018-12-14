
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


//�����εĸߵ��ֽ�
#define LSB(a)                              ((a).cV[0])
#define MSB(a)   	                        ((a).cV[1])

//���ֽ��͵ĸߵ�4λ
#define HALF_LSB(a)	                        ((a).HalfByte.Lsb)
#define HALF_MSB(a)	                        ((a).HalfByte.Msb)

//��ָ��
#define Nop()                               {}

////��������Ŀ
//#define DEVICE_ALL_TUNNEL_NUMBER            (5 * 10)

//���չʾλ��Ŀ��������ϵİ�������
#define DEVICE_ALL_POSITION_NUMBER          4

//�豸�Ļ��ͱ�ʶ��
#define DEVICE_MACHINE_TYPE_ID              1

//�����豸��
#define LOCAL_DEVICE_ID                     1

//�жϻ����Ƿ�����
#define JUDGE_TUNNEL_RUNNING_STATUS         0x40        //

//�жϻ�������Ʒʣ����
#define JUDGE_TUNNEL_RESIDUE_NUMBER         0x3F

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

// �¶ȿ��Ƶ�ʱ����
#define SYS_TEMP_CTR_TIME_INTERVAL          (1000 * 3)  //Ĭ����3s

// ϵͳ����ʱ���жϵ�ʱ����
#define SYS_WORK_TIME_JUDGE_INTERVAL        (1000 * 1)  //Ĭ����1s


// ϵͳ����ʱ�¶ȿ��Ƶ��²��
#define COLD_TEMP_DIFFERIENCE_UPPER_LIMIT    2           //2��������²�
#define COLD_TEMP_DIFFERIENCE_LOWER_LIMIT    2           //2��������²�

// ϵͳ����ʱ�¶ȿ��Ƶ��²��
#define HEAT_TEMP_DIFFERIENCE_UPPER_LIMIT    2           //2��������²�
#define HEAT_TEMP_DIFFERIENCE_LOWER_LIMIT    2           //2��������²�

// ϵͳ������򿪺�رյ���ʱ
#define EMLOCK_OPEN_TIMEOUT                 (1000 * 5)



//������õ��������ͷ�����Ĭ��ֵ
#define GOODS_COORDS_MAX_ROW_NUM            8               //ϵͳ֧����������Ϊ16
#define GOODS_COORDS_MAX_LEFT_COLUMN_NUM    8               //ϵͳ֧�������������Ϊ8
#define GOODS_COORDS_MAX_RIGHT_COLUMN_NUM   8               //ϵͳ֧�������Ҳ�����Ϊ8

#define GOODS_COORDS_BLOCK_NUM              2               //�����Ϊ2�飬�������Ҳ�
#define GOODS_COORDS_ROW_NUM                1               //�����Ϊ5��
#define GOODS_COORDS_LEFT_COLUMN_NUM        1               //�����������Ϊ6
#define GOODS_COORDS_RIGHT_COLUMN_NUM       1               //�����Ҳ�����Ϊ5

// ���������ķ�Χ
#define PULLOUT_TUNNEL_MIN_VALUE            0x11            //��С�Ļ�����1-1
#define PULLOUT_TUNNEL_MAX_VALUE            (GOODS_COORDS_ROW_NUM * 16 + \
                                            (GOODS_COORDS_LEFT_COLUMN_NUM + GOODS_COORDS_RIGHT_COLUMN_NUM))    //���Ļ�����

// ϵͳ������ĵ�·������
#define VMC_ALL_HAVE_FIRMVARE_PCBS_NUM      3           //�ܹ���3��������PCB��
// ͨ�Ű�Ӳ���汾��
#define SYSTEM_MC_HARDWARE_VERSION          0x0100      //Ӳ���汾��V1.0.0
// ͨ�Ű�̼��汾��
#define SYSTEM_MC_SOFTWARE_VERSION          0x0140      //�̼��汾��V1.4.0

// �豸��Ӳ���汾��
#define SYSTEM_GG_HARDWARE_VERSION          0x1100      //Ӳ���汾��V1.0.0
// �豸�˹̼��汾��
#define SYSTEM_GG_SOFTWARE_VERSION          0x1100      //�̼��汾��V1.0.0

// ��դ��Ӳ���汾��
#define SYSTEM_RD_HARDWARE_VERSION          0x2100      //Ӳ���汾��V1.0.0
// ��դ��̼��汾��
#define SYSTEM_RD_SOFTWARE_VERSION          0x2100      //�̼��汾��V1.0.0

// �豸����
#define SYSTEM_DEVICE_NAME                  "H11"       //��������һ��Ѱַ��

//���֧�ֵ�һ���Գ�����������
#define ONCE_PULLOUT_MAX_CHANNEL_NUMBER     8







/******************************************************************************
//ö�١��ṹ�����Ͷ���
*******************************************************************************/
//�Ƿ���ҪӦ���ö��
typedef enum
{
    DONT_RESPONSE = 0x00,       //����ҪӦ��
    NEED_RESPONSE = 0x80        //��ҪӦ��
}CommResponseFlagEnum;

// vmc_st���ۻ���״̬��ö��
typedef enum
{
    DEVICE_WORK_NORMAL = 0,     //��������
    DEVICE_ERROR,               //����
    DEVICE_BUSY,                //æµ��
//    DEVICE_MAINTAIN             //�豸ά��
}VMCStatusEnum;

// ά��ģʽ״̬��ö��
typedef enum
{
    EXIT_MAINTAIN = 0,              //�˳�ά��ģʽ
    ACCESS_MAINTAIN                 //����ά��ģʽ
}MaintainModeEnum;

// VMC���ſ��Ƶ�ѡ��ö��
typedef enum
{
    DEVICE_DOOR = 0,                //�豸����
    GET_GOODS_DOOR                  //ȡ����
}OpenDoorSelectEnum;

// VMC���ƿ��Ƶ�ѡ��ö��
typedef enum
{
    OUTLINE_LED = 0,                //ʾ����
    POSITION_LED                    //��尴����
}OpenLEDSelectEnum;

// �����Ƿ�������ö��
typedef enum
{
    TUNNEL_NORMAL = 0x00,           //��������
    TUNNEL_ERROR  = 0x40            //��������
}TunnelRunStatusEnum;

//���ڵ�ö��
typedef enum
{
    MONDAY = 1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
}WeekDayEnum;

//����ʱ���ö��
typedef enum
{
    ENERGY_SAVE_TIME = 0,       //���ܹ���ʱ���
    SALE_WORK_TIME,             //��������ʱ���
    OTHER_WORK_TIME             //����ʱ���
}SysWorkTimeEnum;

//ϵͳ�¶ȿ���ö��
typedef enum
{
    HOLD_NORMAL_TEMP = 0,       //���ֳ���
    REFRIGERATION_TEMP,         //����
    WARMMING_TEMP               //����
}SysTempCtrStatusEnum;

// ϵͳ�¶ȿ���ģʽö��
typedef enum
{
    ACCORD_TO_TIME_FRAME = 0,   //����ʱ��ο���
    ACCORD_TO_AUTO,             //�Զ�����
    ACCORD_TO_NO_TEMP           //û���·��¶ȣ�������
}SysTempCtrModeEnum;

// ϵͳ����״̬�仯��־ö��
typedef enum
{
    SYSTEM_RUN_STATUS_CHANGED = 0,      //�б仯
    SYSTEM_RUN_STATUS_STAY              //û�б仯
}SysRunStatusChangeEnum;


//����״̬�����
//���������״̬ö��
typedef enum
{
    GET_GOODS_SUCCEED = 0,              //�����ɹ�
    GET_GOODS_FAILURE = 1,              //����ʧ�ܣ����������
    GET_GOODS_NO_GOODS_FAILURE = 2,     //�û�������Ʒ����ʧ��
    GET_GOODS_ING = 0xFF                //���ڳ���
}GetGoodsResultEnum;

//����״̬ö�ٶ���
typedef enum
{
    NO_REPLENISH = 0,   //û�в�������
    REPLENISH_OVER      //�������
}ReplenishStatusEnum;

//ȡ���ſ���״̬��ö��
typedef enum
{
    DOOR_PULLOUT_OPEN = 0,  //ȡ���Ŵ���
    DOOR_PULLOUT_CLOSE,     //ȡ���Źر�
//    DOOR_PULLOUT_UNKNOWN    //ȡ���ŷǿ��ǹ�״̬
}PulloutDoorSwitchEnum;

//��λ�Ĳ������״̬ö��
typedef enum
{
    ALL_STEP_MOTOR_NORMAL = 0,      //���еĲ������������
    HORIZONTAL_MOTOR_FAULT,         //ˮƽ�Ĳ����������
    VERTICAL_MOTOR_FAULT,           //��ֱ�Ĳ����������
    ALL_STEP_MOTOR_FAULT,           //���еĲ����������
}LocateStepMotorStatusEnum;


//�������Ƿ��л����ö��
typedef enum
{
    BOX_NO_GOODS = 0,
    BOX_HAVE_GOODS          //�������л���
}BoxGoodsStatusEnum;

//�������⴫����״̬ö��
typedef enum
{
    INFRARED_SENSOR_NORMAL = 0, //����������
    INFRARED_SENSOR_FAULT
}PulloutInfraredSensorEnum;

//����ֱ�����״̬ö��
typedef enum
{
    ALL_PULLOUT_MOTOR_NORMAL = 0,   //���еĳ���ֱ���������
    LEFT_PULLOUT_MOTOR_FAULT,       //���ȡ���������
    LEFT_ROTATE_MOTOR_FAULT,        //�����ת�������
    RIGHT_PULLOUT_MOTOR_FAULT,      //�Ҳ�ȡ���������
    RIGHT_ROTATE_MOTOR_FAULT,       //�Ҳ���ת�������
    PULLOUT_DOOR_MOTOR_FAULT        //ȡ���ŵ������
}PulloutMotorStatusEnum;

//�������̵ı�־ö��
typedef enum
{
    GET_GOODS_IDLE = 0,     //û�г�����������
    LOCATE_RUNING,          //��λ������
    PULLOUT_RUNING,        //�ӻ���ȡ����
    LOCATE_DOOR,            //��λȡ������
    WAIT_TAKE_GOODS,        //�ȴ�ȡ�߻���
    BACK_TO_ORIGIN,         //��ԭ����

    GET_GOODS_OVER,         //��������
    GET_GOODS_ERROR = 0xFF  //��������
}GetGoodsProcessEnum;


//������Ȧ��
//ϵͳ��λ/����״̬ö��
typedef enum
{
    SYSTEM_RST = 0,     //�Ѹ�λ�ɹ�
    SYSTEM_NORMAL       //û������
}SystemResetStatusEnum;

//״ָ̬ʾ��״̬ö��
typedef enum
{
    LED_TURN_OFF = 0,
    LED_TURN_ON         //ָʾ�ƴ�
}IndicateLEDEnum;

//������״̬ö��
typedef enum
{
    BUZZER_OPEN = 1,        //��������
    BUZZER_CLOSE
}BuzzerStatusEnum;


//���ݰ�������־ö��
typedef enum
{
    PACKAGE_UNCONTINUE_TRANS = 0,
    PACKAGE_CONTINUE_TRANS = 1
}PackageContinueTransStatusEnum;

//��������ö��
typedef enum
{
    ACTION_BUTTON = 0,      //��������
    ACTION_DEVICE_DOOR,      //�豸���Ŷ���
    ACTION_HUMANBODY_INFRARED//��������Ӧ
}ActionTypeEnum;

//���״̬ö��
typedef enum
{
    MOTOR_NORMAL = 0,      //�������
    MOTOR_FAULT,            //�������
}SysMotorStatusEnum;

//ȡ����״̬ö��
typedef enum
{
    DOOR_CLOSE = 0,     //ȡ���Źر�
    DOOR_OPEN
}PulloutDoorStatusEnum;

// ����ʣ�����״̬
typedef enum
{
    REMAIN_NO_GOODS             = 0,    //û�л�����
    REMAIN_HAVE_GOODS           = 1,    //���л���
    REMAIN_UNKNOWN_GOODS_STATUS = 0xFF  //���ܼ��
}ChannelInnageStatus;

// ��������Ӧ��ö��
typedef enum
{
    PULLOUT_RIGHTNOW = 0,       //��������
    PULLOUT_BUSY,               //������
    PULLOUT_CHANNEL_WRONG       //���������Ŵ���
}PulloutCtrReplyEnum;




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

//ϵͳ�¶ȿ��Ƶļ�ʱ
extern u16 g_SysTempCtrTimeCnt;

//ϵͳ����ʱ���жϵļ�ʱ
extern u16 g_SysWorkTimeJudgeCnt;

//ϵͳ����ʱ��α�־
extern u8 g_SysEnergySaveTimeFlag;

//ϵͳ����ʱ��α�־
extern u8 g_SysSaleTimeFlag;

// ϵͳ����ʱ������ñ�־
extern u8 g_SysEnergySaveTimeSetFlag;

// ϵͳ����ʱ�����ñ�־
extern u8 g_SysSaleTimeSetFlag;

// ϵͳ����״̬�仯��־
extern u8 g_SysRunStatusChangeFlag;

// �豸�˸�λ��־
extern u8 g_PulloutBoardReseted;

// ϵͳУʱ��־
extern u8 g_SysCorrectDateTimeFlag;

// ϵͳ��ʼ��״̬��־
extern u8 g_SysInitStatusFlag;

// ϵͳ����/�˳�����״̬ʱ��������״ֵ̬�ı���
extern u8 g_SysAccessExitErrorVMCStatus;

//// ������򿪺�رյļ�ʱ
//extern u16 g_SysEMLockOpenCnt;

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
