#include"uiWin.h"

enum
{
	MENU_USB_SELECT_ID=0,
};
static widgetCreateInfor menuUsbWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_BLACK,WIN_ABS_POS),
	#if SMALL_PANEL_SUPPORT==0
	createItemManage(MENU_USB_SELECT_ID,Rx(120),Ry(0), Rw(100),Rh(240),INVALID_COLOR),
	#else
	createItemManage(MENU_USB_SELECT_ID,Rx(80),Ry(0), Rw(160),Rh(240),INVALID_COLOR),
	#endif
	widgetEnd(),
};



