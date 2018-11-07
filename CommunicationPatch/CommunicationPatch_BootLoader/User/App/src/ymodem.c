/*******************************************************************************
// Copyright(c)2017, 天津新众聚联信息科技股份有限公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language：C)
// Date   : 2017-04-01
// Author : 刘锋
// Functiom: 下载和上传文件的Ymodem解析的源文件
*******************************************************************************/  

/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//变量定义
*******************************************************************************/
uint8_t  file_name[FILE_NAME_LENGTH];                      //定义存储文件名临时变量
uint32_t FlashDestination = ApplicationAddress;            //定义Flash用户程序目的地址变量
uint16_t PageSize = PAGE_SIZE;
uint32_t EraseCounter = 0x0;
uint32_t NbrOfPage = 0;
FLASH_Status FLASHStatus = FLASH_COMPLETE;
uint32_t RamSource;
extern uint8_t tab_1024[1024];

// 从临时应用程序存储区读取出来数据转存到实际应用程序存储区的缓存空间
extern int32_t AppSpaceBuf[(PAGE_SIZE / 4)];




/******************************************************************************
//函数声明
*******************************************************************************/



/******************************************************************************
//函数定义
*******************************************************************************/
/**
  * @brief  Receive byte from sender
  * @param  c: Character
  * @param  timeout: Timeout
  * @retval 0: Byte received
  *         -1: Timeout
  */
//接收字节函数(在给定的超时次数内，接收到字符返回0，否则返回-1)
static  int32_t Receive_Byte (uint8_t *c, uint32_t timeout)
{
    while (timeout-- > 0)
    {
        if (SerialKeyPressed(c) == 1)
        {
            return 0;                                      //接收到字节返回
        }//if (SerialKeyPressed(c) == 1)
    }//end of while (timeout-- > 0)

    return -1;                                             //接收超时返回
}


/**
  * @brief  Send a byte
  * @param  c: Character
  * @retval 0: Byte sent
  */
//发送字节函数
static uint32_t Send_Byte (uint8_t c)
{
    SerialPutChar(c);                                      //串口发送一字节数据
    return 0;
}


/**
  * @brief  Receive a packet from sender
  *         从发送端接收一个包
  * @param  data
  * @param  length
  * @param  timeout
  *     0: end of transmission 传输中止
  *    -1: abort by sender     发送端中止发送
  *    >0: packet length       包长度
  * @retval 0: normally return          正常返回
  *        -1: timeout or packet error  超时或包出错
  *         1: abort by user            用户中止
  */
//接收1包数据
static int32_t Receive_Packet (uint8_t *data, int32_t *length, uint32_t timeout)
{
    uint16_t i, packet_size;
    uint8_t c;
    *length = 0;
    if (Receive_Byte(&c, timeout) != 0)
    {
        return -1;                                         //接收超时返回
    }//end of if (Receive_Byte(&c, timeout) != 0)

    switch (c)                                             //正常接收到
    {
        case SOH:                                          //数据块大小为128字节
            packet_size = PACKET_SIZE;
            break;
            
        case STX:                                          //数据块大小为1K字节
            packet_size = PACKET_1K_SIZE;
            break;
            
        case EOT:                                          //文件传输结束
            return 0;
        
        case CA:                                           //取消传输
            if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
            {
                *length = -1;
                return 0;                                  //正常返回
            }
            else
            {
                return -1;                                 //超时或报错误返回
            }
        case ABORT1:
        case ABORT2:
             return 1;                                     //用户中止传输
        
        default:                                           //超时或报错误返回
             return -1;
    }//end of switch (c)
    
    //连续接收一包数据
    *data = c;
    for (i = 1; i < (packet_size + PACKET_OVERHEAD); i ++)
    {
        if (Receive_Byte(data + i, timeout) != 0)
        {
            return -1;
        }
    }
    
    //判断序号和补码是否正确
    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
    {
        return -1;
    }
    *length = packet_size;
    
    return 0;
}


/**
  * @brief  Receive a file using the ymodem protocol
  *         使用ymodem协议接收一个文件
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
    int32_t size = 0;                                      //定义下载文件的大小变量
    int32_t send_crc16_count = 0;
    

    /*
     * 初始化FLASH目标地址变量FlashDestination：先存放到临时应用程序存储区//指向用户程序的起始地址
     */
    FlashDestination = ApplicationTempAddress;

    for (session_done = 0, errors = 0, session_begin = 0; ;)
    {
        for (packets_received = 0, file_done = 0, buf_ptr = buf; ;)
        {
            switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
            {
                case 0:                                    //正常接收一包数据
                    errors = 0;
                    switch (packet_length)
                    {
                        case - 1:                          //发送端中断传输
                            Send_Byte(ACK);
                            return 0;

                        case 0:                            //传输结束
                            Send_Byte(ACK);
                            file_done = 1;
                            break;

                        default:                           //接收为正常数据包
                            if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                            {
                                Send_Byte(NAK);
                            }
                            else
                            {
                                if (packets_received == 0)
                                {                          //接收文件包处理
                                    if (packet_data[PACKET_HEADER] != 0)
                                    {
                                        /* Filename packet has valid data */
                                        for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                                        {
                                            file_name[i++] = *file_ptr++;
                                        }
                                        file_name[i++] = '\0';//保存文件名
                                        for (i = 0, file_ptr ++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
                                        {
                                            file_size[i++] = *file_ptr++;
                                        }
                                        file_size[i++] = '\0';
                                        Str2Int(file_size, &size);//保存文件大小
                                        
                                        //判断文件名是否是该电路的应用程序"TF-VMC-Hxx-MC-Vx.x.x"
                                        if((file_name[0] != 'T') || (file_name[1] != 'F')
                                           || (file_name[3] != 'V') || (file_name[4] != 'M') || (file_name[5] != 'C')
                                           || (file_name[7] != 'H')
                                           || (file_name[11] != 'M') || (file_name[12] != 'C'))
                                        {
                                            // 如果文件名不对，则主动取消升级
                                            Send_Byte(CA);
                                            Send_Byte(CA);
                                            return -1;
                                        }
                                        
                                        /* Test the size of the image to be sent */
                                        /* Image size is greater than Flash size */
                                        if (size > ApplicationTempSize) //(FLASH_SIZE - 1)
                                        {                         //若接收到的文件大小大于FLASH大小，则终止传输
                                            /* End session */
                                            Send_Byte(CA);
                                            Send_Byte(CA);
                                            return -1;
                                        }
                                        
                                        /* Erase the needed pages where the user application will be loaded */
                                        /* Define the number of page to be erased */
                                        NbrOfPage = FLASH_PagesMask(size);//计算需要擦除的应用程序页数
                                        
                                        //擦除FLASH页
                                        for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
                                        {
                                            FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
                                        }
                                        Send_Byte(ACK);                   //发送文件包的应答信号
                                        Send_Byte(CRC16);
                                        
                                    }
                                    /* Filename packet is empty, end session */
                                    else
                                    {                                     //文件包空，中断传输
                                        Send_Byte(ACK);
                                        file_done = 1;
                                        session_done = 1;
                                        break;
                                    }
                                }
                                //接收到的为数据包
                                else
                                {
                                    memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);//将buf_ptr内容拷贝到packet_data对应的地址
                                    RamSource = (uint32_t)buf;
                                    //循环将接收到的数据包程序写入对应的FLASH
                                    for (j = 0;(j < packet_length) && (FlashDestination <  ApplicationTempAddress + size);j += 4)
                                    {
                                        /* Program the data received into STM32F10x Flash */
                                        FLASH_ProgramWord(FlashDestination, *(uint32_t*)RamSource);//写入FLASH程序
                                        
                                        if (*(uint32_t*)FlashDestination != *(uint32_t*)RamSource)//校验是否写入FLASH成功
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
                                    
                                    //置位接收到编程文件
                                    g_ProgramFlashFileRecFlag = 1;
                                }
                                packets_received ++;
                                session_begin = 1;
                            }
                    }
                    break;

                case 1:                                    //用户中止传输
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
                    //超时退出升级
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

    
    // 接收成功了，则从临时存储区拷贝到应用程序区
    FlashDestination = ApplicationAddress;
    //擦除应用程序区的FLASH页
    for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
    {
        FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
    }
    //循环将临时应用程序存储区的数据读取出来再转存到实际应用程序存储区
    for (j = 0; j < NbrOfPage; j++)
    {	
        FlashDestination = ApplicationTempAddress + (j * PAGE_SIZE);//地址指针指向读取地址
        for (i = 0; i < (PAGE_SIZE / 4); i++)
        {
            AppSpaceBuf[i] = *((uint32_t*)FlashDestination); //读数据
            FlashDestination += 4;  
        }
        
        FlashDestination = ApplicationAddress + (j * PAGE_SIZE);//地址指针指向存储地址
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
//准备第一个块
void Ymodem_PrepareIntialPacket(uint8_t *data, const uint8_t* fileName, uint32_t *length)
{
    uint16_t i, j;
    uint8_t file_ptr[10];
    
    /* Make first three packet */
    data[0] = SOH;
    data[1] = 0x00;
    data[2] = 0xff;
    
    /* Filename packet has valid data */
    //文件名包拥有有效的数据
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
//准备数据包
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
//计算YModem包的CRC16
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
//计算YModem包的校验和
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
//使用ymodem协议发送一个数据包
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
//使用ymodem协议传输一个文件
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
    //填充文件名
    for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
    {
        FileName[i] = sendFileName[i];
    }
    CRC16_F = 1;
    
    /* Prepare first block */
    //准备第1个块
    Ymodem_PrepareIntialPacket(&packet_data[0], FileName, &sizeFile);
    //发送第一包
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
    //如果超过MAX_ERRORS收不到应答相应，结束通信
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
        //准备下一包
        Ymodem_PreparePacket(buf_ptr, &packet_data[0], blkNumber, size);
        ackReceived = 0;
        receivedC[0]= 0;
        errors = 0;
        do
        {
            /* Send next packet */
            //发送下一包数据
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
            //发送校验码
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
          
            //等待应答
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
        //如果超过MAX_ERRORS次收不到应答响应，结束通信
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
        //发送传输结束标记
        Send_Byte(EOT);

        //等待应答
        if ((Receive_Byte(&receivedC[0], NAK_TIMEOUT) == 0)  && receivedC[0] == ACK)
        {
            ackReceived = 1;  
        }
        else
        {
            errors++;
        }
    }while (!ackReceived && (errors < MAX_ERRORS));
    //如果超过MAX_ERRORS次收不到应答响应，结束通信
    if (errors >=  MAX_ERRORS)
    {
       return errors;
    }


    //准备最后一包
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
        //发送包
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
        /* Send CRC or Check Sum based on CRC16_F */
        //发送校验码
        tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
        Send_Byte(tempCRC >> 8);
        Send_Byte(tempCRC & 0xFF);
        
        /* Wait for Ack and 'C' */
        //等待应答和'C'
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
    //如果超过MAX_ERRORS次收不到应答相应，结束通信
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
    //超过MAX_ERRORS次收不到应答响应，结束通信
    if (errors >=  MAX_ERRORS)
    {
        return errors;
    }


    return 0;        //文件传输成功
}


//////////////////////////////END OF C文件////////////////////////////////////
