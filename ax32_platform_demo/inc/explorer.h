/****************************************************************************
                                      EXPLORER
                       THE APPOTECH X OPERATION SYSTEM
                                EXPLORER FILE LIST
                
                            (C) COPYRIGHT 2016 BUILDWIN 
                                
                    BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
                
* File Name   : explorer.h
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 12/08/2016
* Description : 
*                 
*                 
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  EXPLORER_H
    #define  EXPLORER_H



/*******************************************************************************
* Function Name  : explorerListInit
* Description    : explorer file list initial
* Input          : none
* Output         : none                                            
* Return         : int : 0
*******************************************************************************/
int explorerListInit(void);
/*******************************************************************************
* Function Name  : explorerListCreat
* Description    : explorer file list create a queue
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : queue index
*******************************************************************************/
int explorerListCreat(void);
/*******************************************************************************
* Function Name  : explorerListCreat
* Description    : explorer file list create a queue
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : queue index
*******************************************************************************/
int explorerListDestory(int queue);
/*******************************************************************************
* Function Name  : explorerListAdd
* Description    : explorer file list add a file
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : file node
*******************************************************************************/
int explorerListAdd(int queue,char *name);
/*******************************************************************************
* Function Name  : explorerListDelByName
* Description    : explorer file list del file by file name
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : 0
*******************************************************************************/
int explorerListDelByName(int queue,char *name);
/*******************************************************************************
* Function Name  : explorerListDelByIndex
* Description    : explorer file list del file by index
* Input          : int index : file index
* Output         : none                                            
* Return         : int
*******************************************************************************/
int explorerListDelByIndex(int queue,int index);
/*******************************************************************************
* Function Name  : explorerListGetCount
* Description    : explorer file list get file total number
* Input          : 
* Output         : none                                            
* Return         : int
*******************************************************************************/
int explorerListGetCount(int queue);
/*******************************************************************************
* Function Name  : explorerListFindName
* Description    : explorer file list find file by name
* Input          : char *name : file name
* Output         : none                                            
* Return         : int : 0
*******************************************************************************/
int explorerListFindName(int queue,char *name);
/*******************************************************************************
* Function Name  : explorerListGetName
* Description    : explorer file list get file name by index
* Input          : int index : file index
* Output         : none                                            
* Return         : char * : file anme
*******************************************************************************/
char *explorerListGetName(int queue,int index);


/*******************************************************************************
* Function Name  : explorerListHook
* Description    : explorer file list order,user should fill this function 
* Input          : char *name1 : file 1
                      char *name2 : file 2
* Output         : none                                            
* Return         : 
*******************************************************************************/
int explorerListHook(char *name1,char *name2);



#endif
