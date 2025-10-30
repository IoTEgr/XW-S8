#include"uiWin.h"

enum
{
	
	PRINT_DENSITY_BATTERY_ID=0,
	PRINT_DENSITY_SEL1_ID,
	PRINT_DENSITY_SEL2_ID,
	PRINT_DENSITY_SEL3_ID,
	PRINT_DENSITY_SEL4_ID,
};

#define PRINT_DENSITY_NUM				5
#define PRINT_DENSITY_ITEM_NUM		4


static widgetCreateInfor printDensityWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createImageIcon(PRINT_DENSITY_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringRim(PRINT_DENSITY_SEL1_ID,Rx(0),Ry(100), Rw(320),Rh(25),R_ID_STR_COM_LEVEL_0,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(PRINT_DENSITY_SEL2_ID,Rx(0),Ry(100+25), Rw(320),Rh(25),R_ID_STR_COM_LEVEL_1,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(PRINT_DENSITY_SEL3_ID,Rx(0),Ry(100+50), Rw(320),Rh(25),R_ID_STR_COM_LEVEL_2,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(PRINT_DENSITY_SEL4_ID,Rx(0),Ry(100+75), Rw(320),Rh(25),R_ID_STR_COM_LEVEL_3,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),

	widgetEnd(),
};


static void settingPrintDensityBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,PRINT_DENSITY_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,PRINT_DENSITY_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,PRINT_DENSITY_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,PRINT_DENSITY_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,PRINT_DENSITY_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,PRINT_DENSITY_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,PRINT_DENSITY_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,PRINT_DENSITY_BATTERY_ID),true);
}

static void settingPrintDensitySelShow(winHandle handle,u8 sel_idx)
{
	u8 page,idx,i;
	if(sel_idx>=PRINT_DENSITY_NUM)
	{
		deg_Printf("err sel:%d\n",sel_idx);
		return;
	}

	page=sel_idx/PRINT_DENSITY_ITEM_NUM;
	idx=sel_idx%PRINT_DENSITY_ITEM_NUM;
	
	deg_Printf("sel:%d\n",sel_idx);
	winSetStrInfor(winItem(handle,PRINT_DENSITY_SEL1_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,PRINT_DENSITY_SEL2_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,PRINT_DENSITY_SEL3_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,PRINT_DENSITY_SEL4_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);

	winSetVisible(winItem(handle,PRINT_DENSITY_SEL1_ID),false);
	winSetVisible(winItem(handle,PRINT_DENSITY_SEL2_ID),false);
	winSetVisible(winItem(handle,PRINT_DENSITY_SEL3_ID),false);
	winSetVisible(winItem(handle,PRINT_DENSITY_SEL4_ID),false);

	for(i=0;i<PRINT_DENSITY_NUM-page*PRINT_DENSITY_ITEM_NUM;i++)
	{
		winSetResid(winItem(handle,PRINT_DENSITY_SEL1_ID+i),R_ID_STR_COM_LEVEL_0+page*PRINT_DENSITY_ITEM_NUM+i);
		winSetVisible(winItem(handle,PRINT_DENSITY_SEL1_ID+i),true);
	}

	winSetStrInfor(winItem(handle,PRINT_DENSITY_SEL1_ID+idx),0,ALIGNMENT_CENTER,R_COLOR_RED);
}



