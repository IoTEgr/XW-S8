#ifndef  __FRAME_WIN_H
#define __FRAME_WIN_H
#include "widget.h"

#define WIGET_HANDLE_MAX_NUM    20
typedef struct
{
	winObj win;
	winCB cb;
	winHandle widgetHandle[WIGET_HANDLE_MAX_NUM];
	void* prvate;
}frameWinObj;

winHandle frameWinCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,winCB cb);
winHandle frameWinCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb);


#endif
