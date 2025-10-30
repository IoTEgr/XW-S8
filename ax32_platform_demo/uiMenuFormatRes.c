#include"uiWin.h"

enum
{
	FORMAT_BATTERY_ID=0,
	FORMAT_MSGBOX_BK_ID,
	FORMAT_TIPS_ID,
	FORMAT_YES_BK_ID,
	FORMAT_NO_BK_ID,
	FORMAT_YES_STR_ID,
	FORMAT_NO_STR_ID,
};
static widgetCreateInfor formatWin[] =
{
	//createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_GRAY,WIN_ABS_POS),
	createFrameWin(Rx(50),Ry(50), Rw(220),Rh(142),R_COLOR_GRAY/*R_COLOR_TRANSFER*/,WIN_ABS_POS),
	createImageIcon(FORMAT_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),

//==msgbox==
	createRect(FORMAT_MSGBOX_BK_ID,   Rx(0),Ry(100-100), Rw(220),Rh(142),R_COLOR_GRAY2),
	createStringIcon(FORMAT_TIPS_ID,  Rx(0),Ry(100-100), Rw(220),Rh(40),R_ID_STR_SET_FORMAT,ALIGNMENT_CENTER, R_COLOR_BLUE,0),

	createRect(FORMAT_YES_BK_ID,       Rx(0),Ry(140-100), Rw(220),Rh(40),R_COLOR_GRAY),
	createStringIcon(FORMAT_YES_STR_ID,Rx(0),Ry(140-100), Rw(220),Rh(40),R_ID_STR_COM_OK,ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	createRect(FORMAT_NO_BK_ID,        Rx(0),Ry(182-100), Rw(220),Rh(40),R_COLOR_GRAY),
	createStringIcon(FORMAT_NO_STR_ID, Rx(0),Ry(182-100), Rw(220),Rh(40),R_ID_STR_COM_CANCEL,ALIGNMENT_CENTER, R_COLOR_WHITE,0),


	widgetEnd(),
};


static void settingFormatBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,FORMAT_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,FORMAT_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,FORMAT_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,FORMAT_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,FORMAT_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,FORMAT_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,FORMAT_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,FORMAT_BATTERY_ID),true);
}

static void settingFormatSelShow(winHandle handle,u8 idx)
{
	if(0==idx)
	{
		winSetfgColor(winItem(handle,FORMAT_YES_BK_ID), R_COLOR_YELLOW);
		winSetfgColor(winItem(handle,FORMAT_NO_BK_ID), R_COLOR_GRAY);
	}
	else 
	{
		winSetfgColor(winItem(handle,FORMAT_YES_BK_ID), R_COLOR_GRAY);
		winSetfgColor(winItem(handle,FORMAT_NO_BK_ID), R_COLOR_YELLOW);
	}
}

static void settingFormatTipsShow(winHandle handle,resID str)
{
	winSetVisible(winItem(handle,FORMAT_TIPS_ID),false);
	winSetVisible(winItem(handle,FORMAT_NO_STR_ID),false);

	winSetfgColor(winItem(handle,FORMAT_YES_BK_ID), R_COLOR_GRAY);
	winSetfgColor(winItem(handle,FORMAT_NO_BK_ID), R_COLOR_GRAY);

	winSetResid(winItem(handle,FORMAT_YES_STR_ID),str);
}


