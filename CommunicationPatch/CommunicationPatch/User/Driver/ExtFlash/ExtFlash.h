/*******************************************************************************
//ExtFlash.h
//片外Flash存储头文件
*******************************************************************************/

#ifndef	EXTFLASH_H
#define	EXTFLASH_H

/******************************************************************************
//宏定义
*******************************************************************************/
/*SPI接口定义-开头****************************/
//SPI号
#define FLASH_SPI                   SPI1
#define FLASH_SPI_CLK               RCC_APB2Periph_SPI1
#define FLASH_SPI_CLK_INIT          RCC_APB2PeriphClockCmd
//SCK引脚
#define FLASH_SPI_SCK_PIN           GPIO_Pin_5                  
#define FLASH_SPI_SCK_GPIO_PORT     GPIOA                       
#define FLASH_SPI_SCK_GPIO_CLK      RCC_APB2Periph_GPIOA
#define FLASH_SPI_SCK_PINSOURCE     GPIO_PinSource5
#define FLASH_SPI_SCK_AF            GPIO_AF_SPI1
//MISO引脚
#define FLASH_SPI_MISO_PIN          GPIO_Pin_6                
#define FLASH_SPI_MISO_GPIO_PORT    GPIOA                   
#define FLASH_SPI_MISO_GPIO_CLK     RCC_APB2Periph_GPIOA
#define FLASH_SPI_MISO_PINSOURCE    GPIO_PinSource6
#define FLASH_SPI_MISO_AF           GPIO_AF_SPI1
//MOSI引脚
#define FLASH_SPI_MOSI_PIN          GPIO_Pin_7                
#define FLASH_SPI_MOSI_GPIO_PORT    GPIOA                      
#define FLASH_SPI_MOSI_GPIO_CLK     RCC_APB2Periph_GPIOA
#define FLASH_SPI_MOSI_PINSOURCE    GPIO_PinSource7
#define FLASH_SPI_MOSI_AF           GPIO_AF_SPI1
//CS(NSS)引脚
#define FLASH_CS_PIN                GPIO_Pin_4                
#define FLASH_CS_GPIO_PORT          GPIOA                     
#define FLASH_CS_GPIO_CLK           RCC_APB2Periph_GPIOA
#define FLASH_SPI_CS_PINSOURCE      GPIO_PinSource4
//控制CS(NSS)引脚输出低电平
#define SPI_FLASH_CS_LOW()          GPIO_ResetBits(FLASH_CS_GPIO_PORT, FLASH_CS_PIN)	
//控制CS(NSS)引脚输出高电平			
#define SPI_FLASH_CS_HIGH()         GPIO_SetBits(FLASH_CS_GPIO_PORT, FLASH_CS_PIN)



//#define SPI_PREC_2			        (uint8_t)0x01
//#define SPI_PREC_4			        (uint8_t)0x02
//#define SPI_PREC_8			        (uint8_t)0x03
//#define SPI_PREC_16			        (uint8_t)0x04
//#define SPI_PREC_32			        (uint8_t)0x05
//#define SPI_PREC_64 		        (uint8_t)0x06
//#define SPI_PREC_128		        (uint8_t)0x07
//#define SPI_PREC_256		        (uint8_t)0x08




/* Private typedef -----------------------------------------------------------*/
//#define  sFLASH_ID                0xEF3015     //W25X16
//#define  sFLASH_ID                0xEF4015	 //W25Q16
//#define  sFLASH_ID                0XEF4017     //W25Q64
#define  sFLASH_ID                  0XEF4018     //W25Q128


#define SPI_FLASH_PerSectorPage     16      //每个扇区的页数
#define SPI_FLASH_PageSize          256     //每个页的大小(Byte)
#define SPI_FLASH_PerWritePageSize  256     //每个页写入大小(Byte)
#define SPI_FLASH_PerSectorSize     4096    //每个扇区大小(Byte)


#define  FLASH_PACKAGE_NUM_ADDRESS  0x00000

/* Private define ------------------------------------------------------------*/
/*命令定义-开头*******************************/
#define W25X_WriteEnable		    0x06 
#define W25X_WriteDisable		    0x04 
#define W25X_ReadStatusReg		    0x05 
#define W25X_WriteStatusReg		    0x01 
#define W25X_ReadData			    0x03 
#define W25X_FastReadData		    0x0B 
#define W25X_FastReadDual		    0x3B 
#define W25X_PageProgram		    0x02 
#define W25X_BlockErase			    0xD8 
#define W25X_SectorErase		    0x20 
#define W25X_ChipErase			    0xC7 
#define W25X_PowerDown			    0xB9 
#define W25X_ReleasePowerDown	    0xAB 
#define W25X_DeviceID			    0xAB 
#define W25X_ManufactDeviceID   	0x90 
#define W25X_JedecDeviceID		    0x9F 

#define WIP_Flag                    0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte                  0xFF


/*等待超时时间*/
#define SPIT_FLAG_TIMEOUT           ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT           ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))


#define countof(a)                  (sizeof(a) / sizeof(*(a)))

// 存储传感器数据包最小页和最大页
#define SENSOR_DATA_MIN_PAGE_NUM    16
#define SENSOR_DATA_MAX_PAGE_NUM    ((60*24*45) + SENSOR_DATA_MIN_PAGE_NUM)		//按照每分钟1个数据包、45天的存储量，即 60分钟 X 24小时 X 45天 = 64800




/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
















/******************************************************************************
//变量定义/声明
*******************************************************************************/
// 存储数据的页码
extern u16 g_DataPageNum;
	















/******************************************************************************
//函数声明
*******************************************************************************/
//函数功能: SPI初始化函数
void SPI_FLASH_Init(void);

//函数功能: 擦除FLASH扇区，整片擦除函数
void SPI_FLASH_BulkErase(void);

//函数功能: 读取FLASH数据
void SPI_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);

//函数功能: 片外Flash检测函数
u8  Ext_Flash_Detect(void);

//函数功能: 数据存储处理函数
void Data_Storge_Process(u8* data, u16 len);










//函数功能: 片外Flash测试函数
void Ext_Flash_Test(void);





#endif

