#include "uiNoFileRes.c"

static int noFileKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}

static int noFileSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
#if ENABLE_FLASH_PHOTO
	extern uint8 noFile_flag;
	if(SysCtrl.sdcard == SDC_STAT_NORMAL) 
	{
		noFile_flag = 0;
		if(managerFileCount(SysCtrl.avi_list)>0)
			winDestroy(&handle);
	}else if(SDC_STAT_NULL==SysCtrl.sdcard)
	{
		if(spi_udisk_jpg_num()>0)
		{
			winDestroy(&handle);
		}

	}

#else

	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		backBrushBlack();
		if(managerFileCount(SysCtrl.avi_list)>0)
			winDestroy(&handle);
	}
	

#endif

	return 0;
}

#if 0
static int noFileKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
//		 taskStart(TASK_PHOTO_ENCODE,0);
		 deamon_keysound_play();
	}
	return 0;
}

static int noFileKeyMsgVideo(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
//		taskStart(TASK_VIDEO_RECORD,0);
		deamon_keysound_play();
	}
	return 0;
}

static int noFileKeyMsgPlayBack(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		 deamon_keysound_play();
	}
	return 0;
}
#endif

static int noFileKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		 deamon_keysound_play();
	}
	return 0;
}

static int noFileKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		 deamon_keysound_play();
	}
	return 0;
}

static int noFileKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		 deamon_keysound_play();
		 taskStart(TASK_MAIN_MENU,0);
		 if(0==SysCtrl.cartoon_mode)
		 {
			 SysCtrl.cartoon_mode=2;
			 SysCtrl.cartoon_show_cnt=0;
		 }
	}
	return 0;
}
/*
static int noFileKeyMsgPrintEn(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		 deamon_keysound_play();
	}
	return 0;
}

static int noFileKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		 deamon_keysound_play();
	}
	return 0;
}
*/

static int noFileKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_keysound_play();
		taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}



static int noFileOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	//logo_image_show(53);
	
	deg_Printf("noFile open Win!!!\n");
	if(parameNum==1)
	{
		backBrushBlack();
		winSetResid(winItem(handle,NO_FILE_TIP),parame[0]);

	}
		
	return 0;
}
static int noFileCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	backBrushBlack();

	deg_Printf("noFile Close Win!!!\n");
	return 0;
}
static int noFileWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("noFile WinChild Close!!!\n");
	return 0;
}

msgDealInfor noFileMsgDeal[]=
{
	{KEY_EVENT_MODE,noFileKeyMsgMode},
	{SYS_OPEN_WINDOW,noFileOpenWin},
	{SYS_CLOSE_WINDOW,noFileCloseWin},
	{SYS_CHILE_COLSE,noFileWinChildClose},
	{SYS_EVENT_SDC,noFileSysMsgSD},
//	{KEY_EVENT_PLAYBACK,noFileKeyMsgPlayBack},
//	{KEY_EVENT_PHOTO,noFileKeyMsgPhoto},
//	{KEY_EVENT_RECORD,noFileKeyMsgVideo},
	{KEY_EVENT_OK,noFileKeyMsgOk},
	{KEY_EVENT_RETURN,noFileKeyMsgMenu},
//	{KEY_EVENT_PRINTER_EN,noFileKeyMsgPrintEn},
	{KEY_EVENT_UP,noFileKeyMsgUp},
	{KEY_EVENT_DOWN,noFileKeyMsgDown},
//	{KEY_EVENT_POWER,noFileKeyMsgPower},

	{EVENT_MAX,NULL},
};

WINDOW(noFileWindow,noFileMsgDeal,noFileWin)



