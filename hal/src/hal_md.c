/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         MD HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_md.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"

//=====sensor 480P  check all frame=======
/*
#define  HAL_CFG_MD_SX        0
#define  HAL_CFG_MD_EX        640
#define  HAL_CFG_MD_SY        0
#define  HAL_CFG_MD_EY        480
*/
//=====sensor 720P  check frame center =======
#define  HAL_CFG_MD_SX        320
#define  HAL_CFG_MD_EX        (320+640)
#define  HAL_CFG_MD_SY        180
#define  HAL_CFG_MD_EY        (180+360)

#define  HAL_CFG_MD_THRESHOLD    1  // THRESHOLD LEVEL   0: easy happen, 1: normal  happen, 2 :not easy happen

#if(0 == HAL_CFG_MD_THRESHOLD)
#define  HAL_CFG_MD_PIXELNUM     5	// value more min ,more easy to happen
#define  HAL_CFG_MD_BLOCKNUM     5	// value more min ,more easy to happen
#define  HAL_CFG_MD_FRAMENUM     1	// value more min ,more easy to happen
#elif(1 == HAL_CFG_MD_THRESHOLD)
#define  HAL_CFG_MD_PIXELNUM     20	// value more min ,more easy to happen
#define  HAL_CFG_MD_BLOCKNUM     20	// value more min ,more easy to happen
#define  HAL_CFG_MD_FRAMENUM     1	// value more min ,more easy to happen
#else
#define  HAL_CFG_MD_PIXELNUM     40	// value more min ,more easy to happen
#define  HAL_CFG_MD_BLOCKNUM     40	// value more min ,more easy to happen
#define  HAL_CFG_MD_FRAMENUM     1	// value more min ,more easy to happen
#endif


static u8 halMDFlag=0;
/*******************************************************************************
* Function Name  : hal_mdISR
* Description    : hal layer .motion dectetion isr
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
static void hal_mdCallback(void)
{
	halMDFlag |= 0x10;

}
/*******************************************************************************
* Function Name  : hal_mdInit
* Description    : hal layer .motion dectetion initial
* Input          : none
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_mdInit(void)
{
	ax32xx_mdXPos(HAL_CFG_MD_SX,HAL_CFG_MD_EX);
	
	ax32xx_mdYPos(HAL_CFG_MD_SY,HAL_CFG_MD_EY);

	ax32xx_mdInit(HAL_CFG_MD_PIXELNUM,HAL_CFG_MD_BLOCKNUM,HAL_CFG_MD_FRAMENUM);
	
	ax32xx_csiISRRegiser(CSI_IRQ_MDT_MOTION_DET,hal_mdCallback);

    halMDFlag = 0;
	return 0;
}
/*******************************************************************************
* Function Name  : hal_dacCallback
* Description    : hal layer .motion dectetion enable  set
* Input          : u8 en : 1-enable,0-disable
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_mdEnable(u8 en)
{
//	hal_mdInit();
	ax32xx_mdEnable(en);
    if(en)
    {
       halMDFlag = 0x01;
	  // ax32xx_csiISRRegiser(CSI_IRQ_MDT_MOTION_DET,hal_mdCallback);
    }
	else
	{
	//	ax32xx_csiISRRegiser(CSI_IRQ_MDT_MOTION_DET,NULL);
	}
	return 0;
}


/*******************************************************************************
* Function Name  : hal_mdCheck
* Description    : hal layer .motion dectetion check md event
* Input          : 
* Output         : None
* Return         : s32 : 0 :no md event happend
                             >0: md event happend
*******************************************************************************/
s32 hal_mdCheck(void)
{
	u8 temp;

	temp = halMDFlag;
	halMDFlag &= 0x01;
	
    if(temp&0x01)
	    return (temp&0xf0);
	else
		return 0;
}















