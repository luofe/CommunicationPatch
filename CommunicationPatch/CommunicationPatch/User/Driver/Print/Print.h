
/*******************************************************************************
//Print.h
//���Դ�ӡ����ͨ��USART1������ͷ�ļ�
*******************************************************************************/

#ifndef	PRINT_H
#define	PRINT_H

/******************************************************************************
//�궨��
*******************************************************************************/

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

#define SERVER_PRINTF_EN		            1	//�����Ƿ��ӡ��� TRUE/FALSE
#define DEVICE_PRINTF_EN                    1	//�����Ƿ��ӡ��� TRUE/FALSE
#define SERVER_AT_PRINTF_EN                 1	//�����Ƿ��ӡ��� TRUE/FALSE
#define FLASH_PRINTF_EN                     1	//�����Ƿ��ӡ��� TRUE/FALSE

//#define DEBUG_USART_BAUDRATE            115200                       //����DEBUG_USARTͨ�Ų�����
//#define DEBUG_USART                     USART3                       //����DEBUG_USARTʹ�õĴ��ں�
//#define DEBUG_USART_CLK                 RCC_APB1Periph_USART3        //����DEBUG_USARTʹ�õ�ģ��ʱ��
//
//#define DEBUG_USART_IRQn                USART3_IRQn
//#define DEBUG_USART_IRQHandler          USART3_IRQHandler
//
////DEBUG_USART_TX -----------------------------------------------
//#define DEBUG_USART_TX_GPIO_CLK         RCC_APB2Periph_GPIOB         //����DEBUG_USART_TX�˿ڵ�ʱ��
//#define DEBUG_USART_TX_GPIO_PORT        GPIOB                        //����DEBUG_USART_TXռ�õ�GPIO�˿�
//#define DEBUG_USART_TX_GPIO_PIN         GPIO_Pin_10                   //����DEBUG_USART_TX��GPIO���ź�
//
////DEBUG_USART_RX -----------------------------------------------
//#define DEBUG_USART_RX_GPIO_CLK         RCC_APB2Periph_GPIOB         //����DEBUG_USART_RX�˿ڵ�ʱ��
//#define DEBUG_USART_RX_GPIO_PORT        GPIOB                        //����DEBUG_USART_RXռ�õ�GPIO�˿�
//#define DEBUG_USART_RX_GPIO_PIN         GPIO_Pin_11                  //����DEBUG_USART_RX��GPIO���ź�

#define DEBUG_USART_BAUDRATE            115200                       //����DEBUG_USARTͨ�Ų�����
#define DEBUG_USART                     USART1                       //����DEBUG_USARTʹ�õĴ��ں�
#define DEBUG_USART_CLK                 RCC_APB2Periph_USART1        //����DEBUG_USARTʹ�õ�ģ��ʱ��

#define DEBUG_USART_IRQn                USART1_IRQn
#define DEBUG_USART_IRQHandler          USART1_IRQHandler

//DEBUG_USART_TX -----------------------------------------------
#define DEBUG_USART_TX_GPIO_CLK         RCC_APB2Periph_GPIOA         //����DEBUG_USART_TX�˿ڵ�ʱ��
#define DEBUG_USART_TX_GPIO_PORT        GPIOA                        //����DEBUG_USART_TXռ�õ�GPIO�˿�
#define DEBUG_USART_TX_GPIO_PIN         GPIO_Pin_9                   //����DEBUG_USART_TX��GPIO���ź�

//DEBUG_USART_RX -----------------------------------------------
#define DEBUG_USART_RX_GPIO_CLK         RCC_APB2Periph_GPIOA         //����DEBUG_USART_RX�˿ڵ�ʱ��
#define DEBUG_USART_RX_GPIO_PORT        GPIOA                        //����DEBUG_USART_RXռ�õ�GPIO�˿�
#define DEBUG_USART_RX_GPIO_PIN         GPIO_Pin_10                  //����DEBUG_USART_RX��GPIO���ź�







//USART���ճ�ʱ
#define DEBUG_COMM_RX_DATA_TIMEOUT              5	    //û�н��ճ�ʱ5ms���ǽ������






/******************************************************************************
//ö�١��ṹ�����Ͷ���
*******************************************************************************/
//�����õ�USART�������ݵĽṹ�嶨��
typedef struct
{
    u8  RxStatus;                   //��������״̬��־
    u16 RxTimeout_Count;            //���ճ�ʱ����,��λms
    u8  RxBuffer[250];              //�������ݻ�����
    u16 RxIndex;                    //�������ݼ���

    u8  TxBuffer[250];              //�������ݻ�����
    u16 TxIndex;                    //�������ݼ���
}DebugCommStruct;








/******************************************************************************
//��������/����
*******************************************************************************/
//���ڵ��Ե�USART�����ݽṹ��
extern DebugCommStruct s_DebugComm;















/******************************************************************************
//��������
*******************************************************************************/
//����:	������Կڵĳ�ʼ������
void Debug_Init(void);

int putchar(int ch);	//��дputchar ����

//��������: ����Կ�ͨ�ŵļ�⺯��
void Debug_Comm_Rec_Monitor(void);










#endif