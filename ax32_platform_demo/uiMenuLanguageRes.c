#include"uiWin.h"

enum
{
	
	LANGUAGE_BATTERY_ID=0,
	LANGUAGE_SEL1_ID,
	LANGUAGE_SEL2_ID,
	LANGUAGE_SEL3_ID,
	LANGUAGE_SEL4_ID,
};

#define LANGUAGE_NUM			15
#define LANGUAGE_ITEM_NUM		4

static u32 language_tab[LANGUAGE_NUM]={
	R_ID_STR_LAN_ENGLISH,
	R_ID_STR_LAN_SCHINESE,
	R_ID_STR_LAN_TCHINESE,
	R_ID_STR_LAN_JAPANESE,
	R_ID_STR_LAN_KOERA,
	R_ID_STR_LAN_RUSSIAN,
	R_ID_STR_LAN_FRECH,
	R_ID_STR_LAN_GERMAN,
	R_ID_STR_LAN_CZECH,
	R_ID_STR_LAN_PORTUGUESE,	// 10
	R_ID_STR_LAN_POLISH,
	R_ID_STR_LAN_DUTCH,
	R_ID_STR_LAN_TURKEY,
	R_ID_STR_LAN_TAI,
	R_ID_STR_LAN_SPANISH
};


static widgetCreateInfor languageWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createImageIcon(LANGUAGE_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringRim(LANGUAGE_SEL1_ID,Rx(0),Ry(100), Rw(320),Rh(25),R_ID_STR_LAN_ENGLISH,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(LANGUAGE_SEL2_ID,Rx(0),Ry(100+25), Rw(320),Rh(25),R_ID_STR_LAN_SCHINESE,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(LANGUAGE_SEL3_ID,Rx(0),Ry(100+50), Rw(320),Rh(25),R_ID_STR_LAN_TCHINESE,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(LANGUAGE_SEL4_ID,Rx(0),Ry(100+75), Rw(320),Rh(25),R_ID_STR_LAN_JAPANESE,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),

	widgetEnd(),
};


static void settingLanguageBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,LANGUAGE_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,LANGUAGE_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,LANGUAGE_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,LANGUAGE_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,LANGUAGE_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,LANGUAGE_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,LANGUAGE_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,LANGUAGE_BATTERY_ID),true);
}

static void settingLanguageSelShow(winHandle handle,u8 sel_idx)
{
	u8 page,idx,i;
	if(sel_idx>=LANGUAGE_NUM)
	{
		deg_Printf("err sel:%d\n",sel_idx);
		return;
	}

	page=sel_idx/LANGUAGE_ITEM_NUM;
	idx=sel_idx%LANGUAGE_ITEM_NUM;
	
	deg_Printf("sel:%d\n",sel_idx);
	winSetStrInfor(winItem(handle,LANGUAGE_SEL1_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,LANGUAGE_SEL2_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,LANGUAGE_SEL3_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,LANGUAGE_SEL4_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);

	winSetVisible(winItem(handle,LANGUAGE_SEL1_ID),false);
	winSetVisible(winItem(handle,LANGUAGE_SEL2_ID),false);
	winSetVisible(winItem(handle,LANGUAGE_SEL3_ID),false);
	winSetVisible(winItem(handle,LANGUAGE_SEL4_ID),false);

	for(i=0;i<LANGUAGE_NUM-page*LANGUAGE_ITEM_NUM;i++)
	{
		winSetResid(winItem(handle,LANGUAGE_SEL1_ID+i),language_tab[page*LANGUAGE_ITEM_NUM+i]);
		winSetVisible(winItem(handle,LANGUAGE_SEL1_ID+i),true);
	}

	winSetStrInfor(winItem(handle,LANGUAGE_SEL1_ID+idx),0,ALIGNMENT_CENTER,R_COLOR_RED);
}



