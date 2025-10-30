#include "application.h"


void taskGameMenuOpen(uint32 arg)
{
	deg_Printf("game menu open\n");

	hal_csiEnable(0);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);

	uiOpenWindow(&gameMenuWindow,0);
}

void taskGameMenuService(uint32 arg)
{

}
void taskGameMenuClose(uint32 arg)
{

	deg_Printf("game menu exit.\n");
}
sysTask taskGameMenu=
{
	"Game menu",
	(u32)NULL,
	taskGameMenuOpen,
	taskGameMenuClose,
	taskGameMenuService,
};


