/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **              MAX ROURCE ICON MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : R_icon.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib resource icon managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
* 2016-09-21  :
*      <1>.Add user_icon.c,user can handle it's own icon
*      <2>.Change version  to v0103
* 2016-10-17  :
*      <1>.Add resource load function,user can load the resource when system running
*      <2>.Modify user icon get way
*      <3>.Change version to v0104
******************************************************************************/
#include "./../max.h"
#include "./../maxlib.h"



#define  ICON_SIZE(w,h)    (w*h)
#define  ICON_FREE       0
#define  ICON_MALL       1


static unsigned int R_ICON_COUNT=0,R_ID_ICON_MAX=0;
static unsigned int firstAddr = 0;
//extern User_Icon_Table[R_ICON_COUNT];
static R_ICON_T *R_Icon_Table=NULL;// = User_Icon_Table;

/*******************************************************************************
* Function Name  : R_iconMemGet
* Description    : malloc memory for icon 
* Input          :  u32_t size : size
                       unsigned char gflag : flag,malloc or free
* Output        : 
* Return         : void *  0 : fail

******************************************************************************/
void *R_iconMemGet(u32_t size,unsigned char gflag)
{
    static u32_t memCount=0;
    u32_t mem;
    
#if CFG_R_ICON_AUTO > 0	    
    u32_t i,timeout,min;
    R_ICON_T *table;
#endif    
    if(gflag == ICON_FREE)
    {
		//max_mem_free((void *)size);
		
    }
	else
	{
//TRY_GET_MEM:
        if((memCount+size)<ICON_MEM_COUNT_MAX)   	
	       mem = 0;//(u32_t )max_mem_malloc(size);
	    else
	       mem = 0;
	    if(firstAddr == 0)
	    {
	       firstAddr = mem;
	    }
		if(mem)
		{
		   memCount+=size;
		   return ((void *)mem);
		}
	#if CFG_R_ICON_AUTO > 0	
		if(memCount<size)
		   return NULL;
		
		timeout = 0xffffffff;
		table = R_Icon_Table;
		min = 0;
		for(i=0;i<R_ICON_COUNT;i++)
		{
		    if(table->r_data !=NULL && timeout>table->timeout)
		    {
		        min = i;
		        timeout = table->timeout;
		    }
		    table++;
		}
		table = R_Icon_Table+min;
		if(timeout!= 0xffffffff && table->r_data !=NULL)
		{
		    //max_mem_free((void *)table->r_data);
		    table->r_data  = NULL;
		    table->timeout = 0;
		    memCount -= ICON_SIZE(table->r_width,table->r_height);
		    
		    goto TRY_GET_MEM;
		}  
	#endif	 
	}
    
    return NULL;

}
/*******************************************************************************
* Function Name  : R_iconGetName
* Description    : load icon name    
* Input          :  unsigned int id : icon id
* Output        : 
* Return         : void *  0 : fail

******************************************************************************/
static void *R_iconGetName(unsigned int id)
{
	unsigned int idx,i;

	idx = id &R_ID_MASK;
	if(idx>=R_ICON_COUNT)
		return NULL;
	
	if(R_Icon_Table[idx].r_id == id)
		return ((void *)R_Icon_Table[idx].r_name);
	
	for(i=0;i<R_ICON_COUNT;i++)
	{
		if(id == R_Icon_Table[idx].r_id)
			break;
	}

	if(i>=R_ICON_COUNT)
		return NULL;
	else
		return ((void *)R_Icon_Table[i].r_name);
}

/*******************************************************************************
* Function Name  : R_iconLoadResData
* Description    : load icon data    
* Input          :  R_ICON_T *icon : icon node
* Output        : 
* Return         : void *  0 : fail

******************************************************************************/
static void *R_iconLoadResData(R_ICON_T *icon)
{
    void *iconname;
	int ret;
//    short int w,h;

	iconname = R_iconGetName(icon->r_id);
//	if(iconname == NULL)
//		return NULL;
    
    if(icon->r_height == 0|| icon->r_width==0)
    {
        ret = user_get_iconSize((u32_t)iconname,&(icon->r_width),&(icon->r_height));
        if(ret<0)
            return NULL;
    }
#if CFG_R_ICON_LOAD > 0  // buffer cache by  r_icon
    icon->r_data = (void *)R_iconMemGet(ICON_SIZE(icon->r_height,icon->r_width),ICON_MALL);
    if(icon->r_data == NULL)
		return NULL;
       
    ret = user_get_icon((u32_t)iconname,(u32_t)icon->r_data,ICON_SIZE(icon->r_height,icon->r_width));
    if(ret<0)
    {
        R_iconMemGet((u32_t)icon->r_data,ICON_FREE);
        icon->r_data = NULL;
        return NULL;
    }
#else // line cache by draw
    icon->r_data = (void *)user_get_icon_addr((u32_t)iconname);
#endif
	return icon->r_data;
}
//--------------------------------------------------------------------------------------------------------


/*******************************************************************************
* Function Name  : R_iconGetIndex
* Description    : get icon index        
* Input          :  unsigned long int r_id : icon id
* Output        : 
* Return         : int  -1 : fail
                            :index  
******************************************************************************/
static int R_iconGetIndex(unsigned long int r_id)
{
    unsigned int index,i;
    R_ICON_T *table;
    
    if(r_id>=R_ID_ICON_MAX || R_Icon_Table == NULL)
       return -1;
       
    if((r_id & R_ID_TYPE_ICON)==0)
       return -1;
    
    index = r_id &R_ID_MASK;
    table = R_Icon_Table+index;
    if(table->r_id == r_id)
       return index;
    
    for(i=0;i<R_ICON_COUNT;i++)
    {
        if(table->r_id == r_id)
           return i;
    }
    
    return -1;
}
/*******************************************************************************
* Function Name  : R_iconLoadResource
* Description    : load icon resource table       
* Input          : R_STRING_T *res : icon table
                   unsigned long int length : table length
* Output         : 
* Return         : none
******************************************************************************/
void R_iconLoadResource(R_ICON_T *res,unsigned long int length)
{
	int i;
    R_ICON_T *table;

    R_Icon_Table = res;
	
    R_ID_ICON_MAX = length|R_ID_TYPE_ICON;
    R_ICON_COUNT = (R_ID_ICON_MAX&R_ID_MASK);

	for(i=0;i<R_ICON_COUNT;i++)
	{
        table = R_Icon_Table+i;
		if(table->r_data)
		   R_iconMemGet((u32_t)table->r_data,ICON_FREE);
		table->r_data = NULL;
	}
}
/*******************************************************************************
* Function Name  : R_iconGetData
* Description    : get icon data        
* Input          :  unsigned long int r_id : icon id
* Output        : 
* Return         : int  0 : fail
                            :data  
******************************************************************************/
void *R_iconGetData(unsigned long int r_id)
{
    int index;
    R_ICON_T *table;

	//printf("R : get data : 0x%x\r\n",r_id);
    index = R_iconGetIndex(r_id);
    if(index<0)
    {
//		printf("R icon : get index error : 0x%x\r\n",r_id);
       return (void *)0;
    }
    table = R_Icon_Table+index;


	if(table->r_data == (void *)0)
	{
		R_iconLoadResData(table);
	}
#if CFG_R_ICON_AUTO > 0	
    if(table->r_data!=NULL)
       table->timeout = drawGetTime();
#endif       
    return (void *)table->r_data;     
}
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
int R_iconGetSize(unsigned long int r_id,unsigned short *width,unsigned short *height)
{
    int index;
    R_ICON_T *table;
    
    index = R_iconGetIndex(r_id);
    if(index<0)
       return -1;
	
    
    table = R_Icon_Table+index;
	if(table->r_width ==0 || table->r_height == 0)
	     user_get_iconSize((u32_t)table->r_name,&(table->r_width),&(table->r_height));   
    if(width)
       *width = table->r_width;
    if(height)
       *height = table->r_height;
       
    return 0;   
}
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
void *R_iconGetDataAndSize(unsigned long int r_id,unsigned short *width,unsigned short *height)
{
    int index;
    R_ICON_T *table;
    
    index = R_iconGetIndex(r_id);
    if(index<0)
       return (void *)0;

	if(R_iconGetData(r_id)==NULL)
		return NULL;
       
    table = R_Icon_Table+index;
    if(width)
       *width = table->r_width;
    if(height)
       *height = table->r_height;
       
    return table->r_data;      
}
/*******************************************************************************
* Function Name  : R_iconFreeData
* Description    : free icon data       
* Input          :  unsigned long int r_id : icon id
* Output        : 
* Return         :
*******************************************************************************/
void R_iconFreeData(unsigned long int r_id)
{
    int index;
    R_ICON_T *table;
    
    index = R_iconGetIndex(r_id);
    if(index<0)
       return ;
    table = R_Icon_Table+index;  
     
    R_iconMemGet((u32_t)table->r_data,ICON_FREE);
	
	table->r_data = NULL;
	
#if CFG_R_ICON_AUTO > 0	
       table->timeout = 0;
#endif 
}
/*******************************************************************************
* Function Name  : R_iconGetTColor
* Description    : free icon t color       
* Input          :  unsigned long int r_id : icon id
* Output        : 
* Return         : int 
                            :color  
*******************************************************************************/
unsigned int R_iconGetTColor(unsigned long int r_id)
{
	int index;
    R_ICON_T *table;
    
    index = R_iconGetIndex(r_id);
    if(index<0)
       return 0xffffffff;

	table = R_Icon_Table+index;

	return table->r_tcolor;
}	

