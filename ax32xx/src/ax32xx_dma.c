/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX DMA
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_dma.c
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
#include "../inc/ax32xx.h"

#define  R_DMA_CTRL0    NOC_REG0
static volatile u8 dmaCopyFlag;

const noc_cfg_t default_cfg = {
.NOC_SDC0_QOS_STEP               = 1,
.NOC_SDC0_LATENCY                = 0,
.NOC_DE_QOS_STEP                 = 2,
.NOC_DE_LATENCY                  = 2500,
.NOC_BACK_CODEC_WR2CARD_QOS_STEP = 2,
.NOC_BACK_CODEC_WR2CARD_LATENCY  = 2500,
.NOC_BACK_CODEC_RD_QOS_STEP      = 2,
.NOC_BACK_CODEC_RD_LATENCY       = 2500,
.NOC_BACK_CODEC_WR2LCD_QOS_STEP  = 2,
.NOC_BACK_CODEC_WR2LCD_LATENCY   = 5000,
.NOC_PRE_CODEC_WR2CARD_QOS_STEP  = 2,
.NOC_PRE_CODEC_WR2CARD_LATENCY   = 5000,
.NOC_PRE_CODEC_RD_QOS_STEP       = 2,
.NOC_PRE_CODEC_RD_LATENCY        = 5000,
.NOC_ISP_WR2CARD_QOS_STEP        = 1,
.NOC_ISP_WR2CARD_LATENCY         = 0,
.NOC_ISP_WR2WIFI_QOS_STEP        = 1,
.NOC_ISP_WR2WIFI_LATENCY         = 0,
.NOC_ISP_WR2LCD_QOS_STEP         = 1,
.NOC_ISP_WR2LCD_LATENCY          = 0,
.NOC_DCACHE_QOS_STEP             = 1,
.NOC_DCACHE_LATENCY              = 0,
.NOC_ICACHE_QOS_STEP             = 1,
.NOC_ICACHE_LATENCY              = 0,
};
//const noc_cfg_t prejpeg_prior = {
//.NOC_SDC0_QOS_STEP               = 2,
//.NOC_SDC0_LATENCY                = 5000,
//.NOC_DE_QOS_STEP                 = 3,
//.NOC_DE_LATENCY                  = 2500,
//.NOC_BACK_CODEC_WR2CARD_QOS_STEP = 1,
//.NOC_BACK_CODEC_WR2CARD_LATENCY  = 0,
//.NOC_BACK_CODEC_RD_QOS_STEP      = 2,
//.NOC_BACK_CODEC_RD_LATENCY       = 2500,
//.NOC_BACK_CODEC_WR2LCD_QOS_STEP  = 2,
//.NOC_BACK_CODEC_WR2LCD_LATENCY   = 2500,
//.NOC_PRE_CODEC_WR2CARD_QOS_STEP  = 1,
//.NOC_PRE_CODEC_WR2CARD_LATENCY   = 0,
//.NOC_PRE_CODEC_RD_QOS_STEP       = 1,
//.NOC_PRE_CODEC_RD_LATENCY        = 0,
//.NOC_ISP_WR2CARD_QOS_STEP        = 1,
//.NOC_ISP_WR2CARD_LATENCY         = 0,
//.NOC_ISP_WR2WIFI_QOS_STEP        = 1,
//.NOC_ISP_WR2WIFI_LATENCY         = 0,
//.NOC_ISP_WR2LCD_QOS_STEP         = 2,
//.NOC_ISP_WR2LCD_LATENCY          = 2500,
//.NOC_DCACHE_QOS_STEP             = 1,
//.NOC_DCACHE_LATENCY              = 0,
//.NOC_ICACHE_QOS_STEP             = 1,
//.NOC_ICACHE_LATENCY              = 0,
//};

void ax32xx_dmaNocDefault(void)
{
    ax32xx_dmaNocCfg((noc_cfg_t *)&default_cfg);
}

void ax32xx_dmaNocCfg(noc_cfg_t * p_cfg)
{
////////////////////////////Register
////SDC0
u32  NOC_SDC0_QOS_STEP = p_cfg->NOC_SDC0_QOS_STEP;
u32  NOC_SDC0_LATENCY  = p_cfg->NOC_SDC0_LATENCY;
////DE
u32  NOC_DE_QOS_STEP = p_cfg->NOC_DE_QOS_STEP;
u32  NOC_DE_LATENCY  = p_cfg->NOC_DE_LATENCY;
////back jpeg codec write to card
u32  NOC_BACK_CODEC_WR2CARD_QOS_STEP = p_cfg->NOC_BACK_CODEC_WR2CARD_QOS_STEP;
u32  NOC_BACK_CODEC_WR2CARD_LATENCY  = p_cfg->NOC_BACK_CODEC_WR2CARD_LATENCY;
////back jpeg codec read
u32  NOC_BACK_CODEC_RD_QOS_STEP = p_cfg->NOC_BACK_CODEC_RD_QOS_STEP;
u32  NOC_BACK_CODEC_RD_LATENCY  = p_cfg->NOC_BACK_CODEC_RD_LATENCY;
////back jpeg codec write to lcd
u32  NOC_BACK_CODEC_WR2LCD_QOS_STEP = p_cfg->NOC_BACK_CODEC_WR2LCD_QOS_STEP;
u32  NOC_BACK_CODEC_WR2LCD_LATENCY  = p_cfg->NOC_BACK_CODEC_WR2LCD_LATENCY;
////pre jpeg codec write to card
u32  NOC_PRE_CODEC_WR2CARD_QOS_STEP = p_cfg->NOC_PRE_CODEC_WR2CARD_QOS_STEP;
u32  NOC_PRE_CODEC_WR2CARD_LATENCY  = p_cfg->NOC_PRE_CODEC_WR2CARD_LATENCY;
////pre jpeg codec read
u32  NOC_PRE_CODEC_RD_QOS_STEP = p_cfg->NOC_PRE_CODEC_RD_QOS_STEP;
u32  NOC_PRE_CODEC_RD_LATENCY  = p_cfg->NOC_PRE_CODEC_RD_LATENCY;
////ISP write to card
u32  NOC_ISP_WR2CARD_QOS_STEP = p_cfg->NOC_ISP_WR2CARD_QOS_STEP;
u32  NOC_ISP_WR2CARD_LATENCY  = p_cfg->NOC_ISP_WR2CARD_LATENCY;
////ISP write to WIFI
u32  NOC_ISP_WR2WIFI_QOS_STEP = p_cfg->NOC_ISP_WR2WIFI_QOS_STEP;
u32  NOC_ISP_WR2WIFI_LATENCY  = p_cfg->NOC_ISP_WR2WIFI_LATENCY;
////ISP write to LCD
u32  NOC_ISP_WR2LCD_QOS_STEP = p_cfg->NOC_ISP_WR2LCD_QOS_STEP;
u32  NOC_ISP_WR2LCD_LATENCY  = p_cfg->NOC_ISP_WR2LCD_LATENCY;
////DCACHE
u32  NOC_DCACHE_QOS_STEP = p_cfg->NOC_DCACHE_QOS_STEP;
u32  NOC_DCACHE_LATENCY  = p_cfg->NOC_DCACHE_LATENCY;
////ICACHE
u32  NOC_ICACHE_QOS_STEP = p_cfg->NOC_ICACHE_QOS_STEP;
u32  NOC_ICACHE_LATENCY  = p_cfg->NOC_ICACHE_LATENCY;


////////////////////////////Configure the NOC
//channel 4
//channel 5
//channel 6
//channel 7
//channel 8
//channel 9
//channel 10
//channel 11
//channel 12//sdc0
u32  CH12_QOS_MAX = 0x1F & 30;
u32  CH12_QOS_MIN = 0x1F & (CH12_QOS_MAX-NOC_SDC0_QOS_STEP+1);
u32  CH12_LATENCY = NOC_SDC0_LATENCY & 0xFFFF;
NOC_REG9  =  (CH12_LATENCY<<16)|
             (CH12_QOS_MAX<<8) |
             (CH12_QOS_MIN<<3) | 1;
//channel 13
//channel 14
//channel 15
//channel 16
u32  CH16_QOS_MAX = 0x1F & 16 ;
u32  CH16_QOS_MIN = 0x1F & (CH16_QOS_MAX-NOC_ICACHE_QOS_STEP+1);
u32  CH16_LATENCY = NOC_ICACHE_LATENCY & 0xFFFF;
NOC_REG13  = (CH16_LATENCY<<16) |
             (CH16_QOS_MAX<<8) |
             (CH16_QOS_MIN<<3) | 1;

//channel 17
u32  CH17_QOS_MAX = 0x1F & 17 ;
u32  CH17_QOS_MIN = 0x1F & (CH17_QOS_MAX-NOC_DCACHE_QOS_STEP+1);
u32  CH17_LATENCY = NOC_DCACHE_LATENCY & 0xFFFF;
NOC_REG14  = (CH17_LATENCY<<16) |
             (CH17_QOS_MAX<<8) |
             (CH17_QOS_MIN<<3) | 1;

//channel 18
//channel 19
u32  CH19_QOS_MAX = 0x1F & 19 ;
u32  CH19_QOS_MIN = 0x1F & (CH19_QOS_MAX-NOC_ISP_WR2WIFI_QOS_STEP+1);
u32  CH19_LATENCY = NOC_ISP_WR2WIFI_LATENCY & 0xFFFF;
NOC_REG16  = (CH19_LATENCY<<16) |
             (CH19_QOS_MAX<<8) |
             (CH19_QOS_MIN<<3) | 1; 

//channel 20
u32  CH20_QOS_MAX = 0x1F & 20 ;
u32  CH20_QOS_MIN = 0x1F & (CH20_QOS_MAX-NOC_PRE_CODEC_WR2CARD_QOS_STEP+1);
u32  CH20_LATENCY = NOC_PRE_CODEC_WR2CARD_LATENCY & 0xFFFF;
NOC_REG17  = (CH20_LATENCY<<16) |
             (CH20_QOS_MAX<<8) |
             (CH20_QOS_MIN<<3) | 1; 

//channel 21
u32  CH21_QOS_MAX = 0x1F & 21 ;
u32  CH21_QOS_MIN = 0x1F & (CH21_QOS_MAX-NOC_PRE_CODEC_RD_QOS_STEP+1);
u32  CH21_LATENCY = NOC_PRE_CODEC_RD_LATENCY & 0xFFFF;
NOC_REG18  = (CH21_LATENCY<<16) |
             (CH21_QOS_MAX<<8) |
             (CH21_QOS_MIN<<3) | 1;

//channel 22
u32  CH22_QOS_MAX = 0x1F & 22 ;
u32  CH22_QOS_MIN = 0x1F & (CH22_QOS_MAX-NOC_ISP_WR2CARD_QOS_STEP+1);
u32  CH22_LATENCY = NOC_ISP_WR2CARD_LATENCY & 0xFFFF;
NOC_REG19  = (CH22_LATENCY<<16) |
             (CH22_QOS_MAX<<8) |
             (CH22_QOS_MIN<<3) | 1;

//channel 23
u32  CH23_QOS_MAX = 0x1F & 23 ;
u32  CH23_QOS_MIN = 0x1F & (CH23_QOS_MAX-NOC_BACK_CODEC_WR2CARD_QOS_STEP+1);
u32  CH23_LATENCY = NOC_BACK_CODEC_WR2CARD_LATENCY & 0xFFFF;
NOC_REG20  = (CH23_LATENCY<<16) |
             (CH23_QOS_MAX<<8) |
             (CH23_QOS_MIN<<3) | 1;

//channel 24
u32  CH24_QOS_MAX = 0x1F & 24 ; 
u32  CH24_QOS_MIN = 0x1F & (CH24_QOS_MAX-NOC_BACK_CODEC_WR2LCD_QOS_STEP+1);
u32  CH24_LATENCY = NOC_BACK_CODEC_WR2LCD_LATENCY & 0xFFFF;
NOC_REG21  = (CH24_LATENCY<<16) |
             (CH24_QOS_MAX<<8) |
             (CH24_QOS_MIN<<3) | 1;

//channel 25
u32  CH25_QOS_MAX = 0x1F & 25 ; 
u32  CH25_QOS_MIN = 0x1F & (CH25_QOS_MAX-NOC_BACK_CODEC_RD_QOS_STEP+1);
u32  CH25_LATENCY = NOC_BACK_CODEC_RD_LATENCY & 0xFFFF;
NOC_REG22  = (CH25_LATENCY<<16) |
             (CH25_QOS_MAX<<8) |
             (CH25_QOS_MIN<<3) | 1; 

//channel 26
u32  CH26_QOS_MAX = 0x1F & 26 ;
u32  CH26_QOS_MIN = 0x1F & (CH26_QOS_MAX-NOC_ISP_WR2LCD_QOS_STEP+1);
u32  CH26_LATENCY = NOC_ISP_WR2LCD_LATENCY & 0xFFFF;
NOC_REG23  = (CH26_LATENCY<<16) |
             (CH26_QOS_MAX<<8) |
             (CH26_QOS_MIN<<3) | 1;

//channel 27
u32  CH27_QOS_MAX = 0x1F & 27 ;
u32  CH27_QOS_MIN = 0x1F & (CH27_QOS_MAX-NOC_DE_QOS_STEP+1);
u32  CH27_LATENCY = NOC_DE_LATENCY & 0xFFFF;
NOC_REG24  = (CH27_LATENCY<<16) |
             (CH27_QOS_MAX<<8) |
             (CH27_QOS_MIN<<3) | 1;

}

/*******************************************************************************
* Function Name  : ax32xx_dmaChannelEnable
* Description    : dma channel enable
* Input          : u8 channel : dma channel 
*                  u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_dmaChannelEnable(u8 channel,u8 en)
{
	if(en)
		R_DMA_CTRL0 |= 1<<channel;
	else
		R_DMA_CTRL0 &= ~(1<<channel);
}
