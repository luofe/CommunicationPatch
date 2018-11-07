/**
  ******************************************************************************
  * @file    IAP/inc/common.h 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    10/15/2010
  * @brief   This file provides all the headers of the common functions.
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
#ifndef _COMMON_H
  #define _COMMON_H


/* Exported types ------------------------------------------------------------*/
//******************************************************************************
//����ָ�����Ͷ���
typedef  void (*pFunction)(void);


//******************************************************************************
//�����궨��
#define CMD_STRING_SIZE      128                                                //����������ģʽ�������ַ�������󳤶�

#define FLASH_IMAGE_SIZE     (uint32_t) (FLASH_SIZE - (ApplicationAddress - 0x08000000))
                                                                                //����Flash�ϴ������С


//******************************************************************************
//�����궨��
#define IS_AF(c)             ((c >= 'A') && (c <= 'F'))                         //�ж��Ƿ�ΪA~F֮����ַ�
#define IS_af(c)             ((c >= 'a') && (c <= 'f'))                         //�ж��Ƿ�Ϊa~f֮����ַ�
#define IS_09(c)             ((c >= '0') && (c <= '9'))                         //�ж��Ƿ�Ϊ0~9֮�������
#define ISVALIDHEX(c)        IS_AF(c) || IS_af(c) || IS_09(c)                   //�ж��Ƿ�Ϊ��Ч��ʮ���ַ�
#define ISVALIDDEC(c)        IS_09(c)                                           //�ж��Ƿ�Ϊ��Ч��ʮ�����ַ�
#define CONVERTDEC(c)        (c - '0')                                          //��'0'~'9'֮����ַ�ת��Ϊ0~9֮�������

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))           //��'A'~'F','a'~'f'֮����ַ�ת��Ϊ10-15������
#define CONVERTHEX(c)        (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))       //��ʮ�����Ƶ�һλ�ַ�ת��Ϊ��Ӧ��0~15֮�������

#define SerialPutString(x)   //Serial_PutString((uint8_t*)(x))                    //���ڴ�ӡ�ַ�����


//******************************************************************************
//��������
int  putchar(int ch);                                       //��дputchar����

void Int2Str(uint8_t* str,int32_t intnum);                  //������ת��Ϊ�ַ�������
uint32_t Str2Int(uint8_t *inputstr,int32_t *intnum);        //�ַ���ת��Ϊ����������
uint32_t GetIntegerInput(int32_t * num);                    //�ӳ����жϻ�ȡһ������������
uint32_t SerialKeyPressed(uint8_t *key);                    //���Գ����ն��Ƿ��м����º���
uint8_t  GetKey(void);                                      //�ӳ����жϻ�ȡһ����������
void SerialPutChar(uint8_t c);                              //������������ַ�����
void Serial_PutString(uint8_t *s);                          //��������ַ�������
void GetInputString(uint8_t * buffP);                       //�ӳ����ж϶�ȡһ���ַ�������
uint32_t FLASH_PagesMask(__IO uint32_t Size);               //����ָ����С������Ҫ��ҳ������
void FLASH_DisableWriteProtectionPages(void);               //��ֹ����FLASHҳ��д��������
void Main_Menu(void);                                       //IAP�������˵�����
void SerialDownload(void);                                  //�������س�����
void SerialUpload(void);                                    //�����ϴ�������

#endif  /* _COMMON_H */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
