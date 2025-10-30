#include"uiWin.h"

enum
{
	TIP1_STRING_ID=0,
	TIP1_TIPS_ID,
};
static widgetCreateInfor tips1Win[] =
{
	createFrameWin(Rx(70),Ry(50), Rw(180),Rh(140),R_COLOR_TBLACK,WIN_ABS_POS),
	createStringIcon(TIP1_TIPS_ID,Rx(0),Ry(0), Rw(180),Rh(45),R_ID_STR_SET_PROMT,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(TIP1_STRING_ID,Rx(0),Ry(45), Rw(180),Rh(95),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	widgetEnd(),
};



