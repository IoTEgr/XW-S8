#include "../../../ax32_platform_demo/application.h"
#include "../inc/btcom.h"
#include "../inc/btcom_inner.h"

//Bt_BufManage_T manager_data;
static Bt_BufManage_T manager_data;

/**
 * 创建保存帧数据的缓存
 *
 */
FRAME_MANAGE_T *btcomBufCreate(void)
{
	FRAME_MANAGE_T *manager;
	int i;

	// 分配空间
	manager = hal_sysMemMalloc(sizeof(FRAME_MANAGE_T), 64);
	if (manager == NULL) {
		deg_Printf("[%s]malloc fail\n",__func__);
		return NULL;
	}
	// 设置链表
	for (i = 0; i < NUM_OF_FRAME-1; i++) {
		manager->buf[i].next = &(manager->buf[i+1]);
	}
	manager->buf[i].next = NULL;
	manager->free = &(manager->buf[0]);
	manager->busy = NULL;
	manager->working = NULL;
	manager->remain_frameCount = NUM_OF_FRAME;
	return manager;
}

/**
 * 销毁数据缓存
 *
 */
void btcomBufDestroy(FRAME_MANAGE_T *manager)
{
	if (manager) {
		hal_sysMemFree(manager);
		manager->remain_frameCount = 0;
	}
}

/**
 * 选一个空闲的buffer接收数据
 *
 * TRUE-成功；FALSE-失败
 */
bool btcomGetIdleBuf(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *idle = manager->free;

	// 已分配有
	if (manager->working != NULL) {
		return true;
	}
	// 取一个未用buf
	if (idle != NULL) {
		idle->len = 0;
		manager->working = idle;
		manager->free = idle->next;
	}
	//deg_Printf("wk:%d\n",manager->working->len);
	return (manager->working == NULL) ? false : true;
}

/**
 * 查找是否有数据
 *
 */
FRAME_BUF_T *btcomTestBusyBuf(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *head, *item, *busy;

	head = item = manager->busy;
	busy = NULL;
	while (item) {
		if (item->total_len > 0) {
			busy = item;
			break;
		}
		if (item->next == head) {
			break;
		}
		item = item->next;
	}

	return busy;
}

/**
 * 标记数据已处理，可回收
 *
 */
void btcomMarkConsumed(FRAME_BUF_T *item)
{
	if (item) {
		item->total_len = 0;
		//deg_Printf("free-:%x\n",item);
		//回收，count++
	}
}

/**
 * 回收buffer
 *
 */
void btcomRecycleBuf(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *head, *item,*next,*prev=NULL;

	head = item = manager->busy;
	if (head) {
		prev = head->prev;
	}
	while (item) {
		// 遇到有效的，跳出循环
		if (item->total_len > 0) {
			break;
		}
		// 记录下一个item
		if (item->next == head) {
			next = NULL;
		} else {
			next = item->next;
		}
		// 放入free 表
		item->next = manager->free;
		manager->free = item;

		// 继续下一个
		item = next;
	}
	// 有删除，则重设header
	if (manager->busy != item) {
		manager->busy = item;
		if (manager->busy != NULL) {
			manager->busy->prev = prev;
			prev->next = manager->busy;
		}
	}
}

/**
 * 数据放入待处理链表
 *
 */
void btcomPutBusy(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *item = manager->working;

	if (manager == NULL || item == NULL) {
		return;
	}

	if (manager->busy == NULL) {
		item->next = item;
		item->prev = item;
		manager->busy = item;
	} else {
		item->next = manager->busy;
		item->prev = manager->busy->prev;
		manager->busy->prev->next = item;
		manager->busy->prev = item;
	}
	manager->working = NULL;
}

/**
 * 打印buffer使用情况
 *
 */
void btcomCheckManager(FRAME_MANAGE_T *manager)
{
	FRAME_BUF_T *item, *head;
	int nFree,nBusy,nWorking;

	btcomRecycleBuf(manager);
	uart_Printf("free=0x%X,busy=0x%X,working=0x%X\n",manager->free,manager->busy,manager->working);

	nFree = 0;
	item = manager->free;
	while (item) {
		nFree++;
		item = item->next;
	}

	nBusy = 0;
	head = item = manager->busy;
	while (item) {
		nBusy++;
		if (item->next == head) {
			break;
		}
		item = item->next;
	}

	if (manager->working) {
		nWorking = 1;
	} else {
		nWorking = 0;
	}

	uart_Printf("nFree=%d,nBusy=%d,nWorking=%d\n",nFree,nBusy,nWorking);
}

Bt_BufManage_T btdataBufCreate(void)
{

	// 分配空间
	manager_data.al_addr = hal_sysMemMalloc(sizeof(struct Bt_Rec_Buffer), 64);
	
	if (manager_data.al_addr == NULL) {
		deg_Printf("[%s]malloc fail\n",__func__);
		return manager_data;
	}else{
			//deg_Printf("%s:BT_manage:0x%x\n",__func__,manager_data->buffer);
	}
	//XOSTimeDly(100);
	// 设置链表
	/*
	for (i = 0; i < NUM_OF_FRAME-1; i++) {
		manager->buffer[i].next = &(manager->buffer[i+1]);
	}
	*/
	manager_data.buffer = (manager_data.al_addr + 0x3f )& (~0x3f);
	
	deg_Printf("buf[0x%x] BTbuf[0x%x] BT_buf+1:[0x%x] BT_buf[0]+1:[0x%x]\n",manager_data.buffer,manager_data.buffer->BT_buf,manager_data.buffer->BT_buf+1,manager_data.buffer->BT_buf[0]+1);
	deg_Printf("BT_buf[0]:[0x%x] BT_buf[1]:[0x%x]\n",manager_data.buffer->BT_buf[0],manager_data.buffer->BT_buf[1]);
	deg_Printf(" ----BT_buf[0]+100*1024:[0x%x] BT_buf[0]+102401:[0x%x] BT_buf[1]+1:[0x%x]\n",(manager_data.buffer->BT_buf[0]+100*1024),manager_data.buffer->BT_buf[0]+102401,manager_data.buffer->BT_buf[1]+1);

	hal_streamInit(&manager_data.vid_s,manager_data.Node,BT_BUFFER_NUM,(u32)manager_data.buffer,BT_BUFFER_NUM*BT_BUFFER_EACH_LEN);
	manager_data.Btbuf_QueueRemain = BT_BUFFER_NUM;	//init BTbuf num
	
	// manager_data.curBuffer = hal_streamMalloc(&manager_data.vid_s,BT_BUFFER_EACH_LEN);
	// if(manager_data.curBuffer==NULL){
	// 	deg_Printf("bt_manageBuf err/n");
	// 	return ;
	// }else{
	// 	deg_Printf("bt_manageBuf malloc succ manager->buffer[0x%x] manager->curBuffer[0x%x]/n",manager_data.buffer,manager_data.curBuffer);
	// 	manager_data.Btbuf_QueueRemain--;
	// }
	//hal_sysMemPrint();
	return manager_data;
}


Bt_BufManage_T* Get_btdataBufManage(void)
{
	return &manager_data;
}


