#ifndef __BUTTON_H
#define __BUTTON_H
#include "widget.h"

typedef struct
{
	widgetObj widget;
	resID  image;
	uint8 imageAlign;
	resID  str;
	charFont font;
	uint8 strAlign;
	uiColor fontColor;
}buttonObj;

winHandle buttonCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle buttonCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb);





#endif
