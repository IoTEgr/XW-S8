#include"uiWin.h"

enum
{
	VIDEO_MODE_ID=0,
	VIDEO_HIDE_RECT_ID,
	VIDEO_REC_TIME_ID,
	VIDEO_RESOLUTION_ID,
	VIDEO_MD_ID,// motion detect
	VIDEO_MONITOR_ID, // parking monitoring
	VIDEO_IRLED_ID,	// 6
	VIDEO_LOCK_ID,
	VIDEO_SD_ID,
	VIDEO_MIC_ID,	// 9
	VIDEO_BATERRY_ID,
	VIDEO_POWERON_TIME_ID,
	VIDEO_CAPTURE_PHOTO_ID,	// 12
	VIDEO_SYSTIME_ID,
	VIDEO_BOTTOM_RECT_ID,
	VIDEO_LOOP_RECORD_ID,
	VIDEO_IR_ID,
	VIDEO_TESTMODE_ID,
	VIDEO_MAX_ID
};


static widgetCreateInfor videoRecordeWin[] =
{
	createFrameWin( Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createRect(VIDEO_HIDE_RECT_ID,       Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_TBLACK),
	//createRect(VIDEO_HIDE_RECT_ID, Rx(0),Ry(0), Rw(320),Rh(32),R_COLOR_BLACK),
	createImageIcon(VIDEO_MODE_ID, Rx(2), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTRECORD,ALIGNMENT_LEFT),
	createStringIcon(VIDEO_REC_TIME_ID,  Rx(140), Ry(0), Rw(75), Rh(32),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(VIDEO_RESOLUTION_ID,Rx(230), Ry(0), Rw(32), Rh(32),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(VIDEO_TESTMODE_ID,Rx(5),  Ry(30),  Rw(75),  Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_RED,0),
//	createImageIcon(VIDEO_MD_ID,        Rx(130), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
//	createImageIcon(VIDEO_MONITOR_ID,   Rx(150), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
//#if (1 == IR_MENU_EN)
//	createImageIcon(VIDEO_IRLED_ID,     Rx(170),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
//#endif
//	createImageIcon(VIDEO_LOCK_ID,      Rx(190),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),
	createImageIcon(VIDEO_SD_ID, Rx(4+32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
//	createImageIcon(VIDEO_MIC_ID,       Rx(230), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(VIDEO_BATERRY_ID,Rx(320-32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
//	createStringIcon(VIDEO_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHITE,0),
//	createImageIcon(VIDEO_CAPTURE_PHOTO_ID,  Rx(147), Ry(26), Rw(20), Rh(25), R_ID_ICON_MTPHOTO,ALIGNMENT_CENTER),
	//createRect(VIDEO_BOTTOM_RECT_ID,Rx(0),Ry(215),  Rw(320),Rh(25),R_COLOR_BLACK),
	createStringIcon(VIDEO_SYSTIME_ID,Rx(5), Ry(215), Rw(220), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createImageIcon(VIDEO_LOOP_RECORD_ID, Rx(70), Ry(0), Rw(32), Rh(32), /*NULL*/0,ALIGNMENT_CENTER),
	//createImageIcon(VIDEO_IR_ID,Rx(105), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
   
	widgetEnd(),
};

static void videoIRShow(winHandle handle)
{

//	u32 val=0;
/*	boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,&val);
	if(val)
		winSetResid(winItem(handle,VIDEO_IR_ID),R_ID_ICON_MTIRON);
	else
		winSetResid(winItem(handle,VIDEO_IR_ID),R_ID_ICON_MTIROFF); */
}

static void videoResolutionShow(winHandle handle)
{

	if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_HD)
		winSetResid(winItem(handle,VIDEO_RESOLUTION_ID),strToResId("HD"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_FHD)
		winSetResid(winItem(handle,VIDEO_RESOLUTION_ID),strToResId("FHD"));
	else if(configGet(CONFIG_ID_RESOLUTION)==R_ID_STR_RES_VGA)
		winSetResid(winItem(handle,VIDEO_RESOLUTION_ID),strToResId("VGA"));
	else 
		winSetResid(winItem(handle,VIDEO_RESOLUTION_ID),strToResId("???"));
}
static void videoMDShow(winHandle handle)
{

/*
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,VIDEO_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,VIDEO_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,VIDEO_MD_ID),false);
*/
}
static void videoMonitorShow(winHandle handle)
{

/*
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,VIDEO_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,VIDEO_MONITOR_ID),R_ID_ICON_MTPARKOFF);
*/
}
static void videoIrLedShow(winHandle handle)
{

/*
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,VIDEO_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,VIDEO_IRLED_ID),R_ID_ICON_MTIRON);
#endif
*/
}
static void videoLockShow(winHandle handle)
{

/*
	if(SysCtrl.g_lock)
	{
		winSetResid(winItem(handle,VIDEO_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,VIDEO_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,VIDEO_LOCK_ID),false);
*/
}
static void videoSDShow(winHandle handle)
{

	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,VIDEO_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,VIDEO_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,VIDEO_SD_ID),R_ID_ICON_MTSDCNORMAL);
}
static void videoLoopRecordShow(winHandle handle)
{

	if(configGet(CONFIG_ID_LOOPTIME) == R_ID_STR_TIM_1MIN)
		winSetResid(winItem(handle,VIDEO_LOOP_RECORD_ID),R_ID_ICON_MTLOOP1);
	else if(configGet(CONFIG_ID_LOOPTIME) == R_ID_STR_TIM_5MIN)
		winSetResid(winItem(handle,VIDEO_LOOP_RECORD_ID),R_ID_ICON_MTLOOP5);
	else if(configGet(CONFIG_ID_LOOPTIME) == R_ID_STR_TIM_10MIN)
		winSetResid(winItem(handle,VIDEO_LOOP_RECORD_ID),R_ID_ICON_MTLOOP10);
	
	if(configGet(CONFIG_ID_LOOPTIME) == R_ID_STR_COM_OFF)
		winSetVisible(winItem(handle,VIDEO_LOOP_RECORD_ID),false);
	else
		winSetVisible(winItem(handle,VIDEO_LOOP_RECORD_ID),true);
	drawUIService(true);
}
static void videoMicShow(winHandle handle)
{

/*
	if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,VIDEO_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,VIDEO_MIC_ID),R_ID_ICON_MTMICON);
*/
}

static void videoBaterryShow(winHandle handle)
{

	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
	    winSetResid(winItem(handle,VIDEO_BATERRY_ID),R_ID_ICON_MTBATTERY4); //llh
	winSetVisible(winItem(handle,VIDEO_BATERRY_ID),true);
}

#if 0 //SMALL_PANEL_SUPPORT==0
static void videoPoweOnTimeShow(winHandle handle,uint32 sec)
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
	winSetResid(winItem(handle,VIDEO_POWERON_TIME_ID),strToResId(powerOnTimeStr));
*/
}
#endif

static void videoRemainTimeShow(winHandle handle,uint32 sec)
{
	static char recTimeStr[]="00:00:00";
	recTimeStr[0] = sec/36000+'0';  // h
	recTimeStr[1] = ((sec/3600)%10)+'0';   // h
	recTimeStr[2] = ':';
	sec=sec%3600;
	recTimeStr[3] = sec/600+'0';  // m
	recTimeStr[4] = ((sec/60)%10)+'0';   // m
	recTimeStr[5] = ':';
	sec=sec%60;
	recTimeStr[6] = sec/10+'0';  // s
	recTimeStr[7] = sec%10+'0';   // s
	recTimeStr[8] = 0;
	winSetStrInfor(winItem(handle,VIDEO_REC_TIME_ID),0,ALIGNMENT_LEFT,R_COLOR_WHITE);
	winSetResid(winItem(handle,VIDEO_REC_TIME_ID),strToResId(recTimeStr));
}

static void videoRecTimeShow(winHandle handle,uint32 sec)
{
	static char recTimeStr[]="00:00";
	sec=sec%3600;
	recTimeStr[0] = sec/600+'0';  // m
	recTimeStr[1] = ((sec/60)%10)+'0';   // m
	recTimeStr[2] = ':';
	sec=sec%60;
	recTimeStr[3] = sec/10+'0';  // s
	recTimeStr[4] = sec%10+'0';   // s
	recTimeStr[5] = 0;
	winSetStrInfor(winItem(handle,VIDEO_REC_TIME_ID),0,ALIGNMENT_LEFT,R_COLOR_RED);
	winSetResid(winItem(handle,VIDEO_REC_TIME_ID),strToResId(recTimeStr));
}

static void videoTestModeShow(winHandle handle,int show)
{
	if(show == 1)
	{
		winSetResid(winItem(handle,VIDEO_TESTMODE_ID),strToResId("TEST ON"));
		winSetVisible(winItem(handle,VIDEO_TESTMODE_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,VIDEO_TESTMODE_ID),false);
	}
}


