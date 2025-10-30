/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                  THE APPOTECH HAL
   **** **                 EMI HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_emi.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN EMI HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#if 0
#include "../inc/hal.h"

static u8 tx_data[] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,
    0x80,0x90,0xA0,0xB0,0xC0,0xD0,0xE0,0xF0,
};

static void emi_tx_done(u32 header)
{
    debg("tx done\n");
}

void hal_emiMasterTest(void)
{
    ax32xx_emiInit(EMI_MASTER,2000000);
    ax32xx_emiISRRegister(emi_tx_done);
    ax32xx_emiKick(0x5,tx_data,sizeof(tx_data));
    LEAVE();
}

static u8 rx_data[1024];
static void emi_rx_done(u32 header)
{
    ax32xx_emiKick(0,rx_data,sizeof(tx_data));
    debgreg(header);
    debgbuf(rx_data,64);
}

void hal_emiSlaveTest(void)
{
    ax32xx_emiInit(EMI_SLAVE,0);
    ax32xx_emiISRRegister(emi_rx_done);
    ax32xx_emiKick(0,rx_data,sizeof(tx_data));
    LEAVE();
}
#endif






