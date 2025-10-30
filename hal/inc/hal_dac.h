/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         DAC HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_dac.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#ifndef  HAL_DAC_H
    #define  HAL_DAC_H



	

/*******************************************************************************
* Function Name  : hal_dacInit
* Description	 : hal layer .initial dac
* Input 		 : none
* Output		 : None
* Return		 : s32 : 0 success
							 <0 fail 
*******************************************************************************/
void hal_dacInit(void);
/*******************************************************************************
* Function Name  : hal_dacPlayStart
* Description	 : hal layer .start dac play
* Input 		 : u32 sampleRate : sample rate
					  u32 buffer : buffer
					  u32 size : size
* Output		 : None
* Return		 : s32 : 0 success
							 <0 fail 
*******************************************************************************/
s32 hal_dacPlayStart(u32 sampleRate,u32 buffer,u32 size);
/*******************************************************************************
* Function Name  : hal_dacPlayStop
* Description    : hal layer .stop dac play
* Input          : none
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacPlayStop(void);
/*******************************************************************************
* Function Name  : hal_dacSetVolume
* Description    : hal layer .set dac volume
* Input          : u8 volume : dac volume
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacSetVolume(u8 volume);
/*******************************************************************************
* Function Name  : hal_dacSetBuffer
* Description    : hal layer .set dac buffer
* Input          : u32 buffer : buffer addr
                      u32 size : size
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacSetBuffer(u32 buffer,u32 size);
/*******************************************************************************
* Function Name  : hal_dacFlush
*Bref: void hal_dacFlush(u32 size)
* Description    : 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
#define hal_dacFlush  ax32xx_dacBufferFlush
/*******************************************************************************
* Function Name  : hal_dacCallBackRegister
* Description    : hal layer .dac callback register
* Input          : void (*callback)(int flag) : callback
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacCallBackRegister(void (*callback)(int flag));
/*******************************************************************************
* Function Name  : hal_dacHPSet
* Description    : hal layer .dac hp vdd output
* Input          :  u8 en : enable.1->enable,0-disable
                       u32 level : vdd level.SEE HP_VDD_E

* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacHPSet(u8 en,u32 level);




#endif


