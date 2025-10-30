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
* File Name   : hal_stream.c
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
******************************************************************************/
#include "../inc/hal.h"


/*******************************************************************************
* Function Name  : hal_streamInit
* Description    : hal layer,stream management.for stream media.stream initial
* Input          : Stream_Head_T *head : stream head for operation handler
				Stream_Node_T *pool  : stream node
				u32 num                   : number of stream nodes
				u32 mem                 : start memory
				u32 mem_len            : memory length
* Output         : None
* Return         :  0: success
*******************************************************************************/
int hal_streamInit(Stream_Head_T *head,Stream_Node_T *pool,u32 num,u32 mem,u32 mem_len)
{
	int i;
	if((head==NULL)||(pool==NULL)||(mem == 0))
		return -1;

	for(i=0;i<num;i++)
	{
		pool[i].sta = 0;
		pool[i].pbuf = NULL;
		pool[i].buflen =  0;
		pool[i].tsync =  0;
		pool[i].last =(i == 0)?&pool[num-1] : &pool[i-1];
		pool[i].next = ((i + 1) == num)?&pool[0] : &pool[i+1];
	}
	head->p_qdt = head->g_qdt = &pool[0];
	
	head->c_inf.semf = 0;
	head->c_inf.pcache = (u8 *)mem;
	head->c_inf.cachelen = mem_len;
	head->c_inf.sf = (u8 *)mem;
	head->c_inf.ef = (u8 *)(mem+mem_len);
	
	head->head_size=0;
	return 0;
}
/*******************************************************************************
* Function Name  : hal_srreamMalloc
* Description    : hal layer,stream management.for stream media.stream malloc a memory for write
* Input          : Stream_Head_T *head : stream head for operation handler
				u32 mlen : need memory length
* Output         : None
* Return         :  mem
*******************************************************************************/
u32 hal_streamMalloc(Stream_Head_T *head, u32 mlen)
{
	u8 *malloc_buf ;
	
	Stream_Node_T *p = head->p_qdt->last;
	Stream_Node_T *g = head->g_qdt;
	
	volatile u32 sf = (u32)head->c_inf.sf;
	volatile u32 ef = (u32)head->c_inf.ef;
	
	while(1)
	{
		//first macllo
		if((p->pbuf == NULL) && (g->pbuf == NULL))
		{
			malloc_buf = (u8*)sf;
			break;
		}
		
		//not first macllo
		u32 pt = (u32)(p ->pbuf + p ->buflen);
		u32 gt = (u32)(head ->g_qdt->pbuf);
		
		if(pt > gt)
		{
			if(ef >= pt + mlen)
			{
				malloc_buf = (u8*)pt;
				break;
			}
			else
			{
				if(gt >= sf + mlen)
				{
					malloc_buf = (u8*)sf;
					break;
				}
				else
				{
					malloc_buf = NULL;
					break;
				}	
			}
		}
		if(pt < gt)
		{
			if(gt  >= pt + mlen)
			{
				malloc_buf =  (u8*)pt;	
				break;
			}
			else
			{
				malloc_buf = NULL;
				break;
			}	
		}
		
		//cache full
		if(pt == gt)
		{
			//debg("gt=pf\n");
			malloc_buf = NULL;
			break;
		}	
	}
	if(malloc_buf == NULL){
		/*u32 pt = (u32)(p ->pbuf + p ->buflen);
		u32 gt = (u32)(head ->g_qdt->pbuf);
		
		debg("[sf:%x,ef:%x,pt:%x,gt:%x]",sf,ef,pt,gt);
		if(pt>gt)
			debg("[%d%]",((pt-gt)*100)/(ef-sf));
		else
			debg("[%d%]",((ef-sf-pt+gt-200*1024)*100)/(ef-sf));
		*/
		return 0;
	}
	
	head->p_qdt->sta = 0;
	head->p_qdt->pbuf = malloc_buf;
	head->p_qdt->buflen = mlen;
	head->p_qdt = head->p_qdt->next;
	head->c_inf.semf++;
//	if(head->c_inf.semf > 2) //adc pre malloc
//		debg("+sem:%x\n",head-> c_inf.semf);
	return (u32)malloc_buf;
}
void hal_streamIn_Drop(Stream_Head_T *head)
{	
	if(head->p_qdt->last->sta==1){
		head->p_qdt->last->sta = 0;
		head->head_size -= head->p_qdt->last->buflen;
		head->p_qdt->last->buflen = 0;
	}
	head->c_inf.semf++;	
}
/*******************************************************************************
* Function Name  : hal_streamIn
* Description    : hal layer,stream management.for stream media.stream queue in
* Input          : Stream_Head_T *head : stream head for operation handler
				u32 p : memory address
				u32 len: memory length
* Output         : None
* Return         :  0:success
*******************************************************************************/
int hal_streamIn(Stream_Head_T *head, u32 p, u32 len,s32 t_sync,s32 t_sync_next)
{
	if(head->c_inf.semf == 0)
	{
		debg("cache_put err\n");//while(1);
		return -1;
	}
	Stream_Node_T *pt = head->p_qdt;
	
	u8 i;
	for(i = 0; i < head->c_inf.semf; i++)
		pt = pt->last;
	//sem--
	head->c_inf.semf--;
	//debg("-sem:%x\n",head-> c_inf.semf);
	if((pt ->pbuf != NULL) /*&& (pt->buflen != 0)*/ && (pt->sta == 0))
	{
		pt->sta = 1;
		pt ->buflen = len;	
		pt->tsync=t_sync;
		pt->tsync_next = t_sync_next;
		head->head_size+=len;
		return 0;
	}
	else
	{
		LogPrint("qdt err",0);
		debg("qdt: <list full>\n");exception_trigger();while(1);
		return -1;	
	}
}
/*******************************************************************************
* Function Name  : hal_streamOut
* Description    : hal layer,stream management.for stream media.stream queue out
* Input          : Stream_Head_T *head : stream head for operation handler
				u32 *p : memory address
				u32 *len: memory length
* Output         : None
* Return         :  0:success
*******************************************************************************/
int hal_streamOut(Stream_Head_T *head, u32 *p, u32 *len,s32 *sync, s32 *sync_next)
{
	Stream_Node_T *gt = head->g_qdt;
	
	if((gt->pbuf != NULL) /*&& (gt->buflen != 0)*/&& (gt->sta == 1))
	{
		if(p){
		    *p = (u32)gt->pbuf;	
		}
		if(len){
		    *len = gt->buflen;
		}
		if(sync){
		    *sync = gt->tsync;
		}
		if (sync_next){
			*sync_next = gt->tsync_next;
		}
		//gt->sta = 0;
		//gt->pbuf = NULL;
		//gt->buflen = 0;
		//head->g_qdt = gt->next;
		return 0;
	}
	else
	{
		*p = 0;//NULL;
		return -1;	
	}
}


int hal_streamOut_next(Stream_Head_T *head, u32 *p, u32 *len,s32 *sync, s32 *sync_next)
{
	Stream_Node_T *gt = head->g_qdt->next;
	
	if((gt->pbuf != NULL) /*&& (gt->buflen != 0)*/&& (gt->sta == 1))
	{
		if(p){
		    *p = (u32)gt->pbuf;	
		}
		if(len){
		    *len = gt->buflen;
		}
		if(sync){
		    *sync = gt->tsync;
		}
		if (sync_next){
			*sync_next = gt->tsync_next;
		}
		//gt->sta = 0;
		//gt->pbuf = NULL;
		//gt->buflen = 0;
		//head->g_qdt = gt->next;
		return 0;
	}
	else
	{
		*p = 0;//NULL;
		return -1;	
	}
}


/*******************************************************************************
* Function Name  : hal_streamfree
* Description    : hal layer,stream management.for stream media.stream queue free
* Input          : Stream_Head_T *head : stream head for operation handler
* Output         : None
* Return         :  0:success
*******************************************************************************/
int hal_streamfree(Stream_Head_T *head)
{
	
	HAL_CRITICAL_INIT();
	if(head && (head->g_qdt->pbuf != NULL) /*&& (head->g_qdt->buflen != 0)*/&& (head->g_qdt->sta== 1)){
		//*p = gt ->pbuf;	
		//*len = gt->buflen;
		
		HAL_CRITICAL_ENTER();
		head->head_size-=head->g_qdt->buflen;
		head->g_qdt->sta = 0;
		head->g_qdt->pbuf = NULL;
		head->g_qdt->buflen = 0;
		head->g_qdt->tsync = 0;
		head->g_qdt->tsync_next = 0;
	
		head->g_qdt = head->g_qdt->next;
		HAL_CRITICAL_EXIT() ;
		
		return 0;
		
	}else{
		//debg("N");
		return -1;	
	}
}

s32 hal_stream_size(Stream_Head_T *head)
{
	return head->head_size;
}

