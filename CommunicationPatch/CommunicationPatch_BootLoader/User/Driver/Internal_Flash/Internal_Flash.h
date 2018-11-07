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

//�洢��ַ
#define INTERNAL_FLASH_END_ADDR            ((u32)(0x08000000 + FLASH_SIZE))	        //������ַ����1ҳ���ֽ�
#define INTERNAL_FLASH_START_ADDR          (INTERNAL_FLASH_END_ADDR - PAGE_SIZE)    //��ʼ�洢��ַΪ���һҳ


//�洢���ݵ�ƫ�Ƶ�ַ
#define FLASH_ADD_TEMPERATURE_STORE_INFO    0                   //�洢�¶Ȳ��Ա�־��ƫ�Ƶ�ַ
#define FLASH_ADD_THERMOSTAT_MAX_NUM        1                   //�洢������������ƫ�Ƶ�ַ

#define FLASH_ADD_PROGRAM_UPGRADE_FLAG      0                   //�洢�Ƿ�̼�Ҫ������ƫ�Ƶ�ַ






/******************************************************************************
//ö�١��ṹ�����Ͷ���
*******************************************************************************/
















/******************************************************************************
//��������/����
*******************************************************************************/
//�̼�������־
extern u8 SygProgramUpgradeFlag;















/******************************************************************************
//��������
*******************************************************************************/
//����:	��Ƭ��Ƭ��Flash�洢��2�ֽڶ�ȡ
void Internal_Flash_Read_HalfWord(u32 addr, u16* buffer, u16 length);

//����:	��Ƭ��Ƭ��Flash�洢��2�ֽ�д��
u8 Internal_Flash_Write_HalfWord(u32 addr, u16 *buffer, u16 length);

//����:	��ȡƬ��Flash���Ƿ�̼������ı�־
void Internal_Flash_Upgrade_ReadOut(void);

//����:	�洢�Ƿ�̼������ı�־��Ƭ��Flash
void Internal_Flash_Upgrade_StoreIn(void);

//����:	��Ƭ��Ƭ��Flash�洢���� 
void Internal_Flash_Test(void);

#endif