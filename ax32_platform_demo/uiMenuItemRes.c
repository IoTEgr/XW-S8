#include"uiWin.h"

enum
{
	ITEM_SELECT_ID=0,
	ITEM_MODE_ID,
	ITEM_HIDE_RECT_ID,
	ITEM_REC_TIME_ID,
	ITEM_RESOLUTION_ID,
	ITEM_MD_ID,// motion detect
	ITEM_MONITOR_ID, // parking monitoring
	ITEM_IRLED_ID,
	ITEM_SD_ID,
	ITEM_MIC_ID,
	ITEM_BATERRY_ID,
	ITEM_POWERON_TIME_ID,
	ITEM_SYSTIME_ID,
};

#if 0//SMALL_PANEL_SUPPORT>0
static widgetCreateInfor menuItemWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_DBLUE,0),
	createRect(ITEM_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_TBLACK),
	createImageIcon(ITEM_MODE_ID,           Rx(0), Ry(0), Rw(24), Rh(25), R_ID_ICON_MTMENU,ALIGNMENT_LEFT),
	createStringIcon(ITEM_RESOLUTION_ID,Rx(132), Ry(0), Rw(40), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
//	createImageIcon(ITEM_MD_ID,                Rx(172), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
//	createImageIcon(ITEM_MONITOR_ID,     Rx(192), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
//	createImageIcon(ITEM_IRLED_ID,             Rx(214),Ry(0), Rw(22), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),

	createImageIcon(ITEM_SD_ID,                  Rx(270), Ry(0), Rw(22), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
//	createImageIcon(ITEM_MIC_ID,                Rx(280), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(ITEM_BATERRY_ID,       Rx(290), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	//createStringIcon(ITEM_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createItemManage(ITEM_SELECT_ID,         Rx(0),Ry(25), Rw(320),Rh(215),INVALID_COLOR),
	widgetEnd(),
};
#else
static widgetCreateInfor menuItemWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_DBLUE,0),
	createRect(ITEM_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_BLACK),
	createImageIcon(ITEM_MODE_ID,           Rx(5), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMENU,ALIGNMENT_LEFT),
	
	createStringIcon(ITEM_REC_TIME_ID,  Rx(25), Ry(0), Rw(75), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(ITEM_RESOLUTION_ID,Rx(132), Ry(0), Rw(30), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
//	createImageIcon(ITEM_MD_ID,                Rx(130), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
//	createImageIcon(ITEM_MONITOR_ID,     Rx(150), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
//	createImageIcon(ITEM_IRLED_ID,             Rx(170),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),

	//createImageIcon(ITEM_SD_ID,                  Rx(270), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
//	createImageIcon(ITEM_MIC_ID,                Rx(230), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	//createImageIcon(ITEM_BATERRY_ID,       Rx(290), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
//	createStringIcon(ITEM_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
	createItemManage(ITEM_SELECT_ID,         Rx(0),Ry(25), Rw(320),Rh(215),INVALID_COLOR),

//	createImageIcon(ITEM_MD_ID, Rx(30), Ry(180), Rw(32), Rh(32), R_ID_ICON_MTOK,ALIGNMENT_LEFT),
//	createImageIcon(ITEM_MONITOR_ID, Rx(180), Ry(180), Rw(64), Rh(32), R_ID_ICON_MTEXIT,ALIGNMENT_LEFT),
	widgetEnd(),
};
#endif

#if 0
static void menuResolutionShow(winHandle handle)
{
	if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_HD)
		winSetResid(winItem(handle,ITEM_RESOLUTION_ID),strToResId("HD"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_FHD)
		winSetResid(winItem(handle,ITEM_RESOLUTION_ID),strToResId("FHD"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_VGA)
		winSetResid(winItem(handle,ITEM_RESOLUTION_ID),strToResId("VGA"));
	else 
		winSetResid(winItem(handle,ITEM_RESOLUTION_ID),strToResId("???"));
}
#endif
static void menuMDShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,ITEM_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,ITEM_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,ITEM_MD_ID),false);
*/
}
static void menuMonitorShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,ITEM_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,ITEM_MONITOR_ID),R_ID_ICON_MTPARKOFF);
*/
}
static void menuIrLedShow(winHandle handle)
{
/*
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,ITEM_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,ITEM_IRLED_ID),R_ID_ICON_MTIRON);
#else
	winSetVisible(winItem(handle,ITEM_IRLED_ID),false);
#endif
*/
}
static void menuSDShow(winHandle handle)
{
	/*if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,ITEM_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,ITEM_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,ITEM_SD_ID),R_ID_ICON_MTSDCNORMAL);*/
}
static void menuMicShow(winHandle handle)
{
/*
	if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,ITEM_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,ITEM_MIC_ID),R_ID_ICON_MTMICON);
*/
}

static void menuBaterryShow(winHandle handle)
{
	/*if(SysCtrl.usb != USB_STAT_NULL)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY5);
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,ITEM_BATERRY_ID),R_ID_ICON_MTBATTERY4);//llh
	winSetVisible(winItem(handle,ITEM_BATERRY_ID),true);*/
}

#if 0//SMALL_PANEL_SUPPORT==0
static void itemPoweOnTimeShow(winHandle handle,uint32 sec)
{
/*
	static char powerOnTimeStr[]="00:00";
	powerOnTimeStr[0] = sec/36000+'0';  // h
	powerOnTimeStr[1] = ((sec/3600)%10)+'0';   // h
	powerOnTimeStr[2] = ':';
	sec=sec%3600;
	powerOnTimeStr[3] = sec/600+'0';  // m
	powerOnTimeStr[4] = ((sec/60)%10)+'0';   // m
	powerOnTimeStr[5] = 0;
	winSetResid(winItem(handle,ITEM_POWERON_TIME_ID),strToResId(powerOnTimeStr));
*/
}
#endif
