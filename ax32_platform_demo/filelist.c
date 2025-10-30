/****************************************************************************
**
 **                              RESOURCE MANGAER 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  FILE LIST
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : filelist.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/

#include "application.h"
#include <string.h>

#if MANAGER_LIST_USE == MANAGER_FILELIST


#define  FILELIST_NODE_NUM    MANAGER_LIST_NUM

#define  FILELIST_CACHE_SIZE  256
#define  FILELIST_LIST_MAX    (MANAGER_LIST_MAX<<1) 
#define LIST_ARG_CHECK(h)   {if(h<0 || h>=FILELIST_NODE_NUM)return -1;}
#define LIST_ARG_CHEKN(h)   {if(h<0 || h>=FILELIST_NODE_NUM)return NULL;}

typedef struct FILELIST_CTRL_S
{
	char ordername[48];    // 12+4

	short int drity;
	short int scan;	

	int count;
	int doffset;
	int aoffset;


	RES_NAME_T name[FILELIST_LIST_MAX];
}FILELIST_CTRL_T;

//static char filelistCache[FILELIST_CACHE_SIZE];
static FILELIST_CTRL_T filelistNode[FILELIST_NODE_NUM];
static int filelistHandle;
static RES_NAME_T * filelistTempCache;

static int filelistInit(void)
{
	int i;

	for(i=0;i<FILELIST_NODE_NUM;i++)
		filelistNode[i].ordername[0] = 0;
    filelistTempCache = NULL;
	filelistHandle = -1;
	return 0;
}



static int filelistCreate(char *path)
{
	char *src,*tar,name[32];
	int i,idx;

	tar = src = path;
    while(*src)
    {
		if(*src == '/' || *src == '\\')
		{
			 if(src[1] != 0)
			     tar = &src[1];
			 else
				break;
		}
		src++;
    }

    src = name;
	while(*tar)
	{
		if(*tar == '/' || *tar == '\\')
			break;
		*src++ = *tar++;
	}
	*src++ = '.';
	*src++ = 'L';
	*src++ = 'S';
	*src++ = 'T';
	*src++ = 0;
    idx = FILELIST_NODE_NUM;
	for(i=0;i<FILELIST_NODE_NUM;i++)
	{
		if(filelistNode[i].ordername[0] == 0)
		{
			if(idx == FILELIST_NODE_NUM)
				idx = i;
			continue;
		}
		else
		{
			if(strcmp(filelistNode[i].ordername,name)==0)
				break;
		}
	}
    if((i>=FILELIST_NODE_NUM) && (idx == FILELIST_NODE_NUM))
		return -1;

	if(i>=FILELIST_NODE_NUM)
	{
		i = idx;
		//strcpy(filelistNode[i].ordername,path);
		strcpy(filelistNode[i].ordername,name);
		filelistNode[i].count = 0;
		filelistNode[i].aoffset = 0;
		filelistNode[i].doffset = 0;
		filelistNode[i].drity = 0;
		filelistNode[i].scan = 0;
	}
	
	return i;
}
static int filelistDestory(int handle)
{
	LIST_ARG_CHECK(handle);

	filelistNode[handle].ordername[0] = 0;
	filelistNode[handle].count = 0;
	filelistNode[handle].aoffset = 0;
	filelistNode[handle].doffset = 0;

	return 0;
}



static int filelistScanStart(int handle)
{
	int fd,i,j;
	RES_NAME_T *tname;
	TCHAR fname[64];

	LIST_ARG_CHECK(handle);

    Ascii2Tchar(filelistNode[handle].ordername, fname, sizeof(fname)/sizeof(fname[0]));
	fd = open(/*filelistNode[handle].ordername*/fname,FA_WRITE | FA_READ);
//	deg_Printf("cleate file: %s.%d\n",filelistNode[handle].ordername,fd);
	if(fd<0)
	{
		fd = open(/*filelistNode[handle].ordername*/fname,FA_WRITE | FA_READ|FA_CREATE_NEW);
		f_chmod(/*filelistNode[handle].ordername*/fname,6,0x06);
		if(fd<0)
		{
			return -1;
		}
		filelistNode[handle].count = 0;
		filelistNode[handle].aoffset=0;
		tname = filelistNode[handle].name;
		for(i=0;i<FILELIST_LIST_MAX;i++)
		{
			tname[i].index_h = MA_FILE_IVL; // invliad
		}
	}
	else
	{
		filelistNode[handle].count = 0;
		tname = &filelistNode[handle].name[0];
		lseek((FHANDLE)fd,0,0);
		read((FHANDLE)fd,(void *)tname,FILELIST_LIST_MAX*sizeof(RES_NAME_T));
		for(i=0,j=0;i<FILELIST_LIST_MAX;i++)
		{
			if((tname[i].index_h&MA_FILE_IVL)==0)
			{
				tname[i].index_h&=~MA_FILE_BSY;
				if(i!=j)
				{
					tname[j].index_h = tname[i].index_h;
					tname[j].index_l = tname[i].index_l;
				}
				j++;
				//deg_Printf("scan : %d,[%x,%x]\n",i,tname[i].index_h,tname[i].index_l);
				filelistNode[handle].count++;
				filelistNode[handle].aoffset = j;
			}
		}
	}
    filelistNode[handle].drity = 1;
	filelistNode[handle].scan = 1;
	filelistHandle = fd;

	return filelistNode[handle].count;
}
static int filelistFlush(int handle)
{
	int i,j;
	RES_NAME_T *tname;

	if(filelistNode[handle].drity == 0)
		return filelistNode[handle].count;
	filelistNode[handle].drity = 0;
	tname = filelistNode[handle].name;

	for(i=0,j=0;i<FILELIST_LIST_MAX;i++)
    {
		if((tname[i].index_h&MA_FILE_IVL)|| ((tname[i].index_h&MA_FILE_BSY)==0)) // invliad file
		{
			tname[i].index_h = 0xffffffff;
			tname[i].index_l = 0xffffffff;
		}
		else
		{
			//tname[i].index_h&=~MA_FILE_BSY; // vilad file
			if(i!=j)
			{
				tname[j].index_h = tname[i].index_h;
				tname[j].index_l = tname[i].index_l;
				tname[i].index_h = 0xffffffff;
				tname[i].index_l = 0xffffffff;
			}
			j++;
		}
    }
	if(filelistHandle<0)
	{
		TCHAR fname[64];
		Ascii2Tchar(filelistNode[handle].ordername, fname, sizeof(fname)/sizeof(fname[0]));
		filelistHandle = open(/*filelistNode[handle].ordername*/fname,FA_WRITE | FA_READ);
	}
	else		
		lseek((FHANDLE)filelistHandle,0,0);
	write((FHANDLE)filelistHandle,(void *)filelistNode[handle].name,FILELIST_LIST_MAX*sizeof(RES_NAME_T));
	close((FHANDLE)filelistHandle);

	filelistHandle = -1;

	return j;
}

static int filelistScanEnd(int handle)
{

    filelistNode[handle].aoffset = filelistFlush(handle);
	filelistNode[handle].count = filelistNode[handle].aoffset;
	filelistFlush(handle);
    filelistNode[handle].scan = 0;

//	deg_Printf("file list end : %d\n",filelistNode[handle].count);
	return 0;
}

static int filelistCount(int handle,int ma)
{
	LIST_ARG_CHECK(handle);
	int i,count;
	RES_NAME_T *tname;	
	tname = &filelistNode[handle].name[0];
	count =0;
	filelistNode[handle].aoffset = filelistNode[handle].count = filelistFlush(handle);
	for(i=0;i<filelistNode[handle].count/*FILELIST_LIST_MAX*/;i++)
	{
		if(tname[i].index_h&MA_FILE_IVL)
			continue;
		if(tname[i].index_h&ma)
		{
			count++;
		}
	}

	return count;
}

static int filelistAdd(int handle,RES_NAME_T *node)
{
	int i;
	RES_NAME_T *tname;	
	LIST_ARG_CHECK(handle);

    if(!filelistNode[handle].scan)
    {
		if(filelistNode[handle].count>=FILELIST_LIST_MAX)
			return -1;
    }
	if(/*(filelistNode[handle].count>=FILELIST_LIST_MAX) || */(node->index_h&MA_FILE_IVL))
		return -2;

	tname = &filelistNode[handle].name[0];
	for(i=0;i<filelistNode[handle].count/*FILELIST_LIST_MAX*/;i++)
	{
		if(tname[i].index_h&MA_FILE_IVL)
			continue;
		if(((tname[i].index_h&(~MA_FILE_BSY)) == node->index_h) && (tname[i].index_l == node->index_l))
		{
			tname[i].index_h|=MA_FILE_BSY; // vliad
			filelistNode[handle].drity = 1;
			//deg_Printf("add : hit.%d,[%x,%x]\n",i,tname[i].index_h,tname[i].index_l);
			break;
		}
	}

	if(i>=filelistNode[handle].count/*FILELIST_LIST_MAX*/)
	{
		tname[filelistNode[handle].aoffset].index_h = node->index_h|MA_FILE_BSY;
		tname[filelistNode[handle].aoffset].index_l = node->index_l;
		filelistNode[handle].drity = 1;
		filelistNode[handle].aoffset++;
		if(filelistNode[handle].aoffset>=FILELIST_LIST_MAX)
			filelistNode[handle].aoffset = 0;
		filelistNode[handle].count++;
		//deg_Printf("add : %d,[%x,%x]\n",filelistNode[handle].aoffset,node->index_h|MA_FILE_BSY,node->index_l);
	}
    if(filelistNode[handle].scan==0)
	    filelistNode[handle].aoffset = filelistNode[handle].count = filelistFlush(handle);
	return 0;
}
static int filelistGetIndex(int handle,INT32U ma)
{
	int i;
	RES_NAME_T *tname;	
	LIST_ARG_CHECK(handle);

    tname = &filelistNode[handle].name[0];
	for(i=filelistNode[handle].count-1;i>=0;i--)
	{
		if((tname[i].index_h&MA_FILE_IVL)||(!(tname[i].index_h&MA_FILE_BSY)))
			continue;
		if(tname[i].index_h&ma)
			return tname[i].index_l;
	}

	return 0;
}


static RES_NAME_T *filelistGetName(int handle,int index)
{
	int i,j;
	RES_NAME_T *tname;
	LIST_ARG_CHEKN(handle);

	if(((index-1)<0) || (index>filelistNode[handle].count))
		return NULL;
	tname = filelistNode[handle].name;
	for(i=0,j=0;i<FILELIST_LIST_MAX;i++)
	{
		if(tname[i].index_h&MA_FILE_IVL)
			continue;
		j++;
		if(index == j)
		{
		//	deg_Printf("i=%d,%d,%d\n",i,filelistNode[handle].name[i].index_h,filelistNode[handle].name[i].index_l);
			return &filelistNode[handle].name[i];
		}
	}
	return NULL;
}

static int filelistFindName(int handle,RES_NAME_T *node,int cmp)
{
	int i;
	RES_NAME_T *tname;
	
	LIST_ARG_CHECK(handle);
	if(node->index_h&MA_FILE_IVL)
		return -1;

	tname = filelistNode[handle].name;
	for(i=0;i<FILELIST_LIST_MAX;i++)
	{
		if(tname[i].index_h&MA_FILE_IVL)
			continue;
		if(cmp==0)
		{
			if(((tname[i].index_h&MA_FILE_TMASK)&(node->index_h&MA_FILE_TMASK)) && (tname[i].index_l == node->index_l))
			  return i;
		}
		else
		{
			if(((tname[i].index_h&(~(MA_FILE_BSY|MA_FILE_LOK))) == node->index_h) && (tname[i].index_l == node->index_l))
				return i;
		}
	}

	return -1;
}

static int filelistDelByName(int handle,RES_NAME_T *node)
{
    int i;

	if(filelistNode[handle].count<=0)
		return -1;

	i = filelistFindName(handle,node,1);
	if(i<0)
		return -1;
   if(filelistNode[handle].name[i].index_h & MA_FILE_LOK)
		return 1;
	if(i>=0)
	{
		filelistNode[handle].name[i].index_h = 0xffffffff;
		filelistNode[handle].name[i].index_l = 0xffffffff;
		filelistNode[handle].count--;
		filelistNode[handle].drity = 1;
	}
	filelistNode[handle].aoffset = filelistNode[handle].count = filelistFlush(handle);

	return 0;
}

static int filelistDelByIndex(int handle,int index)
{
	RES_NAME_T *tname;

	LIST_ARG_CHECK(handle);
	
	tname = filelistGetName(handle,index);	
	if(tname!=NULL)
	{
		if(tname->index_h & MA_FILE_LOK)
			return 1;
		tname->index_h = 0xffffffff;
		tname->index_l = 0xffffffff;
		filelistNode[handle].count--;
		filelistNode[handle].drity = 1;
	}	
	return 0;
}

static int filelistDelAll(int handle)
{
	int i;
	RES_NAME_T *tname;

	LIST_ARG_CHECK(handle);

	if(filelistNode[handle].count<=0)
		return -1;
	
	tname = filelistNode[handle].name;
	for(i=0;i<FILELIST_LIST_MAX;i++)
	{
		if(tname[i].index_h & MA_FILE_LOK)
			continue;
		tname[i].index_h=0xffffffff;
	}
    filelistNode[handle].drity = 1;
	filelistNode[handle].aoffset = filelistNode[handle].count = filelistFlush(handle);
	
	return 0;	
}
static int filelistDirty(int handle)
{
	LIST_ARG_CHECK(handle);
	filelistNode[handle].drity = 1;

	return 0;
}
static int filelistFlush2(int handle)
{
	LIST_ARG_CHECK(handle);
//	filelistNode[handle].drity = 1;
	filelistNode[handle].aoffset = filelistNode[handle].count = filelistFlush(handle);

	return 0;
}
static int fileistRename(int handle,int index,RES_NAME_T *node)
{
	RES_NAME_T *tname;

	LIST_ARG_CHECK(handle);

	tname = filelistGetName(handle,index);

    if(tname!=NULL)
    {
		tname->index_h = node->index_h|MA_FILE_BSY;
		tname->index_l = node->index_l;
		filelistNode[handle].drity = 1;
    }
	filelistNode[handle].aoffset = filelistNode[handle].count = filelistFlush(handle);

	return 0;
}
static int filelistPrint(int handle)
{
	/*int i;

	LIST_ARG_CHECK(handle);

	for(i=0;i<FILELIST_LIST_MAX;i++)
	{
		if((filelistNode[handle].name[i].index_h&MA_FILE_BSY) && (filelistNode[handle].name[i].index_h&MA_FILE_IVL)==0)
		{
			deg_Printf("list[%d] : %x,%x\n",i,filelistNode[handle].name[i].index_h,filelistNode[handle].name[i].index_l);
		}
	}*/

	return 0;
}

FILE_MANAGER_T filelist =
{
	.init = filelistInit,
	.uninit = NULL,
	.create = filelistCreate,
	.destory= filelistDestory,
	.count = filelistCount,
	.name = filelistGetName,
	.find = filelistFindName,
	.add = filelistAdd,
	.del_name = filelistDelByName,
	.del_index = filelistDelByIndex,
	.del_all = filelistDelAll,
	.scan_start = filelistScanStart,
	.scan_end = filelistScanEnd,
	.flush = filelistFlush2,
	.rename = fileistRename,
	.print = filelistPrint,
	.drity = filelistDirty,
	.index = filelistGetIndex,
};
#endif

