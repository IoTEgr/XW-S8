#include"uiWin.h"

enum
{
	
	VERSION_BATTERY_ID=0,
	VERSION_TIPS_IDA,
	VERSION_TIPS_ID,
};

static widgetCreateInfor versionWin[] =
{
	//createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createFrameWin(Rx(50),Ry(50), Rw(220),Rh(142),R_COLOR_GRAY2/*R_COLOR_TRANSFER*/,WIN_ABS_POS),
	//createImageIcon(VERSION_BATTERY_ID,Rx(320-32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringRim(VERSION_TIPS_ID,Rx(0),Ry(70), Rw(220),Rh(30),SOFTWARE_VERSION,ALIGNMENT_CENTER, R_COLOR_BLUE,0,R_COLOR_BLACK),
	createStringIcon(VERSION_TIPS_IDA,Rx(0),Ry(40), Rw(220),Rh(30),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(VERSION_TIPS_ID,Rx(0),Ry(70), Rw(220),Rh(30),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	widgetEnd(),
};


static void settingVersionBaterryShow(winHandle handle)
{

	return ;
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,VERSION_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,VERSION_BATTERY_ID),true);
}



static void settingVersionStrShow(winHandle handle,bool show)
{
	winSetResid(winItem(handle,VERSION_TIPS_ID),strToResId(SOFTWARE_VERSION));
	winSetResid(winItem(handle,VERSION_TIPS_IDA),strToResId("A"));

	if(true==show)
	{
		winSetVisible(winItem(handle,VERSION_TIPS_ID),true);
		if(SysCtrl.credit_flag)	//若已授权 版本号多显示一个A
			winSetVisible(winItem(handle,VERSION_TIPS_IDA),true);
		else
			winSetVisible(winItem(handle,VERSION_TIPS_IDA),false);
	}
	else 
	{
		winSetVisible(winItem(handle,VERSION_TIPS_ID),false);
		winSetVisible(winItem(handle,VERSION_TIPS_IDA),false);
	}
}
