/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         IIC DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_iic.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN IIC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_IIC_H
    #define AX32XX_IIC_H


#define   IIC0   0
#define   IIC1   1




/*******************************************************************************
* Function Name  : ax32xx_iic0Init
* Description    : iic 0 initial
* Input          : u32 baudrate : iic0 baudrate
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0Init(u32 baudrate);
/*******************************************************************************
* Function Name  : ax32xx_iic1Uninit
* Description    : iic 1 uninitial
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0Uninit(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0Start
* Description    : iic 0 send start singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0Start(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0Stop
* Description    : iic 0 send stop singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0Stop(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0Stop
* Description    : iic 0 recv ack singal
* Input          :  none
* Output         : None
* Return         : 0->ack,1->nack
*******************************************************************************/
u8 ax32xx_iic0RecvACK(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0SendACK
* Description    : iic 0 send ack singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0SendACK(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0SendByte
* Description    : iic 0 send one byte
* Input          :  u8 byte : byte
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0SendByte(u8 byte);
/*******************************************************************************
* Function Name  : ax32xx_iic0RecvByte
* Description    : iic 0 recv one byte
* Input          :  none
* Output         : None
* Return         : u8 byte : byte
*******************************************************************************/
u8 ax32xx_iic0RecvByte(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0Init
* Description    : iic 0 initial
* Input          : u32 baudrate : iic0 baudrate
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1Init(u32 baudrate);
/*******************************************************************************
* Function Name  : ax32xx_iic1Uninit
* Description    : iic 1 uninitial
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1Uninit(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0Start
* Description    : iic 0 send start singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1Start(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0Stop
* Description    : iic 0 send stop singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1Stop(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0Stop
* Description    : iic 0 recv ack singal
* Input          :  none
* Output         : None
* Return         : 0->ack,1->nack
*******************************************************************************/
u8 ax32xx_iic1RecvACK(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0SendACK
* Description    : iic 0 send ack singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1SendACK(void);
/*******************************************************************************
* Function Name  : ax32xx_iic0SendByte
* Description    : iic 0 send one byte
* Input          :  u8 byte : byte
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1SendByte(u8 byte);
/*******************************************************************************
* Function Name  : ax32xx_iic0RecvByte
* Description    : iic 0 recv one byte
* Input          :  none
* Output         : None
* Return         : u8 byte : byte
*******************************************************************************/
u8 ax32xx_iic1RecvByte(void);







#endif
