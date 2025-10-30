/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         PMU DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_pmu.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN PMU HARDWARE LAYER.
*               
*               
* History     :
* 2018-05-01  : 
*      <1>.This is created by ,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"
/*******************************************************************************
* Function Name  : pmu_test
* Description	 : hal layer get rtc time
* Input 		 : u32 baudrate, u8 ch
* Output		 : None
* Return		 : 
*******************************************************************************/
#if 0
void pmu_test(u32 baudrate){
	
//	SDIDX = (3300<<0)|(1<<12)|(1<<13);  // AVDD,up  AVDD,hold
//	SDIDX &=~((1<<12)|(1<<13)); 
	
	PMUREFVOL = *(u32 *)(0x5000-140+0x30 + 4) & 0x3ff;//0x32a;//efuse读值？
	PMUCON  = (((APB_CLK/(2*baudrate) - 1) & 0x7F)<<2);	//2M
	PMUFSBAUD = 32*2*(((PMUCON>>2) & 0x7F)+1);//fpga: baudrate*1/32;芯片：baudrate*1/24;
	//debgstr("\nADC_BAUD:");
	//debghex(((PMUCON>>2) & 0x7F));
	debg("ADC_BAUD:%x\n",((PMUCON>>2) & 0x7F));
//
	//debgstr("\nPMUFSBAUD:");	
	//debghex(PMUFSBAUD);
	debg("PMUFSBAUD:%x\n",PMUFSBAUD);
	//INT:0:DISABLE 1:NORMAL 2:EXCEPTION
	//PMUCON1 |= (2<<BAT_HW_IE)/*|(2<<BAT_LW_IE)|(2<<BAT_PROT_IE)|(2<<VDD_HW_IE)|(2<<VDD_LW_IE)|(2<<VDD_PROT_IE)|*/(1<<0);//PMU_SAM_POWER
	PMUCON1 |= (1<<BAT_HW_IE)|(1<<BAT_LW_IE)|(1<<BAT_PROT_IE)|(1<<VDD_HW_IE)|(1<<VDD_LW_IE)|(1<<VDD_PROT_IE)|(3<<0);	
	PMUBATHW  = 0x1072;
	PMUBATLW  = 0xff;
	PMUBATPROT= 0x1070;
	
	PMUDBGHW  = 0xfb;
	PMUDBGLW  = 0xfb;
	PMUDBGPROT= 0xfb;
	PMUCON  |= (1<<1)|(1<<0);
}
/*******************************************************************************
* Function Name  : pmu_manual
* Description	 : hal layer get rtc time
* Input 		 : u32 baudrate,u32 ch
* Output		 : None
* Return		 : 
*******************************************************************************/
#define combo_delay(t)  do {int i=(t)*2; while(i--) asm("l.nop"); } while(0)
u32 pmu_manual(u32 baudrate,u32 ch)  
{
    //PCON0   &=~(1<<29);
	//SDIDX = (3600<<0)|(1<<12)|(1<<13);  // AVDD,up  AVDD,hold
	//SDIDX &=~BIT(12); 
	//PMUREFVOL = 0x32a;//VBG
	PMUREFVOL = *(u32 *)(0x5000-140+0x30 + 4) & 0x3ff;
	//PMU_ADCBAUD
	PMUCON =0;
    PMUCON = (/*((APB_CLK/(2*baudrate) - 1) & 0x7F)*/1<<2)|(1<<1)|(1<<0);//((127 & 0x7F)<<2)
    combo_delay(10);
		//ADCSEL
    PMUADCCON  |= (ch & 0xf);  //fpga_adc_cs
    PMUADCCON  |= (1<<5);//ADCGO
    while(PMUADCCON & (1<<4));
//	debgstr("PMUADCDAT:");	
//	debghex(PMUADCDAT);
	debg("PMUADCDAT:%x\n",PMUADCDAT);
    return (PMUADCDAT & 0x3ff);
}
#endif
u16 checkbat_first(void){
	u16 bat;
	bat = PMUBATVOL & 0x1fff;
	debg("bat:%dmV\n",bat);
	return bat;
}