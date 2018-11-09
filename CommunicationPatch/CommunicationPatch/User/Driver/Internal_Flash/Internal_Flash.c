
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:��Ŀ����ʹ���ڲ�Flash�洢���������Դ����
//��ע: 
1������������2K/ҳ����������С��������1K/ҳ��
2��������������256ҳ��������������128ҳ��С����������32ҳ��

//��Ҫ�洢����: 	
1���¶Ȳ��Դ洢��־ + ���������� + ������1�¶ȿ��� + ������1�¶�ֵ + ������2...... + ѹ������Ϣʱ�� + ѹ��������ʱ��
2�����ִ����־����־: ��������һЩ��ÿһ��������ռ��2�������ֽ�

*******************************************************************************/

/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/
//�¶Ȳ����Ƿ�洢�ı�־
u8 SysTempInfoStoredFlag = FALSE;

//�̼�������־
u8 SygProgramUpgradeFlag = FALSE;

// �洢����ʼ��ַ�ͽ�����ַ
u32  g_InternalFlashStartAddr = DATA_EEPROM_START_ADDR;













/******************************************************************************
//��������
*******************************************************************************/
//����:	��Ƭ��Ƭ��Flash�洢��2�ֽڶ�ȡ
void Internal_Flash_Read_HalfWord(u32 addr, u16* buffer, u16 length);

//����:	��Ƭ��Ƭ��Flash�洢��2�ֽ�д��
u8 Internal_Flash_Write_HalfWord(u32 addr, u16 *buffer, u16 length);

//����:	��ȡƬ��Flash������
void Internal_Flash_ReadOut(void);

//����:	�����ݽṹ����Flash
void Internal_Flash_StoreIn(u8 store_param_type);

//����:	��ȡƬ��Flash���Ƿ�̼������ı�־
void Internal_Flash_Upgrade_ReadOut(void);

//����:	�洢�Ƿ�̼������ı�־��Ƭ��Flash
void Internal_Flash_Upgrade_StoreIn(void);

//����:	��Ƭ��Ƭ��Flash�洢���� 
void Internal_Flash_Test(void);














/******************************************************************************
//��������
*******************************************************************************/
/**********************************
//����:	Internal_Flash_Read_HalfWord
//����:	��Ƭ��Ƭ��Flash�洢��2�ֽڶ�ȡ
//��ڲ���:	u32 addr������ȡ���ݵ���ʼ��ַ��uint8 *buffer������Ŷ�ȡ���ݵ�ָ�룻u16 length������ȡ���ݳ��ȣ�
//���ڲ���:	��
**********************************/ 
void Internal_Flash_Read_HalfWord(u32 addr, u16* buffer, u16 length)
{
    volatile u16 i = 0;
    addr = g_InternalFlashStartAddr + addr;	//��ַָ��ָ���ȡ��ַ
    
    for (i = 0; i < length; i++)
    {
        buffer[i] = *((u16*)addr); //������
        addr += 2;  
    }
}

/**********************************
//����:	Internal_Flash_Write_HalfWord
//����:	��Ƭ��Ƭ��Flash�洢��2�ֽ�д��
//��ڲ���:	
	u32 addr����д���ݵ���ʼ��ַ��u8 *buffer����д�����ݵ�ָ�룻u16 length����д�����ݳ��ȣ�
//���ڲ���:	u8 �ɹ�����ʧ��
**********************************/ 
u8 Internal_Flash_Write_HalfWord(u32 addr, u16 *buffer, u16 length)
{
    volatile u16 i = 0;
    FLASH_Status sta;
    
    addr = (u32)(addr + g_InternalFlashStartAddr);//��ַָ��ָ���ȡ��ַ

    FLASH_Unlock();            //���������ɲ���
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//���־λ
    
    sta = FLASH_ErasePage(g_InternalFlashStartAddr); //����255ҳ
    if(FLASH_COMPLETE != sta)         
    {
        FLASH_Lock();
        return FAILURE; 
    }
    
    for(i = 0; i < length; i++)              //д������
    {     
        sta = FLASH_ProgramHalfWord(addr, buffer[i]);
        if (FLASH_COMPLETE != sta)
        {
            FLASH_Lock();
            return FAILURE;
        }  
        
        addr += 2;               //16λ���ݣ���ַ��2
    }
    
    FLASH_Lock();                     //����
    
    return SUCCEED;
                
}	

/**********************************
//����:	Internal_Flash_ReadOut
//����:	��ȡƬ��Flash������
//��ڲ���:	��
//���ڲ���:	��
**********************************/ 
void Internal_Flash_ReadOut(void)
{        
    
}

/**********************************
//����:	Internal_Flash_StoreIn
//����:	�����ݽṹ����Flash
//��ڲ���:	u8 store_param_type�����洢�������ͣ��¶Ȼ����豸���
//���ڲ���:	��
**********************************/ 
void Internal_Flash_StoreIn(u8 store_param_type)
{      
    
}

/**********************************
//����:	Internal_Flash_Upgrade_ReadOut
//����:	��ȡƬ��Flash���Ƿ�̼������ı�־
//��ڲ���:	��
//���ڲ���:	��
**********************************/ 
void Internal_Flash_Upgrade_ReadOut(void)
{        
    //�洢��Flash�ڵ���������
    u16 temp_array[1];

    g_InternalFlashStartAddr = INTERNAL_FLASH_START_ADDR;
    
    //��ȡ����������
    Internal_Flash_Read_HalfWord(FLASH_ADD_PROGRAM_UPGRADE_FLAG, temp_array, 1); 
    
    SygProgramUpgradeFlag = temp_array[0]; 
}

/**********************************
//����:	Internal_Flash_Upgrade_StoreIn
//����:	�洢�Ƿ�̼������ı�־��Ƭ��Flash
//��ڲ���:	��
//���ڲ���:	��
**********************************/ 
void Internal_Flash_Upgrade_StoreIn(void)
{      
    u8  i = 10;
    u16 temp_array[1];

    g_InternalFlashStartAddr = INTERNAL_FLASH_START_ADDR;
    
    temp_array[0] = SygProgramUpgradeFlag;
    
    //�����¶Ȳ�������
    while(i--)
    {
        if(Internal_Flash_Write_HalfWord(FLASH_ADD_PROGRAM_UPGRADE_FLAG, temp_array, 1) == SUCCEED)
        {
            break;
        }
    }
}

/**********************************
//����:	Internal_Flash_Test
//����:	��Ƭ��Ƭ��Flash�洢����
//��ڲ���:	��
//���ڲ���:	��
**********************************/ 
void Internal_Flash_Test(void)
{
    u16 i = 0, j = 18;
    
    u16 Internal_Flash_Data[INTERNAL_FLASH_MAX_LENGTH] = {0};

    g_InternalFlashStartAddr = DATA_EEPROM_START_ADDR;
        
    Internal_Flash_Read_HalfWord(FLASH_ADD_TEMPERATURE_STORE_INFO, Internal_Flash_Data, INTERNAL_FLASH_MAX_LENGTH); 
        
#if (SERVER_PRINTF_EN)
printf("��ȡ��������:\r\n");
for(u16 x = 0; x < INTERNAL_FLASH_MAX_LENGTH; x++)
{
    printf("0x%02X", (u8)(Internal_Flash_Data[x] >> 8));
    printf("%02X ", (u8)(Internal_Flash_Data[x]));
}
printf("\r\n");
#endif          

    for(i = 0; i < INTERNAL_FLASH_MAX_LENGTH; i++)
    {
        Internal_Flash_Data[i] = i * j ;
    }

    Internal_Flash_Write_HalfWord(FLASH_ADD_TEMPERATURE_STORE_INFO,Internal_Flash_Data,INTERNAL_FLASH_MAX_LENGTH);	//��MAC��ַд���ڲ�Flash
        
#if (SERVER_PRINTF_EN)
printf("д���������:\r\n");
for(u16 x = 0; x < INTERNAL_FLASH_MAX_LENGTH; x++)
{
    printf("0x%02X", (u8)(Internal_Flash_Data[x] >> 8));
    printf("%02X ", (u8)(Internal_Flash_Data[x]));
}
printf("\r\n");
#endif          
         
    for(i = 0; i < INTERNAL_FLASH_MAX_LENGTH; i++)
    {
        Internal_Flash_Data[i] = 0;
    }

    Internal_Flash_Read_HalfWord(FLASH_ADD_TEMPERATURE_STORE_INFO, Internal_Flash_Data, INTERNAL_FLASH_MAX_LENGTH); 
        
#if (SERVER_PRINTF_EN)
printf("��ȡ��������:\r\n");
for(u16 x = 0; x < INTERNAL_FLASH_MAX_LENGTH; x++)
{
    printf("0x%02X", (u8)(Internal_Flash_Data[x] >> 8));
    printf("%02X ", (u8)(Internal_Flash_Data[x]));
}
printf("\r\n");
#endif     
        
    while(9);

//    j++;
//
//    Delay_ms(2000);
}

