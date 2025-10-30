/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                  THE APPOTECH HAL
   **** **                 AUADC HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_auadc.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AUADC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  HAL_AUADC_H
    #define  HAL_AUADC_H



/*typedef union
{
	u8 pcm[HAL_CFG_PCM_BUFFER_NUM][HAL_CFG_PCM_BUFFER_SIZE];
	u8 uac[HAL_CFG_PCM_BUFFER_NUM][64]; 	
}_PCMBUF_;*/









s32 hal_audc_stamp_out(void);
s32 hal_audc_stamp_next(void);


/*******************************************************************************
* Function Name  : hal_auadcInit
* Description    : hal layer.auadc initial
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void hal_auadcInit(void);
/*******************************************************************************
* Function Name  : hal_auadcMemInit
* Description    : hal layer.adadc memory initial
* Input          : 
* Output         : None
* Return         : int
*******************************************************************************/
int hal_auadcMemInit(void);
int hal_auadcMemInitB(void);
/*******************************************************************************
* Function Name  : hal_auadcMemUninit
* Description    : hal layer.adadc memory uninitial
* Input          : 
* Output         : None
* Return         : int
*******************************************************************************/
int hal_auadcMemUninit(void);
int hal_auadcMemUninitB(void);
/*******************************************************************************
* Function Name  : hal_auadcStartForAVI
* Description    : hal layer.auadc start for avi using
* Input          : _PCM_FRQ_: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
int hal_auadcStartForAVI(_PCM_FRQ_ frq,int volume);
int hal_auadcStartForAVIB(_PCM_FRQ_ frq,int volume);
/*******************************************************************************
* Function Name  : hal_auadcStartForWAV
* Description    : hal layer.auadc start for wav using
* Input          : _PCM_FRQ_: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
int hal_auadcStartForWAV(_PCM_FRQ_ frq,int volume);
/*******************************************************************************
* Function Name  : hal_auadcStartForUAC
* Description    : hal layer.auadc start for UAC
* Input          : _PCM_FRQ_: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
int hal_auadcStartForUAC(_PCM_FRQ_ frq,int volume);
/*******************************************************************************
* Function Name  : hal_mjpegUVCProcess
* Description    : hal layer .mjpeg process for uvc in mjpeg mode
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_auadcUACProcess(void);
/*******************************************************************************
* Function Name  : hal_auadcStop
* Description    : hal layer.auadc stop
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void hal_auadcStop(void);
/*******************************************************************************
* Function Name  : hal_auadcBufferRelease
* Description    : hal layer.set pcm raw data addr & length
* Input          : void *buffer : buffer addr
* Output         : None
* Return         : none
*******************************************************************************/
void hal_auadcBufferRelease(void *buffer);
void hal_auadcBufferReleaseB(void *buffer);
/*******************************************************************************
* Function Name  : hal_auadcBufferGet
* Description    : hal layer.get pcm raw data addr & length
* Input          : u32 *addr : addr
                      u32 *len : length
* Output         : None
* Return         : int : 0: data ready
*******************************************************************************/
void *hal_auadcBufferGet(u32 *addr,u32 *len,s32 *syncnt,s32 *syncnt_next);
void *hal_auadcBufferGetB(u32 *addr,u32 *len,s32 *syncnt,s32 *syncnt_next);


bool hal_adcBuffer_prefull(void);
bool hal_adcBuffer_halffull(void);

void hal_adc_volume_set(int volume);
void hal_adc_volume_setB(int volume);
void hal_uac_send(void);
u32 hal_auadc_cnt(void);





















#endif

