#include "application.h"
#include "taskAppRes.c"
#include "../multimedia/btcom/inc/btcom_user.h"
#include "../device/printer/printer.h"
#include "../multimedia/btcom/inc/btcom.h"
#define APPQR_PIXEL_W 		129
#define APPQR_PIXEL_H 		129


//static 	u8 *rgb24_buf;

//static u8* app_qrin_bk_buf;
//static u8* app_qrout_bk_buf;
//static u16 bk_w,bk_h;

//static u8* app_id_buf[2];
//static u16 id_w,id_h;

extern int task_image_createfile(int channel,char **filename);


static int AppKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	int ret;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		
	}
	return 0;
}
// extern u8 tmpdata[1024*300];
//extern u8 *tmpdata;

void GBKToHex(const char* str) {
     unsigned char* p = (unsigned char*)str;
    while (*p) {
        deg_Printf("%x ", *p);
        p++;
    }
    deg_Printf("\n");
}


static int AppKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		#if WRITE_CARD_TEST
		deg_Printf("begin write card\n");
		char *name;
		int fHandle;
		fHandle = task_image_createfile(VIDEO_CH_A,&name);
		
		write(fHandle,(void *)get_pic_add(),get_pic_w());
		deg_Printf(" test addr:%x Size:%d\n",get_pic_add(),get_pic_w());
		deamon_fsSizeModify(-1,fs_size(fHandle));
		
		close(fHandle);
		deg_Printf("write card over!\n");

		memset(get_pic_add(),0,JPG_LEN_MAX);
		reset_pic_data();
		#else
		Bt_Get_Printer_State(NULL,0);
		//bt_AT_Send();
		//checkBtbuf();
		//CleanISR_Btbuf();
		deg_Printf("change again\n");
		//checkBtbuf();
		#endif
	}
	return 0;
}

static int AppKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		deg_Printf("show des 1 %d\n", configValue2Int(/*R_ID_STR_COM_LEVEL_4*/CONFIG_ID_BAT_OLD));

	}
	return 0;
}

static int AppKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		deg_Printf("show des 5 %d\n", configValue2Int(/*R_ID_STR_COM_LEVEL_5*/CONFIG_ID_BAT_OLD));


	}
	return 0;
}

#if 0
static int AppKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int AppKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
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

static void App_showbuf()
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		jpg_decode_buf(RES_APP_QR,p_lcd_buffer->y_addr,p_lcd_buffer->uv_addr,lcd_w,lcd_h);

		//==draw bk==
		/*if(R_ID_STR_LAN_SCHINESE == configGet(CONFIG_ID_LANGUAGE))
			memcpy(p_lcd_buffer->y_addr,app_qrin_bk_buf,bk_w*bk_h*3/2);
		else*/
			//memcpy(p_lcd_buffer->y_addr,app_qrout_bk_buf,bk_w*bk_h*3/2);
		
		//==draw QR==
		//yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,95,110,id_w,id_h,app_id_buf[0],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		
		ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}


static int AppOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
#if 0
	hal_lcdGetResolution(&bk_w,&bk_h);
	/*app_qrin_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	if(NULL!=app_qrin_bk_buf)
	{
		jpg_decode_buf(RES_APP_QR,app_qrin_bk_buf,app_qrin_bk_buf+bk_w*bk_h,bk_w,bk_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}*/

	app_qrout_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	if(NULL!=app_qrout_bk_buf)
	{
		jpg_decode_buf(RES_APP_QR,app_qrout_bk_buf,app_qrout_bk_buf+bk_w*bk_h,bk_w,bk_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}
	#endif

/*
	id_w=(APPQR_PIXEL_W+0x1)&(~0x1); // bmp must 2pixel align
	id_h=(APPQR_PIXEL_H+0x1)&(~0x1);
	
	rgb24_buf=hal_sysMemMalloc(id_w*id_h*3,32);
	if(NULL==rgb24_buf)
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	
	if(NULL==app_id_buf[0])
	{
		app_id_buf[0]=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(app_id_buf[0])
		{
			bmp24_to_yuv420_buf(RES_QR_IN,rgb24_buf,app_id_buf[0],app_id_buf[0]+id_w*id_h,id_w,id_h);
		}
	}

	if(NULL==app_id_buf[1])
	{
		app_id_buf[1]=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(app_id_buf[1])
		{
			bmp24_to_yuv420_buf(RES_QR_OUT,rgb24_buf,app_id_buf[1],app_id_buf[1]+id_w*id_h,id_w,id_h);
		}
	}
*/

	
	App_showbuf();
	AppStrShow(handle);
	AppPrinterTipsShow(handle,false,0,0);
	//hal_sysMemPrint();
	//AppQRShow(handle);
	return 0;
}

static int AppCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
//	int i;
#if 0
	if(NULL!=app_qrin_bk_buf)
	{
		hal_sysMemFree(app_qrin_bk_buf);
		app_qrin_bk_buf=NULL;
	}

	if(NULL!=app_qrout_bk_buf)
	{
		hal_sysMemFree(app_qrout_bk_buf);
		app_qrout_bk_buf=NULL;
	}
#endif

/*	for(i=0;i<2;i++)
	{
		if(NULL!=app_id_buf[i])
		{
			hal_sysMemFree(app_id_buf[i]);
			app_id_buf[i]=NULL;
		}
	}

	if(NULL!=rgb24_buf)
	{
		hal_sysMemFree(rgb24_buf);
		rgb24_buf=NULL;
	}
*/
	return 0;
}

static int AppSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	//Bt_Get_Printer_State(NULL,0);
	//deg_Printf("rp:%d line[%d] remain:%d tm[%d]\n",SysCtrl.ready_print,get_pic_w(),get_remainFreme(),hal_adcGetChannel(ADC_CH_PA6));
	return 0;
}

int app_tips_type = 0;
static int AppSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	u8* y_buf;
	u8 ret_a =0;

	if(SysCtrl.ready_print && (XOSTimeGet()-(SysCtrl.ready_print_time)>500))
	{
		//deg_Printf("ticktime:%d time:%d\n",SysCtrl.ready_print_time,XOSTimeGet());
		btcomService();
		//y_buf=get_pic_add();
		//y_buf=get_dot_tmp_add();//get_pic_add();
		//deg_Printf("1s :addr:%x pic_w:%d mode:%d\n",y_buf,get_pic_w(),SysCtrl.printer_print_mode);
		#if 1
		
		ret_a = printer_jpeg_buf((u8*)y_buf,SysCtrl.printer_level,SysCtrl.printer_print_mode,SysCtrl.battery,get_pic_w());
		#else
		if(SysCtrl.printer_print_mode == 0)
		{
			ret_a = printer_jpeg_buf((u8*)y_buf,SysCtrl.printer_level,SysCtrl.printer_print_mode,SysCtrl.battery,get_pic_w());
		}else
		{
			ret_a = printer_app_buf((u8* )y_buf,get_pic_w());
			
		}
		#endif
		deamon_auto_poweroff(1);
		deamon_screen_save(1);
		//hal_sysMemFree((u8*)y_buf);
		//y_buf=NULL;
		#if !WRITE_CARD_TEST
		//memset(y_buf,0,20*1024/*JPG_LEN_MAX*/);
		reset_pic_data();
		#endif
		SysCtrl.ready_print = 0;
	}

	if(ret_a == 8)
	{
		ret_a = 0;
		AppPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_NO_PAPER);
		drawUIService(true);
		XOSTimeDly(2000);				
		AppPrinterTipsShow(handle,false,0,0);
	}
	else if(ret_a == 9)
	{
		ret_a = 0;
		AppPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_OVER_HEAT);
		drawUIService(true);
		XOSTimeDly(2000);				
		AppPrinterTipsShow(handle,false,0,0);
	}
	else if(ret_a == 12)
	{
		ret_a = 0;
		AppPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIPS_CHARG_N_PRINT);
		drawUIService(true);
		XOSTimeDly(2000);				
		AppPrinterTipsShow(handle,false,0,0);
	}
	else if(ret_a == 15)
	{
		ret_a = 0;
		AppPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIP_KEEPQ);
		drawUIService(true);
		XOSTimeDly(2000);				
		AppPrinterTipsShow(handle,false,0,0);
	}else if(ret_a > 0 && ret_a < 8)
	{
		ret_a = 0;
		deg_Printf("app msg :pic err\n");
		AppPrinterTipsShow(handle,true,R_ID_STR_SET_PROMT,R_ID_STR_TIP_PIC_ERR);
		drawUIService(true);
		XOSTimeDly(2000);				
		AppPrinterTipsShow(handle,false,0,0);
	}


	
	return 0;
}

msgDealInfor AppMsgDeal[]=
{
	{SYS_OPEN_WINDOW,AppOpenWin},
	{SYS_CLOSE_WINDOW,AppCloseWin},
	{KEY_EVENT_RIGHT,AppKeyMsgUp},
	{KEY_EVENT_LEFT,AppKeyMsgDown},
//	{KEY_EVENT_MENU,AppKeyMsgMenu},
	{KEY_EVENT_OK,AppKeyMsgOk},
	{KEY_EVENT_RETURN,AppKeyMsgExit},
	{SYS_EVENT_500MS,AppSysMsg500MS},
	{SYS_EVENT_1S,AppSysMsg1S},
	{KEY_EVENT_RECORD,AppKeyMsgPhoto},
	{EVENT_MAX,NULL},
};

WINDOW(AppWindow,AppMsgDeal,AppWin)

