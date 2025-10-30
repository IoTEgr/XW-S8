/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA STREAM WATERMARK
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : stream.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is watermark stream
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"



extern const unsigned char *ascii_draw_get(char c,unsigned char *width,unsigned char *heigth,unsigned char font);


INT8U *streamSprintf(char *str,INT8U *buffer,INT16U width,INT8U font)
{
    int i,j,n;
	INT8U *src,*tar,w,h,tw;

    if((str==NULL) || (buffer == NULL))
		return NULL;
	n = 0;
	tw = (width+7)/8;
	tar = buffer;
    while(str[n])
    {
		src = (INT8U *)ascii_draw_get((char)str[n],&w,&h,font);
		if(src != NULL)
		{
			//src = (INT8U *)ascii_draw_get((char)' ',&w,&h,font);
			for(i=0;i<h;i++)
			{
				for(j=0;j<(w+7)/8;j++)
				    tar[i*tw+j] = *src++;
			}
			
		}

		tar+=(w+7)/8;
		n++;
		if(tar>=&buffer[width>>3])
			break;
    }
//	deg_Printf("streamsprintf %d,%d\n",n,font);
	return buffer;
}


INT32U streamFont(INT16U width,INT16U height)
{
	INT8U font,h;
	
	/*if(width<=640)
		font = R_FONT_NUM2;
	else if(width<=1280)
		font =  R_FONT_NUM2;
	else if(width<=1920)
		font =  R_FONT_NUM2;
	else
		font =  R_FONT_NUM2;
	*/
	//--new
	if(width<=640)
		font = R_FONT_NUM2;
	else if(width<=1920)
		//font =  R_FONT_NUM2;
	//else if(width<=2560)
		font =  R_FONT_NUM3;
	//else if(width<=3200)
	//	font =  R_FONT_NUM4;
	else
		font =  R_FONT_NUM4;//R_FONT_NUM5;
    ascii_draw_get((char)'0',NULL,&h,font);

	return ((h<<8)|font);
	
}
















