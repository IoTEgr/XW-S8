/****************************************************************************
                                      EXPLORER
                       THE APPOTECH X OPERATION SYSTEM
                                EXPLORER FILE LIST
                
                            (C) COPYRIGHT 2016 BUILDWIN 
                                
                    BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
                
* File Name   : explorer.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 12/08/2016
* Description : 
*                 
*                 
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "application.h"
#include <string.h>

#if MANAGER_LIST_USE == MANAGER_EXPLORER

#define  EXPLORER_CFG_NODE_MAX           MANAGER_LIST_MAX  //

#define  EXPLORER_CFG_MALLOC            0

#define  EXPLORER_CFG_NAMELEN           MANAGER_NAME_LEN    //yyyymmddhhmmssn.avi

#define  EXPLORER_CFG_MAX                 3     // type of file list


#if EXPLORER_CFG_MALLOC > 0
     #define EXPLORER_MALLOC_NODE         1000
#endif

#define  NODE_NULL      0xffff

extern int explorerListHook(RES_NAME_T *name1,RES_NAME_T *name2);

typedef struct EXPLORER_NODE_S
{	
	unsigned short prev;
	unsigned short next;

	RES_NAME_T name;

}EXPLORER_NODE_T;



typedef struct EXPLORER_S
{
	unsigned short count;

    unsigned short index;
    unsigned short node;
	
	unsigned short busy;

	unsigned short tial;

	unsigned short stat;
}EXPLORER_T;

static EXPLORER_NODE_T ExplorerNodePool[EXPLORER_CFG_NODE_MAX];// SECTION(".big_buffer") ALIGNED(4);
static unsigned short IdleQueue;
static EXPLORER_T ExplorerCtrl[EXPLORER_CFG_MAX];

/*******************************************************************************
* Function Name  : explorerListGetNodeIdle
* Description    : explorer file list get file node in idle
* Input          : none
* Output         : none                                            
* Return         : int :file node index
*******************************************************************************/
static int explorerListGetNodeIdle(void)
{
	int node;

	if(IdleQueue!=NODE_NULL)
	{
		node = IdleQueue;
		IdleQueue = ExplorerNodePool[node].next;
		ExplorerNodePool[node].next = NODE_NULL;
		ExplorerNodePool[node].prev = NODE_NULL;
		return node;
	}

	return NODE_NULL;
}
/*******************************************************************************
* Function Name  : explorerListSetNodeIdle
* Description    : explorer file list set file node in idle
* Input          : int node : file node
* Output         : none                                            
* Return         : none
*******************************************************************************/
static void explorerListSetNodeIdle(int node)
{
	ExplorerNodePool[node].prev = NODE_NULL;
    ExplorerNodePool[node].next = IdleQueue;
	IdleQueue = node;
	
}
/*******************************************************************************
* Function Name  : explorerListDelNodeBusy
* Description    : explorer file list del node in busy queue
* Input          : int node : file node
* Output         : none                                            
* Return         : none
*******************************************************************************/
static void explorerListDelNodeBusy(EXPLORER_T *explorer,int node)
{
	int cur,next;

	cur = node;

    if(cur == NODE_NULL)
		return ;

	next = ExplorerNodePool[cur].next;

	if(explorer->busy == cur)
	{
		explorer->busy = ExplorerNodePool[cur].next;
		if(ExplorerNodePool[cur].next!=NODE_NULL)
			ExplorerNodePool[ExplorerNodePool[cur].next].prev = NODE_NULL;
	}
    else if(next==NODE_NULL)
	{
		if(ExplorerNodePool[cur].prev!=NODE_NULL)
			ExplorerNodePool[ExplorerNodePool[cur].prev].next = NODE_NULL;		
	}
	else
	{
		ExplorerNodePool[next].prev = ExplorerNodePool[cur].prev;
		ExplorerNodePool[ExplorerNodePool[cur].prev].next = next;
	}
}
/*******************************************************************************
* Function Name  : explorerListFindIndex
* Description    : explorer file list find file by file index
* Input          : int index : file index
* Output         : none                                            
* Return         : int : file node
*******************************************************************************/
static int explorerListFindIndex(EXPLORER_T *explorer,int index)
{
	int cur,start,count,prev;
	
	if(index>explorer->count)
		return NODE_NULL;

	if(index>explorer->index && explorer->node!=NODE_NULL)
	{
		start = explorer->node;
		count = explorer->index;
	}
    else
    {
		start = explorer->busy;
		count = 1;
    }
    prev =  NODE_NULL;

	for(cur=start;cur!=NODE_NULL;cur = ExplorerNodePool[cur].next)
	{
		prev = cur;
		if(index == count)
			break;
		count++;		
	}
	if(cur != NODE_NULL)
	{
		explorer->index = index;
		explorer->node  = prev;	
	}

	return cur;
	
}
/*******************************************************************************
* Function Name  : explorerListInit
* Description    : explorer file list initial
* Input          : none
* Output         : none                                            
* Return         : int : 0
*******************************************************************************/
static int explorerListInit(void)
{
    int i;

    for(i=0;i<EXPLORER_CFG_MAX;i++)
    {
		ExplorerCtrl[i].count = 0;
		ExplorerCtrl[i].busy = NODE_NULL;
		ExplorerCtrl[i].node = NODE_NULL;
		ExplorerCtrl[i].index= NODE_NULL;
		ExplorerCtrl[i].stat = 0;
    }
    ExplorerNodePool[0].prev = NODE_NULL;
	//ExplorerNodePool[0].name[0] = 0;
	
	for(i=1;i<EXPLORER_CFG_NODE_MAX;i++)
	{
		ExplorerNodePool[i].prev = i-1;
		ExplorerNodePool[i-1].next = i;
		//ExplorerNodePool[i].name[0] = 0;
	}
	ExplorerNodePool[i-1].next = NODE_NULL;

	IdleQueue = 0;
	
	return 0;
}
/*******************************************************************************
* Function Name  : explorerListCreat
* Description    : explorer file list create a queue
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : queue index
*******************************************************************************/
static int explorerListCreat(char *path)
{
	int i;

	for(i=0;i<EXPLORER_CFG_MAX;i++)
	{
		if(ExplorerCtrl[i].stat==0)
		{
			ExplorerCtrl[i].stat = 1;
			ExplorerCtrl[i].busy = NODE_NULL;
			ExplorerCtrl[i].count=0;
			ExplorerCtrl[i].tial = NODE_NULL;
			ExplorerCtrl[i].node = NODE_NULL;
			break;
		}
	}

	if(i<EXPLORER_CFG_MAX)
		return i;
	else
		return -1;
}
/*******************************************************************************
* Function Name  : explorerListCreat
* Description    : explorer file list create a queue
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : queue index
*******************************************************************************/
static int explorerListDestory(int queue)
{
	int node,cur;

	if(queue>=EXPLORER_CFG_MAX || queue<0)
		return -1;
	for(cur=ExplorerCtrl[queue].busy;cur!=NODE_NULL;)
	{
		node = ExplorerNodePool[cur].next;
		explorerListSetNodeIdle(cur);
		cur = node;
	}

	ExplorerCtrl[queue].stat = 0;

	return 0;
}
/*******************************************************************************
* Function Name  : explorerListAdd
* Description    : explorer file list add a file
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : file node
*******************************************************************************/
static int explorerListAdd(int queue,RES_NAME_T *name)
{
	int cur,node,prev;
	EXPLORER_T *explorer;

	if(queue>=EXPLORER_CFG_MAX || queue<0)
		return -1;
	
	node = explorerListGetNodeIdle();
	if(node == NODE_NULL)
		return -1;

    explorer = &ExplorerCtrl[queue];
    explorer->count++;
	//strcpy(ExplorerNodePool[node].name,name);
	ExplorerNodePool[node].name.index_h = name->index_h;
	ExplorerNodePool[node].name.index_l = name->index_l;

	if(explorer->tial!=NODE_NULL)
	{
		if(!explorerListHook(name,&ExplorerNodePool[explorer->tial].name))
		{
			ExplorerNodePool[explorer->tial].next = node;
			ExplorerNodePool[node].prev = explorer->tial;
			ExplorerNodePool[node].next = NODE_NULL;
			explorer->tial = node;
			return 0;
		}
	}

	if(explorer->busy == NODE_NULL)
	{
		explorer->busy = node;
		explorer->tial = node;
		ExplorerNodePool[node].prev = NODE_NULL;
		ExplorerNodePool[node].next = NODE_NULL;
		return 0;
	}
	prev = NODE_NULL;
	for(cur = explorer->busy;cur!=NODE_NULL;)
	{
		if(explorerListHook(name,&ExplorerNodePool[cur].name))
			break;
		prev = cur;
		cur = ExplorerNodePool[cur].next;
	}

    if(cur == NODE_NULL)
    {
		ExplorerNodePool[node].prev = prev;
		ExplorerNodePool[node].next = NODE_NULL;
		ExplorerNodePool[prev].next = node;
		explorer->tial = node;
    }
	else if(prev == NODE_NULL)
	{
		ExplorerNodePool[node].next = cur;
		ExplorerNodePool[node].prev = NODE_NULL;
        ExplorerNodePool[cur].prev  = node;
        explorer->busy = node;
	}
	else
	{
		ExplorerNodePool[node].prev = ExplorerNodePool[cur].prev;
		ExplorerNodePool[node].next = cur;
		ExplorerNodePool[cur].prev  = node;
		ExplorerNodePool[prev].next = node;
	}

	return 0;
}
/*******************************************************************************
* Function Name  : explorerListDelByName
* Description    : explorer file list del file by file name
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : 0
*******************************************************************************/
static int explorerListDelByName(int queue,RES_NAME_T *name)
{
	int cur;
	EXPLORER_T *explorer;

	if(queue>=EXPLORER_CFG_MAX || queue<0)
		return -1;
    explorer = &ExplorerCtrl[queue];
	
	for(cur = explorer->busy;cur!=NODE_NULL;)
	{
		//if(strcmp(name,ExplorerNodePool[cur].name)==0)
		//	break;
		if((name->index_h == ExplorerNodePool[cur].name.index_h) && (name->index_l == ExplorerNodePool[cur].name.index_l))
			break;
		cur = ExplorerNodePool[cur].next;
	}

	if(cur == NODE_NULL)
		return 1;

	explorer->count--;
	ExplorerNodePool[cur].name.index_h = 0;
	ExplorerNodePool[cur].name.index_l = 0;

	explorerListDelNodeBusy(explorer,cur);

	explorerListSetNodeIdle(cur);
	
	return 0;
}
/*******************************************************************************
* Function Name  : explorerListFindName
* Description    : explorer file list find file by name
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : 0
*******************************************************************************/
static int explorerListFindName(int queue,RES_NAME_T *name,int cmp)
{
	int cur,index;
	EXPLORER_T *explorer;

	if(queue>=EXPLORER_CFG_MAX || queue<0)
		return -1;
    explorer = &ExplorerCtrl[queue];
	
    index = 0;
	for(cur = explorer->busy;cur!=NODE_NULL;)
	{
		//if(strcmp(name,ExplorerNodePool[cur].name)==0)
		//	break;
		if((name->index_h == ExplorerNodePool[cur].name.index_h) && (name->index_l == ExplorerNodePool[cur].name.index_l))
			break;
		cur = ExplorerNodePool[cur].next;
		index++;
	}

	if(cur == NODE_NULL)
		return -1;	
	return index;
}
/*******************************************************************************
* Function Name  : explorerListGetName
* Description    : explorer file list get file name by index
* Input          : int index : file index
* Output         : none                                            
* Return         : char * : file anme
*******************************************************************************/
static RES_NAME_T *explorerListGetName(int queue,int index)
{
    int cur;
	EXPLORER_T *explorer;

	if(queue>=EXPLORER_CFG_MAX || queue<0)
		return NULL;
    explorer = &ExplorerCtrl[queue];
	
	cur = explorerListFindIndex(explorer,index);

	if(cur == NODE_NULL)
		return NULL;

	return &ExplorerNodePool[cur].name;
	
}
/*******************************************************************************
* Function Name  : explorerListDelByIndex
* Description    : explorer file list del file by index
* Input          : int index : file index
* Output         : none                                            
* Return         : int
*******************************************************************************/
static int explorerListDelByIndex(int queue,int index)
{
    int cur;
	EXPLORER_T *explorer;

	if(queue>=EXPLORER_CFG_MAX || queue<0)
		return -1;
    explorer = &ExplorerCtrl[queue];
	
	cur = explorerListFindIndex(explorer,index);

	if(cur == NODE_NULL)
		return 1;

	explorer->count--;
	ExplorerNodePool[cur].name.index_h = 0;
	ExplorerNodePool[cur].name.index_l = 0;

	explorerListDelNodeBusy(explorer,cur);

	explorerListSetNodeIdle(cur);

	return 0;
}
/*******************************************************************************
* Function Name  : explorerListGetCount
* Description    : explorer file list get file total number
* Input          : 
* Output         : none                                            
* Return         : int
*******************************************************************************/
static int explorerListGetCount(int queue)
{
	EXPLORER_T *explorer;

	if(queue>=EXPLORER_CFG_MAX || queue<0)
		return -1;
    explorer = &ExplorerCtrl[queue];
	
	return explorer->count;
}



FILE_MANAGER_T explorer=
{
	.init = explorerListInit,
	.uninit = NULL,
	.create = explorerListCreat,
	.destory= explorerListDestory,
	.count = explorerListGetCount,
	.name = explorerListGetName,
	.find = explorerListFindName,
	.add = explorerListAdd,
	.del_name = explorerListDelByName,
	.del_index= explorerListDelByIndex,
};


#endif