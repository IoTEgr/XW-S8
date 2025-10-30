/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         AUADC DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_auadc.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AUDIO ADC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_AUADC_H
    #define AX32XX_AUADC_H

typedef enum
{
	ADC_SAMPLEREATE_8000 = 8000,	
	ADC_SAMPLEREATE_16000 = 16000,	
	ADC_SAMPLEREATE_24000 = 24000,	
	ADC_SAMPLEREATE_32000 = 32000,	
}_PCM_FRQ_;
#define  ADC_SAMPLE_RATE_32000    2
#define  ADC_SAMPLE_RATE_24000    3
#define  ADC_SAMPLE_RATE_16000    5
#define  ADC_SAMPLE_RATE_8000     10

/*******************************************************************************
* Function Name  : ax32xx_auadcIRQHandler
* Description    : auadc irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_auadcHalfIRQRegister
* Description    : auadc hanlf irq regiser
* Input          :void (*isr)(void) : isr call back
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcHalfIRQRegister(void (*isr)(void));
/*******************************************************************************
* Function Name  : ax32xx_auadcHalfIRQRegister
* Description    : auadc end irq regiser
* Input          :void (*isr)(void) : isr call back
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcEndIRQRegister(void (*isr)(void));
/*******************************************************************************
* Function Name  : ax32xx_auadcEnable
* Description    : auadc enable
* Input          :u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_auadcBufferSet
* Description    : auadc set frame buffer
* Input          :u32 addr : addr
                     u32 len   : buffer length
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcBufferSet(u32 addr,u32 len);
/*******************************************************************************
* Function Name  : ax32xx_auadcInit
* Description    : auadc intial 
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcInit(void);
/*******************************************************************************
* Function Name  : ax32xx_auadcSetSampleSet
* Description    : auadc set sample set 
* Input          : int sample_rate : sample rate
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcSetSampleSet(int sample_rate);
/*******************************************************************************
* Function Name  : ax32xx_auadcAGCEnable
* Description    : auadc agc enable
* Input          :u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcAGCEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_auadcGainSet
* Description    : auadc agc gain set
* Input          :u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcGainSet(u8 again, u8 again_mute,u8 again_max, u8 again_min);


void agc_gain_mnt(void);









#endif
