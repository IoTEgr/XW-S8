/****************************************************************************
**
 **                              BOARD
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  BOARD HEADER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : board.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is for user board
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  BOARD_H
    #define  BOARD_H

#define __IO_R(x)     (x|0x00000000)
#define __IO_W(x)     (x|0x10000000)
#define __IO_C(x)     (x|0x20000000)

typedef struct Board_Node_S
{
	char name[12];

	int (*init)(void);

	int (*ioctrl)(INT32U prit,INT32U op,INT32U para);

    INT32U prit;
}Board_Node_T;

#define  DEV_NAME_BATTERY      "battery"
#define  DEV_NAME_LCD            "lcd"
#define  DEV_NAME_LED            "led"
#define  DEV_NAME_GSENSOR      "g-sensor"
#define  DEV_NAME_SDCARD       "sd-card"
#define  DEV_NAME_USB            "usb"
#define  DEV_NAME_ADKEY         "ad-key"
#define  DEV_NAME_USENSOR      "usensor"
#define  DEV_NAME_IR      		"ir"
#if HALL_EN
#define DEV_NAME_HALL "hall"
#endif

//----------io command-----------------
#define  IOCTRL_SDC_CHECK    __IO_C(1)
#define  IOCTRL_USB_CHECK    __IO_C(2)
#define  IOCTRL_BAT_CHECK    __IO_C(3)
#define  IOCTRL_LCD_BKLIGHT  __IO_C(4)
#define  IOCTRL_LCD_LCMOFF   __IO_C(5)
#define  IOCTRL_ADC_CHECK    __IO_C(6)

#if HALL_EN
#define IOCTRL_HALL_CHECK __IO_C(12)
#endif
/*
#define  IOCTRL_GSE_LOCK     __IO_C(5)
#define  IOCTRL_GSE_PARK     __IO_C(6)
#define  IOCTRL_GSE_SETLOCK  __IO_C(7)
#define  IOCTRL_GSE_SETPARK  __IO_C(8)
#define  IOCTRL_GSE_STABLE   __IO_C(9) // wait gsensor stable,when power off
#define  IOCTRL_GSE_GETNAME  __IO_C(10)
*/
#define  IOCTRL_KEY_READ     __IO_R(9)
#define  IOCTRL_KEY_POWER    __IO_C(13)
#define  IOCTRL_KEY_POWEROFF __IO_C(14)
#define  IOCTRL_LED_NO0      __IO_C(10)
#define  IOCTRL_LED_CHECK    __IO_C(11)

#define  IOCTRL_CMOS_INIT    __IO_C(15)
#define  IOCTRL_USB_POWER    __IO_C(16) // for usensor power enable
#define  IOCTRL_IR_SET    	__IO_C(17)
#define  IOGET_IR_GET    	__IO_C(18)
#define  IOGET_LCD_BKLIGHT  __IO_C(19)

/*******************************************************************************
* Function Name  : boardInit
* Description    : 
* Input          : Board_Node_T *table : board node table
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int boardInit(Board_Node_T *table);
/*******************************************************************************
* Function Name  : boardOpen
* Description    :  open a board node by node name
* Input          : char *name : name
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int boardOpen(char *name);
/*******************************************************************************
* Function Name  : boardIoctrl
* Description    :  ioctrl for board node
* Input          : int fd : node index
                      INT32U op: operation
                      INT32U para:paramater
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int boardIoctrl(int fd,INT32U op,INT32U para);
/*******************************************************************************
* Function Name  : boardNameIoctrl
* Description    :  ioctrl for board node
* Input          : char *name : name
                      INT32U op: operation
                      INT32U para:paramater
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int boardNameIoctrl(char *name,INT32U op,INT32U para);






















#endif


