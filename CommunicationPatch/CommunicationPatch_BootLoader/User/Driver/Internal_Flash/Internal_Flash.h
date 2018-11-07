/*******************************************************************************
//Internal_Flash.h
//片内Flash存储头文件
*******************************************************************************/

#ifndef	INTERNAL_FLASH_H
#define	INTERNAL_FLASH_H

/******************************************************************************
//宏定义
*******************************************************************************/
#define ENABLE_INT()                        __enable_irq();     //系统开全局中断  
#define DISABLE_INT()                       __disable_irq();    //系统关全局中断 

//存储地址
#define INTERNAL_FLASH_END_ADDR            ((u32)(0x08000000 + FLASH_SIZE))	        //结束地址，有1页的字节
#define INTERNAL_FLASH_START_ADDR          (INTERNAL_FLASH_END_ADDR - PAGE_SIZE)    //起始存储地址为最后一页


//存储数据的偏移地址
#define FLASH_ADD_TEMPERATURE_STORE_INFO    0                   //存储温度策略标志的偏移地址
#define FLASH_ADD_THERMOSTAT_MAX_NUM        1                   //存储保温箱数量的偏移地址

#define FLASH_ADD_PROGRAM_UPGRADE_FLAG      0                   //存储是否固件要升级的偏移地址






/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
















/******************************************************************************
//变量定义/声明
*******************************************************************************/
//固件升级标志
extern u8 SygProgramUpgradeFlag;















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

#endif