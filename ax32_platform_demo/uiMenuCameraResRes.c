#include"uiWin.h"

enum
{
	
	CAMERA_RES_BATTERY_ID=0,
	CAMERA_RES_SEL1_ID,
	CAMERA_RES_SEL2_ID,
	CAMERA_RES_SEL3_ID,
	CAMERA_RES_SEL4_ID,
};

#define CAMERA_RES_NUM				5//7//
#define CAMERA_RES_ITEM_NUM		4

static u32 camera_res_tab[CAMERA_RES_NUM]={
	R_ID_STR_RES_1M,
	R_ID_STR_RES_3M,
	R_ID_STR_RES_5M,
	R_ID_STR_RES_8M,
	R_ID_STR_RES_12M,
	//R_ID_STR_RES_20M,
	//R_ID_STR_RES_40M
};


static widgetCreateInfor cameraResWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	createImageIcon(CAMERA_RES_BATTERY_ID,Rx(320-33), Ry(0), Rw(33), Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringRim(CAMERA_RES_SEL1_ID,Rx(0),Ry(100), Rw(320),Rh(25),R_ID_STR_RES_1M,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(CAMERA_RES_SEL2_ID,Rx(0),Ry(100+25), Rw(320),Rh(25),R_ID_STR_RES_2M,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(CAMERA_RES_SEL3_ID,Rx(0),Ry(100+50), Rw(320),Rh(25),R_ID_STR_RES_5M,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),
	createStringRim(CAMERA_RES_SEL4_ID,Rx(0),Ry(100+75), Rw(320),Rh(25),R_ID_STR_RES_8M,ALIGNMENT_CENTER,R_COLOR_WHITE,0,R_COLOR_BLACK),

	widgetEnd(),
};


static void settingCameraResBaterryShow(winHandle handle)
{
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,CAMERA_RES_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,CAMERA_RES_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,CAMERA_RES_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,CAMERA_RES_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,CAMERA_RES_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,CAMERA_RES_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,CAMERA_RES_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,CAMERA_RES_BATTERY_ID),true);
}


static void settingCameraResSelShow(winHandle handle,u8 sel_idx)
{
	u8 page,idx,i,item;
	if(sel_idx>=CAMERA_RES_NUM)
	{
		deg_Printf("err sel:%d\n",sel_idx);
		return;
	}

	if(sel_idx >= ((CAMERA_RES_NUM/CAMERA_RES_ITEM_NUM)*CAMERA_RES_ITEM_NUM))
		item = CAMERA_RES_NUM - ((CAMERA_RES_NUM/CAMERA_RES_ITEM_NUM)*CAMERA_RES_ITEM_NUM);
	else
		item = CAMERA_RES_ITEM_NUM;

	page=sel_idx/CAMERA_RES_ITEM_NUM;
	idx=sel_idx%CAMERA_RES_ITEM_NUM;
	
	deg_Printf("sel:%d\n",sel_idx);
	winSetStrInfor(winItem(handle,CAMERA_RES_SEL1_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,CAMERA_RES_SEL2_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,CAMERA_RES_SEL3_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	winSetStrInfor(winItem(handle,CAMERA_RES_SEL4_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);

	winSetVisible(winItem(handle,CAMERA_RES_SEL1_ID),false);
	winSetVisible(winItem(handle,CAMERA_RES_SEL2_ID),false);
	winSetVisible(winItem(handle,CAMERA_RES_SEL3_ID),false);
	winSetVisible(winItem(handle,CAMERA_RES_SEL4_ID),false);

	for(i=0;i<item;i++)
	{
		winSetResid(winItem(handle,CAMERA_RES_SEL1_ID+i),camera_res_tab[page*CAMERA_RES_ITEM_NUM+i]);
		winSetVisible(winItem(handle,CAMERA_RES_SEL1_ID+i),true);
	}

	winSetStrInfor(winItem(handle,CAMERA_RES_SEL1_ID+idx),0,ALIGNMENT_CENTER,R_COLOR_RED);

}



