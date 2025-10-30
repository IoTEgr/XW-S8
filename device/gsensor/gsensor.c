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
* File Name   : gsensor.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is gsensor device file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "gsensor.h"

#define  ARG_NULL_CHECK(arg)   {if(arg==NULL)return -1;}

extern G_Sensor_OP_T gma301;
extern G_Sensor_OP_T sc7a30e;
extern G_Sensor_OP_T da380;
static G_Sensor_OP_T  *gSensorPool[G_SENSOR_MAX]=
{
#if 0//HAL_CFG_EN_DBG == 0
	&gma301,  // gma301	
	&sc7a30e,
	&da380,
#else
    NULL,
    NULL,
    NULL,
#endif
};

static G_Sensor_OP_T *gSensor = NULL;
static INT8U parkFlag = 0;

/*******************************************************************************
* Function Name  : gsensor_iic_read
* Description    : read gsensor data from iic bus
* Input          : INT8U sid : slave id
                     INT8U addr: read address
                     INT8U *data : data
* Output         : none                                            
* Return         : none
*******************************************************************************/
void gsensor_iic_read(INT8U sid,INT8U addr,INT8U *data)
{
#if HAL_CFG_EN_IIC1
    hal_iic18bitAddrReadData(sid,addr,data);
#endif
}
/*******************************************************************************
* Function Name  : gsensor_iic_write
* Description    : write gsensor data to iic bus
* Input          : INT8U sid : slave id
                     INT8U addr: read address
                     INT8U data : data
* Output         : none                                            
* Return         : none
*******************************************************************************/
void gsensor_iic_write(INT8U sid,INT8U addr,INT8U data)
{
#if HAL_CFG_EN_IIC1
	 hal_iic18bitAddrWriteData(sid,addr,data);
#endif
}
/*******************************************************************************
* Function Name  : gsensor_iic_enable
* Description    : enable iic for gsensor r/w
* Input          :none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void gsensor_iic_enable(void)
{
//	hal_uartIOShare();
//	hal_gpioEPullSet(GPIO_PA,GPIO_PIN7,GPIO_PULLE_FLOATING);
#if HAL_CFG_EN_IIC1
    hal_iic1Init();	
#endif
}
/*******************************************************************************
* Function Name  : gsensor_iic_disable
* Description    : disable gsensor r/w iic
* Input          :none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void gsensor_iic_disable(void)
{
#if HAL_CFG_EN_IIC1
	hal_iic1Uninit();  // release iic1
#endif
//	hal_iicInit(IIC0);

   
}

/*******************************************************************************
* Function Name  : gSensorInit
* Description    : initial gsensor
* Input          :none
* Output         : none                                            
* Return         : wake up state
*******************************************************************************/
int gSensorInit(void)
{
	int i,ret;

	hal_vddGSENEnable(1);
//	hal_sysDelayMS(10);
//----------------------auto check current gsensor type----------------
	for(i=0;i<G_SENSOR_MAX;i++)
	{
		
		if(gSensorPool[i] && gSensorPool[i]->init)
		{
			deg_Printf("gsensor : check %s\n",gSensorPool[i]->name);
			ret = gSensorPool[i]->init();
			if(ret>=0)
				break;
			
		}
	}
	if(i>=G_SENSOR_MAX)
	{
		deg_Printf("gsensor : initial fail.can not find support gsensor type.\n");
		gSensor = NULL;
		//ax32xx_VDDGSENEnable(0);
		return -1;
	}
    deg_Printf("gsensor : initial ok :%s.parkFlag=%d\n",gSensorPool[i]->name,parkFlag);
	gSensor = gSensorPool[i];  // save current gsensor type
	parkFlag = ret; // 

	return ret;
}
/*******************************************************************************
* Function Name  : gSensorUninit
* Description    : ubinitial gsensor.disable gsensor
* Input          :none
* Output         : none                                            
* Return         : 
*******************************************************************************/
int gSensorUninit(void)
{
	ARG_NULL_CHECK(gSensor);
	ARG_NULL_CHECK(gSensor->ioctrl);

	return gSensor->ioctrl(gSensor->prity,G_IOCTRL_DISABLE,0);
}
/*******************************************************************************
* Function Name  : gSensorWakeUpGet
* Description    : get wakeup state
* Input          :none
* Output         : none                                            
* Return         : int : 0->no wakeup
                             1->gsensor waked
*******************************************************************************/
int gSensorWakeUpGet(void)
{
	ARG_NULL_CHECK(gSensor);
	
	return parkFlag;
}
/*******************************************************************************
* Function Name  : gSensorWakeupSet
* Description    : set wakeup state
* Input          :int level : wake up level
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int gSensorWakeupSet(int level)
{
	ARG_NULL_CHECK(gSensor);
	ARG_NULL_CHECK(gSensor->ioctrl);

	return gSensor->ioctrl(gSensor->prity,G_IOCTRL_WAKEUPSET,level);
}
/*******************************************************************************
* Function Name  : gSensorActiveGet
* Description    : get active
* Input          :none
* Output         : none                                            
* Return         : int : 0->no active
                             1->gsensor actived
*******************************************************************************/
int gSensorActiveGet(void)
{
	ARG_NULL_CHECK(gSensor);
	ARG_NULL_CHECK(gSensor->ioctrl);

	return gSensor->ioctrl(gSensor->prity,G_IOCTRL_ACTIVEGET,0);
}
/*******************************************************************************
* Function Name  : gSensorActiveSet
* Description    : set active state
* Input          :int level : active level
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int gSensorActiveSet(int level)
{
	ARG_NULL_CHECK(gSensor);
	ARG_NULL_CHECK(gSensor->ioctrl);
	
	return gSensor->ioctrl(gSensor->prity,G_IOCTRL_ACTIVESET,level);	
}
/*******************************************************************************
* Function Name  : gSensorRead
* Description    : read gsensor
* Input          :
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int gSensorRead(int *x,int *y,int *z)
{
	ARG_NULL_CHECK(gSensor);
	ARG_NULL_CHECK(gSensor->readxyz);

	return gSensor->readxyz(x,y,z);
}
/*******************************************************************************
* Function Name  : gSensorMircoMotionWait
* Description    : wait gsensor stable
* Input          :
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int gSensorMircoMotionWait(int level)
{
    volatile int cnt=0;

    ARG_NULL_CHECK(gSensor);
	ARG_NULL_CHECK(gSensor->ioctrl);


	while(1)
	{
		if(gSensor->ioctrl(gSensor->prity,G_IOCTRL_ACTIVECHK,level))
		{
			cnt = 0;
		}
		else
			cnt++;
		if(cnt>=10)
			return 1;
	}
	
}
/*******************************************************************************
* Function Name  : gSensorGetName
* Description    : get gsensor name
* Input          :
* Output         : none                                            
* Return         : char *
*******************************************************************************/
char *gSensorGetName(void)
{
	if(gSensor == NULL)
		return ((char *)("NULL"));

	return gSensor->name;
}








