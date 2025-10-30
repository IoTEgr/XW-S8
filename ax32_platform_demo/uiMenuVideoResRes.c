#include"uiWin.h"

enum
{
	
	VIDEO_RES_BATTERY_ID=0,
	VIDEO_RES_SEL1_ID,
	VIDEO_RES_SEL2_ID,
};

static widgetCreateInfor videoResWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createImageIcon(VIDEO_RES_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringRim(VIDEO_RES_SEL1_ID,Rx(0),Ry(100), Rw(320),Rh(25),R_ID_STR_RES_1080P_SHORT,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(VIDEO_RES_SEL2_ID,Rx(0),Ry(100+25), Rw(320),Rh(25),R_ID_STR_RES_720P_SHORT,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),

	widgetEnd(),
};


static void settingVideoResBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,VIDEO_RES_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,VIDEO_RES_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,VIDEO_RES_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,VIDEO_RES_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,VIDEO_RES_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,VIDEO_RES_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,VIDEO_RES_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,VIDEO_RES_BATTERY_ID),true);
}

static void settingVideoResSelShow(winHandle handle,u8 idx)
{
	deg_Printf("sel:%d\n",idx);
	winSetStrInfor(winItem(handle,VIDEO_RES_SEL1_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,VIDEO_RES_SEL2_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);

	if(0==idx)
	{
		winSetStrInfor(winItem(handle,VIDEO_RES_SEL1_ID),0,ALIGNMENT_CENTER,R_COLOR_RED);
	}
	else
	{
		winSetStrInfor(winItem(handle,VIDEO_RES_SEL2_ID),0,ALIGNMENT_CENTER,R_COLOR_RED);
	}
}



