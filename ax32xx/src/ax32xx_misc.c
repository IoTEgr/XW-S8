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
* File Name   : ax32xx_misc.c
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
#include "../inc/ax32xx.h"

/*******************************************************************************
* Function Name  : str_seek
* Description    : seek for the obj string or char
* Input          : void * src : string
*                  const char *obj
* Output         : none
* Return         : u8 * obj the start address
*******************************************************************************/
u8 *str_seek(void * src,const char * obj)
{
	char * src_cpy = (char *)src;
	char * obj_cpy = (char *)obj;
	bool compare = 0;
	while((*src_cpy) != '\0'){
		while((*obj_cpy) != '\0'){
			if(*src_cpy == *obj_cpy){
				src_cpy++;
				obj_cpy++;
				compare = 1;
			}else{
				obj_cpy = (char *)obj;
				compare = 0;
				break;
			}
		}
		if (compare){
		return (u8 *)src_cpy;
		}
		src_cpy++;		
	}
	return NULL;
}
/*******************************************************************************
* Function Name  : dec_num2str
* Description    : dec value to string
* Input          : u8 src value
*                  u8* dest : string address
* Output         : none
* Return         : none
*******************************************************************************/
void dec_num2str(u8 src , u8* dest)
{
	u8 temp = src / 10;
	dest[0] = '0'+temp;
	dest[1] = '0' + src -temp*10;
}
/*******************************************************************************
* Function Name  : char_2_hex
* Description    : char to hex
* Input          : u8 val : char value
* Output         : none
* Return         : hex
*******************************************************************************/
u8 char_2_hex(u8 val)
{
	if((val >= '0')&& (val <= '9')){
		return val - '0';	
	}
	if((val >= 'a')&& (val <= 'f')){
		return (val - 'a') + 10;	
	}
	if((val >= 'A')&& (val <= 'F')){
		return (val - 'A') + 10;	
	}
	return 0;
}
/*******************************************************************************
* Function Name  : data_check
* Description    : check data in range
* Input          : void *p_data : data poninter, min: low_bound max:high_bound
* Output         : none
* Return         : bool
*******************************************************************************/
bool data_check(void *p_data,u32 min,u32 max)
{
	u8 data = *(u8*)p_data;
	if((data>=(u8)min)&&(data<=(u8)max)){
		return true;
	}else{
		debg("data:%x out of range!!!\n",p_data);
		return false;
	}
}
/*******************************************************************************
* Function Name  : pointer_check
* Description    : check pointer
* Input          : void* p_pointer pointer of a pointer 
* Output         : none
* Return         : bool
*******************************************************************************/
bool pointer_check(void* p_pointer)
{
	u32 pointer = *(u32*)p_pointer;
	if(pointer){
		return true;
	}else{
		debg("pointer:%x not register!!!\n",p_pointer);
		return false;
	}
}
/*******************************************************************************
* Function Name  : half_word_memcpy
* Description    : half word memory copy
* Input          : void *dst :dest addr
* 				   void *src :source addr
* 				   int size  :  size
** Output         : none
* Return         : hex
*******************************************************************************/
void half_word_memcpy(void *dst, void *src, int size)
{
	u16 *pDstTemp = (u16 *)dst;
	u16 *pSrcTemp = (u16 *)src;
	while (size){
		*pDstTemp++ = *pSrcTemp++;
		size -=2;
	}
}
/*******************************************************************************
* Function Name  : word_memcpy
* Description    : word memory copy
* Input          : void *dst :dest addr
* 				   void *src :source addr
* 				   int size  :  size
** Output         : none
* Return         : hex
*******************************************************************************/
void word_memcpy(void *dst, void *src, int size)
{
	u32 *pDstTemp = (u32 *)dst;
	u32 *pSrcTemp = (u32 *)src;
	while (size){
		*pDstTemp++ = *pSrcTemp++;
		size-=4;
	}
}
/*******************************************************************************
* Function Name  : word_reg_memcpy
* Description    : word_reg_memcpy
* Input          : void *dst :dest addr
* 				   void src :reg addr
* 				   int cnt  :  word cnt
** Output         : none
* Return         : hex
*******************************************************************************/
void word_reg_memcpy(void *dst, u32 src, int cnt)
{
	u32 *pDstTemp = (u32 *)dst;
	u32 pSrcTemp = src;
	while (cnt > 0)
	{
		*pDstTemp++ = SFR_AT(pSrcTemp);
		pSrcTemp+=4;
		cnt --;
	}
}



