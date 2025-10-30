/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         LCD HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_lcdframe.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN LCD HAL LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"

/* video :
 *  CSI --> [rotateframe0/1 --> ROTATE -->] dispframe0/1 --> DE
 * 
 * osd :
 *   UI --> [rotateframe0 --> ROTATE -->] compressframe0 --> OSDCMP --> dispframe0/1 --> DE
 */

struct disp_frames_mgr_s dfm;

 s32 hal_lcdAddrCalculate(u32 src_w,u32 src_h,u32 scan_mode,u32 stride,u32 *y_offset,u32 *uv_offset)
{
#if HAL_CFG_EN_LCD > 0		

	switch(scan_mode)
	{
		default:
		case 0://nomal left-top
			*y_offset = *uv_offset = 0;
			break;
			
		case 5://rotate 270
		case 1://vertical mirror left-down
			*y_offset = (src_h - 1) * stride;
			*uv_offset = (src_h / 2 - 1) * stride;
			break;
		
		case 4://rotate 90
		case 2://horizontal mirror right-top
			*y_offset = *uv_offset = src_w - 1;
			break;
			
		case 3://rotate 180 right-down
			*y_offset  = (src_h - 1) * stride + src_w - 1;
			*uv_offset = (src_h / 2 - 1) * stride + src_w - 1;
			break;
	}
#endif
    return 0;
}

static void frames_vr_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_VIDEO_ROTATE;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size, w, h;

    if(buf_nums)
    {
        w       = disp_w;
        h       = disp_h;
        stride  = (w + LCD_DE_STRIDE) & (~LCD_DE_STRIDE);
        y_size  = stride * h;
        size    = y_size * 3 / 2;
        
        p_frames->frame_nums = buf_nums;
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;
        
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = hal_sysMemMalloc(size,64);
            p_frame->uv_addr    = &p_frame->y_addr[y_size];
            p_frame->_y_addr    = NULL;
            p_frame->_uv_addr   = NULL;
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
            p_frame->w          = w;
            p_frame->h          = h;
			p_frame->posX       =0;
			p_frame->posY       =0;
            p_frame->stride     = stride;
            p_frame->scan_mode  = LCD_ROTATE_90;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}

static void frames_vd_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_VIDEO_DISPLAY;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size, w, h, _y_ofs, _uv_ofs;

    if(buf_nums)
    {
        w       = disp_w;
        h       = disp_h;
        if((scan_mode == LCD_ROTATE_90) || (scan_mode == LCD_ROTATE_270))
        {
            w   = disp_h;
            h   = disp_w;
            if(scan_mode == LCD_ROTATE_90)
                scan_mode = LCD_ROTATE_0;
            else
                scan_mode = LCD_ROTATE_180;
        }
        stride  = (w + LCD_DE_STRIDE) & (~LCD_DE_STRIDE);
        y_size  = stride * h;
        size    = y_size * 3 / 2;
        hal_lcdAddrCalculate(w,h,scan_mode,stride,&_y_ofs,&_uv_ofs);
        
        p_frames->frame_nums = buf_nums;
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;
        
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = hal_sysMemMalloc(size,64);
            p_frame->uv_addr    = &p_frame->y_addr[y_size];
            p_frame->_y_addr    = &p_frame->y_addr[_y_ofs];
            p_frame->_uv_addr   = &p_frame->uv_addr[_uv_ofs];
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
            p_frame->w          = w;
            p_frame->h          = h;
			p_frame->posX       =0;
			p_frame->posY       =0;
            p_frame->stride     = stride;
            p_frame->scan_mode  = scan_mode;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}

static void frames_or_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_OSD_ROTATE;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size, w, h;

    if(buf_nums)
    { 
        w       = disp_w;
        h       = disp_h;
        stride  = w;
        y_size  = stride * h;
        size    = y_size;
        
        p_frames->frame_nums = buf_nums;
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;
        
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = hal_sysMemMalloc(size,64);
            p_frame->uv_addr    = NULL;
            p_frame->_y_addr    = NULL;
            p_frame->_uv_addr   = NULL;
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
            p_frame->w          = w;
            p_frame->h          = h;
            p_frame->stride     = stride;
            p_frame->scan_mode  = LCD_ROTATE_90;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}

static void frames_oc_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_OSD_COMPRESS;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size, w, h;
        
    if(buf_nums)
    {
        w         = disp_w;
        h         = disp_h;
        if((scan_mode == LCD_ROTATE_90) || (scan_mode == LCD_ROTATE_270))
        {
            w     = disp_h;
            h     = disp_w;
        }
        stride    = w;
        y_size    = stride * h;
        size      = y_size;
        
        if((scan_mode == LCD_ROTATE_180) || (scan_mode == LCD_ROTATE_270))
            scan_mode = LCD_ROTATE_180;
        else
            scan_mode = LCD_ROTATE_0;
        
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;
        
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = hal_sysMemMalloc(size,64);
            p_frame->uv_addr    = NULL;
            p_frame->_y_addr    = NULL;
            p_frame->_uv_addr   = NULL;
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
            p_frame->w          = w;
            p_frame->h          = h;
            p_frame->stride     = stride;
            p_frame->scan_mode  = scan_mode;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}

static void frames_od_init(u32 disp_w,u32 disp_h,u8 scan_mode)
{
    int i;
    u8 type = FTYPE_OSD_DISPLAY;
    disp_frames_t * p_frames = &dfm.types[type];
    disp_frame_t  * p_frame  = NULL;
    u32 buf_nums = p_frames->frame_nums;
    u32 stride, y_size, size, w, h;
    
    if(buf_nums)
    {
        w       = disp_w;
        h       = disp_h;
        stride  = w;
        y_size  = stride * h / 2;
        size    = y_size;
        
        p_frames->frame_type = type;
        p_frames->freelist = p_frames->_frames;
        
        for(i = 0;i < buf_nums;i++)
        {
            p_frame             = &p_frames->_frames[i];
            p_frame->next       = &p_frame[1];
            p_frame->y_addr     = hal_sysMemMalloc(size,64);
            p_frame->uv_addr    = NULL;
            p_frame->_y_addr    = NULL;
            p_frame->_uv_addr   = NULL;
            p_frame->priv_func  = NULL;
            p_frame->buf_size   = size;
            p_frame->data_size  = 0;
            p_frame->w          = w;
            p_frame->h          = h;
            p_frame->stride     = stride;
            p_frame->scan_mode  = LCD_ROTATE_0;
            p_frame->id         = i;
            p_frame->pip_sta    = 0;
            p_frame->buf_sta    = 0;
            p_frame->frame_type = type;
        }
        p_frame->next = NULL;
    }
}

static void frame_debug(disp_frame_t * p_frame)
{
    debg("---- frame : %X ----\n",p_frame    );
    debg("frame_type: %X\n",p_frame->frame_type);
    debg("id        : %X\n",p_frame->id        );
    debg("w         : %X\n",p_frame->w         );
    debg("h         : %X\n",p_frame->h         );
    debg("stride    : %X\n",p_frame->stride    );
    debg("scan_mode : %X\n",p_frame->scan_mode );
    debg("y_addr    : %X\n",p_frame->y_addr    );
    debg("uv_addr   : %X\n",p_frame->uv_addr   );
    debg("_y_addr   : %X\n",p_frame->_y_addr   );
    debg("_uv_addr  : %X\n",p_frame->_uv_addr  );
    debg("priv_func : %X\n",p_frame->priv_func );
    debg("buf_size  : %X\n",p_frame->buf_size  );
    debg("data_size : %X\n",p_frame->data_size );
    debg("pip_sta   : %X\n",p_frame->pip_sta   );
    debg("buf_sta   : %X\n",p_frame->buf_sta   );
    debg("next      : %X\n",p_frame->next      );
}

static void frames_debug(disp_frames_t * p_frames)
{
    debg("\n---- frames : %X ----\n",p_frames);
    debg("_frames   : %X\n",p_frames->_frames   );
    debg("frame_nums: %X\n",p_frames->frame_nums);
    debg("frame_type: %X\n",p_frames->frame_type);
    debg("freelist  : %X\n",p_frames->freelist  );
    
    int i;
    for(i = 0;i < p_frames->frame_nums;i++)
    {
        frame_debug(&p_frames->_frames[i]);
    }
}
/*******************************************************************************
* Function Name  : hal_dispframeInit
* Description    : init all display frames
* Input          : u32 width,u32 height : video/osd display size
*                  u8 scan_mode : video/osd display rotate mode
* Output         : None
* Return         : None
*******************************************************************************/
void hal_dispframeInit(u32 vsrc_w,u32 vsrc_h,u32 osd_w,u32 osd_h,u8 scan_mode)
{
    memset(&dfm,0,sizeof(dfm));
    
    if((scan_mode == LCD_ROTATE_90) || (scan_mode == LCD_ROTATE_270))
    {
        dfm.types[FTYPE_VIDEO_ROTATE].frame_nums  = 2;
        dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums = 2;
        dfm.types[FTYPE_OSD_ROTATE].frame_nums    = 1;
        dfm.types[FTYPE_OSD_COMPRESS].frame_nums  = 1;
        dfm.types[FTYPE_OSD_DISPLAY].frame_nums   = 3;
    }
    else
    {
        dfm.types[FTYPE_VIDEO_ROTATE].frame_nums  = 0;
        dfm.types[FTYPE_VIDEO_DISPLAY].frame_nums = 3;
        dfm.types[FTYPE_OSD_ROTATE].frame_nums    = 0;
        dfm.types[FTYPE_OSD_COMPRESS].frame_nums  = 1;
        dfm.types[FTYPE_OSD_DISPLAY].frame_nums   = 3;
    }
	#if(OSD_SOFTWARE_ROTATE!=0)
	dfm.types[FTYPE_OSD_ROTATE].frame_nums    = 0;
    dfm.types[FTYPE_OSD_COMPRESS].frame_nums  = 0;
    dfm.types[FTYPE_OSD_DISPLAY].frame_nums   = 0;
    #endif
    frames_vr_init(vsrc_w,vsrc_h,scan_mode);
    frames_vd_init(vsrc_w,vsrc_h,scan_mode);
    frames_or_init(osd_w,osd_h,scan_mode);
    frames_oc_init(osd_w,osd_h,scan_mode);
    frames_od_init(osd_w,osd_h,scan_mode);
    
    frames_debug(&dfm.types[0]);
    frames_debug(&dfm.types[1]);
    frames_debug(&dfm.types[2]);
    frames_debug(&dfm.types[3]);
    frames_debug(&dfm.types[4]);
}

static void frames_vr_cfg(disp_frame_t* p_frame,u16 posX,u16 posY,u16 width,u16 height)
{
    u32 stride, y_size, size;
	if(p_frame->posX==posX&&p_frame->posY==posY&&p_frame->w==width&&p_frame->h==height)
		return;
    stride  = (width + LCD_DE_STRIDE) & (~LCD_DE_STRIDE);
    y_size  = stride * height;
    size    = y_size * 3 >>1;
    p_frame->uv_addr    = &p_frame->y_addr[y_size];
    p_frame->buf_size   = size;
    p_frame->w          = width;
    p_frame->h          = height;
	p_frame->posX       =posX;
	p_frame->posY       =posY;
    p_frame->stride     = stride;
	//p_frame->configLcdEn=1;
}
static void frames_vd_cfg(disp_frame_t* p_frame,u16 posX,u16 posY,u16 width,u16 height)
{
    u32 stride, y_size, size, w, h, _y_ofs, _uv_ofs;
	INT16U lcdW=0,lcdH=0,x,y;
	u8 scan_mode=hal_lcdGetShowingRotate();
    hal_lcdGetResolution(&lcdW,&lcdH);
	if(scan_mode==LCD_ROTATE_0)
	{
		x       =posX;
		y       =posY;
	}
	else if(scan_mode==LCD_ROTATE_90)
	{
		x       =posY;
		y       =lcdW-posX-width;
	}
	else if(scan_mode==LCD_ROTATE_270)
	{
		x       =lcdH-posY-height;
		y       =posX;
	}
	else
	{
		x       =lcdW-posX-width;
		y       =lcdH-posY-height;
	}
	w       = width;
    h       = height;
    if((scan_mode == LCD_ROTATE_90) || (scan_mode == LCD_ROTATE_270))
    {
        w   = height;
        h   = width;
        if(scan_mode == LCD_ROTATE_90)
            scan_mode = LCD_ROTATE_0;
        else
            scan_mode = LCD_ROTATE_180;
    }
	if(p_frame->posX==x&&p_frame->posY==y&&p_frame->w==w&&p_frame->h==h)
		return;
    stride  = (w + LCD_DE_STRIDE) & (~LCD_DE_STRIDE);
    y_size  = stride * h;
    size    = y_size * 3 >>1;
    hal_lcdAddrCalculate(w,h,scan_mode,stride,&_y_ofs,&_uv_ofs);
    p_frame->uv_addr    = &p_frame->y_addr[y_size];
    p_frame->_y_addr    = &p_frame->y_addr[_y_ofs];
    p_frame->_uv_addr   = &p_frame->uv_addr[_uv_ofs];
    p_frame->buf_size   = size;
    p_frame->w          = w;
    p_frame->h          = h;
	p_frame->posX       =x;
	p_frame->posY       =y;
    p_frame->stride     = stride;
    p_frame->scan_mode  = scan_mode;
	//p_frame->configLcdEn=1;
}
/*******************************************************************************
* Function Name  : hal_dispframeVideoCfg
* Description    : config video layer position and size
* Input          : p_frame: video frame
* 			   posX:video layer X position
*                    posY:video layer X position
*                    width:video layer width
*                    height:video layer height
posX+width do not large than lcd width
posY+height do not large than lcd height
* Output         : None
* Return         : None
*******************************************************************************/
void hal_dispframeVideoCfg(disp_frame_t* p_frame,u16 posX,u16 posY,u16 width,u16 height)
{
	HAL_CRITICAL_INIT();
    HAL_CRITICAL_ENTER();
#if 0
	if(NULL==p_frame)
	{
		HAL_CRITICAL_EXIT();
		uart_Printf("p_frame NULL\n");
		return;
	}
	uart_Printf("set %d\n",p_frame->id);
#endif
	if(p_frame->frame_type==FTYPE_VIDEO_ROTATE)
		frames_vr_cfg(p_frame,posX,posY,width,height);
	else if(p_frame->frame_type==FTYPE_VIDEO_DISPLAY)
		frames_vd_cfg(p_frame,posX,posY,width,height);
	HAL_CRITICAL_EXIT();
}
/*******************************************************************************
* Function Name  : hal_dispframeMalloc
* Description    : malloc a idle frame
* Input          : u8 frame_type : enum frame_type_u
* Output         : None
* Return         : NULL is fail
*******************************************************************************/
disp_frame_t * hal_dispframeMalloc(u8 frame_type)
{
    HAL_CRITICAL_INIT();
    HAL_CRITICAL_ENTER();
    
    disp_frame_t ** pp_free = &dfm.types[frame_type].freelist;
    disp_frame_t * p = NULL;

    if(*pp_free)
    {
        p = *pp_free;
        *pp_free = p->next;
        
        p->next = NULL;
        p->priv_func = NULL;
        p->data_size = 0;
        p->pip_sta = 0;
        p->buf_sta = 1;
    }
    
    HAL_CRITICAL_EXIT();
    
    if(!p)
    {
        //deg_Printf("[f_malloc]%d no frame\n",frame_type);
    }
    
    return p;
}
/*******************************************************************************
* Function Name  : hal_dispframeFree
* Description    : free a frame
* Input          : disp_frame_t * p_frame
* Output         : None
* Return         : None
*******************************************************************************/
void hal_dispframeFree(disp_frame_t * p_frame)
{
    HAL_CRITICAL_INIT();
    HAL_CRITICAL_ENTER();
    
    disp_frame_t ** pp_free = &dfm.types[p_frame->frame_type].freelist;
    
    if(p_frame && p_frame->buf_sta && (*pp_free != p_frame))
    {
        if(p_frame->next)
        {
            deg_Printf("[f_free] err frame\n");
        }
        
        p_frame->buf_sta = 0;
        p_frame->next = *pp_free;
        *pp_free = p_frame;
    }
    
    HAL_CRITICAL_EXIT();
}
/*******************************************************************************
* Function Name  : hal_dispframeSetCallback
* Description    : set frame callback func
* Input          : disp_frame_t * p_frame
*                  void (*priv_func)(disp_frame_t *)
* Output         : None
* Return         : None
*******************************************************************************/
void hal_dispframeSetCallback(disp_frame_t * p_frame,void (*priv_func)(disp_frame_t *))
{
    p_frame->priv_func = priv_func;
}
/*******************************************************************************
* Function Name  : hal_dispframeGetFrameNums
* Description    : get frame numbers of the type
* Input          : u32 type
* Output         : None
* Return         : None
*******************************************************************************/
u32 hal_dispframeGetFrameNums(u32 type)
{
    return dfm.types[type].frame_nums;
}

