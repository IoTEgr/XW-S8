#include "uiMenuVersionRes.c"


static int versionKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
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

static int versionKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int versionOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("version Open Win!!!\n");
	
	//setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_VERSION);
	//==show osd==
	settingVersionBaterryShow(handle);
	settingVersionStrShow(handle,true);
	return 0;
}
static int versionCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("version Close Win!!!\n");
	settingVersionStrShow(handle,false);
	return 0;
}
static int versionWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("version WinChild Close!!!\n");
	return 0;
}

static int versionSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingVersionBaterryShow(handle);	
	}

	return 0;
}


msgDealInfor verisonMsgDeal[]=
{
	{SYS_OPEN_WINDOW,versionOpenWin},
	{SYS_CLOSE_WINDOW,versionCloseWin},
	{SYS_CHILE_COLSE,versionWinChildClose},
	{KEY_EVENT_OK,versionKeyMsgOk},
	{KEY_EVENT_RETURN,versionKeyMsgMenu},
	{SYS_EVENT_1S,versionSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(versionWindow,verisonMsgDeal,versionWin)


