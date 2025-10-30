/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         IIC HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_iic.h
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
#ifndef  HAL_IIC_H
    #define  HAL_IIC_H





/*******************************************************************************
* Function Name  : hal_iicInit
* Description	 : hal layer .iic initial
* Input 		 : none
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_iic0Init(void);
	/*******************************************************************************
* Function Name  : hal_iicUninit
* Description    : hal layer .iic uninitial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic0Uninit(void);
/*******************************************************************************
* Function Name  : hal_iic8bitAddrWriteData
* Description	 : hal layer .iic write data for 8bit address slave
* Input 		 : u8 slaveid : slave id
					  u8 addr	 : slave addr
					  u8 data	 : data
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_iic08bitAddrWriteData(u8 slaveid,u8 addr,u8 data);
/*******************************************************************************
* Function Name  : hal_iic8bitAddrReadData
* Description    : hal layer .iic read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic08bitAddrReadData(u8 slaveid,u8 addr,u8 *data);
/*******************************************************************************
* Function Name  : hal_iic8bitAddrWrite
* Description    : hal layer .iic write data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic08bitAddrWrite(u8 slaveid,u8 addr,u8 *data,u8 len);
/*******************************************************************************
* Function Name  : hal_iic8bitAddrRead
* Description    : hal layer .iic read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic08bitAddrRead(u8 slaveid,u8 addr,u8  *data,u8 len);
/*******************************************************************************
* Function Name  : hal_iic16bitAddrWriteData
* Description    : hal layer .iic write data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 data    : data
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic016bitAddrWriteData(u8 slaveid,u16 addr,u8 data);
/*******************************************************************************
* Function Name  : hal_iic16bitAddrReadData
* Description    : hal layer .iic read data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic016bitAddrReadData(u8 slaveid,u16 addr,u8 *data);
/*******************************************************************************
* Function Name  : hal_iic16bitAddrWrite
* Description    : hal layer .iic write data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic016bitAddrWrite(u8 slaveid,u16 addr,u8 *data,u8 len);
/*******************************************************************************
* Function Name  : hal_iic16bitAddrRead
* Description    : hal layer .iic read data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic016bitAddrRead(u8 slaveid,u16 addr,u8  *data,u8 len);








/*******************************************************************************
* Function Name  : hal_iic1IOShare
* Description    : hal layer .iic1 io share
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
u8 hal_iic1IOShare(void);
/*******************************************************************************
* Function Name  : hal_iicInit
* Description	 : hal layer .iic initial
* Input 		 : none
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_iic1Init(void);
	/*******************************************************************************
* Function Name  : hal_iicUninit
* Description    : hal layer .iic uninitial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic1Uninit(void);

/*******************************************************************************
* Function Name  : hal_iic8bitAddrWriteData
* Description	 : hal layer .iic write data for 8bit address slave
* Input 		 : u8 slaveid : slave id
					  u8 addr	 : slave addr
					  u8 data	 : data
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_iic18bitAddrWriteData(u8 slaveid,u8 addr,u8 data);
/*******************************************************************************
* Function Name  : hal_iic8bitAddrReadData
* Description    : hal layer .iic read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic18bitAddrReadData(u8 slaveid,u8 addr,u8 *data);
/*******************************************************************************
* Function Name  : hal_iic8bitAddrWrite
* Description    : hal layer .iic write data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic18bitAddrWrite(u8 slaveid,u8 addr,u8 *data,u8 len);
/*******************************************************************************
* Function Name  : hal_iic8bitAddrRead
* Description    : hal layer .iic read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic18bitAddrRead(u8 slaveid,u8 addr,u8  *data,u8 len);
/*******************************************************************************
* Function Name  : hal_iic16bitAddrWriteData
* Description    : hal layer .iic write data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 data    : data
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic116bitAddrWriteData(u8 slaveid,u16 addr,u8 data);
/*******************************************************************************
* Function Name  : hal_iic16bitAddrReadData
* Description    : hal layer .iic read data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic116bitAddrReadData(u8 slaveid,u16 addr,u8 *data);
/*******************************************************************************
* Function Name  : hal_iic16bitAddrWrite
* Description    : hal layer .iic write data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic116bitAddrWrite(u8 slaveid,u16 addr,u8 *data,u8 len);
/*******************************************************************************
* Function Name  : hal_iic16bitAddrRead
* Description    : hal layer .iic read data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic116bitAddrRead(u8 slaveid,u16 addr,u8  *data,u8 len);





















#endif


