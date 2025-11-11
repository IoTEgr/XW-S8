#include "application.h"
#include "taskVideoRecordRes.c"
#include "../multimedia/interface/userInterface.h"
//static uint32 mdRecTime=0;
static uiRect recordTimeRect;
static bool curWinIsVideo=false;
//static bool firstPowerOnRec=true;


static uint32 FrameRecTime=0;
static u8 testmode_flag = 0;
bool V_sensorChanegeCtrl=TRUE;

extern void photo_animation_effect(UserInterface name,uint8 flag);

static void videoEnterTestMode(winHandle handle)
{
	testmode_flag = 1;
	videoTestModeShow(handle,1);
}

static void videoExitTestMode(winHandle handle)
{
	testmode_flag = 0;
	videoTestModeShow(handle,0);
}

bool isLongTestMode(void)
{
	return testmode_flag ? true : false;
}

#if defined(USE_BOARD_J591) || defined(USE_BOARD_J592) || defined(USE_BOARD_DY01)||defined(USE_BOARD_J599)
static int videoKeyMsgTestMode(winHandle handle,u32 parameNum,u32* parame)
{
	u32 keyState = KEY_STATE_INVALID;
	if(parameNum == 1)
		keyState = parame[0];
	if(keyState == KEY_PRESSED)
	{
		if((SysCtrl.usb == USB_STAT_DCIN)&&(testmode_flag==0)){
			videoEnterTestMode(handle);
		}
	}
	return 0;
}
#endif

#if 0
static int videoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deg_Printf("ok key\n");
	}
	return 0;
}
#endif

static int videoKeyMsgIr(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		u32 val=0;
		boardIoctrl(SysCtrl.bfd_ir,IOGET_IR_GET,(INT32U)&val);
		deg_Printf("ir:%d\n",val);
		if(val)
		{
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
		}
		else
		{
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);
		}
		videoIRShow(handle);
	}
	return 0;
}

extern u32 sensor_rotate_flag;
#if 0
static int videoKeyMsgSensorRotate(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deg_Printf("SensorRotate:%d\n",sensor_rotate_flag);
		if(sensor_rotate_flag == 1)
		{
			sensor_rotate_flag=0;
		}
		else if(sensor_rotate_flag == 0)
		{
			sensor_rotate_flag=1;
		}
		hal_csiEnable(0);
		_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
		csiSensor->p_fun_adapt.fp_rotate(sensor_rotate_flag);
		hal_csiEnable(1);
	}
	return 0;
}
#endif


static int videoRecordLongKey = 0;
static int videoKeyMsgRecord(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint8 flag=0;
	uint16 lcd_w,lcd_h;

	if(parameNum==1)
		keyState=parame[0];
	
	if(keyState==KEY_RELEASE)//KEY_PRESSED
	{
//deg_Printf
		uart_Printf("record key\n");

		/*if(videoRecordLongKey==1)
		{
			videoRecordLongKey = 0;
			return 0;
		}*/

		if(task_video_Get_Status() == MEDIA_STAT_START)
		{
			task_record_stop();
			/*
			if(SysCtrl.pip_mode != DISP_PIP_BACK)
			{
				if(SysCtrl.battery > BATTERY_STAT_2)
					setSoftwareDetachUsensor(false);
			}
			*/
#if 1
			SysCtrl.photo_task=1;
			UserInterface captureAnimation;
			
			ANIMATION(captureAnimation, LEFTBOTTOM2RIGHTUPPER)
			photo_animation_effect(captureAnimation,1);
			
			hal_csiEnable(1);
			dispLayerSetPIPMode(SysCtrl.pip_mode);
			//hal_lcd_get_next_frame();
			//drawUIService(true);

			SysCtrl.photo_task=0;
#endif
		}
		else
		{
			video_record_start();
			if(videoRecordFrameEnGet())	//  frame
			{
				FrameRecTime=hal_rtcSecondGet();
			}
			/*
			if((video_record_start()>=0)&&(SysCtrl.pip_mode != DISP_PIP_BACK))
				setSoftwareDetachUsensor(true);
			*/
		}
	}
	/*else if(keyState==KEY_CONTINUE)
	{
		uart_Printf("record key KEY_CONTINUE(%d,%d,%d)\n",SysCtrl.sdcard,SysCtrl.usb,testmode_flag);
		
		if((SysCtrl.sdcard == SDC_STAT_NULL)&&(SysCtrl.usb == USB_STAT_DCIN)&&(testmode_flag==0)){
			videoEnterTestMode(handle);
		}
		#ifdef DOUBLE_CAM
		//else// if(SysCtrl.usb == USB_STAT_NULL)
		{
			if(videoRecordLongKey == 0)
			{
				
				videoRecordLongKey = 1;
				if(task_video_Get_Status() == MEDIA_STAT_STOP)
				{
					//sensor_change();
					SysCtrl.sensor_change_flag =1;
				}else
				{
					return 0;  //do not switch when recording
				}
		#if 1
			{
				SysCtrl.photo_task=1;
				deg_Printf("ready video interface!\n");
				disp_frame_t * handle_frame=NULL;
				UserInterface recordeAnimation;
				ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)

				hal_csiEnable(1);
				dispLayerSetPIPMode(SysCtrl.pip_mode);
				hal_lcd_get_next_frame();
				drawUIService(true);

				
				
				bool change_finir = false;
				while(1)
				{
					deg_Printf("..a\n");
					if(0==SysCtrl.photo_software_handle)
					{
						deg_Printf("..b\n");
						handle_frame=yuv420_software_get_handle_frame();
						if(handle_frame)
						SysCtrl.photo_software_handle=1;
					}
					else if(1==SysCtrl.photo_software_handle)
					{
						deg_Printf("..c\n");
						SysCtrl.photo_software_handle=2;
						if(!flag)
						{
							flag=1;
							hal_lcdGetResolution(&lcd_w,&lcd_h);
							lcd_w=(lcd_w + 0x1f) & (~0x1f); // add 32bit align
							memcpy(recordeAnimation.preFrameBuf,handle_frame->y_addr,lcd_w*lcd_h*3/2);
							ax32xx_sysDcacheFlush((u32)recordeAnimation.preFrameBuf,lcd_w*lcd_h*3/2);
						}
						change_finir = recordeAnimation.run(&recordeAnimation, handle_frame->y_addr);
						hal_lcdDealBuffer(handle_frame);
						hal_lcd_get_next_frame();
						if(change_finir == true)
						break;
					}
				}
			SysCtrl.photo_task=0;
			deg_Printf("ready video interface! finish\n");

			}
#endif
			SysCtrl.sensor_change_flag =0;
			}
		}
		#endif

		
	}*/

	return 0;
}

static int videoKeyMsgSensorChange(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	uint8 flag=0;
	uint16 lcd_w,lcd_h;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		if(V_sensorChanegeCtrl)
		{
			V_sensorChanegeCtrl = FALSE;
			return 0;
		}
		if(task_video_Get_Status() == MEDIA_STAT_STOP)
		{
			SysCtrl.sensor_change_flag =1;
		}else
		{
			return 0;  //do not switch when recording
		}
#if 1
	
		SysCtrl.photo_task=1;
		deg_Printf("ready video interface!\n");
		disp_frame_t * handle_frame=NULL;
		UserInterface recordeAnimation;
		ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)

		/*hal_csiEnable(1);
		dispLayerSetPIPMode(SysCtrl.pip_mode);
		hal_lcd_get_next_frame();
		drawUIService(true);*/

		
		
		bool change_finir = false;
		while(1)
		{
			if(0==SysCtrl.photo_software_handle)
			{
				handle_frame=yuv420_software_get_handle_frame();
				if(handle_frame)
				SysCtrl.photo_software_handle=1;
			}
			else if(1==SysCtrl.photo_software_handle)
			{
				SysCtrl.photo_software_handle=2;
				if(!flag)
				{
					flag=1;
					hal_lcdGetResolution(&lcd_w,&lcd_h);
					lcd_w=(lcd_w + 0x1f) & (~0x1f); // add 32bit align
					memcpy(recordeAnimation.preFrameBuf,handle_frame->y_addr,lcd_w*lcd_h*3/2);
					ax32xx_sysDcacheFlush((u32)recordeAnimation.preFrameBuf,lcd_w*lcd_h*3/2);
				}
				change_finir = recordeAnimation.run(&recordeAnimation, handle_frame->y_addr);
				hal_lcdDealBuffer(handle_frame);
				hal_lcd_get_next_frame();
				if(change_finir == true)
				break;
			}
		}
	SysCtrl.photo_task=0;

	
#endif
		SysCtrl.sensor_change_flag =0;
			
		}

}

static int videoKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_MAIN_MENU,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}

static int videoKeyMsgPlayBack(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		taskStart(TASK_PLAY_BACK,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=2;
			SysCtrl.cartoon_show_cnt=0;
		}
	}
	return 0;
}


#if 0
static int videoKeyMsgSensorChange(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(task_video_Get_Status() == MEDIA_STAT_STOP)
		{
			sensor_change();
		}
	}
	return 0;
}
#endif


static int videoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	s32 ret;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
	#if 0	
		if(SysCtrl.spec_color_index>0)
		{
			SysCtrl.spec_color_index--;
		}
		else
		{
			SysCtrl.spec_color_index=5;;
		}
		cmos_spec_color(SysCtrl.spec_color_index);
	#else
	
		if(SysCtrl.spec_color_index>0)
		{
			SysCtrl.spec_color_index--;
			cmos_spec_color(SysCtrl.spec_color_index);
			
			if(0==SysCtrl.spec_color_index)
				cmos_spec_color(SysCtrl.spec_color_index);

		}else{
			SysCtrl.spec_color_index=6;		//030 :6 type ,3a03 & 20a6 :5 type
			cmos_spec_color(SysCtrl.spec_color_index);
		}

		
		return 0;
		

		if(videoRecordFrameEnGet()==0)
		{
			SysCtrl.photo_task=1;
			//SysCtrl.photo_mode_switch=3;
			SysCtrl.photo_mode_switch=2;
			hal_custom_frame_add_enable(1);
			videoRecordCmdSet(CMD_COM_FRAME,1);
			
			SysCtrl.photo_effect_idx=0;
			ret=hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
			if(ret<0)
			{
				deg_Printf(">>frame err!!%d<<\n",ret);
			}
		}
		else
		{
			//SysCtrl.photo_frame_idx++;
			SysCtrl.photo_effect_idx++;
			if(SysCtrl.photo_effect_idx>CAM_EFFECT_MAX -1)
			{
				if(videoRecordFrameEnGet())
				{
					//==close frame==
					videoRecordCmdSet(CMD_COM_FRAME,0);
					hal_custom_frame_add_enable(0);
					hal_custom_frame_unit();
					SysCtrl.photo_mode_switch=0;
					SysCtrl.photo_frame_idx=0;
					SysCtrl.photo_task=0;
				}
			}
			else
			{
				ret=hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				if(ret<0)
				{
					deg_Printf(">>frame err!!%d<<\n",ret);
				}
			}
		}



	#endif
	}

	return 0;
}
static int videoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	static u32 test = 0;
//    static u32 addr = 0;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
	#if 0
		SysCtrl.spec_color_index++;
		if(SysCtrl.spec_color_index>5)
		{
			SysCtrl.spec_color_index=0;
		}
		cmos_spec_color(SysCtrl.spec_color_index);
	#else


		if(SysCtrl.spec_color_index<6)	//030 :6 type ,3a03 & 20a6 :5 type
		{
			SysCtrl.spec_color_index++;
			cmos_spec_color(SysCtrl.spec_color_index);
		}
		else	// switch effect mode
		{
			//==normal color==
			SysCtrl.spec_color_index=0;
			cmos_spec_color(SysCtrl.spec_color_index);
		}

		return 0;
		

		if(videoRecordFrameEnGet()==0)
		{
			SysCtrl.photo_task=1;
			//SysCtrl.photo_mode_switch=3;
			SysCtrl.photo_mode_switch=2;
			hal_custom_frame_add_enable(1);
			videoRecordCmdSet(CMD_COM_FRAME,1);
			//SysCtrl.photo_frame_idx = ICON_FRAME_NUM -1;
			SysCtrl.photo_effect_idx = CAM_EFFECT_MAX -1;
			s32 ret=hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
			if(ret<0)
			{
				deg_Printf(">>frame err!!%d<<\n",ret);
			}
		}
		else
		{
			//SysCtrl.photo_frame_idx--;
			SysCtrl.photo_effect_idx--;
			if(SysCtrl.photo_frame_idx>250)
			{
				if(videoRecordFrameEnGet())
				{
					//==close frame==
					videoRecordCmdSet(CMD_COM_FRAME,0);
					hal_custom_frame_add_enable(0);
					hal_custom_frame_unit();
					SysCtrl.photo_mode_switch=0;
					SysCtrl.photo_frame_idx=0;
					SysCtrl.photo_task=0;
				}
			}
			else
			{
				s32 ret=hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				if(ret<0)
				{
					deg_Printf(">>frame err!!%d<<\n",ret);
				}
			}

		}


	#endif
	}

	return 0;
}

extern menu MENU(movie);
#if 0
static int videoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(task_video_Get_Status() != MEDIA_STAT_START)
		{
			uiOpenWindow(&menuItemWindow,1,&MENU(movie));
		}
		else
		{
			//winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),true);
			//videoCapturePhoto();
		}
	}
	return 0;
}
#endif
#if 0
static int videoKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(task_video_Get_Status() != MEDIA_STAT_START)
				taskStart(TASK_PHOTO_ENCODE,0);
	}
	return 0;
}
#endif

static int videoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard != SDC_STAT_NORMAL && task_video_Get_Status() == MEDIA_STAT_START) // sdc out when recording
	{
		task_record_stop();
		if(SysCtrl.pip_mode != DISP_PIP_BACK)
		{
			if(SysCtrl.battery > BATTERY_STAT_2)
				setSoftwareDetachUsensor(false);
		}
	}
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	videoSDShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,SDC_NULL_TIMEOUT);
	deg_Printf("SD state:%d\n",SysCtrl.sdcard);
	return 0;
}

static int videoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	videoBaterryShow(handle);
	return 0;
}

static int videoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
	{
		videoBaterryShow(handle);
		if(testmode_flag == 1){
			videoExitTestMode(handle);
		}
	}
#if 0
	if(SysCtrl.battery <= BATTERY_STAT_2)
	{
		if(backrec_adpt())
		{
			task_record_stop();
			videoRemainTimeShow(handle, SysCtrl.t_remain);
		}
	}
#endif
	return 0;
}
#if 0
static int videoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(task_video_Get_Status() != MEDIA_STAT_START)
	{
		
		if((video_record_start()>=0)&&(SysCtrl.pip_mode != DISP_PIP_BACK))
			setSoftwareDetachUsensor(true);
	}
	mdRecTime=XOSTimeGet();
	return 0;
}
#endif
#if 0
static int videoSysMsgRestart(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("!!!\n");
	if(task_video_Get_Status() != MEDIA_STAT_START)
	{
		
		if((video_record_start()>=0)&&(SysCtrl.pip_mode != DISP_PIP_BACK))
			setSoftwareDetachUsensor(true);
	}
	return 0;
}
#endif
extern uint32 videoRecordGetTimeSec(void);
extern void drawUIService1(void);
extern XWork_T *  recordIncrease1S;
void recordTimeCount1S(void)
{
	static char recTimeStr[]="00:00";
	uint32 sec,frameCntTime;
	if(task_video_Get_Status() == MEDIA_STAT_START)
	{
//		if(SysCtrl.pip_mode == DISP_PIP_BACK )
//			frameCntTime=videoRecordGetTimeSecB();
//		else
			frameCntTime=videoRecordGetTimeSec();

		if(videoRecordFrameEnGet()==0)	// no frame
		{
			if(SysCtrl.t_rec<frameCntTime)
			{
				if(recordIncrease1S)
					recordIncrease1S->reload=900*X_TICK;
			}
			else if(SysCtrl.t_rec>frameCntTime)
			{
				if(recordIncrease1S)
					recordIncrease1S->reload=1100*X_TICK;
			}
			else
			{
				if(recordIncrease1S)
				recordIncrease1S->reload=1000*X_TICK;
			}

			sec=SysCtrl.t_rec;
			SysCtrl.t_rec++;
			if(SysCtrl.t_rec>SysCtrl.t_mrec)
			SysCtrl.t_rec=0;
			
			if(curWinIsVideo)
			{
				sec=sec%3600;
				recTimeStr[0] = sec/600+'0';  // m
				recTimeStr[1] = ((sec/60)%10)+'0';	 // m
				recTimeStr[2] = ':';
				sec=sec%60;
				recTimeStr[3] = sec/10+'0';  // s
				recTimeStr[4] = sec%10+'0';   // s
				recTimeStr[5] = 0;
				//deg_Printf("%d...%d...%d...%d...\n",recordTimeRect.x0, recordTimeRect.x1, recordTimeRect.y0, recordTimeRect.y1);
				drawString(&recordTimeRect,&recordTimeRect,strToResId(recTimeStr),R_FONT_DEFAULT,ALIGNMENT_LEFT,R_COLOR_RED,R_COLOR_TRANSFER,R_COLOR_TRANSFER);
				drawUIService1();
			}
		}
		else
		{
			if(recordIncrease1S)
				recordIncrease1S->reload=200*X_TICK;
			
			if( ( hal_rtcSecondGet() - FrameRecTime) >0)
			{
				FrameRecTime=hal_rtcSecondGet();
				sec=SysCtrl.t_rec;
				SysCtrl.t_rec++;

				if(SysCtrl.t_rec>SysCtrl.t_mrec)
					SysCtrl.t_rec=0;
		
				if(curWinIsVideo)
				{
					sec=sec%3600;
					recTimeStr[0] = sec/600+'0';  // m
					recTimeStr[1] = ((sec/60)%10)+'0';	 // m
					recTimeStr[2] = ':';
					sec=sec%60;
					recTimeStr[3] = sec/10+'0';  // s
					recTimeStr[4] = sec%10+'0';   // s
					recTimeStr[5] = 0;
					//deg_Printf("%d...%d...%d...%d...\n",recordTimeRect.x0, recordTimeRect.x1, recordTimeRect.y0, recordTimeRect.y1);
					drawString(&recordTimeRect,&recordTimeRect,strToResId(recTimeStr),R_FONT_DEFAULT,ALIGNMENT_LEFT,R_COLOR_RED,R_COLOR_TRANSFER,R_COLOR_TRANSFER);
					drawUIService1();
				}
				
			}
		}
	}
}

static int videoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	INT32U sensorValue = 0;
	if (boardIoctrl(SysCtrl.bfd_hall, IOCTRL_HALL_CHECK, (INT32U)&sensorValue) >= 0)
	{
		if (sensorValue == 0)
		{
			sensor_rotate(0);
		}
		else
		{
			sensor_rotate(1);
		}
		SysCtrl.hall_stat = sensorValue;
	}
	deg_Printf("video Open Win!!!%d %d\n",SysCtrl.sdcard,SysCtrl.usb);
	layout_version_get();
	winSetResid(winItem(handle,VIDEO_MODE_ID),R_ID_ICON_MTRECORD);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
//	#if SMALL_PANEL_SUPPORT==0
//	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	videoResolutionShow(handle);
	videoMDShow(handle);
	videoMonitorShow(handle);
	videoIrLedShow(handle);
	videoLockShow(handle);
	videoSDShow(handle);
	videoMicShow(handle);
	videoBaterryShow(handle);
	videoLoopRecordShow(handle);
	videoIRShow(handle);
	V_sensorChanegeCtrl=TRUE;

	if(SysCtrl.usb != USB_STAT_DCIN){
		videoExitTestMode(handle);
	}else{
		videoTestModeShow(handle,testmode_flag);
	}
#if 0
	if(firstPowerOnRec)
	{
		if(SysCtrl.sdcard == SDC_STAT_NORMAL&&SysCtrl.battery > BATTERY_STAT_2)
		{
			deg_Printf("power on,start record\n");
			if(SysCtrl.usensor==USENSOR_STAT_NULL||(SysCtrl.usensor!=USENSOR_STAT_NULL&&api_backrec_status()))
			{
				firstPowerOnRec=false;
				XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_ON,0));
			}
		}
		else
			firstPowerOnRec=false;
	}
#endif
	winGetPos(winItem(handle,VIDEO_REC_TIME_ID),&recordTimeRect);
	curWinIsVideo=true;
	
	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}

	return 0;
}
static int videoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video Close Win!!!\n");
	boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
	curWinIsVideo=false;
	return 0;
}
static int videoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	curWinIsVideo=true;
	deg_Printf("video WinChild Close!!!\n");
	if(task_video_Get_Status() != MEDIA_STAT_START)
		videoRemainTimeShow(handle, SysCtrl.t_remain);
//	#if SMALL_PANEL_SUPPORT==0
//	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif
	videoResolutionShow(handle);
	videoMDShow(handle);
	videoMonitorShow(handle);
	videoIrLedShow(handle);
	videoLockShow(handle);
	videoSDShow(handle);
	videoMicShow(handle);
	videoBaterryShow(handle);
//	winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
	drawUIService(true);

	if(videoRecordFrameEnGet())
	{
		//==close frame==
		videoRecordCmdSet(CMD_COM_FRAME,0);
		hal_custom_frame_add_enable(0);
		hal_custom_frame_unit();
		SysCtrl.photo_mode_switch=0;
		SysCtrl.photo_frame_idx=0;
		SysCtrl.photo_task=0;
	}

	//useDoubleBuff(false);
	return 0;
}
static int videoWinChildOpen(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("video WinChild Open!!!\n");
	curWinIsVideo=false;
	return 0;
}
static int videoRecordStopped(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("stop msg\n");
	deamon_screen_save(1);
//	videoLockShow(handle);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	return 0;
}
static int videoRecordStarted(winHandle handle,uint32 parameNum,uint32* parame)
{
	//SysCtrl.t_rec = 0;
	deg_Printf("start msg\n");
	videoLockShow(handle);
	videoRecTimeShow(handle, 0);
	drawUIService(true);
	return 0;
}
static int videoRecordResume1(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("resume msg\n");
//	videoLockShow(handle);
	return 0;
}

static int videoRecordErr(winHandle handle,uint32 parameNum,uint32* parame)
{
//	videoLockShow(handle);
	videoRemainTimeShow(handle, SysCtrl.t_remain);
	return 0;
}
#if 0
static int videoRecordLockFile(winHandle handle,uint32 parameNum,uint32* parame)
{
//	SysCtrl.g_lock = 1;
//	videoLockShow(handle);
	return 0;
}
#endif
static int videoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	static uint32 flag=0;
	u8 timestramp;
	//deg_Printf("video 1111 remain:[%x]\n",hal_sysMemRemain());
	timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
	if(timestramp == 1)
	{
		winSetResid(winItem(handle,VIDEO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
		winSetVisible(winItem(handle,VIDEO_SYSTIME_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,VIDEO_SYSTIME_ID),false);
	}	
	
//	#if SMALL_PANEL_SUPPORT==0
//	videoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif

	videoIrLedShow(handle);

//	if(get == stop)

	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		videoBaterryShow(handle);	
	}
/*
	if(configGet(CONFIG_ID_MOTIONDECTION)==R_ID_STR_COM_ON)
	{
		if(mdRecTime&&XOSTimeGet()-mdRecTime>=MOTION_DEC_TIME*1000)
		{
			task_record_stop();
			if(SysCtrl.pip_mode != DISP_PIP_BACK)
			{
				if(SysCtrl.battery > BATTERY_STAT_2)
					setSoftwareDetachUsensor(false);
			}
			mdRecTime=0;
		}
	}
*/
	if(task_video_Get_Status() == MEDIA_STAT_START)
	{
		if(flag&1)
			boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
		else
			boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
		flag++;
	}
	/*
	if(winIsVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID)))
		winSetVisible(winItem(handle,VIDEO_CAPTURE_PHOTO_ID),false);
		*/

	if(testmode_flag == 1){
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
	}
	drawUIService(true);
	return 0;
}

msgDealInfor videoRecordeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,videoOpenWin},
	{SYS_CLOSE_WINDOW,videoCloseWin},
	{SYS_CHILE_COLSE,videoWinChildClose},
	{SYS_CHILE_OPEN,videoWinChildOpen},
	//{KEY_EVENT_ROTATE,videoKeyMsgSensorRotate},
//	{KEY_EVENT_OK,/*videoKeyMsgOk*/videoKeyMsgRecord},
	{KEY_EVENT_RECORD,videoKeyMsgRecord},
	{KEY_EVENT_RETURN,videoKeyMsgExit},
	{KEY_EVENT_LEFT,videoKeyMsgUp},
	{KEY_EVENT_RIGHT,videoKeyMsgDown},
	// {KEY_EVENT_OK,videoKeyMsgSensorChange},
	{KEY_EVENT_PLAYBACK,videoKeyMsgPlayBack},
	{SYS_EVENT_SDC,videoSysMsgSD},
	{SYS_EVENT_USB,videoSysMsgUSB},
	{SYS_EVENT_BAT,videoSysMsgBattery},
	{VID_EVENT_REC_STOP,videoRecordStopped},
	{VID_EVENT_REC_START,videoRecordStarted},
	{VID_EVENT_REC_RESUME,videoRecordResume1},
	{VID_EVENT_REC_ERROR,videoRecordErr},
//	{VID_EVENT_LOCK_FILE,videoRecordLockFile},
	{SYS_EVENT_1S,videoSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(videoRecordWindow,videoRecordeMsgDeal,videoRecordeWin)

bool cut_sdr_data_en = 0;
u32 cut_sdr_data_addr = 0;
#if _DEBG_CUT_SDR_DATA_
typedef struct 
{
	void * sdr_data_buff;
	u32 len;
}SDR_DATA_MSG;
SDR_DATA_MSG sdr_data;
/*******************************************************************************
* Function Name  : sdram_data_capture_cfg
* Description    : config for capture sdram_data
* Input          : void *sdr_data_buff
* Output         : None
* Return         : None
*******************************************************************************/
void sdram_data_capture_cfg(void *sdr_data_buff)
{
	//dvp
	void hal_Cut_SDR_SrcCSI(void *sdr_buf,bool csi_save);
#if (_DEBG_CUT_SDR_DATA_&0xf0) == _DEBG_CUT_BEFOR_CSI_
	hal_Cut_SDR_SrcCSI(sdr_data_buff,1);
#elif _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_POST_YUV_
	hal_Cut_SDR_SrcCSI(sdr_data_buff,0);
#endif
}
/*******************************************************************************
* Function Name  : sdram_data_capture_task
* Description    : capture sdram data 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void sdram_data_capture_task(u8 *fname)
{
	ax32xx_csiEnable(0);
	hal_mjpegEncodeUninit();
	//capture rawdata
	_Sensor_Adpt_ *senradpt = hal_csiAdptGet();
#if _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_CSI_RAW_
	debg("capture rawdata start\n");
	TCHAR filename[18] = {'D','E','B','G','/','R','A','W','D','0','0','0','0','.','r','a','w',0};
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 2;
	sdr_data.sdr_data_buff = hal_sysMemMalloc(senradpt->pixelh*senradpt->pixelw*2,64);
#elif _DEBG_CUT_SDR_DATA_ == _DEBG_CUT_CSI_YUV_
	debg("capture csi yuv422 data start\n");
	TCHAR filename[18] = {'D','E','B','G','/','Y','U','V','8','0','0','0','0','.','r','a','w',0};
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 2;
#else
	debg("capture post yuv420 data start\n");
	TCHAR filename[18] = {'D','E','B','G','/','Y','U','V','6','0','0','0','0','.','y','u','v',0};
	sdr_data.len = senradpt->pixelw * senradpt->pixelh * 3 / 2;
	#if _DEBG_CUT_SDR_DATA_  ==  _DEBG_CUT_POST_YUV_INAVI_    
		sdr_data.sdr_data_buff = (void * )cut_sdr_data_addr;
	#elif _DEBG_CUT_SDR_DATA_  ==  _DEBG_CUT_POST_YUV_LCD_
		sdr_data.sdr_data_buff = (void * )cut_sdr_data_addr;
		sdr_data.len = 480*272*3/2;
	#endif
#endif
	if(sdr_data.sdr_data_buff ==NULL){
		debg("sdr_data_buff  apply fail \n");
		return;
	}
	sdram_data_capture_cfg(sdr_data.sdr_data_buff);
	//msg deal
	TCHAR path[] = {'D','E','B','G',0};
	FIL Fil;
	FRESULT ret = f_mkdir(/*"DEBG\0"*/path);
	if(ret != FR_OK){	
		if(ret != FR_EXIST){
			debg("DEBG mkdir err\n");
			return;
		}
	}
	u32 filenum ,loop = 1000000;
	debg("CSIMODE:%x CSIPMODE2:%x CSI_DMA_MODE:%x ",CSIMODE,ISPMODE,CSI_DMA_MODE);
	while(loop--){
		hal_wdtClear();
		if(cut_sdr_data_en == 1){
			debg("capture sdrdata pending\n");
			cut_sdr_data_en = 0;
			uint write;
			for(filenum = 0;filenum < 1000;filenum++){
				filename[9]='0'+ ((filenum / 1000)%10);
				filename[10]='0'+ ((filenum / 100)%10);
				filename[11]='0'+ ((filenum / 10)%10);
				filename[12]='0'+ (filenum  % 10);
				if(FR_OK != f_open(&Fil,filename, FA_READ)){
					if(FR_OK == f_open(&Fil,filename, FA_CREATE_NEW | FA_WRITE)){
						f_write(&Fil, (const void*)sdr_data.sdr_data_buff, sdr_data.len, &write);
						debg("Raw len = %x\n",write);
						loop = 0;
						hal_sysMemFree(sdr_data.sdr_data_buff);
						//memcpy(fname,filename,sizeof(filename));
						Tchar2Ascii(filename, fname, sizeof(filename)/sizeof(filename[0]))
						hal_mjpegMemUninit();//mjpegEncCtrl.ybuffer = 0;
					}
					f_close(&Fil);
					break;
				}
				f_close(&Fil);
			}		
		}
	}
	
}
#endif


