/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         SPI0 DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_spi.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN SPI HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"
/**
 * \module SPI0
 * \pin 
 *         SPI0    CLK   FCS    PCS    DATx  D1   D2   D3
 *                 PA11  PA10   PE0    PA8   PA9  PA6  PA7
 */
/**
 * \module 	SPI1
 * \pin 
 * 		GROUP   CLK		DATx	MISO
 * 		0		PD7     PD6     PD8
 * 		1		PA3     PA4     PA2
 * 		2		PD2     PD1     PD0
 * \note 
 * 		1.CS由使用者自己定义以及控制
 *      2.writing _SPIBUF/_SPIDMACNT clean done pending
 */
/*******************************************************************************
* Function Name  : ax32xx_spi0PinConfig
* Description	 : spi0 pin configure
* Input 		 : u8 clk_src : 0 : sysclk,1 : spll,2 : dpll
* Output		 : None
* Return		 : spi0 src clk freq 
*******************************************************************************/
static u32 ax32xx_spi0ClkConfig(u8 clk_src,u8 div)
{
    u32 spif_clk = 0;
    
    CLKCON2 &=~(0xf << 28);// clear spi_tclk source setting
    CLKCON2 |= (clk_src << 30)|// spi_tclk source = master clock
               (div << 28);
    PCON1   &=~(1<<3);
    
    if(clk_src == 0)
    {
        spif_clk = APB_CLK / (div + 1);
    }
    
    return spif_clk;
}
/*******************************************************************************
* Function Name  : ax32xx_spi0PinConfig
* Description	 : spi0 pin configure
* Input 		 : u8 bus_mode : SPI0_1_LINE/SPI0_2_LINE0/SPI0_2_LINE1/SPI0_4_LINE
* Output		 : None
* Return		 : none 
*******************************************************************************/
static void ax32xx_spi0PinConfig(u8 bus_mode)
{	
	ax32xx_gpioSFRSet(GPIO_MAP_SPI0,bus_mode);
    
    //cs/clk
    ax32xx_gpioMapSet (GPIO_PA,GPIO_PIN11|GPIO_PIN10,GPIO_FUNC_SFR);
    ax32xx_gpioDirSet (GPIO_PA,GPIO_PIN11|GPIO_PIN10,GPIO_OUTPUT);
    ax32xx_gpioDataSet(GPIO_PA,           GPIO_PIN10,GPIO_HIGH);
    
    if(bus_mode == SPI0_1_LINE)//DATX
    {
        ax32xx_gpioMapSet(GPIO_PA,GPIO_PIN8,GPIO_FUNC_SFR);
        ax32xx_gpioDirSet(GPIO_PA,GPIO_PIN8,GPIO_OUTPUT);
    }
    else if(bus_mode == SPI0_2_LINE0)//clk/DAT0/DAT1
    {
        ax32xx_gpioMapSet (GPIO_PA,GPIO_PIN9|GPIO_PIN8,GPIO_FUNC_SFR);
        ax32xx_gpioDirSet (GPIO_PA,GPIO_PIN9|GPIO_PIN8,GPIO_INPUT);
        ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN9|GPIO_PIN8,GPIO_PULL_UP);
    }
    else if(bus_mode == SPI0_2_LINE1)//clk/DAT_IN/DAT_OUT
    {
        ax32xx_gpioMapSet (GPIO_PA,GPIO_PIN9|GPIO_PIN8,GPIO_FUNC_SFR);
        ax32xx_gpioDirSet (GPIO_PA,          GPIO_PIN8,GPIO_OUTPUT);
        ax32xx_gpioDirSet (GPIO_PA,GPIO_PIN9,          GPIO_INPUT);
        ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN9,          GPIO_PULL_UP);
    }
    else if(bus_mode == SPI0_4_LINE)//clk/DAT0/DAT1/DAT2/DAT3
    {
        ax32xx_gpioMapSet (GPIO_PA,GPIO_PIN9|GPIO_PIN8|GPIO_PIN7|GPIO_PIN6,GPIO_FUNC_SFR);
        ax32xx_gpioDirSet (GPIO_PA,GPIO_PIN9|GPIO_PIN8|GPIO_PIN7|GPIO_PIN6,GPIO_INPUT);
        ax32xx_gpioPullSet(GPIO_PA,GPIO_PIN9|GPIO_PIN8|GPIO_PIN7|GPIO_PIN6,GPIO_PULL_UP);
    }
}
/*******************************************************************************
* Function Name  : ax32xx_spi0Init
* Description	 : spi0 initial
* Input 		 : u32 u32SPIBaud : spi0 baudrate set
*                  u8 bus_mode : SPI0_1_LINE/SPI0_2_LINE0/SPI0_2_LINE1/SPI0_4_LINE
* Output		 : None
* Return		 : none 
*******************************************************************************/
void ax32xx_spi0Init(u32 u32SPIBaud,u8 bus_mode)
{
    u32 spif_clk = ax32xx_spi0ClkConfig(0,3);
    
	ax32xx_sysClkSet(SYS_CLK_SPI0,1);
    
    ax32xx_spi0PinConfig(bus_mode);
    
    SPIFGCON  = (1<<24)|//cs min high time
                (1<<20)|//cs min hold time
                (1<<16)|//cs min setup time
                (0<<15)|//spi_csp mode select
                (1<<14)|//spi_csf mode select
                (0<<13)|//spi_csp en(for psram)
                (1<<12)|//spi_csf en(for flash)
                (0<<10)|//cs special mode en(auto mode)
                (((bus_mode - 1) & 3)<<8)| //data bus width(manual mode)
                (0<<7)| //transfer dir(manual mode)
                (0<<5)| //LFSR enc/dec en
                (0<<4)| //crc16 enable
                (0<<3)| //sample edge,0:raise,1:fall
                (1<<2)| //spi_clk idle polarity
                (0<<1)| //auto mode en
                (1<<0); //spif en
                
    SPIFFACON = (0<<20)|//dummy bytes count(auto mode)
                (0<<18)|//addr bytes count(auto mode)
                (0<<14)|//mode bus width(auto mode)
                (0<<12)|//cmd bus width(auto mode)
                (0<<10)|//addr bus width(auto mode)
                (0<<8)| //data bus width(auto mode)
                (0<<6)| //dummy phase enable(auto mode)
                (0<<5)| //mode phase enable(auto mode)
                (0<<4)| //cmd phase enable(auto mode)
                (0<<2)| //dma device en
                (0<<1)| //cpu data en
                (0<<0); //cpu instruction en
                
    SPIFPACON = (0<<20)|//dummy bytes count(auto mode)
                (0<<18)|//addr bytes count(auto mode)
                (0<<14)|//mode bus width(auto mode)
                (0<<12)|//cmd bus width(auto mode)
                (0<<10)|//addr bus width(auto mode)
                (0<<8)| //data bus width(auto mode)
                (0<<6)| //dummy phase enable(auto mode)
                (0<<5)| //mode phase enable(auto mode)
                (0<<4); //cmd phase enable(auto mode)

    SPIFDCON  = (0<<24)|//package size
                (0<<20)|//dummy bytes count(dma mode)
                (0<<18)|//addr bytes count(dma mode)
                (0<<14)|//mode bus width(dma mode)
                (0<<12)|//cmd bus width(dma mode)
                (0<<10)|//addr bus width(dma mode)
                (((bus_mode - 1) & 3)<<8)| //data bus width(dma mode)
                (0<<7)| //transfer dir(dma mode)
                (0<<6)| //dummy phase enable(dma mode)
                (0<<5)| //mode phase enable(dma mode)
                (0<<4)| //cmd phase enable(dma mode)
                (0<<3)| //addr phase enable(dma mode)
                (0<<2)| //data phase en
                (0<<1)| //auto package en
                (0<<0); //dma mode en
                
    SPIFACT   = (1<<25)|//clear flag
                (1<<24)|//clear flag
                (1<<23)|//clear flag
                (1<<22)|//clear flag
                (1<<21)|//clear flag
                (1<<20)|//clear flag
                (1<<19)|//clear flag
                (1<<18)|//clear flag
                (1<<17)|//clear flag
                (1<<16)|//clear flag
                (0<<7) |//dam device function lock(psram)
                (0<<6) |//cpu data function lock(psram)
                (0<<5) |//cpu instruction function lock(psram)
                (0<<4) |//dam device function lock(flash)
                (0<<3) |//cpu data function lock(flash)
                (0<<2) |//cpu instruction function lock(flash)
                (0<<1) |//dma mode kick start
                (0<<0) ;//buffer mode kick start
                
    SPIFSTA   = (0<<31)|//dma device hold flag(pasram)
                (0<<30)|//cpu data hold flag(pasram)
                (0<<29)|//cpu instruction hold flag(pasram)
                (0<<28)|//dma device hold flag(flash)
                (0<<27)|//cpu data hold flag(flash)
                (0<<26)|//cpu instruction hold flag(flash)
                (0<<25)|//dma device wr err int en(flash)
                (0<<24)|//cpu data wr err int en(flash)
                (0<<23)|//dma device int en(psram)
                (0<<22)|//cpu data int en(psram)
                (0<<21)|//cpu intstruction int en(psram)
                (0<<20)|//dma device int en(flash)
                (0<<19)|//cpu data int en(flash)
                (0<<18)|//cpu intstruction int en(flash)
                (0<<17)|//dma mode int en
                (0<<16)|//manual mode int en
                (0<<10)|//crc idle flag
                (0<<9) |//dma device wr err flag(flash)
                (0<<8) |//cpu data wr err flag(flash)
                (0<<7) |//dma device err flag(psram)
                (0<<6) |//cpu data err flag(psram)
                (0<<5) |//cpu intstruction err flag(psram)
                (0<<4) |//dma device err flag(flash)
                (0<<3) |//cpu data err flag(flash)
                (0<<2) |//cpu intstruction err flag(flash)
                (0<<1) |//rx/tx done for dma mode
                (0<<0) ;//rx/tx done for manual mode

    SPIFBAUD  = spif_clk / u32SPIBaud - 1;
    SPIFDBUF  = (0<<0);//
    SPIFDCNT  = (0<<0);//
    SPIFBADR  = (0<<2);//sdram base address (dma device)
    SPIFSADR  = (0<<0);//spi address (dma device)
    SPIFCRC   = 0xffff;//crc16 value
    SPIFPOLY  = 0xffffffff;//poly
    SPIFLFSR  = 0;//LFSR
    SPIFFAMSK = 0xfffffff;//address mask(flash,auto mode)
    SPIFPAMSK = 0xfffffff;//address mask(psram,auto mode)

    SPIFFACMD = (0<<16)|//mode value(flash,auto mode)
                (0<<8)| //write cmd(flash,auto mode)
                (0<<0); //read cmd(flash,auto mode)

    SPIFPACMD = (0<<16)|//mode value(psram,auto mode)
                (0<<8)| //write cmd(psram,auto mode)
                (0<<0); //read cmd(psram,auto mode)
                
    SPIFDCMD  = (0<<16)|//mode value(dma device,auto mode)
                (0<<8)| //write cmd(dma device,auto mode)
                (0<<0); //read cmd(dma device,auto mode)
}
/*******************************************************************************
* Function Name  : ax32xx_spi0SendByte
* Description	 : spi0 send one byte
* Input 		 : u8 byte : data
* Output		 : None
* Return		 : u8 : 
*******************************************************************************/
void ax32xx_spi0SendByte(u8 byte)
{
    SPIFGCON &=~BIT(7);
    SPIFDBUF = byte;
    SPIFACT  = 1;
    while((SPIFSTA & 1) == 0);
}
/*******************************************************************************
* Function Name  : ax32xx_spi0RecvByte
* Description	 : spi0 recv one byte
* Input 		 : none
* Output		 : None
* Return		 : u8 : 
*******************************************************************************/
u8 ax32xx_spi0RecvByte(void)
{
    SPIFGCON |= BIT(7);
    SPIFDBUF = 0xff;
    SPIFACT  = 1;
    while((SPIFSTA & 1) == 0);
    return SPIFDBUF;
}
/*******************************************************************************
* Function Name  : ax32xx_spi0Send0
* Description	 : spi0 send date by DMA
* Input 		 : u32 pDataBuf : data buffer
                   u32 u32DataLen : data len,>=8
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_spi0Send0(void *pDataBuf,u32 u32DataLen)
{
	if(u32DataLen<8)
		return ;
	SPIFGCON |= (1<<1);
    SPIFDCON |= (0<<24)|//package size
                (0<<20)|//dummy bytes count(dma mode)
                (0<<18)|//addr bytes count(dma mode)
                (0<<14)|//mode bus width(dma mode)
                (0<<12)|//cmd bus width(dma mode)
                (0<<10)|//addr bus width(dma mode)
                (0<<8)| //data bus width(dma mode)
                (0<<7)| //transfer dir(dma mode)
                (0<<6)| //dummy phase enable(dma mode)
                (0<<5)| //mode phase enable(dma mode)
                (0<<4)| //cmd phase enable(dma mode)
                (0<<3)| //addr phase enable(dma mode)
                (1<<2)| //data phase en
                (0<<1)| //auto package en
                (1<<0); //dma mode en
				
	SPIFDCNT = u32DataLen/4-1;
	SPIFBADR = (u32)pDataBuf;
	SPIFSADR = 0x6000000|0;
	
    ax32xx_sysDcacheWback((u32)pDataBuf,u32DataLen);
    
	SPIFACT  = BIT(17); //dmapnd clear
	SPIFACT  = BIT(1);  //dma mode kick start
	while((SPIFSTA & BIT(1))== 0); 

	SPIFGCON &= ~(1<<1);
	SPIFDCON &= ~((3<<18)|(0xff<<0));
}
/*******************************************************************************
* Function Name  : ax32xx_spi0Recv0
* Description	 : spi0 recv date by DMA
* Input 		 : u32 pDataBuf : data buffer
                   u32 u32DataLen : data len,>=8
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_spi0Recv0(void *pDataBuf,u32 u32DataLen)
{
	if(u32DataLen<8)
		return ;
	SPIFGCON |= (1<<1);	
	SPIFDCON |= (0x0<<24)|//package size
				(0<<20)|//dummy bytes count(dma mode)
				(0<<18)|//addr bytes count(dma mode)
				(0<<14)|//mode bus width(dma mode)
				(0<<12)|//cmd bus width(dma mode)
				(0<<10)|//addr bus width(dma mode)
				(0<<8)| //data bus width(dma mode)
				(1<<7)| //transfer dir(dma mode)
				(0<<6)| //dummy phase enable(dma mode)
				(0<<5)| //mode phase enable(dma mode)
				(0<<4)| //cmd phase enable(dma mode)
				(0<<3)| //addr phase enable(dma mode)
				(1<<2)| //data phase en
				(0<<1)| //auto package en
				(1<<0); //dma mode en
			
	SPIFDCNT = u32DataLen/4-1;
	SPIFBADR = (u32)pDataBuf;
	SPIFSADR = 0x6000000|0;

    ax32xx_sysDcacheFlush((u32)pDataBuf,u32DataLen);
    
	SPIFACT  = BIT(17); //dmapnd clear
	SPIFACT  = BIT(1);  //dma mode kick start
	while((SPIFSTA & BIT(1))== 0); 

	SPIFGCON &= ~(1<<1);
	SPIFDCON &= ~((3<<18)|(0xff<<0));
}
/*******************************************************************************
* Function Name  : ax32xx_spi0CS0Config
* Description	 : spi0 cs(flash) config
* Input 		 : u8 level : 0:cs is low
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_spi0CS0Config(u8 level)
{
    if(level)
    {
        SPIFGCON |= (0x1 << 14);
    }
    else
        SPIFGCON &=~(0x1 << 14);
}
/*******************************************************************************
* Function Name  : ax32xx_spi0PinConfig
* Description	 : spi0 pin configure
* Input 		 : u8 bus_mode : 0:3 LINE(DIN, DOUT,CLK) ,1 :2 LINE(CLK, DIN/DOUT)
* Output		 : None
* Return		 : none 
*******************************************************************************/
void spi1_pin_config(u8 bus)
{
	//CS_PA0  
	//EXAMPLE : use PA3 PA4 PA2
	PAFMAP |= (1<<0); //DI1,CLK1,DO1
	PADIR  &=~(1<<0); //out
	PAPU   |= (1<<0);
	PORTA  |= (1<<0);
	
		if(0 == bus){
			//3w
			PMAPCON1 &=~(3<<14);  
			PMAPCON1 |= (2<<14);  
			PAFMAP |= (1<<2)|(1<<3)|(1<<4); //DI1,CLK1,DO1
			//PAFMAP &=~((1<<2)|(1<<3)|(1<<4)); //io_test
			PADIR  &=~((1<<3)|(1<<4)); //out
			PADIR  |= (1<<2); 		   //in	
		
			}
		else{
			//2w
			PMAPCON1 &=~(3<<14);  
			PMAPCON1 |= (2<<14);  
			PAFMAP |= (1<<3)|(1<<4); 
			PADIR  &=~((1<<3)|(1<<4)); 		
		
		}
}
/*******************************************************************************
* Function Name  : hal_spi1Init
* Description	 : spi1 init
* Input 		 : u8 bus_mode : 0:3 LINE(DIN, DOUT,CLK) ,1 :2 LINE(CLK, DIN/DOUT)
*                  SPIBAUD 
* Output		 : None
* Return		 : none 
*******************************************************************************/
void hal_spi1Init(u32 u32SPIBaud,u8 bus_mode)
{
	ax32xx_sysClkSet(SYS_CLK_SPI1,1);
	spi1_pin_config(bus_mode);
	SPI1CON = 0;
	SPI1CON = (0<<8)|//spi high speed en
			  (0<<7)|//interrupt pending
			  (0<<6)|//master:0/slave:1 select
			  (0<<5)|//spi trans dir,0:send 1:recevie
			  (bus_mode<<4)|//data bus width,0:3w 1:2w
			  (0<<3)|//clean pending
			  (1<<2)|//sample edge
			  (0<<1)|//spi clk idle value
			  (1<<0);//spi en
	
	SPI1BAUD = APB_CLK/(2*u32SPIBaud) - 1; //effective only in master mode
}

void spi1_send_byte(u8 u8Data)
{
	/****if use spi1 as spi function, should add cs pin control**********************************/
	//add cs low
	SPI1CON &=~BIT(5);  			//TX
	SPI1BUF = u8Data;
	//while(!(SPI1CON & BIT(7)));
	ax32xx_timerTickStart();
	while(!(SPI1CON & BIT(7))){
		if(ax32xx_timerTickCount() >= APB_CLK*5)
		{
			//ret = 0;
			while(1);	//WDT
		}
	}
	ax32xx_timerTickStop();
	//add cs high
	//SPI1CON &=~BIT(3);
}

u8 spi1_receive_byte(void)
{
	/****if use spi1 as spi function, should add cs pin control**********************************/
	//add cs low
	SPI1CON |= BIT(5);  			//RX
	SPI1BUF = 0xff;
	while(!(SPI1CON & BIT(7)));
	//SPI1CON &=~BIT(3);
	//add cs high
	return SPI1BUF;
}

BOOL spi1_recevie_dma(u8 *pDataBuf,u32 u32DataLen)
{
	BOOL ret = 1;
	ax32xx_sysDcacheFlush((u32)pDataBuf,u32DataLen);
	/****if use spi1 as spi function, should add cs pin control**********************************/
	//add cs low
	SPI1CON |= BIT(5);  			//RX
	SPI1DMAADR = (u32)pDataBuf;
	SPI1DMACNT = u32DataLen/4-1;
	ax32xx_timerTickStart();
	while(!(SPI1CON & BIT(7))){
		if(ax32xx_timerTickCount() >= APB_CLK*5)
		{
			ret = 0;
			break;
		}
	}
	ax32xx_timerTickStop();
	//add cs high
	//SPI1CON &=~BIT(3);
	return ret;
}

BOOL spi1_send_dma(u8 *pDataBuf,u32 u32DataLen)
{
	BOOL ret = 1;
	ax32xx_sysDcacheWback((u32)pDataBuf,u32DataLen);
	/****if use spi1 as spi function, should add cs pin control**********************************/
	//add cs low
	SPI1CON &= ~BIT(5);  			//TX
	SPI1DMAADR = (u32)pDataBuf;
	SPI1DMACNT = u32DataLen/4-1;
	ax32xx_timerTickStart();
	while(!(SPI1CON & BIT(7))){
		if(ax32xx_timerTickCount() >= APB_CLK*5)
		{
			ret = 0;
			break;
		}
	}
	ax32xx_timerTickStop();
	//add cs high
	//while(!(SPI1CON & BIT(7)));
	//SPI1CON &=~BIT(3);
	return ret;
}
void spi1_enable(bool enable)
{
	if(enable)
        SPI1CON |= 1;
    else
        SPI1CON &=~1;
}