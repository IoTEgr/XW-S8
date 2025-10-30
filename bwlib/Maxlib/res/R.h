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
#ifndef  R_H
    #define   R_H



//#include "R_user_gps.h"
//#include "max_user_icon.h"
#include "R_icon.h"
#include "R_string.h"
//#include "../../../ax32_platform_demo/resource/r_palette.h"
//-------------------color table----------------------
/*#define  R_COLOR_WHITE      0xffff
#define  R_COLOR_BLACK      0x0000
#define  R_COLOR_BLUE       0x001f
#define  R_COLOR_GREEN      0x07e0
#define  R_COLOR_RED        0xf800
#define  R_COLOR_YELLOW     0xffe0
#define  R_COLOR_GRAY       0x8c72
#define  R_COLOR_GRAY2      0x9cf3
#define  R_COLOR_ORANGE     0xfca3//0xfe20//0xf381
#define  R_COLOR_TRANSFER   0x8c71
#define  R_COLOR_YWGREEN    0xbea8 // yellow-green
*/

#define R_COLOR_TRANSFER   0xf9		
#define R_COLOR_TBLACK     0xf8


#define R_COLOR_GREEN      0xfd
#define R_COLOR_RED        0xfe
#define R_COLOR_WHITE      0xfb			
#define R_COLOR_BLACK      0xfa

#define R_COLOR_YELLOW     0xf7

#define R_COLOR_BLUE       0xfc
#define R_COLOR_DBLUE      0xf4
#define R_COLOR_BLUE1      0xf5
#define R_COLOR_BLUE2      0xf3

#define R_COLOR_ORANGE     0xfb


#define R_COLOR_GRAY        0xf6
#define R_COLOR_GRAY2       0xf2
#define R_COLOR_GRAY3       0xf1


#define  DRAW_BORDER_COLOR        R_COLOR_WHITE         
#define  DRAW_FILL_COLOR          R_COLOR_BLACK
#define  DRAW_STRING_COLOR        R_COLOR_WHITE
#define  DRAW_MENU_SOPTION        R_COLOR_GRAY
#define  DRAW_INVLIDOP_COLOR      R_COLOR_GRAY
#define  DRAW_TITLE_COLOR         R_COLOR_WHITE
#define  DRAW_SELECTBAR_COLOR     R_COLOR_ORANGE


#define  R_TRANSFER_COLOR_CHECK(c)    (c&0xff000000)
#define  R_TRANSFER_COLOR_GET(c)      (c&0x00ffffff)
#define R_TRANSFER_COLOR_MAKE(c)      (c|0xff000000)
//------------------font table-----------------------
#define  ASCII_ENABLE  1

#define  R_FONT_PAN           0x10   // font with transfer panel
#define  R_FONT_BRD           0x20   // font with border
#define  R_FONT_DEFAULT     0       // ºÚÌå£¬´ÖÌå£¬Ð¡ËÄ
#define  R_FONT_NUM2         0x02  // small 2,black big
#define  R_FONT_NUM3         0x03
#define  R_FONT_NUM4         0x04  // @guliche ´ÖÌå£¬48
#define  R_FONT_NUM5         0x05

#define CHAR_INTER_VAL      0

#define R_TEXT_BOADER       1     //text boader


#define R_SELECTE_BAR_H   28


// menu,dialog,color define


   










/*******************************************************************************
* Function Name  : R_loadResource
* Description    : load resource table             
* Input          : R_ICON_T *res : resource table  
                   unsigned long int r_id : resource id
				   unsigned long int cnt  : table length
* Output         : 
* Return         : none
*******************************************************************************/
extern void R_loadResource(void *res,unsigned long int length);
/*******************************************************************************
* Function Name  : R_getData
* Description    : get resource data              
* Input          :  unsigned long int r_id : resource id
* Output        : 
* Return         : void * 0 : fail
                                  : resource address
*******************************************************************************/
extern void *R_getData(unsigned long int r_id);
/*******************************************************************************
* Function Name  : R_freeData
* Description    : free resource data              
* Input          :  unsigned long int r_id : resource id
* Output        : 
* Return         : 
*******************************************************************************/
extern void R_freeData(unsigned long int r_id);
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
extern int R_getSize(unsigned long int r_id,unsigned short *width,unsigned short *height);
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
extern void *R_getDataAndSize(unsigned long int r_id,unsigned short *width,unsigned short *height);
/*******************************************************************************
* Function Name  : R_getTColor
* Description    : get icon t color           
* Input          :  unsigned long int r_id : resource id
* Output        : 
* Return         : void * 0 : fail
                                 :t color  
*******************************************************************************/
extern unsigned int R_getTColor(unsigned long int r_id);
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
extern int R_getAsciiCharSize(char c,unsigned short *width,unsigned short *heigth,unsigned char font);
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
extern unsigned int R_getAsciiStringSize(char *str,unsigned short *width,unsigned short *heigth,unsigned char font);
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
extern int R_drawAsciiChar(unsigned short *obuff,char c,int x,int y,unsigned short width,unsigned short color,unsigned short fcolor,unsigned char font);
/*******************************************************************************
* Function Name  : R_drawAsciiString
* Description    : draw ascii string   
* Input          :  unsigned short *obuff : outbuff
                       char c : string
                       int x : draw x
                       int y : draw y
                       unsigned short width : buffer width
                       unsigned short color : color
* Output        :      unsigned char font   : font
* Return         : int -1 : fail
                            0 :success  
*******************************************************************************/
extern int R_drawAsciiString(unsigned short *obuff,char *str,int x,int y,unsigned short width,unsigned short color,unsigned short fcolor,unsigned char font);

#endif