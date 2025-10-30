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
#ifndef HAL_WATERMARK_H
#define HAL_WATERMARK_H


/**********for timeinfo*****************************/
 #define  TIME_INFO_Y        	0xff //0xe1//0x7f//0xe1      // Time information color Y: JPTINF[23:16]
#define   TIME_INFO_U        	0x80 //0x01//0x95//0x01      // Time information color U: JPTINF[15:8]
#define   TIME_INFO_V        	0x80 //0x95//0x01//0x95      // Time information color V: JPTINF[7:0]

#define	  TIME3_INFO_LEN	0	     // the length of the time info buffer  
#define	  TIME3_FONT_W		32
#define	  TIME3_FONT_H		32
#define   TIME3_W			TIME3_FONT_W*TIME3_INFO_LEN 
#define   TIME3_H       	TIME3_FONT_H
#define   FONT3_LEN         TIME3_FONT_W*TIME3_FONT_H/8

#define	  TIME2_INFO_LEN	0//20  	     // the length of the time info buffer  
#define	  TIME2_FONT_W		32//16
#define	  TIME2_FONT_H		32//24
#define   TIME2_W			TIME2_FONT_W*TIME2_INFO_LEN 
#define   TIME2_H       	TIME2_FONT_H
#define   FONT2_LEN         TIME2_FONT_W*TIME2_FONT_H/8                



#define	  TIME1_INFO_LEN	0//20	     // the length of the time info buffer  
#define	  TIME1_FONT_W		32//16
#define	  TIME1_FONT_H		32//24
#define   TIME1_W			TIME1_FONT_W*TIME1_INFO_LEN 
#define   TIME1_H       	TIME1_FONT_H
#define   FONT1_LEN         TIME1_FONT_W*TIME1_FONT_H/8    



#define	  TIME0_INFO_LEN	20//20	     // the length of the time info buffer  
#define	  TIME0_FONT_W		40//32
#define	  TIME0_FONT_H		64//32
#define   TIME0_W			TIME0_FONT_W*TIME0_INFO_LEN 
#define   TIME0_H       	TIME0_FONT_H
#define   FONT0_LEN         TIME0_FONT_W*TIME0_FONT_H/8    


#define    FONT_LEN 		FONT0_LEN
#define	   TIME_INFO_LEN  	TIME0_INFO_LEN
#define    TIME_W			TIME0_W
#define    TIME_H			TIME0_H



#define W16   (16*24/8/2)


typedef struct
{
	u8 win0[TIME0_W*TIME0_H/8];
	u8 win1[TIME1_W*TIME1_H/8];
	u8 win2[TIME2_W*TIME2_H/8];
    u8 win3[TIME3_W*TIME3_H/8];

}_TIM_FONT_;


/*******************************************************************************
* Function Name  : hal_watermarkInit
* Description    : hal watermark init
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkInit(void);
/*******************************************************************************
* Function Name  : hal_watermarkClose
* Description    : hal watermark close stream
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkClose(s32 layer);
/*******************************************************************************
* Function Name  : hal_watermarkClear
* Description    : hal watermark clear 
* Input          :  s32 layer : layer
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkClear(s32 layer);
/*******************************************************************************
* Function Name  : hal_watermarkOpen
* Description    : hal watermark open stream
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkOpen(void);
/*******************************************************************************
* Function Name  : hal_watermarkEnable
* Description    : hal watermark enable
* Input          : u8 layer : timeinfo layer
                      u8 en : enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkEnable(s32 layer,u8 en);
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
s32 hal_watermarkColor(s32 layer,u8 y,u8 u,u8 v);
/*******************************************************************************
* Function Name  : hal_watermarkAddr
* Description    : hal watermark addr
* Input          : u8 layer : timeinfo layer
                      void *addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkAddr(s32 layer,void *addr);
/*******************************************************************************
* Function Name  : hal_watermarkSize
* Description    : hal watermark size
* Input          : u8 layer : timeinfo layer
                      u32 width    : width
                      u32 height   : height
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkSize(s32 layer,u32 width,u32 height);
/*******************************************************************************
* Function Name  : hal_watermarkPosition
* Description    : hal watermark psotion
* Input          : u8 layer : timeinfo layer
                      s32 x    : x position
                      s32 y   : y posotion
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkPosition(s32 layer,s32 x,s32 y);
/*******************************************************************************
* Function Name  : hal_watermarkCallback
* Description    : hal watermark callback register
* Input          : s32 layer : layer
                      void (*callback)(void) : callback
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_watermarkCallbackRegister(s32 layer,void (*callback)(void));
/*******************************************************************************
* Function Name  : hal_watermarkRam
* Description    : hal watermark ram get
* Input          : s32 layer : layer
                      u32 width    : width
                      u32 height   : height                      
* Output         : None
* Return         : None
*******************************************************************************/
void  *hal_watermarkRam(s32 layer,INT16U *width,INT16U *height);
void hal_watermarkBPos_Adjust(s32 y);



#endif