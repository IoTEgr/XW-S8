#include "uiMenuUsbRes.c"


static uint32 getMenuUsbResInfor(uint32 item,uint32* image,uint32* str)
{
	if(image)
		*image=INVALID_RES_ID;
	if(item==0)
	{
		if(str)
			*str=R_ID_STR_SET_USBMASS;
	}
	else if(item==1)
	{
		if(str)
			*str=R_ID_STR_SET_USBCAM;
	}
	else if(item==2)
	{
		if(str)
			*str=R_ID_STR_SET_VIDEO;
	}
	return 0;
}

static int menuUsbKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,MENU_USB_SELECT_ID));
		if(item==0)
		{
			deg_Printf("usb mass storage,0x%x\n",R_ID_STR_SET_USBMASS);
			taskStart(TASK_USB_DEVICE,USB_DEV_MASS);
		}
		else if(item==1)
		{
			deg_Printf("usb pc camera,0x%x\n",R_ID_STR_SET_USBCAM);
			taskStart(TASK_USB_DEVICE,USB_DEV_PCCAMERA);
		}
		else
			taskStart(TASK_MAIN_MENU,0);
	}
	return 0;
}
static int menuUsbKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,MENU_USB_SELECT_ID));
	}
	return 0;
}
static int menuUsbKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,MENU_USB_SELECT_ID));
	}
	return 0;
}
static int menuUsbKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//winDestroy(&handle);
	}
	return 0;
}
static int menuUsbKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int menuUsbSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		taskStart(TASK_POWER_OFF,0);
	return 0;
}
static int menuUsbOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	itemManageSetRowSum(winItem(handle,MENU_USB_SELECT_ID),3,R1h(32));
	itemManageCreateItem(winItem(handle,MENU_USB_SELECT_ID),itemCreatemenuOption,getMenuUsbResInfor,3);
	itemManageSetCharInfor(winItem(handle,MENU_USB_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,MENU_USB_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,MENU_USB_SELECT_ID),R_COLOR_GRAY3);
	itemManageSetCurItem(winItem(handle,MENU_USB_SELECT_ID),0);
	return 0;
}
static int menuUsbCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuUsb Close Win!!!\n");
	return 0;
}
static int menuUsbWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuUsb WinChild Close!!!\n");
	return 0;
}
#if 0
static int menuUsbSysMsg500Ms(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
#endif

msgDealInfor menuUsbMsgDeal[]=
{
	{SYS_OPEN_WINDOW,menuUsbOpenWin},
	{SYS_CLOSE_WINDOW,menuUsbCloseWin},
	{SYS_CHILE_COLSE,menuUsbWinChildClose},
	{KEY_EVENT_OK,menuUsbKeyMsgOk},
	{KEY_EVENT_UP,menuUsbKeyMsgUp},
	{KEY_EVENT_DOWN,menuUsbKeyMsgDown},
	{KEY_EVENT_MENU,menuUsbKeyMsgMenu},
	{KEY_EVENT_MODE,menuUsbKeyMsgMode},
	{SYS_EVENT_USB,menuUsbSysMsgUSB},
	{EVENT_MAX,NULL},
};

WINDOW(menuUsbWindow,menuUsbMsgDeal,menuUsbWin)


