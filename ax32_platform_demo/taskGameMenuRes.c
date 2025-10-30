#include"uiWin.h"

enum
{
	GAMEMENU_SEL_LEFT_ID=0,
	GAMEMENU_SEL_RIGHT_ID,
	GAMEMENU_MAX_ID
};

static widgetCreateInfor gameMenuWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

//	createImageIcon(GAMEMENU_SEL_LEFT_ID,Rx(32), Ry(110), Rw(20), Rh(20), R_ID_ICON_MTGLEFT,ALIGNMENT_CENTER),
//	createImageIcon(GAMEMENU_SEL_RIGHT_ID,Rx(320-32), Ry(110), Rw(20), Rh(20), R_ID_ICON_MTGRIGHT,ALIGNMENT_CENTER),

//	createStringIcon(MAINMENU_PHOTO_ID,Rx(0), Ry(96), Rw(106), Rh(25),strToResId("PHOTO"),ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	widgetEnd(),
};


