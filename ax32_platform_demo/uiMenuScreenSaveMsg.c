#include "uiMenuScreenSaveRes.c"

static u8 sel_idx;
static int screenSaveKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			configSet(CONFIG_ID_SCREENSAVE,screen_save_tab[sel_idx]);
		}
		else if(1==sel_idx)
		{
			configSet(CONFIG_ID_SCREENSAVE,screen_save_tab[sel_idx]);
		}
		else if(2==sel_idx)
		{
			configSet(CONFIG_ID_SCREENSAVE,screen_save_tab[sel_idx]);
		}
		else if(3==sel_idx)
		{
			configSet(CONFIG_ID_SCREENSAVE,screen_save_tab[sel_idx]);
		}

		configSys(CONFIG_ID_SCREENSAVE);

		winDestroy(&handle);
	}
	return 0;
}

static int screenSaveKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int screenSaveKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<=0)
		{
			sel_idx=SCREEN_SAVE_NUM-1;
		}
		else
		{
			sel_idx--;
		}
		settingScreenSaveSelShow(handle,sel_idx);
	}
	return 0;
}

static int screenSaveKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>=SCREEN_SAVE_NUM-1)
		{
			sel_idx=0;
		}
		else
		{
			sel_idx++;
		}
		settingScreenSaveSelShow(handle,sel_idx);
	}
	return 0;
}

static int screenSaveOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 cam_res;
	u8 i;
	cam_res=configGet(CONFIG_ID_SCREENSAVE);
	for(i=0;i<SCREEN_SAVE_NUM;i++)
	{
		if(screen_save_tab[i]==cam_res)
		{
			break;
		}
	}
	sel_idx=i;

	deg_Printf("cam res:sel_idx=%d\n",sel_idx);
//	setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_SCREEN_PROTECT);
	//==show osd==
	settingScreenSaveBaterryShow(handle);
	settingScreenSaveSelShow(handle,sel_idx);

	return 0;
}
static int screenSaveCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{


	return 0;
}
static int screenSaveWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int screenSaveSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingScreenSaveBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor screenSaveMsgDeal[]=
{
	{SYS_OPEN_WINDOW,screenSaveOpenWin},
	{SYS_CLOSE_WINDOW,screenSaveCloseWin},
	{SYS_CHILE_COLSE,screenSaveWinChildClose},
	{KEY_EVENT_OK,screenSaveKeyMsgOk},
	{KEY_EVENT_RETURN,screenSaveKeyMsgMenu},
	{KEY_EVENT_RIGHT,screenSaveKeyMsgUp},
	{KEY_EVENT_LEFT,screenSaveKeyMsgDown},
	{SYS_EVENT_1S,screenSaveSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(screenSaveWindow,screenSaveMsgDeal,screenSaveWin)


