/****************************************************************************
**
 **                         BUILDWIN HARDWARE LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                  TIMER HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_timer.c
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




/*******************************************************************************
* Function Name  : hal_timerStart
* Description    : timer start
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
                      u32 frq  : frq .>20hz
                      void (*isr)(void) : callback function
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_timerStart(u8 htimer,u32 frq,void (*isr)(void))
{
	return ax32xx_timerStart(htimer,frq,isr);
}
/*******************************************************************************
* Function Name  : hal_timerStop
* Description    : timer stop
* Input          : u8 htimer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_timerStop(u8 htimer)
{
	return ax32xx_timerStop(htimer);
}
/*******************************************************************************
* Function Name  : hal_timerEnable
* Description    : timer enable /disable
* Input          : u8 htimer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
                      u8 en      : 0:disable,1 :enable
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_timerEnable(u8 htimer,u8 en)
{
	if(en)
		return ax32xx_timerEnable(htimer);
	else
		return ax32xx_timerDisable(htimer);
}
/*******************************************************************************
* Function Name  : hal_timerTickEnable
* Description    : timer tick enable /disable
* Input          :u8 en      : 0:disable,1 :enable
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
s32 hal_timerTickEnable(u8 en)
{
	if(en)
		ax32xx_timerTickStart();
	else
		ax32xx_timerTickStop();
	return 0;
}
/*******************************************************************************
* Function Name  : hal_timerTickGet
* Description    : timer tick count
* Input          : 
* Output         : None
* Return         : s32 : tick count
*******************************************************************************/
s32 hal_timerTickGet(void)
{
	return ax32xx_timerTickCount();
}


/*******************************************************************************
* Function Name  : hal_timerPWMStart
* Description    : timer PWM start
* Input          : timer: timer select
* 				   pwm_map: PWM output group select
* 				   frq:     PWM frequency configure
* 				   percent: PWM duty rate(<100)
* Output         : None
* Return         : count
*******************************************************************************/
// s8 hal_timerPWMStart(u8 timer,u8 pwm_map,u32 frq, u8 percent)
// {
// 	return ax32xx_timerPWMStart(timer,pwm_map,frq,percent);
// }

// s8  hal_timerPWMStop(u8 timer)
// {
// 	return  ax32xx_timerPWMStop(timer);
// }














