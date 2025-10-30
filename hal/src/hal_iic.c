/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         IIC HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_iic.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN IIC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"

#if HAL_CFG_EN_IIC1>0
static u8 halIIC1ShareFlag=0;
#endif
#if HAL_CFG_EN_IIC0 >0
static volatile u8 halIIC0Lock = 0;
#define  HAL_IIC0_LOCK()       if(halIIC0Lock){return ;}halIIC0Lock =1;
#define  HAL_IIC0_UNLOCK()    halIIC0Lock=0;
#endif
#if HAL_CFG_EN_IIC0 >0
/*******************************************************************************
* Function Name  : hal_iicInit
* Description    : hal layer .iic0 initial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic0Init(void)
{
	ax32xx_iic0Init(HAL_CFG_IIC0_BAUDRATE);

	ax32xx_iic0Stop();

}
/*******************************************************************************
* Function Name  : hal_iicUninit
* Description    : hal layer .iic0 uninitial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic0Uninit(void)
{
     ax32xx_iic0Uninit();
}
/*******************************************************************************
* Function Name  : hal_iic8bitAddrWriteData
* Description    : hal layer .iic0 write data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 data    : data
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic08bitAddrWriteData(u8 slaveid,u8 addr,u8 data)
{
	 HAL_IIC0_LOCK();
     ax32xx_iic0Start();                 // send Start singal

	 ax32xx_iic0SendByte(slaveid);  // send slaveid

	 ax32xx_iic0RecvACK();             // recv ack singal

     ax32xx_iic0SendByte(addr);     // send address

     ax32xx_iic0RecvACK();            // recv ack singal

   	 ax32xx_iic0SendByte(data);
	 ax32xx_iic0RecvACK();

	 ax32xx_iic0Stop();

	 HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic8bitAddrReadData
* Description    : hal layer .iic0 read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic08bitAddrReadData(u8 slaveid,u8 addr,u8 *data)
{
     u8 temp;	
	 HAL_IIC0_LOCK();
     ax32xx_iic0Start();                 // send Start singal

	 ax32xx_iic0SendByte(slaveid);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal

	 ax32xx_iic0SendByte(addr);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal

	 ax32xx_iic0Stop();                 // stop

	 
	 ax32xx_iic0Start();				 // send Start singal

	 ax32xx_iic0SendByte(slaveid|1);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal


   	 temp = ax32xx_iic0RecvByte();
//	 ax32xx_iic1SendACK();

	 ax32xx_iic0Stop();	
    
     if(data)
	 	*data = temp;
	 HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic8bitAddrWrite
* Description    : hal layer .iic0 write data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic08bitAddrWrite(u8 slaveid,u8 addr,u8 *data,u8 len)
{
     int i;	
	 HAL_IIC0_LOCK();
     ax32xx_iic0Start();                 // send Start singal

	 ax32xx_iic0SendByte(slaveid);  // send slaveid

	 ax32xx_iic0RecvACK();             // recv ack singal

     ax32xx_iic0SendByte(addr);     // send address

     ax32xx_iic0RecvACK();            // recv ack singal
	 for(i=0;i<len;i++)
	 {
	 	ax32xx_iic0SendByte(data[i]);    // send data
		ax32xx_iic0RecvACK();              // recv ack
	 }

	 ax32xx_iic0Stop();                   // send stop singal	
	 HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic8bitAddrRead
* Description    : hal layer .iic0 read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic08bitAddrRead(u8 slaveid,u8 addr,u8  *data,u8 len)
{
     int i;	
     HAL_IIC0_LOCK();
     ax32xx_iic0Start();                 // send Start singal

	 ax32xx_iic0SendByte(slaveid);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal

     ax32xx_iic0SendByte(addr);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal

	 ax32xx_iic0Stop();                 // stop

	 
	 ax32xx_iic0Start();				 // send Start singal

	 ax32xx_iic0SendByte(slaveid|1);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal
	 
	 for(i=0;i<len;i++)
	 {
	 	data[i] = ax32xx_iic0RecvByte();    // send data
		//ax32xx_iic0SendACK();              // recv ack
	 }

	 ax32xx_iic0Stop();                   // send stop singal
	 HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic16bitAddrWriteData
* Description    : hal layer .iic write data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 data    : data
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic016bitAddrWriteData(u8 slaveid,u16 addr,u8 data)
{
     HAL_IIC0_LOCK();
     ax32xx_iic0Start();                 // send Start singal

	 ax32xx_iic0SendByte(slaveid);  // send slaveid

	 ax32xx_iic0RecvACK();             // recv ack singal

     ax32xx_iic0SendByte(addr>>8);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal

	 ax32xx_iic0SendByte(addr&0xff);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal

   	 ax32xx_iic0SendByte(data);
	 ax32xx_iic0RecvACK();

	 ax32xx_iic0Stop();
	 HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic16bitAddrReadData
* Description    : hal layer .iic0 read data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic016bitAddrReadData(u8 slaveid,u16 addr,u8 *data)
{
     u8 temp;	
     HAL_IIC0_LOCK();
     ax32xx_iic0Start();                 // send Start singal

	 ax32xx_iic0SendByte(slaveid);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal

	 ax32xx_iic0SendByte(addr>>8);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal

	 ax32xx_iic0SendByte(addr&0xff);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal

	 ax32xx_iic0Stop();                 // stop

	 
	 ax32xx_iic0Start();				 // send Start singal

	 ax32xx_iic0SendByte(slaveid|1);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal


   	 temp = ax32xx_iic0RecvByte();
//	 ax32xx_iic1SendACK();

	 ax32xx_iic0Stop();
	 
	 if(data)
		 *data =temp;
	 HAL_IIC0_UNLOCK();
}
/*******************************************************************************
* Function Name  : hal_iic16bitAddrWrite
* Description    : hal layer .iic0 write data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic016bitAddrWrite(u8 slaveid,u16 addr,u8 *data,u8 len)
{
     int i;	
	 HAL_IIC0_LOCK();
     ax32xx_iic0Start();                 // send Start singal

	 ax32xx_iic0SendByte(slaveid);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal

	 ax32xx_iic0SendByte(addr>>8);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal

	 ax32xx_iic0SendByte(addr&0xff);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal
     

	 for(i=0;i<len;i++)
	 {
	 	ax32xx_iic0SendByte(data[i]);    // send data
		ax32xx_iic0RecvACK();              // recv ack
	 }

	 ax32xx_iic0Stop();                   // send stop singal
	 HAL_IIC0_UNLOCK();
}

/*******************************************************************************
* Function Name  : hal_iic16bitAddrRead
* Description    : hal layer .iic0 read data for 16bit address slave
* Input          : u8 slaveid : slave id
                      u16 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic016bitAddrRead(u8 slaveid,u16 addr,u8  *data,u8 len)
{
     int i;	
	 HAL_IIC0_LOCK();
     ax32xx_iic0Start();                 // send Start singal

	 ax32xx_iic0SendByte(slaveid);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal

	 ax32xx_iic0SendByte(addr>>8);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal

	 ax32xx_iic0SendByte(addr&0xff);     // send address
     ax32xx_iic0RecvACK();            // recv ack singal


	 ax32xx_iic0Stop();                 // stop

	 
	 ax32xx_iic0Start();				 // send Start singal

	 ax32xx_iic0SendByte(slaveid|1);  // send slaveid
	 ax32xx_iic0RecvACK();             // recv ack singal
	 
	 for(i=0;i<len;i++)
	 {
	 	data[i] = ax32xx_iic0RecvByte();    // send data
	//	ax32xx_iic1SendACK();              // recv ack
	 }

	 ax32xx_iic0Stop();                   // send stop singal
	 HAL_IIC0_UNLOCK();
 }

#endif


#if HAL_CFG_EN_IIC1 >0
/*******************************************************************************
* Function Name  : hal_iic1IOShare
* Description    : hal layer .iic1 io share
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
u8 hal_iic1IOShare(void)
{
	u8 temp;

	temp = halIIC1ShareFlag;

	halIIC1ShareFlag = 1;

	return temp;
}
/*******************************************************************************
* Function Name  : hal_iicInit
* Description    : hal layer .iic1 initial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic1Init(void)
{
	halIIC1ShareFlag = 0;
	ax32xx_iic1Init(HAL_CFG_IIC1_BAUDRATE);

	ax32xx_iic1Stop();
}


/*******************************************************************************
* Function Name  : hal_iicUninit
* Description    : hal layer .iic1 uninitial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic1Uninit(void)
{
	ax32xx_iic1Uninit();
	halIIC1ShareFlag = 0;
}


/*******************************************************************************
* Function Name  : hal_iic8bitAddrWriteData
* Description    : hal layer .iic1 write data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 data    : data
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic18bitAddrWriteData(u8 slaveid,u8 addr,u8 data)
{

	ax32xx_iic1Start(); 				// send Start singal

	ax32xx_iic1SendByte(slaveid);  // send slaveid

	ax32xx_iic1RecvACK();			  // recv ack singal

	ax32xx_iic1SendByte(addr);	   // send address

	ax32xx_iic1RecvACK();			 // recv ack singal

	ax32xx_iic1SendByte(data);
	ax32xx_iic1RecvACK();

	ax32xx_iic1Stop();
    halIIC1ShareFlag = 0;
}

/*******************************************************************************
* Function Name  : hal_iic8bitAddrReadData
* Description    : hal layer .iic1 read data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic18bitAddrReadData(u8 slaveid,u8 addr,u8 *data)
{
     u8 temp;	

    ax32xx_iic1Start();                 // send Start singal

	 ax32xx_iic1SendByte(slaveid);  // send slaveid
	 ax32xx_iic1RecvACK();             // recv ack singal

	  ax32xx_iic1SendByte(addr);     // send address
     ax32xx_iic1RecvACK();            // recv ack singal

	 ax32xx_iic1Stop();                 // stop

	 
	 ax32xx_iic1Start();				 // send Start singal

	 ax32xx_iic1SendByte(slaveid|1);  // send slaveid
	 ax32xx_iic1RecvACK();             // recv ack singal


   	 temp = ax32xx_iic1RecvByte();
//	 ax32xx_iic1SendACK();

	 ax32xx_iic1Stop();	
    
     if(data)
	 	*data = temp;
	 halIIC1ShareFlag = 0;
}

/*******************************************************************************
* Function Name  : hal_iic8bitAddrWrite
* Description    : hal layer .iic1 write data for 8bit address slave
* Input          : u8 slaveid : slave id
                      u8 addr    : slave addr
                      u8 *data  : data buffer
                      u8 len      : data length
* Output         : None
* Return         : none
*******************************************************************************/
void hal_iic18bitAddrWrite(u8 slaveid,u8 addr,u8 *data,u8 len)
{
     int i;	


	ax32xx_iic1Start(); 				// send Start singal

	ax32xx_iic1SendByte(slaveid);  // send slaveid

	ax32xx_iic1RecvACK();			  // recv ack singal

	ax32xx_iic1SendByte(addr);	   // send address

	ax32xx_iic1RecvACK();			 // recv ack singal
	for(i=0;i<len;i++)
	{
	   ax32xx_iic1SendByte(data[i]);	// send data
	   ax32xx_iic1RecvACK();			  // recv ack
	}

	ax32xx_iic1Stop();					 // send stop singal	

	halIIC1ShareFlag = 0;
}

 /*******************************************************************************
 * Function Name  : hal_iic8bitAddrRead
 * Description	  : hal layer .iic1 read data for 8bit address slave
 * Input		  : u8 slaveid : slave id
					   u8 addr	  : slave addr
					   u8 *data  : data buffer
					   u8 len	   : data length
 * Output		  : None
 * Return		  : none
 *******************************************************************************/
 void hal_iic18bitAddrRead(u8 slaveid,u8 addr,u8  *data,u8 len)
{
	int i;
	
	 ax32xx_iic1Start();				 // send Start singal

	 ax32xx_iic1SendByte(slaveid);	// send slaveid
	 ax32xx_iic1RecvACK();			   // recv ack singal

	 ax32xx_iic1SendByte(addr); 	// send address
	 ax32xx_iic1RecvACK();			  // recv ack singal

	 ax32xx_iic1Stop(); 				// stop

	 
	 ax32xx_iic1Start();				 // send Start singal

	 ax32xx_iic1SendByte(slaveid|1);  // send slaveid
	 ax32xx_iic1RecvACK();			   // recv ack singal
	 
	 for(i=0;i<len;i++)
	 {
		data[i] = ax32xx_iic1RecvByte();	// send data
	//	ax32xx_iic1SendACK();			   // recv ack
	 }

	 ax32xx_iic1Stop(); 				  // send stop singal	

	 halIIC1ShareFlag = 0;
}

/*******************************************************************************
* Function Name  : hal_iic16bitAddrWriteData
* Description	 : hal layer .iic write data for 16bit address slave
* Input 		 : u8 slaveid : slave id
					  u8 addr	 : slave addr
					  u8 data	 : data
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_iic116bitAddrWriteData(u8 slaveid,u16 addr,u8 data)
{
	ax32xx_iic1Start(); 				// send Start singal

	ax32xx_iic1SendByte(slaveid);  // send slaveid

	ax32xx_iic1RecvACK();			  // recv ack singal

	ax32xx_iic1SendByte(addr>>8);	  // send address
	ax32xx_iic1RecvACK();			 // recv ack singal

	ax32xx_iic1SendByte(addr&0xff); 	// send address
	ax32xx_iic1RecvACK();			 // recv ack singal

	ax32xx_iic1SendByte(data);
	ax32xx_iic1RecvACK();

	ax32xx_iic1Stop();		

	halIIC1ShareFlag = 0;
}

 /*******************************************************************************
 * Function Name  : hal_iic16bitAddrReadData
 * Description	  : hal layer .iic1 read data for 16bit address slave
 * Input		  : u8 slaveid : slave id
					   u16 addr    : slave addr
					   u8 *data  : data buffer
					   u8 len	   : data length
 * Output		  : None
 * Return		  : none
 *******************************************************************************/
 void hal_iic116bitAddrReadData(u8 slaveid,u16 addr,u8 *data)
 {
 	u8 temp;
	
     ax32xx_iic1Start();                 // send Start singal

	 ax32xx_iic1SendByte(slaveid);  // send slaveid
	 ax32xx_iic1RecvACK();             // recv ack singal

	 ax32xx_iic1SendByte(addr>>8);     // send address
     ax32xx_iic1RecvACK();            // recv ack singal

	 ax32xx_iic1SendByte(addr&0xff);     // send address
     ax32xx_iic1RecvACK();            // recv ack singal

	 ax32xx_iic1Stop();                 // stop

	 
	 ax32xx_iic1Start();				 // send Start singal

	 ax32xx_iic1SendByte(slaveid|1);  // send slaveid
	 ax32xx_iic1RecvACK();             // recv ack singal


   	 temp = ax32xx_iic1RecvByte();
//	 ax32xx_iic1SendACK();

	 ax32xx_iic1Stop();

	if(data)
	   *data = temp;
	halIIC1ShareFlag = 0;
}

 /*******************************************************************************
 * Function Name  : hal_iic16bitAddrWrite
 * Description	  : hal layer .iic1 write data for 16bit address slave
 * Input		  : u8 slaveid : slave id
					   u16 addr    : slave addr
					   u8 *data  : data buffer
					   u8 len	   : data length
 * Output		  : None
 * Return		  : none
 *******************************************************************************/
 void hal_iic116bitAddrWrite(u8 slaveid,u16 addr,u8 *data,u8 len)
{
	int i;
	
	ax32xx_iic1Start(); 				// send Start singal

	ax32xx_iic1SendByte(slaveid);  // send slaveid
	ax32xx_iic1RecvACK();			  // recv ack singal

	ax32xx_iic1SendByte(addr>>8);	  // send address
	ax32xx_iic1RecvACK();			 // recv ack singal

	ax32xx_iic1SendByte(addr&0xff); 	// send address
	ax32xx_iic1RecvACK();			 // recv ack singal


	for(i=0;i<len;i++)
	{
	   ax32xx_iic1SendByte(data[i]);	// send data
	   ax32xx_iic1RecvACK();			  // recv ack
	}

	ax32xx_iic1Stop();					 // send stop singal

	halIIC1ShareFlag = 0;
}

 /*******************************************************************************
 * Function Name  : hal_iic16bitAddrRead
 * Description	  : hal layer .iic1 read data for 16bit address slave
 * Input		  : u8 slaveid : slave id
					   u16 addr    : slave addr
					   u8 *data  : data buffer
					   u8 len	   : data length
 * Output		  : None
 * Return		  : none
 *******************************************************************************/
 void hal_iic116bitAddrRead(u8 slaveid,u16 addr,u8  *data,u8 len)
 {
 	int i;
	
	 ax32xx_iic1Start();				 // send Start singal

	 ax32xx_iic1SendByte(slaveid);	// send slaveid
	 ax32xx_iic1RecvACK();			   // recv ack singal

	 ax32xx_iic1SendByte(addr>>8);	   // send address
	 ax32xx_iic1RecvACK();			  // recv ack singal

	 ax32xx_iic1SendByte(addr&0xff);	 // send address
	 ax32xx_iic1RecvACK();			  // recv ack singal


	 ax32xx_iic1Stop(); 				// stop

	 
	 ax32xx_iic1Start();				 // send Start singal

	 ax32xx_iic1SendByte(slaveid|1);  // send slaveid
	 ax32xx_iic1RecvACK();			   // recv ack singal
	 
	 for(i=0;i<len;i++)
	 {
		data[i] = ax32xx_iic1RecvByte();	// send data
	//	ax32xx_iic1SendACK();			   // recv ack
	 }

	 ax32xx_iic1Stop(); 				  // send stop singal

	 halIIC1ShareFlag = 0;

}


#endif




