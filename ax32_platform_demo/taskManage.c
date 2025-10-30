#include "application.h"

static sysTask* taskArray[TASK_MAX];
static sysTask* curTask;
static taskID   curTaskId;
static taskID   nextTaskId;
static taskID   preTaskId;

void taskInit(void)
{
	int i;
	for(i=0;i<TASK_MAX;i++)
	{
		taskArray[i]=NULL;
	}
	curTask=NULL;
	curTaskId=TASK_POWER_ON;
	nextTaskId=TASK_MAX;
	preTaskId=TASK_MAX;
}

void taskInfor(int n)
{
	int i;
	deg_Printf("task infor[%d]:\n",n);
	for(i=0;i<TASK_MAX;i++)
	{
		if(taskArray[i])
			deg_Printf("task [%s] normal\n",taskArray[i]->name);
		else
			deg_Printf("task [%d] abnormal\n",i);
	}
}

void taskRegister(taskID id,sysTask* task)
{
	if(id<TASK_MAX)
	{
		if(taskArray[id])
			deg_Printf("waring: task[%s]already registered\n",taskArray[id]->name);
		taskArray[id]=task;
		deg_Printf("task[%s] register\n",taskArray[id]->name);
	}
	else
	{
		deg_Printf("task id[%d] too large,max id:%d\n",id,TASK_MAX);
	}
}
taskID taskPrevious(void)
{

	return preTaskId;
}
taskID taskCurrent(void)
{

	return curTaskId;
}
taskID taskNext(void)
{

	return nextTaskId;
}


void taskStart(taskID id,uint32 arg)
{
	if(id<TASK_MAX)
	{
		if(nextTaskId==TASK_POWER_OFF)
		{
			deg_Printf("task [%s] has highest priority,task [%d] was ignored\n",taskArray[TASK_POWER_OFF]->name,id);
			return;
		}
		if(taskArray[id])
		{
			taskArray[id]->arg=arg;
			preTaskId=curTaskId;
			nextTaskId=id;
			deg_Printf("will start task [%s]\n",taskArray[id]->name);
		}
		else
		{
			if(id==TASK_USB_UPDATE)
			{
				nextTaskId=id;
				deg_Printf("start usb update\n");
			}
			else
			{
				deg_Printf("start task [%d] not registered\n",id);
			}
		}
	}
}
extern void deamon_key_check(void);
void taskService(void)
{
	uint32 n=0;
	while(1)
	{
		if(nextTaskId==TASK_USB_UPDATE)
			return;
		if(curTask)
		{
			deg_Printf("task [%s] closed\n",curTask->name);
			if(curTask->taskClose)
				curTask->taskClose(curTask->arg);
			winDestroyDeskTopChildWin();
		}
		uiHeapInfor(n++);
		curTaskId=nextTaskId;
		nextTaskId=TASK_MAX;
		curTask=taskArray[curTaskId];
		if(curTask==NULL)
		{
			deg_Printf("task [%d] not registered,start [%s] task\n",curTaskId,taskArray[TASK_POWER_OFF]->name);
			curTaskId=TASK_POWER_OFF;
			curTask=taskArray[curTaskId];
		}
		deg_Printf("task [%s] open\n",curTask->name);
		if(curTask->taskOpen)
			curTask->taskOpen(curTask->arg);
		if(curTaskId!=TASK_POWER_OFF)
		{
			drawUIService(true);
		}
		while(1)
		{
			if(nextTaskId<TASK_MAX)
				break;
			systemService();
			if(curTask->taskService)
				curTask->taskService(curTask->arg);
		}
	}
}


