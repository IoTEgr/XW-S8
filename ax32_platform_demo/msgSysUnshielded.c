#include "application.h"
#define BATCHECK_CNT_RELOAD  30
static uint32 usbOutPowerOffCnt=0;
static uint32 batlowPowerOffCnt=0;
static uint32 batCheckFlagCnt = BATCHECK_CNT_RELOAD;
extern int usb_bat_flag;
extern int usb_bat_draw_first_pic;
extern int usb_pwroff_logo_flag;
//static u8 pre_SD_status =0;
static int unshieldedKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}
static int unshieldedKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}
static int unshieldedKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}
static int unshieldedKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}
static int unshieldedKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}

static int unshieldedKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}

static int unshieldedKeyMsgRec(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}

#if 0
static int unshieldedKeyMsgPlayer(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}
#endif

static int unshieldedKeyMsgPrinter_en(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	return 0;
}


extern void setKeyEn(bool en);
static int unshieldedKeyMsgPower(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 lcdOn=0/*,irState=0*/; 
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deamon_auto_poweroff(1);
	
		boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
		if(lcdOn&&SysCtrl.astern == ASTERN_OFF)
		{
			if((taskCurrent()==TASK_USB_DEVICE)&&(usb_bat_flag==1))
			{
				usb_bat_flag = 0;
				//usb_bat_draw_first_pic = 1;
				layout_logo_show(1,0,1);
				XOSTimeDly(1000);
				taskStart(TASK_MAIN_MENU,0);
			}
			else
			{
				boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0);
			}
		}
		else
		{
			//boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,1);
			deamon_screen_save(1);
			setKeyEn(true);
		}

	/*
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
		if((task_video_Get_Status() == MEDIA_STAT_STOP)&&(0 == SysCtrl.game_stat))
		{
			if(videoRecordFrameEnGet())
			{
				//disable frame show
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				//SysCtrl.frame_index_last = SysCtrl.frame_index;  //save last frame num
				//SysCtrl.frame_index = -1;
			}
			else
			{
				if((SysCtrl.frame_index>=0) && (SysCtrl.frame_index < ICON_FRAME_NUM))
				{
					if(hal_custom_frame_create(R_FRAME[SysCtrl.frame_index])<0)
					{
						SysCtrl.frame_index = -1;
						SysCtrl.frame_index_last = -1;
						deg_Printf(">>---frame image is not prepared sucessfully,will not display frame--<<\n");
					}
				}
				//enable frame show
				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);
				SysCtrl.frame_index = SysCtrl.frame_index_last;
				//cmos_spec_color(0);	// set normal
			}
		}
		//deg_Printf("sys deal:powerkey\n");
	*/

	}
	return 0;
}
extern INT8S usbdev;
extern u32 csi_en_flag;
static int unshieldedKeyMsgPowerOff(winHandle handle,uint32 parameNum,uint32* parame)
{

	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.usb == USB_STAT_NULL)
		{
			usb_bat_flag = 0;
			taskStart(TASK_POWER_OFF,0);
		}
		else
		{
			if(SysCtrl.usb > USB_STAT_DCIN){
				SysCtrl.usb = USB_STAT_DCIN;
				usbdev = 0;
				hal_usbdUninit();
			}
			if(usb_bat_flag==0)
			{
				usb_bat_draw_first_pic = 0;
				usb_pwroff_logo_flag = 1;
			}
			else
			{
				//usb_bat_draw_first_pic = 0;
				if((taskCurrent()==TASK_USB_DEVICE)&&(usb_bat_flag==1)){
					usb_bat_flag = 0;
					usb_bat_draw_first_pic = 1;
					layout_logo_show(1,0,1);
					XOSTimeDly(1000);
					taskStart(TASK_MAIN_MENU,0);
				}
				return 0;
			}
			taskStart(TASK_USB_DEVICE,0);
		}
	}
	return 0;
}
extern void setSoftwareDetachUsensor(BOOL en);
static int unshieldedMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	deamon_screen_save(1);
	if((SysCtrl.usb != USB_STAT_NULL))  // DC IN
	{
		setSoftwareDetachUsensor(FALSE);
		usbOutPowerOffCnt=0;
		batlowPowerOffCnt=0;
	}
	else
		usbOutPowerOffCnt=BAT_SAVE_POWER;
	if((SysCtrl.usb == USB_STAT_PC)) 
		//taskStart(TASK_USB_DEVICE,USB_DEV_PCCAMERA);
		uiOpenWindow(&menuUsbWindow,0);
	return 0;
}
static int unshieldedMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{    
	deamon_screen_save(1);
#if SUPPORT_SELF_TEST
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
	{
		int ret;
		ret = open((const char *)"SELFTEST.bin",FA_READ);
		if(ret>=0)
		{
			close(ret);
			uiOpenWindow(&selfTestWindow,0);
		}
	}
#endif

	return 0;
}

static int unshieldedMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
	if(SysCtrl.battery <= BATTERY_STAT_2)
	{
		setSoftwareDetachUsensor(TRUE);
		if(backrec_adpt())
		{
			backrec_stop();
			deg_Printf("software detach Usensor!!!\n\n\n\n");
			if(SysCtrl.battery < BATTERY_STAT_2)
				uiOpenWindow(&tips1Window,2,TIPS_POWER_LOW,TIPS_KEEP_SHOWING);
			else
				uiOpenWindow(&tips1Window,2,TIPS_USENSOR_POWER_LOW,TIPS_KEEP_SHOWING);
		}
	}
*/
	if(BATTERY_STAT_0==SysCtrl.battery)
	{
		if(batlowPowerOffCnt==0)
			batlowPowerOffCnt=5;
		uiOpenWindow(&tips1Window,2,TIPS_NO_POWER,TIPS_KEEP_SHOWING);
	}else if(BATTERY_STAT_1==SysCtrl.battery)
	{
		uiOpenWindow(&tips1Window,2,TIPS_POWER_LOW,TIPS_KEEP_SHOWING);
	}
	

	return 0;
}
extern uint32 uvcIntCnt;
static int unshieldedMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
/*
	if((pre_SD_status != SDC_STAT_NORMAL) && (SysCtrl.sdcard == SDC_STAT_NORMAL))
	{
		deg("file list allready build!\n");
		filelist_build();

		deamon_fsFreeSize();
		task_video_record_caltime();

	//hal_sdUnlock();
		XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,SysCtrl.sdcard));
		XOSTimeDly(100);
		XMsgQPost(SysCtrl.sysQ,(void *)makeEvent(SYS_EVENT_SDC,SysCtrl.sdcard));

	}
*/
	if(usbOutPowerOffCnt)
	{
		usbOutPowerOffCnt--;
		if(usbOutPowerOffCnt==0)
		{
			deg_Printf("sys deal:usb out,auto power off\n");
			taskStart(TASK_POWER_OFF,0);
		}
	}

	if(batlowPowerOffCnt)
	{
		batlowPowerOffCnt--;
		if(batlowPowerOffCnt==0)
		{
			deg_Printf("sys deal:battery low,auto power off\n");
			taskStart(TASK_POWER_OFF,0);
		}
	}

#ifndef BAT_UPDATE_RTIME_EN
	if((configGet(CONFIG_ID_BAT_CHECK_FLAG)==R_ID_STR_COM_ON) && USB_STAT_NULL == SysCtrl.usb)
	{
		if(batCheckFlagCnt)
		{
			batCheckFlagCnt--;
			if(0==batCheckFlagCnt)
			{
				deg_Printf("msgun: batcheckcnt done! before flag[%d]\n",configValue2Int(CONFIG_ID_BAT_CHECK_FLAG));
				configSet(CONFIG_ID_BAT_CHECK_FLAG,R_ID_STR_COM_OFF);
				userConfigSave();
				deg_Printf(" - after flag[%d]\n",configValue2Int(CONFIG_ID_BAT_CHECK_FLAG));
				batCheckFlagCnt = BATCHECK_CNT_RELOAD;
			}
		}
	}
#endif
	//pre_SD_status = SysCtrl.sdcard; 
	//deg("msgUnshield : %d\n",pre_SD_status);
	return 0;
}


msgDealInfor sysUnshieldedMsgDeal[]=
{
	{KEY_EVENT_OK,unshieldedKeyMsgOk},
	{KEY_EVENT_UP,unshieldedKeyMsgUp},
	{KEY_EVENT_DOWN,unshieldedKeyMsgDown},
	{KEY_EVENT_LEFT,unshieldedKeyMsgDown},
	{KEY_EVENT_RIGHT,unshieldedKeyMsgUp},
	{KEY_EVENT_RETURN,unshieldedKeyMsgUp},
	{KEY_EVENT_MENU,unshieldedKeyMsgMenu},
	{KEY_EVENT_MODE,unshieldedKeyMsgMode},
	{KEY_EVENT_PHOTO,unshieldedKeyMsgPhoto},
	{KEY_EVENT_RECORD,unshieldedKeyMsgRec},
//	{KEY_EVENT_PLAYBACK,unshieldedKeyMsgPlayer},
	{KEY_EVENT_PRINTER_EN,unshieldedKeyMsgPrinter_en},
	{KEY_EVENT_POWER,unshieldedKeyMsgPower},
	{KEY_EVENT_POWEROFF,unshieldedKeyMsgPowerOff},
	{SYS_EVENT_SDC,unshieldedMsgSD},
	{SYS_EVENT_USB,unshieldedMsgUSB},
	{SYS_EVENT_BAT,unshieldedMsgBattery},
	{SYS_EVENT_1S,unshieldedMsg1S},
	{EVENT_MAX,NULL},
};




