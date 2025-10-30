#ifndef SPI_BOOT_CFG_H
#define SPI_BOOT_CFG_H

#include "inc/ax32xx_cfg.h"


#define COMBO_CLOCK             144000000//48000000
#define PLL_DIV                 288000000/COMBO_CLOCK
#define SPI_BAUDRATE            12000000//spi of loader
#define ADC_BAUDRATE            1000000//adc of loader
////////////////////////////////////////////////////////////////////////////////

#define ENCRYPT            0
#define NO_CHKSUM          1
#define INVALID_KEY        0

#define SPI_DMA            1
//#define SPI_1W             1
#define SPI_BAUD           COMBO_CLOCK / (2*SPI_BAUDRATE) - 1
#define ADC_BAUD           (COMBO_CLOCK/(2*ADC_BAUDRATE)- 1) & 0x7F
#define ADC_CHANNEL        1
#define ADKEY_DOWNVALUE    1
#define ADKEY_UPVALUE      0x5c
////////////////////////////////////////////////////////////////////////////////

#define SDRAM_2MB   0  
#define SDRAM_8MB   1  
#define SDRAM_16MB  2  
#define SDRAM_32MB  3  

#if SDRAM_SIZE == SDRAM_SIZE_2M
#define SDR_CFG0_VAL      SDRAM_2MB
#else
#define SDR_CFG0_VAL      SDRAM_8MB
#endif

#define SDR_CFG1_VAL      0x2297fbf5//turning 0x28737f，需要修改为0x20735f。  873那三个是用来配置CBR的时间间隔的。这个是根据每个SDRAM不一样进行配置的

#define SDRITUN  ((1<<6)  | (1<<5)  | (0<<4)   | (2<<0))
#define SCOTUN             ((1<<12) | (1<<11)  | (0<<7))


////////////////////////////////////////////////////////////////////////////////

//#define SPI_2W2D_DEFAULT
//#define SPI_2W2D_CFG
//#define SPI_4W4D_DEFAULT

#define SPI_2W2D_READCMD        0x3b
#define SPI_2W2D_DUMMYVAL       0
#define SPI_2W2D_DUMMYCNT       1
#define SPI_2W2D_ADDR_2B        0
#define SPI_2W2D_ADDR_4B        0
#define SPI_2W2D_ADDR_WIDTH     0


////////////////////////////////////////////////////////////////////////////////

#endif




