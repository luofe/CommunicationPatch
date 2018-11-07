/*******************************************************************************
// Copyright(c)2017, 天津新众聚联信息科技股份有限公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language：C)
// Date   : 2017-04-01
// Author : 刘锋
// Functiom: 从PC机下载固件到单片机的源文件
*******************************************************************************/ 


/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"


/******************************************************************************
//变量定义
*******************************************************************************/
extern uint8_t file_name[FILE_NAME_LENGTH];
uint8_t tab_1024[1024] =
{
    0
};                                                        //定义1K的缓冲区变量

// 从临时应用程序存储区读取出来数据转存到实际应用程序存储区的缓存空间
int32_t AppSpaceBuf[(PAGE_SIZE / 4)] = {0};



/******************************************************************************
//函数声明
*******************************************************************************/



/******************************************************************************
//函数定义
*******************************************************************************/
//********************************************************
//函数名称: SerialDownload
//函数功能: 通过串口下载一个文件函数
//输    入: 无
//输    出: 无
//备    注: 
//********************************************************
void SerialDownload(void)
{
    uint8_t Number[10] = "          ";
    int32_t Size = 0;
    
    SerialPutString("等待发送文件 ... (按 'a' 取消)\n\r");
    Size = Ymodem_Receive(&tab_1024[0]);
    if (Size > 0)
    {
        SerialPutString("\n\n\r编程成功!\r\n Name: ");
        SerialPutString(file_name);
        Int2Str(Number, Size);
        Serial_PutString("\n\rSize: ");
        Serial_PutString(Number);
        Serial_PutString("Bytes\r\n");
        
        //设置用户应用程序地址
        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
        pFunction Jump_To_Application = (pFunction) JumpAddress;
        
        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//初始化用户应用程序的堆栈指针
        
        Jump_To_Application();                          //跳转到用户应用程序
    }
//    else if (Size == -1)
//    {
//        //镜像文件大于允许的存储空间
//        SerialPutString("\n\n\r下载程序的大小大于可用的存储空间!\n\r");
//        
//        //设置用户应用程序地址
//        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//        pFunction Jump_To_Application = (pFunction) JumpAddress;
//        
//        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//初始化用户应用程序的堆栈指针
//        
//        Jump_To_Application();                          //跳转到用户应用程序
//    }
//    else if (Size == -2)
//    {
//        //校验失败
//        SerialPutString("\n\n\r校验失败!\n\r");
//        
//        //设置用户应用程序地址
//        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//        pFunction Jump_To_Application = (pFunction) JumpAddress;
//        
//        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//初始化用户应用程序的堆栈指针
//        
//        Jump_To_Application();                          //跳转到用户应用程序
//    }
//    else if (Size == -3)
//    {
//        //用户中止
//        SerialPutString("\r\n\n用户取消!\n\r");
//        
//        //设置用户应用程序地址
//        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//        pFunction Jump_To_Application = (pFunction) JumpAddress;
//        
//        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//初始化用户应用程序的堆栈指针
//        
//        Jump_To_Application();                          //跳转到用户应用程序
//    }
//    else if (Size == -4)
//    {
//        //超时上位机没有升级操作，则退出
//        SerialPutString("\r\n\n超时退出升级!\n\r");
//        
//        //设置用户应用程序地址
//        uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
//        pFunction Jump_To_Application = (pFunction) JumpAddress;
//        
//        __set_MSP(*(__IO uint32_t*) ApplicationAddress);//初始化用户应用程序的堆栈指针
//        
//        Jump_To_Application();                          //跳转到用户应用程序
//    }
    else if (Size == -5)
    {
        //超时上位机没有升级操作，则退出
        SerialPutString("\r\n\n从临时存储区拷贝到实际存储区时出错!\n\r");
    }
    else
    {
        // 先判断是否有应用程序
        AppSpaceBuf[0] = *((uint32_t*)ApplicationAddress); //读数据
        if((AppSpaceBuf[0] != 0xFFFFFFFF) && (AppSpaceBuf[0] != 0)) //不为空，则说明有应用程序
        {
            //设置用户应用程序地址
            uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
            pFunction Jump_To_Application = (pFunction) JumpAddress;
            
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);//初始化用户应用程序的堆栈指针
            
            Jump_To_Application();                          //跳转到用户应用程序
        }
    }//end of if (Size > 0)
}


//////////////////////////////END OF C文件////////////////////////////////////
