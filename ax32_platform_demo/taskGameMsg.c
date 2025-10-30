#include "application.h"
#include"uiWin.h"
#include "taskgame.h"

static int gameKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}

static int gameKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}

static int gameKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}

static int gameKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}

static int gameKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_GAME_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

static int gameOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int gameCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}





static int gameSysMsgGameTick(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}


static widgetCreateInfor gameWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),
	widgetEnd(),
};


msgDealInfor gameMsgDeal[]=
{
	{SYS_OPEN_WINDOW,gameOpenWin},
	{SYS_CLOSE_WINDOW,gameCloseWin},
	{KEY_EVENT_UP,gameKeyMsgUp},
	{KEY_EVENT_DOWN,gameKeyMsgDown},
	{KEY_EVENT_PHOTO,gameKeyMsgPhoto},
	{KEY_EVENT_OK,gameKeyMsgOk},
	{KEY_EVENT_RETURN,gameKeyMsgExit},
	{SYS_EVENT_GAME_TICK,gameSysMsgGameTick},
	{EVENT_MAX,NULL},
};

WINDOW(gameWindow,gameMsgDeal,gameWin)



