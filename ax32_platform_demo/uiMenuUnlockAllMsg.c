#if 0
#include "uiMenuUnlockAllRes.c"

static uint32 getunlockAllResInfor(uint32 item,uint32* image,uint32* str)
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

static int unlockAllKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	int i,cnt;
	char *name;
	char *srcName;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,UNLOCKALL_SELECT_ID));
		if(item==0)
		{
			cnt = managerFileCount(SysCtrl.avi_list);
			if(cnt>0)
			{
				srcName = (char *)hal_sysMemMalloc(64,32);
				if(srcName==NULL)
				{
					deg_Printf("[err]memory malloc error!!!\n");
					uiOpenWindow(&tipsWindow,2,"memory malloc failed",2);
					return 0;
				}
				uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_COM_WAITING,5);
				for(i=0;i<cnt;i++)
				{
					if(managerCheckLockExt(SysCtrl.avi_list,i+1)>0)
					{
						name = manangerGetFileFullPathName(SysCtrl.avi_list,i+1,NULL);
						strcpy(srcName,name);
						managerUnlockFile(name);
						deg_Printf("unlock : %s -> %s.",srcName,name);
						if(f_rename(srcName,name)==FR_OK)  // rename in file system
						{
							deg_Printf("->ok\n");
							managerUnlockFileExt(SysCtrl.avi_list,i+1);
						}
						else
							deg_Printf("->fail\n");
						managerFlush(SysCtrl.avi_list);
					}
				}
				hal_sysMemFree(srcName);
				uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
			}
			else
				uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int unlockAllKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,UNLOCKALL_SELECT_ID));
	}
	return 0;
}
static int unlockAllKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,UNLOCKALL_SELECT_ID));
	}
	return 0;
}
static int unlockAllKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int unlockAllKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int unlockAllOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("unlockAll Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,UNLOCKALL_SELECT_ID),1,R1h(32));
	#if SMALL_PANEL_SUPPORT==0
	itemManageSetColumnSumWithGap(winItem(handle,UNLOCKALL_SELECT_ID),0,2,R1w(50),R1w(12));
	#else
	itemManageSetColumnSumWithGap(winItem(handle,UNLOCKALL_SELECT_ID),0,2,R1w(100),R1w(6));
	#endif 
	itemManageCreateItem(winItem(handle,UNLOCKALL_SELECT_ID),itemCreatemenuOption,getunlockAllResInfor,2);
	itemManageSetCharInfor(winItem(handle,UNLOCKALL_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,UNLOCKALL_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,UNLOCKALL_SELECT_ID),R_COLOR_GRAY);

	itemManageSetCurItem(winItem(handle,UNLOCKALL_SELECT_ID),1);
	
	return 0;
}
static int unlockAllCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("unlockAll Close Win!!!\n");
	return 0;
}
static int unlockAllWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("unlockAll WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor unlockAllMsgDeal[]=
{
	{SYS_OPEN_WINDOW,unlockAllOpenWin},
	{SYS_CLOSE_WINDOW,unlockAllCloseWin},
	{SYS_CHILE_COLSE,unlockAllWinChildClose},
	{KEY_EVENT_PRINTER_EN,unlockAllKeyMsgOk},
	{KEY_EVENT_UP,unlockAllKeyMsgUp},
	{KEY_EVENT_DOWN,unlockAllKeyMsgDown},
	{KEY_EVENT_MENU,unlockAllKeyMsgMenu},
	{KEY_EVENT_MODE,unlockAllKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(unlockAllWindow,unlockAllMsgDeal,unlockAllWin)

#endif

