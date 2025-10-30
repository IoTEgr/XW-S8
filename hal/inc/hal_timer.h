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
* File Name   : hal_timer.h
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

#ifndef  HAL_TIMER
     #define  HAL_TIMER








 /*******************************************************************************
 * Function Name  : hal_timerStart
 * Description	  : timer start
 * Input		  : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
					   u32 frq	: frq .>20hz
					   void (*isr)(void) : callback function
 * Output		  : None
 * Return		  : s32 : 0 success
							  <0 fail 
 *******************************************************************************/
 s32 hal_timerStart(u8 htimer,u32 frq,void (*isr)(void));
 /*******************************************************************************
 * Function Name  : hal_timerStop
 * Description	  : timer stop
 * Input		  : u8 htimer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
 * Output		  : None
 * Return		  : s32 : 0 success
							  <0 fail 
 *******************************************************************************/
 s32 hal_timerStop(u8 htimer);
 /*******************************************************************************
 * Function Name  : hal_timerEnable
 * Description	  : timer enable /disable
 * Input		  : u8 htimer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
					   u8 en	  : 0:disable,1 :enable
 * Output		  : None
 * Return		  : s32 : 0 success
							  <0 fail 
 *******************************************************************************/
 s32 hal_timerEnable(u8 htimer,u8 en);
 /*******************************************************************************
 * Function Name  : hal_timerTickEnable
 * Description	  : timer tick enable /disable
 * Input		  :u8 en	  : 0:disable,1 :enable
 * Output		  : None
 * Return		  : s32 : 0 success
							  <0 fail 
 *******************************************************************************/
 s32 hal_timerTickEnable(u8 en);
 /*******************************************************************************
 * Function Name  : hal_timerTickGet
 * Description	  : timer tick count
 * Input		  : 
 * Output		  : None
 * Return		  : s32 : tick count
 *******************************************************************************/
 s32 hal_timerTickGet(void);


 /*******************************************************************************
 * Function Name  : hal_timerPWMStart
 * Description	  : timer PWM start
 * Input		  : timer: timer select
 *					pwm_map: PWM output group select
 *					frq:	 PWM frequency configure
 *					percent: PWM duty rate(<100)
 * Output		  : None
 * Return		  : count
 *******************************************************************************/
 s8 hal_timerPWMStart(u8 timer,u8 pwm_map,u32 frq, u8 percent);
 
 s8  hal_timerPWMStop(u8 timer);




#endif

