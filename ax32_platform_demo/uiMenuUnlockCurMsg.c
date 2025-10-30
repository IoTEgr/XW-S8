#if 0
#include "uiMenuUnlockCurRes.c"

static uint32 getunlockCurResInfor(uint32 item,uint32* image,uint32* str)
{
	if(item==0)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_OK;
	}
	else if(item==1)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_COM_CANCEL;
	}
	return 0;
}

static int unlockCurKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	char *name;
	char *srcName;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,UNLOCKCUR_SELECT_ID));
		if(item==0)
		{
			if(managerFileCount(SysCtrl.avi_list)<=0)
			{
				uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
				return;
			}
			if(managerCheckLockExt(SysCtrl.avi_list,SysCtrl.file_index)>0)
			{
				srcName = (char *)hal_sysMemMalloc(64,32);
				if(srcName==NULL)
				{
					deg_Printf("[err]memory malloc error!!!\n");
					uiOpenWindow(&tipsWindow,2,"memory malloc failed",2);
					return 0;
				}	
				name = manangerGetFileFullPathName(SysCtrl.avi_list,SysCtrl.file_index,NULL);
				strcpy(srcName,name);
				managerUnlockFile(name);
				deg_Printf("unlock : %s -> %s.",srcName,name);
				if(f_rename(srcName,name)==FR_OK)  // rename in file system
				{
					deg_Printf("->ok\n");
					managerUnlockFileExt(SysCtrl.avi_list,SysCtrl.file_index);
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
				}
				else
				{
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
					deg_Printf("->fail\n");
				}
				hal_sysMemFree(srcName);
				managerFlush(SysCtrl.avi_list);
			}		
			else
				uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int unlockCurKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,UNLOCKCUR_SELECT_ID));
	}
	return 0;
}
static int unlockCurKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,UNLOCKCUR_SELECT_ID));
	}
	return 0;
}
static int unlockCurKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int unlockCurKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int unlockCurOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("unlockCur Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,UNLOCKCUR_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,UNLOCKCUR_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,UNLOCKCUR_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif 
	itemManageCreateItem(winItem(handle,UNLOCKCUR_SELECT_ID),itemCreatemenuOption,getunlockCurResInfor,2);
	itemManageSetCharInfor(winItem(handle,UNLOCKCUR_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,UNLOCKCUR_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,UNLOCKCUR_SELECT_ID),R_COLOR_GRAY);

	itemManageSetCurItem(winItem(handle,UNLOCKCUR_SELECT_ID),1);
	
	return 0;
}
static int unlockCurCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("unlockCur Close Win!!!\n");
	return 0;
}
static int unlockCurWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("unlockCur WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor unlockCurMsgDeal[]=
{
	{SYS_OPEN_WINDOW,unlockCurOpenWin},
	{SYS_CLOSE_WINDOW,unlockCurCloseWin},
	{SYS_CHILE_COLSE,unlockCurWinChildClose},
	{KEY_EVENT_PRINTER_EN,unlockCurKeyMsgOk},
	{KEY_EVENT_UP,unlockCurKeyMsgUp},
	{KEY_EVENT_DOWN,unlockCurKeyMsgDown},
	{KEY_EVENT_MENU,unlockCurKeyMsgMenu},
	{KEY_EVENT_MODE,unlockCurKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(unlockCurWindow,unlockCurMsgDeal,unlockCurWin)

#endif


