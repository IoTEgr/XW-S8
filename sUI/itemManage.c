#include "itemManage.h"


static void itemManageCB(winMsg* msg)
{
	winHandle hWin=msg->hWin;
	itemManageObj* pitemManage=(itemManageObj*)uiHandleToPtr(hWin);
	winObj* pWin=&(pitemManage->widget.win);
	touchInfor *tInfor;
	uint32 i;
	if(widgetProc(msg))
		return;
	switch(msg->id)
	{
		case MSG_WIN_CREATE:
			return;
		case MSG_WIN_PAINT:
			if(pWin->bgColor!=INVALID_COLOR)
				drawRectangle((uiRect*)(msg->parameter.p),pWin->bgColor);
			//debug_msg("paint itemManage [%d]:[%d %d %d %d]\n",pitemManage->widget.id,pWin->invaliditemManage.x0,pWin->invaliditemManage.y0,pWin->invaliditemManage.x1,pWin->invaliditemManage.y1);
			return;
		case MSG_WIN_TOUCH:
			tInfor=(touchInfor *)(msg->parameter.p);
			if(tInfor->touchState==TOUCH_PRESS)
			{
				itemManageSetCurItem(hWin,tInfor->touchItem);
			}
			break;
		case MSG_WIN_TOUCH_GET_INFOR:
			itemManageGetTouchInfor(hWin,(touchInfor *)(msg->parameter.p));
			return;
		case MSG_WIN_SELECT_INFOR_EX:
		case MSG_WIN_UNSELECT_INFOR_EX:
			for(i=0;i<pitemManage->itemSum;i++)
			{
				winSendMsg(pitemManage->item[i],msg);
			}
			return;
		default:
			break;
	}
	winDefaultProc(msg);
}

winHandle itemManageCreate(int16 x0,int16 y0,uint16 width,uint16 height,winHandle parent,uint16 style,uint16 id)
{
	winHandle hitemManage;
	hitemManage=winCreate(x0,y0,width,height,parent,itemManageCB,sizeof(itemManageObj),WIN_WIDGET|style);
	widgetSetId(hitemManage,id);
	widgetSetType(hitemManage,WIDGET_ITEM_MANAGE);
	return hitemManage;
}

winHandle itemManageCreateIndirect(widgetCreateInfor* infor,winHandle parent,winCB cb)
{
	winHandle hitemManage;
	itemManageObj* pitemManage;
	hitemManage = itemManageCreate(infor->x0,infor->y0,infor->width,infor->height,parent,infor->style|WIN_TOUCH_SUPPORT,infor->id);
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pitemManage->currentRes=0xffffffff;
		winSetbgColor(hitemManage, infor->bgColor);
		//pitemManage->widgetBegin=INVALID_WIDGET_ID-50;
	}
	return hitemManage;
}


static void itemManageSetDefaultWidth(itemManageObj* pitemManage,uint16 width)
{
	int i;
	for(i=0;i<MAX_ROW_NUM;i++)
	{
		pitemManage->row[i].firstItemGap=0;
		pitemManage->row[i].itemGap=0;
		pitemManage->row[i].itemWidth=width;
		pitemManage->row[i].itemSum=1;
	}
}
uint32 itemManageSetItemHeight(winHandle hitemManage,uint16 itemHeight)
{
	itemManageObj* pitemManage;
	winObj* pWin;
	uint16 winHeight;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return 0;
	if(hitemManage!=INVALID_HANDLE)
	{
		itemHeight=((itemHeight)*LCD_HEIGHT/OSD_HEIGHT);
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pWin=&(pitemManage->widget.win);
		winHeight=pWin->rect.y1-pWin->rect.y0+1;
		if(itemHeight>winHeight)
			return 0;
		itemManageSetDefaultWidth(pitemManage,pWin->rect.x1-pWin->rect.x0+1);
		pitemManage->itemHeight=itemHeight;
		pitemManage->itemSum=winHeight/itemHeight;
		if(pitemManage->itemSum>MAX_ROW_NUM)
			pitemManage->itemSum=MAX_ROW_NUM;
		if(pitemManage->itemSum==1)
		{
			pitemManage->itemGap=0;
			pitemManage->firstItemGap=(winHeight-pitemManage->itemHeight)>>1;
		}
		else
		{
			pitemManage->itemGap=(winHeight%itemHeight)/(pitemManage->itemSum-1);
			if(pitemManage->itemGap==0)
				pitemManage->itemSum--;
			if(pitemManage->itemSum==1)
			{
				pitemManage->itemGap=0;
				pitemManage->firstItemGap=(winHeight-pitemManage->itemHeight)>>1;
			}
			else
			{
				pitemManage->itemGap=(winHeight-itemHeight*pitemManage->itemSum)/(pitemManage->itemSum-1);
				pitemManage->firstItemGap=(winHeight-itemHeight*pitemManage->itemSum)%(pitemManage->itemSum-1)>>1;
			}
		}
		return pitemManage->itemSum;
	}
	return 0;
}
uint32 itemManageSetItemRowSumGap(winHandle hitemManage,uint16 rowSum,uint16 gap)
{
	itemManageObj* pitemManage;
	winObj* pWin;
	uint16 winHeight,itemHeight;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return 0;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pWin=&(pitemManage->widget.win);
		winHeight=pWin->rect.y1-pWin->rect.y0+1;
		if(rowSum>MAX_ROW_NUM)
			rowSum=MAX_ROW_NUM;
		itemHeight=winHeight/rowSum;
		itemHeight-=gap;
		if(itemHeight==0)
			return 0;
		itemManageSetDefaultWidth(pitemManage,pWin->rect.x1-pWin->rect.x0+1);
		pitemManage->itemHeight=itemHeight;
		pitemManage->itemSum=rowSum;
		pitemManage->itemGap=gap; 
		pitemManage->firstItemGap=0;
		return pitemManage->itemSum;
	}
	return 0;
}

uint32 itemManageSetHeightAvgGap(winHandle hitemManage,uint16 itemHeight)
{
	itemManageObj* pitemManage;
	winObj* pWin;
	uint16 winHeight;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return 0;
	if(hitemManage!=INVALID_HANDLE)
	{
		itemHeight=((itemHeight)*LCD_HEIGHT/OSD_HEIGHT);
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pWin=&(pitemManage->widget.win);
		winHeight=pWin->rect.y1-pWin->rect.y0+1;
		if(itemHeight>winHeight)
			return 0;
		itemManageSetDefaultWidth(pitemManage,pWin->rect.x1-pWin->rect.x0+1);
		pitemManage->itemHeight=itemHeight;
		pitemManage->itemSum=winHeight/itemHeight;
		if(pitemManage->itemSum>MAX_ROW_NUM)
			pitemManage->itemSum=MAX_ROW_NUM;
		pitemManage->itemGap=(winHeight%itemHeight)/(pitemManage->itemSum+1);
		pitemManage->firstItemGap=pitemManage->itemGap;
		if(pitemManage->itemGap==0&&pitemManage->itemSum>1)
		{
			pitemManage->itemSum--;
			pitemManage->itemGap=(winHeight-itemHeight*pitemManage->itemSum)/(pitemManage->itemSum+1);
			pitemManage->firstItemGap=pitemManage->itemGap;
		}
		return pitemManage->itemSum;
	}
	return 0;
}

uint32 itemManageSetRowSum(winHandle hitemManage,uint16 itemSum,uint16 itemHeight)
{
	itemManageObj* pitemManage;
	winObj* pWin;
	uint16 winHeight;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return 0;
	if(hitemManage!=INVALID_HANDLE)
	{
		itemHeight=((itemHeight)*LCD_HEIGHT/OSD_HEIGHT);
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pWin=&(pitemManage->widget.win);
		winHeight=pWin->rect.y1-pWin->rect.y0+1;
		if(itemSum>MAX_ROW_NUM)
			itemSum=MAX_ROW_NUM;
		if(itemSum==0||itemHeight==0)
			return 0;
		itemManageSetDefaultWidth(pitemManage,pWin->rect.x1-pWin->rect.x0+1);
		/*
		if(itemSum*itemHeight>(winHeight-2*(itemSum+1)))
			pitemManage->itemHeight=(winHeight-2*(itemSum+1))/itemSum;
		else
			pitemManage->itemHeight=itemHeight;
		*/
		pitemManage->itemHeight=itemHeight;
		pitemManage->itemSum=itemSum;
		pitemManage->itemGap=(winHeight-pitemManage->itemHeight*itemSum)/(pitemManage->itemSum+1);
		pitemManage->firstItemGap=pitemManage->itemGap;
		return pitemManage->itemSum;
	}
	return 0;
}

uint32 itemManageSetColumnSum(winHandle hitemManage,uint8 rowNum,uint16 itemSum,uint16 itemWidth)
{
	itemManageObj* pitemManage;
	winObj* pWin;
	uint16 winWidth;
	rowItemInfor* rowInfor;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return 0;
	if(hitemManage!=INVALID_HANDLE)
	{
		itemWidth=((itemWidth)*LCD_WIDTH/OSD_WIDTH);
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pWin=&(pitemManage->widget.win);
		if(rowNum>=pitemManage->itemSum)
			return 0;
		if(itemSum==0||itemWidth==0)
			return 0;
		winWidth=pWin->rect.x1-pWin->rect.x0+1;
		if(itemSum>MAX_COLUMN_NUM)
			itemSum=MAX_COLUMN_NUM;
		rowInfor=&(pitemManage->row[rowNum]);
		if(itemSum*itemWidth>(itemWidth-2*(itemSum+1)))
			rowInfor->itemWidth=(winWidth-2*(itemSum+1))/itemSum;
		else
			rowInfor->itemWidth=itemWidth;
		rowInfor->itemSum=itemSum;
		rowInfor->itemGap=(winWidth-rowInfor->itemWidth*itemSum)/(rowInfor->itemSum+1);
		rowInfor->firstItemGap=rowInfor->itemGap;
		return rowInfor->itemSum;
	}
	return 0;
}

uint32 itemManageSetColumnSumWithGap(winHandle hitemManage,uint8 rowNum,uint16 itemSum,uint16 itemWidth,uint16 gap)
{
	itemManageObj* pitemManage;
	winObj* pWin;
	uint16 winWidth;
	rowItemInfor* rowInfor;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return 0;
	if(hitemManage!=INVALID_HANDLE)
	{
		itemWidth=((itemWidth)*LCD_WIDTH/OSD_WIDTH);
		gap=((gap)*LCD_WIDTH/OSD_WIDTH);
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pWin=&(pitemManage->widget.win);
		if(rowNum>=pitemManage->itemSum)
			return 0;
		if(itemSum==0||itemWidth==0)
			return 0;
		winWidth=pWin->rect.x1-pWin->rect.x0+1;
		if(itemSum>MAX_COLUMN_NUM)
			itemSum=MAX_COLUMN_NUM;
		rowInfor=&(pitemManage->row[rowNum]);
		if(itemSum*(itemWidth+gap)>winWidth)
		{
			rowInfor->itemWidth=(winWidth-2*(itemSum-1))/itemSum;
			gap=2;
		}
		else
			rowInfor->itemWidth=itemWidth;
		rowInfor->itemSum=itemSum;
		rowInfor->itemGap=gap;
		rowInfor->firstItemGap=(winWidth-rowInfor->itemWidth*itemSum-gap*(itemSum-1))>>1;
		return rowInfor->itemSum;
	}
	return 0;
}
uint32 itemManageSetColumnMargin(winHandle hitemManage,uint8 rowNum,uint16 itemSum,uint16 margin)
{
	itemManageObj* pitemManage;
	winObj* pWin;
	uint16 winWidth;
	rowItemInfor* rowInfor;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return 0;
	if(hitemManage!=INVALID_HANDLE)
	{
		margin=((margin)*LCD_WIDTH/OSD_WIDTH);
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pWin=&(pitemManage->widget.win);
		if(rowNum>=pitemManage->itemSum)
			return 0;
		winWidth=pWin->rect.x1-pWin->rect.x0+1;
		if((margin<<1)>winWidth)
			return 0;
		if(itemSum>MAX_COLUMN_NUM)
			itemSum=MAX_COLUMN_NUM;
		rowInfor=&(pitemManage->row[rowNum]);
		rowInfor->itemWidth=(winWidth-(margin<<1))/itemSum;
		if(rowInfor->itemWidth==0)
			rowInfor->itemWidth=winWidth/itemSum;
		rowInfor->itemSum=itemSum;
		rowInfor->itemGap=0;
		rowInfor->firstItemGap=(winWidth-rowInfor->itemWidth*itemSum)>>1;
		return rowInfor->itemSum;
	}
	return 0;
}


static void itemManageCreateRowItem(itemManageObj* pitemManage,uint8 rowNum,itemCreateFunc func,int16 x,int16 y)
{
	uint8 i;
	if(pitemManage->itemSum>=MAX_ITEM_NUM)
			return;
	x+=pitemManage->row[rowNum].firstItemGap;
	for(i=0;i<pitemManage->row[rowNum].itemSum;i++)
	{
		pitemManage->item[pitemManage->itemSum]=func(x,y,pitemManage->row[rowNum].itemWidth,pitemManage->itemHeight);
		pitemManage->itemSum++;
		if(pitemManage->itemSum>=MAX_ITEM_NUM)
			break;
		x+=pitemManage->row[rowNum].itemGap+pitemManage->row[rowNum].itemWidth;
	}
}
void itemManageCreateItem(winHandle hitemManage,itemCreateFunc func,getResInfor getRes,uint32 resItemSum)
{
	itemManageObj* pitemManage;
	uint8 i,rowSum;
	uiRect pos;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		winGetRelativePos(hitemManage,&pos);
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pitemManage->getRes=getRes;
		pitemManage->resSum=resItemSum;
		if(func==NULL)
			return;
		pos.y0+=pitemManage->firstItemGap;
		rowSum=pitemManage->itemSum;
		pitemManage->itemSum=0;
		for(i=0;i<rowSum;i++)
		{
			//pitemManage->item[i]=func(pos.x0,pos.y0,pos.x1-pos.x0+1,pitemManage->itemHeight);
			itemManageCreateRowItem(pitemManage,i,func,pos.x0,pos.y0);
			pos.y0+=pitemManage->itemHeight+pitemManage->itemGap;
			if(pitemManage->itemSum>=MAX_ITEM_NUM)
				break;
		}
	}
}
void itemManageUpdateRes(winHandle hitemManage,uint32 resItemSum,uint32 curResItem)
{
	itemManageObj* pitemManage;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		if(pitemManage->currentRes<pitemManage->resSum)
			winSendMsgNoData(pitemManage->item[pitemManage->currentRes%pitemManage->itemSum],MSG_WIN_UNSELECT);
		pitemManage->resSum=resItemSum;
		pitemManage->currentRes=0xffffffff;
		itemManageSetCurItem(hitemManage,curResItem);
	}
}
void itemManageUpdateAllitem(winHandle hitemManage)
{
	itemManageObj* pitemManage;
	uint32 curResItem;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		curResItem=pitemManage->currentRes;
		pitemManage->currentRes=0xffffffff;
		itemManageSetCurItem(hitemManage,curResItem);
	}
}

void itemManageSetResInforFuncEx(winHandle hitemManage,getResInforEx getResEx)
{
	itemManageObj* pitemManage;
	uiRect pos;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		winGetRelativePos(hitemManage,&pos);
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		pitemManage->getResEx=getResEx;
	}
}

void itemManageSetCurItem(winHandle hitemManage,uint32 itemResNum)
{
	itemManageObj* pitemManage;
	uint32 itemResStart,itemResEnd,i,cnt;
	resID image,str,selectImage,selectStr;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		if(itemResNum>=pitemManage->resSum)
			return;
		itemResStart=(itemResNum/pitemManage->itemSum)*pitemManage->itemSum;
		itemResEnd=itemResStart+pitemManage->itemSum-1;
		if(itemResEnd>=pitemManage->resSum)
			itemResEnd=pitemManage->resSum-1;
		if(pitemManage->currentRes<itemResStart||pitemManage->currentRes>itemResEnd)
		{
			cnt=0;
			if(pitemManage->getResEx)
			{
				for(i=itemResStart;i<=itemResEnd;i++)
				{
					pitemManage->getResEx(i,&image,&str,&selectImage,&selectStr);
					winSendMsgNoData(pitemManage->item[cnt],MSG_WIN_INVALID_RESID);
					if(image!=INVALID_RES_ID)
						winSetResid(pitemManage->item[cnt],image);
					if(str!=INVALID_RES_ID)
						winSetResid(pitemManage->item[cnt],str);
					if(selectImage!=INVALID_RES_ID)
						winSetItemSelResid(pitemManage->item[cnt],selectImage);
					if(selectStr!=INVALID_RES_ID)
						winSetItemSelResid(pitemManage->item[cnt],selectStr);
					cnt++;
				}
			}
			else if(pitemManage->getRes)
			{
				for(i=itemResStart;i<=itemResEnd;i++)
				{
					pitemManage->getRes(i,&image,&str);
					winSendMsgNoData(pitemManage->item[cnt],MSG_WIN_INVALID_RESID);
					if(image!=INVALID_RES_ID)
						winSetResid(pitemManage->item[cnt],image);
					if(str!=INVALID_RES_ID)
						winSetResid(pitemManage->item[cnt],str);
					cnt++;
				}
			}
			for(i=0;i<pitemManage->itemSum;i++)
				winSetVisible(pitemManage->item[i],true);
			cnt=itemResEnd-itemResStart+1;
			if(cnt<pitemManage->itemSum)
			{
				for(i=cnt;i<pitemManage->itemSum;i++)
				{
					//winSetVisible(pitemManage->item[i],false);
					winSendMsgNoData(pitemManage->item[i],MSG_WIN_INVALID_RESID);
				}
			}
		}
		if(pitemManage->currentRes<pitemManage->resSum)
			winSendMsgNoData(pitemManage->item[pitemManage->currentRes%pitemManage->itemSum],MSG_WIN_UNSELECT);
		pitemManage->currentRes=itemResNum;
		winSendMsgNoData(pitemManage->item[pitemManage->currentRes-itemResStart],MSG_WIN_SELECT);
	}
	return ;
}
void itemManageUpdateCurItem(winHandle hitemManage)
{
	itemManageObj* pitemManage;
	uint32 itemNum;
	resID image,str,selectImage,selectStr;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		if(pitemManage->currentRes>=pitemManage->resSum)
			return;
		itemNum=pitemManage->currentRes%pitemManage->itemSum;
		if(pitemManage->getResEx)
		{
			pitemManage->getResEx(pitemManage->currentRes,&image,&str,&selectImage,&selectStr);
			winSendMsgNoData(pitemManage->item[itemNum],MSG_WIN_INVALID_RESID);
			if(image!=INVALID_RES_ID)
				winSetResid(pitemManage->item[itemNum],image);
			if(str!=INVALID_RES_ID)
				winSetResid(pitemManage->item[itemNum],str);
			if(selectImage!=INVALID_RES_ID)
				winSetItemSelResid(pitemManage->item[itemNum],selectImage);
			if(selectStr!=INVALID_RES_ID)
				winSetItemSelResid(pitemManage->item[itemNum],selectStr);
		}
		else if(pitemManage->getRes)
		{
			pitemManage->getRes(pitemManage->currentRes,&image,&str);
			winSendMsgNoData(pitemManage->item[itemNum],MSG_WIN_INVALID_RESID);
			if(image!=INVALID_RES_ID)
				winSetResid(pitemManage->item[itemNum],image);
			if(str!=INVALID_RES_ID)
				winSetResid(pitemManage->item[itemNum],str);
		}
		winSendMsgNoData(pitemManage->item[itemNum],MSG_WIN_UNSELECT);
		winSendMsgNoData(pitemManage->item[itemNum],MSG_WIN_SELECT);
	}
	return ;
}
void itemManageNextItem(winHandle hitemManage)
{
	itemManageObj* pitemManage;
	uint32 itemResNum;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		itemResNum=pitemManage->currentRes+1;
		if(itemResNum>=pitemManage->resSum)
			itemResNum=0;
		itemManageSetCurItem(hitemManage,itemResNum);	
	}
}
void itemManagePreItem(winHandle hitemManage)
{
	itemManageObj* pitemManage;
	uint32 itemResNum;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		itemResNum=pitemManage->currentRes-1;
		if(itemResNum>=pitemManage->resSum)
			itemResNum=pitemManage->resSum-1;
		itemManageSetCurItem(hitemManage,itemResNum);	
	}
}
void itemManageNextPage(winHandle hitemManage)
{
	itemManageObj* pitemManage;
	uint32 itemResNum;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		if(pitemManage->resSum<=pitemManage->itemSum)
			return;
		itemResNum=(pitemManage->currentRes/pitemManage->itemSum)*pitemManage->itemSum+pitemManage->itemSum;
		if(itemResNum>=pitemManage->resSum)
			itemResNum=0;
		itemManageSetCurItem(hitemManage,itemResNum);	
	}
}
void itemManagePrePage(winHandle hitemManage)
{
	itemManageObj* pitemManage;
	uint32 itemResNum,itemResStart;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		if(pitemManage->resSum<=pitemManage->itemSum)
			return;
		itemResStart=(pitemManage->currentRes/pitemManage->itemSum)*pitemManage->itemSum;
		if(itemResStart==0)
			itemResNum=(pitemManage->resSum/pitemManage->itemSum)*pitemManage->itemSum;
		else
			itemResNum=itemResStart-pitemManage->itemSum;
		itemManageSetCurItem(hitemManage,itemResNum);	
	}
}

uint32 itemManageGetCurrentItem(winHandle hitemManage)
{
	itemManageObj* pitemManage;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return 0;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		if(pitemManage->currentRes>=pitemManage->resSum)
			return 0;
		return pitemManage->currentRes;
	}
	return 0;
}

void itemManageSetCharInfor(winHandle hitemManage,charFont font,uint8 strAlign,uiColor fontColor)
{
	itemManageObj* pitemManage;
	uint32 i;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		for(i=0;i<pitemManage->itemSum;i++)
			winSetStrInfor(pitemManage->item[i],font,strAlign,fontColor);
	}
}

void itemManageSetSelectColor(winHandle hitemManage,uiColor color)
{
	itemManageObj* pitemManage;
	uint32 i;
	resInfor infor;
	resInforInit(&infor);
	infor.color=color;
	winMsg msg;
	msg.id=MSG_WIN_SELECT_INFOR;
	msg.parameter.p=&infor;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		for(i=0;i<pitemManage->itemSum;i++)
			winSendMsg(pitemManage->item[i],&msg);
	}
}
void itemManageSetSelectImage(winHandle hitemManage,resID image)
{
	itemManageObj* pitemManage;
	uint32 i;
	resInfor infor;
	resInforInit(&infor);
	infor.image=image;
	winMsg msg;
	msg.id=MSG_WIN_SELECT_INFOR;
	msg.parameter.p=&infor;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		for(i=0;i<pitemManage->itemSum;i++)
			winSendMsg(pitemManage->item[i],&msg);
	}
}
void itemManageSetUnselectColor(winHandle hitemManage,uiColor color)
{
	itemManageObj* pitemManage;
	uint32 i;
	resInfor infor;
	resInforInit(&infor);
	infor.color=color;
	winMsg msg;
	msg.id=MSG_WIN_UNSELECT_INFOR;
	msg.parameter.p=&infor;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		for(i=0;i<pitemManage->itemSum;i++)
			winSendMsg(pitemManage->item[i],&msg);
	}
}
void itemManageSetUnselectImage(winHandle hitemManage,resID image)
{
	itemManageObj* pitemManage;
	uint32 i;
	resInfor infor;
	resInforInit(&infor);
	infor.image=image;
	winMsg msg;
	msg.id=MSG_WIN_UNSELECT_INFOR;
	msg.parameter.p=&infor;
	if(widgetGetType(hitemManage)!=WIDGET_ITEM_MANAGE)
		return ;
	if(hitemManage!=INVALID_HANDLE)
	{
		pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
		for(i=0;i<pitemManage->itemSum;i++)
			winSendMsg(pitemManage->item[i],&msg);
	}
}

void itemManageGetTouchInfor(winHandle hitemManage,touchInfor* infor)
{
	uint32 itemResStart,itemResNum;
	winObj* pWin;
	itemManageObj* pitemManage;
	uint32 i=0;

	pitemManage=(itemManageObj*)uiHandleToPtr(hitemManage);
	for(i=0;i<pitemManage->itemSum;i++)
	{
		pWin=(winObj*)uiHandleToPtr(pitemManage->item[i]);
		if(winContain(&(pWin->rect),&(infor->touchArea)))
			break;
	}
	itemResStart=(pitemManage->currentRes/pitemManage->itemSum)*pitemManage->itemSum;
	itemResNum=itemResStart+i;
	if(i>=pitemManage->itemSum||itemResNum>=pitemManage->resSum)
		return ;
	infor->touchWin=pitemManage->widget.win.parent;
	infor->touchHandle=hitemManage;
	infor->touchID=pitemManage->widget.id;
	infor->touchItem=itemResNum;
}



