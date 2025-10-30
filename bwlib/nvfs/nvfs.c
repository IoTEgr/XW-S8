/****************************************************************************
**
**                              NV FS
** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
**** **                    NV FS FOR SPI FLASH
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : nvfs.c
* Author      : Mark.Douglas
* Version     : V100
* Date        : 09/22/2016
* Description : This file is user MEDIA header file
*
* History     :
* 2016-09-22  :
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/

#include "../bwlib.h"
// #include "application.h"

typedef struct Res_Info_S
{
	INT32U address; // addr of spiflsh
	INT32U length;
} Res_Info_T;

typedef struct Res_Ctrl_S
{
	INT32U resAddress;
	INT32U resSize;

	INT32U firstResAddr;
	INT32U lastResNum;
	Res_Info_T lastRes;
} Res_Ctrl_T;

static Res_Ctrl_T nvInfo;

#if NVFS_STROAGE == NV_STROAGE_SDC
FIL nvFILE;
#define NVNAME "RES.BIN"
#elif (NVFS_STROAGE == NV_STROAGE_SDRAM)
u8 *res_buf = NULL;
#endif

void nv_port_read(INT32U addr, INT32 buffer, INT32U len)
{
#if NVFS_STROAGE == NV_STROAGE_SDC
	// UINT temp;

	//    _f_lseek(&nvFILE,addr);
	// _f_read(&nvFILE,(void *)buffer,len,&temp);
#elif NVFS_STROAGE == NV_STROAGE_SPI
	hal_spiFlashRead(addr, buffer, len);
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	if (NULL != res_buf)
	{
		memcpy(buffer, res_buf + addr, len);
		ax32xx_sysDcacheWback(buffer, len);
	}
#endif
}

/*******************************************************************************
* Function Name  : nv_Init
* Description    :  nv fs initial
* Input          : none
* Output         : none
* Return         : int

*******************************************************************************/
int nv_init(void)
{
	/*if(nvInfo.resAddress != 0)
	{
		 return nvInfo.resAddress;
	}*/
#if NVFS_STROAGE == NV_STROAGE_SDC
	f_open(&nvFILE, (const char *)NVNAME, FA_READ);
	//    f_fastseek_init(&nvFILE);
	//    nvFILE.fs = NULL;
	nvInfo.resAddress = 0;
	nvInfo.lastResNum = -1;
#elif NVFS_STROAGE == NV_STROAGE_SPI
	INT32U mem = 0;
	nv_port_read(4, (INT32U)&mem, 4);
	if (mem != 0x52444c42)
	{
		nvInfo.resAddress = 0;
		deg_Printf("nvfs init : fail.no boot sector.\n");
		return -1;
	}
	mem = 0;
	nv_port_read(9, (INT32U)&mem, 1);
	// debgbuf((u8 *)__TEMP_BUFF_BEG, 512);
	mem = mem << 4;
	deg_Printf("nvfs init : mem = %d\n", mem);

	nv_port_read(mem + 0x08, (INT32U)&nvInfo.resAddress, 4);
	nvInfo.resAddress = nvInfo.resAddress << 9;

	nv_port_read(mem + 0x0c, (INT32U)&nvInfo.resSize, 4);
	nvInfo.resSize = nvInfo.resSize << 9;

	nv_port_read(nvInfo.resAddress, (INT32U)&nvInfo.lastRes, sizeof(Res_Info_T)); // load res 0

	nvInfo.firstResAddr = nvInfo.lastRes.address; // res table end address
	nvInfo.lastResNum = 0;
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	INT32U mem = 0;
	hal_spiFlashRead(4, (INT32U)&mem, 4);
	if (mem != 0x52444c42)
	{
		nvInfo.resAddress = 0;
		deg_Printf("nvfs init : fail.no boot sector.\n");
		return -1;
	}
	mem = 0;
	hal_spiFlashRead(9, (INT32U)&mem, 1);
	mem = mem << 4;
	deg_Printf("nvfs init : mem = %d\n", mem);

	hal_spiFlashRead(mem + 0x08, (INT32U)&nvInfo.resAddress, 4);
	nvInfo.resAddress = nvInfo.resAddress << 9;

	hal_spiFlashRead(mem + 0x0c, (INT32U)&nvInfo.resSize, 4);
	nvInfo.resSize = nvInfo.resSize << 9;

	if (NULL == res_buf)
	{
		res_buf = (INT8U *)hal_sysMemMalloc(nvInfo.resSize, 64);
		if (NULL == res_buf)
		{
			deg_Printf("nvfs init : res mem err,size=0x%x\n", nvInfo.resSize);
		}
	}
	hal_spiFlashRead(nvInfo.resAddress, res_buf, nvInfo.resSize);
	memcpy((INT32U)&nvInfo.lastRes, res_buf, sizeof(Res_Info_T));
	ax32xx_sysDcacheWback(&nvInfo.lastRes, sizeof(Res_Info_T));
	// hal_spiFlashRead(nvInfo.resAddress,(INT32U)&nvInfo.lastRes,sizeof(Res_Info_T)); // load res 0

	nvInfo.firstResAddr = nvInfo.lastRes.address; // res table end address
	nvInfo.lastResNum = 0;

#endif
	deg_Printf("nvfs init : 0x%x,0x%x\n", nvInfo.resAddress, nvInfo.resSize);

	return nvInfo.resAddress;
}
/*******************************************************************************
* Function Name  : nv_uninit
* Description    :  nv fs uninitial
* Input          : none
* Output         : none
* Return         : int

*******************************************************************************/
int nv_uninit(void)
{
#if NVFS_STROAGE == NV_STROAGE_SDC
	nvInfo.resAddress = 0;
	_f_close(&nvFILE);
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	if (res_buf)
	{
		hal_sysMemFree(res_buf);
		res_buf = NULL;
	}
#endif
	return 0;
}
/*******************************************************************************
* Function Name  : nv_configAddr
* Description    :  get nv fs configure address
* Input          : none
* Output         : int res_num
* Return         : int

*******************************************************************************/
int nv_configAddr(void)
{
	INT32U addr;
#if NVFS_STROAGE == NV_STROAGE_SPI
	if (nvInfo.resAddress == 0)
		return 0x00ffffff;
#endif
	addr = nvInfo.resAddress + nvInfo.resSize;

	if (addr & 0xfff)
		addr = (addr & 0xfffff000) + 0x1000; // 4096 algin
	deg_Printf("nvfs config : addr = 0x%x\n", addr);
	return (addr);
}
/*******************************************************************************
* Function Name  : nv_open
* Description    :  nv fs open
* Input          : none
* Output         : int res_num
* Return         : int

*******************************************************************************/
int nv_open(int res_num)
{
	INT32U resoff;
#if NVFS_STROAGE == NV_STROAGE_SPI
	if (nvInfo.resAddress == 0)
		return -1;
#endif

#if NVFS_STROAGE == NV_STROAGE_SPI
	if (nvInfo.lastResNum == res_num)
		return (nvInfo.lastRes.address + nvInfo.resAddress);

	resoff = sizeof(Res_Info_T) * res_num;

	if (resoff >= nvInfo.firstResAddr)
		return -1;
	nv_port_read(nvInfo.resAddress + resoff, (INT32U)&nvInfo.lastRes, sizeof(Res_Info_T));

	nvInfo.lastResNum = res_num;

	return (nvInfo.lastRes.address + nvInfo.resAddress);
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	if (nvInfo.lastResNum == res_num)
		return (nvInfo.lastRes.address);

	resoff = sizeof(Res_Info_T) * res_num;

	if (resoff >= nvInfo.firstResAddr)
		return -1;
	nv_port_read(resoff, (INT32U)&nvInfo.lastRes, sizeof(Res_Info_T));
	nvInfo.lastResNum = res_num;
	return (nvInfo.lastRes.address);

#endif
}
/*******************************************************************************
* Function Name  : nv_size
* Description    :  nv fs get file size
* Input          : none
* Output         : int res_num
* Return         : int

*******************************************************************************/
int nv_size(int res_num)
{
	INT32U resoff;
#if NVFS_STROAGE == NV_STROAGE_SPI
	if (nvInfo.resAddress == 0)
		return -1;
#endif

	if (nvInfo.lastResNum == res_num)
		return (nvInfo.lastRes.length);

	resoff = sizeof(Res_Info_T) * res_num;
#if NVFS_STROAGE == NV_STROAGE_SPI
	if (resoff >= nvInfo.firstResAddr)
		return -1;
	nv_port_read(nvInfo.resAddress + resoff, (INT32U)&nvInfo.lastRes, sizeof(Res_Info_T));

	nvInfo.lastResNum = res_num;

	return (nvInfo.lastRes.length);
#elif NVFS_STROAGE == NV_STROAGE_SDRAM
	if (resoff >= nvInfo.firstResAddr)
		return -1;
	nv_port_read(resoff, (INT32U)&nvInfo.lastRes, sizeof(Res_Info_T));
	nvInfo.lastResNum = res_num;
	return (nvInfo.lastRes.length);

#endif
}
/*******************************************************************************
* Function Name  : nv_read
* Description    :  nv fs read data
* Input          : none
* Output         : int res_num
* Return         : int

*******************************************************************************/
int nv_read(int addr, void *buffer, int size)
{
	nv_port_read(addr, (INT32U)buffer, size);
	return size;
}
