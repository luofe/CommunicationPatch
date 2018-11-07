/*******************************************************************************
//Copyright(C)2018 , 蛙鸣公司
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : 刘锋
// Functiom:APP应用程序的工程说明文档，记录程序进度
*******************************************************************************/

20181106:
1、开始编写无线模块的AT指令操作程序；


20181103:
1、开始编写与服务器通信的程序；


20181028:
1、完成DEBUG口的通信程序；


20181027:
1、建立工程；
//////////////////////////////////////////////////////////////////////
与设备端通信的工作流程：
1、先禁止所有的信息打印，即"$HIDE_ALL"；
2、再打开传感器信息打印，即"$SHOW_DATA"；
3、
/*
******************************************************************************
* Function Name  : SSInfoPrint()
* Description    : 传感器信息输出格式
* Input          : 
* Output         : None
* Return         : None
//左对齐输出
******************************************************************************
*/
static	void	SSInfoPrint (void)
{
		myPrintf("\r\n[SS-Temp  ]=%-d",	(s32)(s_trhData.Ttrue * 100 + 0.5));
		myPrintf("\r\n[SS-RH    ]=%-d",	(s32)(s_trhData.RHtrue * 100 + 0.5));
		myPrintf("\r\n[SS-PM2.5 ]=%-d,%-d,%-d",	s_pm25Data.trueNd, s_pm25Data.labVal, s_pm25Data.appVal);
		myPrintf("\r\n[SS-PM10  ]=%-d,%-d,%-d",	s_pm10Data.trueVol, s_pm10Data.labVal, s_pm10Data.appVal);
		myPrintf("\r\n[SS-CO(3) ]=%-d,%-d,%-d",	s_gasCalc.COTrueVol, s_gasCalc.COVal[0], s_gasCalc.COVal[1]);
		myPrintf("\r\n[SS-NO2(4)]=%-d,%-d,%-d",	s_gasCalc.NO2TrueVol, s_gasCalc.NO2Val[0], s_gasCalc.NO2Val[1]);
		myPrintf("\r\n[SS-O3(5) ]=%-d,%-d,%-d",	s_gasCalc.O3TrueVol, s_gasCalc.O3Val[0], s_gasCalc.O3Val[1]);
		myPrintf("\r\n[SS-SO2(6)]=%-d,%-d,%-d",	s_gasCalc.SO2TrueVol, s_gasCalc.SO2Val[0], s_gasCalc.SO2Val[1]);
		myPrintf("\r\n[SS-NO(7) ]=%-d,%-d,%-d",	s_gasCalc.NOTrueVol, s_gasCalc.NOVal[0], s_gasCalc.NOVal[1]);	
		myPrintf("\r\n[SS-TVOC(9]=%-d,%-d,%-d",	s_gasCalc.TVOCTrueVol, s_gasCalc.TVOCVal[0], s_gasCalc.TVOCVal[1]);	
		myPrintf("\r\n[SS-Temp-W]=%-d",	(s32)(s_exorData.T.a * 100 + 0.5));
		myPrintf("\r\n[SS-RH-W  ]=%-d",	(s32)(s_exorData.RH.a * 100 + 0.5));
		myPrintf("\r\n[SS-WD    ]=%-d",	(s32)(s_exorData.WD.a * 100 + 0.5));
		myPrintf("\r\n[SS-WS    ]=%-d",	(s32)(s_exorData.WS.a * 100 + 0.5));
		myPrintf("\r\n[SS-PA    ]=%-d",	(s32)(s_exorData.PA.a * 100 + 0.5));
		myPrintf("\r\n[FUN-main ]=%d/min",	s_fun.mFreq);
		myPrintf("\r\n[FUN-pm10 ]=%d/min",	s_fun.pm10Freq);	
}

/*
******************************************************************************
* Function Name  : GPSInfoPrint()
* Description    : GPS信息输出
* Input          : 
* Output         : None
* Return         : None
//左对齐
******************************************************************************
*/
static	void	GPSInfoPrint (void)
{
		myPrintf("\r\n[GPS-STA  ]=%c",	s_gps.sta);
		myPrintf("\r\n[GPS-FS   ]=%c",	s_gps.fs);
		myPrintf("\r\n[GPS-LAT  ]=%d",	s_gps.latitude);
		myPrintf("\r\n[GPS-LON  ]=%d",	s_gps.longitude);
		myPrintf("\r\n[GPS-HIGH ]=%-d",	s_gps.high);
		myPrintf("\r\n[GPS-SPEED]=%d",	s_gps.speed);
		myPrintf("\r\n[GPS-HEAD ]=%d",	s_gps.heading);
		myPrintf("\r\n[GPS-NOSV ]=%d",	s_gps.noSV);
		myPrintf("\r\n[GPS-24RST]=%d",	s_err.gpsResetTimes);			
}




//////////////////////////////////////////////////////////////////////
// 系统应用参数区-默认参数定义
#define	DEF_DFT_SYSCFG_SAVED_MARK			((u16)0x66AA)				// 系统应用参数区初始化完成标志((默0x66AA,修改后参数将复位)
#define	DEF_DFT_SYSCFG_SUM						((u16)0x0000)			// 需要根据具体参数计算!!!!
#define	DEF_DFT_SYSCFG_AESTYPE				((u8)0x00)					// 加密类型	
#define	DEF_DFT_SYSCFG_AESKEY					("")					// 秘钥
#define	DEF_DFT_SYSCFG_APN						("")					// 系统APN	
#define	DEF_DFT_SYSCFG_APNUSER				("")						// 系统APN登陆用户	
#define	DEF_DFT_SYSCFG_APNPASS				("")						// 系统APN登陆密码	
#define	DEF_DFT_SYSCFG_DOMAINEN				((u8)0)                     // 域名连接使能如果为1则使能域名连接为0则禁止域名连接即IP连接
#define	DEF_DFT_SYSCFG_DNSIP					("202.106.0.20")		// 系统域名服务器IP地址
#define	DEF_DFT_SYSCFG_DOMAIN1				("null1")					// 系统域名1
#define	DEF_DFT_SYSCFG_DOMAIN2				("null2")					// 系统域名2
#define	DEF_DFT_SYSCFG_IP1						("58.83.189.147")		// 系统主服务器IP地址(主IP)
#define	DEF_DFT_SYSCFG_PORT1					("21009")				// 系统主服务器端口号
#define	DEF_DFT_SYSCFG_IP2						("58.83.189.147")		// 系统辅助服务器IP地址(辅助IP)
#define	DEF_DFT_SYSCFG_PORT2					("21009")				// 系统辅助服务器端口号
#define	DEF_DFT_SYSCFG_NATIONCODE			("86")						// 国家代码(中国默认"86")使用PDU方式发送短信时使用		
#define	DEF_DFT_SYSCFG_SMSCENTER			("")						// 监控平台SMS猫电话号码	










//////////////////////////////C文件/////////////////////////////////////
/*******************************************************************************
//XXX.c
//XXX驱动文件
*******************************************************************************/

/******************************************************************************
//包含头文件
*******************************************************************************/
#include "stm32l1xx.h"
#include "system_stm32l1xx.h"
#include "stm32l1xx_conf.h"
#include "XXX/XXX.h"

/******************************************************************************
//变量定义
*******************************************************************************/


/******************************************************************************
//函数定义
*******************************************************************************/



//////////////////////////////END OF C文件////////////////////////////////////


//////////////////////////////头文件/////////////////////////////////////
/*******************************************************************************
//XXX.h
//XXX驱动头文件
*******************************************************************************/

#ifndef	XXX_H
#define	XXX_H

/******************************************************************************
//宏定义
*******************************************************************************/

