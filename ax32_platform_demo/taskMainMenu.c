#include "application.h"


void taskMainMenuOpen(uint32 arg)
{
	deg_Printf("main menu open\n");

	hal_csiEnable(0);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);
	
	uiOpenWindow(&mainMenuWindow,0);
}

void taskMainMenuService(uint32 arg)
{

}
void taskMainMenuClose(uint32 arg)
{

	deg_Printf("main menu exit.\n");
}
sysTask taskMainMenu=
{
	"Main menu",
	(u32)NULL,
	taskMainMenuOpen,
	taskMainMenuClose,
	taskMainMenuService,
};


