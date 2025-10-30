/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         UART DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_uart0.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN UART0 HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#ifndef AX32XX_UART_H
#define AX32XX_UART_H



#define  UART0   0
#define  UART1   1


/*******************************************************************************
* Function Name  : ax32xx_uart0IRQHandler
* Description    : uart 0 IRQ handler
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0IRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_uart0IOCfg
* Description    : uart0 rx tx config
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0IOCfg(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_uart0Init
* Description    : uart0 initial 
* Input          : u32 baudrate : uart0 baudrate
                      void (*isr)(u8 data) : rx callback
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0Init(u32 baudrate,void (*isr)(u8 data));
/*******************************************************************************
* Function Name  : ax32xx_uart0SendByte
* Description    : uart0 send data
* Input          : u8 data : send data
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0SendByte(u8 data);





#endif
