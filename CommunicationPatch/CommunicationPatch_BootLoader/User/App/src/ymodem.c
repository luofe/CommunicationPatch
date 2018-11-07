/*******************************************************************************
// Copyright(c)2017, ������ھ�����Ϣ�Ƽ��ɷ����޹�˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language��C)
// Date   : 2017-04-01
// Author : ����
// Functiom: ���غ��ϴ��ļ���Ymodem������Դ�ļ�
*******************************************************************************/  

/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//��������
*******************************************************************************/
uint8_t  file_name[FILE_NAME_LENGTH];                      //����洢�ļ�����ʱ����
uint32_t FlashDestination = ApplicationAddress;            //����Flash�û�����Ŀ�ĵ�ַ����
uint16_t PageSize = PAGE_SIZE;
uint32_t EraseCounter = 0x0;
uint32_t NbrOfPage = 0;
FLASH_Status FLASHStatus = FLASH_COMPLETE;
uint32_t RamSource;
extern uint8_t tab_1024[1024];

// ����ʱӦ�ó���洢����ȡ��������ת�浽ʵ��Ӧ�ó���洢���Ļ���ռ�
extern int32_t AppSpaceBuf[(PAGE_SIZE / 4)];




/******************************************************************************
//��������
*******************************************************************************/



/******************************************************************************
//��������
*******************************************************************************/
/**
  * @brief  Receive byte from sender
  * @param  c: Character
  * @param  timeout: Timeout
  * @retval 0: Byte received
  *         -1: Timeout
  */
//�����ֽں���(�ڸ����ĳ�ʱ�����ڣ����յ��ַ�����0�����򷵻�-1)
static  int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
{
    while (timeout-- > 0)
    {
        if (SerialKeyPressed(c) == 1)
        {
            return 0;                                      //���յ��ֽڷ���
        }//if (SerialKeyPressed(c) == 1)
    }//end of while (timeout-- > 0)

    return -1;                                             //���ճ�ʱ����
}


/**
  * @brief  Send a byte
  * @param  c: Character
  * @retval 0: Byte sent
  */
//�����ֽں���
static uint32_t Send_Byte (uint8_t c)
{
    SerialPutChar(c);                                      //���ڷ���һ�ֽ�����
    return 0;
}


/**
  * @brief  Receive a packet from sender
  *         �ӷ��Ͷ˽���һ����
  * @param  data
  * @param  length
  * @param  timeout
  *     0: end of transmission ������ֹ
  *    -1: abort by sender     ���Ͷ���ֹ����
  *    >0: packet length       ������
  * @retval 0: normally return          ��������
  *        -1: timeout or packet error  ��ʱ�������
  *         1: abort by user            �û���ֹ
  */
//����1������
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
    uint16_t i, packet_size;
    uint8_t c;
    *length = 0;
    if (Receive_Byte(&c, timeout) != 0)
    {
        return -1;                                         //���ճ�ʱ����
    }//end of if (Receive_Byte(&c, timeout) != 0)

    switch (c)                                             //�������յ�
    {
        case SOH:                                          //���ݿ��СΪ128�ֽ�
            packet_size = PACKET_SIZE;
            break;
            
        case STX:                                          //���ݿ��СΪ1K�ֽ�
            packet_size = PACKET_1K_SIZE;
            break;
            
        case EOT:                                          //�ļ��������
            return 0;
        
        case CA:                                           //ȡ������
            if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
            {
                *length = -1;
                return 0;                                  //��������
            }
            else
            {
                return -1;                                 //��ʱ�򱨴��󷵻�
            }
        case ABORT1:
        case ABORT2:
             return 1;                                     //�û���ֹ����
        
        default:                                           //��ʱ�򱨴��󷵻�
             return -1;
    }//end of switch (c)
    
    //��������һ������
    *data = c;
    for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
    {
        if (Receive_Byte(data + i, timeout) != 0)
        {
            return -1;
        }
    }
    
    //�ж���źͲ����Ƿ���ȷ
    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
    {
        return -1;
    }
    *length = packet_size;
    
    return 0;
}


/**
  * @brief  Receive a file using the ymodem protocol
  *         ʹ��ymodemЭ�����һ���ļ�
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
int32_t Ymodem_Receive (uint8_t *buf)
{
    uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr;
    int32_t i,j;
    int32_t packet_length;
    int32_t session_done;
    int32_t file_done;
    int32_t packets_received;
    int32_t errors;
    int32_t session_begin;
    int32_t size = 0;                                      //���������ļ��Ĵ�С����
    int32_t send_crc16_count = 0;
    

    /*
     * ��ʼ��FLASHĿ���ַ����FlashDestination���ȴ�ŵ���ʱӦ�ó���洢��//ָ���û��������ʼ��ַ
     */
    FlashDestination = ApplicationTempAddress;

    for (session_done = 0, errors = 0, session_begin = 0; ;)
    {
        for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
        {
            switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
            {
                case 0:                                    //��������һ������
                    errors = 0;
                    switch (packet_length)
                    {
                        case - 1:                          //���Ͷ��жϴ���
                            Send_Byte(ACK);
                            return 0;

                        case 0:                            //�������
                            Send_Byte(ACK);
                            file_done = 1;
                            break;

                        default:                           //����Ϊ�������ݰ�
                            if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                            {
                                Send_Byte(NAK);
                            }
                            else
                            {
                                if (packets_received == 0)
                                {                          //�����ļ�������
                                    if (packet_data[PACKET_HEADER] != 0)
                                    {
                                        /* Filename packet has valid data */
                                        for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                                        {
                                            file_name[i++] = *file_ptr++;
                                        }
                                        file_name[i++] = '\0';//�����ļ���
                                        for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
                                        {
                                            file_size[i++] = *file_ptr++;
                                        }
                                        file_size[i++] = '\0';
                                        Str2Int(file_size, &size);//�����ļ���С
                                        
                                        //�ж��ļ����Ƿ��Ǹõ�·��Ӧ�ó���"TF-VMC-Hxx-MC-Vx.x.x"
                                        if((file_name[0] != 'T') || (file_name[1] != 'F')
                                           || (file_name[3] != 'V') || (file_name[4] != 'M') || (file_name[5] != 'C')
                                           || (file_name[7] != 'H')
                                           || (file_name[11] != 'M') || (file_name[12] != 'C'))
                                        {
                                            // ����ļ������ԣ�������ȡ������
                                            Send_Byte(CA);
                                            Send_Byte(CA);
                                            return -1;
                                        }
                                        
                                        /* Test the size of the image to be sent */
                                        /* Image size is greater than Flash size */
                                        if (size > ApplicationTempSize) //(FLASH_SIZE - 1)
                                        {                         //�����յ����ļ���С����FLASH��С������ֹ����
                                            /* End session */
                                            Send_Byte(CA);
                                            Send_Byte(CA);
                                            return -1;
                                        }
                                        
                                        /* Erase the needed pages where the user application will be loaded */
                                        /* Define the number of page to be erased */
                                        NbrOfPage = FLASH_PagesMask(size);//������Ҫ������Ӧ�ó���ҳ��
                                        
                                        //����FLASHҳ
                                        for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
                                        {
                                            FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
                                        }
                                        Send_Byte(ACK);                   //�����ļ�����Ӧ���ź�
                                        Send_Byte(CRC16);
                                        
                                    }
                                    /* Filename packet is empty, end session */
                                    else
                                    {                                     //�ļ����գ��жϴ���
                                        Send_Byte(ACK);
                                        file_done = 1;
                                        session_done = 1;
                                        break;
                                    }
                                }
                                //���յ���Ϊ���ݰ�
                                else
                                {
                                    memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);//��buf_ptr���ݿ�����packet_data��Ӧ�ĵ�ַ
                                    RamSource = (uint32_t)buf;
                                    //ѭ�������յ������ݰ�����д���Ӧ��FLASH
                                    for (j = 0;(j < packet_length) && (FlashDestination <  ApplicationTempAddress + size);j += 4)
                                    {
                                        /* Program the data received into STM32F10x Flash */
                                        FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);//д��FLASH����
                                        
                                        if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)//У���Ƿ�д��FLASH�ɹ�
                                        {
                                            /* End session */
                                            Send_Byte(CA);
                                            Send_Byte(CA);
                                            return -2;
                                        }
                                        FlashDestination += 4;
                                        RamSource += 4;
                                    }
                                    Send_Byte(ACK);
                                    
                                    //��λ���յ�����ļ�
                                    g_ProgramFlashFileRecFlag = 1;
                                }
                                packets_received ++;
                                session_begin = 1;
                            }
                    }
                    break;

                case 1:                                    //�û���ֹ����
                    Send_Byte(CA);
                    Send_Byte(CA);
                    return -3;

                default:
                    if (session_begin > 0)
                    {
                        errors ++;
                    }
                    if (errors > MAX_ERRORS)
                    {
                        Send_Byte(CA);
                        Send_Byte(CA);
                        return 0;
                    }
                    Send_Byte(CRC16);
                    //��ʱ�˳�����
                    if(send_crc16_count++ > WAIT_UPGRADE_TIMEOUT_MAX_COUNT)
                    {
                        size = -4;
                        return (int32_t)size;
                    }
                    break;
            }//end of switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
            if (file_done != 0)
            {
                break;
            }
        }//end of for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
        if (session_done != 0)
        {
            break;
        }//end of if (session_done != 0)
    }//end of for (session_done = 0, errors = 0, session_begin = 0; ;)

    
    // ���ճɹ��ˣ������ʱ�洢��������Ӧ�ó�����
    FlashDestination = ApplicationAddress;
    //����Ӧ�ó�������FLASHҳ
    for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
    }
    //ѭ������ʱӦ�ó���洢�������ݶ�ȡ������ת�浽ʵ��Ӧ�ó���洢��
    for (j = 0; j < NbrOfPage; j++)
    {	
        FlashDestination = ApplicationTempAddress + (j * PAGE_SIZE);//��ַָ��ָ���ȡ��ַ
        for (i = 0; i < (PAGE_SIZE / 4); i++)
        {
            AppSpaceBuf[i] = *((uint32_t*)FlashDestination); //������
            FlashDestination += 4;  
        }
        
        FlashDestination = ApplicationAddress + (j * PAGE_SIZE);//��ַָ��ָ��洢��ַ
        for (i = 0; i < (PAGE_SIZE / 4); i++)
        {
            if (FLASH_COMPLETE != FLASH_ProgramWord(FlashDestination, AppSpaceBuf[i]))
            {
                Send_Byte(CA);
                Send_Byte(CA);
                return -5;
            }  
            
            FlashDestination += 4;
        }
    }
    
    return (int32_t)size;
}


/**
  * @brief  check response using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
int32_t Ymodem_CheckResponse(uint8_t c)
{
    return 0;
}


/**
  * @brief  Prepare the first block
  * @param  timeout
  *     0: end of transmission
  */
//׼����һ����
void Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length)
{
    uint16_t i, j;
    uint8_t file_ptr[10];
    
    /* Make first three packet */
    data[0] = SOH;
    data[1] = 0x00;
    data[2] = 0xff;
    
    /* Filename packet has valid data */
    //�ļ�����ӵ����Ч������
    for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH);i++)
    {
        data[i + PACKET_HEADER] = fileName[i];
    }
    
    data[i + PACKET_HEADER] = 0x00;
    
    Int2Str (file_ptr, *length);
    for (j =0, i = i + PACKET_HEADER + 1; file_ptr[j] != '\0' ; )
    {
        data[i++] = file_ptr[j++];
    }
    
    for (j = i; j < PACKET_SIZE + PACKET_HEADER; j++)
    {
        data[j] = 0;
    }
}


/**
  * @brief  Prepare the data packet
  * @param  timeout
  *     0: end of transmission
  */
//׼�����ݰ�
void Ymodem_PreparePacket(uint8_t *SourceBuf, uint8_t *data, uint8_t pktNo, uint32_t sizeBlk)
{
    uint16_t i, size, packetSize;
    uint8_t* file_ptr;
    
    /* Make first three packet */
    packetSize = sizeBlk >= PACKET_1K_SIZE ? PACKET_1K_SIZE : PACKET_SIZE;
    size = sizeBlk < packetSize ? sizeBlk :packetSize;
    if (packetSize == PACKET_1K_SIZE)
    {
        data[0] = STX;
    }
    else
    {
        data[0] = SOH;
    }
    data[1] = pktNo;
    data[2] = (~pktNo);
    file_ptr = SourceBuf;
    
    /* Filename packet has valid data */
    for (i = PACKET_HEADER; i < size + PACKET_HEADER;i++)
    {
        data[i] = *file_ptr++;
    }
    if ( size  <= packetSize)
    {
        for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
        {
          data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
        }
    }
}


/**
  * @brief  Update CRC16 for input byte
  * @param  CRC input value 
  * @param  input byte
   * @retval None
  */
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
   uint32_t crc = crcIn;
   uint32_t in = byte|0x100;
   
   do
   {
       crc <<= 1;
       in <<= 1;
       if(in&0x100)
       ++crc;
       if(crc&0x10000)
       crc ^= 0x1021;
   }
   while(!(in&0x10000));
   
   return crc&0xffffu;
}


/**
  * @brief  Cal CRC16 for YModem Packet
  * @param  data
  * @param  length
   * @retval None
  */
//����YModem����CRC16
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
    uint32_t crc = 0;
    const uint8_t* dataEnd = data+size;
    
    while(data<dataEnd)
     crc = UpdateCRC16(crc,*data++);
    
    crc = UpdateCRC16(crc,0);
    crc = UpdateCRC16(crc,0);
    
    return crc&0xffffu;
}


/**
  * @brief  Cal Check sum for YModem Packet
  * @param  data
  * @param  length
   * @retval None
  */
//����YModem����У���
uint8_t CalChecksum(const uint8_t* data, uint32_t size)
{
    uint32_t sum = 0;
    const uint8_t* dataEnd = data+size;
    while(data < dataEnd )
      sum += *data++;
    return sum&0xffu;
}


/**
  * @brief  Transmit a data packet using the ymodem protocol
  * @param  data
  * @param  length
   * @retval None
  */
//ʹ��ymodemЭ�鷢��һ�����ݰ�
void Ymodem_SendPacket(uint8_t *data, uint16_t length)
{
    uint16_t i;
    i = 0;
    while (i < length)
    {
        Send_Byte(data[i]);
        i++;
    }
}


/**
  * @brief  Transmit a file using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
//ʹ��ymodemЭ�鴫��һ���ļ�
uint8_t Ymodem_Transmit (uint8_t *buf, const uint8_t* sendFileName, uint32_t sizeFile)
{
    uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];
    uint8_t FileName[FILE_NAME_LENGTH];
    uint8_t *buf_ptr, tempCheckSum ;
    uint16_t tempCRC, blkNumber;
    uint8_t receivedC[2], CRC16_F = 0, i;
    uint32_t errors, ackReceived, size = 0, pktSize;
    
    errors = 0;
    ackReceived = 0;
    //����ļ���
    for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
    {
        FileName[i] = sendFileName[i];
    }
    CRC16_F = 1;
    
    /* Prepare first block */
    //׼����1����
    Ymodem_PrepareIntialPacket(&packet_data[0], FileName, &sizeFile);
    //���͵�һ��
    do 
    {
        /* Send Packet */
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
        /* Send CRC or Check Sum based on CRC16_F */
        if (CRC16_F)
        {
           tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
           Send_Byte(tempCRC >> 8);
           Send_Byte(tempCRC & 0xFF);
        }
        else
        {
           tempCheckSum = CalChecksum (&packet_data[3], PACKET_SIZE);
           Send_Byte(tempCheckSum);
        }
        
        /* Wait for Ack and 'C' */
        if (Receive_Byte(&receivedC[0], NAK_TIMEOUT) == 0)  
        {
            if (receivedC[0] == ACK)
            { 
                /* Packet transfered correctly */
                ackReceived = 1;
            }
        }
        else
        {
            errors++;
        }
    }while (!ackReceived && (errors < MAX_ERRORS));
    //�������MAX_ERRORS�ղ���Ӧ����Ӧ������ͨ��
    if (errors >=  MAX_ERRORS)
    {
        return errors;
    }
    
    
    buf_ptr = buf;
    size = sizeFile;
    blkNumber = 0x01;
    /* Here 1024 bytes package is used to send the packets */

    /* Resend packet if NAK  for a count of 10 else end of commuincation */
    while (size)
    {
        /* Prepare next packet */
        //׼����һ��
        Ymodem_PreparePacket(buf_ptr, &packet_data[0], blkNumber, size);
        ackReceived = 0;
        receivedC[0]= 0;
        errors = 0;
        do
        {
            /* Send next packet */
            //������һ������
            if (size >= PACKET_1K_SIZE)
            {
                pktSize = PACKET_1K_SIZE;
            }
            else
            {
                pktSize = PACKET_SIZE;
            }
            Ymodem_SendPacket(packet_data, pktSize + PACKET_HEADER);
            
            /* Send CRC or Check Sum based on CRC16_F */
            /* Send CRC or Check Sum based on CRC16_F */
            //����У����
            if (CRC16_F)
            {
                tempCRC = Cal_CRC16(&packet_data[3], pktSize);
                Send_Byte(tempCRC >> 8);
                Send_Byte(tempCRC & 0xFF);
            }
            else
            {
                tempCheckSum = CalChecksum (&packet_data[3], pktSize);
                Send_Byte(tempCheckSum);
            }
          
            //�ȴ�Ӧ��
            if ((Receive_Byte(&receivedC[0], NAK_TIMEOUT) == 0)  && (receivedC[0] == ACK))
            {
                ackReceived = 1;  
                if (size > pktSize)
                {
                    buf_ptr += pktSize;  
                    size -= pktSize;
                    if (blkNumber == (FLASH_IMAGE_SIZE/1024))
                    {
                      return 0xFF; /*  error */
                    }
                    else
                    {
                       blkNumber++;
                    }
                }
                else
                {
                    buf_ptr += pktSize;
                    size = 0;
                }
            }
            else
            {
                errors++;
            }
        }while(!ackReceived && (errors < MAX_ERRORS));
        //�������MAX_ERRORS���ղ���Ӧ����Ӧ������ͨ��
        if (errors >=  MAX_ERRORS)
        {
          return errors;
        }
    }//end of while (size)
    
    ackReceived = 0;
    receivedC[0] = 0x00;
    errors = 0;
    do 
    {
        //���ʹ���������
        Send_Byte(EOT);

        //�ȴ�Ӧ��
        if ((Receive_Byte(&receivedC[0], NAK_TIMEOUT) == 0)  && receivedC[0] == ACK)
        {
            ackReceived = 1;  
        }
        else
        {
            errors++;
        }
    }while (!ackReceived && (errors < MAX_ERRORS));
    //�������MAX_ERRORS���ղ���Ӧ����Ӧ������ͨ��
    if (errors >=  MAX_ERRORS)
    {
       return errors;
    }


    //׼�����һ��
    ackReceived = 0;
    receivedC[0] = 0x00;
    errors = 0;
    
    packet_data[0] = SOH;
    packet_data[1] = 0;
    packet_data[2] = 0xFF;
    
    for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
    {
       packet_data [i] = 0x00;
    }
    
    do 
    {
        //���Ͱ�
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
        /* Send CRC or Check Sum based on CRC16_F */
        //����У����
        tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
        Send_Byte(tempCRC >> 8);
        Send_Byte(tempCRC & 0xFF);
        
        /* Wait for Ack and 'C' */
        //�ȴ�Ӧ���'C'
        if (Receive_Byte(&receivedC[0], NAK_TIMEOUT) == 0)  
        {
            if (receivedC[0] == ACK)
            { 
              /* Packet transfered correctly */
              ackReceived = 1;
            }
        }
        else
        {
            errors++;
        }
    }while (!ackReceived && (errors < MAX_ERRORS));
    //�������MAX_ERRORS���ղ���Ӧ����Ӧ������ͨ��
    if (errors >=  MAX_ERRORS)
    {
        return errors;
    }  


    do 
    {
        Send_Byte(EOT);
        /* Send (EOT); */
        /* Wait for Ack */
        if ((Receive_Byte(&receivedC[0], NAK_TIMEOUT) == 0)  && receivedC[0] == ACK)
        {
          ackReceived = 1;  
        }
        else
        {
          errors++;
        }
    }while (!ackReceived && (errors < MAX_ERRORS));
    //����MAX_ERRORS���ղ���Ӧ����Ӧ������ͨ��
    if (errors >=  MAX_ERRORS)
    {
        return errors;
    }


    return 0;        //�ļ�����ɹ�
}


//////////////////////////////END OF C�ļ�////////////////////////////////////
