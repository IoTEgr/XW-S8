#ifndef __itemManage_ICON_H
#define __itemManage_ICON_H
#include "widget.h"

#define MAX_ROW_NUM       10
#define MAX_COLUMN_NUM    5
#define MAX_ITEM_NUM      (MAX_ROW_NUM*MAX_COLUMN_NUM)
typedef winHandle (*itemCreateFunc)(int16 x0,int16 y0,uint16 width,uint16 height);
typedef uint32 (*getResInfor)(uint32 item,resID* image,resID* str);
typedef uint32 (*getResInforEx)(uint32 item,resID* image,resID* str,resID* selectImage,resID* selectStr);

typedef struct
{
	uint16 itemWidth;
	uint16 itemGap;
	uint16 firstItemGap;
	uint8 itemSum;
}rowItemInfor;

typedef struct
{
	widgetObj widget;
	winHandle item[MAX_ITEM_NUM];
	uint16 itemHeight;
	uint16 itemGap;
	uint16 firstItemGap;
	rowItemInfor row[MAX_ROW_NUM];
	uint8 itemSum;
	getResInfor getRes;
	getResInforEx getResEx;
	uint32 resSum;
	uint32 currentRes;
}itemManageObj;



winHandle itemManageCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id);
winHandle itemManageCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb);

uint32 itemManageSetItemHeight(winHandle hitemManage,uint16 itemHeight);
uint32 itemManageSetHeightAvgGap(winHandle hitemManage,uint16 itemHeight);
uint32 itemManageSetItemRowSumGap(winHandle hitemManage,uint16 rowSum,uint16 gap);
uint32 itemManageSetRowSum(winHandle hitemManage,uint16 itemSum,uint16 itemHeight);
uint32 itemManageSetColumnSum(winHandle hitemManage,uint8 rowNum,uint16 itemSum,uint16 itemWidth);
uint32 itemManageSetColumnSumWithGap(winHandle hitemManage,uint8 rowNum,uint16 itemSum,uint16 itemWidth,uint16 gap);
uint32 itemManageSetColumnMargin(winHandle hitemManage,uint8 rowNum,uint16 itemSum,uint16 margin);

void itemManageCreateItem(winHandle hitemManage,itemCreateFunc func,getResInfor getRes,uint32 resItemSum);
void itemManageSetResInforFuncEx(winHandle hitemManage,getResInforEx getResEx);
void itemManageUpdateRes(winHandle hitemManage,uint32 resItemSum,uint32 curResItem);
void itemManageUpdateAllitem(winHandle hitemManage);


void itemManageSetCurItem(winHandle hitemManage,uint32 itemResNum);
void itemManageUpdateCurItem(winHandle hitemManage);
void itemManageNextItem(winHandle hitemManage);
void itemManagePreItem(winHandle hitemManage);
void itemManageNextPage(winHandle hitemManage);
void itemManagePrePage(winHandle hitemManage);
uint32 itemManageGetCurrentItem(winHandle hitemManage);

void itemManageSetCharInfor(winHandle hitemManage,charFont font,uint8 strAlign,uiColor fontColor);
void itemManageSetSelectColor(winHandle hitemManage,uiColor color);
void itemManageSetSelectImage(winHandle hitemManage,resID image);
void itemManageSetUnselectColor(winHandle hitemManage,uiColor color);
void itemManageSetUnselectImage(winHandle hitemManage,resID image);

void itemManageGetTouchInfor(winHandle hitemManage,touchInfor* infor);



#endif
