#include "uiMenuVolumeRes.c"

static u8 sel_idx;

#define VOLUME_ICON_NUM	VOLUME_LEVEL
#define VOLUME_ICON_W 	10
#define VOLUME_ICON_H 	20

static u8* volume_black_buf;
static u8* volume_white_buf;
static u16 volume_w,volume_h;

static disp_frame_t *p_lcd_buffer;

static void settingVolumeSelShow(disp_frame_t *p_lcd_buffer,u8 idx)
{
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		u8 i;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		//==draw black==
		for(i=0;i<VOLUME_ICON_NUM;i++)
		{
			yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,84+i*16,116,volume_w,volume_h,volume_black_buf,volume_w,volume_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		}
		
		//==draw white==
		for(i=0;i<idx;i++)
		{
			yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,84+i*16,116,volume_w,volume_h,volume_white_buf,volume_w,volume_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		}

		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);

	}
	else
	{
		deg_Printf("video buf null!\n");
	}

}


static int volumeKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		configSet(CONFIG_ID_VOLUME,sel_idx);
		winDestroy(&handle);
	}
	return 0;
}

static int volumeKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}

static int volumeKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx<VOLUME_ICON_NUM)
		{
			sel_idx++;
		}

		SysCtrl.volume_level=sel_idx;
		hal_dacSetVolume(sel_idx*VOLUME_ICON_NUM);
		settingVolumeSelShow(p_lcd_buffer,sel_idx);
		configSet(CONFIG_ID_VOLUME,sel_idx);
	}
	return 0;
}

static int volumeKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		if(sel_idx>0)
		{
			sel_idx--;
		}
		SysCtrl.volume_level=sel_idx;
		hal_dacSetVolume(sel_idx*VOLUME_ICON_NUM);
		settingVolumeSelShow(p_lcd_buffer,sel_idx);
		configSet(CONFIG_ID_VOLUME,sel_idx);
	}
	return 0;
}

static int volumeOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
/*	u8 *rgb24_buf;
	sel_idx=SysCtrl.volume_level;
	deg_Printf("volume:sel_idx=%d\n",sel_idx);
	p_lcd_buffer=setting_showbuf(RES_GAMEMENU_BK,RES_SETTING_VOLUME);


	volume_w=(VOLUME_ICON_W+0x1)&(~0x1);	// bmp must 2pixel align
	volume_h=(VOLUME_ICON_H+0x1)&(~0x1);
	deg_Printf("volume_w=%d,volume_h=%d\n",volume_w,volume_h);

	rgb24_buf=hal_sysMemMalloc(volume_w*volume_h*3,32);
	if(NULL==rgb24_buf)
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	volume_black_buf=hal_sysMemMalloc(volume_w*volume_h*3/2,32);
	if(NULL!=volume_black_buf)
	{
		bmp24_to_yuv420_buf(RES_SETTING_VOLUME_BLACK_BAR,rgb24_buf,volume_black_buf,volume_black_buf+volume_w*volume_h,volume_w,volume_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	volume_white_buf=hal_sysMemMalloc(volume_w*volume_h*3/2,32);
	if(NULL!=volume_white_buf)
	{
		bmp24_to_yuv420_buf(RES_SETTING_VOLUME_WHITE_BAR,rgb24_buf,volume_white_buf,volume_white_buf+volume_w*volume_h,volume_w,volume_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	if(NULL!=rgb24_buf)
	{
		hal_sysMemFree(rgb24_buf);
		rgb24_buf=NULL;
	}
	XOSTimeDly(30);
	settingVolumeSelShow(p_lcd_buffer,sel_idx);

	
	//==show osd==
	settingVolumeBaterryShow(handle);
*/

	return 0;
}

static int volumeCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(NULL!=volume_black_buf)
	{
		hal_sysMemFree(volume_black_buf);
		volume_black_buf=NULL;
	}
	
	if(NULL!=volume_white_buf)
	{
		hal_sysMemFree(volume_white_buf);
		volume_white_buf=NULL;
	}

	return 0;
}
static int volumeWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int volumeSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_DCIN)
	{
		settingVolumeBaterryShow(handle);	
	}
	return 0;
}


msgDealInfor volumeMsgDeal[]=
{
	{SYS_OPEN_WINDOW,volumeOpenWin},
	{SYS_CLOSE_WINDOW,volumeCloseWin},
	{SYS_CHILE_COLSE,volumeWinChildClose},
	{KEY_EVENT_OK,volumeKeyMsgOk},
	{KEY_EVENT_RETURN,volumeKeyMsgMenu},
	{KEY_EVENT_RIGHT,volumeKeyMsgUp},
	{KEY_EVENT_LEFT,volumeKeyMsgDown},
	{SYS_EVENT_1S,volumeSysMsg1S},

	{EVENT_MAX,NULL},
};

WINDOW(volumeWindow,volumeMsgDeal,volumeWin)


