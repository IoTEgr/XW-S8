/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX 
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_misc.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : 
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  AX32XX_MISC_H
#define  AX32XX_MISC_H







/*******************************************************************************
* Function Name  : str_seek
* Description    : seek for the obj string or char
* Input          : void * src : string
*                  const char *obj
* Output         : none
* Return         : u8 * obj the start address
*******************************************************************************/
u8 *str_seek(void * src,const char * obj);
/*******************************************************************************
* Function Name  : dec_num2str
* Description    : dec value to string
* Input          : u8 src value
*                  u8* dest : string address
* Output         : none
* Return         : none
*******************************************************************************/
void dec_num2str(u8 src , u8* dest);
/*******************************************************************************
* Function Name  : char_2_hex
* Description    : char to hex
* Input          : u8 val : char value
* Output         : none
* Return         : hex
*******************************************************************************/
u8 char_2_hex(u8 val);
/*******************************************************************************
* Function Name  : data_check
* Description    : check data in range
* Input          : void *p_data : data poninter, min: low_bound max:high_bound
* Output         : none
* Return         : bool
*******************************************************************************/
bool data_check(void *p_data,u32 min,u32 max);
/*******************************************************************************
* Function Name  : pointer_check
* Description    : check pointer
* Input          : void* p_pointer pointer of a pointer 
* Output         : none
* Return         : bool
*******************************************************************************/
bool pointer_check(void* p_pointer);
/*******************************************************************************
* Function Name  : half_word_memcpy
* Description    : half word memory copy
* Input          : void *dst :dest addr
* 				   void *src :source addr
* 				   int size  :  size
** Output         : none
* Return         : hex
*******************************************************************************/
void half_word_memcpy(void *dst, void *src, int size);
/*******************************************************************************
* Function Name  : word_memcpy
* Description    : word memory copy
* Input          : void *dst :dest addr
* 				   void *src :source addr
* 				   int size  :  size
** Output         : none
* Return         : hex
*******************************************************************************/
void word_memcpy(void *dst, void *src, int size);
/*******************************************************************************
* Function Name  : word_reg_memcpy
* Description    : word_reg_memcpy
* Input          : void *dst :dest addr
* 				   void src :reg addr
* 				   int cnt  :  word cnt
** Output         : none
* Return         : hex
*******************************************************************************/
void word_reg_memcpy(void *dst, u32 src, int cnt);




















#endif