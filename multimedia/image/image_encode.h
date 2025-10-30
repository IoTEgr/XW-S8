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

#ifndef  IMAGE_ENCODE_H
    #define  IMAGE_ENCODE_H






/*******************************************************************************
* Function Name  : imageEncodeInit
* Description	 : image encode initial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0
*******************************************************************************/
int imageEncodeInit(void);
/*******************************************************************************
* Function Name  : imageEncodeUninit
* Description	 : image encode uninitial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int imageEncodeUninit(void);
/*******************************************************************************
* Function Name  : imageEncodeStart
* Description	 : take a photo
* Input 		 : FHANDLE fileHanle : file handle for write photo ram data to the file
                 : INT16U image_width : image width
                 : INT16U image_height:image height
                 : INT8U image_q qaulity,refence to __42__,__50__,__62__,__75__,__81__
                 : INT8U timestramp : time stramp.0->disable,
* Output		 : none 										   
* Return		 : int : 0 success
                                -1 fail
*******************************************************************************/
int imageEncodeStart(FHANDLE fileHanle,INT16U image_width,INT16U image_height,INT8U image_q,INT8U timestramp,INT8U frame_enable);


int imageEncodeStartB(FHANDLE fileHanle,INT16U image_width,INT16U image_height,INT8U image_q,INT8U timestramp,INT8U frame_enable);



int jpeg_exif_head_init(u8 *head_buf,u16 width,u16 height,u8* datatime);















#endif
