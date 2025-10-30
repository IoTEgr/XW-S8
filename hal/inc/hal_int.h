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
* File Name   : hal_int.h
* Author      : Ticru 
* Version     : V0200
* Date        : 05/02/2017
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-05-02  : 
*      <1>.This is created by ticru,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  HAL_INT_H
#define  HAL_INT_H


/*******************************************************************************
* Function Name  : hal_intEnable(u8 irqNum,u8 en)
* Description    : int enable set
* Input          : u8 irqNum : irq index,in IRQ_IDX_E
                   u8 en     : 1->enable,0->disable
* Output         : none
* Return         : none
*******************************************************************************/
#define hal_intEnable          ax32xx_intEnable


/*******************************************************************************
* Function Name  : ax32xx_int_priority(u8 irqNum,u8 level)
* Description    : int enable set
* Input          : u8 irqNum : irq index,in IRQ_IDX_E
                   u8 level     : 1->high priority,0->low priority
* Output         : none
* Return         : none
*******************************************************************************/
#define hal_int_priority  		ax32xx_int_priority

/*******************************************************************************
* Function Name  : hal_intInit
* Description    : int initial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_intInit(void);
/*******************************************************************************
* Function Name  : 
* Description    : usage:
*                   void foo(void)
*                   {
*                       HAL_CRITICAL_INIT();
* 
*                       HAL_CRITICAL_ENTER();
*                       // do something
* 
*                       HAL_CRITICAL_EXIT();
*                   }
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
#define HAL_CRITICAL_INIT()     ax32xx_intCriticalInit()

#define HAL_CRITICAL_ENTER()    ax32xx_intCriticalEnter()

#define HAL_CRITICAL_EXIT()     ax32xx_intCriticalExit()



#endif


