#ifndef __WIN_MANAGE_H
#define __WIN_MANAGE_H
#include "mType.h"
#include "userDef.h"
#include "memPool.h"

#define uiLock()
#define uiUnlock()
typedef  uint32  resID;
#define   INVALID_RES_ID  0x7fffffff
#define   strToResId(addr) ((resID)(addr))
#define   resIdIsStr(id) (((id)&0x80000000)==0)
#define   resIdToStrAddr(id) ((char*)(id))
#define   resIdIsImage(id)   (((id)&0xff000000)==0x80000000)

#define   ALIGNMENT_LEFT  (1<<0)
#define   ALIGNMENT_CENTER  (1<<1)
#define   ALIGNMENT_RIGHT  (1<<2)
typedef  uint8 charFont;
#define   DEFAULT_FONT       0;


#define WIN_INVALID        (1<<0)
#define WIN_WIDGET         (1<<1)
#define WIN_VISIBLE        (1<<2)
#define WIN_ABS_POS        (1<<3)  // create window by absolute position
#define WIN_FRAME          (1<<4)
#define WIN_NOT_ZOOM       (1<<5)
#define WIN_TOUCH_SUPPORT  (1<<6)
#define WIN_UNI_STR         (1<<7)


typedef enum
{
	MSG_WIN_CREATE=0,
	MSG_WIN_DESTROY,
	MSG_WIN_CHILE_DESTROY,
	MSG_WIN_WIDGET_DESTROY,
	MSG_WIN_PARENT_DEAL,
	MSG_WIN_INVALID,
	MSG_WIN_PAINT,
	MSG_WIN_CHANGE_RESID,
	MSG_WIN_CHG_ITEM_SEL_RESID,
	MSG_WIN_INVALID_RESID,
	MSG_WIN_UPDATE_RESID,
	MSG_WIN_VISIBLE_SET,
	MSG_WIN_VISIBLE_GET,
	MSG_WIN_CHANGE_BG_COLOR,
	MSG_WIN_CHANGE_FG_COLOR,
	MSG_WIN_CHANGE_STRINFOR,
	MSG_WIN_PROGRESS_RATE,
	MSG_WIN_SELECT_INFOR,
	MSG_WIN_UNSELECT_INFOR,
	MSG_WIN_SELECT_INFOR_EX,
	MSG_WIN_UNSELECT_INFOR_EX,
	MSG_WIN_TOUCH,
	MSG_WIN_TOUCH_GET_INFOR,
	MSG_WIN_SELECT,
	MSG_WIN_UNSELECT,
	MSG_WIN_GET_WIDGET,
	MSG_WIN_ADD_WIDGET,
	MSG_WIDGET_GET_ID,
	MSG_WIDGET_SET_ID,
	MSG_WIDGET_NEXT,
	MSG_WIDGET_PRE,
	MSG_WIDGET_CUR,
	MSG_WIDGET_SELECT,
	MSG_WIDGET_CHANGE_ALL_RESID,
	MSG_DIALOG_INIT,
	MSG_DIALOG_CLOSE,
	MSG_MAX,
}msgId;


typedef uint8 uiColor;
#define DEFAULT_COLOR  0xfb
#define INVALID_COLOR  0xff
typedef void* winHandle;
//#define INVALID_HANDLE      0xffff
#define INVALID_HANDLE      NULL
typedef struct _uiRect
{
	int16 x0;
	int16 x1;
	int16 y0;
	int16 y1;
	struct _uiRect* next;
}uiRect;
typedef struct
{
	msgId id;
	winHandle hWin;
	winHandle hSrc;
	union
	{
		void* p;
		uint32 v;
	}parameter;
}winMsg;
typedef void (*winCB)(winMsg* msg);

typedef struct _winObj // all window object inherited winObj,
{
	uiRect rect;// window position in  screen
	uiRect invalidRect;// redraw area of the window
	uiColor bgColor;// window background color
	uint16 style;
	char* name;
	winCB cb;// window's callback function,process received message
	winHandle parent;
	winHandle child;
	winHandle next; // widget window linked here
}winObj;
#define  WIN_SIZE  sizeof(winObj)

#define TOUCH_NONE       0
#define TOUCH_PRESS      1
#define TOUCH_OVER       2
#define TOUCH_RELEASE    3

typedef struct _touchInfor
{
	uiRect      touchArea;
	winHandle  touchWin;
	winHandle  touchHandle;
	uint32      touchID;
	uint32      touchItem;
	uint32      touchState;
}touchInfor;

typedef struct _widgetCreateInfor widgetCreateInfor;
typedef winHandle(*widgetCreateFun)(widgetCreateInfor* infor,winHandle parent,winCB cb);
typedef struct _widgetCreateInfor
{
	widgetCreateFun widgetCreate;
	uint16  id;
	int16 x0;
	int16 y0;
	uint16 width;
	uint16 height;
	uint16 style;
	uiColor bgColor;
	resID  image;
	uint8 imageAlign;
	resID  str;
	uint8 strAlign;
	uiColor fontColor;
	charFont font;
	//bool  strIsASCII;
	void* private;
	
	uint8 strAlignS;
	charFont fontS;
	uiColor fontColorS;
	uiColor bgColorS;
	uiColor rimColorS;
	uiColor rimColor;
}widgetCreateInfor;

#define createFrameWin(x0,y0,width,height,bgColor,style)   										        { frameWinCreateIndirect,INVALID_WIDGET_ID,x0,y0,width,height,style,bgColor,INVALID_RES_ID,0,INVALID_RES_ID,0,INVALID_COLOR,0,NULL,0,0,INVALID_COLOR,bgColor,INVALID_COLOR,INVALID_COLOR} 
#define createItemManage(id,x0,y0,width,height,bgColor)   										        { itemManageCreateIndirect,id,x0,y0,width,height,0,bgColor,INVALID_RES_ID,0,INVALID_RES_ID,0,INVALID_COLOR,0,NULL,0,0,INVALID_COLOR,bgColor,INVALID_COLOR,INVALID_COLOR}
#define createButton(id,x0,y0,width,height,image,imageAlign,str,strAlign,fontColor,font)  { buttonCreateIndirect,id,x0,y0,width,height,0,INVALID_COLOR,image,imageAlign,str,strAlign,fontColor,font,NULL,strAlign,font,fontColor,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR}
#define createImageIcon(id,x0,y0,width,height,image,imageAlign)    				        { imageIconCreateIndirect,id,x0,y0,width,height,0,INVALID_COLOR,image,imageAlign,INVALID_RES_ID,0,INVALID_COLOR,0,NULL,0,0,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR}
#define createRect(id,x0,y0,width,height,fgColor)											{ rectCreateIndirect,id,x0,y0,width,height,0,INVALID_COLOR,INVALID_RES_ID,0,INVALID_RES_ID,0,fgColor,0,NULL,0,0,fgColor,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR}
// font: R_FONT_DEFAULT R_FONT_NUM2 R_FONT_NUM3 R_FONT_NUM4 R_FONT_NUM5
#define createStringIcon(id,x0,y0,width,height,str,strAlign,fontColor,font)                { stringIconCreateIndirect,id,x0,y0,width,height,0,INVALID_COLOR,INVALID_RES_ID,0,str,strAlign,fontColor,font,NULL,strAlign,font,fontColor,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR}
//#define createStringRim(id,x0,y0,width,height,str,strAlign,fontColor,font,rimColor)                { stringIconCreateIndirect,id,x0,y0,width,height,0,INVALID_COLOR,INVALID_RES_ID,0,str,strAlign,fontColor,font,NULL,strAlign,font,fontColor,INVALID_COLOR,INVALID_COLOR,rimColor}
#define createStringRim(id,x0,y0,width,height,str,strAlign,fontColor,font,rimColor)                { stringIconCreateIndirect,id,x0,y0,width,height,0,INVALID_COLOR,INVALID_RES_ID,0,str,strAlign,R_COLOR_WHITE,font,NULL,strAlign,font,R_COLOR_WHITE,INVALID_COLOR,INVALID_COLOR,R_COLOR_TRANSFER}//Mandatory white without border
#define createProgressBar(id,x0,y0,width,height,bgColor,fgColor)									{ progressBarCreateIndirect,id,x0,y0,width,height,0,bgColor,INVALID_RES_ID,0,INVALID_RES_ID,0,fgColor,0,NULL,0,0,fgColor,bgColor,INVALID_COLOR,INVALID_COLOR}

#define createStringItem(id,x0,y0,width,height,str,strAlign,fontColor,font,bgColor,strAlignSel,fontColorSel,fontSel,bgColorSel)     { stringIconCreateIndirect,id,x0,y0,width,height,0,bgColor,INVALID_RES_ID,0,str,strAlign,fontColor,font,NULL,strAlignSel,fontSel,fontColorSel,bgColorSel,INVALID_COLOR,INVALID_COLOR}
#define createWidgetManage(id,minID,maxID,getResFun) { widgetManageCreateIndirect,id,0,0,minID,maxID,0,0,INVALID_RES_ID,0,INVALID_RES_ID,0,INVALID_COLOR,0,getResFun,0,0,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR}    
#define createUniStrIcon(id,x0,y0,width,height,str,strAlign,fontColor,font)                { stringIconCreateIndirect,id,x0,y0,width,height,WIN_UNI_STR,INVALID_COLOR,INVALID_RES_ID,0,str,strAlign,fontColor,font,NULL,strAlign,font,fontColor,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR}

#define widgetEnd()             { NULL,INVALID_WIDGET_ID,0,0,0,0,0,0,INVALID_RES_ID,0,INVALID_RES_ID,0,INVALID_COLOR,0,NULL,0,0,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR,INVALID_COLOR}    

typedef struct
{
	charFont font;
	uint8 strAlign;
	uiColor fontColor;
	uiColor bgColor;
	uiColor rimColor;
}strInfor;
typedef struct
{
	charFont font;
	uint8 strAlign;
	uiColor fontColor;
	uiColor bgColor;
	uiColor rimColor;
	
	uiColor color;
	resID   image;
}resInfor;

void resInforInit(resInfor* infor);
bool winContain(uiRect* parent,uiRect* child);
void winSendMsg(winHandle handle,winMsg* msg);
void winSendMsgNoData(winHandle handle,msgId id);
void winSendMsgToParent(winHandle handle,winMsg* msg);
void winSendMsgToParentNoData(winHandle handle,msgId id);
void winDefaultProc(winMsg* msg);
winHandle winCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,winCB cb,uint32 size,uint16 style);
void winDestroy(winHandle* hWin);
int winTouch(uiRect* rect,uint32 touchState);
void winGetTouchInfor(winHandle hWin,touchInfor *infor);
void winSetbgColor(winHandle hWin,uiColor bgColor);
void winSetfgColor(winHandle hWin,uiColor bgColor);
void winSetPorgressRate(winHandle hWin,uint32 rate);
void winResize(winHandle hWin,int16 x0,int16 y0,int16 x1,int16 y1);
void winSetVisible(winHandle hWin,bool visible);
bool winIsVisible(winHandle hWin);
void winSetResid(winHandle hWin,resID id);
void winSetItemSelResid(winHandle hWin,resID id);
void winSetStrInfor(winHandle hWin,charFont font,uint8 strAlign,uiColor fontColor);
void winSetSelectInfor(winHandle hWin,resInfor* infor);
#define winSetInfor  winSetUnselectInfor
void winSetUnselectInfor(winHandle hWin,resInfor* infor);
void winChangeNotify(winHandle hWin);
void winUpdate(winHandle hWin);
uint8 winPaint(void);
void winGetRelativePos(winHandle hWin,uiRect* pos);
void winGetPos(winHandle hWin,uiRect* pos);
winHandle winGetCurrent(void);
void winDestroyDeskTopChildWin(void);
void winSetName(winHandle hWin,char* name);
void winInit(void);
void winUninit(void);
void winUpdateAllResId(void);
bool winOverlapping(uiRect* rect1,uiRect* rect2);
void winIntersection(uiRect* out,uiRect* win1,uiRect* win2);



void drawRectangle(uiRect* rect,uiColor color);
void drawImage(uiRect* winRect,uiRect* drawRect,resID id,uint8 alignment,uiColor bgColor);
void drawString(uiRect* winRect,uiRect* drawRect,resID id,charFont font,uint8 alignment,uiColor fontColor,uiColor bgColor,uiColor rimColor);
uint32 resGetAddrAndSize(resID id,uint16 *width,uint16 *height);

void set_ebook_font(bool enable);
bool get_ebook_font(void);



#include "uiAlloc.h"

#endif
