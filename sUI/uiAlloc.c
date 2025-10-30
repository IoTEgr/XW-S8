#include "uiAlloc.h"
#include "stdlib.h"

typedef struct _heapHead
{
	struct _heapHead* next;
	uint32 size;	
}heapHead;
#define HEAP_HEAD_SIZE  sizeof(heapHead)
typedef struct
{
	heapHead* head;
	uint32 heapFreeBytes;
}uiHeap;

static uiHeap heap;

#if UIALLOC_DEBUG
#define MAX_NODE 100
typedef struct {
	winHandle handle;
	const char *func;
	int line;
	int size;
} MEM_DEBUG_T;
MEM_DEBUG_T g_malloc_info[MAX_NODE] = {{0}};
void addNode(winHandle handle, const char *func, int line, int size)
{
	int i;

	for (i=0; i < MAX_NODE; i++) {
		if (g_malloc_info[i].handle == NULL) {
			g_malloc_info[i].handle = handle;
			g_malloc_info[i].func = func;
			g_malloc_info[i].line = line;
			g_malloc_info[i].size = size;
			break;
		}
	}
	if (i >= MAX_NODE) {
		uart_Printf("MAX_NODE is overflow\n");
	}
}
void delNode(winHandle handle)
{
	int i;

	for (i=0; i < MAX_NODE; i++) {
		if (g_malloc_info[i].handle == handle) {
			g_malloc_info[i].handle = NULL;
			break;
		}
	}
}

void PrintNode(void)
{
	int i;

	for (i=0; i < MAX_NODE; i++) {
		if (g_malloc_info[i].handle != NULL) {
			uart_Printf("\tfunc=%s,line=%d,size=%d\n",g_malloc_info[i].func,g_malloc_info[i].line,g_malloc_info[i].size);
		}
	}
}
#endif

void uiHeapInit(uint8* addr,uint32 size)
{
	heapHead* head=(heapHead*)addr;
	heap.head=head;
	heap.heapFreeBytes=size;
	head->next=NULL;
	head->size=size;
}

#if UIALLOC_DEBUG
winHandle uiAllocExt(uint32 size,const char *func, int line)
#else
winHandle uiAlloc(uint32 size)
#endif
{
	heapHead* head;
	heapHead* prev;
	uint8* addr=NULL;
	if(size==0)
		return INVALID_HANDLE;
	size=size+sizeof(uint32);
	if(size&0x3)
		size=(size&(~0x3))+0x4;
	if(size<HEAP_HEAD_SIZE)
		size=HEAP_HEAD_SIZE;
	head=heap.head;
	prev=NULL;
	//prev=head=heap.head;  //memory leak
	while(head!=NULL)
	{
		if(head->size>=size)
		{
			addr=(uint8*)head;
			if(head->size-size>=HEAP_HEAD_SIZE)
			{
				head=(heapHead*)(addr+size);
				head->next=((heapHead*)addr)->next;
				head->size=((heapHead*)addr)->size-size;
			}
			else
			{
				size=head->size;
				head=head->next;
			}
			*((uint32*)addr)=size;
			addr+=sizeof(uint32);
			//if(prev==heap.head)  //memory leak
			if(prev==NULL)
				heap.head=head;
			else
				prev->next=head;
			heap.heapFreeBytes-=size;
			break;
		}
		prev=head;
		head=head->next;
	}
	if(addr!=NULL) {
		memset(addr,0,size-sizeof(uint32));
		#if UIALLOC_DEBUG
		addNode(addr, func, line, size);
		#endif
	}
	else {
		debug_msg("no memory left!!!!!!\n");
		#if UIALLOC_DEBUG
		PrintNode();
		#endif
	}
	return (winHandle)addr;
}
void uiFree(winHandle handle)
{
	uint8* addr;
	uint32 size;
	heapHead* freeHeap;
	heapHead* current;
	heapHead* next;
	if(handle==INVALID_HANDLE)
		return;
	#if UIALLOC_DEBUG
	delNode(handle);
	#endif
	addr=(uint8*)handle;
	addr-=sizeof(uint32);
	size=*((uint32*)addr);
	freeHeap=(heapHead*)addr;
	freeHeap->next=NULL;
	freeHeap->size=size;
	heap.heapFreeBytes+=size;
	if(heap.head==NULL)
	{
		heap.head=freeHeap;
		return;
	}
	if(heap.head>=freeHeap)
	{
		next=heap.head;
		heap.head=freeHeap;
		if(((uint8*)freeHeap)+size==(uint8*)next)
		{
			freeHeap->next=next->next;
			freeHeap->size+=next->size;
		}
		else
		{
			freeHeap->next=next;
		}
		return;
	}
	current=heap.head;
	while(current->next!=NULL)
	{
		if(current->next>=freeHeap)
			break;
		current=current->next;
	}
	next=current->next;
	if(((uint8*)current)+current->size==(uint8*)freeHeap)
		current->size+=freeHeap->size;
	else
	{
		current->next=freeHeap;
		current=freeHeap;
	}
	if(next)
	{
		if(((uint8*)current)+current->size==(uint8*)next)
		{
			current->size+=next->size;
			current->next=next->next;
		}
		else
			current->next=next;
	}
}
void uiHeapInfor(uint32 n)
{
	uint32 i=0,size=0;
	heapHead* head=heap.head;
	debug_msg("UI heap information [%d]:\n",n);
	while(head!=NULL)
	{
		i++;
		size+=head->size;
		debug_msg("%d:0x%x - 0x%x - 0x%x",i,head,head->size,(uint32)head+head->size);
		//debug_msg("%3d:0x%-8x - 0x%-4x - 0x%-8x",i,head,head->size,(uint32)head+head->size);
		if(head->next)
		{
			debug_msg("   0x%x\n",(uint32)(head->next)-((uint32)head+head->size));
		}
		head=head->next;
	}
	debug_msg("\n");
}

void heapAllocTest(void)
{
#if 0
	#define WIN_HEAP_SIZE  (1024*32)
	uint32 i=0,j=0,k=0;
	winHandle handle[1000];
	uint32 range[10]={50,20,80,0,90,30,60,10,40,70};	
	static uint8 heap[WIN_HEAP_SIZE];
	
	uiHeapInit(heap,WIN_HEAP_SIZE);
	uiHeapInfor(0);
	for(i=0;i<1000;i++)
	{
		handle[i]=uiAlloc((rand()&31)+1);
	}
	for(k=0;k<10;k++)
	{
		for(j=range[k];j<range[k]+10;j++)
		{
			for(i=0;i<10;i++)
				uiFree(handle[j*10+i]);
			for(i=0;i<10;i++)
				handle[j*10+i]=uiAlloc((rand()&31)+1);
		}
		uiHeapInfor(k+1);
	}
	for(i=0;i<1000;i++)
		uiFree(handle[i]);
	uiHeapInfor(11);
#endif
}



















