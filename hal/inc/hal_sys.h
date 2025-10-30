/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                   THE APPOTECH HAL
   **** **                 SYSTEM HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_sys.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN SYSTEM HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  HAL_SYSTEM_H
#define  HAL_SYSTEM_H

#define HAL_ALLOC_DEBUG 1

typedef struct
{
	u8 ep0_fifo[64];
	u8 msc_txfifo[512+16];
	u8 msc_rxfifo[512+16];
	u8 uac_txfifo[128];
	u8 uvc_txfifo[__UVC_PKG_SZ__];
}__FIFO_USB__;

typedef struct
{
//	FATFS work_fatfs;
//	FIL Fil;
//	FIL iFil;
//	FIL BFil;
//	FIL BiFil;
//	u32 Tbdri[2][__DRI_ALIST__];
//	_TIMB_FONT_	tminfB_font ;
	_TIM_FONT_ 	tminf_font ;
	
	u8 ep0_buf[64];
	u8 msc_txbuf[512];
	u8 msc_rxbuf[512];
	u8 uac_txbuf[128];
	u8 uvc_txbuf[1][__UVC_PKG_SZ__];
	
}__RAM_USE__;

extern __RAM_USE__ 		ramuse  ;
#define Tbdri			ramuse.Tbdri
#define tminf_font 		ramuse.tminf_font
#define tminfB_font		ramuse.tminfB_font
//#define work_fatfs		ramuse.work_fatfs
//#define Fil				ramuse.Fil
//#define iFil			ramuse.iFil
//#define BFil			ramuse.BFil
//#define BiFil			ramuse.BiFil

#define _ep0_buf_ 		ramuse.ep0_buf
#define _msc_txbuf_ 	ramuse.msc_txbuf
#define _msc_rxbuf_ 	ramuse.msc_rxbuf
#define _uvc_txbuf_ 	ramuse.uvc_txbuf
#define _uac_txbuf_		ramuse.uac_txbuf
//#define _csi_histbuf_	ramuse.csi_stat.csi_histbuf
//#define _csi_wptbuf_	ramuse.csi_stat.csi_wpbuf
#define _uvc_payld_		ramuse.uvc_txbuf
#define _uac_buf_		ramuse.uac_txbuf

extern __FIFO_USB__ 	usbfifo;
#define _ep0_fifo_			usbfifo.ep0_fifo
#define _msc_txfifo_ 		usbfifo.msc_txfifo
#define _msc_rxfifo_ 		usbfifo.msc_rxfifo
#define _uvc_txfifo_ 		usbfifo.uvc_txfifo
#define _uac_txfifo_		usbfifo.uac_txfifo

extern unsigned char halSysMemLcdYBuffer[];
extern unsigned char halSysMemLcdUVBuffer[];
extern unsigned char halSysMemOSD0Buffer[];
extern unsigned char halSysMemOSD1Buffer[];
extern unsigned char halSysMemOSDTempBuffer[];
extern unsigned char halSysMemLineCache[];
extern unsigned char halSysMemIdxBuffer[];
extern unsigned char halSysMemPCMTempBuffer[];




/*******************************************************************************
* Function Name  : hal_sysInit
* Bref               : void hal_sysInit(int ms)
* Description    : hal layer.initial system for platform using.
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
int hal_sysInit(void);
/*******************************************************************************
* Function Name  : hal_sysMemPrint
* Description    : printf system memory info
* Input          : 
* Output         : None
* Return         :
*******************************************************************************/
void hal_sysMemPrint(void);

/*******************************************************************************
* Function Name  : hal_sysMemMalloc
* Bref               : hal_sysMemMalloc(u32 size,u8 algin)
* Description    : hal layer.system memory malloc.this function only for temp using.memory can not be free
* Input          : u32 size : malloc size
                      u8 algin : algin bit
* Output         : None
* Return         : None
*******************************************************************************/
#if HAL_ALLOC_DEBUG
void *hal_sysMemMallocExt(u32 size,u8 algin,const char *func,int line);
#define hal_sysMemMalloc(size, algin) hal_sysMemMallocExt(size, algin, __func__, __LINE__)
#else
void *hal_sysMemMalloc(u32 size,u8 algin);
#endif
/*******************************************************************************
* Function Name  : hal_sysMemFree
* Bref               : void hal_sysMemFree(void *mem)
* Description    : hal layer.system memory free
* Input          : void *mem : memory address
* Output         : None
* Return         : None
*******************************************************************************/
void hal_sysMemFree(void *mem);
/*******************************************************************************
* Function Name  : hal_sysMemRemain
* Bref               : u32 hal_sysMemRemain(void)
* Description    : hal layer.system memory remain check.this function only for temp using.memory can not be free
* Input          : 
* Output         : None
* Return         : u32 : remain size
*******************************************************************************/
u32 hal_sysMemRemain(void);
/*******************************************************************************
* Function Name  : hal_sysUninit
* Bref           : void hal_sysUninit(void)
* Description    : hal layer.uninitial system .
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
#define hal_sysUninit            ax32xx_sysUninit
/*******************************************************************************
* Function Name  : hal_sysDelayMS
* Bref               : void hal_sysDelayMS(int ms)
* Description    : hal layer.delay for ms in cpu nop
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
#define   hal_sysDelayMS           ax32xx_sysCpuMsDelay
/*******************************************************************************
* Function Name  : hal_sysReset
* Bref           : void hal_sysReset(void)
* Description    : hal layer.sysreset by watchdog
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
#define   hal_sysReset             ax32xx_wdtReset
/*******************************************************************************
* Function Name  : ax32xx_sysLDOSet
* Bref           : void hal_sysLDOSet(u8 ldo,u8 sel,u8 en)
* Description    : set system ldo for power support
* Input          : u8 ldo : ldo type
*                  u8 sel : power sel
*                  u8 en  : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
#define   hal_sysLDOSet            ax32xx_sysLDOSet
 /*******************************************************************************
* Function Name  : hal_sysDcacheWBack
* Bref           : void hal_sysDcacheWBack(u32 addr, u32 size)  
* Description    : data cache write back
* Input          : u32 addr : address
*                  u32 size : size
* Output         : None
* Return         : None
*******************************************************************************/
#define  hal_sysDcacheWBack     ax32xx_sysDcacheWback

#endif
