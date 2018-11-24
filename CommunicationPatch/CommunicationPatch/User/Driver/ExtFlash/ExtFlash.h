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


//#define SPI_FLASH_PageSize        4096
#define SPI_FLASH_PageSize          256
#define SPI_FLASH_PerWritePageSize  256


#define  FLASH_WriteAddress         0x00000
#define  FLASH_ReadAddress          FLASH_WriteAddress
#define  FLASH_SectorToErase        FLASH_WriteAddress

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


#define countof(a)      (sizeof(a) / sizeof(*(a)))




/******************************************************************************
//枚举、结构体类型定义
*******************************************************************************/
















/******************************************************************************
//变量定义/声明
*******************************************************************************/
	















/******************************************************************************
//函数声明
*******************************************************************************/
void SPI_FLASH_Init(void);
void SPI_FLASH_SectorErase(u32 SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void DMA_FLASH_Write(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void DMA_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);																					
void SPI_FLASH_BufferWrite(u8* pBuffer, u32 WriteAddr, u16 NumByteToWrite);
void DMA_FLASH_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead);
u32 SPI_FLASH_ReadID(void);
u32 SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(u32 ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);


u8 SPI_FLASH_ReadByte(void);
u8 SPI_FLASH_SendByte(u8 byte);
u16 SPI_FLASH_SendHalfWord(u16 HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

//片外Flash检测函数
u8  Ext_Flash_Detect(void);

//测试函数
void Ext_Flash_Test(void);





#endif

