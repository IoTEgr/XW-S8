/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX user port MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : max_user.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib user port managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../bwlib.h"
#include "../../ax32_platform_demo/inc/display.h"
#include "../../ax32_platform_demo/application.h"


static INT8U *iconCache;
#if(OSD_SOFTWARE_ROTATE==0)
static disp_frame_t *iconTempBuffer;
#endif
static INT8U*  doubleBuffer=NULL;
static bool    useDoubleBuffer=false;

uint16 LCD_WIDTH=0;
uint16 LCD_HEIGHT=0;



extern void *dispLayerGetFrame(INT8U layer);
extern int fontDrawString(INT8U *dest,INT8U num,INT16S x,INT16S y,INT16U width,INT8U color);
extern int fontGetStringInfo(INT8U num,INT16U *width,INT16U *height);
extern int iconGetData(int num,INT8U *buffer,int len);
extern int iconGetAddr(int num);
extern int iconGetSize(int num,INT16U *width,INT16U *height);

s32_t user_string_size_get(u32_t id,u16_t *width,u16_t *height)
{
     return fontGetStringInfo(id,width,height);
 //    return 0;
}
u8_t *user_string_data_get(u32_t id)
{
	return NULL;
}
s32_t user_get_icon(u32_t name,u32_t buff,u32_t size)
{
	iconGetData(name,(u8_t *)buff,size);
	
    return 1;
}
s32_t user_get_icon_addr(u32_t name)
{
	return iconGetAddr(name);
}
void *user_get_icon_line(u32_t addr,u32_t size)
{
	if(nv_read(addr,(void *)iconCache,size)<0)
		return NULL;

	return iconCache;
}
s32_t user_get_iconSize(u32_t name,u16_t *width,u16_t *height)
{
    iconGetSize(name,width,height);
    return 0;
}
s32_t initDrawBuffWH(uint16 width,uint16 height)
{
#if CFG_R_ICON_LOAD>0
	iconCache = hal_sysMemMalloc(width*height,32);
#else
	iconCache = halSysMemLineCache;
#endif	
	LCD_WIDTH=width;
	LCD_HEIGHT=height;
	#if(OSD_SOFTWARE_ROTATE==0)
	INT32U ret = hal_lcdGetShowingRotate();
    if((ret == LCD_ROTATE_90) || (ret == LCD_ROTATE_270))
        iconTempBuffer = hal_osdGetIdleRotateBuffer();
    else
        iconTempBuffer = hal_osdGetIdleCmpBuffer();
	iconTempBuffer->data_size = LCD_WIDTH*LCD_HEIGHT;
	debg("iconTempBuffer[%x],size[%d]\n",iconTempBuffer,iconTempBuffer->data_size);
	#endif
	useDoubleBuff(true/*false*/);
    return 0;
}
void lcdBuffCopy(uint8* dest,uint8* src)
{
	uint32 w,h;
	for(h=0;h<LCD_HEIGHT;h++)
	{
		for(w=0;w<LCD_WIDTH;w++)
			dest[w]=src[w];
		ax32xx_sysDcacheWback((u32)dest,LCD_WIDTH);
		dest+=LCD_WIDTH;
		src+=LCD_WIDTH;
	}
}
void useDoubleBuff(bool en)
{
	#if(OSD_SOFTWARE_ROTATE!=0)
	if(en==useDoubleBuffer)
		return ;
	if(en)
	{
		if(doubleBuffer==NULL)
			doubleBuffer=hal_sysMemMalloc(LCD_WIDTH*LCD_HEIGHT,32);
		if(doubleBuffer==NULL)
			return;
		ax32xx_sys_dma_memkick((void *)doubleBuffer,(void *)dispLayerGetFrame(DISP_LAYER_OSD0),LCD_WIDTH*LCD_HEIGHT);
		useDoubleBuffer=en;
		deg_Printf("use double buffer!!!\n");
	}
	else
	{
		if(doubleBuffer==NULL)
		{
			useDoubleBuffer=en;
			return;
		}
		ax32xx_sys_dma_memkick((void *)dispLayerGetFrame(DISP_LAYER_OSD0),(void *)doubleBuffer,LCD_WIDTH*LCD_HEIGHT);
		useDoubleBuffer=en;
		hal_sysMemFree(doubleBuffer);
		doubleBuffer=NULL;
		deg_Printf("use single buffer!!!\n");
	}
	#endif
}

INT8U* getDrawBuff(void)
{
	if(useDoubleBuffer)
		return doubleBuffer;
	return (INT8U*)dispLayerGetFrame(DISP_LAYER_OSD0);
}
extern uint8 winPaint(void);
extern void sendDrawUIMsg(void);

static void drawUICartoon(u8 *addr)
{
#if 0
	u16 w,h;
	if(1==SysCtrl.cartoon_mode)	// in 
	{
		for(h=0;h<LCD_HEIGHT;h++)
		{
			for(w=0;w<LCD_WIDTH;w++)
			{
				if(((h<(LCD_HEIGHT-SysCtrl.cartoon_show_cnt*30)/2)||(h>SysCtrl.cartoon_show_cnt*30+(LCD_HEIGHT-SysCtrl.cartoon_show_cnt*30)/2))
					&&((w<(LCD_WIDTH-SysCtrl.cartoon_show_cnt*40)/2)||(w>SysCtrl.cartoon_show_cnt*40+(LCD_WIDTH-SysCtrl.cartoon_show_cnt*40)/2)))
				{
					*(addr+h*LCD_WIDTH+w)=R_COLOR_BLACK;
				}
			}
		}
		ax32xx_sysDcacheWback((u32)addr,LCD_WIDTH*LCD_HEIGHT);
		//deg_Printf("cartoon:%d\n",SysCtrl.cartoon_show_cnt);
	}
	else if(2==SysCtrl.cartoon_mode)	// out 
	{
		for(h=0;h<LCD_HEIGHT;h++)
		{
			for(w=0;w<LCD_WIDTH;w++)
			{
				if(((h<(LCD_HEIGHT-SysCtrl.cartoon_show_cnt*30)/2)||(h>SysCtrl.cartoon_show_cnt*30+(LCD_HEIGHT-SysCtrl.cartoon_show_cnt*30)/2))
					&&((w<(LCD_WIDTH-SysCtrl.cartoon_show_cnt*40)/2)||(w>SysCtrl.cartoon_show_cnt*40+(LCD_WIDTH-SysCtrl.cartoon_show_cnt*40)/2)))
				{
					*(addr+h*LCD_WIDTH+w)=R_COLOR_BLACK;
				}
			}
		}
		ax32xx_sysDcacheWback((u32)addr,LCD_WIDTH*LCD_HEIGHT);
	}

	SysCtrl.cartoon_show_cnt++;
	if(SysCtrl.cartoon_show_cnt>=8)
	{
		SysCtrl.cartoon_mode=0;
		SysCtrl.cartoon_show_cnt=0;
		SysCtrl.redraw_lcd_flag=1;
	}
#else

	SysCtrl.cartoon_mode=0;
	SysCtrl.cartoon_show_cnt=0;
	SysCtrl.redraw_lcd_flag=1;

#endif


}


void drawUIService(bool force)
{

//===lcd back light close,will draw black===
	u32 lcdon;
	boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdon);
	if(0==lcdon && useDoubleBuffer)
	{
		u8* addr = (u8*)dispLayerGetFreeFrame(DISP_LAYER_OSD0);
		memset((void *)addr,/*(void *)*/R_COLOR_BLACK,LCD_WIDTH*LCD_HEIGHT);
		ax32xx_sysDcacheWback((u32)addr,LCD_WIDTH*LCD_HEIGHT);
		dispLayerSetFrame(DISP_LAYER_OSD0,(u32)addr);
#if(OSD_SOFTWARE_ROTATE!=0)
		ax32xx_deUpdate();
#else
		ax32xx_sysDcacheWback((u32)iconTempBuffer->y_addr,iconTempBuffer->data_size);
		hal_osdDealBuffer(iconTempBuffer);
#endif
		return;
	}
//===end lcd back light close,will draw black===

	if(winPaint()==0)
	{
		if((false==force)&&(0==SysCtrl.redraw_lcd_flag)&&(0==SysCtrl.cartoon_mode))
		{
			return;
		}
		else
		{
			SysCtrl.redraw_lcd_flag=0;
		}
	}
	else
	{
		
	}

	sendDrawUIMsg();
	
	if(useDoubleBuffer)
	{
		u8* addr = (u8*)dispLayerGetFreeFrame(DISP_LAYER_OSD0);
		ax32xx_sys_dma_memkick((void *)addr,(void *)doubleBuffer,LCD_WIDTH*LCD_HEIGHT);
		if(SysCtrl.cartoon_mode)
		{
			drawUICartoon(addr);
		}
		dispLayerSetFrame(DISP_LAYER_OSD0,(u32)addr);
	}
	#if(OSD_SOFTWARE_ROTATE!=0)
	ax32xx_deUpdate();
	#else
	ax32xx_sysDcacheWback((u32)iconTempBuffer->y_addr,iconTempBuffer->data_size);
	hal_osdDealBuffer(iconTempBuffer);
	#endif
	if(SysCtrl.cartoon_mode)
	{
		XOSTimeDly(30);
	}

}

void drawUIService1(void)
{
//===lcd back light close,will draw black===
	u32 lcdon;
	boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdon);
	if(0==lcdon && useDoubleBuffer)
	{
		u8* addr = (u8*)dispLayerGetFreeFrame(DISP_LAYER_OSD0);
		memset((void *)addr,/*(void *)*/R_COLOR_BLACK,LCD_WIDTH*LCD_HEIGHT);
		ax32xx_sysDcacheWback((u32)addr,LCD_WIDTH*LCD_HEIGHT);
		dispLayerSetFrame(DISP_LAYER_OSD0,(u32)addr);
#if(OSD_SOFTWARE_ROTATE!=0)
		ax32xx_deUpdate();
#else
		ax32xx_sysDcacheWback((u32)iconTempBuffer->y_addr,iconTempBuffer->data_size);
		hal_osdDealBuffer(iconTempBuffer);
#endif
		return;
	}
//===end lcd back light close,will draw black===


	if(useDoubleBuffer)
	{
		//ax32xx_sys_dma_memkick((void *)dispLayerGetFrame(DISP_LAYER_OSD0),(void *)doubleBuffer,LCD_WIDTH*LCD_HEIGHT);
		u8* addr = (u8*)dispLayerGetFreeFrame(DISP_LAYER_OSD0);
		ax32xx_sys_dma_memkick((void *)addr,(void *)doubleBuffer,LCD_WIDTH*LCD_HEIGHT);
		dispLayerSetFrame(DISP_LAYER_OSD0,(u32)addr);
	}
	#if(OSD_SOFTWARE_ROTATE!=0)
	ax32xx_deUpdate();
	#else
	ax32xx_sysDcacheWback((u32)iconTempBuffer->y_addr,iconTempBuffer->data_size);
	hal_osdDealBuffer(iconTempBuffer);
	#endif
}

