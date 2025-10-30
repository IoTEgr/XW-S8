/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                     WATER MARK DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_watermark.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN WATER MARK HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"


#define  R_MJPEGA_TIMEINFO_CTRL         PRE_JPTINF

#define  R_MJPEGA_TIMEINFO_SIZE0        PRE_JPT0SIZE
#define  R_MJPEGA_TIMEINFO_ADDR0        PRE_JPT0BASE
#define  R_MJPEGA_TIMEINFO_POS0         PRE_JPT0POS

#define  R_MJPEGA_TIMEINFO_SIZE1        PRE_JPT1SIZE
#define  R_MJPEGA_TIMEINFO_ADDR1        PRE_JPT1BASE
#define  R_MJPEGA_TIMEINFO_POS1         PRE_JPT1POS

#define  R_MJPEGA_TIMEINFO_SIZE2        PRE_JPT2SIZE
#define  R_MJPEGA_TIMEINFO_ADDR2        PRE_JPT2BASE
#define  R_MJPEGA_TIMEINFO_POS2         PRE_JPT2POS

#define  R_MJPEGA_TIMEINFO_SIZE3        PRE_JPT3SIZE
#define  R_MJPEGA_TIMEINFO_ADDR3        PRE_JPT3BASE
#define  R_MJPEGA_TIMEINFO_POS3         PRE_JPT3POS

#define  R_MJPEGB_TIMEINFO_CTRL         BACK_JPTINF
#define  R_MJPEGB_TIMEINFO_SIZE0        BACK_JPT0SIZE
#define  R_MJPEGB_TIMEINFO_ADDR0        BACK_JPT0BASE
#define  R_MJPEGB_TIMEINFO_POS0         BACK_JPT0POS
#define  R_MJPEGB_TIMEINFO_SIZE1        BACK_JPT1SIZE
#define  R_MJPEGB_TIMEINFO_ADDR1        BACK_JPT1BASE
#define  R_MJPEGB_TIMEINFO_POS1         BACK_JPT1POS
#define  R_MJPEGB_TIMEINFO_SIZE2        BACK_JPT2SIZE
#define  R_MJPEGB_TIMEINFO_ADDR2        BACK_JPT2BASE
#define  R_MJPEGB_TIMEINFO_POS2         BACK_JPT2POS

#define  R_MJPEGB_TIMEINFO_SIZE3        BACK_JPT2SIZE
#define  R_MJPEGB_TIMEINFO_ADDR3        BACK_JPT2BASE
#define  R_MJPEGB_TIMEINFO_POS3         BACK_JPT2POS

typedef struct
{
	u32 ctl;
	u32 size0;
	u32 size1;
	u32 size2;
	u32 pos0;
	u32 pos1;
	u32 pos2;
	u32 adr0;
	u32 adr1;
	u32 adr2;
}MJPBTMINF_MAP;

static MJPBTMINF_MAP mjpBtminf;
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoEnable
* Description    : mjpeg time info en
* Input          : u8 layer : layer
                      u8 en : 0->disable,
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegATimeinfoEnable(u8 layer,u8 en)
{
	if(MJPEG_TIMEINFO_MAX>layer)
	{
		if(en)
		{
			if(layer==MJPEG_TIMEINFO_LAYER3)
				R_MJPEGA_TIMEINFO_CTRL|=(1<<27);
			else
				R_MJPEGA_TIMEINFO_CTRL|=(1<<(26-layer));
		}
		else
		{
			if(layer==MJPEG_TIMEINFO_LAYER3)
				R_MJPEGA_TIMEINFO_CTRL&=(~(1<<27));
			else
				R_MJPEGA_TIMEINFO_CTRL&=(~(1<<(26-layer)));
		}
			
	}
	mjpBtminf.ctl = R_MJPEGB_TIMEINFO_CTRL;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoEnable
* Description    : mjpeg time info en
* Input          : u8 layer : layer
                      u8 en : 0->disable,
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegBTimeinfoEnable(u8 layer,u8 en)
{
	if(MJPEG_TIMEINFO_MAX>layer)
	{
		if(en)
		{
			if(layer==MJPEG_TIMEINFO_LAYER3)
				layer=MJPEG_TIMEINFO_LAYER2;
			R_MJPEGB_TIMEINFO_CTRL|=(1<<(26-layer));
		}
		else
			R_MJPEGB_TIMEINFO_CTRL&=~(1<<(26-layer));
	}
	mjpBtminf.ctl = R_MJPEGB_TIMEINFO_CTRL;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoColor
* Description    : mjpeg time info color in yuv422
* Input          : u32 y : y-byte
                      u32 u : u-bye
                      u32 v : v-byte
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegATimeinfoColor(u32 y,u32 u,u32 v)
{
	R_MJPEGA_TIMEINFO_CTRL &= ~0xffffff;
	R_MJPEGA_TIMEINFO_CTRL |= ((y<<16)|(u<<8)|(v));
}
void ax32xx_mjpegBTimeinfoColor(u32 y,u32 u,u32 v)
{
	R_MJPEGB_TIMEINFO_CTRL &= ~0xffffff;
	R_MJPEGB_TIMEINFO_CTRL |= ((y<<16)|(u<<8)|(v));
	mjpBtminf.ctl = R_MJPEGB_TIMEINFO_CTRL;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoSize
* Description    : mjpeg time info ram size
* Input          : u8 layer : timeinfo layer
                      u32 width    : width -byte
                      u32 height   : height-byte
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegATimeinfoSize(u8 layer,u32 width,u32 height)
{

	switch(layer)
	{
		case MJPEG_TIMEINFO_LAYER0 : R_MJPEGA_TIMEINFO_SIZE0 = (width<<16)|height;break;
		case MJPEG_TIMEINFO_LAYER1 : R_MJPEGA_TIMEINFO_SIZE1 = (width<<16)|height;break;
		case MJPEG_TIMEINFO_LAYER2 : R_MJPEGA_TIMEINFO_SIZE2 = (width<<16)|height;break;
		case MJPEG_TIMEINFO_LAYER3 : R_MJPEGA_TIMEINFO_SIZE3 = (width<<16)|height;break;
	}
}
void ax32xx_mjpegBTimeinfoSize(u8 layer,u32 width,u32 height)
{

	switch(layer)
	{
		case MJPEG_TIMEINFO_LAYER0 : R_MJPEGB_TIMEINFO_SIZE0 = (width<<16)|height;break;
		case MJPEG_TIMEINFO_LAYER1 : R_MJPEGB_TIMEINFO_SIZE1 = (width<<16)|height;break;
		case MJPEG_TIMEINFO_LAYER2 : R_MJPEGB_TIMEINFO_SIZE2 = (width<<16)|height;break;
		case MJPEG_TIMEINFO_LAYER3 : R_MJPEGB_TIMEINFO_SIZE3 = (width<<16)|height;break;
	}
	mjpBtminf.size0 =  R_MJPEGB_TIMEINFO_SIZE0;
	mjpBtminf.size1 =  R_MJPEGB_TIMEINFO_SIZE1;
	mjpBtminf.size2 =  R_MJPEGB_TIMEINFO_SIZE2;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoPos
* Description    : mjpeg time info position
* Input          : u8 layer : timeinfo layer
                      s32 x    : x position
                      s32 y   : y posotion
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegATimeinfoPos(u8 layer,s32 x,s32 y)
{
	switch(layer)
	{
		case MJPEG_TIMEINFO_LAYER0 : R_MJPEGA_TIMEINFO_POS0 = (x<<16)|y;break;
		case MJPEG_TIMEINFO_LAYER1 : R_MJPEGA_TIMEINFO_POS1 = (x<<16)|y;break;
		case MJPEG_TIMEINFO_LAYER2 : R_MJPEGA_TIMEINFO_POS2 = (x<<16)|y;break;
		case MJPEG_TIMEINFO_LAYER3 : R_MJPEGA_TIMEINFO_POS3 = (x<<16)|y;break;
	}	
}
void ax32xx_mjpegBTimeinfoPos(u8 layer,s32 x,s32 y)
{
	switch(layer)
	{
		case MJPEG_TIMEINFO_LAYER0 : R_MJPEGB_TIMEINFO_POS0 = (x<<16)|y;break;
		case MJPEG_TIMEINFO_LAYER1 : R_MJPEGB_TIMEINFO_POS1 = (x<<16)|y;break;
		case MJPEG_TIMEINFO_LAYER2 : R_MJPEGB_TIMEINFO_POS2 = (x<<16)|y;break;
		case MJPEG_TIMEINFO_LAYER3 : R_MJPEGB_TIMEINFO_POS3 = (x<<16)|y;break;
	}	
	mjpBtminf.pos0 = R_MJPEGB_TIMEINFO_POS0;
	mjpBtminf.pos1 = R_MJPEGB_TIMEINFO_POS1;
	mjpBtminf.pos2 = R_MJPEGB_TIMEINFO_POS2;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegTimeinfoAddr
* Description    : mjpeg time info adrr
* Input          : u8 layer : timeinfo layer
                      u32 addr: ram addrress
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpegATimeinfoAddr(u8 layer,u32 addr)
{
	switch(layer)
	{
		case MJPEG_TIMEINFO_LAYER0 : R_MJPEGA_TIMEINFO_ADDR0 = addr;break;
		case MJPEG_TIMEINFO_LAYER1 : R_MJPEGA_TIMEINFO_ADDR1 = addr;break;
		case MJPEG_TIMEINFO_LAYER2 : R_MJPEGA_TIMEINFO_ADDR2 = addr;break;
		case MJPEG_TIMEINFO_LAYER3 : R_MJPEGA_TIMEINFO_ADDR3 = addr;break;
	}	
}
void ax32xx_mjpegBTimeinfoAddr(u8 layer,u32 addr)
{
	switch(layer)
	{
		case MJPEG_TIMEINFO_LAYER0 : R_MJPEGB_TIMEINFO_ADDR0 = addr;break;
		case MJPEG_TIMEINFO_LAYER1 : R_MJPEGB_TIMEINFO_ADDR1 = addr;break;
		case MJPEG_TIMEINFO_LAYER2 : R_MJPEGB_TIMEINFO_ADDR2 = addr;break;
		case MJPEG_TIMEINFO_LAYER3 : R_MJPEGB_TIMEINFO_ADDR3 = addr;break;
	}	
	mjpBtminf.adr0 = R_MJPEGB_TIMEINFO_ADDR0;
	mjpBtminf.adr1 = R_MJPEGB_TIMEINFO_ADDR1;
	mjpBtminf.adr2 = R_MJPEGB_TIMEINFO_ADDR2;
}

void ax32xx_recfg_mjpb_tminf(void)
{
	R_MJPEGB_TIMEINFO_ADDR0 = mjpBtminf.adr0;
	R_MJPEGB_TIMEINFO_ADDR1 = mjpBtminf.adr1;
	R_MJPEGB_TIMEINFO_ADDR2 = mjpBtminf.adr2;	
	R_MJPEGB_TIMEINFO_POS0 = mjpBtminf.pos0;
	R_MJPEGB_TIMEINFO_POS1 = mjpBtminf.pos1;
	R_MJPEGB_TIMEINFO_POS2 = mjpBtminf.pos2;
	R_MJPEGB_TIMEINFO_SIZE0 = mjpBtminf.size0;
	R_MJPEGB_TIMEINFO_SIZE1 = mjpBtminf.size1;
	R_MJPEGB_TIMEINFO_SIZE2 = mjpBtminf.size2;
	R_MJPEGB_TIMEINFO_CTRL = mjpBtminf.ctl;
}
