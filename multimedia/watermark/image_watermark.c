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

//-----




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
		//small_pic_id_buf
		yuv420_draw_buf(ydst_buf,dst_w,dst_h,pos_x,pos_y,id_w_st,id_h_st,small_pic_id_buf[SysCtrl.timestemp_idx],id_w_st,id_h_st,0x7f,YUV_ALPHA_UV);
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








