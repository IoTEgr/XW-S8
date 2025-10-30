/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         TIMER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_timer.h
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
#ifndef AX32XX_TIMER_H
    #define AX32XX_TIMER_H

enum
{
	TIMER0=0,
	TIMER1,
	TIMER2,
	TIMER3,

	TIMER_MAX
};

#define  TIMER_FRQ_MIN         20   // min frq hz	
/*******************************************************************************
* Function Name  : ax32xx_timer0IRQHanlder
* Description    : timer0 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer0IRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_timer1IRQHanlder
* Description    : timer1 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer1IRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_timer2IRQHanlder
* Description    : timer2 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer2IRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_timer3IRQHanlder
* Description    : timer3 irq handler
* Input          : 

* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer3IRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_timerISRRegister
* Description    : timer isr register
* Input          :  u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
                       void (*isr)(void) : callback function
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerISRRegister(u8 timer,void (*isr)(void));
/*******************************************************************************
* Function Name  : ax32xx_timerStart
* Description    : timer start
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
                      u32 frq  : frq .>20hz
                      void (*isr)(void) : callback function
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerStart(u8 timer,u32 frq,void (*isr)(void));
/*******************************************************************************
* Function Name  : ax32xx_timeStop
* Description    : timer stop
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerStop(u8 timer);
/*******************************************************************************
* Function Name  : ax32xx_timerEnable
* Description    : timer enable
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerEnable(u8 timer);
/*******************************************************************************
* Function Name  : ax32xx_timerDisable
* Description    : timer disable
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerDisable(u8 timer);
/*******************************************************************************
* Function Name  : ax32xx_timerTickStart
* Description    : tick timer start
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerTickStart(void);
s32 ax32xx_timer3TickStart(void);
/*******************************************************************************
* Function Name  : ax32xx_timerTickStop
* Description    : tick timer stop
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerTickStop(void);
s32 ax32xx_timer3TickStop(void);
/*******************************************************************************
* Function Name  : ax32xx_timerTickCount
* Description    : tick timer counter get
* Input          : 
* Output         : None
* Return         : count
*******************************************************************************/
u32 ax32xx_timerTickCount(void);
u32 ax32xx_timer3TickCount(void);
/*******************************************************************************
* Function Name  : ax32xx_printTimerStart
* Description    : tick timer counter get
* Input          : 
* Output         : None
* Return         : count
*******************************************************************************/
s32 ax32xx_printTimerStart(u8 timer,u32 preload,void (*isr)(void));


#endif
