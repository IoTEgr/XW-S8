/****************************************************************************
**
 **                              MANAGER
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                      RESOURCE MANAGER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : resource_manager.c
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

typedef struct RES_MANG_S
{
	INT32U res_ma;   
	int index;
	int exp;
	int count;
}RES_MANG_T;

#define ARG_CHECK(arg)   {if(arg==NULL)return -1;}
#define ARG_CHECK2(arg)   {if(arg<0)return -1;}
#define ARG_CHECK3(arg,r) {if((arg<0)||(arg>=MANAGER_NODE_NUM)) return r;}

extern FILE_MANAGER_T explorer;
extern FILE_MANAGER_T filelist;

static FILE_MANAGER_T *manager=NULL;
static RES_MANG_T resManager[MANAGER_NODE_NUM];

static char resourcePathPool[32+MANAGER_NAME_LEN+1];  // path+file name

static int string2name(RES_NAME_T *rname,char *string)
{
    INT32U value,i;	

	rname->index_h = 0;
	rname->index_l = 0;
#if (MANAGER_NAME&MANAGER_NAME_DATETIME)	
    INT32U idx = 0;	
    #if (MANAGER_NAME&MANAGER_NAME_DATE)
	    for(value=0,i=0;i<8;i++)
	    {
			if((string[idx+i]>'9') || (string[idx+i]<'0'))
				return -1;
			value = value*10 + string[idx+i]-'0';
	    }
		rname->index_h = value;
		idx +=8;
	#endif
	#if(MANAGER_NAME&MANAGER_NAME_TIME)
		for(value=0,i=0;i<8;i++)
		{
			if((string[idx+i]>'9') || (string[idx+i]<'0'))
				return -1;
			value = value*10 + string[idx+i]-'0';
		}
		rname->index_l = value;
		idx+=8;
	#endif
#else
    for(value=0,i=3;i<8;i++)
    {
		if((string[i]>'9') || (string[i]<'0'))
				return -1;
		value = value*10 + string[i]-'0';
    }
	rname->index_l = value;
#endif
    return 0;
}
static int name2string(RES_NAME_T *rname,char *string)
{
    INT32U value,i=0;	
#if (MANAGER_NAME&MANAGER_NAME_DATETIME)	
    #if (MANAGER_NAME&MANAGER_NAME_DATE)
    
	    value = rname->index_h&MA_NAME_MASK;
	    string[i+0] = ((value/10000000)%10) + '0';
		string[i+1] = ((value/1000000)%10) + '0';
		string[i+2] = ((value/100000)%10) + '0';
		string[i+3] = ((value/10000)%10) + '0';
		string[i+4] = ((value/1000)%10) + '0';
		string[i+5] = ((value/100)%10) + '0';
		string[i+6] = ((value/10)%10) + '0';
		string[i+7] = ((value/1)%10) + '0';
		i += 8;
    
	#endif
	#if(MANAGER_NAME&MANAGER_NAME_TIME)
	
	    value = rname->index_l;
	    string[i+0] = ((value/10000000)%10) + '0';
		string[i+1] = ((value/1000000)%10) + '0';
		string[i+2] = ((value/100000)%10) + '0';
		string[i+3] = ((value/10000)%10) + '0';
		string[i+4] = ((value/1000)%10) + '0';
		string[i+5] = ((value/100)%10) + '0';
		string[i+6] = ((value/10)%10) + '0';
		string[i+7] = ((value/1)%10) + '0';
		i+=8;
	#endif
#else
    value = rname->index_l;
    //string[i+0] = ((value/10000000)%10) + '0';
	//string[i+1] = ((value/1000000)%10) + '0';
	//string[i+2] = ((value/100000)%10) + '0';
	string[i+3] = ((value/10000)%10) + '0';
	string[i+4] = ((value/1000)%10) + '0';
	string[i+5] = ((value/100)%10) + '0';
	string[i+6] = ((value/10)%10) + '0';
	string[i+7] = ((value/1)%10) + '0';
	i+=8;	
#endif
   string[i] = 0; // end string
    return 0;	
}
static int stringCreate(RES_NAME_T *rname,INT32U idx)
{
#if (MANAGER_NAME&MANAGER_NAME_DATETIME)		
	date dtime;	 
#endif
	rname->index_h = 0;
	rname->index_l = 0;
#if (MANAGER_NAME&MANAGER_NAME_DATETIME)	
	now(&dtime);
    #if (MANAGER_NAME&MANAGER_NAME_DATE)
	    rname->index_h = dtime.year;
	    rname->index_h = rname->index_h*100+dtime.month;
		rname->index_h = rname->index_h*100+dtime.day;
    #endif
    #if(MANAGER_NAME&MANAGER_NAME_TIME)
        rname->index_l = dtime.hour;
	    rname->index_l = rname->index_l*100+dtime.min;
		rname->index_l = rname->index_l*100+dtime.sec;
	#endif
	rname->index_l = rname->index_l*10+idx;
#else
    rname->index_l = idx;
#endif

    return 0;
}

static void namemake(char *path,char *str,RES_NAME_T *mname)
{
#if (MANAGER_NAME&MANAGER_NAME_INDEX)
	if(strncmp(str,PREFIX_AVI,3)!=0 && strncmp(str,PREFIX_JPG,3)!=0 && strncmp(str,PREFIX_WAV,3)!=0 && strncmp(str,PREFIX_LOK,3)!=0)
	{
		mname->index_h |= MA_FILE_IVL;
		return ;
	}
#endif 

	if(string2name(mname,str)<0)
	{
		mname->index_h |= MA_FILE_IVL;
		return ;
	}

	int type = manangerCheckFileType(str);
	if(type == FILE_TYPE_AVI)
	{
		int i=0;
		while(path[i]&&(path[i]!='/')){i++;}
		if(path[i-1] == 'A')
			mname->index_h |= MA_FILE_AVIA;
		else if(path[i-1] == 'B')
			mname->index_h |= MA_FILE_AVIB;
		else
		    mname->index_h |= MA_FILE_AVI;
	}
	else if(type == FILE_TYPE_JPG)
		mname->index_h |= MA_FILE_JPG;
	else if(type == FILE_TYPE_WAV)
		mname->index_h |= MA_FILE_WAV;
	else
	{
		mname->index_h |= MA_FILE_IVL;
	}

	if(strncmp(str,PREFIX_LOK,3)==0)
		mname->index_h |= MA_FILE_LOK;

}
/*******************************************************************************
* Function Name  : managerInit
* Description    : initial resource manager
* Input          : 
* Output         : none                                            
* Return         : handle
*******************************************************************************/
int managerInit(void)
{
#if MANAGER_LIST_USE == MANAGER_EXPLORER	
	manager = &explorer;
#elif MANAGER_LIST_USE == MANAGER_FILELIST	
    manager = &filelist;
#endif
    int i;
    
	ARG_CHECK(manager);
	ARG_CHECK(manager->init);

	for(i=0;i<MANAGER_NODE_NUM;i++)
	{
		resManager[i].exp = -1;
		resManager[i].index = 0;
	}
	
	return manager->init();
}
/*******************************************************************************
* Function Name  : managerCreate
* Description    : create a manager
* Input          : 
* Output         : none                                            
* Return         : handle
*******************************************************************************/
int managerCreate(char *path,INT32U type,int combain)
{	
	ARG_CHECK(manager);
	ARG_CHECK(manager->create);

	int i,exp;
	for(i=0;i<MANAGER_NODE_NUM;i++)
	{
		if(resManager[i].exp<0)
			break;
	}
	if(i>=MANAGER_NODE_NUM)
		return -1;
	if((combain<0)||(combain>=MANAGER_NODE_NUM))
		exp = manager->create(path);
	else
		exp = resManager[combain].exp;
	if(exp<0)
		return -1;
	resManager[i].exp = exp;
	resManager[i].res_ma = type;
	resManager[i].index = 0;
    resManager[i].count = 0;
	return i;
}
/*******************************************************************************
* Function Name  : managerDestory
* Description    : destory a manager
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerDestory(int exp)
{
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->destory);
	
	manager->destory(resManager[exp].exp);	
	resManager[exp].exp = -1;
	
	return 0;
}
/*******************************************************************************
* Function Name  : managerFileCount
* Description    : get file list file count
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerFileCount(int exp)
{
	ARG_CHECK3(exp,-1);		
	ARG_CHECK(manager);
	ARG_CHECK(manager->count);
	//filelist_build();

	u32 i,ma=0;
#if (MANAGER_NAME==MANAGER_NAME_INDEX)
	for(i=0;i<MANAGER_NODE_NUM;i++)
	{
		if(resManager[i].exp == resManager[exp].exp)
			ma |= resManager[i].res_ma;
	}
#else
    ma = resManager[exp].res_ma;
#endif
	return manager->count(resManager[exp].exp,ma);

}
/*******************************************************************************
* Function Name  : managerFileCountEx
* Description    : get file list file count only sigle list
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerFileCountEx(int exp)
{
	ARG_CHECK3(exp,-1);		
	ARG_CHECK(manager);
	ARG_CHECK(manager->count);	

	return manager->count(resManager[exp].exp,resManager[exp].res_ma);
}
/*******************************************************************************
* Function Name  : managerAddFile
* Description    : add file to list
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerAddFile(int exp,char *path,char *name)
{	
	RES_NAME_T tname;
	ARG_CHECK3(exp,-1);		
	ARG_CHECK(manager);
	ARG_CHECK(manager->add);

    namemake(path,name,&tname);
    resManager[exp].count++;
	if(manager->add(resManager[exp].exp,&tname)<0)
	{
		deg_Printf("add file fail.%d/%s,%x,%x\n",path,name,tname.index_h,tname.index_l);
		return -1;
	}

	return 0;

}
/*******************************************************************************
* Function Name  : managerAddFileEx
* Description    : add file to list
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerAddFileEx(int exp,char *fullname)
{
	int i=0,j=0;
	
	while(fullname[j])
	{
		if(fullname[j] == '/')
			i = j;
		j++;
	}
	if(i==0)
		return -1;
	return managerAddFile(exp,fullname,&fullname[i+1]);
}
/*******************************************************************************
* Function Name  : managerLockFile
* Description    : lock file
* Input          : char *name : file name,lock file to rename
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerLockFile(char *name)
{
#if (MANAGER_NAME==MANAGER_NAME_INDEX)
    int i=0,j=0;
	
	while(name[j])
	{
		if(name[j] == '/')
			i = j;
		j++;
	}
    if((i==0)&&(name[0] != '/'))
		i=0;
	else
		i+=1;
	strncpy(&name[i],PREFIX_LOK,3);
#endif

    return 0;
}
/*******************************************************************************
* Function Name  : managerLockFile
* Description    : lock file
* Input          : int exp : handle
                      int index: index

* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerLockFileExt(int exp,int index)
{
	RES_NAME_T *tname;
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->name);

	tname = manager->name(resManager[exp].exp,index);	
	if(tname != NULL)
	{
		tname->index_h|=MA_FILE_LOK;
		if(manager->drity)
			manager->drity(resManager[exp].exp);
	}
    
	return 0;

}
/*******************************************************************************
* Function Name  : managerUnlockFile
* Description    : unlock file
* Input          : char *name : file name,lock file to rename
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerUnlockFile(char *name)
{
#if (MANAGER_NAME==MANAGER_NAME_INDEX)    
    int type = manangerCheckFileType(name);

    int i=0,j=0;
	
	while(name[j])
	{
		if(name[j] == '/')
			i = j;
		j++;
	}
	if((i==0)&&(name[0] != '/'))
		i=0;
	else
		i+=1;
	if(type == FILE_TYPE_AVI)
		strncpy(&name[i],PREFIX_AVI,3);
	else if(type == FILE_TYPE_JPG)
		strncpy(&name[i],PREFIX_JPG,3);
	else if(type == FILE_TYPE_WAV)
		strncpy(&name[i],PREFIX_WAV,3);
#endif

    return 0;
}
/*******************************************************************************
* Function Name  : managerUnlockFileExt
* Description    : unlock file
* Input          : int exp : handle
                      int index: index                                        
* Return         : int
*******************************************************************************/
int managerUnlockFileExt(int exp,int index)
{
	RES_NAME_T *tname;
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->name);

	tname = manager->name(resManager[exp].exp,index);	
	if(tname != NULL)
	{
		tname->index_h&=~MA_FILE_LOK;
		if(manager->drity)
			manager->drity(resManager[exp].exp);
	}
	return 0;

}
/*******************************************************************************
* Function Name  : managerCheckFile
* Description    : check file is lcoked or not
* Input          : char *name : file name
* Output         : none                                            
* Return         : int 1 : locked
                           0 : unlocked
*******************************************************************************/
int managerCheckLock(char *name)
{
#if (MANAGER_NAME==MANAGER_NAME_INDEX)
	 if(strncmp(name,PREFIX_LOK,3)==0)
		return 1;
#endif
	
		return 0;

}

/*******************************************************************************
* Function Name  : managerCheckLockExt
* Description    : check file is lcoked or not
* Input          : int exp : handle
                      int index: index
* Output         : none                                            
* Return         : int 1 : locked
                           0 : unlocked
*******************************************************************************/
int managerCheckLockExt(int exp,int index)
{
	RES_NAME_T *tname;
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->name);

	tname = manager->name(resManager[exp].exp,index);	
	if(tname != NULL)
	{
		if(tname->index_h&MA_FILE_LOK)
			return 1;
	}

	return 0;
}
/*******************************************************************************
* Function Name  : managerFlush
* Description    : flush file list
* Input          : int exp : handle
* Output         : none                                            
* Return         : int 
*******************************************************************************/
int managerFlush(int exp)
{
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->flush);

	manager->flush(resManager[exp].exp);	

	return 0;	
}
/*******************************************************************************
* Function Name  : managerDelFileByName
* Description    : delete file by file name 
* Input          : int exp : manager handle
                      char *name : file name
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerDelFileByName(int exp,char *name)
{
	RES_NAME_T rname;

	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->del_name);	

	if(string2name(&rname,name)<0)
		return -1;

	int type = manangerCheckFileType(name);
	if(type == FILE_TYPE_AVI)
		rname.index_h |= MA_FILE_AVI;
	else if(type == FILE_TYPE_JPG)
		rname.index_h |= MA_FILE_JPG;
	else if(type == FILE_TYPE_WAV)
		rname.index_h |= MA_FILE_WAV;

	int ret = manager->del_name(resManager[exp].exp,&rname);
	if(ret<0)
		return -1;
#if (MANAGER_NAME==MANAGER_NAME_INDEX)	
	int i;
		
	for(i=0;i<MANAGER_NODE_NUM;i++)
	{
		if((resManager[i].exp == resManager[exp].exp) &&(resManager[i].res_ma&rname.index_h))
			resManager[i].count--;
		if(resManager[i].count==0)
				resManager[i].index = 0;
	}
#endif 
     
	return ret;
}
/*******************************************************************************
* Function Name  : managerDelFileByFullName
* Description    : delete file by file name 
* Input          : int exp : manager handle
                      char *name : file name
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerDelFileByFullName(int exp,char *fullname)
{
	RES_NAME_T rname;
	int i,j;

	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->del_name);

    i = 0;
	j = 0;
	while(fullname[j])
	{
		if(fullname[j] == '/')
			i = j;
		j++;
	}
	if(i == 0)
		return managerDelFileByName(exp,fullname);

	if(string2name(&rname,&fullname[i+1])<0)
		return -1;

	namemake(fullname,&fullname[i+1],&rname);
	int ret = manager->del_name(resManager[exp].exp,&rname);
	if(ret<0)
		return -1;
#if (MANAGER_NAME==MANAGER_NAME_INDEX)	
		
	for(i=0;i<MANAGER_NODE_NUM;i++)
	{
		if((resManager[i].exp == resManager[exp].exp) &&(resManager[i].res_ma&rname.index_h))
			resManager[i].count--;
		if(resManager[i].count==0)
				resManager[i].index = 0;
	}
#endif 

    return 0;
}
/*******************************************************************************
* Function Name  : managerDelFileByIndex
* Description    : delete file by file index 
* Input          : int exp : manager handle
                      int index: index
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerDelFileByIndex(int exp,int index)
{
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->del_index);	
	INT32U ma;
	
    RES_NAME_T *tname = manager->name(resManager[exp].exp,index);	
	ma = tname->index_h;
	int ret = manager->del_index(resManager[exp].exp,index);
    if(ret<0)
    {
		return -1;
    }
	
#if (MANAGER_NAME==MANAGER_NAME_INDEX)	
    int i;
		
	for(i=0;i<MANAGER_NODE_NUM;i++)
	{
		if((resManager[i].exp == resManager[exp].exp) &&(ma&resManager[i].res_ma))
		{
			resManager[i].count--;
			if(resManager[i].count==0)
				resManager[i].index = 0;
		}
	}
	
#endif 
    return ret;
}
/*******************************************************************************
* Function Name  : managerDelFileAll
* Description    : delete all file
* Input          : int exp : manager handle
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerDelFileAll(int exp)
{
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->del_all);	

    
	int ret = manager->del_all(resManager[exp].exp);	
	if(ret<0)
		return -1;
#if (MANAGER_NAME==MANAGER_NAME_INDEX)	
    if(ret>=0)
    {
		int i;
		for(i=0;i<MANAGER_NODE_NUM;i++)
		{
			if(resManager[i].exp == resManager[exp].exp)
			{
				
				resManager[i].count = manager->count(resManager[i].exp,resManager[i].res_ma);
				if(resManager[i].count==0)
					resManager[i].index= 0;
			}
		}
    }
#endif   
    return ret;
}
/*******************************************************************************
* Function Name  : managerScanStart
* Description    : scan file list start
* Input          : int exp : manager handle
* Output         : none                                            
* Return         :
*******************************************************************************/
int managerScanStart(int exp)
{
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->scan_start);	

	return manager->scan_start(resManager[exp].exp);	
}
/*******************************************************************************
* Function Name  : managerScanEnd
* Description    : scan file list end
* Input          : int exp : manager handle
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerScanEnd(int exp)
{
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->scan_end);

	manager->scan_end(resManager[exp].exp);	
#if (MANAGER_NAME&MANAGER_NAME_INDEX)
	int i;
    for(i=0;i<MANAGER_NODE_NUM;i++)
    {
		if(resManager[i].exp == resManager[exp].exp)
		{
			resManager[i].index = manager->index(resManager[exp].exp,resManager[i].res_ma);
			deg_Printf("last index[%d] = %d\n",i,resManager[i].index);
		}
    }
#endif	

	return 0;
}
/*******************************************************************************
* Function Name  : managerRenameExt
* Description    : rename file
* Input          : int exp : manager handle
                      int index: file index
                      char *newname : new name
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerRenameExt(int exp,int index,char *path,char *newname)
{
	RES_NAME_T mname;
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);	
	ARG_CHECK(manager->rename);	

	namemake(path,newname,&mname);

	return manager->rename(resManager[exp].exp,index,&mname);
}
/*******************************************************************************
* Function Name  : managerRename
* Description    : rename file
* Input          : int exp : manager handle
                      char *odlname : old name
                      char *newname : new name
* Output         : none                                     

* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerRename(int exp,char *path,char *odlname,char *newname)
{
	RES_NAME_T name1;
	int index;
	ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->find);	

	namemake(path,odlname,&name1);

	index = manager->find(resManager[exp].exp,&name1,1);
	if(index<0)
		return -1;
	return managerRenameExt(exp,index,path,newname);
}
/*******************************************************************************
* Function Name  : managerFileNameCreate
* Description    : create a file name by suffix
* Input          : char *suffix : file type suffix
* Output         : none                                            
* Return         : char * file name if not NULL
*******************************************************************************/
char *managerFileNameCreater(int exp,char *suffix,char *fpath)
{
    int index = 0,scans=0;
	int mafile;
	char *path = resourcePathPool,*name; 
	RES_NAME_T rname;

//	ARG_CHECK3(exp,-1);
    if(exp<0)
		return NULL;

	int type = manangerCheckFileType(suffix);
	if(type == FILE_TYPE_AVI)
	{
	    mafile = MA_FILE_AVI;
	}
	else if(type == FILE_TYPE_JPG)
	{
	//	strcpy(path,FILEDIR_PHOTO);
		mafile = MA_FILE_JPG;
	}
	else if(type == FILE_TYPE_WAV)
	{
	//	strcpy(path,FILEDIR_AUDIO);
		mafile = MA_FILE_WAV;
	}
	else
		return NULL;  // unknow file type
    strcpy(path,fpath);

	if(manager == NULL || manager->find == NULL)
		return NULL;
	if(resManager[exp].count>=MANAGER_LIST_MAX)
		return NULL;
#if (MANAGER_NAME==MANAGER_NAME_INDEX)	
	index = resManager[exp].index;
    if(index>=MANAGER_LIST_MAX)
		index = 0;
#endif
    while(1)
    {
		 stringCreate(&rname,index);	
		 index++;
		 rname.index_h |= mafile;
		 
	     if(manager->find(resManager[exp].exp,&rname,mafile != MA_FILE_AVI)<0) // find the same file
			 break;
		 scans++;
	     if(scans>=MANAGER_LIST_MAX)
		 	return NULL;
	#if (MANAGER_NAME==MANAGER_NAME_INDEX)	 	     
		 if(index>=MANAGER_LIST_MAX)
		   index = 0;
	#else
         if(index>=10)
		 	return NULL;
	#endif
    }	
	
    name = &path[strlen(path)];
#if (MANAGER_NAME&MANAGER_NAME_INDEX)
    if(type == FILE_TYPE_AVI)
		strcpy(name,PREFIX_AVI);
	else if(type == FILE_TYPE_JPG)
		strcpy(name,PREFIX_JPG);
	else if(type == FILE_TYPE_WAV)
		strcpy(name,PREFIX_WAV);
	resManager[exp].index = index;
#endif 	
	name2string(&rname,name);//change to file name
	strcat(name,suffix); // add suffix

	return path;
}
/*******************************************************************************
* Function Name  : explorerListHook
* Description    : explorer file list order,user should fill this function 
* Input          : char *name1 : file 1
                      char *name2 : file 2
* Output         : none                                            
* Return         : 
*******************************************************************************/
int explorerListHook(RES_NAME_T *name1,RES_NAME_T *name2)
{
#if (MANAGER_NAME&MANAGER_NAME_DATETIME)	
//	if(strcmp(name1,name2)<0)
//		return 1;
    if((name2->index_h&MA_NAME_MASK)>(name1->index_h&MA_NAME_MASK))
		return 1;
	else if((name2->index_h&MA_NAME_MASK)==(name1->index_h&MA_NAME_MASK) && (name2->index_l>name1->index_l))
		return 1;
#endif			
	return 0;
}

/*******************************************************************************
* Function Name  : managerFileScan
* Description    : scan dir to list files
* Input          : int exp : explorer file list index
                      char *dir : dir to scan
* Output         : none                                            
* Return         : int ,file count
*******************************************************************************/
int managerFileScan(int exp,char *dir)
{
	FRESULT res;
	FILINFO finfo;
	RES_NAME_T tname;
	int count=0,type;
	char *path;
	char fname[32];
	TCHAR dirpath[32];
	TCHAR pattern[] = {'*','.','*',0};

	DIR drent;
    ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->add);

    deg_Printf("file scan : dir:%s\n",dir);
	count = 0;
	path = dir;
	while(*path)
	{
		if((*path == '/')&&(0 == *(path+1)))
		{
			break;
		}
		dirpath[count++] = *path++;
	}
	dirpath[count++] = 0;
   /* res = f_opendir(&drent,dirpath);
	if(res!=FR_OK)
	{
		deg_Printf("file scan : open dir fail <%d>\n",res);
		return 0;
	}
	res = f_readdir(&drent,&finfo);*/
	count = 0;
	

	res = f_findfirst(&drent,&finfo,dirpath,pattern);
	while(1)
	{         
		 if(res!=FR_OK || finfo.fname[0] == 0)
		 	break;
		 if(finfo.fname[0] != '.')
		 {
		 	Tchar2Ascii(finfo.fname, fname, sizeof(fname));
		 	 type = manangerCheckFileType(/*finfo.*/fname);
		 	 if(type<0)
		 	 {
			 	//strcpy(resourcePathPool,dir);
				//strcat(resourcePathPool,finfo.fname);
				//f_unlink(resourcePathPool);
				deg_Printf("unknow type : %s\n",resourcePathPool);
		 	 }
			 else
			 {
				 count++;
				 namemake(dir,/*finfo.*/fname,&tname);
				 resManager[exp].count++;
			#if 0//(MANAGER_NAME&MANAGER_NAME_INDEX)
				 if((nameIndex[exp])<(tname.index_l))
				 {
				 	nameIndex[exp] = tname.index_l;
				 }
			#endif	 
	             if(manager->add(resManager[exp].exp,&tname)<0)
				 	deg_Printf("e");
				//managerAddFile(exp,finfo.fname);

			 }
		 }
		 res = f_findnext(&drent,&finfo);
		// f_nextdir(&drent);
	}
	f_closedir(&drent);

    deg_Printf("file scan : total %d,[%d]=%d\n",count,exp,resManager[exp].count);

	return count;	
}
/*******************************************************************************
* Function Name  : managerNameChange
* Description    : change filename RES_NAME_T to char*
* Input          : int exp : explorer file list index
                     int index: index
                     char *name : string
                     int path : add path or not
* Output         : none                                            
* Return         : int ,file count
*******************************************************************************/
int managerNameChange(int exp,RES_NAME_T *rname,char *name,int path)
{
	int len,type;
    ARG_CHECK3(exp,-1);	
	ARG_CHECK(manager);
	ARG_CHECK(manager->name);

	
	if(rname == NULL)
	{
		deg_Printf("name fail!\n");
		return -1;
	}

    if(path)
    {
		if(rname->index_h&MA_FILE_AVI)
		{
			if((rname->index_h&MA_FILE_AVI) == MA_FILE_AVIA)
			    strcat(name,FILEDIR_VIDEOA);
			else if((rname->index_h&MA_FILE_AVI) == MA_FILE_AVIB)
			    strcat(name,FILEDIR_VIDEOB);
			else
				strcat(name,FILEDIR_VIDEO);
		}
		else if(rname->index_h&MA_FILE_JPG)
			strcat(name,FILEDIR_PHOTO);
		else if(rname->index_h&MA_FILE_WAV)
			strcat(name,FILEDIR_AUDIO);
		else
			return -1;
    }
	
	len = strlen(name);

	name2string(rname,&name[len]);
	
	if(rname->index_h&MA_FILE_AVI)
	{
#if (MANAGER_NAME&MANAGER_NAME_INDEX)
        strncpy(&name[len],PREFIX_AVI,3);
#endif
		strcat(name,SUFFIX_AVI);
		type =  FILE_TYPE_AVI;
	}
	else if(rname->index_h&MA_FILE_JPG)
	{
#if (MANAGER_NAME&MANAGER_NAME_INDEX)
        strncpy(&name[len],PREFIX_JPG,3);
#endif		
		strcat(name,SUFFIX_JPG);
		type =  FILE_TYPE_JPG;
	}
	else if(rname->index_h&MA_FILE_WAV)
	{
#if (MANAGER_NAME&MANAGER_NAME_INDEX)
        strncpy(&name[len],PREFIX_WAV,3);
#endif		
		strcat(name,SUFFIX_WAV);
		type = FILE_TYPE_WAV;
	}
	else
		type = -1;

	if(rname->index_h&MA_FILE_LOK)
	{
#if (MANAGER_NAME&MANAGER_NAME_INDEX)
        strncpy(&name[len],PREFIX_LOK,3);
     //   manager->print(exp);
	//	while(1);
#endif			
	}

	return type;
	
}
/*******************************************************************************
* Function Name  : managerSpaceCheck
* Description    : check sdc free sapce & delete file
* Input          : int exp : explorer file list index
                      char *dir : dir to scan
* Output         : none                                            
* Return         : int ,file count
*******************************************************************************/
int managerSpaceCheck(int exp,char *dir,int remain_space)
{
	char *filename = resourcePathPool;
	RES_NAME_T *tname;
	RES_NAME_T ttname;
	FILINFO Finfo;
	TCHAR fname[32+MANAGER_NAME_LEN+1];
	int idx,tidx;
	INT32U space;
    FRESULT res=FR_OK;
	ARG_CHECK3(exp,-1);	

//	u8 no_video_flag = 0;
	ARG_CHECK2(exp);	

	ARG_CHECK(manager);

    
	deg_Printf("space check : remain space %dM %dKB\n",remain_space>>10,remain_space&0x3ff);
    space = SysCtrl.sd_freesize;//fs_free_size();
	if(remain_space==0)
	{
		remain_space = space>>10;
		if(remain_space>REMAIN_MIN_VIDEO)
			return remain_space;
		else
			return -1;
	}	
    idx = 1;
	while(1)
	{
		if((space>remain_space)&& (resManager[exp].count<MANAGER_LIST_MAX)) // no space or file count max ,need delete file
			break;
		//if(manager->count(resManager[exp].exp)<idx)
		if(resManager[exp].count<=0)
		{
			deg_Printf("no video file.%d\n",exp);
			//no_video_flag = 1;
			break;
		}		
		//strcpy(filename,dir);
		*filename = 0;
		tname = manager->name(resManager[exp].exp,idx);
		if(tname==NULL)
			break;
		if((tname->index_h&MA_FILE_LOK) || (!(tname->index_h&resManager[exp].res_ma)))
		{
			idx++;
			continue;
		}
		
		if((tname->index_h&MA_FILE_JPG))	// don't del jpg file 
		{
			idx++;
			continue;
		}
		
		managerNameChange(exp,tname,filename,1);
		memcpy(&ttname,tname,sizeof(RES_NAME_T));
        ttname.index_h &= ~MA_FILE_BSY;
		Ascii2Tchar(filename, fname, sizeof(fname)/sizeof(fname[0]));
		res = f_stat(/*filename*/fname, &Finfo);
		if(res==FR_OK)
		{
			
		#if FILE_SYNC_DELETE>0	
			if(f_unlink_ext(filename)<0) // delete file step by step
			    res = f_unlink(filename);  // delete file now
		#else
			res = f_unlink(/*filename*/fname);  // delete file now
		#endif		    
		}
		else
			deg_Printf("fsstat 0: %s fail,%d\n",filename,res);
	    if(res == FR_OK)
	    {
			space+=Finfo.fsize>>10;
	        deamon_fsSizeModify(1,Finfo.fsize);
			deg_Printf("delete 0: %s.res = %d\n",filename,res);
			if(manager->del_index)
			{
				//if(manager->del_index(resManager[exp].exp,idx)<0)//; // delete first
				if(manager->del_name(resManager[exp].exp,&ttname)<0)
				    deg_Printf(" del index fail : %d\n",idx);
			}
			resManager[exp].count--;
	    }
		else
		{
			idx++;
			//continue;
		}
	//	ttname.index_h &= ~MA_FILE_BSY;
        if(((ttname.index_h&MA_FILE_AVI) == MA_FILE_AVI) || ((ttname.index_h&MA_FILE_AVI) == 0)) // only video a or b need delete
        {
			deg_Printf(">>>>>>>>>>>>>>>>>>>>>>>>not a b video.%x,%x\n",ttname.index_h,ttname.index_l);
			continue;
        }
//-------delete A OR B video the same time			
				
		if((ttname.index_h&MA_FILE_AVI) == MA_FILE_AVIA) // try to find b video
		{
			ttname.index_h = (ttname.index_h&(~MA_FILE_AVIA))|MA_FILE_AVIB;
		}
		else // try to find a video
		{
			ttname.index_h = (ttname.index_h&(~MA_FILE_AVIB))|MA_FILE_AVIA;
		}
		
		tidx = manager->find(resManager[exp].exp,&ttname,1); 
		if(tidx<0)// can not find 
		{
			deg_Printf(">>>>>>>>>>>>>>>>>>>>>>>>can not find.%x,%x\n",ttname.index_h,ttname.index_l);
			continue;
		}
		*filename = 0;
		managerNameChange(exp,&ttname,filename,1);
		Ascii2Tchar(filename, fname, sizeof(fname)/sizeof(fname[0]));
		res = f_stat(/*filename*/fname, &Finfo);
		if(res == FR_OK)
		{
			
		#if FILE_SYNC_DELETE>0	
			if(f_unlink_ext(filename)<0) // delete file step by step
			    res = f_unlink(filename);  // delete file now
		#else
			res = f_unlink(/*filename*/fname);  // delete file now
		#endif
		}
		else
			deg_Printf("fsstat 1: %s fail,%d\n",filename,res);
	    if(res == FR_OK)
	    {
			space+=Finfo.fsize>>10;
	        deamon_fsSizeModify(1,Finfo.fsize);
			deg_Printf("delete 1: %s.res = %d\n",filename,res);
			if(manager->del_index)
			{
				
				//if(manager->del_index(resManager[exp].exp,tidx))//; // delete first
				if(manager->del_name(resManager[exp].exp,&ttname)<0)
				{
					deg_Printf(" del index fail : %d\n",tidx);
				}
			}
			resManager[exp].count--;
	    }
		
		//del_name
	}
	if(manager->flush)
		manager->flush(resManager[exp].exp);
	idx = space>>10;
    deg_Printf("free space : %dG %dMB\n",idx>>10,idx&0x3ff);
	if(space>=remain_space){
		deg_Printf("--idx:%d\n",idx);
		return idx;
	}
	else
	{
		if(resManager[exp].res_ma == MA_FILE_AVI)
		    remain_space = REMAIN_MIN_VIDEO;
		else if(resManager[exp].res_ma == MA_FILE_JPG)
            remain_space = REMAIN_MIN_PHOTO;
		else if(resManager[exp].res_ma == MA_FILE_WAV)
		    remain_space = REMAIN_MIN_MUSIC;
		else
		    remain_space = REMAIN_MIN_VIDEO;
		if(idx>=remain_space){
			deg_Printf("--2--idx:%d\n",idx);
			return idx;
		}
		return -1; // no more space		
	}
}
/*******************************************************************************
* Function Name  : manangerCheckFileType
* Description    : check  file type by filename suffix
* Input          : char *name : file name
* Output         : none                                            
* Return         : type
*******************************************************************************/
int manangerCheckFileType(char *name)
{
	char *str,*point;

    str = name;
	point = NULL;
	while(*str)
	{
		if(*str == '.')
			point = str;
		str++;
	}

	if(point == NULL)
		return -1;
	else if(strcmp(point,SUFFIX_AVI)==0)
		return FILE_TYPE_AVI;
	else if(strcmp(point,SUFFIX_WAV)==0)
		return FILE_TYPE_WAV;
	else if(strcmp(point,SUFFIX_JPG)==0)
		return FILE_TYPE_JPG;
	else if((strcasecmp(point,SUFFIX_MP3)==0))
		return FILE_TYPE_MP3;

	return -1;
}
/*******************************************************************************
* Function Name  : manangerGetFileFullPathName
* Description    : get file full name .path & filename
* Input          : int exp : file list
                      int index: index
                      int *ftype : file type
* Output         : none                                            
* Return         : path
*******************************************************************************/
char *manangerGetFileFullPathName(int exp,int index,int *ftype)
{
	char *filename = resourcePathPool;
	int type;
	
	if(exp<0)
		return NULL;

	RES_NAME_T *rname = manager->name(resManager[exp].exp,index); // get name
	*filename = 0;
	deg("\nshow index :%d h:%d l:%d\n",index,rname->index_h,rname->index_l);

	type = managerNameChange(exp,rname,filename,1);
	if(type<0)
		return NULL;
	if(ftype)
		*ftype = type;
    
	return filename;
}
/*******************************************************************************
* Function Name  : manangerGetFileName
* Description    : get file full name .path & filename
* Input          : int exp : file list
                      int index: index
                      int *ftype : file type
* Output         : none                                            
* Return         : path
*******************************************************************************/
char *manangerGetFileName(int exp,int index)
{
	char *name = resourcePathPool;
	if(exp<0)
		return NULL;
	
	*name = 0;
	
    RES_NAME_T *rname = manager->name(resManager[exp].exp,index); // get name	
   	if(managerNameChange(exp,rname,name,0)<0)
		return NULL;

	return name;
}
/*******************************************************************************
* Function Name  : manangerFindFile
* Description    : find a and return the full path
* Input          : int exp : file list
                      int index: index
                      int *ftype : file type
* Output         : none                                            
* Return         : path
*******************************************************************************/
char *manangerFindFileAndDel(int exp,int *index,int type)
{
	RES_NAME_T *tname;
	int i;
	if(exp<0)
		return NULL;

	i = *index;
	while(1)
	{
		tname = manager->name(resManager[exp].exp,i);
		if(tname == NULL)
			return NULL;
		if((tname->index_h&type)&&(!(tname->index_h&MA_FILE_LOK))) // do not find locked file
			break;
		i++;
	}
	*index = i;
	char *name = manangerGetFileFullPathName(exp,i,NULL);
//	if(name)
//		manager->del_index(exp,i);
	return name;
}




void getExtName(const TCHAR *fname, char *extname, int len)
{
	const TCHAR *str,*point;

	str = fname;
	point = NULL;
	while(*str)
	{
		if(*str == '.')
		{
			point = str;
		}
		str++;
	}

	if(point == NULL)
	{
		extname[0] = 0;
	}
	else
	{
		Tchar2Ascii(point, extname, len);
	}
}









