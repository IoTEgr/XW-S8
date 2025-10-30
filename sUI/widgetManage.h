#ifndef __WIDGETMANAGE_H
#define __WIDGETMANAGE_H
#include "widget.h"


typedef resID (*getResByID)(uint16 id);


typedef struct
{
	widgetObj widget;
	uint16    minID;
	uint16    maxID;
	uint16    curID;
	getResByID  getRes;
}widgetManageObj;

winHandle widgetManageCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle widgetManageCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb);
void widgetManageUpdateAll(winHandle hWin);
void widgetManageUpdateCur(winHandle hWin);
void widgetManageNext(winHandle hWin);
void widgetManagePre(winHandle hWin);
void widgetManageSelect(winHandle hWin,uint16 id);
uint16 widgetManageCurId(winHandle hWin);





#endif
