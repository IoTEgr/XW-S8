#ifndef  __OS_MEM_POOL_H__
#define  __OS_MEM_POOL_H__
#include "winManage.h"

typedef struct _memPool
{
	void* mempPtr;
	uint32   blkSize;
	uint32   freeBlks;
	uint32   maxBlks;

	uint32   minFreeBlks;
}memPool;

uint32 mempCreate(memPool* memp,void* ptr,uint32 size,uint32 blk_size);
void* mempGet(memPool* memp);
uint32 mempPut(memPool* memp,void* ptr);
void mempInfor(memPool* memp);



#endif
