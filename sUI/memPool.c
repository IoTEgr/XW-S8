#include "memPool.h"

uint32 mempCreate(memPool* memp,void* ptr,uint32 size,uint32 blkSize)
{
	uint8* startAddr;
	uint8* endAddr;
	uint8* nextAddr;
	uint32 blkCnt=0;
	if(blkSize<sizeof(uint8*))
		blkSize=sizeof(uint8*);
	if(blkSize>size)
		return 0;
	endAddr=(uint8*)ptr+size;
	startAddr=(uint8*)ptr;
	memp->mempPtr=(void*)startAddr;
	nextAddr=startAddr+blkSize;
	while(nextAddr<=endAddr)
	{
		*(uint8**)startAddr=nextAddr;
		startAddr=nextAddr;
		nextAddr=startAddr+blkSize;
		blkCnt++;
	}
	memp->blkSize=blkSize;
	memp->freeBlks=blkCnt;
	memp->maxBlks=blkCnt;
	memp->minFreeBlks=blkCnt;
	
	return memp->maxBlks;
}

void* mempGet(memPool* memp)
{
	void* ptr;		
	if(memp->freeBlks==0)
	{
		return (void*)0;
	}
	ptr=memp->mempPtr;
	memp->freeBlks--;
	if(memp->freeBlks)
		memp->mempPtr=*(void**)ptr;
	else
		memp->mempPtr=(void*)0;
	if(memp->minFreeBlks>memp->freeBlks)
		memp->minFreeBlks=memp->freeBlks;
	return ptr;
}

uint32 mempPut(memPool* memp,void* ptr)
{	
	if(memp->freeBlks>=memp->maxBlks)
	{	
		return 0;
	}
	*(void**)ptr=memp->mempPtr;
	memp->mempPtr=ptr;
	memp->freeBlks++;
	return memp->freeBlks;
}

void mempInfor(memPool* memp)
{
	uart_Printf("memPool blkSize:%d\n",memp->blkSize);
	uart_Printf("memPool freeBlks:%d\n",memp->freeBlks);
	uart_Printf("memPool maxBlks:%d\n",memp->maxBlks);
	uart_Printf("memPool minFreeBlks:%d\n",memp->minFreeBlks);
}

