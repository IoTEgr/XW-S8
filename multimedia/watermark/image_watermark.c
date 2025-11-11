/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA IMAGE ENCODE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : image_encode.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"
#include "../../hal/inc/hal_mjpeg.h"
#include "../../ax32_platform_demo/application.h"

#include "image_watermark.h"

#if MEDIA_CFG_IMAGE_ENCODE_EN >0


//------for watermark



static u16 id_w_st,id_h_st;

//static u16 bk_w_sm,bk_h_sm;

static u16 wm_pixel_w,wm_pixel_h;
static u8  wm_pic_num;

// 背景亮度阈值，如果背景亮度超过此值，认为是白色背景，需要反转水印为黑色
#define BRIGHTNESS_THRESHOLD 160

#if 1

extern u8* small_pic_id_buf[20];
extern u32  small_pic_id[20];

#else

//small_pic_id_buf[20];

 u32  small_pic_id[20]={RES_NUM_0,RES_NUM_1,RES_NUM_2,RES_NUM_3,RES_NUM_4,
								RES_NUM_5,RES_NUM_6,RES_NUM_7,RES_NUM_8,RES_NUM_9,
								RES_NUM_SLASH,RES_NUM_COLON,RES_NUM_BLANK,
							   };

#endif

/*******************************************************************************
* Function Name  : yuv420_draw_buf_adaptive
* Description    : 自适应绘制水印，根据背景亮度逐像素选择颜色
* Input          : u8 *dst_ybuf: 目标YUV缓冲区
                   u16 dst_w: 目标宽度
                   u16 dst_h: 目标高度
                   s16 draw_x: 绘制X位置
                   s16 draw_y: 绘制Y位置
                   u16 draw_w: 绘制宽度
                   u16 draw_h: 绘制高度
                   u8 *src_ybuf: 源水印缓冲区
                   u16 src_w: 源水印宽度
                   u16 src_h: 源水印高度
                   u8 alpha_y: Y分量透明色
                   u8 alpha_uv: UV分量透明色
* Output         : None
* Return         : None
*******************************************************************************/
static void yuv420_draw_buf_adaptive(u8 *dst_ybuf, u16 dst_w, u16 dst_h, s16 draw_x, s16 draw_y, 
                                      u16 draw_w, u16 draw_h, u8 *src_ybuf, u16 src_w, u16 src_h, 
                                      u8 alpha_y, u8 alpha_uv)
{
	u16 i, j;
	u16 draw_offset_x, draw_offset_y;
	u8 *dy, *duv, *sy, *suv;
	u8 bg_brightness;
	u8 src_y_val, src_uv_val;
	u8 output_y;

	draw_x &= ~0x1;		// align
	draw_y &= ~0x1;		// align
	draw_w &= ~0x1;		// align
	draw_h &= ~0x1;		// align

	if((draw_x + draw_w <= 0) || (draw_y + draw_h <= 0) || (draw_x >= dst_w) || (draw_y >= dst_h))
	{
		return;
	}

	if(draw_x < 0)
	{
		draw_offset_x = -draw_x;
		draw_x = 0;
	}
	else
	{
		draw_offset_x = 0;
	}
	
	if(draw_y < 0)
	{
		draw_offset_y = -draw_y;
		draw_y = 0;
	}
	else
	{
		draw_offset_y = 0;
	}

	if(draw_x + src_w > dst_w)
	{
		draw_w = dst_w - draw_x;
	}
	else
	{
		draw_w = src_w - draw_offset_x;
	}

	if(draw_y + src_h > dst_h)
	{
		draw_h = dst_h - draw_y;
	}
	else
	{
		draw_h = src_h - draw_offset_y;
	}
	
	dy = dst_ybuf + dst_w * draw_y + draw_x;
	duv = dst_ybuf + dst_w * dst_h + dst_w * draw_y / 2 + draw_x;
	sy = src_ybuf;
	suv = src_ybuf + src_w * src_h;

	// 逐像素绘制，根据背景亮度自适应反转
	for(j = 0; j < draw_h; j += 2)
	{
		for(i = 0; i < draw_w; i++)
		{
			src_y_val = *(sy + i + draw_offset_x);
			src_uv_val = *(suv + i + draw_offset_x);
			
			// 跳过透明像素（灰色背景）
			if((src_y_val != alpha_y) || (src_uv_val != alpha_uv))
			{
				// 获取当前位置的背景亮度
				bg_brightness = *(dy + i);
				
				// 根据背景亮度决定是否反转水印颜色
				if(bg_brightness > BRIGHTNESS_THRESHOLD)
				{
					// 背景亮，使用黑色水印（反转）
					output_y = 255 - src_y_val;
				}
				else
				{
					// 背景暗，使用白色水印（原样）
					output_y = src_y_val;
				}
				
				*(dy + i) = output_y;
				*(duv + i) = src_uv_val;
			}
		}

		dy += dst_w;
		sy += src_w;

		// 第二行
		for(i = 0; i < draw_w; i++)
		{
			src_y_val = *(sy + i + draw_offset_x);
			src_uv_val = *(suv + i + draw_offset_x);
			
			// 跳过透明像素（灰色背景）
			if((src_y_val != alpha_y) || (src_uv_val != alpha_uv))
			{
				// 获取当前位置的背景亮度
				bg_brightness = *(dy + i);
				
				// 根据背景亮度决定是否反转水印颜色
				if(bg_brightness > BRIGHTNESS_THRESHOLD)
				{
					// 背景亮，使用黑色水印（反转）
					output_y = 255 - src_y_val;
				}
				else
				{
					// 背景暗，使用白色水印（原样）
					output_y = src_y_val;
				}
				
				*(dy + i) = output_y;
				*(duv + i) = src_uv_val;
			}
		}
		
		dy += dst_w;
		sy += src_w;
		duv += dst_w;
		suv += src_w;
	}
}



static int watermark_bmp2yuv_set(u8* ydst_buf,u16 dst_w,u16 dst_h,u16 pos_x,u16 pos_y,char time_str)
{
	if(time_str == '0')
		SysCtrl.timestemp_idx =0;
	else if(time_str == '1')
		SysCtrl.timestemp_idx =1;
	else if(time_str == '2')
		SysCtrl.timestemp_idx =2;
	else if(time_str == '3')
		SysCtrl.timestemp_idx =3;
	else if(time_str == '4')
		SysCtrl.timestemp_idx =4;
	else if(time_str == '5')
		SysCtrl.timestemp_idx =5;
	else if(time_str == '6')
		SysCtrl.timestemp_idx =6;
	else if(time_str == '7')
		SysCtrl.timestemp_idx =7;
	else if(time_str == '8')
		SysCtrl.timestemp_idx =8;
	else if(time_str == '9')
		SysCtrl.timestemp_idx =9;
	else if(time_str == '/')
		SysCtrl.timestemp_idx =10;
	else if(time_str == ':')
		SysCtrl.timestemp_idx =11;
	else if(time_str == ' ')
		SysCtrl.timestemp_idx =12;



	//if(p_lcd_buffer_st)
	{
//		u16 lcd_w,lcd_h;
		// 使用自适应绘制函数，逐像素根据背景亮度决定颜色
		// 不再需要提前计算整体亮度或反转整个水印
		yuv420_draw_buf_adaptive(ydst_buf, dst_w, dst_h, pos_x, pos_y, id_w_st, id_h_st, 
		                          small_pic_id_buf[SysCtrl.timestemp_idx], id_w_st, id_h_st, 
		                          0x7f, YUV_ALPHA_UV);
		//#endif
		ax32xx_sysDcacheWback((u32)ydst_buf,dst_w*dst_h*3/2);
		//ax32xx_sysDcacheFlush((u32)ydst_buf,640*480*3/2);
	}
	return 0;
}


/*******************************************************************************
* Function Name  : watermark_bmp2yuv_init
* Description    : draw watermark init 
* Input          : 

* Output         : 
* Return         : None
*******************************************************************************/
void watermark_bmp2yuv_init(u16 pixel_w,u16 pixel_h,u8 pic_num)
{
	u32 one_pic_len;
	u8 i;
	u8* rgb24_buf_ts;
	u8 *small_pic_buf;

#if 0
	
	id_w_st=(ST_PIXEL_W+0x1)&(~0x1);	// bmp must 2pixel align
	id_h_st=(ST_PIXEL_H+0x1)&(~0x1);
#else
	wm_pixel_w=pixel_w;
	wm_pixel_h=pixel_h;
	wm_pic_num=pic_num;
	deg_Printf("in lib!!\n");
	deg_Printf("wm-bmp2yuv-init w:%d h:%d num:%d\n",wm_pixel_w,wm_pixel_h,wm_pic_num);
	id_w_st=(wm_pixel_w+0x1)&(~0x1);	// bmp must 2pixel align
	id_h_st=(wm_pixel_h+0x1)&(~0x1);
#endif

	rgb24_buf_ts=hal_sysMemMalloc(id_w_st*id_h_st*3,32);
	if(NULL==rgb24_buf_ts)
	{
		deg_Printf("mem err!\n");
		return /*0*/;
	}

	one_pic_len = id_w_st*id_h_st*3/2;
	small_pic_buf=hal_sysMemMalloc(one_pic_len*wm_pic_num,32);
	if(NULL==small_pic_buf)
	{
		deg_Printf("small_pic_buf err!\n");
		hal_sysMemFree(rgb24_buf_ts);
		return /*0*/;
	}

	for(i=0;i<wm_pic_num;i++)
	{
		//small_pic_id_buf[i]=hal_sysMemMalloc(id_w_st*id_h_st*3/2,32);
		small_pic_id_buf[i]=small_pic_buf;
		small_pic_buf += one_pic_len;
		if(NULL!=small_pic_id_buf[i])
		{
			//--

			bmp24_to_yuv420_buf(small_pic_id[i],rgb24_buf_ts,small_pic_id_buf[i],small_pic_id_buf[i]+id_w_st*id_h_st,id_w_st,id_h_st);
			//debg("id=%d,first pixel:y=0x%x,u=0x%x,y=0x%x,v=0x%x\n",i,*small_pic_id_buf[i],*(small_pic_id_buf[i]+id_w_st*id_h_st),*(small_pic_id_buf[i]+1),*(small_pic_id_buf[i]+id_w_st*id_h_st+1));
			//#endif
		}
		else
		{
			deg_Printf("mem err!\n");
			return /*0*/;
		}
	}

	if(NULL!=rgb24_buf_ts)
	{
		deg_Printf("free rgb24_buf_ts\n");
		hal_sysMemFree(rgb24_buf_ts);
		rgb24_buf_ts=NULL;
	}

}


/*******************************************************************************
* Function Name  : watermark_bmp2yuv_draw
* Description    : draw watermark on photo 
* Input          : u8* ybuf_src: src
				   u16 pos_x: X position
				   u16 pos_y: Y position
 					u16 gap  :	character gap

* Output         : 
* Return         : None
*******************************************************************************/
void watermark_bmp2yuv_draw(u8* ybuf_src,u16 pos_x, u16 pos_y,u16 gap)
{
 	 int i,w_offset=0;
	char *rtctime_pic;
	u16 csi_w,csi_h;
 	rtctime_pic=hal_rtcTime2String(hal_rtcTimeGet()); //get rtc time 
	hal_csiResolutionGet(&csi_w,&csi_h);	//get csi w & h
	deg_Printf("pos_x:%d pos_y:%d\n",pos_x,pos_y);
	deg_Printf("in wm_dram:");
	for(i=0;i<19;i++)
	{
		deg_Printf("%c\t",*(rtctime_pic+i));
		watermark_bmp2yuv_set((u8 *)mjpegEncCtrl.ybuffer,csi_w,csi_h,pos_x+w_offset,pos_y,*(rtctime_pic+i));
		w_offset=w_offset+wm_pixel_w+gap;
	}

}

/*******************************************************************************
* Function Name  : watermark_buf_bmp2yuv_free
* Description    : watermark_buf_free
* Input          : None
                     
* Output         : None
* Return         : None
*******************************************************************************/
void watermark_buf_bmp2yuv_free(void)
{
	int i;
	
	//--------free	 timestemp	buf----------------------------
	hal_sysMemFree(small_pic_id_buf[0]);
		for(i=0;i<wm_pic_num;i++)
		{
			if(NULL!=small_pic_id_buf[i])
			{
				//hal_sysMemFree(small_pic_id_buf[i]);
				small_pic_id_buf[i]=NULL;
			}
		}
	//-------------------------------------------

}


#endif








