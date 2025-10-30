/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX INTRRUPUT
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_int.h
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
#ifndef AX32XX_INT_H
    #define AX32XX_INT_H


typedef enum
{
    IRQ_TIMER0=0,	//0
    IRQ_TIMER1,		//1
    IRQ_TIMER2,		//2
    IRQ_TIMER3,		//3
	IRQ_LCDC,		//4
	IRQ_RTC_WDT,	//5
	IRQ_DAC,		//6
	IRQ_GPIO,		//7
	IRQ_UART0,		//8
    IRQ_SPI0,		//9
	IRQ_SDC0,		//10
	IRQ_SDC1,		//11
	IRQ_SPI1,		//12
	IRQ_LVD,		//13
	IRQ_CSI,		//14
	IRQ_USB20,		//15
	IRQ_JPGA,		//16
    IRQ_IIC0_IIC1,	//17
    IRQ_AUADC,		//18
	IRQ_MCP0,		//19
	IRQ_BTUART,		//20
	IRQ_DE,		    //21
    IRQ_JPGB,       //22
	IRQ_ROTATE,		//23
    IRQ_MCP1,       //24
    IRQ_MCP2,       //25
    IRQ_EMI,        //26
    IRQ_OSDCMP,     //27
    IRQ_PMU,        //28
    IRQ_USB11,      //29
    //IRQ_UART1,
    IRQ_USB11MCP,   //30
    IRQ_TT_MIPI,    //31
    
	IRQ_MAX
}IRQ_IDX_E;

void exception_trigger(void);
/*******************************************************************************
* Function Name  : ax32xx_intEnable
* Description    : int enable set
* Input          : u8 irqNum : irq index,in IRQ_IDX_E
                   u8 en     : 1->enable,0->disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_intEnable(u8 irqNum,u8 en);

/*******************************************************************************
* Function Name  : ax32xx_int_priority
* Description    : int enable set
* Input          : u8 irqNum : irq index,in IRQ_IDX_E
                   u8 level     : 1->high priority,0->low priority
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_int_priority(u8 irqNum,u8 level);

/*******************************************************************************
* Function Name  : ax32xx_intInit
* Description    : int initial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_intInit(void);
/*******************************************************************************
* Function Name  : ax32xx_vddrtcCalculate
* Description    : Calculate vddrtc voltage
* Input          : u32 Artc : adc sample value of vddrtc
*                  u32 Abg  : adc sample value of bandgap
* Output         : none
* Return         : vddrtc voltage(unit : mV)
*******************************************************************************/
u32 ax32xx_vddrtcCalculate(u32 Artc,u32 Abg);
extern void (*fp_usb_isr)(void);
extern void (*fp_usb11_isr)(void);
/*******************************************************************************
* Function Name  : ax32xx_intCriticalInit
* Description    : critical init
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
#define ax32xx_intCriticalInit()    u32 cpu_sr = 0
/*******************************************************************************
* Function Name  : ax32xx_intCriticalEnter
* Description    : critical enter
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
#define ax32xx_intCriticalEnter()   do{cpu_sr = SPR_SR & (1<<1);SPR_SR &=~(1<<1);}while(0)
/*******************************************************************************
* Function Name  : ax32xx_intCriticalExit
* Description    : critical leave
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
#define ax32xx_intCriticalExit()    do{SPR_SR |= cpu_sr;}while(0)

#endif
