#include "application.h"

extern menu MENU(movie);

void taskSettingMenuOpen(uint32 arg)
{
	deg_Printf("setting menu open\n");	
	
	
	hal_osdSetflag(1);
	dispLayerUninit(DISP_LAYER_OSD0);
	XOSTimeDly(20);
	dispLayerInit(DISP_LAYER_OSD0);
	

	hal_csiEnable(0);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);
	uiOpenWindow(&menuItemWindow,1,&MENU(movie)); 
}

void taskSettingMenuService(uint32 arg)
{

}
void taskSettingMenuClose(uint32 arg)
{
	
	hal_osdSetflag(0);
	dispLayerUninit(DISP_LAYER_OSD0);
	XOSTimeDly(20);
	dispLayerInit(DISP_LAYER_OSD0);
	
	//yuv_rgb_table_uninit();
	deg_Printf("setting menu exit.\n");
}
sysTask taskSettingMenu=
{
	"setting menu",
	(u32)NULL,
	taskSettingMenuOpen,
	taskSettingMenuClose,
	taskSettingMenuService,
};


