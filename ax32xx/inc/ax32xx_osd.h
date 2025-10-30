/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         OSD DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_osd.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN OSD HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_OSD_H
    #define AX32XX_OSD_H


#define  LCD_ROTATE_0            0
#define  LCD_ROTATE_180          3
#define  LCD_ROTATE_90           4
#define  LCD_ROTATE_270          5
#define  LCD_ROTATE_MASK         7

#define  OSD_SOFTWARE_ROTATE     1


//-------alpha type

enum
{
	ALPHA_NORMAL=0,
	ALPHA_GLOBAL
};


//--------osd type
enum
{
	OSD0=0,
	OSD1,

	OSD_MAX
};
/*******************************************************************************
* Function Name  : ax32xx_deWait
* Description    : de wait
* Input          :  none
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deWait(void);
/*******************************************************************************
* Function Name  : ax32xx_deUpdate
* Description    : de update
* Input          :  none
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deUpdate(void);
/*******************************************************************************
* Function Name  : ax32xx_deIRQHandler
* Description    : initial de irq handler
* Input          :  none
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_deISRRegister
* Description    : register de isr 
* Input          :  void (*isr)(void) : call back
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deISRRegister(void (*isr)(void));
/*******************************************************************************
* Function Name  : ax32xx_deInit
* Description    : initial de for video/osd0/osd1 layers
* Input          :  u16 width : display width
                       u16 height: display height
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deInit(u16 width,u16 height);
/*******************************************************************************
* Function Name  : ax32xx_deSetBackground
* Description    : set video layer background color
* Input          :  u8 red : red value 
                       u8 green:green value 
                       u8 blue  : blue value 
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deSetBackground(u8 red,u8 green,u8 blue);
/*******************************************************************************
* Function Name  : ax32xx_deSetCSC
* Description    : set video layer csc effect
* Input          :  u16 brightness : brightness
                       u16 saturation:saturation
                       u16 contrast  : contrast
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deSetCSC(u16 brightness,u16 saturation,u16 contrast);
/*******************************************************************************
* Function Name  : ax32xx_videoSetDither
* Description    : set video layer color dither width
* Input          :  u8 red : red color width
                       u8 green:green color width
                       u8 blue  : blue color width
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_videoSetDither(u8 red,u8 green,u8 blue);
/*******************************************************************************
* Function Name  : ax32xx_videoSetScanMode
* Description    : set video layer scan mode.scan mode is rotate
* Input          :  u8 scan_mode : scan mode 
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_videoSetScanMode(u8 scan_mode);
/*******************************************************************************
* Function Name  : ax32xx_videoGetYAddr
* Description    : set video layer current y buffer addr
* Input          :  
* Output         : None
* Return         : buffer addr
*******************************************************************************/
u32 ax32xx_videoGetYAddr(void);
/*******************************************************************************
* Function Name  : ax32xx_videoGetUVAddr
* Description    : set video layer current uv buffer addr
* Input          :  
* Output         : None
* Return         : buffer addr
*******************************************************************************/
u32 ax32xx_videoGetUVAddr(void);
/*******************************************************************************
* Function Name  : ax32xx_videoSetAddr
* Description    : set video layer buffer addr
* Input          :  u32 y_addr : ystride
                       u32 uv_addr : uv_addr
                       u8 update  : 1-update now,0-update late
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetAddr(u32 y_addr,u32 uv_addr,u8 update);
/*******************************************************************************
* Function Name  : ax32xx_videoSetScale
* Description    : set video layer scale
* Input          :  u8 tap0 : tap0
                       u8 tap1 : tap1
                       u8 tap2 : tap2
                       u8 tap3 : tap3
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_videoSetScale(u8 tap3,u8 tap2,u8 tap1,u8 tap0);
/*******************************************************************************
* Function Name  : ax32xx_videoSetStride
* Description    : set video layer stride
* Input          :  u32 ystride : ystride
                       u32 uvstride : uvstride
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetStride(u32 ystride,u32 uvstride);
/*******************************************************************************
* Function Name  : ax32xx_videoSetSize
* Description    : set video layer size
* Input          :  u16 width : width
                       u16 height : height
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetSize(u16 width,u16 height);
/*******************************************************************************
* Function Name  : ax32xx_videoSetPosition
* Description    : set video position
* Input          :  s16 x : x
                       s16 y : y
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetPosition(s16 x,s16 y);
/*******************************************************************************
* Function Name  : ax32xx_videoCopyCfg
* Description    : set video copy function,src size must be 320x240
* Input          : s16 dst_w : dest width,range (320,640]
*                  s16 dst_h : dest height,range (320,640]
*                  bool enable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoCopyCfg(s16 dst_w,s16 dst_h,bool enable);
/*******************************************************************************
* Function Name  : ax32xx_videoEnable
* Description    : video enable
* Input          :  u8 en: 0-disable,1-enable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_videoGammaEnable
* Description    : enable de video gamma table
* Input          :  u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoGammaEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_videoGammaRed
* Description    : de video gamma table for blue color
* Input          :  u32 lut : table addr
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoGammaBlue(u32 lut);
/*******************************************************************************
* Function Name  : ax32xx_videoGammaRed
* Description    : de video gamma table for green color
* Input          :  u32 lut : table addr
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoGammaGreen(u32 lut);
/*******************************************************************************
* Function Name  : ax32xx_videoGammaRed
* Description    : de video gamma table for red color
* Input          :  u32 lut : table addr
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoGammaRed(u32 lut);
/*******************************************************************************
* Function Name  : ax32xx_deSetGAMA
* Description    : set GAMA table by index
* Input          :  u8 gamma_index :gamma table index
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_deSetGAMA(u32 gred,u32 ggreen,u32 gblue);
/*******************************************************************************
* Function Name  : ax32xx_deSet_DE_CCM
* Description    : set de_ccm
* Input          :  u32 *p_ccm:pointer of ccm matrix 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_deSet_DE_CCM(u32 *p_ccm);
/*******************************************************************************
* Function Name  : ax32xx_deSet_DE_SAJ
* Description    : set de_saj
* Input          :  u32 *p_saj:pointer of saj 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_deSet_DE_SAJ(u32 *p_saj);
/*******************************************************************************
* Function Name  : ax32xx_osdEnable
* Description    : osd enable
* Input          : osd : osd id,OSD0,OSD1
                      u8 en: 0-disable,1-enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdEnable(u8 osd,u8 en);
/*******************************************************************************
* Function Name  : ax32xx_osdSetSize
* Description    : osd set size
* Input          : osd : osd id,OSD0,OSD1
                      u16 width : width
                      u16 height: height
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdSetSize(u8 osd,u16 width,u16 height);
/*******************************************************************************
* Function Name  : ax32xx_osdSetPosition
* Description    : osd set position
* Input          : osd : osd id,OSD0,OSD1
                      u16 x : x
                      u16 y: y
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdSetPosition(u8 osd,s16 x,s16 y);
/*******************************************************************************
* Function Name  : ax32xx_osdSetPalette
* Description    : osd set palette
* Input          : osd : osd id,OSD0,OSD1
                      u32 addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdSetPalette(u8 osd,u32 addr);
/*******************************************************************************
* Function Name  : ax32xx_osdSetAddr
* Description    : osd set fb addr
* Input          : osd : osd id,OSD0,OSD1
                   u32 addr : addr
                   u32 data_len : 
* Output         : None
* Return         : 
*******************************************************************************/
s32 ax32xx_osdSetAddr(u8 osd,u32 addr,u32 data_len);
/*******************************************************************************
* Function Name  : ax32xx_osdSetAlpha
* Description    : osd set set alpha type
* Input          : osd : osd id,OSD0,OSD1
                      u8 type : ALPHA_NORMAL,ALPHA_GLOBAL
                      u8 value : gblobal alpha value
* Output         : None
* Return         : 
*******************************************************************************/
s32 ax32xx_osdSetAlpha(u8 osd,u8 type,u8 value);
/*******************************************************************************
* Function Name  : ax32xx_deGetUpdateStatus
* Description    : get de update status
* Input          :  none
* Output         : bit 1 status
* Return         : 
*******************************************************************************/
u8 ax32xx_deGetUpdateStatus(void);
/*******************************************************************************
* Function Name  : ax32xx_videoSetScaleLine
* Description    : set video layer scale specific line
* Input          : u16 line:line number
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_videoSetScaleLine(u16 line,u8 enable);
/*******************************************************************************
* Function Name  : ax32xx_osdSetUpscaler
* Description    : osd set set alpha type
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_osdSetUpscaler(u16 src_w,u16 src_h,u16 dst_w,u16 dst_h,u32 * args);
/*******************************************************************************
* Function Name  : ax32xx_osdEncodeOneCode
* Description    : create a compressed code
* Input          : u8 data : 
                   u32 cnt : need encode 
                   u32 *used : used cnt
* Output         : encoded code
* Return         : none
*******************************************************************************/
u32 ax32xx_osdEncodeOneCode(u8 data,u32 cnt,u32 *used);
/*******************************************************************************
* Function Name  : ax32xx_osdGetAddr
* Description    : get osd layer current buffer addr
* Input          : osd : osd id,OSD0,OSD1
* Output         : None
* Return         : buffer addr
*******************************************************************************/
u32 ax32xx_osdGetAddr(u8 osd);
#endif
