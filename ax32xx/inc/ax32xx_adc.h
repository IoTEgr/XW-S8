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
* File Name   : adc_dc.h
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
#ifndef AX32XX_ADC_H
   #define AX32XX_ADC_H

#define  ADC_CH_PA4      0
#define  ADC_CH_PA6      1
#define  ADC_CH_PA10     2
#define  ADC_CH_PB1      3
#define  ADC_CH_PD0      4
#define  ADC_CH_PF8     5
#define  ADC_CH_PE0     6
#define  ADC_CH_VDDSD    7
#define  ADC_CH_VDDHP    8
#define  ADC_CH_MVOUT    9
#define  ADC_CH_VPTAT    10
#define  ADC_CH_BGOP     11


#define  ADC_EN       0x10
#define  ADC_TICK     0x20
#define  ADC_EOC      0x40
#define  ADC_GO       0x80




/*******************************************************************************
* Function Name  : ax32xx_adcEnable
* Description    : enable adc channel
* Input          : u8 ch : channel
                      u8 en : 0->disable,1-enable,
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_adcEnable(u8 ch,u8 en);
/*******************************************************************************
* Function Name  : ax32xx_adcSetBaudrate
* Description    : adc set baudrate
* Input          : u32 baudrate : baudrate,must <2M HZ 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_adcSetBaudrate(u32 baudrate);
/*******************************************************************************
* Function Name  : ax32xx_adcConversionStart
* Description    : adc start once conversion
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_adcConverStart(void);
/*******************************************************************************
* Function Name  : ax32xx_adcRead
* Description    : adc read value
* Input          : 
* Output         : None
* Return         : u16 value
*******************************************************************************/
u16 ax32xx_adcRead(void);
u16 ax32xx_adcRead2(void);


















#endif
