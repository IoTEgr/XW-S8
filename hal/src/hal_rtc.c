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
* File Name   : hal_rtc.c
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

#define  HAL_RTC_FLAG       0x00435452  // RTC
#define  HAL_RTC_ALARM      0x41000000  // A
#define  HAL_RTC_ISRNUM     4
#define  HAL_RTCFLAG1_ADDR  4           // RTC RAM ADDR

static HAL_RTC_T   halRTCTime;
static char halTimeString[20],halTimeUpdate,halRTCAlarm;
static const u8 monDaysTable[12]={31,28,31,30,31,30,31,31,30,31,30,31};	

static void (*hal_rtcCallBack[HAL_RTC_ISRNUM])(void);

/*******************************************************************************
* Function Name  : hal_rtcSencodISR
* Description	 : hal layer second irq from rtc
* Input 		 : none
* Output		 : None
* Return		 : s32 : 
*******************************************************************************/
static void hal_rtcSencodISR(void)
{
	halRTCTime.totalsec++;
	halRTCTime.sec++;
	halTimeUpdate|=0x01;

	if(halRTCTime.sec>59)
	{
		halRTCTime.sec = 0;
		halRTCTime.min++;
		halTimeUpdate|=0x02;
		if(halRTCTime.min>59)
		{
			halRTCTime.min=0;
			halRTCTime.hour++;
			halTimeUpdate|=0x04;
			if(halRTCTime.hour>23)
			{
				halTimeUpdate|=0x08;
				halRTCTime.hour=0;
				halRTCTime.day++;
				u8 mon; 
				if(halRTCTime.month==2)
					mon = hal_rtcLeapYear(halRTCTime.year)+28;
				else
					mon = monDaysTable[halRTCTime.month-1];
				if(halRTCTime.day>mon)
				{
					halTimeUpdate|=0x10;
					halRTCTime.day = 1;
					halRTCTime.month++;
					if(halRTCTime.month>12)
					{
						halTimeUpdate|=0x20;
						halRTCTime.month=1;
						halRTCTime.year++;
					}
				}
			}
		}
	}
	int i;
    for(i=0;i<HAL_RTC_ISRNUM;i++)
    {
		if(hal_rtcCallBack[i])
			hal_rtcCallBack[i]();
    }
}
/*******************************************************************************
* Function Name  : hal_rtcAlarmISR
* Description	 : hal layer alarm irq from rtc
* Input 		 : none
* Output		 : None
* Return		 : s32 : 
*******************************************************************************/
static void hal_rtcAlarmISR(void)
{
	debg("HAL: <RTC>[INFO] alarm\n");
	halRTCAlarm = 1;
}
/*******************************************************************************
* Function Name  : hal_rtcIRQRegister
* Description	 : hal layer irq register
* Input 		 : none
* Output		 : None
* Return		 : s32 : 
*******************************************************************************/
int hal_rtcCallBackRegister(void (*callback)(void))
{
	int i;
	for(i=0;i<HAL_RTC_ISRNUM;i++)
	{
		if(hal_rtcCallBack[i]==NULL)
		{
			hal_rtcCallBack[i] = callback;
			return i;
		}
	}
	return -1;
}
/*******************************************************************************
* Function Name  : hal_rtcCallBackRelease
* Description	 : hal layer irq release
* Input 		 : none
* Output		 : None
* Return		 : s32 : 
*******************************************************************************/
void hal_rtcCallBackRelease(int fd)
{
	if((fd>=0) && (fd<HAL_RTC_ISRNUM))
		hal_rtcCallBack[fd] = NULL;
}
/*******************************************************************************
* Function Name  : hal_rtcInit
* Description	 : hal layer initial rtc
* Input 		 : none
* Output		 : None
* Return		 : s32 : 
*******************************************************************************/
s32 hal_rtcInit(void)
{
	u32 timec,rtcFlag,i;

	halTimeUpdate = 0xff;
	halRTCAlarm = 0;
    for(i=0;i<HAL_RTC_ISRNUM;i++)
    {
		hal_rtcCallBack[i]=NULL;
    }
	ax32xx_rtcInit();
	ax32xx_rtcRamRead(HAL_RTCFLAG1_ADDR,(u8 *)&rtcFlag,4);
	if((rtcFlag&0xff000000) == HAL_RTC_ALARM)
	{
		halRTCAlarm = 1;
		rtcFlag&=~0xff000000;
	}
	if(rtcFlag!=HAL_RTC_FLAG)
	{
		timec = 0;
		ax32xx_rtcSet(0);
		halRTCTime.totalsec = 0;
	}
    else
    {
	    timec = ax32xx_rtcGet();
		halRTCTime.totalsec = timec;
    }
	rtcFlag = HAL_RTC_FLAG;
	ax32xx_rtcRamWrite(HAL_RTCFLAG1_ADDR,(u8 *)&rtcFlag,4);

	hal_rtcTime(timec,&halRTCTime);

	ax32xx_rtcSencodEnable(1,hal_rtcSencodISR);

	return timec;
}
/*******************************************************************************
* Function Name  : hal_rtcUninit
* Description	 : hal layer disable rtc flag
* Input 		 : none
* Output		 : None
* Return		 : int 0
*******************************************************************************/
s32 hal_rtcUninit(void)
{
	u32 rtcFlag = 0;
	ax32xx_rtcRamWrite(HAL_RTCFLAG1_ADDR,(u8 *)&rtcFlag,4);

	return 0;
}
/*******************************************************************************
* Function Name  : hal_rtcTimeGet
* Description	 : hal layer get rtc time
* Input 		 : none
* Output		 : None
* Return		 : HAL_RTC_T * : date time
*******************************************************************************/
HAL_RTC_T *hal_rtcTimeGet(void)
{
	return &halRTCTime;
}
/*******************************************************************************
* Function Name  : hal_rtcTimeGetExt
* Description	 : hal layer get rtc time
* Input 		 : HAL_RTC_T *rtcTime
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_rtcTimeGetExt(HAL_RTC_T *rtcTime)
{
	if(rtcTime == NULL)
		return ;
	memcpy(rtcTime,&halRTCTime,sizeof(HAL_RTC_T));
}
/*******************************************************************************
* Function Name  : hal_rtcSecondGet
* Description	 : hal layer get rtc second
* Input 		 : none
* Output		 : None
* Return		 : u32
*******************************************************************************/
u32 hal_rtcSecondGet(void)
{
	return halRTCTime.totalsec;
//	return ax32xx_rtcGet();
}
/*******************************************************************************
* Function Name  : hal_rtcTimeSet
* Description	 : hal layer set rtc time
* Input 		 : HAL_RTC_T * : date time
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_rtcTimeSet(HAL_RTC_T *rtcTime)
{
	u32 second = hal_rtcValue(rtcTime);

    if(second==0)
      return ;
	halTimeUpdate=0xff;
	halRTCTime.year = rtcTime->year;
	halRTCTime.month= rtcTime->month;
	halRTCTime.day= rtcTime->day;
	halRTCTime.hour= rtcTime->hour;
	halRTCTime.min= rtcTime->min;
	halRTCTime.sec= rtcTime->sec;
	halRTCTime.totalsec = second;
	deg_Printf("rtc = %d\n",second);
	ax32xx_rtcSet(second);
}
/*******************************************************************************
* Function Name  : hal_rtcSecondSet
* Description	 : hal layer set rtc time
* Input 		 : u32 second : second
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_rtcSecondSet(u32 second)
{
	ax32xx_rtcSet(second);

	hal_rtcTime(second,&halRTCTime);
}
/*******************************************************************************
* Function Name  : hal_rtcTime2String
* Description	 : hal layer change date time to string
* Input 		 : HAL_RTC_T * : date time
* Output		 : None
* Return		 : char *string
*******************************************************************************/
char *hal_rtcTime2String(HAL_RTC_T *rtcTime)
{
#if 1	
	if(rtcTime==NULL)
		return NULL;

    if(halTimeUpdate&0x20)
    {
		halTimeString[0] = (rtcTime->year/1000)%10 +'0';
		halTimeString[1] = (rtcTime->year/100)%10 +'0';
		halTimeString[2] = (rtcTime->year/10)%10 +'0';
		halTimeString[3] = (rtcTime->year)%10 +'0';
	    halTimeString[4] = '/';
    }
	if(halTimeUpdate&0x10)
	{
		halTimeString[5] = (rtcTime->month/10)%10 +'0';
		halTimeString[6] = (rtcTime->month)%10 +'0';
	    halTimeString[7] = '/';
	}
	if(halTimeUpdate&0x08)
	{
		halTimeString[8] = (rtcTime->day/10)%10 +'0';
		halTimeString[9] = (rtcTime->day)%10 +'0';
		halTimeString[10] = ' ';
	}
	if(halTimeUpdate&0x04)
	{
		/*if(rtcTime->hour>12)
		{
			rtcTime->hour-=12;
		}
		else if(0==rtcTime->hour)
		{
			rtcTime->hour=12;
		}*/
		halTimeString[11] = (rtcTime->hour/10)%10 +'0';
		halTimeString[12] = (rtcTime->hour)%10 +'0';
		halTimeString[13] = ':';
	}
	if(halTimeUpdate&0x02)
	{
		halTimeString[14] = (rtcTime->min/10)%10 +'0';
		halTimeString[15] = (rtcTime->min)%10 +'0';
		halTimeString[16] = ':';
	}
//	if(halTimeUpdate&0x01)
	{
		halTimeString[17] = (rtcTime->sec/10)%10 +'0';
		halTimeString[18] = (rtcTime->sec)%10 +'0';
		halTimeString[19] = '\0';
	}
    halTimeUpdate = 0;
#endif	
	return halTimeString;
}
/*******************************************************************************
* Function Name  : hal_rtcTime2StringExt
* Description	 : change time date to string
* Input 		 : int year : year
* Output		 : None
* Return		 : 
*******************************************************************************/
void hal_rtcTime2StringExt(char *str,HAL_RTC_T *rtcTime)
{
	if(str == NULL || rtcTime == NULL)
		return ;
	memcpy(str,hal_rtcTime2String(rtcTime),20);
}
/*******************************************************************************
* Function Name  : hal_rtcLeapYear
* Description	 : hal layer leap year check
* Input 		 : int year : year
* Output		 : None
* Return		 : u8 : 1->leapyear
*******************************************************************************/
u8 hal_rtcLeapYear(int year)
{
	if(((year%4==0)&&(year%100!=0))||(year%400==0))
		return 1;
	else
		return 0;
}
/*******************************************************************************
* Function Name  : hal_rtcTime
* Description	 : hal layer change second to date time
* Input 		 : u32 second : second from defaul year
* Output		 : HAL_RTC_T *rtcTime : date time
* Return		 : none
*******************************************************************************/
void hal_rtcTime(u32 second,HAL_RTC_T *rtcTime)
{

	u32 temp = 365,leap;
	u32 days = second/(24*60*60);

	if(rtcTime==NULL)
		return ;
	
	second -= days * (24*60*60);
	
	rtcTime->hour = second /(60*60);	
	rtcTime->min = (second%3600)/60;
	rtcTime->sec = second%60;
    rtcTime->year = HAL_DEFAULT_YEAR;
	rtcTime->month = 1;
	while(1)//Calc the year
	{
		
		if(hal_rtcLeapYear(rtcTime->year))
		     temp = 366;
		else
			temp = 365;
		
		if (days < temp)
			break;	//less one year
		rtcTime->year++;
		days -= temp;
	}
    leap = hal_rtcLeapYear(rtcTime->year);
	while(1) //calc the month
	{
		if(rtcTime->month == 2)
			temp = monDaysTable[rtcTime->month-1]+leap;
		else
			temp = monDaysTable[rtcTime->month-1];
		if(days<temp)
		{
			break;
		}
		rtcTime->month++;
		days-=temp;
	}	

	rtcTime->day=days+1;
}

/*******************************************************************************
* Function Name  : hal_rtcValue
* Description	 : hal layer change date time to second
* Input 		 :  HAL_RTC_T *rtcTime : date time
* Output		 :
* Return		 : second,from defaul year
*******************************************************************************/
u32 hal_rtcValue(HAL_RTC_T *rtcTime)
{
	u32 second,days,temp;

	if(rtcTime==NULL)
		return 0;

    second = HAL_DEFAULT_YEAR;
	days = 0;
	temp = rtcTime->year;
	while(temp>HAL_DEFAULT_YEAR)
	{
		temp--;
		if(hal_rtcLeapYear(temp))
			days+=366;
		else
			days+=365;
	}
    second = hal_rtcLeapYear(rtcTime->year);
	for(temp=1;temp<rtcTime->month;temp++)
	{
		if(temp==2)
			days+=monDaysTable[temp-1]+second;
		else
			days+=monDaysTable[temp-1];
	}
	days+=rtcTime->day-1;

	second = days*24*60*60;
	second+=rtcTime->hour*60*60;
	second+=rtcTime->min*60;
	second+=rtcTime->sec;

	return second;
}
/*******************************************************************************
* Function Name  : hal_rtcAlarmSet
* Description	 : hal layer .rtc alarm set
* Input 		 :      u32 alarmtime : alarm time
*                       u8 enable : 0->disable,1->enable
* Output		 :
* Return		 : 
*******************************************************************************/
void hal_rtcAlarmSet(u32 alarmtime,u8 en)
{
	u32 rtcFlag;
	if(en)
	{
		rtcFlag = HAL_RTC_FLAG|HAL_RTC_ALARM;
	    ax32xx_rtcRamWrite(HAL_RTCFLAG1_ADDR,(u8 *)&rtcFlag,4);
	    ax32xx_rtcAlamSet(alarmtime);
	}
	else
	{
		rtcFlag = HAL_RTC_FLAG;
	    ax32xx_rtcRamWrite(HAL_RTCFLAG1_ADDR,(u8 *)&rtcFlag,4);
	}
	ax32xx_rtcAlamEnable(en,hal_rtcAlarmISR);
}
/*******************************************************************************
* Function Name  : hal_vddWKOEnable
* Description	 : hal layer .enable WKO output
* Input 		 :      HAL_RTC_T *rtcTime : rtc time
*                       u8 enable : 0->disable,1->enable
* Output		 :
* Return		 : 
*******************************************************************************/
void hal_rtcAlarmSetExt(HAL_RTC_T *rtcTime,u8 en)
{	
	if(rtcTime)
	{
		u32 rtcFlag;
		if(en)
		{
			rtcFlag = HAL_RTC_FLAG|HAL_RTC_ALARM;
		    ax32xx_rtcRamWrite(HAL_RTCFLAG1_ADDR,(u8 *)&rtcFlag,4);
			rtcFlag = hal_rtcValue(rtcTime);
			ax32xx_rtcAlamSet(rtcFlag);
		}
		else
		{
			rtcFlag = HAL_RTC_FLAG;
	        ax32xx_rtcRamWrite(HAL_RTCFLAG1_ADDR,(u8 *)&rtcFlag,4);
		}
		ax32xx_rtcAlamEnable(en,hal_rtcAlarmISR);
	}
}
/*******************************************************************************
* Function Name  : hal_rtcAlarmStatusGet
* Description	 : hal layer .get rtc alarm status
* Input 		 :  u8 clear : 1-clar status
* Output		 :
* Return		 : 1->alarm active
*******************************************************************************/
u8 hal_rtcAlarmStatusGet(u8 clear)
{
	u8 temp = halRTCAlarm;
	if(clear)
		halRTCAlarm = 0;
	return temp;
}
/*******************************************************************************
* Function Name  : hal_vddWKOEnable
* Description	 : hal layer .enable WKO output
* Input 		 :  u8 enable : 0->disable,1->enable
* Output		 :
* Return		 : 
*******************************************************************************/
void hal_vddWKOEnable(u8 enable)
{
	ax32xx_WKOEnable(enable);
}
/*******************************************************************************
* Function Name  : hal_vddGSENEnable
* Description	 : hal layer.enable vdd-gsen output
* Input 		 :  u8 enable : 0->disable,1->enable
* Output		 :
* Return		 : 
*******************************************************************************/
void hal_vddGSENEnable(u8 enable)
{
	ax32xx_VDDGSENEnable(enable);
}





