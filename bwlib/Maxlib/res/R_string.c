/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **       MAX ROURCE STRING MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : R_string.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib resource string managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
* 2016-09-21  :
*      <1>.Add user_str.c,user can handle it's own string
*      <2>.Change version  to v0103
* 2016-10-17  :
*      <1>.Add resource load function,user can load the resource when system running
*      <2>.Modify user icon get way
*      <3>.Change version to v0104
******************************************************************************/

#include "./../max.h"
#include "./../maxlib.h"



static R_STRING_T *R_String_Table=((void *)0);// = User_String_Table;
static unsigned int R_STRING_MAX,R_STRING_COUNT;


/*******************************************************************************
* Function Name  : R_stringGetIndex
* Description    : get string index        
* Input          :  unsigned long int r_id : string id
* Output        : 
* Return         : int  -1 : fail
                            :index  
******************************************************************************/
static int R_stringGetIndex(unsigned long int r_id)
{
    unsigned int index,i;
    R_STRING_T *table;
    
    if(r_id>=R_STRING_MAX || R_String_Table==(void *)0)
       return -1;
       
    if((r_id & R_ID_TYPE_STR)==0)
       return -1;
    
    index = r_id &R_ID_MASK;
    table = R_String_Table+index;
    if(table->r_id == r_id)
       return index;
    
    for(i=0;i<R_STRING_COUNT;i++)
    {
        if(table->r_id == r_id)
           return i;
    }
    
    return -1;
}
/*******************************************************************************
* Function Name  : R_stringLoadResource
* Description    : load string resource table       
* Input          : R_STRING_T *res : string table
                   unsigned long int length : table length
* Output         : 
* Return         : none
******************************************************************************/
void R_stringLoadResource(R_STRING_T *res,unsigned long int length)
{
	 unsigned int i;
     R_STRING_T *table;

     R_String_Table = res;
	 R_STRING_MAX = length|R_ID_TYPE_STR;
     R_STRING_COUNT=(R_STRING_MAX&R_ID_MASK);

	 for(i=0;i<R_STRING_COUNT;i++)
	 {
        table = R_String_Table+i;
		table->string = NULL;
		table->width = 0;
		table->height=0;
	 }
}
/*******************************************************************************
* Function Name  : R_stringGetData
* Description    : get string data        
* Input          :  unsigned long int r_id : string id
* Output        : 
* Return         : int  0 : fail
                            :data  
******************************************************************************/
void *R_stringGetData(unsigned int r_id)
{
    int index;

    R_STRING_T *table;

	//printf("R : get data : 0x%x\r\n",r_id);
    index = R_stringGetIndex(r_id);
    if(index<0)
    {
		//printf("R string : get index error : 0x%x\r\n",r_id);
       return (void *)0;
    }
    table = R_String_Table+index;
	if(table->string == (char *)0)
        table->string = (char *)user_string_data_get(index);
	return table->string;

}
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
int R_stringGetSize(unsigned int r_id,unsigned short *width,unsigned short *height)
{
    int index;
    R_STRING_T *table;
    
    index = R_stringGetIndex(r_id);
    if(index<0)
       return -1;
	
	table = R_String_Table+index;

	if(table->height == 0 && table->width == 0)
	{
	   user_string_size_get(table->r_name,&table->width,&table->height);   
	}

	if(width)
		*width = table->width;
	if(height)
		*height = table->height;

	return 0;
}
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
void *R_stringGetDataAndSize(unsigned long int r_id,unsigned short *width,unsigned short *height)
{
	R_stringGetSize(r_id,width,height);
	
	return R_stringGetData(r_id);  
}

