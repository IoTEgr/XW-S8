#include "uitips1Res.c"

static uint32 tip1ResId;
static uint32 continueTime1=0xffffffff;

static int tips1KeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int tips1KeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int tips1KeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int tips1KeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
		XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_MENU,KEY_PRESSED));
	}
	return 0;
}
static int tips1KeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int tips1SysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		if(tip1ResId==TIPS_SD_NOT_INSERT||
			tip1ResId==TIPS_SD_FULL||
			tip1ResId==TIPS_SD_ERROR)
			winDestroy(&handle);
	}
	return 0;
}
static int tips1SysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		if(tip1ResId==TIPS_USENSOR_POWER_LOW||
			tip1ResId==TIPS_POWER_LOW||
			tip1ResId==TIPS_NO_POWER)
			winDestroy(&handle);
	}
	return 0;
}
static int tips1SysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.battery == BATTERY_STAT_1)
	{
		tip1ResId=TIPS_POWER_LOW;
		winSetResid(winItem(handle,TIP1_STRING_ID),tip1ResId);
	}
	else if(SysCtrl.battery == BATTERY_STAT_0)
	{
		tip1ResId=TIPS_NO_POWER;
		winSetResid(winItem(handle,TIP1_STRING_ID),tip1ResId);
	}
	return 0;
}
static int tips1OpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum<2)
		return 0;
	tip1ResId=parame[0];
	continueTime1=parame[1];
	winSetResid(winItem(handle,TIP1_STRING_ID),tip1ResId);
	deg_Printf("tips1 Open Win!!!\n");
	return 0;
}
static int tips1CloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("tips1 Close Win!!!\n");
	return 0;
}
static int tips1WinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("tips1 WinChild Close!!!\n");
	return 0;
}
static int tips1SysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	uiParentDealMsg(handle,SYS_EVENT_1S);
	if(continueTime1)
		continueTime1--;
	if(continueTime1==0)
		winDestroy(&handle);
	return 0;
}

msgDealInfor tips1MsgDeal[]=
{
	{SYS_OPEN_WINDOW,tips1OpenWin},
	{SYS_CLOSE_WINDOW,tips1CloseWin},
	{SYS_CHILE_COLSE,tips1WinChildClose},
	{KEY_EVENT_PRINTER_EN,tips1KeyMsgOk},
	{KEY_EVENT_UP,tips1KeyMsgUp},
	{KEY_EVENT_DOWN,tips1KeyMsgDown},
	{KEY_EVENT_MENU,tips1KeyMsgMenu},
	{KEY_EVENT_MODE,tips1KeyMsgMode},
	{SYS_EVENT_SDC,tips1SysMsgSD},
	{SYS_EVENT_USB,tips1SysMsgUSB},
	{SYS_EVENT_BAT,tips1SysMsgBattery},
	{SYS_EVENT_1S,tips1SysMsg1S},
	{EVENT_MAX,NULL},
};

MULTIWIN(tips1Window,tips1MsgDeal,tips1Win)



