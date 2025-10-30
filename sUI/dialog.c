#include "dialog.h"
#include "frameWin.h"

winHandle dialogCreate(widgetCreateInfor* pWidgets,winCB cb,winHandle parent)
{
	winHandle hDialog;
	winHandle handle;
	winObj* pWin;
	if(pWidgets==NULL)
		return INVALID_HANDLE;
	hDialog=pWidgets->widgetCreate(pWidgets,parent,cb);
	if(hDialog==INVALID_HANDLE)
		return INVALID_HANDLE;
	pWidgets++;
	pWin=(winObj*)uiHandleToPtr(hDialog);
	if(pWin&&(pWin->style&WIN_WIDGET))
	{
		debug_err("first window must not widget!!!\n");
		winDestroy(&hDialog);
		return INVALID_HANDLE;
	}
	while(pWidgets->widgetCreate)
	{
		//if(pWidgets->strIsASCII)
		//	pWidgets->str=strToResId(pWidgets->str);
		handle=pWidgets->widgetCreate(pWidgets,hDialog,cb);
		winSendMsgToParentNoData(handle,MSG_WIN_ADD_WIDGET);
		pWidgets++;
	}
	return hDialog;
}

winHandle dialogItem(winHandle hDialog,uint16 id)
{
	winObj* pWin=NULL;
	winMsg msg;
	if(hDialog==INVALID_HANDLE)
		return INVALID_HANDLE;
	if(id==INVALID_WIDGET_ID)
		return INVALID_HANDLE;
	msg.id=MSG_WIN_GET_WIDGET;
	msg.hSrc=INVALID_HANDLE;
	msg.parameter.v=id;
	winSendMsg(hDialog,&msg);
	if(msg.hSrc!=INVALID_HANDLE)
		return msg.hSrc;
	if(id<WIGET_HANDLE_MAX_NUM)
		return INVALID_HANDLE;
	//debug_wrn("get [%d] handle ineffective!!!!!!\n",id);
	pWin=(winObj*)uiHandleToPtr(hDialog);
	while(pWin!=NULL)
	{
		if(widgetGetId(pWin->next)==id)
			return pWin->next;
		pWin=(winObj*)uiHandleToPtr(pWin->next);
	}
	return INVALID_HANDLE;
}


