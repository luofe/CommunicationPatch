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
//�궨��
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)                                //128�ֽڰ���С
#define PACKET_1K_SIZE          (1024)                               //1K�ֽڰ���С

#define FILE_NAME_LENGTH        (256)                                //�����ļ�������
#define FILE_SIZE_LENGTH        (16)                                 //�����ļ���С����

#define SOH                     (0x01)                               //128�ֽ����ݰ���ʼ�ַ�
#define STX                     (0x02)                               //1024�ֽ����ݰ���ʼ�ַ�
#define EOT                     (0x04)                               //�ļ��������
#define ACK                     (0x06)                               //ȷ��Ӧ��
#define NAK                     (0x15)                               //��Ӧ��(���ִ���)
#define CA                      (0x18)                               //�����������ַ�ȡ������
#define CRC16                   (0x43)                               //'C' == 0x43, request 16-bit CRC
                                                                     
#define ABORT1                  (0x41)                               //'A' == 0x41,�û�ȡ��
#define ABORT2                  (0x61)                               //'a' == 0x61,�û�ȡ��

//#define WAIT_UPGRADE_TIMEOUT_MAX_COUNT  (30)                        //����30�γ�ʱû���������������Զ��˳�����
      
      

//******************************************************************************
//��������
int32_t Ymodem_Receive (uint8_t *);                                  //ʹ��Ymodem����һ���ļ�����
uint8_t Ymodem_Transmit (uint8_t *,const  uint8_t* , uint32_t );     //ʹ��Ymodem����һ���ļ�����

#endif  /* _YMODEM_H_ */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
