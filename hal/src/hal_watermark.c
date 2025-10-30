/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                   THE APPOTECH HAL
   **** **                 TINFO HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_watermark.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TINFO HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"


typedef struct HAL_WATERMARK_S
{
	u8 en;
	u8 stat;

	u16 width;
	u16 height;
	u16 x;
	u16 y;
	u8 *ram;
	void (*callback)(void);
}HAL_WATERMARK_T;
static HAL_WATERMARK_T hal_watermark_ctrl[MJPEG_TIMEINFO_MAX]=
{
	{
		.width=TIME0_W,
		.height=TIME0_H,
		.ram = tminf_font.win0,
    },
    {
		.width=TIME1_W,
		.height=TIME1_H,
		.ram = tminf_font.win1,
    },
    {
		.width=TIME2_W,
		.height=TIME2_H,
		.ram = tminf_font.win2,
    },
	{
		.width=TIME3_W,
		.height=TIME3_H,
		.ram = tminf_font.win3,
    }
};
static u32 HAL_Y,HAL_U,HAL_V;
static s32 HAL_B_Y_POS_ADJ;





/*******************************************************************************
* Function Name  : hal_watermarkISR
* Description    : hal watermark isr
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
static void hal_watermarkCallBack(void)
{
	int i;

	for(i=0;i<MJPEG_TIMEINFO_MAX;i++)
	{
		if(hal_watermark_ctrl[i].stat && hal_watermark_ctrl[i].en && hal_watermark_ctrl[i].callback)
			hal_watermark_ctrl[i].callback();
	}
}
/*******************************************************************************
* Function Name  : hal_watermarkInit
* Description    : hal watermark init
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkInit(void)
{
	int i;

	for(i=0;i<MJPEG_TIMEINFO_MAX;i++)
	{
		hal_watermark_ctrl[i].stat = 0;
		hal_watermark_ctrl[i].en = 0;
		hal_watermark_ctrl[i].callback = NULL;
		ax32xx_mjpegATimeinfoEnable(i,0); 
		ax32xx_mjpegBTimeinfoEnable(i,0);
	}
    HAL_Y = 0xff;HAL_U = 0x80;HAL_V = 0x80;
//	ax32xx_mjpegTimeinfoColor(0xff,0x80,0x80);  // set default color

	hal_rtcCallBackRegister(hal_watermarkCallBack);

	return 0;
}
/*******************************************************************************
* Function Name  : hal_watermarkClose
* Description    : hal watermark close stream
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkClose(s32 layer)
{
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return -1;
	
	if(hal_watermark_ctrl[layer].en){
		ax32xx_mjpegATimeinfoEnable(layer,0); // disable mjpeg timeinfo
		ax32xx_mjpegBTimeinfoEnable(layer,0); // disable mjpeg timeinfo
	}
	hal_watermark_ctrl[layer].stat = 0;
	
    return 0;
}
/*******************************************************************************
* Function Name  : hal_watermarkClear
* Description    : hal watermark clear 
* Input          :  s32 layer : layer
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkClear(s32 layer)
{
	int j;
	
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return -1;	
	for(j=0;j<(hal_watermark_ctrl[layer].width*hal_watermark_ctrl[layer].height/8);j++)
			hal_watermark_ctrl[layer].ram[j] = 0;
	return 0;
}
/*******************************************************************************
* Function Name  : hal_watermarkOpen
* Description    : hal watermark open stream
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkOpen(void)
{
	int i;

	for(i=0;i<MJPEG_TIMEINFO_MAX;i++)
	{
		if(hal_watermark_ctrl[i].stat==0)
		{
			if((hal_watermark_ctrl[i].width == 0)||(hal_watermark_ctrl[i].height==0))  // can not be used
		         continue;
			hal_watermark_ctrl[i].stat = 1;
			hal_watermark_ctrl[i].en = 0;
			hal_watermark_ctrl[i].callback = NULL;
            hal_watermarkClear(i);
			
			return i;
		}
	}

	return -1;
}

/*******************************************************************************
* Function Name  : hal_watermarkColor
* Description    : hal watermark color
* Input          : u8 layer : timeinfo layer
* Input		   : u32 y : y-byte
				u32 u : u-bye
				u32 v : v-byte
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkColor(s32 layer,u8 y,u8 u,u8 v)
{
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return -1;
	
	HAL_Y = y;HAL_U = u;HAL_V = v;
//	ax32xx_mjpegTimeinfoColor(HAL_Y,HAL_U,HAL_V);  // set default color

	return 0;
}
/*******************************************************************************
* Function Name  : hal_watermarkAddr
* Description    : hal watermark addr
* Input          : u8 layer : timeinfo layer
                      void *addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkAddr(s32 layer,void *addr)
{
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return -1;
//	ax32xx_mjpegTimeinfoAddr(layer,(u32)addr);   // can not use user ram

	return -1;
}
/*******************************************************************************
* Function Name  : hal_watermarkSize
* Description    : hal watermark size
* Input          : u8 layer : timeinfo layer
                      u32 width    : width
                      u32 height   : height
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkSize(s32 layer,u32 width,u32 height)
{
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return -1;

	hal_watermark_ctrl[layer].width = width;
	hal_watermark_ctrl[layer].height= height;
//    ax32xx_mjpegTimeinfoSize(layer,width,height);
	
    return 0;
}
/*******************************************************************************
* Function Name  : hal_watermarkPosition
* Description    : hal watermark psotion
* Input          : u8 layer : timeinfo layer
                      s32 x    : x position
                      s32 y   : y posotion
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkPosition(s32 layer,s32 x,s32 y)
{
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return -1;
	
		if(layer >= 2)
	{
		x = (x+0x1f)&~0x1f;
		y = (y+0x0f)&~0x0f;
	}
	hal_watermark_ctrl[layer].x = x;
	hal_watermark_ctrl[layer].y = y;
//	ax32xx_mjpegTimeinfoPos(layer,x,y);
    return 0;
}
/*******************************************************************************
* Function Name  : hal_watermarkCallback
* Description    : hal watermark callback register
* Input          : s32 layer : layer
                      void (*callback)(void) : callback
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkCallbackRegister(s32 layer,void (*callback)(void))
{
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return -1;
    hal_watermark_ctrl[layer].callback = callback;
	return 0;

}
/*******************************************************************************
* Function Name  : hal_watermarkRam
* Description    : hal watermark ram get
* Input          : s32 layer : layer
                      u32 width    : width
                      u32 height   : height                      
* Output         : None
* Return         : None
*******************************************************************************/
void  *hal_watermarkRam(s32 layer,INT16U *width,INT16U *height)
{
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return NULL;
    if((hal_watermark_ctrl[layer].width == 0)||(hal_watermark_ctrl[layer].height==0))
		return NULL;
		
	if(width)
		*width = hal_watermark_ctrl[layer].width;
	if(height)
		*height = hal_watermark_ctrl[layer].height;
	return (void *)hal_watermark_ctrl[layer].ram;

}
/*******************************************************************************
* Function Name  : hal_watermarkEnable
* Description    : hal watermark enable
* Input          : u8 layer : timeinfo layer
                      u8 en : enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkEnable(s32 layer,u8 en)
{
	if((layer<0)  || (layer>=MJPEG_TIMEINFO_MAX))
	     return -1;	
	if(en)
	{
		if((hal_watermark_ctrl[layer].width == 0)||(hal_watermark_ctrl[layer].height==0))  // this layer can not be used
		    return -1;
		if(layer>=2)
		{
			PRE_JPTINF|=(1<<30);
			PRE_JPTINF |= 0x3<<28;
		}
		ax32xx_mjpegATimeinfoPos(layer,hal_watermark_ctrl[layer].x,hal_watermark_ctrl[layer].y);
		ax32xx_mjpegBTimeinfoPos(layer,hal_watermark_ctrl[layer].x,/*hal_watermark_ctrl[layer].y*/HAL_B_Y_POS_ADJ-32);
		
        ax32xx_mjpegATimeinfoSize(layer,hal_watermark_ctrl[layer].width,hal_watermark_ctrl[layer].height);
		ax32xx_mjpegBTimeinfoSize(layer,hal_watermark_ctrl[layer].width,hal_watermark_ctrl[layer].height);
		
	    ax32xx_mjpegATimeinfoAddr(layer,(u32)hal_watermark_ctrl[layer].ram);
		ax32xx_mjpegBTimeinfoAddr(layer,(u32)hal_watermark_ctrl[layer].ram);
		
		ax32xx_mjpegBTimeinfoColor(HAL_Y,HAL_U,HAL_V);
		ax32xx_mjpegATimeinfoColor(HAL_Y,HAL_U,HAL_V);  // set default color
	}
	hal_watermark_ctrl[layer].en = en;
	ax32xx_mjpegATimeinfoEnable(layer,en);
	ax32xx_mjpegBTimeinfoEnable(layer,en);
	return 0;
}
void hal_watermarkBPos_Adjust(s32 y)
{
	HAL_B_Y_POS_ADJ = y;
}










