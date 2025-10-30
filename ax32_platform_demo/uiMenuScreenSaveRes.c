#include"uiWin.h"

enum
{
	
	SCREEN_SAVE_BATTERY_ID=0,
	SCREEN_SAVE_SEL1_ID,
	SCREEN_SAVE_SEL2_ID,
	SCREEN_SAVE_SEL3_ID,
	SCREEN_SAVE_SEL4_ID,
};

#define SCREEN_SAVE_NUM			4
#define SCREEN_SAVE_ITEM_NUM		4

static u32 screen_save_tab[SCREEN_SAVE_NUM]={
	R_ID_STR_COM_OFF,
	R_ID_STR_TIM_1MIN,
	R_ID_STR_TIM_3MIN,
	R_ID_STR_TIM_5MIN
};


static widgetCreateInfor screenSaveWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createImageIcon(SCREEN_SAVE_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringRim(SCREEN_SAVE_SEL1_ID,Rx(0),Ry(100), Rw(320),Rh(25),R_ID_STR_COM_OFF,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(SCREEN_SAVE_SEL2_ID,Rx(0),Ry(100+25), Rw(320),Rh(25),R_ID_STR_TIM_1MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(SCREEN_SAVE_SEL3_ID,Rx(0),Ry(100+50), Rw(320),Rh(25),R_ID_STR_TIM_3MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(SCREEN_SAVE_SEL4_ID,Rx(0),Ry(100+75), Rw(320),Rh(25),R_ID_STR_TIM_5MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),

	widgetEnd(),
};


static void settingScreenSaveBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,SCREEN_SAVE_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,SCREEN_SAVE_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,SCREEN_SAVE_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,SCREEN_SAVE_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,SCREEN_SAVE_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,SCREEN_SAVE_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,SCREEN_SAVE_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,SCREEN_SAVE_BATTERY_ID),true);
}


static void settingScreenSaveSelShow(winHandle handle,u8 sel_idx)
{
	u8 page,idx,i;
	if(sel_idx>=SCREEN_SAVE_NUM)
	{
		deg_Printf("err sel:%d\n",sel_idx);
		return;
	}

	page=sel_idx/SCREEN_SAVE_NUM;
	idx=sel_idx%SCREEN_SAVE_NUM;
	
	deg_Printf("sel:%d\n",sel_idx);
	winSetStrInfor(winItem(handle,SCREEN_SAVE_SEL1_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,SCREEN_SAVE_SEL2_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,SCREEN_SAVE_SEL3_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,SCREEN_SAVE_SEL4_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);

	winSetVisible(winItem(handle,SCREEN_SAVE_SEL1_ID),false);
	winSetVisible(winItem(handle,SCREEN_SAVE_SEL2_ID),false);
	winSetVisible(winItem(handle,SCREEN_SAVE_SEL3_ID),false);
	winSetVisible(winItem(handle,SCREEN_SAVE_SEL4_ID),false);

	for(i=0;i<SCREEN_SAVE_NUM-page*SCREEN_SAVE_ITEM_NUM;i++)
	{
		winSetResid(winItem(handle,SCREEN_SAVE_SEL1_ID+i),screen_save_tab[page*SCREEN_SAVE_ITEM_NUM+i]);
		winSetVisible(winItem(handle,SCREEN_SAVE_SEL1_ID+i),true);
	}

	winSetStrInfor(winItem(handle,SCREEN_SAVE_SEL1_ID+idx),0,ALIGNMENT_CENTER,R_COLOR_RED);

}



