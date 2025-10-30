/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX SYSTEM
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_sys.c
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


#define  R_SYS_CLK      PCON0
#define  R_SYS_CLK1     PCON1
#define  R_SYS_LDO      LDOCON
#define  R_SYS_RST      RSTCON

#ifndef __ASSEMBLER__
extern u32 __bss_start,__bss_end ;
extern u32 __sram_start,__sram_end ;
#endif

extern int _sdram_remian_addr;
extern int __sdram_remain_size;
volatile u8 smph_dmacopy;
/*******************************************************************************
* Function Name  : ax32xx_sysDelay
* Description    : system delay ms
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_sysCpuMsDelay(int dtime)
{
	volatile int cnt=dtime*(APB_CLK / 1000 / 9);
	while(cnt--)
	{
		asm("l.nop");
	}
}
/*******************************************************************************
* Function Name  : ax32xx_sysCpuNopDelay
* Description    : system delay nop
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_sysCpuNopDelay(int dtime)
{
	while(dtime--)
	{
		asm("l.nop");
	}
}
/*******************************************************************************
* Function Name  : ax32xx_sysDcacheInit
* Description    : dcache initial
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_sysDcacheInit(void) 
{
#if CFG_DCACHE_EN
	__sprclr(SPR_DCCR, 0);
    SPR_DCAIR = 0x2000000;
	__sprset(SPR_DCCR, 0);
#else
	__sprclr(SPR_DCCR, 0);
#endif
}    
/*******************************************************************************
* Function Name  : ax32xx_sysIcacheInit
* Description    : icache initial
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_sysIcacheInit(void)
{
#if CFG_ICACHE_EN
	u32 cnt=0;
	__sprclr(SPR_ICCR, 0);
	// invalidate 256 lines
	{
		//mtspr(SPR_ICBIR, cnt << 4);
		mtspr(SPR_ICAIR, cnt << 4);
	}
	__sprset(SPR_ICCR, 0);
	//debg("I-cache open\n");
#else
	__sprclr(SPR_ICCR, 0);
#endif	
}
 /*******************************************************************************
* Function Name  : ax32xx_sysDcacheWback
* Description    : data cache write back
* Input          : u32 addr : address
*                  u32 size : size
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysDcacheWback(u32 addr, u32 size)  
{ 
#if CFG_DCACHE_EN	
	if(addr & BIT(31))
		return;
	if(addr < 0x44000)
		return;	

	while(size)
	{
		u32 wb_size = min(size,(4*1024));
		u32 dccnt = ((addr + wb_size -1) - (addr & ~0x0f)) / 16;
		SPR_DCCNT = dccnt; 
		SPR_DCBWR = addr;
		addr += wb_size;
		size -= wb_size;
	}

#endif	
} 
 /*******************************************************************************
* Function Name  : ax32xx_sysDcacheFlush
* Description    : data cache flush
* Input          : u32 addr : address
*                  u32 size : size
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysDcacheFlush(u32 addr, u32 size) 
{ 
#if CFG_DCACHE_EN		
	if(addr & BIT(31))
		return;
	if(addr < 0x44000)
		return;
	
	while(size)
	{
		u32 flush_size = min(size,(4*1024));
		u32 dccnt = ((addr + flush_size -1) - (addr & ~0x0f)) / 16;
		SPR_DCCNT = dccnt;
		SPR_DCBFR = addr;
		addr += flush_size;
		size -= flush_size;
	}

#endif	
}
 /*******************************************************************************
* Function Name  : ax32xx_sysDcacheInvalid
* Description    : data cache Invalid
* Input          : u32 addr : address
*                  u32 size : size
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysDcacheInvalid(u32 addr, u32 size) 
{ 
#if CFG_DCACHE_EN		
	if(addr & BIT(31))
		return;
	if(addr < 0x44000)
		return;
	
	while(size)
	{
		u32 invald_size = min(size,(4*1024));
		u32 dccnt = ((addr + invald_size -1) - (addr & ~0x0f)) / 16;
		SPR_DCCNT = dccnt;
		SPR_DCBIR = addr;
		addr += invald_size;
		size -= invald_size;
	}

#endif	
}
/*******************************************************************************
* Function Name  : ax32xx_sysSRAMClear
* Description    : clear system sram
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysSRAMClear(void)
{
	u32 cnt,*dst;
	
	cnt = ((u32)(&__sram_end) - (u32)(&__sram_start))/4;
	dst = &__sram_start;
	while(cnt--)
	   *dst++ = 0;	
}
/*******************************************************************************
* Function Name  : ax32xx_sysBSSClear
* Description    : clear system bss phase
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysBSSClear(void)
{
	u32 cnt,*dst;
	
	cnt = ((u32)(&__bss_end) - (u32)(&__bss_start))/4;
	dst = &__bss_start;
	while(cnt--)
	   *dst++ = 0;	
    //write back all cache line
    SPR_DCAWR = 0xffffffff;
    
    ax32xx_sysSRAMClear();
}
/*******************************************************************************
* Function Name  : ax32xx_sysLDOSet
* Description    : set system ldo for power support
* Input          : u8 ldo : ldo type
*                  u8 sel : power sel
*                  u8 en  : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysLDOSet(u8 ldo,u8 sel,u8 en)
{
   switch(ldo)
   {
      case SYS_LDO_HSEN : 
		    if(en)
			{
				#if (0==SDRAM_PSRAM_SEL)
				if((CHIP_ID &0xffff0000) != THUNDERSE_A1)
				{
					if(sel>SYS_VOL_V3_1)    //USE VDDRTC, OPEN HERE
					{
						ax32xx_rtcSenHVEnable(1);
						R_SYS_LDO |=(1<<6); //enable vddsenio>3.1V,use vddrtc
					}
				}
				#endif
				//R_SYS_LDO = (R_SYS_LDO & ~0x07)|(sel&0x07)|(1<<3);
				R_SYS_LDO = (R_SYS_LDO & ~0x1f)|(sel&0x1f)|(1<<5);
			}
			else
			{
				#if (0==SDRAM_PSRAM_SEL)
				if((CHIP_ID &0xffff0000) != THUNDERSE_A1)
				{
					ax32xx_rtcSenHVEnable(0);//USE VDDRTC, OPEN HERE
					R_SYS_LDO &= ~(1<<6);    //USE VDDRTC, OPEN HERE
				}
				#endif
				//R_SYS_LDO &= ~(1<<3);
				R_SYS_LDO &= ~(1<<5);
				
			}
			break;
	  case SYS_LDO_LSEN :
	        if(en)
			{
				//R_SYS_LDO = (R_SYS_LDO & ~(0x07<<4))|((sel&0x07)<<4)|(1<<7);			
				R_SYS_LDO = (R_SYS_LDO & ~(0x1f<<7))|((sel&0x1f)<<7)|(1<<12);			
			}
			else
				R_SYS_LDO &= ~(1<<12);
			break;
	  case SYS_LDO_BGOE :
	  	    if(en)
            {
		   	    //R_SYS_LDO |= 1<<11;
				R_SYS_LDO |= 1<<15;
                //R_SYS_LDO  = (R_SYS_LDO & ~(0x07<<12))|((sel&0x07)<<12);
				R_SYS_LDO  = (R_SYS_LDO & ~(0x1f<<16))|((sel&0x1f)<<16);
            }
			else
				//R_SYS_LDO &= ~(1<<11);
				R_SYS_LDO &= ~(1<<15);
			break;
	  case SYS_LDO_DDR :
	  	     if(en)
			 	R_SYS_LDO |= 1<<10;//?
			 else
			 	R_SYS_LDO &= ~(1<<10);//?
			 break;
	  case SYS_LDO_V1_2 :
//			R_SYS_LDO = (R_SYS_LDO & ~(0x03<<8))|((sel&0x03)<<8);
			R_SYS_LDO = (R_SYS_LDO & ~(0x03<<13))|((sel&0x03)<<13);
            break;	
	  case SYS_LDO_SDC :
			if(en)
				//R_SYS_LDO |= 1<<15;
				R_SYS_LDO |= 1<<21;
			else
				//R_SYS_LDO &= ~(1<<15);
				R_SYS_LDO &= ~(1<<21);
			break;
   }
}
/*******************************************************************************
* Function Name  : ax32xx_sysClkSet
* Description    : system clk set
* Input          : u8 clk_type : clk type.SYS_CLK_E
*                  u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysClkSet(u8 clk_type,u8 en)
{
    u32 CLKSFR = (u32)&R_SYS_CLK;
    u8 dev_id = clk_type;
    
    if(clk_type > 31)
    {
        CLKSFR = (u32)&R_SYS_CLK1;
        dev_id = clk_type - 32;
    }
    
	if(en)
		SFR_AT CLKSFR &=~(1<<dev_id);
	else
		SFR_AT CLKSFR |= (1<<dev_id);
}
/*******************************************************************************
* Function Name  : ax32xx_sysReset
* Description    : system module reset
* Input          : u8 clk_type : clk type.SYS_RESET_E
*                  u8 en : 1-reset,0-normal
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysReset(u8 rst_type,u8 en)
{
	if(en)
		R_SYS_RST &= ~rst_type;
	else
		R_SYS_RST |= rst_type;
}
/*******************************************************************************
* Function Name  : dma_memcpy
* Description    : copy data from src to dst
* Input          : *dst: destination pointer
*                  *src: source pointer
*                  cnt :length
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sys_dma_memdown(void)
{
	while(smph_dmacopy > 0){
		if(MCP0CON & BIT(1)){
			smph_dmacopy=0;//before clear dma_copy pengding
			MCP0CON |= BIT(3);//clear dma_copy pengding
			break;
		}	
	}
}
/*******************************************************************************
* Function Name  : dma_memcpy
* Description    : copy data from src to dst
* Input          : *dst: destination pointer
*                  *src: source pointer
*                  cnt :length
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sys_dma_memkick(void *dst, void *src, int cnt)
{
    ax32xx_intCriticalInit();
	if(smph_dmacopy > 0)
	{
		//debg("!:%x",smph_dmacopy);
		//debg("#");
		ax32xx_sys_dma_memdown();
	}
	
	ax32xx_sysDcacheWback((u32)src,cnt);
	ax32xx_sysDcacheFlush((u32)dst,cnt);

	ax32xx_intCriticalEnter();
	MCP0CON = BIT(3);
	MCP0CON = BIT(0); 	//en
	
	MCP0SCADDR = (u32)src;
	MCP0TGADDR = (u32)dst;
	MCP0LENGTH = cnt-1;
	smph_dmacopy=1;
	ax32xx_intCriticalExit();
	//debg("s-%x, d-%x, n-%x \n",(u32)src,(u32)dst,cnt);
	
}

void ax32xx_mcpy_gram2ufifo(void *dst, void *src, int cnt)
{
	if(cnt == 0) return ;
	MCP2CON = BIT(3);
	MCP2CON = BIT(0); 	//en
	MCP2SCADDR = (u32)src;
	MCP2TGADDR = (u32)dst;
	MCP2LENGTH = cnt-1;
	while(1){
		if(MCP2CON & BIT(1)){
			MCP2CON |= BIT(3);//clear dma_copy pengding	
			break;
		}	
	}
}
void ax32xx_mcpy1_sdram2gram(void *dst, void *src, int cnt)
{
	ax32xx_sysDcacheWback((u32)src,cnt);
	ax32xx_sysDcacheFlush((u32)dst,cnt);
	
	if(cnt == 0) return ;
	MCP1CON = BIT(3);
	MCP1CON = BIT(0); 	//en
	MCP1SCADDR = (u32)src;
	MCP1TGADDR = (u32)dst;
	MCP1LENGTH = cnt-1;
	while(1){
		if(MCP1CON & BIT(1)){
			MCP1CON |= BIT(3);//clear dma_copy pengding	
			break;
		}	
	}	
}
void ax32xx_mcpy1_sdram2gram_2(void *dst, void *src, int cnt)
{
	//ax32xx_sysDcacheWback((u32)src,cnt);
	//ax32xx_sysDcacheFlush((u32)dst,cnt);
	
	if(cnt == 0) return ;
	MCP1CON = BIT(3);
	MCP1CON = BIT(0); 	//en
	MCP1SCADDR = (u32)src;
	MCP1TGADDR = (u32)dst;
	MCP1LENGTH = cnt-1;
	while(1){
		if(MCP1CON & BIT(1)){
			MCP1CON |= BIT(3);//clear dma_copy pengding	
			break;
		}	
	}	
}

/*******************************************************************************
* Function Name  : ax32xx_sysInit
* Description    : initial system for platform using.
* Input          : u32 *saddr : sdram free start address 
                      u32 *eaddr : sdram free end address
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysInit(u32 *saddr,u32 *eaddr)
{
	//debg("sys init\n");
	__LGIE_DIS__();
	__HGIE_DIS__();
    
	ax32xx_wdtClear();

//-----cache set
	ax32xx_sysIcacheInit();
	

	ax32xx_sysDcacheInit();	
//hal_dacSetVolume(0);
	ax32xx_dacVolumeSet(0);
//-----bss clear	
	ax32xx_sysBSSClear();

//-----system power
	//debg("R_SYS_LDO:%x\n",R_SYS_LDO);
    R_SYS_LDO |= (0<<21)|//VDDSD en15
				// (0x0f<<16)|//bandgap voltage select (3<<12)
				 (1<<15);//adkey bandgap enable11

	ax32xx_sysLDOSet(SYS_LDO_V1_2,SYS_VOL_V1_30,1);	 //VDDCORE is better to config before clktun
	ax32xx_sysCpuMsDelay(20);
	//ax32xx_sysLDOSet(SYS_LDO_LSEN,SYS_VOL_V1_5,1);
	LDOCON &= ~(1<<12);//นุต๔
	ax32xx_sysCpuMsDelay(20);
	ax32xx_sysLDOSet(SYS_LDO_HSEN,SYS_VOL_V3_0,1);
	

//-----system colck	
    ax32xx_sysClkSet(SYS_CLK_DVP_CSI,1);
	ax32xx_sysClkSet(SYS_CLK_JPGA,1);
	ax32xx_sysClkSet(SYS_CLK_JPGB,1);
	ax32xx_sysClkSet(SYS_CLK_RTC,1);
    //ax32xx_usbPhyPllEnable();
 	fp_usb_isr = ax32xx_usbIRQHanlder;

	
	
//----INT initial
	ax32xx_intInit();
    if(saddr)
		*saddr = (u32)&_sdram_remian_addr;//0x58400000; // from 1M_sdram_remian_addr;//
	if(eaddr)
		*eaddr = ((u32)&_sdram_remian_addr)+((u32)&__sdram_remain_size);//(0x58000000+SDRAM_SIZE); // end address

	uart_Printf("sdram *saddr=0x%x,*eaddr=0x%x\n",*saddr,*eaddr);
	
    ax32xx_dmaNocDefault();
    ax32xx_wdtEnable(1);

	__LGIE_EN__();
	__HGIE_EN__();
}
/*******************************************************************************
* Function Name  : ax32xx_sysUninit
* Description    : uninitial system .
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_sysUninit(void)
{
//	ax32xx_sysLDOSet(SYS_LDO_BGOE,0,0);	
	ax32xx_sysLDOSet(SYS_LDO_LSEN,0,0);	
	ax32xx_sysLDOSet(SYS_LDO_HSEN,SYS_VOL_V3_1,0);	
//-----system colck	
    ax32xx_sysClkSet(SYS_CLK_DVP_CSI,0);
	ax32xx_sysClkSet(SYS_CLK_JPGA,0);
	ax32xx_sysClkSet(SYS_CLK_JPGB,0);
    ax32xx_sysClkSet(SYS_CLK_LCDC,0);
    ax32xx_sysClkSet(SYS_CLK_ADC,0);
	ax32xx_sysClkSet(SYS_CLK_AUADC,0);
	ax32xx_sysClkSet(SYS_CLK_DAC,0);
	ax32xx_sysClkSet(SYS_CLK_DE,0);
//	ax32xx_sysClkSet(SYS_CLK_IIC0,0);
//	ax32xx_sysClkSet(SYS_CLK_IIC1,0);
	ax32xx_sysClkSet(SYS_CLK_SD0,0);
	ax32xx_sysClkSet(SYS_CLK_SPI0,0);
}

