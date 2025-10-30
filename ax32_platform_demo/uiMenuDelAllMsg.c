#if 0
#include "uiMenuDelAllRes.c"

static uint32 getdelAllResInfor(uint32 item,uint32* image,uint32* str)
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

static int delAllKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	int i,cnt;
	char *name;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,DELALL_SELECT_ID));
		if(item==0)
		{
			uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_COM_WAITING,5);
			cnt = managerFileCount(SysCtrl.avi_list);
			for(i=0;i<cnt;i++)
			{
				if(managerCheckLockExt(SysCtrl.avi_list,i+1)<=0)
				{
					name = manangerGetFileFullPathName(SysCtrl.avi_list,i+1,NULL);
					deg_Printf("delete : %s.",name);
					if(f_unlink(name)==FR_OK)
						deg_Printf("->ok\n");
					else
						deg_Printf("->fail\n");
				}
			}
			if(cnt>0)
				uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
			else
				uiOpenWindow(&tipsWindow,2,R_ID_STR_FIL_NULL,2);
			deamon_fsFreeSize();
			managerDelFileAll(SysCtrl.avi_list);
			SysCtrl.file_index=managerFileCount(SysCtrl.avi_list);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int delAllKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,DELALL_SELECT_ID));
	}
	return 0;
}
static int delAllKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,DELALL_SELECT_ID));
	}
	return 0;
}
static int delAllKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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
static int delAllKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int delAllOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delAll Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,DELALL_SELECT_ID),2,R1h(44));
	itemManageSetColumnSumWithGap(winItem(handle,DELALL_SELECT_ID),2,2,R1w(220),R1w(1));

	itemManageCreateItem(winItem(handle,DELALL_SELECT_ID),itemCreatemenuOption,getdelAllResInfor,2);
	itemManageSetCharInfor(winItem(handle,DELALL_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,DELALL_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,DELALL_SELECT_ID),R_COLOR_BLACK);

	itemManageSetCurItem(winItem(handle,DELALL_SELECT_ID),1);
	
	return 0;
}
static int delAllCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delAll Close Win!!!\n");
	return 0;
}
static int delAllWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delAll WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor delAllMsgDeal[]=
{
	{SYS_OPEN_WINDOW,delAllOpenWin},
	{SYS_CLOSE_WINDOW,delAllCloseWin},
	{SYS_CHILE_COLSE,delAllWinChildClose},
	{KEY_EVENT_OK,delAllKeyMsgOk},
	{KEY_EVENT_UP,delAllKeyMsgUp},
	{KEY_EVENT_DOWN,delAllKeyMsgDown},
	{KEY_EVENT_PLAYBACK,delAllKeyMsgMenu},
//	{KEY_EVENT_MODE,delAllKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(delAllWindow,delAllMsgDeal,delAllWin)
#endif

