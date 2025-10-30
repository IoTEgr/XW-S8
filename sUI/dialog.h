#ifndef __DIALOG_H
#define __DIALOG_H
#include "widget.h"


winHandle dialogCreate(widgetCreateInfor* pWidgets,winCB cb,winHandle parent);
winHandle dialogItem(winHandle hDialog,uint16 id);





#endif
