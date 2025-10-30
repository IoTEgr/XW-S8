/****************************************************************************
**
 **                              BOARD
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  BOARD 
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : board.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is for user board
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"


typedef struct Board_S
{
	INT16S count;
	INT16S stat;
	Board_Node_T *table;
	
}Board_T;

extern Board_Node_T  board_bwv1[];
extern Board_Node_T  board_fpga[];


static Board_T board=
{
#if (SYSTEM_BOARD == BOARD_TYPE_BWV1)
    .table = board_bwv1,
#elif (SYSTEM_BOARD == BOARD_TYPE_FPGA)
	.table = board_fpga,
#endif
	.count = 0,
};

/*******************************************************************************
* Function Name  : boardInit
* Description    : 
* Input          : Board_Node_T *table : board node table
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int boardInit(Board_Node_T *table)
{
	int i;
	Board_Node_T *node;

	
//--------------system device initial-----------------

    i = hal_rtcInit();	

    hal_rtcAlarmSet(0,0);
    
	hal_spiInit();
	i = hal_spiFlashReadID();

	hal_watermarkInit();

	//hal_mdInit();

    hal_adcInit();

	hal_usbdEnable(1);

	hal_auadcInit();
	hal_dacInit();	
//-----------------------------------------------	
	if(table == NULL && board.table == NULL)
	{
		deg_Printf("board installer : no board installed.\n");
		return -1;
	}

	if(table!=NULL)
	{
		board.count = 0;
		board.table = table;
	}

	if(board.table == NULL)
	{
		deg_Printf("board installer : fail.\n");
		return -1;
	}

	i = 0;
	node = board.table;
	while(1)
	{
		if(node->name[0]==0)
			break;
		if(board.count && i>=board.count)
			break;
		i++;
		deg_Printf("board installer : installing %s...",node->name);
		if(node->init)
		{
			if(node->init()<0)
			{
				deg_Printf("->fail.\n");
				node->prit = 0;
			}
			else
			{
				deg_Printf("->ok.\n");
				node->prit = 1;
			}
		}
        else
        {
			deg_Printf("->unknow.\n");
			node->prit = 1;
        }
		node++;
	}

	if(board.count ==0)
		board.count  = i;
	deg_Printf("board installer : finish.total : %d\n",i);
//---------------------------------
  
    
	return 0;
}
/*******************************************************************************
* Function Name  : boardOpen
* Description    :  open a board node by node name
* Input          : char *name : name
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int boardOpen(char *name)
{
	Board_Node_T *node;
	int i;

    if(board.table == NULL)
		return -1;
	node = board.table;
	for(i=0;i<board.count;i++)
	{
		deg_Printf("%s      %s \n",name ,  node->name);
	    if(strcmp(name,node->name) == 0)
	    {
            if(node->prit)
			    return (i+1);
			else
				break;
	    }
		node++;
	}

	return -1;
}
/*******************************************************************************
* Function Name  : boardIoctrl
* Description    :  ioctrl for board node
* Input          : int fd : node index
                      INT32U op: operation
                      INT32U para:paramater
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int boardIoctrl(int fd,INT32U op,INT32U para)
{
	Board_Node_T *node;	
	
	if(fd>=0)
       fd-=1;
	
    if((board.table == NULL) || (fd<0) || (fd>=board.count))
    {
		//deg_Printf("board : ioctrl fail.curfd = %d,maxfd = %d\n",fd,board.count);
		return -1;
    }
	node = board.table;	

	node+=fd;

	if(node->ioctrl)
		return node->ioctrl(node->prit,op,para);

	return -1;
}
/*******************************************************************************
* Function Name  : boardNameIoctrl
* Description    :  ioctrl for board node
* Input          : char *name : name
                      INT32U op: operation
                      INT32U para:paramater
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int boardNameIoctrl(char *name,INT32U op,INT32U para)
{
	Board_Node_T *node;
	int i;

    if(board.table == NULL)
		return -1;
	node = board.table;
	for(i=0;i<board.count;i++)
	{
	    if(strcmp(name,node->name) == 0)
			break;
		node++;
	}	

	if(i>=board.count)
		return -1;

	if(node->ioctrl)
		return node->ioctrl(node->prit,op,para);

	return -1;	
}






