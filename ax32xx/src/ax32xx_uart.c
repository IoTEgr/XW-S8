/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         UART DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_uart.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN UART0 HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"

#define  R_UART_CTRL0         UART0CON
#define  R_UART_BAUDRATE0     UART0BAUD
#define  R_UART_PEND0         UART0PND
#define  R_UART_DATA0         UART0DAT

static void (*uart0Callback)(u8 data);
/*******************************************************************************
* Function Name  : ax32xx_uart0IRQHandler
* Description    : uart 0 IRQ handler
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0IRQHandler(void)
{
    R_UART_PEND0 |= BIT(4);
	if(uart0Callback)
		uart0Callback(REG32(R_UART_DATA0)&0xff);
}
/*******************************************************************************
* Function Name  : ax32xx_uart0IOCfg
* Description    : uart0 rx tx config
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0IOCfg(u8 en)
{
	if(en)
	{
		u8 ch;
		u32 pin;

        //ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);
        //ax32xx_gpioSFRSet(GPIO_MAP_UARTRX0,UART0_RX_POS);
        
        if(UART0_TX_POS == UART0_POS_PB0)
        {
            ax32xx_gpioDigitalSet(GPIO_PB,GPIO_PIN0,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet (GPIO_PB,GPIO_PIN0,GPIO_OUTPUT);
            ax32xx_gpioPullSet(GPIO_PB,GPIO_PIN0,GPIO_PULL_UP);
            ax32xx_gpioDataSet(GPIO_PB,GPIO_PIN0,GPIO_HIGH);
			 ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);   // tx-pa7
        }
		else if(UART0_TX_POS == UART0_POS_PA7)
        {
			ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN7,GPIO_OUTPUT);
            ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN7,GPIO_PULL_UP);
            ax32xx_gpioDataSet(GPIO_PA,GPIO_PIN7,GPIO_HIGH);
            ax32xx_gpioMapSet(GPIO_PA,GPIO_PIN7,GPIO_FUNC_SFR);		
            ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);   // tx-pa7
        }
		else if(UART0_TX_POS == UART0_POS_PE1)
        {
			ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN1,GPIO_OUTPUT);
            ax32xx_gpioPullSet(GPIO_PE,GPIO_PIN1,GPIO_PULL_UP);
            ax32xx_gpioDataSet(GPIO_PE,GPIO_PIN1,GPIO_HIGH);
            ax32xx_gpioMapSet(GPIO_PE,GPIO_PIN1,GPIO_FUNC_SFR);		
            ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_TX_POS);   // tx-pE1
        }

		if(UART0_RX_POS == UART0_POS_PB6)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN6;
		}else if(UART0_RX_POS == UART0_POS_PB0)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN0;
		}
		else
		{
			ch = 0;
			pin = 0;
		}
		if (pin != 0)
		{
			ax32xx_gpioDigitalSet(ch,pin,GPIO_DIGITAL_EN);
			ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_SFR);	  // set as gpio func
			ax32xx_gpioDirSet(ch,pin,GPIO_INPUT);					  // set direction
			ax32xx_gpioPullSet(ch,pin,GPIO_PULL_FLOATING);	 // set pull level,

			ax32xx_gpioSFRSet(GPIO_MAP_UARTRX0,UART0_RX_POS);
		}
	}
	else
	{
		ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0,UART0_POS_NONE);
		ax32xx_gpioSFRSet(GPIO_MAP_UARTRX0,UART0_POS_NONE);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_uart0Init
* Description    : uart0 initial 
* Input          : u32 baudrate : uart0 baudrate
                      void (*isr)(u8 data) : rx callback
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0Init(u32 baudrate,void (*isr)(u8 data))
{
	
    uart0Callback = isr;
	
	R_UART_CTRL0 = 0;
	R_UART_BAUDRATE0 = APB_CLK/baudrate-1;                           //baud rate 
	R_UART_PEND0 |= 3<<4;    
	R_UART_CTRL0 |= (1<<4);         

	R_UART_CTRL0 |= 1; //RX	

	ax32xx_intEnable(IRQ_UART0,1); // enable irq

	ax32xx_int_priority(IRQ_UART0,1);//�������ȼ����ܹؼ�����Ȼ©����
}
/*******************************************************************************
* Function Name  : ax32xx_uart0SendByte
* Description    : uart0 send data
* Input          : u8 data : send data
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart0SendByte(u8 data)
{
	R_UART_DATA0 = data;
	while((R_UART_PEND0 & 0x2)==0);
	R_UART_PEND0 |= 1;
}


//===========uart1 ===========
#define  R_UART_CTRL1         BUCON
#define  R_UART_BAUDRATE1     BUBAUD
#define  R_UART_PEND1         BUSTA
#define  R_UART_DATA1         BUDATA

static void (*uart1Callback)(u8 data);
/*******************************************************************************
* Function Name  : ax32xx_uart0IRQHandler
* Description    : uart 0 IRQ handler
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart1IRQHandler(void)
{
    R_UART_PEND1 &= ~BIT(3);	// clear rec int flag
	if(uart1Callback)
		uart1Callback(REG32(R_UART_DATA1)&0xff);
}
/*******************************************************************************
* Function Name  : ax32xx_uart0IOCfg
* Description    : uart0 rx tx config
* Input          : 
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart1IOCfg(u8 en)
{
	u8 ch;
	u32 pin;

	if(en)
	{
		if(UART1_TX_POS == UART1_POS_PB6)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN6;
		}
		else if(UART1_TX_POS == UART1_POS_PB5)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN5;
		}else if(UART1_TX_POS == UART1_POS_PA7)
		{
			ch = GPIO_PA;
			pin = GPIO_PIN7;
		}else if(UART1_TX_POS == UART1_POS_PE1)
		{
			ch = GPIO_PE;
			pin = GPIO_PIN1;
		}else if(UART1_TX_POS == UART1_POS_PB0)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN0;
		}
		else
		{
			ch = 0;
			pin = 0;
		}
		if (pin != 0)
		{
			//ax32xx_gpioDigitalSet(ch,pin,GPIO_DIGITAL_EN);
			ax32xx_gpioDirSet (ch,pin,GPIO_OUTPUT);
			ax32xx_gpioPullSet(ch,pin,GPIO_PULL_UP);
			ax32xx_gpioDataSet(ch,pin,GPIO_HIGH);
			ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_SFR);	  // set as gpio func
			//ax32xx_gpioDrvSet(ch,pin,GPIO_DRV_NORMAL);	 // set normal drving
			ax32xx_gpioSFRSet(GPIO_MAP_UARTTX1,UART1_TX_POS);
			
//			uart_Printf("set uart1-TX(0x%X,0x%X)\n",ch,pin);
        }

		if(UART1_RX_POS == UART1_POS_PB6)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN6;
		}
		else if(UART1_RX_POS == UART1_POS_PB5)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN5;
		}else if(UART1_RX_POS == UART1_POS_PB0)
		{
			ch = GPIO_PB;
			pin = GPIO_PIN0;
		}else if(UART1_RX_POS == UART1_POS_PE1)
		{
			ch = GPIO_PE;
			pin = GPIO_PIN1;
		}
		else
		{
			ch = 0;
			pin = 0;
		}
		if (pin != 0)
		{
			ax32xx_gpioDigitalSet(ch,pin,GPIO_DIGITAL_EN);
			ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_SFR);	  // set as gpio func
			//ax32xx_gpioDrvSet(ch,pin,GPIO_DRV_NORMAL);	 // set normal drving
			ax32xx_gpioDirSet(ch,pin,GPIO_INPUT);					  // set direction
			ax32xx_gpioLedPull(ch,pin,GPIO_PULLE_FLOATING);  // set LED pull level, 
			//ax32xx_gpioPullSet(ch,pin,GPIO_PULL_UP);				 // set pull level,

			//ax32xx_gpioSFRSet(GPIO_MAP_UARTRX1,UART1_POS_NONE);
			ax32xx_gpioSFRSet(GPIO_MAP_UARTRX1,UART1_RX_POS);

//			uart_Printf("set uart1-RX(0x%X,0x%X)\n",ch,pin);
		}
	}
	else
	{
		ax32xx_gpioSFRSet(GPIO_MAP_UARTTX1,UART1_POS_NONE);
		ax32xx_gpioSFRSet(GPIO_MAP_UARTRX1,UART1_POS_NONE);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_uart0Init
* Description    : uart0 initial 
* Input          : u32 baudrate : uart0 baudrate
                      void (*isr)(u8 data) : rx callback
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart1Init(u32 baudrate,void (*isr)(u8 data))
{
    uart1Callback = isr;
	
	R_UART_CTRL1 = 0;
	R_UART_BAUDRATE1 = APB_CLK/baudrate-1; //baud rate 
	//R_UART_PEND1 |= 1<<3;    
	R_UART_CTRL1 |= (1<<4);         

	R_UART_CTRL1 |= (1<<2); //RX int enable
	ax32xx_intEnable(IRQ_BTUART,1); // enable irq
}


/*******************************************************************************
* Function Name  : ax32xx_uart1SendByte
* Description    : uart1 send data
* Input          : u8 data : send data
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_uart1SendByte(u8 data)
{
	(R_UART_DATA1) = data;
	while(((R_UART_PEND1) & 0x4)==0);
	return;
}



