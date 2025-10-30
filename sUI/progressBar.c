#include "progressBar.h"


static void progressBarCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	progressBarObj* pprogressBar=(progressBarObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pprogressBar->widget.win);
	uint32 length;
	uiRect rect;
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			return;
		case MSG_WIN_PAINT:
			length=pWin->rect.x1-pWin->rect.x0+1;
			length=pprogressBar->percentage*length/100;
			if(length==0)
				drawRectangle(&(pWin->rect),pWin->bgColor);
			else if(pprogressBar->percentage==100)
				drawRectangle(&(pWin->rect),pprogressBar->color);
			else
			{
				rect.y0=pWin->rect.y0;
				rect.y1=pWin->rect.y1;
				rect.x0=pWin->rect.x0;
				rect.x1=rect.x0+length-1;
				drawRectangle(&rect,pprogressBar->color);
				rect.x0=pWin->rect.x0+length-1;
				rect.x1=pWin->rect.x1;
				drawRectangle(&rect,pWin->bgColor);
			}
			return;
		case MSG_WIN_PROGRESS_RATE:
			length=msg->parameter.v;
			if(length>100)
				length=100;
			if(pprogressBar->percentage==length)
				return;
			pprogressBar->percentage=length;
			winUpdate(hWin);
			return;
		case MSG_WIN_CHANGE_FG_COLOR:
			pprogressBar->color=msg->parameter.v;
			return;
		case MSG_WIN_TOUCH:
			break;
		case MSG_WIN_TOUCH_GET_INFOR:
			((touchInfor *)(msg->parameter.p))->touchWin=pWin->parent;
			((touchInfor *)(msg->parameter.p))->touchHandle=hWin;
			((touchInfor *)(msg->parameter.p))->touchID=pprogressBar->widget.id;
			((touchInfor *)(msg->parameter.p))->touchItem=0;
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle progressBarCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hprogressBar;
	progressBarObj* pprogressBar;
	hprogressBar=winCreate(x0,y0,width,height,parent,progressBarCB,sizeof(progressBarObj),WIN_WIDGET|style);
	widgetSetId(hprogressBar,id);
	if(hprogressBar!=INVALID_HANDLE)
	{
		pprogressBar=(progressBarObj*)uiHandleToPtr(hprogressBar);
		pprogressBar->percentage=0;
	}
	return hprogressBar;
}
winHandle progressBarCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb)
{
	winHandle hprogressBar;
	progressBarObj* pprogressBar;
	hprogressBar = progressBarCreate(infor->x0,infor->y0,infor->width,infor->height,parent,infor->style,infor->id);
	if(hprogressBar!=INVALID_HANDLE)
	{
		pprogressBar=(progressBarObj*)uiHandleToPtr(hprogressBar);
		winSetfgColor(hprogressBar, infor->fontColor);
		winSetbgColor(hprogressBar, infor->bgColor);
	}
	(void)pprogressBar;
	return hprogressBar;
}





