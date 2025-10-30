/****************************************************************************
        ***           ***                      MAXLIB-GRAPHIC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX RPOCESS BAR MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : process_bar_style1.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib process bar style1 managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#include "VIEW.h"
#include "progress_bar.h"
#include "../res/R.h"
#include "draw.h"
//#include "font.h"

#define PROGRESS_BAR_STYLE1_HIEGHT    2

#if MAXLIB_PROGRESS_USE > 0

static s32_t Style1Panle,Styple1TotalProgress,Styple1CurProgress,Style1Info;
static u32_t PROGRESS_TOTAL_WIDTH;

s32_t progressbar_style1Init(void)
{
	Style1Panle = 0;
	Styple1TotalProgress=0;
	Styple1CurProgress = 0;
	PROGRESS_TOTAL_WIDTH = 0;
	Style1Info = 0;
    
    return 0;
}

s32_t progressbar_style1Create(char *title)
{
	u16_t x,y,w,h;
	
	progressbar_style1Init();
	
	Style1Panle               = viewRegister();
	Styple1TotalProgress  = viewRegister();
	Styple1CurProgress     = viewRegister();
	Style1Info                = viewRegister();

    viewSetType(Style1Panle,VIEW_TYPE_DRAWRRECT);
	viewSetType(Styple1TotalProgress,VIEW_TYPE_DRAWRECT);
	viewSetType(Styple1CurProgress,VIEW_TYPE_DRAWRECT);
	viewSetType(Style1Info,VIEW_TYPE_TEXT);

	viewSetFillColor(Style1Panle,R_COLOR_GRAY2);
	viewSetFillColor(Styple1TotalProgress,R_COLOR_GRAY);
	viewSetFillColor(Styple1CurProgress,R_COLOR_BLUE);
	viewSetFillColor(Style1Info,R_COLOR_WHITE);

	w = R_SCREEN_WIDTH*2/3;
	h = 28;

	x = (R_SCREEN_WIDTH-w)>>1;
	y = R_SCREEN_HEIGHT-2*h;
	PROGRESS_TOTAL_WIDTH = w-16;

	viewSetSize(Style1Panle,w,h);
	viewSetSize(Styple1TotalProgress,PROGRESS_TOTAL_WIDTH,PROGRESS_BAR_STYLE1_HIEGHT);
	viewSetSize(Styple1CurProgress,0,PROGRESS_BAR_STYLE1_HIEGHT);

	viewSetPosition(Style1Panle,x,y);
	viewSetPosition(Styple1TotalProgress,x+8,y+((h-PROGRESS_BAR_STYLE1_HIEGHT)>>1));
	viewSetPosition(Styple1CurProgress,x+8,y+((h-PROGRESS_BAR_STYLE1_HIEGHT)>>1));
    viewSetPosition(Style1Info,x+90,y+2);
	return 0;
}

s32_t progressbar_style1Show(u32_t handle)
{
	char *str;
	
	if(handle != PROGRESS_BAR_TYPE_STYLE1)
		return -1;

	viewSetVisiable(Style1Panle,1);
	viewSetVisiable(Styple1TotalProgress,1);
	viewSetVisiable(Styple1CurProgress,1);
	str = viewGetText(Style1Info);
	if(str != NULL && str[0] != 0)
		viewSetVisiable(Style1Info,1);

	return 0;
}

s32_t progressbar_style1Destory(u32_t handle)
{
	if(handle != PROGRESS_BAR_TYPE_STYLE1)
		return -1;
	
	viewRelease(Style1Panle);
	viewRelease(Styple1TotalProgress);
	viewRelease(Styple1CurProgress);
    viewRelease(Style1Info);
	
	return 0;
}

s32_t progressbar_style1SetProgress(u32_t handle,s32_t progress)
{
	u32_t width;

	if(handle != PROGRESS_BAR_TYPE_STYLE1)
		return -1;

	if(progress>100)
		progress = 100;
	else if(progress<0)
		progress = 0;

	width = (PROGRESS_TOTAL_WIDTH*progress)/100;
	viewSetSize(Styple1CurProgress,width,PROGRESS_BAR_STYLE1_HIEGHT);

	return progress;
}

s32_t progressbar_style1IOCtrl(u32_t handle,u32_t op,u32_t para)
{
	
	if(handle != PROGRESS_BAR_TYPE_STYLE1)
		return -1;

	if(op == PROGRESSBAR_OP_DEFAULT)
	{
		return progressbar_style1SetProgress(handle,para);
	}
	else if(op == PROGRESSBAR_OP_USERINFO)
	{
		viewSetText(Style1Info,(char *)para,TEXTVIEW_CONTEXT_MAX);
		viewSetVisiable(Style1Info,1);
	}


	return 0;
}


ProgressBar_OP_T style1ProgressBar_op = 
{
	progressbar_style1Create,
	progressbar_style1Destory,
	progressbar_style1Show,
	progressbar_style1SetProgress,
	progressbar_style1IOCtrl
};
#endif

