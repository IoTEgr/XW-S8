#ifndef  __TASK_MANAGE_H
#define  __TASK_MANAGE_H

#include"uiWin.h"

typedef enum
{
	TASK_POWER_ON=0,
	TASK_POWER_OFF,
	TASK_VIDEO_RECORD,
	TASK_PHOTO_ENCODE,
	TASK_PLAY_BACK,
	TASK_AUDIO_PLAYER,
	TASK_MAIN_MENU,
	TASK_GAME_MENU,
	TASK_GAME,
	TASK_APP,
	TASK_SETTING_MENU,
	TASK_USB_DEVICE,
	TASK_USB_UPDATE,

	TASK_MAX
}taskID;

typedef struct _task
{
	char* name;
	uint32 arg;
	void  (*taskOpen)(uint32 arg);
	void  (*taskClose)(uint32 arg);
	void  (*taskService)(uint32 arg);
}sysTask;


void taskInit(void);
void taskRegister(taskID id,sysTask* task);
taskID taskCurrent(void);
taskID taskNext(void);
void taskStart(taskID id,uint32 arg);
void taskService(void);
taskID taskPrevious(void);



#endif
