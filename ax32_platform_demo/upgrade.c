/****************************************************************************
**
 **                              DISPLAY
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  DISPLAY
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : display.c
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


#define UPGRADE_VERIFY          1
#define UPGRADE_DELETE          0   // delete update file
#define UPGRADE_RESOURCE       0   // update resource when upgrade end
#if UPGRADE_VERIFY
    #define  UPGRADE_BACKUP     0
#else
    #define  UPGRADE_BACKUP 	 0
#endif
//#define UPGRADE_PATH_NAME      "DestBin.bin"
const TCHAR UPGRADE_PATH_NAME[] = {'D','e','s','t','B','i','n','.','b','i','n',0};



winHandle progressBar;
winHandle strHandle;


void upgrade_uiProgress(uint32 progress)
{
    hal_wdtClear();
	winSetPorgressRate(progressBar,progress);
	drawUIService(false);
}

void upgrade_led_control()	// 200 ms
{
	static u8 flag=0;

	if(0==flag)
	{
		flag=1;
		boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,1);
	}
	else
	{
		flag=0;
		boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);
	}
}
/*******************************************************************************
* Function Name  : upgrade
* Description    : upgrade
* Input          : 
* Output         : none                                            
* Return         : int : 0: ->upgrade fail
                            -1:->no upgrade file
                           <-1:->upgrade fail,fireware maybe error
*******************************************************************************/
int upgrade(void)
{
	FHANDLE fd;
	int ret,size,progress,i,addr,res;
	INT8U *buffer=NULL;
#if UPGRADE_BACKUP	
    FHANDLE fd1;
#endif
	strHandle=INVALID_HANDLE;
	progressBar=INVALID_HANDLE;
    if(SysCtrl.sdcard==SDC_STAT_NULL)
    {
		deg_Printf("upgrade : no sdc\n");
		return -1;
    }
	res = fs_mount(0);
	if(res<0)
	{
		hal_sdSetCardState(SDC_STATE_NULL);
		if(hal_sdInit(SD_BUS_WIDTH1)>=0) // sdc intial 1line
		{
			res = fs_mount(0);
		}
	}
	if(res<0)
	{
		deg_Printf("upgrade : sdc mount fail\n");
		return -1;
	}
	deg_Printf("upgrade : find upgrade file <%s>->",UPGRADE_PATH_NAME);
//-------find upgrade file
	ret = open(UPGRADE_PATH_NAME,FA_READ);
	if(ret<0)
	{
		deg_Printf("fail\n");
		return -1;
	}
//	ax32xx_wdtEnable(0);//must off WDT  ,do not close WDT here
    deg_Printf("ok\n");
//-------get file size	
	fd = (FHANDLE)ret;
	size = fs_size(fd);
//-------get cache memory
	ret = -1;
	buffer = (INT8U *)hal_sysMemMalloc(4096,64);//shareMemMalloc(4096);
    if(buffer == NULL)
         goto UPGRADE_END;
	

    deg_Printf("upgrade : start.>>Please do not power off.\n");
//-------erase flash

	deg_Printf("upgrade : erase.\n");
	winSetfgColor(rectCreate(R1x(0),R1y(0),R1w(320),R1h(240),INVALID_HANDLE,WIN_ABS_POS,0), R_COLOR_GRAY);
	progressBar=progressBarCreate(R1x(70),R1y(117),R1w(180),R1h(6),INVALID_HANDLE,WIN_ABS_POS,0);
	winSetfgColor(progressBar, R_COLOR_BLUE);
	winSetbgColor(progressBar, R_COLOR_GRAY3);
	strHandle=stringIconCreate(R1x(70),R1y(125),R1w(180),R1h(35),INVALID_HANDLE,WIN_ABS_POS,0);
	winSetStrInfor(strHandle,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_BLUE);
	winSetResid(strHandle,R_ID_STR_UPDATE_START);
	drawUIService(false);
	hal_wdtEnable(0);
	
	XWorkCreate(200*X_TICK,upgrade_led_control);
	
	if(hal_spiFlashEraseChip()<0)
	{
		deg_Printf("upgrade : erase chip err!\n");
		// need try erase Sector
		u32 temp_flash_addr = 0;
		for(i = 0;(i * 0x1000) < size;i++)	// erase sector is 4K 
		{
			if(hal_spiFlashEraseSector(temp_flash_addr)<0)
			{
				deg_Printf("upgrade : erase sector err!\n");
			}
			temp_flash_addr+=0x1000;
		}
	}
	hal_wdtEnable(1);

//-------write
    deg_Printf("upgrade : write.\n");
    
	addr = 512;
	i = addr;
	ret = 0;
	progress = 0;
	lseek(fd,addr,0);
    while(1)
    {
		ret = read(fd,buffer,4096);
		if(ret<0)
		{
			ret = -2;
			goto UPGRADE_END;
		}
		if(ret==0) // no data
			break;
		if(hal_spiFlashWrite(addr,(INT32U)buffer,ret)<0)
		{
			ret = -2;
			goto UPGRADE_END;
		}

		i+=ret;
		addr+=ret;
		if(((i*100)/size)!=progress)
		{
			progress = (i*100)/size;
			deg_Printf("\rupgrade : %d%",progress);
		#if UPGRADE_VERIFY	
			upgrade_uiProgress(progress>>1);
		#else
		    upgrade_uiProgress(progress);
		#endif
		}
		if(i>=size)
			break;	
		
    }
//-----write back boot sector	
	lseek(fd,0,0);
	read(fd,buffer,512);
	hal_spiFlashWrite(0,(INT32U)buffer,512);

	
	deg_Printf("\n");
//-----verify
#if UPGRADE_VERIFY > 0
    int j;
	deg_Printf("upgrade : verify.\n");
    i = 0;
	addr = 0;
	progress = 0;
	lseek(fd,0,0);
#if UPGRADE_BACKUP		
	fd1 = (FHANDLE)open((const char *)"backup.bin",FA_READ|FA_WRITE|FA_CREATE_ALWAYS);
#endif
	while(1)
	{
		ret = read(fd,buffer,2048);
		if(ret<0)
		{
			ret = -3;
			goto UPGRADE_END;
		}
		if(hal_spiFlashRead(addr,(INT32U)(buffer+2048),2048)<0)
		{
			ret = -3;
			goto UPGRADE_END;
		}
	#if UPGRADE_BACKUP		
		write(fd1,(buffer+2048),2048);
	#endif	
		for(j=0;j<ret;j++)
		{
			if(buffer[j] != buffer[2048+j])
			{
				ret = -3;
				goto UPGRADE_END;
			}
		}
		addr+=2048;
		i+=ret;

		if(((i*100)/size)!=progress)
		{
			progress = (i*100)/size;
			deg_Printf("\rupgrade : %d%",progress);
		#if UPGRADE_VERIFY	
			upgrade_uiProgress(50+(progress>>1));
		#endif
		}
		if(i>=size)
			break;
		
	}
#if UPGRADE_BACKUP		
	close(fd1);
#endif
	deg_Printf("\n");
#endif
//------end
    ret = 0;
UPGRADE_END:
	close(fd);
	if(buffer)
	    hal_sysMemFree(buffer);
	
#if UPGRADE_DELETE
		f_unlink(UPGRADE_PATH_NAME);
		deg_Printf("upgrade : delete file ->%s!\n",UPGRADE_PATH_NAME);
#endif

	if(ret<0)
	{
		deg_Printf("upgrade : fail!\n");
		winSetStrInfor(strHandle,R_FONT_DEFAULT,ALIGNMENT_CENTER,R_COLOR_RED);
		winSetResid(strHandle,(u32)"upgrade failed!!!");
		drawUIService(true);
		XOSTimeDly(1000);
	}
    else
    {
		deg_Printf("upgrade : success!\n");
		uninit();	// success power off
    }

	taskStart(TASK_POWER_OFF,0);

	deg_Printf("upgrade : power off!\n");
	boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0);

	return ret;
}
#if 0
static void taskUpgradeOpen(uint32 arg)
{
	upgrade();
}

sysTask taskUpgrade=
{
	"upgrade",
	(u32)NULL,
	taskUpgradeOpen,
	NULL,
	NULL,
};
#endif






