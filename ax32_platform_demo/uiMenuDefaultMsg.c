#include "uiMenuDefaultRes.c"
#include "../hal/inc/hal_uart.h" 

static u8 sel_idx;

static int defaultKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			userConfigReset(0);
			configSystem();	

			if(configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
			{
				Bt_Control_Led_OnOff(1);
			}
			else
			{
				Bt_Control_Led_OnOff(0);
			}
		}
		winDestroy(&handle);
	}
	return 0;
}
static int defaultKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			sel_idx=1;
		}
		else
		{
			sel_idx=0;
		}
		settingDefaultSelShow(handle,sel_idx);

	}
	return 0;
}
static int defaultKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		if(0==sel_idx)
		{
			sel_idx=1;
		}
		else
		{
			sel_idx=0;
		}
		settingDefaultSelShow(handle,sel_idx);

	}
	return 0;
}

static int defaultKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int defaultOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("default Open Win!!!\n");
	sel_idx=0;
	//setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_DEFAULT);

	//==osd==
	settingDefaultBaterryShow(handle);
	settingDefaultSelShow(handle,sel_idx);

	return 0;

}
static int defaultCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("default Close Win!!!\n");
	return 0;
}
static int defaultWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int defaultSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingDefaultBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor defaultMsgDeal[]=
{
	{SYS_OPEN_WINDOW,defaultOpenWin},
	{SYS_CLOSE_WINDOW,defaultCloseWin},
	{SYS_CHILE_COLSE,defaultWinChildClose},
	{KEY_EVENT_OK,defaultKeyMsgOk},
	{KEY_EVENT_UP,defaultKeyMsgUp},
	{KEY_EVENT_DOWN,defaultKeyMsgDown},
	{KEY_EVENT_RETURN,defaultKeyMsgMenu},
	{SYS_EVENT_1S,defaultSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(defaultWindow,defaultMsgDeal,defaultWin)


