#include "widgetManage.h"
#include "dialog.h"


static void widgetManageProc(winMsg* msg)
{
	winHandle hCurWin;
	uint16 id;
	widgetManageObj* pwidgetManage=(widgetManageObj*)uiHandleToPtr(msg->hWin);
	winHandle hParentWin=pwidgetManage->widget.win.parent;
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIDGET_NEXT:
			winSendMsgNoData(dialogItem(hParentWin,pwidgetManage->curID),MSG_WIN_UNSELECT);
			if(pwidgetManage->curID<INVALID_WIDGET_ID)
				pwidgetManage->curID++;
			if(pwidgetManage->curID>pwidgetManage->maxID)
				pwidgetManage->curID=pwidgetManage->minID;
			hCurWin=dialogItem(hParentWin,pwidgetManage->curID);
			if(pwidgetManage->getRes)
				winSetResid(hCurWin,pwidgetManage->getRes(pwidgetManage->curID));
			winSendMsgNoData(hCurWin,MSG_WIN_SELECT);
			return ;
		case MSG_WIDGET_PRE:
			winSendMsgNoData(dialogItem(hParentWin,pwidgetManage->curID),MSG_WIN_UNSELECT);	
			if(pwidgetManage->curID>pwidgetManage->minID)
				pwidgetManage->curID--;
			else
				pwidgetManage->curID=pwidgetManage->maxID;
			hCurWin=dialogItem(hParentWin,pwidgetManage->curID);
			if(pwidgetManage->getRes)
				winSetResid(hCurWin,pwidgetManage->getRes(pwidgetManage->curID));
			winSendMsgNoData(hCurWin,MSG_WIN_SELECT);
			return ;
		case MSG_WIDGET_SELECT:
			id=msg->parameter.v;
			if(id==pwidgetManage->curID)
				return;
			if(id<pwidgetManage->minID||id>pwidgetManage->maxID)
				return;
			winSendMsgNoData(dialogItem(hParentWin,pwidgetManage->curID),MSG_WIN_UNSELECT);	
			pwidgetManage->curID=id;
			hCurWin=dialogItem(hParentWin,pwidgetManage->curID);
			if(pwidgetManage->getRes)
				winSetResid(hCurWin,pwidgetManage->getRes(pwidgetManage->curID));
			winSendMsgNoData(hCurWin,MSG_WIN_SELECT);
			return;
		case MSG_WIDGET_CUR:
			msg->parameter.v=pwidgetManage->curID;
			return;
		case MSG_WIDGET_CHANGE_ALL_RESID:
			if(pwidgetManage->getRes==NULL)
				return;
			for(id=pwidgetManage->minID;id<=pwidgetManage->maxID;id++)
			{
				winSetResid(dialogItem(hParentWin,id),pwidgetManage->getRes(id));
			}
			return;
		case MSG_WIN_UPDATE_RESID:
			winSetResid(dialogItem(hParentWin,pwidgetManage->curID),pwidgetManage->getRes(pwidgetManage->curID));
			return;
		case MSG_WIN_SELECT_INFOR_EX:
		case MSG_WIN_UNSELECT_INFOR_EX:
			for(id=pwidgetManage->minID;id<=pwidgetManage->maxID;id++)
			{
				winSendMsg(dialogItem(hParentWin,id),msg);
			}
		default:
			return;
	}
	winDefaultProc(msg);
}

winHandle widgetManageCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hwidgetManage;
	widgetManageObj* pwidgetManage;
	hwidgetManage=winCreate(0,0,1,1,parent,widgetManageProc,sizeof(widgetManageObj),WIN_WIDGET|style);
	widgetSetId(hwidgetManage,id);
	if(hwidgetManage!=INVALID_HANDLE)
	{
		pwidgetManage=(widgetManageObj*)uiHandleToPtr(hwidgetManage);
		pwidgetManage->curID=INVALID_WIDGET_ID;
		pwidgetManage->getRes=NULL;
	}
	return hwidgetManage;
}

winHandle widgetManageCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb)
{
	winHandle hwidgetManage;
	widgetManageObj* pwidgetManage;
	hwidgetManage = widgetManageCreate(0,0,1,1,parent,infor->style|WIN_TOUCH_SUPPORT,infor->id);
	if(hwidgetManage!=INVALID_HANDLE)
	{
		pwidgetManage=(widgetManageObj*)uiHandleToPtr(hwidgetManage);
		pwidgetManage->minID=infor->width;
		pwidgetManage->maxID=infor->height;
		pwidgetManage->getRes=infor->private;
	}
	return hwidgetManage;
}

void widgetManageUpdateAll(winHandle hWin)
{
	winSendMsgNoData(hWin,MSG_WIDGET_CHANGE_ALL_RESID);
}
void widgetManageUpdateCur(winHandle hWin)
{
	winSendMsgNoData(hWin,MSG_WIN_UPDATE_RESID);
}
void widgetManageNext(winHandle hWin)
{
	winSendMsgNoData(hWin,MSG_WIDGET_NEXT);
}
void widgetManagePre(winHandle hWin)
{
	winSendMsgNoData(hWin,MSG_WIDGET_PRE);
}
void widgetManageSelect(winHandle hWin,uint16 id)
{
	winMsg msg;
	msg.id=MSG_WIDGET_SELECT;
	msg.parameter.v=id;
	winSendMsg(hWin,&msg);
}
uint16 widgetManageCurId(winHandle hWin)
{
	winMsg msg;
	msg.id=MSG_WIDGET_CUR;
	winSendMsg(hWin,&msg);
	return msg.parameter.v;
}



