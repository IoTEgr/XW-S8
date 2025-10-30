#include "uiMenuVideoResRes.c"

static u8 sel_idx;
static int videoResKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			configSet(CONFIG_ID_RESOLUTION,R_ID_STR_RES_FHD);
		}
		else
		{
			configSet(CONFIG_ID_RESOLUTION,R_ID_STR_RES_HD);
		}
		configSys(CONFIG_ID_RESOLUTION);

		winDestroy(&handle);
	}
	return 0;
}

static int videoResKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int videoResKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<=0)
		{
			sel_idx=1;
		}
		else
		{
			sel_idx=0;
		}
		settingVideoResSelShow(handle,sel_idx);
	}
	return 0;
}

static int videoResKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>=1)
		{
			sel_idx=0;
		}
		else
		{
			sel_idx=1;
		}
		settingVideoResSelShow(handle,sel_idx);
	}
	return 0;
}

static int videoResOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 res=configValue2Int(CONFIG_ID_RESOLUTION);
	if(((1920<<16)|1080)==res)
	{
		sel_idx=0;
	}
	else
	{
		sel_idx=1;
	}
	deg_Printf("res:sel_idx=%d\n",sel_idx);
	//setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_VIDEO_RES);
	//==show osd==
	settingVideoResBaterryShow(handle);
	settingVideoResSelShow(handle,sel_idx);

	return 0;
}
static int videoResCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int videoResWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int videoResSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingVideoResBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor videoResMsgDeal[]=
{
	{SYS_OPEN_WINDOW,videoResOpenWin},
	{SYS_CLOSE_WINDOW,videoResCloseWin},
	{SYS_CHILE_COLSE,videoResWinChildClose},
	{KEY_EVENT_OK,videoResKeyMsgOk},
	{KEY_EVENT_RETURN,videoResKeyMsgMenu},
	{KEY_EVENT_RIGHT,videoResKeyMsgUp},
	{KEY_EVENT_LEFT,videoResKeyMsgDown},
	{SYS_EVENT_1S,videoResSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(videoResWindow,videoResMsgDeal,videoResWin)


