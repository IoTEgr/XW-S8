/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         IIC DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_iic.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN IIC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"
// IIC0 MAPPING GROUP
//   GROUP    SCL     SDA
// -------------------------
//   0        PE2     PE13
// -------------------------
//   1        PE0     PE1
// -------------------------
//   2        PE13    PE15
// -------------------------
//   3        PE14    PE13

// IIC1 MAPPING GROUP
//   GROUP    SCL     SDA
// -------------------------
//   0        PA6     PA7
// -------------------------
//   1        PB5     PB6
// -------------------------
//   2        PD0     PD1
// -------------------------
//   3        PD3     PD4
// -------------------------
//   4        PD5     PD6
// -------------------------
//   5        PD9     PD8
// -------------------------
//   6        PE0     PE1
    
#define  R_IIC_BAUDRATE0        IIC0BAUD
#define  R_IIC_CTRL0            IIC0CON
#define  R_IIC_COMMAND0         IIC0CMD
#define  R_IIC_STATUS0          IIC0STA
#define  R_IIC_ODATA0           IIC0TDATA
#define  R_IIC_IDATA0           IIC0RDATA

#define  R_IIC_BAUDRATE1        IIC1BAUD
#define  R_IIC_CTRL1            IIC1CON
#define  R_IIC_COMMAND1         IIC1CMD
#define  R_IIC_STATUS1          IIC1STA
#define  R_IIC_ODATA1           IIC1TDATA
#define  R_IIC_IDATA1           IIC1RDATA

#define  USE_SOFTWARE_IIC0		0		// 0 : hardware iic ,1:  software iic

#define SOFTWARE_IIC_DELAY	6//#define G_IE_DIS	
//#define G_IE_EN 
#define G_IE_DIS	__LGIE_DIS__()
#define G_IE_EN 	__LGIE_EN__()

#define SOFTWARE_IIC_CH 		GPIO_PE
#define SOFTWARE_IICSDA_PIN 	GPIO_PIN15
#define SOFTWARE_IICSCL_PIN 	GPIO_PIN13
/*
#if (IIC0_POS == IIC0_POS0)
#define SOFTWARE_IICSDA_PIN 	GPIO_PIN13
#define SOFTWARE_IICSCL_PIN 	GPIO_PIN2
#elif (IIC0_POS == IIC0_POS1)
#define SOFTWARE_IICSDA_PIN 	GPIO_PIN0
#define SOFTWARE_IICSCL_PIN 	GPIO_PIN1
#elif (IIC0_POS == IIC0_POS2)
#define SOFTWARE_IICSDA_PIN 	GPIO_PIN15
#define SOFTWARE_IICSCL_PIN 	GPIO_PIN13
#elif (IIC0_POS == IIC0_POS3)
#define SOFTWARE_IICSDA_PIN 	GPIO_PIN13
#define SOFTWARE_IICSCL_PIN 	GPIO_PIN14
#else
	#error "Undefine iic0 pin group";
#endif
*/
static void software_iic_delay_us(u32 dtime)
{
	volatile int cnt=dtime;
	while(cnt--)
	{
		asm("l.nop");
	}
}

static void software_iic_gpioInit(u8 ch,u32 pin,u8 dir,u8 pull)
{
	ax32xx_gpioDigitalSet(ch,pin,GPIO_DIGITAL_EN);
	ax32xx_gpioMapSet(ch,pin,GPIO_FUNC_GPIO);     // set as gpio func
	ax32xx_gpioDrvSet(ch,pin,GPIO_DRV_NORMAL);   // set normal drving
	ax32xx_gpioDirSet(ch,pin,dir);                    // set direction
	ax32xx_gpioPullSet(ch,pin,pull);                 // set pull level,
}
void software_iic_sdaout(void)
{
	software_iic_gpioInit(SOFTWARE_IIC_CH,SOFTWARE_IICSDA_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
	software_iic_gpioInit(SOFTWARE_IIC_CH,SOFTWARE_IICSCL_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
}

void software_iic_sdain(void)
{
	software_iic_gpioInit(SOFTWARE_IIC_CH,SOFTWARE_IICSDA_PIN,GPIO_INPUT,GPIO_PULL_UP);
	software_iic_gpioInit(SOFTWARE_IIC_CH,SOFTWARE_IICSCL_PIN,GPIO_OUTPUT,GPIO_PULL_UP);
}
void software_iic_sda_set(u8 data)
{
	ax32xx_gpioDataSet(SOFTWARE_IIC_CH,SOFTWARE_IICSDA_PIN,data);
}
void software_iic_scl_set(u8 data)
{
	ax32xx_gpioDataSet(SOFTWARE_IIC_CH,SOFTWARE_IICSCL_PIN,data);
}
u8 software_iic_sda_get(void)
{
	return ax32xx_gpioDataGet(SOFTWARE_IIC_CH,SOFTWARE_IICSDA_PIN);
} 
void software_iic_init()
{
	software_iic_sdaout();
	software_iic_sda_set(1);
	software_iic_scl_set(1);
}
 
void software_iic_start(void)
{
	G_IE_DIS;//critical enter
	software_iic_sdaout();
	software_iic_sda_set(1);
	software_iic_scl_set(1);
	software_iic_delay_us(SOFTWARE_IIC_DELAY*2);
	software_iic_sda_set(0);
	software_iic_delay_us(SOFTWARE_IIC_DELAY*2);
	software_iic_scl_set(0);
	G_IE_EN;//critical exit
}
 
void software_iic_stop(void)
{
	G_IE_DIS;//critical enter
	software_iic_sdaout();
	software_iic_scl_set(0);
	software_iic_sda_set(0);
	software_iic_delay_us(SOFTWARE_IIC_DELAY*2);
	software_iic_scl_set(1);
	software_iic_sda_set(1);
	software_iic_delay_us(SOFTWARE_IIC_DELAY*2);
	G_IE_EN;//critical exit
}
 
u8 software_iic_wait_ack(void)//0:ACK 1: NACK
{
	G_IE_DIS;//critical enter
	u8 time=0;
	software_iic_sdain();
	software_iic_sda_set(1);
	software_iic_delay_us(SOFTWARE_IIC_DELAY/2);
	software_iic_scl_set(1);
	software_iic_delay_us(SOFTWARE_IIC_DELAY/2);
	while(software_iic_sda_get())
	{
	  time++;
		if(time>250)
		{
			software_iic_stop();
			G_IE_EN;//critical exit
			return 1;
		}
	}
	software_iic_scl_set(0);
	G_IE_EN;//critical exit
	return 0;

}
 
#if USE_SOFTWARE_IIC0
static void software_iic_ack(void)
{
	G_IE_DIS;//critical enter
	software_iic_scl_set(0);
	software_iic_sdaout();
	software_iic_sda_set(0);
	software_iic_delay_us(SOFTWARE_IIC_DELAY);
	software_iic_scl_set(1);
	software_iic_delay_us(SOFTWARE_IIC_DELAY);
	software_iic_scl_set(0);
	G_IE_EN;//critical exit
}
 
static void software_iic_nack(void)
{
	G_IE_DIS;//critical enter
	software_iic_scl_set(0);
	software_iic_sdaout();
	software_iic_sda_set(1);
	software_iic_delay_us(SOFTWARE_IIC_DELAY);
	software_iic_scl_set(1);
	software_iic_delay_us(SOFTWARE_IIC_DELAY);
	software_iic_scl_set(0);
	G_IE_EN;//critical exit
}
 
static void software_iic_sendbyte(u8 w)
{
	G_IE_DIS;//critical enter
	//debg("sendbyte:%x\n",w);
	u8 i;
	software_iic_sdaout();
	software_iic_scl_set(0);
	for(i=0;i<8;i++)
	{
		software_iic_sda_set((w&0x80)>>7);//iicsda=(w&0x80)>>7;
		w<<=1;
		software_iic_delay_us(SOFTWARE_IIC_DELAY);
		software_iic_scl_set(1);
		software_iic_delay_us(SOFTWARE_IIC_DELAY);
		software_iic_scl_set(0);
	}
	software_iic_delay_us(SOFTWARE_IIC_DELAY);
	G_IE_EN;//critical exit
}

static u8 software_iic_readbyte(unsigned char ack)
{
	G_IE_DIS;//critical enter
	//debg("readbyte:");
	unsigned char i=0,r=0;
	software_iic_sdain();
	for(i=0;i<8;i++)
	{
		software_iic_scl_set(0);
		software_iic_delay_us(SOFTWARE_IIC_DELAY);
		software_iic_scl_set(1);
		r<<=1;
		if(software_iic_sda_get())
			r++;
		software_iic_delay_us(SOFTWARE_IIC_DELAY/2);
	}
	if(!ack)
		software_iic_nack();
	else
		software_iic_ack();
	//debg("%x\n",r);
	G_IE_DIS;//critical exit
	return r;
}
#endif


/*******************************************************************************
* Function Name  : ax32xx_iic0Init
* Description    : iic 0 initial
* Input          : u32 baudrate : iic0 baudrate
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0Init(u32 baudrate)
{
#if USE_SOFTWARE_IIC0
	software_iic_init();
#else
	ax32xx_gpioSFRSet(GPIO_MAP_IIC0,IIC0_POS); // 

    if(IIC0_POS == IIC0_POS2)
    {
        ax32xx_gpioDigitalSet(GPIO_PE,GPIO_PIN13|GPIO_PIN15,GPIO_DIGITAL_EN);
        ax32xx_gpioMapSet (GPIO_PE,GPIO_PIN13|GPIO_PIN15,GPIO_FUNC_SFR);
        ax32xx_gpioDataSet(GPIO_PE,GPIO_PIN13|GPIO_PIN15,GPIO_HIGH);
        ax32xx_gpioDirSet (GPIO_PE,GPIO_PIN13,GPIO_OUTPUT);
        ax32xx_gpioDirSet (GPIO_PE,GPIO_PIN15,GPIO_INPUT);
        ax32xx_gpioPullSet(GPIO_PE,GPIO_PIN13|GPIO_PIN15,GPIO_PULL_UP);
    }
    else
        return;
    
	REG32(R_IIC_BAUDRATE0) = APB_CLK/((5*baudrate))-1;
	REG32(R_IIC_CTRL0) |= (1<<7);     
#endif  
}
/*******************************************************************************
* Function Name  : ax32xx_iic0Uninit
* Description    : iic 0 uninitial
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0Uninit(void)
{
	#if USE_SOFTWARE_IIC0
	#else
		ax32xx_gpioSFRSet(GPIO_MAP_IIC0,IIC0_POS_NONE); // 
		REG32(R_IIC_CTRL0) &= ~(1<<7);   

    if(IIC0_POS == IIC0_POS2)
    {
        ax32xx_gpioDigitalSet(GPIO_PE,GPIO_PIN13|GPIO_PIN15,GPIO_DIGITAL_EN);
        ax32xx_gpioMapSet (GPIO_PE,GPIO_PIN13|GPIO_PIN15,GPIO_FUNC_GPIO);
        ax32xx_gpioDirSet (GPIO_PE,GPIO_PIN13,GPIO_INPUT);
        ax32xx_gpioDirSet (GPIO_PE,GPIO_PIN15,GPIO_INPUT);
        ax32xx_gpioPullSet(GPIO_PE,GPIO_PIN13|GPIO_PIN15,GPIO_PULL_FLOATING);
    }

	#endif
}

/*******************************************************************************
* Function Name  : ax32xx_iic0Start
* Description    : iic 0 send start singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0Start(void)
{
	#if USE_SOFTWARE_IIC0
		software_iic_start();
	#else
		REG32(R_IIC_COMMAND0) = (1<<7);
	#endif
}
/*******************************************************************************
* Function Name  : ax32xx_iic0Stop
* Description    : iic 0 send stop singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0Stop(void)
{
	#if USE_SOFTWARE_IIC0
		software_iic_stop();
	#else
		REG32(R_IIC_COMMAND0) = (1<<6);   // generate stop signal

		while(!(R_IIC_STATUS0&(1))){};
		
	    REG32(R_IIC_COMMAND0)|=(1);	
	#endif
}
/*******************************************************************************
* Function Name  : ax32xx_iic0Stop
* Description    : iic 0 recv ack singal
* Input          :  none
* Output         : None
* Return         : 0->ack,1->nack
*******************************************************************************/
u8 ax32xx_iic0RecvACK(void)
{
	#if USE_SOFTWARE_IIC0
		return (!software_iic_wait_ack());
	#else
		return  (!(REG32(R_IIC_STATUS0)&(1<<7)));
	#endif
}
/*******************************************************************************
* Function Name  : ax32xx_iic0SendACK
* Description    : iic 0 send ack singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0SendACK(void)
{
	#if USE_SOFTWARE_IIC0
		software_iic_ack();
	#else
		REG32(R_IIC_COMMAND0) |= (1<<3);
	//	while(!(R_IIC_STATUS0 & 1));
	#endif
}
/*******************************************************************************
* Function Name  : ax32xx_iic0SendByte
* Description    : iic 0 send one byte
* Input          :  u8 byte : byte
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic0SendByte(u8 byte)
{
	#if USE_SOFTWARE_IIC0
		software_iic_sendbyte(byte);
	#else
	    REG32(R_IIC_ODATA0) = byte;
	    REG32(R_IIC_COMMAND0) |= (1<<4);      
		while(!(R_IIC_STATUS0 & 1));
		REG32(R_IIC_COMMAND0) |= 1;	
	#endif
}
/*******************************************************************************
* Function Name  : ax32xx_iic0RecvByte
* Description    : iic 0 recv one byte
* Input          :  none
* Output         : None
* Return         : u8 byte : byte
*******************************************************************************/
u8 ax32xx_iic0RecvByte(void)
{
	#if USE_SOFTWARE_IIC0
		return software_iic_readbyte(1);
	#else
	    R_IIC_COMMAND0 |= (1<<3)|(1<<5);   //
		while(!(R_IIC_STATUS0 & 1)); 
	    R_IIC_COMMAND0 |= 1;
		return (REG32(R_IIC_IDATA0)&0xff);	
	#endif
}

/*******************************************************************************
* Function Name  : ax32xx_iic0Init
* Description    : iic 0 initial
* Input          : u32 baudrate : iic0 baudrate
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1Init(u32 baudrate)
{
	ax32xx_gpioSFRSet(GPIO_MAP_IIC1,IIC1_POS);
    
    if(IIC1_POS == IIC1_POS2)
    {
        ax32xx_gpioDigitalSet(GPIO_PD,GPIO_PIN0|GPIO_PIN1,GPIO_DIGITAL_EN);
        ax32xx_gpioMapSet (GPIO_PD,GPIO_PIN0|GPIO_PIN1,GPIO_FUNC_SFR);
        ax32xx_gpioDataSet(GPIO_PD,GPIO_PIN0|GPIO_PIN1,GPIO_HIGH);
        ax32xx_gpioDirSet (GPIO_PD,GPIO_PIN0,GPIO_OUTPUT);
        ax32xx_gpioDirSet (GPIO_PD,GPIO_PIN1,GPIO_INPUT);
        ax32xx_gpioPullSet(GPIO_PD,GPIO_PIN0|GPIO_PIN1,GPIO_PULL_UP);
    }
	else if(IIC1_POS == IIC1_POS0)
    {
        ax32xx_gpioDigitalSet(GPIO_PA,GPIO_PIN6|GPIO_PIN7,GPIO_DIGITAL_EN);
        ax32xx_gpioMapSet (GPIO_PA,GPIO_PIN6|GPIO_PIN7,GPIO_FUNC_SFR);
        ax32xx_gpioDataSet(GPIO_PA,GPIO_PIN6|GPIO_PIN7,GPIO_HIGH);
        ax32xx_gpioDirSet (GPIO_PA,GPIO_PIN6,GPIO_OUTPUT);
        ax32xx_gpioDirSet (GPIO_PA,GPIO_PIN7,GPIO_INPUT);
        ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN6|GPIO_PIN7,GPIO_PULL_UP);
    }
	else if(IIC1_POS == IIC1_POS6)
	{
		ax32xx_gpioDigitalSet(GPIO_PE,GPIO_PIN0|GPIO_PIN1,GPIO_DIGITAL_EN);
        ax32xx_gpioMapSet (GPIO_PE,GPIO_PIN0|GPIO_PIN1,GPIO_FUNC_SFR);
        ax32xx_gpioDataSet(GPIO_PE,GPIO_PIN0|GPIO_PIN1,GPIO_HIGH);
        ax32xx_gpioDirSet (GPIO_PE,GPIO_PIN0,GPIO_OUTPUT);
        ax32xx_gpioDirSet (GPIO_PE,GPIO_PIN1,GPIO_INPUT);
        ax32xx_gpioPullSet(GPIO_PE,GPIO_PIN0|GPIO_PIN1,GPIO_PULL_UP);
	}
    else
        return ;
    
	REG32(R_IIC_BAUDRATE1) = APB_CLK/((5*baudrate))-1;
	REG32(R_IIC_CTRL1) |= (1<<7);     
}
/*******************************************************************************
* Function Name  : ax32xx_iic1Uninit
* Description    : iic 1 uninitial
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1Uninit(void)
{
	ax32xx_gpioSFRSet(GPIO_MAP_IIC1,IIC1_POS_NONE); // 
	REG32(R_IIC_CTRL1) &= ~(1<<7);  
    if(IIC1_POS == IIC1_POS0)
    {
        ax32xx_gpioMapSet(GPIO_PA,GPIO_PIN6,GPIO_FUNC_GPIO);
        ax32xx_gpioMapSet(GPIO_PA,GPIO_PIN7,GPIO_FUNC_GPIO);   
        ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN6,GPIO_OUTPUT);
        ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN7,GPIO_OUTPUT);
        ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN6|GPIO_PIN7,GPIO_PULL_DOWN);
        ax32xx_gpioDataSet(GPIO_PA,GPIO_PIN6|GPIO_PIN7,GPIO_LOW);
    }
    else if(IIC1_POS == IIC1_POS1)
    {
        ax32xx_gpioMapSet(GPIO_PD,GPIO_PIN10,GPIO_FUNC_GPIO);
        ax32xx_gpioMapSet(GPIO_PD,GPIO_PIN11,GPIO_FUNC_GPIO);
    }
	else if(IIC1_POS == IIC1_POS6)
	{
		ax32xx_gpioMapSet(GPIO_PE,GPIO_PIN0,GPIO_FUNC_GPIO);
        ax32xx_gpioMapSet(GPIO_PE,GPIO_PIN1,GPIO_FUNC_GPIO);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_iic1Start
* Description    : iic 1 send start singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1Start(void)
{
	REG32(R_IIC_COMMAND1) = (1<<7);
}
/*******************************************************************************
* Function Name  : ax32xx_iic1Stop
* Description    : iic 1 send stop singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1Stop(void)
{
	volatile int timeout = 0x8000;
	REG32(R_IIC_COMMAND1) = (1<<6);   // generate stop signal

	while(!(R_IIC_STATUS1&(1)))
	{
		timeout--;
		if(timeout==0)
			break;
	}
	
    REG32(R_IIC_COMMAND1)|=(1);	
}
/*******************************************************************************
* Function Name  : ax32xx_iic1Stop
* Description    : iic 1 recv ack singal
* Input          :  none
* Output         : None
* Return         : 0->ack,1->nack
*******************************************************************************/
u8 ax32xx_iic1RecvACK(void)
{
	return  (!(REG32(R_IIC_STATUS1)&(1<<7)));
}
/*******************************************************************************
* Function Name  : ax32xx_iic1SendACK
* Description    : iic 1 send ack singal
* Input          :  none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1SendACK(void)
{
	REG32(R_IIC_COMMAND1) |= (1<<3);
}
/*******************************************************************************
* Function Name  : ax32xx_iic1SendByte
* Description    : iic 1 send one byte
* Input          :  u8 byte : byte
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_iic1SendByte(u8 byte)
{
	volatile int timeout = 0x8000;
    REG32(R_IIC_ODATA1) = byte;
    REG32(R_IIC_COMMAND1) |= (1<<4);      
	while(!(R_IIC_STATUS1 & 1))
	{
		timeout--;
		if(timeout==0)
			break;
	}
	REG32(R_IIC_COMMAND1) |= 1;	
}
/*******************************************************************************
* Function Name  : ax32xx_iic1RecvByte
* Description    : iic 1 recv one byte
* Input          :  none
* Output         : None
* Return         : u8 byte : byte
*******************************************************************************/
u8 ax32xx_iic1RecvByte(void)
{
//	if(I2C_NACK == u8AckFlag)    
	{
	    REG32(R_IIC_COMMAND1) |= BIT(3)|BIT(5);   //

    }
/*	else  
	{
	   REG32(R_IIC_COMMAND1) |= (1<<5);
	}*/
    volatile int i = 0;
	while(!(R_IIC_STATUS1 & 1))
	{
		i++;
		if(i>=0x8000)
			break;
	}
    R_IIC_COMMAND1 |= 1;
	return (REG32(R_IIC_IDATA1)&0xff);	
}




