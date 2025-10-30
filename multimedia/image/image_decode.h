/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA IMAGE DECODE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : image_decode.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image decode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  IMAGE_DECODE_H
   #define IMAGE_DECODE_H


typedef struct Image_ARG_S
{
   INT8U  type;   // JPG,BMP
   INT8U  wait;    // 0: do not wait decode end,1:wait decode end
   INT8U  fit;   // fit screen,or fit picture 
   INT8U  rev;	  // frame rate  

//   INT32U size;
   INT8U *yout;
   INT8U *uvout;
   
   Media_T media;

   Media_Res_T target; // mjpeg resolution
}Image_ARG_T;












/*******************************************************************************
* Function Name  : imageDecodeInit
* Description	 : image decode initial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0
*******************************************************************************/
int imageDecodeInit(void);
/*******************************************************************************
* Function Name  : imageDecodeUninit
* Description	 : image decode uninitial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int imageDecodeUninit(void);
/*******************************************************************************
* Function Name  : imageDecodeStart
* Description	 : image decode
* Input 		 : Image_ARG_T *arg : image arg
* Output		 : none 										   
* Return		 : int : 0 success
                                -1 fail
*******************************************************************************/
int imageDecodeStart(Image_ARG_T *arg);
/*******************************************************************************
* Function Name  : imageDecodeGetResolution
* Description	 : image decode get jpeg resolution
* Input 		 : INT16U *width,INT16U *height
* Output		 : none 										   
* Return		 : int : 0 success
                                -1 fail
*******************************************************************************/
int imageDecodeGetResolution(INT16U *width,INT16U *height);


int imagelDecodeGetResolution(INT16U *width,INT16U *height);











#endif
