#include "tips.h"


static void tipsCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	tipsObj* ptips=(tipsObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(ptips->widget.win);
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			//debug_msg("tips win create\n");
			return;
		case MSG_WIN_PAINT:
			if(ptips->visible==true)
				drawString(&pWin->rect,(uiRect*)(msg->parameter.p),ptips->str,ptips->font,ptips->strAlign,ptips->fontColor,ptips->fgColor,INVALID_COLOR);
			else if(pWin->bgColor!=INVALID_COLOR)
				drawRectangle((uiRect*)(msg->parameter.p),pWin->bgColor);
			return;
		case MSG_WIN_UPDATE_RESID:
			if(winIsVisible(hWin))
			{
				if(pWin->bgColor==INVALID_COLOR)
					winChangeNotify(hWin);
				else
					winUpdate(hWin);
			}
			return;
		case MSG_WIN_CHANGE_RESID:
			if(resIdIsStr(ptips->str)==0&&ptips->str==msg->parameter.v)
				return;
			ptips->str=msg->parameter.v;
			if(winIsVisible(hWin))
			{
				if(pWin->bgColor==INVALID_COLOR)
					winChangeNotify(hWin);
				else
					winUpdate(hWin);
			}
			return;
		case MSG_WIN_CHANGE_FG_COLOR:
			if(ptips->fgColor==msg->parameter.v)
				return;
			ptips->fgColor=msg->parameter.v;
			if(winIsVisible(hWin))
			{
				if(pWin->bgColor==INVALID_COLOR)
					winChangeNotify(hWin);
				else
					winUpdate(hWin);
			}
			break;
		case MSG_WIN_VISIBLE_SET:
			if(pWin->bgColor==INVALID_COLOR)
				break;
			if(msg->parameter.v)
				ptips->visible=true;
			else
				ptips->visible=false;
			winUpdate(hWin);
			return;
		case MSG_WIN_VISIBLE_GET:
			if(pWin->bgColor==INVALID_COLOR)
				break;
			if(ptips->visible)
				msg->parameter.v=1;
			else
				msg->parameter.v=0;
			return;
		case MSG_WIN_TOUCH:
			break;
		case MSG_WIN_TOUCH_GET_INFOR:
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle tipsCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle htips;
	tipsObj* ptips;
	htips=winCreate(x0,y0,width,height,parent,tipsCB,sizeof(tipsObj),WIN_WIDGET|style);
	widgetSetId(htips,id);
	if(htips!=INVALID_HANDLE)
	{
		ptips=(tipsObj*)uiHandleToPtr(htips);
		ptips->fgColor=INVALID_COLOR;
		ptips->visible=true;
	}
	return htips;
}

winHandle tipsCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb)
{
	winHandle htips;
	tipsObj* ptips;
	htips = tipsCreate(infor->x0,infor->y0,infor->width,infor->height,parent,infor->style,infor->id);
	if(htips!=INVALID_HANDLE)
	{
		ptips=(tipsObj*)uiHandleToPtr(htips);
		ptips->str=infor->str;
		ptips->strAlign=infor->strAlign;
		ptips->fontColor=infor->fontColor;
		ptips->font=infor->font;
		winSetbgColor(htips, infor->bgColor);
	}
	return htips;
}


