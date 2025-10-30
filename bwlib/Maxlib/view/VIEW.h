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
#ifndef  VIEW_H
    #define  VIEW_H

//#include "typedef.h"
#include "./../max.h"
//#include "dialog.h"
//#include "progress_bar.h"




//#define  ID_TAG_VIEW        0x01000000
//#define  ID_TAG_IDMASK      0x00ffffff
//#define  ID_TAG_IDLE        0xffffffff

enum
{
	VIEW_TYPE_TEXT=0,       //------ascii string
	VIEW_TYPE_TEXTPANEL,    //------text panel
	VIEW_TYPE_TEXTLINE,
	
	VIEW_TYPE_STRING,       //-------resource string   
	
	VIEW_TYPE_IMAGE,     
	VIEW_TYPE_BUTTON,
	
	VIEW_TYPE_DRAWRECT,
	VIEW_TYPE_DRAWRRECT,
	VIEW_TYPE_DRAWRECTF,
	VIEW_TYPE_DRAWRRECTF,
	VIEW_TYPE_DRAWRECTB,
	VIEW_TYPE_DRAWRRECTB,
    VIEW_TYPE_DRAWLINE,
    
	VIEW_TYPE_MAX
};

enum
{
    VIEW_INVISIABLE=0,
    VIEW_VISIABLE,
    
    VIEW_VI_MAX
};

enum
{
	VIEW_STAT_IDLE=0,
	VIEW_STAT_BUSY,

    VIEW_STAT_SAVE = 0x80,
	VIEW_STAT_MAX
};
enum 
{
    VIEW_CTRL_ENABLE = 0,
	VIEW_CTRL_UPDATE,
    VIEW_CTRL_FCOLOR,
    VIEW_CTRL_BCOLOR,
    VIEW_CTRL_ROTATE,
    VIEW_CTRL_ZOOM,
    VIEW_CTRL_MOVE,
    
    VIEW_CTRL_MAX
};
enum
{
	VIEW_LAYER_0 = 0,
	VIEW_LAYER_1,
	VIEW_LAYER_2,
	VIEW_LAYER_3,

	VIEW_LAYER_MAX
};

typedef struct LAYOUT_VIEW_S
{
    u16_t  view_type;
    u16_t  view_visiable;
    
    u32_t view_resid;
    
    s16_t view_x;
    s16_t view_y;
    
    
}LAYOUT_VIEW_T;

typedef struct VIEW_S
{
	u8_t  view_type;
	u8_t  view_stat;
	u8_t  view_visiable;
	u8_t  view_savestat;
	
	u8_t  font;
	u8_t  style;
	u8_t  update;
	u8_t  rev;

//	s32_t view_iconid;       //icon id

	u16_t rotate;
	u16_t zoom;  
	  
	s16_t view_x;
	s16_t view_y;

	s16_t view_ex;
	s16_t view_ey;
	
	s16_t draw_x;
	s16_t draw_y;
	
	u16_t view_width;
	u16_t view_height;
	
	u32_t fill_color;
	u32_t boarder_color;

    u32_t view_resid;            //resource id
    
    u32_t view_ctrl;
	
	
	
    void *prit;
//	char   *text;//[TEXTVIEW_CONTEXT_MAX];
	
}VIEW_T;

/*
typedef struct ButtonView_S
{
    VIEW_T view;
    
    u32_t button_event;
    
    void (*onClickLisener)(u32_t view_id,u8_t key_id,u8_t key_event);
}ButtonView_T;


typedef struct TextView_S
{
    VIEW_T view;
    
    char text[TEXTVIEW_CONTEXT_MAX];
}TextView_T;


typedef struct ImageView_S
{
    VIEW_T view;
}ImageView_T;
*/
typedef  int   IMAGEVIEW,TEXTVIEW,STRINGVIEW;
/*******************************************************************************
* Function Name  : viewInit
* Description    : view initial
* Input          : 
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewInit(void);

/*******************************************************************************
* Function Name  : viewUninit
* Description    : view uninitial
* Input          : 
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewUninit(void);

/*******************************************************************************
* Function Name  : viewRegister
* Description    : view register
* Input          : 
* Output         : 
* Return         : s32_t -1 : fail
                                     : view id
*******************************************************************************/
extern s32_t viewRegister(void);

/*******************************************************************************
* Function Name  : viewRelease
* Description    : view release
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewRelease(s32_t id);
/*******************************************************************************
* Function Name  : viewSetHighLayer
* Description    : set view high layer if possiable
* Input          :  s32_t id : view id
                       
* Output         : 
* Return         : s32_t -1 : fail
                       new view id
*******************************************************************************/
extern s32_t viewSetHighLayer(s32_t id);
/*******************************************************************************
* Function Name  : viewSetLowLayer
* Description    : set view low layer if possiable
* Input          :  s32_t id : view id
                       
* Output         : 
* Return         : s32_t -1 : fail
                       new view id
*******************************************************************************/
extern s32_t viewSetLowLayer(s32_t id);
/*******************************************************************************
* Function Name  : viewSetType
* Description    : set view type
* Input          :  s32_t id  : view id
                       u8_t type: view type
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetType(s32_t id,u8_t type);

/*******************************************************************************
* Function Name  : viewGetType
* Description    : get view type
* Input          :  s32_t id  : view id
* Output         : 
* Return         : s32_t -1 : fail
                                     : view type
*******************************************************************************/
extern s32_t viewGetType(s32_t id);

/*******************************************************************************
* Function Name  : viewSetPosition
* Description    : get view position
* Input          :  s32_t id  : view id
                       s16_t *x : position x
                       s16_t *y : posiiton y
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetPosition(s32_t id,s16_t *x,s16_t *y);

/*******************************************************************************
* Function Name  : viewSetPosition
* Description    : set view position
* Input          :  s32_t id  : view id
                       s16_t *x : position x
                       s16_t *y : posiiton y
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetPosition(s32_t id,s16_t x,s16_t y);
/*******************************************************************************
* Function Name  : viewSetPosition2
* Description    : get view position
* Input          :  s32_t id  : view id
                       s16_t *sx : position sx
                       s16_t *sy : posiiton sy
                       s16_t *ex : position ex
                       s16_t *ey : posiiton ey
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetPosition2(s32_t id,s16_t sx,s16_t sy,s16_t ex,s16_t ey);
/*******************************************************************************
* Function Name  : viewGetEndPosition
* Description    : get view position
* Input          :  s32_t id  : view id
                       s16_t *x : position x
                       s16_t *y : posiiton y
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetEndPosition(s32_t id,s16_t *x,s16_t *y);
/*******************************************************************************
* Function Name  : viewSetPosition
* Description    : set really drawing position,this position will be set by darw service
* Input          :  s32_t id  : view id
                       s16_t x : position x
                       s16_t y : posiiton y
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetDrawPosition(s32_t id,s16_t x,s16_t y);
/*******************************************************************************
* Function Name  : viewSetPosition
* Description    : get really drawing position,this position will be set by darw service
* Input          :  s32_t id  : view id
                       s16_t *x : position x
                       s16_t *y : posiiton y
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetDrawPosition(s32_t id,s16_t *x,s16_t *y);
/*******************************************************************************
* Function Name  : viewSetResId
* Description    : set view resource id
* Input          :  s32_t id : view id
                       u32_t resid : resource id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetResId(s32_t id,u32_t resid);
/*******************************************************************************
* Function Name  : viewGetResId
* Description    : get view resource id
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                    : resource id
*******************************************************************************/
extern s32_t viewGetResId(s32_t id);

/*******************************************************************************
* Function Name  : viewSetVisiable
* Description    : set view visiable state
* Input          :  s32_t id : view id
                       u8_t visiable : visiabe state,VIEW_INVISIABLE/VIEW_VISIABLE
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetVisiable(s32_t id,u16_t visiable);
/*******************************************************************************
* Function Name  : viewGetVisiable
* Description    : get view visiable state
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetVisiable(s32_t id);
/*******************************************************************************
* Function Name  : viewSetFont
* Description    : set view font
* Input          :  s32_t id  : view id
                       u8_t font : string font
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetFont(s32_t id,u8_t font);
/*******************************************************************************
* Function Name  : viewGetFont
* Description    : get view text
* Input          :  s32_t id  : view id
* Output         : 
* Return         : s32_t -1 : fail
                            : string font
*******************************************************************************/
extern u8_t viewGetFont(s32_t id);
/*******************************************************************************
* Function Name  : viewSetText
* Description    : set view text
* Input          :  s32_t id  : view id
                       char *string : string text
                       u8_t size : string length
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetText(s32_t id,char *string,u8_t size);

/*******************************************************************************
* Function Name  : viewgetText
* Description    : get view text
* Input          :  s32_t id  : view id
* Output         : 
* Return         : s32_t -1 : fail
                                     : string text
*******************************************************************************/
extern char  *viewGetText(s32_t id);

/*******************************************************************************
* Function Name  : viewSetFillColor
* Description    : set view fill color
* Input          :  s32_t id  : view id
                       u32_t color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetFillColor(s32_t id,u32_t color);

/*******************************************************************************
* Function Name  : viewGetFillColor
* Description    : get view fill color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetFillColor(s32_t id);

/*******************************************************************************
* Function Name  : viewSetBorderColor
* Description    : set view boarder color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetBorderColor(s32_t id,u32_t color);

/*******************************************************************************
* Function Name  : viewGetBoarderColor
* Description    : get view Boarder color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetBoarderColor(s32_t id);

/*******************************************************************************
* Function Name  : viewSetTransparentColor
* Description    : set view Transparent color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetTransparentColor(s32_t id,u32_t color);

/*******************************************************************************
* Function Name  : viewGetTransparentColor
* Description    : get view Transparent color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetTransparentColor(s32_t id);

/*******************************************************************************
* Function Name  : viewClearTransparentColor
* Description    : clear view Transparent color
* Input          :  s32_t id  : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewClearTransparentColor(s32_t id);
/*******************************************************************************
* Function Name  : viewSetRotate
* Description    : set view rotate degree
* Input          :  s32_t id  : view id
                       u32_t degree : rotate degree
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetRotate(s32_t id,s32_t degree);
/*******************************************************************************
* Function Name  : viewCheckFlag
* Description    : check view  flag
* Input          :  s32_t id  : view id
                       u8_t flag : flag
* Output         : 
* Return         : s32_t 1 : flag set
                               0 : flag cleared
*******************************************************************************/
extern s32_t viewCheckFlag(s32_t id,u8_t flag);
/*******************************************************************************
* Function Name  : viewSetFlag
* Description    : set view enable flag
* Input          :  s32_t id  : view id
                       u8_t flag : flag
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetFlag(s32_t id,u8_t flag);
/*******************************************************************************
* Function Name  : viewClearFlag
* Description    : clear enable flag
* Input          :  s32_t id  : view id
                       u8_t flag : flag
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewClearFlag(s32_t id,u8_t flag);
/*******************************************************************************
* Function Name  : viewGetRotate
* Description    : get view rotate degree
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetRotate(s32_t id);
/*******************************************************************************
* Function Name  : viewGetZoom
* Description    : get view Zoom
* Input          :  s32_t id  : view id
                       u32_t Zoom : Zoom
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetZoom(s32_t id);
/*******************************************************************************
* Function Name  : viewSetZoom
* Description    : set view Zoom
* Input          :  s32_t id  : view id
                       u32_t Zoom : Zoom
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetZoom(s32_t id,s32_t zoom);
/*******************************************************************************
* Function Name  : viewSetSize
* Description    : set view size
* Input          :  s32_t id  : view id
                       u16_t width : width
                       u16_t height: height
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewSetSize(s32_t id,u16_t width,u16_t height);

/*******************************************************************************
* Function Name  : viewGetSize
* Description    : get view size
* Input          :  s32_t id  : view id
                       u16_t *width : width
                       u16_t *height: height
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern s32_t viewGetSize(s32_t id,u16_t *width,u16_t *height);

/*******************************************************************************
* Function Name  : viewFindNext
* Description    : find next view
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                     : view id
*******************************************************************************/
extern s32_t viewFindNext(void);
/*******************************************************************************
* Function Name  : viewFindFirst
* Description    : find first view
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                     : view id
*******************************************************************************/
extern s32_t viewFindFirst(void);
/*******************************************************************************
* Function Name  : viewFindEnd
* Description    : find end view
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                     : 
*******************************************************************************/
extern s32_t viewFindEnd(void);

/*******************************************************************************
* Function Name  : viewGetUpdateFlag
* Description    : view get update flag
* Input          :  
* Output         : 
* Return         : s32_t : flag
*******************************************************************************/
extern s32_t viewGetUpdateFlag(void);
/*******************************************************************************
* Function Name  : viewSetUpdateFlag
* Description    : view set update flag
* Input          :  u8_t uflag : flag
* Output         : 
* Return         : s32_t : 0
*******************************************************************************/
extern s32_t viewSetUpdateFlag(u8_t uflag);
/*******************************************************************************
* Function Name  : viewSetRestore
* Description    : restore view state
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern void viewSetRestore(s32_t id);
/*******************************************************************************
* Function Name  : viewSetSave
* Description    : save view state
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern void viewSetSave(s32_t id);
/*******************************************************************************
* Function Name  : viewRegisterByLayer
* Description    : register view at the layer
* Input          :  u8_t layer : layer
* Output         : 
* Return         : s32_t : 0
*******************************************************************************/
extern s32_t viewRegisterByLayer(u8_t layer);
/*******************************************************************************
* Function Name  : viewGetRawDate
* Description    : set view size
* Input          :  s32_t id  : view id
                       u8_t ctrl : using way
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
extern u32_t viewGetRawDate(s32_t id,u8_t ctrl);
/*******************************************************************************
* Function Name  : viewPrint
* Description    : print view node
* Input          : u32_t viewId : view id
* Output         : 
* Return         : 
*******************************************************************************/
extern void viewPrint(u32_t viewId);
/*******************************************************************************
* Function Name  : imageView
* Description    : register a image view
* Input          : u32_t resid : resid
                      s16_t x : position x
                      s16_t y : position y
* Output         : 
* Return         : s32_t : <0 fail
                                      : view id
*******************************************************************************/
extern s32_t imageView(u32_t resid,s16_t x,s16_t y);
/*******************************************************************************
* Function Name  : stringView
* Description    : register a string view
* Input          : u32_t resid : resid
                      s16_t x : position x
                      s16_t y : position y
* Output         : 
* Return         : s32_t : <0 fail
                                      : view id
*******************************************************************************/
extern s32_t stringView(u32_t resid,s16_t x,s16_t y);
/*******************************************************************************
* Function Name  : stringView
* Description    : register a text view
* Input          : char *str : text 
                      s16_t x : position x
                      s16_t y : position y
* Output         : 
* Return         : s32_t : <0 fail
                                      : view id
*******************************************************************************/
extern s32_t textView(char *str,s16_t x,s16_t y,u8_t font);
/*******************************************************************************
* Function Name  : drawView
* Description    : register a draw view
* Input          : u8_t type : draw type
                      s16_t x : position x
                      s16_t y : position y
* Output         : 
* Return         : s32_t : <0 fail
                                      : view id
*******************************************************************************/
extern s32_t drawView(u8_t type,s16_t x,s16_t y);



#endif