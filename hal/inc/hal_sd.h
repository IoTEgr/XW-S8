/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                   THE APPOTECH HAL
   **** **                 SD HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_sd.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN SD HAL LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef HAL_SD_H
#define HAL_SD_H



typedef enum
{
	SDC_STATE_NULL = 0,
    
    SDC_STATE_READ,
    SDC_STATE_WRITE,
 //   SD_ABEND,
    SDC_STATE_FREE,
    SDC_STATE_ERROR,
}SDC_STAT_E;
typedef enum
{
    SDC_TYPE_NULL,
    SDC_TYPE_10,
    SDC_TYPE_20_NC,
    SDC_TYPE_20_HC,
    SDC_TYPE_MMC
}SDC_TYPE_E;


typedef struct SDC_STAT_S
{
	u8 	    busy;  
    u8		eVer;
	u8	    eCardState;
	u8   	bus_width;
    u32   	dwCap;
	u32		dwRCA;
    u32   	dwNextLBA;
	
    
}SDC_STAT_T;



/*******************************************************************************
* Function Name  : hal_sdStop
* Description    : hal layer. sdc stop tranfer
* Input          : none
* Output         : None
* Return         : int 1 : ok
                           <0 : fail
*******************************************************************************/
s32 hal_sdStop(void);
/*******************************************************************************
* Function Name  : hal_sdInit
* Description    : hal layer. initial sd
* Input          : sd_bus_width:  SD_BUS_WIDTH1 or SD_BUS_WIDTH4 
* Output         : None
* Return         : none
*******************************************************************************/
s32 hal_sdInit(u8 sd_bus_width);
/*******************************************************************************
* Function Name  : hal_sdUninit
* Description    : hal layer. uninitial sd
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_sdUninit(void);
/*******************************************************************************
* Function Name  : hal_sdLock
* Description    : hal layer.lock sdc state
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_sdLock(void);
/*******************************************************************************
* Function Name  : hal_sdUnlock
* Description    : hal layer. unlock sdc state
* Input          : none
* Output         : None
* Return         :none
*******************************************************************************/
void hal_sdUnlock(void);
/*******************************************************************************
* Function Name  : hal_sdCheck
* Description    : hal layer.sd check if there is sdc
* Input          : none
* Output         : None
* Return         : int 1 : sd
                           0 : no sd card
*******************************************************************************/
s32 hal_sdCheck(void);

/*******************************************************************************
* Function Name  : u8 hal_sdSetCardState(u8 state)
* Description    : hal layer. set sdc state
* Input          : state
* Output         : none
* Return         : none
*******************************************************************************/
void hal_sdSetCardState(u8 state);
/*******************************************************************************
* Function Name  : u8 hal_sdGetCardState(u8 state)
* Description    : hal layer. get sdc state
* Input          : state
* Output         : none
* Return         : none
*******************************************************************************/
u8 hal_sdGetCardState(void);
/*******************************************************************************
* Function Name  : hal_sdGetBusWidth()
* Description    : hal layer. get sdc bus width
* Input          : none
* Output         : none
* Return         : sd bus width
*******************************************************************************/
u8 hal_sdGetBusWidth(void);

/*******************************************************************************
* Function Name  : hal_sdGetCapacity
* Description    : hal layer. get sdc capacity
* Input          : none
* Output         : None
* Return         : sd capacity
*******************************************************************************/
u32 hal_sdCapacity(void);
/*******************************************************************************
* Function Name  : hal_sdExist
* Description    : hal layer.sd check exist
* Input          : none
* Output         : None
* Return         : int 1 : sd
                           0 : no sd card
*******************************************************************************/
s32 hal_sdExist(void);
/*******************************************************************************
* Function Name  : hal_sdWrite
* Description    : hal layer.sd write
* Input          : void *pDataBuf : data buffer
                      u32 dwLBA : block index
                      u32 dwLBANum : block num
* Output         : None
* Return         : int 1 : ok
                           0 : fail
*******************************************************************************/
s32 hal_sdWrite(void *pDataBuf, u32 dwLBA, u32 dwLBANum);
/*******************************************************************************
* Function Name  : hal_sdRead
* Description    : hal layer.sd cread
* Input          : void *pDataBuf : data buffer
                      u32 dwLBA : block index
                      u32 dwLBANum : block num
* Output         : None
* Return         : int 1 : ok
                           0 : fail
*******************************************************************************/
s32 hal_sdRead(void *pDataBuf, u32 dwLBA, u32 dwLBANum);


s32 hal_sdNextLBA(void);
void hal_speed_callback(void);







#endif
