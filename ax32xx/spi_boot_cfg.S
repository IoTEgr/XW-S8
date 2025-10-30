#include <or1k-asm.h>
#include "./inc/ax329x.h"
#include "spi_boot_cfg.h"
    #define SPINAND_CMD    0x13C00F
    #define rd_sel         (1<<5)
	
    .global boot_cfgfunc
    .equ    boot_cfgfunc,      CFG_FUNC

    //.global boot_flagbyte
    //.equ    boot_flagbyte,     FLAG_BYTE

    //.global boot_clktun
    //.equ    boot_clktun,       CLKTUN_ADDR

	.global spi_baud
    .equ    spi_baud,          SPI_BAUD

	.global boot_clktun_val
    .equ    boot_clktun_val,   CLKTUN_VAL

	.global boot_cfg0
    .equ    boot_cfg0,        (SDR_CFG0_VAL | 0x180 | rd_sel)

	.global boot_cfg1
    .equ    boot_cfg1,         SDR_CFG1_VAL
    
    .global boot_pll_div
    .equ    boot_pll_div,      PLL_DIV
    
    .global boot_adc_baud
    .equ    boot_adc_baud,     ADC_BAUD

    .global boot_adc_channel
    .equ    boot_adc_channel,     ADC_CHANNEL
    
    .global boot_adkey_downvalue
    .equ    boot_adkey_downvalue,     ADKEY_DOWNVALUE
    
    .global boot_adkey_upvalue
    .equ    boot_adkey_upvalue,       ADKEY_UPVALUE
    
	.global boot_flagbyte
	.global spi_dma_shift
	.global spinand_cmd
	.global boot_clktun_val	
    .global psram_cfg
    .global psram_cmd
		
#if ENCRYPT|INVALID_KEY|NO_CHKSUM
    .equ    boot_flagbyte,     (ENCRYPT<<4)|(NO_CHKSUM<<2)|(INVALID_KEY<<1)|(1<<0)	
#else
    .equ    boot_flagbyte,     (1<<0)	
#endif 

#if SCOTUN | SDRITUN
    .equ    boot_clktun_val,   SCOTUN | SDRITUN
#else
    .equ    boot_clktun_val,   0x0033b8b6  //default
#endif

//BIT_SPI_SD[7]|BIT_ADDR_2B[6]|BIT_ADDR_4B[5]|BIT_FAST[4]|BIT_SPI_DMA[3]|BIT_ADDR_WIDTH[2]|BIT_DBWIDTH[1:0]
//BIT_DBWIDTH[1:0]  SPI2W1D:0b00  SPI2W2D:0x01  SPI2W4D:0x10//0x11

#if  defined (SPI_2W2D_CFG)
    .equ    spi_dma_shift,         0x00009100|(SPI_2W2D_DUMMYVAL<<24)|(SPI_2W2D_DUMMYCNT<<16)|(SPI_2W2D_ADDR_2B<<14)|
	                               (SPI_2W2D_ADDR_4B<<13)|(SPI_DMA<<11)|(SPI_2W2D_ADDR_WIDTH<<10)
	.equ    spinand_cmd,           0x13C00F|(SPI_2W2D_READCMD<<24)
	
#elif  defined (SPI_2W2D_DEFAULT)
    .equ    spi_dma_shift,         0x00019100|(SPI_DMA<<11)
	.equ    spinand_cmd,           0x3b13C00F
     //(PSRAM_2W1D)
    .equ    psram_cfg,             (0<<20)|(2<<18)|(0<<14)|(0<<12)|(0<<10)|((2 & 3)<<8)|(0<<6)|(0<<5)|(1<<4)
	.equ    psram_cmd,             0x0203
    
#elif  defined (SPI_4W4D_DEFAULT)
    .equ    spi_dma_shift,         0x00019300|(SPI_DMA<<11)
	.equ    spinand_cmd,           0x6b13C00F
    //(PSRAM_4W4D)
    .equ    psram_cfg,             (2<<20)|(2<<18)|(0<<14)|(0<<12)|(3<<10)|((3 & 3)<<8)|(1<<6)|(0<<5)|(1<<4)
	.equ    psram_cmd,             0x38eb
    
#else
    .equ    spi_dma_shift,         (SPI_DMA<<11)//0x00008000|(SPI_DMA<<11)
	.equ    spinand_cmd,           0x0313C00F
    .equ    psram_cfg,             (0<<20)|(2<<18)|(0<<14)|(0<<12)|(0<<10)|((0 & 3)<<8)|(0<<6)|(0<<5)|(1<<4)
	.equ    psram_cmd,             0x0203
#endif


	
////////////////////////////////////////////////////////////////////////////////
    .equ    btsct, .bootsect   //引用库中符号，将库链接进来

