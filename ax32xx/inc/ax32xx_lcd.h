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
* File Name   : ax32xx_lcd.h
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
#ifndef AX32XX_LCD_H
#define AX32XX_LCD_H

#define  LCD_BUS_I80         1    // i80-mcu-cpu
#define  LCD_BUS_RGB         0   // rgb ,srgb,prgb


#define  LCD_BUS_WIDTH_8   8   // 8-bit p
#define  LCD_BUS_WIDTH_6   6   // 8-bit p
#define  LCD_BUS_WIDTH_5   5   // 8-bit p
#define  LCD_BUS_WIDTH_4   4   // 8-bit p

#define  LCD_IRQ_TE       20
#define  LCD_IRQ_DEND    19
#define  LCD_IRQ_LEND    18
#define  LCD_IRQ_FEND    17
#define  LCD_IRQ_FSRT    16
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
void ax32xx_lcdSPIMode(u8 cpol,u8 cpha,u8 order,u8 bits);
/*******************************************************************************
* Function Name  : ax32xx_lcdSPIInit
* Description    : lcd spi initial GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSPIInit(void);
/*******************************************************************************
* Function Name  : ax32xx_lcdSPIUninit
* Description    : lcd spi uninitial release GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSPIUninit(void);
/*******************************************************************************
* Function Name  : ax32xx_lcdSPISendData
* Description    : lcd spi send data
* Input          : u32 data : data
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdSPISendData(u32 data);
/*******************************************************************************
* Function Name  : ax32xx_lcdMcuSendCmd
* Description    : lcd i80 send command
* Input          : u32 cmd : cmdcommand
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdMcuSendCmd(u32 cmd);
/*******************************************************************************
* Function Name  : ax32xx_lcdMcuSendData
* Description    : lcd i80 send data
* Input          : u32 data : data
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdMcuSendData(u32 data);
/*******************************************************************************
* Function Name  : ax32xx_lcdMcuSendCmd
* Description    : lcd i80 send command
* Input          : u32 cmd : cmdcommand
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdMcuSendCmd16(u32 cmd);
/*******************************************************************************
* Function Name  : ax32xx_lcdMcuSendData
* Description    : lcd i80 send data
* Input          : u32 data : data
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdMcuSendData16(u32 data);




/*******************************************************************************
* Function Name  : ax32xx_lcdInit
* Description    : lcd initial
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdInit(void);
/*******************************************************************************
* Function Name  : ax32xx_lcdIRQEnable
* Description    : lcd IRQ enable,TE_IRQ table
* Input          : u8 itype : irq ttype
*                  u8 en    : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdIRQEnable(u8 itype,u8 en);
/*******************************************************************************
* Function Name  : ax32xx_lcdPreLineSet
* Description    : lcd line width set
* Input          : u32 line : pre line
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdPreLineSet(u32 line);
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
void ax32xx_lcdSignalSet(u8 vsync,u8 inv_pclk,u8 inv_de,u8 inv_hs,u8 inv_vs,u8 en_de,u8 en_hs,u8 en_vs);
/*******************************************************************************
* Function Name  : ax32xx_lcdBusWidth
* Description    : lcd bus width set
* Input          : u32 width : 
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdBusWidth(u32 width);
/*******************************************************************************
* Function Name  : ax32xx_lcdBusEnable
* Description    : lcd bus enable
* Input          : u32 mask : mask word
                      u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdBusEnable(u32 mask,u8 en);
/*******************************************************************************
* Function Name  : ax32xx_lcdClkSet
* Description    : lcd clk set
* Input          : u32 width : 
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdClkSet(u32 clk_div);
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
void ax32xx_lcdSyncSet(u32 hsfp,u32 hsbp,u32 vsfp,u32 vsbp);
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
void ax32xx_lcdDESignalSet(u32 defp,u32 debp,u32 destart,u32 deend);
/*******************************************************************************
* Function Name  : ax32xx_lcdPositionSet
* Description    : lcd position
* Input          : u32 x : x
*                  u32 y : y
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdPositionSet(u32 x,u32 y);
/*******************************************************************************
* Function Name  : ax32xx_lcdResolutionSet
* Description    : lcd reslution
* Input          : u32 width : width
*                  u32 height : height
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdResolutionSet(u32 width,u32 height);
/*******************************************************************************
* Function Name  : ax32xx_lcdWindowSizeSet
* Description    : lcd window size
* Input          : u32 width : width
*                  u32 height : height
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdWindowSizeSet(u32 width,u32 height);
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
void ax32xx_lcdDataModeSet(u32 mode,u32 mode1,u8 even_order,u8 odd_order);
/*******************************************************************************
* Function Name  : ax32xx_lcdClkNumberSet
* Description    : lcd total pixel set
* Input          : u32 num : pixels
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdClkNumberSet(u32 num);
/*******************************************************************************
* Function Name  : ax32xx_lcdEndLineSet
* Description    : lcd enable line
* Input          : u32 eline : end line
*                  u32 aline : active line
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdEndLineSet(u32 eline,u32 aline);
/*******************************************************************************
* Function Name  : ax32xx_lcdEnable
* Description    : lcd enable 
* Input          : u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_lcdTeMode
* Description    : lcd TE MODE 
* Input          : u8 mode : mode
*                : u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdTeMode(u8 mode,u8 en);
/*******************************************************************************
* Function Name  : ax32xx_lcdKick
* Description    : lcd start frame show once
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdKick(void);
/*******************************************************************************
* Function Name  : ax32xx_lcdIRQHandler
* Description    : lcd IRQ Handler
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_lcdISRRegister
* Description    : lcd isr register
* Input          : u8 type : irq .LCD_IRQ_FSRT~LCD_IRQ_TE
                      void (*isr)(void) : irq callback
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdISRRegister(u8 type,void (*isr)(void));
/*******************************************************************************
* Function Name  : ax32xx_lcdMCUIOConfig
* Description    : lcd io configure for i80
* Input          : u8 bus_width : bus width
*                  u8 te_enable : te enable
* Output         : None
* Return         : None
*******************************************************************************/ 
s32 ax32xx_lcdMCUIOConfig(u8 bus_width,u8 te_enable);
/*******************************************************************************
* Function Name  : ax32xx_lcdMCUIOConfig
* Description    : lcd io configure for rgb
* Input          : u8 bus_width : bus width
*                  u8 de_enable : data enable
* Output         : None
* Return         : None
*******************************************************************************/ 
s32 ax32xx_lcdRGBIOConfig(u8 bus_width,u8 de_enable);
/*******************************************************************************
* Function Name  : ax32xx_lcdPanelMode
* Description    : lcd interface type
* Input          : u8 panel : 0-RGB,1-MCU
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcdPanelMode(u8 panel);

/*******************************************************************************
* Function Name  : ax32xx_lcd_lsawbtooth
* Description    : anti-aliasing on lcd screen
* Input          : u32 *anti_lsawtooth
*                  
* Output         : None
* Return         : None
*******************************************************************************/ 
void ax32xx_lcd_lsawbtooth(u32 *anti_lsawtooth);


#endif
