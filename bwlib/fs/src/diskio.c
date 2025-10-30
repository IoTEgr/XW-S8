/*-----------------------------------------------------------------------*/
/* Low level disk I/O module SKELETON for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "../inc/diskio.h"
#include "../inc/fs.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */


void(*fp_tmnow)(DATE_TIME_T *pdate);
s32 (*fp_storage_init)(u8 bus);
s32 (*fp_storage_sta)(void);
s32 (*fp_storage_rd)(void *pDataBuf, u32 dwLBA, u32 dwLBANum);
s32 (*fp_storage_wr)(void *pDataBuf, u32 dwLBA, u32 dwLBANum);
s32 (*fp_f_write)(const BYTE *pDataBuf, DWORD dwLBA, DWORD dwLBANum);

//=======fs fat time=====
U32 get_fattime ()
{
	DATE_TIME_T today;
	(*fp_tmnow)(&today);
	/* Returns current time packed into a DWORD variable */
	return	  ((U32)(today.year - 1980) << 25)	/* Year 2013 */  //can't  < 1980 or show time err!!!
			| ((U32)today.month << 21)				/* Month 7 */		//can't  = 0   or show time err!!!
			| ((U32)today.day << 16)				/* Mday 28 */	// can't = 0   or show time err!!!
			| ((U32)today.hour << 11)				/* Hour 0 */
			| ((U32)today.min << 5)				/* Min 0 */
			| ((U32)today.sec >> 1);				/* Sec 0 */
}

int fs_init(void)
{
	fp_tmnow = hal_rtcTimeGetExt;//now;
	fp_storage_init = hal_sdInit;
	fp_storage_sta = hal_sdExist;
	fp_storage_rd = hal_sdRead;
	fp_storage_wr = hal_sdWrite;

	return 0;
}


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	//DSTATUS stat;
	//int result;
/*
	switch (pdrv) {
	case DEV_RAM :
		result = RAM_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		result = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
*/
	return (*fp_storage_init)(hal_sdGetBusWidth());	//sd_ident
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	//DSTATUS stat;
	//int result;
/*
	switch (pdrv) {
	case DEV_RAM :
		result = RAM_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_MMC :
		result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case DEV_USB :
		result = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;

*/
return !(*fp_storage_sta)();
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
/*	DRESULT res;
	//int result;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		result = RAM_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;

*/
	if((*fp_storage_rd)(buff,sector,count)<0)
		return 1;
	else
		return 0;
  /*return (*fp_storage_rd)((void *)buff,sector,count);*/
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	//DRESULT res;
	//int result;
/*
	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		result = RAM_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_MMC :
		// translate the arguments here

		result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case DEV_USB :
		// translate the arguments here

		result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	}

	return RES_PARERR;

*/
	if((*fp_storage_wr)((void *)buff,sector,count)<0)
		return 1;
	else
		return 0;
/*	return (*fp_storage_wr)((void *)buff,sector,count);*/

}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res=RES_OK;
	//int result;
/*
	switch (pdrv) {
	case ATA :
		// pre-process here

		result = ATA_disk_ioctl(cmd, buff);

		// post-process here

		return res;

	case MMC :
		// pre-process here

		result = MMC_disk_ioctl(cmd, buff);

		// post-process here

		return res;

	case USB :
		// pre-process here

		result = USB_disk_ioctl(cmd, buff);

		// post-process here

		return res;
	}
	return RES_PARERR;
*/
	/*if (sys_ctl.SD_err_off_flag)		//sd init err
		return RES_NOTRDY;*/

	switch (cmd) {
	case CTRL_SYNC:
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT:
		*(DWORD*)buff =  hal_sdCapacity();;//g_stcSDInfo.dwCap;		//init  card cap 
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE:
		*(WORD*)buff = 512;		//default set it 
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE:
		*(DWORD*)buff = 128;		//default set it
		res = RES_OK;
		break;

	case CTRL_TRIM:
		res = RES_ERROR;			//not finish
		break;


	}

	//deg_Printf("disk_ioctl cmd : %x\n",cmd);
	return res;
}

