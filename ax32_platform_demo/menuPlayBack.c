/****************************************************************************
**
 **                              LAYOUT
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  LAYOUT FOR SETTING MENU
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : menu_setting.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : 
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"
#include "uiWin.h"
#include "menu.h"


void menuProcDelCur(winHandle handle,uint32 parameNum,uint32* parame);
void menuProcDelAll(winHandle handle,uint32 parameNum,uint32* parame);
void menuProcLockCur(winHandle handle,uint32 parameNum,uint32* parame);
void menuProcUnlockCur(winHandle handle,uint32 parameNum,uint32* parame);
void menuProcUnlockAll(winHandle handle,uint32 parameNum,uint32* parame);

MENU_ITME_START(playBack)
MENU_ITEM_PROC(menuProcDelCur,R_ID_ICON_MENUDELONE,R_ID_STR_SET_DELETE)
MENU_ITEM_PROC(menuProcDelAll,R_ID_ICON_MENUDELALL,R_ID_STR_SET_DELETEALL)
//MENU_ITEM_PROC(menuProcLockCur,R_ID_ICON_MENUUNLOCK,R_ID_STR_SET_LOCKCUR)
//MENU_ITEM_PROC(menuProcUnlockCur,R_ID_ICON_MENUUNLOCK,R_ID_STR_SET_UNLOCKCUR)
//MENU_ITEM_PROC(menuProcUnlockAll,R_ID_ICON_MENUUNLOCK,R_ID_STR_SET_UNLOCKALL)
MENU_ITME_END()


MENU_PAGE_START(playBack)
MENU_PAGE_ITEMS(playBack,R_ID_ICON_MTRECORD,R_ID_ICON_MTRECORD,R_ID_STR_SET_VIDEO)
MENU_PAGE_END()

MENU_DEFINE(playBack)

#if 0
static void nouse(void)
{
}
#endif


void menuProcDelCur(winHandle handle,uint32 parameNum,uint32* parame)
{
//	uiOpenWindow(&delCurWindow,0);
}
void menuProcDelAll(winHandle handle,uint32 parameNum,uint32* parame)
{
//	uiOpenWindow(&delAllWindow,0);
}
#if 0
void menuProcLockCur(winHandle handle,uint32 parameNum,uint32* parame)
{
//	uiOpenWindow(&lockCurWindow,0);
}
void menuProcUnlockCur(winHandle handle,uint32 parameNum,uint32* parame)
{
//	uiOpenWindow(&unlockCurWindow,0);
}
void menuProcUnlockAll(winHandle handle,uint32 parameNum,uint32* parame)
{
//	uiOpenWindow(&unlockAllWindow,0);
}

#endif


