/*******************************************************************************
// Copyright(c)2017, 天津新众聚联信息科技股份有限公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103VCT6
// Built  : IAR For ARM v7.40(Language：C)
// Date   : 2017-04-01
// Author : 刘锋
// Functiom:项目工程使用内部Flash存储各项参数的源程序。
//备注: 
1、大容量的是2K/页，中容量和小容量的是1K/页；
2、大容量的是有256页，中容量的是有128页，小容量的是有32页；

//主要存储内容: 	
1、
*******************************************************************************/

/******************************************************************************
//包含头文件
*******************************************************************************/
#include "includes.h"




/******************************************************************************
//变量定义
*******************************************************************************/
//固件升级标志
u8 SygProgramUpgradeFlag = 0;













/******************************************************************************
//函数声明
*******************************************************************************/
//功能:	单片机片内Flash存储按2字节读取
void Internal_Flash_Read_HalfWord(u32 addr, u16* buffer, u16 length);

//功能:	单片机片内Flash存储按2字节写入
u8 Internal_Flash_Write_HalfWord(u32 addr, u16 *buffer, u16 length);

//功能:	读取片内Flash的是否固件升级的标志
void Internal_Flash_Upgrade_ReadOut(void);

//功能:	存储是否固件升级的标志到片内Flash
void Internal_Flash_Upgrade_StoreIn(void);

//功能:	单片机片内Flash存储测试 
void Internal_Flash_Test(void);














/******************************************************************************
//函数定义
*******************************************************************************/
/**********************************
//名称:	Internal_Flash_Read_HalfWord
//功能:	单片机片内Flash存储按2字节读取
//入口参数:	u32 addr——读取数据的起始地址；uint8 *buffer——存放读取数据的指针；u16 length——读取数据长度；
//出口参数:	无
**********************************/ 
void Internal_Flash_Read_HalfWord(u32 addr, u16* buffer, u16 length)
{
    volatile u16 i = 0;
    addr = INTERNAL_FLASH_START_ADDR + addr;	//地址指针指向读取地址
    
    for (i = 0; i < length; i++)
    {
        buffer[i] = *((u16*)addr); //读数据
        addr += 2;  
    }
}

/**********************************
//名称:	Internal_Flash_Write_HalfWord
//功能:	单片机片内Flash存储按2字节写入
//入口参数:	
	u32 addr——写数据的起始地址；u8 *buffer——写入数据的指针；u16 length——写入数据长度；
//出口参数:	u8 成功或者失败
**********************************/ 
u8 Internal_Flash_Write_HalfWord(u32 addr, u16 *buffer, u16 length)
{
    volatile u16 i = 0;
    FLASH_Status sta;
    
    addr = (u32)(addr + INTERNAL_FLASH_START_ADDR);//地址指针指向读取地址

    FLASH_Unlock();            //解锁，方可擦除
    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);//清标志位
    
    sta = FLASH_ErasePage(INTERNAL_FLASH_START_ADDR); //擦除255页
    if(FLASH_COMPLETE != sta)         
    {
        FLASH_Lock();
        return 1; 
    }
    
    for(i = 0; i < length; i++)              //写入数据
    {     
        sta = FLASH_ProgramHalfWord(addr, buffer[i]);
        if (FLASH_COMPLETE != sta)
        {
            FLASH_Lock();
            return 1;
        }  
        
        addr += 2;               //16位数据，地址加2
    }
    
    FLASH_Lock();                     //上锁
    
    return 0;
                
}	

/**********************************
//名称:	Internal_Flash_Upgrade_ReadOut
//功能:	读取片内Flash的是否固件升级的标志
//入口参数:	无
//出口参数:	无
**********************************/ 
void Internal_Flash_Upgrade_ReadOut(void)
{        
    //存储到Flash内的坐标数据
    u16 temp_array[1];
    
    //读取出坐标数据
    Internal_Flash_Read_HalfWord(FLASH_ADD_PROGRAM_UPGRADE_FLAG, temp_array, 1); 
    
    SygProgramUpgradeFlag = temp_array[0]; 
}

/**********************************
//名称:	Internal_Flash_Upgrade_StoreIn
//功能:	存储是否固件升级的标志到片内Flash
//入口参数:	无
//出口参数:	无
**********************************/ 
void Internal_Flash_Upgrade_StoreIn(void)
{      
    u8  i = 10;
    u16 temp_array[1];
    
    temp_array[0] = SygProgramUpgradeFlag;
    
    //存入温度策略数据
    while(i--)
    {
        if(Internal_Flash_Write_HalfWord(FLASH_ADD_PROGRAM_UPGRADE_FLAG, temp_array, 1) == 0)
        {
            break;
        }
    }
}

/**********************************
//名称:	Internal_Flash_Test
//功能:	单片机片内Flash存储测试
//入口参数:	无
//出口参数:	无
**********************************/ 
void Internal_Flash_Test(void)
{
    u16 i = 0, j = 18;
    
    u16 Internal_Flash_Data[60] = {0};
        
    Internal_Flash_Read_HalfWord(FLASH_ADD_TEMPERATURE_STORE_INFO, Internal_Flash_Data, 60); 
        
#if (USART_PRINTF_EN)
printf("读取的数据是:\r\n");
for(u16 x = 0; x < 60; x++)
{
    printf("0x%02X", (u8)(Internal_Flash_Data[x] >> 8));
    printf("%02X ", (u8)(Internal_Flash_Data[x]));
}
printf("\r\n");
#endif          

    for(i = 0; i < 60; i++)
    {
        Internal_Flash_Data[i] = i * j ;
    }

    Internal_Flash_Write_HalfWord(FLASH_ADD_TEMPERATURE_STORE_INFO,Internal_Flash_Data,60);	//将MAC地址写入内部Flash
        
#if (USART_PRINTF_EN)
printf("写入的数据是:\r\n");
for(u16 x = 0; x < INTERNAL_FLASH_MAX_LENGTH; x++)
{
    printf("0x%02X", (u8)(Internal_Flash_Data[x] >> 8));
    printf("%02X ", (u8)(Internal_Flash_Data[x]));
}
printf("\r\n");
#endif          
         
    for(i = 0; i < 60; i++)
    {
        Internal_Flash_Data[i] = 0;
    }

    Internal_Flash_Read_HalfWord(FLASH_ADD_TEMPERATURE_STORE_INFO, Internal_Flash_Data, 60); 
        
#if (USART_PRINTF_EN)
printf("读取的数据是:\r\n");
for(u16 x = 0; x < INTERNAL_FLASH_MAX_LENGTH; x++)
{
    printf("0x%02X", (u8)(Internal_Flash_Data[x] >> 8));
    printf("%02X ", (u8)(Internal_Flash_Data[x]));
}
printf("\r\n");
#endif     
        
    while(9);

//    j++;
//
//    Delay_ms(2000);
}

