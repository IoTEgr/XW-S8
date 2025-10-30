#include "winManage.h"

typedef struct
{
	uint32 winSum;
	uint32 invalidWinSum;
	winHandle deskTopWin;
	winHandle curWin;
	uint8 destroyWin;
	bool mempError;
	uiRect errTipRect;
}uiState;
#define DEFAULT_STYLE  (WIN_INVALID|WIN_VISIBLE)
static uiState state;
static memPool rectMemp;

void winErrShow(resID id)
{
	drawString(&(state.errTipRect),&(state.errTipRect),id,0x02,ALIGNMENT_CENTER,0xfe,0xf8,INVALID_COLOR);
}

/*
send message to a window
*/
void winSendMsg(winHandle handle,winMsg* msg)
{
	winObj* pWin;
	uiLock();
	if(handle!=INVALID_HANDLE)
	{	
		pWin=(winObj*)uiHandleToPtr(handle);
		if(pWin==NULL||pWin->cb==NULL)
		{
			uiUnlock();
			return;
		}
		msg->hWin=handle;
		(*pWin->cb)(msg);
	}
	uiUnlock();
}
void winSendMsgNoData(winHandle handle,msgId id)
{
	winMsg msg;
	msg.id=id;
	winSendMsg(handle,&msg);
}
void winSendMsgToParent(winHandle handle,winMsg* msg)
{
	winObj* pWin;
	uiLock();
	if(handle!=INVALID_HANDLE)
	{
		msg->hSrc=handle;
		pWin=(winObj*)uiHandleToPtr(handle);
		winSendMsg(pWin->parent,msg);
	}
	uiUnlock();
}
void winSendMsgToParentNoData(winHandle handle,msgId id)
{
	winMsg msg;
	msg.id=id;
	winSendMsgToParent(handle,&msg);
}
/*
determine whether the two windows overlap
*/
bool winOverlapping(uiRect* rect1,uiRect* rect2)
{
	if(rect1->y1<rect2->y0||
	    rect1->x1<rect2->x0||
	    rect1->x0>rect2->x1||
	    rect1->y0>rect2->y1)
	    return false;
	return true;
}
/*
determine if two windows completely contain
*/
bool winContain(uiRect* parent,uiRect* child)
{
	if(parent->x0<=child->x0&&
	    parent->y0<=child->y0&&
	    parent->x1>=child->x1&&
	    parent->y1>=child->y1)
	    return true;
	return false;
}
bool winHasInvalidRect(uiRect* win,uiRect* invalidRect)
{
	while(win)
	{
		if(winContain(win,invalidRect))
		{
			return true;
		}
		else if(winContain(invalidRect,win))
		{
			win->x0=invalidRect->x0;
			win->x1=invalidRect->x1;
			win->y0=invalidRect->y0;
			win->y1=invalidRect->y1;
			return true;
		}
		win=win->next;
	}
	return false;
}
#define  winAllocRect()     (uiRect*)mempGet(&rectMemp)
void winFreeInvalidRect(winObj* pWin)
{
	uiRect* rect=pWin->invalidRect.next;
	while(rect)
	{
		mempPut(&rectMemp,rect);
		rect=rect->next;
	}
	pWin->invalidRect.next=NULL;
}
/* 
set invalid area of window, winPaint() function will redraw this invalid area
*/
static void winSetInvalid(winHandle hWin,uiRect* invalidRect)
{
	winObj* pWin;
//	winMsg msg;
	uiRect curInvalidRect;
	uiRect* rect;
	if(hWin==INVALID_HANDLE)
		return;
	pWin=(winObj*)uiHandleToPtr(hWin);
	curInvalidRect.x0=MAX(invalidRect->x0,pWin->rect.x0);
	curInvalidRect.x1=MIN(invalidRect->x1,pWin->rect.x1);
	curInvalidRect.y0=MAX(invalidRect->y0,pWin->rect.y0);
	curInvalidRect.y1=MIN(invalidRect->y1,pWin->rect.y1);
	if(pWin->style&WIN_INVALID)
	{
		if(winHasInvalidRect(&(pWin->invalidRect),&curInvalidRect))
			return;
		rect=winAllocRect();
		if(rect==NULL)
			return;
		rect->x0=curInvalidRect.x0;
		rect->x1=curInvalidRect.x1;
		rect->y0=curInvalidRect.y0;
		rect->y1=curInvalidRect.y1;
		rect->next=pWin->invalidRect.next;
		pWin->invalidRect.next=rect;
		return;
	}
	pWin->style|=WIN_INVALID;
	pWin->invalidRect.x0=curInvalidRect.x0;
	pWin->invalidRect.x1=curInvalidRect.x1;
	pWin->invalidRect.y0=curInvalidRect.y0;
	pWin->invalidRect.y1=curInvalidRect.y1;
	pWin->invalidRect.next=NULL;
	state.invalidWinSum++;
}
/*
all window's callback function must call this function, to handle basic messages 
*/
void winDefaultProc(winMsg* msg) 
{
	winHandle hWin=msg->hWin;
	uiRect* invalidRect;
	winObj* pWin=(winObj*)uiHandleToPtr(hWin);
	winHandle hTemp;
	switch(msg->id)
	{
		case MSG_WIN_DESTROY:
			winSendMsg(pWin->child,msg);
			if(pWin->parent!=INVALID_HANDLE)
				((winObj*)uiHandleToPtr(pWin->parent))->child=pWin->child;
			invalidRect=(uiRect*)msg->parameter.p;
			if(invalidRect->x0>pWin->rect.x0)
				invalidRect->x0=pWin->rect.x0;
			if(invalidRect->y0>pWin->rect.y0)
				invalidRect->y0=pWin->rect.y0;
			if(invalidRect->x1<pWin->rect.x1)
				invalidRect->x1=pWin->rect.x1;
			if(invalidRect->y1<pWin->rect.y1)
				invalidRect->y1=pWin->rect.y1;
			//winSendMsg(hWin,&msg);
			state.curWin=hWin;
			//winSendMsgToParentNoData(hWin,MSG_WIN_CHILE_DESTROY);
			winSendMsgNoData(hWin,MSG_DIALOG_CLOSE);
			while(hWin!=INVALID_HANDLE)
			{
				state.winSum--;
				pWin=(winObj*)uiHandleToPtr(hWin);
				hTemp=pWin->next;
				//if(pWin->name)
				//	debug_msg("destroyed [%s] %d,style:0x%x\n",pWin->name,state.winSum,pWin->style);
				//else
				//debug_msg("destroyed %d,style:0x%x\n",state.winSum,pWin->style);
				winFreeInvalidRect(pWin);
				uiFree(hWin);
				hWin=hTemp;
			}
			break;
		case MSG_WIN_INVALID:
			invalidRect=(uiRect*)msg->parameter.p;
			hTemp=hWin;
			if(winOverlapping(&(pWin->rect), invalidRect))
			{
				while(hWin!=INVALID_HANDLE)
				{
					pWin=(winObj*)uiHandleToPtr(hWin);
					if(pWin->style&WIN_VISIBLE)
						if(winOverlapping(&(pWin->rect), invalidRect))
							winSetInvalid(hWin, invalidRect);
					hWin=pWin->next;
				}
				pWin=(winObj*)uiHandleToPtr(hTemp);
				if(pWin->bgColor!=INVALID_COLOR)
					if(winContain(&(pWin->rect),invalidRect))
						break;
				winSendMsg(pWin->parent,msg);
				break;
			}
			winSendMsg(pWin->parent,msg);
			break;
		case MSG_WIN_VISIBLE_SET:
			if(msg->parameter.v)
				pWin->style|=WIN_VISIBLE;
			else
				pWin->style&=(~WIN_VISIBLE);
			winChangeNotify(hWin);
			break;
		case MSG_WIN_VISIBLE_GET:
			if(pWin->style&WIN_VISIBLE)
				msg->parameter.v=1;
			else
				msg->parameter.v=0;
			break;
		case MSG_WIN_CHANGE_BG_COLOR:
			pWin->bgColor=msg->parameter.v;
			break;
		case MSG_WIN_TOUCH:
			winSendMsgToParent(hWin,msg);
			break;
		default:
			break;
			//debug_msg("msg [%d] was ignored!!!\n",msg->id);
	}
}
/*
desktop window's callback function
*/
static void deskTopWinCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	winObj* pWin=(winObj*)uiHandleToPtr(hWin);
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			break;
		case MSG_WIN_PAINT:
			//drawRectangle((uiRect*)(msg->parameter.p), pWin->bgColor);
			debug_msg("paint [deskTop win]:[%d %d %d %d]\n",pWin->invalidRect.x0,pWin->invalidRect.y0,pWin->invalidRect.x1,pWin->invalidRect.y1);
			(void)pWin;
			break;
		case MSG_WIN_TOUCH:
			return;
		case MSG_WIN_TOUCH_GET_INFOR:
			return;
		default:
			winDefaultProc(msg);
			break;
	}
}
/*
create window or widget
*/
winHandle winCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,winCB cb,uint32 size,uint16 style)
{
	winHandle hWin;
	winObj* pWin=NULL;
	winObj* pParentWin=NULL;
	winObj* pTemp;
	if(state.destroyWin)
	{
		while(1)
		{
			debug_msg("creating a window is not allowed when window is being destroyed!!!\n\n");
			debug_msg("do not create window in message [SYS_CLOSE_WINDOW]\n");
		}
	}
	if((style&WIN_NOT_ZOOM)==0)
	{
		x0=((x0)*LCD_WIDTH/OSD_WIDTH);
		y0=((y0)*LCD_HEIGHT/OSD_HEIGHT);
	    width=((width)*LCD_WIDTH/OSD_WIDTH);
		height=((height)*LCD_HEIGHT/OSD_HEIGHT);
	}
	uiLock();
	if(parent==INVALID_HANDLE)
		parent=state.curWin;
	if(parent!=INVALID_HANDLE)
	{
		pParentWin=(winObj*)uiHandleToPtr(parent);
		if((style&WIN_ABS_POS)==0)
		{
			x0+=pParentWin->rect.x0;
			y0+=pParentWin->rect.y0;
		}
		if(width==0)
			width=pParentWin->rect.x1-pParentWin->rect.x0+1;
		if(height==0)
			height=pParentWin->rect.y1-pParentWin->rect.y0+1;
		
	}
	hWin=uiAlloc(size);
	if(hWin==INVALID_HANDLE)
	{
		uiUnlock();
		debug_err("uiAlloc no memery!!!\n");
		return INVALID_HANDLE;
	}
	//debug_msg("create window:%d\n",state.winSum);
	state.winSum++;
	pWin=(winObj*)uiHandleToPtr(hWin);
	pWin->invalidRect.x0=pWin->rect.x0=CLIP(x0,0,LCD_WIDTH-1);
	pWin->invalidRect.x1=pWin->rect.x1=CLIP(x0+width-1,0,LCD_WIDTH-1);
	pWin->invalidRect.y0=pWin->rect.y0=CLIP(y0,0,LCD_HEIGHT-1);
	pWin->invalidRect.y1=pWin->rect.y1=CLIP(y0+height-1,0,LCD_HEIGHT-1);
	pWin->invalidRect.next=NULL;
	pWin->bgColor=INVALID_COLOR;
	pWin->name=NULL;
	pWin->cb=cb;
	pWin->style=DEFAULT_STYLE|style;
	pWin->parent=parent;
	pWin->child=INVALID_HANDLE;
	pWin->next=INVALID_HANDLE;
	if(pParentWin)
	{
		if(style&WIN_WIDGET)
		{
			pTemp=pParentWin;
			while(pTemp->next!=INVALID_HANDLE)	
				pTemp=(winObj*)uiHandleToPtr(pTemp->next);
			pTemp->next=hWin;
			pWin->invalidRect.x0=pWin->rect.x0=CLIP(x0,pParentWin->rect.x0,pParentWin->rect.x1);
			pWin->invalidRect.x1=pWin->rect.x1=CLIP(x0+width-1,pParentWin->rect.x0,pParentWin->rect.x1);
			pWin->invalidRect.y0=pWin->rect.y0=CLIP(y0,pParentWin->rect.y0,pParentWin->rect.y1);
			pWin->invalidRect.y1=pWin->rect.y1=CLIP(y0+height-1,pParentWin->rect.y0,pParentWin->rect.y1);
			pWin->bgColor=pParentWin->bgColor;
		}
		else
		{
			pWin->child=pParentWin->child;
			pParentWin->child=hWin;
		}
	}
	if((style&WIN_WIDGET)==0)
		state.curWin=hWin;
	state.invalidWinSum++;
	winSendMsgNoData(hWin,MSG_WIN_CREATE);
	uiUnlock();
	return hWin;
}
/*
destroy window or widget
*/
void winDestroy(winHandle* hWin)
{
	winMsg msg;
	uiRect invalidRect;
	winObj* pWin;
	winHandle hParent;
	if(state.destroyWin)
	{
		while(1)
		{
			debug_msg("destroy a window is not allowed when window is being destroyed!!!\n\n");
			debug_msg("do not destroy window in message [SYS_CLOSE_WINDOW]\n");
		}
	}
	uiLock();
	if(*hWin==INVALID_HANDLE)
	{
		uiUnlock();
		return;
	}
	pWin=(winObj*)uiHandleToPtr(*hWin);
	invalidRect.x0=pWin->rect.x0;
	invalidRect.x1=pWin->rect.x1;
	invalidRect.y0=pWin->rect.y0;
	invalidRect.y1=pWin->rect.y1;
	hParent=pWin->parent;  
	if(pWin->style&WIN_WIDGET)
	{
		pWin=(winObj*)uiHandleToPtr(pWin->parent);
		while(pWin!=NULL)
		{
			if(pWin->next==*hWin)
				break;
			pWin=(winObj*)uiHandleToPtr(pWin->next);
		}
		if(pWin==NULL)
		{
			debug_err("can not find this widget\n");
			uiUnlock();
			return;
		}
		pWin->next=((winObj*)uiHandleToPtr(*hWin))->next;
		state.winSum--;
		debug_msg("destroyed %d\n",state.winSum);
		winSendMsgToParentNoData(*hWin,MSG_WIN_WIDGET_DESTROY);
		winFreeInvalidRect((winObj*)uiHandleToPtr(*hWin));
		uiFree(*hWin);	
	}
	else
	{
		msg.id=MSG_WIN_DESTROY;
		msg.parameter.p=(void*)&invalidRect;
		state.destroyWin=1;
		winSendMsg(*hWin,&msg);
		state.curWin=hParent;
		state.destroyWin=0;
/*
Processing In the recording mode, when the menu is closed, 
the double buff is switched to the single buff because the childcolse message is sent first.
After sending the wininvalid message to redraw the window, all windows are drawn in the 
lcd display buff, a splash screen will appear
*/		
		msg.id=MSG_WIN_INVALID;
		msg.parameter.p=(void*)&invalidRect;
		winSendMsg(hParent,&msg);
		*hWin=INVALID_HANDLE;
		
		winSendMsgNoData(hParent,MSG_WIN_CHILE_DESTROY);
		uiUnlock();
		return;
	}
	msg.id=MSG_WIN_INVALID;
	msg.parameter.p=(void*)&invalidRect;
	winSendMsg(hParent,&msg);
	*hWin=INVALID_HANDLE;
	uiUnlock();
}
/*
find which window the rectangular area belongs to,and send MSG_WIN_TOUCH
*/
int winTouch(uiRect* rect,uint32 touchState)
{
	static touchInfor lastInfor={{0,0,0,0},INVALID_HANDLE,INVALID_HANDLE,0,0,TOUCH_NONE};
	winHandle hWin;
	winObj* pWin;
	winMsg msg;
	touchInfor infor;
	if(rect==NULL||state.curWin==INVALID_HANDLE)
	{
		if(state.curWin!=lastInfor.touchWin||lastInfor.touchWin==INVALID_HANDLE||touchState==TOUCH_OVER)
		{
			lastInfor.touchHandle=lastInfor.touchWin=INVALID_HANDLE;
			return 0;
		}
		lastInfor.touchState=TOUCH_RELEASE;
		msg.id=MSG_WIN_TOUCH;
		msg.parameter.p=(void*)&lastInfor;
		winSendMsg(lastInfor.touchHandle,&msg);
		lastInfor.touchHandle=lastInfor.touchWin=INVALID_HANDLE;
		return 1;
	}
	pWin=(winObj*)uiHandleToPtr(state.curWin);
	if(winContain(&(pWin->rect),rect)==false)
	{
		lastInfor.touchHandle=lastInfor.touchWin=INVALID_HANDLE;
		return 0;
	}
	infor.touchArea.x0=rect->x0;
	infor.touchArea.x1=rect->x1;
	infor.touchArea.y0=rect->y0;
	infor.touchArea.y1=rect->y1;
	infor.touchWin=INVALID_HANDLE;
	hWin=pWin->next;
	while(hWin!=INVALID_HANDLE)
	{
		pWin=(winObj*)uiHandleToPtr(hWin);
		if(pWin->style&WIN_TOUCH_SUPPORT)
			if(winContain(&(pWin->rect),rect))
				winGetTouchInfor(hWin,&infor);
		hWin=pWin->next;
	}
	if(infor.touchWin==INVALID_HANDLE)
	{
		lastInfor.touchHandle=lastInfor.touchWin=INVALID_HANDLE;
		return 0;
	}
	if(infor.touchWin==lastInfor.touchWin
		&&infor.touchID==lastInfor.touchID
		&&infor.touchItem==lastInfor.touchItem)
	{
		return 1;
	}
	lastInfor.touchState=TOUCH_OVER;
	msg.id=MSG_WIN_TOUCH;
	msg.parameter.p=(void*)&lastInfor;
	winSendMsg(lastInfor.touchHandle,&msg);
	infor.touchState=TOUCH_PRESS;
	msg.id=MSG_WIN_TOUCH;
	msg.parameter.p=(void*)&infor;
	winSendMsg(infor.touchHandle,&msg);
	lastInfor.touchWin=infor.touchWin;
	lastInfor.touchHandle=infor.touchHandle;
	lastInfor.touchID=infor.touchID;
	lastInfor.touchItem=infor.touchItem;
	lastInfor.touchState=infor.touchState;
	return 1;
}

void winGetTouchInfor(winHandle hWin,touchInfor *infor)
{
	winMsg msg;
	msg.id=MSG_WIN_TOUCH_GET_INFOR;
	msg.parameter.p=infor;
	winSendMsg(hWin,&msg);
}

void winSetbgColor(winHandle hWin,uiColor bgColor)
{
	winMsg msg;
	msg.id=MSG_WIN_CHANGE_BG_COLOR;
	msg.parameter.v=bgColor;
	winSendMsg(hWin,&msg);
}
void winSetfgColor(winHandle hWin,uiColor bgColor)
{
	winMsg msg;
	msg.id=MSG_WIN_CHANGE_FG_COLOR;
	msg.parameter.v=bgColor;
	winSendMsg(hWin,&msg);
}

void winResize(winHandle hWin,int16 x0,int16 y0,int16 x1,int16 y1)
{
	winObj* pWin;
	winObj* pParentWin;
	uiLock();
	if(hWin==INVALID_HANDLE)
	{
		uiUnlock();
		return;
	}
	pWin=(winObj*)uiHandleToPtr(hWin);
	if((pWin->style&WIN_WIDGET)==0)
	{
		uiUnlock();
		return;
	}
	winChangeNotify(hWin);
	winFreeInvalidRect(pWin);
	pParentWin=(winObj*)uiHandleToPtr(pWin->parent);
	pWin->invalidRect.x0=pWin->rect.x0=CLIP(x0,pParentWin->rect.x0,pParentWin->rect.x1);
	pWin->invalidRect.x1=pWin->rect.x1=CLIP(x1,pParentWin->rect.x0,pParentWin->rect.x1);
	pWin->invalidRect.y0=pWin->rect.y0=CLIP(y0,pParentWin->rect.y0,pParentWin->rect.y1);
	pWin->invalidRect.y1=pWin->rect.y1=CLIP(y1,pParentWin->rect.y0,pParentWin->rect.y1);
	uiUnlock();
	winChangeNotify(hWin);
}

/*
show or hide a widget
*/
void winSetVisible(winHandle hWin,bool visible)
{
	winMsg msg;
	msg.id=MSG_WIN_VISIBLE_GET;
	winSendMsg(hWin,&msg);
	if(visible)
	{
		if(msg.parameter.v)
			return;
		msg.parameter.v=1;
	}
	else
	{
		if(msg.parameter.v==0)
			return;
		msg.parameter.v=0;		
	}
	msg.id=MSG_WIN_VISIBLE_SET;
	winSendMsg(hWin,&msg);
}
bool winIsVisible(winHandle hWin)
{
	winMsg msg;
	msg.id=MSG_WIN_VISIBLE_GET;
	winSendMsg(hWin,&msg);
	if(msg.parameter.v)
		return true;
	return false;
}
void winSetResid(winHandle hWin,resID id)
{
	winMsg msg;
	msg.id=MSG_WIN_CHANGE_RESID;
	msg.parameter.v=id;
	winSendMsg(hWin,&msg);
}
void winSetItemSelResid(winHandle hWin,resID id)
{
	winMsg msg;
	msg.id=MSG_WIN_CHG_ITEM_SEL_RESID;
	msg.parameter.v=id;
	winSendMsg(hWin,&msg);
}
void winSetStrInfor(winHandle hWin,charFont font,uint8 strAlign,uiColor fontColor)
{
	strInfor infor;
	winMsg msg;
	infor.font=font;
	infor.strAlign=strAlign;
	infor.fontColor=fontColor;
	infor.rimColor=INVALID_COLOR;
	msg.id=MSG_WIN_CHANGE_STRINFOR;
	msg.parameter.p=&infor;
	winSendMsg(hWin,&msg);
}
void resInforInit(resInfor* infor)
{
	infor->font=0;
	infor->strAlign=ALIGNMENT_CENTER;
	infor->fontColor=INVALID_COLOR;
	infor->bgColor=INVALID_COLOR;
	infor->rimColor=INVALID_COLOR;
	infor->color=INVALID_COLOR;
	infor->image=INVALID_RES_ID;
}
void winSetSelectInfor(winHandle hWin,resInfor* infor)
{
	winMsg msg;
	msg.id=MSG_WIN_SELECT_INFOR_EX;
	msg.parameter.p=infor;
	winSendMsg(hWin,&msg);
}
void winSetUnselectInfor(winHandle hWin,resInfor* infor)
{
	winMsg msg;
	msg.id=MSG_WIN_UNSELECT_INFOR_EX;
	msg.parameter.p=infor;
	winSendMsg(hWin,&msg);
}

void winChangeNotify(winHandle hWin)
{
	winObj* pWin;
	uiRect invalidRect;
	winMsg msg;
	uiLock();
	if(hWin==INVALID_HANDLE)
	{
		uiUnlock();
		return;
	}
	pWin=(winObj*)uiHandleToPtr(hWin);
	invalidRect.x0=pWin->rect.x0;
	invalidRect.x1=pWin->rect.x1;
	invalidRect.y0=pWin->rect.y0;
	invalidRect.y1=pWin->rect.y1;
	msg.id=MSG_WIN_INVALID;
	msg.parameter.p=(void*)&invalidRect;
	winSendMsg(pWin->parent,&msg);
	uiUnlock();
}
void winGetRelativePos(winHandle hWin,uiRect* pos)
{
	winObj* pWin;
	winObj* pParenWin;
	uiLock();
	if(hWin==INVALID_HANDLE)
	{
		uiUnlock();
		return;
	}
	pWin=(winObj*)uiHandleToPtr(hWin);
	if(pWin->parent==INVALID_HANDLE)
	{
		pos->x0=pWin->rect.x0;
		pos->x1=pWin->rect.x1;
		pos->y0=pWin->rect.y0;
		pos->y1=pWin->rect.y1;
		uiUnlock();
		return;
	}
	pParenWin=(winObj*)uiHandleToPtr(pWin->parent);
	pos->x0=pWin->rect.x0-pParenWin->rect.x0;
	pos->x1=pWin->rect.x1-pParenWin->rect.x0;
	pos->y0=pWin->rect.y0-pParenWin->rect.y0;
	pos->y1=pWin->rect.y1-pParenWin->rect.y0;
	uiUnlock();
}
void winGetPos(winHandle hWin,uiRect* pos)
{
	winObj* pWin;
	uiLock();
	if(hWin==INVALID_HANDLE)
	{
		uiUnlock();
		return;
	}
	pWin=(winObj*)uiHandleToPtr(hWin);
	pos->x0=pWin->rect.x0;
	pos->x1=pWin->rect.x1;
	pos->y0=pWin->rect.y0;
	pos->y1=pWin->rect.y1;
	uiUnlock();
}
void winUpdate(winHandle hWin)
{
	winObj* pWin;
	uiLock();
	if(hWin==INVALID_HANDLE)
	{
		uiUnlock();
		return;
	}
	pWin=(winObj*)uiHandleToPtr(hWin);
	if((pWin->style&WIN_INVALID)==0)
		state.invalidWinSum++;
	pWin->style|=WIN_INVALID;
	pWin->invalidRect.x0=pWin->rect.x0;
	pWin->invalidRect.x1=pWin->rect.x1;
	pWin->invalidRect.y0=pWin->rect.y0;
	pWin->invalidRect.y1=pWin->rect.y1;
	uiUnlock();
}
void winSetPorgressRate(winHandle hWin,uint32 rate)
{
	winMsg msg;
	msg.id=MSG_WIN_PROGRESS_RATE;
	msg.parameter.v=rate;
	winSendMsg(hWin,&msg);
}

winHandle winGetCurrent(void)
{
	return state.curWin;
}
static void winPaint1(winHandle hWin,uiRect* childRect)
{
	winMsg msg;
	winObj* pWin;
	uiRect invalidRect;
	uiRect* rect;
	msg.id=MSG_WIN_PAINT;
	while(state.invalidWinSum)
	{
		pWin=(winObj*)uiHandleToPtr(hWin);
		if(pWin==NULL)
			break;
		if(pWin->style&WIN_INVALID)
		{
			state.invalidWinSum--;
			if(pWin->style&WIN_VISIBLE)
			{
				if(childRect)
				{
					rect=&(pWin->invalidRect);
					while(rect)
					{
						if(winContain(childRect,rect)==false)
						{
							if((pWin->style&WIN_WIDGET)==0&&winOverlapping(rect, childRect))
							{
								msg.parameter.p=&invalidRect;
								if(rect->x0<childRect->x0)
								{
									invalidRect.x0=rect->x0;
									invalidRect.x1=childRect->x0-1;
									invalidRect.y0=rect->y0;
									invalidRect.y1=rect->y1;
									winSendMsg(hWin,&msg);
								}
								if(rect->y0<childRect->y0)
								{
									invalidRect.x0=MAX(rect->x0,childRect->x0);
									invalidRect.x1=rect->x1;
									invalidRect.y0=rect->y0;
									invalidRect.y1=childRect->y0-1;
									winSendMsg(hWin,&msg);
								}
								if(rect->x1>childRect->x1)
								{
									invalidRect.x0=childRect->x1+1;
									invalidRect.x1=rect->x1;
									invalidRect.y0=MAX(rect->y0,childRect->y0);
									invalidRect.y1=rect->y1;
									winSendMsg(hWin,&msg);
								}
								if(rect->y1>childRect->y1)
								{
									invalidRect.x0=MAX(rect->x0,childRect->x0);
									invalidRect.x1=MIN(rect->x1,childRect->x1);
									invalidRect.y0=childRect->y1+1;
									invalidRect.y1=rect->y1;
									winSendMsg(hWin,&msg);
								}
							}
							else
							{
								msg.parameter.p=rect;
								winSendMsg(hWin,&msg);
							}
						}
						rect=rect->next;
					}
				}
				else
				{
					rect=&(pWin->invalidRect);
					while(rect)
					{
						msg.parameter.p=rect;
						winSendMsg(hWin,&msg);
						rect=rect->next;
					}
				}
			}
			winFreeInvalidRect(pWin);
			pWin->style&=(~WIN_INVALID);
		}
		hWin=pWin->next;
	}
}
uint8 winPaint(void)
{
	winHandle hWin;
	winObj* pWin;
	winObj* pChild;
	if(state.invalidWinSum==0)
		return 0;
	hWin=state.deskTopWin;
	uiLock();
	while(state.invalidWinSum)
	{
		pWin=(winObj*)uiHandleToPtr(hWin);
		if(pWin->child==INVALID_HANDLE)
		{
			winPaint1(hWin,NULL);
			break;
		}
		else
		{
			pChild=(winObj*)uiHandleToPtr(pWin->child);
			if((pChild->style&WIN_VISIBLE)&&pChild->bgColor!=INVALID_COLOR)
				winPaint1(hWin,&(pChild->rect));
			else
				winPaint1(hWin,NULL);
		}
		hWin=pWin->child;
	}
	if(rectMemp.freeBlks!=rectMemp.maxBlks||rectMemp.minFreeBlks==0)
		state.mempError=true;
	if(state.mempError)
	{
		winErrShow(strToResId("mempool err"));
		uart_Printf("memPool error information:\n");
		mempInfor(&rectMemp);
	}
	state.invalidWinSum=0;
	uiUnlock();
	return 1;
}
void winSetName(winHandle hWin,char* name)
{
	winObj* pWin;
//	uiRect invalidRect;
//	winMsg msg;
	uiLock();
	if(hWin==INVALID_HANDLE)
	{
		uiUnlock();
		return;
	}
	pWin=(winObj*)uiHandleToPtr(hWin);
	pWin->name=name;
	uiUnlock();
}

void winIntersection(uiRect* out,uiRect* win1,uiRect* win2)
{
	out->x0=MAX(win1->x0,win2->x0);
	out->x1=MIN(win1->x1,win2->x1);
	out->y0=MAX(win1->y0,win2->y0);
	out->y1=MIN(win1->y1,win2->y1);
}
void winUpdateAllResId(void)
{
	winHandle hWin,hChild;
	winObj* pWin;
	uiLock();
	hWin=state.deskTopWin;
	while(hWin!=INVALID_HANDLE)
	{
		pWin=(winObj*)uiHandleToPtr(hWin);
		hChild=pWin->child;
		while(hWin!=INVALID_HANDLE)
		{
			winSendMsgNoData(hWin,MSG_WIN_UPDATE_RESID);
			hWin=pWin->next;
			pWin=(winObj*)uiHandleToPtr(hWin);
		}
		hWin=hChild;
	}
	uiUnlock();
}
void winDestroyDeskTopChildWin(void)
{
	winObj* pWin;
	winHandle hWin,next;
	uiLock();
	hWin=state.deskTopWin;
	pWin=(winObj*)uiHandleToPtr(hWin);
	if(pWin==NULL)
	{
		uiUnlock();
		return;
	}
	if(pWin->child!=INVALID_HANDLE)
		winDestroy(&(pWin->child));
	hWin=pWin->next;
	while(hWin!=INVALID_HANDLE)
	{
		pWin=(winObj*)uiHandleToPtr(hWin);
		next=pWin->next;
		winDestroy(&hWin);
		hWin=next;
	}
	uiUnlock();
}
extern void resBuffInit(void);
void winInit(void)
{
	#define WIN_HEAP_SIZE  (1024*64*1)
	__attribute__ ((section(".big_buffer"))) __attribute__ ((aligned(32))) static uint8 winHeap[WIN_HEAP_SIZE];
	uiHeapInit(winHeap,WIN_HEAP_SIZE);
	resBuffInit();
	mempCreate(&rectMemp,uiAlloc(sizeof(uiRect)*500), sizeof(uiRect)*500, sizeof(uiRect));
	state.winSum=0;
	state.invalidWinSum=0;
	state.deskTopWin=INVALID_HANDLE;
	state.curWin=INVALID_HANDLE;
	state.destroyWin=0;
	state.mempError=false;
	state.errTipRect.x0=0;
	state.errTipRect.y0=(LCD_HEIGHT-60)/2;
	state.errTipRect.x1=LCD_WIDTH;
	state.errTipRect.y1=(LCD_HEIGHT-60)/2+60;
	state.deskTopWin=winCreate(0,0,LCD_WIDTH,LCD_HEIGHT,INVALID_HANDLE,deskTopWinCB,WIN_SIZE,WIN_NOT_ZOOM);
	winSetbgColor(state.deskTopWin,DEFAULT_COLOR);
}
extern void resBuffUninit(void);
void winUninit(void)
{
	if(state.deskTopWin!=INVALID_HANDLE)
		winDestroy(&state.deskTopWin);
	resBuffUninit();
	state.curWin=INVALID_HANDLE;
	state.winSum=0;
	state.destroyWin=0;
	state.invalidWinSum=0;
	if(state.mempError)
		uart_Printf("memPool error information:\n");
	else
		uart_Printf("\n\n");
	mempInfor(&rectMemp);
}


