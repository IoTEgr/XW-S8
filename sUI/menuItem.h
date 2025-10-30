#ifndef __MENU_ITEM_H
#define __MENU_ITEM_H
#include "widget.h"

typedef struct
{
	widgetObj widget;
	resID  image;
	resID  selectImage;
	uiColor color;
	uiColor selectColor;
	winHandle hImage;
	winHandle hStr;
	bool select;
	//bool  visible;
}menuItemObj;

winHandle menuItemCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle itemCreateMenuItem(int16 x0,int16 y0,uint16 width,uint16 height);






#endif
