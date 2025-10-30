#include "uiMenuCameraResRes.c"

static u8 sel_idx;
static int cameraResKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			configSet(CONFIG_ID_PRESLUTION,camera_res_tab[sel_idx]);
		}
		else if(1==sel_idx)
		{
			configSet(CONFIG_ID_PRESLUTION,camera_res_tab[sel_idx]);
		}
		else if(2==sel_idx)
		{
			configSet(CONFIG_ID_PRESLUTION,camera_res_tab[sel_idx]);
		}
		else if(3==sel_idx)
		{
			configSet(CONFIG_ID_PRESLUTION,camera_res_tab[sel_idx]);
		}
		else if(4==sel_idx)
		{
			configSet(CONFIG_ID_PRESLUTION,camera_res_tab[sel_idx]);
		}
		else if(5==sel_idx)
		{
			configSet(CONFIG_ID_PRESLUTION,camera_res_tab[sel_idx]);
		}
		else if(6==sel_idx)
		{
			configSet(CONFIG_ID_PRESLUTION,camera_res_tab[sel_idx]);
		}

		configSys(CONFIG_ID_PRESLUTION);

		winDestroy(&handle);
	}
	return 0;
}

static int cameraResKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int cameraResKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<=0)
		{
			sel_idx=CAMERA_RES_NUM-1;
		}
		else
		{
			sel_idx--;
		}
		settingCameraResSelShow(handle,sel_idx);
	}
	return 0;
}

static int cameraResKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>=CAMERA_RES_NUM-1)
		{
			sel_idx=0;
		}
		else
		{
			sel_idx++;
		}
		settingCameraResSelShow(handle,sel_idx);
	}
	return 0;
}

static int cameraResOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	u32 cam_res;
	u8 i;
	cam_res=configGet(CONFIG_ID_PRESLUTION);
	for(i=0;i<CAMERA_RES_NUM;i++)
	{
		if(camera_res_tab[i]==cam_res)
		{
			break;
		}
	}
	sel_idx=i;

	deg_Printf("cam res:sel_idx=%d\n",sel_idx);
//	setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_IMAGE_RES);
	//==show osd==
	settingCameraResBaterryShow(handle);
	settingCameraResSelShow(handle,sel_idx);

	return 0;
}
static int cameraResCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{


	return 0;
}
static int cameraResWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}

static int cameraResSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingCameraResBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor cameraResMsgDeal[]=
{
	{SYS_OPEN_WINDOW,cameraResOpenWin},
	{SYS_CLOSE_WINDOW,cameraResCloseWin},
	{SYS_CHILE_COLSE,cameraResWinChildClose},
	{KEY_EVENT_OK,cameraResKeyMsgOk},
	{KEY_EVENT_RETURN,cameraResKeyMsgMenu},
	{KEY_EVENT_RIGHT,cameraResKeyMsgUp},
	{KEY_EVENT_LEFT,cameraResKeyMsgDown},
	{SYS_EVENT_1S,cameraResSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(cameraResWindow,cameraResMsgDeal,cameraResWin)


