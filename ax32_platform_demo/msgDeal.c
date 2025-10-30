#include "application.h"


static msgDealFunc taskMsgDeal[EVENT_MAX];
static msgDealFunc unshieldedMsgDeal[EVENT_MAX];
extern msgDealInfor sysRecommendMsgDeal[];

void msgFuncRegister(msgDealInfor* infor)
{
	uint32 i;
	for(i=0;i<EVENT_MAX;i++)
		taskMsgDeal[i]=NULL;
	i=0;
	for(;;)
	{
		if(sysRecommendMsgDeal[i].msgId>=EVENT_MAX||sysRecommendMsgDeal[i].func==NULL)
			break;
		taskMsgDeal[sysRecommendMsgDeal[i].msgId]=sysRecommendMsgDeal[i].func;
		i++;
	}
	if(infor==NULL)
		return;
	i=0;
	for(;;)
	{
		if(infor[i].msgId>=EVENT_MAX||infor[i].func==NULL)
			break;
		taskMsgDeal[infor[i].msgId]=infor[i].func;
		i++;
	}
}
void unshieldedMsgFuncRegister(msgDealInfor* infor)
{
	uint32 i;
	for(i=0;i<EVENT_MAX;i++)
		unshieldedMsgDeal[i]=NULL;
	if(infor==NULL)
		return;
	i=0;
	for(;;)
	{
		if(infor[i].msgId>=EVENT_MAX||infor[i].func==NULL)
			break;
		unshieldedMsgDeal[infor[i].msgId]=infor[i].func;
		i++;
	}
}

void msgDeal(void)
{
	MSG_T* msgid;
	INT8U err;
	uint32 msgType;
	uint32 msgData;
	msgid = XMsgQPend(SysCtrl.sysQ,&err);  // check system event
	if(err == X_ERR_NONE)
	{
		msgType=getType(msgid);
		//debg("msgType=%d\n",msgType);
		if(msgType<EVENT_MAX)
		{
			msgData=getValue(msgid);

			if(msgType>=KEY_EVENT_START&&msgType<KEY_EVENT_END&&msgData==KEY_PRESSED)
			{
				if(!(!menuIsOpen()&&(taskCurrent()==TASK_PLAY_BACK))&&(msgType!=KEY_EVENT_POWEROFF)&&(MEDIA_STAT_START!=task_video_Get_Status())
					&&!((TASK_AUDIO_PLAYER==taskCurrent())&&(MP3_DAC_START==mp3_play_sta())))
				{
					deamon_keysound_play();
				}
			}

			if(taskMsgDeal[msgType])
				taskMsgDeal[msgType](winGetCurrent(),1,&msgData);
			if(unshieldedMsgDeal[msgType])
				unshieldedMsgDeal[msgType](winGetCurrent(),1,&msgData);
		}
	}
}

void msgDealByType(uint32 msgType,void* handle,uint32 parameNum,uint32* parame)
{
	if(taskMsgDeal[msgType])
		taskMsgDeal[msgType](handle,parameNum,parame);
	if(unshieldedMsgDeal[msgType])
		unshieldedMsgDeal[msgType](handle,parameNum,parame);
}
void msgDealByMsgInfor(msgDealInfor* infor,uint32 msgType,void* handle,uint32 parameNum,uint32* parame)
{
	uint32 i;
	if(infor==NULL)
		return;
	i=0;
	for(;;)
	{
		if(infor[i].msgId>=EVENT_MAX||infor[i].func==NULL)
			break;
		if(infor[i].msgId==msgType)
		{
			if(infor[i].func)
				infor[i].func( handle,parameNum,parame);
			break;
		}
			
		i++;
		//deg_Printf("msgDealByMsgInfor(msgDealInfor* infor,uint32 msgType,void* handle,uint32 parameNum,uint32* parame)\n");
	}
}







