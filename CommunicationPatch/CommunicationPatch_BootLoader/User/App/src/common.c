/*******************************************************************************
// Copyright(c)2017, 天津新众聚联信息科技股份有限公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language：C)
// Date   : 2017-04-01
// Author : 刘锋
// Functiom: 与PC机通信相关的源文件
*******************************************************************************/


/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//变量定义
*******************************************************************************/
pFunction       Jump_To_Application;                      //定义应用程序函数指针变量
uint32_t        JumpAddress;                              //定义程序的跳转地址变量
uint32_t        BlockNbr = 0, UserMemoryMask = 0;
__IO uint32_t   FlashProtection = 0;
extern uint32_t FlashDestination;

// 升级文件已经写入Flash内的标志
u8  g_ProgramFlashFileRecFlag = 0;


/******************************************************************************
//函数声明
*******************************************************************************/




/******************************************************************************
//函数定义
*******************************************************************************/
//
//重写putchar函数，供系统库函数调用；
//
//********************************************************
int  putchar(int ch)
{
    USART_SendData(USE_USART, ch);                        //通过USART输出一个字符
    while(USART_GetFlagStatus(USE_USART, USART_FLAG_TXE) == RESET);
                                                          //等待发送完成

    return ch;
}


//********************************************************
//函数名称: Int2Str
//函数功能: 将一个整数转换为字符串函数
//输    入: 
//         *str: uint8，存放转换后字符串的指针；
//         intnum: int32_t，需要被转换的整数；
//输    出: 无
//备    注: 
//********************************************************
void Int2Str(uint8_t* str, int32_t intnum)
{
    uint32_t i, Div = 1000000000, j = 0, Status = 0;

    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + 48;

        intnum = intnum % Div;
        Div /= 10;
        if ((str[j-1] == '0') & (Status == 0))
        {
            j = 0;
        }
        else
        {
            Status++;
        }//end of if ((str[j-1] == '0') & (Status == 0))
    }//end of for (i = 0; i < 10; i++)
}


//********************************************************
//函数名称: Str2Int
//函数功能: 将一个字符串转换为整形函数
//输    入: 
//         *inputstr: uint8，需要被转换的字符串指针；
//         intnum: int32_t，存放转换后整数值的指针；
//输    出: 1: 正确；0: 出错；
//备    注: 
//********************************************************
uint32_t Str2Int(uint8_t *inputstr, int32_t *intnum)
{
    uint32_t i = 0, res = 0;
    uint32_t val = 0;
    
    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
    {
        if (inputstr[2] == '\0')
        {
            return 0;
        }
        for (i = 2; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1; */
                res = 1;
                break;
            }
            if (ISVALIDHEX(inputstr[i]))
            {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* over 8 digit hex --invalid */
        if (i >= 11)
        {
          res = 0;
        }
    }
    else /* max 10-digit decimal input */
    {
        for (i = 0;i < 11;i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1 */
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
            {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
            {
                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            }
            else if (ISVALIDDEC(inputstr[i]))
            {
                val = val * 10 + CONVERTDEC(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* Over 10 digit decimal --invalid */
        if (i >= 11)
        {
            res = 0;
        }
    }
    
    return res;
}


//********************************************************
//函数名称: GetIntegerInput
//函数功能: 从超级终端获取一个整数函数
//输    入: 
//         *num: int32_t，存放整数值的指针；
//输    出: 1: 正确；0: 出错；
//备    注: 
//********************************************************
uint32_t GetIntegerInput(int32_t * num)
{
    uint8_t inputstr[16];

    while (1)
    {
        GetInputString(inputstr);
        if (inputstr[0] == '\0')
            continue;
        if ((inputstr[0] == 'a' || inputstr[0] == 'A') && inputstr[1] == '\0')
        {
            SerialPutString("用户取消！\r\n");
            return 0;
        }
        
        if (Str2Int(inputstr, num) == 0)
        {
            SerialPutString("出错, 请重新输入: \r\n");
        }
        else
        {
            return 1;
        }
    }//end of while (1)
}


//********************************************************
//函数名称: SerialKeyPressed
//函数功能: 测试超级终端是否有按键按下函数
//输    入: 
//         *key: uint8_t，存放超级终端按键键值的指针；
//输    出: 1: 正确；0: 出错；
//备    注: 
//********************************************************
uint32_t SerialKeyPressed(uint8_t *key)
{

    if( USART_GetFlagStatus(USE_USART, USART_FLAG_RXNE) != RESET)
    {
        *key = (uint8_t)USE_USART->DR;
        return 1;
    }
    else
    {
        return 0;
    }
}


//********************************************************
//函数名称: GetKey
//函数功能: 从超级终端获取一个键值函数
//输    入: 无
//输    出: uint8_t,获取的键值；
//备    注: 
//********************************************************
uint8_t GetKey(void)
{
    uint8_t key = 0;

     //等待用户输入
     while (1)
     {
         if (SerialKeyPressed((uint8_t*)&key)) break;
     }

     return key;
}


//********************************************************
//函数名称: SerialPutChar
//函数功能: 打印一个字符串到超级终端函数
//输    入: c，uint8_t，需要打印到超级终端的字符；
//输    出: 无
//备    注: 
//********************************************************
void SerialPutChar(uint8_t c)
{
    USART_SendData(USE_USART, c);
    while (USART_GetFlagStatus(USE_USART, USART_FLAG_TXE) == RESET)
    {
    }
}


//********************************************************
//函数名称: Serial_PutString
//函数功能: 打印字符串到超级终端函数
//输    入: *s，uint8_t，需要打印到超级终端的字符串指针；
//输    出: 无
//备    注: 
//********************************************************
void Serial_PutString(uint8_t *s)
{
    while (*s != '\0')
    {
      SerialPutChar(*s);
      s++;
    }
}


//********************************************************
//函数名称: GetInputString
//函数功能: 从超级终端读取一个字符串函数
//输    入: *buffP，uint8_t，存放读取字符串的指针；
//输    出: 无
//备    注: 
//********************************************************
void GetInputString (uint8_t * buffP)
{
    uint32_t bytes_read = 0;
    uint8_t  c = 0;
    
    do
    {
        c = GetKey();
        if (c == '\r')
          break;
        if (c == '\b') /* Backspace */
        {
            if (bytes_read > 0)
            {
              SerialPutString("\b \b");
              bytes_read --;
            }
            continue;
        }
        if (bytes_read >= CMD_STRING_SIZE )
        {
            SerialPutString("命令字符串大小溢出！\r\n");
            bytes_read = 0;
            continue;
        }
        if (c >= 0x20 && c <= 0x7E)
        {
            buffP[bytes_read++] = c;
            SerialPutChar(c);
        }
    }
    while (1);
    SerialPutString(("\n\r"));
    buffP[bytes_read] = '\0';
}


//********************************************************
//函数名称: FLASH_PagesMask
//函数功能: 计算页数函数
//输    入: 
//          Size: uint32_t，镜像的大小；
//输    出: uint32_t，镜像的页数；
//备    注: 
//********************************************************
uint32_t FLASH_PagesMask(__IO uint32_t Size)
{
    uint32_t pagenumber = 0x0;
    uint32_t size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    
    return pagenumber;
}


//********************************************************
//函数名称: FLASH_DisableWriteProtectionPages
//函数功能: 禁止所需FLASH页的写保护函数
//输    入: 无
//输    出: 无
//备    注: 
//********************************************************
void FLASH_DisableWriteProtectionPages(void)
{
    uint32_t useroptionbyte = 0, WRPR = 0;
    uint16_t var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
    FLASH_Status status = FLASH_BUSY;

    WRPR = FLASH_GetWriteProtectionOptionByte();


    //判断用户存储空间是否进行写保护
    if ((WRPR & UserMemoryMask) != UserMemoryMask)
    {
        useroptionbyte = FLASH_GetUserOptionByte();

        UserMemoryMask |= WRPR;

        status = FLASH_EraseOptionBytes();

        if (UserMemoryMask != 0xFFFFFFFF)
        {
            status = FLASH_EnableWriteProtection((uint32_t)~UserMemoryMask);
        }

        //判断用户操作字是否被编程
        if ((useroptionbyte & 0x07) != 0x07)
        { 
            //恢复用户操作字节
            if ((useroptionbyte & 0x01) == 0x0)
            {
                var1 = OB_IWDG_HW;
            }
            if ((useroptionbyte & 0x02) == 0x0)
            {
                var2 = OB_STOP_RST;
            }
            if ((useroptionbyte & 0x04) == 0x0)
            {
                var3 = OB_STDBY_RST;
            }

            FLASH_UserOptionByteConfig(var1, var2, var3);
        }
        
        if (status == FLASH_COMPLETE)
        {
            SerialPutString("禁止写保护...\r\n");
            SerialPutString("...系统将进行复位来载入新的操作码\r\n");

            //产生系统复位来载入新的操作字值
            NVIC_SystemReset();
        }
        else
        {
            SerialPutString("错误: Flash解除写保护失败...\r\n");
        }
    }
    else
    {
        SerialPutString("Flash存储空间未写保护...\r\n");
    }
}


//********************************************************
//函数名称: Main_Menu
//函数功能: 在超级终端上显示主菜单函数
//输    入: 无
//输    出: 无
//备    注: 
//********************************************************
void Main_Menu(void)
{
//    uint8_t key = 0;
    
    /* Get the number of block (4 or 2 pages) from where the user program will be loaded */
    BlockNbr = (FlashDestination - 0x08000000) >> 12;
    
    /* Compute the mask to test if the Flash memory, where the user program will be
       loaded, is write protected */
#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
    UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
#else
    if (BlockNbr < 62)
    {
        UserMemoryMask = ((uint32_t)~((1 << BlockNbr) - 1));
    }
    else
    {
        UserMemoryMask = ((uint32_t)0x80000000);
    }
#endif


    /*
     * 检查应用程序将要被载入的Flash存储器的页是否为写保护
     */
    if ((FLASH_GetWriteProtectionOptionByte() & UserMemoryMask) != UserMemoryMask)
    {
        FlashProtection = 1;
    }
    else
    {
        FlashProtection = 0;
    }


    while (1)
    {
        /*
         * 打印菜单信息
         */
        SerialPutString("\r\n检测是否存有升级标志\r\n\n");

        SerialPutString("1     -----------------更新应用程序\r\n\n");
        SerialPutString("0/FFFF-----------------启动应用程序\r\n\n");
        if(FlashProtection != 0)
        {
            //若所需的FLASH页写保护，添加写保护菜单选项
            SerialPutString("3   禁止写保护\r\n\n");
        }//end of if(FlashProtection != 0)
        
        SerialDownload();
    }//end of while (1)
}


//////////////////////////////END OF C文件////////////////////////////////////
