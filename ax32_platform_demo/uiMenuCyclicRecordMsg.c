#include "uiMenuCyclicRecordRes.c"

static u8 sel_idx;
static int cyclicRecordKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			configSet(CONFIG_ID_LOOPTIME,cyclic_record_tab[sel_idx]);
		}
		else if(1==sel_idx)
		{
			configSet(CONFIG_ID_LOOPTIME,cyclic_record_tab[sel_idx]);
		}
		else if(2==sel_idx)
		{
			configSet(CONFIG_ID_LOOPTIME,cyclic_record_tab[sel_idx]);
		}
		else if(3==sel_idx)
		{
			configSet(CONFIG_ID_LOOPTIME,cyclic_record_tab[sel_idx]);
		}

		configSys(CONFIG_ID_LOOPTIME);

		winDestroy(&handle);
	}
	return 0;
}

static int cyclicRecordKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int cyclicRecordKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<=0)
		{
			sel_idx=CYCLIC_RECORD_NUM-1;
		}
		else
		{
			sel_idx--;
		}
		settingCyclicRecordSelShow(handle,sel_idx);
	}
	return 0;
}

static int cyclicRecordKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>=CYCLIC_RECORD_NUM-1)
		{
			sel_idx=0;
		}
		else
		{
			sel_idx++;
		}
		settingCyclicRecordSelShow(handle,sel_idx);
	}
	return 0;
}

static int cyclicRecordOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 cam_res;
	u8 i;
	cam_res=configGet(CONFIG_ID_LOOPTIME);
	for(i=0;i<CYCLIC_RECORD_NUM;i++)
	{
		if(cyclic_record_tab[i]==cam_res)
		{
			break;
		}
	}
	sel_idx=i;

	deg_Printf("cam res:sel_idx=%d\n",sel_idx);
//	setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_LOOPRECORD);
	//==show osd==
	settingCyclicRecordBaterryShow(handle);
	settingCyclicRecordSelShow(handle,sel_idx);

	return 0;
}
static int cyclicRecordCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int cyclicRecordWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int cyclicRecordSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingCyclicRecordBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor cyclicRecordMsgDeal[]=
{
	{SYS_OPEN_WINDOW,cyclicRecordOpenWin},
	{SYS_CLOSE_WINDOW,cyclicRecordCloseWin},
	{SYS_CHILE_COLSE,cyclicRecordWinChildClose},
	{KEY_EVENT_OK,cyclicRecordKeyMsgOk},
	{KEY_EVENT_RETURN,cyclicRecordKeyMsgMenu},
	{KEY_EVENT_RIGHT,cyclicRecordKeyMsgUp},
	{KEY_EVENT_LEFT,cyclicRecordKeyMsgDown},
	{SYS_EVENT_1S,cyclicRecordSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(cyclicRecordWindow,cyclicRecordMsgDeal,cyclicRecordWin)


