/**
  ******************************************************************************
  * @file    IAP/inc/ymodem.h 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   This file provides all the software function headers of the ymodem.c 
  *          file.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _YMODEM_H_
    #define _YMODEM_H_


//******************************************************************************
//宏定义
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)                                //128字节包大小
#define PACKET_1K_SIZE          (1024)                               //1K字节包大小

#define FILE_NAME_LENGTH        (256)                                //定义文件名长度
#define FILE_SIZE_LENGTH        (16)                                 //定义文件大小长度

#define SOH                     (0x01)                               //128字节数据包起始字符
#define STX                     (0x02)                               //1024字节数据包起始字符
#define EOT                     (0x04)                               //文件传输结束
#define ACK                     (0x06)                               //确认应答
#define NAK                     (0x15)                               //非应答(出现错误)
#define CA                      (0x18)                               //连续两个本字符取消传输
#define CRC16                   (0x43)                               //'C' == 0x43, request 16-bit CRC
                                                                     
#define ABORT1                  (0x41)                               //'A' == 0x41,用户取消
#define ABORT2                  (0x61)                               //'a' == 0x61,用户取消

//#define WAIT_UPGRADE_TIMEOUT_MAX_COUNT  (30)                        //超过30次超时没有升级操作，则自动退出升级
      
      

//******************************************************************************
//函数声明
int32_t Ymodem_Receive (uint8_t *);                                  //使用Ymodem接收一个文件函数
uint8_t Ymodem_Transmit (uint8_t *,const  uint8_t* , uint32_t );     //使用Ymodem发送一个文件函数

#endif  /* _YMODEM_H_ */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
