#include"uiWin.h"

enum
{
	
	AUTO_POWER_OFF_BATTERY_ID=0,
	AUTO_POWER_OFF_SEL1_ID,
	AUTO_POWER_OFF_SEL2_ID,
	AUTO_POWER_OFF_SEL3_ID,
	AUTO_POWER_OFF_SEL4_ID,
};

#define AUTO_POWER_OFF_NUM				4
#define AUTO_POWER_OFF_ITEM_NUM		4

static u32 auto_power_off_tab[AUTO_POWER_OFF_NUM]={
	R_ID_STR_COM_OFF,
	R_ID_STR_TIM_1MIN,
	R_ID_STR_TIM_3MIN,
	R_ID_STR_TIM_5MIN
};


static widgetCreateInfor autoPowerOffWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createImageIcon(AUTO_POWER_OFF_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringRim(AUTO_POWER_OFF_SEL1_ID,Rx(0),Ry(100), Rw(320),Rh(25),R_ID_STR_COM_OFF,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(AUTO_POWER_OFF_SEL2_ID,Rx(0),Ry(100+25), Rw(320),Rh(25),R_ID_STR_TIM_1MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(AUTO_POWER_OFF_SEL3_ID,Rx(0),Ry(100+50), Rw(320),Rh(25),R_ID_STR_TIM_3MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(AUTO_POWER_OFF_SEL4_ID,Rx(0),Ry(100+75), Rw(320),Rh(25),R_ID_STR_TIM_5MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),

	widgetEnd(),
};


static void settingAutoPowerOffBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,AUTO_POWER_OFF_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,AUTO_POWER_OFF_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,AUTO_POWER_OFF_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,AUTO_POWER_OFF_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,AUTO_POWER_OFF_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,AUTO_POWER_OFF_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,AUTO_POWER_OFF_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,AUTO_POWER_OFF_BATTERY_ID),true);
}


static void settingAutoPowerOffSelShow(winHandle handle,u8 sel_idx)
{
	u8 page,idx,i;
	if(sel_idx>=AUTO_POWER_OFF_NUM)
	{
		deg_Printf("err sel:%d\n",sel_idx);
		return;
	}

	page=sel_idx/AUTO_POWER_OFF_NUM;
	idx=sel_idx%AUTO_POWER_OFF_NUM;
	
	deg_Printf("sel:%d\n",sel_idx);
	winSetStrInfor(winItem(handle,AUTO_POWER_OFF_SEL1_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,AUTO_POWER_OFF_SEL2_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,AUTO_POWER_OFF_SEL3_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,AUTO_POWER_OFF_SEL4_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);

	winSetVisible(winItem(handle,AUTO_POWER_OFF_SEL1_ID),false);
	winSetVisible(winItem(handle,AUTO_POWER_OFF_SEL2_ID),false);
	winSetVisible(winItem(handle,AUTO_POWER_OFF_SEL3_ID),false);
	winSetVisible(winItem(handle,AUTO_POWER_OFF_SEL4_ID),false);

	for(i=0;i<AUTO_POWER_OFF_NUM-page*AUTO_POWER_OFF_ITEM_NUM;i++)
	{
		winSetResid(winItem(handle,AUTO_POWER_OFF_SEL1_ID+i),auto_power_off_tab[page*AUTO_POWER_OFF_ITEM_NUM+i]);
		winSetVisible(winItem(handle,AUTO_POWER_OFF_SEL1_ID+i),true);
	}

	winSetStrInfor(winItem(handle,AUTO_POWER_OFF_SEL1_ID+idx),0,ALIGNMENT_CENTER,R_COLOR_RED);

}



