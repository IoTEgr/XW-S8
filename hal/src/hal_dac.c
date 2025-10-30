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
* File Name   : hal_dac.c
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
#include "../inc/hal.h"
#define HAL_VOLUME_MAKE(x)          ((x*0x7fff)/100)


const static u32 halSampleRateTable[]=
{
	48000,44100,32000,0,24000,22050,16000,0,12000,11025,8000
};
static u8 halVolume;
static void (*halDacCallBack)(int flag);
/*******************************************************************************
* Function Name  : hal_dacCallback
* Description    : hal layer .dac callback
* Input          : none
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
static void hal_dacCallback(u8 flag)
{
	
	if(halDacCallBack)
		halDacCallBack(flag);
	else
	{
		if(flag == 4)
		   ax32xx_dacStop();
	}
}
/*******************************************************************************
* Function Name  : hal_dacInit
* Description    : hal layer .initial dac
* Input          : none
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
void hal_dacInit(void)
{
	ax32xx_dacInit();

	ax32xx_dacEnable(1);

	ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(0));

	halVolume = 0;

	halDacCallBack = NULL;

	ax32xx_dacISRRegister(hal_dacCallback);
}
/*******************************************************************************
* Function Name  : hal_dacPlayStart
* Description    : hal layer .start dac play
* Input          : u32 sampleRate : sample rate
                      u32 buffer : buffer
                      u32 size : size
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacPlayStart(u32 sampleRate,u32 buffer,u32 size)
{
	int i;

    if(sampleRate==0)
		return -1;
	for(i=0;i<11;i++)
	{
		if(sampleRate == halSampleRateTable[i])
			break;
	}
	if(i>=11)
	{
		return -1;
	}
//	ax32xx_dacEnable(1);
//	hal_dacInit();
	ax32xx_dacSampleRateSet(i);
//    deg_Printf("halVolume = %d,%x\n",halVolume,HAL_VOLUME_MAKE(halVolume));
	ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(halVolume));

//    ax32xx_dacReset();	

//	ax32xx_dacBufferSet(buffer,size);
   
	ax32xx_dacStart(buffer,size);

	return 0;
}
/*******************************************************************************
* Function Name  : hal_dacPlayStop
* Description    : hal layer .stop dac play
* Input          : none
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacPlayStop(void)
{
//    halVolume = 0;
	ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(0));
	ax32xx_dacStop();
 //   ax32xx_dacEnable(0);
    return 0;
}
/*******************************************************************************
* Function Name  : hal_dacSetVolume
* Description    : hal layer .set dac volume
* Input          : u8 volume : dac volume
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacSetVolume(u8 volume)
{
	if(halVolume==volume)
		return 0;
	if(volume>100)
		volume = 100;
	halVolume = volume;

	ax32xx_dacVolumeSet(HAL_VOLUME_MAKE(halVolume));

	return 0;
}
/*******************************************************************************
* Function Name  : hal_dacSetBuffer
* Description    : hal layer .set dac buffer
* Input          : u32 buffer : buffer addr
                      u32 size : size
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacSetBuffer(u32 buffer,u32 size)
{
	ax32xx_dacBufferSet(buffer,size);

	return 0;
}
/*******************************************************************************
* Function Name  : hal_dacCallBackRegister
* Description    : hal layer .dac callback register
* Input          : void (*callback)(int flag) : callback
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacCallBackRegister(void (*callback)(int flag))
{
	halDacCallBack = callback;

	return 0;
}
/*******************************************************************************
* Function Name  : hal_dacHPSet
* Description    : hal layer .dac hp vdd output
* Input          :  u8 en : enable.1->enable,0-disable
                       u32 level : vdd level.SEE HP_VDD_E
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_dacHPSet(u8 en,u32 level)
{
	ax32xx_dacHPSet(en,level);

	return 0;
}


