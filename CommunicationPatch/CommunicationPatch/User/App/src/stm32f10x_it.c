
/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
// Functiom:项目工程的所有中断处理的源文件
*******************************************************************************/



/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//变量定义
*******************************************************************************/










/******************************************************************************
//函数声明
*******************************************************************************/










/******************************************************************************
//函数定义
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
printf("硬件错误！！！！！！！！！！！！\r\n");
printf("复位！！！！！！！！！！！！\r\n");
#endif	

        __set_FAULTMASK(1);      // 关闭所有中断
        NVIC_SystemReset();// 复位
    
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
//名称:	SysTick_Handler
//功能:	系统滴答中断函数
//入口参数:	无
//出口参数:	无
**********************************/
void SysTick_Handler(void)
{
	g_ms_Timing_Count++;	            //ms计时
    
    s_ServerCommRx.Timeout_Count++;         //接收服务器数据超时等待计数
    
    s_DeviceCommRx.Timeout_Count++;     //接收设备端的数据超时等待计数
    
    s_DebugComm.RxTimeout_Count++;      //与调试口通信的数据接收等待超时计数
    
    g_SysPollTimeCnt++;                 //发送Poll的时间计数
    
    g_GetPulloutStatusTimeCnt++;        //查询设备端工作状态的时间计数
    
    g_SysTempCtrTimeCnt++;              //系统温度控制的时间计数
    
    g_SysWorkTimeJudgeCnt++;            //系统工作时间判断的计时

    s_ServerCommTx.WaitResponseTimeout++;   //与服务器通信时等待服务器应答数据包的计数

    s_DeviceCommTx.WaitResponseTimeout++;   //与设备端通信时等待服务器应答数据包的计数
}


/******************************************************************************/
/*            STM32 Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**********************************
//名称:	SERVER_COMM_USART_IRQHandler
//功能:	与设备端通信的USART中断函数
//入口参数:	无
//出口参数:	无
**********************************/
void SERVER_COMM_USART_IRQHandler(void)
{
	if(USART_GetITStatus(SERVER_COMM_USART, USART_IT_RXNE) == SET)	//假如有接收中断
	{
        s_ServerCommRx.Status = TRUE;
        if(s_ServerCommRx.Index >= PC_COM_MAX_DATA_LENGTH)  //防止溢出
        {
            s_ServerCommRx.Index = 0;
        }
        s_ServerCommRx.Buffer[s_ServerCommRx.Index++] = USART_ReceiveData(SERVER_COMM_USART);
        s_ServerCommRx.Timeout_Count = 0;
	}
    
	if (USART_GetFlagStatus(SERVER_COMM_USART, USART_FLAG_ORE) != RESET)//注意！必须要清除ORE中断
	{
		USART_ReceiveData(SERVER_COMM_USART);
	}
}

/**********************************
//名称:	DEVICE_COMM_USART_IRQHandler
//功能:	与设备端通信的USART中断函数
//入口参数:	无
//出口参数:	无
**********************************/
void DEVICE_COMM_USART_IRQHandler(void)
{
	if(USART_GetITStatus(DEVICE_COMM_USART, USART_IT_RXNE) == SET)	//假如有接收中断
	{
        s_DeviceCommRx.Status = TRUE;
        if(s_DeviceCommRx.Index >= DEVICE_COMM_DATA_MAX_LENGTH)    //防止溢出
        {
            s_DeviceCommRx.Index = 0;
        }
        s_DeviceCommRx.Buffer[s_DeviceCommRx.Index++] = USART_ReceiveData(DEVICE_COMM_USART);
        s_DeviceCommRx.Timeout_Count = 0;
	}
    
	if (USART_GetFlagStatus(DEVICE_COMM_USART, USART_FLAG_ORE) != RESET)//注意！必须要清除ORE中断
	{
		USART_ReceiveData(DEVICE_COMM_USART);
	}
}

/**********************************
//名称:	DEBUG_USART_IRQHandler
//功能:	调试打印的USART中断函数
//入口参数:	无
//出口参数:	无
**********************************/
void DEBUG_USART_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USART, USART_IT_RXNE) == SET)	//假如有接收中断
	{
        s_DebugComm.RxStatus = TRUE;
        if(s_DebugComm.RxIndex >= 250)  //防止溢出
        {
            s_DebugComm.RxIndex = 0;
        }
        s_DebugComm.RxBuffer[s_DebugComm.RxIndex++] = USART_ReceiveData(DEBUG_USART);
        s_DebugComm.RxTimeout_Count = 0;
	}
    
	if (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_ORE) != RESET)//注意！必须要清除ORE中断
	{
		USART_ReceiveData(DEBUG_USART);
	}
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
