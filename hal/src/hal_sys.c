/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                   THE APPOTECH HAL
   **** **                 SYSTEM HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_sys.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN SYSTEM HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
* 2017-06-01  :
*      <1>.Modify memory register to dynamic malloc and free.only support 30memory blocks
******************************************************************************/
#include "../inc/hal.h"


// system internal ram using
__RAM_USE__ ramuse  __attribute__ ((section(".sram_comm"))); 
__FIFO_USB__ usbfifo __attribute__ ((section(".sram_Usbfifo")));  


// system memory for pcm buffer .SDRAM.audio play will using this buffer as decoded pcm raw data cache
//SECTION(".big_buffer") ALIGNED(64) _PCMBUF_ halSysMemPCMBuffer;
//SECTION(".big_buffer") ALIGNED(64) u8 halSysMemPCMTempBuffer[HAL_CFG_PCM_BUFFER_SIZE];



//system memory for mjpeg buffer .SDRAM.video play will using this buff er as avi file idx cache
//SECTION(".big_buffer") ALIGNED(64) _MJPEG_T halSysMemMJPEGBuffer; 


//system memory for lcd show buffer .SDRAM
#if 0//HAL_CFG_EN_LCD
     SECTION(".big_buffer") ALIGNED(64) unsigned char halSysMemLcdYBuffer[HAL_CFG_LCD_BUFFER_NUM*HAL_CFG_LCD_STRIDE * (HAL_CFG_LCD_HEIGHT+HAL_CFG_LCD_EXTRA_LINE)];
     SECTION(".big_buffer") ALIGNED(64) unsigned char halSysMemLcdUVBuffer[HAL_CFG_LCD_BUFFER_NUM * HAL_CFG_LCD_STRIDE*((HAL_CFG_LCD_HEIGHT+HAL_CFG_LCD_EXTRA_LINE+1)>>1)];
#endif

//system memory for osd0 show buffer .SDRAM
#if 0//HAL_CFG_EN_OSD && HAL_CFG_OSD0_ENABLE
     SECTION(".big_buffer") ALIGNED(32)  unsigned char halSysMemOSD0Buffer[HAL_CFG_OSD0_WIDTH * HAL_CFG_OSD0_HEIGHT*HAL_CFG_OSD0_NUM];
#endif


//system memory for osd1 show buffer .SDRAM
#if 0//HAL_CFG_EN_OSD && HAL_CFG_OSD1_ENABLE
     SECTION(".big_buffer") ALIGNED(32)  unsigned char halSysMemOSD1Buffer[HAL_CFG_OSD1_WIDTH * HAL_CFG_OSD1_HEIGHT*HAL_CFG_OSD1_NUM];
#endif


//system memory for osd UI DRAW buffer .SDRAM
#if 0//HAL_CFG_EN_OSDTEMP
     SECTION(".big_buffer") ALIGNED(32)  unsigned char halSysMemOSDTempBuffer[HAL_CFG_OSD0_WIDTH * HAL_CFG_OSD0_HEIGHT];
#endif


//system memory for mjpeg decode cache & ui read line cache .SDRAM
#if HAL_CFG_EN_LINE 
     SECTION(".big_buffer") ALIGNED(32)  unsigned char halSysMemLineCache[HAL_CFG_LINE_CACHE+1024];
#endif

//system meory for avi idx cache.SDRAM
//SECTION(".big_buffer") ALIGNED(32)  unsigned char halSysMemIdxBuffer[HAL_CFG_IDX_CACHE];

#define  MEM_NODE_MAX      64
typedef struct MEM_NODE_S
{
	u32 flag;
	u32 addr;
	u32 size;
	struct MEM_NODE_S *next;
#if HAL_ALLOC_DEBUG
	const char *func;
	int line;
	u32 appsize;
#endif
}MEM_NODE_T;
static MEM_NODE_T memNode[MEM_NODE_MAX],*idleMem,*busyMem;
static u32 halMemStart,halMemEnd,halMemIdx;
static MEM_NODE_T *hal_memNodeMalloc(void)
{
	MEM_NODE_T *node;
	node = idleMem;
	if(node)
	{
		idleMem = node->next;
		node->flag = 0;
		node->addr = 0;
		node->size = 0;
	}

	return node;
}
static void hal_memNodeFree(MEM_NODE_T *node)
{
	if(node)
	{
		node->next = idleMem;
		idleMem = node;
		node->flag = 0;
	}
}
/*******************************************************************************
* Function Name  : hal_sysInit
* Bref               : void hal_sysInit(int ms)
* Description    : hal layer.initial system for platform using.
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
int hal_sysInit(void)
{
	MEM_NODE_T *node;
	int i;
	
	ax32xx_sysInit((u32 *)&halMemStart,(u32 *)&halMemEnd);
    
	halMemStart = (halMemStart+0x3f)&(~0x3f);
	halMemIdx = halMemStart;

    idleMem = &memNode[0];
	for(i=0;i<MEM_NODE_MAX;i++)
	{
		if(i==(MEM_NODE_MAX-1))
		    memNode[i].next = NULL;
		else
			memNode[i].next = &memNode[i+1];
		memNode[i].flag = 0;
		memNode[i].addr = 0;
		memNode[i].size = 0;
	}
	busyMem = NULL;
	node = hal_memNodeMalloc(); // memory end node
	node->size = 0;
	node->next = NULL;
	node->flag = 1;
	busyMem = node;
	node = hal_memNodeMalloc(); // memory start node
	node->addr = halMemStart;
	node->size = halMemEnd-halMemStart;
	node->flag = 0;
	node->next = busyMem;
	busyMem = node;
	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_sysMemPrint
* Description    : printf system memory info
* Input          : 
* Output         : None
* Return         :
*******************************************************************************/
void hal_sysMemPrint(void)
{
	MEM_NODE_T *node;

#if HAL_ALLOC_DEBUG
	uart_Printf("<HEAP>total = %d\n",halMemEnd-halMemStart);
#endif
	node = busyMem;	
	while(node)
	{
#if HAL_ALLOC_DEBUG
		if (node->flag) {
					u8 *p = (u8 *)(node->addr + node->appsize);
			if (node->line > 0 &&(p[0]!=0x55 || p[1]!=0x55 || p[2]!=0x55 || p[3]!=0x55)) {
				uart_Printf("-------OVERFLOW-------\n");
				uart_Printf("[%s]line=%d,size=%d,[%X %X %X %X]\n",node->func,node->line,node->appsize,p[0],p[1],p[2],p[3]);
				uart_Printf("----------------------\n");
			} else {
				uart_Printf("<HEAP> [%d] %x,%d,%s,%d\n",node->flag,node->addr,node->size,node->func,node->line);
			}
		} else {
		uart_Printf("<HEAP> [%d] %x,%d\n",node->flag,node->addr,node->size);
		}
#else
		deg_Printf("HAL : [SYS]<INFO> [%d] %x,%d,\n",node->flag,node->addr,node->size);
#endif
		
		node = node->next;
	}	
}
/*******************************************************************************
* Function Name  : hal_sysMemMalloc
* Bref               : hal_sysMemMalloc(u32 size,u8 algin)
* Description    : hal layer.system memory malloc.this function only for temp using
* Input          : u32 size : malloc size
                      u8 algin : algin bit
* Output         : None
* Return         : None
*******************************************************************************/
#if HAL_ALLOC_DEBUG
void *hal_sysMemMallocExt(u32 size,u8 algin, const char *func, int line)
#else
void *hal_sysMemMalloc(u32 size,u8 algin)
#endif
{
	u32 addr;
	#if HAL_ALLOC_DEBUG
	u32 appsize = size;
#endif
/*	addr = halMemIdx;
	if(algin)
	{
		addr = (addr+algin-1)&(~(algin-1));
	}
	if((halMemEnd-addr)<size)
		return NULL;
	halMemIdx = addr+size;
*/
	MEM_NODE_T *cur,*node;

	node = hal_memNodeMalloc();
	if(node == NULL) // no more node
		return NULL;
	size = ((size+4+0x3f)&(~0x3f));
	cur = busyMem;	
	while(cur)
	{
		if((cur->flag==0) && (cur->size>=size))
			break;
		cur = cur->next;
	}
	if(cur == NULL) // no more memory
	{
		hal_memNodeFree(node);
		//hal_sysMemRemain();
		#if HAL_ALLOC_DEBUG
		uart_Printf("<ALLOC>fail:%s,%d,%d\n",func,line,size);
		#endif
		hal_sysMemPrint();
		return NULL;
	}
	addr = cur->addr;
	node->size = cur->size-size; // algin -64
	node->addr = cur->addr+size;
	node->flag = 0;
	node->next = cur->next;
#if HAL_ALLOC_DEBUG
	cur->func = func;
	cur->line = line;
	cur->appsize = appsize;
	memset((u8 *)(addr+appsize), 0x55, 4);
#endif
	cur->next = node;
	cur->flag = 1;
	cur->size = size;
	//deg_Printf("mem:0x%x,0x%x\n",addr,size);

	return ((void *)addr);
}
/*******************************************************************************
* Function Name  : hal_sysMemFree
* Bref               : void hal_sysMemFree(void *mem)
* Description    : hal layer.system memory free
* Input          : void *mem : memory address
* Output         : None
* Return         : None
*******************************************************************************/
void hal_sysMemFree(void *mem)
{
	if(mem == NULL) // address  is null
		return ;
	MEM_NODE_T *prev,*cur,*node;
	u32 addr;
	addr = (u32)mem;

	cur = busyMem;
	prev = NULL;
	while(cur)
	{
		if((cur->flag) && (cur->addr==addr))
			break;
		prev = cur;
		cur = cur->next;
	}
	if(cur==NULL)  // can not find this address
		return ;
#if HAL_ALLOC_DEBUG
		u8 *p = (u8 *)(addr + cur->appsize);
		if (p[0]!=0x55 || p[1]!=0x55 || p[2]!=0x55 || p[3]!=0x55) {
			deg_Printf("-------OVERFLOW-------\n");
			deg_Printf("[%s]line=%d,size=%d,[%X %X %X %X]\n",cur->func,cur->line,cur->appsize,p[0],p[1],p[2],p[3]);
			deg_Printf("----------------------\n");
		}
#endif

	node = cur->next;
	cur->flag = 0;
	if(node&&(node->flag==0))
	{
		cur->size +=node->size;
		cur->next = node->next;
		node->next = NULL;
		hal_memNodeFree(node);
	}
	if(prev && (prev->flag==0))
	{
		prev->size+=cur->size;
		prev->next = cur->next;
		cur->next = NULL;
		hal_memNodeFree(cur);
	}
}
/*******************************************************************************
* Function Name  : hal_sysMemRemain
* Bref               : u32 hal_sysMemRemain(void)
* Description    : hal layer.system memory remain check.this function only for temp using.memory can not be free
* Input          : 
* Output         : None
* Return         : u32 : remain size
*******************************************************************************/
u32 hal_sysMemRemain(void)
{
	MEM_NODE_T *node,*mnode;
	u32 freesize=0,busysize=0;
	node = busyMem;	
	mnode = NULL;
	while(node)
	{
		//debg("HAL : [SYS]<INFO> [%d] %x,%x,\n",node->flag,node->addr,node->size);
		if(node->flag==0)
		{
			if(mnode)
			{
				if(mnode->size<node->size)
					mnode = node;
			}
			else
				mnode = node;
			freesize+=node->size;
		}
		else
		{
			busysize+=node->size;
		}
		node = node->next;
	}
    //debg("HAL : [SYS]<INFO>saddr =%x,eaddr =%x,busysize= %x,freesize = %x,\n",halMemStart,halMemEnd,busysize,freesize);
	
	if(mnode)
		return mnode->size;
	else
		return 0;
}



