#include"uiWin.h"
#if 0

enum
{
	UNLOCKCUR_TIPS_ID=0,
	UNLOCKCUR_SELECT_ID
};
static widgetCreateInfor unlockCurWin[] =
{
	#if SMALL_PANEL_SUPPORT==0
	createFrameWin(Rx(70),Ry(42), Rw(180),Rh(142),R_COLOR_GRAY2,WIN_ABS_POS),
	createStringIcon(UNLOCKCUR_TIPS_ID,Rx(0),Ry(0), Rw(180),Rh(100),R_ID_STR_SET_UNLOCKCUR,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(UNLOCKCUR_SELECT_ID,Rx(0),Ry(100), Rw(180),Rh(40),INVALID_COLOR),
	#else
	createFrameWin(Rx(30),Ry(50), Rw(260),Rh(140),R_COLOR_GRAY2,WIN_ABS_POS),
	createStringIcon(UNLOCKCUR_TIPS_ID,Rx(0),Ry(0), Rw(260),Rh(100),R_ID_STR_SET_UNLOCKCUR,ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createItemManage(UNLOCKCUR_SELECT_ID,Rx(0),Ry(100), Rw(260),Rh(40),INVALID_COLOR),
	#endif
	widgetEnd(),
};

#endif


