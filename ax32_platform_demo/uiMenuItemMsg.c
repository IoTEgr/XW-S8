#include "uiMenuItemRes.c"

#define currentPage()    (currentMenu->pPage[currentMenu->curPage])
#define getItem(n)   (currentPage().pItem[n])
static menu* currentMenu=NULL;
static bool menuOpened=false;

bool menuIsOpen(void)
{
	return menuOpened;
}

#if 0
static uint32 getItemResInfor(uint32 item,uint32* image,uint32* str)
{
	if(image)
		*image=getItem(item).image;
	if(str)
		*str=getItem(item).str;
	return 0;
}
#endif

static uint32 getItemResInforEx(uint32 item,uint32* image,uint32* str,uint32* selectImage,uint32* selectStr)
{
	uint32 config;
	menuItem* pItem=&getItem(item);
	if(image)
		*image=pItem->image;
	if(str)
		*str=pItem->str;
	if(pItem->configId==0&&pItem->optionSum==0)
	{
		if(selectImage)
			*selectImage=INVALID_RES_ID;
		if(selectStr)
			*selectStr=INVALID_RES_ID;
	}
	else if(pItem->configId==0)
	{
		if(selectImage)
			*selectImage=INVALID_RES_ID;
		if(selectStr)
			*selectStr=pItem->pOption[0].str;
	}
	else
	{
		if(CONFIG_ID_LANGUAGE == pItem->configId)	// only language show cfg
		{
			config=configGet(pItem->configId);
			if(pItem->optionSum==2&&(config==R_ID_STR_COM_OFF||config==R_ID_STR_COM_ON))
			{
				if(selectImage)
				{
					if(config==R_ID_STR_COM_OFF)
						*selectImage=R_ID_ICON_MTOFF;
					else
						*selectImage=R_ID_ICON_MTON;
				}
				if(selectStr)
					*selectStr=INVALID_RES_ID;
			}
			else
			{
				if(selectImage)
					*selectImage=R_ID_ICON_MTMORE;
				if(selectStr)
					*selectStr=config;
			}
		}
		else
		{
			if(selectImage)
				*selectImage=INVALID_RES_ID;
			if(selectStr)
				*selectStr=INVALID_RES_ID;
		}
	}
	return 0;	
}

static int menuItemKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	menuItem* pItem;
	#if SMALL_PANEL_SUPPORT==1
	//uint32 config;	
	#endif
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		pItem=&getItem(itemManageGetCurrentItem(winItem(handle,ITEM_SELECT_ID)));
		if(pItem->optionSum==0)
			((menuItemProc)pItem->pOption)(handle,parameNum,parame);
		else if(pItem->optionSum>1)
		{
		/*
			#if SMALL_PANEL_SUPPORT==1// only 2 option and one of them is "OFF" and another is "ON"
			config=configGet(pItem->configId);
			if(pItem->optionSum==2&&(config==R_ID_STR_COM_OFF||config==R_ID_STR_COM_ON))
			{
				if(config==R_ID_STR_COM_OFF)
					configSet(pItem->configId,R_ID_STR_COM_ON);
				else
					configSet(pItem->configId,R_ID_STR_COM_OFF);
				configSys(pItem->configId);
				userConfigSave();
				
				menuMDShow(handle);
				menuMonitorShow(handle);
				menuMicShow(handle);
				itemManageUpdateCurItem(winItem(handle,ITEM_SELECT_ID));
			}
			else
			#endif
			*/
				uiOpenWindow(&menuOptionWindow,1,pItem);
		}
	}
	return 0;
}
static int menuItemKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManagePreItem(winItem(handle,ITEM_SELECT_ID));
	}
	return 0;
}
static int menuItemKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		itemManageNextItem(winItem(handle,ITEM_SELECT_ID));
	}
	return 0;
}
#if 0
static int menuItemKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		currentMenu->curPage++;
		if(currentMenu->curPage<currentMenu->pageSum)
		{
			winSetResid(winItem(handle,ITEM_MODE_ID),currentPage().selectImage);
			itemManageUpdateRes(winItem(handle,ITEM_SELECT_ID),currentPage().itemSum,0);
		}
		else
			winDestroy(&handle);
	}
	return 0;
}
static int menuItemKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
#endif
static int menuItemSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	menuSDShow(handle);
	return 0;
}
static int menuSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	menuBaterryShow(handle);
	return 0;
}
static int menuSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		menuBaterryShow(handle);
	return 0;
}
static int menuItemOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(parameNum<1)
	{
		winDestroy(&handle);
		return 0;
	}
	deg_Printf("menuItem Open Win!!!\n");
	currentMenu=(menu*)parame[0];
	currentMenu->curPage=0;
	itemManageSetItemHeight(winItem(handle,ITEM_SELECT_ID),R1h(35));
	//#if SMALL_PANEL_SUPPORT>0
	itemManageCreateItem(winItem(handle,ITEM_SELECT_ID),itemCreateMenuItem,NULL,currentPage().itemSum);// no sub menu preview 
	/*
	#else
	itemManageCreateItem(winItem(handle,ITEM_SELECT_ID),itemCreateMenuItemEx,NULL,currentPage().itemSum);
	#endif
	*/
	itemManageSetResInforFuncEx(winItem(handle,ITEM_SELECT_ID),getItemResInforEx);
	
	itemManageSetCharInfor(winItem(handle,ITEM_SELECT_ID),0,ALIGNMENT_LEFT,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,ITEM_SELECT_ID),R_COLOR_YELLOW/*R_COLOR_BLUE1*/);
	itemManageSetUnselectColor(winItem(handle,ITEM_SELECT_ID),R_COLOR_GRAY);
	itemManageNextItem(winItem(handle,ITEM_SELECT_ID));

	//winSetResid(winItem(handle,ITEM_MODE_ID),currentPage().selectImage);
	//menuResolutionShow(handle);
	menuMDShow(handle);
	menuMonitorShow(handle);
	menuIrLedShow(handle);
	menuSDShow(handle);
	menuMicShow(handle);
	menuBaterryShow(handle);
//	#if SMALL_PANEL_SUPPORT==0
//	itemPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	menuOpened=true;
//======
	if(parameNum>=2)
	{
		u32 open_item=parame[1];
		if(8==open_item)
		{
			u8 i;
			menuItem* pItem;
//			uint32 config;	
			for(i=0;i<open_item;i++)
			{
				itemManageNextItem(winItem(handle,ITEM_SELECT_ID));
			}
			
			pItem=&getItem(itemManageGetCurrentItem(winItem(handle,ITEM_SELECT_ID)));
			if(pItem->optionSum==0)
				((menuItemProc)pItem->pOption)(handle,parameNum,parame);
			else if(pItem->optionSum>1)
			{
				uiOpenWindow(&menuOptionWindow,1,pItem);
			}
		}
	}
	return 0;
}
static int menuItemCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuItem Close Win!!!\n");
	currentMenu=NULL;
	menuOpened=false;
	return 0;
}
static int menuItemWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("menuItem WinChild Close!!!\n");
	//winSetResid(winItem(handle,ITEM_MODE_ID),currentPage().selectImage);
	//menuResolutionShow(handle);
	menuMDShow(handle);
	menuMonitorShow(handle);
	menuIrLedShow(handle);
	menuSDShow(handle);
	menuMicShow(handle);
	menuBaterryShow(handle);
	//itemManageUpdateCurItem(winItem(handle,ITEM_SELECT_ID));
	itemManageUpdateAllitem(winItem(handle,ITEM_SELECT_ID));
//	#if SMALL_PANEL_SUPPORT==0
//	itemPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif

	if(0==configGet(CONFIG_ID_LANGUAGE))
	{
		menuItem* pItem;
		itemManagePreItem(winItem(handle,ITEM_SELECT_ID));
		itemManagePreItem(winItem(handle,ITEM_SELECT_ID));
		itemManagePreItem(winItem(handle,ITEM_SELECT_ID));
		itemManagePreItem(winItem(handle,ITEM_SELECT_ID));
		itemManagePreItem(winItem(handle,ITEM_SELECT_ID));
		itemManagePreItem(winItem(handle,ITEM_SELECT_ID));

		pItem=&getItem(itemManageGetCurrentItem(winItem(handle,ITEM_SELECT_ID)));
		if(pItem->optionSum==0)
			((menuItemProc)pItem->pOption)(handle,parameNum,parame);
		else if(pItem->optionSum>1)
		{
			uiOpenWindow(&menuOptionWindow,1,pItem);
		}	
	}

/*
	if(1==SysCtrl.first_menu_lan)
	{
		SysCtrl.first_menu_lan=0;
		winDestroy(&handle);

		//==cancal frame===
		videoRecordCmdSet(CMD_COM_FRAME,0);
		hal_custom_frame_add_enable(0);
		//==normal color===
		SysCtrl.spec_color_index=0;
		cmos_spec_color(SysCtrl.spec_color_index);
		//==pic task==
		if(TASK_PHOTO_ENCODE!=taskCurrent())
		{
			taskStart(TASK_PHOTO_ENCODE,0);
		}
	}
*/
	return 0;
}
static int menuItemSysMsg1s(winHandle handle,uint32 parameNum,uint32* parame)
{
#if 0
	if(SysCtrl.usb == USB_STAT_DCIN)
	{
		if(winIsVisible(winItem(handle,ITEM_BATERRY_ID)))
			winSetVisible(winItem(handle,ITEM_BATERRY_ID),false);
		else
		{
			winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY5);
			winSetVisible(winItem(handle,ITEM_BATERRY_ID),true);
		}	
	}
#endif    
//	#if SMALL_PANEL_SUPPORT==0
//	itemPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	return 0;
}

static int menuKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

msgDealInfor menuItemMsgDeal[]=
{
	{SYS_OPEN_WINDOW,menuItemOpenWin},
	{SYS_CLOSE_WINDOW,menuItemCloseWin},
	{SYS_CHILE_COLSE,menuItemWinChildClose},
	{KEY_EVENT_OK,menuItemKeyMsgOk},
	{KEY_EVENT_UP,menuItemKeyMsgUp},
	{KEY_EVENT_DOWN,menuItemKeyMsgDown},
	{KEY_EVENT_LEFT,menuItemKeyMsgUp},
	{KEY_EVENT_RIGHT,menuItemKeyMsgDown},
	{KEY_EVENT_RETURN,menuKeyMsgExit},
//	{KEY_EVENT_MENU,menuItemKeyMsgMenu},
//	{KEY_EVENT_PLAYBACK,menuItemKeyMsgMenu},
	{SYS_EVENT_SDC,menuItemSysMsgSD},
	{SYS_EVENT_USB,menuSysMsgUSB},
	{SYS_EVENT_BAT,menuSysMsgBattery},
	{SYS_EVENT_1S,menuItemSysMsg1s},
	{EVENT_MAX,NULL},
};

WINDOW(menuItemWindow,menuItemMsgDeal,menuItemWin)


