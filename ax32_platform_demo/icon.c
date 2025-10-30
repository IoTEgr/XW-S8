/****************************************************************************
**
 **                              RESOURCE 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  RESOURCE FONT
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : font.c
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


#define  ICON_DATA_ADDR             RES_OSD_SOURCE
#define  ICON_PALETTE_ADDR        RES_PALETTE


typedef struct Icon_Info_S
{
	INT32U width;
	INT32U height;
	INT32U offset;
}Icon_Info_T;
typedef struct Icon_Ctrl_S
{
	INT32U Icon;
	INT32U iconAddr;

	Icon_Info_T iconInfo;
}Icon_Ctrl_T;


static Icon_Ctrl_T iconCtrl;

/*******************************************************************************
* Function Name  : iconInit
* Description    :  icon initial
* Input          : 
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int iconInit(void)
{
	int ret;

	ret = nv_open(ICON_DATA_ADDR);
	if(ret<0)
	{
		deg_Printf("icon : find data fail\n");
		return -1;
	}

    iconCtrl.iconAddr = ret;
	iconCtrl.Icon = 0xffffffff;
    deg_Printf("icon : initial ok.%x\n",iconCtrl.iconAddr);
	return 0;
}
/*******************************************************************************
* Function Name  : iconGetSize
* Description    :  get icon size
* Input          : int num : icon index
                      INT16U *width : width
                      INT16U *height: height
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int iconGetSize(int num,INT16U *width,INT16U *height)
{
    if(iconCtrl.Icon != num)
    {
		nv_read(iconCtrl.iconAddr+sizeof(Icon_Info_T)*num,&iconCtrl.iconInfo,sizeof(Icon_Info_T));
		iconCtrl.Icon = num;
    }

	if(width)
		*width = iconCtrl.iconInfo.width;
	if(height)
		*height = iconCtrl.iconInfo.height;

	return 0;
}
/*******************************************************************************
* Function Name  : iconGetData
* Description    :  get icon data
* Input          : int num : icon index
                      INT8U *buffer : buffer 
                      int len : length
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int iconGetData(int num,INT8U *buffer,int len)
{
	if(iconCtrl.Icon != num)
	{
		nv_read(iconCtrl.iconAddr+sizeof(Icon_Info_T)*num,&iconCtrl.iconInfo,sizeof(Icon_Info_T));
		iconCtrl.Icon = num;
	}

	nv_read(iconCtrl.iconAddr+iconCtrl.iconInfo.offset,buffer,len);

	return 0;
}
/*******************************************************************************
* Function Name  : iconGetAddr
* Description    :  get icon addr
* Input          : int num : icon index                      
* Output         : none                                            
* Return         : int : address
*******************************************************************************/
int iconGetAddr(int num)
{
	if(iconCtrl.Icon != num)
	{
		nv_read(iconCtrl.iconAddr+sizeof(Icon_Info_T)*num,&iconCtrl.iconInfo,sizeof(Icon_Info_T));
		iconCtrl.Icon = num;
	}

	return (iconCtrl.iconAddr+iconCtrl.iconInfo.offset);
}
/*******************************************************************************
* Function Name  : iconGetPalette
* Description    :  get icon palette
* Input          : 
                      INT8U *buffer : buffer 
* Output         : none                                            
* Return         : int : palette size
*******************************************************************************/
int iconGetPalette(INT8U *buffer)
{
	int ret,size;

	ret = nv_open(ICON_PALETTE_ADDR);
	if(ret<0)
	{
		deg_Printf("icon : find palette fail\n");
		return -1;
	}
	size = nv_size(ICON_PALETTE_ADDR);

	nv_read(ret,buffer,size);

//	uart_PrintfBuf((void *)buffer,1024);

	return size;
}














