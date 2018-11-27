
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:��Ŀ���̸����������ͨ�ŵ�Դ����
*******************************************************************************/

/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//��������
*******************************************************************************/
//�����������ͨ�ŵĽ������ݽṹ��
ServerCommRxStruct s_ServerCommRx;

//�������ͨ�ŵ����ݰ��ṹ��
ServerCommPackageStruct s_ServerCommPackage;

//�������ͨ�ŵķ������ݵĽṹ�嶨��
ServerCommTxStruct s_ServerCommTx;

// ��������������
SystemParaStruct s_SystemPara;

// SIM�������ṹ������
SIMCardParaStruct s_SIMCardPara;

// ���ձ������к�
u8 g_RecServerPackageSN[2] = {0x00,0x01};

// ���ձ��Ľ���������
u8  g_RecServerPackageResult = SUCCEED;

// ����һ�η��͵�������
u16 g_LastSendServerCmd = SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE; //Ĭ�������ְ�


uc16 CRC_TABLE_XW[256] =
{
    /* CRC���ұ� */
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040,
};








/******************************************************************************
//��������
*******************************************************************************/
//����:	�������ͨ����صĳ�ʼ������
void Server_Comm_Init(void);

//����:	�������ͨ�ŵ�USART�ĳ�ʼ������
void Server_Comm_Usart_Init(void);

//����: �������ͨ�ŵķ������ݺ���
void Server_Comm_Send_Data(u8 *buffer, u16 data_l);

//��������: �������ͨ�ŵ����ݰ�CRC16У��������
u16 GetCheckCRC_XW(u8 *pData, u16 len);

//��������: �������ͨ�ŵ����ݰ����ͺ���
void Server_Comm_Package_Send(void);

//��������: �������ͨ�ŵ����ݰ��������
void Server_Comm_Package_Bale(u16 cmd);

//��������: �������ͨ�ŵ����ݰ�������
void Server_Comm_Package_Process(u16 cmd, u8* data, u16 len);

//��������: �������ͨ�ŵ����ݰ���������
u8 Server_Comm_Package_Analysis(u8 *rec_array, u16 rec_length);

//��������: �������ͨ�ŵļ�⺯��
void Server_Comm_Rec_Monitor(void);

//��������: �������ͨ�ŵĴ�������
void Server_Comm_Error_Process(void);

//����: ���������豸�˵Ĺ̼������ĺ���
u8 Server_Comm_Upgrade_Pullout_Board(void);
















//����: �������ͨ�ŵĲ��Ժ���
void Server_Comm_Test(void);

/******************************************************************************
//��������
*******************************************************************************/
/**********************************
//����:	Server_Comm_Init
//����:	�������ͨ����صĳ�ʼ������
//��ڲ���:	��
//���ڲ���:	��
**********************************/
void Server_Comm_Init(void)
{
    Server_Comm_Usart_Init();
}

/**********************************
//����:	Server_Comm_Usart_Init
//����:	�������ͨ�ŵ�USART�ĳ�ʼ������
//��ڲ���:	��
//���ڲ���:	��
**********************************/
void Server_Comm_Usart_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	//��ʼ��NVIC�ṹ��
    
    //��λSERVER_COMM_USARTģ��
    USART_DeInit(SERVER_COMM_USART);
    
    //ʹ��SERVER_COMM_USART_TX����ģ��ʱ��
    RCC_APB2PeriphClockCmd(SERVER_COMM_USART_TX_GPIO_CLK, ENABLE);
    
    //ʹ��SERVER_COMM_USART_RX����ģ��ʱ��
    RCC_APB2PeriphClockCmd(SERVER_COMM_USART_RX_GPIO_CLK, ENABLE);
    
    //ʹ��USARTģ��ʱ��
    if(SERVER_COMM_USART == USART1)
    {
        RCC_APB2PeriphClockCmd(SERVER_COMM_USART_CLK, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(SERVER_COMM_USART_CLK, ENABLE);
    }
    
    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    //SERVER_COMM_USART��GPIO����
    //SERVER_COMM_USART_TX: ���츴�����
    GPIO_InitStructure.GPIO_Pin   = SERVER_COMM_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //���츴�����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//ѡ��50MHz
    GPIO_Init(SERVER_COMM_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    //SERVER_COMM_USART_RX: ��������(�����������)
    GPIO_InitStructure.GPIO_Pin   = SERVER_COMM_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(SERVER_COMM_USART_RX_GPIO_PORT, &GPIO_InitStructure);
    
    //SERVER_COMM_USARTģ���������
    //������: USART1_BAUDRATE��8������λ��1��ֹͣλ����У��λ����Ӳ�������ƣ�ʹ�ܷ��ͺͽ��գ�
    USART_InitStructure.USART_BaudRate            = SERVER_COMM_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(SERVER_COMM_USART, &USART_InitStructure); 
    
    USART_Cmd(SERVER_COMM_USART, ENABLE);                         //ʹ��SERVER_COMM_USARTģ��
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//ʹ�����ȼ�����2	
    NVIC_InitStructure.NVIC_IRQChannel                      = SERVER_COMM_USART_IRQn; //ʹ��USART�ж�	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;                //�������ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0;                //��Ӧ���ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	//����USART��Ƕ�������ж�
    
	USART_ITConfig(SERVER_COMM_USART, USART_IT_RXNE, DISABLE);	//�Ƚ�ֹ�����ж�     
}

/*****************************************
//����: Server_Comm_Send_Data
//����: �������ͨ�ŵķ������ݺ���
//���: u8 *buffer������Ҫ���͵�����;u16 data_l�������ݳ���
//����: ��
******************************************/
void Server_Comm_Send_Data(u8 *buffer, u16 data_l)
{  
    
#if (SERVER_PRINTF_EN)
    printf("\r\n�������ݸ�������:");
#endif
    
    while(data_l--)  
    {
        
#if (SERVER_PRINTF_EN)
        printf("%02X ", *buffer);
#endif
        
        USART_SendData(SERVER_COMM_USART, *buffer++); 
        while(USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_TXE) == RESET);//�ȴ��������
    } 
    
#if (SERVER_PRINTF_EN)
    printf("\r\n");
#endif
    
}

//********************************************************
//��������: GetCheckCRC_XW
//��������: �������ͨ�ŵ����ݰ�CRC16У��������
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
u16 GetCheckCRC_XW(u8 *pData, u16 len)
{
    u16 i=0;
    u16 crc = 0xffff;
    
    for ( i = 0; i < len; i++ )
    {
        crc = ( crc >> 8 ) ^ CRC_TABLE_XW[( crc ^ pData[i] ) & 0xff];
    }
    return __REV16(crc);	// ȡ������
}

//********************************************************
//��������: Server_Comm_Package_Send
//��������: �������ͨ�ŵ����ݰ����ͺ���
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void Server_Comm_Package_Send(void)
{
    u16 temp_crc;
    
    s_ServerCommTx.Index = 0;
    
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = s_ServerCommPackage.Head;
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = s_ServerCommPackage.Identify;
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)(s_ServerCommPackage.Length >> 8);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)s_ServerCommPackage.Length;
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)(s_ServerCommPackage.ADF.SN >> 8);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)s_ServerCommPackage.ADF.SN;
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)(s_ServerCommPackage.ADF.CMD >> 8);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)s_ServerCommPackage.ADF.CMD;
    g_LastSendServerCmd = s_ServerCommPackage.ADF.CMD;  //�������η��͵�������
    memcpy(&s_ServerCommTx.Buffer[s_ServerCommTx.Index], s_ServerCommPackage.ADF.Data, (s_ServerCommPackage.Length - 6));
    s_ServerCommTx.Index += s_ServerCommPackage.Length - 6;
    temp_crc = GetCheckCRC_XW(&s_ServerCommTx.Buffer[4], s_ServerCommTx.Index - 4);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)(temp_crc >> 8);
    s_ServerCommTx.Buffer[s_ServerCommTx.Index++] = (u8)temp_crc;
    
    Server_Comm_Send_Data(s_ServerCommTx.Buffer, s_ServerCommTx.Index);
    
    if(s_ServerCommPackage.ADF.SN == 0xFFFF)    //
    {
        s_ServerCommPackage.ADF.SN = 1;     //��ˮ������
    }
    else
    {
        s_ServerCommPackage.ADF.SN++;                                                   //��ˮ�ŵ���
    }
}

//********************************************************
//��������: Server_Comm_Package_Bale
//��������: �������ͨ�ŵ����ݰ��������
//��    ��: u16 cmd����������
//��    ��: ��
//��    ע: ��
//********************************************************
void Server_Comm_Package_Bale(u16 cmd)
{
    u16 i;
    s_ServerCommPackage.ADF.CMD = cmd;
    switch(cmd)
    {
        case SERVER_COMM_PACKAGE_CMD_RESPONSE:   //ͨ��Ӧ��
        {
            i = 0;
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageSN[0];
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageSN[1];
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageResult;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_HANDSHAKE:   //����
        {
            i = 0;
            //����RTCʱ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //Э������
            s_ServerCommPackage.ADF.Data[i++] = s_SystemPara.proc_type;
            //Э�鳧��
            s_ServerCommPackage.ADF.Data[i++] = s_SystemPara.manu_type;
            //�豸����
            s_ServerCommPackage.ADF.Data[i++] = s_SystemPara.device_type;
            //�豸���
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SystemPara.deviceID, 4);
            i += 4; 
            //CODE��
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SystemPara.Code, 8);
            i += 8;  
            //����汾
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SystemPara.Ver, 5);
            i += 5;
            // SIM����
            s_ServerCommPackage.ADF.Data[i++] = s_SIMCardPara.num_len;
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SIMCardPara.num, s_SIMCardPara.num_len);
            i += s_SIMCardPara.num_len;
            //SIM����IMSI��
            s_ServerCommPackage.ADF.Data[i++] = s_SIMCardPara.IMSI_len;
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SIMCardPara.IMSI, s_SIMCardPara.IMSI_len);
            i += s_SIMCardPara.IMSI_len;
            //SIM����IMEI��
            s_ServerCommPackage.ADF.Data[i++] = s_SIMCardPara.IMEI_len;
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SIMCardPara.IMEI, s_SIMCardPara.IMEI_len);
            i += s_SIMCardPara.IMEI_len;
            //SIM����CCID��
            s_ServerCommPackage.ADF.Data[i++] = s_SIMCardPara.CCID_len;
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SIMCardPara.CCID, s_SIMCardPara.CCID_len);
            i += s_SIMCardPara.CCID_len;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_HEARTBEAT:  //����
        {
            i = 0;
            //����RTCʱ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_DATA:   //�ϱ�����������
        {
            u8  sen_num_index;
            i = 0;
            //����RTCʱ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //�豸״̬��
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_SensorData.device_sta, 4);
            i += 4;
            //��ص�ѹ
            memcpy(&s_ServerCommPackage.ADF.Data[i], s_DevicePower.vol, 2);
            i += 2;
            //����������
            sen_num_index = i;
            s_SensorData.sensor_num = 0;
            i++;
            //���¶��Ǵ���������
            //PM2.5
            s_ServerCommPackage.ADF.Data[i++] = 0x00;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM2_5.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM2_5.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM2_5.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM2_5.app_val;
            s_SensorData.sensor_num++;
            //PM10
            s_ServerCommPackage.ADF.Data[i++] = 0x02;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM10.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM10.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.PM10.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.PM10.app_val;
            s_SensorData.sensor_num++;
            //CO
            s_ServerCommPackage.ADF.Data[i++] = 0x03;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.CO.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.CO.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.CO.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.CO.app_val;
            s_SensorData.sensor_num++;
            //NO2
            s_ServerCommPackage.ADF.Data[i++] = 0x04;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO2.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO2.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO2.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO2.app_val;
            s_SensorData.sensor_num++;
            //O3
            s_ServerCommPackage.ADF.Data[i++] = 0x05;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.O3.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.O3.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.O3.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.O3.app_val;
            s_SensorData.sensor_num++;
            //SO2
            s_ServerCommPackage.ADF.Data[i++] = 0x06;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.SO2.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.SO2.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.SO2.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.SO2.app_val;
            s_SensorData.sensor_num++;
            //NO
            s_ServerCommPackage.ADF.Data[i++] = 0x07;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.real_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.real_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.real_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO.real_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.label_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.label_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.label_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO.label_val;
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.app_val >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.app_val >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.NO.app_val >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.NO.app_val;
            s_SensorData.sensor_num++;
            
            // �°汾���豸û��TVOC������2018-11-10
//            //TVOC
//            s_ServerCommPackage.ADF.Data[i++] = 0x09;  
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.real_val >> 24);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.real_val >> 16);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.real_val >> 8);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TVOC.real_val;
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.label_val >> 24);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.label_val >> 16);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.label_val >> 8);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TVOC.label_val;
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.app_val >> 24);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.app_val >> 16);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TVOC.app_val >> 8);
//            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TVOC.app_val;
//            s_SensorData.sensor_num++;
            //��ʪ�ȣ��ڣ�
            s_ServerCommPackage.ADF.Data[i++] = 0x60;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.temp >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.temp >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.temp >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TRH.temp;
            for(u8 j = 0; j < 8; j++)   //ʵ����У׼ֵ��Ӧ��У׼ֵ���0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x61; 
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.humi >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.humi >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.TRH.humi >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.TRH.humi;
            for(u8 j = 0; j < 8; j++)   //ʵ����У׼ֵ��Ӧ��У׼ֵ���0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            //�ⲿ������
            s_ServerCommPackage.ADF.Data[i++] = 0x62;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.wd >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.wd >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.wd >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.wd;
            for(u8 j = 0; j < 8; j++)   //ʵ����У׼ֵ��Ӧ��У׼ֵ���0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x63; 
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.ws >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.ws >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.ws >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.ws;
            for(u8 j = 0; j < 8; j++)   //ʵ����У׼ֵ��Ӧ��У׼ֵ���0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x64;  
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.temp >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.temp >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.temp >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.temp;
            for(u8 j = 0; j < 8; j++)   //ʵ����У׼ֵ��Ӧ��У׼ֵ���0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x65; 
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.humi >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.humi >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.humi >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.humi;
            for(u8 j = 0; j < 8; j++)   //ʵ����У׼ֵ��Ӧ��У׼ֵ���0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            s_ServerCommPackage.ADF.Data[i++] = 0x66; 
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.pa >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.pa >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)((u32)s_SensorData.ExtSensor.pa >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorData.ExtSensor.pa;
            for(u8 j = 0; j < 8; j++)   //ʵ����У׼ֵ��Ӧ��У׼ֵ���0
            {
                s_ServerCommPackage.ADF.Data[i++] = 0;
            }
            s_SensorData.sensor_num++;
            //�����봫��������
            s_ServerCommPackage.ADF.Data[sen_num_index] = s_SensorData.sensor_num;
            
            if((g_SysInitStatusFlag == TRUE) && (g_WireLessModuleInitFlag == TRUE))   //��ʼ�����,����ģ��Ҳ��ʼ�������
            {
                //���ݳ���
                s_ServerCommPackage.Length = i + 6;
                Server_Comm_Package_Send();
            }
            else
            {
                                        
#if (SERVER_PRINTF_EN)
        printf("���������ϴ����ݰ��洢��Flash��\r\n");
#endif
        
                u8  temp_array[256];
                temp_array[0] = 0x02;                       //���ݰ�����
                temp_array[1] = (u8)s_ServerCommPackage.Length; //���ݰ�����
                memcpy(&temp_array[2], s_ServerCommPackage.ADF.Data, i);
                
                Data_Storge_Process(s_ServerCommPackage.ADF.Data, (i + 2));
            }
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_PARA:   //�ϱ�������Ϣ
        {
            i = 0;
            //����RTCʱ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);  
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime; 
            //����֮ǰ�Ĳ�ѯ������ˮ��
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageSN[0];
            s_ServerCommPackage.ADF.Data[i++] = g_RecServerPackageSN[1];
            // �����������
            s_ServerCommPackage.ADF.Data[i++] = s_SystemPara.para_type;
            switch(s_SystemPara.para_type)
            {
                case SYSTEM_PARA_TYPE_IP_ADDR_PORT: //IP��ַ����
                {  
                    // �����������
                    s_ServerCommPackage.ADF.Data[i++] = strlen((const char*)s_IPAddrPort.ip_port); 
                    //����IP��ַ
                    memcpy(&s_ServerCommPackage.ADF.Data[i], s_IPAddrPort.ip_port, strlen((const char*)s_IPAddrPort.ip_port));
                    i += strlen((const char*)s_IPAddrPort.ip_port);
                }
                break;
                
                case SYSTEM_PARA_TYPE_UPLOAD_INTERVAL:  //ͨ�������ϴ����
                {
                    // �����������
                    s_ServerCommPackage.ADF.Data[i++] = 4; 
                    //����ͨ�������ϴ����
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_UploadInterval.time1 >> 24);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_UploadInterval.time1 >> 16);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_UploadInterval.time1 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_UploadInterval.time1;
                }
                break;
                
                case SYSTEM_PARA_TYPE_HEARTBEAT_INTERVAL:   //�������
                {
                    // �����������
                    s_ServerCommPackage.ADF.Data[i++] = 2; 
                    //�����������
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_UploadInterval.heartbeat >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_UploadInterval.heartbeat;
                }
                break;
                
                case SYSTEM_PARA_TYPE_LAB_ADJUST:   //ʵ����У׼
                {
                    // �����������
                    s_ServerCommPackage.ADF.Data[i++] = 11; 
                    // ���봫��������
                    s_ServerCommPackage.ADF.Data[i++] = s_SensorAdj.type;
                    // ������Сϵ��
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_SensorAdj.multiple >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.multiple;
                    // ����K1
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.K1 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.K1;
                    // ����B1
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.B1 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.B1;
                    // ����K2
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.K2 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.K2;
                    // ����B2
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.B2 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.B2;
                }
                break;
                
                case SYSTEM_PARA_TYPE_APP_ADJUST:   //Ӧ��У׼
                {
                    // �����������
                    s_ServerCommPackage.ADF.Data[i++] = 11; 
                    // ���봫��������
                    s_ServerCommPackage.ADF.Data[i++] = s_SensorAdj.type;
                    // ������Сϵ��
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(s_SensorAdj.multiple >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.multiple;
                    // ����K3
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.K3 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.K3;
                    // ����B3
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.B3 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.B3;
                    // ����K4
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.K4 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.K4;
                    // ����B4
                    s_ServerCommPackage.ADF.Data[i++] = (u8)((u16)s_SensorAdj.B4 >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)s_SensorAdj.B4;
                }
                break;
                
                default:
                break;
            }
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_GPS:   //�ϱ�GPS
        {
            i = 0;
            //����֡�� 
            s_ServerCommPackage.ADF.Data[i++] = 1;
            //ʱ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //����
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.longitude >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.longitude >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.longitude >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.longitude;
            //γ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.latitude >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.latitude >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.latitude >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.latitude;
            //�ٶ�
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.speed >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.speed;
            //����
            s_ServerCommPackage.ADF.Data[i++] = s_GPSInfo.heading;
            //�߶�
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.high >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.high >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.high >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.high;
            //��״̬
            s_ServerCommPackage.ADF.Data[i++] = s_GPSInfo.fs;
            //����
            s_ServerCommPackage.ADF.Data[i++] = s_GPSInfo.noSV;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_REPORT_FLASH:   //�ϱ�Ƭ��Flash�����ݰ�
        {
            u8  Rx_Buffer[256];
            
//            if(g_DataPageNum > 4)   //̫���˻ᳬ���ɷ��ͻ���
//            {
//                                        
//#if (SERVER_PRINTF_EN)
//        printf("Flash�д洢��֡��=%d\r\n", g_DataPageNum);
//#endif
//        
//                return;
//            }
            
            i = 0;
            //ʱ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //����֡��     
            s_ServerCommPackage.ADF.Data[i++] = 1;
            //��ȡ���ݰ�
            SPI_FLASH_BufferRead(Rx_Buffer, (g_DataPageNum * 256), sizeof(Rx_Buffer));
            
            //�������ݰ����� 
            s_ServerCommPackage.ADF.Data[i++] = Rx_Buffer[0];
            //�������ݰ�����
            s_ServerCommPackage.ADF.Data[i++] = Rx_Buffer[1];
            //������ˮ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_ServerCommPackage.ADF.SN >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_ServerCommPackage.ADF.SN >> 0);
            //����������
            switch(Rx_Buffer[0])
            {
                case EXTFLASH_PACKAGE_TYPE_REPORT_DATA: //����洢�����ݰ��Ǵ����������ϴ�
                {
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(SERVER_COMM_PACKAGE_CMD_REPORT_DATA >> 8);
                    s_ServerCommPackage.ADF.Data[i++] = (u8)(SERVER_COMM_PACKAGE_CMD_REPORT_DATA >> 0);
                }
                break;
            }
            //������������
            for(u16 j = 0; j < Rx_Buffer[1]; j++)
            {
                s_ServerCommPackage.ADF.Data[i++] = Rx_Buffer[j + 2];
            }
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
            
            s_ServerCommTx.WaitResponse = NEED_RESPONSE;    //�ȴ�Ӧ��
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_RPT_RESET:   //�ϱ���λ
        {
            i = 0;
            //ʱ��
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 24);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 16);
            s_ServerCommPackage.ADF.Data[i++] = (u8)(s_GPSInfo.gmtTime >> 8);
            s_ServerCommPackage.ADF.Data[i++] = (u8)s_GPSInfo.gmtTime;
            //��λԭ��
            s_ServerCommPackage.ADF.Data[i++] = 0x04;
            
            s_ServerCommPackage.Length = i + 6;
            Server_Comm_Package_Send();
        }
        break;
        
        default :
        
        break;
    }
}

//********************************************************
//��������: Server_Comm_Package_Process
//��������: �������ͨ�ŵ����ݰ�������
//��    ��: u8 cmd����������, u8* data������������, u16 len�������ݳ���
//��    ��: ��
//��    ע: ��
//********************************************************
void Server_Comm_Package_Process(u16 cmd, u8* data, u16 len)
{
    u16 i;
    
    switch(cmd)
    {
        case SERVER_COMM_PACKAGE_CMD_RESPONSE:   //ͨ��Ӧ��
        {
            i = 0;
            //��ȡ��ˮ��
            u16  temp_sn = data[i++];
            temp_sn <<= 8;
            temp_sn += data[i++];
            //�ж�Ӧ����
            if(data[i++] == SUCCEED)
            {
                if(g_SysInitStatusFlag == FALSE) //�����û������
                {
                    
#if (SERVER_PRINTF_EN)
                    printf("�������\r\n");
#endif   
                    
                    g_SysInitStatusFlag = TRUE;
                }
                if(g_SysInitStatusFlag == TRUE)    //����Ѿ��������
                {      
                    if(s_ServerCommPackage.ADF.CMD == SERVER_COMM_PACKAGE_CMD_REPORT_FLASH)
                    {
                        s_ServerCommTx.WaitResponse = DONT_RESPONSE;    //�ȴ�Ӧ���־��λ
                        s_ServerCommTx.RepeatNum = 0;   //�ط�������λ
                        g_DataPageNum--;
                    }
                }
                
                
                //��������ݰ��洢��Ƭ��Flash
                if((g_DataPageNum >= SENSOR_DATA_MIN_PAGE_NUM) && (g_DataPageNum < 0xFFFF))
                {
                    
#if (SERVER_PRINTF_EN)
                    printf("g_DataPageNum=%d,��%d֡\r\n", g_DataPageNum, ((g_DataPageNum - SENSOR_DATA_MIN_PAGE_NUM) + 1));
#endif   
                    
                    Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_FLASH);
                }
            }
            else
            {
                //�ж��Ƿ�Ҫ�ط�
            }
        }
        break;
        case SERVER_COMM_PACKAGE_CMD_SET_PARA:   //���ò���
        {        
            u8 temp_len;    //��������    
            i = 0;
            //��ȡ��������
            s_SystemPara.para_type = data[i++];
            temp_len = data[i++];
            switch(s_SystemPara.para_type)
            {
                case SYSTEM_PARA_TYPE_IP_ADDR_PORT: //IP��ַ����
                {
                    memset(s_IPAddrPort.ip_port, 0, sizeof(s_IPAddrPort.ip_port));
                    memcpy(s_IPAddrPort.ip_port, &data[i], temp_len); //��ȡIP��ַ
                    
                    
#if (SERVER_PRINTF_EN)
                    printf("���õ�IP��ַ=%s\r\n", s_IPAddrPort.ip_port);
#endif   
                    
                    //���豸�·�IP��ַ
                    Device_Printf_Ctr(DEVICE_SET_IP_ADDR_CMD);
                    Delay_ms(500);
                    // ��ȡIP�˿�
                    Device_Printf_Ctr(DEVICE_READ_IP_ADDR_CMD);
                    
                    //��λ����ģ���ʼ��״̬��������ʼ��״̬����������ģ�����³�ʼ��
                    g_WireLessModuleInitFlag = FALSE;
                    g_SysInitStatusFlag = FALSE;
                    
                    //����ģ�����³�ʼ��
//                    WireLess_Initial();
                    
//                    //����IP�Ͷ˿ڵ�����ģ����
//                    //���˳�͸��ģʽ��������ģʽ
//                    if(WireLess_AT_Command_Ctr(AT_COMMAND_SWITCH_CMD) == FAILURE)
//                    {
//                        g_RecServerPackageResult = RES_FAILURE;
//                        break;
//                    }
//                    //�ر�����
//                    if(WireLess_AT_Command_Ctr(AT_COMMAND_QICLOSE) == FAILURE)
//                    {
//                        g_RecServerPackageResult = RES_FAILURE;
//                        break;
//                    }
//                    //��������
//                    if(WireLess_AT_Command_Ctr(AT_COMMAND_QIOPEN) == FAILURE)
//                    {
//                        g_RecServerPackageResult = RES_FAILURE;
//                        break;
//                    }
                    g_RecServerPackageResult = RES_SUCCEED;
                }
                break;
                
                case SYSTEM_PARA_TYPE_TIME_SYNCH: //ϵͳʱ��ͬ��
                {
                    s_GPSInfo.gmtTime = 0;
                    while(temp_len--)
                    {
                        s_GPSInfo.gmtTime <<= 8;
                        s_GPSInfo.gmtTime += data[i++];
                    }
                    g_RecServerPackageResult = RES_SUCCEED;
                }
                break;
                
                case SYSTEM_PARA_TYPE_UPLOAD_INTERVAL:  //ͨ�������ϴ����
                {
                    s_UploadInterval.time1 = data[i++];
                    s_UploadInterval.time1 <<= 8;
                    s_UploadInterval.time1 += data[i++];
                    s_UploadInterval.time1 <<= 8;
                    s_UploadInterval.time1 += data[i++];
                    s_UploadInterval.time1 <<= 8;
                    s_UploadInterval.time1 += data[i++];
                    
#if (SERVER_PRINTF_EN)
                    printf("s_UploadInterval.time1=%d\r\n", s_UploadInterval.time1);
#endif   
                    
                    if(Device_Printf_Ctr(DEVICE_SET_UPLOAD_INTERVAL_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_HEARTBEAT_INTERVAL:   //�������
                {
                    s_UploadInterval.heartbeat = data[i++];
                    s_UploadInterval.heartbeat <<= 8;
                    s_UploadInterval.heartbeat += data[i++];
                    
                    if(Device_Printf_Ctr(DEVICE_SET_HEARTBEAT_INTERVAL_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_LAB_ADJUST:   //ʵ����У׼
                {
                    u16 temp_data = 0;
                    
                    s_SensorAdj.type = data[i++];
                    
                    s_SensorAdj.multiple = data[i++]; 
                    s_SensorAdj.multiple <<= 8;
                    s_SensorAdj.multiple += data[i++];
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K1 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B1 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K2 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B2 = (s16)temp_data;
                    
                    if(Device_Printf_Ctr(DEVICE_SET_SENSOR_LAB_ADJUST_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_APP_ADJUST:   //Ӧ��У׼
                {
                    u16 temp_data = 0;
                    
                    s_SensorAdj.type = data[i++];
                    
                    s_SensorAdj.multiple = data[i++]; 
                    s_SensorAdj.multiple <<= 8;
                    s_SensorAdj.multiple += data[i++];
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K3 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B3 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K4 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B4 = (s16)temp_data;
                    
                    if(Device_Printf_Ctr(DEVICE_SET_SENSOR_APP_ADJUST_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_BATCH_APP_ADJUST:   //����Ӧ��У׼
                {
                    u16 temp_data = 0;
                    
                    s_SensorAdj.type = data[i++];
                    
                    s_SensorAdj.multiple = data[i++]; 
                    s_SensorAdj.multiple <<= 8;
                    s_SensorAdj.multiple += data[i++];
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K3 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B3 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K4 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B4 = (s16)temp_data;
                    
                    if(Device_Printf_Ctr(DEVICE_SET_SENSOR_APP_ADJUST_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                case SYSTEM_PARA_TYPE_BATCH_LAB_ADJUST:   //����ʵ����У׼
                {
                    u16 temp_data = 0;
                    
                    s_SensorAdj.type = data[i++];
                    
                    s_SensorAdj.multiple = data[i++]; 
                    s_SensorAdj.multiple <<= 8;
                    s_SensorAdj.multiple += data[i++];
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K1 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B1 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.K2 = (s16)temp_data;
                    
                    temp_data = data[i++];
                    temp_data <<= 8;
                    temp_data += data[i++];
                    s_SensorAdj.B2 = (s16)temp_data;
                    
                    if(Device_Printf_Ctr(DEVICE_SET_SENSOR_LAB_ADJUST_CMD) == SUCCEED)
                    {
                        g_RecServerPackageResult = RES_SUCCEED;
                    }
                    else
                    {
                        g_RecServerPackageResult = RES_FAILURE;
                    }
                }
                break;
                
                default:
                break;
            }
            //ͨ��Ӧ��
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RESPONSE);
            Delay_ms(500);  //��ʱһ��ʱ�䣬����������������ģ�鵼�·���ʧ��
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_READ_PARA:   //��ȡ��Ϣ
        {
            //��ͨ��Ӧ��
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RESPONSE);
            i = 0;
            //��ȡ��������
            s_SystemPara.para_type = data[i++];
            Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_REPORT_PARA);
        }
        break;
        
        case SERVER_COMM_PACKAGE_CMD_CTR_RESET:   //���Ƹ�λ
        {            
            if(Device_Printf_Ctr(DEVICE_CTR_RESET_CMD) == SUCCEED)
            {
                g_RecServerPackageResult = RES_SUCCEED;
                //ͨ��Ӧ��
                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RESPONSE);
                Delay_ms(500);  //��ֹ����
                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RPT_RESET);
                Delay_ms(500);  //��ʱһ��ʱ�䣬����������������ģ�鵼�·���ʧ��
                
                __set_FAULTMASK(1);      // �ر������ж�
                NVIC_SystemReset();     // ��λ
            }
            else
            {
                g_RecServerPackageResult = RES_FAILURE;
                Server_Comm_Package_Bale(SERVER_COMM_PACKAGE_CMD_RESPONSE);
            }
        }
        break;
        
        default :
        
        break;
    }
}

//********************************************************
//��������: Server_Comm_Package_Analysis
//��������: �������ͨ�ŵ����ݰ���������
//��    ��: u8 *rec_array������������, u16 rec_length�������ݳ���
//��    ��: ���ؽ���״̬
//��    ע: ��
//********************************************************
u8 Server_Comm_Package_Analysis(u8 *rec_array, u16 rec_length)
{
	u16 i = 0;
    u8  temp_data = 0;
    u16 temp_index = 0;        //��������
	u8  data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_HEAD;	    //���ݷ���״̬
    u8  packet_analysis_error_status = PACKAGE_ANALYSIS_SUCCEED;    //���ݰ���������״̬
    
    ServerCommPackageStruct *p_Package = (ServerCommPackageStruct*)malloc(sizeof(ServerCommPackageStruct));
    if(p_Package == NULL) 
    {
                 
#if (SERVER_PRINTF_EN)
        printf("�ڴ�ռ䲻�㣬�޷�����������\r\n");
#endif	
        
        return PACKAGE_ANALYSIS_UNKNOWN_ERROR;
    }
         
#if (SERVER_PRINTF_EN)
    printf("���յ�����������: ");
#endif	
    
	for(temp_index = 0; temp_index < rec_length; temp_index++)        //
	{
		temp_data = rec_array[temp_index];  //ȡ�����ջ���������
        
#if (SERVER_PRINTF_EN)
        printf("%02X ",temp_data);  // 
#endif

		switch(data_analysis_status)    //���ݲ���״̬����
		{
			case SERVER_COMM_PACKAGE_ANALYSIS_HEAD:	    //����ǰ�ͷ
            {
                if(temp_data == SERVER_COMM_PACKAGE_HEAD)   //����ǰ�ͷ
                {   
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_IDENT; //����״̬תΪ���ձ�ʶ��
                }
                else
                {
                    packet_analysis_error_status = PACKAGE_ANALYSIS_HEAD_ERROR; //��ͷ����
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_IDENT:	        //����Ǳ�ʶ��
            {
                if(temp_data == SERVER_COMM_PACKAGE_IDENTIFY)   //����Ǳ�ʶ��
                {   
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_LEN; //����״̬תΪ�������ݳ���
                    i = 0;
                    p_Package->Length = 0;
                }
                else
                {
                    packet_analysis_error_status = PACKAGE_ANALYSIS_IDENT_ERROR; //��ʶ������
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_LEN:    //��������ݳ���
            {
                p_Package->Length += temp_data; 
                i++;
                if(i == 2)
                {     
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_SN; //����״̬תΪ������������ˮ��
                    i = 0;
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_SN:          //�������ˮ��
            {
                g_RecServerPackageSN[i++] = temp_data; 
                if(i == 2)
                {  
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_CMD; //����״̬תΪ����������������
                    i = 0;
                    p_Package->ADF.CMD = 0;
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_CMD:          //�����������
            {
                p_Package->ADF.CMD <<= (i * 8);
                p_Package->ADF.CMD += temp_data; 
                i++;
                if(i == 2)
                {     
                    if(p_Package->Length == 6)      //û���������ݵ����ݰ�
                    {
                        data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_CRC; //����״̬תΪУ����
                        p_Package->ADF.Crc = 0;
                    }
                    else
                    {
                        data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_DATA; //����״̬תΪ������������������
                    }
                    i = 0;
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_DATA:          //�������������
            {
                p_Package->ADF.Data[i++] = temp_data;      //��ȡ��������  
                if(i == (p_Package->Length - 6)) //���ճ��ȹ���
                {   
                    data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_CRC; //����״̬תΪУ����
                    p_Package->ADF.Crc = 0;
                    i = 0;
                }
            }
            break;
            
			case SERVER_COMM_PACKAGE_ANALYSIS_CRC:          //�����У����
            {
                p_Package->ADF.Crc <<= (i * 8);
                p_Package->ADF.Crc += temp_data; 
                i++;
                if(i == 2)
                {
                    i = 0;
                    if(p_Package->ADF.Crc == GetCheckCRC_XW(&rec_array[temp_index - p_Package->Length + 1], (p_Package->Length - 2)))  //����У�������
                    {      
                        
#if (SERVER_PRINTF_EN)
                        printf("���ݰ������ɹ�\r\n");
#endif
                        
                        g_RecServerPackageResult = SUCCEED; //���յ����ݰ������ɹ�
                        //��ʼ������յ������ݰ�
                        Server_Comm_Package_Process(p_Package->ADF.CMD, p_Package->ADF.Data, (p_Package->Length - 6));
                        
                        data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_HEAD;	//״̬תΪ���Ұ�ͷ
                    }
                    else        //����У�������
                    {
                        packet_analysis_error_status = PACKAGE_ANALYSIS_CRC_ERROR; //У�������
                        //�ͷſռ�
                        free(p_Package); 
                        p_Package = NULL;
                        
                        return packet_analysis_error_status;
                    }
                }
            }
            break;
            
			default:
            {
                data_analysis_status = SERVER_COMM_PACKAGE_ANALYSIS_HEAD;	//״̬תΪ���Ұ�ͷ
                packet_analysis_error_status = PACKAGE_ANALYSIS_UNKNOWN_ERROR;
            }
            break;
		}
	}
    
    //�������ʧ��
    if(packet_analysis_error_status != PACKAGE_ANALYSIS_SUCCEED)
    {
        char com_str[50];  //�����ַ���
        char const temp_str[] = "NO CARRIE";
        
        for(temp_index = 0; temp_index < rec_length; temp_index++)
        {
            //"NO CARRIER"
            memcpy(com_str, &rec_array[temp_index], strlen(temp_str));
            com_str[strlen(temp_str)] = '\0';  //���������
            if(strcmp(com_str, temp_str) == SUCCEED)
            {
                //����ģ�鸴λΪδ��ʼ��
                g_WireLessModuleInitFlag = FALSE;
                //ϵͳ��ʼ��״̬��λΪδ��ʼ��
                g_SysInitStatusFlag = FALSE;
            }
        }
    }
    //�ͷſռ�
    free(p_Package); 
    p_Package = NULL;
                        
    return packet_analysis_error_status;
}

//********************************************************
//��������: Server_Comm_Rec_Monitor
//��������: �������ͨ�ŵļ�⺯��
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void Server_Comm_Rec_Monitor(void)
{
    if(s_ServerCommRx.Status == TRUE)   //�н���
    {
        if(s_ServerCommRx.Timeout_Count >= SERVER_COMM_RX_DATA_TIMEOUT)    //�Ƿ�ʱ��
        {
            u16 temp_l = s_ServerCommRx.Index;    //���������ݳ���
            //�����ݿ�������������������ֹ���µ�������û
            memcpy(g_PublicDataBuffer, s_ServerCommRx.Buffer, temp_l); 
            
            s_ServerCommRx.Status = FALSE;
            s_ServerCommRx.Index = 0;
            s_ServerCommRx.Timeout_Count = 0;
            
            //�������ݰ�������ɹ����򷵻ضԷ�����
            if(Server_Comm_Package_Analysis(g_PublicDataBuffer, temp_l) != PACKAGE_ANALYSIS_SUCCEED)   //u8 i = 
            {
                //                  Server_Comm_Error_Process();
            }
            
            //#if (SERVER_PRINTF_EN)
            //printf("\r\n");
            //printf("���ݰ����� %d\r\n", i);
            //#endif
            
        }
    }
    //����ܳ�ʱ��û�н��յ������������ݣ�˵��������Ҫô����ػ��ˣ�Ҫô�����߳������ˣ������½����ʼ������
    else if(s_ServerCommRx.Timeout_Count >= SERVER_COMM_NO_DATA_REC_TIMEOUT)
    {
        s_ServerCommRx.Timeout_Count = 0;
        g_SysInitStatusFlag = FALSE;
    }
}

//********************************************************
//��������: Server_Comm_Error_Process
//��������: �������ͨ�ŵĴ�������
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void Server_Comm_Error_Process(void)
{
    u8 temp_status = s_ServerCommTx.WaitResponse;   //����ȴ�Ӧ��״̬
    
    s_ServerCommTx.WaitResponse = DONT_RESPONSE;    //�ȴ�Ӧ���־��λ
    
    s_ServerCommPackage.ADF.Data[0] = FAILURE;
    s_ServerCommPackage.Length = 9;     //�������ݳ���
    
    Server_Comm_Package_Send();
    
    s_ServerCommTx.WaitResponse = temp_status;   //�ָ��ȴ�Ӧ��״̬
}

/*****************************************
//����: Server_Comm_Test
//����: �������ͨ�ŵĲ��Ժ���
//���: ��
//����: ��
******************************************/
void Server_Comm_Test(void)
{  
    if(s_ServerCommRx.Status == TRUE) //����н���
    {
        if(s_ServerCommRx.Timeout_Count > SERVER_COMM_RX_DATA_TIMEOUT)        //�����ʱû���յ������ˣ�˵���������
        {
            Server_Comm_Send_Data(s_ServerCommRx.Buffer, s_ServerCommRx.Index);
            
            s_ServerCommRx.Status = FALSE;
            s_ServerCommRx.Timeout_Count = 0;
            s_ServerCommRx.Index = 0;
            
            //            Delay_ms(500);
        }
    }
}


//////////////////////////////END OF C�ļ�////////////////////////////////////
