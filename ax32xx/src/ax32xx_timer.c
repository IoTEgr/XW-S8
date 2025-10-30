/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         TIMER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_timer.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"


#define  R_TIMER_CTRL0       TMR0CON
#define  R_TIMER_PLOAD0      TMR0PR
#define  R_TIMER_PWM0        TMR0PWM
#define  R_TIMER_COUNTER0    TMR0CNT
#define  R_TIMER_STATUS0     TMR0CPND
#define  R_TIMER_CTRL1       TMR1CON
#define  R_TIMER_PLOAD1      TMR1PR
#define  R_TIMER_PWM1        TMR1PWM
#define  R_TIMER_COUNTER1    TMR1CNT
#define  R_TIMER_STATUS1     TMR1CPND
#define  R_TIMER_CTRL2       TMR2CON
#define  R_TIMER_PLOAD2      TMR2PR
#define  R_TIMER_PWM2        TMR2PWM
#define  R_TIMER_COUNTER2    TMR2CNT
#define  R_TIMER_STATUS2     TMR2CPND
#define  R_TIMER_CTRL3       TMR3CON
#define  R_TIMER_PLOAD3      TMR3PR
#define  R_TIMER_PWM3        TMR3PWM
#define  R_TIMER_COUNTER3    TMR3CNT
#define  R_TIMER_STATUS3     TMR3CPND

#define  R_TICK_CTRL         TTCTL
#define  R_TICK_COUNTER      TTCNT
#define  R_TICK_PRLOAD       TTPR

void (*timerISR[TIMER_MAX])(void);
/*******************************************************************************
* Function Name  : ax32xx_timer0IRQHanlder
* Description    : timer0 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer0IRQHandler(void)
{
	R_TIMER_STATUS0|= BIT(5);	
	if(timerISR[TIMER0])
		timerISR[TIMER0]();
}
/*******************************************************************************
* Function Name  : ax32xx_timer1IRQHanlder
* Description    : timer1 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer1IRQHandler(void)
{
	R_TIMER_STATUS1|= BIT(5);	
	if(timerISR[TIMER1])
		timerISR[TIMER1]();
}
/*******************************************************************************
* Function Name  : ax32xx_timer2IRQHanlder
* Description    : timer2 irq handler
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer2IRQHandler(void)
{
	R_TIMER_STATUS2|= BIT(5);	
	if(timerISR[TIMER2])
		timerISR[TIMER2]();
}
/*******************************************************************************
* Function Name  : ax32xx_timer3IRQHanlder
* Description    : timer3 irq handler
* Input          : 

* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_timer3IRQHandler(void)
{
	R_TIMER_STATUS3|= BIT(5);	
	if(timerISR[TIMER3])
		timerISR[TIMER3]();
}
/*******************************************************************************
* Function Name  : ax32xx_timerISRRegister
* Description    : timer isr register
* Input          :  u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
                       void (*isr)(void) : callback function
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerISRRegister(u8 timer,void (*isr)(void))
{
    if(timer>=TIMER_MAX)
		return -1;
	timerISR[timer] = isr;

	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timerStart
* Description    : timer start
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
                      u32 frq  : frq .>20hz
                      void (*isr)(void) : callback function
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerStart(u8 timer,u32 frq,void (*isr)(void))
{
	
    if((timer>=TIMER_MAX))// || (frq<20))
		return -1;
    u32 clkdiv,preload;


    if(frq>=750000) // 750K
		clkdiv = 3;// 12M
	else
		clkdiv = 7;// 750K
    preload = (((APB_CLK/frq)/(1<<clkdiv))-1);
	if(preload>0xffff)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD0 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1); //
	          R_TIMER_COUNTER0 = 0x0000;
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD1 =  preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);//
	          R_TIMER_COUNTER1 = 0x0000;
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD2 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);
	          R_TIMER_COUNTER2 = 0x0000;
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD3 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);
	          R_TIMER_COUNTER3 = 0x0000;
			  break;
		default : return -1;break;
	}
	timerISR[timer] = isr;
		
	ax32xx_intEnable(IRQ_TIMER0+timer,1);
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timeStop
* Description    : timer stop
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerStop(u8 timer)
{
    if(timer>=TIMER_MAX)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  &= ~(BIT(3)|BIT(0));
			  break;
		default : return -1;break;		
	}
	ax32xx_intEnable(IRQ_TIMER0+timer,0);	
    timerISR[timer] = NULL;
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timerEnable
* Description    : timer enable
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerEnable(u8 timer)
{
    if(timer>=TIMER_MAX)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  |= (BIT(3)|BIT(0));
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  |= (BIT(3)|BIT(0));
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  |= (BIT(3)|BIT(0));
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  |= (BIT(3)|BIT(0));
			  break;
		default : return -1;break;		
	}
	return 0;	
}
/*******************************************************************************
* Function Name  : ax32xx_timerDisable
* Description    : timer disable
* Input          : u8 timer : timer id.TIMER0/TIMER1/TIMER2/TIMER3
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerDisable(u8 timer)
{
    if(timer>=TIMER_MAX)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  &= ~(BIT(3)|BIT(0));
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  &= ~(BIT(3)|BIT(0));
			  break;
		default : return -1;break;		
	}

	return 0;	
}
/*******************************************************************************
* Function Name  : ax32xx_timerTickStart
* Description    : tick timer start
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerTickStart(void)
{
	R_TICK_CTRL = 0;
	R_TICK_PRLOAD  = 0xffffffff;
	R_TICK_COUNTER = 0;
	
	R_TICK_CTRL = (1<<6)|
			(0<<5)|
			(0<<4)|
			(2<<0);
	return 0;
}

s32 ax32xx_timer3TickStart(void)
{
	R_TIMER_CTRL3 = 0;
	R_TIMER_PLOAD3  = 0xffffffff;
	R_TIMER_COUNTER3 = 0;
	
	R_TIMER_CTRL3 = (1<<6)|
			(0<<5)|
			(0<<4)|
			(2<<0);
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timerTickStop
* Description    : tick timer stop
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_timerTickStop(void)
{
	R_TICK_CTRL = (1<<6);

	return 0;
}

s32 ax32xx_timer3TickStop(void)
{
	R_TIMER_CTRL3 = (1<<6);

	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_timerTickCount
* Description    : tick timer counter get
* Input          : 
* Output         : None
* Return         : count
*******************************************************************************/
u32 ax32xx_timerTickCount(void)
{
	return R_TICK_COUNTER;
}

u32 ax32xx_timer3TickCount(void)
{
	return R_TIMER_COUNTER3;
}
/*******************************************************************************
* Function Name  : ax32xx_printTimerStart
* Description    : us timer 
* Input          : 
* Output         : None
* Return         : count
*******************************************************************************/
s32 ax32xx_printTimerStart(u8 timer,u32 preload,void (*isr)(void))//
{
	
    if((timer>=TIMER_MAX))// || (frq<20))
		return -1;
    u32 clkdiv;
    clkdiv = 7;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD0 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1); //
	          R_TIMER_COUNTER0 = 0x0000;
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD1 =  preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);//
	          R_TIMER_COUNTER1 = 0x0000;
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD2 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);
	          R_TIMER_COUNTER2 = 0x0000;
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3  = (clkdiv<<11) | BIT(3) | BIT(0); 
	          R_TIMER_PLOAD3 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1);
	          R_TIMER_COUNTER3 = 0x0000;
			  break;
		default : return -1;break;
	}
	if(isr != 0)
	{
		timerISR[timer] = isr;
		ax32xx_intEnable(IRQ_TIMER0+timer,1);
	}
	return 0;
}

/*******************************************************************************
* Function Name  : ax32xx_timerPWMStart
* Description    : timer PWM start
* Input          : timer: timer select
* 				   pwm_map: PWM output group select
* 				   frq:     PWM frequency configure
* 				   percent: PWM duty rate(<100)
* Output         : None
* Return         : count
*******************************************************************************/
s8 ax32xx_timerPWMStart(u8 timer,u8 pwm_map,u32 frq, u8 percent)
{
	//uart_Printf("%d,%d,%d,%d\n",timer,pwm_map,frq,percent);
    if((timer>=TIMER_MAX))// || (frq<20))
		return -1;
	if(pwm_map == 0)
		return -1;
	if(percent>100)
		percent = 100;
		
    u32 clkdiv,preload, pwm;
	/*
    if(frq>=750000) // 750K
		clkdiv = 3;// 12M
	else
		clkdiv = 7;// 750K
	*/
	clkdiv = 1;
    preload = (((APB_CLK/frq)/(1<<clkdiv))-1);
	pwm = preload * percent/100;
	if(preload>0xffff)
		return -1;
	switch(timer)
	{
		case TIMER0 :
			if(pwm_map>TMR0_POS_PE15)
				return -1;
			ax32xx_gpioSFRSet(GPIO_MAP_TMR0,pwm_map);
			switch(pwm_map)
			{
				case TMR0_POS_PA1: ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN1,GPIO_OUTPUT); break;
				case TMR0_POS_PA4: ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN4,GPIO_OUTPUT); break;
				case TMR0_POS_PB0: ax32xx_gpioDirSet(GPIO_PB,GPIO_PIN0,GPIO_OUTPUT); break;
				case TMR0_POS_PB1: ax32xx_gpioDirSet(GPIO_PB,GPIO_PIN1,GPIO_OUTPUT); break;
				case TMR0_POS_PB5: ax32xx_gpioDirSet(GPIO_PB,GPIO_PIN5,GPIO_OUTPUT); break;
				case TMR0_POS_PB6: ax32xx_gpioDirSet(GPIO_PB,GPIO_PIN6,GPIO_OUTPUT); break;
				case TMR0_POS_PD2: ax32xx_gpioDirSet(GPIO_PD,GPIO_PIN2,GPIO_OUTPUT); break;
				case TMR0_POS_PF2: ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN2,GPIO_OUTPUT); break;
				case TMR0_POS_PF11: ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT); break;
				case TMR0_POS_PE15: ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN15,GPIO_OUTPUT); break;
				default: break;
			}
			ax32xx_sysClkSet(SYS_CLK_TIMER0,1);
			R_TIMER_CTRL0  = (clkdiv<<11); 
			R_TIMER_PLOAD0 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1); //
			R_TIMER_PWM0 	 = pwm;
			R_TIMER_COUNTER0 = 0x0000;
			R_TIMER_STATUS0 = BIT(4)|BIT(5);
			R_TIMER_CTRL0 |= BIT(1); //TIMER enable and work in PWM
			break;
		case TIMER1 : 
			if(pwm_map>TMR1_POS_PE0)
				return -1;
			ax32xx_gpioSFRSet(GPIO_MAP_TMR1,pwm_map);
			switch(pwm_map)
			{
				case TMR1_POS_PA1: ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN1,GPIO_OUTPUT); break;
				case TMR1_POS_PA5: ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN5,GPIO_OUTPUT); break;
				case TMR1_POS_PB4: ax32xx_gpioDirSet(GPIO_PB,GPIO_PIN4,GPIO_OUTPUT); break;
				case TMR1_POS_PD3: ax32xx_gpioDirSet(GPIO_PD,GPIO_PIN3,GPIO_OUTPUT); break;
				case TMR1_POS_PD10: ax32xx_gpioDirSet(GPIO_PD,GPIO_PIN10,GPIO_OUTPUT); break;
				case TMR1_POS_PF7: ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN7,GPIO_OUTPUT); break;
				case TMR1_POS_PE0: ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN0,GPIO_OUTPUT); break;
				default: break;
			}
			ax32xx_sysClkSet(SYS_CLK_TIMER1,1);
			R_TIMER_CTRL1  = (clkdiv<<11); 
			R_TIMER_PLOAD1 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1); //
			R_TIMER_PWM1 	 = pwm;
			R_TIMER_COUNTER1 = 0x0000;
			R_TIMER_STATUS1 = BIT(4)|BIT(5);
			R_TIMER_CTRL1 |= BIT(1); //TIMER enable and work in PWM
			break;
		case TIMER2 : 
			if(pwm_map>TMR2_POS_PE1)
				return -1;
			ax32xx_gpioSFRSet(GPIO_MAP_TMR2,pwm_map);
			switch(pwm_map)
			{
				case TMR2_POS_PA2: ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN2,GPIO_OUTPUT); break;
				case TMR2_POS_PA6: ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN6,GPIO_OUTPUT); break;
				case TMR2_POS_PD0: ax32xx_gpioDirSet(GPIO_PD,GPIO_PIN0,GPIO_OUTPUT); break;
				case TMR2_POS_PF0: ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN0,GPIO_OUTPUT); break;
				case TMR2_POS_PF8: ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN8,GPIO_OUTPUT); break;
				case TMR2_POS_PE1: ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN1,GPIO_OUTPUT); break;
				default: break;
			}
			ax32xx_sysClkSet(SYS_CLK_TIMER2,1);
			R_TIMER_CTRL2  = (clkdiv<<11); 
			R_TIMER_PLOAD2 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1); //
			R_TIMER_PWM2 	 = pwm;
			R_TIMER_COUNTER2 = 0x0000;
			R_TIMER_STATUS2 = BIT(4)|BIT(5);
			R_TIMER_CTRL2 |= BIT(1); //TIMER enable and work in PWM
			break;
		case TIMER3 : 
			if(pwm_map>TMR3_POS_PE2)
				return -1;
			ax32xx_gpioSFRSet(GPIO_MAP_TMR3,pwm_map);
			switch(pwm_map)
			{
				case TMR3_POS_PA3: ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN3,GPIO_OUTPUT); break;
				case TMR3_POS_PA9: ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN9,GPIO_OUTPUT); break;
				case TMR3_POS_PF1: ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN1,GPIO_OUTPUT); break;
				case TMR3_POS_PF9: ax32xx_gpioDirSet(GPIO_PF,GPIO_PIN9,GPIO_OUTPUT); break;
				case TMR3_POS_PE2: ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN2,GPIO_OUTPUT); break;
				default: break;
			}
			ax32xx_sysClkSet(SYS_CLK_TIMER3,1);
			R_TIMER_CTRL3  = (clkdiv<<11); 
			R_TIMER_PLOAD3 = preload;//(((APB_CLK/frq)/(1<<clkdiv))-1); //
			R_TIMER_PWM3 	 = pwm;
			R_TIMER_COUNTER3 = 0x0000;
			R_TIMER_STATUS3 = BIT(4)|BIT(5);
			R_TIMER_CTRL3 |= BIT(1); //TIMER enable and work in PWM
			break;
		default :
			return -1; 
			break;
	}
//	timerISR[timer] = isr;
//		
//	ax32xx_intEnable(IRQ_TIMER0+timer,1);
	return  0;
}

s8  ax32xx_timerPWMStop(u8 timer)
{
	if(timer>=TIMER_MAX)
		return -1;
	switch(timer)
	{
		case TIMER0 : 
			  R_TIMER_CTRL0   = 0;
			  ax32xx_sysClkSet(SYS_CLK_TIMER0,0);
			  break;
		case TIMER1 : 
			  R_TIMER_CTRL1   = 0;
			  ax32xx_sysClkSet(SYS_CLK_TIMER1,0);
			  break;
		case TIMER2 : 
			  R_TIMER_CTRL2   = 0;
			  ax32xx_sysClkSet(SYS_CLK_TIMER2,0);
			  break;
		case TIMER3 : 
			  R_TIMER_CTRL3   = 0;
			  ax32xx_sysClkSet(SYS_CLK_TIMER3,0);
			  break;
		default : return -1;break;		
	}
}


