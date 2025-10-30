#include "application.h"
#include "taskMainMenuRes.c"

#define MAINMENU_TASK_NUM 		6
#define MAINMENU_PIXEL_W 		96
#define MAINMENU_PIXEL_H 		96



static u32 mainmenu_show_time;
static u8 show_bk;
#if BT_MODE//MAIN_MENU_UI
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_APP,
								 RES_MAINMENU_PLAYBACK,RES_MAINMENU_GAME,RES_MAINMENU_SETTING,
								};
//static u8* mainmenu_id_buf;//[MAINMENU_TASK_NUM];

#else
static u32 mainmenu_bmp_id[MAINMENU_TASK_NUM]={RES_MAINMENU_PHOTO,RES_MAINMENU_VIDEO,RES_MAINMENU_AUDIO,
								 RES_MAINMENU_PLAYBACK,RES_MAINMENU_GAME,RES_MAINMENU_SETTING,
								};
//static u8* mainmenu_id_buf[MAINMENU_TASK_NUM];

#endif

#if MAIN_MENU_UI
	static u8* mainmenu_id_buf;
#else
	static u8* mainmenu_id_buf[MAINMENU_TASK_NUM];
#endif

static u8* mainmenu_bk_buf;
static u16 bk_w,bk_h;


static u16 id_w,id_h;


typedef	struct
{
	u16 x;
	u16 y; 
}MainMenu_Pos;

#if !MAIN_MENU_UI
static MainMenu_Pos mainMenu_showpos[MAINMENU_TASK_NUM]=
{
//==two pixel align==
	{12,18},	
	{12+100,18},
	{12+100+100,18},
	{12,18+108},
	{12+100,18+108},
	{12+100+100,18+108},
};
#endif

static void mainMenu_showbuf(u8 bk)	// bk  0: show idx , 1: show bk 
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		if(0==bk)
		{
			//
			#if MAIN_MENU_UI
			memcpy(p_lcd_buffer->y_addr,mainmenu_id_buf/*[SysCtrl.mainmenu_taskidx]*/,id_w*id_h*3/2);
			#else
			memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
			yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,mainMenu_showpos[SysCtrl.mainmenu_taskidx].x,mainMenu_showpos[SysCtrl.mainmenu_taskidx].y,id_w,id_h,mainmenu_id_buf[SysCtrl.mainmenu_taskidx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			#endif
			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
		}
		else
		{
			memcpy(p_lcd_buffer->y_addr,mainmenu_bk_buf,bk_w*bk_h*3/2);
			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
		}
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}


static int mainMenuKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_RELEASE)
	{
		mainMenuStrShow(handle,false);
		drawUIService(true);		// clean str
		
		taskStart(TASK_PHOTO_ENCODE,0);
		//taskStart(TASK_APP,0);
	}
	return 0;
}


static int mainMenuKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		mainMenuStrShow(handle,false);
		drawUIService(true);		// clean str
		
		if(0==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_PHOTO_ENCODE,0);
		}
		else if(1==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_VIDEO_RECORD,0);
		}
		else if(2==SysCtrl.mainmenu_taskidx)
		{
			#if BT_MODE//MAIN_MENU_UI
			//taskStart(TASK_PLAY_BACK,0);
			//taskStart(TASK_AUDIO_PLAYER,0);
			taskStart(TASK_APP,0);
			#else
			taskStart(TASK_AUDIO_PLAYER,0);
			#endif

		}
		else if(3==SysCtrl.mainmenu_taskidx)
		{
			
			//filelist_build();
			#if MAIN_MENU_UI
			//taskStart(TASK_APP,0);
			taskStart(TASK_PLAY_BACK,0);
			#else
			taskStart(TASK_PLAY_BACK,0);
			#endif
			if(0==SysCtrl.cartoon_mode)
			{
				SysCtrl.cartoon_mode=1;
				SysCtrl.cartoon_show_cnt=0;
			}
		}
		else if(4==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_GAME_MENU,0);
		}
		else if(5==SysCtrl.mainmenu_taskidx)
		{
			taskStart(TASK_SETTING_MENU,0);
		}

		deg_Printf("id_idx:%d\n",SysCtrl.mainmenu_taskidx);
	}
	return 0;
}


static int mainMenuKeyMsgPlayback(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		mainMenuStrShow(handle,false);
		drawUIService(true);		// clean str
		
		taskStart(TASK_PLAY_BACK,0);

		deg_Printf("id_idx:%d\n",SysCtrl.mainmenu_taskidx);
	}
	return 0;
}

static int mainMenuKeyMsgLeft(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx>0)
		{
			SysCtrl.mainmenu_taskidx--;
		}
		else
		{
			SysCtrl.mainmenu_taskidx=(MAINMENU_TASK_NUM-1);
		}
		#if MAIN_MENU_UI
		jpg_decode_buf(mainmenu_bmp_id[SysCtrl.mainmenu_taskidx],mainmenu_id_buf,mainmenu_id_buf+id_w*id_h,id_w,id_h);
		#endif
		show_bk=0;
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=XOSTimeGet();

	}
	return 0;
}

static int mainMenuKeyMsgRight(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx<(MAINMENU_TASK_NUM-1))
		{
			SysCtrl.mainmenu_taskidx++;
		}
		else
		{
			SysCtrl.mainmenu_taskidx=0;
		}
		#if MAIN_MENU_UI
		jpg_decode_buf(mainmenu_bmp_id[SysCtrl.mainmenu_taskidx],mainmenu_id_buf,mainmenu_id_buf+id_w*id_h,id_w,id_h);
		#endif
		show_bk=0;
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=XOSTimeGet();
	}
	return 0;
}


static int mainMenuKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx>2)
		{
			SysCtrl.mainmenu_taskidx-=3;
		}
		else
		{
			SysCtrl.mainmenu_taskidx+=3;
		}
		#if MAIN_MENU_UI
		jpg_decode_buf(mainmenu_bmp_id[SysCtrl.mainmenu_taskidx],mainmenu_id_buf,mainmenu_id_buf+id_w*id_h,id_w,id_h);
		#endif
		show_bk=0;
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=XOSTimeGet();

	}
	return 0;
}

static int mainMenuKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		if(SysCtrl.mainmenu_taskidx>2)
		{
			SysCtrl.mainmenu_taskidx-=3;
		}
		else
		{
			SysCtrl.mainmenu_taskidx+=3;
		}
		#if MAIN_MENU_UI
		jpg_decode_buf(mainmenu_bmp_id[SysCtrl.mainmenu_taskidx],mainmenu_id_buf,mainmenu_id_buf+id_w*id_h,id_w,id_h);
		#endif
		show_bk=0;
		mainMenu_showbuf(show_bk);
		mainmenu_show_time=XOSTimeGet();
	}
	return 0;
}


static int mainMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{



	deg_Printf("main menu Open Win:%d\n",XOSTimeGet());
	//hal_wdtClear();
#if !MAIN_MENU_UI
	u8 i;
	u8 *rgb24_buf;
#endif
	show_bk=0;
	if(SysCtrl.mainmenu_taskidx>=MAINMENU_TASK_NUM)
	{
		SysCtrl.mainmenu_taskidx=0;
	}
	deg_Printf("SysCtrl.mainmenu_taskidx=%d\n",SysCtrl.mainmenu_taskidx);
	//==init==
	hal_lcdGetResolution(&bk_w,&bk_h);
	mainmenu_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,64);
	if(NULL!=mainmenu_bk_buf)
	{
		jpg_decode_buf(RES_MAINMENU_BK,mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		//jpg_decode_buf(RES_MAINMENU_BK,mainmenu_bk_buf,mainmenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
		
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	//
	hal_lcdGetResolution(&id_w,&id_h);
#if !MAIN_MENU_UI	
	id_w=(MAINMENU_PIXEL_W+0x1)&(~0x1);	// bmp must 2pixel align
	id_h=(MAINMENU_PIXEL_H+0x1)&(~0x1);
	

	deg_Printf("bk_w=%d,bk_h=%d,id_w=%d,id_h=%d\n",bk_w,bk_h,id_w,id_h);
	rgb24_buf=hal_sysMemMalloc(id_w*id_h*3,32);
	if(NULL==rgb24_buf)
	{
		deg_Printf("mem err!\n");
		return 0;
	}
#endif
deg_Printf("\n\n");
//hal_sysMemPrint();
deg_Printf("\n\n");

#if !MAIN_MENU_UI
	for(i=0;i<MAINMENU_TASK_NUM;i++)
	{
		mainmenu_id_buf[i]=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(NULL!=mainmenu_id_buf[i])
		{
			//
			bmp24_to_yuv420_buf(mainmenu_bmp_id[i],rgb24_buf,mainmenu_id_buf[i],mainmenu_id_buf[i]+id_w*id_h,id_w,id_h);
			//deg_Printf("id=%d,first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",i,*mainmenu_id_buf[i],*(mainmenu_id_buf[i]+id_w*id_h),*(mainmenu_id_buf[i]+1),*(mainmenu_id_buf[i]+id_w*id_h+1));
			deg_Printf("res id :%d , i :%d",mainmenu_bmp_id[i],i);
		}
		else
		{
			deg_Printf("mem err!\n");
			return 0;
		}
	}
#else
	mainmenu_id_buf=hal_sysMemMalloc(id_w*id_h*3/2,64);
	if(NULL!=mainmenu_id_buf)
	{
		jpg_decode_buf(mainmenu_bmp_id[SysCtrl.mainmenu_taskidx],mainmenu_id_buf,mainmenu_id_buf+id_w*id_h,id_w,id_h);
		jpg_decode_buf(mainmenu_bmp_id[SysCtrl.mainmenu_taskidx],mainmenu_id_buf,mainmenu_id_buf+id_w*id_h,id_w,id_h);

		
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}
#endif

#if !MAIN_MENU_UI
	if(NULL!=rgb24_buf)
	{
		hal_sysMemFree(rgb24_buf);
		rgb24_buf=NULL;
	}
#endif

	//==show==
	mainMenu_showbuf(show_bk);
	mainmenu_show_time=XOSTimeGet();
	deg_Printf("show time:%d\n",XOSTimeGet());

	mainMenuStrShow(handle,false);
	mainMenuBaterryShow(handle);

	return 0;
}

static int mainMenuCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("main menu Close Win!!!\n");
	if(NULL!=mainmenu_bk_buf)
	{
		hal_sysMemFree(mainmenu_bk_buf);
		mainmenu_bk_buf=NULL;
	}
#if !MAIN_MENU_UI
	u8 i;
	for(i=0;i<MAINMENU_TASK_NUM;i++)
	{
		if(NULL!=mainmenu_id_buf[i])
		{
			hal_sysMemFree(mainmenu_id_buf[i]);
			mainmenu_id_buf[i]=NULL;
		}
	}
#else
	hal_sysMemFree(mainmenu_id_buf);
	mainmenu_id_buf=NULL;
#endif

	return 0;
}

static int mainMenuSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(XOSTimeGet()<mainmenu_show_time+300)	// too short not show it
	{
		return 0;
	}

	if(show_bk)
	{
		show_bk=0;
	}
	else
	{
		show_bk=1;
	}
	mainMenu_showbuf(show_bk);
	mainmenu_show_time=XOSTimeGet();
	return 0;
}
volatile INT32U *temp_a;

static int mainMenuSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{

	if(SysCtrl.usb == USB_STAT_DCIN)
	{
		mainMenuBaterryShow(handle);	
	}

	return 0;
}

static int mainMenuSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		mainMenuBaterryShow(handle);
	return 0;
}
static int mainMenuSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		mainMenuBaterryShow(handle);
	return 0;
}

msgDealInfor mainMenuMsgDeal[]=
{
	{SYS_OPEN_WINDOW,mainMenuOpenWin},
	{SYS_CLOSE_WINDOW,mainMenuCloseWin},
	{KEY_EVENT_UP,mainMenuKeyMsgUp},
	{KEY_EVENT_DOWN,mainMenuKeyMsgDown},
	{KEY_EVENT_LEFT,mainMenuKeyMsgLeft},
	{KEY_EVENT_RIGHT,mainMenuKeyMsgRight},
	{KEY_EVENT_PLAYBACK,mainMenuKeyMsgPlayback},
	{KEY_EVENT_RECORD,mainMenuKeyMsgPhoto},
	{KEY_EVENT_OK,mainMenuKeyMsgOk},
	{SYS_EVENT_500MS,mainMenuSysMsg500MS},
	{SYS_EVENT_1S,mainMenuSysMsg1S},
	{SYS_EVENT_USB,mainMenuSysMsgUSB},
	{SYS_EVENT_BAT,mainMenuSysMsgBattery},
	{EVENT_MAX,NULL},
};

WINDOW(mainMenuWindow,mainMenuMsgDeal,mainMenuWin)



