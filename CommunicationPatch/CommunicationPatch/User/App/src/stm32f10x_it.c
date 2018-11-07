
/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:��Ŀ���̵������жϴ����Դ�ļ�
*******************************************************************************/



/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//��������
*******************************************************************************/










/******************************************************************************
//��������
*******************************************************************************/










/******************************************************************************
//��������
*******************************************************************************/
/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
        
#if (SERVER_PRINTF_EN)
printf("Ӳ�����󣡣���������������������\r\n");
printf("��λ������������������������\r\n");
#endif	

        __set_FAULTMASK(1);      // �ر������ж�
        NVIC_SystemReset();// ��λ
    
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/**********************************
//����:	SysTick_Handler
//����:	ϵͳ�δ��жϺ���
//��ڲ���:	��
//���ڲ���:	��
**********************************/
void SysTick_Handler(void)
{
	g_ms_Timing_Count++;	            //ms��ʱ
    
    s_ServerCommRx.Timeout_Count++;         //���շ��������ݳ�ʱ�ȴ�����
    
    s_DeviceCommRx.Timeout_Count++;     //�����豸�˵����ݳ�ʱ�ȴ�����
    
    s_DebugComm.RxTimeout_Count++;      //����Կ�ͨ�ŵ����ݽ��յȴ���ʱ����
    
    g_SysPollTimeCnt++;                 //����Poll��ʱ�����
    
    g_GetPulloutStatusTimeCnt++;        //��ѯ�豸�˹���״̬��ʱ�����
    
    g_SysTempCtrTimeCnt++;              //ϵͳ�¶ȿ��Ƶ�ʱ�����
    
    g_SysWorkTimeJudgeCnt++;            //ϵͳ����ʱ���жϵļ�ʱ

    s_ServerCommTx.WaitResponseTimeout++;   //�������ͨ��ʱ�ȴ�������Ӧ�����ݰ��ļ���

    s_DeviceCommTx.WaitResponseTimeout++;   //���豸��ͨ��ʱ�ȴ�������Ӧ�����ݰ��ļ���
}


/******************************************************************************/
/*            STM32 Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**********************************
//����:	SERVER_COMM_USART_IRQHandler
//����:	���豸��ͨ�ŵ�USART�жϺ���
//��ڲ���:	��
//���ڲ���:	��
**********************************/
void SERVER_COMM_USART_IRQHandler(void)
{
	if(USART_GetITStatus(SERVER_COMM_USART, USART_IT_RXNE) == SET)	//�����н����ж�
	{
        s_ServerCommRx.Status = TRUE;
        if(s_ServerCommRx.Index >= PC_COM_MAX_DATA_LENGTH)  //��ֹ���
        {
            s_ServerCommRx.Index = 0;
        }
        s_ServerCommRx.Buffer[s_ServerCommRx.Index++] = USART_ReceiveData(SERVER_COMM_USART);
        s_ServerCommRx.Timeout_Count = 0;
	}
    
	if (USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_ORE) != RESET)//ע�⣡����Ҫ���ORE�ж�
	{
		USART_ReceiveData(SERVER_COMM_USART);
	}
}

/**********************************
//����:	DEVICE_COMM_USART_IRQHandler
//����:	���豸��ͨ�ŵ�USART�жϺ���
//��ڲ���:	��
//���ڲ���:	��
**********************************/
void DEVICE_COMM_USART_IRQHandler(void)
{
	if(USART_GetITStatus(DEVICE_COMM_USART, USART_IT_RXNE) == SET)	//�����н����ж�
	{
        s_DeviceCommRx.Status = TRUE;
        if(s_DeviceCommRx.Index >= DEVICE_COMM_DATA_MAX_LENGTH)    //��ֹ���
        {
            s_DeviceCommRx.Index = 0;
        }
        s_DeviceCommRx.Buffer[s_DeviceCommRx.Index++] = USART_ReceiveData(DEVICE_COMM_USART);
        s_DeviceCommRx.Timeout_Count = 0;
	}
    
	if (USART_GetFlagStatus(DEVICE_COMM_USART, USART_FLAG_ORE) != RESET)//ע�⣡����Ҫ���ORE�ж�
	{
		USART_ReceiveData(DEVICE_COMM_USART);
	}
}

/**********************************
//����:	DEBUG_USART_IRQHandler
//����:	���Դ�ӡ��USART�жϺ���
//��ڲ���:	��
//���ڲ���:	��
**********************************/
void DEBUG_USART_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) == SET)	//�����н����ж�
	{
        s_DebugComm.RxStatus = TRUE;
        if(s_DebugComm.RxIndex >= 250)  //��ֹ���
        {
            s_DebugComm.RxIndex = 0;
        }
        s_DebugComm.RxBuffer[s_DebugComm.RxIndex++] = USART_ReceiveData(DEBUG_USART);
        s_DebugComm.RxTimeout_Count = 0;
	}
    
	if (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_ORE) != RESET)//ע�⣡����Ҫ���ORE�ж�
	{
		USART_ReceiveData(DEBUG_USART);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
