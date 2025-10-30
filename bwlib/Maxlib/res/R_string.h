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

#ifndef  R_STRING_H
    #define  R_STRING_H

typedef struct R_STRING_S
{
	unsigned int r_id;
    char *string;

	unsigned short width;
	unsigned short height;

	unsigned int r_name;
	
}R_STRING_T;


//#define  R_STRING_MAX        R_ID_STR_MAX
//#define  R_STRING_COUNT     (R_STRING_MAX&R_ID_MASK)	









/*******************************************************************************
* Function Name  : R_stringLoadResource
* Description    : load string resource table       
* Input          : R_STRING_T *res : string table
                   unsigned long int length : table length
* Output         : 
* Return         : none
******************************************************************************/
extern void R_stringLoadResource(R_STRING_T *res,unsigned long int length);
/*******************************************************************************
* Function Name  : R_stringGetData
* Description    : get string data        
* Input          :  unsigned long int r_id : string id
* Output        : 
* Return         : int  0 : fail
                            :data  
******************************************************************************/
extern void *R_stringGetData(unsigned int r_id);
/*******************************************************************************
* Function Name  : R_stringGetSize
* Description    : get string size        
* Input          :  unsigned long int r_id : str id
                       unsigned short *width: string width
                       unsigned short *height: string height
* Output        : 
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
extern int R_stringGetSize(unsigned int r_id,unsigned short *width,unsigned short *height);
/*******************************************************************************
* Function Name  : R_stringGetDataAndSize
* Description    : get string data and size        
* Input          :  unsigned long int r_id : string id
				unsigned short *width: string width
				unsigned short *height: string height

* Output        : 
* Return         : int  0 : fail
                            :data  
*******************************************************************************/
extern void *R_stringGetDataAndSize(unsigned long int r_id,unsigned short *width,unsigned short *height);






#endif
