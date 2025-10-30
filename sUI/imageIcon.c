#include "imageIcon.h"


static void imageIconCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	imageIconObj* pimageIcon=(imageIconObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pimageIcon->widget.win);
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			//debug_msg("imageIcon win create\n");
			return;
		case MSG_WIN_PAINT:
			if(pimageIcon->visible&&pimageIcon->image!=INVALID_RES_ID)
				drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pimageIcon->image,pimageIcon->imageAlign,pWin->bgColor);
			else if(pWin->bgColor!=INVALID_COLOR)
				drawRectangle((uiRect*)(msg->parameter.p),pWin->bgColor);
			//debug_msg("paint imageIcon [%d]:[%d %d %d %d]\n",pimageIcon->widget.id,pWin->invalidRect.x0,pWin->invalidRect.y0,pWin->invalidRect.x1,pWin->invalidRect.y1);
			return;
		case MSG_WIN_CHANGE_RESID:
			if(pimageIcon->image==msg->parameter.v)
				return;
			pimageIcon->image=msg->parameter.v;
			if(winIsVisible(hWin))
			{
				if(pWin->bgColor==INVALID_COLOR)
					winChangeNotify(hWin);
				else
					winUpdate(hWin);
			}
			return;
		case MSG_WIN_VISIBLE_SET:
			if(pWin->bgColor==INVALID_COLOR)
				break;
			if(msg->parameter.v)
				pimageIcon->visible=true;
			else
				pimageIcon->visible=false;
			winUpdate(hWin);
			return;
		case MSG_WIN_VISIBLE_GET:
			if(pWin->bgColor==INVALID_COLOR)
				break;
			if(pimageIcon->visible)
				msg->parameter.v=1;
			else
				msg->parameter.v=0;
			return;
		case MSG_WIN_TOUCH:
			break;
		case MSG_WIN_TOUCH_GET_INFOR:
			((touchInfor *)(msg->parameter.p))->touchWin=pWin->parent;
			((touchInfor *)(msg->parameter.p))->touchHandle=hWin;
			((touchInfor *)(msg->parameter.p))->touchID=pimageIcon->widget.id;
			((touchInfor *)(msg->parameter.p))->touchItem=0;
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle imageIconCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle himageIcon;
	imageIconObj* pimageIcon;
	himageIcon=winCreate(x0,y0,width,height,parent,imageIconCB,sizeof(imageIconObj),WIN_WIDGET|style);
	widgetSetId(himageIcon,id);
	if(himageIcon!=INVALID_HANDLE)
	{
		pimageIcon=(imageIconObj*)uiHandleToPtr(himageIcon);
		pimageIcon->visible=true;
		pimageIcon->image=INVALID_RES_ID;
		pimageIcon->imageAlign=ALIGNMENT_CENTER;
		winSetbgColor(himageIcon,INVALID_COLOR);
	}
	return himageIcon;
}

winHandle imageIconCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb)
{
	winHandle himageIcon;
	imageIconObj* pimageIcon;
	himageIcon = imageIconCreate(infor->x0,infor->y0,infor->width,infor->height,parent,infor->style|WIN_TOUCH_SUPPORT,infor->id);
	if(himageIcon!=INVALID_HANDLE)
	{
		pimageIcon=(imageIconObj*)uiHandleToPtr(himageIcon);
		pimageIcon->image=infor->image;
		pimageIcon->imageAlign=infor->imageAlign;
		winSetbgColor(himageIcon, infor->bgColor);
	}
	return himageIcon;
}




