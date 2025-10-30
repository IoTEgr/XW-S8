#include "menuOption.h"
#include "imageIcon.h"
#include "stringIcon.h"


static void menuOptionCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	menuOptionObj* pmenuOption=(menuOptionObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pmenuOption->widget.win);
	resInfor* infor;
//	strInfor* stringInfor;
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			//debug_msg("menuOption win create\n");
			return;
		case MSG_WIN_PAINT:
			if(pmenuOption->select==true)
			{
				if(pmenuOption->selectColor!=INVALID_COLOR)
					drawRectangle((uiRect*)(msg->parameter.p),pmenuOption->selectColor);
				if(pmenuOption->selectImage!=INVALID_RES_ID)
					drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pmenuOption->selectImage,ALIGNMENT_CENTER,pWin->bgColor);
			}
			else
			{
				if(pmenuOption->selectColor!=INVALID_COLOR)
					drawRectangle((uiRect*)(msg->parameter.p),pmenuOption->color);
				if(pmenuOption->selectImage!=INVALID_RES_ID)
					drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pmenuOption->image,ALIGNMENT_CENTER,pWin->bgColor);
			}
			return;
		case MSG_WIN_CHANGE_RESID:
			if(!resIdIsImage(msg->parameter.v))
				winSetResid(pmenuOption->hStr,msg->parameter.v);
			return;
		case MSG_WIN_INVALID_RESID:
			winSetResid(pmenuOption->hStr,INVALID_RES_ID);
			return;
		case MSG_WIN_SELECT_INFOR_EX:
			infor=(resInfor*)(msg->parameter.p);
			pmenuOption->selectColor=infor->color;
			pmenuOption->selectImage=infor->image;
			winSendMsg(pmenuOption->hStr,msg);
			return;
		case MSG_WIN_UNSELECT_INFOR_EX:
			infor=(resInfor*)(msg->parameter.p);
			pmenuOption->color=infor->color;
			pmenuOption->image=infor->image;
			winSendMsg(pmenuOption->hStr,msg);
			return;
		case MSG_WIN_CHANGE_STRINFOR:
			winSendMsg(pmenuOption->hStr,msg);
			return;
		case MSG_WIN_SELECT_INFOR:
			infor=(resInfor*)(msg->parameter.p);
			pmenuOption->selectColor=infor->color;
			pmenuOption->selectImage=infor->image;
			return;
		case MSG_WIN_UNSELECT_INFOR:
			infor=(resInfor*)(msg->parameter.p);
			pmenuOption->color=infor->color;
			pmenuOption->image=infor->image;
			return;
		case MSG_WIN_UNSELECT:
			if(pmenuOption->select==false)
				return;
			pmenuOption->select=false;
			if(winIsVisible(hWin))
				winChangeNotify(hWin);
			winSendMsg(pmenuOption->hStr,msg);
			return;
		case MSG_WIN_SELECT:
			if(pmenuOption->select==true)
				return;
			pmenuOption->select=true;
			if(winIsVisible(hWin))
				winChangeNotify(hWin);
			winSendMsg(pmenuOption->hStr,msg);
			return;
		case MSG_WIN_VISIBLE_SET:
			if(msg->parameter.v)
				winSetVisible(pmenuOption->hStr,true);
			else
				winSetVisible(pmenuOption->hStr,false);
			break;
		case MSG_WIN_TOUCH:
			break;
		case MSG_WIN_TOUCH_GET_INFOR:
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle menuOptionCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hmenuOption;
	menuOptionObj* pmenuOption;
	hmenuOption=winCreate(x0,y0,width,height,parent,menuOptionCB,sizeof(menuOptionObj),WIN_WIDGET|style);
	widgetSetId(hmenuOption,id);
	if(hmenuOption!=INVALID_HANDLE)
	{
		pmenuOption=(menuOptionObj*)uiHandleToPtr(hmenuOption);
		pmenuOption->color=INVALID_COLOR;
		pmenuOption->selectColor=INVALID_COLOR;
		pmenuOption->image=INVALID_RES_ID;
		pmenuOption->selectImage=INVALID_RES_ID;
		pmenuOption->hStr=INVALID_HANDLE;
		pmenuOption->select=false;
	}
	return hmenuOption;
}

winHandle itemCreatemenuOption(int16 x0,int16 y0,uint16 width,uint16 height)
{
	winHandle hmenuOption;
	menuOptionObj* pmenuOption;
	hmenuOption=menuOptionCreate(x0,y0,width,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
	if(hmenuOption!=INVALID_HANDLE)
	{
		pmenuOption=(menuOptionObj*)uiHandleToPtr(hmenuOption);
		winSetbgColor(hmenuOption, INVALID_COLOR);
		pmenuOption->hStr=stringIconCreate(x0,y0,width,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
	}
	return hmenuOption;
}




