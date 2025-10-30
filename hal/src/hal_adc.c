/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         ADC HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_adc.h
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



/*******************************************************************************
* Function Name  : hal_adcInit
* Description    : timer initial
* Input          : none
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_adcInit(void)
{
	ax32xx_adcEnable(HAL_CFG_ADC_CH,1);
	ax32xx_adcSetBaudrate(HAL_CFG_ADC_BAUDRATE);

	ax32xx_adcConverStart();

	return 0;
}
/*******************************************************************************
* Function Name  : hal_adcRead
* Description    : read adc value
* Input          : none
* Output         : None
* Return         : s32 : value
*******************************************************************************/
s32 hal_adcRead(void)
{
	s32 value;

	value =ax32xx_adcRead();

	ax32xx_adcConverStart();  // start next once 
	return value;
}
/*******************************************************************************
* Function Name  : hal_adcSetChannel
* Description    : change adc channel
* Input          : u8 ch : channel
* Output         : None
* Return         : s32 : value,last channel value
*******************************************************************************/
s32 hal_adcSetChannel(u8 ch)
{
	s32 value;

	value =ax32xx_adcRead();
	ax32xx_adcEnable(ch,1);
	ax32xx_adcSetBaudrate(HAL_CFG_ADC_BAUDRATE);
	ax32xx_adcConverStart();

	return value;
}
/*******************************************************************************
* Function Name  : hal_adcGetChannel
* Description    : change adc channel & get value
* Input          : u8 ch : channel
* Output         : None
* Return         : s32 : value,last channel value
*******************************************************************************/
s32 hal_adcGetChannel(u8 ch)
{
	ax32xx_adcEnable(ch,1);
	ax32xx_adcSetBaudrate(HAL_CFG_ADC_BAUDRATE);
	ax32xx_adcConverStart();
	return ax32xx_adcRead();
}

s32 hal_adcGetChannel2(u8 ch,u32 ret)
{
	if(ret)
	{
		ax32xx_adcEnable(ch,1);
		ax32xx_adcSetBaudrate(HAL_CFG_ADC_BAUDRATE);
		ax32xx_adcConverStart();
	}
	return ax32xx_adcRead2();
}

/*******************************************************************************
* Function Name  : ax32xx_vddrtcCalculate
* Description    : Calculate vddrtc voltage
* Input          : u32 Artc : adc sample value of vddrtc
*                  u32 Abg  : adc sample value of bandgap
* Output         : none
* Return         : vddrtc voltage(unit : mV)
*******************************************************************************/
u32 hal_adcVDDRTCCalculate(u32 Artc,u32 Abg)
{
    return ax32xx_vddrtcCalculate(Artc,Abg);
}



