#if 0
#include "uiMenuDateTime1Res.c"

static u8 dayspermon[12]={31,28,31,30,31,30,31,31,30,31,30,31};

static void MenuDateTimeIncreace()
{
	if(0 == dt_idx)
	{
		datetime.year++;
		if(datetime.year>2080)
		{
			datetime.year=2017;
		}
	}
	else if(1 == dt_idx)
	{
		datetime.month++;
		if(datetime.month>12)
		{
			datetime.month=1;
		}
	}
	else if(2 == dt_idx)
	{
		datetime.day++;
		if(datetime.day>dayspermon[datetime.month-1])
		{
			datetime.day=1;
		}
	}
	else if(3 == dt_idx)
	{
		datetime.hour++;
		if(datetime.hour>23)
		{
			datetime.hour=0;
		}
	}
	else if(4 == dt_idx)
	{
		datetime.min++;
		if(datetime.min>59)
		{
			datetime.min=0;
		}
	}
	else if(5 == dt_idx)
	{
		datetime.sec++;
		if(datetime.sec>59)
		{
			datetime.sec=0;
		}
	}
}
static void MenuDateTimeDecreace()
{
	if(0 == dt_idx)
	{
		datetime.year--;
		if(datetime.year<2017)
		{
			datetime.year=2050;
		}
	}
	else if(1 == dt_idx)
	{
		datetime.month--;
		if(datetime.month<1)
		{
			datetime.month=12;
		}
	}
	else if(2 == dt_idx)
	{
		datetime.day--;
		if(datetime.day<1)
		{
			datetime.day=dayspermon[datetime.month-1];
		}
	}
	else if(3 == dt_idx)
	{
		if(datetime.hour>0)
		{
			datetime.hour--;
		}
		else
		{
			datetime.hour=23;
		}
	}
	else if(4 == dt_idx)
	{
		if(datetime.min>0)
		{
			datetime.min--;
		}
		else
		{
			datetime.min=59;
		}
	}
	else if(5 == dt_idx)
	{
		if(datetime.sec>0)
		{
			datetime.sec--;
		}
		else
		{
			datetime.sec=59;
		}
	}
}

static int MenuDateTimeKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(dt_idx>=5)
		{
			dt_idx=0;
		}
		else
		{
			dt_idx++;
		}
		MenuDateTimeShow(handle,dt_idx);
	}
	return 0;
}

static int MenuDateTimeKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		MenuDateTimeIncreace();
		MenuDateTimeSettingTimeStr();
		hal_rtcTimeSet(&datetime);
		MenuDateTimeShow(handle,dt_idx);
	}
	return 0;
}

static int MenuDateTimeKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		MenuDateTimeDecreace();
		MenuDateTimeSettingTimeStr();
		hal_rtcTimeSet(&datetime);
		MenuDateTimeShow(handle,dt_idx);
	}
	return 0;
}

static int MenuDateTimeKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}

static int MenuDateTimeKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}


static int MenuDateTimeOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	dt_idx = 0;
	hal_rtcTimeGetExt(&datetime);

	deg_Printf("year=%d,min=%d\n",datetime.year,datetime.min);
	MenuDateTimeSettingTimeStr();
	MenuDateTimeShow(handle,dt_idx);
	return 0;
}
static int MenuDateTimeCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int MenuDateTimeWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int MenuDateTimeSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	MenuDateTimeShow(handle,dt_idx);
	return 0;
}

msgDealInfor menudatetimeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,MenuDateTimeOpenWin},
	{SYS_CLOSE_WINDOW,MenuDateTimeCloseWin},
	{SYS_CHILE_COLSE,MenuDateTimeWinChildClose},
	{KEY_EVENT_OK,MenuDateTimeKeyMsgOk},
	{KEY_EVENT_UP,MenuDateTimeKeyMsgUp},
	{KEY_EVENT_DOWN,MenuDateTimeKeyMsgDown},
	{KEY_EVENT_PLAYBACK,MenuDateTimeKeyMsgMenu},
//	{KEY_EVENT_MODE,MenuDateTimeKeyMsgMode},
	{SYS_EVENT_1S,MenuDateTimeSysMsg1S},
	{EVENT_MAX,NULL},
};

MULTIWIN(menudatetimeWindow,menudatetimeMsgDeal,menudatetimeWin)

#endif

