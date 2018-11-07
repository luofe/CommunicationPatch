
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

#define PAGE_SIZE                (0x400)                      //定义FLASH页大小：1 Kbytes(小容量和中容量是1K)
#define FLASH_SIZE               (0x20000)                    //定义FLASH空间大小：128 KBytes


#define INTERNAL_FLASH_MAX_LENGTH	        10//(1 + (1 + VMC_MAX_THERMOSTAT_NUM * 2 + 2) + 1)    //存储数据最大长度 = 温度策略存储标志 + 温度策略结构体 + 设备编号

//存储地址
#define DATA_EEPROM_END_ADDR                ((u32)(0x08000000 + FLASH_SIZE) - PAGE_SIZE)	        //结束地址，有1页的字节
#define DATA_EEPROM_START_ADDR              (DATA_EEPROM_END_ADDR - PAGE_SIZE)	//起始存储地址为倒数第二页


//存储数据的偏移地址
#define FLASH_ADD_TEMPERATURE_STORE_INFO    0                   //存储温度策略标志的偏移地址
#define FLASH_ADD_SYSTEM_DEVICE_ID          6                   //存储设备编号的偏移地址



#define FLASH_ADD_SYSTEM_ERROR_LOG          72                  //存储各种错误标志的偏移地址


//固件升级存储地址
#define INTERNAL_FLASH_END_ADDR            ((u32)(0x08000000 + FLASH_SIZE))	        //结束地址，有1页的字节
#define INTERNAL_FLASH_START_ADDR          (INTERNAL_FLASH_END_ADDR - PAGE_SIZE)    //起始存储地址为最后一页

#define FLASH_ADD_PROGRAM_UPGRADE_FLAG      0               //存储是否固件要升级的偏移地址

/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
//存储的参数类型
typedef enum
{
    FLASH_STORE_TEMPERATURN     = 0,    //存储温度策略
    FLASH_STORE_DEVICE_ID,              //存储设备编号
}FlashStoreParamEnum;















/******************************************************************************
//变量定义/声明
*******************************************************************************/
//温度策略是否存储的标志
extern u8 SysTempInfoStoredFlag;

//固件升级标志
extern u8 SygProgramUpgradeFlag;

// 存储的起始地址和结束地址
extern u32  g_InternalFlashStartAddr;	















/******************************************************************************
//函数声明
*******************************************************************************/
//功能:	单片机片内Flash存储按2字节读取
void Internal_Flash_Read_HalfWord(u32 addr, u16* buffer, u16 length);

//功能:	单片机片内Flash存储按2字节写入
u8 Internal_Flash_Write_HalfWord(u32 addr, u16 *buffer, u16 length);

//功能:	读取片内Flash的数据
void Internal_Flash_ReadOut(void);

//功能:	将数据结构存入Flash
void Internal_Flash_StoreIn(u8 store_param_type);

//功能:	读取片内Flash的是否固件升级的标志
void Internal_Flash_Upgrade_ReadOut(void);

//功能:	存储是否固件升级的标志到片内Flash
void Internal_Flash_Upgrade_StoreIn(void);

//功能:	单片机片内Flash存储测试 
void Internal_Flash_Test(void);

#endif