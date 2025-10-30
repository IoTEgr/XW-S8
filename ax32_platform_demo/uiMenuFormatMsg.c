#include "uiMenuFormatRes.c"

static u8 sel_idx;

static int formatKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	uint32 item;
	INT8U *mem;
	INT8U res;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(0==sel_idx)
		{
			mem = (INT8U *)hal_sysMemMalloc(32768,64);
			if(mem == NULL)
			{
				deg_Printf("malloc mem for format fail.\n");
				
				return 0;
			}
			//uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_TIPS_PLEASE_WAITING,5);
			settingFormatTipsShow(handle,R_ID_STR_TIPS_PLEASE_WAITING);
			drawUIService(true);
			
			//--new
			TCHAR path[1] = {0};
			res =f_mkfs(path,NULL,(void *)mem,32768);
			//res =f_mkfs("",FM_ANY,0,(void *)mem,32768);
			spi_udisk_format(FLASH_CAPACITY);
			hal_sysMemFree(mem);
			if(res==FR_OK)
			{				
				//SysCtrl.sdcard = SDC_STAT_NULL;  // systemDeamonService will mount 
				managerDestory(SysCtrl.avi_list);
				managerDestory(SysCtrl.jpg_list);
				managerDestory(SysCtrl.wav_list);
				managerInit();
				SysCtrl.avi_list = -1;
				SysCtrl.jpg_list = -1;
				SysCtrl.wav_list = -1;
				deamon_fs_scan();
				//uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_FMT_SUCCESS,2);
				settingFormatTipsShow(handle,R_ID_STR_TIPS_FMT_SUCCESS);
			}
			else
			{
				deg_Printf ("error : %d\n",res);
				//uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_FMT_FAIL,2);
				settingFormatTipsShow(handle,R_ID_STR_TIPS_FMT_FAIL);
			}
			drawUIService(true);
			XOSTimeDly(2000);
		}

		winDestroy(&handle);
	}
	return 0;
}

static int formatKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
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
		settingFormatSelShow(handle,sel_idx);
	}
	return 0;
}
static int formatKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
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
		settingFormatSelShow(handle,sel_idx);
	}
	return 0;
}
static int formatKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int formatOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("format Open Win!!!\n");
	sel_idx=0;
	//setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_FORMAT);

	//==osd==
	settingFormatBaterryShow(handle);
	settingFormatSelShow(handle,sel_idx);

	return 0;
}
static int formatCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("format Close Win!!!\n");
	return 0;
}
static int formatWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int formatSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		settingFormatBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor formatMsgDeal[]=
{
	{SYS_OPEN_WINDOW,formatOpenWin},
	{SYS_CLOSE_WINDOW,formatCloseWin},
	{SYS_CHILE_COLSE,formatWinChildClose},
	{KEY_EVENT_OK,formatKeyMsgOk},
	{KEY_EVENT_UP,formatKeyMsgUp},
	{KEY_EVENT_DOWN,formatKeyMsgDown},
	{KEY_EVENT_RETURN,formatKeyMsgMenu},
	{SYS_EVENT_1S,formatSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(formatWindow,formatMsgDeal,formatWin)


