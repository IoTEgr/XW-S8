/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         OSD HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_osd.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN OSD HAL LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"
#define SUPPORT_OSD1 HAL_CFG_OSD1_ENABLE
u8 OSD_double_buffer_flag = 0;
void hal_osdSetflag(u8 flag)
{
    OSD_double_buffer_flag = flag;
}

#define  OSD_ARG_CHECK(osd)  {if(((osd==OSD0)&&!HAL_CFG_OSD0_ENABLE)||((osd==OSD1)&&!HAL_CFG_OSD1_ENABLE)) return -1;}

static struct osd_config_s 
{
    u16 buf_w; //buffer width
    u16 buf_h;
    u16 layer_w; //
    u16 layer_h;
    u16 rotate_mdoe;
	u8* osdBuff;
	u32 osdBuffSize;
	//#if SUPPORT_OSD1
	u8* osdBuff2;
	u32 osdBuff2Size;
	u8* osd_cur_buf;
	//#endif
} osd_config[2];

/*******************************************************************************
* Function Name  : hal_osdGetIdleRotateBuffer
* Description    : get free osd rotate(input) buffer
* Input          : None
* Output         : None
* Return         : NULL is fail
*******************************************************************************/
disp_frame_t * hal_osdGetIdleRotateBuffer(void)
{
    disp_frame_t * p_osd_buffer = hal_dispframeMalloc(FTYPE_OSD_ROTATE);
    return p_osd_buffer;
}
/*******************************************************************************
* Function Name  : hal_osdGetIdleCmpBuffer
* Description    : get free osd compress(input) buffer
* Input          : None
* Output         : None
* Return         : NULL is fail
*******************************************************************************/
disp_frame_t * hal_osdGetIdleCmpBuffer(void)
{
    disp_frame_t * p_osd_buffer = hal_dispframeMalloc(FTYPE_OSD_COMPRESS);
    return p_osd_buffer;
}
/*******************************************************************************
* Function Name  : hal_osdGetIdleDispBuffer
* Description    : get free osd display(input) buffer
* Input          : None
* Output         : None
* Return         : NULL is fail
*******************************************************************************/
disp_frame_t * hal_osdGetIdleDispBuffer(void)
{
    disp_frame_t * p_osd_buffer = hal_dispframeMalloc(FTYPE_OSD_DISPLAY);
    return p_osd_buffer;
}
/*******************************************************************************
* Function Name  : hal_osdBufferRelease
* Description    : set a osdbuffer as free
* Input          : osd_buffer_t * p_osd_buffer
* Output         : None
* Return         : None
*******************************************************************************/
void hal_osdBufferRelease(disp_frame_t * p_osd_buffer)
{
    if(p_osd_buffer)
    {
        //deg_Printf("[osd]free %d,%d,%X\n",p_osd_buffer->frame_type,p_osd_buffer->id,p_osd_buffer->y_addr);
        hal_dispframeFree(p_osd_buffer);
    }
}
/*******************************************************************************
* Function Name  : hal_osdDealBuffer
* Description    : add buffer to queue
* Input          : osd_buffer_t * p_osd_buffer
* Output         : None
* Return         : None
*******************************************************************************/
void hal_osdDealBuffer(disp_frame_t * p_osd_buffer)
{
    if(p_osd_buffer->scan_mode == LCD_ROTATE_90)
    {
        //need rotate
        disp_frame_t * dst = hal_osdGetIdleCmpBuffer();
        if(dst)
        {
            dst->data_size = p_osd_buffer->data_size;
            hal_rotateAdd(ROTATE_CH1,p_osd_buffer,dst);
        }
        else
        {
            hal_dispframeFree(p_osd_buffer);
        }
    }
    else
    {
        //just compress
        disp_frame_t * dst = hal_osdGetIdleDispBuffer();
        if(dst)
        {
            hal_osdcmpStart(p_osd_buffer,dst);
        }
        else
        {
            hal_dispframeFree(p_osd_buffer);
        }
    }
}
/*******************************************************************************
* Function Name  : hal_osdInit
* Description    : osd initial
* Input          : osd : osd id,OSD0,OSD1
				 u16 width : width
				 u16 height: height
				u16 x : x
				u16 y: y
				u32 palette : palette addr
                u8 rotate_mode
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdInit(u8 osd,u16 width,u16 height,s16 x,s16 y,u32 palette,u8 rotate_mode)
{
#if HAL_CFG_EN_OSD > 0
    OSD_ARG_CHECK(osd);

    osd_config[osd].rotate_mdoe = rotate_mode;
    osd_config[osd].buf_w = (width + 3) & ~3;
    osd_config[osd].buf_h = (height + 3) & ~3;
    osd_config[osd].layer_w = osd_config[osd].buf_w;
    osd_config[osd].layer_h = osd_config[osd].buf_h;

    if(rotate_mode == LCD_ROTATE_90 || rotate_mode == LCD_ROTATE_270)
    {
        osd_config[osd].layer_w = osd_config[osd].buf_h;
        osd_config[osd].layer_h = osd_config[osd].buf_w;
    }
    
    u16 w = osd_config[osd].layer_w;
    u16 h = osd_config[osd].layer_h;
    
	ax32xx_deWait();
	ax32xx_osdSetPalette(osd,palette);
	ax32xx_osdSetPosition(osd,x,y);
	ax32xx_osdSetSize(osd,w,h);
    ax32xx_osdSetAlpha(osd,ALPHA_NORMAL,0);
   #if(OSD_SOFTWARE_ROTATE!=0)
   if(osd_config[osd].osdBuffSize!=osd_config[osd].buf_w*osd_config[osd].buf_h)
   	{
		if(osd_config[osd].osdBuff!=NULL)
		{
			hal_sysMemFree(osd_config[osd].osdBuff);
			osd_config[osd].osdBuff=NULL;
		}
   	}
 if (OSD_double_buffer_flag)
 {

   if(osd_config[osd].osdBuff2Size!=osd_config[osd].buf_w*osd_config[osd].buf_h)
   	{
		if(osd_config[osd].osdBuff2!=NULL)
		{
			hal_sysMemFree(osd_config[osd].osdBuff2);
			osd_config[osd].osdBuff2=NULL;
		}
   	}
}
   osd_config[osd].osdBuffSize=osd_config[osd].buf_w*osd_config[osd].buf_h;
   if(osd_config[osd].osdBuff==NULL)
   		osd_config[osd].osdBuff=hal_sysMemMalloc(osd_config[osd].osdBuffSize,32);
   if(osd_config[osd].osdBuff==NULL)
   	return -1;
if (OSD_double_buffer_flag)
{
   osd_config[osd].osdBuff2Size=osd_config[osd].buf_w*osd_config[osd].buf_h;
   if(osd_config[osd].osdBuff2==NULL)
   		osd_config[osd].osdBuff2=hal_sysMemMalloc(osd_config[osd].osdBuff2Size,32);
   if(osd_config[osd].osdBuff2==NULL)
   	return -1;
}
   uint32 i;
   for(i=0;i<osd_config[osd].osdBuffSize;i++)
   	osd_config[osd].osdBuff[i]=0xF9;
   ax32xx_sysDcacheWback((u32)osd_config[osd].osdBuff,osd_config[osd].osdBuffSize);
   ax32xx_deWait();
   ax32xx_osdSetAddr(osd,(u32)osd_config[osd].osdBuff,osd_config[osd].osdBuffSize);
if (OSD_double_buffer_flag)
{
   osd_config[osd].osd_cur_buf = osd_config[osd].osdBuff;
}
   #else
    disp_frame_t * p_osd_buffer = hal_osdGetIdleDispBuffer();
    if(p_osd_buffer)
    {
        u32 osdnum = w * h;
        u32 * p = (u32 *)p_osd_buffer->y_addr;
        
        while(osdnum)
        {
            u32 used = 0;
            *p++ = ax32xx_osdEncodeOneCode(0xF9,osdnum,&used);
            osdnum -= used;
            used = 0;
        }
        p_osd_buffer->data_size = (u32)p - (u32)p_osd_buffer->y_addr;
        
        ax32xx_sysDcacheWback((u32)p_osd_buffer->y_addr,p_osd_buffer->data_size);
        hal_osdSetBuffer(osd,p_osd_buffer);
    }
    #endif
	ax32xx_deUpdate();
#endif	
	return 0;
}
 u8* hal_osdGetCurBuff(u8 osd)
{
	if(osd<2)
	{
		if (OSD_double_buffer_flag)
    	{
			return (u8*)osd_config[osd].osd_cur_buf;
		}else
		{
			return (u8*)osd_config[osd].osdBuff;
		}
	}
	return NULL;
}

 u8* hal_osdGetFreeBuff(u8 osd)
{
	 if (osd < 2)
	 {
		 if (OSD_double_buffer_flag)
		 {
			 if (osd_config[osd].osd_cur_buf == osd_config[osd].osdBuff)
			 {
				 return osd_config[osd].osdBuff2;
			 }else
			 {
				 return osd_config[osd].osdBuff;
			 }
		 }else
		 {
			 return osd_config[osd].osdBuff;
		 } 
	 }
	 return NULL;

}


void hal_osdSetBuff(u8 osd,u32 addr)
{
	 if (OSD_double_buffer_flag)
	 {
		 if (osd < 2)
		 {
			 ax32xx_deWait();
			 ax32xx_osdSetAddr(osd, (u32)addr, osd_config[osd].osdBuffSize);
			 osd_config[osd].osd_cur_buf = (u8 *)addr;
		 }
	 }


}
/*******************************************************************************
* Function Name  : hal_osdEnable
* Description    : osd set enable
* Input          : osd : osd id,OSD0,OSD1
                     u8 en: 0-disable,1-enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdEnable(u8 osd,u8 en)
{
#if HAL_CFG_EN_OSD >0	
    OSD_ARG_CHECK(osd);
	if(OSD_double_buffer_flag==0 && en==0)
	{
		if(osd_config[osd].osdBuff2!=NULL)
		{
			//deg_Printf("osd config[osd].osdBuff2 %x\n",osd_config[osd].osdBuff2);
			hal_sysMemFree(osd_config[osd].osdBuff2);
			osd_config[osd].osdBuff2=NULL;
		}
	}

    ax32xx_osdEnable(osd,en);
	ax32xx_deUpdate();
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_osdSetAddr
* Description    : osd set addr
* Input          : osd : osd id,OSD0,OSD1
                      u32 addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetAddr(u8 osd,u32 addr)
{
#if HAL_CFG_EN_OSD >0	
    OSD_ARG_CHECK(osd);

    ax32xx_osdSetAddr(osd,addr,0);
	ax32xx_deUpdate();
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_osdSetBuffer
* Description    : osd set addr
* Input          : osd : osd id,OSD0,OSD1
                   osd_buffer_t * buf : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetBuffer(u8 osd,disp_frame_t * p_osd_buffer)
{
#if HAL_CFG_EN_OSD >0	
    OSD_ARG_CHECK(osd);
    
    static disp_frame_t * using_buf = NULL;
	static disp_frame_t * using_buf_last = NULL;
    if(p_osd_buffer)
    {
		//deg_Printf("[osd]set %d\n",p_osd_buffer->id);
       
	// deg_Printf("[osd]set %d,%x,%x\n",p_osd_buffer->id,p_osd_buffer->y_addr,p_osd_buffer->data_size);
		ax32xx_deWait();
        ax32xx_osdSetAddr(osd,(u32)p_osd_buffer->y_addr,p_osd_buffer->data_size);
        ax32xx_deUpdate();
        
        hal_osdBufferRelease(using_buf_last);
		using_buf_last = using_buf;
        using_buf = p_osd_buffer;
    }
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_osdSetPosition
* Description    : osd set position
* Input          : osd : osd id,OSD0,OSD1
				u16 x : x
				u16 y: y
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetPosition(u8 osd,s16 x,s16 y)
{
#if HAL_CFG_EN_OSD >0	
    OSD_ARG_CHECK(osd);

    ax32xx_osdSetPosition(osd,x,y);
	ax32xx_deUpdate();
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_osdSetPalette
* Description    : osd set palette
* Input          : osd : osd id,OSD0,OSD1
                      u32 addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetPalette(u8 osd,u32 addr)
{
#if HAL_CFG_EN_OSD >0
    OSD_ARG_CHECK(osd);

    ax32xx_osdSetPalette(osd,addr);
	ax32xx_deUpdate();
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_osdSetSize
* Description    : osd set size
* Input          : osd : osd id,OSD0,OSD1
                      u16 width : width
                      u16 height: height
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetSize(u8 osd,u16 width,u16 height)
{
#if HAL_CFG_EN_OSD >0	
    OSD_ARG_CHECK(osd);

    ax32xx_osdSetSize(osd,width,height);
	ax32xx_deUpdate();
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_osdSetAlpha
* Description    : osd set alpha
* Input          : osd : osd id,OSD0,OSD1
                      u8 type : ALPHA_NORMAL,ALPHA_GLOBAL
                      u8 value : gblobal alpha value
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_osdSetAlpha(u8 osd,u8 type,u8 value)
{
#if HAL_CFG_EN_OSD >0
    OSD_ARG_CHECK(osd);

    ax32xx_osdSetAlpha(osd,type,value);
	ax32xx_deUpdate();
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_osdGetBufferResolution
* Description    : hardware layer ,get osd buffer resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_osdGetBufferResolution(u8 osd,u16 *width,u16 *height)
{
#if HAL_CFG_EN_OSD > 0
	if(osd_config[osd].buf_h == 0)
		return -1;
	if(width)
		*width = osd_config[osd].buf_w;
	if(height)
		*height = osd_config[osd].buf_h;
#endif
	return 0;
}
/*******************************************************************************
* Function Name  : hal_deSet_DE_CCM
* Description    : set de's ccm
* Input          : u32 *p_ccm
* Output         : None
* Return         : None
*******************************************************************************/
void hal_deSet_DE_CCM(u32 *p_ccm)
{
	if(NULL == p_ccm)
		return;
	ax32xx_deSet_DE_CCM(p_ccm);
}

/*******************************************************************************
* Function Name  : hal_deSet_DE_SAJ
* Description    : set de's saj
* Input          : u32 *p_saj
* Output         : None
* Return         : None
*******************************************************************************/
void hal_deSet_DE_SAJ(u32 *p_saj)
{
	if(NULL == p_saj)
		return;
	ax32xx_deSet_DE_SAJ(p_saj);
}
