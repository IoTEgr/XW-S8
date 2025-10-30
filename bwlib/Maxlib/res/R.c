/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **              MAX ROURCE MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : R.h
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib resource managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "./../max.h"
#include "R.h"

/*******************************************************************************
* Function Name  : R_loadResource
* Description    : load resource table             
* Input          : R_ICON_T *res : resource table  
                   unsigned long int r_id : resource id
				   unsigned long int cnt  : table length
* Output         : 
* Return         : none
*******************************************************************************/
void R_loadResource(void *res,unsigned long int length)
{
	unsigned int r_id;

	if(res == NULL)
		return ;
	r_id = *((unsigned int *)res);
    if((r_id & R_ID_CHECK)==R_ID_TYPE_ICON)
	{
		R_iconLoadResource(res,length);
	}
	else if((r_id & R_ID_CHECK)==R_ID_TYPE_STR)
	{
		R_stringLoadResource(res,length);
	}

}
/*******************************************************************************
* Function Name  : R_getData
* Description    : get resource data              
* Input          :  unsigned long int r_id : resource id
* Output        : 
* Return         : void * 0 : fail
                                  : resource address
*******************************************************************************/
void *R_getData(unsigned long int r_id)
{

	if((r_id & R_ID_CHECK)==R_ID_TYPE_STR)
	{
		return R_stringGetData(r_id);
	}
	else if((r_id & R_ID_CHECK)==R_ID_TYPE_ICON)
	{
		return R_iconGetData(r_id);
	}
    else
		return (void *)0;
}
/*******************************************************************************
* Function Name  : R_freeData
* Description    : free resource data              
* Input          :  unsigned long int r_id : resource id
* Output        : 
* Return         : 
*******************************************************************************/
void R_freeData(unsigned long int r_id)
{
	if((r_id & R_ID_CHECK)==R_ID_TYPE_STR)
	{
		//return R_stringFreeData(r_id);
	}
	else if((r_id & R_ID_CHECK)==R_ID_TYPE_ICON)
	{
		R_iconFreeData(r_id);
	}

}
/*******************************************************************************
* Function Name  : R_getSize
* Description    : get resource size              
* Input          :  unsigned long int r_id : resource id
                      unsigned short *width : width
                      unsigned short *height:height
* Output        : 
* Return         : int 0 : success
                           -1 :fail
*******************************************************************************/
int R_getSize(unsigned long int r_id,unsigned short *width,unsigned short *height)
{
    if((r_id & R_ID_CHECK)==R_ID_TYPE_STR)
	{
		return R_stringGetSize(r_id,width,height);
	}
	else if((r_id & R_ID_CHECK)==R_ID_TYPE_ICON)
	{
		return R_iconGetSize(r_id,width,height);
	}
    else
		return -1; 
}
/*******************************************************************************
* Function Name  : R_getDataAndSize
* Description    : get resource size  & data             
* Input          :  unsigned long int r_id : resource id
                      unsigned short *width : width
                      unsigned short *height:height
* Output        : 
* Return         : void * 0 : fail
                                 :data address
*******************************************************************************/

void *R_getDataAndSize(unsigned long int r_id,unsigned short *width,unsigned short *height)
{
   if((r_id & R_ID_CHECK)==R_ID_TYPE_STR)
	{
		return R_stringGetDataAndSize(r_id,width,height);
	}
	else if((r_id & R_ID_CHECK)==R_ID_TYPE_ICON)
	{
		return R_iconGetDataAndSize(r_id,width,height);
	}
    else
		return (void *)0;      
}
/*******************************************************************************
* Function Name  : R_getTColor
* Description    : get icon t color           
* Input          :  unsigned long int r_id : resource id
* Output        : 
* Return         : void * 0 : fail
                                 :t color  
*******************************************************************************/
unsigned int R_getTColor(unsigned long int r_id)
{
	if((r_id & R_ID_CHECK)==R_ID_TYPE_ICON)
	{
		return R_iconGetTColor(r_id);
	}

	return 0xffffffff;
}
#if ASCII_ENABLE == 1

extern const unsigned char *ascii_draw_get(char c,unsigned char *width,unsigned char *heigth,unsigned char font);
/*******************************************************************************
* Function Name  : R_getAsciiCharSize
* Description    : get ascii char size        
* Input          :  char c : char value
                       unsigned short *width : width
                       unsigned short *heigth:height
* Output        : 
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
int R_getAsciiCharSize(char c,unsigned short *width,unsigned short *heigth,unsigned char font)
{
     unsigned char w,h;
     const unsigned char *table;

	 table = ascii_draw_get(c,&w,&h,font);
	 if(table == ((void *)0))
		return -1;

	 if(width)
		 *width = w+CHAR_INTER_VAL;
	 if(heigth)
		 *heigth = h;

	 return 0;
}
/*******************************************************************************
* Function Name  : R_getAsciiStringSize
* Description    : get ascii string size        
* Input          :  char c : string
                       unsigned short *width : width
                       unsigned short *heigth:height
* Output        : 
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
// lyc modify
unsigned int R_getAsciiStringSize(char *str,unsigned short *width,unsigned short *heigth,unsigned char font)
{
    unsigned short w,h;
    unsigned short tw,th;
	unsigned int num=0;
	char c;

	tw = 0;
	th = 0;

	while(*str)
	{
		c = *str++;
		if(R_getAsciiCharSize(c,&w,&h,font)>=0)
		{
			tw +=w;
			if(h>th)
				th = h;
		}
		num++;
	}

	 if(width)
		 *width = tw;
	 if(heigth)
		 *heigth = th;

	return num;
}
/*******************************************************************************
* Function Name  : R_drawAsciiChar
* Description    : draw ascii char   
* Input          :  unsigned short *obuff : outbuff
                       char c : string
                       int x : draw x
                       int y : draw y
                       unsigned short width : buffer width
                       unsigned short color : color
* Output        : 
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
int R_drawAsciiChar(unsigned short *obuff,char c,int x,int y,unsigned short width,unsigned short color,unsigned short fcolor,unsigned char font)
{
	unsigned char w,h,temp,mask,panel;
	int i,j;
	color_t *line,*outbuff;
	const unsigned char *table;

	table = ascii_draw_get(c,&w,&h,font);
	if(table == ((void *)0))
		return -1;

    panel = font&R_FONT_PAN;
    outbuff = (color_t *)obuff;
	line = outbuff+(x)+((y*width));

	for(i=0;i<h;i++)
	{
	  for(j=0;j<(w+7)/8;j++)
	  {

	   temp = *table++;

			 for(mask=0;mask<8;mask++)
			 {
				 
				 if(temp&(0x80>>mask))
				 {
					 *line++ = (color_t)color;
					 
				 }
				 else if(panel)
				 {
					 *line++ = fcolor;
				 }
				 else
				 {
				 	line++;
				 }

			 }
		 }
		 y++;
		 line =outbuff+(x)+((y*width));
		 ax32xx_sysDcacheWback((u32)outbuff+(x)+(((y-1)*width)),(w+7)/8);
	}

	return ((h<<8)|w);		
}
/*******************************************************************************
* Function Name  : R_drawAsciiChar
* Description    : draw ascii char   
* Input          :  unsigned short *obuff : outbuff
                       char c : string
                       int x : draw x
                       int y : draw y
                       unsigned short width : buffer width
                       unsigned short color : color
* Output        : 
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
int R_drawAsciiChar_border(unsigned short *obuff,char c,int x,int y,unsigned short width,unsigned short color,unsigned short fcolor,unsigned char font)
{
	unsigned char w,h,temp,mask,panel;
	int i,j;
	color_t *line,*outbuff;
	const unsigned char *table;
#if R_TEXT_BOADER > 0
	unsigned char tw,tn,tp,fg=0;
#endif
	table = ascii_draw_get(c,&w,&h,font);
	if(table == ((void *)0))
		return -1;

     panel = font&R_FONT_PAN;
    outbuff = (color_t *)obuff;
	line = outbuff+(x)+((y*width));
#if R_TEXT_BOADER > 0
	tw = (w+7)>>3;
#endif
	for(i=0;i<h;i++)
	{
	  for(j=0;j<(w+7)/8;j++)
	  {
#if R_TEXT_BOADER > 0
	   if((i!=h-1))
		  tn = *(table+tw);
	   else
		  tn = 0;
	   if(i!=0)
		   tp = *(table-tw);
	   else
		   tp = 0;
#endif
	   temp = *table++;

			 for(mask=0;mask<8;mask++)
			 {
				 
				 if(temp&(0x80>>mask))
				 {
             #if R_TEXT_BOADER > 0
					 if(mask == 0 && fg==0)
					 {
						 *(line-1) = R_COLOR_BLACK;
					 }
					 fg = 1;
			 #endif
					
					 *line++ = (color_t)color;
			      //   debug_print("*");
					 
				 }
        #if R_TEXT_BOADER > 0
				 else if(tn&(0x80>>mask) || tp&(0x80>>mask)||(mask!=7 && temp&(0x80>>(mask+1))) || (mask!=0 && temp&(0x80>>(mask-1))) || (mask == 0 && fg))
				 {
					 *line++ = R_COLOR_BLACK;  //boader
					 fg = 0;
				 }
        #endif
				 else
				 {
				 	 if(panel)
						 *line++ = fcolor;
					 else
					 	line++;
             #if R_TEXT_BOADER > 0
					 fg = 0;
		     #endif

				 }

			 }
		 }
		 y++;
	//	 debug_print("\n");
		 line =outbuff+(x)+((y*width));
	      ax32xx_sysDcacheWback((u32)outbuff+(x)+(((y-1)*width)),(w+7)/8);
	}

	return ((h<<8)|w);	
}

/*******************************************************************************
* Function Name  : R_drawAsciiString
* Description    : draw ascii string   
* Input          :  unsigned short *obuff : outbuff
                       char c : string
                       int x : draw x
                       int y : draw y
                       unsigned short width : buffer width
                       unsigned short color : color
* Output        : 
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
int R_drawAsciiString(unsigned short *obuff,char *str,int x,int y,unsigned short width,unsigned short color,unsigned short fcolor,unsigned char font)
{
	char c;
	int ret;

	while(*str)
	{
		c = *str++;
		if(font&R_FONT_BRD)
			ret = R_drawAsciiChar_border(obuff,c,x,y,width,color,fcolor,font);
		else
            ret = R_drawAsciiChar(obuff,c,x,y,width,color,fcolor,font);
		if(ret>0)
		{
			x = x+(ret&0xff)+CHAR_INTER_VAL;
		}

	}

	return 0;
}

#endif

