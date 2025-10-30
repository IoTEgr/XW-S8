/****************************************************************************
**
 **                              DEVICE
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  DEVICE TFT-LCD HEADER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : tft_lcd.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is TFT-LCD device file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  TFT_LCD_H
    #define  TFT_LCD_H

#include "../../hal/inc/hal.h"
#include "../../ax32_platform_demo/application.h"



#define  DEV_LCD_ILI9806E02         0
#define  DEV_LCD_ST7789S            1
#define  DEV_LCD_JL                 2
#define  DEV_LCD_ST7282             3  // ok 480*272
#define  DEV_LCD_ILI9806E1540       4
#define  DEV_LCD_KLD4301A           5
#define  DEV_LCD_SPFD5420           6  // ok
#define  DEV_LCD_R61509V            7  // ok
#define  DEV_LCD_ILI8961            8  // ok
#define  DEV_LCD_ILI9342C           9  // ok
#define  DEV_LCD_ILI9328            10 // ok
#define  DEV_LCD_HX8352B            11 // ok-400*240
#define  DEV_LCD_ST7789V            12 	//ok
#define  DEV_LCD_HX8352C            13 // ok 400*240
#define  DEV_LCD_ILI9326G           14 // ok 400*240
#define  DEV_LCD_HX8357B          	15 // ok 480*320
#define  DEV_LCD_OTM4001A          	16 // ok 400*240
#define  DEV_LCD_OTA5182          	17
#define  DEV_LCD_ST7701FW1601     	18
#define	 DEV_LCD_ST7701WV1604		19
#define  DEV_LCD_lgdp4532			20
#define  DEV_LCD_iLi9225G			21// ok  220*176
#define  DEV_LCD_ST7789_666FORMAT	22	// ok

#define  DEV_LCD_ILI9341            23
#define  DEV_LCD_NV3030A            24 	//ok
#define  DEV_LCD_NV3030B            25 	//ok

#define DEV_LCD_R61505V				26


#define  DEV_LCD                    DEV_LCD_ST7789V//DEV_LCD_R61505V//DEV_LCD_ST7789V//

#define LCD_OP_SECTION              SECTION(".lcd_res.struct")     
#define LCD_INTI_TAB_SECTION        SECTION(".lcd_res.init_tab")

#define BEGIN_INIT_TAB              LCD_INTI_TAB_SECTION static u32 init_tab[][2] = {
#define END_INIT_TAB                {LCD_TAB_END,LCD_TAB_END},};

#define BEGIN_UNINIT_TAB            LCD_INTI_TAB_SECTION static u32 uninit_tab[][2] = {
#define END_UNINIT_TAB              {LCD_TAB_END,LCD_TAB_END},};


#define INIT_TAB_INIT               .init_table = init_tab, \
                                    .init_table_size = sizeof(init_tab),
#define UNINIT_TAB_INIT             .uninit_table = uninit_tab, \
                                    .uninit_table_size = sizeof(uninit_tab),


#define BCTAB0	   186789922UL
#define BCTAB1	   220475167UL
#define BCTAB2	   270937372UL
#define BCTAB3	   321399577UL
#define BCTAB4	   371861782UL
#define BCTAB5	   422258707UL
#define BCTAB6	   472720912UL
#define BCTAB7	   523117837UL

#define WBIC_TAB   WBIC_TAB0,WBIC_TAB1,WBIC_TAB2,WBIC_TAB3,WBIC_TAB4,WBIC_TAB5,WBIC_TAB6,WBIC_TAB7
#define HBIC_TAB   HBIC_TAB0,HBIC_TAB1,HBIC_TAB2,HBIC_TAB3,HBIC_TAB4,HBIC_TAB5,HBIC_TAB6,HBIC_TAB7
#define BCTAB      BCTAB0,BCTAB1,BCTAB2,BCTAB3,BCTAB4,BCTAB5,BCTAB6,BCTAB7
/*******************************************************************************
* Function Name  : lcdInit
* Description    : lcd initial
* Input          : 
* Output         : none                                            
* Return         : none
*******************************************************************************/
void lcdInit(void);

/*******************************************************************************
* Function Name  : LcdGetName
* Description    : get lcd name
* Input          :
* Output         : none                                            
* Return         : char *
*******************************************************************************/
char *LcdGetName(void);


u8 devLcdOp_ScanMode_get(void);
















#endif

