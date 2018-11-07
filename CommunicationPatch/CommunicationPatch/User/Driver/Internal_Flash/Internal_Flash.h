
/*******************************************************************************
//Internal_Flash.h
//Ƭ��Flash�洢ͷ�ļ�
*******************************************************************************/

#ifndef	INTERNAL_FLASH_H
#define	INTERNAL_FLASH_H

/******************************************************************************
//�궨��
*******************************************************************************/
#define ENABLE_INT()                        __enable_irq();     //ϵͳ��ȫ���ж�  
#define DISABLE_INT()                       __disable_irq();    //ϵͳ��ȫ���ж� 

#define PAGE_SIZE                (0x400)                      //����FLASHҳ��С��1 Kbytes(С��������������1K)
#define FLASH_SIZE               (0x20000)                    //����FLASH�ռ��С��128 KBytes


#define INTERNAL_FLASH_MAX_LENGTH	        10//(1 + (1 + VMC_MAX_THERMOSTAT_NUM * 2 + 2) + 1)    //�洢������󳤶� = �¶Ȳ��Դ洢��־ + �¶Ȳ��Խṹ�� + �豸���

//�洢��ַ
#define DATA_EEPROM_END_ADDR                ((u32)(0x08000000 + FLASH_SIZE) - PAGE_SIZE)	        //������ַ����1ҳ���ֽ�
#define DATA_EEPROM_START_ADDR              (DATA_EEPROM_END_ADDR - PAGE_SIZE)	//��ʼ�洢��ַΪ�����ڶ�ҳ


//�洢���ݵ�ƫ�Ƶ�ַ
#define FLASH_ADD_TEMPERATURE_STORE_INFO    0                   //�洢�¶Ȳ��Ա�־��ƫ�Ƶ�ַ
#define FLASH_ADD_SYSTEM_DEVICE_ID          6                   //�洢�豸��ŵ�ƫ�Ƶ�ַ



#define FLASH_ADD_SYSTEM_ERROR_LOG          72                  //�洢���ִ����־��ƫ�Ƶ�ַ


//�̼������洢��ַ
#define INTERNAL_FLASH_END_ADDR            ((u32)(0x08000000 + FLASH_SIZE))	        //������ַ����1ҳ���ֽ�
#define INTERNAL_FLASH_START_ADDR          (INTERNAL_FLASH_END_ADDR - PAGE_SIZE)    //��ʼ�洢��ַΪ���һҳ

#define FLASH_ADD_PROGRAM_UPGRADE_FLAG      0               //�洢�Ƿ�̼�Ҫ������ƫ�Ƶ�ַ

/******************************************************************************
//ö�١��ṹ�����Ͷ���
*******************************************************************************/
//�洢�Ĳ�������
typedef enum
{
    FLASH_STORE_TEMPERATURN     = 0,    //�洢�¶Ȳ���
    FLASH_STORE_DEVICE_ID,              //�洢�豸���
}FlashStoreParamEnum;















/******************************************************************************
//��������/����
*******************************************************************************/
//�¶Ȳ����Ƿ�洢�ı�־
extern u8 SysTempInfoStoredFlag;

//�̼�������־
extern u8 SygProgramUpgradeFlag;

// �洢����ʼ��ַ�ͽ�����ַ
extern u32  g_InternalFlashStartAddr;	















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

#endif