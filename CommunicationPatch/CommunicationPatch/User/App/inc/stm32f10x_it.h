
/**
  ******************************************************************************
  * @file    stm32f10x_it.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_IT_H
#define __STM32_IT_H

/* Includes ------------------------------------------------------------------*/
//#include "platform_config.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

//功能:	系统滴答中断函数
void SysTick_Handler(void);

//功能:	与设备端通信的USART中断函数
void SERVER_COMM_USART_IRQHandler(void);

//功能:	与设备端通信的USART中断函数
void DEVICE_COMM_USART_IRQHandler(void);

//功能:	调试打印的USART中断函数
void DEBUG_USART_IRQHandler(void);

//功能:	面板按键1的中断处理函数
void PANEL_BUTTON1_IRQHandler(void);

//功能:	面板按键2的中断处理函数
void PANEL_BUTTON2_IRQHandler(void);

//功能:	面板按键3的中断处理函数
void PANEL_BUTTON3_IRQHandler(void);

//功能:	面板按键4的中断处理函数
void PANEL_BUTTON4_IRQHandler(void);









					 
#endif /* __STM32_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

