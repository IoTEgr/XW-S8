




#include "../inc/diskio.h"
#include "../inc/fs.h"







typedef struct File_Node_S
{
	INT16U stat;
	INT16U rev;
	FIL     handle;
}File_Node_T;

//File_Node_T FSNodePool[FS_CFG_NODE_NUM];

#if FF_USE_FASTSEEK
#define  SZ_TBL 0x1000
DWORD clmt[SZ_TBL];
#endif

FATFS work_fatfs  __attribute__ ((section("._urx_buf_"))); // .sram_comm-->._urx_buf_
static File_Node_T FSNodePool[FS_CFG_NODE_NUM];// __attribute__ ((section(".sram_comm"))); 


int fs_nodeinit(void)
{
	int i;
    for(i=0;i<FS_CFG_NODE_NUM;i++)
    {
		FSNodePool[i].stat=0;
		memset(&FSNodePool[i],0,sizeof(File_Node_T));
    }
	return 0;
}

int fs_mount(int dev)
{	
	TCHAR path[1] = {0};
	memset(&work_fatfs,0,sizeof(FATFS));
	fs_nodeinit();
	if(f_mount(&work_fatfs, path, 1)!=FR_OK)
		return -1;
    switch(work_fatfs.fs_type)
	{
		case FS_FAT16 : deg_Printf("fs : fat16,fat ss = %d\n",work_fatfs.fatbase);break;
		case FS_FAT12 : deg_Printf("fs : fat12,fat ss = %d\n",work_fatfs.fatbase);break;
		case FS_FAT32 : deg_Printf("fs : fat32,fat ss = %d\n",work_fatfs.fatbase);break;
		default : deg_Printf("fs : unknow\n");break;
	}

	return 0;
}


int open(const TCHAR *name,INT32U op)
{
	FRESULT res;
	int i;

	for(i=0;i<FS_CFG_NODE_NUM;i++)
	{
		if(FSNodePool[i].stat==0)
			break;
	}

	if(i>=FS_CFG_NODE_NUM)
		return -1;
    memset(&FSNodePool[i],0,sizeof(File_Node_T));
	res = f_open(&FSNodePool[i].handle,name,op);
	if(res== FR_OK)
		FSNodePool[i].stat = 1;
	else
	{
		//deg_Printf("open file <%s> fail.%d\n",name,res);
		return -1;
	}
	return i;		
}
int close(int fd)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
	FSNodePool[fd].stat = 0;

	if( f_close(&FSNodePool[fd].handle)==FR_OK)
		return 0;
	else
	    return -1;
}
int read(int fd,void *buff,UINT len)
{
	UINT size=0;
	
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
	
	 if(f_read(&FSNodePool[fd].handle,buff,len,&size)!=FR_OK)
	 	  return -1;

	 return size;
}

int write(int fd,const void *buff,UINT len)
{
	UINT size=0;
	
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
	
	if(f_write(&FSNodePool[fd].handle,buff,len,&size)!=FR_OK)
		return -1;

	return size;
}

int lseek(int fd,INT32S offset,INT32U point)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -2;


	if(f_lseek(&FSNodePool[fd].handle,offset)!=FR_OK)
		return -3;
	return offset;
}

int fs_readsector(int fd,void *buffer,int sectors)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
    u32 start_sec = clst2sect(FSNodePool[fd].handle.obj.fs,FSNodePool[fd].handle.obj.sclust);
	return fp_storage_rd(buffer,start_sec,sectors);
}
int fs_writesector(int fd,void *buffer,int sectors)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
    u32 start_sec = clst2sect(FSNodePool[fd].handle.obj.fs,FSNodePool[fd].handle.obj.sclust);
	return fp_storage_wr(buffer,start_sec,sectors);	
}
INT32U fs_getsector(int fd,INT32U offset,int from)
{
	INT32U sector;
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
    FIL *file = &FSNodePool[fd].handle;
	if(from == 0)  // start
		sector = clst2sect(file->obj.fs,file->obj.sclust);
	else if(from == -1) // end
	    sector =  (((file->fptr/512)&(file->obj.fs->csize - 1))+clst2sect(file->obj.fs,file->clust));
	else
		sector =  clst2sect(file->obj.fs,offset);  //cluster no 

    if(sector<=file->obj.fs->database)
    {
		deg_Printf(">>>>WARNING : fs error .%d,%d\n",sector,file->obj.fs->database);
	/*	while(1);
		{
			deg_Printf("a");
		}*/
    }
	return sector;
}
INT32U fs_getclustersize(void)
{
	return (work_fatfs.csize<<9);
}
INT32U fs_getcluster(int fd,INT32U cluster_no)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
    FIL *file = &FSNodePool[fd].handle;
	if(cluster_no == 0)
		return file->obj.sclust;
	return FEX_getlink_clust(&file->obj,cluster_no);//FEX_getfree_clust(file->fs,cluster_no);
}
INT32U fs_size(int fd)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
	
	return FSNodePool[fd].handle.obj.objsize;
}
INT32U fs_size2(int fd)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
	
	return FSNodePool[fd].handle.obj.objsize;	
}
INT32U fs_free_size(void)
{
	TCHAR path[1] = {0};
	FATFS *pfatfs = &work_fatfs;
	u32 free_clst;
	
	f_getfree(path, &free_clst, &pfatfs);
	return (free_clst*work_fatfs.csize);
}


int fs_fastseek_init(int fd)
{
#if FF_USE_FASTSEEK
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;


	FSNodePool[fd].handle.cltbl = clmt;
	clmt[0] = SZ_TBL;
	if( f_lseek(&FSNodePool[fd].handle, CREATE_LINKMAP)!=FR_OK)
		return -1;
#endif
	return 0;
} 


int fs_merge(int fd1,int fd2)
{

	return 0;
}
int fs_bound(int fd1,int fd2)
{
	return 0;
}

INT32U fs_tell(int fd)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;
	
	return FSNodePool[fd].handle.fptr;	
}

int fs_sync(int fd)
{
	if((fd<0) || (fd>=FS_CFG_NODE_NUM))
		return -1;
	if(FSNodePool[fd].stat==0)
		return -1;

	if(f_sync(&FSNodePool[fd].handle)==FR_OK)
		return 0;
	else
	    return -1;
}

int Ascii2Tchar(const char *ascStr, TCHAR *tchStr, int len)
{
	int ret = 0;

	if (ascStr == NULL || tchStr == NULL)
	{
		return 0;
	}

	while (ret < len - 1)
	{
		tchStr[ret] = ascStr[ret];
		ret++;
	}
	tchStr[ret] = 0;

	return ret;
}

int Tchar2Ascii(const TCHAR *tchStr, char *ascStr, int len)
{
	int ret = 0;

	if (ascStr == NULL || tchStr == NULL)
	{
		return 0;
	}

	while (ret < len - 1)
	{
		ascStr[ret] = tchStr[ret];
		ret++;
	}
	ascStr[ret] = 0;

	return ret;
}

int tchNCopy(TCHAR *dst, int dst_len, const TCHAR *src)
{
	int len = 0;

	if (dst == NULL || src == NULL || dst_len < 0)
	{
		return 0;
	}

	dst_len--;
	while(dst_len && *src)
	{
		*dst = *src;
		dst++;
		src++;
		len++;
	}
	*dst = 0;

	return len;
}

int tchLen(const TCHAR *tchStr)
{
	int len = 0;

	if (tchStr == NULL)
	{
		return 0;
	}

	while(tchStr[len])
	{
		len++;
	}

	return len;
}


