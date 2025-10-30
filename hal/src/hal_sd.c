/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                   THE APPOTECH HAL
   **** **                 SD HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_sd.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN SD HAL LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"
#include "../xos/xos.h"

static SDC_STAT_T halSDCState;


/*******************************************************************************
* Function Name  : hal_sdGetVersion
* Description    : hal layer. get sd version
* Input          : none
* Output         : None
* Return         : int : 1 success
                            <0 fail
*******************************************************************************/
static s32 hal_sdGetVersion(void)
{
	int i;
	u8 factor;

	factor = 100;
	halSDCState.dwRCA = 0;
	
    halSDCState.eVer = SDC_TYPE_NULL;	
	for (i=0; i<10; i++)
	{
		ax32xx_sd0SendCmd(0, 0);
		//ax32xx_sysCpuMsDelay(200);		
		if((ax32xx_sd0SendCmd(8, 0x00000155)<0) || (0x00000155 != ax32xx_sd0GetRsp()))
			continue;
		
		goto SD_VER2;
	}
    
	//sd1.0
	for (i=0; i<20*factor; i++)//2000
	{
		if((ax32xx_sd0SendCmd(55, halSDCState.dwRCA)<0) || (ax32xx_sd0SendCmd(41, 0x00ff8000)<0))
		{
			//debg("HAL : [SD] <ERROR> 0\n");
			goto SD_VER_END;
		}

		if((ax32xx_sd0GetRsp() & 0x00ff8000) != 0x00ff8000)
		{
			//debg("HAL : [SD] <ERROR> 1\n");
		     goto SD_VER_END;	
		}
		if (ax32xx_sd0GetRsp() & 0x80000000)
		{
			debg("HAL : [SD] <INFO> v1.0\n");
			halSDCState.eVer = SDC_TYPE_10;
			//deg_Printf("sd 1.0\n");
			goto SD_VER_END;
		}
	}

	//mmc
	for (i=0; i<20*factor; i++)//2000
	{
		if (ax32xx_sd0SendCmd(1, 0x00ff8000)<0)
		{
			continue;
		}
		if (ax32xx_sd0GetRsp() & 0x80000000)
		{
			//deg_Printf("sd mmc\n");
			halSDCState.eVer = SDC_TYPE_MMC;
			debg("HAL : [SD] <INFO> mmc\n");
			goto SD_VER_END;
		}
	}

	//no support
	halSDCState.eVer = SDC_TYPE_NULL;
	debg("HAL : [SD] <ERROR> not support\n");
	return -1;

	//sd2.0
SD_VER2:
	for (i=0; i<5000*factor; i++)//10000
	{
		if((ax32xx_sd0SendCmd(55, halSDCState.dwRCA)<0) || (ax32xx_sd0SendCmd(41, 0x40ff8000)<0))
		{
			//debg("HAL : [SD] <ERROR> 2\n");
			goto SD_VER_END;
		}
		if((ax32xx_sd0GetRsp() & 0x00ff8000) != 0x00ff8000)
        {
			//debg("HAL : [SD] <ERROR> 3\n");
			goto SD_VER_END;
		}
		if (ax32xx_sd0GetRsp() & 0x80000000)
			break;
	}

	if (0xc0000000 == (ax32xx_sd0GetRsp() & 0xc0000000))
	{
		halSDCState.eVer = SDC_TYPE_20_HC;
		debg("HAL : [SD] <INFO> HC20\n");
		goto SD_VER_END;
	}
	else if (0x80000000 == (ax32xx_sd0GetRsp() & 0xc0000000))
	{
		halSDCState.eVer = SDC_TYPE_20_NC;
		debg("HAL : [SD] <INFO> NC20\n");
		goto SD_VER_END;
	}
	else
	{
		debg("HAL : [SD] <ERROR> unknow\n");
		halSDCState.eVer = SDC_TYPE_NULL;
	}
SD_VER_END:
	if(halSDCState.eVer == SDC_TYPE_NULL)
		return -1;

	return 0;
}
/*******************************************************************************
* Function Name  : hal_sdGetCapacity
* Description    : hal layer. get sdc capacity
* Input          : none
* Output         : None
* Return         : sd capacity
*******************************************************************************/
static u32 hal_sdGetCapacity(void)
{
	u8  rdBlkLen, cSizeMult,*buf;
	u16 cSize; 
	u32 dwSDCap = 0;

    buf = ax32xx_sd0Buffer();
	if(halSDCState.eVer == SDC_TYPE_20_HC)
	{
	   	cSize = ((u16)buf[11] << 8) + buf[10];
		dwSDCap = (((u32)(buf[4] & 0x3f) << 16) + (u32)cSize + 1) << 10;
	}
	else
	{
		rdBlkLen = buf[6] & 0x0f;
		cSize = (((u32)buf[5] & 0x03) << 10) + ((u32)buf[4] << 2) + (((u32)buf[11] & 0xc0) >> 6);		
		cSizeMult = ((buf[10] & 0x03) << 1) | (buf[9] >> 7);
		dwSDCap  = ((u32)(cSize + 1)) << (cSizeMult  + rdBlkLen - 7); 
		
	}
	return dwSDCap;

}
/*******************************************************************************
* Function Name  : hal_sdTransferCheck
* Description    : hal layer. transfer state check
* Input          : none
* Output         : None
* Return         : int 1 : ok
                           <0 : fail
*******************************************************************************/
static s32 hal_sdTransferCheck(void)
{
	s32 ret = -1;
	volatile int i;

	i = 0x00ffffff;
	while(1)
	{
		if(ax32xx_sd0SendCmd(13, halSDCState.dwRCA)>=0)
		{
			u32 rps = ax32xx_sd0GetRsp();
			if (0x800 == (0x1E00 & rps))
			{
				halSDCState.eCardState = SDC_STATE_FREE;
				ret = 0;
				break;
			}
			if((rps == 0x00) || (rps == -1))
			{
				ret = -1;
				break;				
			}
		}

		if((--i)==0)
		{
			ret = -1;
			break;
		}

	}

	return ret;
}

#define  R_SD0_CTRL0        SDCON0
#define  R_SD0_CTRL1        SDCON1
#define  R_SD0_BAUDRATE     SDBAUD
#define  R_SD0_CMDIDX       SDIDX
#define  R_SD0_CMDARG       SDARG
#define  R_SD0_CADDR        SDCADR
#define  R_SD0_COUNT        SDDCNT
#define  R_SDO_DADDR        SDDADR

#define BIT_SPCRCSTA 5
#define BIT_8CKE    4
#define BIT_ORISE   3
#define BIT_DW4     2
#define BIT_SDCKE   1
#define BIT_SDE     0

#define BIT(x)     	            (1UL<<(x))

void hal_ClkSet1(u32 hs_clk,u32 ns_clk,int high)
{
	
	R_SD0_CTRL0 &= ~(1<<BIT_SDCKE);

	if(high)//(ax32xx_sdSetFunc())
	{
		R_SD0_CTRL0 |= BIT(31);         ////early clk ;H_freq (sdcon0_bit31_1)
		R_SD0_CTRL0 |= (1<<BIT_ORISE);	//out rise	
		R_SD0_BAUDRATE = APB_CLK/hs_clk -1;//(APB_CLK/SD_HS_CLK) - 1;		
		//debg("SD_HS_CLK:%dHz\n",hs_clk);
	}
	else
	{
		R_SD0_CTRL0 &= ~BIT(31); 
		R_SD0_CTRL0 &= ~(1<<BIT_ORISE);	//out fall	//L_freq (sdcon0_bit31_0)
		R_SD0_BAUDRATE = APB_CLK/ns_clk -1;//(APB_CLK/SD_HS_CLK) - 1;
		//debg("SD_NS_CLK:%dHz\n",ns_clk);
	}		
}

static s32 hal_sdTransferCheck_idle(void)
{
	s32 ret = -1;
	volatile int i;
	i = 0x01;
#if 1

	unsigned int static check_count = -1;
	unsigned int static last_check = -1;		



#if 1
	if((check_count == -1)||
			((XOSTimeGet()>check_count)&&((XOSTimeGet()- check_count)>500))||
			((XOSTimeGet()<check_count)&&((XOSTimeGet()+(0xffffffff-check_count))>500))
			)
		{
			check_count = XOSTimeGet();
		}
		else
		{
			return last_check;
		}

#endif
	hal_ClkSet1(600000, 600000,0);
	while(1)
	{
		if(ax32xx_sd0SendCmd(13, halSDCState.dwRCA)>=0)
		{
			u32 rps = ax32xx_sd0GetRsp();
			//uart_Printf("rps is %d\n",rps);
			if (0x800 == (0x1E00 & rps))
			{
				halSDCState.eCardState = SDC_STATE_FREE;
				ret = 0;
				break;
			}
			if((rps == 0x00) || (rps == -1))
			{
				ret = -1;
				break;				
			}
		}

		if((--i)==0)
		{
			//uart_Printf("ret11 is %d\n",ret);
			ret = -1;
			break;
		}

	}
	//uart_Printf("ret is %d\n",ret);
	hal_ClkSet1(SD_HS_CLK,SD_NS_CLK,1);
	last_check = ret;
	#else
	s32 hal_sdCheck1(u8 sd_bus_width);

	ret=hal_sdCheck1(SD_BUS_WIDTH1);		

	#endif
	//ax32xx_sd0ClkSet(SD_HS_CLK,SD_NS_CLK);
	

	return ret;
}

/*******************************************************************************
* Function Name  : hal_sdStop
* Description    : hal layer. sdc stop tranfer
* Input          : none
* Output         : None
* Return         : int 1 : ok
                           <0 : fail
*******************************************************************************/
s32 hal_sdStop(void)
{
	if (SDC_STATE_FREE != halSDCState.eCardState)
	{
		if ((ax32xx_sd0SendCmd(12, 0)<0) || (ax32xx_sd0WaitDAT0()<0) || (hal_sdTransferCheck()<0))
		{
			return -1;
		}
		halSDCState.eCardState = SDC_STATE_FREE;
	}
	return 0;
}
/*******************************************************************************
* Function Name  : hal_sdUpdate
* Description    : hal layer. sdc update
* Input          : void *pDataBuf : data buffer
                      u32 dwLBA : block index
                      u32 dwLBANum : block num
                      u8 bRWMode : 1-write,0-read
* Output         : None
* Return         : int 1 : ok
                           <0 : fail
*******************************************************************************/
#if 1

static u8 alignBuffer[512] ALIGNED(16);


static s32 hal_sdUpdate(void *pDataBuf, u32 dwLBA, u32 dwLBANum, u8 bRWMode)
{
	//hal_wdtClear();
	int align_flag=1;
	char *in_buf=NULL;

	if((int)pDataBuf%16)
		align_flag=0;

	if(align_flag == 0)
	{
		memcpy(alignBuffer,pDataBuf,512);
		in_buf=pDataBuf;
		pDataBuf = alignBuffer;
	}
	(bRWMode == 1)?ax32xx_sysDcacheWback((u32)pDataBuf,512) : ax32xx_sysDcacheFlush((u32)pDataBuf,512);  // data cache
	
	if (0 == dwLBANum)
		return -1;

	if ((dwLBA + dwLBANum) > halSDCState.dwCap)
		return -2;
	
	if (((bRWMode?SDC_STATE_WRITE: SDC_STATE_READ) != halSDCState.eCardState) || (halSDCState.dwNextLBA != dwLBA))
	{
		if(hal_sdStop()<0)
			return -3;
		halSDCState.eCardState = bRWMode ? SDC_STATE_WRITE : SDC_STATE_READ;
		//deg_Printf("%x,%x,%x\n",bRWMode,dwLBA,halSDCState.dwNextLBA);

		

		if(!bRWMode)
			ax32xx_sd0Recv((u32)pDataBuf , 512);
		if(ax32xx_sd0SendCmd(bRWMode ? 25 : 18, (SDC_TYPE_20_HC==halSDCState.eVer) ? dwLBA : (dwLBA<<9))<0)		//multiple write/read
		    return -4;
		
		if(bRWMode)
			ax32xx_sd0Send((u32)pDataBuf, 512);
	}
	else
	{
		if(bRWMode)
			ax32xx_sd0Send((u32)pDataBuf , 512);
		else
			ax32xx_sd0Recv((u32)pDataBuf , 512);	
	}

	if (ax32xx_sd0WaitPend()<0)	{		
		deg_Printf("waitPend\n");
		return -5;
	}
		
	if(ax32xx_sdCRCCheck(bRWMode)<0){
		deg_Printf("SD -CRC err\n");		
		return -6;
	}


	if(align_flag == 0)
	{
		if(bRWMode == 0 && in_buf != NULL)
			memcpy(in_buf,pDataBuf,512);
	}
	halSDCState.dwNextLBA = dwLBA + dwLBANum;
	

	return 0;
}
#else
static s32 hal_sdUpdate(void *pDataBuf, u32 dwLBA, u32 dwLBANum, u8 bRWMode)
{
	hal_wdtClear();
	(bRWMode == 1)?ax32xx_sysDcacheWback((u32)pDataBuf,512) : ax32xx_sysDcacheFlush((u32)pDataBuf,512);  // data cache
	
	if (0 == dwLBANum)
		return -1;

	if ((dwLBA + dwLBANum) > halSDCState.dwCap)
		return -2;
	
	if (((bRWMode?SDC_STATE_WRITE: SDC_STATE_READ) != halSDCState.eCardState) || (halSDCState.dwNextLBA != dwLBA))
	{
		if(hal_sdStop()<0)
			return -3;
		halSDCState.eCardState = bRWMode ? SDC_STATE_WRITE : SDC_STATE_READ;
		//deg_Printf("%x,%x,%x\n",bRWMode,dwLBA,halSDCState.dwNextLBA);
		
		if(!bRWMode)
			ax32xx_sd0Recv((u32)pDataBuf , 512);
		if(ax32xx_sd0SendCmd(bRWMode ? 25 : 18, (SDC_TYPE_20_HC==halSDCState.eVer) ? dwLBA : (dwLBA<<9))<0)		//multiple write/read
		    return -4;
		
		if(bRWMode)
			ax32xx_sd0Send((u32)pDataBuf, 512);
	}
	else
	{
		if(bRWMode)
			ax32xx_sd0Send((u32)pDataBuf , 512);
		else
			ax32xx_sd0Recv((u32)pDataBuf , 512);	
	}

	if (ax32xx_sd0WaitPend()<0)	{		
		deg_Printf("waitPend\n");
		return -5;
	}
		
	if(ax32xx_sdCRCCheck(bRWMode)<0){
		deg_Printf("SD -CRC err\n");		
		return -6;
	}
	halSDCState.dwNextLBA = dwLBA + dwLBANum;
	

	return 0;
}

#endif

s32 hal_sdNextLBA(void)
{
	return halSDCState.dwNextLBA ;
}
/*******************************************************************************
* Function Name  : hal_sdInit
* Description    : hal layer. initial sd
* Input          : sd_bus_width:   SD_BUS_WIDTH1 or SD_BUS_WIDTH4 
* Output         : None
* Return         : none
*******************************************************************************/
s32 hal_sdInit(u8 sd_bus_width)
{
	if(halSDCState.eCardState != SDC_STATE_NULL)
		return 0;		
	
	memset(&halSDCState,0,sizeof(SDC_STAT_T));
	
	ax32xx_sd0Init(sd_bus_width);
	
	ax32xx_sd0SendCmd(12, 0);
	
    ax32xx_sd0SendCmd(0,0);
    // delay 10ms

//----------get sd version,1.0,2.0,mmc
	if (hal_sdGetVersion()<0) //CMD8 
		return -1;


//----------get all cid
	if (ax32xx_sd0SendCmd(2,0)<0) //CMD2
		return -2;

//-----------get rca
	if(halSDCState.eVer==SDC_TYPE_MMC)  // mmc type
	{
		halSDCState.dwRCA = 1;

        if (ax32xx_sd0SendCmd(3, halSDCState.dwRCA)<0)
			return -3;	
	}
	else // sd1.0,sd2.0
	{		
		if (ax32xx_sd0SendCmd(3, 0)>=0)
			halSDCState.dwRCA = ax32xx_sd0GetRsp();
		else
			return -4;
	}
//----------get csd
	if(ax32xx_sd0SendCmd(9, halSDCState.dwRCA)<0)
		return -5;

//----------get sd cap
    halSDCState.dwCap = hal_sdGetCapacity();
	if (halSDCState.dwCap == 0)
	    return -6;
	debg("sd capacity : %d\n",halSDCState.dwCap);
//----------enter transfer state
    if(ax32xx_sd0SendCmd(7, halSDCState.dwRCA)<0)
		return -7;
//----------set block len
    if(ax32xx_sd0SendCmd(16, 512)<0)
        return -8;

	//set bus width
	if (halSDCState.eVer!=SDC_TYPE_MMC)
	{
		if(SD_BUS_WIDTH4 == sd_bus_width)
		{
			if((ax32xx_sd0SendCmd(55, halSDCState.dwRCA)<0) || (ax32xx_sd0SendCmd(6, 2)<0))
				return -9;
			ax32xx_sd0BusSet(SD_BUS_WIDTH4);
			halSDCState.bus_width = SD_BUS_WIDTH4;
			deg_Printf("HAL : <SD> sd 4line\n");
		}
		else
		{
			if((ax32xx_sd0SendCmd(55, halSDCState.dwRCA)<0) || (ax32xx_sd0SendCmd(6, 0)<0))
				return -9;
			ax32xx_sd0BusSet(SD_BUS_WIDTH1);
			halSDCState.bus_width = SD_BUS_WIDTH1;
			deg_Printf("HAL : <SD> sd 1line\n");
		}
	}
	ax32xx_sd0ClkSet(SD_HS_CLK,SD_NS_CLK);	// set sd clk speed 
	deg_Printf("HAL : <SD> HS_CLK=%d,NS_CLK=%d\n",SD_HS_CLK,SD_NS_CLK);
	halSDCState.eCardState = SDC_STATE_FREE;


	return 0;	
}
/*******************************************************************************
* Function Name  : hal_sdUninit
* Description    : hal layer. uninitial sd
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_sdUninit(void)
{
	if(hal_sdStop()>=0)
	ax32xx_sd0Uninit();	
}
/*******************************************************************************
* Function Name  : hal_sdLock
* Description    : hal layer.lock sdc state
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_sdLock(void)
{
	if(halSDCState.eCardState)
		halSDCState.busy = 1;
	else
		halSDCState.busy = 0;	
}
/*******************************************************************************
* Function Name  : hal_sdUnlock
* Description    : hal layer. unlock sdc state
* Input          : none
* Output         : None
* Return         :none
*******************************************************************************/
void hal_sdUnlock(void)
{
	halSDCState.busy = 0;	
}
/*******************************************************************************
* Function Name  : hal_sdCheck
* Description    : hal layer.sd check if there is sdc
* Input          : none
* Output         : None
* Return         : int 1 : sd
                           0 : no sd card
*******************************************************************************/
s32 hal_sdCheck(void)
{
	if(halSDCState.eCardState == SDC_STATE_NULL)
		return 0;
		
	if(halSDCState.busy)
		return 1;

	if(SDC_STATE_FREE != halSDCState.eCardState)
		hal_sdStop();

	if(hal_sdTransferCheck_idle()>=0)
		return 1;
	
	halSDCState.eCardState = SDC_STATE_NULL;
	
	return 0;
}

/*******************************************************************************
* Function Name  : u8 hal_sdSetCardState(u8 state)
* Description    : hal layer. set sdc state
* Input          : state
* Output         : none
* Return         : none
*******************************************************************************/
void hal_sdSetCardState(u8 state)
{
	halSDCState.eCardState = state;
}
/*******************************************************************************
* Function Name  : u8 hal_sdGetCardState(u8 state)
* Description    : hal layer. get sdc state
* Input          : state
* Output         : none
* Return         : none
*******************************************************************************/
u8 hal_sdGetCardState(void)
{
	return halSDCState.eCardState;
}
/*******************************************************************************
* Function Name  : hal_sdGetBusWidth()
* Description    : hal layer. get sdc bus width
* Input          : none
* Output         : none
* Return         : sd bus width
*******************************************************************************/
u8 hal_sdGetBusWidth(void)
{
	return halSDCState.bus_width;
}

/*******************************************************************************
* Function Name  : hal_sdGetCapacity
* Description    : hal layer. get sdc capacity
* Input          : none
* Output         : None
* Return         : sd capacity
*******************************************************************************/
u32 hal_sdCapacity(void)
{
	return halSDCState.dwCap;
}
/*******************************************************************************
* Function Name  : hal_sdExist
* Description    : hal layer.sd check exist
* Input          : none
* Output         : None
* Return         : int 1 : sd
                           0 : no sd card
*******************************************************************************/
s32 hal_sdExist(void)
{
	if(halSDCState.eCardState == SDC_STATE_NULL)
		return 0;
	return 1;
}
/*******************************************************************************
* Function Name  : hal_sdWrite
* Description    : hal layer.sd write
* Input          : void *pDataBuf : data buffer
                      u32 dwLBA : block index
                      u32 dwLBANum : block num
* Output         : None
* Return         : int 1 : ok
                           0 : fail
*******************************************************************************/
volatile s32 speed;
s32 hal_sdWrite(void *pDataBuf, u32 dwLBA, u32 dwLBANum)
{

	u32 i;
	s32 ret;
	
	for(i = 0;i < dwLBANum; i++)
	{
		ret = hal_sdUpdate(((u8 *)pDataBuf + 512 * i), dwLBA + i, 1, 1);
		if(ret<0)
		{	
			halSDCState.eCardState = SDC_STATE_ERROR;
			//memset(&halSDCState,0,sizeof(SDC_STAT_T));
			debg("HAL : <SD>[ERROR] write .%d,sector no :%d,%x\n",ret,dwLBA,pDataBuf);
			//halSDCState.eCardState = SDC_STATE_NULL;
			sysLog("sd write fail");
			return ret;
		}
	}
	 speed+=dwLBANum;
	return 0;
}
void hal_speed_callback(void)
{
	s32 jspeed=0;
	jspeed=speed;
	speed=0;
	deg_Printf("speed:%dkb/s\n",jspeed/2);
	(void)jspeed;
}
/*******************************************************************************
* Function Name  : hal_sdRead
* Description    : hal layer.sd cread
* Input          : void *pDataBuf : data buffer
                      u32 dwLBA : block index
                      u32 dwLBANum : block num
* Output         : None
* Return         : int 1 : ok
                           0 : fail
*******************************************************************************/
s32 hal_sdRead(void *pDataBuf, u32 dwLBA, u32 dwLBANum)
{
	u32 i;
	s32 ret;
	for( i = 0;i < dwLBANum; i++)
	{
		ret = hal_sdUpdate((pDataBuf + 512 * i), dwLBA + i, 1, 0);
		if(ret<0)
		{
			halSDCState.eCardState = SDC_STATE_ERROR;
			//memset(&halSDCState,0,sizeof(SDC_STAT_T));
			debg("HAL : <SD>[ERROR] read .%d,sector no :%d,%x\n",ret,dwLBA,pDataBuf);
			sysLog("sd read fail");
			//halSDCState.eCardState = SDC_STATE_NULL;
			return ret;
		}
	}

	return 0;
}
