/*******************************************************************************
// Copyright(c)2017, ������ھ�����Ϣ�Ƽ��ɷ����޹�˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language��C)
// Date   : 2017-04-01
// Author : ����
// Functiom: ��PC�����ع̼�����Ƭ����Դ�ļ�
*******************************************************************************/ 


/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//��������
*******************************************************************************/
extern uint8_t file_name[FILE_NAME_LENGTH];
uint8_t tab_1024[1024] =
{
    0
};                                                        //����1K�Ļ���������

// ����ʱӦ�ó���洢����ȡ��������ת�浽ʵ��Ӧ�ó���洢���Ļ���ռ�
int32_t AppSpaceBuf[(PAGE_SIZE / 4)] = {0};



/******************************************************************************
//��������
*******************************************************************************/



/******************************************************************************
//��������
*******************************************************************************/
//********************************************************
//��������: SerialDownload
//��������: ͨ����������һ���ļ�����
//��    ��: ��
//��    ��: ��
//��    ע: 
//********************************************************
void SerialDownload(void)
{
    uint8_t Number[10] = "          ";
    int32_t Size = 0;
    
    SerialPutString("�ȴ������ļ� ... (�� 'a' ȡ��)\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);
    if (Size > 0)
    {
        SerialPutString("\n\n\r��̳ɹ�!\r\n Name: ");
        SerialPutString(file_name);
        Int2Str(Number, Size);
        Serial_PutString("\n\rSize: ");
        Serial_PutString(Number);
        Serial_PutString("Bytes\r\n");
        
        //�����û�Ӧ�ó����ַ
        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
        pFunction Jump_To_Application = (pFunction) JumpAddress;
        
        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//��ʼ���û�Ӧ�ó���Ķ�ջָ��
        
        Jump_To_Application();                          //��ת���û�Ӧ�ó���
    }
//    else if (Size == -1)
//    {
//        //�����ļ���������Ĵ洢�ռ�
//        SerialPutString("\n\n\r���س���Ĵ�С���ڿ��õĴ洢�ռ�!\n\r");
//        
//        //�����û�Ӧ�ó����ַ
//        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//        pFunction Jump_To_Application = (pFunction) JumpAddress;
//        
//        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//��ʼ���û�Ӧ�ó���Ķ�ջָ��
//        
//        Jump_To_Application();                          //��ת���û�Ӧ�ó���
//    }
//    else if (Size == -2)
//    {
//        //У��ʧ��
//        SerialPutString("\n\n\rУ��ʧ��!\n\r");
//        
//        //�����û�Ӧ�ó����ַ
//        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//        pFunction Jump_To_Application = (pFunction) JumpAddress;
//        
//        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//��ʼ���û�Ӧ�ó���Ķ�ջָ��
//        
//        Jump_To_Application();                          //��ת���û�Ӧ�ó���
//    }
//    else if (Size == -3)
//    {
//        //�û���ֹ
//        SerialPutString("\r\n\n�û�ȡ��!\n\r");
//        
//        //�����û�Ӧ�ó����ַ
//        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//        pFunction Jump_To_Application = (pFunction) JumpAddress;
//        
//        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//��ʼ���û�Ӧ�ó���Ķ�ջָ��
//        
//        Jump_To_Application();                          //��ת���û�Ӧ�ó���
//    }
//    else if (Size == -4)
//    {
//        //��ʱ��λ��û���������������˳�
//        SerialPutString("\r\n\n��ʱ�˳�����!\n\r");
//        
//        //�����û�Ӧ�ó����ַ
//        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//        pFunction Jump_To_Application = (pFunction) JumpAddress;
//        
//        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//��ʼ���û�Ӧ�ó���Ķ�ջָ��
//        
//        Jump_To_Application();                          //��ת���û�Ӧ�ó���
//    }
    else if (Size == -5)
    {
        //��ʱ��λ��û���������������˳�
        SerialPutString("\r\n\n����ʱ�洢��������ʵ�ʴ洢��ʱ����!\n\r");
    }
    else
    {
        // ���ж��Ƿ���Ӧ�ó���
        AppSpaceBuf[0] = *((uint32_t*)ApplicationAddress); //������
        if((AppSpaceBuf[0] != 0xFFFFFFFF) && (AppSpaceBuf[0] != 0)) //��Ϊ�գ���˵����Ӧ�ó���
        {
            //�����û�Ӧ�ó����ַ
            uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
            pFunction Jump_To_Application = (pFunction) JumpAddress;
            
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);//��ʼ���û�Ӧ�ó���Ķ�ջָ��
            
            Jump_To_Application();                          //��ת���û�Ӧ�ó���
        }
    }//end of if (Size > 0)
}


//////////////////////////////END OF C�ļ�////////////////////////////////////
