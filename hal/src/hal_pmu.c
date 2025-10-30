/****************************************************************************
**
 **                          BUILDWIN HAL LAYER
  ** *   **                   THE APPOTECH HAL
   **** **                 GPIO HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_pmu.c
* Author      : Ticru 
* Version     : V0200
* Date        : 05/02/2017
* Description : This file for BUILDWIN PMU HARDWARE LAYER.
*               
*               
* History     :
* 2017-05-02  : 
*      <1>.This is created by ticru,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"
/*******************************************************************************
* Function Name  : hal_PmuISR
* Description	 : 
* Input 		 : none
* Output		 : None
* Return		 :
*******************************************************************************/
void hal_PmuISR(void)
{
	if(PMUFLAG & BIT(1)){ //BAT_LW:3.0v
		debg("BAT_LW");
		PMUFLAG |= BIT(1);
	}
	
	if(PMUFLAG & BIT(0)){ //BAT_PROT:3.5v
		debg("BAT_PROT");
		layout_nopower_display();
		PMUFLAG |= BIT(0);

	}

	if(PMUFLAG & BIT(2)){ //BAT_HW:5.1v
		debg("BAT_HW");
		PMUFLAG |= BIT(2);
	}
}
/*******************************************************************************
* Function Name  : Pmu_Wdt_exception
* Description	 : 
* Input 		 : none
* Output		 : None
* Return		 :开机使用exception，还没到配置普通中断，因为没打开外设，可以直接wko为0关机。
*******************************************************************************/
void close_device(void);
//dialogOpCtrl(dialogIdmain,DIALOG_OP_IO_SETINFO,(u32_t)R_ID_STR_PWR_NO);
void Pmu_Wdt_exception(void)   //
{
	if(PMUFLAG & BIT(1)){ //BAT_LW:3.0v，直接关机
		debg("BAT_LW_ex");
		PMUFLAG |= BIT(1);
		hal_vddWKOEnable(0);
	}
	
	if(PMUFLAG & BIT(0)){ //BAT_PROT:3.5v，提示后关机
		debg("BAT_PROT_ex");
		debg("LOW_power");
		layout_nopower_display();
		PMUFLAG |= BIT(0);
		hal_vddWKOEnable(0);
	}

	if(PMUFLAG & BIT(2)){ //BAT_HW:5.1v
		debg("BAT_HW_ex");
		PMUFLAG |= BIT(2);
	}
	if(WDTCON & BIT(1)){
		debg("WDT_ex");
	}
}

void close_device(void)
{
   //如果VDDRTC过低
    //关闭时钟
    PCON0 |= (0xfff << 20)|(0xff << 9);
    
    //关闭后录电源
    PEFMAP &=~(1<<1);
    PEDIR  &=~(1<<1);
    PORTE  &=~(1<<1);
    
    //关闭LCD背光
    PAFMAP &=~(1<<8);
    PADIR  &=~(1<<8);
    PORTA  &=~(1<<8);
    
    //关闭SENSOR电源
    LDOCON &=~((1<<15)|(1<<7)|(1<<3));
	//u8 temp;
	//ax32xx_rtcDataRead(RTC_RD_IRTCON0,&temp,1);	  
    //关闭电源
    while(1)
    {
        PAFMAP &=~(3<<6);
        PADIR  &=~(3<<6);
        PORTA  &=~(3<<6); //gsensor i2c output low
        
        //close gsensor power
		    
	  
		//ax32xx_rtcDataWrite(IRTCON0_WR_CMD,(1<<4)|(1<<1));
		ax32xx_rtcDataWrite(IRTCON5_WR_CMD,(7<<5),1);//clean wko/wki0/wki1 weakup pending
		ax32xx_rtcDataWrite(IRTCON4_WR_CMD,(1<<1),1);//wko output 0
    }
}


