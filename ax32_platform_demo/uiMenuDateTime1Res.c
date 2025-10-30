#include"uiWin.h"
#if 0
enum
{
	DATETIME_RECT_ID=0,
	//==date==
	DATETIME_DATE_BK_ID,
	DATETIME_STR_YEAR_ID,
	DATETIME_STR_P1_ID,
	DATETIME_STR_MON_ID,
	DATETIME_STR_P2_ID,
	DATETIME_STR_DAY_ID,
	//==time==
	DATETIME_TIME_BK_ID,
	DATETIME_STR_HOUR_ID,
	DATETIME_STR_DOT1_ID,
	DATETIME_STR_MIN_ID,
	DATETIME_STR_DOT2_ID,
	DATETIME_STR_SEC_ID,

};

static u8 dt_idx;
static HAL_RTC_T datetime;
static u8 str_year[5];
static u8 str_mon[3];
static u8 str_day[3];
static u8 str_hour[3];
static u8 str_min[3];
static u8 str_sec[3];

static widgetCreateInfor menudatetimeWin[] =
{
	createFrameWin(Rx(40),Ry(50), Rw(240),Rh(140),R_COLOR_GRAY2,WIN_ABS_POS),
	//==date==
	createStringIcon(DATETIME_STR_YEAR_ID,Rx(10+32),Ry(40), Rw(64),Rh(32)," ",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	createStringIcon(DATETIME_STR_P1_ID,Rx(76+32),Ry(39), Rw(16),Rh(32),"/",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	createStringIcon(DATETIME_STR_MON_ID,Rx(78+32),Ry(40), Rw(64),Rh(32)," ",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	createStringIcon(DATETIME_STR_P2_ID,Rx(125+32),Ry(39), Rw(16),Rh(32),"/",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	createStringIcon(DATETIME_STR_DAY_ID,Rx(126+32),Ry(40), Rw(64),Rh(32)," ",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	//==time==
	createStringIcon(DATETIME_STR_HOUR_ID,Rx(10+32),Ry(40+48), Rw(64),Rh(32)," ",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	createStringIcon(DATETIME_STR_DOT1_ID,Rx(56+32),Ry(37+48), Rw(16),Rh(32),":",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	createStringIcon(DATETIME_STR_MIN_ID,Rx(60+32),Ry(40+48), Rw(64),Rh(32)," ",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	createStringIcon(DATETIME_STR_DOT2_ID,Rx(106+32),Ry(37+48), Rw(16),Rh(32),":",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),
	createStringIcon(DATETIME_STR_SEC_ID,Rx(106+32),Ry(40+48), Rw(64),Rh(32)," ",ALIGNMENT_CENTER,R_COLOR_WHITE,R_FONT_NUM2),

	widgetEnd(),
};

static void MenuDateTimeShow(winHandle handle,u8 index)
{
	winSetStrInfor(winItem(handle,DATETIME_STR_YEAR_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,DATETIME_STR_MON_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,DATETIME_STR_DAY_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,DATETIME_STR_HOUR_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,DATETIME_STR_MIN_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,DATETIME_STR_SEC_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_WHITE);

	if(0==index)
	{
		winSetStrInfor(winItem(handle,DATETIME_STR_YEAR_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_YELLOW);
	}
	else if(1==index)
	{
		winSetStrInfor(winItem(handle,DATETIME_STR_MON_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_YELLOW);
	}
	else if(2==index)
	{
		winSetStrInfor(winItem(handle,DATETIME_STR_DAY_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_YELLOW);
	}
	else if(3==index)
	{
		winSetStrInfor(winItem(handle,DATETIME_STR_HOUR_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_YELLOW);
	}
	else if(4==index)
	{
		winSetStrInfor(winItem(handle,DATETIME_STR_MIN_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_YELLOW);
	}
	else if(5==index)
	{
		winSetStrInfor(winItem(handle,DATETIME_STR_SEC_ID),R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_YELLOW);
	}

	winSetResid(winItem(handle,DATETIME_STR_YEAR_ID),strToResId(str_year));
	winSetResid(winItem(handle,DATETIME_STR_MON_ID),strToResId(str_mon));
	winSetResid(winItem(handle,DATETIME_STR_DAY_ID),strToResId(str_day));
	winSetResid(winItem(handle,DATETIME_STR_HOUR_ID),strToResId(str_hour));
	winSetResid(winItem(handle,DATETIME_STR_MIN_ID),strToResId(str_min));
	winSetResid(winItem(handle,DATETIME_STR_SEC_ID),strToResId(str_sec));

}


static void MenuDateTimeSettingTimeStr()
{
	str_year[0] = (datetime.year/1000)%10+'0';
	str_year[1] = (datetime.year/100)%10+'0';
	str_year[2] = (datetime.year/10)%10+'0';
	str_year[3] = datetime.year%10+'0';
	str_year[4] = '\0';

	str_mon[0] = (datetime.month/10)%10+'0';
	str_mon[1] = datetime.month%10+'0';
	str_mon[2] = '\0';
	
	str_day[0] = (datetime.day/10)%10+'0';
	str_day[1] = datetime.day%10+'0';
	str_day[2] = '\0';

	if(datetime.hour>12)
	{
		datetime.hour-=12;
	}
	else if(0==datetime.hour)
	{
		datetime.hour=12;
	}

	str_hour[0] = (datetime.hour/10)%10+'0';
	str_hour[1] = datetime.hour%10+'0';
	str_hour[2] = '\0';

	str_min[0] = (datetime.min/10)%10+'0';
	str_min[1] = datetime.min%10+'0';
	str_min[2] = '\0';

	str_sec[0] = (datetime.sec/10)%10+'0';
	str_sec[1] = datetime.sec%10+'0';
	str_sec[2] = '\0';
	
}
#endif


