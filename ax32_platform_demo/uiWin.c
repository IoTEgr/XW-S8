#include"uiWin.h"

extern void msgDealByType(uint32 msgType,void* handle,uint32 parameNum,uint32* parame);
void dialogCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
//	winHandle hSrc=msg->hSrc;
	frameWinObj* pFrameWin=(frameWinObj*)uiHandleToPtr(hWin);
	frameWinObj* pParent;
	uint32* argv=NULL;
	uiWin* win;
	uint32 argv1[3];
	switch(msg->id)
	{
		case MSG_DIALOG_INIT:
			win=(uiWin*)(pFrameWin->prvate);
			if(win)
			{
				pParent=(frameWinObj*)uiHandleToPtr(pFrameWin->win.parent);
				if(pParent)
				{
					if(pParent->win.style&WIN_FRAME)
					{
						if(pParent->prvate)
							msgDealByMsgInfor(((uiWin*)(pParent->prvate))->msgDeal,SYS_CHILE_OPEN,pFrameWin->win.parent,0,NULL);
					}
					else
					{
						debug_msg("parent window is not frame\n");
					}
				}
				argv=msg->parameter.p;
				msgDealByMsgInfor(win->msgDeal,SYS_OPEN_WINDOW,hWin,argv[0],argv+1);
				//msgDealByType(SYS_OPEN_WINDOW,hWin,argv[0],argv+1);
			}
			else
				debug_err("window open error,can not find registered msg!!!\n\n");
			break;
		case MSG_WIN_CHILE_DESTROY:
			win=(uiWin*)(pFrameWin->prvate);
			if(win)
			{
				msgFuncRegister(win->msgDeal);
				msgDealByType(SYS_CHILE_COLSE,hWin,0,NULL);
			}
			else
				debug_err("child window close error,can not find registered msg!!!\n\n");
			break;
		case MSG_DIALOG_CLOSE:
			win=(uiWin*)(pFrameWin->prvate);
			if(win)
			{
				msgDealByMsgInfor(win->msgDeal,SYS_CLOSE_WINDOW,hWin,0,NULL);
				win->handle=INVALID_HANDLE;
			}
			else
				debug_err("window close error,can not find registered msg!!!\n\n");
			break;
		case MSG_WIN_PARENT_DEAL:
			win=(uiWin*)(pFrameWin->prvate);
			msgDealByMsgInfor(win->msgDeal,msg->parameter.v,hWin,0,argv);
			break;
		case MSG_WIN_TOUCH:
			argv1[0]=((touchInfor *)(msg->parameter.p))->touchID;
			argv1[1]=((touchInfor *)(msg->parameter.p))->touchItem;
			argv1[2]=((touchInfor *)(msg->parameter.p))->touchState;
			msgDealByType(SYS_TOUCH_WINDOW,hWin,3,argv1);
			break;
		default:
			break;
	}
}

void uiParentDealMsg(winHandle handle,uint32 parentMsg)
{
	winMsg msg;
	msg.id=MSG_WIN_PARENT_DEAL;
	msg.parameter.v=parentMsg;
	winSendMsgToParent(handle,&msg);
}

winHandle uiOpenWindow(uiWin* winInfor,uint32 argc,...)
{
#define MAX_ARG       5
	winMsg msg;
	winHandle hDialog;
	uint32 argv[MAX_ARG+1];
	uint32 i;
	va_list arglist;
	va_start(arglist, argc);
	if(argc>MAX_ARG)
		argc=MAX_ARG;
	argv[0]=argc;
	for(i=0;i<argc;i++)	
		argv[i+1]=va_arg(arglist, uint32);
	va_end(arglist);
	if(winInfor->handle!=INVALID_HANDLE)
	{
		if(winInfor->repeateOpenSupport==false)
		{
			debug_msg("window has opened,do not surport open a window repeatedly !!!\n");
			return INVALID_HANDLE;
		}
		debug_msg("open a window repeatedly\n");
		goto SEND_MSG;
	}
	if(winInfor->widgetInfor)
		winInfor->widgetInfor->private=winInfor;
	hDialog=dialogCreate(winInfor->widgetInfor,dialogCB,INVALID_HANDLE);
	if(hDialog==INVALID_HANDLE)
	{
		debug_err("create window failed !!!\n");
		return INVALID_HANDLE;
	}
	msgFuncRegister(winInfor->msgDeal);
	winInfor->handle=hDialog;
SEND_MSG:
	msg.id=MSG_DIALOG_INIT;
	msg.parameter.p=argv;
	winSendMsg(winInfor->handle,&msg);
	return winInfor->handle;
}
winHandle uiOpenWinAndShow(uiWin* winInfor,uint32 argc,...)
{
#define MAX_ARG       5
	winMsg msg;
	winHandle hDialog;
	uint32 argv[MAX_ARG+1];
	uint32 i;
	va_list arglist;
	va_start(arglist, argc);
	if(argc>MAX_ARG)
		argc=MAX_ARG;
	argv[0]=argc;
	for(i=0;i<argc;i++)	
		argv[i+1]=va_arg(arglist, uint32);
	va_end(arglist);
	if(winInfor->handle!=INVALID_HANDLE)
	{
		if(winInfor->repeateOpenSupport==false)
		{
			debug_msg("window has opened,do not surport open a window repeatedly !!!\n");
			return INVALID_HANDLE;
		}
		debug_msg("open a window repeatedly\n");
		goto SEND_MSG;
	}
	if(winInfor->widgetInfor)
		winInfor->widgetInfor->private=winInfor;
	hDialog=dialogCreate(winInfor->widgetInfor,dialogCB,INVALID_HANDLE);
	if(hDialog==INVALID_HANDLE)
	{
		debug_err("create window failed !!!\n");
		return INVALID_HANDLE;
	}
	msgFuncRegister(winInfor->msgDeal);
	winInfor->handle=hDialog;
SEND_MSG:
	msg.id=MSG_DIALOG_INIT;
	msg.parameter.p=argv;
	winSendMsg(winInfor->handle,&msg);
	drawUIService(false);
	return winInfor->handle;
}


bool windowIsOpen(uiWin* winInfor)
{
	if(winInfor==NULL)
		return false;
	if(winInfor->handle)
		return true;
	return false;
}













