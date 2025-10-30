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
#ifndef  ICON_H
    #define   ICON_H

/*******************************************************************************
* Function Name  : iconInit
* Description	 :	icon initial
* Input 		 : 
* Output		 : none 										   
* Return		 : int : 
*******************************************************************************/
int iconInit(void);
/*******************************************************************************
* Function Name  : iconGetSize
* Description	 :	get icon size
* Input 		 : int num : icon index
					  INT16U *width : width
					  INT16U *height: height
* Output		 : none 										   
* Return		 : int : 
*******************************************************************************/
int iconGetSize(int num,INT16U *width,INT16U *height);
/*******************************************************************************
* Function Name  : iconGetData
* Description    :  get icon data
* Input          : int num : icon index
                      INT8U *buffer : buffer 
                      int len : length
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int iconGetData(int num,INT8U *buffer,int len);
/*******************************************************************************
* Function Name  : iconGetAddr
* Description    :  get icon addr
* Input          : int num : icon index                      
* Output         : none                                            
* Return         : int : address
*******************************************************************************/
int iconGetAddr(int num);
/*******************************************************************************
* Function Name  : iconGetPalette
* Description    :  get icon palette
* Input          : 
                      INT8U *buffer : buffer 
* Output         : none                                            
* Return         : int : palette size
*******************************************************************************/
int iconGetPalette(INT8U *buffer);































#endif


