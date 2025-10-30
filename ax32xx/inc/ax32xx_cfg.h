/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                           AX32XX 
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx.h
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
#ifndef  AX32XX_CFG_H
#define  AX32XX_CFG_H

#define  CFG_DCACHE_EN       1
#define  CFG_ICACHE_EN       1



#define  CFG_WDT_EN          1   // watch dog



#define  CFG_MCU_DBG_EN      0
#define  CFG_MCU_TFDBG_EN    0

//------------CHIP VISION
#define THUNDERSE_A1		0x00000000
#define THUNDERSE_B1		0x00010000
#define THUNDERSE_B2		0x00030000


//----------ax32xx body define------------
#define  FPGA                0x0080
#define  AX3293A             0x4880
#define  AX3293B             0x4881
#define  AX3282S             0x6480
#define  AX3292              0x6481
//#define  AX3292B           0x6482
#define  AX3291A             0x8080

#define  JT5292				  0x6485
#define  JT5295				  0x6446
#define  JT5293				  0x4845

#define  AX32_PLATFORM      JT5295// AX3291A  //FPGA// AX3291A//AX3282S //AX3292    //  current body     

#if (AX32_PLATFORM==JT5292)||(AX32_PLATFORM==JT5295)||(AX32_PLATFORM==JT5293)
#define  SDRAM_PSRAM_SEL     1 	//  0: is sdram , 1: is psram 
#define  PSRAM_BURST16_EN 	  0		//  0: burst8 , 1: burst16  
#else
#define  SDRAM_PSRAM_SEL     0 	//  0: is sdram , 1: is psram 
#define  PSRAM_BURST16_EN 	  0		//   SE must 0 
#endif

//-----------------------------------------
#define  SDRAM_SIZE_2M       0x200000
#define  SDRAM_SIZE_4M       0x400000
#define  SDRAM_SIZE_8M       0x800000

#if ((AX32_PLATFORM&0xf0)==0x20)
   #define  SDRAM_SIZE       SDRAM_SIZE_2M  // 2M
#elif ((AX32_PLATFORM&0xf0)==0x40)
   #define  SDRAM_SIZE       SDRAM_SIZE_4M  // 4M
#elif ((AX32_PLATFORM&0xf0)==0x80)
   #define  SDRAM_SIZE       SDRAM_SIZE_8M  // 8M
#else
   #define  SDRAM_SIZE       SDRAM_SIZE_8M  // 8M
#endif

#define SPI_FLASH_SIZE 		0x800000 //0x400000
















#endif

