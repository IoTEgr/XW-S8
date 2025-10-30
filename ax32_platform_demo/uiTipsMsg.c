#include "uitipsRes.c"

static uint32 tipResId;
static uint32 continueTime=0xffffffff;

#if 0
static int tipsKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		
	}
	return 0;
}
#endif
static int tipsKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int tipsKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int tipsKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
#if 0
static int tipsKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
#endif
static int tipsSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		if(tipResId==TIPS_SD_NOT_INSERT||
			tipResId==TIPS_SD_FULL||
			tipResId==TIPS_SD_ERROR)
			winDestroy(&handle);
	}
	return 0;
}
static int tipsSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		if(tipResId==TIPS_USENSOR_POWER_LOW||
			tipResId==TIPS_POWER_LOW||
			tipResId==TIPS_NO_POWER)
			winDestroy(&handle);
	}
	return 0;
}
static int tipsSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.battery == BATTERY_STAT_1)
	{
		tipResId=TIPS_POWER_LOW;
		winSetResid(winItem(handle,TIP_STRING_ID),tipResId);
	}
	else if(SysCtrl.battery == BATTERY_STAT_0)
	{
		tipResId=TIPS_NO_POWER;
		winSetResid(winItem(handle,TIP_STRING_ID),tipResId);
	}
	return 0;
}
static int tipsOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum<2)
		return 0;
	tipResId=parame[0];
	continueTime=parame[1];
	winSetResid(winItem(handle,TIP_STRING_ID),tipResId);
	deg_Printf("tips Open Win!!!\n");
	return 0;
}
static int tipsCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("tips Close Win!!!\n");
	return 0;
}
static int tipsWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("tips WinChild Close!!!\n");
	return 0;
}
static int tipsSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(continueTime)
		continueTime--;
	if(continueTime==0)
		winDestroy(&handle);
	return 0;
}

msgDealInfor tipsMsgDeal[]=
{
	{SYS_OPEN_WINDOW,tipsOpenWin},
	{SYS_CLOSE_WINDOW,tipsCloseWin},
	{SYS_CHILE_COLSE,tipsWinChildClose},
	{KEY_EVENT_OK,/*tipsKeyMsgOk*/tipsKeyMsgMenu},
	{KEY_EVENT_UP,tipsKeyMsgUp},
	{KEY_EVENT_DOWN,tipsKeyMsgDown},
//	{KEY_EVENT_PLAYBACK,tipsKeyMsgMenu},
//	{KEY_EVENT_MODE,tipsKeyMsgMode},
	{SYS_EVENT_SDC,tipsSysMsgSD},
	{SYS_EVENT_USB,tipsSysMsgUSB},
	{SYS_EVENT_BAT,tipsSysMsgBattery},
	{SYS_EVENT_1S,tipsSysMsg1S},
	{EVENT_MAX,NULL},
};

MULTIWIN(tipsWindow,tipsMsgDeal,tipsWin)



