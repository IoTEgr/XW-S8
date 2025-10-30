/****************************************************************************
**
 **                              DEVICE
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  DEVICE G-SENSOR HEADER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : gsensor.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  GSENSOR_H
    #define   GSENSOR_H



#include "../../hal/inc/hal.h"   
#include "../../xos/xos.h"

#define  G_IOCTRL_ENABLE         1      // enable gsensor
#define  G_IOCTRL_DISABLE       2      // disable gsensor
#define  G_IOCTRL_WAKEUPSET    3      // wakeup set
#define  G_IOCTRL_WAKEUPGET    4      // wakeup flag get
#define  G_IOCTRL_ACTIVESET    5      // active for lock. active level set
#define  G_IOCTRL_ACTIVEGET    6      // active for lock.active level set
#define  G_IOCTRL_ACTIVECHK    7

typedef struct G_Sensor_OP_S
{
	char name[12];
	int (*init)(void);

    int (*readxyz)(int *x,int *y,int *z);
	int (*ioctrl)(int handle,int op,int para);

	int prity;
}G_Sensor_OP_T;


enum  // g-sensor active level
{
	G_LEVEL_OFF=0,
	G_LEVEL_LOW,
	G_LEVEL_MIDDLE,
	G_LEVEL_HIGH,

	G_LEVEL_MAX
};


typedef enum
{
	G_SENSOR_GMA301=0,
	G_SENSOR_SC7A30E,
	G_SENSOR_DA380,



	G_SENSOR_MAX
}G_SENSOR_TYPE_E;






/*******************************************************************************
* Function Name  : gsensor_iic_read
* Description    : read gsensor data from iic bus
* Input          : INT8U sid : slave id
                     INT8U addr: read address
                     INT8U *data : data
* Output         : none                                            
* Return         : none
*******************************************************************************/
void gsensor_iic_read(INT8U sid,INT8U addr,INT8U *data);
/*******************************************************************************
* Function Name  : gsensor_iic_write
* Description    : write gsensor data to iic bus
* Input          : INT8U sid : slave id
                     INT8U addr: read address
                     INT8U data : data
* Output         : none                                            
* Return         : none
*******************************************************************************/
void gsensor_iic_write(INT8U sid,INT8U addr,INT8U data);
/*******************************************************************************
* Function Name  : gsensor_iic_enable
* Description    : enable iic for gsensor r/w
* Input          :none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void gsensor_iic_enable(void);
/*******************************************************************************
* Function Name  : gsensor_iic_disable
* Description    : disable gsensor r/w iic
* Input          :none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void gsensor_iic_disable(void);
/*******************************************************************************
* Function Name  : gSensorInit
* Description    : initial gsensor
* Input          :none
* Output         : none                                            
* Return         : wake up state
*******************************************************************************/
int gSensorInit(void);
/*******************************************************************************
* Function Name  : gSensorUninit
* Description    : ubinitial gsensor.disable gsensor
* Input          :none
* Output         : none                                            
* Return         : 
*******************************************************************************/
int gSensorUninit(void);
/*******************************************************************************
* Function Name  : gSensorWakeUpGet
* Description    : get wakeup state
* Input          :none
* Output         : none                                            
* Return         : int : 0->no wakeup
                             1->gsensor waked
*******************************************************************************/
int gSensorWakeUpGet(void);
/*******************************************************************************
* Function Name  : gSensorWakeupSet
* Description    : set wakeup state
* Input          :int level : wake up level
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int gSensorWakeupSet(int level);
/*******************************************************************************
* Function Name  : gSensorActiveGet
* Description    : get active
* Input          :none
* Output         : none                                            
* Return         : int : 0->no active
                             1->gsensor actived
*******************************************************************************/
int gSensorActiveGet(void);
/*******************************************************************************
* Function Name  : gSensorActiveSet
* Description    : set active state
* Input          :int level : active level
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int gSensorActiveSet(int level);
/*******************************************************************************
* Function Name  : gSensorRead
* Description    : read gsensor
* Input          :
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int gSensorRead(int *x,int *y,int *z);
/*******************************************************************************
* Function Name  : gSensorMircoMotionWait
* Description    : wait gsensor stable
* Input          :
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int gSensorMircoMotionWait(int level);
/*******************************************************************************
* Function Name  : gSensorGetName
* Description    : get gsensor name
* Input          :
* Output         : none                                            
* Return         : char *
*******************************************************************************/
char *gSensorGetName(void);


























#endif

