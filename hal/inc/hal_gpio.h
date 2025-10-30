/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         GPIO HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_gpio.h
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
#ifndef  HAL_GPIO_H
    #define  HAL_GPIO_H




/*******************************************************************************
* Function Name  : hal_gpioInit
* Description	 : hal layer .gpio initial for normal using
* Input 		 : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
					  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
					  u8 dir  :  dir. ->GPIO_OUTPUT,GPIO_INPUT
					  u8 pull : pull. ->GPIO_PULL_FLOATING,GPIO_PULL_UP,GPIO_PULL_DOWN,GPIO_PULL_UPDOWN
* Output		 : None
* Return		 : s32 : 0 success
							 <0 fail 
*******************************************************************************/
void hal_gpioInit(u8 ch,u32 pin,u8 dir,u8 pull);
void hal_gpioInit_io1d1(u8 ch,u32 pin,u8 dir,u8 pull);  //HIGH VOL  IO
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
void hal_gpioEPullSet(u8 ch,u32 pin,u8 led_pull);
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
void hal_gpioWrite(u8 ch,u32 pin,u8 data);
/*******************************************************************************
* Function Name  : hal_gpioRead
* Description    : hal layer .gpio read input data
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
* Output         : None
* Return         : u8 : 1->high,0->low
*******************************************************************************/
u8 hal_gpioRead(u8 ch,u32 pin);
/*******************************************************************************
* Function Name  : hal_gpioIntInit
* Description    : hal layer .gpio ext irq initial
* Input          : u8 ch : gpio channel->GPIO_INT0_PA8~GPIO_INT15_15
				   u8 trigger : trigger.TRIGGER_LEVEL,TRIGGER_EDGE_RISING,TRIGGER_EDGE_FALLING
				   void (*isr)(void) : call back
* Output         : None
* Return         : u8 : 1->high,0->low
*******************************************************************************/
void hal_gpioIntInit(u8 ch,u8 trigger,void (*isr)(void));
/*******************************************************************************
* Function Name  : hal_gpioLedInit
* Description    : hal layer .gpio hardware led
* Input          :  u8 ch : gpio channel->GPIO_LED0_PA8~GPIO_LED11_PF14
                   u8 dir : pull. ->GPIO_PULLE_FLOATING,GPIO_PULLE_UP,GPIO_PULLE_DOWN,GPIO_PULLE_UPDOWN
                   u8 soft : soft->0:soft control,1->hardware
* Output         : None
* Return         : u8 : 1->high,0->low
*******************************************************************************/
void hal_gpioLedInit(u8 ch,u8 pull,u8 soft);
/*******************************************************************************
* Function Name  : hal_gpioSetDirection
* Bref           : void hal_gpioSetDirection(u8 ch,u32 pin,u8 dir)
* Description	 : hal layer .gpio set gpio direction
* Input 		 : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir  :  dir. ->GPIO_OUTPUT,GPIO_INPUT
* Output		 : None
* Return		 : none
*******************************************************************************/
#define hal_gpioSetDirection      ax32xx_gpioDirSet
/*******************************************************************************
* Function Name  : hal_gpioSetPull
* Bref           : void hal_gpioSetPull(u8 ch,u32 pin,u8 pull)
* Description	 : hal layer .gpio set gpio direction
* Input 		 : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
				   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 pull : pull. ->GPIO_PULL_FLOATING,GPIO_PULL_UP,GPIO_PULL_DOWN,GPIO_PULL_UPDOWN
* Output		 : None
* Return		 : s32 : 0 success
							 <0 fail 
*******************************************************************************/
#define hal_gpioSetPull          ax32xx_gpioPullSet
/*******************************************************************************
* Function Name  : hal_gpioSetFunction
* Bref           : void hal_gpioSetFunction(u8 ch,u32 pin,u8 map)
* Description	 : hal layer .gpio set gpio direction
* Input 		 : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
*                  u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
				   u8 dir : map. ->GPIO_FUNC_GPIO,GPIO_FUNC_SFR
* Output		 : None
* Return		 : s32 : 0 success
							 <0 fail 
*******************************************************************************/
#define hal_gpioSetFunction      ax32xx_gpioMapSet
/*******************************************************************************
* Function Name  : hal_gpioSetSFR
* Bref           : void hal_gpioSetSFR(u8 type,u8 group)
* Description	 : hal layer .gpio set gpio direction
* Input 		 : u8 type : function table,see : GPIO_MAP_E
				   u8 group: function group
* Output		 : None
* Return		 : s32 : 0 success
							 <0 fail 
*******************************************************************************/
#define hal_gpioSetSFR             ax32xx_gpioSFRSet
/*******************************************************************************
* Function Name  : hal_gpioDigitalEnable
* Bref           : void hal_gpioDigitalEnable(u8 ch,u32 pin,u8 en)
* Description    : set gpio digital set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
                   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
                   u8 dir : digital. ->GPIO_DIGITAL_DIS,GPIO_DIGITAL_EN
* Output         : None
* Return         : none
*******************************************************************************/
#define hal_gpioDigitalEnable  ax32xx_gpioDigitalSet


/*******************************************************************************
* Function Name  : ax32xx_gpioLedPull
* Description    : set gpio pull for 300R led set
* Input          : u8 ch : gpio channel->GPIO_PA,GPIO_PB,GPIO_PC,GPIO_PD,GPIO_PE,GPIO_PF,GPIO_PG,GPIO_PF
                   u8 pin : gpio pin.->GPIO_PIN0~GPIO_PIN15
                   u8 dir : pull. ->GPIO_PULLE_FLOATING,GPIO_PULLE_UP,GPIO_PULLE_DOWN,GPIO_PULLE_UPDOWN
* Output         : None
* Return         : none
*******************************************************************************/
#define hal_gpioLedPull ax32xx_gpioLedPull




#endif


