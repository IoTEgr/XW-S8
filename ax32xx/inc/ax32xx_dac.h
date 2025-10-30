/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         AUDAC DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_audac.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AUDIO DAC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_DAC_H
    #define AX32XX_DAC_H

#define  DAC_INT_PEND     1
#define  DAC_INT_HALF     2
#define  DAC_INT_EMPTY   4


#define  DAC_SAMPLE_RATE_8000       10
#define  DAC_SAMPLE_RATE_11025      9
#define  DAC_SAMPLE_RATE_12000      8
#define  DAC_SAMPLE_RATE_16000      6
#define  DAC_SAMPLE_RATE_22050      5
#define  DAC_SAMPLE_RATE_24000      4
#define  DAC_SAMPLE_RATE_32000      2
#define  DAC_SAMPLE_RATE_44100      1
#define  DAC_SAMPLE_RATE_48000      0


typedef enum
{
	HP_VDD_2_6V=0,
	HP_VDD_2_7V,
	HP_VDD_2_8V,
	HP_VDD_2_9V,
	HP_VDD_3_0V,
	HP_VDD_3_1V,
	HP_VDD_3_2V,
	HP_VDD_3_3V
}HP_VDD_E;

/*******************************************************************************
* Function Name  : ax32xx_dacIRQHandler
* Description    : dac irq handler
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_dacInit
* Description    : initial dac
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacInit(void);
/*******************************************************************************
* Function Name  : ax32xx_dacSampleRateSet
* Description    : set dac samlerate
* Input          :  u32 sample_rate : sample rate
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacSampleRateSet(u32 sample_rate);
/*******************************************************************************
* Function Name  : ax32xx_dacEnable
* Description    : enable dac
* Input          :  u8 en
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_dacVolumeSet
* Description    : set dac volume
* Input          :  u16 volume : volume
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacVolumeSet(u16 volume);
/*******************************************************************************
* Function Name  : ax32xx_dacBufferSet
* Description    : set dac play buffer
* Input          :  u32 obuf_addr : out buffer addr
                      u32 obuf_size  : out buffer size
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacBufferSet(u32 obuf_addr,u32 obuf_size);
/*******************************************************************************
* Function Name  : ax32xx_dacBufferFlush
* Description    : dac play reset
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacBufferFlush(u32 obuf_size);
/*******************************************************************************
* Function Name  : ax32xx_dacReset
* Description    : dac play reset
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacReset(void);
/*******************************************************************************
* Function Name  : ax32xx_dacStart
* Description    : dac play start
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacStart(u32 obuf_addr,u32 obuf_size);
/*******************************************************************************
* Function Name  : ax32xx_dacStop
* Description    : dac play stop
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacStop(void);
/*******************************************************************************
* Function Name  : ax32xx_dacISRRegister
* Description    : register irq service
* Input          :  void (*isr)(u8 flag)
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacISRRegister(void (*isr)(u8 flag));
/*******************************************************************************
* Function Name  : ax32xx_dacHPSet
* Description    : dac HP VDD output set
* Input          :  u8 en : enable.1->enable,0-disable
                       u32 level : vdd level
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacHPSet(u8 en,u32 level);


#endif
