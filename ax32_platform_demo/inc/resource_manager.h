/****************************************************************************
                                       MANAGER
                       THE APPOTECH X OPERATION SYSTEM
                                RESOURCE MANAGER
                
                            (C) COPYRIGHT 2016 BUILDWIN 
                                
                    BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
                
* File Name   : resource_manager.c
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
#ifndef  RESOURCE_MANAGER_H
    #define  RESOURCE_MANAGER_H

#define  MANAGER_EXPLORER   1
#define  MANAGER_FILELIST   2



#define  MANAGER_LIST_USE     MANAGER_FILELIST

#define  MANAGER_NAME_LEN     19

#define  MANAGER_LIST_NUM     1    // at least,support 3-list
#define  MANAGER_NODE_NUM     5

#define  MANAGER_LIST_MAX     1000//1024 // max file number

#define  MANAGER_NAME_TIME         1   // file name by date time
#define  MANAGER_NAME_DATE         2
#define  MANAGER_NAME_DATETIME   3
#define  MANAGER_NAME_INDEX	     4   // file name index

#define  MANAGER_NAME       MANAGER_NAME_INDEX//MANAGER_NAME_TIME

#define  SUFFIX_TYPE_AVI   1
#define  SUFFIX_TYPE_JPG   2
#define  SUFFIX_TYPE_WAV   3

#define  MA_FILE_AVIB   0x01000000
#define  MA_FILE_AVIA   0x02000000
#define  MA_FILE_AVI    0x03000000
#define  MA_FILE_JPG    0x04000000
#define  MA_FILE_WAV    0x08000000
#define  MA_FILE_LOK    0x10000000
#define  MA_FILE_BSY    0x20000000
#define  MA_FILE_IVL    0x40000000
#define  MA_FILE_MASK   0xff000000
#define  MA_FILE_TMASK  0x0f000000
#define  MA_NAME_MASK   0x00ffffff



#define  SUFFIX_AVI  ".AVI"
#define  SUFFIX_JPG  ".JPG"
#define  SUFFIX_WAV  ".WAV"
#define  SUFFIX_MP3  ".MP3"


#define  PREFIX_AVI  "MOV"
#define  PREFIX_JPG  "PHO"
#define  PREFIX_WAV  "MIC"
#define  PREFIX_LOK  "LOK"

#define  REMAIN_MIN_VIDEO     (32)  // 32 M ->video
#define  REMAIN_MIN_PHOTO     (3)  // 3M ->photo
#define  REMAIN_MIN_MUSIC    (1)  // 1M ->music

typedef struct RES_NAME_S
{
     INT32U index_h;
	 INT32U index_l; //pohot num
}RES_NAME_T;




typedef struct FILE_MANAGER_S
{
	int (*init)(void);
	int (*uninit)(void);
	int (*create)(char *path);
	int (*destory)(int handle);
	int (*count)(int handle,int ma);
	RES_NAME_T* (*name)(int handle,int index);
	int (*find)(int handle,RES_NAME_T *name,int cmp);
	int (*add)(int handle,RES_NAME_T *name);
	int (*del_name)(int handle,RES_NAME_T *name);
	int (*del_index)(int handle,int index);
	int (*del_all)(int handle);
	int (*scan_start)(int handle);
	int (*scan_end)(int handle);
	int (*flush)(int handle);
	int (*rename)(int handle,int index,RES_NAME_T *name);
	int (*print)(int handle);
	int (*drity)(int handle);
	int (*index)(int handle,INT32U ma);
}FILE_MANAGER_T;


/*******************************************************************************
* Function Name  : managerInit
* Description    : initial resource manager
* Input          : 
* Output         : none                                            
* Return         : handle
*******************************************************************************/
int managerInit(void);
/*******************************************************************************
* Function Name  : managerCreate
* Description    : create a manager
* Input          : 
* Output         : none                                            
* Return         : handle
*******************************************************************************/
int managerCreate(char *path,INT32U type,int combain);
/*******************************************************************************
* Function Name  : managerDestory
* Description    : destory a manager
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerDestory(int exp);
/*******************************************************************************
* Function Name  : managerFileCount
* Description    : get file list file count
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerFileCount(int exp);
/*******************************************************************************
* Function Name  : managerFileCountEx
* Description    : get file list file count only sigle list
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerFileCountEx(int exp);
/*******************************************************************************
* Function Name  : managerAddFile
* Description    : add file to list
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerAddFile(int exp,char *path,char *name);
/*******************************************************************************
* Function Name  : managerAddFileEx
* Description    : add file to list
* Input          : int exp : handle
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerAddFileEx(int exp,char *fullname);
/*******************************************************************************
* Function Name  : managerDelFileByName
* Description    : delete file by file name 
* Input          : int exp : manager handle
                      char *name : file name
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerDelFileByName(int exp,char *name);
/*******************************************************************************
* Function Name  : managerDelFileByFullName
* Description    : delete file by file name 
* Input          : int exp : manager handle
                      char *name : file name
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerDelFileByFullName(int exp,char *fullname);
/*******************************************************************************
* Function Name  : managerDelFileByName
* Description    : delete file by file index 
* Input          : int exp : manager handle
                      int index: index
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerDelFileByIndex(int exp,int index);
/*******************************************************************************
* Function Name  : managerDelFileByName
* Description    : delete all file
* Input          : int exp : manager handle
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerDelFileAll(int exp);
/*******************************************************************************
* Function Name  : managerScanStart
* Description    : scan file list start
* Input          : int exp : manager handle
* Output         : none                                            
* Return         :
*******************************************************************************/
int managerScanStart(int exp);
/*******************************************************************************
* Function Name  : managerDelFileByName
* Description    : scan file list end
* Input          : int exp : manager handle
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerScanEnd(int exp);
/*******************************************************************************
* Function Name  : managerRenameExt
* Description    : rename file
* Input          : int exp : manager handle
                      int index: file index
                      char *newname : new name
* Output         : none                                            
* Return         : 
*******************************************************************************/
int managerRenameExt(int exp,int index,char *path,char *newname);
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
int managerRename(int exp,char *path,char *odlname,char *newname);
/*******************************************************************************
* Function Name  : managerFileNameCreate
* Description	 : create a file name by suffix
* Input 		 : char *suffix : file type suffix
* Output		 : none 										   
* Return		 : char * file name if not NULL
*******************************************************************************/
char *managerFileNameCreater(int exp,char *suffix,char *fpath);
/*******************************************************************************
* Function Name  : managerFileScan
* Description    : scan dir to list files
* Input          : int exp : explorer file list index
                      char *dir : dir to scan
* Output         : none                                            
* Return         : int ,file count
*******************************************************************************/
int managerFileScan(int exp,char *dir);
/*******************************************************************************
* Function Name  : managerSpaceCheck
* Description    : check sdc free sapce & delete file
* Input          : int exp : explorer file list index
                      char *dir : dir to scan
* Output         : none                                            
* Return         : int ,file count
*******************************************************************************/
int managerSpaceCheck(int exp,char *dir,int remain_space);
/*******************************************************************************
* Function Name  : manangerCheckFileType
* Description    : check  file type by filename suffix
* Input          : char *name : file name
* Output         : none                                            
* Return         : type
*******************************************************************************/
int manangerCheckFileType(char *name);
/*******************************************************************************
* Function Name  : manangerGetFileFullPathName
* Description    : get file full name .path & filename
* Input          : int exp : file list
                      int index: index
                      int *ftype : file type
* Output         : none                                            
* Return         : path
*******************************************************************************/
char *manangerGetFileFullPathName(int exp,int index,int *ftype);
/*******************************************************************************
* Function Name  : manangerGetFileName
* Description    : get file full name .path & filename
* Input          : int exp : file list
                      int index: index
                      int *ftype : file type
* Output         : none                                            
* Return         : path
*******************************************************************************/
char *manangerGetFileName(int exp,int index);
/*******************************************************************************
* Function Name  : managerLockFile
* Description    : lock file
* Input          : char *name : file name,lock file to rename
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerLockFile(char *name);
/*******************************************************************************
* Function Name  : managerLockFile
* Description    : lock file
* Input          : int exp : handle
                      int index: index

* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerLockFileExt(int exp,int index);
/*******************************************************************************
* Function Name  : managerUnlockFile
* Description    : unlock file
* Input          : char *name : file name,lock file to rename
* Output         : none                                            
* Return         : int
*******************************************************************************/
int managerUnlockFile(char *name);
/*******************************************************************************
* Function Name  : managerUnlockFileExt
* Description    : unlock file
* Input          : int exp : handle
                      int index: index                                        
* Return         : int
*******************************************************************************/
int managerUnlockFileExt(int exp,int index);
/*******************************************************************************
* Function Name  : managerCheckFile
* Description    : check file is lcoked or not
* Input          : char *name : file name
* Output         : none                                            
* Return         : int 1 : locked
                           0 : unlocked
*******************************************************************************/
int managerCheckLock(char *name);
/*******************************************************************************
* Function Name  : managerCheckLockExt
* Description    : check file is lcoked or not
* Input          : int exp : handle
                      int index: index
* Output         : none                                            
* Return         : int 1 : locked
                           0 : unlocked
*******************************************************************************/
int managerCheckLockExt(int exp,int index);
/*******************************************************************************
* Function Name  : managerFlush
* Description    : flush file list
* Input          : int exp : handle
* Output         : none                                            
* Return         : int 
*******************************************************************************/
int managerFlush(int exp);
/*******************************************************************************
* Function Name  : manangerFindFile
* Description    : find a and return the full path
* Input          : int exp : file list
                      int index: index
                      int *ftype : file type
* Output         : none                                            
* Return         : path
*******************************************************************************/
char *manangerFindFileAndDel(int exp,int *index,int type);



































#endif


