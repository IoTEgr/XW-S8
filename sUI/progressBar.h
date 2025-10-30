#ifndef __PROGRESS_BAR_ICON_H
#define __PROGRESS_BAR_ICON_H
#include "widget.h"

typedef struct
{
	widgetObj widget;
	uiColor color;
	uint32 percentage;
}progressBarObj;

winHandle progressBarCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle progressBarCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb);






#endif
