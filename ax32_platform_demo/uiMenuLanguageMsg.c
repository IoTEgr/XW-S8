#include "uiMenuLanguageRes.c"

static u8 sel_idx;
static int languageKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		configSet(CONFIG_ID_LANGUAGE,language_tab[sel_idx]);
		configSys(CONFIG_ID_LANGUAGE);
		
		winDestroy(&handle);
	}
	return 0;
}

static int languageKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int languageKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<=0)
		{
			sel_idx=LANGUAGE_NUM-1;
		}
		else
		{
			sel_idx--;
		}
		settingLanguageSelShow(handle,sel_idx);
	}
	return 0;
}

static int languageKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>=LANGUAGE_NUM-1)
		{
			sel_idx=0;
		}
		else
		{
			sel_idx++;
		}
		settingLanguageSelShow(handle,sel_idx);
	}
	return 0;
}

static int languageOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 lang=configGet(CONFIG_ID_LANGUAGE);
	for(sel_idx=0;sel_idx<LANGUAGE_NUM;sel_idx++)
	{
		if(lang==language_tab[sel_idx])
		{
			break;
		}
	}
	deg_Printf("language:sel_idx=%d\n",sel_idx);
	//setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_LANGUAGE);
	//==show osd==
	settingLanguageBaterryShow(handle);
	settingLanguageSelShow(handle,sel_idx);

	return 0;
}
static int languageCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int languageWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int languageSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingLanguageBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor languageMsgDeal[]=
{
	{SYS_OPEN_WINDOW,languageOpenWin},
	{SYS_CLOSE_WINDOW,languageCloseWin},
	{SYS_CHILE_COLSE,languageWinChildClose},
	{KEY_EVENT_OK,languageKeyMsgOk},
	{KEY_EVENT_RETURN,languageKeyMsgMenu},
	{KEY_EVENT_RIGHT,languageKeyMsgUp},
	{KEY_EVENT_LEFT,languageKeyMsgDown},
	{SYS_EVENT_1S,languageSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(languageWindow,languageMsgDeal,languageWin)


