#include "uiMenuFrequencyRes.c"

static u8 sel_idx;
static int frequencyKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			configSet(CONFIG_ID_FREQUNCY,R_ID_STR_COM_50HZ);
		}
		else
		{
			configSet(CONFIG_ID_FREQUNCY,R_ID_STR_COM_60HZ);
		}
		configSys(CONFIG_ID_FREQUNCY);

		winDestroy(&handle);
	}
	return 0;
}

static int frequencyKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int frequencyKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
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
		settingFrequencySelShow(handle,sel_idx);
	}
	return 0;
}

static int frequencyKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
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
		settingFrequencySelShow(handle,sel_idx);
	}
	return 0;
}

static int frequencyOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 res=configGet(CONFIG_ID_FREQUNCY);
	if(R_ID_STR_COM_50HZ==res)
	{
		sel_idx=0;
	}
	else
	{
		sel_idx=1;
	}
	deg_Printf("res:sel_idx=%d\n",sel_idx);
//	setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_FREQ);
	//==show osd==
	settingFrequencyBaterryShow(handle);
	settingFrequencySelShow(handle,sel_idx);

	return 0;
}
static int frequencyCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int frequencyWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int frequencySysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingFrequencyBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor frequencyMsgDeal[]=
{
	{SYS_OPEN_WINDOW,frequencyOpenWin},
	{SYS_CLOSE_WINDOW,frequencyCloseWin},
	{SYS_CHILE_COLSE,frequencyWinChildClose},
	{KEY_EVENT_OK,frequencyKeyMsgOk},
	{KEY_EVENT_RETURN,frequencyKeyMsgMenu},
	{KEY_EVENT_RIGHT,frequencyKeyMsgUp},
	{KEY_EVENT_LEFT,frequencyKeyMsgDown},
	{SYS_EVENT_1S,frequencySysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(frequencyWindow,frequencyMsgDeal,frequencyWin)


