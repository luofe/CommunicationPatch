
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:��Ŀ����ʹ���ⲿFlash�洢���������ݵ�Դ����
//��ע: 
The W25Q128FV array is organized into 65,536 programmable pages of 256-bytes each. Up to 256
bytes can be programmed at a time. Pages can be erased in groups of 16 (4KB sector erase), groups of
128 (32KB block erase), groups of 256 (64KB block erase) or the entire chip (chip erase). The
W25Q128FV has 4,096 erasable sectors and 256 erasable blocks respectively. The small 4KB sectors
allow for greater flexibility in applications that require data and parameter storage. (See Figure 2.)
*******************************************************************************/

/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT; 

// �洢���ݵ�ҳ��
u16 g_DataPageNum = SENSOR_DATA_MIN_PAGE_NUM - 1;
















/******************************************************************************
//��������
*******************************************************************************/
//��������: SPI��ʼ������
void SPI_FLASH_Init(void);

//��������: ����FLASH��������
void SPI_FLASH_SectorErase(u32 SectorAddr);

//��������: ����FLASH��������Ƭ��������
void SPI_FLASH_BulkErase(void);

//��������: ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

//��������: ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);

//��������: ��ȡFLASH����
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);

//��������: ��ȡFLASH ID
u32 SPI_FLASH_ReadID(void);

//��������: ��ȡFLASH Device ID
u32 SPI_FLASH_ReadDeviceID(void);

//��������: Flash��ʼ��ȡ��˳��
void SPI_FLASH_StartReadSequence(u32 ReadAddr);

//��������: ʹ��SPI��ȡһ���ֽڵ�����
u8 SPI_FLASH_ReadByte(void);

//��������: ʹ��SPI����һ���ֽڵ�����
u8 SPI_FLASH_SendByte(u8 byte);

//��������: ���Ͱ���ֵĺ���
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);

//��������: ��FLASH���� дʹ�� ����
void SPI_FLASH_WriteEnable(void);

//��������: �ȴ���FLASH�ڲ�����д�����
void SPI_FLASH_WaitForWriteEnd(void);

//��������: �������ģʽ
void SPI_Flash_PowerDown(void);

//��������: ����Flash
void SPI_Flash_WAKEUP(void);

//��������: �ȴ���ʱ�ص�����
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

//��������: Ƭ��Flash��⺯��
u8  Ext_Flash_Detect(void);

//��������: ���ݴ洢������
void Data_Storge_Process(u8* data, u16 len);










//��������: Ƭ��Flash���Ժ���
void Ext_Flash_Test(void);

/******************************************************************************
//��������
*******************************************************************************/
//********************************************************
//��������: SPI_FLASH_Init
//��������: SPI��ʼ������
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //��λSPIģ��
    SPI_I2S_DeInit(FLASH_SPI);
    
    //ʹ��USARTģ��ʱ��
    if(FLASH_SPI == SPI1)
    {
        RCC_APB2PeriphClockCmd(FLASH_SPI_CLK, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(FLASH_SPI_CLK, ENABLE);
    }
    /* ʹ�� FLASH_SPI ��GPIO ʱ�� */
    RCC_APB2PeriphClockCmd(FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK|FLASH_SPI_MOSI_GPIO_CLK|FLASH_CS_GPIO_CLK, ENABLE);
    
    //����ʱ��
    GPIO_InitStructure.GPIO_Pin     = FLASH_SPI_SCK_PIN;       
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz; 
    GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    //��������
    GPIO_InitStructure.GPIO_Pin     = FLASH_SPI_MISO_PIN;       
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz; 
    GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    
    //�������
    GPIO_InitStructure.GPIO_Pin     = FLASH_SPI_MOSI_PIN;      
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP; 
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz; 
    GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);       
    
    //����CSƬѡ�˿�
    GPIO_InitStructure.GPIO_Pin     = FLASH_CS_PIN;                             
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);
    //��ʼ����ƽ
    SPI_FLASH_CS_HIGH();       //��ʼ״̬ʧ��
    
    //SPI���ֳ�ʼ������
    //��ʼSD��ʼ���׶Σ�SPIʱ��Ƶ�ʱ���С��400K
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;  //˫��ȫ˫��
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;                  //��ģʽ
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;                  //8λ����
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;                    //����Ҫע�⣬һ��Ҫ����Ϊ������������Ч����ΪSD��Ϊ������������Ч
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;                   //��SCK��ż���߲ɼ�����    
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;                     //Ƭѡ��ʹ�����ģʽ                      
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;          //�����ʷ�Ƶֵ����
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;                 //�������ݸ�λ��ǰ
    SPI_InitStructure.SPI_CRCPolynomial     = 7;                                //CRCУ���еĶ���ʽ��
    SPI_Init(FLASH_SPI, &SPI_InitStructure);                                  //SPI1��ʼ����
    
    SPI_Cmd(FLASH_SPI,ENABLE);                               //ʹ��SPI1 
}

//********************************************************
//��������: SPI_FLASH_SectorErase
//��������: ����FLASH��������
//��    ��: SectorAddr��Ҫ������������ַ
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_SectorErase(u32 SectorAddr)
{
    /* ����FLASHдʹ������ */
    SPI_FLASH_WriteEnable();
    SPI_FLASH_WaitForWriteEnd();
    /* �������� */
    /* ѡ��FLASH: CS�͵�ƽ */
    SPI_FLASH_CS_LOW();
    /* ������������ָ��*/
    SPI_FLASH_SendByte(W25X_SectorErase);
    /*���Ͳ���������ַ�ĸ�λ*/
    SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* ���Ͳ���������ַ����λ */
    SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* ���Ͳ���������ַ�ĵ�λ */
    SPI_FLASH_SendByte(SectorAddr & 0xFF);
    /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
    SPI_FLASH_CS_HIGH();
    /* �ȴ��������*/
    SPI_FLASH_WaitForWriteEnd();
}

//********************************************************
//��������: SPI_FLASH_BulkErase
//��������: ����FLASH��������Ƭ��������
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_BulkErase(void)
{
    /* ����FLASHдʹ������ */
    SPI_FLASH_WriteEnable();
    
    /* ���� Erase */
    /* ѡ��FLASH: CS�͵�ƽ */
    SPI_FLASH_CS_LOW();
    /* �����������ָ��*/
    SPI_FLASH_SendByte(W25X_ChipErase);
    /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
    SPI_FLASH_CS_HIGH();
    
    /* �ȴ��������*/
    SPI_FLASH_WaitForWriteEnd();
}

//********************************************************
//��������: SPI_FLASH_PageWrite
//��������: ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
//��    ��: pBuffer��Ҫд�����ݵ�ָ�룬WriteAddr��д���ַ��NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    /* ����FLASHдʹ������ */
    SPI_FLASH_WriteEnable();
    
    /* ѡ��FLASH: CS�͵�ƽ */
    SPI_FLASH_CS_LOW();
    /* дҳдָ��*/
    SPI_FLASH_SendByte(W25X_PageProgram);
    /*����д��ַ�ĸ�λ*/
    SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /*����д��ַ����λ*/
    SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /*����д��ַ�ĵ�λ*/
    SPI_FLASH_SendByte(WriteAddr & 0xFF);
    
    if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
    {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
        
#if (FLASH_PRINTF_EN)
        printf("SPI_FLASH_PageWrite too large!\r\n");
#endif   
        
    }
    
    /* д������*/
    while (NumByteToWrite--)
    {
        /* ���͵�ǰҪд����ֽ����� */
        SPI_FLASH_SendByte(*pBuffer);
        /* ָ����һ�ֽ����� */
        pBuffer++;
    }
    
    /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
    SPI_FLASH_CS_HIGH();
    
    /* �ȴ�д�����*/
    SPI_FLASH_WaitForWriteEnd();
}

//********************************************************
//��������: SPI_FLASH_BufferWrite
//��������: ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
//��    ��: pBuffer��Ҫд�����ݵ�ָ�룬WriteAddr��д���ַ��NumByteToWrite��д�����ݳ���
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite)
{
    u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
	/*mod�������࣬��writeAddr��SPI_FLASH_PageSize��������������AddrֵΪ0*/
    Addr = WriteAddr % SPI_FLASH_PageSize;
	
	/*��count������ֵ���պÿ��Զ��뵽ҳ��ַ*/
    count = SPI_FLASH_PageSize - Addr;	
	/*�����Ҫд��������ҳ*/
    NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	/*mod�������࣬�����ʣ�಻��һҳ���ֽ���*/
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
    
    /* Addr=0,��WriteAddr �պð�ҳ���� aligned  */
    if (Addr == 0) 
    {
		/* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0) 
        {
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
			/*�Ȱ�����ҳ��д��*/
            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
			
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        }
    }
	/* ����ַ�� SPI_FLASH_PageSize ������  */
    else 
    {
		/* NumByteToWrite < SPI_FLASH_PageSize */
        if (NumOfPage == 0) 
        {
			/*��ǰҳʣ���count��λ�ñ�NumOfSingleС��д����*/
            if (NumOfSingle > count) 
            {
                temp = NumOfSingle - count;
				
				/*��д����ǰҳ*/
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;
				
				/*��дʣ�������*/
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
            }
            else /*��ǰҳʣ���count��λ����д��NumOfSingle������*/
            {				
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
			/*��ַ����������count�ֿ������������������*/
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
            
            SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;
			
			/*������ҳ��д��*/
            while (NumOfPage--)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr +=  SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }
			/*���ж���Ĳ���һҳ�����ݣ�����д��*/
            if (NumOfSingle != 0)
            {
                SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

//********************************************************
//��������: SPI_FLASH_BufferRead
//��������: ��ȡFLASH����
//��    ��: pBuffer��Ҫд�����ݵ�ָ�룬ReadAddr����ȡ��ַ��NumByteToRead����ȡ���ݳ���
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
    /* ѡ��FLASH: CS�͵�ƽ */
    SPI_FLASH_CS_LOW();
    
    /* ���� �� ָ�� */
    SPI_FLASH_SendByte(W25X_ReadData);
    
    /* ���� �� ��ַ��λ */
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* ���� �� ��ַ��λ */
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* ���� �� ��ַ��λ */
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
    
	/* ��ȡ���� */
    while (NumByteToRead--)
    {
        /* ��ȡһ���ֽ�*/
        *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
        /* ָ����һ���ֽڻ����� */
        pBuffer++;
    }
    
    /* ֹͣ�ź� FLASH: CS �ߵ�ƽ */
    SPI_FLASH_CS_HIGH();
}

//********************************************************
//��������: SPI_FLASH_ReadID
//��������: ��ȡFLASH ID
//��    ��: ��
//��    ��: FLASH ID
//��    ע: ��
//********************************************************
u32 SPI_FLASH_ReadID(void)
{
    u32 Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
    
    /* ��ʼͨѶ��CS�͵�ƽ */
    SPI_FLASH_CS_LOW();
    
    /* ����JEDECָ���ȡID */
    SPI_FLASH_SendByte(W25X_JedecDeviceID);
    
    /* ��ȡһ���ֽ����� */
    Temp0 = SPI_FLASH_SendByte(Dummy_Byte);
    
    /* ��ȡһ���ֽ����� */
    Temp1 = SPI_FLASH_SendByte(Dummy_Byte);
    
    /* ��ȡһ���ֽ����� */
    Temp2 = SPI_FLASH_SendByte(Dummy_Byte);
    
    /* ֹͣͨѶ��CS�ߵ�ƽ */
    SPI_FLASH_CS_HIGH();
    
	/*�����������������Ϊ�����ķ���ֵ*/
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    
    return Temp;
}

//********************************************************
//��������: SPI_FLASH_ReadDeviceID
//��������: ��ȡFLASH Device ID
//��    ��: ��
//��    ��: FLASH Device ID
//��    ע: ��
//********************************************************
u32 SPI_FLASH_ReadDeviceID(void)
{
    u32 Temp = 0;
    
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();
    
    /* Send "RDID " instruction */
    SPI_FLASH_SendByte(W25X_DeviceID);
    SPI_FLASH_SendByte(Dummy_Byte);
    SPI_FLASH_SendByte(Dummy_Byte);
    SPI_FLASH_SendByte(Dummy_Byte);
    
    /* Read a byte from the FLASH */
    Temp = SPI_FLASH_SendByte(Dummy_Byte);
    
    /* Deselect the FLASH: Chip Select high */
    SPI_FLASH_CS_HIGH();
    
    return Temp;
}

//********************************************************
//��������: SPI_FLASH_StartReadSequence
//��������: Flash��ʼ��ȡ��˳��
//��    ��: ReadAddr������ȡ��ַ
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_StartReadSequence(u32 ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    SPI_FLASH_CS_LOW();
    
    /* Send "Read from Memory " instruction */
    SPI_FLASH_SendByte(W25X_ReadData);
    
    /* Send the 24-bit address of the address to read from -----------------------*/
    /* Send ReadAddr high nibble address byte */
    SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

//********************************************************
//��������: SPI_FLASH_ReadByte
//��������: ʹ��SPI��ȡһ���ֽڵ�����
//��    ��: ��
//��    ��: ���ؽ��յ�������
//��    ע: ��
//********************************************************
u8 SPI_FLASH_ReadByte(void)
{
    return (SPI_FLASH_SendByte(Dummy_Byte));
}

//********************************************************
//��������: SPI_FLASH_SendByte
//��������: ʹ��SPI����һ���ֽڵ�����
//��    ��: byte��Ҫ���͵�����
//��    ��: ���ؽ��յ�������
//��    ע: ��
//********************************************************
u8 SPI_FLASH_SendByte(u8 byte)
{
    SPITimeout = SPIT_FLAG_TIMEOUT;
    
    /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
    }
    
    /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
    SPI_I2S_SendData(FLASH_SPI, byte);
    
    SPITimeout = SPIT_FLAG_TIMEOUT;
    
    /* �ȴ����ջ������ǿգ�RXNE�¼� */
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
    }
    
    /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
    return SPI_I2S_ReceiveData(FLASH_SPI);
}

//********************************************************
//��������: SPI_FLASH_SendHalfWord
//��������: ���Ͱ���ֵĺ���
//��    ��: HalfWord������������
//��    ��: ���ؽ��յ�������
//��    ע: ��
//********************************************************
u16 SPI_FLASH_SendHalfWord(u16 HalfWord)
{
    
    SPITimeout = SPIT_FLAG_TIMEOUT;
    
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(2);
    }
    
    /* Send Half Word through the FLASH_SPI peripheral */
    SPI_I2S_SendData(FLASH_SPI, HalfWord);
    
    SPITimeout = SPIT_FLAG_TIMEOUT;
    
    /* Wait to receive a Half Word */
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(3);
    }
    /* Return the Half Word read from the SPI bus */
    return SPI_I2S_ReceiveData(FLASH_SPI);
}

//********************************************************
//��������: SPI_FLASH_WriteEnable
//��������: ��FLASH���� дʹ�� ����
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_WriteEnable(void)
{
    /* ͨѶ��ʼ��CS�� */
    SPI_FLASH_CS_LOW();
    
    /* ����дʹ������*/
    SPI_FLASH_SendByte(W25X_WriteEnable);
    
    /*ͨѶ������CS�� */
    SPI_FLASH_CS_HIGH();
}

//********************************************************
//��������: SPI_FLASH_WaitForWriteEnd
//��������: �ȴ���FLASH�ڲ�����д�����
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_FLASH_WaitForWriteEnd(void)
{
    u8 FLASH_Status = 0;
    
    /* ѡ�� FLASH: CS �� */
    SPI_FLASH_CS_LOW();
    
    /* ���� ��״̬�Ĵ��� ���� */
    SPI_FLASH_SendByte(W25X_ReadStatusReg);
    
    SPITimeout = SPIT_FLAG_TIMEOUT;
    /* ��FLASHæµ����ȴ� */
    do
    {
        /* ��ȡFLASHоƬ��״̬�Ĵ��� */
        FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);	 
        
        {
            if((SPITimeout--) == 0) 
            {
                SPI_TIMEOUT_UserCallback(4);
                return;
            }
        } 
    }
    while ((FLASH_Status & WIP_Flag) == SET); /* ����д���־ */
    
    /* ֹͣ�ź�  FLASH: CS �� */
    SPI_FLASH_CS_HIGH();
}

//********************************************************
//��������: SPI_Flash_PowerDown
//��������: �������ģʽ
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_Flash_PowerDown(void)   
{ 
    /* ѡ�� FLASH: CS �� */
    SPI_FLASH_CS_LOW();
    
    /* ���� ���� ���� */
    SPI_FLASH_SendByte(W25X_PowerDown);
    
    /* ֹͣ�ź�  FLASH: CS �� */
    SPI_FLASH_CS_HIGH();
}   

//********************************************************
//��������: SPI_Flash_WAKEUP
//��������: ����Flash
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void SPI_Flash_WAKEUP(void)   
{
    /*ѡ�� FLASH: CS �� */
    SPI_FLASH_CS_LOW();
    
    /* ���� �ϵ� ���� */
    SPI_FLASH_SendByte(W25X_ReleasePowerDown);
    
    /* ֹͣ�ź� FLASH: CS �� */
    SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   

//********************************************************
//��������: SPI_TIMEOUT_UserCallback
//��������: �ȴ���ʱ�ص�����
//��    ��: errorCode����������
//��    ��: ��
//��    ע: ��
//********************************************************
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* �ȴ���ʱ��Ĵ���,���������Ϣ */
#if (FLASH_PRINTF_EN)
    printf("SPI �ȴ���ʱ!errorCode = %d",errorCode);
#endif   
    
    return 0;
}

//********************************************************
//��������: Ext_Flash_Detect
//��������: Ƭ��Flash��⺯��
//��    ��: ��
//��    ��: ���ؼ�������ɹ�����ʧ��
//��    ע: ��
//********************************************************
u8  Ext_Flash_Detect(void)
{
    //��ȡ��ID�洢λ��
    u32 DeviceID = 0;
    u32 FlashID = 0;
    
    /* ��ȡ Flash Device ID */
	DeviceID = SPI_FLASH_ReadDeviceID();	
	Delay_us( 200 );	
	/* ��ȡ SPI Flash ID */
	FlashID = SPI_FLASH_ReadID();
    
#if (FLASH_PRINTF_EN)
    printf("FlashID is 0x%X,  Device ID is 0x%X\r\n", FlashID, DeviceID);
#endif   
    
	/* ���� SPI Flash ID */
	if (FlashID == sFLASH_ID)
    {
        
        return SUCCEED;
    }
    else
    {
        return FAILURE;
    }
}

//********************************************************
//��������: Data_Storge_Process
//��������: ���ݴ洢������
//��    ��: u8* data������������, u16 len�������ݳ���
//��    ��: ��
//��    ע: 
//1����һ������ǰ2���ֽ����ڴ�ű��δ洢���ݵ�ҳ�룻
//2���ڶ���������ʼ��ÿҳ���һ��δ���͵����ݰ���
//3��ÿ��������16ҳ���ܹ���4096���ֽڣ�ÿ�δ洢һҳ��
//4���Ȱ�����������ȡ������Ȼ��Ѷ�ӦҪд���ҳ�����ݸ��µ����������ڣ�����������д�룻
//5���������ÿ����һ�����ݰ��Ĵ洢�㣬����֧�����45������ݴ洢���������ÿ�����������ݰ��Ĵ洢�㣬�����ֻ��֧��22�죻
//6������洢����45�죬�����´�����洢��ַ��ʼ���ǡ�
//********************************************************
void Data_Storge_Process(u8* data, u16 len)
{
    u8  page_num[2];
    u8  temp_array[SPI_FLASH_PageSize * SPI_FLASH_PerSectorPage] = {0}; //����һ�������Ĵ洢�ռ�
    
    SPI_FLASH_BufferRead(page_num, FLASH_PACKAGE_NUM_ADDRESS, sizeof(page_num));   
    g_DataPageNum = page_num[0] * 256 + page_num[1];
    if((g_DataPageNum == 0xFFFF) || (g_DataPageNum == 0x0000)) //˵����û�д洢��
    {
        g_DataPageNum = SENSOR_DATA_MIN_PAGE_NUM;
    }
    else                        //������֮ǰ�Ѿ��洢����
    {
        g_DataPageNum++;
        if(g_DataPageNum >= SENSOR_DATA_MAX_PAGE_NUM)       //ע�⣺�˴�������">="
        {
            g_DataPageNum = SENSOR_DATA_MIN_PAGE_NUM;
        }
    }
        
#if (FLASH_PRINTF_EN)
    printf("\r\n����д���ҳ����%d\r\n", g_DataPageNum);
#endif 
    
    //���汾��д�����ݵ�ҳ��
    page_num[0] = (u8)(g_DataPageNum >> 8);
    page_num[1] = (u8)(g_DataPageNum >> 0);
    SPI_FLASH_SectorErase(FLASH_PACKAGE_NUM_ADDRESS);
    SPI_FLASH_BufferWrite(page_num, FLASH_PACKAGE_NUM_ADDRESS, sizeof(page_num));
    
    //������ǰ��������������
    SPI_FLASH_BufferRead(temp_array, ((g_DataPageNum / SPI_FLASH_PerSectorPage) * SPI_FLASH_PerSectorSize), sizeof(temp_array));
    //��Ҫ�洢�����ݿ��������������Ӧ��λ��
    memcpy(&temp_array[(g_DataPageNum % SPI_FLASH_PerSectorPage) * SPI_FLASH_PageSize], data, len);
    //������ǰҪд��ҳ��Ӧ������
    SPI_FLASH_SectorErase(g_DataPageNum * SPI_FLASH_PageSize);
    
    //����ǰ����������д��
    SPI_FLASH_BufferWrite(temp_array, ((g_DataPageNum / SPI_FLASH_PerSectorPage) * SPI_FLASH_PerSectorSize), sizeof(temp_array));
}

//********************************************************
//��������: Ext_Flash_Test
//��������: Ƭ��Flash���Ժ���
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void Ext_Flash_Test(void)
{
    u8  Tx_Buffer[] = {
0x5B, 0xF8, 0x1F, 0x19, 
0xFF, 0xFF, 0xFF, 0xFF, 
0x05, 0xF1,
0x0E,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x03, 0x00, 0x00, 0x59, 0x0B, 0x00, 0x13, 0x53, 0x63, 0x00, 0x0B, 0x68, 0x8B,
0x04, 0xFF, 0xFF, 0xD4, 0x9E, 0xFF, 0xFF, 0x92, 0xED, 0x00, 0x00, 0x79, 0x8B,
0x05, 0xFF, 0xFF, 0xE7, 0x5D, 0xFF, 0xFF, 0x15, 0xFA, 0x00, 0x01, 0x10, 0x48,
0x06, 0x00, 0x02, 0x63, 0x99, 0x00, 0x03, 0x9B, 0x13, 0x00, 0x00, 0x0B, 0xF3,
0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x60, 0x00, 0x00, 0x08, 0x16, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x61, 0x00, 0x00, 0x1A, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x62, 0x00, 0x00, 0x4E, 0x84, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x63, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x64, 0x00, 0x00, 0x08, 0x8E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x65, 0x00, 0x00, 0x18, 0x1A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x66, 0x00, 0x01, 0x8D, 0xB2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    
    u8  Rx_Buffer[256];
    
    s_GPSInfo.gmtTime = 0x5BF81F17;
    Ext_Flash_Detect();     //���Ƭ��flash�Ƿ����
    while(9)
    {
        s_GPSInfo.gmtTime += 2;
        Tx_Buffer[0] = (u8)(s_GPSInfo.gmtTime >> 24);
        Tx_Buffer[1] = (u8)(s_GPSInfo.gmtTime >> 16);
        Tx_Buffer[2] = (u8)(s_GPSInfo.gmtTime >> 8);
        Tx_Buffer[3] = (u8)(s_GPSInfo.gmtTime >> 0);
        
#if (FLASH_PRINTF_EN)
        printf("\r\nд�������Ϊ��\r\n");
        for(u16 i = 0; i < sizeof(Tx_Buffer); i++)
        {
            printf("%02X ", Tx_Buffer[i]);
        }
#endif   
        
        Data_Storge_Process(Tx_Buffer, sizeof(Tx_Buffer));
        
        /* ���ո�д������ݶ������ŵ����ջ������� */
        SPI_FLASH_BufferRead(Rx_Buffer, (g_DataPageNum * 256), sizeof(Tx_Buffer));
        
#if (FLASH_PRINTF_EN)
        printf("\r\n����������Ϊ��\r\n");
        for(u16 i = 0; i < sizeof(Tx_Buffer); i++)
        {
            printf("%02X ", Rx_Buffer[i]);
        }
#endif
        
        Delay_ms(2000);
    }
}


/*********************************************END OF FILE**********************/
