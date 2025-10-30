/****************************************************************************
**
 **                          BUILDWIN HAL LAYER
  ** *   **                   THE APPOTECH HAL
   **** **                 GPIO HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_gpio.c
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
* Function Name  : hal_gpioInit
* Description    : hal layer .gpio initial for normal using
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
                      u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
                      u8 dir  :  dir. ->GPIO_OUTPUT,GPIO_INPUT
                      u8 pull : pull. ->GPIO_PULL_FLOATING,GPIO_PULL_UP,GPIO_PULL_DOWN,GPIO_PULL_UPDOWN
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
void hal_gpioInit(u8 ch,u32 pin,u8 dir,u8 pull)
{
	ax32xx_gpioDigitalSet(ch,pin,GPIO_DIGITAL_EN);
	ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_GPIO);     // set as gpio func
	ax32xx_gpioDrvSet(ch,pin,GPIO_DRV_NORMAL);   // set normal drving
	ax32xx_gpioDirSet(ch,pin,dir);                    // set direction
	ax32xx_gpioLedPull(ch,pin,GPIO_PULLE_FLOATING);  // set LED pull level,	
	ax32xx_gpioPullSet(ch,pin,pull);                 // set pull level,
}
void hal_gpioInit_io1d1(u8 ch,u32 pin,u8 dir,u8 pull)  //HIGH VOL IO
{
	ax32xx_gpioDigitalSet(ch,pin,GPIO_DIGITAL_DIS);
	ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_GPIO);     // set as gpio func
	ax32xx_gpioDrvSet(ch,pin,GPIO_DRV_NORMAL);   // set normal drving
	ax32xx_gpioDirSet(ch,pin,dir);                    // set direction
	ax32xx_gpioLedPull(ch,pin,GPIO_PULLE_FLOATING);  // set LED pull level,	
	ax32xx_gpioPullSet(ch,pin,pull);                 // set pull level,
}
/*******************************************************************************
* Function Name  : hal_gpioLedInit
* Description    : hal layer .gpio initial for led control
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
                      u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
                      u8 pull : pull. ->GPIO_PULLE_FLOATING,GPIO_PULLE_UP,GPIO_PULLE_DOWN,GPIO_PULLE_UPDOWN
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
void hal_gpioEPullSet(u8 ch,u32 pin,u8 led_pull)
{
	ax32xx_gpioPullSet(ch,pin,GPIO_PULL_FLOATING);  // set pull level,	

	ax32xx_gpioLedPull(ch,pin,led_pull);  // set pull level,	
}
/*******************************************************************************
* Function Name  : hal_gpioWrite
* Description    : hal layer .gpio write to output
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
                      u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
                      u8 data : data. ->GPIO_LOW,GPIO_HIGH
* Output         : None
* Return         : s32 : 0 success
                             <0 fail 
*******************************************************************************/
void hal_gpioWrite(u8 ch,u32 pin,u8 data)
{
	ax32xx_gpioDataSet(ch,pin,data);
}
/*******************************************************************************
* Function Name  : hal_gpioRead
* Description    : hal layer .gpio read input data
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
                      u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
* Output         : None
* Return         : u8 : 1->high,0->low
*******************************************************************************/
u8 hal_gpioRead(u8 ch,u32 pin)
{
	return ax32xx_gpioDataGet(ch,pin);
}
/*******************************************************************************
* Function Name  : hal_gpioIntInit
* Description    : hal layer .gpio ext irq initial
* Input          : u8 ch : gpio channel->GPIO_INT0_PA8~GPIO_INT15_15
                      u8 trigger : trigger.TRIGGER_LEVEL,TRIGGER_EDGE_RISING,TRIGGER_EDGE_FALLING
                      void (*isr)(void) : call back
* Output         : None
* Return         : u8 : 1->high,0->low
*******************************************************************************/
void hal_gpioIntInit(u8 ch,u8 trigger,void (*isr)(void))
{
	ax32xx_gpioINTInit(ch,trigger,isr);
}
/*******************************************************************************
* Function Name  : hal_gpioLedInit
* Description    : hal layer .gpio hardware led
* Input          :  u8 ch : gpio channel->GPIO_LED0_PA8~GPIO_LED11_PF14
                   u8 dir : pull. ->GPIO_PULLE_FLOATING,GPIO_PULLE_UP,GPIO_PULLE_DOWN,GPIO_PULLE_UPDOWN
                   u8 soft : soft->0:soft control,1->hardware
* Output         : None
* Return         : u8 : 1->high,0->low
*******************************************************************************/
void hal_gpioLedInit(u8 ch,u8 pull,u8 soft)
{
	ax32xx_gpioLedInit(ch,pull,soft);
}
