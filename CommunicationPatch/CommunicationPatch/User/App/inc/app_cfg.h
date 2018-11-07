
/*
 app_cfg.h  系统配置头文件
*/
/*******************************************************************************
//app_cfg.h
//系统配置头文件
*******************************************************************************/
#ifndef APP_CFG_H
#define APP_TCFG_H

/******************************************************************************
//宏定义
*******************************************************************************/
//#ifndef TRUE
//    #define TRUE        1
//#endif
//
//#ifndef FALSE
//    #define FALSE       0
//#endif
/* Boolean type */
#ifndef BOOL
typedef enum 
{ 
    FALSE = 0, 
    TRUE
}BOOL;
#endif

#ifndef SUCCEED
    #define SUCCEED     0
#endif

#ifndef FAILURE
    #define FAILURE     1
#endif

#ifndef NULL
    #define NULL       '\0'
#endif



#define __CLOCK_SETUP               1
#define __RCC_CR_VAL                0x01000083
#define __RCC_CFGR_VAL              0x00440402
#define __HSE                       12000000
#define __RCC_AHB					0x00000001
#define __RCC_APB1					0x00020007
#define __RCC_APB2					0x00004A3D

//******************************************************************************
//数据类型定义
//     char Use 1 Bytes
//      int Use 4 Bytes
//     long Use 4 Bytes
//    short Use 2 Bytes
// long int Use 4 Bytes
//short int Use 2 Bytes
//long long Use 8 Bytes
//    float Use 4 Bytes
//   double Use 8 Bytes

//无符号数据类型
#define u8              unsigned char
#define u16             unsigned short int
#define u32             unsigned long int

//有符号数据类型
#define s8              signed char
#define s16             signed short int
#define s32             signed long int

/* These types must be 16-bit, 32-bit or larger integer */
typedef int				INT;
typedef unsigned int	UINT;

/* These types must be 8-bit integer */
typedef signed char		CHAR;
typedef unsigned char	UCHAR;
typedef unsigned char	BYTE;

/* These types must be 16-bit integer */
typedef short			SHORT;
typedef unsigned short	USHORT;
typedef unsigned short	WORD;
typedef unsigned short	WCHAR;

/* These types must be 32-bit integer */
typedef long			LONG;
typedef unsigned long	ULONG;
typedef unsigned long	DWORD;

//时间和日期枚举
typedef enum  	
{ 
    SUN=0, 
    MON, 
    TUE, 
    WED, 
    THU, 
    FRI, 
    SAT 
}E_WEEK;

typedef enum	
{ 
    Jan=0, 
    Feb, 
    Mar, 
    Apr, 
    May, 
    Jun, 
    Jul, 
    Aug, 
    Sep, 
    Oct, 
    Nov, 
    Dec 
}E_MONTH; 

typedef enum  	
{ 
    Sec=0, 
    Min, 
    Hour, 
    Date, 
    Week, 
    Month, 
    Year
}E_TIME;

typedef struct
{
    unsigned B0:1;
    unsigned B1:1;
    unsigned B2:1;
    unsigned B3:1;
    unsigned B4:1;
    unsigned B5:1;
    unsigned B6:1;
    unsigned B7:1;
}S_byte;

typedef union 
{
    u8      Value;
    S_byte  BIT;
    struct
    {
		unsigned  Lsb:4;
		unsigned  Msb:4;	   
	}HalfByte;
		
}U_BYTE;

typedef union 
{	
	u16	    Value;
	u8 	    cV[2];
	struct	
	{
		u8  Lsb;
		u8  Msb;
	}ft;
}U_WORD;

typedef union
{
	u32	    Value;
	u16     iV[2];
	u8 	    cV[4];

}U_DWORD;

typedef union
{
	float   Value;
	u8 	    cV[4];
}U_FLOAT;

/******************************************************************************
//变量定义/声明
*******************************************************************************/







/******************************************************************************
//函数声明
*******************************************************************************/















#endif

/**********************************END OF FILE*********************************/

