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
* File Name   : hal_stream.h
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
#ifndef  HAL_STREAM_H
    #define  HAL_STREAM_H



typedef struct Stream_Node_S
{
	u8 sta;
	u8	*pbuf;
	u32 buflen;
	s32 tsync;
	s32 tsync_next;
	struct Stream_Node_S *last; 
	struct Stream_Node_S *next; 
}Stream_Node_T;



typedef struct Stream_Info_S
{
	u8 semf;
	u8 *sf;
	u8 *ef;
	u8 *pcache;
	u32 cachelen;
	u32 cacheuse;
}Stream_Info_T;


typedef struct Stream_Head_S
{
	Stream_Info_T c_inf;
	Stream_Node_T *p_qdt; 
	Stream_Node_T *g_qdt;
	volatile s32 head_size;
}Stream_Head_T;







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
int hal_streamInit(Stream_Head_T *head,Stream_Node_T *pool,u32 num,u32 mem,u32 mem_len);
/*******************************************************************************
* Function Name  : hal_srreamMalloc
* Description    : hal layer,stream management.for stream media.stream malloc a memory for write
* Input          : Stream_Head_T *head : stream head for operation handler
				u32 mlen : need memory length
* Output         : None
* Return         :  mem
*******************************************************************************/
u32 hal_streamMalloc(Stream_Head_T *head, u32 mlen);
/*******************************************************************************
* Function Name  : hal_streamIn
* Description    : hal layer,stream management.for stream media.stream queue in
* Input          : Stream_Head_T *head : stream head for operation handler
				u32 p : memory address
				u32 len: memory length
* Output         : None
* Return         :  0:success
*******************************************************************************/
int hal_streamIn(Stream_Head_T *head, u32 p, u32 len,s32 t_sync,s32 t_sync_next);
/*******************************************************************************
* Function Name  : hal_streamOut
* Description    : hal layer,stream management.for stream media.stream queue out
* Input          : Stream_Head_T *head : stream head for operation handler
				u32 *p : memory address
				u32 *len: memory length
* Output         : None
* Return         :  0:success
*******************************************************************************/
int hal_streamOut(Stream_Head_T *head, u32 *p, u32 *len,s32 *sync, s32 *sync_next);
int hal_streamOut_next(Stream_Head_T *head, u32 *p, u32 *len,s32 *sync, s32 *sync_next);

/*******************************************************************************
* Function Name  : hal_streamfree
* Description    : hal layer,stream management.for stream media.stream queue free
* Input          : Stream_Head_T *head : stream head for operation handler
* Output         : None
* Return         :  0:success
*******************************************************************************/
int hal_streamfree(Stream_Head_T *c);
void hal_streamIn_Drop(Stream_Head_T *head);

s32 hal_stream_size(Stream_Head_T *head);










#endif



