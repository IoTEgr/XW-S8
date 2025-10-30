/****************************************************************************
        ***           ***                      MAXLIB-GRAPHIC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX VIEW MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : view.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib VIEW managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
* 2016-09-08  : mark
*     <1>.change member of text in VIEW_T to prit,add system_view_text_pool for text use
*     <2>.prit member for different type use
******************************************************************************/

#include "./../max.h"
#include "./../maxlib.h"

//#include <stdio.h>


#define  CONFIG_VIEW_NODE_MAX      CFG_VIEW_NODE_NUM  // view node max


#define  CONFIG_TEXT_POOL_MAX      CFG_VIEW_TEXT_NUM   // text pool max
#define  TEXT_POOL_MASK_NUM         ((CONFIG_TEXT_POOL_MAX+31)>>5)
#define  TEXT_CONTENT_MAX           CFG_VIEW_TEXT_LEN   // text view text max lenght
#define  TEXT_LINE_MAX                CFG_VIEW_TEXT_LIN // text line view ,line max legth


#define  VIEW_ID_MAKE(idx)  (R_ID_TYPE_VIEW|idx)
#define  VIEW_ID_CHECK(id)  (id&R_ID_TYPE_VIEW)
#define  VIEW_ID_GET(id)    (id&R_ID_MASK)

#define  VIEW_SIZE_MAKE(w,h)  ((w<<16)|h)
#define  VIEW_SIZE_GETW(s)    (s>>16)
#define  VIEW_SIZE_GETH(s)    (s&0xffff)

#define  VIEW_CTRL_SET(p)   (1<<p)
#define  VIEW_CTRL_CLEAR(p) (~(1<<p))

static u32_t viewFindIndex=0;


static VIEW_T system_view_static_pool[CONFIG_VIEW_NODE_MAX];
static char system_view_text_pool[CONFIG_TEXT_POOL_MAX][TEXT_CONTENT_MAX];
static u32_t maskTEXT[TEXT_POOL_MASK_NUM];
//static char   system_view_mem_pool[CONFIG_VIEW_NODE_MAX][CONFIG_VIEW_MEM_MAX];
static u8_t  viewUpdateFlag=0;
/*******************************************************************************
* Function Name  : viewCheckId
* Description    : check view id
* Input          : s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                     : view idx
*******************************************************************************/
static s32_t viewCheckId(s32_t id)
{
    u32_t idx;
		
	if(VIEW_ID_CHECK(id)==0 || id<0)
	{
		//DBG_PRINT("view [CheckId] : id error <0x%x>\r\n",id);
		return -1;
	}

	idx = VIEW_ID_GET(id);
	
	if(system_view_static_pool[idx].view_stat == VIEW_STAT_IDLE)
	{
		//DBG_PRINT("view [CheckId] : stat error <%d>\r\n",system_view_static_pool[idx].view_stat);
		return -1;
	}
		
	return idx;	
}
/*******************************************************************************
* Function Name  : viewGetNode
* Description    : get view node
* Input          : s32_t id : view id
* Output         : 
* Return         : VIEW_T *
*******************************************************************************/
static VIEW_T *viewGetNode(s32_t id)
{
	u32_t idx;
	idx = VIEW_ID_GET(id);

	if(idx<CONFIG_VIEW_NODE_MAX)
		return &system_view_static_pool[idx];
	else
		return ((void *)0);
}
/*******************************************************************************
* Function Name  : viewInitOne
* Description    : initial view node
* Input          : VIEW_T *view : view node
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
static s32_t viewInitOne(VIEW_T *view)
{
	if(view == NULL)
		return -1;
	view->view_savestat      = 0;
	view->view_resid        = 0;
	view->view_stat         = VIEW_STAT_IDLE;

	view->view_type         = VIEW_TYPE_MAX;
	view->view_visiable     = 0;
	view->view_x            = 0;
	view->view_y            = 0;
	
	view->view_width        = 0;
	view->view_height       = 0;
	view->fill_color        = 0;
	view->boarder_color     = 0;
	view->prit                = NULL;
	view->view_ctrl         = 0;
    view->font                = 0;
	view->update             = 0;
	return 0;
}
/*******************************************************************************
* Function Name  : viewTextGet
* Description    : get text pool
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
static char *viewTextGet(void)
{
	u32_t i,j,mask;
	
	for(i=0;i<TEXT_POOL_MASK_NUM;i++)
	{
		if(maskTEXT[i] != 0)
			break;
		
	}
	if(i>=TEXT_POOL_MASK_NUM)
		return NULL;
	mask = 0x80000000;
	for(j=0;j<32;j++)
	{
		if(maskTEXT[i] & mask)
			break;
		mask>>=1;
	}

    mask = (i<<5)+j;
	if(mask>=CONFIG_TEXT_POOL_MAX)
		return NULL;
	maskTEXT[i] &= ~(0x80000000>>j);
	system_view_text_pool[mask][0] = 0;
	return system_view_text_pool[mask];
}
/*******************************************************************************
* Function Name  : viewTextSet
* Description    : get text pool
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
static void viewTextSet(char *text)
{
	u32_t i,j;

	for(i=0;i<CONFIG_TEXT_POOL_MAX;i++)
	{
		if(system_view_text_pool[i] == text)
			break;
	}
	if(i>=CONFIG_TEXT_POOL_MAX)
		return ;
	j = i&0x1f;
	i = i>>5;

	maskTEXT[i] |= 0x80000000>>j;
}
/*******************************************************************************
* Function Name  : viewInit
* Description    : view initial
* Input          : 
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewInit(void)
{
	u32_t i;

	for(i=0;i<CONFIG_VIEW_NODE_MAX;i++)
	{
		viewInitOne(&system_view_static_pool[i]);
	}

	for(i=0;i<TEXT_POOL_MASK_NUM;i++)
	{
		maskTEXT[i] = 0xffffffff;
	}
#if MAXLIB_DIALOG_USE >0	
	dialogInit();
#endif	
#if MAXLIB_PROGRESS_USE>0
	progressbarInit();
#endif	
#if MAXLIB_ROTATE_USE >0
    rotateInit();
#endif
#if MAXLIB_MOVE_USE > 0
    moveInit();
#endif
#if MAXLIB_SCALER_USE > 0
    scalerInit();
#endif
#if MAXLIB_MENU_USE > 0 
    menuInit();
#endif
	viewUpdateFlag = 0;

	return 0;
}
/*******************************************************************************
* Function Name  : viewUninit
* Description    : view uninitial
* Input          : 
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewUninit(void)
{
	viewInit();
	viewUpdateFlag = 0;
	
	return 0;	
}
/*******************************************************************************
* Function Name  : viewRegister
* Description    : view register
* Input          : 
* Output         : 
* Return         : s32_t -1 : fail
                                     : view id
*******************************************************************************/
s32_t viewRegister(void)
{
	u32_t i;
//	s32_t iconid;

	for(i=0;i<CONFIG_VIEW_NODE_MAX;i++)
	{
		 if(system_view_static_pool[i].view_stat == VIEW_STAT_IDLE)
		 	break;
	}

	if(i>=CONFIG_VIEW_NODE_MAX)
	{
		return -1;
	}

	viewInitOne(&system_view_static_pool[i]);
	system_view_static_pool[i].view_stat = VIEW_STAT_BUSY;

	return (VIEW_ID_MAKE(i));
}
/*******************************************************************************
* Function Name  : viewRelease
* Description    : view release
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewRelease(s32_t id)
{
	u32_t idx;
		
	if(VIEW_ID_CHECK(id)==0 || id<0)
		return -1;

	idx = VIEW_ID_GET(id);

	if(system_view_static_pool[idx].view_stat == VIEW_STAT_IDLE)
	{
		return -1;
	}
    if((system_view_static_pool[idx].view_type == VIEW_TYPE_TEXT)||\
		(system_view_static_pool[idx].view_type == VIEW_TYPE_TEXTPANEL)||\
		(system_view_static_pool[idx].view_type == VIEW_TYPE_TEXTLINE))
    {
		viewTextSet((char *)system_view_static_pool[idx].prit);
    }
#if MAXLIB_MOVE_USE >0
    if(system_view_static_pool[idx].view_ctrl&VIEW_CTRL_SET(VIEW_CTRL_MOVE))
    {
		moveStop(id);
    }
#endif
#if MAXLIB_SCALER_USE >0
    if(system_view_static_pool[idx].view_ctrl&VIEW_CTRL_SET(VIEW_CTRL_ZOOM))
    {
		scalerStop(id);
    }
#endif
#if MAXLIB_ROTATE_USE >0
    if(system_view_static_pool[idx].view_ctrl&VIEW_CTRL_SET(VIEW_CTRL_ROTATE))
    {
		rotateStop(id);
    }
#endif
	system_view_static_pool[idx].view_stat = VIEW_STAT_IDLE;
//	system_view_mem_pool[idx][0] = 0;
	viewUpdateFlag |= /*|DRAW_CLEAR*/DRAW_AL_CHECK;

	return 0;
}
/*******************************************************************************
* Function Name  : viewSetHighLayer
* Description    : set view high layer if possiable
* Input          :  s32_t id : view id
                       
* Output         : 
* Return         : s32_t -1 : fail
                       new view id
*******************************************************************************/
s32_t viewSetHighLayer(s32_t id)
{
	u32_t idx;
	s32_t view;
		
	if(VIEW_ID_CHECK(id)==0 || id<0)
		return -1;

	idx = VIEW_ID_GET(id);	

	for(view=CONFIG_VIEW_NODE_MAX-1;view>=0;view--)
	{
		 if(system_view_static_pool[view].view_stat == VIEW_STAT_IDLE)
		 	break;
	}
	if(view<0)
		return id;
	if(view<idx)
		return id;
	memcpy(&system_view_static_pool[view],&system_view_static_pool[idx],sizeof(VIEW_T));
	viewRelease(id);
	return (VIEW_ID_MAKE(view));
}
/*******************************************************************************
* Function Name  : viewSetLowLayer
* Description    : set view low layer if possiable
* Input          :  s32_t id : view id
                       
* Output         : 
* Return         : s32_t -1 : fail
                       new view id
*******************************************************************************/
s32_t viewSetLowLayer(s32_t id)
{
	u32_t idx;
	s32_t view;
		
	if(VIEW_ID_CHECK(id)==0 || id<0)
		return -1;

	idx = VIEW_ID_GET(id);	

	view = viewRegister();
	if(view<0)
		return id;
	if(idx<VIEW_ID_GET(view))
	{
		viewRelease(view);
		return id;
	}
	memcpy(&system_view_static_pool[VIEW_ID_GET(view)],&system_view_static_pool[idx],sizeof(VIEW_T));
	viewRelease(id);
	
	return view;
}
/*******************************************************************************
* Function Name  : viewSetVisiable
* Description    : set view visiable state
* Input          :  s32_t id : view id
                       u8_t visiable : visiabe state,VIEW_INVISIABLE/VIEW_VISIABLE
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetVisiable(s32_t id,u16_t visiable)
{
	s32_t idx;
//	u8_t *temp;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	if(system_view_static_pool[idx].view_savestat & VIEW_STAT_SAVE)
	{
		if((system_view_static_pool[idx].view_savestat&0x0f) == visiable)
			return 0;
		system_view_static_pool[idx].view_savestat = VIEW_STAT_SAVE|visiable;
	}
	else
	{
	    if(system_view_static_pool[idx].view_visiable == visiable)
		    return 0;
		system_view_static_pool[idx].view_visiable = (u8_t)visiable;
	}
	
	viewUpdateFlag |= DRAW_AL_CHECK;
//	if(visiable == VIEW_INVISIABLE)
//		viewUpdateFlag |= DRAW_CLEAR;
//	else
		system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);

	return 0;
}
/*******************************************************************************
* Function Name  : viewGetVisiable
* Description    : get view visiable state
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewGetVisiable(s32_t id)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;

	return system_view_static_pool[idx].view_visiable;
}

/*******************************************************************************
* Function Name  : viewSetResId
* Description    : set view resource id
* Input          :  s32_t id : view id
                       u32_t resid : resource id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetResId(s32_t id,u32_t resid)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	    
	if(system_view_static_pool[idx].view_resid != resid)
	{
		system_view_static_pool[idx].view_resid = resid;
		system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
		viewUpdateFlag |= DRAW_AL_CHECK;
	}
	
	return 0;
}
/*******************************************************************************
* Function Name  : viewGetResId
* Description    : get view resource id
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                    : resource id
*******************************************************************************/
s32_t viewGetResId(s32_t id)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;

	return system_view_static_pool[idx].view_resid;

}
/*******************************************************************************
* Function Name  : viewSetPosition
* Description    : set view position
* Input          :  s32_t id : view id
                       s16_t x : position x
                       s16_t y : posiiton y
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetPosition(s32_t id,s16_t x,s16_t y)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	
	if(system_view_static_pool[idx].view_x == x && system_view_static_pool[idx].view_y == y)
		return 0;
	
	system_view_static_pool[idx].view_x = x;
	system_view_static_pool[idx].view_y = y;
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK/*|DRAW_CLEAR*/;
	
	return 0;
}
/*******************************************************************************
* Function Name  : viewGetPosition
* Description    : get view position
* Input          :  s32_t id  : view id
                       s16_t *x : position x
                       s16_t *y : posiiton y
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewGetPosition(s32_t id,s16_t *x,s16_t *y)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;

	if(x)
		*x = system_view_static_pool[idx].view_x;
	if(y)
		*y = system_view_static_pool[idx].view_y;

	return 0;
}
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
s32_t viewSetPosition2(s32_t id,s16_t sx,s16_t sy,s16_t ex,s16_t ey)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	
//	if(system_view_static_pool[idx].view_x == sx && system_view_static_pool[idx].view_y == sy)
//		return 0;
	
	system_view_static_pool[idx].view_x = sx;
	system_view_static_pool[idx].view_y = sy;
	system_view_static_pool[idx].view_ex = ex;
	system_view_static_pool[idx].view_ey = ey;
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK/*|DRAW_CLEAR*/;
	
	return 0;	
}
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
s32_t viewGetEndPosition(s32_t id,s16_t *x,s16_t *y)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;

	if(x)
		*x = system_view_static_pool[idx].view_ex;
	if(y)
		*y = system_view_static_pool[idx].view_ey;

	return 0;
}

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
s32_t viewSetDrawPosition(s32_t id,s16_t x,s16_t y)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	
	if(system_view_static_pool[idx].draw_x == x && system_view_static_pool[idx].draw_y == y)
		return 0;
	
	system_view_static_pool[idx].draw_x = x;
	system_view_static_pool[idx].draw_y = y;
//	viewUpdateFlag = 1;
	
	return 0;
}
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
s32_t viewGetDrawPosition(s32_t id,s16_t *x,s16_t *y)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;

	if(x)
		*x = system_view_static_pool[idx].draw_x;
	if(y)
		*y = system_view_static_pool[idx].draw_y;

	return 0;
}
/*******************************************************************************
* Function Name  : viewSetType
* Description    : set view type
* Input          :  s32_t id  : view id
                       u8_t type: view type
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetType(s32_t id,u8_t type)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;

	system_view_static_pool[idx].view_type = type;

	return 0;
}
/*******************************************************************************
* Function Name  : viewGetType
* Description    : get view type
* Input          :  s32_t id  : view id
* Output         : 
* Return         : s32_t -1 : fail
                                     : view type
*******************************************************************************/
s32_t viewGetType(s32_t id)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;

	return (system_view_static_pool[idx].view_type);
}
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
s32_t viewSetText(s32_t id,char *string,u8_t size)
{
	s32_t idx,i;
	char *text;
		
	idx = viewCheckId(id);
	if(idx<0)
	{
	    return -1;
	}
	if((system_view_static_pool[idx].view_type != VIEW_TYPE_TEXT)&&\
		(system_view_static_pool[idx].view_type != VIEW_TYPE_TEXTPANEL)&&\
		(system_view_static_pool[idx].view_type != VIEW_TYPE_TEXTLINE))
	{
		//DBG_PRINT("view [SetText]: type error<0x%x>[%d]\r\n",id,system_view_static_pool[idx].view_type);
	    return -1;
	}
	if(size>TEXT_CONTENT_MAX)
	   size = TEXT_CONTENT_MAX; 
	if(system_view_static_pool[idx].prit == NULL)
		system_view_static_pool[idx].prit = (char*)viewTextGet();
	text = (char *)system_view_static_pool[idx].prit;
	if(text == NULL)
		return -1;
	for(i=0;i<size;i++)
	{
	    text[i] = string[i];
		if(string[i] == 0)
			break;
	}        
	
	text[i] = 0;  //string end
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;
	
	return 0;
}
/*******************************************************************************
* Function Name  : viewSetFont
* Description    : set view font
* Input          :  s32_t id  : view id
                       u8_t font : string font
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetFont(s32_t id,u8_t font)
{
	s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	{
	    return -1;
	}
	if((system_view_static_pool[idx].view_type != VIEW_TYPE_TEXT)&&\
		(system_view_static_pool[idx].view_type != VIEW_TYPE_TEXTPANEL)&&\
		(system_view_static_pool[idx].view_type != VIEW_TYPE_TEXTLINE))
	{
		//DBG_PRINT("view [SetText]: type error<0x%x>[%d]\r\n",id,system_view_static_pool[idx].view_type);
	    return -1;
	}

    if(system_view_static_pool[idx].font == font)
    {
        return 0;
    } 
    
    system_view_static_pool[idx].font = font;

	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;
	
	return 0;
}
/*******************************************************************************
* Function Name  : viewGetFont
* Description    : get view text
* Input          :  s32_t id  : view id
* Output         : 
* Return         : s32_t -1 : fail
                            : string font
*******************************************************************************/
u8_t viewGetFont(s32_t id)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return 0xff;
	if((system_view_static_pool[idx].view_type != VIEW_TYPE_TEXT)&&\
		(system_view_static_pool[idx].view_type != VIEW_TYPE_TEXTPANEL)&&\
		(system_view_static_pool[idx].view_type != VIEW_TYPE_TEXTLINE))
	    return 0xff;
	
	return system_view_static_pool[idx].font;
}
/*******************************************************************************
* Function Name  : viewgetText
* Description    : get view text
* Input          :  s32_t id  : view id
* Output         : 
* Return         : s32_t -1 : fail
                                     : string text
*******************************************************************************/
char *viewGetText(s32_t id)
{
    s32_t idx;
	char *text;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return NULL;
	if((system_view_static_pool[idx].view_type != VIEW_TYPE_TEXT)&&\
		(system_view_static_pool[idx].view_type != VIEW_TYPE_TEXTPANEL)&&\
		(system_view_static_pool[idx].view_type != VIEW_TYPE_TEXTLINE))
	    return NULL;
	text = (char *)system_view_static_pool[idx].prit;
	if(text == NULL)
	{
		text = R_stringGetData(system_view_static_pool[idx].view_resid);
		//system_view_static_pool[idx].prit = text;
	}
	return text;
}
/*******************************************************************************
* Function Name  : viewSetFillColor
* Description    : set view fill color
* Input          :  s32_t id  : view id
                       u32_t color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetFillColor(s32_t id,u32_t color)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	system_view_static_pool[idx].fill_color = color;
	system_view_static_pool[idx].view_ctrl |=VIEW_CTRL_SET(VIEW_CTRL_FCOLOR);
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;
	
	return 0; 
}
/*******************************************************************************
* Function Name  : viewGetFillColor
* Description    : get view fill color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewGetFillColor(s32_t id)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	    
	return system_view_static_pool[idx].fill_color;
	 
}
/*******************************************************************************
* Function Name  : viewSetBorderColor
* Description    : set view boarder color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetBorderColor(s32_t id,u32_t color)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	system_view_static_pool[idx].boarder_color = color;
	system_view_static_pool[idx].view_ctrl |=VIEW_CTRL_SET(VIEW_CTRL_BCOLOR);
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;
	
	return 0; 
}
/*******************************************************************************
* Function Name  : viewGetBoarderColor
* Description    : get view Boarder color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewGetBoarderColor(s32_t id)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	    
	return system_view_static_pool[idx].boarder_color;
	 
}
/*******************************************************************************
* Function Name  : viewSetRotate
* Description    : set view rotate degree
* Input          :  s32_t id  : view id
                       u32_t degree : rotate degree
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetRotate(s32_t id,s32_t degree)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	if(degree>=360)
		degree = degree%360;
	else if(degree<0)
	{
		degree = 360-((0-degree)%360);
	}
	system_view_static_pool[idx].rotate = degree;
	if(degree)
		system_view_static_pool[idx].view_ctrl |=VIEW_CTRL_SET(VIEW_CTRL_ROTATE);
	else
		system_view_static_pool[idx].view_ctrl &=VIEW_CTRL_CLEAR(VIEW_CTRL_ROTATE);
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);

	viewUpdateFlag |= DRAW_AL_CHECK/*|DRAW_CLEAR*/;
	
	return 0; 
}
/*******************************************************************************
* Function Name  : viewGetRotate
* Description    : get view Rotate
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewGetRotate(s32_t id)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return 0;	
	if(system_view_static_pool[idx].view_ctrl&VIEW_CTRL_SET(VIEW_CTRL_ROTATE))
		return system_view_static_pool[idx].rotate;
	else
		return 0;
}
/*******************************************************************************
* Function Name  : viewSetZoom
* Description    : set view Zoom
* Input          :  s32_t id  : view id
                       u32_t Zoom : Zoom
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetZoom(s32_t id,s32_t zoom)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	if(zoom<0)
		zoom = 100;
	if((system_view_static_pool[idx].zoom == zoom) && (system_view_static_pool[idx].view_ctrl &VIEW_CTRL_SET(VIEW_CTRL_ZOOM)))
		return 1;
	system_view_static_pool[idx].zoom = zoom;
	if(zoom!=100)
		system_view_static_pool[idx].view_ctrl |=VIEW_CTRL_SET(VIEW_CTRL_ZOOM);
	else
		system_view_static_pool[idx].view_ctrl &=VIEW_CTRL_CLEAR(VIEW_CTRL_ZOOM);
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK/*|DRAW_CLEAR*/;
	
	return 0; 
}
/*******************************************************************************
* Function Name  : viewGetZoom
* Description    : get view Zoom
* Input          :  s32_t id  : view id
                       u32_t Zoom : Zoom
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewGetZoom(s32_t id)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return 100;	
	if(system_view_static_pool[idx].view_ctrl&VIEW_CTRL_SET(VIEW_CTRL_ZOOM))
		return system_view_static_pool[idx].zoom;
	else
		return 100;
}
/*******************************************************************************
* Function Name  : viewCheckFlag
* Description    : check view  flag
* Input          :  s32_t id  : view id
                       u8_t flag : flag
* Output         : 
* Return         : s32_t 1 : flag set
                               0 : flag cleared
*******************************************************************************/
s32_t viewCheckFlag(s32_t id,u8_t flag)
{
    s32_t idx;

	if(flag>=VIEW_CTRL_MAX)
		return -1;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return 0;	
	if(system_view_static_pool[idx].view_ctrl&VIEW_CTRL_SET(flag))
		return 1;
	else
		return 0;
}
/*******************************************************************************
* Function Name  : viewSetFlag
* Description    : set view enable flag
* Input          :  s32_t id  : view id
                       u8_t flag : flag
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetFlag(s32_t id,u8_t flag)
{
    s32_t idx;

	if(flag>=VIEW_CTRL_MAX)
		return -1;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return 0;	
	system_view_static_pool[idx].view_ctrl|=VIEW_CTRL_SET(flag);

	return 0;
}
/*******************************************************************************
* Function Name  : viewClearFlag
* Description    : clear enable flag
* Input          :  s32_t id  : view id
                       u8_t flag : flag
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewClearFlag(s32_t id,u8_t flag)
{
    s32_t idx;

	if(flag>=VIEW_CTRL_MAX)
		return -1;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return 0;	
	system_view_static_pool[idx].view_ctrl&=VIEW_CTRL_CLEAR(flag);

	return 0;
}

/*******************************************************************************
* Function Name  : viewSetTransparentColor
* Description    : set view Transparent color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewSetTransparentColor(s32_t id,u32_t color)
{
    return viewSetBorderColor(id,color);
}
/*******************************************************************************
* Function Name  : viewGetTransparentColor
* Description    : get view Transparent color
* Input          :  s32_t id  : view id
                       u32_t *color : color value
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewGetTransparentColor(s32_t id)
{
    return viewGetBoarderColor(id);
}
/*******************************************************************************
* Function Name  : viewClearTransparentColor
* Description    : clear view Transparent color
* Input          :  s32_t id  : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t viewClearTransparentColor(s32_t id)
{
    s32_t idx;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
	
	system_view_static_pool[idx].view_ctrl &= VIEW_CTRL_CLEAR(VIEW_CTRL_BCOLOR);   
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;
	
	return 0; 
}
/*******************************************************************************
* Function Name  : viewGetRawDate
* Description    : set view size
* Input          :  s32_t id  : view id
                       u8_t ctrl : using way
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
u32_t viewGetRawDate(s32_t id,u8_t ctrl)
{
    s32_t idx;
#if MAXLIB_ROTATE_USE >0	
	u32_t flag;
#endif	
		
	idx = viewCheckId(id);
	if(idx<0)
	    return 0;

#if MAXLIB_ROTATE_USE >0
	flag = system_view_static_pool[idx].view_ctrl;    
	if(ctrl == VIEW_CTRL_ZOOM && (flag&VIEW_CTRL_SET(VIEW_CTRL_ROTATE))) // rotate->zoom->
	{
		return rotateGetData(id);
	}
#endif	
	return (u32_t)R_getData(system_view_static_pool[idx].view_resid);
}
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
s32_t viewSetSize(s32_t id,u16_t width,u16_t height)
{
    s32_t idx;
//    u32_t temp;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;
//	temp = width;    

	system_view_static_pool[idx].view_width = width;
	system_view_static_pool[idx].view_height= height;
	system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK/*|DRAW_CLEAR*/;
	
	return 0;     
}
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
s32_t viewGetSize(s32_t id,u16_t *width,u16_t *height)
{
    s32_t idx;
	u16_t w,h,dh,index;
	char str[132],*string;
		
	idx = viewCheckId(id);
	if(idx<0)
	    return -1;  

	if(system_view_static_pool[idx].view_type <= VIEW_TYPE_STRING)
	{
		if(system_view_static_pool[idx].view_width!=0 && system_view_static_pool[idx].view_height!=0)
		{
			w = system_view_static_pool[idx].view_width;
		    h = system_view_static_pool[idx].view_height;
		}
		else
		{
			if(system_view_static_pool[idx].view_type == VIEW_TYPE_STRING)
			{
				user_string_size_get(system_view_static_pool[idx].view_resid,&w,&h);
			}
			else if(system_view_static_pool[idx].view_type == VIEW_TYPE_TEXT)
			{
				string = (char *)system_view_static_pool[idx].prit; 
				if(string == NULL || string[0] == 0)
					R_getAsciiStringSize(R_stringGetData(system_view_static_pool[idx].view_resid),&w,&h,system_view_static_pool[idx].font);
				else
				   R_getAsciiStringSize(string,&w,&h,system_view_static_pool[idx].font);
			}
			else if(system_view_static_pool[idx].view_type == VIEW_TYPE_TEXTPANEL)
			{
				w = system_view_static_pool[idx].view_width;
		        h = system_view_static_pool[idx].view_height;				
			}
			else if(system_view_static_pool[idx].view_type == VIEW_TYPE_TEXTLINE)
			{
				string = (char *)system_view_static_pool[idx].prit;
				if(string == NULL||string[0]==0)
				{
				    string = R_stringGetData(system_view_static_pool[idx].view_resid);
					//system_view_static_pool[idx].prit = string;
				}
				w = 0;h = 0;index = 0;dh=0;

				if(string!=NULL)
				{
					while(1)
					{
						if(*string == '\n' || index == 128 || *string == 0)
						{
							str[index] = 0;
							R_getAsciiStringSize(str,&w,&h,system_view_static_pool[idx].font);
							dh+=h;
							index = 0;
							string++;
							if(*string == 0)
							{
								h = dh;
								break;
							}
						}
						str[index++] = *string++;
					}
				}
			}
		}
	}
	else if(system_view_static_pool[idx].view_type <= VIEW_TYPE_BUTTON)
	{
		

		if(system_view_static_pool[idx].view_width==0 || system_view_static_pool[idx].view_height==0)
		{
			R_getSize(system_view_static_pool[idx].view_resid,&system_view_static_pool[idx].view_width,&system_view_static_pool[idx].view_height);
		}

		w = system_view_static_pool[idx].view_width;
		h = system_view_static_pool[idx].view_height;
	}
	else
	{
		w = system_view_static_pool[idx].view_width;
		h = system_view_static_pool[idx].view_height;
	}
	    
	if(width)
	   *width = w;
	if(height)
	   *height= h;  
	
	return 0;   
}
/*******************************************************************************
* Function Name  : viewFindFirst
* Description    : find first view
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                     : view id
*******************************************************************************/
s32_t viewFindFirst(void)
{
    viewFindIndex = 0;
    
    return viewFindNext();
}

/*******************************************************************************
* Function Name  : viewFindEnd
* Description    : find end view
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                     : 
*******************************************************************************/
s32_t viewFindEnd(void)
{
    viewFindIndex = 0;
    
    return 0;
}

/*******************************************************************************
* Function Name  : viewFindNext
* Description    : find next view
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                     : view id
*******************************************************************************/
s32_t viewFindNext(void)
{
    s32_t i;
    
    for(i=viewFindIndex;i<CONFIG_VIEW_NODE_MAX;i++)
    {
         if((system_view_static_pool[i].view_stat != VIEW_STAT_IDLE) && system_view_static_pool[i].view_visiable)
             break;
    }
    
    viewFindIndex = i+1;
    
    if(i<CONFIG_VIEW_NODE_MAX)
    {
        return (VIEW_ID_MAKE(i)); 
    }
    
    return -1;
}
/*******************************************************************************
* Function Name  : viewSetSave
* Description    : save view state
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
void viewSetSave(s32_t id)
{
	s32_t idx;

	idx = viewCheckId(id);
	if(idx<0)
	    return ;  
    
     if(system_view_static_pool[idx].view_stat != VIEW_STAT_IDLE)
     {
	 	system_view_static_pool[idx].view_savestat = system_view_static_pool[idx].view_visiable | VIEW_STAT_SAVE;
		system_view_static_pool[idx].view_visiable = 0;
     }

	 system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK/*|DRAW_CLEAR*/;
}
/*******************************************************************************
* Function Name  : viewSetRestore
* Description    : restore view state
* Input          :  s32_t id : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
void viewSetRestore(s32_t id)
{
	s32_t idx;

	idx = viewCheckId(id);
	if(idx<0)
	    return ;
    
     if((system_view_static_pool[idx].view_stat != VIEW_STAT_IDLE) && (system_view_static_pool[idx].view_savestat&VIEW_STAT_SAVE))
     {
	 	system_view_static_pool[idx].view_visiable = system_view_static_pool[idx].view_savestat&0x0f;
		system_view_static_pool[idx].view_savestat = 0;
		system_view_static_pool[idx].view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	    viewUpdateFlag |= DRAW_AL_CHECK/*|DRAW_CLEAR*/;
     }
	 
}
/*******************************************************************************
* Function Name  : viewGetUpdateFlag
* Description    : view get update flag
* Input          :  
* Output         : 
* Return         : s32_t : flag
*******************************************************************************/
s32_t viewGetUpdateFlag(void)
{
#if MAXLIB_MOVE_USE >0
		moveService();
#endif	
	return viewUpdateFlag;
}
/*******************************************************************************
* Function Name  : viewSetUpdateFlag
* Description    : view set update flag
* Input          :  u8_t uflag : flag
* Output         : 
* Return         : s32_t : 0
*******************************************************************************/
s32_t viewSetUpdateFlag(u8_t uflag)
{
	viewUpdateFlag = uflag;

	return 0;
}
/*******************************************************************************
* Function Name  : viewRegisterByLayer
* Description    : register view at the layer
* Input          :  u8_t layer : layer
* Output         : 
* Return         : s32_t : 0
*******************************************************************************/
s32_t viewRegisterByLayer(u8_t layer)
{
	u32_t idx;
	u32_t i;

	if(layer>=VIEW_LAYER_MAX)
		return -1;

	idx = (CONFIG_VIEW_NODE_MAX/(layer+1))*layer;

	for(i=idx;i<CONFIG_VIEW_NODE_MAX;i++)
	{
		 if(system_view_static_pool[i].view_stat == VIEW_STAT_IDLE)
		 	break;
	}

	if(i>=CONFIG_VIEW_NODE_MAX)
	{
		return -1;
	}

	viewInitOne(&system_view_static_pool[i]);
	system_view_static_pool[i].view_stat = VIEW_STAT_BUSY;

	return (VIEW_ID_MAKE(i));
}
/*******************************************************************************
* Function Name  : viewPrint
* Description    : print view node
* Input          : u32_t viewId : view id
* Output         : 
* Return         : 
*******************************************************************************/
void viewPrint(u32_t viewId)
{
/*    s32_t idx;
	VIEW_T *view;

	idx = viewCheckId(viewId);
	if(idx<0)
	    return ;	
    view = &system_view_static_pool[idx];*/
//	mprintf("view :idx ,visiable,width,height,x,  y, type,resid\r\n");
//	mprintf("      %d,  %d,      %d,   %d,    %d,%d, %d,%d\r\n",idx,view->view_visiable,view->view_width,view->view_height,view->view_x,view->view_y,view->view_type,view->view_resid);
}
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
s32_t imageView(u32_t resid,s16_t x,INT16 y)
{
	s32_t viewid;
	VIEW_T *view;
	
    viewid = viewRegister();
	if(viewid<0)
		return -1;
	view = viewGetNode(viewid);
	if(view == NULL)
	{
		viewRelease(viewid);
		return -2;
	}
	view->view_type = VIEW_TYPE_IMAGE;
	view->view_resid = resid;
	view->view_x = x;
	view->view_y = y;
	view->view_visiable = VIEW_VISIABLE;
	view->view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;

	return viewid;
}
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
s32_t stringView(u32_t resid,s16_t x,INT16 y)
{
	s32_t viewid;
	VIEW_T *view;
	
    viewid = viewRegister();
	if(viewid<0)
		return -1;
	view = viewGetNode(viewid);
	if(view == NULL)
	{
		viewRelease(viewid);
		return -2;
	}
	view->view_type = VIEW_TYPE_STRING;
	view->view_resid = resid;
	view->view_x = x;
	view->view_y = y;
	view->view_visiable = VIEW_VISIABLE;
	view->view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;

	return viewid;
}
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
s32_t textView(char *str,s16_t x,INT16 y,u8_t font)
{
	s32_t viewid,i;
	VIEW_T *view;
	char *text;
	
    viewid = viewRegister();
	if(viewid<0)
		return -1;
	view = viewGetNode(viewid);
	if(view == NULL)
	{
		viewRelease(viewid);
		return -2;
	}
	view->view_type = VIEW_TYPE_TEXT;
	view->view_x = x;
	view->view_y = y;
	view->font   = font;
	view->view_visiable = VIEW_VISIABLE;
	view->view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;

    i = 0;
	if(str!=NULL)
	{
		text = viewTextGet();
		if(text == NULL)
			return viewid;
		view->prit = text;
		for(i=0;i<TEXT_CONTENT_MAX;i++)
		{
			if(str[i] == 0)
				break;
		    text[i] = str[i];
			
		}
		text[i] = 0;  //string end
	}	

	return viewid;
}
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
s32_t drawView(u8_t type,s16_t x,s16_t y)
{
	s32_t viewid;
	VIEW_T *view;

	if(type<VIEW_TYPE_DRAWRECT || type>VIEW_TYPE_DRAWLINE)
		return -1;
	
    viewid = viewRegister();
	if(viewid<0)
		return -1;
	view = viewGetNode(viewid);
	if(view == NULL)
	{
		viewRelease(viewid);
		return -2;
	}
	view->view_type = type;
	view->view_x = x;
	view->view_y = y;
	view->view_visiable = VIEW_VISIABLE;
	view->view_ctrl |= VIEW_CTRL_SET(VIEW_CTRL_UPDATE);
	viewUpdateFlag |= DRAW_AL_CHECK;  
	
	return viewid;	
}
