/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         AUDAC DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_audac.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AUDIO DAC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"

#define  R_SYS_CLK1     CLKCON1
#define  R_SYS_CLK0     CLKCON0
//#define  R_SYS_CTRL0    PCON0

#define  R_DAC_CTRL0    DACACON0
#define  R_DAC_CTRL1    DACACON1
#define  R_DAC_CTRL2    DACACON2
#define  R_DAC_ADR      DACADR
#define  R_DAC_DATA     DACDAT
#define  R_DAC_OBUFF    DACOBSTADR
#define  R_DAC_OSIZE    DACOBSIZE
#define  R_DAC_FIFO     DACFIFOCNT
#define  R_DAC_OSYNC    DACOBSYNC    
#define  R_DAC_OCTRL    DACOBCON
#define  R_DAC_OSTAUS   DACOBSTA
void eq_coeff_init();
void eq_gain_init();
static void (*ax32xxDACISR)(u8 flag);
/*******************************************************************************
* Function Name  : ax32xx_dacIRQHandler
* Description    : dac irq handler
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacIRQHandler(void)
{
    //u8 dacflag=0;
    

    if(R_DAC_OSTAUS&0x01)
    {
        R_DAC_OSYNC |= 1; // clear all flag
    //    dacflag = 1;
        if(ax32xxDACISR)
            ax32xxDACISR(1);
    }
    else if(R_DAC_OSTAUS&0x02)
    {
        R_DAC_OSYNC |= 2; // clear all flag
        if(ax32xxDACISR)
            ax32xxDACISR(2);
    }
    else if(R_DAC_OSTAUS&0x04)
    {
        R_DAC_OSYNC |= 4; // clear all flag
        if(ax32xxDACISR)
            ax32xxDACISR(4);
    }

//    if(ax32xxDACISR)
//        ax32xxDACISR(dacflag);
}
/*******************************************************************************
* Function Name  : ax32xx_dacWriteREG
* Description    : set dac internal register
* Input          :  u8 addr : register addr
                       u32 reg_dat : data
* Output         : None
* Return         : 
*******************************************************************************/
static void ax32xx_dacWriteREG(u8 addr, u32 reg_dat)
{
    R_DAC_ADR = (1<<30)|(addr<<24)|reg_dat;
    while((R_DAC_ADR & (1<<31)) ==(1<<31));
}
/*******************************************************************************
* Function Name  : ax32xx_dacReadREG
* Description    : get dac internal register
* Input          :  u8 addr : register addr
* Output         : None
* Return         : u32 reg_dat : data
*******************************************************************************/
static u32 ax32xx_dacReadREG(u8 addr)
{
    R_DAC_ADR = (addr<<24);
    while((R_DAC_ADR & (1<<31)) == (1<<31));
    return (u32)R_DAC_DATA;
}
/*******************************************************************************
* Function Name  : ax32xx_dacInit
* Description    : initial dac
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacInit(void)
{
//-------------dac clk----------------------
#if (AX32_PLATFORM == FPGA)
    R_SYS_CLK1 &=~(3<<26);
    R_SYS_CLK1 |= (3<<24); 
    R_SYS_CLK0 &= ~(1<<9);/*select pll0_out(96M)*/ 
    R_SYS_CLK0 |= (1<<8);
#else
    #if 0
        CLKCON0 |= BIT(8); //USB PHY 96MHz
        CLKCON0 &=~BIT(9);
        CLKCON1 &=~(0xf<<24);
        CLKCON1 |= ((96000000/24000000 - 1)<<24); 
    #else
        CLKCON0 &=~BIT(8); //SPLL
        CLKCON0 &=~BIT(9);
        CLKCON1 &=~(0xf<<24);
        CLKCON1 |= ((PLL_CLK/24000000 - 1)<<24); 
    #endif
#endif
    ax32xx_sysClkSet(SYS_CLK_AUADC,1);
    ax32xx_sysClkSet(SYS_CLK_DAC,1);
    
//------------dac configure-----------------
	R_DAC_CTRL0 |= 
				//(0<<31)|
				//(1<<29)|
				//(1<<28)|
				//(1<<27)|
				(1<<24)|		//LPF2HP_EN
                (0<<23)|
                (1<<22)|
                (0<<21)|
                (1<<20)|
                (1<<19)|        //DAC2LPF_EN
                (0<<18)|
                (0<<17)|
                (0<<16)|
                (0<<15)|
                (0<<14)|
                (1<<13)|        //EN_VCM 
                (1<<12)|     // preamp midamp enable
                (0<<11)|
                (1<<10)|        //EN_HP  
                (1<<9)|         //HP_EN
                (0<<8)|
                (0<<7)|
                (0<<6)|
                (0<<5)|
                (0<<4)|
                (1<<3)|         //EN_DAC
                (1<<2)|         //EN_AUDIOBIAS
                (1<<1)|         //EN_ANABIAS
                (1<<0);         //EN_ABG     
	R_DAC_CTRL1 = (1<<2)|(1<<5)|(1<<8); // PWM_EN_DRIVE,PWEM_EN_INT			
    


    R_DAC_CTRL0 |= (1<<0)\
                     | (1<<1)\
                     | (1<<2)\
                     | (1<<3)\
                     | (1<<9)\
                     | (1<<10)\
                     | (1<<13)\
                     | (1<<19)\
                     | (1<<20)\
                     | (1<<22)\
                     | (1<<24);
    R_DAC_CTRL1 |= (1<<2)\
                     | (1<<5);

    R_DAC_CTRL0 &=~((1<<9)|(1<<30)|(1<<31));    
    R_DAC_CTRL1 |= 1<<1;                    
   // R_DAC_CTRL1 |= (1<<9);//Class AB
    R_DAC_CTRL1 &=~(1<<9);  //Class D
    R_DAC_CTRL1 |= 1;
    
    R_DAC_CTRL2 = (1<<9)|//VCMOPA en
                  (0<<8)|//VDDHP output 0 en
                  (0x5<<5)|//HPGX
                  (0x1b<<0);//HPGSEL

    R_DAC_OCTRL = (64<<10)|    //EMPTY_SIZE_BIT  // 64
               (1<<5)|        //SAMP_CNT_BIT
               (0<<3)|        //OBUF_EMPTY_EN_BIT
               (0<<2)|        //OBUF_EMPTY_INT_BIT
               (0<<1)|        //OBUF_HFPND_INT_BIT
               (0<<0);        //OBUF_PND_INT_BIT
    
    R_DAC_OBUFF = 0;
    R_DAC_OSYNC  = (0<<3)|    //RELOAD
                 (1<<2)|    //ADDR_CNT_CLR_BIT
                 (1<<1)|    //OBUF_HPND_CLR_BIT
                 (1<<0);    //OBUF_FPND_CLR_BIT    

    R_DAC_OSIZE  = 0;    //preset OBUF each block samples, when one block done,OBUF_PND pending    
    R_DAC_FIFO = 0;

    //ax32xx_sysClkSet(SYS_CLK_DAC,0);
    ax32xxDACISR = NULL;
	eq_coeff_init();
	eq_gain_init();
}
/*******************************************************************************
* Function Name  : ax32xx_dacSampleRateSet
* Description    : set dac samlerate
* Input          :  u32 sample_rate : sample rate
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacSampleRateSet(u32 sample_rate)
{
    if((sample_rate>10) || ((sample_rate&0x03)==0x03))
        return ;
    
    ax32xx_dacWriteREG(DACCFG, (ax32xx_dacReadREG(DACCFG) & 0xffffff0f) | (sample_rate<<4));    
}
/*******************************************************************************
* Function Name  : ax32xx_dacEnable
* Description    : enable dac
* Input          :  u8 en
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacEnable(u8 en)
{
    if(en)
    {
        //ax32xx_sysClkSet(SYS_CLK_DAC,1);
        ax32xx_dacWriteREG(DACCFG, (ax32xx_dacReadREG(DACCFG)) | (en&0x01));
    }
    else
    {
        ax32xx_dacWriteREG(DACCFG, (ax32xx_dacReadREG(DACCFG)) | 0);
        //ax32xx_sysClkSet(SYS_CLK_DAC,0);
    }
    
}
/*******************************************************************************
* Function Name  : ax32xx_dacVolumeSet
* Description    : set dac volume
* Input          :  u16 volume : volume
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacVolumeSet(u16 volume)
{
    ax32xx_dacWriteREG(DACCLR, (1<<4));                        /*clear volume pending*/
    ax32xx_dacWriteREG(DACVOL, volume&0x7fff);
    ax32xx_dacWriteREG(DACVCON,(0<<4)|(1<<2)|(3));    /*BIT4=mode,BIT3=1,BIT(2,1)=step*/

    ax32xx_dacWriteREG(DACCLR, (1<<4));                        /*clear volume pending*/
    if(0 == volume)
    {
        R_DAC_CTRL2 &= 0xFF00;//gain 0
    }
    else
    {
        R_DAC_CTRL2 |= (0x5<<5)|//HPGX
                      (0x1b<<0);//HPGSEL
    }
}
/*******************************************************************************
* Function Name  : ax32xx_dacBufferSet
* Description    : set dac play buffer
* Input          :  u32 obuf_addr : out buffer addr
                      u32 obuf_size  : out buffer size
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacBufferSet(u32 obuf_addr,u32 obuf_size)
{
    R_DAC_OBUFF   = obuf_addr;
    R_DAC_FIFO    = (obuf_size>>1);
//    R_DAC_OSIZE   = (obuf_size>>2) - 1;
//    R_DAC_OSYNC  |= (1<<3);
}
/*******************************************************************************
* Function Name  : ax32xx_dacBufferFlush
* Description    : dac play reset
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacBufferFlush(u32 obuf_size)
{
    R_DAC_OSIZE   = (obuf_size>>2) - 1;
//    R_DAC_OSYNC  |= (1<<3);    
}
/*******************************************************************************
* Function Name  : ax32xx_dacReset
* Description    : dac play reset
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacReset(void)
{
    R_DAC_OCTRL = (8<<10)|    //EMPTY_SIZE_BIT
                   (1<<5)|        //SAMP_CNT_BIT
                   (1<<3)|        //OBUF_EMPTY_EN_BIT
                   (1<<2)|        //OBUF_EMPTY_INT_BIT
                   (0<<1)|        //OBUF_HFPND_INT_BIT
                   (0<<0);        //OBUF_PND_INT_BIT
    R_DAC_OSYNC  = (0<<3)|    //RELOAD
                     (1<<2)|    //ADDR_CNT_CLR_BIT
                     (1<<1)|    //OBUF_HPND_CLR_BIT
                     (1<<0);    //OBUF_FPND_CLR_BIT        
}
/*******************************************************************************
* Function Name  : ax32xx_dacStart
* Description    : dac play start
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacStart(u32 obuf_addr,u32 obuf_size)
{
	R_DAC_CTRL1 &= ~(1<<8);
    R_DAC_OCTRL = (8<<10)|    //EMPTY_SIZE_BIT
                   (1<<5)|        //SAMP_CNT_BIT
                   (1<<3)|        //OBUF_EMPTY_EN_BIT
                   (1<<2)|        //OBUF_EMPTY_INT_BIT
                   (0<<1)|        //OBUF_HFPND_INT_BIT
                   (0<<0);        //OBUF_PND_INT_BIT
    R_DAC_OSYNC  = (0<<3)|    //RELOAD
                     (1<<2)|    //ADDR_CNT_CLR_BIT
                     (1<<1)|    //OBUF_HPND_CLR_BIT
                     (1<<0);    //OBUF_FPND_CLR_BIT    
                     

/**/    R_DAC_OBUFF   = obuf_addr;
    R_DAC_OSIZE   = (obuf_size>>2) - 1;
    R_DAC_FIFO    = (obuf_size>>1);
    
    
    R_DAC_OSYNC  |= (1<<3);

    ax32xx_intEnable(IRQ_DAC,1);                 
}
/*******************************************************************************
* Function Name  : ax32xx_dacStop
* Description    : dac play stop
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacStop(void)
{
    R_DAC_OSYNC = (0<<3)|        //RELOAD
                (1<<2)|        //ADDR_CNT_CLR_BIT
                (1<<1)|        //OBUF_HPND_CLR_BIT
                (1<<0);        //OBUF_FPND_CLR_BIT
    ax32xx_intEnable(IRQ_DAC,0);    

//    ax32xx_sysClkSet(SYS_CLK_DAC,0);
}
/*******************************************************************************
* Function Name  : ax32xx_dacISRRegister
* Description    : register irq service
* Input          :  void (*isr)(u8 flag)
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacISRRegister(void (*isr)(u8 flag))
{
    ax32xxDACISR = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_dacHPSet
* Description    : dac HP VDD output set
* Input          :  u8 en : enable.1->enable,0-disable
                       u32 level : vdd level.SEE HP_VDD_E
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_dacHPSet(u8 en,u32 level)
{
    
    if(en)
    {
        level &= 0x07;
        R_DAC_CTRL0 |= (3<<0);
        R_DAC_CTRL2 &=~(1<<8);
        R_DAC_CTRL0 = (R_DAC_CTRL0 & ~((7<<27)))|(level<<27)|(1<<10);
    }
    else
    {
        R_DAC_CTRL0 &=~(1<<10);
        R_DAC_CTRL2 |= (1<<8);
    }
}
#define G_14DB		642
#define G_13DB		572
#define G_12DB		510
#define G_11DB		454

#define G_10DB		405
#define G_9DB		361
#define G_8DB		322
#define G_7DB		287
#define G_6DB		255
#define G_5DB		228
#define G_4DB		203
#define G_3DB		81 
#define G_2DB		161
#define G_1DB		144
#define G_0DB		128
#define G_M1DB      114
#define G_M2DB      102
#define G_M3DB      91 
#define G_M4DB		80 
#define G_M5DB		72 
#define G_M6DB		64 
#define G_M7DB		57 
#define G_M8DB		51 
#define G_M9DB		45 
#define G_M10DB     40 
#define G_M11DB     36 
#define G_M12DB     32 
#define G_M13DB     29 
#define G_M14DB     26 
#define G_M15DB     23 
#define G_M16DB     20 






//------------------------------------------------------------------------------
// DAC Internal SFR Address Defines              
//------------------------------------------------------------------------------
#define DACCFG                 0
#define DACSTA                 1
#define DACCLR                 2
#define DACVOL                 3
#define DACVCON                4
#define TRIMCON1               5
#define TRREGL                 6
#define TRREGR                 7
#define DACLRMIX0              8
#define DACLRMIX1              9
#define DACEQGAIN0             10
#define DACEQGAIN1             11
#define DACEQGAIN2             12
#define DACEQGAIN3             13
#define DACEQGAIN4             14
#define EQCOEFFADR             15
#define EQCOEFF                16


//EQ滤波器系数表
const u8 eq_coeff[54] =
{
    0x00, 0x34, 0x7F, 0x98, 0xc0, 0x67,
    0x00, 0x68, 0x7f, 0x2d, 0xc0, 0xcf,
    0x00, 0xce, 0x7e, 0x54, 0xc1, 0x9c,
    0x01, 0x96, 0x7c, 0x94, 0xc3, 0x2d,
    0x03, 0x1a, 0x78, 0xd7, 0xc6, 0x34,
    0x06, 0x03, 0x70, 0x4f, 0xcc, 0x06,
    0x0a, 0x00, 0x5e, 0xda, 0xd3, 0xff,
    0x11, 0xf1, 0x32, 0x87, 0xe3, 0xe2,
    0x19, 0xa8, 0xe3, 0x7a, 0xf3, 0x50
};

void eq_coeff_init()
{
	unsigned char i;
	u32 eqcoeff_value;
	u32 eqcoeffadr_value;
	for(i=0;i<9*3;i++){
		eqcoeff_value = (eq_coeff[2*i]<<8)|(eq_coeff[2*i+1]);
		eqcoeffadr_value = i;
		ax32xx_dacWriteREG(EQCOEFFADR, BIT(23)|(eqcoeffadr_value<<18)|eqcoeff_value);			
	}
}
const u16 gain[9] =
{
	G_M16DB, G_M16DB, G_M16DB, G_M8DB, G_M8DB, G_M8DB, G_M8DB, G_M8DB, G_M8DB	
};

void eq_gain_init()
{	
	ax32xx_dacWriteREG(DACEQGAIN0, (gain[1]<<11)|gain[0]);
	ax32xx_dacWriteREG(DACEQGAIN1, (gain[3]<<11)|gain[2]);
	ax32xx_dacWriteREG(DACEQGAIN2, (gain[5]<<11)|gain[4]);
	ax32xx_dacWriteREG(DACEQGAIN3, (gain[7]<<11)|gain[6]);
	ax32xx_dacWriteREG(DACEQGAIN4, (gain[9]<<11)|gain[8]);	

	ax32xx_dacWriteREG(DACCLR, BIT(7));	
}
