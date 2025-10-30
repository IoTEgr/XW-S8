#ifndef __MENU_ITEM_EX_H
#define __MENU_ITEM_EX_H
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
	winHandle hSelImage;
	winHandle hSelStr;
	bool select;
	//bool  visible;
}menuItemExObj;

winHandle menuItemExCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle itemCreateMenuItemEx(int16 x0,int16 y0,uint16 width,uint16 height);






#endif
