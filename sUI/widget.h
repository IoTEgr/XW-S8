#ifndef __WIDGET_H
#define __WIDGET_H
#include "winManage.h"

#define    INVALID_WIDGET_ID    0xffff
typedef enum
{
	WIDGET_ITEM_MANAGE=1,
	WIDGET_TYPE_MAX
}widgetType;

typedef struct
{
	winObj   win;
	uint16    id;
	widgetType  type;
}widgetObj;


bool widgetProc(winMsg* msg);
void widgetSetType(winHandle hWin,widgetType type);
widgetType widgetGetType(winHandle hWin);
uint16 widgetGetId(winHandle hWin);
void widgetSetId(winHandle hWin,uint16 id);






#endif
