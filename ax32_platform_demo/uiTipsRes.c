#include"uiWin.h"

enum
{
	TIP_STRING_ID=0,
};
static widgetCreateInfor tipsWin[] =
{
	createFrameWin(Rx(50),Ry(50), Rw(220),Rh(140),R_COLOR_BLACK,WIN_ABS_POS),
	createStringIcon(TIP_STRING_ID,Rx(0),Ry(0), Rw(220),Rh(140),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	widgetEnd(),
};



