/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         LCD DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_lcd.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN LCD HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"


typedef struct LCD_SPI_MODE_S {
	u8 CPOL,	//CPOL = 0,CLK is "Low" when inactive
	   CPHA,	//CPHA = 0,sample edge is first edge
	   ORDER,	//ORDER = 0,MSB
	   BITS;	//number of data bits
} LCD_SPI_MODE_T;

#define  R_LCD_CTRL          LCDCTRL
#define  R_LCD_STATUS        LCDSTA
#define  R_LCD_COMMAND       LCDCMD
#define  R_LCD_RLINE         LCDPREL
#define  R_LCD_SIGNAL        LCDSCON
#define  R_LCD_SIGNAL0       LCDDOE
#define  R_LCD_VSYNCF        LCDVSRP
#define  R_LCD_HSYNCF        LCDHSRP
#define  R_LCD_VSYNCB        LCDVSFP
#define  R_LCD_HSYNCB        LCDHSFP
#define  R_LCD_DEF           LCDDERP
#define  R_LCD_DEB           LCDDEFP
#define  R_LCD_DESTART       LCDDESL
#define  R_LCD_DEEND         LCDDEEL
#define  R_LCD_POSX          LCDFSTX
#define  R_LCD_POSY          LCDFSTY
#define  R_LCD_WIDTH         LCDFRMW
#define  R_LCD_HEIGHT        LCDFRMH
#define  R_LCD_WINWIDTH      LCDWINX
#define  R_LCD_WINHEIGHT     LCDWINY
#define  R_LCD_MODE          LCDDMOD
#define  R_LCD_MODE1         LCDDMOD1
#define  R_LCD_CSSTATUS      LCDCST
#define  R_LCD_ENDLINE       LCDENDL
#define  R_LCD_ACTIVELINE    LCDBLKL
#define  R_LCD_NUMBER        LCDDNUM
#define  R_LCD_CLKSEL        LCDTCON
#define  R_LCD_CMMAND        LCDCMD
#define  R_LCD_RGBORD        LCDRGBORD


#if (AX32_PLATFORM == AX3291A)

#define  LCD_SPI_CS_CH	   GPIO_PD
#define  LCD_SPI_CS_PIN    GPIO_PIN5

#define  LCD_SPI_CLK_CH    GPIO_PD
#define  LCD_SPI_CLK_PIN   GPIO_PIN6

#define  LCD_SPI_DAT_CH    GPIO_PD
#define  LCD_SPI_DAT_PIN   GPIO_PIN7

#define  LCD_MCU_RS_CH	   GPIO_PF
#define  LCD_MCU_RS_PIN    GPIO_PIN2

#define  LCD_MCU_CS_CH	   GPIO_PF
#define  LCD_MCU_CS_PIN    GPIO_PIN4

#define  LCD_MCU_WR_CH	   GPIO_PF
#define  LCD_MCU_WR_PIN    GPIO_PIN1
	 
#elif (AX32_PLATFORM == AX3282S)

	#define  LCD_SPI_CS_CH     GPIO_PB
	#define  LCD_SPI_CS_PIN    GPIO_PIN5
	
	#define  LCD_SPI_CLK_CH    GPIO_PB
	#define  LCD_SPI_CLK_PIN   GPIO_PIN6
	
	#define  LCD_SPI_DAT_CH    GPIO_PD
	#define  LCD_SPI_DAT_PIN   GPIO_PIN5

	#define  LCD_MCU_RS_CH     GPIO_PF
	#define  LCD_MCU_RS_PIN    GPIO_PIN2

	#define  LCD_MCU_CS_CH     GPIO_PF
	#define  LCD_MCU_CS_PIN    GPIO_PIN4

	#define  LCD_MCU_WR_CH     GPIO_PF
	#define  LCD_MCU_WR_PIN    GPIO_PIN1
#elif (AX32_PLATFORM == AX3292)
	#define  LCD_SPI_CS_CH     GPIO_PD
	#define  LCD_SPI_CS_PIN    GPIO_PIN5
	
	#define  LCD_SPI_CLK_CH    GPIO_PD
	#define  LCD_SPI_CLK_PIN   GPIO_PIN6
	
	#define  LCD_SPI_DAT_CH    GPIO_PD
	#define  LCD_SPI_DAT_PIN   GPIO_PIN7
	
	#define  LCD_MCU_RS_CH     GPIO_PF
	#define  LCD_MCU_RS_PIN    GPIO_PIN2

	#define  LCD_MCU_CS_CH     GPIO_PF
	#define  LCD_MCU_CS_PIN    GPIO_PIN4

	#define  LCD_MCU_WR_CH     GPIO_PF
	#define  LCD_MCU_WR_PIN    GPIO_PIN1	
#elif  (AX32_PLATFORM == JT5292)||(AX32_PLATFORM == JT5295)

	#define  LCD_SPI_CS_CH	   GPIO_PD
	#define  LCD_SPI_CS_PIN    GPIO_PIN5

	#define  LCD_SPI_CLK_CH    GPIO_PD
	#define  LCD_SPI_CLK_PIN   GPIO_PIN6

	#define  LCD_SPI_DAT_CH    GPIO_PD
	#define  LCD_SPI_DAT_PIN   GPIO_PIN7

	#define  LCD_MCU_RS_CH	   GPIO_PF
	#define  LCD_MCU_RS_PIN    GPIO_PIN2

	#define  LCD_MCU_CS_CH	   GPIO_PF
	#define  LCD_MCU_CS_PIN    GPIO_PIN4

	#define  LCD_MCU_WR_CH	   GPIO_PF
	#define  LCD_MCU_WR_PIN    GPIO_PIN1
	

#else
	#error "check PIN MAPPING"
#endif


#define LCD_SPI_CS_L       ax32xx_gpioDataSet(LCD_SPI_CS_CH,LCD_SPI_CS_PIN,GPIO_LOW) 
#define LCD_SPI_CS_H       ax32xx_gpioDataSet(LCD_SPI_CS_CH,LCD_SPI_CS_PIN,GPIO_HIGH) 

#define LCD_SPI_CLK_L      ax32xx_gpioDataSet(LCD_SPI_CLK_CH,LCD_SPI_CLK_PIN,GPIO_LOW) 
#define LCD_SPI_CLK_H      ax32xx_gpioDataSet(LCD_SPI_CLK_CH,LCD_SPI_CLK_PIN,GPIO_HIGH) 

#define LCD_SPI_DAT_L      ax32xx_gpioDataSet(LCD_SPI_DAT_CH,LCD_SPI_DAT_PIN,GPIO_LOW) 
#define LCD_SPI_DAT_H      ax32xx_gpioDataSet(LCD_SPI_DAT_CH,LCD_SPI_DAT_PIN,GPIO_HIGH)  

#define	LCD_MCU_RS_H	   ax32xx_gpioDataSet(LCD_MCU_RS_CH,LCD_MCU_RS_PIN,GPIO_HIGH) 
#define	LCD_MCU_RS_L	   ax32xx_gpioDataSet(LCD_MCU_RS_CH,LCD_MCU_RS_PIN,GPIO_LOW) 

#define LCD_MCU_CS_H       ax32xx_gpioDataSet(LCD_MCU_CS_CH,LCD_MCU_CS_PIN,GPIO_HIGH) 
#define LCD_MCU_CS_L       ax32xx_gpioDataSet(LCD_MCU_CS_CH,LCD_MCU_CS_PIN,GPIO_LOW) 

#define LCD_MCU_WR_H       ax32xx_gpioDataSet(LCD_MCU_WR_CH,LCD_MCU_WR_PIN,GPIO_HIGH) 
#define LCD_MCU_WR_L       ax32xx_gpioDataSet(LCD_MCU_WR_CH,LCD_MCU_WR_PIN,GPIO_LOW) 

#define LCD_SPI_DELAY     ax32xx_sysCpuNopDelay(100)


void (*ax32xx_lcdISR[LCD_IRQ_TE-LCD_IRQ_FSRT+1])(void);

static LCD_SPI_MODE_T lcd_spi_cfg= // default set
{
	.CPOL=0,
	.CPHA=0,
	.ORDER=0,
	.BITS=8
};
/*******************************************************************************
* Function Name  : ax32xx_lcdSPIMode
* Description    : lcd spi mode set
* Input          : u8 cpol :cpol 0/1
*                  u8 cpha : cpha 0/1
*                  u8 order: 0/1
*                  u8 bits : 
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSPIMode(u8 cpol,u8 cpha,u8 order,u8 bits)
{
	lcd_spi_cfg.CPOL = cpol;
	lcd_spi_cfg.CPHA = cpha;
	lcd_spi_cfg.ORDER = order;
	lcd_spi_cfg.BITS = bits;	
}
/*******************************************************************************
* Function Name  : ax32xx_lcdSPIInit
* Description    : lcd spi initial GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSPIInit(void)
{
	ax32xx_gpioMapSet(LCD_SPI_CS_CH,LCD_SPI_CS_PIN,GPIO_FUNC_GPIO);
	ax32xx_gpioPullSet(LCD_SPI_CS_CH,LCD_SPI_CS_PIN,GPIO_PULL_UP);
	ax32xx_gpioDirSet(LCD_SPI_CS_CH,LCD_SPI_CS_PIN,GPIO_OUTPUT);
	LCD_SPI_CS_H;

	ax32xx_gpioMapSet(LCD_SPI_CLK_CH,LCD_SPI_CLK_PIN|LCD_SPI_DAT_PIN,GPIO_FUNC_GPIO);
    ax32xx_gpioPullSet(LCD_SPI_CLK_CH,LCD_SPI_CLK_PIN|LCD_SPI_DAT_PIN,GPIO_PULL_UP);
	ax32xx_gpioDirSet(LCD_SPI_CLK_CH,LCD_SPI_CLK_PIN|LCD_SPI_DAT_PIN,GPIO_OUTPUT);
	if(lcd_spi_cfg.CPOL == 0)
		ax32xx_gpioDataSet(LCD_SPI_CLK_CH,LCD_SPI_CLK_PIN|LCD_SPI_DAT_PIN,GPIO_LOW);
	else
		ax32xx_gpioDataSet(LCD_SPI_CLK_CH,LCD_SPI_CLK_PIN|LCD_SPI_DAT_PIN,GPIO_HIGH);
}
/*******************************************************************************
* Function Name  : ax32xx_lcdSPIUninit
* Description    : lcd spi uninitial release GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSPIUninit(void)
{
	ax32xx_gpioMapSet(LCD_SPI_CLK_CH,LCD_SPI_CLK_PIN|LCD_SPI_DAT_PIN,GPIO_FUNC_SFR);

	LCD_SPI_CS_H;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdSPISendData
* Description    : lcd spi send data
* Input          : u32 data : data
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSPISendData(u32 data)
{
	u32 mask = (lcd_spi_cfg.ORDER == 0) ? (1 << (lcd_spi_cfg.BITS - 1)) : 1;
	u8 i = lcd_spi_cfg.BITS;

	LCD_SPI_CS_L;
	LCD_SPI_DELAY;                    // delay
    while(i)
    {
		if(lcd_spi_cfg.CPHA == 0)  // data out check-0
		{
			if(data & mask)
				LCD_SPI_DAT_H;
			else
				LCD_SPI_DAT_L;
		}
		LCD_SPI_DELAY;					  // delay

		if(lcd_spi_cfg.CPOL == 0) // clk
			LCD_SPI_CLK_H;
		else
			LCD_SPI_CLK_L;

		LCD_SPI_DELAY;                    // delay

		if(lcd_spi_cfg.CPHA == 1) //data out check -1
		{
			if(data & mask)
				LCD_SPI_DAT_H;
			else
				LCD_SPI_DAT_L;
		}
		LCD_SPI_DELAY;					  // delay

		if(lcd_spi_cfg.CPOL == 0)// clk
			LCD_SPI_CLK_L;
		else
			LCD_SPI_CLK_H;
		LCD_SPI_DELAY;					  // delay

		if(lcd_spi_cfg.ORDER == 0) // data shift
			data <<= 1;
		else
			data >>= 1;

		LCD_SPI_DELAY;                  // delay

		i--;
    }

	LCD_SPI_CS_H;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdMcuSendCmd
* Description    : lcd i80 send command
* Input          : u32 cmd : cmdcommand
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdMcuSendCmd(u32 cmd)
{
	LCD_MCU_RS_L;
	ax32xx_sysCpuNopDelay(200);
	R_LCD_CMMAND = cmd;
	while((R_LCD_STATUS & (1<<31)) == 0);
	LCD_MCU_RS_H;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdMcuSendData
* Description    : lcd i80 send data
* Input          : u32 data : data
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdMcuSendData(u32 data)
{
	LCD_MCU_RS_H;
	ax32xx_sysCpuNopDelay(200);
	R_LCD_CMMAND = data;
	while((R_LCD_STATUS & (1<<31)) == 0);	
}
/*******************************************************************************
* Function Name  : ax32xx_lcdMcuSendCmd
* Description    : lcd i80 send command
* Input          : u32 cmd : cmdcommand
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdMcuSendCmd16(u32 cmd)
{
	LCD_MCU_RS_L;
	ax32xx_sysCpuNopDelay(1200);
	R_LCD_CMMAND = cmd>>8;
	while((R_LCD_STATUS & (1<<31)) == 0);

	ax32xx_sysCpuNopDelay(200);
	R_LCD_CMMAND = cmd;
	while((R_LCD_STATUS & (1<<31)) == 0);
	
	LCD_MCU_RS_H;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdMcuSendData
* Description    : lcd i80 send data
* Input          : u32 data : data
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdMcuSendData16(u32 data)
{
	LCD_MCU_RS_H;
	ax32xx_sysCpuNopDelay(1200);
	R_LCD_CMMAND = data>>8;
	while((R_LCD_STATUS & (1<<31)) == 0);

	ax32xx_sysCpuNopDelay(200);
	R_LCD_CMMAND = data;
	while((R_LCD_STATUS & (1<<31)) == 0);
	LCD_MCU_RS_L;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdInit
* Description    : lcd initial
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdInit(void)
{
	ax32xx_sysClkSet(SYS_CLK_LCDC,1);
	ax32xx_sysCpuMsDelay(20);
	R_LCD_CTRL = 0;
	
	R_LCD_STATUS   = (0<<31)|//timing controller ready flag
			   (1<<23)|//fifo underflow int en
			   (0<<20)|//te interrupt en(mcu)
			   (0<<19)|//data end int en(mcu)
			   (0<<18)|//line end int en(rgb)
			   (0<<17)|//frame end int en(rgb)
			   (0<<16)|//frame start int en(rgb)
			   (1<<15)|//clear fifo underflow pending
			   (1<<12)|//clear te pending
			   (1<<11)|//clear data end pending
			   (1<<10)|//clear line end pending
			   (1<<9)| //clear frame end pending
			   (1<<8)| //clear frame start pending
			   (0<<7)| //fifo underflow pending
			   (0<<4)| //te pending
			   (0<<3)| //data end pending (mcu mode)
			   (0<<2)| //line end pending
			   (0<<1)| //frame end pending (rgb mode)
			   (0<<0); //frame start pending	
}
/*******************************************************************************
* Function Name  : ax32xx_lcdIRQEnable
* Description    : lcd IRQ enable,TE_IRQ table
* Input          : u8 itype : irq ttype
*                  u8 en    : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdIRQEnable(u8 itype,u8 en)
{
	if(en)
		R_LCD_STATUS |= 1<<itype;
	else
		R_LCD_STATUS &= ~(1<<itype);
}
/*******************************************************************************
* Function Name  : ax32xx_lcdPreLineSet
* Description    : lcd pre line conifg,for DE get ready
* Input          : u32 line : pre line
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdPreLineSet(u32 line)
{
	R_LCD_RLINE  = ((line - 1)<<0);//pre line
}
/*******************************************************************************
* Function Name  : ax32xx_lcdSignalSet
* Description    : lcd signal set for rgb mode
* Input          : u8 vsync : vsync inactive
*                  u8 inv_pclk : pclk invactive 1->inv,0->normarl
*                  u8 inv_de   : data enable invactive
*                  u8 inv_hs   : hsync invactive
*                  u8 inv_vs   : vsync invactive
*                  u8 en_de    : data en sigable enable
*                  u8 en_hs    : hsync signal enable
*                  u8 en_vs    : vcyns signal enable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSignalSet(u8 vsync,u8 inv_pclk,u8 inv_de,u8 inv_hs,u8 inv_vs,u8 en_de,u8 en_hs,u8 en_vs)
{
	R_LCD_SIGNAL  = (vsync<<16)|//vsync inactive timing
			        (inv_pclk<<7)| //pclk(wr) idle polarity
			        (inv_de<<6)| //de(cs) idle polarity
			        (inv_hs<<5)| //hsync idle polarity
			        (inv_vs<<4)| //vsync idle polarity
			        (en_de<<2)| //de(cs) en
			        (en_hs<<1)| //hsync en
			        (en_vs<<0); //vsync en
	R_LCD_SIGNAL0 = (en_de<<27)|//de(cs) output en
			        (en_hs<<26)|//hsync output en
			        (en_vs<<25)|//vsync output en
			        (1<<24);//pclk(wr) output en
}
/*******************************************************************************
* Function Name  : ax32xx_lcdBusWidth
* Description    : lcd bus width set
* Input          : u32 width : 
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdBusWidth(u32 width)
{
	R_LCD_SIGNAL0 &= ~0xffffff;
	R_LCD_SIGNAL0 |= ((1<<width)-1)&0xffffff; //data output en		
#if AX32_PLATFORM == FPGA
    //80pin LCD_D0 => USB1.1_DM
	R_LCD_SIGNAL0 &= ~1;
#endif
}
/*******************************************************************************
* Function Name  : ax32xx_lcdBusEnable
* Description    : lcd bus enable
* Input          : u32 mask : mask word
                      u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdBusEnable(u32 mask,u8 en)
{
	if(en)
		R_LCD_SIGNAL0 |= mask&0xffffff;
	else
		R_LCD_SIGNAL0 &= ~mask;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdClkSet
* Description    : lcd clk set
* Input          : u32 width : 
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdClkSet(u32 clk_div)
{
	R_LCD_CLKSEL = (0<<8)|//pclk tuning
			       ((clk_div-1)<<2)| //pclk div(max 63)
			       (0<<0);//pclk source
}
/*******************************************************************************
* Function Name  : ax32xx_lcdSyncSet
* Description    : lcd sync signal info
* Input          : u32 hsfp : hsync front phase
*                  u32 hsbp : hsync back phase
*                  u32 vsfp : vsync front phase
*                  u32 hsbp : vsync back phase
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSyncSet(u32 hsfp,u32 hsbp,u32 vsfp,u32 vsbp)
{
	R_LCD_VSYNCF = vsfp;
	R_LCD_VSYNCB = vsbp;
	R_LCD_HSYNCF = hsfp;
	R_LCD_HSYNCB = hsbp;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdDESignalSet
* Description    : lcd data enable signal info
* Input          : u32 defp : data front phase
*                  u32 debp : data back phase
*                  u32 destart : data start
*                  u32 deend : data end
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdDESignalSet(u32 defp,u32 debp,u32 destart,u32 deend)
{
	R_LCD_DEF = defp;
	R_LCD_DEB = debp;
	R_LCD_DESTART = destart;
	R_LCD_DEEND = deend;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdPositionSet
* Description    : lcd position
* Input          : u32 x : x
*                  u32 y : y
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdPositionSet(u32 x,u32 y)
{
	R_LCD_POSX = x;
	R_LCD_POSY = y;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdResolutionSet
* Description    : lcd reslution
* Input          : u32 width : width
*                  u32 height : height
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdResolutionSet(u32 width,u32 height)
{
	R_LCD_WIDTH = width;
	R_LCD_HEIGHT = height;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdWindowSizeSet
* Description    : lcd window size
* Input          : u32 width : width
*                  u32 height : height
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdWindowSizeSet(u32 width,u32 height)
{
	R_LCD_WINWIDTH = width;
	R_LCD_WINHEIGHT= height;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdDataModeSet
* Description    : lcd color coding,fifo mode,shfit
* Input          : u32 mode : mode
*                  u32 mode : mode1
*                  u8 odd_order : RGB order,line 1,3,5... 
*                  u8 even_order: RGB order,line 0,2,4...
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdDataModeSet(u32 mode,u32 mode1,u8 even_order,u8 odd_order)
{
	R_LCD_MODE   = mode;
	R_LCD_MODE1  = mode1;
	R_LCD_RGBORD = ((odd_order != even_order)<<6)|//odd_even mode en
                   (odd_order<<3)|//odd line rgb order
                   (even_order<<0);//even line rgb order
}
/*******************************************************************************
* Function Name  : ax32xx_lcdClkNumberSet
* Description    : lcd total pixel set
* Input          : u32 num : pixels
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdClkNumberSet(u32 num)
{
	R_LCD_NUMBER  = (num<<0);//how many pclk per frame(only mcu)
}
/*******************************************************************************
* Function Name  : ax32xx_lcdEndLineSet
* Description    : lcd enable line
* Input          : u32 eline : end line
*                  u32 aline : active line
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdEndLineSet(u32 eline,u32 aline)
{
	R_LCD_ENDLINE  = eline;//line end pending active line
	
	R_LCD_ACTIVELINE  = aline;//blanking signal active line	
}
/*******************************************************************************
* Function Name  : ax32xx_lcdPanelMode
* Description    : lcd interface type
* Input          : u8 panel : 0-RGB,1-MCU
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdPanelMode(u8 panel)
{
    R_LCD_CTRL = (R_LCD_CTRL & ~(1<<4)) | ((!!panel)<<4);
}
/*******************************************************************************
* Function Name  : ax32xx_lcdEnable
* Description    : lcd enable 
* Input          : u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdEnable(u8 en)
{
	if(en)
	{
		R_LCD_CTRL |= 1;
		ax32xx_intEnable(IRQ_LCDC,1);
	}
	else
	{
		R_LCD_CTRL &=~1;
		ax32xx_intEnable(IRQ_LCDC,0);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_lcdTeMode
* Description    : lcd TE MODE 
* Input          : u8 mode : mode
*                : u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdTeMode(u8 mode,u8 en)
{
	R_LCD_CTRL = (R_LCD_CTRL & ~((1<<7)|(1<<6)|(1<<5))) | ((mode&1)<<6)|((en&1)<<5);
}
/*******************************************************************************
* Function Name  : ax32xx_lcdKick
* Description    : lcd start frame show once
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
u8 lcdc_mcu_toggle_state = 0;
void ax32xx_lcdKick(void)
{
	if(0 == lcdc_mcu_toggle_state)
	R_LCD_CTRL |= (1<<1);
}
/*******************************************************************************
* Function Name  : ax32xx_lcdIRQHandler
* Description    : lcd IRQ Handler
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdIRQHandler(void)
{
	u32 lcdc_status = R_LCD_STATUS;
	
	//fifo underflow
	if(lcdc_status & (1<<7))
	{
		// while(1);
		R_LCD_STATUS |= (1<<15);debg("underflow\n");
	}
	
	//line end
	if(lcdc_status & (1<<2))
	{
		R_LCD_STATUS |= (1<<10);
		if(ax32xx_lcdISR[LCD_IRQ_LEND-LCD_IRQ_FSRT])
			  ax32xx_lcdISR[LCD_IRQ_LEND-LCD_IRQ_FSRT]();
	}

	//frame end
	if(lcdc_status & (1<<1))
	{
		R_LCD_STATUS |= (1<<9);
		if(ax32xx_lcdISR[LCD_IRQ_FEND-LCD_IRQ_FSRT])
			  ax32xx_lcdISR[LCD_IRQ_FEND-LCD_IRQ_FSRT]();
	}

	//data end
	if(lcdc_status & (1<<3))
	{
		R_LCD_STATUS |= (1<<11);
		if(ax32xx_lcdISR[LCD_IRQ_DEND-LCD_IRQ_FSRT])
			ax32xx_lcdISR[LCD_IRQ_DEND-LCD_IRQ_FSRT]();
        
        if((R_LCD_CTRL & (3<<4)) == (1<<4))
        {
            ax32xx_lcdKick();
        }
	}

	//frame start
	if(lcdc_status & (1<<0))
	{
		//debgchar('0');
		R_LCD_STATUS |= (1<<8);
		if(ax32xx_lcdISR[LCD_IRQ_FSRT-LCD_IRQ_FSRT])
			  ax32xx_lcdISR[LCD_IRQ_FSRT-LCD_IRQ_FSRT]();
	}
	
	//te
	if((lcdc_status & ((1<<4)|(1<<20))) == ((1<<4)|(1<<20)))
	{
		R_LCD_STATUS |= (1<<12);
        if(ax32xx_lcdISR[LCD_IRQ_TE-LCD_IRQ_FSRT])
            ax32xx_lcdISR[LCD_IRQ_TE-LCD_IRQ_FSRT]();
	}
}
/*******************************************************************************
* Function Name  : ax32xx_lcdISRRegister
* Description    : lcd isr register
* Input          : u8 type : irq .LCD_IRQ_FSRT~LCD_IRQ_TE
                      void (*isr)(void) : irq callback
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdISRRegister(u8 type,void (*isr)(void))
{
	if((type>LCD_IRQ_TE) || (type<LCD_IRQ_FSRT))
		return ;
	ax32xx_lcdISR[type-LCD_IRQ_FSRT] = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdReset
* Description    : lcd reset panel
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdReset(void)
{
	ax32xx_gpioMapSet(GPIO_PB,GPIO_PIN4,GPIO_FUNC_GPIO);
	ax32xx_gpioDirSet(GPIO_PB,GPIO_PIN4,GPIO_OUTPUT);
    ax32xx_gpioDataSet(GPIO_PB,GPIO_PIN4,GPIO_LOW);
	ax32xx_sysCpuMsDelay(50);
	ax32xx_gpioDataSet(GPIO_PB,GPIO_PIN4,GPIO_HIGH);
	ax32xx_sysCpuMsDelay(10);	
}
/*******************************************************************************
* Function Name  : ax32xx_lcdMCUIOConfig
* Description    : lcd io configure for i80
* Input          : u8 bus_width : bus width
*                  u8 te_enable : te enable
* Output         : None
* Return         : None
*******************************************************************************/ 
s32 ax32xx_lcdMCUIOConfig(u8 bus_width,u8 te_enable)
{
    u32 pins = 0;
    
	ax32xx_gpioSFRSet(GPIO_MAP_LCD,LCD_POS);
    
    if(LCD_POS == LCD_POS9)
    {
        //D[23:0]={PF[6:0],PD[14:0],PB[6]},CLK=PF7,RS=PA5,CS=PA4,TE=PF9
        ax32xx_gpioDigitalSet(GPIO_PB,GPIO_PIN6,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PB,GPIO_PIN6,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PB,GPIO_PIN6,GPIO_FUNC_SFR);
        
        pins = GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1|GPIO_PIN0;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);

        pins = GPIO_PIN7;
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);

        pins = GPIO_PIN5|GPIO_PIN4;  //cs
        ax32xx_gpioDigitalSet(GPIO_PA,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PA,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PA,pins,GPIO_FUNC_SFR);
        ax32xx_gpioMapSet    (GPIO_PA,GPIO_PIN5,GPIO_FUNC_GPIO);
        
        if(te_enable)
        {
            pins = GPIO_PIN9;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_INPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        
        if(bus_width == 8)
            return 0;
        else if(bus_width == 24)
        {
            pins = GPIO_PIN14|GPIO_PIN13|GPIO_PIN12|GPIO_PIN11|GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7;
            ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);
            
            pins = GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1|GPIO_PIN0;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        else
            return -1;
	}
	else if(LCD_POS == LCD_POS14){
		//D[7:0]={PD[13:6]},CLK/WR=PF7,RS=PF8,CS=PA4,TE=PF9
        
		pins = GPIO_PIN13|GPIO_PIN12|GPIO_PIN11|GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7|GPIO_PIN6;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);
		//PDDRV = 0xffff;
		
        pins = GPIO_PIN7;  //WR
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);

		
		pins = GPIO_PIN8; //RS
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
		//ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_GPIO);
		//PFDRV = 0xffff;

        pins = GPIO_PIN4;  //cs
        ax32xx_gpioDigitalSet(GPIO_PA,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PA,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PA,pins,GPIO_FUNC_SFR);
		 
		
        if(te_enable)
        {
            pins = GPIO_PIN9; //TE
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_INPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        
        if(bus_width == 8)
            return 0;
        else
            return -1;
    }
    else if(LCD_POS == LCD_POS7)
    {
        //lcd[7:0] map to PD[10:5] PB[6:5]
		pins = GPIO_PIN5|GPIO_PIN6;
        ax32xx_gpioDigitalSet(GPIO_PB,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet(GPIO_PB,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet(GPIO_PB,pins,GPIO_FUNC_SFR);
		
        pins = GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7|GPIO_PIN6|GPIO_PIN5;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet(GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet(GPIO_PD,pins,GPIO_FUNC_SFR);

        pins = GPIO_PIN1|GPIO_PIN2|GPIO_PIN3;
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet(GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet(GPIO_PF,pins,GPIO_FUNC_SFR);

		pins = GPIO_PIN4;
	    ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
	    ax32xx_gpioDirSet(GPIO_PF,pins,GPIO_OUTPUT);
	    ax32xx_gpioMapSet(GPIO_PF,pins,GPIO_FUNC_GPIO);
	    ax32xx_gpioPullSet(GPIO_PF,pins,GPIO_PULL_DOWN);// pull down LCD_CS pin to make it stable
        
        if(te_enable!=0xff) //TE
        {
            pins = GPIO_PIN3;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet(GPIO_PF,pins,GPIO_INPUT);
            ax32xx_gpioMapSet(GPIO_PF,pins,GPIO_FUNC_SFR);
        }
		 else
		 {
	            pins = GPIO_PIN3;
	            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
	            ax32xx_gpioDirSet(GPIO_PF,pins,GPIO_OUTPUT);
	            ax32xx_gpioMapSet(GPIO_PF,pins,GPIO_FUNC_GPIO);
		     	ax32xx_gpioDataSet(GPIO_PF,pins,GPIO_HIGH);
		 }
        if(bus_width == 8)
            return 0;
 		else
            return -1;
    }
	else if(LCD_POS == LCD_POS12)
    {
         	//D[7:0]={PD[12:5]},CLK/WR=PF1,HS/RS=PF2,DE/CS=PF4,VS/TE=PF3
        
        pins = GPIO_PIN12|GPIO_PIN11|GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7|GPIO_PIN6|GPIO_PIN5;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);

        pins = GPIO_PIN1|GPIO_PIN2|GPIO_PIN4;  //CLK //HS//DE
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        
        if(te_enable)
        {
            pins = GPIO_PIN3;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_INPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        
        if(bus_width == 8)
            return 0;
        else 
        {
			debg("##not support lcd bus_width##\n");
			return -1;
        }
    }
    else
        return -1;			

    return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_lcdMCUIOConfig
* Description    : lcd io configure for rgb
* Input          : u8 bus_width : bus width
*                  u8 de_enable : data enable
* Output         : None
* Return         : None
*******************************************************************************/ 
s32 ax32xx_lcdRGBIOConfig(u8 bus_width,u8 de_enable)
{
    u32 pins = 0;
    
	ax32xx_gpioSFRSet(GPIO_MAP_LCD,LCD_POS);
    
    if(LCD_POS == LCD_POS9)
    {
        //D[23:0]={PF[6:0],PD[14:0],PB[6]},CLK=PF7,HS=PF8,VS=PF9,DE=PA4
        //ax32xx_gpioDigitalSet(GPIO_PB,GPIO_PIN6,GPIO_DIGITAL_EN);
        //ax32xx_gpioDirSet    (GPIO_PB,GPIO_PIN6,GPIO_OUTPUT);
        //ax32xx_gpioMapSet    (GPIO_PB,GPIO_PIN6,GPIO_FUNC_SFR);
        
        pins = GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1|GPIO_PIN0;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);

        pins = GPIO_PIN9|GPIO_PIN8|GPIO_PIN7;
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        
        if(de_enable) //de enable
        {
            pins = GPIO_PIN4;
            ax32xx_gpioDigitalSet(GPIO_PA,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PA,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PA,pins,GPIO_FUNC_SFR);
        }
        
        if(bus_width == 8)
            return 0;
        else if(bus_width == 24)
        {
            pins = GPIO_PIN14|GPIO_PIN13|GPIO_PIN12|GPIO_PIN11|GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7;
            ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);
            
            pins = GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1|GPIO_PIN0;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        else
            return -1;
    }
	else if(LCD_POS == LCD_POS7)  
    {
        //D[23:0]={PF[6:0],PD[14:0],PB[6]},CLK=PF7,HS=PF8,VS=PF9,DE=PA4
        //ax32xx_gpioDigitalSet(GPIO_PB,GPIO_PIN6,GPIO_DIGITAL_EN);
        //ax32xx_gpioDirSet    (GPIO_PB,GPIO_PIN6,GPIO_OUTPUT);
        //ax32xx_gpioMapSet    (GPIO_PB,GPIO_PIN6,GPIO_FUNC_SFR);
		pins = GPIO_PIN5|GPIO_PIN6;
        ax32xx_gpioDigitalSet(GPIO_PB,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PB,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PB,pins,GPIO_FUNC_SFR);
		
		
        pins = GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7|GPIO_PIN6|GPIO_PIN5;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);

        pins = GPIO_PIN1|GPIO_PIN2|GPIO_PIN3;
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        
        if(de_enable) //de enable
        {
            pins = GPIO_PIN4;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        
        if(bus_width == 8)
            return 0;
        else if(bus_width == 24)
        {
            pins = GPIO_PIN14|GPIO_PIN13|GPIO_PIN12|GPIO_PIN11|GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7;
            ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);
            
            pins = GPIO_PIN6|GPIO_PIN5|GPIO_PIN4|GPIO_PIN3|GPIO_PIN2|GPIO_PIN1|GPIO_PIN0;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        else
            return -1;
    }
	if(LCD_POS == LCD_POS12)
    {
       //D[7:0]={PD[12:5]},CLK/WR=PF1,HS/RS=PF2,DE/CS=PF4,VS/TE=PF3
        
        pins = GPIO_PIN12|GPIO_PIN11|GPIO_PIN10|GPIO_PIN9|GPIO_PIN8|GPIO_PIN7|GPIO_PIN6|GPIO_PIN5;
        ax32xx_gpioDigitalSet(GPIO_PD,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PD,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PD,pins,GPIO_FUNC_SFR);

        pins = GPIO_PIN1|GPIO_PIN2|GPIO_PIN3; //clk//hs//vs
        ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
        ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
        ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        
        if(de_enable) //de enable
        {
            pins = GPIO_PIN4;
            ax32xx_gpioDigitalSet(GPIO_PF,pins,GPIO_DIGITAL_EN);
            ax32xx_gpioDirSet    (GPIO_PF,pins,GPIO_OUTPUT);
            ax32xx_gpioMapSet    (GPIO_PF,pins,GPIO_FUNC_SFR);
        }
        
        if(bus_width == 8)
            return 0;
        else
            return -1;
    }
    else
        return -1;			

    return 0;
}

/*
void ax32xx_lcdPrint(void)
{
	debg("R_LCD_CTRL = %x\n",R_LCD_CTRL);
	debg("R_LCD_STATUS = %x\n",R_LCD_STATUS);
	debg("R_LCD_COMMAND = %x\n",R_LCD_COMMAND);
	debg("R_LCD_RLINE = %x\n",R_LCD_RLINE);
	debg("R_LCD_SIGNAL = %x\n",R_LCD_SIGNAL);
	debg("R_LCD_SIGNAL0 = %x\n",R_LCD_SIGNAL0);
	debg("R_LCD_VSYNCF = %x\n",R_LCD_VSYNCF);
	debg("R_LCD_HSYNCF = %x\n",R_LCD_HSYNCF);
	debg("R_LCD_VSYNCB = %x\n",R_LCD_VSYNCB);
	debg("R_LCD_HSYNCB = %x\n",R_LCD_HSYNCB);
	debg("R_LCD_DEF = %x\n",R_LCD_DEF);
	debg("R_LCD_DEB = %x\n",R_LCD_DEB);
	debg("R_LCD_DESTART = %x\n",R_LCD_DESTART);
	debg("R_LCD_DEEND = %x\n",R_LCD_DEEND);
	debg("R_LCD_POSX = %x\n",R_LCD_POSX);
	debg("R_LCD_POSY = %x\n",R_LCD_POSY);
	debg("R_LCD_WIDTH = %x\n",R_LCD_WIDTH);
	debg("R_LCD_HEIGHT = %x\n",R_LCD_HEIGHT);
	debg("R_LCD_WINWIDTH = %x\n",R_LCD_WINWIDTH);
	debg("R_LCD_WINHEIGHT = %x\n",R_LCD_WINHEIGHT);
	debg("R_LCD_MODE = %x\n",R_LCD_MODE);
	debg("R_LCD_CSSTATUS = %x\n",R_LCD_CSSTATUS);
	debg("R_LCD_ENDLINE = %x\n",R_LCD_ENDLINE);
	debg("R_LCD_ACTIVELINE = %x\n",R_LCD_ACTIVELINE);
	debg("R_LCD_NUMBER = %x\n",R_LCD_NUMBER);
	debg("R_LCD_CLKSEL = %x\n",R_LCD_CLKSEL);

}*/

/*******************************************************************************
* Function Name  : ax32xx_lcd_lsawbtooth
* Description    : anti-aliasing on lcd screen
* Input          : u32 *anti_lsawtooth
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcd_lsawbtooth(u32 *anti_lsawtooth)
{
	ISP_LS_HTAB0 = anti_lsawtooth[0];
    ISP_LS_HTAB1 = anti_lsawtooth[1];
    ISP_LS_HTAB2 = anti_lsawtooth[2];
    ISP_LS_HTAB3 = anti_lsawtooth[3];
    ISP_LS_HTAB4 = anti_lsawtooth[4];
    ISP_LS_HTAB5 = anti_lsawtooth[5];
    ISP_LS_HTAB6 = anti_lsawtooth[6];
    ISP_LS_HTAB7 = anti_lsawtooth[7];
    ISP_LS_HTAB8 = anti_lsawtooth[8];
    ISP_LS_HTAB9 = anti_lsawtooth[9];
    ISP_LS_HTAB10= anti_lsawtooth[10];
    ISP_LS_HTAB11= anti_lsawtooth[11];
    ISP_LS_HTAB12= anti_lsawtooth[12];
    ISP_LS_HTAB13= anti_lsawtooth[13];
    ISP_LS_HTAB14= anti_lsawtooth[14];
    ISP_LS_HTAB15= anti_lsawtooth[15];//*/
    		
	ISP_LS_VTAB0 = anti_lsawtooth[16];
    ISP_LS_VTAB1 = anti_lsawtooth[17];
    ISP_LS_VTAB2 = anti_lsawtooth[18];
    ISP_LS_VTAB3 = anti_lsawtooth[19];
    ISP_LS_VTAB4 = anti_lsawtooth[20];
    ISP_LS_VTAB5 = anti_lsawtooth[21];
    ISP_LS_VTAB6 = anti_lsawtooth[22];
    ISP_LS_VTAB7 = anti_lsawtooth[23];
	ISPCTRL |= ISP_CTRL_LS_KS;
}


