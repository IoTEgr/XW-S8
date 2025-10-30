/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         ADC DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : adc_dc.c
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
#include "../inc/ax32xx.h"



#define  R_ADC_CTRL            ADCCON
#define  R_ADC_BAUDRATE      ADCBAUD
#define  R_ADC_DATA            ADCDAT


/*******************************************************************************
* Function Name  : ax32xx_adcEnable
* Description    : enable adc channel
* Input          : u8 ch : channel
                      u8 en : 0->disable,1-enable,
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_adcEnable(u8 ch,u8 en)
{
	en&=1;
	if(ch>ADC_CH_BGOP)
		return ;
	if(en)
	{
		ax32xx_sysClkSet(SYS_CLK_ADC,1);
		R_ADC_CTRL  = (ADC_EN)|(ch);

	}
	else
		R_ADC_CTRL  &= ~(ADC_EN);
	
}
/*******************************************************************************
* Function Name  : ax32xx_adcSetBaudrate
* Description    : adc set baudrate
* Input          : u32 baudrate : baudrate,must <2M HZ 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_adcSetBaudrate(u32 baudrate)
{
	R_ADC_BAUDRATE = (APB_CLK/(2*baudrate)- 1) & 0x7F;	
}
/*******************************************************************************
* Function Name  : ax32xx_adcConversionStart
* Description    : adc start once conversion
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_adcConverStart(void)
{
	R_ADC_CTRL |= ADC_GO;	 // start one change
}
/*******************************************************************************
* Function Name  : ax32xx_adcRead
* Description    : adc read value
* Input          : 
* Output         : None
* Return         : u16 value
*******************************************************************************/
u16 ax32xx_adcRead(void)
{
	volatile int i=0x8ffff;
	u16 value;
//	if(R_ADC_CTRL&(ADC_EN))
	 while(R_ADC_CTRL&ADC_EOC)
	 {
	 	i--;
		if(i==0)
		{

			break;
		}
	 }
	value = R_ADC_DATA&0x3ff;

	return value;
}
u16 ax32xx_adcRead2(void)
{
	volatile int i=0xffff;//0x8ffff;
	u16 value;
//	if(R_ADC_CTRL&(ADC_EN))
	if(!(R_ADC_CTRL&ADC_EOC))// while(R_ADC_CTRL&ADC_EOC)
	 {
		value =  R_ADC_DATA&0x3ff;
	 }
	else{
		value = 0;
	}
#if 0	
	 	i--;
		if(i==0)
		{

			//break;
		}
		value = 0;
	 }else
		value = R_ADC_DATA&0x3ff;
#endif
	return value;
}

