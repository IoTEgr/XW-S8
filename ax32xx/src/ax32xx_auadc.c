/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         AUADC DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_auadc.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AUDIO ADC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"
enum
{
	__50DB__ = 5,
	__60DB__ = 6,	
	
};
/*
#define  R_AGC_STATUS     AUADCSTA
#define  R_AGC_CLEAR      AGCCFG
#define  R_AGC_CTRL0      AGCCFG
#define  R_AGC_CTRL1      AGCCFG
#define  R_AGC_CTRL2      AGCLVL
#define  R_AGC_CTRL3      AGCDRC
#define  R_AGC_ADDR       AUADCBADR
#define  R_AUADC_CTRL     AUADCCFG
*/
static void (*ax32xxAuadcISR0)(void);
static void (*ax32xxAuadcISR1)(void);
/*******************************************************************************
* Function Name  : ax32xx_auadcIRQHandler
* Description    : auadc irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcIRQHandler(void)
{
	if(AUADCSTA & BIT(1))
	{
		if(ax32xxAuadcISR0)
			ax32xxAuadcISR0();
		AUADCSTA |= BIT(9); //HALF CLR
	}	
	else if(AUADCSTA & BIT(0))
	{
		if(ax32xxAuadcISR1)
			ax32xxAuadcISR1();
		AUADCSTA |= BIT(8); //FULL CLR
//		videoRecordsyncService(); //gongcheng mdf
	}
	
	//R_AGC_CLEAR = BIT(0);		
}
/*******************************************************************************
* Function Name  : ax32xx_auadcHalfIRQRegister
* Description    : auadc hanlf irq regiser
* Input          :void (*isr)(void) : isr call back
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcHalfIRQRegister(void (*isr)(void))
{
	ax32xxAuadcISR0 = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_auadcHalfIRQRegister
* Description    : auadc end irq regiser
* Input          :void (*isr)(void) : isr call back
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcEndIRQRegister(void (*isr)(void))
{
	ax32xxAuadcISR1 = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_auadcEnable
* Description    : auadc enable
* Input          :u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcEnable(u8 en)
{
	if(en)
	{
		AUADCCFG |= (1<<2); //RMDC EN
		AUADCCFG |=  (0x1 << 3);  //DMA EN
		// Init SRC filter SRAM
		AUADCSTA |= 0x1 << 31;
		while((AUADCSTA & 0x4) == 0x0){;}
		AUADCSTA = 0x1 << 10; // Clear SRAM flag
		AUADCCFG |= 0x1; // Kick start AUADC
		// Enable all AUADC interrupt flags
		AUADCSTA = 0x3 << 16;  //half/full ie enable
		
		ax32xx_intEnable(IRQ_AUADC,1);
	}
	else
	{
		AUADCSTA |= (1<<11)|(1<<10)|(1<<9)|(1<<8); // AGC INT PENDING CLEAR	
		AUADCCFG &=~(1<<0);//AUADC_DIS
		AUADCCFG &=~((1<<3)|(1<<2)|(1<<1));//(DMA_DIS|RMDC_DIS|DGAIN_DIS)
		AGCCFG &=~(1<<0); //AGC DIS
		AUADCSTA |= (1<<11)|(1<<10)|(1<<9)|(1<<8); // AGC INT PENDING CLEAR	

		ax32xx_intEnable(IRQ_AUADC,0);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_auadcAGCEnable
* Description    : auadc agc enable
* Input          :u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcAGCEnable(u8 en)
{
	AGCLVL   = (0x4000 << 16)    //max_thd
             | (0x3000 << 0);     //min_thd
	AGCDRC   = (0x0100 << 16)   //vad_thd
             | (0x7800 << 0);    //drc_thd
	AGCCFG   = (AGCCFG&(0xfff00000))//again_mute
           //  | (8 << 24)  //again_max
           //  | (1 << 20)  //again_min
             | (3 << 16)  //vad_scnt
             | (3 << 12)  //agc_scnt
             | (3 << 8)   //mute_scnt
             | (0 << 4)  //mute_en
             | (1 << 3)  //drc_en
             | (1 << 2)  //zcd_en
             | (1 << 1)  //vad_en
             | (1 << 0)  //agc_en
             ;
	  AUADCCFG |= (1<<1); //dgain en
	if(!en){
	   AGCCFG &=~((1<<3)| (1<<0)); //drc_dis,agc_dis
	   AUADCCFG &= ~(1<<1); // DGAIN dis
	}
	
}
/*******************************************************************************
* Function Name  : ax32xx_auadcGainSet
* Description    : auadc agc gain set
* Input          :u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcGainSet(u8 again, u8 again_mute,u8 again_max, u8 again_min)
{
	AUADCGAIN = (0x78 << 24)   //zcd_thd// dgain range 0~0x78 12db
			  | (0x2  << 18)   //dec_step  (00~11:-0.1,-0.2,-0.3,-0.4 dB)
              | (0x2  << 16)  //inc_step  (00~11:0.1,0.2,0.3,0.4 dB)
              | (0x0  << 8)       //dgain (0~0x3c)
              | (again << 0);   //again (0~0xd)
   AGCCFG &= ~(0xfff<<20);
   AGCCFG |= (again_mute<<28)|(again_max <<24)|(again_min<<20);
   AUADCSTA |= (0x1 << 30);
}
/*******************************************************************************
* Function Name  : ax32xx_auadcBufferSet
* Description    : auadc set frame buffer
* Input          :u32 addr : addr
                     u32 len   : buffer length
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcBufferSet(u32 addr,u32 len)
{
	ax32xx_sysDcacheFlush(addr,len);

	AUADCBADR = addr;
	AUADCDCNT = (len >> 2)-1;	/*AGCCON3 单位为word*/		
	AUADCCFG  |= (0 << 4);     // DMASK_SEL:DMA AUDIO DATA SELECTION.(0:0dB; 1:-6dB, 2: -12dB,  3: all data mask to 0)
	AUADCCFG  |=  (0x1 << 3);   //DMA EN
}
/*******************************************************************************
* Function Name  : ax32xx_auadcInit
* Description    : auadc intial 
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcInit(void)
{
#if (AX32_PLATFORM == FPGA)
	CLKCON1 &=~(BIT(27)|BIT(26));
	CLKCON1 |= (BIT(25)|BIT(24)); 
	CLKCON0 &= ~(BIT(9));/*select pll0_out(96M)*/ 
	CLKCON0 |= BIT(8);
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
    
    DACACON0 |= (1<<12);  //Preamp and Midamp enable 
	
	DACACON0 |= (BIT(13)| //EN_VCM 
					BIT(11)|  //MIC BIAS 
					BIT(10)|  //EN_HP  
					BIT(4)|
					BIT(3)|  //EN_DAC 
					BIT(2)|  //EN_AUDIOBIAS 
					BIT(1)|  //EN_ANABIAS 
					BIT(0)); //EN_ABG 
//	AUADCCFG |= (0x1 << 14); // Release AUADC reset

}
/*******************************************************************************
* Function Name  : ax32xx_auadcSetSampleSet
* Description    : auadc set sample set 
* Input          : int sample_rate : sample rate
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_auadcSetSampleSet(int sample_rate)
{
	AUADCCFG |= (0x1 << 14); // Release AUADC reset
	AUADCBAUD = 0x007c0002; // 14bit SARADC Fmax = 4.8MHz, FS = 192KHz
	AUADCCFG &= ~0xffff0000; // clear setting
	if (16000 == sample_rate)
	{
		//ADCBAUD = 0x00001005;
		//AUADCBAUD = 4;			//AUADCBAUD=(24000000/192000/25)-1
	  AUADCCFG |= (0x5 << 18); // Set FILT1DN = 6/1
      AUADCCFG |= (0x1 << 16); // Enable Filter 1
      AUADCCFG |= (0x1 << 21); // Set FILT2DN = 2/1
      AUADCCFG |= (0x1 << 17); // Enable Filter 2
	  debg ("- adc 16k\n");
	}	
	else if (32000== sample_rate)
	{
		//ADCBAUD = 0x00001005;
		//AUADCBAUD = 4;			//AUADCBAUD=(24000000/192000/25)-1
      AUADCCFG |= (0x5 << 18); // Set FILT1DN = 6/1
      AUADCCFG |= (0x1 << 16); // Enable Filter 1
		//AGCCON0  |= (2<<0);		//SR_SEL
	  debg ("- adc 32k\n");
	}
	else if (8000 == sample_rate)
	{
		//ADCBAUD = 0x00001005;
		//AUADCBAUD = 4;			//AUADCBAUD=(24000000/192000/25)-1
		AUADCCFG |= (0x5 << 18); // Set FILT1DN = 6/1
		AUADCCFG |= (0x1 << 16); // Enable Filter 1
		AUADCCFG |= (0x3 << 21); // Set FILT2DN = 4/1
		AUADCCFG |= (0x1 << 17); // Enable Filter 2
	//	AGCCON0  |= (3<<0);		//SR_SEL
		debg ("- adc 8k\n");
	}
}
void agc_gain_mnt(void)
{
	debg("AGC TEST:%x AGCPWR:%x\n",AGCTEST,AGCPWR);
}
