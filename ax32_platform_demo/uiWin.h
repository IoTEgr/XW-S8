#ifndef __UI_WIN_H
#define  __UI_WIN_H
#include"../sUI/sUI.h"
#include "application.h"
#include "menu.h"

#define  TIPS_KEEP_SHOWING     5 //0xffffffff

#define  TIPS_SD_NOT_INSERT    R_ID_STR_TIPS_SDC_NULL
#define  TIPS_SD_FULL          R_ID_STR_TIPS_SDC_FULL
#define  TIPS_SD_ERROR         R_ID_STR_SDC_ERROR
#define  TIPS_USENSOR_POWER_LOW  R_ID_STR_PWR_BACKLOW
#define  TIPS_POWER_LOW   R_ID_STR_PWR_LOW
#define  TIPS_NO_POWER   R_ID_STR_TIPS_NO_POWER

typedef struct _uiWin
{
	msgDealInfor* msgDeal;
	widgetCreateInfor* widgetInfor;
	winHandle   handle;
	bool  repeateOpenSupport;
}uiWin;



extern uiWin videoRecordWindow;
extern uiWin photoEncodeWindow;
extern uiWin playBackWindow;
extern uiWin audioPlayerWindow;
extern uiWin mainMenuWindow;
extern uiWin gameMenuWindow;
extern uiWin settingMenuWindow;
extern uiWin AppWindow;


extern uiWin menuItemWindow;
extern uiWin menuOptionWindow;
extern uiWin tipsWindow;
extern uiWin tips1Window;


extern uiWin languageWindow;
extern uiWin printDensityWindow;
extern uiWin printModeWindow;
extern uiWin dateTimeWindow;
extern uiWin volumeWindow;
extern uiWin videoResWindow;
extern uiWin cameraResWindow;
extern uiWin formatWindow;
extern uiWin autoPowerOffWindow;
extern uiWin screenSaveWindow;
extern uiWin cyclicRecordWindow;
extern uiWin frequencyWindow;
extern uiWin defaultWindow;
extern uiWin versionWindow;


extern uiWin menudatetimeWindow;
extern uiWin menuUsbWindow;
extern uiWin noFileWindow;
extern uiWin delCurWindow;
extern uiWin delAllWindow;
extern uiWin lockCurWindow;
extern uiWin unlockAllWindow;
extern uiWin unlockCurWindow;
extern uiWin asternWindow;
extern uiWin selfTestWindow;



void uiParentDealMsg(winHandle handle,uint32 parentMsg);
winHandle uiOpenWindow(uiWin* winInfor,uint32 argc,...);
winHandle uiOpenWinAndShow(uiWin* winInfor,uint32 argc,...);
bool windowIsOpen(uiWin* winInfor);


#define winItem         dialogItem
#define   WINDOW(name,msg,widget)        uiWin name={msg,widget,INVALID_HANDLE,false};
#define   MULTIWIN(name,msg,widget)        uiWin name={msg,widget,INVALID_HANDLE,true};



#endif

