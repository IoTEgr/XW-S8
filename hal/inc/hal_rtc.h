/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         RTC HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_rtc.h
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

#ifndef HAL_RTC_H
#define HAL_RTC_H


typedef struct HAL_RTC_S
{
	u16 year;
	u8 month;
	u8 day;
	u8 hour;
	u8 min;
	u8 sec;
	u32 totalsec;
} HAL_RTC_T,DATE_TIME_T;

#define HAL_DEFAULT_YEAR    2000   // 2017/01/01 00:00:00




/*******************************************************************************
* Function Name  : hal_rtcIRQRegister
* Description	 : hal layer irq register
* Input 		 : none
* Output		 : None
* Return		 : s32 : 
*******************************************************************************/
int hal_rtcCallBackRegister(void (*callback)(void));
/*******************************************************************************
* Function Name  : hal_rtcCallBackRelease
* Description	 : hal layer irq release
* Input 		 : none
* Output		 : None
* Return		 : s32 : 
*******************************************************************************/
void hal_rtcCallBackRelease(int fd);
/*******************************************************************************
* Function Name  : hal_rtcInit
* Description	 : hal layer initial rtc
* Input 		 : none
* Output		 : None
* Return		 : s32 : 
*******************************************************************************/
s32 hal_rtcInit(void);
/*******************************************************************************
* Function Name  : hal_rtcUninit
* Description	 : hal layer disable rtc flag
* Input 		 : none
* Output		 : None
* Return		 : int 0
*******************************************************************************/
s32 hal_rtcUninit(void);
/*******************************************************************************
* Function Name  : hal_rtcTimeGet
* Description	 : hal layer get rtc time
* Input 		 : none
* Output		 : None
* Return		 : HAL_RTC_T * : date time
*******************************************************************************/
HAL_RTC_T *hal_rtcTimeGet(void);
/*******************************************************************************
* Function Name  : hal_rtcSecondGet
* Description	 : hal layer get rtc second
* Input 		 : none
* Output		 : None
* Return		 : u32
*******************************************************************************/
u32 hal_rtcSecondGet(void);
/*******************************************************************************
* Function Name  : hal_rtcTimeSet
* Description	 : hal layer set rtc time
* Input 		 : HAL_RTC_T * : date time
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_rtcTimeSet(HAL_RTC_T *rtcTime);
/*******************************************************************************
* Function Name  : hal_rtcTimeGetExt
* Description	 : hal layer get rtc time
* Input 		 : HAL_RTC_T *rtcTime
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_rtcTimeGetExt(HAL_RTC_T *rtcTime);
/*******************************************************************************
* Function Name  : hal_rtcSecondSet
* Description	 : hal layer set rtc time
* Input 		 : u32 second : second
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_rtcSecondSet(u32 second);
/*******************************************************************************
* Function Name  : hal_rtcTime2String
* Description	 : hal layer change date time to string
* Input 		 : HAL_RTC_T * : date time
* Output		 : None
* Return		 : char *string
*******************************************************************************/
char *hal_rtcTime2String(HAL_RTC_T *rtcTime);
/*******************************************************************************
* Function Name  : hal_rtcTime2StringExt
* Description	 : change time date to string
* Input 		 : int year : year
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_rtcTime2StringExt(char *str,HAL_RTC_T *rtcTime);
/*******************************************************************************
* Function Name  : hal_rtcLeapYear
* Description	 : hal layer leap year check
* Input 		 : int year : year
* Output		 : None
* Return		 : u8 : 1->leapyear
*******************************************************************************/
u8 hal_rtcLeapYear(int year);
/*******************************************************************************
* Function Name  : hal_rtcTime
* Description	 : hal layer change second to date time
* Input 		 : u32 second : second from defaul year
* Output		 : HAL_RTC_T *rtcTime : date time
* Return		 : none
*******************************************************************************/
void hal_rtcTime(u32 second,HAL_RTC_T *rtcTime);

/*******************************************************************************
* Function Name  : hal_rtcValue
* Description	 : hal layer change date time to second
* Input 		 :  HAL_RTC_T *rtcTime : date time
* Output		 :
* Return		 : second,from defaul year
*******************************************************************************/
u32 hal_rtcValue(HAL_RTC_T *rtcTime);
/*******************************************************************************
* Function Name  : hal_rtcAlarmSet
* Description	 : hal layer .rtc alarm set
* Input 		 :      u32 alarmtime : alarm time
*                       u8 enable : 0->disable,1->enable
* Output		 :
* Return		 : 
*******************************************************************************/
void hal_rtcAlarmSet(u32 alarmtime,u8 en);
/*******************************************************************************
* Function Name  : hal_vddWKOEnable
* Description	 : hal layer .enable WKO output
* Input 		 :      HAL_RTC_T *rtcTime : rtc time
*                       u8 enable : 0->disable,1->enable
* Output		 :
* Return		 : 
*******************************************************************************/
void hal_rtcAlarmSetExt(HAL_RTC_T *rtcTime,u8 en);
/*******************************************************************************
* Function Name  : hal_rtcAlarmStatusGet
* Description	 : hal layer .get rtc alarm status
* Input 		 :  u8 clear : 1-clar status
* Output		 :
* Return		 : 1->alarm active
*******************************************************************************/
u8 hal_rtcAlarmStatusGet(u8 clear);
/*******************************************************************************
* Function Name  : hal_rtcAlarm_weakup_reset
* Description	 : rtc enable alam time
* Input 		 : second:     poweron after second
* Output		 : None
* Return		 : none
*******************************************************************************/
//#define  hal_rtcAlarm_weakup_reset rtc_alarm_weakup_reset

/*******************************************************************************
* Function Name  : hal_vddWKOEnable
* Description	 : hal layer .enable WKO output
* Input 		 :  u8 enable : 0->disable,1->enable
* Output		 :
* Return		 : 
*******************************************************************************/
void hal_vddWKOEnable(u8 enable);
/*******************************************************************************
* Function Name  : hal_vddGSENEnable
* Description	 : hal layer.enable vdd-gsen output
* Input 		 :  u8 enable : 0->disable,1->enable
* Output		 :
* Return		 : 
*******************************************************************************/
void hal_vddGSENEnable(u8 enable);
/*******************************************************************************
* Function Name  : hal_wki1InputEnable
* Bref  : void hal_wki1InputEnable(u8 en)
* Description	 : hal layer .WKI input enable 
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
#define  hal_wki1InputEnable    ax32xx_WKI1InputEnable
/*******************************************************************************
* Function Name  : hal_wki0InputEnable
* Bref  : void hal_wki0InputEnable(u8 en)
* Description	 : hal layer .WKI input enable 
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
#define  hal_wki0InputEnable    ax32xx_WKI0InputEnable
/*******************************************************************************
* Function Name  : hal_wki1Read
* Bref  : u8 hal_wki1Read(void)
* Description	 : hal layer .WKI 1 read
* Input 		 :      
* Output		 : None
* Return		 : 0-low,1-high
*******************************************************************************/
#define  hal_wki1Read              ax32xx_WKI1Read
/*******************************************************************************
* Function Name  : hal_wki0Read
* Bref  : u8 hal_wki0Read(void)
* Description	 : hal layer .WKI 0 read. 
* Input 		 :      
* Output		 : None
* Return		 : 0-low,1-high
*******************************************************************************/
#define  hal_wki0Read              ax32xx_WKI0Read
/*******************************************************************************
* Function Name  : hal_wkiWakeup
* Bref  : u8 hal_wkiWakeup(u8 en)
* Description	 : hal layer .WKI 1 wakeup enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
#define  hal_wkiWakeup            ax32xx_WKI1WakeupEnable
/*******************************************************************************
* Function Name  : hal_batDetectEnable
* Bref  : u8 hal_batDetectEnable(u8 en)
* Description	 : hal layer .battery detect enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
#define  hal_batDetectEnable   ax32xx_rtcBatDecEnable
/*******************************************************************************
* Function Name  : ax32xx_wkiCleanPending
* Bref  : void ax32xx_wkiCleanPending(void)
* Description	 : clean wakeup pending
* Input 		 : None
* Output		 : None
* Return		 : 
*******************************************************************************/
#define ax32xx_wkiCleanPending    ax32xx_WKI1CleanPending
#endif

