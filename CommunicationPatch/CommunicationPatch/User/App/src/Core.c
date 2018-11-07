
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
u16 g_SysPollTimeCnt = SEND_POLL_PACKAGE_TIME_INTERVAL;     //Ĭ����������һ��POLL��  

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
u8 g_SysInitStatusFlag = TRUE;         //Ĭ��δ���

// ϵͳ����/�˳�����״̬ʱ��������״ֵ̬�ı���
u8 g_SysAccessExitErrorVMCStatus;

//// ������򿪺�رյļ�ʱ
//u16 g_SysEMLockOpenCnt = 0;

// �豸���ϱ�־
u8 g_SystemDeviceErrorFlag = FALSE;







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
    //�Ƚ�ֹ�豸�˵����д�ӡ��Ϣ
    Device_Comm_Package_Bale(DEVICE_CTR_ALL_HIDE_CMD);
    
    Delay_ms(100);
    //�򿪴�������Ϣ��ӡ
    Device_Comm_Package_Bale(DEVICE_CTR_SENSOR_PRINTF_CMD);
        
//    Delay_ms(100);
//    //��GPS��Ϣ��ӡ
//    Device_Comm_Package_Bale(DEVICE_CTR_GPS_PRINTF_CMD);
    
    
    //    FATFS_Init();       //SD���ļ�ϵͳ��ʼ��
    
    
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
    //��ȡƬ��Flash�����ݣ��õ��¶��ϴε��¶Ȳ���
    Internal_Flash_ReadOut();
    
    //���������ͨ����صı���
    s_ServerCommRx.Status = FALSE;
    s_ServerCommRx.Timeout_Count = 0;
    s_ServerCommRx.Index = 0;
    memset(s_ServerCommRx.Buffer, 0, sizeof(s_ServerCommRx.Buffer));
    
    
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
    //���緢������������������ʱ�䵽�ˣ����ҳ�ʼ�������
    if(g_SysInitStatusFlag == TRUE) //��ʼ�����
    {
        //��������ط���һֱû�н��գ����ҵ�ʱ����
        if((g_SysPollTimeCnt >= SEND_POLL_PACKAGE_TIME_INTERVAL))   //(s_ServerCommTx.RepeatNum < SERVER_COMM_REPEAT_SEND_TIME) && 
        {
            g_SysPollTimeCnt = 0;
            
//            u8 test_array[] = "$Q_ID\r\n";
//            
//            Device_Comm_Send_Data(test_array, (sizeof(test_array) - 1));
//            
//#if (SERVER_PRINTF_EN)
//            printf("��ѯ�豸ID: ");
//            for(u8 i = 0; i < sizeof(test_array) - 1; i++)
//            {
//                printf("%c", test_array[i]);
//            }
//            printf("\r\n");
//#endif	
//            
//            u8 test_array2[] = "AT+CGPADDR=1";
//            Server_Comm_Send_Data(test_array2, (sizeof(test_array2) - 1));
//                        
//#if (SERVER_PRINTF_EN)
//            printf("�鿴PDP��ַ: ");
//            for(u8 i = 0; i < sizeof(test_array2) - 1; i++)
//            {
//                printf("%c", test_array2[i]);
//            }
//            printf("\r\n");
//#endif	
            
        }
    }
}








//////////////////////////////END OF C�ļ�////////////////////////////////////

