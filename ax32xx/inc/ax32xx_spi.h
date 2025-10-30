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
* File Name   : ax32xx_spi.h
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
#ifndef AX32XX_SPI_H
    #define AX32XX_SPI_H

#define  SPI0            0
#define  SPI1            1

/*******************************************************************************
* Function Name  : ax32xx_spi0Init
* Description	 : spi0 initial
* Input 		 : u32 u32SPIBaud : spi0 baudrate set
*                  u8 bus_mode : SPI0_1_LINE/SPI0_2_LINE0/SPI0_2_LINE1/SPI0_4_LINE
* Output		 : None
* Return		 : none 
*******************************************************************************/
void ax32xx_spi0Init(u32 u32SPIBaud,u8 bus_mode);
/*******************************************************************************
* Function Name  : ax32xx_spi0SendByte
* Description	 : spi0 send one byte
* Input 		 : u8 byte : data
* Output		 : None
* Return		 : u8 : 
*******************************************************************************/
void ax32xx_spi0SendByte(u8 byte);
/*******************************************************************************
* Function Name  : ax32xx_spi0RecvByte
* Description	 : spi0 recv one byte
* Input 		 : none
* Output		 : None
* Return		 : u8 : 
*******************************************************************************/
u8 ax32xx_spi0RecvByte(void);
/*******************************************************************************
* Function Name  : ax32xx_spi0Send0
* Description	 : spi0 send date by DMA
* Input 		 : u32 pDataBuf : data buffer
                   u32 u32DataLen : data len,>=8
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_spi0Send0(void *pDataBuf,u32 u32DataLen);
/*******************************************************************************
* Function Name  : ax32xx_spi0Recv0
* Description	 : spi0 recv date by DMA
* Input 		 : u32 pDataBuf : data buffer
                   u32 u32DataLen : data len,>=8
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_spi0Recv0(void *pDataBuf,u32 u32DataLen);
/*******************************************************************************
* Function Name  : ax32xx_spi0CS0Config
* Description	 : spi0 cs(flash) config
* Input 		 : u8 level : 0:cs is low
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_spi0CS0Config(u8 level);


/*******************************************************************************
* Function Name  : hal_spi1Init
* Description	 : spi1 init
* Input 		 : u8 bus_mode : 0:3 LINE(DIN, DOUT,CLK) ,1 :2 LINE(CLK, DIN/DOUT)
*                  SPIBAUD 
* Output		 : None
* Return		 : none 
*******************************************************************************/
void hal_spi1Init(u32 u32SPIBaud,u8 bus_mode);

void spi1_pin_config(u8 bus);

void spi1_send_byte(u8 u8Data);

u8 spi1_receive_byte(void);

BOOL spi1_recevie_dma(u8 *pDataBuf,u32 u32DataLen);
BOOL spi1_send_dma(u8 *pDataBuf,u32 u32DataLen);
void spi1_enable(bool enable);





#endif
