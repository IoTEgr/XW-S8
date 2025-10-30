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
* File Name   : image_encode.h
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


#define SM_PIC_ST_NUM 13  //small pic timestemp num
#if 0
 u8* small_pic_id_buf[20];

 u32 small_pic_id[20];
#endif


/*******************************************************************************
* Function Name  : watermark_bmp2yuv_init
* Description    : draw watermark init 
* Input          : 

* Output         : 
* Return         : None
*******************************************************************************/
void watermark_bmp2yuv_init(u16 pixel_w,u16 pixel_h,u8 pic_num);

/*******************************************************************************
* Function Name  : watermark_draw
* Description    : draw watermark on photo 
* Input          : u8* ybuf_src: src
				   u16 pos_x: X position
				   u16 pos_y: Y position
                   u16 gap  :  character gap
* Output         : 
* Return         : None
*******************************************************************************/
void watermark_bmp2yuv_draw(u8* ybuf_src,u16 pos_x, u16 pos_y,u16 gap);
/*******************************************************************************
* Function Name  : hal_watermark_buf_free
* Description    : hal_watermark_buf_free
* Input          : None
                     
* Output         : None
* Return         : None
*******************************************************************************/
void watermark_buf_bmp2yuv_free(void);



