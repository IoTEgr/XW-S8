/****************************************************************************
**
 **                              NV FS
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                    NV FS FOR SPI FLASH
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : nvfs.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is user MEDIA header file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  NVFS_H
    #define  NVFS_H


#define  NV_STROAGE_SPI         0
#define  NV_STROAGE_SDC         1
#define  NV_STROAGE_SDRAM         2


#define  NVFS_STROAGE      NV_STROAGE_SPI        // -----user resource configure


/*******************************************************************************
* Function Name  : nv_Init
* Description	 :	nv fs initial
* Input 		 : none
* Output		 : none
* Return		 : int 
					  
*******************************************************************************/
int nv_init(void);
/*******************************************************************************
* Function Name  : nv_uninit
* Description    :  nv fs uninitial
* Input          : none
* Output         : none
* Return         : int 
                      
*******************************************************************************/
int nv_uninit(void);
/*******************************************************************************
* Function Name  : nv_configAddr
* Description	 :	get nv fs configure address
* Input 		 : none
* Output		 : int res_num
* Return		 : int 
					  
*******************************************************************************/
int nv_configAddr(void);
/*******************************************************************************
* Function Name  : nv_open
* Description	 :	nv fs open
* Input 		 : none
* Output		 : int res_num
* Return		 : int 
					  
*******************************************************************************/
int nv_open(int res_num);
/*******************************************************************************
* Function Name  : nv_size
* Description    :  nv fs get file size
* Input          : none
* Output         : int res_num
* Return         : int 
                      
*******************************************************************************/
int nv_size(int res_num);
/*******************************************************************************
* Function Name  : nv_read
* Description    :  nv fs read data
* Input          : none
* Output         : int res_num
* Return         : int 
                      
*******************************************************************************/
int nv_read(int addr,void *buffer,int size);























#endif







