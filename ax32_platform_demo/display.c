/****************************************************************************
**
 **                              DISPLAY
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  DISPLAY
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : display.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : 
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"
#define  DISP_LAYER_VIDEO_EN      HAL_CFG_EN_LCD
#define  DISP_LAYER_OSD0_EN       (HAL_CFG_EN_OSD&HAL_CFG_OSD0_ENABLE)
#define  DISP_LAYER_OSD1_EN       (HAL_CFG_EN_OSD&HAL_CFG_OSD1_ENABLE)

typedef struct Disp_Layer_S
{
	INT16U framecnt;
	INT16U frameidx;
	INT16U width;
	INT16U height;
	INT16S x;
	INT16S y;
	INT32U frameaddr;
}Disp_Layer_T;
static Disp_Layer_T  DispNode[DISP_LAYER_MAX];

/*******************************************************************************
* Function Name  : dispLayerInit
* Description    : display layer initiail
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerInit(INT8U layer)
{
	INT16U width = 0,height = 0,x=0,y=0,lcdW=0,lcdH=0;

	if(layer>=DISP_LAYER_MAX)
		return -1;
	
	hal_lcdGetOSDShowingResolution(&width,&height);
	hal_lcdGetOSDShowingPosition(&x,&y);
	hal_lcdGetResolution(&lcdW,&lcdH);

	DispNode[layer].width = width;
	DispNode[layer].height= height;
	DispNode[layer].x = x;
	DispNode[layer].y = y;
	DispNode[layer].framecnt = 0;
	if(layer == DISP_LAYER_OSD0)
	{
#if DISP_LAYER_OSD0_EN >0
        INT8U * temp = (INT8U *)hal_sysMemMalloc(256 * 4,32);
        INT32 rotate_mode = hal_lcdGetShowingRotate();
        
        // load palette
        iconGetPalette(temp);
        #if(OSD_SOFTWARE_ROTATE==0)
        hal_osdcmpInit();
		#endif
		if(rotate_mode==LCD_ROTATE_0)
			hal_osdInit(OSD0,width,height,x,y,(u32)temp,rotate_mode);
		else if(rotate_mode==LCD_ROTATE_90)
        	hal_osdInit(OSD0,width,height,y,lcdW-x-width,(u32)temp,rotate_mode);
		else if(rotate_mode==LCD_ROTATE_270)
        	hal_osdInit(OSD0,width,height,lcdH-y-height,x,(u32)temp,rotate_mode);
		else
			hal_osdInit(OSD0,width,height,lcdW-x-width,lcdH-y-height,(u32)temp,rotate_mode);
        hal_osdEnable(OSD0,1);
        hal_sysMemFree(temp);
        
        DispNode[layer].frameaddr = 0;
        DispNode[layer].framecnt = 1;
        DispNode[layer].frameidx = 0;
#endif		
	}
	else if(layer == DISP_LAYER_OSD1)
	{
#if DISP_LAYER_OSD1_EN >0	

#endif		
	}
	else if(layer == DISP_LAYER_VIDEO)
	{
#if DISP_LAYER_VIDEO_EN > 0

		dispLayerSetRatio(DISP_LAYER_VIDEO,VIDEO_RATIO);
		dispLayerSetRotate(DISP_LAYER_VIDEO,VIDEO_ROTATE);
		hal_csiResolutionGet(&width,&height);
	  	hal_lcdPreviewStart(width,height);
		DispNode[layer].frameaddr = 0;
		DispNode[layer].framecnt = 1;
		DispNode[layer].frameidx = 0;
#endif		
	}
	
	return 0;
}
/*******************************************************************************
* Function Name  : dispLayerunInit
* Description    : display layer uninitiail
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerUninit(INT8U layer)
{
	
	if(layer == DISP_LAYER_OSD0)
	{
		hal_osdEnable(OSD0,0);
	}
	else if(layer == DISP_LAYER_OSD1)
	{
		hal_osdEnable(OSD1,0);
	}
	else if(layer == DISP_LAYER_VIDEO)
	{
		hal_lcdPreviewStop();
	}

	return 0;
}
/*******************************************************************************
* Function Name  : dispLayerResolution
* Description    : get display layer resolution
* Input          : INT8U layer : display layer index
                      INT16U *width : width 
                      INT16U *height : height
* Output         : none                                            
* Return         :int 0:success
*******************************************************************************/
int dispLayerGetResolution(INT8U layer,INT16U *width,INT16U *height)
{
    if(layer>=DISP_LAYER_MAX)
		return -1;

    if(layer == DISP_LAYER_OSD0 || layer == DISP_LAYER_OSD1)
    {
        hal_osdGetBufferResolution(layer - 1,&DispNode[layer].width,&DispNode[layer].height);
    }
    else if(layer == DISP_LAYER_VIDEO)
    {
        hal_lcdGetBufferResolution(&DispNode[layer].width,&DispNode[layer].height);
    }

	if(width)
		*width = DispNode[layer].width;
	if(height)
		*height= DispNode[layer].height;

	return 0;
}
/*******************************************************************************
* Function Name  : dispLayerSetResolution
* Description    : set display layer resolution
* Input          : INT8U layer : display layer index
                      INT16U *width : width 
                      INT16U *height : height
* Output         : none                                            
* Return         :int 0:success
*******************************************************************************/
int dispLayerSetResolution(INT8U layer,INT16U width,INT16U height)
{	
	if(layer == DISP_LAYER_VIDEO)
	{
#if DISP_LAYER_VIDEO_EN >0		
		 INT16U csi_w,csi_h;
         hal_csiResolutionGet(&csi_w,&csi_h);
		return hal_lcdSetShowingSize(csi_w,csi_h,width,height);
#endif		
	}
	else if(layer == DISP_LAYER_OSD0 || layer == DISP_LAYER_OSD1)
	{
#if DISP_LAYER_OSD0_EN || DISP_LAYER_OSD1_EN
		INT16S x,y;

		x = (DispNode[DISP_LAYER_VIDEO].width-width);
		y = (DispNode[DISP_LAYER_VIDEO].height-height);
		if(x<0)
			x = 0;
		if(y<0)
			y = 0;
		hal_osdSetPosition(layer-1,x>>1,y>>1);
		hal_osdSetSize(layer-1,width,height);
#endif			
	}
	else 
         return -1;

	return 0;
}
/*******************************************************************************
* Function Name  : dispLayerSetRotate
* Description    : set display layer raote,only used in video layer
* Input          : INT8U layer : display layer index
                      INT8U rotate:roate degree
* Output         : none                                            
* Return         :int 0:success
*******************************************************************************/
int dispLayerSetRotate(INT8U layer,INT8U rotate)
{
#if DISP_LAYER_VIDEO_EN >0	
	if(layer == DISP_LAYER_VIDEO)
	{
		return hal_lcdSetRoate(rotate);
	}
#endif
	return -1;
}
/*******************************************************************************
* Function Name  : dispRatioResolutionCalaulate
* Description    : calculate resolution of ratio
* Input          : INT8U ratio : ratio .4:3,16:9,16:10
                       INT16U *width : width 
                      INT16U *height : height
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispRatioResolutionCalculate(INT8U ratio,INT16U *width,INT16U *height)
{
	const INT8U ratioWTable[] = {4,16,16},ratioHTable[] = {3,9,10}; // 4:3,16:9,16:10
	INT8U rw,rh;
	int ratw,rath;
	INT16U w,h;

	if(ratio >=3)
		return 1;
	rw = ratioWTable[ratio];
	rh = ratioHTable[ratio];
	
	ratw = *width/rw;
	rath = *height/rh;
	if(ratw == rath)
		return 0;

	h = ratw*rh;
	if(h>*height)
	{
		w = rath*rw;
		if(w<*width)
			*width = w;
	}
	else
		*height = h;
    return 0;
}
/*******************************************************************************
* Function Name  : dispLayerSetRatio
* Description    : display set video ratio,only used on video layer
* Input          : INT8U layer : layer index,video,osd0,osd1
                      INT8U ratio : ratio .4:3,16:9,16:10
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerSetRatio(INT8U layer,INT8U ratio)
{
	INT16U width,height;
#if DISP_LAYER_VIDEO_EN >0	
	if((layer == DISP_LAYER_VIDEO)&&(ratio<3))
	{		
		width = DispNode[DISP_LAYER_VIDEO].width;
		height = DispNode[DISP_LAYER_VIDEO].height;
		dispRatioResolutionCalculate(ratio,&width,&height);
		return dispLayerSetResolution(DISP_LAYER_VIDEO,width,height);
		
	}
#endif

	return -1;
}
/*******************************************************************************
* Function Name  : dispFrameShow
* Description    : display show for video,osd0,osd1 set buffer to show,can not used in csi preview
* Input          : INT8U layer : layer index,video,osd0,osd1
                      INT32U addr: buffer to show
                      INT16U width:buffer width
                      INT16U height:buffer height
                      INT8U rotate: rotate
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerShow(INT8U layer,INT32U addr,INT16U width,INT16U height,INT8U rotate)
{
#if DISP_LAYER_VIDEO_EN >0	
	if(layer == DISP_LAYER_VIDEO) // lcd can not do scaler itself.user should calculate the width &height by dispRatioResolutionCalculate,before jpeg decode
	{
        //INT16U x,y;
		//hal_lcdGetShowingPosition(&x,&y);
		hal_lcdSetFrame(0,0,0,0,(void *)addr,0);
	}
#endif

	return 0;
}
/*******************************************************************************
* Function Name  : dispLayerGetFrame
* Description    : display get osd0,osd1 frame buffer
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         :0
*******************************************************************************/
void *dispLayerGetFrame(INT8U layer)
{
	if(layer>=DISP_LAYER_MAX)
		return NULL;

    void * res = NULL;

    switch(layer)
    {
		case DISP_LAYER_VIDEO:
			   res = ((void *)hal_lcdGetIdleBuffer());
               break;
	    case DISP_LAYER_OSD0:
	    case DISP_LAYER_OSD1:
        {
			#if(OSD_SOFTWARE_ROTATE!=0)
			res = hal_osdGetCurBuff(OSD0);
			#else
            INT32U r = hal_lcdGetShowingRotate();
            if((r == LCD_ROTATE_90) || (r == LCD_ROTATE_270))
                res = dfm.types[FTYPE_OSD_ROTATE]._frames[0].y_addr;
            else
                res = dfm.types[FTYPE_OSD_COMPRESS]._frames[0].y_addr;
			#endif
            break;
        }
    }

	return res;
}
/*******************************************************************************
* Function Name  : dispLayerGetFreeFrame
* Description    : display get osd0,osd1 frame buffer
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         :0
*******************************************************************************/
void *dispLayerGetFreeFrame(INT8U layer)
{
	if(layer>=DISP_LAYER_MAX)
		return NULL;

    void * res = NULL;

    switch(layer)
    {
		case DISP_LAYER_VIDEO:
               break;
	    case DISP_LAYER_OSD0:
	    case DISP_LAYER_OSD1:
        {
			#if(OSD_SOFTWARE_ROTATE!=0)
			res = hal_osdGetFreeBuff(OSD0);
			#else
            INT32U r = hal_lcdGetShowingRotate();
            if((r == LCD_ROTATE_90) || (r == LCD_ROTATE_270))
                res = dfm.types[FTYPE_OSD_ROTATE]._frames[0].y_addr;
            else
                res = dfm.types[FTYPE_OSD_COMPRESS]._frames[0].y_addr;
			#endif
            break;
        }
    }

	return res;
}

/*******************************************************************************
* Function Name  : dispLayerFreeFrame
* Description    : display get osd0,osd1 frame buffer
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         : 0
*******************************************************************************/
s32 dispLayerFreeFrame(INT8U layer,void * frame)
{
	if(layer>=DISP_LAYER_MAX || !frame)
		return -1;

    switch(layer)
    {
		case DISP_LAYER_VIDEO:
			   hal_dispframeFree(frame);
               break;
	    case DISP_LAYER_OSD0:
	    case DISP_LAYER_OSD1:
        {
            
        }
    }

    return 0;
}
/*******************************************************************************
* Function Name  : dispLayerSetFrame
* Description    : display show for osd0,osd1
* Input          : INT8U layer : layer index,video,osd0,osd1
                      INT32U addr: buffer to show
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerSetFrame(INT8U layer,u32 addr)
{
	if(layer>=DISP_LAYER_MAX)
		return -1;

	if((layer == DISP_LAYER_OSD0) || (layer == DISP_LAYER_OSD1))
	{
		hal_osdSetBuff(OSD0,addr);
	}
	return 0;
}
/*******************************************************************************
* Function Name  : dispLayerPIPOnlyFront
* Description    : display only front sensor
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void dispLayerPIPOnlyFront(void)
{
    INT16U video_w,video_h;
    hal_lcdGetBufferResolution(&video_w,&video_h);
    hal_lcdPreSetPIPEnable(1);
    hal_lcdSetVCH(VCH_ISP,VCH_TOP,0,0,video_w,video_h,VCH_EN);

/*
	if(backrec_adpt()){
		hal_lcdSetVCH(VCH_JPG,VCH_BOT,0,0,video_w,video_h,VCH_EN);
	}else{
		hal_lcdSetVCH(VCH_JPG,VCH_BOT,0,0,video_w,video_h,VCH_DIS);
	}
*/
	hal_lcdSetVCH(VCH_JPG,VCH_BOT,0,0,video_w,video_h,VCH_DIS);

  //  debg("lcdwinset: <only isp>\n");
}
/*******************************************************************************
* Function Name  : dispLayerPIPOnlyBack
* Description    : display only back sensor
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void dispLayerPIPOnlyBack(void)
{
    INT16U video_w,video_h;
    hal_lcdGetBufferResolution(&video_w,&video_h);
    hal_lcdPreSetPIPEnable(1);
    hal_lcdSetVCH(VCH_ISP,VCH_BOT,0,0,video_w,video_h,VCH_EN);
    hal_lcdSetVCH(VCH_JPG,VCH_TOP,0,0,video_w,video_h,VCH_EN);
  //  debg("lcdwinset: <only jpg>\n");
}
/*******************************************************************************
* Function Name  : dispLayerPIPBakRightTop
* Description    : display back sensor is right-top
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void dispLayerPIPBakRightTop(void)
{
    INT16U win_w = 160,win_h = 120;
    INT16U video_w,video_h,x,y;
    
    hal_lcdGetBufferResolution(&video_w,&video_h);
    x = (video_w - win_w) & ~7;
    win_w = video_w - x;
    //y = video_h - win_h;
    y= 20 ;
    hal_lcdPreSetPIPEnable(1);
    hal_lcdSetVCH(VCH_ISP,VCH_BOT,0,0,video_w,video_h,VCH_EN);
    hal_lcdSetVCH(VCH_JPG,VCH_TOP,x,y,win_w,win_h,VCH_EN);
   // debg("lcdwinset:<jpg right-top>\n");
}
/*******************************************************************************
* Function Name  : dispLayerPIPFrtRightTop
* Description    : display front sensor is right-top
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void dispLayerPIPFrtRightTop(void)
{
    INT16U video_w,video_h;
    hal_lcdGetBufferResolution(&video_w,&video_h);
    hal_lcdPreSetPIPEnable(1);
    hal_lcdSetVCH(VCH_ISP,VCH_TOP,video_w-160,20,160,120,VCH_EN);
    hal_lcdSetVCH(VCH_JPG,VCH_BOT,0,0,video_w,video_h,VCH_EN);
   // debg("winset: <isp right-top>\n");
}
/*******************************************************************************
* Function Name  : dispLayerPIPFBHalf
* Description    : display front-sensor is left,back-sensor is right
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void dispLayerPIPFBHalf(void)
{
    INT16U video_w,video_h;
    hal_lcdGetBufferResolution(&video_w,&video_h);
    hal_lcdPreSetPIPEnable(0);
    hal_lcdSetVCH(VCH_ISP,VCH_BOT,0,0,video_w/2,video_h,VCH_EN);
    hal_lcdSetVCH(VCH_JPG,VCH_TOP,video_w/2,0,video_w/2,video_h,VCH_EN);
  //  debg("winset: <isp left,jpg right>\n");
}
/*******************************************************************************
* Function Name  : dispLayerPIPBFHalf
* Description    : display front-sensor is right,back-sensor is left
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void dispLayerPIPBFHalf(void)
{
    INT16U video_w,video_h;
    hal_lcdGetBufferResolution(&video_w,&video_h);
    hal_lcdPreSetPIPEnable(0);
    hal_lcdSetVCH(VCH_ISP,VCH_BOT,video_w/2,0,video_w/2,video_h,VCH_EN);
    hal_lcdSetVCH(VCH_JPG,VCH_TOP,0,0,video_w/2,video_h,VCH_EN);
  //  debg("winset: <jpg left,isp right>\n");
}
/*******************************************************************************
* Function Name  : dispLayerSetResolution
* Description    : set Pic-in-Pic mode
* Input          : INT8U mode : mode index
* Output         : none                                            
* Return         : int 0:success
*******************************************************************************/
int dispLayerSetPIPMode(INT8U mode)
{
    static void * modes[] = {
        [DISP_PIP_FRONT] = dispLayerPIPOnlyFront,
        [DISP_PIP_BACK] = dispLayerPIPOnlyBack,
        [DISP_PIP_BACK_RIGHTTOP] = dispLayerPIPBakRightTop,
        [DISP_PIP_FRONT_RIGHTTOP] = dispLayerPIPFrtRightTop,
        [DISP_PIP_FRONT_LEFT_BACK_RIGHT] = dispLayerPIPFBHalf,
        [DISP_PIP_FRONT_RIGHT_BACK_LEFT] = dispLayerPIPBFHalf,
    };
    
    if(DISP_PIP_DISABLE == mode)
    {
        hal_lcdSetPIPEnable(0);
        return 0;
    }
    deg_Printf("pip mode = %d\n",mode);
    if(mode > (ARRAY_NUM(modes) - 1))
        return -1;
        
    ((void (*)(void))modes[mode])();
    return 0;
}


void dispLayer_ABwin(INT8U mode)
{
	//debg("set win:%x,%x\n",SysCtrl.pip_mode,mode);
	//if(SysCtrl.pip_mode != mode){
	deg_Printf("dispLayer_ABwin pip mode = %d\n",mode);
		SysCtrl.pip_mode = mode;
		dispLayerSetPIPMode(SysCtrl.pip_mode);	
	//}
}
u8 dispLayer_getABwin(void)
{
	return SysCtrl.pip_mode;
}



/*******************************************************************************
* Function Name  : dispLayerSetFrontCrop
* Description    : set Pic-in-Pic mode
* Input          : INT8U level :  scale level
                    :mjpg_crop: 0 recode crop image , 1 recode sensor image
* Output         : none                                            
* Return         : int 0:success
*******************************************************************************/

int dispLayerSetFrontCrop(INT8U level,u8 mjpg_crop)
{
//=====720P sensor , show 4:3 lcd ====
	u16 csi_w,csi_h,crop_w,crop_h;
	u16 sx=0,sy=0,ex=0,ey=0;

	hal_csiResolutionGet(&csi_w,&csi_h);
	deg_Printf("dispLayerSetFrontCrop:csi_w=%d,csi_h=%d\n",csi_w,csi_h);
	if(1280 == csi_w)
	{
		//crop_w = 960;	// 960/720 = 4/3
		//crop_h = 720;
		crop_w = 1280;	// 16:9
		crop_h = 720;
#if (1 == LCD_CROP_SCALE)
		if(level < ARRAY_NUM(crop_levels))
		{
			u32 l = crop_levels[level];
			u16 scale_crop_w,scale_crop_h;
			scale_crop_w = ((1280 * l / 100) + 0x1f) & (~0x1f);debgreg(crop_w);
			scale_crop_h = (crop_h * l / 100) & ~1;debgreg(crop_h);
			sx = (crop_w-scale_crop_w) / 2;
			ex = sx + scale_crop_w;
			sy = (crop_h-scale_crop_h) / 2;
			ey = sy + scale_crop_h;
		}
#else
		sx = (csi_w - crop_w) / 2;
		ex = sx + crop_w;
		sy = (csi_h - crop_h) / 2;
		ey = sy + crop_h;
#endif
		deg_Printf("crop_w=%d,crop_h=%d,sx=%d,ex=%d,sy=%d,ey=%d\n",crop_w,crop_h,sx,ex,sy,ey);
		hal_lcdSetCsiCrop(sx,sy,ex,ey);

		if(mjpg_crop)
		{
			deg_Printf("mjprec:crop_w=%d,crop_h=%d,sx=%d,ex=%d,sy=%d,ey=%d\n",crop_w,crop_h,sx,ex,sy,ey);
			hal_mjpeg_avi_crop(csi_w,csi_h,crop_w,crop_h,sx,sy,ex,ey);	// VGA sensor , crop rec 640*360
		}
		else
		{
			static u8 first_setting=0;
			if(0 == first_setting)
			{
				deg_Printf("mjprec:crop_w=%d,crop_h=%d,sx=%d,ex=%d,sy=%d,ey=%d\n",crop_w,crop_h,sx,ex,sy,ey);
				hal_mjpeg_avi_crop(csi_w,csi_h,crop_w,crop_h,sx,sy,ex,ey);	// VGA sensor , crop rec 640*360
				first_setting = 1;
			}
		}
	}
	else
	{
		crop_w = csi_w;
		crop_h = csi_h;
#if (1 == LCD_CROP_SCALE)	
		if(level < ARRAY_NUM(crop_levels))
		{
			u32 l = crop_levels[level];
			u16 scale_crop_w,scale_crop_h,encode_h;
			#if PIC_16_9
			u16 half_720W,half_720H;
			hal_csiHalf720P_ResolutionGet(&half_720W,&half_720H);	//get half 720P resolution
			encode_h = half_720H;
			#else
			encode_h = crop_h;
			#endif
			scale_crop_w = (crop_w * l / 100) & ~1;debgreg(crop_w);
			scale_crop_h = (encode_h * l / 100) & ~1;debgreg(encode_h);
			sx = (crop_w-scale_crop_w) / 2;
			ex = sx + scale_crop_w;
			sy = (crop_h-scale_crop_h) / 2;
			ey = sy + scale_crop_h;
		}
#else
		sx = (csi_w - crop_w) / 2;
		ex = sx + crop_w;
		sy = (csi_h - crop_h) / 2;
		ey = sy + crop_h;
#endif
		deg_Printf("LCDshow:crop_w=%d,crop_h=%d,sx=%d,ex=%d,sy=%d,ey=%d\n",crop_w,crop_h,sx,ex,sy,ey);
		hal_lcdSetCsiCrop(sx,sy,ex,ey);

		if(mjpg_crop)
		{
			crop_w = 640;
			crop_h = 360;
			sx = (csi_w - crop_w) / 2;
			ex = sx + crop_w;
			sy = (csi_h - crop_h) / 2;
			ey = sy + crop_h;
			deg_Printf("mjprec:crop_w=%d,crop_h=%d,sx=%d,ex=%d,sy=%d,ey=%d\n",crop_w,crop_h,sx,ex,sy,ey);
			hal_mjpeg_avi_crop(csi_w,csi_h,crop_w,crop_h,sx,sy,ex,ey);	// VGA sensor , crop rec 640*360
		}
		else
		{
			static u8 first_setting=0;
			if(0 == first_setting)
			{
				crop_w = 640;
				crop_h = 360;
				sx = (csi_w - crop_w) / 2;
				ex = sx + crop_w;
				sy = (csi_h - crop_h) / 2;
				ey = sy + crop_h;
				deg_Printf("mjprec:crop_w=%d,crop_h=%d,sx=%d,ex=%d,sy=%d,ey=%d\n",crop_w,crop_h,sx,ex,sy,ey);
				hal_mjpeg_avi_crop(csi_w,csi_h,crop_w,crop_h,sx,sy,ex,ey);	// VGA sensor , crop rec 640*360
				first_setting = 1;
			}
		}
	}

    return 0;
}














