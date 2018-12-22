
/*******************************************************************************
//Server_Comm.h
//�������ͨ����ص�����ͷ�ļ�
*******************************************************************************/

#ifndef	SERVER_COMM_H
#define	SERVER_COMM_H

/******************************************************************************
//�궨��
*******************************************************************************/
//#define SERVER_COMM_USART_BAUDRATE                  9600                       //����SERVER_COMM_USARTͨ�Ų�����
//#define SERVER_COMM_USART                           USART1                       //����SERVER_COMM_USARTʹ�õĴ��ں�
//#define SERVER_COMM_USART_CLK                       RCC_APB2Periph_USART1        //����SERVER_COMM_USARTʹ�õ�ģ��ʱ��
//
//#define SERVER_COMM_USART_IRQn                      USART1_IRQn
//#define SERVER_COMM_USART_IRQHandler                USART1_IRQHandler
//
////SERVER_COMM_USART_TX -----------------------------------------------
//#define SERVER_COMM_USART_TX_GPIO_CLK               RCC_APB2Periph_GPIOA         //����SERVER_COMM_USART_TX�˿ڵ�ʱ��
//#define SERVER_COMM_USART_TX_GPIO_PORT              GPIOA                        //����SERVER_COMM_USART_TXռ�õ�GPIO�˿�
//#define SERVER_COMM_USART_TX_GPIO_PIN               GPIO_Pin_9                   //����SERVER_COMM_USART_TX��GPIO���ź�
//
////SERVER_COMM_USART_RX -----------------------------------------------
//#define SERVER_COMM_USART_RX_GPIO_CLK               RCC_APB2Periph_GPIOA         //����SERVER_COMM_USART_RX�˿ڵ�ʱ��
//#define SERVER_COMM_USART_RX_GPIO_PORT              GPIOA                        //����SERVER_COMM_USART_RXռ�õ�GPIO�˿�
//#define SERVER_COMM_USART_RX_GPIO_PIN               GPIO_Pin_10                  //����SERVER_COMM_USART_RX��GPIO���ź�

#define SERVER_COMM_USART_BAUDRATE              115200                       //����SERVER_COMM_USARTͨ�Ų�����
#define SERVER_COMM_USART                       USART2                       //����SERVER_COMM_USARTʹ�õĴ��ں�
#define SERVER_COMM_USART_CLK                   RCC_APB1Periph_USART2        //����SERVER_COMM_USARTʹ�õ�ģ��ʱ��

#define SERVER_COMM_USART_IRQn                  USART2_IRQn
#define SERVER_COMM_USART_IRQHandler            USART2_IRQHandler

//SERVER_COMM_USART_TX -----------------------------------------------
#define SERVER_COMM_USART_TX_GPIO_CLK           RCC_APB2Periph_GPIOA         //����SERVER_COMM_USART_TX�˿ڵ�ʱ��
#define SERVER_COMM_USART_TX_GPIO_PORT          GPIOA                        //����SERVER_COMM_USART_TXռ�õ�GPIO�˿�
#define SERVER_COMM_USART_TX_GPIO_PIN           GPIO_Pin_2                   //����SERVER_COMM_USART_TX��GPIO���ź�

//SERVER_COMM_USART_RX -----------------------------------------------
#define SERVER_COMM_USART_RX_GPIO_CLK           RCC_APB2Periph_GPIOA         //����SERVER_COMM_USART_RX�˿ڵ�ʱ��
#define SERVER_COMM_USART_RX_GPIO_PORT          GPIOA                        //����SERVER_COMM_USART_RXռ�õ�GPIO�˿�
#define SERVER_COMM_USART_RX_GPIO_PIN           GPIO_Pin_3                  //����SERVER_COMM_USART_RX��GPIO���ź�





//ͨ�����ݰ�����
#define SERVER_COMM_PACKAGE_HEAD                    0x7E    //��ͷ
#define SERVER_COMM_PACKAGE_IDENTIFY                0xAA    //��ʶ��
#define SERVER_COMM_PACKAGE_VERSION                 0x01    //�汾��1
#define SERVER_COMM_PACKAGE_DT                      0x00    //�豸������Ѱַ��
#define SERVER_COMM_PACKAGE_DT_ALL                  0xFF    //֧�����е�����(�����Լ�����ʱ�ж�)
#define SERVER_COMM_PACKAGE_DID                     0x01    //�豸���1
#define SERVER_COMM_PACKAGE_BROADCAST_DID           0x00    //�㲥�豸���
#define SERVER_COMM_PACKAGE_RESERVED                0x00    //Ԥ���������ֽ�
#define SERVER_COMM_PACKAGE_CONTINUE_TRANS          0x80    //������
#define SERVER_COMM_PACKAGE_UNCONTINUE_TRANS        0x00    //�����������򵥰�
#define SERVER_COMM_PACKAGE_TAIL                    0x54    //��β


//���ݰ��������б�
//ͨ��Ӧ��
#define SERVER_COMM_PACKAGE_CMD_RESPONSE            0x6001    //
//������
#define SERVER_COMM_PACKAGE_CMD_SET_PARA            0x0B00    //���ò���
//��ѯ��
#define SERVER_COMM_PACKAGE_CMD_READ_PARA           0x0B01    //��ȡ��Ϣ
//������
#define SERVER_COMM_PACKAGE_CMD_CTR_RESET           0x0C00    //���Ƹ�λ
//�ϱ���
#define SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE    0x0D80    //��������
#define SERVER_COMM_PACKAGE_CMD_REPORT_HEARTBEAT    0x0D81    //��������
#define SERVER_COMM_PACKAGE_CMD_REPORT_DATA         0x0D83    //���洫��������
#define SERVER_COMM_PACKAGE_CMD_REPORT_PARA         0x0D87    //�����������
#define SERVER_COMM_PACKAGE_CMD_REPORT_GPS          0x0D8B    //����GPS��Ϣ
#define SERVER_COMM_PACKAGE_CMD_REPORT_FLASH        0x0E01    //�ϱ�Ƭ��Flash�����ݰ�
#define SERVER_COMM_PACKAGE_CMD_RPT_RESET           0x0D82    //�ϱ���λ

// �������Ͷ���
#define SYSTEM_PARA_TYPE_IP_ADDR_PORT               0x01        //IP��ַ���˿ں�
#define SYSTEM_PARA_TYPE_TIME_SYNCH                 0x02        //ϵͳʱ��ͬ��
#define SYSTEM_PARA_TYPE_UPLOAD_INTERVAL            0x04        //ͨ�������ϴ����
#define SYSTEM_PARA_TYPE_HEARTBEAT_INTERVAL         0x06        //�������
#define SYSTEM_PARA_TYPE_LAB_ADJUST                 0x07        //ʵ����У׼
#define SYSTEM_PARA_TYPE_APP_ADJUST                 0x08        //Ӧ��У׼
#define SYSTEM_PARA_TYPE_BATCH_APP_ADJUST           0xA0        //����Ӧ��У׼
#define SYSTEM_PARA_TYPE_BATCH_LAB_ADJUST           0xB0        //����ʵ����У׼

// �ⲿ�洢�����ݰ����Ͷ���
#define EXTFLASH_PACKAGE_TYPE_DEVICE_RESET          0x01        //�豸��λ
#define EXTFLASH_PACKAGE_TYPE_REPORT_DATA           0x02        //���������ݰ�
#define EXTFLASH_PACKAGE_TYPE_HEARTBEAT             0x03        //�������ݰ�




// ͨ�����ݰ�����״̬
//�������ݰ��ṹ
#define SERVER_COMM_PACKAGE_ANALYSIS_HEAD       0       //��ͷ
#define SERVER_COMM_PACKAGE_ANALYSIS_IDENT      1       //��ʶ��
#define SERVER_COMM_PACKAGE_ANALYSIS_LEN        2       //���ݳ���
//#define SERVER_COMM_PACKAGE_ANALYSIS_ADF        3       //������
//������
#define SERVER_COMM_PACKAGE_ANALYSIS_SN         4       //��ˮ��
#define SERVER_COMM_PACKAGE_ANALYSIS_CMD        5       //������
#define SERVER_COMM_PACKAGE_ANALYSIS_DATA       6      //��������
#define SERVER_COMM_PACKAGE_ANALYSIS_CRC        7       //У����


// ͨ�����ݰ�����������
//�������ݰ��ṹ
#define PACKAGE_ANALYSIS_SUCCEED                0       //������ȷ
#define PACKAGE_ANALYSIS_HEAD_ERROR             1       //��ͷ����
#define PACKAGE_ANALYSIS_IDENT_ERROR            2       //��ʶ������
//����˾�ṹ
#define PACKAGE_ANALYSIS_SN_ERROR               3       //���кŴ���
#define PACKAGE_ANALYSIS_CMD_ERROR              4       //���������
#define PACKAGE_ANALYSIS_DATA_ERROR             5       //�������ݴ���
#define PACKAGE_ANALYSIS_CRC_ERROR              6       //У�������

#define PACKAGE_ANALYSIS_NOT_RESPONSE_ERROR     7      //��Ӧ�������
#define PACKAGE_ANALYSIS_UNKNOWN_ERROR          0xFF    //δ֪����



//�������ͨ�ŵ�������󳤶�
#define PC_COM_MAX_DATA_LENGTH                  (512)     //�Ƕ��豸�˽�������ʱ�����ݰ���С

//USART���ճ�ʱ
#define SERVER_COMM_RX_DATA_TIMEOUT             5	    //û�н��ճ�ʱ5ms���ǽ������
//�������ͨ������ط�����
#define SERVER_COMM_REPEAT_SEND_TIME            3       //�ط�����2��
//�������ͨ�����ȴ����ʱ��
#define SERVER_COMM_WAIT_RESPONSE_TIMEOUT       (1000 * 5)  //�ȴ�������Ӧ��ʱ

//��������ָ�����������ʱ����
#define SERVER_COMM_HANDSHAKE_INTERVAL          (1000 * 30) //

// ������������ģ���ʱ����
#define RESET_WIRELESSMODULE_INTERVAL           (1000UL * 60 * 2) //Ĭ��1����








/******************************************************************************
//ö�١��ṹ�����Ͷ���
*******************************************************************************/
//�������ͨ�ŵĽ������ݵĽṹ�嶨��
typedef struct
{
    u8  Status;                         //��������״̬��־
    u32 Timeout_Count;                  //���ճ�ʱ����,��λms
    u8  Buffer[PC_COM_MAX_DATA_LENGTH]; //�������ݻ�����
    u16 Index;                          //�������ݼ���
}ServerCommRxStruct;

//�������ͨ�ŵķ������ݵĽṹ�嶨��
typedef struct
{
    u8  Buffer[PC_COM_MAX_DATA_LENGTH]; //�������ݻ�����
    u16 Index;                          //�������ݼ���

    u8  RepeatNum;                      //�ط�������0�������ط�

    u8  WaitResponse;                   //�ȴ�������Ӧ���־
    u16 WaitResponseTimeout;            //�ȴ�Ӧ��ĳ�ʱʱ��
}ServerCommTxStruct;

// �������ͨ�ŵ����ݽṹ��
typedef struct
{
    u8  Head;           //��ͷ
    u8  Identify;       //��ʶ��
    u16 Length;         //����
    union
    {
        u8 Buff[PC_COM_MAX_DATA_LENGTH];//��Ϣ��
        struct
        {
            u16 SN;     //��ˮ��
            u16 CMD;    //�����
            u8  Data[PC_COM_MAX_DATA_LENGTH - 4 - 2];//��������
            u16 Crc;    //CRCУ����
        };
    }ADF;
}ServerCommPackageStruct;

// �����������ṹ��
typedef struct
{
    u8  para_type;       //��������
//    //ʵ����У׼����
//    struct
//    {
//        u8  sensor_type;    //����������
//        u16 shrink;     //��Сϵ��
//        u16 K1;
//        u16 B1;
//        u16 K2;
//        u16 B2;
//    }Lab;
//    //Ӧ��У׼����
//    struct
//    {
//        u8  sensor_type;    //����������
//        u16 shrink;     //��Сϵ��
//        u16 K3;
//        u16 B3;
//        u16 K4;
//        u16 B4;
//    }App;
//    //IP��ַ�Ͷ˿ں�
//    u8  IPAddr[25];
//    // ͨ�������ϴ����(s)
//    u32 Upload_interval;
//    // �����ϴ����(min)
//    u16 Heartbeat_interval;
    //Э������
    u8  proc_type;
    //����
    u8  manu_type;
    //�豸����
    u8  device_type;
    //�豸���
    u8  deviceID[4];
    //��������
    u8  Code[8];
    // ����汾
    u8  Ver[5];
}SystemParaStruct;

// SIM����ز���
typedef struct
{
    u8  num_len;    //sim���ų��ȣ��й���11λ
    u8  num[2 + 11];//ǰ���86

    u8  IMSI_len;   //imsi�볤��
    u8  IMSI[15];    //һ�㶼��15λ

    u8  IMEI_len;   //imei�볤��
    u8  IMEI[15];    //һ�㶼��15λ

    u8  CCID_len;   //ccid�볤��
    u8  CCID[20];    //һ�㶼��20λ
}SIMCardParaStruct;


// ͨ��Ӧ��Ľ��ö��
typedef enum
{
    RES_SUCCEED = 0,
    RES_FAILURE,
    RES_BUSY,
    RES_UPGRADE,
    RES_PARA_ERROR,
    RES_READ_WRITE_FAILURE,
    RES_UNKNOWN,
    RES_AUTHEN_ERROR,
}ComResponseResultEnum;

/******************************************************************************
//��������/����
*******************************************************************************/
//�����������ͨ�ŵĽ������ݽṹ��
extern ServerCommRxStruct s_ServerCommRx;

//�������ͨ�ŵ����ݰ��ṹ������
extern ServerCommPackageStruct s_ServerCommPackage;

//�������ͨ�ŵķ������ݵĽṹ������
extern ServerCommTxStruct s_ServerCommTx;

// ��������������
extern SystemParaStruct s_SystemPara;

// SIM�������ṹ������
extern SIMCardParaStruct s_SIMCardPara;

// ����һ�η��͵�������
extern u16 g_LastSendServerCmd;



/******************************************************************************
//��������
*******************************************************************************/
//����:	�������ͨ����صĳ�ʼ������
void Server_Comm_Init(void);

//����: �������ͨ�ŵķ������ݺ���
void Server_Comm_Send_Data(u8 *buffer, u16 data_l);

//��������: �������ͨ�ŵ����ݰ��������
void Server_Comm_Package_Bale(u16 cmd);

//��������: �������ͨ�ŵļ�⺯��
void Server_Comm_Rec_Monitor(void);












//����: �������ͨ�ŵĲ��Ժ���
void Server_Comm_Test(void);


#endif