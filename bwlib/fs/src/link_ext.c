

#include "../inc/ff.h"			/* Declarations of FatFs API */
#include "../inc/diskio.h"		/* Declarations of device I/O functions */



#if FF_UNLINK_EXT	

#define CHAIN_DEPTH		16
static DWORD removeChain_pool[CHAIN_DEPTH],removeChainCur;
static u8 removeChain_length,removeChain_outptr,removeChain_inptr;
static FATFS del_fatfs;
static FFOBJID del_obj;

#endif


FRESULT f_unlink_removeDir(const TCHAR* path,FATFS* pDelFS,DWORD *sclust)
{
#if FF_UNLINK_EXT		
	FRESULT res;
	DIR dj, sdj;
	BYTE *dir;
	DWORD dclst;
	DEF_NAMEBUF;


	/* Get logical drive number */
	res = find_volume(&dj.fs, &path, 1);
	if (res == FR_OK) {
		INIT_BUF(dj);
		res = follow_path(&dj, path);		/* Follow the file path */
		if (_FS_RPATH && res == FR_OK && (dj.fn[NS] & NS_DOT))
			res = FR_INVALID_NAME;			/* Cannot remove dot entry */
#if _FS_LOCK
		if (res == FR_OK) res = chk_lock(&dj, 2);	/* Cannot remove open file */
#endif
		if (res == FR_OK) {					/* The object is accessible */
			dir = dj.dir;
			if (!dir) {
				res = FR_INVALID_NAME;		/* Cannot remove the start directory */
			} else {
				if (dir[DIR_Attr] & AM_RDO)
					res = FR_DENIED;		/* Cannot remove R/O object */
			}
			dclst = ld_clust(dj.fs, dir);
			if (res == FR_OK && (dir[DIR_Attr] & AM_DIR)) 
			{	/* Is it a sub-dir? */
				if (dclst < 2) 
				{
					res = FR_INT_ERR;
				} 
				else 
				{
					mem_cpy(&sdj, &dj, sizeof (DIR));	/* Check if the sub-directory is empty or not */
					sdj.sclust = dclst;
					res = dir_sdi(&sdj, 2);		/* Exclude dot entries */
					if (res == FR_OK) 
					{
						res = dir_read(&sdj, 0);	/* Read an item */
						if (res == FR_OK		/* Not empty directory */
#if _FS_RPATH
						|| dclst == dj.fs->cdir	/* Current directory */
#endif
						) res = FR_DENIED;
						if (res == FR_NO_FILE) res = FR_OK;	/* Empty */
					}
				}
			}
			if (res == FR_OK) 
			{
				res = dir_remove(&dj);		/* Remove the directory entry */	
				if(res == FR_OK)
				{
					put_removeChain(dclst);
					if(sclust)
						*sclust = dclst;
					pDeleteFS = pDelFS;
				}
			}
		}
		FREE_BUF();
	}

	LEAVE_FF(dj.fs, res);
#endif	
    return FR_INT_ERR;
}

FRESULT f_unlink_removeChain(FFOBJID* obj,FATFS* pWorkFs,DWORD *pDelclst,DWORD mode)
{
#if FF_UNLINK_EXT		
	FRESULT res = FR_OK;
	DWORD clst;
	DWORD loopCnt,nxt;
	
	clst = *pDelclst;

	if(clst == 0)
		return FR_OK;

	if ((clst < 2) || (clst >= obj->fs->n_fatent)) 
	{	/* Check range */
		clst = 0;
		res = FR_INT_ERR;

	} 
	else 
	{
		res = FR_OK;
		loopCnt = 0;
		while (clst < obj->fs->n_fatent) 
		{			/* Not a last link? */
			nxt = get_fat(obj, clst);			/* Get cluster status */
			if (nxt == 0) 				/* Empty cluster? */
			{
				break;
			}
			else if (nxt == 1) /* Internal error? */
			{ 
				clst = 0; 
				res = FR_INT_ERR; 
				break; 
			}	
			else if (nxt == 0xFFFFFFFF)/* Disk error? */
			{
				clst = 0; 
				res = FR_DISK_ERR; 
				break; 
			}	
			res = put_fat(obj->fs, clst, 0);			/* Mark the cluster "empty" */
			if (res != FR_OK) {clst=0; break;}
			if (pWorkFs->free_clust != 0xFFFFFFFF) {	/* Update FSINFO */
				//work_fatfs.free_clust++;
				pWorkFs->fsi_flag |= 1;
				pWorkFs->free_clust++;
			}
			clst = nxt;	/* Next cluster */
			loopCnt++;
			if(mode == 0) 
			{	//slow speed delete chain
				if(((loopCnt * obj->fs->csize) >=48) && pWorkFs->free_clust>=1024) 
				{
					loopCnt=0;
					if (clst < obj->fs->n_fatent) 
					{
						res = FR_TIMEOUT;
						break;
					}
				}
			}
			else 
			{	//high speed delete chain
				if(loopCnt >= 256) 
				{
					loopCnt=0;
					if (clst < obj->fs->n_fatent) 
					{
						res = FR_TIMEOUT;
						break;
					}
				}
			}
			
		}
	}
	*pDelclst = clst;
	if(res == FR_OK) 
	{
		sync_window(obj->fs);
		sync_fs(pWorkFs);
	}
	return res;
#endif	
    return FR_OK;
}
/*******************************************************************************
* Function Name  : flush_msg
* Description    : clear the message
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void removeChainInit(FATFS *pDelFS)
{
#if FF_UNLINK_EXT	
	pDeleteFS = pDelFS;
    removeChain_inptr = 0;
    removeChain_outptr = 0;
	removeChain_length = 0;
	removeChainCur = 0;
#endif	
}


/*******************************************************************************
* Function Name  : put_msg
* Description    : add message
* Input          : msg
* Output         : None
* Return         : None
*******************************************************************************/
void put_removeChain(DWORD clst)
{
#if FF_UNLINK_EXT		
    removeChain_pool[removeChain_inptr] = clst;
    removeChain_inptr++;

    if (removeChain_inptr == CHAIN_DEPTH) {
        removeChain_inptr = 0;
    }
	removeChain_length++;
#endif	
}

u8 removeChainCheck(void)
{
#if FF_UNLINK_EXT		
	if(removeChain_length>=CHAIN_DEPTH)
		return 1;
	else
		return 0;
#endif	
    return 0;
}
/*******************************************************************************
* Function Name  : get_msg
* Description    : get message
* Input          : None
* Output         : msg
* Return         : None
*******************************************************************************/
DWORD get_removeChain(void)
{
#if FF_UNLINK_EXT		
	DWORD clst;
    if (removeChain_length==0) 
        return 0;
    removeChain_length--;
    clst = removeChain_pool[removeChain_outptr];
    removeChain_outptr++;
    if (removeChain_outptr == CHAIN_DEPTH) {
        removeChain_outptr = 0;
    }
    return clst;
#endif	
    return 0;
}
int f_unlink_ext_init(FATFS *fswork)
{
#if FF_UNLINK_EXT	
	memset(&del_fatfs,0,sizeof(FATFS));
	removeChainInit(&del_fatfs);
	mem_cpy(&del_fatfs,fswork,sizeof(FATFS));

	del_obj.fs = &del_fatfs;
#endif	
	return 0;
}


int f_unlink_ext_service(void)
{
#if FF_UNLINK_EXT	
	FRESULT res;
	
    if(removeChainCur==0)
		removeChainCur = get_removeChain();
	if(removeChainCur!=0)
	{
		res = f_unlink_removeChain(&del_obj, &work_fatfs,&removeChainCur,1); //  1
        if(res == FR_OK)
        {
			removeChainCur = 0;
			deg_Printf("fs del finish\n");
        }
		else if(res == FR_TIMEOUT)
		{
			
		}
		else
		{
			removeChainCur = 0;
			deg_Printf("fs del error.%d\n",res);
		}

		if(removeChainCur==0)
			removeChainCur = get_removeChain();
	}
	if(removeChainCur)
		return 1;
#endif	
    return 0;
}

int  f_unlink_ext(const TCHAR *filename)
{
#if FF_UNLINK_EXT	
	FRESULT res=FR_OK;
	
    if(removeChainCheck())  // remove qqueue is full
		return -1;
	if(removeChainCur != 0) 
	{
		put_removeChain(removeChainCur);	// put not finished clst into pool again.
		removeChainCur = 0;
	}
    res = f_unlink_removeDir(filename,&del_fatfs,&removeChainCur);

    f_unlink_removeChain(&del_obj, &work_fatfs,&removeChainCur,1);
 
   if(res==FR_OK)
   	   return 0;
#endif	   
	return -1;
}





























