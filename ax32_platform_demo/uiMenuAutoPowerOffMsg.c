#include "uiMenuAutoPowerOffRes.c"

static u8 sel_idx;
static int autoPowerOffKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			configSet(CONFIG_ID_AUTOOFF,auto_power_off_tab[sel_idx]);
		}
		else if(1==sel_idx)
		{
			configSet(CONFIG_ID_AUTOOFF,auto_power_off_tab[sel_idx]);
		}
		else if(2==sel_idx)
		{
			configSet(CONFIG_ID_AUTOOFF,auto_power_off_tab[sel_idx]);
		}
		else if(3==sel_idx)
		{
			configSet(CONFIG_ID_AUTOOFF,auto_power_off_tab[sel_idx]);
		}

		configSys(CONFIG_ID_AUTOOFF);

		winDestroy(&handle);
	}
	return 0;
}

static int autoPowerOffKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int autoPowerOffKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<=0)
		{
			sel_idx=AUTO_POWER_OFF_NUM-1;
		}
		else
		{
			sel_idx--;
		}
		settingAutoPowerOffSelShow(handle,sel_idx);
	}
	return 0;
}

static int autoPowerOffKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>=AUTO_POWER_OFF_NUM-1)
		{
			sel_idx=0;
		}
		else
		{
			sel_idx++;
		}
		settingAutoPowerOffSelShow(handle,sel_idx);
	}
	return 0;
}

static int autoPowerOffOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 cam_res;
	u8 i;
	cam_res=configGet(CONFIG_ID_AUTOOFF);
	for(i=0;i<AUTO_POWER_OFF_NUM;i++)
	{
		if(auto_power_off_tab[i]==cam_res)
		{
			break;
		}
	}
	sel_idx=i;

	deg_Printf("cam res:sel_idx=%d\n",sel_idx);
//	setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_AUTO_SHUTOFF);
	//==show osd==
	settingAutoPowerOffBaterryShow(handle);
	settingAutoPowerOffSelShow(handle,sel_idx);

	return 0;
}
static int autoPowerOffCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{


	return 0;
}
static int autoPowerOffWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int autoPowerOffSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingAutoPowerOffBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor autoPowerOffMsgDeal[]=
{
	{SYS_OPEN_WINDOW,autoPowerOffOpenWin},
	{SYS_CLOSE_WINDOW,autoPowerOffCloseWin},
	{SYS_CHILE_COLSE,autoPowerOffWinChildClose},
	{KEY_EVENT_OK,autoPowerOffKeyMsgOk},
	{KEY_EVENT_RETURN,autoPowerOffKeyMsgMenu},
	{KEY_EVENT_RIGHT,autoPowerOffKeyMsgUp},
	{KEY_EVENT_LEFT,autoPowerOffKeyMsgDown},
	{SYS_EVENT_1S,autoPowerOffSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(autoPowerOffWindow,autoPowerOffMsgDeal,autoPowerOffWin)


