#include "uiMenuDelCurRes.c"

#define MP3_FILE_LEN 128
#define MP3_FILE_NUM 256
#define  PLAY_WAIT_KEYSOUND_END           while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY){XOSTimeDly(10);}

typedef struct {
	TCHAR fname[MP3_FILE_LEN];
}FNAME_T;

static FNAME_T *g_mp3_name = NULL; //[256][128];

u32 audioplayer_file_list_scan2(char *dir, FNAME_T *name_arr)
{
	FRESULT res;
	FILINFO finfo;
	int count=0,type;
	TCHAR dirpath[64];
	TCHAR pattern[] = {'*','.','*',0};
	char extname[8];
	char *path;
	DIR drent;
    //deg_Printf("file scan : dir:%s\n",dir);
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

	count = 0;
	res = f_findfirst(&drent,&finfo,dirpath,pattern);
	while(1)
	{         
		 if(res!=FR_OK || finfo.fname[0] == 0)
		 	break;
		 if(finfo.fname[0] != '.')
		 {
		 	 //deg_Printf("scan:%s\n",finfo.fname);
		 	 getExtName(finfo.fname, extname, sizeof(extname));
		 	 type = manangerCheckFileType(extname);
		 	 if(type<0)
		 	 {
				 //deg_Printf("unknow type:%s\n",finfo.fname);
		 	 }
			 else
			 {
			 	 //deg_Printf("mp3:%s\n",finfo.fname);
				 if(FILE_TYPE_MP3==type)
				 {
					 //deg_Printf("mp3 file\n");
					 //deg_Printf("scan:%s\n",finfo.fname);

#if 0
					  memcpy(name_arr[count].fname,FILEDIR_MP3,sizeof(FILEDIR_MP3));
					  strcpy((char *)name_arr[count].fname+strlen(FILEDIR_MP3),finfo.fname);
					  //memcpy(mp3_name[count], finfo.fname, 50);
#else
					 int dirLen = strlen(FILEDIR_MP3);
					 Ascii2Tchar(FILEDIR_MP3, name_arr[count].fname, MP3_FILE_LEN);
					 tchNCopy(name_arr[count].fname+dirLen, MP3_FILE_LEN-dirLen, finfo.fname);
#endif

					 count++;
					 // deg_Printf("mp3_name:%s,,%d\n",name_arr[count].fname,count);
					 if(count>=MP3_FILE_NUM)
					 {
						break;
					 }
				 }
			 }
		 }
		 res = f_findnext(&drent,&finfo);

	}
	f_closedir(&drent);

	return count;	

}

static uint32 getdelCurResInfor(uint32 item,uint32* image,uint32* str)
{
	if(item==0)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_SET_DELETECUR;
	}
	else if(item==1)
	{
		if(image)
			*image=INVALID_RES_ID;
		if(str)
			*str=R_ID_STR_SET_DELETEALL;
	}
	return 0;
}

extern TCHAR cur_file_name[/*256*/1];
u32 mp3_list_idx;
u32 mp3_list_num;
u32 mp3_list_num2;
u8 mp3_flag=0;
static int delCurKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	TCHAR fname[MP3_FILE_LEN];
	uint32 keyState=KEY_STATE_INVALID;
	uint32 item;
	char *name;
	int i,cnt;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		item=itemManageGetCurrentItem(winItem(handle,DELCUR_SELECT_ID));

		if(item==0)
		{
			if(mp3_flag){
				if(mp3_list_num<=0)
				{
					uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_NO_FILES,2);
					return 0;
				}

				if(f_unlink(cur_file_name)==FR_OK)
				{
					deg_Printf("MP3->ok\n");
					//managerDelFileByIndex(SysCtrl.avi_list,SysCtrl.file_index);
					//if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
					//	SysCtrl.file_index=managerFileCount(SysCtrl.avi_list);
					//deamon_fsFreeSize();
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
				}
				else
				{
					deg_Printf("MP3->fail\n");
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
				}
				/*
				deg_Printf("MP3 : %s.",cur_file_name);
				if(mp3_list_num>0)
				{
				get_mp3_file_name(FILEDIR_MP3, mp3_list_idx);
				deg_Printf("MP3 : %s.",cur_file_name);
				
				mp3_list_num2=audioplayer_file_list_scan2(FILEDIR_MP3);
				deg_Printf("mp3_list_num2=%d\n",mp3_list_num2);
				//==debug name==
				for(i=0;i<mp3_list_num2;i++)
					deg_Printf("list_name[%d]=%s\n",i,mp3_name[i]);
				}*/
			}else{
				#if ENABLE_FLASH_PHOTO
				if(NULL==SysCtrl.sdcard)
				{
					if(spi_udisk_jpg_num()>0)
					{
						if(0==spi_udisk_del_jpg(SysCtrl.file_index))
						{
							if(SysCtrl.file_index>=spi_udisk_jpg_num())
							{
								SysCtrl.file_index=spi_udisk_jpg_num()-1;
							}
							deg_Printf("SysCtrl.file_index=%d\n",SysCtrl.file_index);
							uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
						}
						else
						{
							uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
						}
					}
					else
					{
						uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_NO_FILES,2);
						return;
					}
					
				}else
				#endif
				{
					if(managerFileCount(SysCtrl.avi_list)<=0)
					{
						uiOpenWindow(&tipsWindow,2,R_ID_STR_TIPS_NO_FILES,2);
						return 0;
					}
					if(managerCheckLockExt(SysCtrl.avi_list,SysCtrl.file_index)<=0)
					{
						name = manangerGetFileFullPathName(SysCtrl.avi_list,SysCtrl.file_index,NULL);
						deg_Printf("delete3 : %d.",SysCtrl.avi_list);
						deg_Printf("delete4 : %d.",SysCtrl.file_index);
						deg_Printf("delete : %s.",name);
						
						Ascii2Tchar(name, fname, sizeof(fname)/sizeof(fname[0]));
						if(f_unlink(/*name*/fname)==FR_OK)
						{
							deg_Printf("->ok\n");
							managerDelFileByIndex(SysCtrl.avi_list,SysCtrl.file_index);
							if(SysCtrl.file_index>managerFileCount(SysCtrl.avi_list))
								SysCtrl.file_index=managerFileCount(SysCtrl.avi_list);
							deamon_fsFreeSize();
							uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
						}
						else
						{
							deg_Printf("->fail\n");
							uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
						}
					}
					else
						uiOpenWindow(&tipsWindow,2,R_ID_STR_SET_LOCKED,2);
				}
			}
		}
		else
		{
			if(mp3_flag){
				g_mp3_name = (FNAME_T *)hal_sysMemMalloc(MP3_FILE_NUM * sizeof(FNAME_T), 64);
				if (g_mp3_name == NULL) {
					return 0;
				}
				uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_TIPS_PLEASE_WAITING,2);
				mp3_list_num2=audioplayer_file_list_scan2(FILEDIR_MP3, g_mp3_name);
				for(i=0;i<mp3_list_num2;i++)
				{
						//name = manangerGetFileFullPathName(SysCtrl.avi_list,i+1,NULL);
						deg_Printf("delete : %s.",g_mp3_name[i].fname);
						if(f_unlink(g_mp3_name[i].fname)==FR_OK)
							deg_Printf("MP3->ok\n");
						else
							deg_Printf("MP3->fail\n");
				}
				if(mp3_list_num2>0)
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
				else
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
				//deamon_fsFreeSize();
				//managerDelFileAll(SysCtrl.avi_list);
				//SysCtrl.file_index=managerFileCount(SysCtrl.avi_list);
				hal_sysMemFree(g_mp3_name);
				g_mp3_name = NULL;
			}else{
				#if ENABLE_FLASH_PHOTO
				if(NULL==SysCtrl.sdcard)
				{
					spi_udisk_format(FLASH_CAPACITY);
					uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
					backBrushBlack();
				}else
				#endif
				{
					uiOpenWinAndShow(&tipsWindow,2,R_ID_STR_TIPS_PLEASE_WAITING,2);
					cnt = managerFileCount(SysCtrl.avi_list);
					for(i=0;i<cnt;i++)
					{
						ax32xx_wdtClear();
						if(managerCheckLockExt(SysCtrl.avi_list,i+1)<=0)
						{
							name = manangerGetFileFullPathName(SysCtrl.avi_list,i+1,NULL);
							deg_Printf("delete : %s.",name);
							
							Ascii2Tchar(name, fname, sizeof(fname)/sizeof(fname[0]));
							if(f_unlink(/*name*/fname)==FR_OK)
								deg_Printf("->ok\n");
							else
								deg_Printf("->fail\n");
						}
					}
					if(cnt>0)
						uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_SUCCESS,2);
					else
						uiOpenWindow(&tipsWindow,2,R_ID_STR_COM_FAILED,2);
					deamon_fsFreeSize();
					managerDelFileAll(SysCtrl.avi_list);
					SysCtrl.file_index=managerFileCount(SysCtrl.avi_list);
					backBrushBlack();
				}
			}
		}
			//winDestroy(&handle);
	}
	return 0;
}
static int delCurKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		itemManagePreItem(winItem(handle,DELCUR_SELECT_ID));
		deamon_keysound_play();
		 PLAY_WAIT_KEYSOUND_END;
	}
	return 0;
}
static int delCurKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED||keyState==KEY_CONTINUE)
	{
		 itemManageNextItem(winItem(handle,DELCUR_SELECT_ID));
		 deamon_keysound_play();
		 PLAY_WAIT_KEYSOUND_END;
	}
	return 0;
}
static int delCurKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		winDestroy(&handle);
	}
	return 0;
}
#if 0
static int delCurKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
#endif
u8 mp3_flag2=0;
static int delCurOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{	if(mp3_flag)mp3_flag2=1;
	deg_Printf("delCur Open Win!!!\n");
	itemManageSetRowSum(winItem(handle,DELCUR_SELECT_ID),2,R1h(40));
	itemManageSetColumnSumWithGap(winItem(handle,DELCUR_SELECT_ID),2,2,R1w(220),0);
	itemManageCreateItem(winItem(handle,DELCUR_SELECT_ID),itemCreatemenuOption,getdelCurResInfor,2);
	itemManageSetCharInfor(winItem(handle,DELCUR_SELECT_ID),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	itemManageSetSelectColor(winItem(handle,DELCUR_SELECT_ID),R_COLOR_BLUE1);
	itemManageSetUnselectColor(winItem(handle,DELCUR_SELECT_ID),R_COLOR_BLACK);

	itemManageSetCurItem(winItem(handle,DELCUR_SELECT_ID),0);
	
	return 0;
}
static int delCurCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delCur Close Win!!!\n");
	return 0;
}
static int delCurWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("delCur WinChild Close!!!\n");
	winDestroy(&handle);
	return 0;
}


msgDealInfor delCurMsgDeal[]=
{
	{SYS_OPEN_WINDOW,delCurOpenWin},
	{SYS_CLOSE_WINDOW,delCurCloseWin},
	{SYS_CHILE_COLSE,delCurWinChildClose},
	{KEY_EVENT_OK,delCurKeyMsgOk},
	{KEY_EVENT_UP,delCurKeyMsgUp},
	{KEY_EVENT_DOWN,delCurKeyMsgDown},
	{KEY_EVENT_RETURN,delCurKeyMsgMenu},
//	{KEY_EVENT_MODE,delCurKeyMsgMode},
	{EVENT_MAX,NULL},
};

WINDOW(delCurWindow,delCurMsgDeal,delCurWin)


