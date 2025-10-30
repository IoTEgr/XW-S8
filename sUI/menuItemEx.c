#include "menuItemEx.h"
#include "imageIcon.h"
#include "stringIcon.h"


static void menuItemExCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	menuItemExObj* pmenuItemEx=(menuItemExObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pmenuItemEx->widget.win);
	resInfor* infor;
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			//debug_msg("menuItemEx win create\n");
			return;
		case MSG_WIN_PAINT:
			if(pmenuItemEx->select==true)
			{
				if(pmenuItemEx->selectColor!=INVALID_COLOR)
					drawRectangle((uiRect*)(msg->parameter.p),pmenuItemEx->selectColor);
				if(pmenuItemEx->selectImage!=INVALID_RES_ID)
					drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pmenuItemEx->selectImage,ALIGNMENT_CENTER,pWin->bgColor);
			}
			else
			{
				if(pmenuItemEx->selectColor!=INVALID_COLOR)
					drawRectangle((uiRect*)(msg->parameter.p),pmenuItemEx->color);
				if(pmenuItemEx->selectImage!=INVALID_RES_ID)
					drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pmenuItemEx->image,ALIGNMENT_CENTER,pWin->bgColor);
			}
			return;
		case MSG_WIN_INVALID_RESID:
			winSetResid(pmenuItemEx->hImage,INVALID_RES_ID);
			winSetResid(pmenuItemEx->hStr,INVALID_RES_ID);
			winSetResid(pmenuItemEx->hSelImage,INVALID_RES_ID);
			winSetResid(pmenuItemEx->hSelStr,INVALID_RES_ID);
			return;
		case MSG_WIN_CHANGE_RESID:
			if(resIdIsImage(msg->parameter.v))
				winSetResid(pmenuItemEx->hImage,msg->parameter.v);
			else
				winSetResid(pmenuItemEx->hStr,msg->parameter.v);
			return;
		case MSG_WIN_CHG_ITEM_SEL_RESID:
			if(resIdIsImage(msg->parameter.v))
				winSetResid(pmenuItemEx->hSelImage,msg->parameter.v);
			else
				winSetResid(pmenuItemEx->hSelStr,msg->parameter.v);
			return;
		case MSG_WIN_SELECT_INFOR_EX:
			infor=(resInfor*)(msg->parameter.p);
			pmenuItemEx->selectColor=infor->color;
			pmenuItemEx->selectImage=infor->image;
			winSendMsg(pmenuItemEx->hStr,msg);
			{
				resInfor tempInfor;
				memcpy(&tempInfor,msg->parameter.p,sizeof(resInfor));
				tempInfor.strAlign=ALIGNMENT_RIGHT;
				winSetSelectInfor(pmenuItemEx->hSelStr,&tempInfor);
			}
			return;
		case MSG_WIN_UNSELECT_INFOR_EX:
			infor=(resInfor*)(msg->parameter.p);
			pmenuItemEx->color=infor->color;
			pmenuItemEx->image=infor->image;
			winSendMsg(pmenuItemEx->hStr,msg);
			{
				resInfor tempInfor;
				memcpy(&tempInfor,msg->parameter.p,sizeof(resInfor));
				tempInfor.strAlign=ALIGNMENT_RIGHT;
				winSetUnselectInfor(pmenuItemEx->hSelStr,&tempInfor);
			}
			return;
		case MSG_WIN_CHANGE_STRINFOR:
			winSendMsg(pmenuItemEx->hStr,msg);
			((strInfor*)(msg->parameter.p))->strAlign=ALIGNMENT_RIGHT;
			winSendMsg(pmenuItemEx->hSelStr,msg);
			return;
		case MSG_WIN_SELECT_INFOR:
			infor=(resInfor*)(msg->parameter.p);
			pmenuItemEx->selectColor=infor->color;
			pmenuItemEx->selectImage=infor->image;
			return;
		case MSG_WIN_UNSELECT_INFOR:
			infor=(resInfor*)(msg->parameter.p);
			pmenuItemEx->color=infor->color;
			pmenuItemEx->image=infor->image;
			return;
		case MSG_WIN_UNSELECT:
			if(pmenuItemEx->select==false)
				return;
			pmenuItemEx->select=false;
			if(winIsVisible(hWin))
				winChangeNotify(hWin);
			winSendMsg(pmenuItemEx->hStr,msg);
			winSendMsg(pmenuItemEx->hSelStr,msg);
			return;
		case MSG_WIN_SELECT:
			if(pmenuItemEx->select==true)
				return;
			pmenuItemEx->select=true;
			if(winIsVisible(hWin))
				winChangeNotify(hWin);
			winSendMsg(pmenuItemEx->hStr,msg);
			winSendMsg(pmenuItemEx->hSelStr,msg);
			return;
		case MSG_WIN_VISIBLE_SET:
			if(msg->parameter.v)
			{
				winSetVisible(pmenuItemEx->hImage,true);
				winSetVisible(pmenuItemEx->hStr,true);
				winSetVisible(pmenuItemEx->hSelImage,true);
				winSetVisible(pmenuItemEx->hSelStr,true);
			}
			else
			{
				winSetVisible(pmenuItemEx->hImage,false);
				winSetVisible(pmenuItemEx->hStr,false);
				winSetVisible(pmenuItemEx->hSelImage,false);
				winSetVisible(pmenuItemEx->hSelStr,false);
			}
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

winHandle menuItemExCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hmenuItemEx;
	menuItemExObj* pmenuItemEx;
	hmenuItemEx=winCreate(x0,y0,width,height,parent,menuItemExCB,sizeof(menuItemExObj),WIN_WIDGET|style);
	widgetSetId(hmenuItemEx,id);
	if(hmenuItemEx!=INVALID_HANDLE)
	{
		pmenuItemEx=(menuItemExObj*)uiHandleToPtr(hmenuItemEx);
		pmenuItemEx->color=INVALID_COLOR;
		pmenuItemEx->selectColor=INVALID_COLOR;
		pmenuItemEx->image=INVALID_RES_ID;
		pmenuItemEx->selectImage=INVALID_RES_ID;
		pmenuItemEx->hImage=INVALID_HANDLE;
		pmenuItemEx->hStr=INVALID_HANDLE;
		pmenuItemEx->hSelImage=INVALID_HANDLE;
		pmenuItemEx->hSelStr=INVALID_HANDLE;
		pmenuItemEx->select=false;
	}
	return hmenuItemEx;
}

winHandle itemCreateMenuItemEx(int16 x0,int16 y0,uint16 width,uint16 height)
{
	winHandle hmenuItemEx;
	menuItemExObj* pmenuItemEx;
	hmenuItemEx=menuItemExCreate(x0,y0,width,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
	if(hmenuItemEx!=INVALID_HANDLE)
	{
		pmenuItemEx=(menuItemExObj*)uiHandleToPtr(hmenuItemEx);
		winSetbgColor(hmenuItemEx, INVALID_COLOR);
		pmenuItemEx->hImage=imageIconCreate(x0,y0,height,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
		pmenuItemEx->hStr=stringIconCreate(x0+height,y0,((width-(height<<1))>>1)+10,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
		pmenuItemEx->hSelStr=stringIconCreate(x0+height+((width-(height<<1))>>1)+10,y0,(width-(height<<1))>>1,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
		pmenuItemEx->hSelImage=imageIconCreate(x0+width-height,y0,height,height,INVALID_HANDLE,WIN_NOT_ZOOM,INVALID_WIDGET_ID);
	}
	return hmenuItemEx;
}




