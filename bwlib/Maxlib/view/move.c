/****************************************************************************
        ***           ***                      MAXLIB-GRAPHIC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX VIEW MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : move.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib VIEW managemant
*               
* History     :
* 2016-09-09  : 
*      <1>.This is created by mark,set version as v0102.
*      <2>.Add basic functions.

******************************************************************************/

#include "./../max.h"
#include "./../maxlib.h"




#if  MAXLIB_MOVE_USE > 0


typedef struct Move_Node_S
{
	u32_t viewid;

	u32_t ctrl;

	s16_t destx;
	s16_t desty;

	s16_t detax;
	s16_t detay;

	s32_t (*callback)(u32_t viewid,u32_t event);
}Move_Node_T;


#define  MOVE_NODE_NUM     CFG_MOVE_NODE_NUM
#define  MOVE_NODE_IDLE    0

static Move_Node_T moveNode[MOVE_NODE_NUM];
/*******************************************************************************
* Function Name  : moveGetNode
* Description    : set move event nove
* Input          :  s32_t viewid  : view id
* Output         : 
* Return         : node num
*******************************************************************************/
static s32_t moveGetNode(u32_t viewid)
{
	s32_t i;

	for(i=0;i<MOVE_NODE_NUM;i++)
	{
		if(moveNode[i].viewid ==viewid)
			break;
	}

	return i;
}
/*******************************************************************************
* Function Name  : moveInit
* Description    : initial move event node
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveInit(void)
{
	s32_t i;

	for(i=0;i<MOVE_NODE_NUM;i++)
	{
		moveNode[i].viewid = 0;
		moveNode[i].callback = NULL;
		moveNode[i].ctrl = MOVE_EVENT_STOP;
	}

	return 0;
}
/*******************************************************************************
* Function Name  : moveUninit
* Description    : uninitial move event node
* Input          :  
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveUninit(void)
{
	return moveInit();
}
/*******************************************************************************
* Function Name  : moveStart
* Description    : start moving
* Input          :  u32_t viewid : view id
                       s16_t endx   : end x
                       s16_t endy   : end y
                       s16_t detax  : deta x
                       s16_t detay  : deta y
                       s32_t (*callback)(u32_t viewid,u32_t event) : callback when move end
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveStart(u32_t viewid,s16_t endx,s16_t endy,s16_t detax,s16_t detay,s32_t (*callback)(u32_t viewid,u32_t event))
{
	s32_t i;
	
	if(detax ==0 && detay == 0)
			return -1;

	i = moveGetNode(MOVE_NODE_IDLE);

	if(i>=MOVE_NODE_NUM)
		return -1;
	if(viewSetFlag(viewid,VIEW_CTRL_MOVE)<0)
		return -1;
    moveNode[i].viewid = viewid;
	moveNode[i].callback = callback;
	moveNode[i].destx = endx;
	moveNode[i].desty = endy;
	moveNode[i].detax = detax;
	moveNode[i].detay = detay;
	moveNode[i].ctrl = MOVE_EVENT_START;
    
	return i;
}
/*******************************************************************************
* Function Name  : moveStop
* Description    : stop moving
* Input          :  u32_t viewid : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveStop(u32_t viewid)
{
	s32_t i;

	i = moveGetNode(viewid);

	if(i>=MOVE_NODE_NUM)
		return -1;
    viewClearFlag(moveNode[i].viewid,VIEW_CTRL_MOVE);
	moveNode[i].viewid = MOVE_NODE_IDLE;
	moveNode[i].callback = NULL;
	moveNode[i].ctrl = MOVE_EVENT_STOP;

	return 0;
}
/*******************************************************************************
* Function Name  : movePuase
* Description    : pause moving
* Input          :  u32_t viewid : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t movePuase(u32_t viewid)
{
	s32_t i;

	i = moveGetNode(viewid);

	if(i>=MOVE_NODE_NUM)
		return -1;

	moveNode[i].ctrl = MOVE_EVENT_PAUSE;

	return 0;
}
/*******************************************************************************
* Function Name  : moveResume
* Description    : resume moving
* Input          :  u32_t viewid : view id
* Output         : 
* Return         : s32_t -1 : fail
                                   0 : success
*******************************************************************************/
s32_t moveResume(u32_t viewid)
{
	s32_t i;

	i = moveGetNode(viewid);

	if(i>=MOVE_NODE_NUM)
		return -1;

	moveNode[i].ctrl = MOVE_EVENT_START;

	return 0;	
}
/*******************************************************************************
* Function Name  : moveService
* Description    : moving service
* Input          :  
* Output         : 
* Return         : 
*******************************************************************************/
void moveService(void)
{
	s32_t i,ret;
	s16_t x,y;

	for(i=0;i<MOVE_NODE_NUM;i++)
	{
		if(moveNode[i].viewid ==0)
			continue;

		ret = viewGetPosition(moveNode[i].viewid,&x,&y);
		if(ret<0) // view id error
		{
			moveStop(moveNode[i].viewid);
			continue;
		}
        ret = 0;
		if(x!=moveNode[i].destx)
		{
			x += moveNode[i].detax;
			if(moveNode[i].detax<0 && x<moveNode[i].destx)
				x = moveNode[i].destx;
			else if(moveNode[i].detax>0 && x>moveNode[i].destx)
				x = moveNode[i].destx;
			ret = 1;
		}
		if(y!=moveNode[i].desty)
		{
			y+=moveNode[i].detay;
			if(moveNode[i].detay<0 && y<moveNode[i].desty)
				y = moveNode[i].desty;
			else if(moveNode[i].detay>0 && y>moveNode[i].desty)
				y = moveNode[i].desty;
			ret |= 2;
		}

		if(ret == 0)
		{
			if(moveNode[i].callback!=NULL)
				moveNode[i].callback(moveNode[i].viewid,MOVE_EVENT_STOP);
			moveStop(moveNode[i].viewid);
		}
		else
		{
			viewSetPosition(moveNode[i].viewid,x,y);
		}
	}
}








#endif

