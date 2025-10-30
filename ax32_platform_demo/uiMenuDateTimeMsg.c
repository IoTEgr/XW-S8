#include "uiMenuDateTimeRes.c"

static u8 sel_idx;

static int dateTimeKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		HAL_RTC_T rtcTime;
		rtcTime.year = dateTimeData[0];
		rtcTime.month =dateTimeData[1];
		rtcTime.day = dateTimeData[2];
		rtcTime.hour = dateTimeData[3];
		rtcTime.min = dateTimeData[4];
		rtcTime.sec = dateTimeData[5];
		hal_rtcTimeSet(&rtcTime);

		winDestroy(&handle);
	}
	return 0;
}
static int dateTimeKeyMsgPre(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	uint32 item;
//	DATE_TIME_T rtcTime;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>0)
		{
			sel_idx--;
		}
		else
		{
			sel_idx=DATE_TIME_ITEM_MAX-1;
		}
		settingDateTimeSelShow(handle,sel_idx);
	}
	return 0;
}

static int dateTimeKeyMsgNext(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	uint32 item;
//	DATE_TIME_T rtcTime;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<(DATE_TIME_ITEM_MAX-1))
		{
			sel_idx++;
		}
		else
		{
			sel_idx=0;
		}
		settingDateTimeSelShow(handle,sel_idx);
	}
	return 0;
}

#if 0
static int dateTimeKeyMsgPrev(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	uint32 item;
//	DATE_TIME_T rtcTime;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx)
		{
			sel_idx--;
		}
		else
		{
			sel_idx=(DATE_TIME_ITEM_MAX-1);
		}
		settingDateTimeSelShow(handle,sel_idx);
	}
	return 0;
}
#endif


static int dateTimeKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		dateTimeIncreace(sel_idx);
		settingDateTimeSelShow(handle,sel_idx);
	}
	return 0;
}

static int dateTimeKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		dateTimeDecreace(sel_idx);
		settingDateTimeSelShow(handle,sel_idx);
	}
	return 0;
}

static int dateTimeKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int dateTimeOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	DATE_TIME_T *rtcTime;
	deg_Printf("dateTime Open Win!!!\n");
	rtcTime = hal_rtcTimeGet();
	dateTimeData[0]=rtcTime->year;
	dateTimeData[1]=rtcTime->month;
	dateTimeData[2]=rtcTime->day;
	dateTimeData[3]=rtcTime->hour;
	dateTimeData[4]=rtcTime->min;
	dateTimeData[5]=rtcTime->sec;

	sel_idx=0;

	//==show video==
	//setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_DATE_TIME);
	//==show osd==
	settingDateTimeBaterryShow(handle);	
	settingDateTimeSelShow(handle,sel_idx);
	
	return 0;
}
static int dateTimeCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("dateTime Close Win!!!\n");
	return 0;
}
static int dateTimeWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("dateTime WinChild Close!!!\n");
	return 0;
}
static int dateTimeSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingDateTimeBaterryShow(handle);	
	}
	return 0;

}



msgDealInfor dateTimeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,dateTimeOpenWin},
	{SYS_CLOSE_WINDOW,dateTimeCloseWin},
	{SYS_CHILE_COLSE,dateTimeWinChildClose},
	{KEY_EVENT_OK,dateTimeKeyMsgOk},
	{KEY_EVENT_DOWN,dateTimeKeyMsgDown},
	{KEY_EVENT_UP,dateTimeKeyMsgUp},
	{KEY_EVENT_LEFT,dateTimeKeyMsgPre},
	{KEY_EVENT_RIGHT,dateTimeKeyMsgNext},
	{KEY_EVENT_RETURN,dateTimeKeyMsgMenu},
	{SYS_EVENT_1S,dateTimeSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(dateTimeWindow,dateTimeMsgDeal,dateTimeWin)


