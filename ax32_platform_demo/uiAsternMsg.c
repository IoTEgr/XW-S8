#if 0

#include "uiAsternRes.c"

static int asternKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int asternKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int asternKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int asternKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		//winDestroy(&handle);
	}
	return 0;
}
static int asternKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int asternSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int asternOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int asternCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
	deg_Printf("astern Close Win!!!\n");
	if(taskCurrent()==TASK_PLAY_BACK)
	{
		dispLayerSetPIPMode(DISP_PIP_DISABLE);
		dispLayerUninit(DISP_LAYER_VIDEO); // preview
		hal_csiEnable(0);						
		deg_Printf("player : astern off\n");
	}
	else
	{
		dispLayerSetPIPMode(SysCtrl.pip_mode);
	}
*/
	return 0;
}
static int asternWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("astern WinChild Close!!!\n");
	return 0;
}
static int asternSysMsgAstern(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.astern == ASTERN_OFF)
	{
		winDestroy(&handle);
	}
	return 0;
}
static int asternSysMsgUsensor(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usensorshow == USENSOR_SHOW_OFF)
	{
		SysCtrl.pip_mode = DISP_PIP_FRONT;
		SysCtrl.astern = ASTERN_OFF;
		api_astern_set_status(false);
		winDestroy(&handle);
	}
	
	return 0;
}
static int asternSysDrawUI(winHandle handle,uint32 parameNum,uint32* parame)
{
#define Ratex(n)       ((n)*LCD_WIDTH/OSD_WIDTH)  //  x position rate
#define Ratey(n)       ((n)*LCD_HEIGHT/OSD_HEIGHT)
#define Ratew(n)       ((n)*LCD_WIDTH/OSD_WIDTH)
#define Rateh(n)       ((n)*LCD_HEIGHT/OSD_HEIGHT)
#define LEFT_RATE   3
	uiRect rect;

	uint16 x,y;
	x=100;
	y=90;
	drawLine(Ratex(x),Ratey(y),Ratex(x+40),Ratey(y),Ratew(5),R_COLOR_GREEN);
	drawLine(Ratex(x),Ratey(y),Ratex(x-10/LEFT_RATE),Ratey(y+10),Ratew(5),R_COLOR_GREEN);
	y+=15;
	x-=(15/LEFT_RATE);
	drawLine(Ratex(x),Ratey(y),Ratex(x-10/LEFT_RATE),Ratey(y+10),Ratew(5),R_COLOR_GREEN);
	y+=15;
	x-=(15/LEFT_RATE);
//////////	
	drawLine(Ratex(x),Ratey(y),Ratex(x+40),Ratey(y),Ratew(5),R_COLOR_YELLOW);
	drawLine(Ratex(x),Ratey(y),Ratex(x-15/LEFT_RATE),Ratey(y+15),Ratew(5),R_COLOR_YELLOW);
	y+=25;
	x-=(25/LEFT_RATE);
	drawLine(Ratex(x),Ratey(y),Ratex(x-15/LEFT_RATE),Ratey(y+15),Ratew(5),R_COLOR_YELLOW);
	y+=25;
	x-=(25/LEFT_RATE);
//////////
	drawLine(Ratex(x),Ratey(y),Ratex(x+40),Ratey(y),Ratew(5),R_COLOR_RED);
	rect.x0=Ratex(x+40);
	drawLine(Ratex(x),Ratey(y),Ratex(x-20/LEFT_RATE),Ratey(y+20),Ratew(5),R_COLOR_RED);
	y+=30;
	x-=(30/LEFT_RATE);
	drawLine(Ratex(x),Ratey(y),Ratex(x-20/LEFT_RATE),Ratey(y+20),Ratew(5),R_COLOR_RED);
/////////////////////////////right////////////////////////////////////////
#define RIGHT_RATE   3
	x=210;
	y=90;
	drawLine(Ratex(x-40),Ratey(y),Ratex(x),Ratey(y),Ratew(5),R_COLOR_GREEN);
	drawLine(Ratex(x-3),Ratey(y),Ratex(x-3+10/RIGHT_RATE),Ratey(y+10),Ratew(5),R_COLOR_GREEN);
	y+=15;
	x+=(15/RIGHT_RATE);
	drawLine(Ratex(x-3),Ratey(y),Ratex(x-3+10/RIGHT_RATE),Ratey(y+10),Ratew(5),R_COLOR_GREEN);
	y+=15;
	x+=(15/RIGHT_RATE);
////////////	
	drawLine(Ratex(x-40),Ratey(y),Ratex(x),Ratey(y),Ratew(5),R_COLOR_YELLOW);
	drawLine(Ratex(x-3),Ratey(y),Ratex(x-3+15/RIGHT_RATE),Ratey(y+15),Ratew(5),R_COLOR_YELLOW);
	y+=25;
	x+=(25/RIGHT_RATE);
	drawLine(Ratex(x-3),Ratey(y),Ratex(x-3+15/RIGHT_RATE),Ratey(y+15),Ratew(5),R_COLOR_YELLOW);
	y+=25;
	x+=(25/RIGHT_RATE);
///////////
	drawLine(Ratex(x-40),Ratey(y),Ratex(x),Ratey(y),Ratew(5),R_COLOR_RED);
	rect.x1=Ratex(x-40);
	rect.y0=Ratey(y-15);
	rect.y1=Ratey(y+25);
	drawLine(Ratex(x-3),Ratey(y),Ratex(x-3+20/RIGHT_RATE),Ratey(y+20),Ratew(5),R_COLOR_RED);
	y+=30;
	x+=(30/RIGHT_RATE);
	drawLine(Ratex(x-3),Ratey(y),Ratex(x-3+20/RIGHT_RATE),Ratey(y+20),Ratew(5),R_COLOR_RED);
////////////////////////////////////////////////////////////////
	drawString(&rect,&rect,"STOP",R_FONT_NUM2,ALIGNMENT_CENTER,R_COLOR_YELLOW,INVALID_COLOR);
	return 0;
}


msgDealInfor asternMsgDeal[]=
{
	{SYS_OPEN_WINDOW,asternOpenWin},
	{SYS_CLOSE_WINDOW,asternCloseWin},
	{SYS_CHILE_COLSE,asternWinChildClose},
	{KEY_EVENT_OK,asternKeyMsgOk},
	{KEY_EVENT_UP,asternKeyMsgUp},
	{KEY_EVENT_DOWN,asternKeyMsgDown},
	{KEY_EVENT_MENU,asternKeyMsgMenu},
	{KEY_EVENT_MODE,asternKeyMsgMode},
	{SYS_EVENT_SDC,asternSysMsgSD},
	{SYS_EVENT_ASTERN,asternSysMsgAstern},
	{SYS_EVENT_USENSOR,asternSysMsgUsensor},
	{SYS_DRAW_UI,asternSysDrawUI},
	{EVENT_MAX,NULL},
};

WINDOW(asternWindow,asternMsgDeal,asternWin)

#endif

