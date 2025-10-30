#include "widget.h"


bool widgetProc(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	widgetObj* pWin=(widgetObj*)uiHandleToPtr(hWin);
	switch(msg->id)
	{
		case MSG_WIDGET_GET_ID:
			msg->parameter.v=pWin->id;
			return true;
		case MSG_WIDGET_SET_ID:
			pWin->id=msg->parameter.v;
			return true;
		default:
			break;
	}
	return false;
}
void widgetSetType(winHandle hWin,widgetType type)
{
	widgetObj* pWin=(widgetObj*)uiHandleToPtr(hWin);
	if(pWin==NULL)
		return;
	if((pWin->win.style&WIN_WIDGET)==0)
		return;
	pWin->type=type;
}
widgetType widgetGetType(winHandle hWin)
{
	widgetObj* pWin=(widgetObj*)uiHandleToPtr(hWin);
	if(pWin==NULL)
		return WIDGET_TYPE_MAX;
	if((pWin->win.style&WIN_WIDGET)==0)
		return WIDGET_TYPE_MAX;
	return pWin->type;
}

uint16 widgetGetId(winHandle hWin)
{
	winMsg msg;
	msg.id=MSG_WIDGET_GET_ID;
	msg.parameter.v=INVALID_WIDGET_ID;
	winSendMsg(hWin,&msg);
	return msg.parameter.v;
}
void widgetSetId(winHandle hWin,uint16 id)
{
	winMsg msg;
	msg.id=MSG_WIDGET_SET_ID;
	msg.parameter.v=id;
	winSendMsg(hWin,&msg);
}














