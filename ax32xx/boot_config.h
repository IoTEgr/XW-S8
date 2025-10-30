#ifndef BOOT_CONFIG_H
#define BOOT_CONFIG_H

////////////////////////////////////////////////////////////////////////////////
#if (AX32_PLATFORM == FPGA)
#define COMBO_CLOCK             72000000
#define APB_CLK					COMBO_CLOCK
#define PLL_CLK					COMBO_CLOCK
#else
#define COMBO_CLOCK             144000000
#define APB_CLK					COMBO_CLOCK
#define PLL_CLK					(APB_CLK*2)
#endif
////////////////////////////////////////////////////////////////////////////////
// SRAM
#define SRAM_END_ADDR           0x10008000
#define BOOT_FAT_TABLE          (SRAM_END_ADDR - 512*4)
#define BOOT_SEC_ADDR           (SRAM_END_ADDR - 512*3)
#define BOOT_SEC_TABLE          (SRAM_END_ADDR - 512*2)
#define BOOT_PARAM_STACK        (SRAM_END_ADDR - 512*1)

////////////////////////////////////////////////////////////////////////////////
// MODE SELECT
#define MODE_PORT               PORTA

////////////////////////////////////////////////////////////////////////////////
// UART
#define UART_CLOCK_BASE         COMBO_CLOCK
#define UART_BAUDRATE           115200
#define UART_PORT               PORTA
#define UART_TX_PIN             2

////////////////////////////////////////////////////////////////////////////////
// SPI
#define SPI_CLOCK_BASE          SYS_CLOCK
#define SPI_BAUDRATE            100000
#define SPI_PORT                PORTA
#define SPI_CLK_PIN             0
#define SPI_DIO_PIN             1
#define SPI_CS_PIN              2
#define SPI_DI_PIN              3

////////////////////////////////////////////////////////////////////////////////
// SD
#define SD_CLOCK_BASE           SYS_CLOCK
#define SD_BAUDRATE             6000000
#define SD_INITBAUD             240000
#define SD_PORT                 PORTA
#define SD_CLK_PIN              0
#define SD_CMD_PIN              1
#define SD_DAT_PIN              2
////////////////////////////////////////////////////////////////////////////////

#define CPU_BIG_ENDIAN                          0

#if CPU_BIG_ENDIAN
    #define BLDR_VER       0x00000200
    #define CHECKSUM       0 
    #define MAGICKEY       0x67452301
    #define SPI_DMA_SHIFT  0
    #define SPINAND_CMD    0x0FC01303
#else
    #define BLDR_VER       0x00020000
    #define CHECKSUM       0 
    #define MAGICKEY       0x01234567
    #define SPI_DMA_SHIFT  0x800
    #define SPINAND_CMD    0x0313C00F
#endif

////////////////////////////////////////////////////////////////////////////////
// Software Delay
#define TICK_PER_US 1
#define COMBO_TICK_PER_US 7
/*
#ifdef __ASSEMBLER__

        .macro soft_delay us
        ldr         r0, =(\us * TICK_PER_US) / 5 | 1
.Lsoft_delay\@:
        sub         r0, #1
        bne         .Lsoft_delay\@
        .endm

        .macro combo_delay us
        ldr         r0, =\us * COMBO_TICK_PER_US
.Lcombo_delay\@:
        sub         r0, #1
        bne         .Lcombo_delay\@
        .endm


#endif
*/
#endif
