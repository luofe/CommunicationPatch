/*
    includes.h头文件包含文件，出现在每个.C文件的第一行
    说明：当不需要使用某个功能时，注释掉对应行.
*/

#ifndef INCLUDES_H
#define INCLUDES_H


//******************************************************************************
//头文件文件
#include "stdio.h"
#include "string.h"

#include "stm32f10x.h"                                         //系统头文件
#include "stm32f10x_conf.h"                                //系统配置头文件

#include "stm32f10x_it.h"                                  //中断函数头文件

#include "iap_cfg.h"                                       //IAP配置头文件

#include "common.h"                                        //通用头文件
#include "ymodem.h"                                        //ymodem协议头文件


#include "Internal_Flash/Internal_Flash.h"              //单片机内部的flash存储的驱动头文件




#endif

