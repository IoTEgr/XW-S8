#include"uiWin.h"

enum
{
	PHOTO_ICON_ID=0,
	PHOTO_HIDE_RECT_ID,
	PHOTO_BOTTOM_RECT_ID,
	PHOTO_PIC_REMAIN_ID,
	PHOTO_RESOLUTION_ID,
	PHOTO_MD_ID,// motion detect
	PHOTO_MONITOR_ID, // parking monitoring
	PHOTO_IRLED_ID,
	//PHOTO_LOCK_ID,
	PHOTO_SD_ID,
	PHOTO_MIC_ID,
	PHOTO_BATERRY_ID,
	PHOTO_POWERON_TIME_ID,
	PHOTO_SYSTIME_ID,
	PHOTO_TIMEPHOTO_ID,
	PHOTO_MOREPHOTO_ID,
	PHOTO_MODE_ID,
	PHOTO_TIMENUM_ID,
	PHOTO_PRINTER_ID,
	PHOTO_PRINTER_NOW_ID,
	PHOTO_PRINTER_DLY_ID,
	//==tips==
	PHOTO_PRINTER_TIPS_BK_ID,
	//PHOTO_TAKING_TIPS_BK_ID,
	PHOTO_PRINTER_TIPS_HEAD_ID,
	//PHOTO_TAKING_TIPS_HEAD_ID,
	PHOTO_PRINTER_TIPS_STR_ID,
	//PHOTO_TAKING_TIPS_STR_ID,
	PHOTO_ZOOM_RATIO_ID,
	PHOTO_FOCUS_ID,
	PHOTO_SURPLUS_QUANTITY_ID,//SurplusQuantity
	PHOTO_IR_ID,
	TEST_VOTAGE_ID,
	PHOTO_PRINTER_TIPS_CHARGING_NPRINT_ID,
	PHOTO_PRINTER_TIPS_PLZ_CHARG,
	//PHOTO_TAKING_TIPS_CHARGING_NPRINT_ID,
	PHOTO_MAX_ID
};

enum
{
	PHOTO_FOCUS_ICON_NONE = 0,
	PHOTO_FOCUS_ICON_YELLOW,
	PHOTO_FOCUS_ICON_RED
};



static widgetCreateInfor photoEncodeWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	//createRect(PHOTO_HIDE_RECT_ID,              Rx(0),Ry(0), Rw(320),Rh(25),R_COLOR_TBLACK),
//	createRect(PHOTO_HIDE_RECT_ID, Rx(0),Ry(0),  Rw(320),Rh(32),R_COLOR_BLACK),//YTS ºÚ¿ò
//	createRect(PHOTO_BOTTOM_RECT_ID,	   Rx(0),Ry(215),  Rw(320),Rh(25),R_COLOR_BLACK),//YTS ºÚ¿ò
	createImageIcon(PHOTO_ICON_ID,           Rx(2), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTPHOTO,ALIGNMENT_LEFT),
//	createStringIcon(PHOTO_PIC_REMAIN_ID,      Rx(25), Ry(0), Rw(75), Rh(25),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PHOTO_RESOLUTION_ID,Rx(320-96), Ry(0), Rw(32), Rh(32),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createImageIcon(PHOTO_TIMEPHOTO_ID,Rx(2+64), Ry(0), Rw(32), Rh(32),R_ID_ICON_MTPHOTOTIME3,ALIGNMENT_CENTER),
//	createImageIcon(PHOTO_MOREPHOTO_ID,Rx(164), Ry(0), Rw(20), Rh(25),R_ID_ICON_MTPHOTO_3,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_TIMENUM_ID,Rx((320-64)/2),Ry((240-64)/2), Rw(64), Rh(64),R_ID_ICON_NUM1,ALIGNMENT_CENTER),
//	createStringIcon(PHOTO_PRINTER_NOW_ID,           Rx(132), Ry(120), Rw(100), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
//	createImageIcon(PHOTO_MD_ID,                Rx(130), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMOTION,ALIGNMENT_CENTER),
//	createImageIcon(PHOTO_MONITOR_ID,     Rx(150), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTPARKOFF,ALIGNMENT_CENTER),
//	createImageIcon(PHOTO_IRLED_ID,             Rx(170),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	//createImageIcon(PHOTO_LOCK_ID,              Rx(190),Ry(0), Rw(20), Rh(25), R_ID_ICON_MTLOCK,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_PRINTER_ID,	Rx(320-64), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTPRINTER,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_SD_ID, Rx(2+32), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTSDCNORMAL,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_MODE_ID, Rx(2+96), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTPHOTO3,ALIGNMENT_CENTER),
//	createImageIcon(PHOTO_MIC_ID,                Rx(230), Ry(0), Rw(20), Rh(25), R_ID_ICON_MTMICOFF,ALIGNMENT_CENTER),
	createImageIcon(PHOTO_BATERRY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
//	createStringIcon(PHOTO_POWERON_TIME_ID,Rx(270), Ry(0), Rw(45), Rh(25),strToResId(" "),ALIGNMENT_RIGHT, R_COLOR_WHddITE,0),

	createStringIcon(PHOTO_SYSTIME_ID, Rx(5), Ry(215), Rw(220), Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_YELLOW,0),
//===tips===
	createRect(PHOTO_PRINTER_TIPS_BK_ID,Rx(70),Ry(50),Rw(180),Rh(140),R_COLOR_TBLACK),
	
	createStringIcon(PHOTO_PRINTER_TIPS_PLZ_CHARG,Rx(70),Ry(142), Rw(180),Rh(32),R_ID_STR_TIPS_PRINTING,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createRect(PHOTO_TAKING_TIPS_BK_ID,Rx(70),Ry(75),Rw(180),Rh(40),R_COLOR_BLUE1),
	
	createStringIcon(PHOTO_PRINTER_TIPS_HEAD_ID,Rx(70),Ry(50), Rw(180),Rh(45),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PHOTO_PRINTER_TIPS_STR_ID,Rx(70),Ry(95), Rw(180),Rh(95),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(PHOTO_PRINTER_TIPS_CHARGING_NPRINT_ID,Rx(70),Ry(95), Rw(180),Rh(95),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	
	//createStringIcon(PHOTO_TAKING_TIPS_HEAD_ID,Rx(70),Ry(50), Rw(180),Rh(30),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createStringIcon(PHOTO_TAKING_TIPS_STR_ID,Rx(70),Ry(80), Rw(180),Rh(30),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createStringIcon(PHOTO_TAKING_TIPS_CHARGING_NPRINT_ID,Rx(70),Ry(80), Rw(180),Rh(30),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),


	createImageIcon(PHOTO_FOCUS_ID,Rx(60), Ry(45), Rw(200), Rh(144), R_ID_ICON_MTPHOTOFOCUSYELLOW,ALIGNMENT_CENTER),
	createStringIcon(PHOTO_ZOOM_RATIO_ID, Rx(320-160), Ry(0), Rw(58), Rh(32),strToResId("01X"),ALIGNMENT_CENTER, R_COLOR_YELLOW,0),
	createStringIcon(PHOTO_SURPLUS_QUANTITY_ID, Rx(320-60), Ry(32), Rw(68), Rh(24),strToResId("000000"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createImageIcon(PHOTO_IR_ID,Rx(120), Ry(0), Rw(32), Rh(32), R_ID_ICON_MTIROFF,ALIGNMENT_CENTER),
	//createStringIcon(TEST_VOTAGE_ID,        Rx(50), Ry(100), Rw(220), Rh(50),strToResId("0"),ALIGNMENT_LEFT, R_COLOR_RED,2),
	createImageIcon(PHOTO_PRINTER_DLY_ID,	  Rx(110), Ry(70), Rw(100), Rh(100), R_ICON_MAX,ALIGNMENT_CENTER),
	widgetEnd(),
};


#if 0
static void photoSYSTIME_ID(winHandle handle)
{	
	//static char cameraid[10]="DSJ-0000";	
	//strncpy(&cameraid[4],getCameraName(),4);
	//cameraid[8] = 0;
	//if(configGet(CONFIG_CAMERA_NAME_ONOFF)==R_ID_STR_COM_ON)
	//winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
	//else
	winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(" "));

}
#endif

static void photoIRShow(winHandle handle)
{

/*	u32 val=0;
	boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,&val);
	if(val)
		winSetResid(winItem(handle,PHOTO_IR_ID),R_ID_ICON_MTIRON);
	else
		winSetResid(winItem(handle,PHOTO_IR_ID),R_ID_ICON_MTIROFF);   */
}
static void photoFocusShow(winHandle handle)
{
	if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_NONE){
		winSetVisible(winItem(handle,PHOTO_FOCUS_ID),false);
	}
	else{
		
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			winSetResid(winItem(handle,PHOTO_FOCUS_ID),R_ID_ICON_MTPHOTOFOCUSYELLOW);
			
			winSetVisible(winItem(handle,PHOTO_FOCUS_ID),true);
			
			}
		else if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_RED){
			winSetResid(winItem(handle,PHOTO_FOCUS_ID),R_ID_ICON_MTPHOTOFOCUSRED);
		winSetVisible(winItem(handle,PHOTO_FOCUS_ID),true);
		}else
		winSetVisible(winItem(handle,PHOTO_FOCUS_ID),false);
	}
}



static void photoZoomRatioShow(winHandle handle)
{
	static char TimeStr[4];
	u8 level = SysCtrl.crop_level + 1;
	TimeStr[0]=level/10%10 + '0';
	//TimeStr[1]='.';
	TimeStr[1]=level%10 + '0';
	TimeStr[2]='X';
	TimeStr[3]=0;
	winSetResid(winItem(handle,PHOTO_ZOOM_RATIO_ID),strToResId(TimeStr));
	winSetVisible(winItem(handle,PHOTO_ZOOM_RATIO_ID),true);
	//drawUIService(true);
}

static u32 getResSize()
{
	u32 res = (configValue2Int(CONFIG_ID_PRESLUTION)>>16)*(configValue2Int(CONFIG_ID_PRESLUTION)&0xffff);
	if(SDC_STAT_NORMAL != SysCtrl.sdcard)
		return 0xffffffff;
	if(res <= 921600)
		return 120;
	else if(res <= 3686400)
		return 260;
	else if(res <= 5760000)
		return 340;
	else if(res <= 8208000)
		return 400;
	else
		return 500;
}

#if ENABLE_FLASH_PHOTO
extern INT32U picSum;
#endif

static void photoSurplusQuantityShow(winHandle handle)
{
	u32 quantity;
#if ENABLE_FLASH_PHOTO
	if(SysCtrl.sdcard == SDC_STAT_NULL)
	{
		if(picSum>0){
			quantity = picSum;
		}else{
			quantity = 0;
		}
	}
	else
#endif
	{
		if(SysCtrl.sd_freesize <= REDUCE_2_5M)
			quantity = 0;
		else
			quantity = (SysCtrl.sd_freesize-REDUCE_2_5M)/getResSize();
		
		if(SDC_STAT_NORMAL != SysCtrl.sdcard)
			quantity = 0;
	}
	char TimeStr[10];
	memset(TimeStr,'0',sizeof(TimeStr));
	
	deg("show q[%d] freesize[%d] resSize[%d] pic_num[%d]\n",quantity,SysCtrl.sd_freesize,getResSize(),picSum);
	if(quantity)
	{
		TimeStr[0]=quantity/100000%10 + '0';
		TimeStr[1]=quantity/10000%10 + '0';
		TimeStr[2]=quantity/1000%10 + '0';
		TimeStr[3]=quantity/100%10 + '0';
		TimeStr[4]=quantity/10%10 + '0';
		TimeStr[5]=quantity%10+ '0'; //29926
		TimeStr[6]= 0;
		deg("[%s]\n",TimeStr);
		winSetResid(winItem(handle,PHOTO_SURPLUS_QUANTITY_ID),strToResId(TimeStr));
	}else
		winSetResid(winItem(handle,PHOTO_SURPLUS_QUANTITY_ID),strToResId("000000"));

	winSetVisible(winItem(handle,PHOTO_SURPLUS_QUANTITY_ID),true);
	drawUIService(true);
}


static void photoResolutionShow(winHandle handle)
{

	if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_1M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("1M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_2M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("2M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_3M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("3M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_5M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("5M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_8M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("8M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_10M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("10M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_12M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("12M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_20M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("20M"));
	else if(configGet(CONFIG_ID_PRESLUTION)==R_ID_STR_RES_48M)
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("48M"));
	else 
		winSetResid(winItem(handle,PHOTO_RESOLUTION_ID),strToResId("???"));
}
static void photoMDShow(winHandle handle)
{

/*
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		winSetResid(winItem(handle,PHOTO_MD_ID),R_ID_ICON_MTMOTION);
		winSetVisible(winItem(handle,PHOTO_MD_ID),true);
	}
	else
		winSetVisible(winItem(handle,PHOTO_MD_ID),false);
*/
}
static void photoMonitorShow(winHandle handle)
{

/*
	if(configGet(CONFIG_ID_PARKMODE)==R_ID_STR_COM_ON)
		winSetResid(winItem(handle,PHOTO_MONITOR_ID),R_ID_ICON_MTPARKON);
	else
		winSetResid(winItem(handle,PHOTO_MONITOR_ID),R_ID_ICON_MTPARKOFF);
*/
}
static void photoIrLedShow(winHandle handle)
{
/*
#if (1 == IR_MENU_EN)
	if(configGet(CONFIG_ID_IR_LED)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PHOTO_IRLED_ID),R_ID_ICON_MTIROFF);
	else
		winSetResid(winItem(handle,PHOTO_IRLED_ID),R_ID_ICON_MTIRON);
#else
	winSetVisible(winItem(handle,PHOTO_IRLED_ID),false);
#endif
*/
}
static void photoLockShow(winHandle handle,bool show)
{

	/*if(show)
	{
		winSetResid(winItem(handle,PHOTO_LOCK_ID),R_ID_ICON_MTLOCK);
		winSetVisible(winItem(handle,PHOTO_LOCK_ID),true);
	}
	else
		winSetVisible(winItem(handle,PHOTO_LOCK_ID),false);*/
}
static void photoSDShow(winHandle handle)
{

	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNORMAL);
	else if(SysCtrl.sdcard == SDC_STAT_NULL)
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNULL);
	else
		winSetResid(winItem(handle,PHOTO_SD_ID),R_ID_ICON_MTSDCNORMAL);
}
static void photoMicShow(winHandle handle)
{

/*
	if(configGet(CONFIG_ID_AUDIOREC)==R_ID_STR_COM_OFF)
		winSetResid(winItem(handle,PHOTO_MIC_ID),R_ID_ICON_MTMICOFF);
	else
		winSetResid(winItem(handle,PHOTO_MIC_ID),R_ID_ICON_MTMICON);
*/
}

#if 0
static void photoPrintShow(winHandle handle)
{

//	winSetResid(winItem(handle,PHOTO_PRINTER_NOW_ID),strToResId(R_ID_STR_FIL_NULL));
}

static void photoPrintOff(winHandle handle)
{

/*
	deg_Printf("photoPrintOff  ***\n");
	winSetResid(winItem(handle,PHOTO_PRINTER_NOW_ID),strToResId(" "));
*/
}
#endif


static void photoBaterryShow(winHandle handle)
{

	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		if(SysCtrl.bat_state ==2)
		{
			winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
		}
		else
		{
			SysCtrl.bat_charge_idx++;
			if(SysCtrl.bat_charge_idx>3)
			{
				SysCtrl.bat_charge_idx=0;
			}
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
		}

	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,PHOTO_BATERRY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,PHOTO_BATERRY_ID),true);
}

static void photo_mode_show(winHandle handle)
{
	//deg_Printf("SysCtrl.photo_mode_idx=%d\n",SysCtrl.photo_mode_idx);

	if(0==SysCtrl.photo_mode_idx)
	{
		winSetVisible(winItem(handle,PHOTO_MODE_ID),false);
	}
	else
	{
		if(1==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTO3);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(2==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTO5);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(3==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME3);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(4==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME5);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(5==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME10);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(6==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOTIME5P3);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(7==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOSKETCH);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}
		else if(8==SysCtrl.photo_mode_idx)
		{
			winSetResid(winItem(handle,PHOTO_MODE_ID),R_ID_ICON_MTPHOTOSKETCHCOLOR);
			winSetVisible(winItem(handle,PHOTO_MODE_ID),true);
		}

	}

}


static void photo_timephoto(winHandle handle,bool show)
{

	if(true == show)
	{
		if(3 == configValue2Int(CONFIG_ID_TIMEPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_TIMEPHOTO_ID),R_ID_ICON_MTPHOTOTIME3);
			winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),true);
		}
		else if(5 == configValue2Int(CONFIG_ID_TIMEPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_TIMEPHOTO_ID),R_ID_ICON_MTPHOTOTIME5);
			winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),true);
		}
		else if(10 == configValue2Int(CONFIG_ID_TIMEPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_TIMEPHOTO_ID),R_ID_ICON_MTPHOTOTIME10);
			winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),true);
		}
		else
		{
			winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),false);
		}
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_TIMEPHOTO_ID),false);
	}
}

/*
static void photo_morephoto(winHandle handle,bool show)
{

	if(true==show)
	{
		if(3==configValue2Int(CONFIG_ID_MOREPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_MOREPHOTO_ID),R_ID_ICON_MTPHOTO_3);
			winSetVisible(winItem(handle,PHOTO_MOREPHOTO_ID),true);
		}
		else if(5==configValue2Int(CONFIG_ID_MOREPHOTO))
		{
			winSetResid(winItem(handle,PHOTO_MOREPHOTO_ID),R_ID_ICON_MTPHOTO_5);
			winSetVisible(winItem(handle,PHOTO_MOREPHOTO_ID),true);
		}
		else
		{
			winSetVisible(winItem(handle,PHOTO_MOREPHOTO_ID),false);
		}
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_MOREPHOTO_ID),false);
	}
}
*/

static void photo_time_num_show(winHandle handle,bool show,u8 num_idx)
{

	if(true == show)
	{
		u32 numres_id;
		switch(num_idx)
		{
			case 1: numres_id = R_ID_ICON_NUM1;
					break;
			case 2: numres_id = R_ID_ICON_NUM2;
					break;
			case 3: numres_id = R_ID_ICON_NUM3;
					break;
			case 4: numres_id = R_ID_ICON_NUM4;
					break;
			case 5: numres_id = R_ID_ICON_NUM5;
					break;
			case 6: numres_id = R_ID_ICON_NUM6;
					break;
			case 7: numres_id = R_ID_ICON_NUM7;
					break;
			case 8: numres_id = R_ID_ICON_NUM8;
					break;
			case 9: numres_id = R_ID_ICON_NUM9;
					break;
			case 10: numres_id = R_ID_ICON_NUM10;
					break;
			default:
				numres_id = R_ID_ICON_NUM1;
				show = false;
				break;
		}
		deg_Printf("timephoto=%d,numres_id=0x%x,%d\n",num_idx,numres_id,show);
		if(show)
		{
			winSetResid(winItem(handle,PHOTO_TIMENUM_ID),numres_id);
		}
		winSetVisible(winItem(handle,PHOTO_TIMENUM_ID),show);
		drawUIService(true);

		if(1==num_idx)	// last wait sound finish
		{
			system_audio_play(RES_MUSIC_PHOTO_TIME,1,1);
		}
		else
		{
			system_audio_play(RES_MUSIC_PHOTO_TIME,1,1);
		}
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_TIMENUM_ID),false);
		drawUIService(true);
	}
}

#if 0
static void photo_timePrint_num(winHandle handle,bool show)
{

/*
	if(true == show)
	{
		u32 numres_id;
		switch(timeprint)
		{
			case 1: numres_id = R_ID_ICON_1;
					break;
			case 2: numres_id = R_ID_ICON_2;
					break;
			case 3: numres_id = R_ID_ICON_3;
					break;
			
			defaut:
				numres_id = R_ID_ICON_NUM1;
				show = false;
				break;
		}
		deg_Printf("timephoto=%d,numres_id=0x%x,%d\n",timeprint,numres_id,show);
		winSetResid(winItem(handle,PHOTO_TIMENUM_ID),numres_id);
		winSetVisible(winItem(handle,PHOTO_TIMENUM_ID),show);
		drawUIService(true);
		if(timeprint<=3)
		{
			//logo_music_show(RES_MUSIC_TIMEPHOTO_FAST);
			if(1==timeprint)
			{
				system_audio_play(RES_MUSIC_TIMEPHOTO_FAST,1,1);
			}
			else
			{
				system_audio_play(RES_MUSIC_TIMEPHOTO_FAST,1,0);
			}
		}
		else
		{
			//logo_music_show(RES_MUSIC_TIMEPHOTO_SLOW);
			system_audio_play(RES_MUSIC_TIMEPHOTO_SLOW,1,0);
		}
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_TIMENUM_ID),false);
	}
*/
}
#endif

static void photoRemainNumShow(winHandle handle)
{
#if 0
	int picSum;
	static char picSumStr[5];
	picSum=managerFileCount(SysCtrl.jpg_list);
	if(picSum>=0)
	{
		intToString(&picSumStr,5,/*MANAGER_LIST_MAX-*/picSum);
		winSetResid(winItem(handle,PHOTO_PIC_REMAIN_ID),strToResId(picSumStr));
	}
	else
		winSetResid(winItem(handle,PHOTO_PIC_REMAIN_ID),strToResId("0"));
#else
//winSetResid(winItem(handle,PHOTO_PIC_REMAIN_ID),strToResId(" "));
#endif
}

static void photoPrinterShow(winHandle handle)
{

	if(SysCtrl.printer_en)
	{
		winSetResid(winItem(handle,PHOTO_PRINTER_ID),R_ID_ICON_MTPRINTER);
		winSetVisible(winItem(handle,PHOTO_PRINTER_ID),true);
	}
	else
		winSetVisible(winItem(handle,PHOTO_PRINTER_ID),false);
}



#if 0
static void photoPoweOnTimeShow(winHandle handle,uint32 sec)
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
	winSetResid(winItem(handle,PHOTO_POWERON_TIME_ID),strToResId(powerOnTimeStr));
*/
}
#endif


static void photoPrinterTipsShow(winHandle handle,bool show,u32 head_str_id,u32 tips_str_id)
{

	if(true==show)
	{
		/*if(tips_str_id == R_ID_STR_TIPS_TAKEING_PHOTO)
		{
			winSetVisible(winItem(handle,PHOTO_TAKING_TIPS_BK_ID),true);
			//winSetResid(winItem(handle,PHOTO_TAKING_TIPS_HEAD_ID),head_str_id);
			//winSetVisible(winItem(handle,PHOTO_TAKING_TIPS_HEAD_ID),true);
			winSetResid(winItem(handle,PHOTO_TAKING_TIPS_STR_ID),tips_str_id);
			
			winSetVisible(winItem(handle,PHOTO_TAKING_TIPS_STR_ID),true);
				
			winSetResid(winItem(handle,PHOTO_TAKING_TIPS_CHARGING_NPRINT_ID),tips_str_id);
			winSetVisible(winItem(handle,PHOTO_TAKING_TIPS_CHARGING_NPRINT_ID),true);

		}else 
		{*/
			winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_BK_ID),true);
			
			winSetResid(winItem(handle,PHOTO_PRINTER_TIPS_HEAD_ID),head_str_id);
			winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_HEAD_ID),true);
			
			winSetResid(winItem(handle,PHOTO_PRINTER_TIPS_STR_ID),tips_str_id);
			winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_STR_ID),true);
				
			winSetResid(winItem(handle,PHOTO_PRINTER_TIPS_CHARGING_NPRINT_ID),tips_str_id);
			winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_CHARGING_NPRINT_ID),true);

		if(tips_str_id==R_ID_STR_TIP_KEEPQ)
		{
			winSetResid(winItem(handle,PHOTO_PRINTER_TIPS_PLZ_CHARG),R_ID_STR_TIP_PLZ_CHARG);
			winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_PLZ_CHARG),true);

		}

		//}
		
	}
	else 
	{
		winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_BK_ID),false);
		winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_HEAD_ID),false);
		winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_STR_ID),false);
		winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_CHARGING_NPRINT_ID),false);
		winSetVisible(winItem(handle,PHOTO_PRINTER_TIPS_PLZ_CHARG),false);
		//winSetVisible(winItem(handle,PHOTO_TAKING_TIPS_BK_ID),false);
		//winSetVisible(winItem(handle,PHOTO_TAKING_TIPS_HEAD_ID),false);
		//winSetVisible(winItem(handle,PHOTO_TAKING_TIPS_STR_ID),false);
		//winSetVisible(winItem(handle,PHOTO_TAKING_TIPS_CHARGING_NPRINT_ID),false);

	}
}

static void photoPrintDlyShow(winHandle handle,bool show,u8 num_cnt)
{
	u32 numres_id;

	deg("photoPrintDlyShow \n");
	
	if(true==show)
	{
		switch(num_cnt)
		{
			case 1: numres_id = R_ID_ICON_MTPRTDLY1;
					break;
			case 2: numres_id = R_ID_ICON_MTPRTDLY2;
					break;
			case 3: numres_id = R_ID_ICON_MTPRTDLY3;
					break;
			
			default:
				numres_id = R_ID_ICON_MTPRTDLY3;
				show = false;
				break;
		}
		deg("num_cnt=%d \n",num_cnt);
		deg("numres_id=%d \n",numres_id);
		if(show)
		{
			winSetResid(winItem(handle,PHOTO_PRINTER_DLY_ID),numres_id);
		}
		winSetVisible(winItem(handle,PHOTO_PRINTER_DLY_ID),show);
		drawUIService(true);
		system_audio_play(RES_MUSIC_PHOTO_TIME,1,1);
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_PRINTER_DLY_ID),false);
	}
}






