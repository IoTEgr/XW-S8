#include"uiWin.h"

enum
{
	MAINMENU_PHOTO_ID=0,
	MAINMENU_VIDEO_ID,
	MAINMENU_MUSIC_ID,
	MAINMENU_PLAYBACK_ID,
	MAINMENU_GAME_ID,
	MAINMENU_SETTING_ID,
	MAINMENU_BATERRY_ID,
	MAINMENU_MAX_ID
};

static widgetCreateInfor mainMenuWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	createStringRim(MAINMENU_PHOTO_ID,Rx(0+12), Ry(108), Rw(98), Rh(25),R_ID_STR_SET_PHOTO,ALIGNMENT_CENTER, R_COLOR_BLACK,0,R_COLOR_WHITE),
	createStringRim(MAINMENU_VIDEO_ID,Rx(112), Ry(108), Rw(98), Rh(25),R_ID_STR_SET_VIDEO,ALIGNMENT_CENTER, R_COLOR_BLACK,0,R_COLOR_WHITE),
	createStringRim(MAINMENU_MUSIC_ID,Rx(214), Ry(108), Rw(98), Rh(25),R_ID_STR_SET_MUSIC,ALIGNMENT_CENTER, R_COLOR_BLACK,0,R_COLOR_WHITE),
	createStringRim(MAINMENU_PLAYBACK_ID,Rx(0+12), Ry(220), Rw(98), Rh(25),R_ID_STR_SET_PLAY,ALIGNMENT_CENTER, R_COLOR_BLACK,0,R_COLOR_WHITE),
	createStringRim(MAINMENU_GAME_ID,Rx(112), Ry(220), Rw(98), Rh(25),R_ID_STR_SET_GAME,ALIGNMENT_CENTER, R_COLOR_BLACK,0,R_COLOR_WHITE),
	createStringRim(MAINMENU_SETTING_ID,Rx(214), Ry(220), Rw(98), Rh(25),R_ID_STR_SET_SETTING,ALIGNMENT_CENTER, R_COLOR_BLACK,0,R_COLOR_WHITE),
	
	//--
	#ifdef J586_XHY_UI
	createImageIcon(MAINMENU_BATERRY_ID,Rx(285), Ry(2), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	#else 
	createImageIcon(MAINMENU_BATERRY_ID,Rx(285), Ry(218), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	#endif
	widgetEnd(),
};

static void mainMenuStrShow(winHandle handle,bool show)
{

	if(true==show)
	{
		winSetVisible(winItem(handle,MAINMENU_PHOTO_ID),true);
		winSetVisible(winItem(handle,MAINMENU_VIDEO_ID),true);
		winSetVisible(winItem(handle,MAINMENU_MUSIC_ID),true);
		winSetVisible(winItem(handle,MAINMENU_PLAYBACK_ID),true);
		winSetVisible(winItem(handle,MAINMENU_GAME_ID),true);
		winSetVisible(winItem(handle,MAINMENU_SETTING_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,MAINMENU_PHOTO_ID),false);
		winSetVisible(winItem(handle,MAINMENU_VIDEO_ID),false);
		winSetVisible(winItem(handle,MAINMENU_MUSIC_ID),false);
		winSetVisible(winItem(handle,MAINMENU_PLAYBACK_ID),false);
		winSetVisible(winItem(handle,MAINMENU_GAME_ID),false);
		winSetVisible(winItem(handle,MAINMENU_SETTING_ID),false);
	}
}

static void mainMenuBaterryShow(winHandle handle)
{

	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		if(SysCtrl.bat_state ==2)
		{
			winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY4);
		}
		else
		{
			SysCtrl.bat_charge_idx++;
			if(SysCtrl.bat_charge_idx>3)
			{
				SysCtrl.bat_charge_idx=0;
			}
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
		}

	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,MAINMENU_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,MAINMENU_BATERRY_ID),true);
}



