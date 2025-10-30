#include "application.h"
#include "taskPhotoEncodeRes.c"
#include "../multimedia/interface/userInterface.h"
#include "../device/printer/printer.h"
#include "../multimedia/watermark/image_watermark.h"

extern int image_take_photo(void);
extern int image_take_photo_to_sdram(u32 *jpg_sdram_addr,u32 *jpg_size);
extern void photo_animation_effect(UserInterface name,uint8 flag);


u32 photo_focus_show_count = 0;
static u8 *focus_sound = 0;
static s32 focus_sound_size = 0;
bool P_sensorChanegeCtrl=TRUE;
//static u32 keep_print_wait_encode;  //when keep print  ,need to wait

//=========for watermark==========
//keep
 u8* small_pic_id_buf[20]; 

 u32  small_pic_id[20]={RES_NUM_0,RES_NUM_1,RES_NUM_2,RES_NUM_3,RES_NUM_4,
								RES_NUM_5,RES_NUM_6,RES_NUM_7,RES_NUM_8,RES_NUM_9,
								RES_NUM_SLASH,RES_NUM_COLON,RES_NUM_BLANK,
							   };
 //=========for watermark==========


void photoFocusIconCancel(winHandle handle)
{
	if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_RED){
		if(XOSTimeGet() - photo_focus_show_count > 800){
			SysCtrl.photo_focus=PHOTO_FOCUS_ICON_NONE;
			photoFocusShow(handle);
			photoZoomRatioShow(handle);
		}
	}
}
/*#ifdef DOUBLE_CAM
static int photoLongKey = 0;
#endif*/
static int photoKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	int ret;

	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		u16 take_photo_num=0,take_photo_delay_time=0,printer_delay_time;
		u32 start_time,space_time;
		//filelist_build();
		deg_Printf("photo_mode_switch=%d,photo_mode_idx=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);
		
		#ifdef DOUBLE_CAM
		/*if(photoLongKey == 1)
		{
			photoLongKey = 0;
			deg_Printf("in double cam change\n");
			return 0;
		}*/
		#endif

		if(0==SysCtrl.photo_mode_switch)	// photo mode
		{
			if((0==SysCtrl.photo_mode_idx)||(7==SysCtrl.photo_mode_idx)||(8==SysCtrl.photo_mode_idx))
			{
				take_photo_num=1;
				take_photo_delay_time=0;
			}
			else if(1==SysCtrl.photo_mode_idx)
			{
				take_photo_num=3;
				take_photo_delay_time=0;
			}
			else if(2==SysCtrl.photo_mode_idx)
			{
				take_photo_num=5;
				take_photo_delay_time=0;
			}
			else if(3==SysCtrl.photo_mode_idx)
			{
				take_photo_num=1;
				take_photo_delay_time=3;
			}
			else if(4==SysCtrl.photo_mode_idx)
			{
				take_photo_num=1;
				take_photo_delay_time=5;
			}
			else if(5==SysCtrl.photo_mode_idx)
			{
				take_photo_num=1;
				take_photo_delay_time=10;
			}
			else if(6==SysCtrl.photo_mode_idx)
			{
				take_photo_num=3;
				take_photo_delay_time=5;
			}

		}
		else
		{
			take_photo_num=1;
			take_photo_delay_time=0;
		}
		//take_photo_delay_time = configValue2Int(CONFIG_ID_TIMEPHOTO);

		deg_Printf("take_photo_num:%d,take_photo_delay_time=%d\n",take_photo_num,take_photo_delay_time);

		//while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY);
		if(0 != take_photo_delay_time)	// delay take photo show num
		{
			start_time = XOSTimeGet();
			XOSTimeDly(100);
			hal_wdtClear();
			photo_time_num_show(handle,true,take_photo_delay_time);
			while(1)
			{
				hal_wdtClear();
				space_time = XOSTimeGet()-start_time;
				if(space_time >= 1000)
				{
					take_photo_delay_time--;
					photo_time_num_show(handle,true,take_photo_delay_time);
					if(0 == take_photo_delay_time)
					{
						photo_time_num_show(handle,false,0);
						break;
					}
					start_time = XOSTimeGet();
				}
				taskPhotoEncodeService(/*NULL*/0);
			}
		}

		while(take_photo_num--)
		{

			//taskPhotoEncodeService(/*NULL*/0);
			XOSTimeDly(100); //keep

			if(SDC_STAT_NORMAL==SysCtrl.sdcard || SDC_STAT_FULL==SysCtrl.sdcard)// take photo to sdcard 
			{
				SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
				photoFocusShow(handle);
				drawUIService(true);

				boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
				//==show pencil effect==
				if(0==SysCtrl.photo_mode_switch)
				{
					if((7==SysCtrl.photo_mode_idx)||(8==SysCtrl.photo_mode_idx))
					{
						disp_frame_t * handle_frame=yuv420_software_get_handle_frame();
						if(NULL!=handle_frame)
						{
							u8 color;
							if(7==SysCtrl.photo_mode_idx)
							{
								color=0;
							}
							else
							{
								color=1;
							}
							yuv420_pencil_sketch(handle_frame->y_addr,handle_frame->uv_addr,handle_frame->w,handle_frame->h,color);
							hal_lcdDealBuffer(handle_frame);
						}
					}
				}



				if(SysCtrl.sd_freesize<=REDUCE_2_5M)
				{
					ret =-1 ;
					SysCtrl.sdcard=SDC_STAT_FULL;
					XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
				
				}else{
						#if 1
						ret = image_take_photo();
						#else
						u32 jpg_addr;
						u32 jpg_size;
						ret=image_take_photo_to_sdram(&jpg_addr,&jpg_size);

						char *name;
						int fHandle;
						fHandle = task_image_createfile(VIDEO_CH_A,&name);
						
						write(fHandle,(void *)jpg_addr,jpg_size);
						deg_Printf("imageL save thumb :%x Size:%x\n",jpg_addr,jpg_size);
						deamon_fsSizeModify(-1,fs_size(fHandle));
						close(fHandle);
						hal_sysMemFree(jpg_addr);
						watermark_buf_bmp2yuv_free();
						#endif

						
						//--add photo effect
						if(ret==0)
						{	
							UserInterface captureAnimation;
							ANIMATION(captureAnimation, LEFTBOTTOM2RIGHTUPPER)
							photo_animation_effect(captureAnimation,1);
							photoRemainNumShow(handle);
							deg_Printf("out photo effect\n");
							

							hal_csiEnable(0);						// set lcd image stop
							dispLayerSetPIPMode(DISP_PIP_DISABLE);

							if(SysCtrl.printer_en)
							{
								
								SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
								photoPrinterTipsShow(handle,false,0,0);
								photoFocusShow(handle);
								drawUIService(true);
								
								if(1) //printer dly 3s
								{
									start_time = XOSTimeGet();
									XOSTimeDly(100);
									hal_wdtClear();
									printer_delay_time = 3;
									photoPrintDlyShow(handle,true,printer_delay_time);
									XOSTimeDly(1000);
									while(1)
									{
										hal_wdtClear();
										space_time = XOSTimeGet()-start_time;
										if(space_time >= 1000)
										{
											printer_delay_time--;
											photoPrintDlyShow(handle,true,printer_delay_time);
											if(0 == printer_delay_time)
											{
												photoPrintDlyShow(handle,false,0);
												break;
											}
											start_time = XOSTimeGet();
										}
									}
								}
							}

						}

				}	
				
				//==end show pencil effect=== 
				boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);

				
				photoPrinterTipsShow(handle,false,0,0);
				SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
				photoFocusShow(handle);
				drawUIService(true);
					
				if(0==ret)
				{
					if(SysCtrl.printer_en)
					{	
						TCHAR fname[64];
						char *name;
						int type;
						int fd;
						if(managerFileCount(SysCtrl.avi_list)<1)
						{
							deg_Printf("no file\n");
							goto PHOTO_TAKE_PIC_TFCARD_EXIT;
						}
						name = manangerGetFileFullPathName(SysCtrl.avi_list,managerFileCount(SysCtrl.avi_list),&type);
						if(name == NULL)
						{
							deg_Printf("index err\n");
							goto PHOTO_TAKE_PIC_TFCARD_EXIT;
						}

						deg_Printf("%s\n",name);
						Ascii2Tchar(name, fname, sizeof(fname)/sizeof(fname[0]));
						fd = (int)open(/*name*/fname,FA_READ);
						if((int)fd<0)
						{
							deg_Printf("open file err\n");
							goto PHOTO_TAKE_PIC_TFCARD_EXIT;
						}
						close(fd);

						SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
						photoFocusShow(handle);
						drawUIService(true);

						photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_PRINTING);
					    drawUIService(true);
						
						fd = (int)open(/*name*/fname,FA_READ);
						deg_Printf("fd=%d,%d\n",fd,fs_size(fd));
						if((int)fd<0)
						{
							deg_Printf("open file err\n");
							goto PHOTO_TAKE_PIC_TFCARD_EXIT;
						}
						
						u8 print_ret=printer_jpeg(fd,SysCtrl.printer_level,SysCtrl.printer_print_mode,SysCtrl.battery);
						if(8==print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_NO_PAPER);
							drawUIService(true);
							XOSTimeDly(2000);
						}
						else if(9==print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_OVER_HEAT);
							drawUIService(true);
							XOSTimeDly(2000);
						}
						else if(15 == print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIP_KEEPQ);
							drawUIService(true);
							XOSTimeDly(2000);
						}else if(12 == print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_CHARG_N_PRINT);
							drawUIService(true);
							XOSTimeDly(2000);
						}
						close(fd);
						photoPrinterTipsShow(handle,false,0,0);
						drawUIService(true);


					}
					else
					{
					}

PHOTO_TAKE_PIC_TFCARD_EXIT:
					
					if(SysCtrl.printer_en)
					{
						hal_wdtClear();
						#if (2 == SENSOR_NUM)
						sensorInit_table();
						//XOSTimeDly(400);
						#endif
						hal_csiEnable(1);
						dispLayerSetPIPMode(SysCtrl.pip_mode);
						XOSTimeDly(300);
						hal_lcd_get_next_frame();
						//drawUIService(true);
						
					}else{
					hal_wdtClear();
					hal_csiEnable(1);
					dispLayerSetPIPMode(SysCtrl.pip_mode);
					}

					

					if(SysCtrl.spec_color_index)
					{
						cmos_spec_color(SysCtrl.spec_color_index);
					}

				}
				else
				{
					SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
					photoFocusShow(handle);
					drawUIService(true);
				}
			}
			else // no TF or TF full , print out
			{
				//XMsgQPost(SysCtrl.sysQ,makeEvent(SYS_EVENT_SDC,0));
#if 1			
				u32 jpg_addr;
				u32 jpg_size;

			#if ENABLE_FLASH_PHOTO
				if(1)
			#else
				if(SysCtrl.printer_en)
			#endif
		        {
					int ret;
					
					boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);

					SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
					photoFocusShow(handle);
					drawUIService(true);
					deg_Printf("before taske photo remain:0x%x\n",hal_sysMemRemain());
					#if 1
					ret=image_take_photo_to_sdram(&jpg_addr,&jpg_size);
					#else
					 disp_frame_t * handle_frame=yuv420_software_get_handle_frame();
					ret=image_take_photo_to_sdram2(handle_frame->y_addr,PHOTO_2_FLASH_W,PHOTO_2_FLASH_H);
					jpg_addr = (u32)handle_frame->y_addr;
					#endif
					deg_Printf("after taske photo remain:0x%x\n",hal_sysMemRemain());
					//--add photo effect
					if(ret==0)
					{					
						UserInterface captureAnimation;
						ANIMATION(captureAnimation, LEFTBOTTOM2RIGHTUPPER)
						photo_animation_effect(captureAnimation,1);
						photoRemainNumShow(handle);
						deg_Printf("out photo effect\n");
						
						hal_csiEnable(0);						// set lcd image stop
						dispLayerSetPIPMode(DISP_PIP_DISABLE);

						SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
						photoFocusShow(handle);
						drawUIService(true);
						
						if(SysCtrl.printer_en)
						{
//							SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
//							photoPrinterTipsShow(handle,false,0,0);
//							photoFocusShow(handle);
//							drawUIService(true);
							
							//hal_sysMemPrint();
							if(1) //printer dly 3s
							{
								start_time = XOSTimeGet();
								XOSTimeDly(100);
								hal_wdtClear();
								printer_delay_time = 3;
								photoPrintDlyShow(handle,TRUE,printer_delay_time);
								XOSTimeDly(1000);
								while(1)
								{
									hal_wdtClear();
									space_time = XOSTimeGet()-start_time;
									if(space_time >= 1000)
									{
										printer_delay_time--;
										photoPrintDlyShow(handle,TRUE,printer_delay_time);
										if(0 == printer_delay_time)
										{
											photoPrintDlyShow(handle,false,0);
											break;
										}
										start_time = XOSTimeGet();
									}
								}
							}
							//hal_sysMemPrint();
						}
					}else{
							SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
							photoPrinterTipsShow(handle,false,0,0);
							photoFocusShow(handle);
							drawUIService(true);
					}

					boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
					
//					photoPrinterTipsShow(handle,false,0,0);
//					SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
//					photoFocusShow(handle);
//					drawUIService(true);

					if(0==ret)
					{

						hal_wdtClear();
						//hal_csiEnable(0);						// set lcd image stop
						//dispLayerSetPIPMode(DISP_PIP_DISABLE);	

						

						deg_Printf("before print remain:0x%x\n",hal_sysMemRemain());
						u8 print_ret=1;
						if(SysCtrl.printer_en)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_PRINTING);
							drawUIService(true);
							XOSTimeDly(100);	//insure ui draw completely
							print_ret=printer_jpeg_buf((u8*)jpg_addr,SysCtrl.printer_level,SysCtrl.printer_print_mode,SysCtrl.battery,jpg_size);
							deg_Printf("after print remain:0x%x\n",hal_sysMemRemain());
						}
						if(8==print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_NO_PAPER);
							drawUIService(true);
							XOSTimeDly(2000);
						}
						else if(9==print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_OVER_HEAT);
							drawUIService(true);
							XOSTimeDly(2000);
						}
						else if(15 == print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIP_KEEPQ);
							drawUIService(true);
							XOSTimeDly(2000);
						}
						else if(12 == print_ret)
						{
							photoPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_CHARG_N_PRINT);
							drawUIService(true);
							XOSTimeDly(2000);
						}
						#if ENABLE_FLASH_PHOTO
						if(1 == SysCtrl.photo_err_tip)
						{	//font need updata a str:R_ID_STR_TIPS_SPI_FULL
							uiOpenWindow(&tipsWindow,2,R_ID_STR_TIP_NO_SPACE,3);
						}
						SysCtrl.photo_err_tip = 0;
						#endif
						
						photoPrinterTipsShow(handle,false,0,0);
						drawUIService(true);

						
						//hal_csiEnable(0);						// set lcd image stop
						//dispLayerSetPIPMode(DISP_PIP_DISABLE);
						if(SysCtrl.printer_en)
						{
							#if (2 == SENSOR_NUM)
							sensorInit_table();
							//XOSTimeDly(400);
							#endif
							hal_csiEnable(1);
							dispLayerSetPIPMode(SysCtrl.pip_mode);
							XOSTimeDly(300);
							hal_lcd_get_next_frame();
						}else{
							hal_wdtClear();
							hal_csiEnable(1);
							dispLayerSetPIPMode(SysCtrl.pip_mode);
						}
						//SysCtrl.lcd_frame_stop = 0;
						//hal_lcd_get_next_frame();
						
						if(SysCtrl.spec_color_index)
						{
							cmos_spec_color(SysCtrl.spec_color_index);
						}

					}
						deg_Printf("before free remain:0x%x addr:%x\n",hal_sysMemRemain(),jpg_addr);
						hal_sysMemFree((u8*)jpg_addr);
						watermark_buf_bmp2yuv_free();
						deg_Printf("after free remain:0x%x\n addr:%x",hal_sysMemRemain(),jpg_addr);
						//hal_sysMemPrint();
				}
				else
				{
					if(SysCtrl.sdcard!=SDC_STAT_NORMAL)	// no tf , not print ,tips 
					{
						XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(SYS_EVENT_SDC,0));
					}
				}
  #endif	              
			}

		}
		//return 0;

		photoSurplusQuantityShow(handle);

	}	
#ifdef DOUBLE_CAM
	#if 0
	else if(keyState==KEY_CONTINUE)
	{
		deg_Printf("key continue-photo\n");
		if(photoLongKey == 0)
		{
			photoLongKey = 1;
			//sensor_change();
			SysCtrl.sensor_change_flag =1;
			#if 1
			uint8 flag=0;
			uint16 lcd_w,lcd_h;
			{
				deg_Printf("ready photo interface!\n");
				disp_frame_t * handle_frame=NULL;
				UserInterface recordeAnimation;
				ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)
	
				//SysCtrl.photo_task=1;
				bool change_finir = false;
				//deg_Printf("..z handle:%d\n",SysCtrl.photo_software_handle);
				while(1)
				{
					//deg_Printf("..a handle:%d\n",SysCtrl.photo_software_handle);
					if(0==SysCtrl.photo_software_handle)
					{
						//deg_Printf("..b\n");
						handle_frame=yuv420_software_get_handle_frame();
						if(handle_frame)
						SysCtrl.photo_software_handle=1;
					}
					else if(1==SysCtrl.photo_software_handle)
					{
						//deg_Printf("..c\n");
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
			//SysCtrl.photo_task=0;
			deg_Printf("ready photo interface! finish\n");

			}
			#endif
			if(SysCtrl.spec_color_index) //follow the orig setting
			{
				cmos_spec_color(SysCtrl.spec_color_index);
			}
			
			SysCtrl.sensor_change_flag =0;
		}
		
	}
	#endif
#endif
	return 0;
}



#if 0
static int photoKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
#endif

#if 0
static int photoKeyMsgIr(winHandle handle,uint32 parameNum,uint32* parame)
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
		photoIRShow(handle);
	}
	return 0;
}
#endif

u32 sensor_rotate_flag = 0;
#if 0
static int photoKeyMsgSensorRotate(winHandle handle,uint32 parameNum,uint32* parame)
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


static int photoKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
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


static int photoKeyMsgPlayBack(winHandle handle,uint32 parameNum,uint32* parame)
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


extern INT8U photoPipMode;
//static int photoLongKeyFlag = 0;
static int photoKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		
		deg_Printf("SysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);

		if(0==SysCtrl.photo_mode_switch)	// photo mode 
		{
			if(SysCtrl.photo_mode_idx>0)	// pre photo mode 
			{
				if(SysCtrl.printer_en)
					SysCtrl.photo_mode_idx=0;
				else
					SysCtrl.photo_mode_idx--;
			}
			else							//  swith frame mode
			{
				SysCtrl.photo_mode_switch=3;

				//reset crop
				/*
				SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/

				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);

				SysCtrl.photo_frame_idx = (ICON_FRAME_NUM -1);
				deg_Printf("photo_frame_idx:%d\n",SysCtrl.photo_frame_idx);

				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
		}
		else if(1==SysCtrl.photo_mode_switch)	// spec col mode
		{
			if(SysCtrl.spec_color_index>0)
			{
				SysCtrl.spec_color_index--;
				cmos_spec_color(SysCtrl.spec_color_index);
				if(0==SysCtrl.spec_color_index)// switch photo mode
				{
					//==color normal
					cmos_spec_color(SysCtrl.spec_color_index);

					SysCtrl.photo_mode_switch=0;
					if(SysCtrl.printer_en)
						SysCtrl.photo_mode_idx=0;//7
					else
						SysCtrl.photo_mode_idx=6;//8
				}
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx>0)
			{
				//SysCtrl.photo_effect_idx--;
				if(SysCtrl.photo_effect_idx == 7)
					SysCtrl.photo_effect_idx = 6;
				else if(SysCtrl.photo_effect_idx == 6)
					//SysCtrl.photo_effect_idx = 5;
					SysCtrl.photo_effect_idx = 4;
				else if(SysCtrl.photo_effect_idx == 5)
					SysCtrl.photo_effect_idx = 4;
				else if(SysCtrl.photo_effect_idx == 4)
					SysCtrl.photo_effect_idx = 1;
				else if(SysCtrl.photo_effect_idx == 1)
					SysCtrl.photo_effect_idx = 0;
			}
			else
			{
				//==close effect==
				//==switch spec color mode==
				SysCtrl.photo_mode_switch=1;
				SysCtrl.spec_color_index=5;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx>0)
			{
				SysCtrl.photo_frame_idx--;
				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				//SysCtrl.photo_frame_idx = ICON_FRAME_NUM -1;

				// switch effect mode 
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=CAM_EFFECT_MAX-2;
				
				//reset crop
				/*SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/
			}
		}
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8)){
			if(SysCtrl.crop_level != 0){
			    SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
				hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				photoZoomRatioShow(handle);
			}
		}

		photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		deg_Printf("S2222ysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);

	/*	if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level+=1;
	    if(SysCtrl.crop_level > 15)
			SysCtrl.crop_level=15;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);  */
		if((SysCtrl.photo_mode_switch==2 )|| (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		deg_Printf("before crop:%d\t",SysCtrl.crop_level);
		SysCtrl.crop_level+=1;
	    if(SysCtrl.crop_level > 5)
			SysCtrl.crop_level=5;
		deg_Printf("aftercrop:%d\t",SysCtrl.crop_level);
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}
	
	//deg_Printf("UP--SysCtrl.photo_effect_idx:%d--\n",SysCtrl.photo_effect_idx);
	return 0;
}
static int photoKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW)
		{
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}

		
		if(0==SysCtrl.photo_mode_switch)	// photo mode 
		{
			if(SysCtrl.photo_mode_idx<6)	// 8next photo mode 
			{
				if(SysCtrl.printer_en)
				{
					//if(SysCtrl.photo_mode_idx == 7)						//	swith spec color mode
					//{
						//==close photo mode show==
						SysCtrl.photo_mode_idx=0;
						
						SysCtrl.photo_mode_switch=1;
						SysCtrl.spec_color_index=1;
						cmos_spec_color(SysCtrl.spec_color_index);
					//}
					//else
					//	SysCtrl.photo_mode_idx = 7;
				}
				else
				{
					SysCtrl.photo_mode_idx++;
				}
			}
			else							//	swith spec color mode
			{
				//==close photo mode show==
				SysCtrl.photo_mode_idx=0;
				
				SysCtrl.photo_mode_switch=1;
				SysCtrl.spec_color_index=1;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
		}
		else if(1==SysCtrl.photo_mode_switch)	// spec col mode
		{
			if(SysCtrl.spec_color_index<5)
			{
				SysCtrl.spec_color_index++;
				cmos_spec_color(SysCtrl.spec_color_index);
			}
			else	// switch effect mode
			{
				//==normal color==
				SysCtrl.spec_color_index=0;
				cmos_spec_color(SysCtrl.spec_color_index);
				//==
				SysCtrl.photo_mode_switch=2;
				SysCtrl.photo_effect_idx=0;

				deg_Printf("photo_mode_switch=%d,idx=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_effect_idx);
			}
		}
		else if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
			if(SysCtrl.photo_effect_idx<CAM_EFFECT_MAX-2)
			{
				
				{
					//SysCtrl.photo_effect_idx++;
					if(SysCtrl.photo_effect_idx == 0)
						SysCtrl.photo_effect_idx = 1;
					else if(SysCtrl.photo_effect_idx == 1)
						SysCtrl.photo_effect_idx = 4;
					else if(SysCtrl.photo_effect_idx == 4)
						//SysCtrl.photo_effect_idx = 5;
						SysCtrl.photo_effect_idx = 6;
					else if(SysCtrl.photo_effect_idx == 5)
						SysCtrl.photo_effect_idx = 6;
					//else if(SysCtrl.photo_effect_idx == 6)
					//	SysCtrl.photo_effect_idx = 7;

				}

			}
			else
			{
				//==close effect==
	
				//==switch frame mode==
				SysCtrl.photo_mode_switch=3;

				//reset crop level , don't set this when frame mode and effect mode 
				/*SysCtrl.crop_level =0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				photoZoomRatioShow(handle);
				*/
				
				hal_custom_frame_add_enable(1);
				videoRecordCmdSet(CMD_COM_FRAME,1);



				SysCtrl.photo_frame_idx = 0;
				deg_Printf("frame_index:%d\n",SysCtrl.photo_frame_idx);

				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
		}
		else if(3==SysCtrl.photo_mode_switch)	// frame mode
		{
			if(SysCtrl.photo_frame_idx<ICON_FRAME_NUM-1)
			{
				SysCtrl.photo_frame_idx++;
				if(hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx])<0)
				{
					deg_Printf(">>frame err!!<<\n");
				}
			}
			else	
			{
				//==close frame==
				videoRecordCmdSet(CMD_COM_FRAME,0);
				hal_custom_frame_add_enable(0);
				hal_custom_frame_unit();
				// switch photo mode 
				SysCtrl.photo_mode_switch=0;
				SysCtrl.photo_mode_idx=0;
			}
		}
		
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8)){
			if(SysCtrl.crop_level != 0){
			    SysCtrl.crop_level=0;
				dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
				//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
				hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
				photoZoomRatioShow(handle);
			}
		}

		photo_mode_show(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		/*if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);*/
		if(((SysCtrl.photo_mode_switch==2 )) || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		deg_Printf("before crop:%d\t",SysCtrl.crop_level);
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		deg_Printf("after crop:%d\n",SysCtrl.crop_level);
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}
	//deg_Printf("DOWN--SysCtrl.photo_effect_idx:%d--\n",SysCtrl.photo_effect_idx);

	return 0;
}

static int photoKeyMsgUp2(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		/*
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		*/
		SysCtrl.crop_level+=1;
	    if(SysCtrl.crop_level > 5)
			SysCtrl.crop_level=5;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			//photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			//return 0;
		}
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}
	else if(keyState==KEY_CONTINUE)
	{
		deg_Printf("S2222ysCtrl.photo_mode_switch:=%d=%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);

		if((SysCtrl.photo_mode_switch==2 ||SysCtrl.photo_mode_switch==3)|| (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level+=1;
	    if(SysCtrl.crop_level > 5)
			SysCtrl.crop_level=5;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}

	return 0;
}
static int photoKeyMsgDown2(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_RELEASE)
	{
		/*
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			return 0;
		}
		*/
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		if(SysCtrl.photo_focus == PHOTO_FOCUS_ICON_YELLOW){
			photo_focus_show_count = XOSTimeGet();
			SysCtrl.photo_focus = PHOTO_FOCUS_ICON_RED;
			//photoFocusShow(handle);
			deamon_sound_play((u32)focus_sound, focus_sound_size);
			//return 0;
		}
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
		
	}
	else if(keyState==KEY_CONTINUE)
	{
		if(SysCtrl.photo_mode_switch==2 || (SysCtrl.photo_mode_idx == 7 || SysCtrl.photo_mode_idx== 8))
			return 0;
		SysCtrl.crop_level-=1;
		if(SysCtrl.crop_level > 200)
	    	SysCtrl.crop_level=0;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 0);
		if(SysCtrl.photo_mode_switch==3)
			//hal_custom_frame_create_zoom(R_FRAME[SysCtrl.photo_frame_idx]);
			hal_custom_frame_create(R_FRAME[SysCtrl.photo_frame_idx]);
		SysCtrl.photo_focus = PHOTO_FOCUS_ICON_YELLOW;
		photoFocusShow(handle);
		photoZoomRatioShow(handle);
	}

	return 0;
}

extern menu MENU(movie);
#if 0
static int photoKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		uiOpenWindow(&menuItemWindow,1,&MENU(movie));

	}
	return 0;
}
#endif


static int photoKeyMsgPrinterEn(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	deg_Printf("msg Print_en photo_mode_switch:%d;photo_mode_idx:%d\n",SysCtrl.photo_mode_switch,SysCtrl.photo_mode_idx);
	

#if 1
		if(SysCtrl.printer_en)
		{
			SysCtrl.printer_en = 0;
			configSet(CONFIG_ID_PRINTER_EN,R_ID_STR_COM_OFF);
		}
		else
		{
			SysCtrl.printer_en = 1;
			configSet(CONFIG_ID_PRINTER_EN,R_ID_STR_COM_ON);
			if(0==SysCtrl.photo_mode_switch)	// photo mode
			{
				if(SysCtrl.photo_mode_idx>0)
				{
					SysCtrl.photo_mode_idx=0;
				}
			}
		}
					
		
		userConfigSave();
		
		photoPrinterShow(handle);
		photo_mode_show(handle);
		
#endif
		
	}
	return 0;
}

static int photoKeyMsgSensorChange(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
		if(parameNum==1)
			keyState=parame[0];
		if(keyState==KEY_PRESSED)
		{
			/*if(P_sensorChanegeCtrl)
			{
				P_sensorChanegeCtrl = false;
				return 0;
			}*/
			deg_Printf("key continue-photo\n");
			SysCtrl.sensor_change_flag =1;
#if 1
			uint8 flag=0;
			uint16 lcd_w,lcd_h;
		
			deg_Printf("ready photo interface!\n");
			disp_frame_t * handle_frame=NULL;
			UserInterface recordeAnimation;
			ANIMATION(recordeAnimation, SQUARE_INSIDE2OUTSIDE)

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
		deg_Printf("ready photo interface! finish\n");

#endif
		if(SysCtrl.spec_color_index) //follow the setting
		{
			cmos_spec_color(SysCtrl.spec_color_index);
		}
		
		SysCtrl.sensor_change_flag =0;
	
	
}

}
static int photoSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoSDShow(handle);
	photoRemainNumShow(handle);
	#if ENABLE_FLASH_PHOTO
	photoImagePreSizeGet();
	#endif
	photoSurplusQuantityShow(handle);
	if(SysCtrl.sdcard == SDC_STAT_NULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_NOT_INSERT,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_FULL)
		uiOpenWindow(&tips1Window,2,TIPS_SD_FULL,SDC_NULL_TIMEOUT);
	else if(SysCtrl.sdcard == SDC_STAT_ERROR)
		uiOpenWindow(&tips1Window,2,TIPS_SD_ERROR,SDC_NULL_TIMEOUT);
	deg_Printf("SD state:%d\n",SysCtrl.sdcard);
	return 0;
}
static int photoSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	photoBaterryShow(handle);
	return 0;
}
static int photoSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		photoBaterryShow(handle);
	return 0;
}
static int photoSysMsgMD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

extern bool key_focus_model;
static int photoOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo Open Win!!!\n");
		SysCtrl.photo_focus=PHOTO_FOCUS_ICON_NONE;
	photoFocusShow(handle);
	layout_version_get();
//	#if SMALL_PANEL_SUPPORT==0
//	photoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif

	photoPrinterTipsShow(handle,false,0,0);

	photoResolutionShow(handle);
	photoMDShow(handle);
	photoMonitorShow(handle);
	photoIrLedShow(handle);
	photoLockShow(handle,false);
	photoSDShow(handle);
	photoMicShow(handle);
	photoBaterryShow(handle);
	photoRemainNumShow(handle);
	photo_timephoto(handle,true);
//	photo_morephoto(handle,true);
	photo_mode_show(handle);

	photo_time_num_show(handle,false,0);
	
	photoPrinterShow(handle);

	photoSurplusQuantityShow(handle);

	//photoPrintDlyShow(handle,false,0);

	key_focus_model = true;
	P_sensorChanegeCtrl = true;
	//SysCtrl.photo_focus = PHOTO_FOCUS_ICON_NONE;
	//photoFocusShow(handle);
	SysCtrl.photo_focus=PHOTO_FOCUS_ICON_NONE;
	photoFocusShow(handle);
	focus_sound = layout_sound_load(RES_MUSIC_PHOTO_FOCUS, &focus_sound_size);

	photoIRShow(handle);

/*
	if(0==configGet(CONFIG_ID_LANGUAGE))
	{
		SysCtrl.first_menu_lan=1;
		uiOpenWindow(&menuItemWindow,2,&MENU(movie),8);// language idx
	}
*/
	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}

	return 0;
}
static int photoCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo Close Win!!!\n");
	boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
	key_focus_model = false;
	if(focus_sound){
		hal_sysMemFree(focus_sound);
		focus_sound = 0;
	}
	hal_custom_frame_unit();
	return 0;
}
static int photoWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("photo WinChild Close!!!\n");
	photoRemainNumShow(handle);
	#if ENABLE_FLASH_PHOTO
	photoImagePreSizeGet();
	#endif
	photoResolutionShow(handle);
	photoMDShow(handle);
	photoMonitorShow(handle);
	photoIrLedShow(handle);
	photoLockShow(handle,false);
	photoSDShow(handle);
	photoMicShow(handle);
	photoBaterryShow(handle);
	photo_timephoto(handle,true);
//	photo_morephoto(handle,true);
	photo_time_num_show(handle,false,0);
	photoPrinterTipsShow(handle,false,0,0);
	photoPrinterShow(handle);
//	#if SMALL_PANEL_SUPPORT==0
//	photoPoweOnTimeShow(handle,SysCtrl.powerOnTime);
//	#endif

	return 0;
}

#if 0
static void VotageShow(winHandle handle)
{

	static char picSumStr[2];
		u8 temp = hal_gpioRead(GPIO_PA, GPIO_PIN7);
		picSumStr[0] = ((temp)%10)+'0';
		picSumStr[1] = 0;
		//intToString(&picSumStr,5,/*MANAGER_LIST_MAX-*/picSum);
		winSetResid(winItem(handle,TEST_VOTAGE_ID),strToResId(picSumStr));

}
#endif

static int photoSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
//	static uint32 flag=0;

	u8 timestramp;
	
	timestramp = configValue2Int(CONFIG_ID_TIMESTAMP);
	if(timestramp == 1 )
	{
		winSetResid(winItem(handle,PHOTO_SYSTIME_ID),strToResId(hal_rtcTime2String(hal_rtcTimeGet())));
		winSetVisible(winItem(handle,PHOTO_SYSTIME_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,PHOTO_SYSTIME_ID),false);
	}	
	
	//photoIrLedShow(handle);
//  VotageShow(handle);
//	photo_timephoto(handle,true);
//	photo_morephoto(handle,true);
	if(SysCtrl.usb == USB_STAT_DCIN)
	{

		photoBaterryShow(handle);	
	}
	/*static u8 printer_en_flag = 0;
	printer_en_flag++;
	if(printer_en_flag == 10){
		printer_en_flag = 0;
		if(SysCtrl.printer_en)
			XMsgQPost(SysCtrl.sysQ,(void*)makeEvent(KEY_EVENT_PHOTO,0));
	}*/
	
	drawUIService(true);

	return 0;
}



msgDealInfor photoEncodeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,photoOpenWin},
	{SYS_CLOSE_WINDOW,photoCloseWin},
	{SYS_CHILE_COLSE,photoWinChildClose},
	
	{KEY_EVENT_PHOTO,photoKeyMsgPhoto},//设置打印按键
	
	{KEY_EVENT_UP,photoKeyMsgUp2},
	{KEY_EVENT_DOWN,photoKeyMsgDown2},

	{KEY_EVENT_LEFT,photoKeyMsgUp},
	{KEY_EVENT_RIGHT,photoKeyMsgDown},
	{KEY_EVENT_RECORD,photoKeyMsgPhoto},
	{KEY_EVENT_RETURN,photoKeyMsgExit},
	// {KEY_EVENT_OK,photoKeyMsgSensorChange},
	//{KEY_EVENT_ROTATE,photoKeyMsgSensorRotate},
	
	{KEY_EVENT_PLAYBACK,photoKeyMsgPlayBack},
	{KEY_EVENT_PRINTER_EN,photoKeyMsgPrinterEn},
	{SYS_EVENT_SDC,photoSysMsgSD},
	{SYS_EVENT_USB,photoSysMsgUSB},
	{SYS_EVENT_BAT,photoSysMsgBattery},
	{SYS_EVENT_MD,photoSysMsgMD},
	{SYS_EVENT_1S,photoSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(photoEncodeWindow,photoEncodeMsgDeal,photoEncodeWin)



