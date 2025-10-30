/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         ADC HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_csi.h
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
#ifndef HAL_CUSTOM_YUV_H
#define HAL_CUSTOM_YUV_H


//#define Y_THD 				0x2B
#define Y_THD 				0x1B
#define UV_THD				12


typedef struct YUV_ADD_S
{

	s32 frame_stat;
	
	u16 sensor_width;
	u16 sensor_height;
	u32 sensor_frame_size;
	u8 *sensor_yuv_buf;

	u16 lcd_width;
	u16 lcd_height;
	u32 lcd_frame_size;
	u8 *lcd_yuv_buf;

	u16 lcd_small_width; // 
	u16 lcd_small_height;
	u32 lcd_small_frame_size;
	u8 *lcd_small_yuv_buf;

//----
	u32 sensor_table_t;
	u32 sensor_table_size;
	u32 sensor_table_index;
	
	u16 usensor_width;
	u16 usensor_height;
	u32 usensor_frame_size;
	u32 usensor_yuv_addr;
	u32 usensor_table_t;
	u32 usensor_table_size;
	u32 usensor_table_index;

	
	
}YUV_ADD_T;


extern YUV_ADD_T   yuv_add_s;
/*******************************************************************************
* Function Name  : hal_custom_yuv_init
* Description    : frame add yuv buf malloc
* Input          : None
* Output         : None
* Return         : 0:Sucess; -1: GET RESOLUTION FAIL -2: MALLOC BUF FAIL
*******************************************************************************/
INT32S hal_custom_frame_init(void);
/*******************************************************************************
* Function Name  : hal_custom_frame_create
* Description    : hal_custom_frame_create
* Input          : frame id :read from resource
* Output         : None
* Return         : 0:Sucess; -1: Fail
*******************************************************************************/
INT32S hal_custom_frame_create(INT32U idx); // should disable pip mode

/*******************************************************************************
* Function Name  : hal_custom_frame_unit
* Description    : hal_custom_frame_unit, free malloc frame buf
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_unit(void);
/*******************************************************************************
* Function Name  : hal_custom_frame_add_enable
* Description    : hal_custom_frame_add_enable
* Input          : en: 1 enable frame add, 0 disable frame add
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_enable(u32 en);
/*******************************************************************************
* Function Name  : hal_custom_frame_add_lcd
* Description    : add frame to lcd display
* Input          : dst_y_addr: lcd yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_lcd(u8 *dst_ybuf);
/*******************************************************************************
* Function Name  : hal_custom_frame_add_mjpeg
* Description    : add frame to mjpeg display
* Input          : dst_y_addr: mjpeg yuv addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_custom_frame_add_mjpeg(u8 *dst_ybuf);
void hal_custom_frame_add_Lmjpeg(u8 *dst_ybuf,u8 pic_16_9);

void hal_custom_frame_add_mjpeg2(u8 *dst_ybuf);

void hal_custom_frame_reset_sensor_szie();

bool decodePic(u8 * jpgBuff,u8 * p_out_y,u8 * p_out_uv,u16 dst_width,u16 dst_height,u32 jpg_size);//copyPic* copyPicInfor,u8* jpgBuff
#endif
