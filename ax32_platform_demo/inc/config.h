/****************************************************************************
**
 **                              CONFIGURE
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                      CONFIGURE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : config.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  CONFIG_H
#define  CONFIG_H


typedef enum
{
	CONFIG_ID_YEAR=0,
	CONFIG_ID_MONTH,
	CONFIG_ID_MDAY,
	CONFIG_ID_WDAY,
	CONFIG_ID_HOUR,
	CONFIG_ID_MIN,
	CONFIG_ID_SEC,
    CONFIG_ID_LANGUAGE,
    CONFIG_ID_AUTOOFF,    
    CONFIG_ID_SCREENSAVE,	// 10
    CONFIG_ID_FREQUNCY,
    CONFIG_ID_ROTATE,
    CONFIG_ID_FILLIGHT,
	CONFIG_ID_RESOLUTION,
	CONFIG_ID_TIMESTAMP,
	CONFIG_ID_MOTIONDECTION,
	CONFIG_ID_PARKMODE,
	CONFIG_ID_GSENSOR,
	CONFIG_ID_KEYSOUND,
	CONFIG_ID_IR_LED,		// 20
    CONFIG_ID_LOOPTIME,  
    CONFIG_ID_AUDIOREC,  
    CONFIG_ID_EV,
    CONFIG_ID_WBLANCE,
	CONFIG_ID_PRESLUTION,
	CONFIG_ID_PFASTVIEW,  
	CONFIG_ID_PTIMESTRAMP,
	CONFIG_ID_PEV,
    CONFIG_ID_VOLUME,
    CONFIG_ID_THUMBNAIL,	// 30
    CONFIG_ID_GSENSORMODE,
    CONFIG_ID_TIMEPHOTO,  	
    CONFIG_ID_MOREPHOTO,  	
    CONFIG_ID_PRINTER_EN,
    CONFIG_ID_PRINTER_DENSITY,
    CONFIG_ID_PRINTER_MODE,
    CONFIG_ID_PRINTER_NEARFAR,
    CONFIG_ID_PRINTER_DELAY,
    CONFIG_ID_BAT_OLD,
	CONFIG_ID_BAT_CHECK_FLAG,
	CONFIG_ID_DEVICE_ID1,
	CONFIG_ID_DEVICE_ID2,
	CONFIG_ID_DEVICE_ID3,
	CONFIG_ID_DEVICE_ID4,
	CONFIG_ID_DEVICE_ID5,
	CONFIG_ID_DEVICE_ID6,
	CONFIG_ID_PRINTER_DENSITY_H,
	CONFIG_ID_PRINTER_DENSITY_L,
	CONFIG_ID_PRINTER_MOTE_SPEED,
    CONFIG_ID_MAX
}CONFIG_ID_E;


#define _DEBG_CUT_DATA_RAW_  0x01
#define _DEBG_CUT_DATA_YUV_  0x02
#define _DEBG_CUT_BEFOR_CSI_  0x10
#define _DEBG_CUT_AFTER_CSI_  0x20

#define _DEBG_CUT_DATA_DIS_             0
#define _DEBG_CUT_CSI_RAW_              (_DEBG_CUT_BEFOR_CSI_|_DEBG_CUT_DATA_RAW_)
#define _DEBG_CUT_CSI_YUV_              (_DEBG_CUT_BEFOR_CSI_|_DEBG_CUT_DATA_YUV_)
#define _DEBG_CUT_POST_YUV_             (_DEBG_CUT_AFTER_CSI_|_DEBG_CUT_DATA_YUV_)
#define _DEBG_CUT_POST_YUV_INAVI_       (0x100|_DEBG_CUT_POST_YUV_)
#define _DEBG_CUT_POST_YUV_LCD_         (0x200|_DEBG_CUT_POST_YUV_)
#define _DEBG_CUT_SDR_DATA_              _DEBG_CUT_DATA_DIS_//_DEBG_CUT_DATA_DIS_//_DEBG_CUT_POST_YUV_LCD_ //control to cut source data or dma data
void sdram_data_capture_task(u8 *fname);
/*******************************************************************************
* Function Name  : userConfigSave
* Description    : save user configure value to spi flash
* Input          : none
* Output         : None
* Return         : s32_t 
*                    0 : always
*******************************************************************************/
extern s32_t userConfigSave(void);
/*******************************************************************************
* Function Name  : userConfigReset
* Description    : reset or default user configure value
* Input          : 0: somesetting not reset , 1: all reset
* Output         : None
* Return         : s32_t 
*                    0 : always
*******************************************************************************/
extern s32_t userConfigReset(u8 all);
/*******************************************************************************
* Function Name  : userConfigInitial
* Description    : initial user configure value
* Input          : none
* Output         : None
* Return         : s32_t 
*                      0 : always
*******************************************************************************/
extern s32_t userConfigInitial(void);
/*******************************************************************************
* Function Name  : configSet
* Description    : set configure value
* Input          : u8_t configId : configure id
*                  u32_t value   : configure value
* Output         : None
* Return         : none
*******************************************************************************/
extern void configSet(u8_t configId,u32_t value);

/*******************************************************************************
* Function Name  : userCofigGetValue
* Description    : get configure value in configure table
* Input          : u8_t configId : configure id
* Output         : None
* Return         : u32_t : configure value
*******************************************************************************/
extern u32_t configGet(u8_t configId);

int configLanguage(void);
void configSys(uint32 configId);
void configSystem(void);
INT32U configValue2Int(INT8U config_id);


#endif