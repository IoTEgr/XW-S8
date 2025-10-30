#ifndef __UIALLOC_H
#define __UIALLOC_H
#include "mType.h"
#include "winManage.h"

#define UIALLOC_DEBUG 0

void uiHeapInit(uint8* addr,uint32 size);
#if UIALLOC_DEBUG
winHandle uiAllocExt(uint32 size,const char *func, int line);
#define uiAlloc(size) uiAllocExt(size, __func__, __LINE__)
#else
winHandle uiAlloc(uint32 size);
#endif
void uiFree(winHandle handle);
void uiHeapInfor(uint32 n);
void heapAllocTest(void);


#define uiHandleToPtr(n)    (n)
#define uiPtrToHandle(n)    (n)





#endif
