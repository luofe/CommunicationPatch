
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
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

//��������: ���ݴ洢������
void Data_Storge_Process(u8* data, u16 len);













/******************************************************************************
//��������
*******************************************************************************/
/**
* @brief  SPI_FLASH��ʼ��
* @param  ��
* @retval ��
*/
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
/**
* @brief  ����FLASH����
* @param  SectorAddr��Ҫ������������ַ
* @retval ��
*/
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


/**
* @brief  ����FLASH��������Ƭ����
* @param  ��
* @retval ��
*/
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



/**
* @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
* @param	pBuffer��Ҫд�����ݵ�ָ��
* @param WriteAddr��д���ַ
* @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���SPI_FLASH_PerWritePageSize
* @retval ��
*/
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


/**
* @brief  ��FLASHд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
* @param	pBuffer��Ҫд�����ݵ�ָ��
* @param  WriteAddr��д���ַ
* @param  NumByteToWrite��д�����ݳ���
* @retval ��
*/
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

/**
* @brief  ��ȡFLASH����
* @param 	pBuffer���洢�������ݵ�ָ��
* @param   ReadAddr����ȡ��ַ
* @param   NumByteToRead����ȡ���ݳ���
* @retval ��
*/
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


/**
* @brief  ��ȡFLASH ID
* @param 	��
* @retval FLASH ID
*/
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

/**
* @brief  ��ȡFLASH Device ID
* @param 	��
* @retval FLASH Device ID
*/
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
/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
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


/**
* @brief  ʹ��SPI��ȡһ���ֽڵ�����
* @param  ��
* @retval ���ؽ��յ�������
*/
u8 SPI_FLASH_ReadByte(void)
{
    return (SPI_FLASH_SendByte(Dummy_Byte));
}


/**
* @brief  ʹ��SPI����һ���ֽڵ�����
* @param  byte��Ҫ���͵�����
* @retval ���ؽ��յ�������
*/
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

/*******************************************************************************
* Function Name  : SPI_FLASH_SendHalfWord
* Description    : Sends a Half Word through the SPI interface and return the
*                  Half Word received from the SPI bus.
* Input          : Half Word : Half Word to send.
* Output         : None
* Return         : The value of the received Half Word.
*******************************************************************************/
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


/**
* @brief  ��FLASH���� дʹ�� ����
* @param  none
* @retval none
*/
void SPI_FLASH_WriteEnable(void)
{
    /* ͨѶ��ʼ��CS�� */
    SPI_FLASH_CS_LOW();
    
    /* ����дʹ������*/
    SPI_FLASH_SendByte(W25X_WriteEnable);
    
    /*ͨѶ������CS�� */
    SPI_FLASH_CS_HIGH();
}

/**
* @brief  �ȴ�WIP(BUSY)��־����0�����ȴ���FLASH�ڲ�����д�����
* @param  none
* @retval none
*/
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


//�������ģʽ
void SPI_Flash_PowerDown(void)   
{ 
    /* ѡ�� FLASH: CS �� */
    SPI_FLASH_CS_LOW();
    
    /* ���� ���� ���� */
    SPI_FLASH_SendByte(W25X_PowerDown);
    
    /* ֹͣ�ź�  FLASH: CS �� */
    SPI_FLASH_CS_HIGH();
}   

//����
void SPI_Flash_WAKEUP(void)   
{
    /*ѡ�� FLASH: CS �� */
    SPI_FLASH_CS_LOW();
    
    /* ���� �ϵ� ���� */
    SPI_FLASH_SendByte(W25X_ReleasePowerDown);
    
    /* ֹͣ�ź� FLASH: CS �� */
    SPI_FLASH_CS_HIGH();                   //�ȴ�TRES1
}   


/**
* @brief  �ȴ���ʱ�ص�����
* @param  None.
* @retval None.
*/
static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* �ȴ���ʱ��Ĵ���,���������Ϣ */
#if (FLASH_PRINTF_EN)
    printf("SPI �ȴ���ʱ!errorCode = %d",errorCode);
#endif   
    
    return 0;
}

/**
* @brief  Ƭ��Flash��⺯��
* @param  None.
* @retval None.
*/
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
//��    ע: ��
//********************************************************
void Data_Storge_Process(u8* data, u16 len)
{
    u8 page_num[2];
    
    SPI_FLASH_BufferRead(page_num, FLASH_PACKAGE_NUM_ADDRESS, sizeof(page_num));   
    g_DataPageNum = page_num[0] * 256 + page_num[1];
    
#if (FLASH_PRINTF_EN)
    printf("\r\n֮ǰg_DataPageNum=%d", g_DataPageNum);
#endif 
    
    if(g_DataPageNum == 0xFFFF)
    {
        SPI_FLASH_BulkErase();  //������Ƭ
        g_DataPageNum = SENSOR_DATA_MIN_PAGE_NUM - 1;
    }
    //���ҳ��
    g_DataPageNum++;
    if(g_DataPageNum > SENSOR_DATA_MAX_PAGE_NUM)
    {
        SPI_FLASH_BulkErase();  //������Ƭ
        g_DataPageNum = SENSOR_DATA_MIN_PAGE_NUM;
    }
    page_num[0] = (u8)(g_DataPageNum >> 8);
    page_num[1] = (u8)(g_DataPageNum >> 0);
    SPI_FLASH_SectorErase(FLASH_PACKAGE_NUM_ADDRESS);
    SPI_FLASH_BufferWrite(page_num, FLASH_PACKAGE_NUM_ADDRESS, sizeof(page_num));

    /* �����ͻ�����������д��flash�� */
    SPI_FLASH_BufferWrite(data, (g_DataPageNum * 256), len);
}

//���Ժ���
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
