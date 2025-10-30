#include "application.h"

#if 0
static int sysMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int sysMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int sysMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{

	return 0;
}
#endif
static int sysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int sysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
#if 0
static int SysMsgAstern(winHandle handle,uint32 parameNum,uint32* parame)
{
#if 0
	if(SysCtrl.astern == ASTERN_ON)
	{
		if(taskCurrent()==TASK_PLAY_BACK)
		{
			videoPlaybackStop();
			hal_csiEnable(1);
			dispLayerInit(DISP_LAYER_VIDEO); // preview
			dispLayerSetPIPMode(DISP_PIP_BACK);
			api_backrec_relinking();
			uiOpenWindow(&asternWindow,0);
			deg_Printf("player : astern on\n");
		}
		else
		{
			dispLayerSetPIPMode(DISP_PIP_BACK);
			uiOpenWindow(&asternWindow,0);
		}
	}
#endif
	return 0;
}
static int SysMsgUsensor(winHandle handle,uint32 parameNum,uint32* parame)
{
#if 0
	if(taskCurrent()!=TASK_PLAY_BACK)
	{
		if(SysCtrl.usensorshow == USENSOR_SHOW_ON)
		{
			SysCtrl.pip_mode = DISP_PIP_FRONT;
			if(api_backrec_status())
			{
				backrec_adpt_set(0);
				api_backrec_relinking();
			}
		    dispLayerSetPIPMode(SysCtrl.pip_mode);
		}
		else
		{
			SysCtrl.pip_mode = DISP_PIP_FRONT;
			SysCtrl.astern = ASTERN_OFF;			// usensor out clean adtern stat
			api_astern_set_status(false);
		    dispLayerSetPIPMode(SysCtrl.pip_mode);	
		}
	}
#endif
	return 0;
}
#endif
msgDealInfor sysRecommendMsgDeal[]=
{
//	{KEY_EVENT_POWER,sysMsgPower},
//	{KEY_EVENT_MENU,sysMsgMenu},
//	{KEY_EVENT_MODE,sysMsgMode},
	{SYS_EVENT_USB,sysMsgUSB},
	{SYS_EVENT_BAT,sysMsgBattery},
//	{SYS_EVENT_ASTERN,SysMsgAstern},
//	{SYS_EVENT_USENSOR,SysMsgUsensor},
	{EVENT_MAX,NULL},
};






