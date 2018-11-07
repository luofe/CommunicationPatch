/*******************************************************************************
//Copyright(C)2018 , ������˾
// All rights reserved.
// Version: v1.0 
// Device : STM32F103C8T6
// Built  : IAR For ARM v7.70(Language: C)
// Date   : 2018-10-27
// Author : ����
// Functiom:APPӦ�ó���Ĺ���˵���ĵ�����¼�������
*******************************************************************************/

20181106:
1����ʼ��д����ģ���ATָ���������


20181103:
1����ʼ��д�������ͨ�ŵĳ���


20181028:
1�����DEBUG�ڵ�ͨ�ų���


20181027:
1���������̣�
//////////////////////////////////////////////////////////////////////
���豸��ͨ�ŵĹ������̣�
1���Ƚ�ֹ���е���Ϣ��ӡ����"$HIDE_ALL"��
2���ٴ򿪴�������Ϣ��ӡ����"$SHOW_DATA"��
3��
/*
******************************************************************************
* Function Name  : SSInfoPrint()
* Description    : ��������Ϣ�����ʽ
* Input          : 
* Output         : None
* Return         : None
//��������
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
* Description    : GPS��Ϣ���
* Input          : 
* Output         : None
* Return         : None
//�����
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
// ϵͳӦ�ò�����-Ĭ�ϲ�������
#define	DEF_DFT_SYSCFG_SAVED_MARK			((u16)0x66AA)				// ϵͳӦ�ò�������ʼ����ɱ�־((Ĭ0x66AA,�޸ĺ��������λ)
#define	DEF_DFT_SYSCFG_SUM						((u16)0x0000)			// ��Ҫ���ݾ����������!!!!
#define	DEF_DFT_SYSCFG_AESTYPE				((u8)0x00)					// ��������	
#define	DEF_DFT_SYSCFG_AESKEY					("")					// ��Կ
#define	DEF_DFT_SYSCFG_APN						("")					// ϵͳAPN	
#define	DEF_DFT_SYSCFG_APNUSER				("")						// ϵͳAPN��½�û�	
#define	DEF_DFT_SYSCFG_APNPASS				("")						// ϵͳAPN��½����	
#define	DEF_DFT_SYSCFG_DOMAINEN				((u8)0)                     // ��������ʹ�����Ϊ1��ʹ����������Ϊ0���ֹ�������Ӽ�IP����
#define	DEF_DFT_SYSCFG_DNSIP					("202.106.0.20")		// ϵͳ����������IP��ַ
#define	DEF_DFT_SYSCFG_DOMAIN1				("null1")					// ϵͳ����1
#define	DEF_DFT_SYSCFG_DOMAIN2				("null2")					// ϵͳ����2
#define	DEF_DFT_SYSCFG_IP1						("58.83.189.147")		// ϵͳ��������IP��ַ(��IP)
#define	DEF_DFT_SYSCFG_PORT1					("21009")				// ϵͳ���������˿ں�
#define	DEF_DFT_SYSCFG_IP2						("58.83.189.147")		// ϵͳ����������IP��ַ(����IP)
#define	DEF_DFT_SYSCFG_PORT2					("21009")				// ϵͳ�����������˿ں�
#define	DEF_DFT_SYSCFG_NATIONCODE			("86")						// ���Ҵ���(�й�Ĭ��"86")ʹ��PDU��ʽ���Ͷ���ʱʹ��		
#define	DEF_DFT_SYSCFG_SMSCENTER			("")						// ���ƽ̨SMSè�绰����	










//////////////////////////////C�ļ�/////////////////////////////////////
/*******************************************************************************
//XXX.c
//XXX�����ļ�
*******************************************************************************/

/******************************************************************************
//����ͷ�ļ�
*******************************************************************************/
#include "stm32l1xx.h"
#include "system_stm32l1xx.h"
#include "stm32l1xx_conf.h"
#include "XXX/XXX.h"

/******************************************************************************
//��������
*******************************************************************************/


/******************************************************************************
//��������
*******************************************************************************/



//////////////////////////////END OF C�ļ�////////////////////////////////////


//////////////////////////////ͷ�ļ�/////////////////////////////////////
/*******************************************************************************
//XXX.h
//XXX����ͷ�ļ�
*******************************************************************************/

#ifndef	XXX_H
#define	XXX_H

/******************************************************************************
//�궨��
*******************************************************************************/

