/*******************************************************************************
// Copyright(c)2017, ������ھ�����Ϣ�Ƽ��ɷ����޹�˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language��C)
// Date   : 2017-04-01
// Author : ����
// Functiom:��Ŀ���̵���Ҫ�����ļ�
*******************************************************************************/

#ifndef IAP_CFG_H
#define IAP_TCFG_H




/******************************************************************************
//�궨��
*******************************************************************************/

#define BOOTLOADER_SIZE         0x2000                      //Bootloader��������Ŀռ䣬��Ӧ�ó���ʵ�����е�ַƫ����
/*
 * ���õ�ַ�����Ϣ
 */
#define ApplicationAddress      (0x8000000 + BOOTLOADER_SIZE) //�����û�Ӧ�ó������ʼ��ַ

#define PAGE_SIZE                (0x400)                      //����FLASHҳ��С��1 Kbytes(С��������������1K)
#define FLASH_SIZE               (0x20000)                    //����FLASH�ռ��С��128 KBytes


// ��ʱ���Ӧ�ó���
#define ApplicationTempAddress  (0x8000000 + ((FLASH_SIZE - (PAGE_SIZE * 2) + BOOTLOADER_SIZE) / 2))  //�����û�Ӧ�ó������ʼ��ַ0x8004000
// ��ʱ���Ӧ�ó���Ŀռ��С
#define ApplicationTempSize     ((FLASH_SIZE - BOOTLOADER_SIZE - (PAGE_SIZE * 2)) / 2)

/*
 * ����Ymodemͨ��Э�鳬ʱ����������������Ϣ(�򽵵�ϵͳ��Ƶ)
 */
#define NAK_TIMEOUT             (0x200000)
#define MAX_ERRORS              (5)

#define WAIT_UPGRADE_TIMEOUT_MAX_COUNT      30              //�ȴ����������ַ���C������30�ξ��Զ�����Ӧ�ó���

/*
 * ���ô�������
 */
//USE_BUTTON-------------------------------------------------- PB2
//#define USE_BUTTON_GPIO_PORT     GPIOA                        //����USE_BUTTONʹ�õĵ�GPIO�˿�
//#define USE_BUTTON_GPIO_CLK      RCC_APB2Periph_GPIOA         //����USE_BUTTON�˿ڵ�ʱ��
//#define USE_BUTTON_GPIO_PIN      GPIO_Pin_10                   //����USE_BUTTON��GPIO���ź�
//
//#define GetButtonStatus()        GPIO_ReadInputDataBit(USE_BUTTON_GPIO_PORT, USE_BUTTON_GPIO_PIN)
                                                              //��ȡ������״̬


/*
 * ����ʹ�õĴ���
 */
#define USE_USART_BAUDRATE       9600                       //����USE_USARTͨ�Ų�����
#define USE_USART                USART3                       //����USE_USARTʹ�õĴ��ں�
#define USE_USART_CLK            RCC_APB1Periph_USART3        //����USE_USARTʹ�õ�ģ��ʱ��

//USE_USART_TX ----------------------------------------------- (USART1_TX��PA9)
#define USE_USART_TX_GPIO_CLK    RCC_APB2Periph_GPIOB         //����USE_USART_TX�˿ڵ�ʱ��
#define USE_USART_TX_GPIO_PORT   GPIOB                        //����USE_USART_TXռ�õ�GPIO�˿�
#define USE_USART_TX_GPIO_PIN    GPIO_Pin_10                   //����USE_USART_TX��GPIO���ź�

//USE_USART_RX ----------------------------------------------- (USART1_RX��PA10)
#define USE_USART_RX_GPIO_CLK    RCC_APB2Periph_GPIOB         //����USE_USART_RX�˿ڵ�ʱ��
#define USE_USART_RX_GPIO_PORT   GPIOB                        //����USE_USART_RXռ�õ�GPIO�˿�
#define USE_USART_RX_GPIO_PIN    GPIO_Pin_11                  //����USE_USART_RX��GPIO���ź�

#define GetButtonStatus()        GPIO_ReadInputDataBit(USE_USART_RX_GPIO_PORT, USE_USART_RX_GPIO_PIN)

/******************************************************************************
//��������/����
*******************************************************************************/
// �����ļ��Ѿ�д��Flash�ڵı�־
extern u8  g_ProgramFlashFileRecFlag;



/******************************************************************************
//��������
*******************************************************************************/





#endif
