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
//函数指针类型定义
typedef  void (*pFunction)(void);


//******************************************************************************
//函数宏定义
#define CMD_STRING_SIZE      128                                                //串口命令行模式下输入字符串的最大长度

#define FLASH_IMAGE_SIZE     (uint32_t) (FLASH_SIZE - (ApplicationAddress - 0x08000000))
                                                                                //计算Flash上传镜像大小


//******************************************************************************
//函数宏定义
#define IS_AF(c)             ((c >= 'A') && (c <= 'F'))                         //判断是否为A~F之间的字符
#define IS_af(c)             ((c >= 'a') && (c <= 'f'))                         //判断是否为a~f之间的字符
#define IS_09(c)             ((c >= '0') && (c <= '9'))                         //判断是否为0~9之间的数字
#define ISVALIDHEX(c)        IS_AF(c) || IS_af(c) || IS_09(c)                   //判断是否为有效的十六字符
#define ISVALIDDEC(c)        IS_09(c)                                           //判断是否为有效的十进制字符
#define CONVERTDEC(c)        (c - '0')                                          //将'0'~'9'之间的字符转换为0~9之间的数字

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))           //将'A'~'F','a'~'f'之间的字符转换为10-15的数字
#define CONVERTHEX(c)        (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))       //将十六进制的一位字符转换为对应的0~15之间的数字

#define SerialPutString(x)   //Serial_PutString((uint8_t*)(x))                    //串口打印字符串宏


//******************************************************************************
//函数声明
int  putchar(int ch);                                       //重写putchar函数

void Int2Str(uint8_t* str,int32_t intnum);                  //整形数转换为字符串函数
uint32_t Str2Int(uint8_t *inputstr,int32_t *intnum);        //字符串转换为整形数函数
uint32_t GetIntegerInput(int32_t * num);                    //从超级中断获取一个整形数函数
uint32_t SerialKeyPressed(uint8_t *key);                    //测试超级终端是否有键按下函数
uint8_t  GetKey(void);                                      //从超级中断获取一个按键函数
void SerialPutChar(uint8_t c);                              //串行输出单个字符函数
void Serial_PutString(uint8_t *s);                          //串行输出字符串函数
void GetInputString(uint8_t * buffP);                       //从超级中断读取一个字符串函数
uint32_t FLASH_PagesMask(__IO uint32_t Size);               //计算指定大小镜像需要的页数函数
void FLASH_DisableWriteProtectionPages(void);               //禁止所需FLASH页的写保护函数
void Main_Menu(void);                                       //IAP程序主菜单函数
void SerialDownload(void);                                  //串行下载程序函数
void SerialUpload(void);                                    //串行上传程序函数

#endif  /* _COMMON_H */

/*******************(C)COPYRIGHT 2010 STMicroelectronics *****END OF FILE******/
