#include "uiMenuPrintModeRes.c"

static u8 sel_idx;
static int printModeKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		configSet(CONFIG_ID_PRINTER_MODE,sel_idx);
		SysCtrl.printer_print_mode=sel_idx;
		deg_Printf("SysCtrl.printer_print_mode=%d\n",SysCtrl.printer_print_mode);
		winDestroy(&handle);
	}
	return 0;
}

static int printModeKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int printModeKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
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
		settingPrintModeSelShow(handle,sel_idx);
	}
	return 0;
}

static int printModeKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
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
		settingPrintModeSelShow(handle,sel_idx);
	}
	return 0;
}

static int printModeOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	sel_idx=configGet(CONFIG_ID_PRINTER_MODE);
	deg_Printf("printMode:sel_idx=%d\n",sel_idx);
//	setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_PRINTER_MODE);
	//==show osd==
	settingPrintModeBaterryShow(handle);
	settingPrintModeSelShow(handle,sel_idx);

	return 0;
}
static int printModeCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{


	return 0;
}
static int printModeWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int printModeSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingPrintModeBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor printModeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,printModeOpenWin},
	{SYS_CLOSE_WINDOW,printModeCloseWin},
	{SYS_CHILE_COLSE,printModeWinChildClose},
	{KEY_EVENT_OK,printModeKeyMsgOk},
	{KEY_EVENT_RETURN,printModeKeyMsgMenu},
	{KEY_EVENT_RIGHT,printModeKeyMsgUp},
	{KEY_EVENT_LEFT,printModeKeyMsgDown},
	{SYS_EVENT_1S,printModeSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(printModeWindow,printModeMsgDeal,printModeWin)


