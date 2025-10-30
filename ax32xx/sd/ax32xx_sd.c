/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX SDC
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_sd.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"


#define  R_SD0_CTRL0        SDCON0
#define  R_SD0_CTRL1        SDCON1
#define  R_SD0_BAUDRATE     SDBAUD
#define  R_SD0_CMDIDX       SDIDX
#define  R_SD0_CMDARG       SDARG
#define  R_SD0_CADDR        SDCADR
#define  R_SD0_COUNT        SDDCNT
#define  R_SDO_DADDR        SDDADR

#define  SD0_BAUDRATE       400000

#define SD_MAP_CON              PMAPCON0
#define SD_MAP_POS              4
#define SD_MAP_MSK              15

#define SD1S_MAP_VAL            0b01
#define SD1S_PORT               PORTA
#define SD1S_CLK_PIN            3
#define SD1S_CMD_PIN            2
#define SD1S_DAT0_PIN           4
#define SD1S_DAT1_PIN           5
#define SD1S_DAT2_PIN           0
#define SD1S_DAT3_PIN           1

#define OFFS_DIR                0x10
#define OFFS_PU                 0x20
#define OFFS_PD                 0x28


//#define WIDTH_CRCS  3
#define BIT_CRCS    21
#define BIT_BUSY    20
#define BIT_NRPS    19
#define BIT_CCRCE   18
#define BIT_CPND    17
#define BIT_DCRCE   16
#define BIT_DPND    15
#define BIT_SDIOPND     14
#define BIT_SDIOCAP     10


#define BIT_DIE     9
#define BIT_CIE     8
#define SDIOIE      7

#define BIT_SPCRCSTA 5
#define BIT_8CKE    4
#define BIT_ORISE   3
#define BIT_DW4     2
#define BIT_SDCKE   1
#define BIT_SDE     0

// SDCCON1
#define BIT_CPCLR      17
#define BIT_DPCLR      15
#define BIT_SDIOPCLR   14
//#define WIDTH_DKST  2
#define BIT_SDIOIEPCLR  7
#define BIT_DKST    4
//#define WIDTH_CKST  3
#define BIT_CKST    0

#define  OFFS_CFG0              0x00
#define  OFFS_CFG1              0x01
#define  OFFS_CFG2              0x02
#define  OFFS_BAUD              0x01
#define  OFFS_DMASP             0x04
#define  OFFS_DCNT              0x05
#define  OFFS_CMD0              0x05
#define  OFFS_CMD1              0x07
#define  OFFS_RESP0             0x08
#define  OFFS_RESP1             0x09
#define  OFFS_RESP2             0x0A
#define  OFFS_RESP3             0x0B
#define  OFFS_RESP4             0x0C

// Alias
#define OFFS_CFG    OFFS_CFG0
#define OFFS_CMD    OFFS_CFG1
#define OFFS_STA    OFFS_CFG2
#define OFFS_INDEX  OFFS_CMD0
#define OFFS_ARG    OFFS_CMD1
#define OFFS_RESP6B OFFS_RESP0

#define GET_SD_R1()         SDARG//SD_REG[OFFS_RESP6B]
#define GET_SD_R7_CMDIDX()  SDIDX//SD_REG[OFFS_RESP1]
//#define GET_SD_CSD1()       GPARAM(SD_RESP1)//SD_REG[OFFS_RESP1]
//#define GET_SD_CSD2()       GPARAM(SD_RESP2)//SD_REG[OFFS_RESP2]
#define CHK_SD_DCRCE()      SDCON0 & BIT(BIT_DCRCE)


#define NO_RESP BIT(BIT_CKST+1)  //send cmd. without receive response; without check busy
#define RESP_1  BIT(BIT_CKST+0)  //send cmd. receive 6 byte response; without check busy
#define RESP_1B (BIT(BIT_CKST+2) | BIT(BIT_CKST+0))  //send cmd. receive 6 byte response; with check busy
#define RESP_2  (BIT(BIT_CKST+1) | BIT(BIT_CKST+0))  //send cmd. receive 17 byte response; without check busy
#define RESP_3  BIT(BIT_CKST+0)  //send cmd. receive 6 byte response; without check busy
#define RESP_6  BIT(BIT_CKST+0)  //send cmd. receive 6 byte response; without check busy
#define RESP_7  BIT(BIT_CKST+0)  //send cmd. receive 6 byte response; without check busy




static u8 sd0StaticBuffer[64] __attribute__ ((section(".sdc_buf")));


/*******************************************************************************
* Function Name  : ax32xx_sd0Init
* Description    : initial sd0 
* Input          : sd_bus_width    0:SD_BUS_WIDTH1,1:SD_BUS_WIDTH4
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0Init(u8 sd_bus_width)
{
    ax32xx_sysClkSet(SYS_CLK_SD0,1);
	ax32xx_dmaChannelEnable(DMA_SD0,1);

	ax32xx_gpioSFRSet(GPIO_MAP_SD0,SD0_POS);
    if(SD0_POS == SD0_POS0) //SD0_POS_CLK_PA2_CMD_PA0_D0_PA1_D1_PA3_D2_PF10_D3_PF11
    {
    	if(SD_BUS_WIDTH1==sd_bus_width)
    	{
			ax32xx_gpioMapSet(GPIO_PA,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2/*|GPIO_PIN3*/,GPIO_FUNC_SFR);
			//ax32xx_gpioMapSet(GPIO_PF,GPIO_PIN10|GPIO_PIN11,GPIO_FUNC_SFR);
	        ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2/*|GPIO_PIN3*/,GPIO_PULL_UP);
			//ax32xx_gpioPullSet(GPIO_PF,GPIO_PIN10|GPIO_PIN11,GPIO_PULL_UP);
	        ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN2,GPIO_OUTPUT);
	        ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN1/*|GPIO_PIN3*/,GPIO_INPUT);
			//ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN10|GPIO_PIN11,GPIO_INPUT);
    	}
		else
		{
	        ax32xx_gpioMapSet(GPIO_PA,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3,GPIO_FUNC_SFR);
			ax32xx_gpioMapSet(GPIO_PF,GPIO_PIN10|GPIO_PIN11,GPIO_FUNC_SFR);
	        ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN0|GPIO_PIN1|GPIO_PIN2|GPIO_PIN3,GPIO_PULL_UP);
			ax32xx_gpioPullSet(GPIO_PF,GPIO_PIN10|GPIO_PIN11,GPIO_PULL_UP);
	        ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN2,GPIO_OUTPUT);
	        ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN1|GPIO_PIN3,GPIO_INPUT);
			ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN10|GPIO_PIN11,GPIO_INPUT);
		}
    }

	R_SD0_CTRL0 = 0;
	R_SD0_CTRL0 |= /*BIT(BIT_8CKE) |*/ BIT(BIT_SDCKE)| BIT(6);/*BIT(5)*/;
	R_SD0_CTRL0 &= ~(BIT(13)|BIT(12)|BIT(31)); //clk early
	R_SD0_BAUDRATE  = APB_CLK/SD0_BAUDRATE-1;//APB_CLK/(250000) - 1;
	R_SD0_CTRL1 &= ~BIT(6);//DSRC:0：dma;1:sfr 
	R_SD0_CTRL0 |= BIT(BIT_SDE);
	//ax32xx_sysCpuMsDelay(20); 
}
/*******************************************************************************
* Function Name  : ax32xx_sd0Uninit
* Description    :uninitial sd0 
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0Uninit(void)
{
	R_SD0_CTRL0 &= ~(1<<BIT_SDE);    			 	//sd0 off
	R_SD0_CTRL1 |= (1<<BIT_DPCLR)|(1<<BIT_CPCLR);
	
	R_SD0_CTRL0 &= ~((1<<BIT_DIE) | (1<<BIT_CIE)); 	//disable command/data interrupt
	ax32xx_sysClkSet(SYS_CLK_SD0,0);  					//SD0 clock disable	
}
/*******************************************************************************
* Function Name  : ax32xx_sd0SendCmd
* Description    : send command  
* Input          : u8 byCMDIndex : command index
                      u32 dwArgument: command argument
* Output         : None
* Return         : int : -1 :fail
                               1 : success
*******************************************************************************/
s32 ax32xx_sd0SendCmd(u8 byCMDIndex, u32 dwArgument)
{
	s32 ret;
	u32 u32Temp=0;
//    volatile u32 i;

	R_SD0_CMDIDX = byCMDIndex | 0x40; // set cmd index

	R_SD0_CMDARG = dwArgument; // set argument
	
	R_SD0_CTRL1 |= BIT(BIT_CPCLR);  // clear cmd pend flag
	

	R_SD0_CADDR = (u32)sd0StaticBuffer;//0x42000;

	if ((0 == byCMDIndex) || (4 == byCMDIndex) || (15 == byCMDIndex)) //no response
	{
		u32Temp =  NO_RESP;
	}
	else if ((2 == byCMDIndex) || (9 == byCMDIndex) || (10 == byCMDIndex)) //R2 response
	{
		u32Temp = RESP_2;  //17 bytes, with check busy
	}
	else if (3 == byCMDIndex)
	{
		u32Temp = RESP_6;
	}
	else if ((7 == byCMDIndex) || (12 == byCMDIndex))
	{
		u32Temp = RESP_1B;
	}
	else if (41 == byCMDIndex)
	{
		u32Temp = RESP_3; 
	}
	else         
	{
		u32Temp = RESP_1;  //6 bytes, with check busy
	}
	
	//REG32(SDCON1) = u32Temp;
	R_SD0_CTRL1 = u32Temp;

//	i = 0; 
    ax32xx_timerTickStart();
	while(1)
	{
		if(R_SD0_CTRL0 & BIT(BIT_CPND))
		{
			ret = 1;
			break;
		}
		/*else if((++i) == 0xffffff)
		{
			ret = -1;
			break;
		}*/

		//if(ax32xx_timerTickCount()>=0x28ffffff)
		if(ax32xx_timerTickCount() >= APB_CLK*5)
		{
			ret = -1;
			break;
		}
	}
	ax32xx_timerTickStop();
	R_SD0_CTRL1 |= BIT(BIT_CPCLR);
//	if(ret<0)
//		debg("ax32xx_sd0SendCmd : %d\n",i);
	return ret;
}
/*******************************************************************************
* Function Name  : ax32xx_sd0BusSet
* Description    : set sd0 bus width 
* Input          : u8 type : SD_BUS_WIDTH4: 4-bit,SD_BUS_WIDTH1-1bit
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0BusSet(u8 type)
{
	if (type==SD_BUS_WIDTH4)
		R_SD0_CTRL0 |= 1<<BIT_DW4;
	else
		R_SD0_CTRL0 &= ~(1<<BIT_DW4);			
}

/*******************************************************************************
* Function Name  : ax32xx_sd0ClkSet
* Description    : set sd0 clock
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
BOOL ax32xx_sdSetFunc(void);

void ax32xx_sd0ClkSet(u32 hs_clk,u32 ns_clk)
{
	
	R_SD0_CTRL0 &= ~(1<<BIT_SDCKE);

	if(ax32xx_sdSetFunc())
	{
		R_SD0_CTRL0 |= BIT(31);         ////early clk ;H_freq (sdcon0_bit31_1)
		R_SD0_CTRL0 |= (1<<BIT_ORISE);	//out rise	
		R_SD0_BAUDRATE = APB_CLK/hs_clk -1;//(APB_CLK/SD_HS_CLK) - 1;		
		debg("SD_HS_CLK:%dHz\n",hs_clk);
	}
	else
	{
		R_SD0_CTRL0 &= ~(1<<BIT_ORISE);	//out fall	//L_freq (sdcon0_bit31_0)
		R_SD0_BAUDRATE = APB_CLK/ns_clk -1;//(APB_CLK/SD_HS_CLK) - 1;
		debg("SD_NS_CLK:%dHz\n",ns_clk);
	}		
}
/*******************************************************************************
* Function Name  : ax32xx_sd0Recv
* Description    : sd0 set recv data
* Input          : u32 addr : buffer address
                      u32 len   : length
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0Recv(u32 addr,u32 len)
{
	R_SDO_DADDR = addr;
	R_SD0_CTRL1 |= (1<<BIT_DPCLR);
	R_SD0_COUNT = len;	
	R_SD0_CTRL1 |= (1<<BIT_DKST);
	R_SD0_CTRL0 &= ~(1<<BIT_SDCKE); 	

}
/*******************************************************************************
* Function Name  : ax32xx_sd0Send
* Description    : sd0 set send data
* Input          : u32 addr : buffer address
                      u32 len   : length
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_sd0Send(u32 addr,u32 len)
{
	R_SDO_DADDR   = addr;
	R_SD0_CTRL1 |= BIT(BIT_DPCLR);
	R_SD0_COUNT  = len;
	R_SD0_CTRL1 |= BIT(BIT_DKST) | BIT(BIT_DKST+1);	
}
/*******************************************************************************
* Function Name  : ax32xx_sd0WaitDAT0
* Description    : sd0 wait data 0 heigh
* Input          : 
* Output         : None
* Return         : int 1: success
                          -1: fail
*******************************************************************************/
s32 ax32xx_sd0WaitDAT0(void)
{
   s32 ret = -1;
 //  volatile u32 i;

	R_SD0_CTRL0 |= BIT(BIT_SDCKE);

//	i=0;
	ax32xx_timerTickStart();
    while(1)
	{
    	if (R_SD0_CTRL0&(1<<BIT_BUSY))
		{
			ret = 1;
			break;
        }
		/*if((++i) == 0x00ffffff)
		{
			ret = -1;
			break;
		}	*/
		//if(ax32xx_timerTickCount() >= 0x8fffffff)
		if(ax32xx_timerTickCount() >= APB_CLK*5)
		{
			ret = -1;
			break;
		}
		//ax32xx_wdtClear();
    }
	ax32xx_timerTickStop();
	R_SD0_CTRL0 &= ~(1<<BIT_SDCKE);
//	if(ret<0)
//		debg("ax32xx_sd0WaitDAT0 : %d\n",i);
	return ret;	
}
/*******************************************************************************
* Function Name  : ax32xx_sd0WaitPend
* Description    : sd0 wait pending flag
* Input          : 
* Output         : None
* Return         : int 1: success
                          -1: fail
*******************************************************************************/
s32 ax32xx_sd0WaitPend(void)
{
	s32 ret = -1;
//    volatile u32 i;
	
//	i = 0;
    ax32xx_timerTickStart();
	while(1)
	{
		if ((R_SD0_CTRL0&(1<<BIT_DPND)))
		{ 
			ret = 1;
			break;
		}
		/*if((++i) == 0x00ffffff)
		{
			ret = -1;
			break;
		}	*/
		
		//if(ax32xx_timerTickCount() >= 0x28ffffff)
		if(ax32xx_timerTickCount() >= APB_CLK*5)
		{
			ret = -1;
			break;
		}
	}
	ax32xx_timerTickStop();
	R_SD0_CTRL1 |= (1<<BIT_DPCLR);
//	if(ret<0)
//		debg("ax32xx_sd0WaitPend : %d\n",i);
	return ret;	
}
/*******************************************************************************
* Function Name  : ax32xx_sd0GetRsp
* Description    : sd0 get rsp resoult
* Input          : 
* Output         : None
* Return         : int rsp resoult
*******************************************************************************/
s32 ax32xx_sd0GetRsp(void)
{
	return R_SD0_CMDARG;
}
/*******************************************************************************
* Function Name  : ax32xx_sdCRCCheck
* Description    : sd crc check
* Input          : u8 mode : 1-write,0-read
* Output         : None
* Return         : int rsp resoult
*******************************************************************************/
s32 ax32xx_sdCRCCheck(u8 mode)
{
	if(mode)
	{
		if((R_SD0_CTRL0&0x600000)==0x400000)
			return 1;
	}
	else
	{
		if(!(R_SD0_CTRL0&(1<<16)))//DCRCE
			return 1;
	}
	return -1;
}
/*******************************************************************************
* Function Name  : ax32xx_sd0Buffer
* Description    : get sd0 static buffer
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
u8 *ax32xx_sd0Buffer(void)
{
	return sd0StaticBuffer;
}
/*******************************************************************************
* Function Name  : ax32xx_sdSetFunc
* Description    : set sd  FUNC
* Input          : ax32xx_sd0Recv,ax32xx_sd0Send,sd0StaticBuffer,ax32xx_sd0SendCmd
* Output         : None ax32xx_sd0WaitPend
* Return         : none
*******************************************************************************/
 BOOL ax32xx_sdSetFunc(void)
 {
	 //check func
	 ax32xx_sd0Recv((u32)sd0StaticBuffer,64);
	 if(!ax32xx_sd0SendCmd(6,0x00ffff01))
		return FALSE;
	if (ax32xx_sd0WaitPend() != 1)
		return FALSE;
	if (REG32(SDCON0) & BIT(11))
		return FALSE;
		//debgbuf(sd0StaticBuffer,64);
	if((sd0StaticBuffer[16] & 0x0f) != 0x01){  //switch or not
		debg("H_speed_switch_err");
		return FALSE;}
		
	//set func	
	ax32xx_sd0Recv((u32)sd0StaticBuffer , 64);
	if(!ax32xx_sd0SendCmd(6,0x80ffff01))
		return FALSE;
	if (ax32xx_sd0WaitPend()!= 1)
		return FALSE;
	if (REG32(SDCON0) & BIT(11))
		return FALSE;	
	if((sd0StaticBuffer[16] & 0x0f) != 0x01){   //set or not
		debg("H_speed_set_err");
		return FALSE;}		
	//debgbuf(sd0StaticBuffer,64);
	
	return TRUE;			
}