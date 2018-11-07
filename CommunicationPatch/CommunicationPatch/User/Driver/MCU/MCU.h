
/*
  bsp.h  ��Ƭ����ʼ����������ͷ�ļ�
*/
#ifndef MCU_H
#define MCU_H

/******************************************************************************
//�궨��
*******************************************************************************/


/******************************************************************************
//��������/����
*******************************************************************************/
// USBʹ�ܿ��ƿ�
#define USB_ENABLE_CTR_GPIO_PIN             GPIO_Pin_12         //�����GPIO���ź�
#define USB_ENABLE_CTR_GPIO_PORT            GPIOB               //����ʹ�õ�GPIO�˿�
#define USB_ENABLE_CTR_GPIO_CLK             RCC_APB2Periph_GPIOB//����˿ڵ�ʱ��
#define USB_ENABLE_CTR_GPIO_SOURCE          GPIO_PinSource12    //�������ź�
// USBʹ�ܿ���
#define SetUSBEnable()                      GPIO_SetBits(USB_ENABLE_CTR_GPIO_PORT, USB_ENABLE_CTR_GPIO_PIN)
#define SetUSBDisable()                     GPIO_ResetBits(USB_ENABLE_CTR_GPIO_PORT, USB_ENABLE_CTR_GPIO_PIN)


/******************************************************************************
//��������
*******************************************************************************/

void MCU_Init(void);             //�弶��ʼ������

//��������: ��Ƭ��ʱ�Ӳ��Ժ���
void RCC_Test(void);







#endif

/**********************************END OF FILE*********************************/

