#include"uiWin.h"

enum
{
	DEFAULT_BATTERY_ID=0,
	DEFAULT_MSGBOX_BK_ID,
	DEFAULT_TIPS_ID,
	DEFAULT_YES_BK_ID,
	DEFAULT_NO_BK_ID,
	DEFAULT_YES_STR_ID,
	DEFAULT_NO_STR_ID,
};

static widgetCreateInfor defaultWin[] =
{
	//createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createFrameWin(Rx(50),Ry(50), Rw(220),Rh(142),R_COLOR_GRAY/*R_COLOR_TRANSFER*/,WIN_ABS_POS),
	createImageIcon(DEFAULT_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),

//==msgbox==
	createRect(DEFAULT_MSGBOX_BK_ID,Rx(0),Ry(100-100), Rw(220),Rh(142),R_COLOR_GRAY2),
	createStringIcon(DEFAULT_TIPS_ID,Rx(0),Ry(100-100), Rw(220),Rh(40),R_ID_STR_SET_DEFAULT_SETTING,ALIGNMENT_CENTER, R_COLOR_BLUE,0),

	createRect(DEFAULT_YES_BK_ID,Rx(0),Ry(140-100), Rw(220),Rh(40),R_COLOR_GRAY),
	createStringIcon(DEFAULT_YES_STR_ID,Rx(0),Ry(140-100), Rw(220),Rh(40),R_ID_STR_COM_OK,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createRect(DEFAULT_NO_BK_ID,Rx(0),Ry(182-100), Rw(220),Rh(40),R_COLOR_GRAY),
	createStringIcon(DEFAULT_NO_STR_ID,Rx(0),Ry(182-100), Rw(220),Rh(40),R_ID_STR_COM_CANCEL,ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	widgetEnd(),
};


static void settingDefaultBaterryShow(winHandle handle)
{

	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,DEFAULT_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,DEFAULT_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,DEFAULT_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,DEFAULT_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,DEFAULT_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,DEFAULT_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,DEFAULT_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,DEFAULT_BATTERY_ID),true);
}

static void settingDefaultSelShow(winHandle handle,u8 idx)
{
	if(0==idx)
	{
		winSetfgColor(winItem(handle,DEFAULT_YES_BK_ID), R_COLOR_YELLOW);
		winSetfgColor(winItem(handle,DEFAULT_NO_BK_ID), R_COLOR_GRAY);
	}
	else 
	{
		winSetfgColor(winItem(handle,DEFAULT_YES_BK_ID), R_COLOR_GRAY);
		winSetfgColor(winItem(handle,DEFAULT_NO_BK_ID), R_COLOR_YELLOW);
	}
}

#if 0
static void settingDefaultTipsShow(winHandle handle,resID str)
{
	winSetVisible(winItem(handle,DEFAULT_TIPS_ID),false);
	winSetVisible(winItem(handle,DEFAULT_NO_STR_ID),false);

	winSetfgColor(winItem(handle,DEFAULT_YES_BK_ID), R_COLOR_GRAY);
	winSetfgColor(winItem(handle,DEFAULT_NO_BK_ID), R_COLOR_GRAY);

	winSetResid(winItem(handle,DEFAULT_YES_STR_ID),str);
}
#endif


