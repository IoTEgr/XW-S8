#include "button.h"


static void buttonCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	buttonObj* pButton=(buttonObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pButton->widget.win);
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			debug_msg("button win create\n");
			return;
		case MSG_WIN_PAINT:
			drawRectangle((uiRect*)(msg->parameter.p),pWin->bgColor);
			if(pButton->image!=INVALID_RES_ID)
				drawImage(&pWin->rect,(uiRect*)(msg->parameter.p),pButton->image,pButton->imageAlign,INVALID_COLOR);
			if(pButton->str!=INVALID_RES_ID)
				drawString(&pWin->rect,(uiRect*)(msg->parameter.p),pButton->str,pButton->font,pButton->strAlign,pButton->fontColor,INVALID_COLOR,INVALID_COLOR);
			//debug_msg("paint button [%d]:[%d %d %d %d]\n",pButton->widget.id,pWin->invalidRect.x0,pWin->invalidRect.y0,pWin->invalidRect.x1,pWin->invalidRect.y1);
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

winHandle buttonCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hButton;
	hButton=winCreate(x0,y0,width,height,parent,buttonCB,sizeof(buttonObj),WIN_WIDGET|style);
	widgetSetId(hButton,id);
	return hButton;
}

winHandle buttonCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb)
{
	winHandle hButton;
	buttonObj* pButton;
	hButton = buttonCreate(infor->x0,infor->y0,infor->width,infor->height,parent,infor->style|WIN_TOUCH_SUPPORT,infor->id);
	if(hButton!=INVALID_HANDLE)
	{
		pButton=(buttonObj*)uiHandleToPtr(hButton);
		pButton->image=infor->image;
		pButton->str=infor->str;
		pButton->imageAlign=infor->imageAlign;
		pButton->strAlign=infor->strAlign;
		pButton->fontColor=infor->fontColor;
		pButton->font=infor->font;
		if(infor->bgColor!=INVALID_COLOR)
			winSetbgColor(hButton, infor->bgColor);
	}
	return hButton;
}




