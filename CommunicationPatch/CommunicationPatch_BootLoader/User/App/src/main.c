/*******************************************************************************
// Copyright(c)2017, ������ھ�����Ϣ�Ƽ��ɷ����޹�˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language��C)
// Date   : 2017-04-01
// Author : ����
// Functiom:��Ŀ���̵�mainԴ�ļ�
*******************************************************************************/


/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;


/******************************************************************************
//��������
*******************************************************************************/
void ButtonInit(void);                                    //������ʼ������
void UsartConfig(void);                                   //USART���ú���


/******************************************************************************
//��������
*******************************************************************************/
//********************************************************
//��������: main
//��������: ϵͳ������
//��    ��: ��
//��    ��: ��
//��    ע: 
//********************************************************
int main(void)
{
    /*
     * ����Flash
     */
    FLASH_Unlock();


    /*
     * ������ʼ��
     */ 
    ButtonInit();
    Internal_Flash_Upgrade_ReadOut();               //��ȡFlash���Ƿ������ı�־  

    /*
     * �ж��Ƿ����ֶ���
     */
    if((SygProgramUpgradeFlag != 0x0000))
    {
        //����ֶ��������£�����IAP���������¶�Flash���
        UsartConfig();                                      //��������
        SerialPutString("\r\n==========================================================");
        SerialPutString("\r\n=                                                        =");
        SerialPutString("\r\n=                    BootLoader                          =");
        SerialPutString("\r\n=                                                        =");
        SerialPutString("\r\n=                    ��дӦ�ó���                         =");
        SerialPutString("\r\n=                                                        =");
        SerialPutString("\r\n==========================================================");
        SerialPutString("\r\n\r\n");

        Main_Menu();                                        //����IAP����
    }
    else
    {                                                       //����Ӧ�ó���
        /*
         * �ж�Ӧ�ó����ַ�Ĵ���ռ��Ƿ񱻱�̹�
         */
        if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
        { 
            /*
             * �����û�Ӧ�ó����ַ
             */
            JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
            Jump_To_Application = (pFunction) JumpAddress;

            /*
             * ��ʼ���û�Ӧ�ó���Ķ�ջָ��
             */
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);

            /*
             * ��ת���û�Ӧ�ó���
             */
            Jump_To_Application();
        }//end of if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    }//end of if(GetButtonStatus() == Bit_RESET)

    while(1);
}


//********************************************************
//��������: ButtonInit
//��������: Button�˿ڳ�ʼ������
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void ButtonInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*
     * USE_BUTTON�˿ڳ�ʼ��: ��������
     */
    RCC_APB2PeriphClockCmd(USE_USART_RX_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = USE_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;      //��������
    GPIO_Init(USE_USART_RX_GPIO_PORT, &GPIO_InitStructure);
}


//********************************************************
//��������: UsartConfig
//��������: Usartģ�����ú���
//��    ��: ��
//��    ��: ��
//��    ע: ��
//********************************************************
void UsartConfig(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /*
     * ��λUSE_USARTģ��
     */
    USART_DeInit(USE_USART);

    /*
     * ʹ��USE_USART_TX����ģ��ʱ��
     */
    RCC_APB2PeriphClockCmd(USE_USART_TX_GPIO_CLK, ENABLE);

    /*
     * ʹ��USE_USART_RX����ģ��ʱ��
     */
    RCC_APB2PeriphClockCmd(USE_USART_RX_GPIO_CLK, ENABLE);

    /*
     * ʹ��USARTģ��ʱ��
     */
    if(USE_USART == USART1)
    {
        RCC_APB2PeriphClockCmd(USE_USART_CLK, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(USE_USART_CLK, ENABLE);
    }//end of if(USE_USART == USART1)


    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /*
     * USE_USART��GPIO����
     */
    //USE_USART_TX: ���츴�����
    GPIO_InitStructure.GPIO_Pin   = USE_USART_TX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;      //���츴�����
    GPIO_Init(USE_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
    //USE_USART_RX: ��������(�����������)
    GPIO_InitStructure.GPIO_Pin   = USE_USART_RX_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;//��������
    GPIO_Init(USE_USART_RX_GPIO_PORT, &GPIO_InitStructure);


    /*
     * USE_USARTģ���������
     * ������: USART1_BAUDRATE��8������λ��1��ֹͣλ����У��λ����Ӳ�������ƣ�ʹ�ܷ��ͺͽ��գ�
     */
    USART_InitStructure.USART_BaudRate            = USE_USART_BAUDRATE;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No ;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USE_USART, &USART_InitStructure); 

    USART_Cmd(USE_USART, ENABLE);                         //ʹ��USE_USARTģ��
}


#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

//////////////////////////////END OF C�ļ�////////////////////////////////////
