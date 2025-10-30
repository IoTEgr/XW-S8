#ifndef __IMAGE_ICON_H
#define __IMAGE_ICON_H
#include "widget.h"

typedef struct
{
	widgetObj widget;
	resID  image;
	uint8 imageAlign;
	bool  visible;
}imageIconObj;

winHandle imageIconCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle imageIconCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb);





#endif
