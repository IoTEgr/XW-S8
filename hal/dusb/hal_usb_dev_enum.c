/******************** (C) COPYRIGHT 2013 BUILDWIN ************************
* File Name          : uart.c
* Author             : pengtao_zhang
* Version            : V0719a
* Date               : 07/19/2013 ;mm/dd/yyyy
* Description        : This file provides all the uart firmware functions.
***************************************************************************/
//#include "../../mcu/mcu_include.h"
#include "../inc/hal.h"
#include "hal_usb_dev_enum.h"
#include "hal_usb_msc.h"
#include "hal_usb_uac.h"
#include "hal_usb_uvc.h"
//#define _DEBG_USB_ENUM__
#include "../../device/cmos_sensor/cmos_sensor.h"


typedef struct
{
	u8 const *pdev;
	u16 devlen;

	u8 const *pcfg;
	u16 cfglen;	

}USB_DEV_DSC;

static  USB_DEV_DSC usbdsc;


#define __UVC_CTL_INTFS__	0
#define __UVC_STRM_INTFS__	1
#define __UAC_CTL_INTFS__ 	2
#define __UAC_STRM_INTFS__	3
#define __MSC_INTFS__		4	


const u8 msc_dev[]= {
	0x12, 0x01, 0x10, 0x01,  0x00, 0x00, 0x00, EP0_BUF_SIZE,  0x19, 0x02, 0x80, 0x32,  0x00, 0x01, 0x01, 0x02, 0x00, 0x01, 
};
const u8 uvc_dev[]={
	0x12, 0x01, 0x10, 0x01,  0xEF, 0x02, 0x01, EP0_BUF_SIZE,  0x08, 0x19, 0x82, 0x32,  0x00, 0x01, 0x01, 0x02, 0x00, 0x01,
};
const u8 TYPE_COMB_DEVICE[]=
{
	0x12, 0x01, 0x10, 0x01,  0xEF, 0x02, 0x01, EP0_BUF_SIZE,  0x08, 0x19, 0x83, 0x32,  0x00, 0x01, 0x01, 0x02, 0x00, 0x01,
};
const u8 msc_cfg[]={
	#define __CONFIG_COMB_SIZE__	(0x20)	
	0x09, 0x02, __CONFIG_COMB_SIZE__&0xff, __CONFIG_COMB_SIZE__>>8,  1, 0x01, 0x00, 0x80, 0x32, 
	0x09, 0x04, 0x04, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,
	0x07, 0x05, __MSC_EPIN__, 0x02, __MSC_SIZE__&0xff, __MSC_SIZE__>>8, 0x01,
	0x07, 0x05, __MSC_EPOUT__, 0x02, __MSC_SIZE__&0xff, __MSC_SIZE__>>8, 0x01,
};
#if HAL_CFG_ISP_SUPPORT
const u8 uvc_cfg_720p[]={

#define __CONFIG_UVC_SIZE__	 (0x16b + 0x6b)

0x09, 0x02, __CONFIG_UVC_SIZE__&0xff, __CONFIG_UVC_SIZE__>>8, 0x04, 0x01, 0x00, 0x80, 0xFA, 
0x08, 0x0B, 0x00, 0x02, 0x0E, 0x03, 0x00, 0x04, 
0x09, 0x04, __UVC_CTL_INTFS__, 0x00, 0x01, 0x0E, 0x01, 0x00, 0x06, 
0x0D, 0x24, 0x01, 0x00, 0x01, 0x4D, 0x00, 0x80, 0xC3, 0xC9, 0x01, 0x01, 0x01, 
0x12, 0x24, 0x02, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 
0x09, 0x24, 0x03, 0x02, 0x01, 0x01, 0x00, 0x04, 0x00, 
0x0B, 0x24, 0x05, 0x03, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
0x1A, 0x24, 0x06, 0x04, 0x92, 0x42, 0x39, 0x46, 0xD1, 0x0C, 0xE3, 0x4A, 0x87, 0x83, 0x31, 0x33, 0xF9, 0xEA, 0xAA, 0x3B, 0x08, 0x01, 0x03, 0x01, 0x00, 0x00, 
0x07, 0x05, __UVC_CTL_EP__, 0x03, 0x40, 0x00, 0x04, 
0x05, 0x25, 0x03, 0x40, 0x00, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x00, 0x00, 0x0E, 0x02, 0x00, 0x00, 
0x0E, 0x24, 0x01, 0x01, 0xD8, 0x00, __UVC_STRM_EP__, 0x00, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 
0x0B, 0x24, 0x06, 0x01, MAX_SOL, 0x00, DFLT_SOL, 0x00, 0x00, 0x00, 0x00, 

//1280*720
0x1E, 0x24, 0x07, SOL1, 0x00, (u8)SOL1_W>>0, SOL1_W>>8, (u8)SOL1_H>>0, SOL1_H>>8, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x20, 0x1C, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//800*600
0x1E, 0x24, 0x07, SOL2, 0x00, (u8)SOL2_W>>0,SOL2_W>>8, (u8)SOL2_H>>0, SOL2_H>>8, 0x00, 0xA0, 0xBB, 0x0D, 0x00, 0xA0, 0xBB, 0x0D, 0x00, 0xA6, 0x0E, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//640*480
0x1E, 0x24, 0x07, SOL3, 0x00, (u8)SOL3_W>>0,SOL3_W>>8, (u8)SOL3_H>>0, SOL3_H>>8, 0x00, 0x00, 0xCA, 0x08, 0x00, 0x00, 0xCA, 0x08, 0x00, 0x60, 0x09, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//352*288
0x1E, 0x24, 0x07, SOL4, 0x00, (u8)SOL4_W>>0,SOL4_W>>8, (u8)SOL4_H>>0, SOL4_H>>8,  0x00, 0x80, 0xE6, 0x02, 0x00, 0x80, 0xE6, 0x02, 0x00, 0x18, 0x03, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//320*240
0x1E, 0x24, 0x07, SOL5, 0x00, (u8)SOL5_W>>0,SOL5_W>>8, (u8)SOL5_H>>0, SOL5_H>>8, 0x00, 0x80, 0x32, 0x02, 0x00, 0x80, 0x32, 0x02, 0x00, 0x58, 0x02, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
0x1A, 0x24, 0x03, 0x00, 0x05, 0x00, 0x05, 0xD0, 0x02, 0x20, 0x03, 0x58, 0x02, 0x80, 0x02, 0xE0, 0x01, 0x60, 0x01, 0x20, 0x01, 0x40, 0x01, 0xF0, 0x00, 0x00, 
0x06, 0x24, 0x0D, 0x01, 0x01, 0x04, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x01, 0x01, 0x0E, 0x02, 0x00, 0x00, 
0x07, 0x05, __UVC_STRM_EP__, 0x05, 0x00, 0x14, 0x01, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x02, 0x01, 0x0E, 0x02, 0x00, 0x00, 
0x07, 0x05, __UVC_STRM_EP__, 0x05, 0x00, 0x14, 0x01,

//UAC_CONFIG_DESC_DATA size = 0x6b
//{
0x08, 0x0b, 0x02, 0x02, 0x01, 0x00, 0x00, 0x04, 
0x09, 0x04, __UAC_CTL_INTFS__, 0x00, 0x00, 0x01, 0x01, 0x00, 0x04,	
0x09, 0x24, 0x01, 0x00, 0x01, 0x26, 0x00, 0x01, 0x03, 
0x0c, 0x24, 0x02, 0x03, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x24, 0x06, 0x05, 0x03, 0x01, 0x03, 0x00, 
0x09, 0x24, 0x03, 0x04, 0x01, 0x01, 0x00, 0x05, 0x00,
0x09, 0x04, __UAC_STRM_INTFS__, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 
0x09, 0x04, __UAC_STRM_INTFS__, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00,
0x07, 0x24, 0x01, 0x04, 0x01, 0x01, 0x00, 
0x0b, 0x24, 0x02, 0x01, 0x01, 0x02, 0x10, 0x01, (u8)__PCM_SAMPL__, (u8)(__PCM_SAMPL__>>8), (u8)(__PCM_SAMPL__>>16),
0x09, 0x05, __UAC_STRM_EP__, 0x01, 0x40, 0x00, 0x04, 0x00, 0x00,
0x07, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00,
//}

};
#endif

const u8 uvc_cfg_vga[]={

#define __CONFIG_UVC_VGA_SIZE__	 (0x16b  - 0x7A + 0x6B)//(0x16b + 0x6b)

0x09, 0x02, __CONFIG_UVC_VGA_SIZE__&0xff, __CONFIG_UVC_VGA_SIZE__>>8, 0x04, 0x01, 0x00, 0x80, 0xFA, 
0x08, 0x0B, 0x00, 0x02, 0x0E, 0x03, 0x00, 0x04, 
0x09, 0x04, __UVC_CTL_INTFS__, 0x00, 0x01, 0x0E, 0x01, 0x00, 0x06, 
0x0D, 0x24, 0x01, 0x00, 0x01, 0x4D, 0x00, 0x80, 0xC3, 0xC9, 0x01, 0x01, 0x01, 
0x12, 0x24, 0x02, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 
0x09, 0x24, 0x03, 0x02, 0x01, 0x01, 0x00, 0x04, 0x00, 
0x0B, 0x24, 0x05, 0x03, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
0x1A, 0x24, 0x06, 0x04, 0x92, 0x42, 0x39, 0x46, 0xD1, 0x0C, 0xE3, 0x4A, 0x87, 0x83, 0x31, 0x33, 0xF9, 0xEA, 0xAA, 0x3B, 0x08, 0x01, 0x03, 0x01, 0x00, 0x00, 
0x07, 0x05, __UVC_CTL_EP__, 0x03, 0x40, 0x00, 0x04, 
0x05, 0x25, 0x03, 0x40, 0x00, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x00, 0x00, 0x0E, 0x02, 0x00, 0x00, 
0x0E, 0x24, 0x01, 0x01, 0xD8 - 0x7A, 0x00, __UVC_STRM_EP__, 0x00, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 
0x0B, 0x24, 0x06, 0x01, MAX_SOL_VGA, 0x00, DFLT_SOL_VGA, 0x00, 0x00, 0x00, 0x00, 

//640*480
0x1E, 0x24, 0x07, SOL1_VGA, 0x00, (u8)SOL1_VGA_W>>0,SOL1_VGA_W>>8, (u8)SOL1_VGA_H>>0, SOL1_VGA_H>>8, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x20, 0x1C, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//320*240
0x1E, 0x24, 0x07, SOL2_VGA, 0x00, (u8)SOL2_VGA_W>>0,SOL2_VGA_W>>8, (u8)SOL2_VGA_H>>0, SOL2_VGA_H>>8, 0x00, 0xA0, 0xBB, 0x0D, 0x00, 0xA0, 0xBB, 0x0D, 0x00, 0xA6, 0x0E, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 

0x09, 0x04, __UVC_STRM_INTFS__, 0x01, 0x01, 0x0E, 0x02, 0x00, 0x00, 
0x07, 0x05, __UVC_STRM_EP__, 0x05, 0x00, 0x14, 0x01, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x02, 0x01, 0x0E, 0x02, 0x00, 0x00, 
0x07, 0x05, __UVC_STRM_EP__, 0x05, 0x00, 0x14, 0x01,

//UAC_CONFIG_DESC_DATA size = 0x6b
//{
0x08, 0x0b, 0x02, 0x02, 0x01, 0x00, 0x00, 0x04, 
0x09, 0x04, __UAC_CTL_INTFS__, 0x00, 0x00, 0x01, 0x01, 0x00, 0x04,	
0x09, 0x24, 0x01, 0x00, 0x01, 0x26, 0x00, 0x01, 0x03, 
0x0c, 0x24, 0x02, 0x03, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
0x08, 0x24, 0x06, 0x05, 0x03, 0x01, 0x03, 0x00, 
0x09, 0x24, 0x03, 0x04, 0x01, 0x01, 0x00, 0x05, 0x00,
0x09, 0x04, __UAC_STRM_INTFS__, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 
0x09, 0x04, __UAC_STRM_INTFS__, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00,
0x07, 0x24, 0x01, 0x04, 0x01, 0x01, 0x00, 
0x0b, 0x24, 0x02, 0x01, 0x01, 0x02, 0x10, 0x01, (u8)__PCM_SAMPL__, (u8)(__PCM_SAMPL__>>8), (u8)(__PCM_SAMPL__>>16),
0x09, 0x05, __UAC_STRM_EP__, 0x01, 0x40, 0x00, 0x04, 0x00, 0x00,
0x07, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00,
//}
};


/**
 *  udisk 描述
*/ 


#if HAL_CFG_ISP_SUPPORT
const u8 TYPE_COMB_CONFIG_720P[]=
{
	
#define __CONFIG_COMB3_SIZE__	(0x16b + 0x6b+ 0x17)
0x09, 0x02, __CONFIG_COMB3_SIZE__&0xff, __CONFIG_COMB3_SIZE__>>8, 0x02 + 1 + 2, 0x01, 0x00, 0x80, 0x32, 

0x08, 0x0B, 0x00, 0x02, 0x0E, 0x03, 0x00, 0x04, 
0x09, 0x04, __UVC_CTL_INTFS__, 0x00, 0x01, 0x0E, 0x01, 0x00, 0x06, 
0x0D, 0x24, 0x01, 0x00, 0x01, 0x4D, 0x00, 0x80, 0xC3, 0xC9, 0x01, 0x01, 0x01, 
0x12, 0x24, 0x02, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 
0x09, 0x24, 0x03, 0x02, 0x01, 0x01, 0x00, 0x04, 0x00, 
0x0B, 0x24, 0x05, 0x03, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
0x1A, 0x24, 0x06, 0x04, 0x92, 0x42, 0x39, 0x46, 0xD1, 0x0C, 0xE3, 0x4A, 0x87, 0x83, 0x31, 0x33, 0xF9, 0xEA, 0xAA, 0x3B, 0x08, 0x01, 0x03, 0x01, 0x00, 0x00, 
0x07, 0x05, __UVC_CTL_EP__, 0x03, 0x40, 0x00, 0x04, 
0x05, 0x25, 0x03, 0x40, 0x00, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x00, 0x00, 0x0E, 0x02, 0x00, 0x00, 
0x0E, 0x24, 0x01, 0x01, 0xD8, 0x00, __UVC_STRM_EP__, 0x00, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 
0x0B, 0x24, 0x06, 0x01, MAX_SOL, 0x00, DFLT_SOL, 0x00, 0x00, 0x00, 0x00, 

//1280*720
0x1E, 0x24, 0x07, SOL1, 0x00, (u8)SOL1_W>>0, SOL1_W>>8, (u8)SOL1_H>>0, SOL1_H>>8, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x20, 0x1C, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//1280*720
0x1E, 0x24, 0x07, SOL1, 0x00, (u8)SOL1_W>>0, SOL1_W>>8, (u8)SOL1_H>>0, SOL1_H>>8, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x20, 0x1C, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//1280*720
0x1E, 0x24, 0x07, SOL1, 0x00, (u8)SOL1_W>>0, SOL1_W>>8, (u8)SOL1_H>>0, SOL1_H>>8, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x20, 0x1C, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//1280*720
0x1E, 0x24, 0x07, SOL1, 0x00, (u8)SOL1_W>>0, SOL1_W>>8, (u8)SOL1_H>>0, SOL1_H>>8, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x20, 0x1C, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//1280*720
0x1E, 0x24, 0x07, SOL1, 0x00, (u8)SOL1_W>>0, SOL1_W>>8, (u8)SOL1_H>>0, SOL1_H>>8, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x20, 0x1C, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
0x1A, 0x24, 0x03, 0x00, 0x05, 0x00, 0x05, 0xD0, 0x02, 0x00, 0x05, 0xD0, 0x02, 0x00, 0x05, 0xD0, 0x02, 0x00, 0x05, 0xD0, 0x02, 0x00, 0x05, 0xD0, 0x02, 0x00, 

/*
//800*600
0x1E, 0x24, 0x07, SOL2, 0x00, (u8)SOL2_W>>0,SOL2_W>>8, (u8)SOL2_H>>0, SOL2_H>>8, 0x00, 0xA0, 0xBB, 0x0D, 0x00, 0xA0, 0xBB, 0x0D, 0x00, 0xA6, 0x0E, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//640*480
0x1E, 0x24, 0x07, SOL3, 0x00, (u8)SOL3_W>>0,SOL3_W>>8, (u8)SOL3_H>>0, SOL3_H>>8, 0x00, 0x00, 0xCA, 0x08, 0x00, 0x00, 0xCA, 0x08, 0x00, 0x60, 0x09, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//352*288
0x1E, 0x24, 0x07, SOL4, 0x00, (u8)SOL4_W>>0,SOL4_W>>8, (u8)SOL4_H>>0, SOL4_H>>8,  0x00, 0x80, 0xE6, 0x02, 0x00, 0x80, 0xE6, 0x02, 0x00, 0x18, 0x03, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//320*240
0x1E, 0x24, 0x07, SOL5, 0x00, (u8)SOL5_W>>0,SOL5_W>>8, (u8)SOL5_H>>0, SOL5_H>>8, 0x00, 0x80, 0x32, 0x02, 0x00, 0x80, 0x32, 0x02, 0x00, 0x58, 0x02, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
*/
//0x1A, 0x24, 0x03, 0x00, 0x05, 0x00, 0x05, 0xD0, 0x02, 0x20, 0x03, 0x58, 0x02, 0x80, 0x02, 0xE0, 0x01, 0x60, 0x01, 0x20, 0x01, 0x40, 0x01, 0xF0, 0x00, 0x00, 

0x06, 0x24, 0x0D, 0x01, 0x01, 0x04, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x01, 0x01, 0x0E, 0x02, 0x00, 0x00, 
0x07, 0x05, __UVC_STRM_EP__, 0x05, 0x00, 0x14, 0x01, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x02, 0x01, 0x0E, 0x02, 0x00, 0x00, 
0x07, 0x05, __UVC_STRM_EP__, 0x05, 0x00, 0x14, 0x01,


//UAC_CONFIG_DESC_DATA size = 0x6b
//{
	0x08, 0x0b, 0x02, 0x02, 0x01, 0x00, 0x00, 0x04, 
	0x09, 0x04, __UAC_CTL_INTFS__, 0x00, 0x00, 0x01, 0x01, 0x00, 0x04,  
	0x09, 0x24, 0x01, 0x00, 0x01, 0x26, 0x00, 0x01, 0x03, 
	0x0c, 0x24, 0x02, 0x03, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x24, 0x06, 0x05, 0x03, 0x01, 0x03, 0x00, 
	0x09, 0x24, 0x03, 0x04, 0x01, 0x01, 0x00, 0x05, 0x00,
	0x09, 0x04, __UAC_STRM_INTFS__, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 
	0x09, 0x04, __UAC_STRM_INTFS__, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00,
	0x07, 0x24, 0x01, 0x04, 0x01, 0x01, 0x00, 
	0x0b, 0x24, 0x02, 0x01, 0x01, 0x02, 0x10, 0x01,(u8)( __PCM_SAMPL__), (u8)(__PCM_SAMPL__>>8), (u8)(__PCM_SAMPL__>>16),
	0x09, 0x05, __UAC_STRM_EP__, 0x01, 0x40, 0x00, 0x04, 0x00, 0x00,
	0x07, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00,
//}

//MSC_CONFIG_DESC_DATA size = 0x17
//{
	0x09, 0x04, 0x04, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,
	0x07, 0x05, __MSC_EPIN__, 0x02, __MSC_SIZE__&0xff, __MSC_SIZE__>>8, 0x01,
	0x07, 0x05, __MSC_EPOUT__, 0x02, __MSC_SIZE__&0xff, __MSC_SIZE__>>8, 0x01,
//}
	
};
#endif



const u8 TYPE_COMB_CONFIG_VGA[]=
{
#undef __CONFIG_COMB3_SIZE__
#define __CONFIG_COMB3_SIZE__	(0x16b - 0x7A + 0x6b+ 0x17)
0x09, 0x02, __CONFIG_COMB3_SIZE__&0xff, __CONFIG_COMB3_SIZE__>>8, 0x02 + 1, 0x01, 0x00, 0x80, 0x32, 

0x08, 0x0B, 0x00, 0x02, 0x0E, 0x03, 0x00, 0x04, 
0x09, 0x04, __UVC_CTL_INTFS__, 0x00, 0x01, 0x0E, 0x01, 0x00, 0x06, 
0x0D, 0x24, 0x01, 0x00, 0x01, 0x4D, 0x00, 0x80, 0xC3, 0xC9, 0x01, 0x01, 0x01, 
0x12, 0x24, 0x02, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 
0x09, 0x24, 0x03, 0x02, 0x01, 0x01, 0x00, 0x04, 0x00, 
0x0B, 0x24, 0x05, 0x03, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
0x1A, 0x24, 0x06, 0x04, 0x92, 0x42, 0x39, 0x46, 0xD1, 0x0C, 0xE3, 0x4A, 0x87, 0x83, 0x31, 0x33, 0xF9, 0xEA, 0xAA, 0x3B, 0x08, 0x01, 0x03, 0x01, 0x00, 0x00, 
0x07, 0x05, __UVC_CTL_EP__, 0x03, 0x40, 0x00, 0x04, 
0x05, 0x25, 0x03, 0x40, 0x00, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x00, 0x00, 0x0E, 0x02, 0x00, 0x00, 
0x0E, 0x24, 0x01, 0x01, 0xD8 - 0x7A, 0x00, __UVC_STRM_EP__, 0x00, 0x02, 0x02, 0x01, 0x01, 0x01, 0x00, 
0x0B, 0x24, 0x06, 0x01, MAX_SOL_VGA, 0x00, DFLT_SOL_VGA, 0x00, 0x00, 0x00, 0x00, 

//640*480
0x1E, 0x24, 0x07, SOL1_VGA, 0x00, (u8)SOL1_VGA_W>>0, SOL1_VGA_W>>8, (u8)SOL1_VGA_H>>0, SOL1_VGA_H>>8, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x00, 0x5E, 0x1A, 0x00, 0x20, 0x1C, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 
//320*240
0x1E, 0x24, 0x07, SOL2_VGA, 0x00, (u8)SOL2_VGA_W>>0,SOL2_VGA_W>>8, (u8)SOL2_VGA_H>>0, SOL2_VGA_H>>8, 0x00, 0xA0, 0xBB, 0x0D, 0x00, 0xA0, 0xBB, 0x0D, 0x00, 0xA6, 0x0E, 0x00, 0x15, 0x16, 0x05, 0x00, 0x01, 0x15, 0x16, 0x05, 0x00, 

0x09, 0x04, __UVC_STRM_INTFS__, 0x01, 0x01, 0x0E, 0x02, 0x00, 0x00, 
0x07, 0x05, __UVC_STRM_EP__, 0x05, 0x00, 0x14, 0x01, 
0x09, 0x04, __UVC_STRM_INTFS__, 0x02, 0x01, 0x0E, 0x02, 0x00, 0x00, 
0x07, 0x05, __UVC_STRM_EP__, 0x05, 0x00, 0x14, 0x01,


//UAC_CONFIG_DESC_DATA size = 0x6b
//{
	0x08, 0x0b, 0x02, 0x02, 0x01, 0x00, 0x00, 0x04, 
	0x09, 0x04, __UAC_CTL_INTFS__, 0x00, 0x00, 0x01, 0x01, 0x00, 0x04,  
	0x09, 0x24, 0x01, 0x00, 0x01, 0x26, 0x00, 0x01, 0x03, 
	0x0c, 0x24, 0x02, 0x03, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 
	0x08, 0x24, 0x06, 0x05, 0x03, 0x01, 0x03, 0x00, 
	0x09, 0x24, 0x03, 0x04, 0x01, 0x01, 0x00, 0x05, 0x00,
	0x09, 0x04, __UAC_STRM_INTFS__, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 
	0x09, 0x04, __UAC_STRM_INTFS__, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00,
	0x07, 0x24, 0x01, 0x04, 0x01, 0x01, 0x00, 
	0x0b, 0x24, 0x02, 0x01, 0x01, 0x02, 0x10, 0x01,(u8)( __PCM_SAMPL__), (u8)(__PCM_SAMPL__>>8), (u8)(__PCM_SAMPL__>>16),
	0x09, 0x05, __UAC_STRM_EP__, 0x01, 0x40, 0x00, 0x04, 0x00, 0x00,
	0x07, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00,
//}

//MSC_CONFIG_DESC_DATA size = 0x17
//{
	0x09, 0x04, 0x04, 0x00, 0x02, 0x08, 0x06, 0x50, 0x00,
	0x07, 0x05, __MSC_EPIN__, 0x02, __MSC_SIZE__&0xff, __MSC_SIZE__>>8, 0x01,
	0x07, 0x05, __MSC_EPOUT__, 0x02, __MSC_SIZE__&0xff, __MSC_SIZE__>>8, 0x01,
//}
	
};


/*=================================================================================
USB Language ID Descriptor
=================================================================================*/
u8 const UsbLanguageID[4] =
{
	4,			// Num bytes of this descriptor
	0x03,			// String Descriptor
	0x09,			// Language ID LSB
	0x04			// Language ID MSB
};
/*================================================================================
USB String Descriptors
================================================================================*/
u8 const UsbStrDescManufacturer[16] =
{
	16,				// Num bytes of this descriptor
	0x03,				// String Descriptor
	'G',	0,
	'e',	0,
	'n',	0,
	'e',	0,
	'r',	0,
	'i',	0,
	'c',	0
};

u8 const UsbStrDescProduct_1[0x1e] =
{
	0x1e, 0x03, 0x47, 0x00, 0x45, 0x00, 0x4e, 0x00, 0x45, 0x00, 0x52, 0x00, 0x41, 0x00, 0x4c, 0x00,
	0x20, 0x00, 0x2d, 0x00, 0x20, 0x00, 0x55, 0x00, 0x56, 0x00, 0x43, 0x00, 0x20, 0x00,
};

u8 const UsbStrDescProduct_0[0x20] =
{
	0x20, 0x03, 0x47, 0x00, 0x45, 0x00, 0x4e, 0x00, 0x45, 0x00, 0x52, 0x00, 0x41, 0x00, 0x4c, 0x00,
	0x20, 0x00, 0x2d, 0x00, 0x20, 0x00, 0x41, 0x00, 0x55, 0x00, 0x44, 0x00, 0x49, 0x00, 0x4f, 0x00,
};

u8 const UsbStrDescSerialNumber[30] =
{
	30,			// Num bytes of this descriptor
	3,			// String descriptor
	'2',	0,
	'0',	0,
	'1',	0,
	'6',	0,
	'0',	0,
	'4',	0,
	'1',	0,
	'8',	0,
	'V',	0,
	'1',	0,
	'.',	0,
	'0',	0,
	'0',	0,//v
	'0',	0
};
u8 const DEV_QAULIFIER_DESC_DATA[10]=
{
      10,
      DEVICE_QUALIFIER_DESCRIPTOR,
      0x00,
      0x02,
      0x00,
      0x00,
      0x00,
      EP0_BUF_SIZE,
      1,
      0
};


//u8 ep0buf[64] __attribute__ ((section("._ep0_fif0_")));
bool (*pSetReqCallback)(u8 *rxbuf);
void (*fp_uvc_on_hal)(void);
void (*fp_uvc_off_hal)(void);
void (*fp_uac_on_hal)(void);
void (*fp_uac_off_hal)(void);
bool (*fp_uac_unit_ctl_hal)(u8 val, u8 rqu, u8 len);




USB_DEV usb_dev;


USB_RQU usb_rqu;



#define USB_REQUEST_DIRECTION          (usb_rqu.rtype & 0x80)
#define USB_REQUEST_TYPE               (usb_rqu.rtype & 0x60)
#define USB_REQUEST_RECIPIENT          (usb_rqu.rtype & 0x1f)
#define USB_REQUEST_ENDPOINT           (usb_rqu.index&0xff)
#define USB_REQUEST_FEATURE_SELECTOR   (usb_rqu.value)
#define USB_REQUEST_DESCRIPTOR_TYPE    ((usb_rqu.value&0xff00)>>8)
#define USB_REQUEST_DESCRIPTOR_INDEX   (usb_rqu.value&0xff)
#define USB_REQUEST_LENGTH             (usb_rqu.length)

/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usbdev_stack_init(void)
{
	memset((u8 *)&usb_dev, 0, sizeof(usb_dev));
}
bool usbdev_rqu(void)
{
	return usb_dev.connect;	
}
void usbdev_reset(void)
{
	usb_dev.connect = false;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//Bulk-Only Transportå¤ä½
void usb_bot_reset(void)
{
	usb_dev.ep1_tx_stall = 0;
    usb_dev.ep1_rx_stall = 0;
    usb_dev.ep1_pipe_stall = 0;

    // Descriptor: mass storage reset and flush FIFO data
	USB20_Index = 1;
	USB20_InCSR1 = (BIT(6) | BIT(3));		//Clear Toggle, flush tx FIFO
	USB20_InCSR2 = BIT(5);	            //Set this EP = TX

	USB20_OutCSR1 = (BIT(7) | BIT(4));	    //Clear Toggle, flush rx FIFO
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_check_faddr(void)
{
    if (usb_dev.set_addr) {
        usb_dev.set_addr = 0;
        USB20_FAddr = ((usb_rqu.value&0xff) | 0x80);
    }
}	
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_clrrx_pkt0(void)
{
	USB20_Index = 0;
    USB20_CSR0 = (BIT(6) | BIT(4));
    usb_dev.ep0_state = TRANSER_OVER_STATE;
    return true;
}


/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_stall_ep(u8 epnum)
{
    u8 index_bak = USB20_Index; //ä¿å­ç«¯ç¹
    USB20_Index = (epnum & 0x7f);
    switch (epnum) {
    case 0x00:
        USB20_CSR0 = (BIT(6) | BIT(5));    //RxRdy, Stall
        break;
    case 0x01:
        usb_dev.ep1_rx_stall = 1;
        USB20_OutCSR1 = BIT(5);           //Stall
        break;
    case 0x81:
        usb_dev.ep1_tx_stall = 1;
        USB20_InCSR1 = BIT(4);           //Stall
        break;
    }
    USB20_Index = index_bak; //æ¢å¤ç«¯ç¹
}

/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
******************************************************************************
void usb_stall_all(u8 epin, u8 epout)
{
    usb_dev.ep1_pipe_stall = 1;
	usb_stall_ep(epout);
	usb_stall_ep(epin);
	while(usb_dev.ep1_pipe_stall) {
        if(usb_dev.ep1_rx_stall == 0) {
            usb_stall_ep(0x01);
		}
        if(usb_dev.ep1_tx_stall == 0) {
    		usb_stall_ep(0x81);
		}
    }
}
*/
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//Clearä¸ä¸ªEndPoint
void usb_clear_ep(u8 epnum)
{
    u8 index_bak = USB20_Index;
    USB20_Index = (epnum & 0x7f);
    switch (epnum) {
    case 0x01:
        usb_dev.ep1_rx_stall = 0;
        USB20_OutCSR1 = BIT(7);    //Clear Data Toggle
        break;
    case 0x81:
        usb_dev.ep1_tx_stall = 0;
        USB20_InCSR1 = BIT(6);    //Clear Data Toggle
        break;
    }
    USB20_Index = index_bak;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_ep0_get_request(void)
{
	memcpy((u8 *)&usb_rqu, usb_dev.ep0_buf, 8);
	usb_dev.connect = true;
#ifdef _DEBG_USB_ENUM__
    //debg("request:");
	debgbuf(usb_dev.ep0_buf,8);
#endif
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_transfer(void)
{
    u8 cmd, len;
	//USB20_Index = 0x00;
    len = usb_dev.ep0_len;
    if (usb_dev.ep0_len) {
        if (usb_dev.ep0_len > usb_dev.ep0_pkt) {
            len = usb_dev.ep0_pkt;
        }
        usb_dev.ep0_len -= len;
        //debgbuf(usb_dev.ep0_ptr,len);
        memcpy(usb_dev.ep0_buf, usb_dev.ep0_ptr, len);
		usb_dev.ep0_ptr += len;
		USB20E0ADR = (u32)usb_dev.ep0_buf;
		USB20CON1 = BIT(13 + 0) + len;
    }
    cmd = BIT(1);
    if (len != usb_dev.ep0_pkt) {
        cmd |= BIT(3);
        usb_dev.ep0_state = TRANSER_OVER_STATE;
		//debg("TRANSER_OVER_STATE\n");
    }
    USB20_CSR0 = cmd;  //DataEnd, TxRdy
    //while(read_usb_reg(CSR0) & BIT(1));
	//debgbuf(ep0buf,len);
    return true;
}

/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_tx(u8* txbuf, u8 len)
{
    USB20_CSR0 = BIT(6);           //æ¸é¤RxRdy
	
	//debgbuf(txbuf,len);
	memcpy(usb_dev.ep0_buf, txbuf, len);
	//USB20_FIFO0 = usb_dev.ep0_buf[0];
	//USB20CON1 &= ~(0x1fff);
	//USB20CON1 |= len;
	//USB20CON1 |= BIT(13 + 0);
	USB20E0ADR = (u32)usb_dev.ep0_buf;
	USB20CON1 = BIT(13 + 0) + len;
	
    usb_dev.ep0_state = TRANSER_OVER_STATE;
	USB20_CSR0 = BIT(3) | BIT(1);  //DataEnd, TxRdy
    return true;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_ep0_receive(void)
{	
	//debg("rvc..\n");
	if(NULL != pSetReqCallback) 
		(*pSetReqCallback)(usb_dev.ep0_buf);
	pSetReqCallback = NULL;	
    usb_ep0_clrrx_pkt0();
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_get_status(void)
{
    if (0 == USB_REQUEST_DIRECTION) {          //æ£æ¥æ°æ®æ¹å?
        return false;
    }
    if (USB_DEFAULT_STATE == usb_dev.cur_state) {
        return false;
    }
    usb_dev.ep0_buf[0] = 0;
    switch (USB_REQUEST_RECIPIENT) {           //å¤æ­æ¥æ¶è?
	
    case REQUEST_TO_DEVICE:                    //ä¸æ¯æè¿ç¨å¤éåèªä¾ç?
        break;
		
    case REQUEST_TO_INTERFACE:
        if (USB_ADDRESS_STATE == usb_dev.cur_state) {
            return false;
        }
        break;                                 //æ¥å£ç´æ¥å?
    case REQUEST_TO_ENDPOINT:
        //æ¥è¯¢ç«¯ç¹æ¯å¦è¢«Stall
        if (USB_ADDRESS_STATE == usb_dev.cur_state && 0 != USB_REQUEST_ENDPOINT) {
            return false;
        }

        if ((USB_REQUEST_ENDPOINT & 0x7f) <= MAX_EP_D) {
            if (USB_REQUEST_ENDPOINT & 0x80) {
                if (usb_dev.ep1_tx_stall) {
                    usb_dev.ep0_buf[0] = 1;
                }
            } else {
                if (usb_dev.ep1_rx_stall) {
                    usb_dev.ep0_buf[0] = 1;
                }
            }
        }
        break;
    default:
        return false;
    }
    usb_dev.ep0_buf[1] = 0;                         //é«Byteå?
    return usb_ep0_tx(usb_dev.ep0_buf,2);
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//Clear Feature
bool usb_ep0_clr_feature(void)
{
    if (USB_DEFAULT_STATE != usb_dev.cur_state &&
      REQUEST_TO_ENDPOINT == USB_REQUEST_RECIPIENT &&
      ENDPOINT_STALL == USB_REQUEST_FEATURE_SELECTOR &&
      !(USB_ADDRESS_STATE == usb_dev.cur_state && USB_REQUEST_ENDPOINT)) {

        usb_clear_ep(USB_REQUEST_ENDPOINT);
        return usb_ep0_clrrx_pkt0();
    }
    return false;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_set_feature(void)
{
    if (REQUEST_TO_ENDPOINT == USB_REQUEST_RECIPIENT &&
      ENDPOINT_STALL == USB_REQUEST_FEATURE_SELECTOR &&
      !(USB_ADDRESS_STATE == usb_dev.cur_state && USB_REQUEST_ENDPOINT)) {

        usb_stall_ep(USB_REQUEST_ENDPOINT);
        return usb_ep0_clrrx_pkt0();
    }
    return false;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_set_address(void)
{
    switch (usb_dev.cur_state) {
    case USB_CONFIG_STATE:
        return false;
    case USB_DEFAULT_STATE:
        if (usb_rqu.value) {
            usb_dev.cur_state = USB_ADDRESS_STATE;
        }
        break;
    case USB_ADDRESS_STATE:
        if (usb_rqu.value == 0) {
            usb_dev.cur_state = USB_DEFAULT_STATE;
        }
        break;
    }
    usb_dev.set_addr = 1;
    return usb_ep0_clrrx_pkt0();
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//Get Configuration
bool usb_ep0_get_configuration(void)
{
    if (USB_DEFAULT_STATE != usb_dev.cur_state) {
        usb_dev.ep0_buf[0] = usb_dev.cfg_value;
        return usb_ep0_tx(usb_dev.ep0_buf,1);
    }
    return false;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_set_configuration(void)
{
    /**/
	
    if (USB_DEFAULT_STATE != usb_dev.cur_state) {
        USB20_IntrUSBE = (/*USB_SOF|*/ BIT(2) | BIT(0) | BIT(3));  //Enable Suspend int
        //write_usb_reg(Power, BIT(7) | BIT(0));          //å¯ç¨ISO Update ä¸?Suspend Mode
        if (usb_rqu.value <= 1) {
            usb_dev.cfg_value = usb_rqu.value;
            if (usb_dev.cfg_value == 0) {
                usb_dev.cur_state = USB_ADDRESS_STATE;
            } else {
                usb_dev.cur_state = USB_CONFIG_STATE;
				//debg("USB_CONFIG_STATE\n");
            }
            usb_bot_reset();                  //å¤ä½Bulkä¼ è¾
            USB20_Index = 0;        //éæ°éæ©EP0
            return usb_ep0_clrrx_pkt0();
        }
    }
    
    return usb_ep0_clrrx_pkt0();
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//Get Interface
bool usb_ep0_get_interface(void)
{
    if (USB_CONFIG_STATE == usb_dev.cur_state) {
        usb_dev.ep0_buf[0] = 0;//usb_config_all_descriptor[12];
//		#warning "usb_config_all_descriptor"
        return usb_ep0_tx(usb_dev.ep0_buf,1);
    }
    return false;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//Set Interface
volatile u8 uvc_onflg;
volatile u8 uac_onflg;
bool uvc_ready(void)
{
	return 	uvc_onflg;
}
bool uac_ready(void)
{
	return 	uac_onflg;
}
 bool usb_ep0_set_interface(void)
{

    if (USB_CONFIG_STATE == usb_dev.cur_state) {

        if (usb_dev.uac_tx_interface == usb_rqu.index) {
            //æ¯æ¬¡å¼å¯åå³é­MICä¹åï¼é½ä¼set interface
            if (usb_rqu.value) {
				debg("UAC ON\n");
				(*fp_uac_on_hal)();
				uac_onflg = 1;
                usb_dev.uac_tx_enable_int = 1;
            } else {
				debg("UAC OFF\n");
				(*fp_uac_off_hal)();
				uac_onflg = 0;
                usb_dev.uac_tx_enable_int = 0;
            }
        }

        if (usb_dev.uvc_tx_interface == usb_rqu.index) {
            //æ¯æ¬¡å¼å¯åå³é­MICä¹åï¼é½ä¼set interface
            if (usb_rqu.value) {
				debg("UVC ON\n");			
				//(*uvc_on_hal)();
				__FP__(fp_uvc_on_hal);
				uvc_onflg = 1;
                usb_dev.uvc_tx_enable_int = 1;
            } else {
				debg("UVC OFF\n");
				uvc_onflg = 0;
				//(*uvc_off_hal)();
				__FP__(fp_uvc_off_hal);
                usb_dev.uvc_tx_enable_int = 0;
				USB20_Index = __UVC_EPX__;//2;
				USB20_InCSR1 = (USB20_InCSR1 | 0X08) & (~BIT(0));
            }
        }
		
        return usb_ep0_clrrx_pkt0();
		
    }
    return false;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_descriptor(void *buf, u16 len)
{
    if (len > USB_REQUEST_LENGTH) {
        len = USB_REQUEST_LENGTH;
    }

    usb_dev.ep0_ptr = (u8 *)buf;
    usb_dev.ep0_len = len;
	//debg("len:");
	//debgbuf(&len,2);
    usb_dev.ep0_state = TRANSFERING_STATE;
    USB20_CSR0 = BIT(6);           //æ¸é¤RxRdy
	//debg("usb_ep0_transfer\n");
    return usb_ep0_transfer();
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//Get Descriptor
/*
desc[DEV_YPE].pdev
desc[DEV_YPE].devlen
desc[DEV_YPE].pcfg
desc[DEV_YPE].cfglen
 */
bool usb_ep0_get_descriptor(void)
{
    //dbg_printf("\r\n usb_ep0_get_descriptor\n");
    //dbg_printf("\r\n type = %x\n",USB_REQUEST_DESCRIPTOR_TYPE);
    //u8 i;
    switch (USB_REQUEST_DESCRIPTOR_TYPE) {       //Request Descriptor Type
    case DEVICE_DESCRIPTOR:
	  // return usb_ep0_descriptor((u8*)desc[syscfg.usbtypdev].pdev, desc[syscfg.usbtypdev].devlen);
	  //return usb_ep0_descriptor(TYPE_UDISK_DEVICE, sizeof(TYPE_UDISK_DEVICE));
	  //return usb_ep0_descriptor((void *)TYPE_COMBYUV_DEVICE, sizeof(TYPE_COMBYUV_DEVICE));
	  //return usb_ep0_descriptor(TYPE_COMBMJP_DEVICE, sizeof(TYPE_COMBMJP_DEVICE));
		return usb_ep0_descriptor((void *)usbdsc.pdev, usbdsc.devlen);

    case CONFIGURATION_DESCRIPTOR:
		//desc[syscfg.usbtypdev].pcfg[144] = syscfg.uvcsol;
		//return usb_ep0_descriptor((u8*)desc[syscfg.usbtypdev].pcfg, desc[syscfg.usbtypdev].cfglen);
		//return usb_ep0_descriptor(TYPE_UDISK_CONFIG, sizeof(TYPE_UDISK_CONFIG));
		//return usb_ep0_descriptor((void *)TYPE_COMBYUV_CONFIG, sizeof(TYPE_COMBYUV_CONFIG));
		//return usb_ep0_descriptor(TYPE_COMBMJP_CONFIG, sizeof(TYPE_COMBMJP_CONFIG));
		return usb_ep0_descriptor((void *)usbdsc.pcfg, usbdsc.cfglen);

		
     case STRING_DESCRIPTOR:
        //å­ç¬¦ä¸?
        switch (USB_REQUEST_DESCRIPTOR_INDEX) {
        case 0:
            return usb_ep0_descriptor((u8 *)UsbLanguageID, sizeof(UsbLanguageID));
        case 1:
            return usb_ep0_descriptor((u8 *)UsbStrDescManufacturer, sizeof(UsbStrDescManufacturer));
        case 2:
            return usb_ep0_descriptor((u8 *)UsbStrDescProduct_0, sizeof(UsbStrDescProduct_0));
        case 3:
            return usb_ep0_descriptor((u8 *)UsbStrDescSerialNumber, sizeof(UsbStrDescSerialNumber));
		case 4:
			return usb_ep0_descriptor((u8 *)UsbStrDescProduct_1, sizeof(UsbStrDescProduct_1));
        default:/**/
            return false;
        }
//    case INTERFACE_DESCRIPTOR:
//    case ENDPOINT_DESCRIPTOR:
    case DEVICE_QUALIFIER_DESCRIPTOR:
        return usb_ep0_descriptor((u8 *)DEV_QAULIFIER_DESC_DATA, sizeof(DEV_QAULIFIER_DESC_DATA));

    default:
        return false;
    }
}

/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//æ åå½ä»¤
bool usb_ep0_standard(void)
{
	//debg(".request:%x\n",usb_rqu.request); 
    //æ åå½ä»¤
	
    switch (usb_rqu.request) {
    case 0x00:     //Get Status
        return usb_ep0_get_status();
    case 0x01:     //Clear Feature
        return usb_ep0_clr_feature();
    case 0x03:     //Set Feature
        return usb_ep0_set_feature();
    case 0x05:     //Set Address
		//debg("-usb set adr\n");
        //usb_dev.ep0_pkt = 64;//tbl_usb_device_descriptor[7];         //è®¾å°ååï¼è®¾ç½®åå¤§å°?
        return usb_ep0_set_address();
    case 0x06:     //Get Descriptor
        return usb_ep0_get_descriptor();
    case 0x08:     //Get Configuration
        return usb_ep0_get_configuration();
    case 0x09:     //Set Configuration
        return usb_ep0_set_configuration();
    case 0x0a:     //Get Interface
        return usb_ep0_get_interface();
    case 0x0b:     //Set Interface

        return usb_ep0_set_interface();
    default:
        return false;
    }
}

 bool usb_ep0_class_get_lun(void)
{
    usb_dev.ep0_buf[0] = 0;
    return usb_ep0_tx(usb_dev.ep0_buf,1);
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
 bool usb_ep0_class(void)
{
	u8 ret = true;
	#define __UVCID__(a,b) ((a << 8)|(b << 0))
	
        if (0xa1 == usb_rqu.rtype && 0xfe == usb_rqu.request) {
            //Class Get Max Lun
            ret = usb_ep0_class_get_lun();
        }
		
		else if(usb_rqu.index == __UVCID__(2,0)){
			//hum	
			ret = (*fp_uvc_unit_ctl_hal)(usb_rqu.value >> 8,usb_rqu.request,usb_rqu.length);
		}
		else if(usb_rqu.index == __UVCID__(3,0)){
			//hum	
			ret = /*(*fp_uvc_unit_ctl_ex_hal)*/uvc_unit_ctl_ex_hal(usb_rqu.value >> 8,usb_rqu.request,usb_rqu.length);
		}
		else if(usb_rqu.index == __UVCID__(4,0)){
			ret = uvc_unit_ctl_hal(usb_rqu.value >> 8,usb_rqu.request,usb_rqu.length);
		}
		else if(usb_rqu.index == __UVCID__(5,0)){
			ret = (*fp_uvc_unit_ctl_hal)(usb_rqu.value >> 8,usb_rqu.request,usb_rqu.length);
		}
		else if(usb_rqu.index == __UVCID__(5,2)){
			ret = (*fp_uac_unit_ctl_hal)(usb_rqu.value >> 8,usb_rqu.request,usb_rqu.length);	
		}
		
		//è¯·æ±UAC
		else if(usb_rqu.index == 0x82){
			ret = (*fp_UacHandleToStreaming)(usb_rqu.value >> 8,usb_rqu.request,usb_rqu.length);	
		}
		
		else if(1 == usb_rqu.index){
			ret = (*fp_uvc_probe_ctl_hal)(usb_rqu.value >> 8,usb_rqu.request,usb_rqu.length);	
			
		}
			
		return ret;
		
}

/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_ep0_request(void)
{
    u8 rtype = USB_REQUEST_TYPE;
	
    if (0 == rtype) {
        return usb_ep0_standard();                 //æ åå½ä»¤å¤ç
    } else if (0x20 == rtype) {
        //ç±»å½ä»?
        return usb_ep0_class();                     //ç±»å½ä»¤å¤ç?
    }
	
    return false;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_ep0_hal(void)
{
    //USB EP0
	//debg("ep0_isr\n");
    u8 index_temp = USB20_Index;
    USB20_Index = 0;                  //éæ©EP0
    usb_check_faddr();                          //æ£æ¥æ¯å¦éè¦æ´æ°å°å
    u8 csr0 = USB20_CSR0;
    if (csr0 & BIT(2)) {                        //SentStall
        USB20_CSR0 = 0;
        usb_dev.ep0_state = IDLE_STATE;
    }

    if (csr0 & BIT(4)) {                        //SetupEnd
       USB20_CSR0 = BIT(7);
        usb_dev.ep0_state = IDLE_STATE;
    }

    if (usb_dev.ep0_state == TRANSER_OVER_STATE) {
        usb_dev.ep0_state = IDLE_STATE;
    }
	//debg("- csr0:%x\n",csr0);
	
    switch (usb_dev.ep0_state) {
    case IDLE_STATE:
        if (csr0 & BIT(0)) {                    //EP0 RxRdy
            usb_ep0_get_request();              //è·åå½ä»¤
			if (!usb_ep0_request()) {           //å¤çå½ä»¤
				usb_stall_ep(0x00);
				
            }
			
        }
        break;
    case TRANSFERING_STATE:
	     //debg("2222\n");
        usb_ep0_transfer();
        break;
    case RECEIVING_STATE:
	    usb_ep0_receive();
        break;
    default:
        break;
    }
    USB20_Index = index_temp;
}


void enum_epx_cfg(u8 devtyp)
{
	usb_dev.ep0_buf = _ep0_fifo_;
    usb_dev.ep0_pkt = 64;                    //初始包大小
    usb_dev.type = devtyp;
	usb_dev.uvc_tx_interface = __UVC_STRM_INTFS__;
	usb_dev.uac_tx_interface = __UAC_STRM_INTFS__;
	
	fp_uac_unit_ctl_hal = uac_unit_ctl_hal;
	fp_UacHandleToStreaming = UacHandleToStreaming;
	fp_uvc_probe_ctl_hal = uvc_probe_ctl_hal;
	fp_usb_ep0_tx = usb_ep0_tx;
	USB20E0ADR = (u32)usb_dev.ep0_buf;
}
void usb_dev_enum_reg(u8 tpe)
{
	usbdev_stack_init();
	//mass 
	if(0 == tpe)
	{
		usbdsc.pdev = msc_dev;
		usbdsc.devlen = sizeof(msc_dev);
		usbdsc.pcfg = msc_cfg;
		usbdsc.cfglen = sizeof(msc_cfg); 
		debg("USB device = MSC\n");
	}
	//uvc
	else if(1 == tpe)
	{

		usbdsc.pdev = uvc_dev;
		usbdsc.devlen = sizeof(uvc_dev);
		debg("USB device = UVC,devSensorOp->pixelw=%d\n",devSensorOp->pixelw);
#if 0
		if(1280 == devSensorOp->pixelw)
		{
			usbdsc.pcfg = uvc_cfg_720p;
			usbdsc.cfglen = sizeof(uvc_cfg_720p); 
		}
		else
#endif
		{
			usbdsc.pcfg = uvc_cfg_vga;
			usbdsc.cfglen = sizeof(uvc_cfg_vga); 
		}

		debg("USB device = UVC\n");
	}
	else if(2 == tpe)
	{
		usbdsc.pdev = TYPE_COMB_DEVICE;
		usbdsc.devlen = sizeof(TYPE_COMB_DEVICE);
#if 0
		if(1280 == devSensorOp->pixelw)
		{
			usbdsc.pcfg = TYPE_COMB_CONFIG_720P;
			usbdsc.cfglen = sizeof(TYPE_COMB_CONFIG_720P); 
		}
		else
#endif
		{
			usbdsc.pcfg = TYPE_COMB_CONFIG_VGA;
			usbdsc.cfglen = sizeof(TYPE_COMB_CONFIG_VGA); 
		}
		debg("USB device = UVC+UAC+MSC\n");		
	}
	
}

