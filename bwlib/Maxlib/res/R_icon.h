/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **                    MAX HEADER
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : max.h
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib internal use
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#ifndef  R_ICON_H
    #define  R_ICON_H

typedef struct R_ICON_S
{
    unsigned long   r_id;

    unsigned short  *r_data;
    
    unsigned long  int timeout;
    
    unsigned short  r_width;
    unsigned short  r_height;

	unsigned long   r_tcolor;

	unsigned int    r_name;

	//char       r_name[32];
    
}R_ICON_T;
//#include "typedef.h"
//#include "max_user_icon.h"	

	
//#define  R_ID_ICON_MAX  R_ID_ICON_MAX
//#define  R_ICON_COUNT   (R_ID_ICON_MAX&R_ID_MASK)
	
//extern R_ICON_T User_Icon_Table[R_ICON_COUNT];
/*******************************************************************************
* Function Name  : R_iconLoadResource
* Description    : load icon resource table       
* Input          : R_STRING_T *res : icon table
                   unsigned long int length : table length
* Output         : 
* Return         : none
******************************************************************************/
extern void R_iconLoadResource(R_ICON_T *res,unsigned long int length);
/*******************************************************************************
* Function Name  : R_iconGetData
* Description    : get icon data        
* Input          :  unsigned long int r_id : icon id
* Output        : 
* Return         : int  0 : fail
                            :data  
******************************************************************************/
extern void *R_iconGetData(unsigned long int r_id);

/*******************************************************************************
* Function Name  : R_iconGetSize
* Description    : get iconsize        
* Input          :  unsigned long int r_id : icon id
                       unsigned short *width : width
                       unsigned short *heigth:height
* Output        : 
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
extern int R_iconGetSize(unsigned long int r_id,unsigned short *width,unsigned short *height);

/*******************************************************************************
* Function Name  : R_iconGetDataAndSize
* Description    : get icon data and size        
* Input          :  unsigned long int r_id : icon id
                       unsigned short *width : width
                       unsigned short *heigth:height
* Output        : 
* Return         : int  0 : fail
                            :data  
*******************************************************************************/
extern void *R_iconGetDataAndSize(unsigned long int r_id,unsigned short *width,unsigned short *height);

/*******************************************************************************
* Function Name  : R_iconGetTColor
* Description    : free icon t color       
* Input          :  unsigned long int r_id : icon id
* Output        : 
* Return         : int 
                            :color  
*******************************************************************************/
extern unsigned int R_iconGetTColor(unsigned long int r_id);
/*******************************************************************************
* Function Name  : R_iconFreeData
* Description    : free icon data       
* Input          :  unsigned long int r_id : icon id
* Output        : 
* Return         :
*******************************************************************************/
extern void R_iconFreeData(unsigned long int r_id);








#endif

