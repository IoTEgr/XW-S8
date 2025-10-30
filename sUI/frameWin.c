#include "frameWin.h"

static void frameWinCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	frameWinObj* pFrameWin=(frameWinObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pFrameWin->win);
	winObj* pChild;
	uint16   chileId;
	if(pFrameWin->cb)
		pFrameWin->cb(msg);
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			//debug_msg("frame win create\n");
			return;
		case MSG_WIN_PAINT:
			if(pWin->bgColor!=INVALID_COLOR)
				drawRectangle((uiRect*)(msg->parameter.p), pWin->bgColor);
			//debug_msg("paint frame win[]:[%d %d %d %d]\n",pWin->invalidRect.x0,pWin->invalidRect.y0,pWin->invalidRect.x1,pWin->invalidRect.y1);
			return;
		case MSG_WIN_TOUCH:
			return;
		case MSG_WIN_TOUCH_GET_INFOR:
			return;
		case MSG_WIN_ADD_WIDGET:
			pChild=(winObj*)uiHandleToPtr(msg->hSrc);
			if(pChild!=NULL&&(pChild->style&WIN_WIDGET))
			{
				chileId=widgetGetId(msg->hSrc);
				if(chileId!=INVALID_WIDGET_ID&&chileId<WIGET_HANDLE_MAX_NUM)
					pFrameWin->widgetHandle[chileId]=msg->hSrc;
			}
			return;
		case MSG_WIN_GET_WIDGET:
			chileId=msg->parameter.v;
			if(chileId!=INVALID_WIDGET_ID&&chileId<WIGET_HANDLE_MAX_NUM)
				msg->hSrc=pFrameWin->widgetHandle[chileId];
			else
				msg->hSrc=INVALID_HANDLE;
			return;
		case MSG_WIN_WIDGET_DESTROY:
			pChild=(winObj*)uiHandleToPtr(msg->hSrc);
			if(pChild!=NULL&&(pChild->style&WIN_WIDGET))
			{
				chileId=widgetGetId(msg->hSrc);
				if(chileId!=INVALID_WIDGET_ID&&chileId<WIGET_HANDLE_MAX_NUM)
					pFrameWin->widgetHandle[chileId]=INVALID_HANDLE;
			}
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle frameWinCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,winCB cb)
{
	winHandle hFrameWin;
	frameWinObj* pFrameWin;
	hFrameWin=winCreate(x0,y0,width,height,parent,frameWinCB,sizeof(frameWinObj),style|WIN_FRAME);
	if(hFrameWin!=INVALID_HANDLE)
	{
		pFrameWin=(frameWinObj*)uiHandleToPtr(hFrameWin);
		pFrameWin->cb=cb;
	}
	return hFrameWin;
}

winHandle frameWinCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb)
{
	winHandle hFrameWin;
	frameWinObj* pFrameWin;
	hFrameWin= frameWinCreate(infor->x0,infor->y0,infor->width,infor->height,parent,infor->style,cb);
	winSetbgColor(hFrameWin, infor->bgColor);
	if(hFrameWin!=INVALID_HANDLE)
	{
		pFrameWin=(frameWinObj*)uiHandleToPtr(hFrameWin);
		pFrameWin->prvate=infor->private;
	}
	return hFrameWin;
}







