#include "application.h"
#include "taskGameMenuRes.c"
#include "taskgame.h"
#if BT_MODE//MAIN_MENU_UI
#define GAME_NUM 	5//6
#else
#define GAME_NUM 	4
#endif
#define GAMEMENU_PIXEL_W 		120
#define GAMEMENU_PIXEL_H 		120
#define GAMEMENU_BFWARD_W 	48
#define GAMEMENU_BFWARD_H 	32

static 	u8 *rgb24_buf;
#if BT_MODE//MAIN_MENU_UI
//static u32 game_bmp_id[GAME_NUM]={RES_GAMEMENU_MUSIC,RES_GAMEMENU_SNAKE,RES_GAMEMENU_MAZE,RES_GAMEMENU_PUSHBOX,RES_GAMEMENU_RB,RES_GAMEMENU_BOLL};
static u32 game_bmp_id[GAME_NUM]={RES_GAMEMENU_MUSIC,RES_GAMEMENU_SNAKE,RES_GAMEMENU_MAZE,RES_GAMEMENU_PUSHBOX,RES_GAMEMENU_RB/*,RES_GAMEMENU_BOLL*/};
#else
static u32 game_bmp_id[GAME_NUM]={RES_GAMEMENU_SNAKE,RES_GAMEMENU_MAZE,RES_GAMEMENU_PUSHBOX,RES_GAMEMENU_RB/*,RES_GAMEMENU_BOLL*/};
#endif
static u8 game_idx;

static u8* gamemenu_bk_buf;
static u16 bk_w,bk_h;

static u8* gamemenu_id_buf[GAME_NUM];
static u16 id_w,id_h;

static u8* gamemenu_backward_buf;
static u8* gamemenu_forward_buf;
static u16 bfward_w,bfward_h;

//==dir 0: left move, 1: right move==
static void gameMenu_show_move(u8 dir)
{
	disp_frame_t *p_lcd_buffer;
	u16 /*x,*/step;
	u8 next_game_idx;

	if(0==dir)
	{
		if(game_idx<(GAME_NUM-1))
		{
			next_game_idx=game_idx+1;
		}
		else
		{
			next_game_idx=0;
		}
	}
	else
	{
		if(game_idx>0)
		{
			next_game_idx=game_idx-1;
		}
		else
		{
			next_game_idx=GAME_NUM-1;
		}
	}

	if(NULL==gamemenu_id_buf[next_game_idx])
	{
		gamemenu_id_buf[next_game_idx]=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(gamemenu_id_buf[next_game_idx])
		{
			bmp24_to_yuv420_buf(game_bmp_id[next_game_idx],rgb24_buf,gamemenu_id_buf[next_game_idx],gamemenu_id_buf[next_game_idx]+id_w*id_h,id_w,id_h);
			//deg_Printf("id=%d,first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",i,*gamemenu_id_buf[game_idx],*(gamemenu_id_buf[game_idx]+id_w*id_h),*(gamemenu_id_buf[game_idx]+1),*(gamemenu_id_buf[game_idx]+id_w*id_h+1));
		}
	}

	for(step=0;step<11;step++)
	{
		p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
		if(p_lcd_buffer)
		{
			u16 lcd_w,lcd_h;
			hal_lcdGetResolution(&lcd_w,&lcd_h);
			//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
			hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

			//==draw bk==
			memcpy(p_lcd_buffer->y_addr,gamemenu_bk_buf,bk_w*bk_h*3/2);
			if(0==dir)
			{
				//==draw game cur==
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,100-step*20,60,id_w,id_h,gamemenu_id_buf[game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw game next==
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,100+220-step*20,60,id_w,id_h,gamemenu_id_buf[next_game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else
			{
				//==draw game cur==
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,100+step*20,60,id_w,id_h,gamemenu_id_buf[game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				//==draw game next==
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,step*20-120,60,id_w,id_h,gamemenu_id_buf[next_game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			//==draw forward backward==
			if(0==dir)
			{
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,0,110,bfward_w,bfward_h,gamemenu_backward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,320-80,110,bfward_w,bfward_h,gamemenu_forward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}
			else
			{
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,32,110,bfward_w,bfward_h,gamemenu_backward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
				yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,320-48,110,bfward_w,bfward_h,gamemenu_forward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
			}

			ax32xx_sysDcacheFlush((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
			dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
			XOSTimeDly(20);
		}
		else
		{
			deg_Printf("video buf null!\n");
		}
	}

}


static void gameMenu_showbuf()
{
	disp_frame_t *p_lcd_buffer;

	p_lcd_buffer = (disp_frame_t *)dispLayerGetFrame(DISP_LAYER_VIDEO);
	if(p_lcd_buffer)
	{
		u16 lcd_w,lcd_h;
		hal_lcdGetResolution(&lcd_w,&lcd_h);
		//deg_Printf("video:w=%d,h=%d,bufw=%d,bufh=%d,\n",lcd_w,lcd_h,p_lcd_buffer->w,p_lcd_buffer->h);
		hal_dispframeVideoCfg(p_lcd_buffer,0,0,lcd_w,lcd_h);

		//==draw bk==
		memcpy(p_lcd_buffer->y_addr,gamemenu_bk_buf,bk_w*bk_h*3/2);
		ax32xx_sysDcacheWback((u32)p_lcd_buffer->y_addr,bk_w*bk_h*3/2);

		//==draw game==
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,100,60,id_w,id_h,gamemenu_id_buf[game_idx],id_w,id_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		//==draw forward backward==
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,32,110,bfward_w,bfward_h,gamemenu_backward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);
		yuv420_draw_buf(p_lcd_buffer->y_addr,p_lcd_buffer->w,p_lcd_buffer->h,320-80,110,bfward_w,bfward_h,gamemenu_forward_buf,bfward_w,bfward_h,YUV_ALPHA_Y,YUV_ALPHA_UV);

		ax32xx_sysDcacheWback((u32)p_lcd_buffer->y_addr,p_lcd_buffer->w*p_lcd_buffer->h*3/2);
		dispLayerShow(DISP_LAYER_VIDEO,(INT32U)p_lcd_buffer,0,0,VIDEO_ROTATE);
	}
	else
	{
		deg_Printf("video buf null!\n");
	}
}



#if 0
static int gameMenuKeyMsgPhoto(winHandle handle,uint32 parameNum,uint32* parame)
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
#endif


static int gameMenuKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	#if BT_MODE//MAIN_MENU_UI
	//msgGameFunc gameList[] = {game_snake_entry,game_snake_entry, game_maze_entry, game_sokoban_entry, game_tetris_entry, game_block_entry};
	msgGameFunc gameList[] = {game_snake_entry,game_snake_entry, game_maze_entry, game_sokoban_entry, game_tetris_entry/*, game_block_entry*/};
	#else
	msgGameFunc gameList[] = {game_snake_entry, game_maze_entry, game_sokoban_entry, game_tetris_entry/*, game_block_entry*/};
	#endif
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		#if BT_MODE//MAIN_MENU_UI
		if(game_idx == 0)
		{
			taskStart(TASK_AUDIO_PLAYER,0);
			deg_Printf("carT mode:%d\n",SysCtrl.cartoon_mode);
			if(0==SysCtrl.cartoon_mode)
			{
				SysCtrl.cartoon_mode=1;
				SysCtrl.cartoon_show_cnt=0;
			}
			/*while(SysCtrl.cartoon_mode)
			{
				drawUIService(true);
			}*/
			return 0;
		}
		#endif
		gameEntry = gameList[game_idx];
		if(NULL == gameEntry)
			return 0;
		
		taskStart(TASK_GAME,0);
		if(0==SysCtrl.cartoon_mode)
		{
			SysCtrl.cartoon_mode=1;
			SysCtrl.cartoon_show_cnt=0;
		}
		/*while(SysCtrl.cartoon_mode)
		{
			drawUIService(true);
		}*/
		deg_Printf("id_idx:%d\n",SysCtrl.mainmenu_taskidx);
	}
	return 0;
}

static int gameMenuKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		gameMenu_show_move(1);
		if(game_idx>0)
		{
			game_idx--;
		}
		else
		{
			game_idx=GAME_NUM-1;
		}
		XOSTimeDly(20);
		gameMenu_showbuf();
	}
	return 0;
}

static int gameMenuKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];

	if(keyState==KEY_PRESSED)
	{
		gameMenu_show_move(0);
		if(game_idx<(GAME_NUM-1))
		{
			game_idx++;
		}
		else
		{
			game_idx=0;
		}
		XOSTimeDly(20);
		gameMenu_showbuf();
	}
	return 0;
}

#if 0
static int gameMenuKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
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

static int gameMenuKeyMsgExit(winHandle handle,uint32 parameNum,uint32* parame)
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

static int gameMenuOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("game menu Open Win:%d\n",XOSTimeGet());
//	u8 i;
	//deg_Printf("\n\n------------1gamenenu--------------\n");
	//hal_sysMemPrint();
	//deg_Printf("----------------1gamenenu----------\n\n");

	if(taskPrevious() == TASK_MAIN_MENU)
		game_idx=0;

	//==init==
	hal_lcdGetResolution(&bk_w,&bk_h);
	gamemenu_bk_buf=hal_sysMemMalloc(bk_w*bk_h*3/2,32);
	if(NULL!=gamemenu_bk_buf)
	{
		jpg_decode_buf(RES_GAMEMENU_BK,gamemenu_bk_buf,gamemenu_bk_buf+bk_w*bk_h,bk_w,bk_h);
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	id_w=(GAMEMENU_PIXEL_W+0x1)&(~0x1);	// bmp must 2pixel align
	id_h=(GAMEMENU_PIXEL_H+0x1)&(~0x1);
	deg_Printf("bk_w=%d,bk_h=%d,id_w=%d,id_h=%d\n",bk_w,bk_h,id_w,id_h);
	rgb24_buf=hal_sysMemMalloc(id_w*id_h*3,32);
	if(NULL==rgb24_buf)
	{
		deg_Printf("mem err!\n");
		return 0;
	}

/*
	for(i=0;i<GAME_NUM;i++)
	{
		gamemenu_id_buf[i]=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(NULL!=gamemenu_id_buf[i])
		{
			bmp24_to_yuv420_buf(game_bmp_id[i],rgb24_buf,gamemenu_id_buf[i],gamemenu_id_buf[i]+id_w*id_h,id_w,id_h);
			deg_Printf("id=%d,first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",i,*gamemenu_id_buf[i],*(gamemenu_id_buf[i]+id_w*id_h),*(gamemenu_id_buf[i]+1),*(gamemenu_id_buf[i]+id_w*id_h+1));
		}
		else
		{
			deg_Printf("mem err!\n");
			return 0;
		}
	}
*/

	if(NULL==gamemenu_id_buf[game_idx])
	{
		gamemenu_id_buf[game_idx]=hal_sysMemMalloc(id_w*id_h*3/2,32);
		if(gamemenu_id_buf[game_idx])
		{
			bmp24_to_yuv420_buf(game_bmp_id[game_idx],rgb24_buf,gamemenu_id_buf[game_idx],gamemenu_id_buf[game_idx]+id_w*id_h,id_w,id_h);
			//deg_Printf("id=%d,first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",i,*gamemenu_id_buf[game_idx],*(gamemenu_id_buf[game_idx]+id_w*id_h),*(gamemenu_id_buf[game_idx]+1),*(gamemenu_id_buf[game_idx]+id_w*id_h+1));
		}
	}

	bfward_w=(GAMEMENU_BFWARD_W+0x1)&(~0x1);	// bmp must 2pixel align
	bfward_h=(GAMEMENU_BFWARD_H+0x1)&(~0x1);

	gamemenu_forward_buf=hal_sysMemMalloc(bfward_w*bfward_h*3/2,32);
	if(NULL!=gamemenu_forward_buf)
	{
		bmp24_to_yuv420_buf(RES_ICON_FORWARD,rgb24_buf,gamemenu_forward_buf,gamemenu_forward_buf+bfward_w*bfward_h,bfward_w,bfward_h);
		//deg_Printf("first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",*gamemenu_forward_buf,*(gamemenu_forward_buf+bfward_w*bfward_h),*(gamemenu_forward_buf+1),*(gamemenu_forward_buf+bfward_w*bfward_h+1));
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}

	gamemenu_backward_buf=hal_sysMemMalloc(bfward_w*bfward_h*3/2,32);
	if(NULL!=gamemenu_backward_buf)
	{
		bmp24_to_yuv420_buf(RES_ICON_BACKWARD,rgb24_buf,gamemenu_backward_buf,gamemenu_backward_buf+bfward_w*bfward_h,bfward_w,bfward_h);
		deg_Printf("first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",*gamemenu_backward_buf,*(gamemenu_backward_buf+bfward_w*bfward_h),*(gamemenu_backward_buf+1),*(gamemenu_backward_buf+bfward_w*bfward_h+1));
	}
	else
	{
		deg_Printf("mem err!\n");
		return 0;
	}


	//==show==
	gameMenu_showbuf();
	deg_Printf("show time:%d\n",XOSTimeGet());

	if(0==SysCtrl.cartoon_mode)
	{
		SysCtrl.cartoon_mode=1;
		SysCtrl.cartoon_show_cnt=0;
	}
	//deg_Printf("\n\n------------2gamenenu--------------\n");
	//hal_sysMemPrint();
	//deg_Printf("----------------2gamenenu----------\n\n");

	return 0;
}

static int gameMenuCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("game menu Close Win!!!\n");
	if(NULL!=gamemenu_bk_buf)
	{
		hal_sysMemFree(gamemenu_bk_buf);
		gamemenu_bk_buf=NULL;
	}
	u8 i;
	for(i=0;i<GAME_NUM;i++)
	{
		if(NULL!=gamemenu_id_buf[i])
		{
			hal_sysMemFree(gamemenu_id_buf[i]);
			gamemenu_id_buf[i]=NULL;
		}
	}
	if(NULL!=gamemenu_forward_buf)
	{
		hal_sysMemFree(gamemenu_forward_buf);
		gamemenu_forward_buf=NULL;
	}
	if(NULL!=gamemenu_backward_buf)
	{
		hal_sysMemFree(gamemenu_backward_buf);
		gamemenu_backward_buf=NULL;
	}

	if(NULL!=rgb24_buf)
	{
		hal_sysMemFree(rgb24_buf);
		rgb24_buf=NULL;
	}



	return 0;
}

static int gameMenuSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}

static int gameMenuSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}



msgDealInfor gameMenuMsgDeal[]=
{
	{SYS_OPEN_WINDOW,gameMenuOpenWin},
	{SYS_CLOSE_WINDOW,gameMenuCloseWin},
	{KEY_EVENT_LEFT,gameMenuKeyMsgUp},
	{KEY_EVENT_RIGHT,gameMenuKeyMsgDown},
//	{KEY_EVENT_MENU,gameMenuKeyMsgMenu},
	{KEY_EVENT_OK,gameMenuKeyMsgOk},
	{KEY_EVENT_RETURN,gameMenuKeyMsgExit},
	{SYS_EVENT_500MS,gameMenuSysMsg500MS},
	{SYS_EVENT_1S,gameMenuSysMsg1S},
	{EVENT_MAX,NULL},
};

WINDOW(gameMenuWindow,gameMenuMsgDeal,gameMenuWin)



