/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                     HARD WARE MANNAGER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  HAL_H
   #define  HAL_H


#include "../../ax32xx/inc/ax32xx.h"
#include "../../ax32_platform_demo/resource/r_palette.h"

#include "hal_cfg.h"

#include "hal_adc.h"
#include "hal_csi.h"
#include "hal_dac.h"
#include "hal_eeprom.h"
#include "hal_gpio.h"
#include "hal_iic.h"
#include "hal_lcdframe.h"
#include "hal_lcd.h"
#include "hal_md.h"
#include "hal_osd.h"
#include "hal_rtc.h"
#include "hal_sd.h"
#include "hal_spi.h"
#include "hal_timer.h"
#include "hal_uart.h"
#include "hal_watermark.h"
#include "hal_auadc.h"
#include "hal_mjpeg.h"
#include "hal_wdt.h"
#include "hal_int.h"
#include "hal_stream.h"
#include "hal_emi.h"
#include "hal_osdcmp.h"
#include "hal_rotate.h"
#include "hal_custom_yuv.h"
//isp
#include "../isp/hal_isp.h"

//usb device
#include "../dusb/hal_usb_api.h"
#include "../dusb/hal_usb_dev_enum.h"
#include "../dusb/hal_usb_msc.h"
#include "../dusb/hal_usb_uac.h"
#include "../dusb/hal_usb_uvc.h"
//usb host
#include "../husb/hal_usb_host_api.h"
#include "../husb/hal_usb_host_enum.h"
#include "../husb/hal_usb_host_tpbulk.h"
#include "../husb/hal_usb_host_uvc.h"

#include "hal_sys.h"
//---------------------------hal configure---------------------------------------
#if (AX32_PLATFORM == FPGA)
     #define  HAL_CFG_ADC_CH        ADC_CH_VDDSD
#else
     #define  HAL_CFG_ADC_CH        ADC_CH_PB1
#endif


#define  HAL_CFG_SPI_BUS_MODE        SPI0_1_LINE

#define  HAL_CFG_DEFALT_SPI          SPI0

#define  HAL_CFG_UART                UART0

//extern void LogPrint(char *log,char flag);
#define LogPrint(n,m)         ;

#define  sysLog(x)    ; //LogPrint(x,0)
	 
//--------------------------define---------------------------------------------
enum
{
	__READY__= 0,
	__BUSY__ = 1,
	__FREE__ = 0xff,
};
enum
{
	__TYP_PCM__ = 0x80,
	__TYP_MJP__ = 0x40,
};

#endif