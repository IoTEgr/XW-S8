#include"uiWin.h"

enum
{
	DELCUR_TIPS_ID=0,
	DELCUR_SELECT_ID
};
static widgetCreateInfor delCurWin[] =
{

	createFrameWin(Rx(50),Ry(50), Rw(220),Rh(140),R_COLOR_BLACK,WIN_ABS_POS),
	createStringIcon(DELCUR_TIPS_ID,Rx(0),Ry(0), Rw(220),Rh(60),R_ID_STR_FMT_DELETE,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(DELCUR_SELECT_ID,Rx(0),Ry(60), Rw(220),Rh(80),INVALID_COLOR),

	widgetEnd(),
};


