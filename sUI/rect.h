#ifndef __RECT_ICON_H
#define __RECT_ICON_H
#include "widget.h"

typedef struct
{
	widgetObj widget;
	uiColor color;
	uiColor selectColor;
	bool select;
}rectObj;

winHandle rectCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle rectCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb);





#endif
