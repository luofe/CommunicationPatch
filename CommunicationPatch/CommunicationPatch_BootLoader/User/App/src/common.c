/*******************************************************************************
// Copyright(c)2017, ������ھ�����Ϣ�Ƽ��ɷ����޹�˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language��C)
// Date   : 2017-04-01
// Author : ����
// Functiom: ��PC��ͨ����ص�Դ�ļ�
*******************************************************************************/


/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/
pFunction       Jump_To_Application;                      //����Ӧ�ó�����ָ�����
uint32_t        JumpAddress;                              //����������ת��ַ����
uint32_t        BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t   FlashProtection = 0;
extern uint32_t FlashDestination;

// �����ļ��Ѿ�д��Flash�ڵı�־
u8  g_ProgramFlashFileRecFlag = 0;


/******************************************************************************
//��������
*******************************************************************************/




/******************************************************************************
//��������
*******************************************************************************/
//
//��дputchar��������ϵͳ�⺯�����ã�
//
//********************************************************
int  putchar(int ch)
{
    USART_SendData(USE_USART, ch);                        //ͨ��USART���һ���ַ�
    while(USART_GetFlagStatus(USE_USART, USART_FLAG_TXE) == RESET);
                                                          //�ȴ��������

    return ch;
}


//********************************************************
//��������: Int2Str
//��������: ��һ������ת��Ϊ�ַ�������
//��    ��: 
//         *str: uint8�����ת�����ַ�����ָ�룻
//         intnum: int32_t����Ҫ��ת����������
//��    ��: ��
//��    ע: 
//********************************************************
void Int2Str(uint8_t* str, int32_t intnum)
{
    uint32_t i, Div = 1000000000, j = 0, Status = 0;

    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + 48;

        intnum = intnum % Div;
        Div /= 10;
        if ((str[j-1] == '0') & (Status == 0))
        {
            j = 0;
        }
        else
        {
            Status++;
        }//end of if ((str[j-1] == '0') & (Status == 0))
    }//end of for (i = 0; i < 10; i++)
}


//********************************************************
//��������: Str2Int
//��������: ��һ���ַ���ת��Ϊ���κ���
//��    ��: 
//         *inputstr: uint8����Ҫ��ת�����ַ���ָ�룻
//         intnum: int32_t�����ת��������ֵ��ָ�룻
//��    ��: 1: ��ȷ��0: ����
//��    ע: 
//********************************************************
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
    uint32_t i = 0, res = 0;
    uint32_t val = 0;
    
    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
    {
        if (inputstr[2] == '\0')
        {
            return 0;
        }
        for (i = 2; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1; */
                res = 1;
                break;
            }
            if (ISVALIDHEX(inputstr[i]))
            {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* over 8 digit hex --invalid */
        if (i >= 11)
        {
          res = 0;
        }
    }
    else /* max 10-digit decimal input */
    {
        for (i = 0;i < 11;i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1 */
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
            {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
            {
                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            }
            else if (ISVALIDDEC(inputstr[i]))
            {
                val = val * 10 + CONVERTDEC(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* Over 10 digit decimal --invalid */
        if (i >= 11)
        {
            res = 0;
        }
    }
    
    return res;
}


//********************************************************
//��������: GetIntegerInput
//��������: �ӳ����ն˻�ȡһ����������
//��    ��: 
//         *num: int32_t���������ֵ��ָ�룻
//��    ��: 1: ��ȷ��0: ����
//��    ע: 
//********************************************************
uint32_t GetIntegerInput(int32_t * num)
{
    uint8_t inputstr[16];

    while (1)
    {
        GetInputString(inputstr);
        if (inputstr[0] == '\0')
            continue;
        if ((inputstr[0] == 'a' || inputstr[0] == 'A') && inputstr[1] == '\0')
        {
            SerialPutString("�û�ȡ����\r\n");
            return 0;
        }
        
        if (Str2Int(inputstr, num) == 0)
        {
            SerialPutString("����, ����������: \r\n");
        }
        else
        {
            return 1;
        }
    }//end of while (1)
}


//********************************************************
//��������: SerialKeyPressed
//��������: ���Գ����ն��Ƿ��а������º���
//��    ��: 
//         *key: uint8_t����ų����ն˰�����ֵ��ָ�룻
//��    ��: 1: ��ȷ��0: ����
//��    ע: 
//********************************************************
uint32_t SerialKeyPressed(uint8_t *key)
{

    if( USART_GetFlagStatus(USE_USART, USART_FLAG_RXNE) != RESET)
    {
        *key = (uint8_t)USE_USART->DR;
        return 1;
    }
    else
    {
        return 0;
    }
}


//********************************************************
//��������: GetKey
//��������: �ӳ����ն˻�ȡһ����ֵ����
//��    ��: ��
//��    ��: uint8_t,��ȡ�ļ�ֵ��
//��    ע: 
//********************************************************
uint8_t GetKey(void)
{
    uint8_t key = 0;

     //�ȴ��û�����
     while (1)
     {
         if (SerialKeyPressed((uint8_t*)&key)) break;
     }

     return key;
}


//********************************************************
//��������: SerialPutChar
//��������: ��ӡһ���ַ����������ն˺���
//��    ��: c��uint8_t����Ҫ��ӡ�������ն˵��ַ���
//��    ��: ��
//��    ע: 
//********************************************************
void SerialPutChar(uint8_t c)
{
    USART_SendData(USE_USART, c);
    while (USART_GetFlagStatus(USE_USART, USART_FLAG_TXE) == RESET)
    {
    }
}


//********************************************************
//��������: Serial_PutString
//��������: ��ӡ�ַ����������ն˺���
//��    ��: *s��uint8_t����Ҫ��ӡ�������ն˵��ַ���ָ�룻
//��    ��: ��
//��    ע: 
//********************************************************
void Serial_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
      SerialPutChar(*s);
      s++;
    }
}


//********************************************************
//��������: GetInputString
//��������: �ӳ����ն˶�ȡһ���ַ�������
//��    ��: *buffP��uint8_t����Ŷ�ȡ�ַ�����ָ�룻
//��    ��: ��
//��    ע: 
//********************************************************
void GetInputString (uint8_t * buffP)
{
    uint32_t bytes_read = 0;
    uint8_t  c = 0;
    
    do
    {
        c = GetKey();
        if (c == '\r')
          break;
        if (c == '\b') /* Backspace */
        {
            if (bytes_read > 0)
            {
              SerialPutString("\b \b");
              bytes_read --;
            }
            continue;
        }
        if (bytes_read >= CMD_STRING_SIZE )
        {
            SerialPutString("�����ַ�����С�����\r\n");
            bytes_read = 0;
            continue;
        }
        if (c >= 0x20 && c <= 0x7E)
        {
            buffP[bytes_read++] = c;
            SerialPutChar(c);
        }
    }
    while (1);
    SerialPutString(("\n\r"));
    buffP[bytes_read] = '\0';
}


//********************************************************
//��������: FLASH_PagesMask
//��������: ����ҳ������
//��    ��: 
//          Size: uint32_t������Ĵ�С��
//��    ��: uint32_t�������ҳ����
//��    ע: 
//********************************************************
uint32_t FLASH_PagesMask(__IO uint32_t Size)
{
    uint32_t pagenumber = 0x0;
    uint32_t size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    
    return pagenumber;
}


//********************************************************
//��������: FLASH_DisableWriteProtectionPages
//��������: ��ֹ����FLASHҳ��д��������
//��    ��: ��
//��    ��: ��
//��    ע: 
//********************************************************
void FLASH_DisableWriteProtectionPages(void)
{
    uint32_t useroptionbyte = 0, WRPR = 0;
    uint16_t var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
    FLASH_Status status = FLASH_BUSY;

    WRPR = FLASH_GetWriteProtectionOptionByte();


    //�ж��û��洢�ռ��Ƿ����д����
    if ((WRPR & UserMemoryMask) != UserMemoryMask)
    {
        useroptionbyte = FLASH_GetUserOptionByte();

        UserMemoryMask |= WRPR;

        status = FLASH_EraseOptionBytes();

        if (UserMemoryMask != 0xFFFFFFFF)
        {
            status = FLASH_EnableWriteProtection((uint32_t)~UserMemoryMask);
        }

        //�ж��û��������Ƿ񱻱��
        if ((useroptionbyte & 0x07) != 0x07)
        { 
            //�ָ��û������ֽ�
            if ((useroptionbyte & 0x01) == 0x0)
            {
                var1 = OB_IWDG_HW;
            }
            if ((useroptionbyte & 0x02) == 0x0)
            {
                var2 = OB_STOP_RST;
            }
            if ((useroptionbyte & 0x04) == 0x0)
            {
                var3 = OB_STDBY_RST;
            }

            FLASH_UserOptionByteConfig(var1, var2, var3);
        }
        
        if (status == FLASH_COMPLETE)
        {
            SerialPutString("��ֹд����...\r\n");
            SerialPutString("...ϵͳ�����и�λ�������µĲ�����\r\n");

            //����ϵͳ��λ�������µĲ�����ֵ
            NVIC_SystemReset();
        }
        else
        {
            SerialPutString("����: Flash���д����ʧ��...\r\n");
        }
    }
    else
    {
        SerialPutString("Flash�洢�ռ�δд����...\r\n");
    }
}


//********************************************************
//��������: Main_Menu
//��������: �ڳ����ն�����ʾ���˵�����
//��    ��: ��
//��    ��: ��
//��    ע: 
//********************************************************
void Main_Menu(void)
{
//    uint8_t key = 0;
    
    /* Get the number of block (4 or 2 pages) from where the user program will be loaded */
    BlockNbr = (FlashDestination - 0x08000000) >> 12;
    
    /* Compute the mask to test if the Flash memory, where the user program will be
       loaded, is write protected */
#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
    UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
#else
    if (BlockNbr < 62)
    {
        UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
    }
    else
    {
        UserMemoryMask = ((uint32_t)0x80000000);
    }
#endif


    /*
     * ���Ӧ�ó���Ҫ�������Flash�洢����ҳ�Ƿ�Ϊд����
     */
    if ((FLASH_GetWriteProtectionOptionByte() & UserMemoryMask) != UserMemoryMask)
    {
        FlashProtection = 1;
    }
    else
    {
        FlashProtection = 0;
    }


    while (1)
    {
        /*
         * ��ӡ�˵���Ϣ
         */
        SerialPutString("\r\n����Ƿ����������־\r\n\n");

        SerialPutString("1     -----------------����Ӧ�ó���\r\n\n");
        SerialPutString("0/FFFF-----------------����Ӧ�ó���\r\n\n");
        if(FlashProtection != 0)
        {
            //�������FLASHҳд���������д�����˵�ѡ��
            SerialPutString("3   ��ֹд����\r\n\n");
        }//end of if(FlashProtection != 0)
        
        SerialDownload();
    }//end of while (1)
}


//////////////////////////////END OF C�ļ�////////////////////////////////////
