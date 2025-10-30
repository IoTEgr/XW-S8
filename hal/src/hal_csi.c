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

SNSER snser; 


/*******************************************************************************
* Function Name  : hal_csiMCLKSet
* Description    : hal layer .csi set m-clk
* Input          : int clk : clk frq
* Output         : None
* Return         : none
*******************************************************************************/
void hal_csiMCLKSet(int clk)
{
	ax32xx_csiMclkSet(clk);
}
/*******************************************************************************
* Function Name  : hal_csiRegister
* Description    : hal layer .csi register
* Input          : sensor_adpt *cmos_sensor : sensor op
* 				   _Sensor_Ident_ *sensor_ident
* Output         : None
* Return         : none
*******************************************************************************/
void hal_csiRegister(_Sensor_Adpt_ *sensor_op,_Sensor_Ident_ *sensor_ident)
{
	snser.p_sensor_cmd = sensor_op;
	snser.p_sensor_ident = sensor_ident;
	snser.online = true;
}
/*******************************************************************************
* Function Name  : hal_csiResolutionGet
* Description    : hal layer .get csi resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0 : success
                      int -1: fail
*******************************************************************************/
int hal_csiResolutionGet(u16 *width,u16 *height)
{
    if(snser.p_sensor_cmd==NULL)
		return -1;
	if(width)
		*width = snser.p_sensor_cmd->pixelw;
	if(height)
		*height=snser.p_sensor_cmd->pixelh;
	return 0;
}
/*******************************************************************************
* Function Name  : hal_csiAdptGet
* Description    : hal layer .get csi adpt
* Input          : 
* Output         : None
* Return         : sensor_adpt *adpt
*******************************************************************************/
_Sensor_Adpt_ *hal_csiAdptGet(void)
{
	return snser.p_sensor_cmd;
}
/*******************************************************************************
* Function Name  : hal_csi_SeninitGet
* Description    : hal layer .get csi Seninit
* Input          : 
* Output         : None
* Return         : _Sensor_Ident_ *ident
*******************************************************************************/
_Sensor_Ident_ *hal_csi_SeninitGet(void)
{
	return snser.p_sensor_ident;
}

/*
get half 720P resolution W & H
*/
void hal_csiHalf720P_ResolutionGet(u16 *width,u16 *height)
{
	if(snser.p_sensor_cmd==NULL)
			return ;

	if(width)
		*width = RESOLUTION_720P_W;
	if(height)
		*height = RESOLUTION_720P_H;
}

