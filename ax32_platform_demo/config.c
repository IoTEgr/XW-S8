/****************************************************************************
**
 **                              CONFIGURE
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  CONFIGURE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : config.c
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
#include "application.h"
#include "version.h"

typedef struct
{
	INT32U flag[127];	// must  CFG_ID_MAX < 127 
	INT32U CheckSum;
}SYSTEM_FLAY;		// size must 256 aligned 
static SYSTEM_FLAY SysFlay __attribute__ ((aligned(32)));
static INT32U configAddress=0;

static void rtcTimeReset()
{
    DATE_TIME_T *rtcTime = hal_rtcTimeGet();	
	#if DATETIME_LOAD_AUTO >0	
        rtcTime->year = configGet(CONFIG_ID_YEAR);
		rtcTime->month = configGet(CONFIG_ID_MONTH);
		rtcTime->day = configGet(CONFIG_ID_MDAY);
		rtcTime->hour = configGet(CONFIG_ID_HOUR);
		rtcTime->min = configGet(CONFIG_ID_MIN);
		rtcTime->sec = configGet(CONFIG_ID_SEC);
    #else
		rtcTime->year = atoi(VERSION_YEAR);
		rtcTime->month = atoi(VERSION_MONTH);
		rtcTime->day = atoi(VERSION_DAY);
		rtcTime->hour = atoi(VERSION_HOUR);
		rtcTime->min = atoi(VERSION_MIN);
		rtcTime->sec = atoi(VERSION_SEC);
    #endif		
		hal_rtcTimeSet(rtcTime); // default time from VERSION_TIME
}


/*******************************************************************************
* Function Name  : userConfigReset
* Description    : reset or default user configure value
* Input          : 0: somesetting not reset , 1: all reset
* Output         : None
* Return         : s32_t 
*                    0 : always
*******************************************************************************/
s32_t userConfigReset(u8 all)
{
    deg_Printf("config : reset\n");
	if(all)
	{
		configSet(CONFIG_ID_YEAR,			  atoi(VERSION_YEAR));
		configSet(CONFIG_ID_MONTH,			  atoi(VERSION_MONTH));
		configSet(CONFIG_ID_MDAY,			  atoi(VERSION_DAY));
		configSet(CONFIG_ID_WDAY,			  0);
		configSet(CONFIG_ID_HOUR,			  atoi(VERSION_HOUR));
		configSet(CONFIG_ID_MIN,			  atoi(VERSION_MIN));
		configSet(CONFIG_ID_SEC,			  atoi(VERSION_SEC));
		rtcTimeReset();
	}

	configSet(CONFIG_ID_PRINTER_EN, 	 R_ID_STR_COM_OFF);	// 0 : close  , 1: open
	configSet(CONFIG_ID_PRINTER_DENSITY, R_ID_STR_COM_LEVEL_4);	// 0~4 
	configSet(CONFIG_ID_PRINTER_MODE, R_ID_STR_SET_PRINT_GRAY);		// 0: DOT_MATRIX,  1: GRAY_LEVEL
	configSet(CONFIG_ID_PRINTER_NEARFAR, 	  R_ID_STR_TIP_NEAR);
	configSet(CONFIG_ID_VOLUME, 		 10);

	#if defined(USE_DEFAULT_ENGLISH)
	configSet(CONFIG_ID_LANGUAGE,         R_ID_STR_LAN_ENGLISH);
	#else
	configSet(CONFIG_ID_LANGUAGE,         R_ID_STR_LAN_RUSSIAN/*R_ID_STR_LAN_SPANISH*/);
	#endif
	configSet(CONFIG_ID_AUTOOFF,          R_ID_STR_TIM_5MIN);
	configSet(CONFIG_ID_SCREENSAVE,       R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_FREQUNCY,         R_ID_STR_COM_50HZ);
	configSet(CONFIG_ID_ROTATE,           R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_FILLIGHT,         R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_RESOLUTION,       R_ID_STR_RES_FHD);
	configSet(CONFIG_ID_TIMESTAMP,        R_ID_STR_COM_ON);
	//configSet(CONFIG_ID_MOTIONDECTION,    R_ID_STR_COM_OFF);
	//configSet(CONFIG_ID_PARKMODE,         R_ID_STR_COM_OFF);
	//configSet(CONFIG_ID_GSENSOR,          R_ID_STR_COM_MIDDLE);
	configSet(CONFIG_ID_KEYSOUND,         R_ID_STR_COM_ON);
	configSet(CONFIG_ID_IR_LED,           R_ID_STR_COM_ON);
	configSet(CONFIG_ID_LOOPTIME,         R_ID_STR_TIM_3MIN/*R_ID_STR_TIM_5MIN*/);
	configSet(CONFIG_ID_AUDIOREC,         R_ID_STR_COM_ON);
	configSet(CONFIG_ID_EV,               R_ID_STR_COM_P0_0);
	configSet(CONFIG_ID_WBLANCE,          R_ID_STR_ISP_AUTO);
	configSet(CONFIG_ID_PRESLUTION,      R_ID_STR_RES_48M);
	configSet(CONFIG_ID_PFASTVIEW,       R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_PTIMESTRAMP,     R_ID_STR_COM_ON);
	configSet(CONFIG_ID_PEV,             R_ID_STR_COM_P0_0);
	configSet(CONFIG_ID_THUMBNAIL,       R_ID_STR_COM_ON);
	//configSet(CONFIG_ID_GSENSORMODE,     R_ID_STR_COM_ON);
	configSet(CONFIG_ID_TIMEPHOTO,	 	 R_ID_STR_COM_OFF);
	configSet(CONFIG_ID_MOREPHOTO,	 	 R_ID_STR_COM_OFF);

	configSet(CONFIG_ID_BAT_OLD,		R_ID_STR_COM_LEVEL_5);	//
	#ifdef	BAT_UPDATE_RTIME_EN
	configSet(CONFIG_ID_BAT_CHECK_FLAG, R_ID_STR_COM_ON);		//bat update flag. 1:updata enable . 0:disable 
	#else
	configSet(CONFIG_ID_BAT_CHECK_FLAG, R_ID_STR_COM_OFF);		//bat update flag. 1:updata enable . 0:disable 
	#endif
	configSet(CONFIG_ID_DEVICE_ID1,0);
	configSet(CONFIG_ID_PRINTER_DENSITY_L,75);	//记录来自app设置的默认浓度_L，保存到flash
	configSet(CONFIG_ID_PRINTER_DENSITY_H,00);	//							_H
	configSet(CONFIG_ID_PRINTER_MOTE_SPEED,23);	//记录来自app设置的马达速度，保存到flash
	userConfigSave();

	return 0;
}

#if NVFS_STROAGE == NV_STROAGE_SDC 
static void configReadFromSD(INT32U buff,INT32U len)
{
	int fd;
//    UINT size;
    fd = open((const char *)"user.cfg",FA_READ);
	if(fd<0)
	    fd = open((const char *)"user.cfg",FA_READ|FA_CREATE_NEW);
	read((FHANDLE)fd,(void *)buff,len);
    close((FHANDLE)fd);
}
static void configWriteToSD(INT32U buff,INT32U len)
{
	int fd;
//    UINT size;
    fd = open((const char *)"user.cfg",FA_READ|FA_WRITE);
	lseek((FHANDLE)fd,0,0);
	write((FHANDLE)fd,(void *)buff,len);	
	close((FHANDLE)fd);
}
#elif NVFS_STROAGE == NV_STROAGE_SPI || NVFS_STROAGE == NV_STROAGE_SDRAM
static void configReadFromSPI(INT32U buff,INT32U len)
{
    INT32U i=0,readPages=len>>8;
	for(i=0;i<readPages;i++)
	{
		hal_spiFlashRead(configAddress+256*i,(INT32U)(buff+256*i),256);
	}
}
static void configWriteToSPI(INT32U buff,INT32U len)
{
	 INT32U i=0,writePages=len>>8;
	 hal_spiFlashEraseSector(configAddress);
     for(i=0;i<writePages;i++)
     {
	     hal_spiFlashWrite(configAddress+256*i,(INT32U)(buff+256*i),256);
     }
}
#endif
static void config_read(INT32U buff,INT32U len)
{
#if NVFS_STROAGE == NV_STROAGE_SDC 
	configReadFromSD(buff,len);
#elif NVFS_STROAGE == NV_STROAGE_SPI
	configReadFromSPI(buff,len);
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	configReadFromSPI(buff,len);
#endif
}
static void config_write(INT32U buff,INT32U len)
{
#if NVFS_STROAGE == NV_STROAGE_SDC 
	configWriteToSD(buff,len);
#elif NVFS_STROAGE == NV_STROAGE_SPI
	configWriteToSPI(buff,len);
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	configWriteToSPI(buff,len);
#endif
}

INT32U configCheckSumCal(void)
{
	INT32U CheckSum=0,i;
	for(i=0;i<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]);i++)
	{
		CheckSum+=SysFlay.flag[i];
	}
	return CheckSum;
}
/*******************************************************************************
* Function Name  : userConfigSave
* Description    : save user configure value to spi flash
* Input          : none
* Output         : None
* Return         : s32_t 
*                    0 : always
*******************************************************************************/
s32_t userConfigSave(void)
{
  	SysFlay.CheckSum=configCheckSumCal();
	deg_Printf("write config check sum:0x%x\n",SysFlay.CheckSum);
	config_write((INT32U)(&SysFlay),sizeof(SYSTEM_FLAY));
    return 0;
}

/*******************************************************************************
* Function Name  : userConfigInitial
* Description    : initial user configure value
* Input          : none
* Output         : None
* Return         : s32_t 
*                      0 : always
*******************************************************************************/
s32_t userConfigInitial(void)
{
    INT32U CheckSum=0;
	configAddress=nv_configAddr();
	config_read((INT32U)(&SysFlay),sizeof(SYSTEM_FLAY));
	CheckSum=configCheckSumCal();
	deg_Printf("read config check sum:0x%x,0x%x\n",CheckSum,SysFlay.CheckSum);
	if((CheckSum!=SysFlay.CheckSum)||(0 == CheckSum))
	{
		deg_Printf("config error,reset flag.0x%x\n",SysFlay.CheckSum);
		memset(&SysFlay,0,sizeof(SYSTEM_FLAY));
		userConfigReset(1);
	}
	/*
	int i;
	for(i=0;i < 32;i++)
	{
		deg_Printf("SysFlay.flag[%d]=0x%x\n",i,SysFlay.flag[i]);
	}
	*/
    return 0;
}
/*******************************************************************************
* Function Name  : configSet
* Description    : set configure value
* Input          : u8_t configId : configure id
*                  u32_t value   : configure value
* Output         : None
* Return         : none
*******************************************************************************/
void configSet(u8_t configId,u32_t value)
{
	if(configId<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]))
    	SysFlay.flag[configId]=value;
}

/*******************************************************************************
* Function Name  : userCofigGetValue
* Description    : get configure value in configure table
* Input          : u8_t configId : configure id
* Output         : None
* Return         : u32_t : configure value
*******************************************************************************/
u32_t configGet(u8_t configId)
{
    if(configId<sizeof(SysFlay.flag)/sizeof(SysFlay.flag[0]))
		return SysFlay.flag[configId];
    return 0;
}


