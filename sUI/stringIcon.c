#include "stringIcon.h"


static void stringIconCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	stringIconObj* pstringIcon=(stringIconObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pstringIcon->widget.win);
	strInfor* stringInfor;
	resInfor* infor;
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			return;
		case MSG_WIN_PAINT:
			if(pstringIcon->str!=INVALID_RES_ID)
			{

				bool bk_font = get_ebook_font();
				if (pWin->style & WIN_UNI_STR)
				{
					set_ebook_font(true);
				}
				if(pstringIcon->select)
					drawString(&pWin->rect,(uiRect*)(msg->parameter.p),pstringIcon->str,pstringIcon->selectInfor.font,pstringIcon->selectInfor.strAlign,pstringIcon->selectInfor.fontColor,pstringIcon->selectInfor.bgColor,pstringIcon->selectInfor.rimColor);
				else
					drawString(&pWin->rect,(uiRect*)(msg->parameter.p),pstringIcon->str,pstringIcon->font,pstringIcon->strAlign,pstringIcon->fontColor,pWin->bgColor,pstringIcon->rimColor);

				set_ebook_font(bk_font);
			}
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
		case MSG_WIN_UNSELECT:
			if(pstringIcon->select==false)
				return;
			pstringIcon->select=false;
			if(winIsVisible(hWin))
			{
				if(pWin->bgColor==INVALID_COLOR)
					winChangeNotify(hWin);
				else
					winUpdate(hWin);
			}
			return;
		case MSG_WIN_SELECT:
			if(pstringIcon->select==true)
				return;
			pstringIcon->select=true;
			if(winIsVisible(hWin))
			{
				if(pWin->bgColor==INVALID_COLOR)
					winChangeNotify(hWin);
				else
					winUpdate(hWin);
			}
			return;
		case MSG_WIN_CHANGE_RESID:
			if(resIdIsStr(pstringIcon->str)==0&&pstringIcon->str==msg->parameter.v)
				return;
			pstringIcon->str=msg->parameter.v;
			if(winIsVisible(hWin))
			{
				if(pWin->bgColor==INVALID_COLOR)
					winChangeNotify(hWin);
				else
					winUpdate(hWin);
			}
			return;
		case MSG_WIN_SELECT_INFOR_EX:
			infor=(resInfor*)(msg->parameter.p);
			pstringIcon->selectInfor.fontColor=infor->fontColor;
			pstringIcon->selectInfor.strAlign=infor->strAlign;
			pstringIcon->selectInfor.font=infor->font;
			pstringIcon->selectInfor.bgColor=infor->bgColor;
			pstringIcon->selectInfor.rimColor=infor->rimColor;
			return;
		case MSG_WIN_UNSELECT_INFOR_EX:
			infor=(resInfor*)(msg->parameter.p);
			pstringIcon->fontColor=infor->fontColor;
			pstringIcon->strAlign=infor->strAlign;
			pstringIcon->font=infor->font;
			pWin->bgColor=infor->bgColor;
			pstringIcon->rimColor=infor->rimColor;
			return;
		case MSG_WIN_CHANGE_STRINFOR:
			stringInfor=(strInfor*)(msg->parameter.p);
			pstringIcon->fontColor=stringInfor->fontColor;
			pstringIcon->strAlign=stringInfor->strAlign;
			pstringIcon->font=stringInfor->font;
			pstringIcon->selectInfor.fontColor=stringInfor->fontColor;
			pstringIcon->selectInfor.strAlign=stringInfor->strAlign;
			pstringIcon->selectInfor.font=stringInfor->font;
			winUpdate(hWin);
			return;
		/*case MSG_WIN_UNSELECT_INFOR:
			stringInfor=(strInfor*)(msg->parameter.p);
			pstringIcon->fontColor=stringInfor->fontColor;
			pstringIcon->strAlign=stringInfor->strAlign;
			pstringIcon->font=stringInfor->font;
			return;
		case MSG_WIN_SELECT_INFOR:
			stringInfor=(strInfor*)(msg->parameter.p);
			pstringIcon->selectInfor.fontColor=stringInfor->fontColor;
			pstringIcon->selectInfor.strAlign=stringInfor->strAlign;
			pstringIcon->selectInfor.font=stringInfor->font;
			return;*/
		case MSG_WIN_TOUCH:
			break;
		case MSG_WIN_TOUCH_GET_INFOR:
			((touchInfor *)(msg->parameter.p))->touchWin=pWin->parent;
			((touchInfor *)(msg->parameter.p))->touchHandle=hWin;
			((touchInfor *)(msg->parameter.p))->touchID=pstringIcon->widget.id;
			((touchInfor *)(msg->parameter.p))->touchItem=0;
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle stringIconCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hstringIcon;
	stringIconObj* pstringIcon;
	hstringIcon=winCreate(x0,y0,width,height,parent,stringIconCB,sizeof(stringIconObj),WIN_WIDGET|style);
	widgetSetId(hstringIcon,id);
	if(hstringIcon!=INVALID_HANDLE)
	{
		pstringIcon=(stringIconObj*)uiHandleToPtr(hstringIcon);
		pstringIcon->str=INVALID_RES_ID;
		pstringIcon->fontColor=DEFAULT_COLOR;
		pstringIcon->rimColor=INVALID_COLOR;
		pstringIcon->selectInfor.bgColor=INVALID_COLOR;
		pstringIcon->selectInfor.rimColor=INVALID_COLOR;
		winSetbgColor(hstringIcon, INVALID_COLOR);
		pstringIcon->select=false;
	}
	return hstringIcon;
}

winHandle stringIconCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb)
{
	winHandle hstringIcon;
	stringIconObj* pstringIcon;
	hstringIcon = stringIconCreate(infor->x0,infor->y0,infor->width,infor->height,parent,infor->style|WIN_TOUCH_SUPPORT,infor->id);
	if(hstringIcon!=INVALID_HANDLE)
	{
		pstringIcon=(stringIconObj*)uiHandleToPtr(hstringIcon);
		pstringIcon->str=infor->str;
		pstringIcon->strAlign=infor->strAlign;
		pstringIcon->fontColor=infor->fontColor;
		pstringIcon->font=infor->font;
		winSetbgColor(hstringIcon, infor->bgColor);
		pstringIcon->selectInfor.font=infor->fontS;
		pstringIcon->selectInfor.strAlign=infor->strAlignS;
		pstringIcon->selectInfor.fontColor=infor->fontColorS;
		pstringIcon->selectInfor.bgColor=infor->bgColorS;
		pstringIcon->rimColor=infor->rimColor;
	}
	return hstringIcon;
}


