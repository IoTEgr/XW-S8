#include"uiWin.h"

enum
{
	
	CYCLIC_RECORD_BATTERY_ID=0,
	CYCLIC_RECORD_SEL1_ID,
	CYCLIC_RECORD_SEL2_ID,
	CYCLIC_RECORD_SEL3_ID,
	CYCLIC_RECORD_SEL4_ID,
};

#define CYCLIC_RECORD_NUM			4
#define CYCLIC_RECORD_ITEM_NUM		4

static u32 cyclic_record_tab[CYCLIC_RECORD_NUM]={
	R_ID_STR_COM_OFF,
	R_ID_STR_TIM_1MIN,
	R_ID_STR_TIM_5MIN,
	R_ID_STR_TIM_10MIN
};


static widgetCreateInfor cyclicRecordWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createImageIcon(CYCLIC_RECORD_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringRim(CYCLIC_RECORD_SEL1_ID,Rx(0),Ry(100), Rw(320),Rh(25),R_ID_STR_COM_OFF,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(CYCLIC_RECORD_SEL2_ID,Rx(0),Ry(100+25), Rw(320),Rh(25),R_ID_STR_TIM_1MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(CYCLIC_RECORD_SEL3_ID,Rx(0),Ry(100+50), Rw(320),Rh(25),R_ID_STR_TIM_5MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(CYCLIC_RECORD_SEL4_ID,Rx(0),Ry(100+75), Rw(320),Rh(25),R_ID_STR_TIM_10MIN,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),

	widgetEnd(),
};


static void settingCyclicRecordBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,CYCLIC_RECORD_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,CYCLIC_RECORD_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,CYCLIC_RECORD_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,CYCLIC_RECORD_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,CYCLIC_RECORD_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,CYCLIC_RECORD_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,CYCLIC_RECORD_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,CYCLIC_RECORD_BATTERY_ID),true);
}


static void settingCyclicRecordSelShow(winHandle handle,u8 sel_idx)
{
	u8 page,idx,i;
	if(sel_idx>=CYCLIC_RECORD_NUM)
	{
		deg_Printf("err sel:%d\n",sel_idx);
		return;
	}

	page=sel_idx/CYCLIC_RECORD_NUM;
	idx=sel_idx%CYCLIC_RECORD_NUM;
	
	deg_Printf("sel:%d\n",sel_idx);
	winSetStrInfor(winItem(handle,CYCLIC_RECORD_SEL1_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,CYCLIC_RECORD_SEL2_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,CYCLIC_RECORD_SEL3_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,CYCLIC_RECORD_SEL4_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);

	winSetVisible(winItem(handle,CYCLIC_RECORD_SEL1_ID),false);
	winSetVisible(winItem(handle,CYCLIC_RECORD_SEL2_ID),false);
	winSetVisible(winItem(handle,CYCLIC_RECORD_SEL3_ID),false);
	winSetVisible(winItem(handle,CYCLIC_RECORD_SEL4_ID),false);

	for(i=0;i<CYCLIC_RECORD_NUM-page*CYCLIC_RECORD_ITEM_NUM;i++)
	{
		winSetResid(winItem(handle,CYCLIC_RECORD_SEL1_ID+i),cyclic_record_tab[page*CYCLIC_RECORD_ITEM_NUM+i]);
		winSetVisible(winItem(handle,CYCLIC_RECORD_SEL1_ID+i),true);
	}

	winSetStrInfor(winItem(handle,CYCLIC_RECORD_SEL1_ID+idx),0,ALIGNMENT_CENTER,R_COLOR_RED);

}



