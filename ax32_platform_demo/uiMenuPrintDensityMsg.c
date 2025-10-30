#include "uiMenuPrintDensityRes.c"

static s16 sel_idx;
static int printDensityKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		configSet(CONFIG_ID_PRINTER_DENSITY,sel_idx);
		SysCtrl.printer_level=sel_idx;
		deg_Printf("SysCtrl.printer_level=%d\n",SysCtrl.printer_level);
		winDestroy(&handle);
	}
	return 0;
}

static int printDensityKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int printDensityKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<=0)
		{
			sel_idx=PRINT_DENSITY_NUM-1;
		}
		else
		{
			sel_idx--;
		}
		settingPrintDensitySelShow(handle,sel_idx);
	}
	return 0;
}

static int printDensityKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>=PRINT_DENSITY_NUM-1)
		{
			sel_idx=0;
		}
		else
		{
			sel_idx++;
		}
		settingPrintDensitySelShow(handle,sel_idx);
	}
	return 0;
}

static int printDensityOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	sel_idx=configGet(CONFIG_ID_PRINTER_DENSITY);
	deg_Printf("printMode:sel_idx=%d\n",sel_idx);
//	setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_PRINTER_DENSITY);
	//==show osd==
	settingPrintDensityBaterryShow(handle);
	settingPrintDensitySelShow(handle,sel_idx);

	return 0;
}
static int printDensityCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{


	return 0;
}
static int printDensityWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int printDensitySysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingPrintDensityBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor printDensityMsgDeal[]=
{
	{SYS_OPEN_WINDOW,printDensityOpenWin},
	{SYS_CLOSE_WINDOW,printDensityCloseWin},
	{SYS_CHILE_COLSE,printDensityWinChildClose},
	{KEY_EVENT_OK,printDensityKeyMsgOk},
	{KEY_EVENT_RETURN,printDensityKeyMsgMenu},
	{KEY_EVENT_RIGHT,printDensityKeyMsgUp},
	{KEY_EVENT_LEFT,printDensityKeyMsgDown},
	{SYS_EVENT_1S,printDensitySysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(printDensityWindow,printDensityMsgDeal,printDensityWin)


