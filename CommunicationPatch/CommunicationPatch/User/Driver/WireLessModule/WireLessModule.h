
/*******************************************************************************
//WireLessModule.h
//��������ģ�������ͷ�ļ�
��ע: 
*******************************************************************************/

#ifndef	WIRELESSMODULE_H
#define	WIRELESSMODULE_H

/******************************************************************************
//�궨��
*******************************************************************************/
//ATָ��������б�
#define AT_COMMAND_CPIN             0       //���SIM��״̬
#define AT_COMMAND_CREG             1       //��⿨����ע��״̬
#define AT_COMMAND_CGREG            2       //���GPRS����ע��״̬
#define AT_COMMAND_QICSGP           3       //����APN��������ID
#define AT_COMMAND_CGQMIN           4       //���ÿɽ��ܵ���С��������
#define AT_COMMAND_QIDEACT          5       //ʧ��PDP������
#define AT_COMMAND_QIACT_EN         6       //����PDP������
#define AT_COMMAND_QIACT_DE         7       //���PDP������
#define AT_COMMAND_CGQMIN_DE        8       //���ɽ��ܵ���С��������
#define AT_COMMAND_QIOPEN           9       //���÷�����IP��ַ�Ͷ˿ںţ���������
#define AT_COMMAND_QICLOSE          10      //�ر�����
#define AT_COMMAND_SWITCH_CMD       11      //�л�������ģʽ��+++����ע�⣺û�лس����У�����
#define AT_COMMAND_ATO              12      //�л���͸��ģʽ
#define AT_COMMAND_QCCID            13      //��ȡSIM����CCID��
#define AT_COMMAND_CIMI             14      //��ȡSIM����ISMI��
#define AT_COMMAND_CGSN             15      //��ȡSIM����IMEI��
#define AT_COMMAND_QPWOD            16      //����ģ��ػ������ڸ�λ��
#define AT_COMMAND_CFUN             17      //����ģ�鹦��ģʽ�������ڸ�λ��
//#define AT_COMMAND_READY            18      //�ȴ�ģ����������RDY



//������ID
#define WIRELESS_CONTEXT_ID         1       //1~16
//����ID��Socket��ţ�
#define WIRELESS_CONNECT_ID         0       //0~11
//�����APN
#define WIRELESS_APN                "CMNET" //�й��ƶ����й����š��й���ͨ�ȵ�Ӣ����д
//����������
#define WIRELESS_SOCKET_TYPE        "TCP"   //��TCP��UDP��TCP-Listener��
//������IP��ַ
#define WIRELESS_SERVER_IP          "58.83.189.147"  
//�����������˿ں�
#define WIRELESS_SERVER_REMOTE_PORT "21009" 
//����ͨ��ģʽ
#define WIRELESS_SERVER_ACCESS_MODE 2       //��0-Bufferģʽ��1-Directģʽ �� 2-͸��ģʽ




// �ȴ�����ģ��������ϵ����ȴ�ʱ��
#define WIRELESS_WAIT_RDY_TIMEOUT       (1000 * 20)     //20s
// �ȴ�SIM��״̬�����ȴ�ʱ��
#define WIRELESS_WAIT_CPIN_TIMEOUT      (1000 * 3)      //20s
// �ȴ�SIM��������ע��״̬���ȴ�ʱ��
#define WIRELESS_WAIT_CREG_TIMEOUT      (1000 * 90)     //90s
// �ȴ�SIM����GPRS����ע��״̬���ȴ�ʱ��
#define WIRELESS_WAIT_CGREG_TIMEOUT     (1000 * 60)     //60s
// �ȴ�����APN��������ID�����ȴ�ʱ��
#define WIRELESS_WAIT_QICSGP_TIMEOUT    (1000 * 1)     //1s
// �ȴ����ÿɽ��ܵ���С�������������ȴ�ʱ��
#define WIRELESS_WAIT_CGQMIN_TIMEOUT    (1000 * 1)     //1s
// �ȴ�ʧ��PDP�����ĵ����ȴ�ʱ��
#define WIRELESS_WAIT_QIDEACT_TIMEOUT   (1000 * 10)    //10s
// �ȴ�����PDP�����ĵ����ȴ�ʱ��
#define WIRELESS_WAIT_QIACT_EN_TIMEOUT  (1000 * 10)    //10s
// �ȴ����PDP�����ĵ����ȴ�ʱ��
#define WIRELESS_WAIT_QIACT_DE_TIMEOUT  (1000 * 10)    //10s 
// �ȴ����ɽ��ܵ���С�������������ȴ�ʱ��
#define WIRELESS_WAIT_CGQMIN_DE_TIMEOUT (1000 * 10)    //10s 
// �ȴ����÷�����IP��ַ�Ͷ˿ںš��������ӵ����ȴ�ʱ��
#define WIRELESS_WAIT_QIOPEN_TIMEOUT    (1000 * 10)    //10s

// ATָ���Ӧ�����2���֣��ֱ����ظ�һ��ָ���Ӧ��ָ��
// ��SUCCEED��FAILUREͬ��ʹ�ã�����Ҫ�ܿ�ǰ��2��ֵ
#define RECEIVE_AT_COMMAND_REPEAT       2













/******************************************************************************
//ö�١��ṹ�����Ͷ���
*******************************************************************************/












/******************************************************************************
//��������/����
*******************************************************************************/










/******************************************************************************
//��������
*******************************************************************************/
//����: ����ATָ��ĺ���
void WireLess_Send_AT_Command(u8 cmd);



























#endif