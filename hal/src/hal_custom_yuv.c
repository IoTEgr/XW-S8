
/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         CSI HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_csi.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN CSI HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"
#include "../../ax32_platform_demo/application.h"


YUV_ADD_T   yuv_add_s;
enum
{
	FRAME_PREPARE_NULL = 0,
	FRAME_BUF_MALLOC,
	FRAME_PREPARE_OK,
	FRAME_ADD
};

/*
enum
{
	FRAME_NULL = 0,
	FRAME_ADD
};
*/
extern int nv_open(int res_num);
extern int nv_size(int res_num);
extern int nv_read(int addr,void *buffer,int size);

#if 0
/*******************************************************************************
* Function Name  : hal_custom_yuv_init
* Description    : frame add yuv buf malloc
* Input          : None
* Output         : None
* Return         : 0:Sucess; -1: GET RESOLUTION FAIL -2: MALLOC BUF FAIL
*******************************************************************************/
INT32S hal_custom_frame_init(void)  //init buf
{
	INT32S ret = 0;
	//--------init yuv_add_s ---------
	memset((void*)&yuv_add_s,0, sizeof(YUV_ADD_T));
	if(hal_csiResolutionGet(&yuv_add_s.sensor_width,&yuv_add_s.sensor_height)<0)
	{
		deg_Printf("----get csi resolution fail-----\n");
		ret = -1;
		goto INIT_FAIL;
	}
	else
	{
		deg_Printf("----sensor width:%d, height:%d-----<<\n",yuv_add_s.sensor_width,yuv_add_s.sensor_height);
		yuv_add_s.sensor_frame_size = yuv_add_s.sensor_width*yuv_add_s.sensor_height;
		yuv_add_s.sensor_yuv_addr = (u32)hal_sysMemMalloc(yuv_add_s.sensor_frame_size*3/2,64);
		if(yuv_add_s.sensor_yuv_addr == 0)
		{
			deg_Printf("----frame add sensor buf malloc fail-----\n");
			ret = -2;
			goto INIT_FAIL;
		}
		else
		{
			deg_Printf("----frame add sensor buf adr:%x, size:%d\n",yuv_add_s.sensor_yuv_addr,yuv_add_s.sensor_frame_size*3/2);
		}
	}
	
	//if(hal_lcdGetOSDShowingResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height)<0)
	if(hal_lcdGetBufferResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height)<0)
	{
		deg_Printf("----get lcd resolution fail-----\n");
		ret = -1;
		goto INIT_FAIL;
	}
	else
	{
		deg_Printf("----lcd width:%d, height:%d-----\n",yuv_add_s.lcd_width,yuv_add_s.lcd_height);
		yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;
		yuv_add_s.lcd_yuv_addr = (u32)hal_sysMemMalloc(yuv_add_s.lcd_frame_size*3/2,64);
		if(yuv_add_s.lcd_yuv_addr == 0)
		{
			deg_Printf("----frame add  lcd buf malloc fail-----\n");
			ret = -2;
			goto INIT_FAIL;
		}
		else
		{
			deg_Printf("----frame add lcd buf adr:%x, size:%d\n",yuv_add_s.lcd_yuv_addr,yuv_add_s.lcd_frame_size*3/2);
		}
	}

	yuv_add_s.lcd_table_size = yuv_add_s.lcd_height*32;
	yuv_add_s.lcd_table_t = (u32)hal_sysMemMalloc(yuv_add_s.lcd_table_size*8,64);
	if(yuv_add_s.lcd_table_t == 0)
	{
		deg_Printf("frame add lcd_table_t malloc fail\n");
		ret = -2;
		goto INIT_FAIL;
	}

	yuv_add_s.sensor_table_size = yuv_add_s.sensor_height*32;
	yuv_add_s.sensor_table_t = (u32)hal_sysMemMalloc(yuv_add_s.sensor_table_size*8,64);
	if(yuv_add_s.sensor_table_t == 0)
	{
		deg_Printf("frame add sensor_table_t malloc fail\n");
		ret = -2;
		goto INIT_FAIL;
	}

	ret = 0;
	yuv_add_s.frame_stat = FRAME_BUF_MALLOC;
	return ret;

INIT_FAIL:
	
	if(yuv_add_s.lcd_yuv_addr)
		hal_sysMemFree((void *)(yuv_add_s.lcd_yuv_addr));
	if(yuv_add_s.lcd_table_t)
		hal_sysMemFree((void *)(yuv_add_s.lcd_table_t));
	if(yuv_add_s.sensor_yuv_addr)
		hal_sysMemFree((void *)(yuv_add_s.sensor_yuv_addr));
	if(yuv_add_s.sensor_table_t)
		hal_sysMemFree((void *)(yuv_add_s.sensor_table_t));

	yuv_add_s.frame_stat = FRAME_PREPARE_NULL;

	return ret;
			
}
/*******************************************************************************
* Function Name  : hal_custom_frame_create
* Description    : hal_custom_frame_create
* Input          : frame id :read from resource
* Output         : None
* Return         : 0:Sucess; -1: Fail
*******************************************************************************/

INT32S hal_custom_frame_create(INT32U idx)
{
	
	INT8U *jpegBuffer, *jpegBuffer_lcd, *jpegBuffer_sensor;
	INT32S len,addr;
	u16 src_width , src_height, decode_lcd_width, decode_lcd_height, decode_sensor_width, decode_sensor_height;
	u32 decode_lcd_size, decode_sensor_size;
	INT32S ret = 0;
	INT8U  backrec_stat = 0;
	jpegBuffer = jpegBuffer_lcd = jpegBuffer_sensor = NULL;
	//--------------frame buf check
	if(yuv_add_s.frame_stat == FRAME_PREPARE_NULL)
	{
		deg_Printf("frame buffer have not been malloced\n");
		return -1;
	}
	//--------init yuv_add_s ---------
	HAL_CRITICAL_INIT();
	HAL_CRITICAL_ENTER();
	
	
	while(hal_BackRecDecodeStatusCheck() || 
        !ax32xx_csiScalerCheck());
	addr = nv_open((int)idx);
	if(addr<0)
	{
		deg_Printf("open frame from resourse fail\n");
		ret = -1;
		goto CREATE_FAIL;
	}
	jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(idx),64);
	if(jpegBuffer == NULL)
	{
		deg_Printf("hal_custom_frame_create malloc fail\n");
		ret = -1;
		goto CREATE_FAIL;
	}
	else
	{
		len = nv_size((int)idx);
		nv_read(addr,jpegBuffer,len);					
	}
	if(api_backrec_status())
	{
		backrec_stat = 1;
		backrec_adpt_set(0);
	}
	hal_lcdSetPIPEnable(0);
	
	
	hal_mjpegHeaderParse(jpegBuffer);
	hal_mjpegDecodeGetResolution(&src_width,&src_height);
	//deg_Printf("src_width:%d,src_height:%d\n",src_width,src_height);
	
	yuv_add_s.frame_stat = FRAME_BUF_MALLOC;
	
//--------if want to decode to bigger width_height,need decode(1:1)-->encode(zoon up)-->decode(1:1)
	if(src_height<yuv_add_s.lcd_height){
		decode_lcd_width  = src_width;
		decode_lcd_height = src_height;
		//ax32xx_sysDcacheWback(jpegBuffer,len);
		jpegBuffer_lcd = (INT8U *)hal_sysMemMalloc(100*1024,64);
		if(jpegBuffer_lcd == NULL)
		{
			deg_Printf("hal_custom_frame jpegBuffer_lcd malloc fail\n");
			ret = -1;
			goto CREATE_FAIL;
		}
	}else{
		decode_lcd_width  = yuv_add_s.lcd_width;
		decode_lcd_height = yuv_add_s.lcd_height;
	}
	decode_lcd_size = decode_lcd_width*decode_lcd_height;
		const u32 levels[] = {100, 96, 92, 88, 84, 80, 76, 72, 68, 64, 60, 56, 52, 48, 44, 40, 36}; //320*240lcd  min :35%
		u16 scale_crop_w=0,scale_crop_h=0;
		if(SysCtrl.crop_level < ARRAY_NUM(levels))
		{
			u32 l = levels[SysCtrl.crop_level];
			scale_crop_w = ((1280 * l / 100) + 0x1f) & (~0x1f);
			scale_crop_h = (720 * l / 100) & ~1;
		}
		//yuv_add_s.sensor_frame_size = scale_crop_w*scale_crop_h;
		yuv_add_s.sensor_width = scale_crop_w;
		yuv_add_s.sensor_height = scale_crop_h;
		
	if(src_height<yuv_add_s.sensor_height){
		decode_sensor_width  = src_width;
		decode_sensor_height = src_height;
		jpegBuffer_sensor = (INT8U *)hal_sysMemMalloc(100*1024,64);
		if(jpegBuffer_sensor == NULL)
		{
			deg_Printf("hal_custom_frame jpegBuffer_lcd malloc fail\n");
			ret = -1;
			goto CREATE_FAIL;
		}
	}else{
		decode_sensor_width  = yuv_add_s.sensor_width;
		decode_sensor_height = yuv_add_s.sensor_height;
	}
	decode_sensor_size = decode_sensor_width*decode_sensor_height;
	yuv_add_s.sensor_frame_size = decode_sensor_size;

/*	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.sensor_yuv_addr),(u8*)(yuv_add_s.sensor_yuv_addr+decode_sensor_size),yuv_add_s.sensor_width,yuv_add_s.sensor_height)<0)
	{
		ret = -1;
		goto CREATE_FAIL;
	}
		
	if(!jpeg1_decode_check())
	{
		ret = -1;
		goto CREATE_FAIL;
	}
*/
	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.lcd_yuv_addr),(u8*)(yuv_add_s.lcd_yuv_addr+decode_lcd_size),decode_lcd_width,decode_lcd_height)<0)
	{
		ret = -1;
		goto CREATE_FAIL;
	}
		
	if(!jpeg1_decode_check())
	{
		ret = -1;
		goto CREATE_FAIL;
	}
/*
	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.usensor_yuv_addr),(u8*)(yuv_add_s.usensor_yuv_addr+decode_usensor_size),decode_usensor_width,decode_usensor_height)<0)
	{
		ret = -1;
		goto CREATE_FAIL;
	}
		
	if(!jpeg1_decode_check())
	{
		ret = -1;
		goto CREATE_FAIL;
	}
	
*/
	//deg_Printf("src_width:%d,src_height:%d\n",src_width,src_height, yuv_add_s.lcd_width, yuv_add_s.lcd_height);
	//deg_Printf("yuv_add_s.lcd_width:%d,yuv_add_s.lcd_height:%d\n",yuv_add_s.lcd_width, yuv_add_s.lcd_height);
	//deg_Printf("yuv_add_s.sensor_width:%d,yuv_add_s.sensor_height:%d\n\n\n",yuv_add_s.sensor_width, yuv_add_s.sensor_height);
	if((src_height<yuv_add_s.sensor_height)&&(jpegBuffer_sensor))
	{
		//encode
		ax32xx_mjpB_Ctl_init(0, JPEG_Q_75, decode_sensor_width, decode_sensor_height, 
							yuv_add_s.sensor_width, yuv_add_s.sensor_height
							);
		ax32xx_mjpB_Linebuf_cfg((u8*)(yuv_add_s.sensor_yuv_addr),(u8*)(yuv_add_s.sensor_yuv_addr+decode_sensor_size));
		ax32xx_mjpB_dma_cfg((u32)jpegBuffer_sensor,(u32)(jpegBuffer_sensor+1024*1024));
		hal_watermarkEnable(0,0);
		ax32xx_mjpB_on_2();
		if(ax32xx_mjpegB_encode_check()<0)
		{
			ret = -1;
			
			goto CREATE_FAIL;
		}
		
		
		// decode again
		if(hal_mjpegDecodeNoIsr(jpegBuffer_sensor,(u8*)yuv_add_s.sensor_yuv_addr,(u8*)(yuv_add_s.sensor_yuv_addr+yuv_add_s.sensor_frame_size),
									yuv_add_s.sensor_width,yuv_add_s.sensor_height)<0)
		{
			ret = -1;
			goto CREATE_FAIL;
		}
		if(!jpeg1_decode_check())
		{
			ret = -1;
			goto CREATE_FAIL;
		}
	}
	
	if((src_height<yuv_add_s.lcd_height)&&(jpegBuffer_lcd)){
			
		ax32xx_mjpB_Ctl_init(0, JPEG_Q_75, decode_lcd_width, decode_lcd_height, 
							yuv_add_s.lcd_width, yuv_add_s.lcd_height
							);
		ax32xx_mjpB_Linebuf_cfg((u8*)(yuv_add_s.lcd_yuv_addr),(u8*)(yuv_add_s.lcd_yuv_addr+decode_lcd_size));
		ax32xx_mjpB_dma_cfg((u32)jpegBuffer_lcd,(u32)(jpegBuffer_lcd+100*1024));
		hal_watermarkEnable(0,0);
		ax32xx_mjpB_on_2();
		if(ax32xx_mjpegB_encode_check()<0)
		{
			ret = -1;
			goto CREATE_FAIL;
		}
		
		if(hal_mjpegDecodeNoIsr(jpegBuffer_lcd,(u8*)(yuv_add_s.lcd_yuv_addr),(u8*)(yuv_add_s.lcd_yuv_addr+yuv_add_s.lcd_frame_size),
									yuv_add_s.lcd_width,yuv_add_s.lcd_height)<0)
		{
			ret = -1;
			goto CREATE_FAIL;
		}
		
		if(!jpeg1_decode_check())
		{
			ret = -1;
			goto CREATE_FAIL;
		}
		
	}

	if(hal_custom_frame_scan()<0)
	{
		ret = -1;
		deg_Printf("---------frame scan fail-------------\n");
		goto CREATE_FAIL;
	}

	if(jpegBuffer)
		hal_sysMemFree((void *)jpegBuffer);
	if(jpegBuffer_lcd)
		hal_sysMemFree((void *)jpegBuffer_lcd);
	if(jpegBuffer_sensor)
		hal_sysMemFree((void *)jpegBuffer_sensor);
//	if(jpegBuffer_usensor)
//		hal_sysMemFree((void *)jpegBuffer_usensor);
	yuv_add_s.frame_stat = FRAME_ADD;
	HAL_CRITICAL_EXIT();
	
	return ret;
	
CREATE_FAIL:
	if(jpegBuffer)
		hal_sysMemFree((void *)jpegBuffer);
	if(jpegBuffer_lcd)
		hal_sysMemFree((void *)jpegBuffer_lcd);
	if(jpegBuffer_sensor)
		hal_sysMemFree((void *)jpegBuffer_sensor);
//	if(jpegBuffer_usensor)
//		hal_sysMemFree((void *)jpegBuffer_usensor);
//	if(backrec_stat)
//	{
		//backrec_adpt_set(1);
//		api_backrec_relinking();
//	}
	(void)backrec_stat;
	HAL_CRITICAL_EXIT();
	return ret;
}
/*******************************************************************************
* Function Name  : hal_custom_frame_create
* Description    : hal_custom_frame_create
* Input          : frame id :read from resource
* Output         : None
* Return         : 0:Sucess; -1: Fail
*******************************************************************************/
INT32S hal_custom_frame_create_zoom(INT32U idx)
{
	
	INT8U *jpegBuffer, *jpegBuffer_lcd, *jpegBuffer_sensor;
	INT32S len,addr;
	u16 src_width , src_height, /*decode_lcd_width, decode_lcd_height,*/ decode_sensor_width, decode_sensor_height;
	u32 /*decode_lcd_size,*/ decode_sensor_size;
	INT32S ret = 0;
	INT8U  backrec_stat = 0;
	jpegBuffer = jpegBuffer_lcd = jpegBuffer_sensor = NULL;
	//--------------frame buf check
	if(yuv_add_s.frame_stat == FRAME_PREPARE_NULL){
		deg_Printf("frame buffer have not been malloced\n");
		return -1;
	}
	//--------init yuv_add_s ---------
	HAL_CRITICAL_INIT();
	HAL_CRITICAL_ENTER();
	
	
	while(hal_BackRecDecodeStatusCheck() || 
        !ax32xx_csiScalerCheck());
	addr = nv_open((int)idx);
	if(addr<0){
		deg_Printf("open frame from resourse fail\n");
		ret = -1;
		goto CREATE_FAIL;
	}
	jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(idx),64);//shareMemMalloc(JPEG_CFG_CACHE_NUM*JPEG_CFG_CACHE_SIZE);//jpegCache;
	if(jpegBuffer == NULL)
	{
		debg("hal_custom_yuv_init malloc fail\n");
		ret = -1;
		goto CREATE_FAIL;
	}
	else
	{
		len = nv_size((int)idx);
		nv_read(addr,jpegBuffer,len);					
	}
	if(api_backrec_status())
	{
		backrec_stat = 1;
		backrec_adpt_set(0);
	}
	hal_lcdSetPIPEnable(0);
	
	
	hal_mjpegHeaderParse(jpegBuffer);
	hal_mjpegDecodeGetResolution(&src_width,&src_height);
	
	yuv_add_s.frame_stat = FRAME_BUF_MALLOC;
	

		const u32 levels[] = {100, 88, 76, 66, 56, 48};
		u16 scale_crop_w,scale_crop_h;
		if(SysCtrl.crop_level < ARRAY_NUM(levels))
		{
			u32 l = levels[SysCtrl.crop_level];
			scale_crop_w = ((640 * l / 100) + 0x1f) & (~0x1f);
			scale_crop_h = (480 * l / 100) & ~1;
		}
		//yuv_add_s.sensor_frame_size = scale_crop_w*scale_crop_h;
		yuv_add_s.sensor_width = scale_crop_w;
		yuv_add_s.sensor_height = scale_crop_h;
		

		decode_sensor_width  = yuv_add_s.sensor_width;
		decode_sensor_height = yuv_add_s.sensor_height;

	decode_sensor_size = decode_sensor_width*decode_sensor_height;
	yuv_add_s.sensor_frame_size = decode_sensor_size;
/*
	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.sensor_yuv_addr),(u8*)(yuv_add_s.sensor_yuv_addr+decode_sensor_size),yuv_add_s.sensor_width,yuv_add_s.sensor_height)<0)
	{
		ret = -1;
		goto CREATE_FAIL;
	}
		
	if(!jpeg1_decode_check())
	{
		ret = -1;
		goto CREATE_FAIL;
	}
*/
	if(hal_custom_frame_scan()<0)
	{
		ret = -1;
		deg_Printf("---------frame scan fail-------------\n");
		goto CREATE_FAIL;
	}
	//deg_Printf("---------frame scan OK:%d, %d-------------\n",yuv_add_s.lcd_table_index,yuv_add_s.sensor_table_index);
	
	if(jpegBuffer)
		hal_sysMemFree((void *)jpegBuffer);
	if(jpegBuffer_lcd)
		hal_sysMemFree((void *)jpegBuffer_lcd);
	if(jpegBuffer_sensor)
		hal_sysMemFree((void *)jpegBuffer_sensor);
	yuv_add_s.frame_stat = FRAME_ADD;
	(void)backrec_stat;
	HAL_CRITICAL_EXIT();
	
	return ret;
	
CREATE_FAIL:
	if(jpegBuffer)
		hal_sysMemFree((void *)jpegBuffer);
	if(jpegBuffer_lcd)
		hal_sysMemFree((void *)jpegBuffer_lcd);
	if(jpegBuffer_sensor)
		hal_sysMemFree((void *)jpegBuffer_sensor);
	HAL_CRITICAL_EXIT();
	return ret;
}
/*******************************************************************************
* Function Name  : hal_custom_frame_scan
* Description    : hal_custom_frame_scan
* Input          : None
* Output         : None
* Return         : 0:Sucess; -1: Fail
*******************************************************************************/
s32 hal_custom_frame_scan(void)
{
	if(yuv_add_s.frame_stat != FRAME_BUF_MALLOC)
		return -1;
//---------------LCD FRAME SCAN-------------//
	u8* lcd_yuv;
	u32 i = 0,j = 0,k = 0, i_temp = 0;
	YUV_TABLE_T* lcd_yuv_table;
	lcd_yuv_table = (YUV_TABLE_T*)(yuv_add_s.lcd_table_t);
	lcd_yuv = (u8*)(yuv_add_s.lcd_yuv_addr);
	//deg_Printf("lcdyuv:%x ,lcd_yuv_table:%x\n",(u32)lcd_yuv,(u32)lcd_yuv_table);
	for(i=0;i<yuv_add_s.lcd_height;i +=2)
	{
		for(j = 0;j<yuv_add_s.lcd_width; j+=2 )
		{
			i_temp = 0;
			k = i*yuv_add_s.lcd_width;
			if(lcd_yuv[k+j] < Y_THD)
			{
				lcd_yuv[k+j] = 0;
				i_temp++;
			}
			if(lcd_yuv[k+j+1] < Y_THD)
			{
				lcd_yuv[k+j+1] = 0;
				i_temp++;
			}
			if(lcd_yuv[k+j+yuv_add_s.lcd_width] < Y_THD)
			{
				lcd_yuv[k+j+yuv_add_s.lcd_width] = 0;
				i_temp++;
			}
			if(lcd_yuv[k+j+yuv_add_s.lcd_width+1] < Y_THD)
			{
				lcd_yuv[k+j+yuv_add_s.lcd_width+1] = 0;
				i_temp++;
			}
			if(i_temp > 3)
			{
				k = yuv_add_s.lcd_frame_size+yuv_add_s.lcd_width *(i>>1)+j;
				lcd_yuv[k] = 0;
				lcd_yuv[k+1] = 0;
			}
		}
	}
	j = 0;
	for(i=0;i<yuv_add_s.lcd_frame_size;i++)
	{
		if(i==0)
		{
			if(lcd_yuv[i] != 0){
				lcd_yuv_table[j].start = i;
			
			}
		}
		else if(lcd_yuv[i]!=0)
		{
			if(lcd_yuv[i-1] ==0){
				lcd_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.lcd_frame_size-1)
			{
				lcd_yuv_table[j++].end = i;
			}
			
		}
		else{ //no need to add area
				if(lcd_yuv[i-1] != 0){
				lcd_yuv_table[j++].end = i-1;
			}
		}
	}
	for(i=yuv_add_s.lcd_frame_size;i<yuv_add_s.lcd_frame_size*3/2;i++)
	{
		if(i==yuv_add_s.lcd_frame_size)
		{
			if(lcd_yuv[i] != 0){
				lcd_yuv_table[j].start = i;
			
			}
		}
		else if(lcd_yuv[i]!=0)
		{
			if(lcd_yuv[i-1] ==0){
				lcd_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.lcd_frame_size*3/2-1)
			{
				lcd_yuv_table[j++].end = i;
			}
			
		}
		else{//no need to add area
				if(lcd_yuv[i-1] != 0){
				lcd_yuv_table[j++].end = i-1;
			}
		}
	}
	if(j > yuv_add_s.lcd_table_size)
	{
		deg_Printf("j=%d,yuv_add_s.lcd_table_size = %d\n ",j,yuv_add_s.lcd_table_size);
		deg_Printf("malloc lcd_table no enough, the yuv frame may be too complex\n");
		return -1;
	}
	yuv_add_s.lcd_table_index = j;  //save lcd yuv table index
	
//---------------SENSOR FRAME SCAN-------------//
/*
	u8* sensor_yuv;
	sensor_yuv = (u8*)(yuv_add_s.sensor_yuv_addr);
	YUV_TABLE_T* mjpeg_yuv_table;
	mjpeg_yuv_table = (YUV_TABLE_T*)(yuv_add_s.sensor_table_t);
	//debgbuf(sensor_yuv,yuv_add_s.sensor_frame_size);
	for(i=0;i<yuv_add_s.sensor_height;i +=2)
	{
	
		for(j = 0;j<yuv_add_s.sensor_width; j+=2 )
		{
			i_temp = 0;
			k = i*yuv_add_s.sensor_width;
			if(sensor_yuv[k+j] < Y_THD)
			{
				sensor_yuv[k+j] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+1] < Y_THD)
			{
				sensor_yuv[k+j+1] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+yuv_add_s.sensor_width] < Y_THD)
			{
				sensor_yuv[k+j+yuv_add_s.sensor_width] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+yuv_add_s.sensor_width+1] < Y_THD)
			{
				sensor_yuv[k+j+yuv_add_s.sensor_width+1] = 0;
				i_temp++;
				
			}
			if(i_temp > 3)
			{
				k = yuv_add_s.sensor_frame_size+yuv_add_s.sensor_width *(i>>1)+j;
				sensor_yuv[k] = 0;
				sensor_yuv[k+1] = 0;
			}
			
		}
	}
	j = 0;
	for(i=0;i<yuv_add_s.sensor_frame_size;i++)
	{
		if(i==0)
		{
			if(sensor_yuv[i] != 0){
				mjpeg_yuv_table[j].start = i;
			
			}
		}
		else if(sensor_yuv[i]!=0)
		{
			if(sensor_yuv[i-1] ==0){
				mjpeg_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.sensor_frame_size-1)
			{
				mjpeg_yuv_table[j++].end = i;
			}
			
		}
		else{//no need to add area
				if(sensor_yuv[i-1] != 0){
				mjpeg_yuv_table[j++].end = i-1;
			}
		}
	}
	for(i=yuv_add_s.sensor_frame_size;i<yuv_add_s.sensor_frame_size*3/2;i++)
	{
		if(i==yuv_add_s.sensor_frame_size)
		{
			if(sensor_yuv[i] != 0){
				mjpeg_yuv_table[j].start = i;
			
			}
		}
		else if(sensor_yuv[i]!=0)
		{
			if(sensor_yuv[i-1] ==0){
				mjpeg_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.sensor_frame_size*3/2-1)
			{
				mjpeg_yuv_table[j++].end = i;
			}
			
		}
		else{//no need to add area
				if(sensor_yuv[i-1] != 0){
				mjpeg_yuv_table[j++].end = i-1;
			}
		}
	}
	if(j>yuv_add_s.sensor_table_size)
	{
		deg_Printf("malloc sensor_table no enough, the yuv frame may be too complex\n");
		return -1;
	}
	yuv_add_s.sensor_table_index = j;  //save lcd yuv table index
*/
	//write back all cache line
	SPR_DCAWR = 0xffffffff;
	//yuv_add_s.sensor_yuv_add_ready = 1;
	return 0;
}



INT32S hal_jpeg_decode(INT32U idx)
{
	INT8U *jpegBuffer = NULL;
	INT32S len,addr;
	INT32S ret = 0;

	u32 out_buf = yuv_add_s.sensor_yuv_addr;
	u16 out_width = yuv_add_s.sensor_width;
	u16 out_height = yuv_add_s.sensor_height;

	//--------init yuv_add_s ---------
	HAL_CRITICAL_INIT();
	HAL_CRITICAL_ENTER();
	
	
	while(hal_BackRecDecodeStatusCheck() || 
        !ax32xx_csiScalerCheck());
	addr = nv_open((int)idx);
	if(addr<0){
		deg_Printf("open frame from resourse fail\n");
		ret = -1;
		goto CREATE_FAIL;
	}
	jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(idx),64);
	if(jpegBuffer == NULL)
	{
		debg("hal_custom_yuv_init malloc fail\n");
		ret = -1;
		goto CREATE_FAIL;
	}
	else
	{
		len = nv_size((int)idx);
		nv_read(addr,jpegBuffer,len);					
	}
	if(api_backrec_status())
	{
		backrec_adpt_set(0);
	}
	hal_lcdSetPIPEnable(0);
	
	
	hal_mjpegHeaderParse(jpegBuffer);

	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)out_buf,(u8*)(out_buf+out_width*out_height),out_width,out_height)<0)
	{
		ret = -1;
		goto CREATE_FAIL;
	}
		
	if(!jpeg1_decode_check())
	{
		ret = -1;
		goto CREATE_FAIL;
	}

CREATE_FAIL:
	if(jpegBuffer)
		hal_sysMemFree((void *)jpegBuffer);

	HAL_CRITICAL_EXIT();
	return ret;
}
s32 hal_custom_frame_scan_sensor(void)
{
//---------------SENSOR FRAME SCAN-------------//
	u8* sensor_yuv;
	u32 i = 0,j = 0,k = 0, i_temp = 0;
	sensor_yuv = (u8*)(yuv_add_s.sensor_yuv_addr);
	YUV_TABLE_T* mjpeg_yuv_table;
	mjpeg_yuv_table = (YUV_TABLE_T*)(yuv_add_s.sensor_table_t);
	//debgbuf(sensor_yuv,yuv_add_s.sensor_frame_size);
	for(i=0;i<yuv_add_s.sensor_height;i +=2)
	{
	
		for(j = 0;j<yuv_add_s.sensor_width; j+=2 )
		{
			i_temp = 0;
			k = i*yuv_add_s.sensor_width;
			if(sensor_yuv[k+j] < Y_THD)
			{
				sensor_yuv[k+j] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+1] < Y_THD)
			{
				sensor_yuv[k+j+1] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+yuv_add_s.sensor_width] < Y_THD)
			{
				sensor_yuv[k+j+yuv_add_s.sensor_width] = 0;
				i_temp++;
				
			}
			if(sensor_yuv[k+j+yuv_add_s.sensor_width+1] < Y_THD)
			{
				sensor_yuv[k+j+yuv_add_s.sensor_width+1] = 0;
				i_temp++;
				
			}
			if(i_temp > 3)
			{
				k = yuv_add_s.sensor_frame_size+yuv_add_s.sensor_width *(i>>1)+j;
				sensor_yuv[k] = 0;
				sensor_yuv[k+1] = 0;
			}
			
		}
	}
	j = 0;
	for(i=0;i<yuv_add_s.sensor_frame_size;i++)
	{
		if(i==0)
		{
			if(sensor_yuv[i] != 0){
				mjpeg_yuv_table[j].start = i;
			
			}
		}
		else if(sensor_yuv[i]!=0)
		{
			if(sensor_yuv[i-1] ==0){
				mjpeg_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.sensor_frame_size-1)
			{
				mjpeg_yuv_table[j++].end = i;
			}
			
		}
		else{//no need to add area
				if(sensor_yuv[i-1] != 0){
				mjpeg_yuv_table[j++].end = i-1;
			}
		}
	}
	for(i=yuv_add_s.sensor_frame_size;i<yuv_add_s.sensor_frame_size*3/2;i++)
	{
		if(i==yuv_add_s.sensor_frame_size)
		{
			if(sensor_yuv[i] != 0){
				mjpeg_yuv_table[j].start = i;
			
			}
		}
		else if(sensor_yuv[i]!=0)
		{
			if(sensor_yuv[i-1] ==0){
				mjpeg_yuv_table[j].start = i;
			}
			if(i == yuv_add_s.sensor_frame_size*3/2-1)
			{
				mjpeg_yuv_table[j++].end = i;
			}
			
		}
		else{//no need to add area
				if(sensor_yuv[i-1] != 0){
				mjpeg_yuv_table[j++].end = i-1;
			}
		}
	}
	if(j>yuv_add_s.sensor_table_size)
	{
		deg_Printf("malloc sensor_table no enough, the yuv frame may be too complex\n");
		return -1;
	}
	yuv_add_s.sensor_table_index = j;  //save lcd yuv table index

	//write back all cache line
	SPR_DCAWR = 0xffffffff;
	//yuv_add_s.sensor_yuv_add_ready = 1;
	return 0;
}
/*******************************************************************************
* Function Name  : hal_custom_frame_unit
* Description    : hal_custom_frame_unit, free malloc frame buf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_unit(void)
{
	HAL_CRITICAL_INIT();
	HAL_CRITICAL_ENTER();
	deg_Printf("---------hal_custom_yuv_unit---------------\n");
	deg_Printf(" %x, %x, %x,%x\n",yuv_add_s.lcd_yuv_addr,yuv_add_s.lcd_table_t,yuv_add_s.sensor_yuv_addr,yuv_add_s.sensor_table_t);
	if(yuv_add_s.lcd_yuv_addr)
		hal_sysMemFree((void *)(yuv_add_s.lcd_yuv_addr));
	if(yuv_add_s.lcd_table_t)
		hal_sysMemFree((void *)(yuv_add_s.lcd_table_t));
	if(yuv_add_s.sensor_yuv_addr)
		hal_sysMemFree((void *)(yuv_add_s.sensor_yuv_addr));
	if(yuv_add_s.sensor_table_t)
		hal_sysMemFree((void *)(yuv_add_s.sensor_table_t));
	memset((void*)&yuv_add_s,0, sizeof(YUV_ADD_T));
	HAL_CRITICAL_EXIT();
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_enable
* Description    : hal_custom_frame_add_enable
* Input          : en: 1 enable frame add, 0 disable frame add
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_enable(u32 en)
{
	//HAL_CRITICAL_INIT();
	//HAL_CRITICAL_ENTER();
	//deg_Printf("yuv_add_s.frame_stat:%d,en:%d\n",yuv_add_s.frame_stat,en);
	if(en)
	{	
		
		if(yuv_add_s.frame_stat == FRAME_PREPARE_OK)
			yuv_add_s.frame_stat = FRAME_ADD;
	}
	else
	{
		if(yuv_add_s.frame_stat == FRAME_ADD)
			yuv_add_s.frame_stat = FRAME_PREPARE_OK;
	}
	//HAL_CRITICAL_EXIT();
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_lcd
* Description    : add frame to lcd display
* Input          : dst_y_addr: lcd yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_lcd(u32 dst_y_addr)
{
	
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return;
	}
	int i = 0;
	u8  *dst_y, *src_y;
	YUV_TABLE_T* lcd_yuv_table;
	dst_y = (u8*) dst_y_addr;
	src_y = (u8*) (yuv_add_s.lcd_yuv_addr);
	lcd_yuv_table = (YUV_TABLE_T*)(yuv_add_s.lcd_table_t);
	
	for(i = 0; i<yuv_add_s.lcd_table_index;i++)
	{
		//deg_Printf("i:%d,start:%d,end:%d\n",i,lcd_yuv_table[i].start,lcd_yuv_table[i].end);
		ax32xx_mcpy1_sdram2gram_2((void*)&dst_y[lcd_yuv_table[i].start],(void*)&src_y[lcd_yuv_table[i].start],lcd_yuv_table[i].end-lcd_yuv_table[i].start+1);
	}
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_mjpeg
* Description    : add frame to mjpeg display
* Input          : dst_y_addr: mjpeg yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_mjpeg(u32 dst_y_addr)
{
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return ;
	}
	int i = 0;
	u8  *dst_y, *src_y;
	YUV_TABLE_T* mjpeg_yuv_table;
	dst_y = (u8*) dst_y_addr;
	src_y = (u8*) (yuv_add_s.sensor_yuv_addr);
	mjpeg_yuv_table = (YUV_TABLE_T*)(yuv_add_s.sensor_table_t);
	for(i = 0; i<yuv_add_s.sensor_table_index;i++)
	{
		//deg_Printf("i:%d,start:%d,end:%d\n",i,mjpeg_yuv_table[i].start,mjpeg_yuv_table[i].end);
		ax32xx_mcpy1_sdram2gram_2((void*)&dst_y[mjpeg_yuv_table[i].start],(void*)&src_y[mjpeg_yuv_table[i].start],mjpeg_yuv_table[i].end-mjpeg_yuv_table[i].start+1);
	}
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_umjpeg
* Description    : add frame to back mjpeg display
* Input          : dst_y_addr: back mjpeg yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_umjpeg(u32 dst_y_addr)
{
#if 0
	if(yuv_add_s.frame_stat != FRAME_ADD){
		return ;
	}
	int i = 0;
	u8  *dst_y, *src_y;
	YUV_TABLE_T* umjpeg_yuv_table;
	dst_y = (u8*) dst_y_addr;
	src_y = (u8*) (yuv_add_s.usensor_yuv_addr);
	umjpeg_yuv_table = (YUV_TABLE_T*)(yuv_add_s.usensor_table_t);
	for(i = 0; i<yuv_add_s.usensor_table_index;i++)
	{
		//deg_Printf("i:%d,start:%d,end:%d\n",i,mjpeg_yuv_table[i].start,mjpeg_yuv_table[i].end);
		ax32xx_mcpy1_sdram2gram_2((void*)&dst_y[umjpeg_yuv_table[i].start],(void*)&src_y[umjpeg_yuv_table[i].start],umjpeg_yuv_table[i].end-umjpeg_yuv_table[i].start+1);
		/*for(j = mjpeg_yuv_table[i].start;j<mjpeg_yuv_table[i].end+1;j++)
		{
			//dst_y[j] = src_y[j];
			src_y[j] = dst_y[j];
		}*/
	}
	
	//write back all cache line
	//write back all cache line
	//SPR_DCAWR = 0xffffffff;;
	//ax32xx_sysDcacheWback((u32)dst_y_addr,y_len+uv_len );
	//HAL_CRITICAL_EXIT() ;
#endif
}







void hal_custom_frame_reset_sensor_szie()
{
	hal_csiResolutionGet(&yuv_add_s.sensor_width,&yuv_add_s.sensor_height);
	yuv_add_s.sensor_frame_size = yuv_add_s.sensor_width*yuv_add_s.sensor_height;
	yuv_add_s.sensor_table_size = yuv_add_s.sensor_height*32;	
	yuv_add_s.sensor_table_index = 0;
}

#else

INT32S hal_custom_frame_init(void)  //init buf
{
	INT32S ret = 0;
	//--------init yuv_add_s ---------
	memset((void*)&yuv_add_s,0, sizeof(YUV_ADD_T));

/*
	if(hal_csiResolutionGet(&yuv_add_s.sensor_width,&yuv_add_s.sensor_height)<0)
	{
		deg_Printf("----get csi resolution fail-----\n");
		ret = -1;
		goto INIT_FAIL;
	}
	else
	{
		deg_Printf("----sensor width:%d, height:%d-----<<\n",yuv_add_s.sensor_width,yuv_add_s.sensor_height);
		yuv_add_s.sensor_frame_size = yuv_add_s.sensor_width*yuv_add_s.sensor_height;
		yuv_add_s.sensor_yuv_addr = (u32)hal_sysMemMalloc(yuv_add_s.sensor_frame_size*3/2,64);
		if(yuv_add_s.sensor_yuv_addr == 0)
		{
			deg_Printf("----frame add sensor buf malloc fail-----\n");
			ret = -2;
			goto INIT_FAIL;
		}
		else
		{
			deg_Printf("----frame add sensor buf adr:%x, size:%d\n",yuv_add_s.sensor_yuv_addr,yuv_add_s.sensor_frame_size*3/2);
		}
	}
	
	//if(hal_lcdGetOSDShowingResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height)<0)
	if(hal_lcdGetBufferResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height)<0)
	{
		deg_Printf("----get lcd resolution fail-----\n");
		ret = -1;
		goto INIT_FAIL;
	}
	else
	{
		deg_Printf("----lcd width:%d, height:%d-----\n",yuv_add_s.lcd_width,yuv_add_s.lcd_height);
		yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;
		yuv_add_s.lcd_yuv_addr = (u32)hal_sysMemMalloc(yuv_add_s.lcd_frame_size*3/2,64);
		if(yuv_add_s.lcd_yuv_addr == 0)
		{
			deg_Printf("----frame add  lcd buf malloc fail-----\n");
			ret = -2;
			goto INIT_FAIL;
		}
		else
		{
			deg_Printf("----frame add lcd buf adr:%x, size:%d\n",yuv_add_s.lcd_yuv_addr,yuv_add_s.lcd_frame_size*3/2);
		}
	}
*/

	return ret;

}
/*******************************************************************************
* Function Name  : hal_custom_frame_create
* Description    : hal_custom_frame_create
* Input          : frame id :read from resource
* Output         : None
* Return         : 0:Sucess; -1: Fail
*******************************************************************************/

INT32S hal_custom_frame_create(INT32U idx)
{
	INT8U *jpegBuffer, /**thumbBuffer = NULL,*/*tempBuffer =NULL;
	INT32S len,addr;
	u16 src_width , src_height;
//	u32 decode_lcd_size, decode_sensor_size;
	INT32S ret = 0;
	
	jpegBuffer = NULL;
	//--------init yuv_add_s ---------
	HAL_CRITICAL_INIT();
	HAL_CRITICAL_ENTER();
	deg_Printf("idx=%d\n",idx);
	doublebuf_And_resbuf_Ctrl(0);

	//==malloc lcd buf==
	hal_lcdGetBufferResolution(&yuv_add_s.lcd_width,&yuv_add_s.lcd_height);
	deg_Printf("lcd w=%d,h=%d\n",yuv_add_s.lcd_width,yuv_add_s.lcd_height);

	if(( yuv_add_s.lcd_width == 220 ) && (yuv_add_s.lcd_height == 176 ) )
	{
		yuv_add_s.lcd_width =320;
		yuv_add_s.lcd_height=240;
		yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;

		yuv_add_s.lcd_small_width =220;
		yuv_add_s.lcd_small_height=176;
		yuv_add_s.lcd_small_width=(yuv_add_s.lcd_small_width + 0x1f) & (~0x1f);  // add 32bit alignment
		yuv_add_s.lcd_small_frame_size = yuv_add_s.lcd_small_width*yuv_add_s.lcd_small_height;
	}
	else
	{
		yuv_add_s.lcd_small_width =0;
		yuv_add_s.lcd_small_height=0;

		yuv_add_s.lcd_frame_size = yuv_add_s.lcd_width*yuv_add_s.lcd_height;
		yuv_add_s.lcd_small_frame_size = yuv_add_s.lcd_small_width*yuv_add_s.lcd_small_height;
	}

	
	if(NULL==yuv_add_s.lcd_yuv_buf)
	{
		yuv_add_s.lcd_yuv_buf = hal_sysMemMalloc(yuv_add_s.lcd_frame_size*3/2,64);
		if(NULL==yuv_add_s.lcd_yuv_buf)
		{
			deg_Printf("lcd buf malloc fail\n");
			ret = -1;
			goto CREATE_FAIL;
		}
	}


	if( (NULL==yuv_add_s.lcd_small_yuv_buf) && ( yuv_add_s.lcd_small_frame_size != 0 )) 
	{
		yuv_add_s.lcd_small_yuv_buf = hal_sysMemMalloc(yuv_add_s.lcd_small_frame_size*3/2,64);
		if(NULL==yuv_add_s.lcd_small_yuv_buf)
		{
			deg_Printf("lcd buf malloc fail\n");
			ret = -1;
			goto CREATE_FAIL;
		}
	}
	
	//==end malloc lcd buf==
	addr = nv_open((int)idx);
	if(addr<0)
	{
		deg_Printf("open frame from resourse fail\n");
		ret = -2;
		goto CREATE_FAIL;
	}
	jpegBuffer = (INT8U *)hal_sysMemMalloc(nv_size(idx),64);
	if(jpegBuffer == NULL)
	{
		deg_Printf("hal_custom_frame_create malloc fail\n");
		ret = -3;
		goto CREATE_FAIL;
	}
	else
	{
		len = nv_size((int)idx);
		nv_read(addr,jpegBuffer,len);
	}

	hal_lcdSetPIPEnable(0);

	ret=hal_mjpegHeaderParse(jpegBuffer,NULL);
	if(ret<0)
	{
		deg_Printf("jpeg header parse fail.%d\n",ret);
		goto CREATE_FAIL;
	}

	hal_mjpegDecodeGetResolution(&src_width,&src_height);
	deg_Printf("src_width:%d,src_height:%d,len=%d\n",src_width,src_height,len);


	if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.lcd_yuv_buf),(u8*)(yuv_add_s.lcd_yuv_buf+yuv_add_s.lcd_width*yuv_add_s.lcd_height),yuv_add_s.lcd_width,yuv_add_s.lcd_height)<0)
	{
		ret = -5;
		goto CREATE_FAIL;
	}

	if(!jpeg1_decode_check())
	{
		ret = -6;
		goto CREATE_FAIL;
	}

	if( yuv_add_s.lcd_small_yuv_buf != NULL )
	{
		if(hal_mjpegDecodeNoIsr(jpegBuffer,(u8*)(yuv_add_s.lcd_small_yuv_buf),(u8*)(yuv_add_s.lcd_small_yuv_buf+yuv_add_s.lcd_small_width*yuv_add_s.lcd_small_height),yuv_add_s.lcd_small_width,yuv_add_s.lcd_small_height)<0)
		{
			ret = -5;
			goto CREATE_FAIL;
		}

		if(!jpeg1_decode_check())
		{
			ret = -6;
			goto CREATE_FAIL;
		}
	}
		

	if(jpegBuffer){
			hal_sysMemFree((void *)jpegBuffer);
	}

	yuv_add_s.frame_stat = FRAME_ADD;
	HAL_CRITICAL_EXIT();
	doublebuf_And_resbuf_Ctrl(1);
	return ret;
	
CREATE_FAIL:
	if(jpegBuffer)
	{
		//hal_sysMemFree(jpegBuffer);
		if(tempBuffer){
			hal_sysMemFree((void *)tempBuffer);
		}
		else
		{
			hal_sysMemFree((void *)jpegBuffer);
		}
	}

	if(yuv_add_s.lcd_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.lcd_yuv_buf);
		yuv_add_s.lcd_yuv_buf=NULL;
	}

	if(yuv_add_s.lcd_small_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.lcd_small_yuv_buf);
		yuv_add_s.lcd_small_yuv_buf=NULL;
	}

	HAL_CRITICAL_EXIT();
	doublebuf_And_resbuf_Ctrl(1);
	return ret;
}

/*******************************************************************************
* Function Name  : hal_custom_frame_unit
* Description    : hal_custom_frame_unit, free malloc frame buf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_unit(void)
{
	if(yuv_add_s.lcd_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.lcd_yuv_buf);
		yuv_add_s.lcd_yuv_buf=NULL;
	}

	if(yuv_add_s.lcd_small_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.lcd_small_yuv_buf);
		yuv_add_s.lcd_small_yuv_buf=NULL;
	}

	if(yuv_add_s.sensor_yuv_buf)
	{
		hal_sysMemFree(yuv_add_s.sensor_yuv_buf);
		yuv_add_s.sensor_yuv_buf=NULL;
	}
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_enable
* Description    : hal_custom_frame_add_enable
* Input          : en: 1 enable frame add, 0 disable frame add
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_enable(u32 en)
{
	//HAL_CRITICAL_INIT();
	//HAL_CRITICAL_ENTER();
	//deg_Printf("yuv_add_s.frame_stat:%d,en:%d\n",yuv_add_s.frame_stat,en);
	if(en)
	{	
		if(yuv_add_s.frame_stat == FRAME_PREPARE_OK/*FRAME_NULL*/)
		{
			yuv_add_s.frame_stat = FRAME_ADD;
		}
	}
	else
	{
		if(yuv_add_s.frame_stat == FRAME_ADD)
		{
			yuv_add_s.frame_stat = FRAME_PREPARE_OK/*FRAME_NULL*/;
		}
	}
	//HAL_CRITICAL_EXIT();
}

 
static void frame_draw_buf(u8 *dst_ybuf,u8 *src_ybuf,u16 src_w,u16 src_h,u8 alpha_y)
{
	u16 i,j;
	u8 *dy,*duv,*sy,*suv;

	src_w=(src_w + 0x1f) & (~0x1f);  // add 32bit alignment

	dy=dst_ybuf;
	duv=dst_ybuf+src_w*src_h;
	sy=src_ybuf;
	suv=src_ybuf+src_w*src_h;

	//==draw ==
	for(j=0;j<src_h;j+=2)
	{
		for(i=0;i<src_w;i++)
		{
			if((*(sy+i)>alpha_y))  // y
			{
				*(dy+i)=*(sy+i);
				*(duv+i)=*(suv+i);
			}
		}

		dy+=src_w;
		sy+=src_w;

		for(i=0;i<src_w;i++)
		{
			if((*(sy+i)>alpha_y))  //uv
			{
				*(dy+i)=*(sy+i);
				*(duv+i)=*(suv+i);
			}
		}
		
		dy+=src_w;
		sy+=src_w;
		duv+=src_w;
		suv+=src_w;
	}
	
}

static void  frame_draw_sensor_buf(u8 *dst_ybuf,u16 *dst_uvbuf,u16 dst_w,u16 dst_h,u8 *src_ybuf,u16 src_w,u16 src_h)
{
	u32 i, j, x, y, offset_pixel, offset_pixel_uv, offset_data, offset_data_uv;
	u16 *src_uvbuf = (u16 *)(src_ybuf + src_w*src_h);
	u32 interval_w = 1000000*src_w/dst_w + 1, interval_h = 1000000*src_h/dst_h + 1, interval_i, interval_j;
	
	for(j = 0, y = 0, interval_j = 0; j < dst_h; j++, interval_j += interval_h)
	{
		if(interval_j >= 1000000)
		{
			interval_j -= 1000000;
			y++;
		}
		offset_data = y * src_w;
		offset_pixel = j * dst_w;
		offset_data_uv = (y >> 1) * (src_w >> 1);
		offset_pixel_uv = (j >> 1) * (dst_w >> 1);
		
		for(i = 0, x = 0, interval_i = 0; i < dst_w; i++, interval_i += interval_w)
		{
			if(interval_i >= 1000000){
				interval_i -= 1000000;
				x++;
			}
			if(*(src_ybuf + offset_data + x) < Y_THD)
				continue;
			*(dst_ybuf + offset_pixel + i) = *(src_ybuf + offset_data + x);
			*(dst_uvbuf + offset_pixel_uv + (i >> 1)) = *(src_uvbuf + offset_data_uv + (x >> 1));
		}
	}
	
	ax32xx_sysDcacheWback((u32)dst_ybuf,dst_w*dst_h);  
	ax32xx_sysDcacheWback((u32)dst_uvbuf, dst_w*dst_h/2);
}
static void frame_draw_sensor_buf2(u8 *dst_ybuf,u8 *src_ybuf,u16 src_w,u16 src_h,u8 alpha_y)
{
  //640*480
	u16 i,j;
	u8 *dy,*duv,*sy,*suv;
//	u8 *s1,*s2;
	u16 src_width , src_height;

	hal_csiResolutionGet(&src_width,&src_height);
	src_w=(src_w + 0x1f) & (~0x1f);  // add 32bit alignment
	
	dy=dst_ybuf;
	duv=dst_ybuf+src_width*src_height;
	sy=src_ybuf;
	suv=src_ybuf+src_w*src_h;

	//==draw ==  y
	for(j=0;j<src_h;j+=2)
	{
		for(i=0;i<src_w;i++)
		{
			if((*(sy+i)>alpha_y))  
			{
				*(dy+i*2)=*(sy+i);
				*(dy+i*2+1)=*(sy+i);
				*(dy+i*2+src_width)=*(sy+i);
				*(dy+i*2+src_width+1)=*(sy+i);
			}
		}

		dy+=src_width*2;
		sy+=src_w;
		for(i=0;i<src_w;i++)
		{
			if((*(sy+i)>alpha_y))  
			{
				*(dy+i*2)=*(sy+i);
				*(dy+i*2+1)=*(sy+i);
				*(dy+i*2+src_width)=*(sy+i);
				*(dy+i*2+src_width+1)=*(sy+i);
			}
		}
		
		dy+=src_width*2;
		sy+=src_w;
	}

	sy=src_ybuf;
	//==draw ==  uv
	for(j=0;j<src_h;j+=2)
	{
		for(i=0;i<src_w/2;i++)
		{
			if((*(sy+i*2)>alpha_y) || (*(sy+i*2+1)>alpha_y))  // y
			{
				*(duv+i*4)=*(suv+i*2);
				*(duv+i*4+1)=*(suv+i*2+1);
				*(duv+i*4+2)=*(suv+i*2);
				*(duv+i*4+3)=*(suv+i*2+1);
			}
		}

		sy+=src_w;
		duv+=src_width;
		for(i=0;i<src_w/2;i++)
		{
			if((*(sy+i*2)>alpha_y) || (*(sy+i*2+1)>alpha_y))  // y
			{
				*(duv+i*4)=*(suv+i*2);
				*(duv+i*4+1)=*(suv+i*2+1);
				*(duv+i*4+2)=*(suv+i*2);
				*(duv+i*4+3)=*(suv+i*2+1);
			}
		}
		sy+=src_w;
		duv+=src_width;
		suv+=src_w;
	}

	ax32xx_sysDcacheWback((u32)dst_ybuf,src_width*src_height);  //Y
	ax32xx_sysDcacheWback((u32)(dst_ybuf+src_width*src_height),src_width*src_height/2);//UV
}


/*******************************************************************************
* Function Name  : hal_custom_frame_add_lcd
* Description    : add frame to lcd display
* Input          : dst_y_addr: lcd yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_lcd(u8 *dst_ybuf)
{
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return;
	}

	if( yuv_add_s.lcd_small_yuv_buf )
	{
		frame_draw_buf(dst_ybuf,yuv_add_s.lcd_small_yuv_buf,yuv_add_s.lcd_small_width,yuv_add_s.lcd_small_height,Y_THD);
		ax32xx_sysDcacheFlush((u32)dst_ybuf,yuv_add_s.lcd_small_frame_size*3/2);
	}
	else
	{
		//deg_Printf("src_width..................................\n");
		frame_draw_buf(dst_ybuf,yuv_add_s.lcd_yuv_buf,yuv_add_s.lcd_width,yuv_add_s.lcd_height,Y_THD);
		ax32xx_sysDcacheFlush((u32)dst_ybuf,yuv_add_s.lcd_frame_size*3/2);
	}
}
/*******************************************************************************
* Function Name  : hal_custom_frame_add_mjpeg
* Description    : add frame to mjpeg display
* Input          : dst_y_addr: mjpeg yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_mjpeg(u8 *dst_ybuf)
{
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return ;
	}
	u16 csi_w,csi_h;//,crop_w,crop_h;

	hal_csiResolutionGet(&csi_w,&csi_h);
	#if PIC_16_9
	u16 half_w720P,half_h720P;
	hal_csiHalf720P_ResolutionGet(&half_w720P,&half_h720P);
	frame_draw_sensor_buf(dst_ybuf,(u16 *)(dst_ybuf+csi_w*half_h720P),csi_w,half_h720P,yuv_add_s.lcd_yuv_buf,yuv_add_s.lcd_width,yuv_add_s.lcd_height);
	#else
	frame_draw_sensor_buf(dst_ybuf,(u16 *)(dst_ybuf+csi_w*csi_h),csi_w,csi_h,yuv_add_s.lcd_yuv_buf,yuv_add_s.lcd_width,yuv_add_s.lcd_height);
	#endif
}

void hal_custom_frame_add_Lmjpeg(u8 *dst_ybuf,u8 pic_16_9)
{
	if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return ;
	}
	u16 csi_w,csi_h,encode_h;
	
	hal_csiResolutionGet(&csi_w,&csi_h);
	
	if(pic_16_9){
		u16 half_w720P,half_h720P;
		hal_csiHalf720P_ResolutionGet(&half_w720P,&half_h720P);
		encode_h = half_h720P;
	}else
		encode_h = csi_h;
	frame_draw_sensor_buf(dst_ybuf,(u16 *)(dst_ybuf+csi_w*encode_h),csi_w,encode_h,yuv_add_s.lcd_yuv_buf,yuv_add_s.lcd_width,yuv_add_s.lcd_height);
}


void hal_custom_frame_add_mjpeg2(u8 *dst_ybuf)
{
	/*if(yuv_add_s.frame_stat != FRAME_ADD)
	{
		return ;
	}*/
	
	frame_draw_sensor_buf2(dst_ybuf,yuv_add_s.lcd_yuv_buf,yuv_add_s.lcd_width,yuv_add_s.lcd_height,Y_THD);
}

#endif


void hal_custom_frame_reset_sensor_szie()
{
	hal_csiResolutionGet(&yuv_add_s.sensor_width,&yuv_add_s.sensor_height);
	yuv_add_s.sensor_frame_size = yuv_add_s.sensor_width*yuv_add_s.sensor_height;
   #if(1 == mjpeg_lcd_cut)//YTS-4:3
	     yuv_add_s.sensor_width = 960;	// 960/720 = 4/3
	     yuv_add_s.sensor_height = 720;  
	 #else
	 #endif
	yuv_add_s.sensor_table_size = yuv_add_s.sensor_height*32;	
	yuv_add_s.sensor_table_index = 0;
}


/*******************************************************************************
* Function Name  : decodePic
* Description    : dicode pic to dst_w & dst_h ,and out put ybuf
* Input          : jpgBuff: jpg src buf 
				    p_out_y:  	output y buf 
				    p_out_uv:  	output uv buf 
				    dst_width: 	dst_w
				    dst_width: 	dst_h
* Output         : None
* Return         : None
*******************************************************************************/
bool decodePic(u8 * jpgBuff,u8 * p_out_y,u8 * p_out_uv,u16 dst_width,u16 dst_height,u32 jpg_size)//copyPic* copyPicInfor,u8* jpgBuff
{
#define JPG_ENCODE_BUFF_SIZE    200*1024
		u16 jpgW=0 , jpgH=0;
		u8* tempDecodeBuff=NULL;
		u8* tempJpgEncodeBuff=NULL;
		u16 /*decodeW,decodeH,*/stride;
		u32 point = 0;

		INT32S ret_jpg;

		if(!jpgBuff)
			deg_Printf("buf is empty!!!!!\n");

		hal_mjpegDecodeGetResolution(&jpgW,&jpgH);

	

#if 1

			ret_jpg = 0;
			ret_jpg = hal_mjpegHeaderParse(jpgBuff,NULL);
			if(ret_jpg == -10)
			{
				INT8U app[12];
				
				point = jpg_size - sizeof app;
				memcpy(app,jpgBuff + point,sizeof app);
				
				if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
				{
					u32 thumbOffset;
					thumbOffset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
		
					jpgBuff = jpgBuff+thumbOffset;
					hal_mjpegHeaderParse(jpgBuff,NULL);
				}
					
			}
			if(ret_jpg < 0)
			{
				ret_jpg = -1;
				deg_Printf("headparse fail\n");
			}
#endif

		
		hal_mjpegHeaderParse(jpgBuff,NULL);
		
		deg_Printf("jpgW:%d,jpgH:%d, w:%d,h:%d\n",jpgW,jpgH,dst_width,dst_height);



		
		if(dst_width<=jpgW&&dst_height<=jpgH)
		{
			if(hal_mjpegDecodeNoIsr(jpgBuff,p_out_y,p_out_uv,dst_width,dst_height)<0)
			{
				deg_Printf("hal_mjpegDecodeNoIsr error\n");
				return false;
			}
			if(!jpeg1_decode_check())
			{
				deg_Printf("jpeg1_decode_check error\n");
				return false;
			}
			return true;
		}


		stride=(jpgW+0x1f)&(~0x1f);
		#if SDRAM_SIZE == SDRAM_SIZE_2M	//if use 8M RAM,don't need to reuse buf
		tempDecodeBuff=p_out_y;
		#else
		tempDecodeBuff=(INT8U *)hal_sysMemMalloc(stride*jpgH*3/2,64);
		#endif
		if(tempDecodeBuff==NULL)
		{
			goto FREE_MEM;
		}
		else
			deg_Printf("tempDecodeBuff malloc success\n");

		
			
		//tempJpgEncodeBuff=(INT8U *)hal_sysMemMalloc(JPG_ENCODE_BUFF_SIZE-20*1024,64);
		if(tempJpgEncodeBuff==NULL)
		{
			u32 temp_size;
			temp_size=hal_sysMemRemain()-23*1024;
			tempJpgEncodeBuff=(INT8U *)hal_sysMemMalloc(temp_size,64);
		}
		
		if(tempJpgEncodeBuff==NULL)
			goto FREE_MEM;
		else
			deg_Printf("tempJpgEncodeBuff malloc success\n");
		
		if(hal_mjpegDecodeNoIsr(jpgBuff,tempDecodeBuff,tempDecodeBuff+jpgW*jpgH,jpgW,jpgH)<0)
		{
			deg_Printf("1 hal_mjpegDecodeNoIsr error\n");
			goto FREE_MEM;
		}else
			deg_Printf("1 hal_mjpegDecodeNoIsr success?\n");
		if(!jpeg1_decode_check())
		{
			deg_Printf("1 jpeg1_decode_check error\n");
			goto FREE_MEM;
		}else
			deg_Printf("1 jpeg1_decode_check success?\n");
			
		ax32xx_sysDcacheWback((u32)tempDecodeBuff,jpgH*jpgW*3/2);
#if 0	
		char *name;
		int fHandle;
		fHandle = task_image_createfile(VIDEO_CH_A,&name);
	
		write(fHandle,(void *)tempDecodeBuff,jpgH*jpgW*3/2);
		deg_Printf("111dicodePic save :%x Size:%x name:%d\n",tempDecodeBuff,jpgH*jpgW*3/2,fHandle);
		deamon_fsSizeModify(-1,fs_size(fHandle));
		close(fHandle);
#endif



			
		deg_Printf("start jpg encode\n");
		ax32xx_mjpB_Ctl_init(0, JPEG_Q_75, jpgW, jpgH, dst_width, dst_height);
		deg_Printf("after mjpB_Ctl_init\n");
		ax32xx_mjpB_Linebuf_cfg(tempDecodeBuff,tempDecodeBuff+stride*jpgH);
		deg_Printf("after mjpB_Linebuf_cfg\n");
		ax32xx_mjpB_dma_cfg((u32)tempJpgEncodeBuff,(u32)(tempJpgEncodeBuff+JPG_ENCODE_BUFF_SIZE));
		deg_Printf("after mjpB_dma_cfg\n");
		hal_watermarkEnable(0,0);
		deg_Printf("after watermark\n");
		ax32xx_mjpB_on_2();
		deg_Printf("after _mjpB_on_2\n");
		if(ax32xx_mjpegB_encode_check()<0)
		{
			deg_Printf("ax32xx_mjpegB_encode_check error\n");
			goto FREE_MEM;
		}else
			deg_Printf("ax32xx_mjpegB_encode_check success?\n");
		memset(p_out_y,0,dst_width*dst_height*3/2);
		if(hal_mjpegDecodeNoIsr(tempJpgEncodeBuff,p_out_y,p_out_uv,dst_width,dst_height)<0)
		{
			deg_Printf("2 hal_mjpegDecodeNoIsr error\n");
			goto FREE_MEM;
		}else
			deg_Printf("2 hal_mjpegDecodeNoIsr success?\n");
		ax32xx_sysDcacheWback((u32)p_out_y,dst_width*dst_height*3/2);
		if(!jpeg1_decode_check())
		{
			deg_Printf("2 jpeg1_decode_check error\n");
			goto FREE_MEM;
		}else
			deg_Printf("2 jpeg1_decode_check success?\n");
		
		deg_Printf("jpg decode ok\n");

#if 0	
			char *name2;
			int fHandle2;
			fHandle2 = task_image_createfile(VIDEO_CH_A,&name2);
		
			write(fHandle2,(void *)p_out_y,dst_width*dst_height*3/2);
			deg_Printf("222dicodePic save :%x Size:%x name:%d\n",p_out_y,dst_width*dst_height*3/2,fHandle2);
			deamon_fsSizeModify(-1,fs_size(fHandle2));
			close(fHandle2);
#endif

		if(tempJpgEncodeBuff)
			hal_sysMemFree((void *)tempJpgEncodeBuff);
		/*if(tempDecodeBuff)
			hal_sysMemFree((void *)tempDecodeBuff);*/
		return true;
	FREE_MEM:
		deg_Printf("jpg decode error\n");
		if(tempJpgEncodeBuff)
			hal_sysMemFree((void *)tempJpgEncodeBuff);
		if(tempDecodeBuff)
			hal_sysMemFree((void *)tempDecodeBuff);
		return false;
}

