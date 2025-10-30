#include "./inc/ax329x.h"
#include "./inc/ax32xx_cfg.h"
#include <or1k-asm.h>
#include "spi_boot_cfg.h"
#include "version.h"

        .macro      PUB_THUMB name
        .global     \name
//        .thumb_func
\name:
        .endm

        .macro      PUB_LABEL name
        .global     \name
\name:
        .endm

#define CPU_BIG_ENDIAN                          0

#if CPU_BIG_ENDIAN
    #define BLDR_VER       0x00000200
    #define CHECKSUM       0 
    #define MAGICKEY       0x67452301
    #define SPI_DMA_SHIFT  0
    #define SPINAND_CMD    0x0FC01303
#else
    #define BLDR_VER       0x00020000
    #define CHECKSUM       0x01234567 
    #define MAGICKEY       0x01234567
    #define SPI_DMA_SHIFT  0x00010200 //BIT_ADDR_2B[6]|BIT_ADDR_4B[5]|BIT_FAST[4]|BIT_SPI_DMA[3]|BIT_ADDR_WIDTH[2]|BIT_DBWIDTH[10]
    #define SPINAND_CMD    0x0313C00F
#endif

#undef COMBO_CLOCK
#define COMBO_CLOCK             96000000
#define UART_BAUDRATE           115200

#define SD4                     0
#define SDRAM_96M               0

//#define PLL_CLOCK             50000
//#define SD_BAUDRATE           6000000
//#define OTHER_SEC_ADDR        wsec_ram_base

//#define NO_CHKSUM             1
//#define INVALID_KEY           0
#define CFG_FUNC                1
//#define ENCRYPT               0

#define USE_ADKEY               0
#define CHECK_KEY               0
#define ADKEY_PRESSED_CNT       10


////////////////////////////////////////////////////////////////////////////////
		.section    ".bootsec", "ax"
.L_0:
__startup:
        .long       BLDR_VER
        .ascii      "BLDR"
        .byte       0x00	                          // Should be Check Sum of the sector
        .byte       (flash_param - __startup) / 16
        .byte       boot_flagbyte                     //(ENCRYPT<<4)|(CFG_FUNC<<0) | (INVALID_KEY<<1) | (NO_CHKSUM<<2)
        .byte       (VERSION_MAIN*16+VERSION_SUB)     //binary version no
        .align      16, 0
		
.L_param:
flash_param:
////////////////////////////////////////////////////////////////////////////////
PUB_LABEL           hex
PUB_LABEL           .hex
        .ascii      "0123456789ABCDEF"

        .long       _text_start                            
        .long       _text_sec                             
        .long       _text_len
        .long       _exception_vma                        //_text_start

        .long       CHECKSUM
        .long       MAGICKEY                      
        .long       spi_dma_shift                         //SPI_DMA_SHIFT         
        .long       spinand_cmd                           //SPINAND_CMD		
        .long       spi_baud                              //SPI Baud
        .long       psram_cfg                             // 		
        .long       psram_cmd                             // 
		
////////////////////////////////////////////////////////////////////////////////

// 配置函數 __NDS32__
#if CFG_FUNC
#define IDX_INIT_SD4            4*0
#define IDX_SDRAM_INIT          4*1
#define IDX_INIT_PLL            4*2
#define IDX_TABLE_INIT          4*3
#define IDX_SOFT_DELAY          4*4
#define IDX_DDR_INIT            4*5
#define IDX_MORE_FUNC           4*6
#define IDX_SDRAM_INIT2         4*7
#define IDX_DLLCON_INIT         4*8
#define IDX_RTC_SOFTDELAY       4*9

#define IDX_SHIFT               6
#define IDX_ADD                 0x20DD309C
#define IDX_SUB                 0x20DD309E
#define IDX_EQU                 0x26DD

        .macro      CONFIG_BEGIN
        .word       0xD7E177F4
        .word       0xD7E157F8
        .word       0xD7E14FFC
        .word       0x9C21FFF0
        .word       0x9D430000
        .word       0xA9C40000
        .endm

        .macro      CONFIG_END
        .word       0x8521000C
        .word       0x85410008
        .word       0x85C10004
OR1K_DELAYED(
OR1K_INST(.word       0x9C210010),
OR1K_INST(.word       0x44004800)
)
        .endm

        .macro      table_init_begin
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_TABLE_INIT),
OR1K_INST(.word       0x48005000)
)
        .endm

        .macro      mem_set addr, val
        .long       \addr, \val
        .endm

        .macro      bic_orr addr, bval, oval
        .long       1+\addr, \bval, \oval
        .endm

        .macro      table_init_end
        .long       0
        .endm

        .macro      param_end
        .long       0
        .endm

        .macro      sdram_init con, tm, mrs
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_SDRAM_INIT),
OR1K_INST(.word       0x48005000)
)
        .word       \con, \tm, \mrs
        .endm

        .macro      sdc_dw4
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_INIT_SD4),
OR1K_INST(.word       0x48005000)
)
        .endm

        .macro      init_pll cfg, i, f, div
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_INIT_PLL),
OR1K_INST(.word       0x48005000)
)
        .short      \i, \f, \cfg
        .byte       \div
        .byte      0
        .endm

        .macro      soft_delay t
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_SOFT_DELAY),
OR1K_INST(.word       0x48005000)
)
        .word       \t
        .endm
        
        .macro      ddr_init con, tm, mrs1, mrs0
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_DDR_INIT),
OR1K_INST(.word       0x48005000)
)
        .word       \con, \tm, \mrs1, \mrs0
        .endm

        .macro      sdram_init2 con, tm, mrs
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_SDRAM_INIT2),
OR1K_INST(.word       0x48005000)
)
        .word       \con, \tm, \mrs
        .endm

        .macro      rtc_soft_delay t, chk
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_RTC_SOFTDELAY),
OR1K_INST(.word       0x48005000)
)
        .word       \t, \chk
        .endm

        .macro      dllcon_init clk, cnt
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_DLLCON_INIT),
OR1K_INST(.word       0x48005000)
)
        .word       \clk, \cnt
        .endm
        
        .macro      return0
        .word       0xA9600000
        .endm

        .macro      return2
        .word       0xA9600002
        .endm

        .macro      adkupd end
        l.jal       adkey_update
        l.nop
        l.bf       \end
        return2		//l.ori r11,r0,0x2  
        .endm

        .macro      checkkey end
        l.jal      check_key
        l.nop
        l.bnf       \end
        return2	//l.ori r11,r0,0x2  
        .endm
		
        .macro      morefunc addr, addr_s, cnt, cmd
OR1K_DELAYED(
OR1K_INST(.word       0x9C6A0000 + IDX_MORE_FUNC),
OR1K_INST(.word       0x48005000)
)
        .word       \addr, \addr_s, \cnt, \cmd
        .endm
        
        .macro      runfunc
        l.jal       run_func
        l.nop
        .endm 
        
// these are for FPGA
#define upll_select_96MHz \
    bic_orr     CLKCON0,  0,(3<<10)

#define spll_to_sysclk \
    bic_orr     CLKCON0,  (1<<4)|(3<<0),(0<<4)|(2<<0)    

#define spll_to_sdrclk  \
     bic_orr    CLKCON0,(1<<23),(1<<22)

#define dpll_to_sdrclk  \
     bic_orr    CLKCON0,(1<<23)|(1<<22),0

config_begin:
.Lconfig_begin:
CONFIG_BEGIN
		
#if SD4
        sdc_dw4
#endif

#if 0
        table_init_begin
        
        bic_orr     CLKCON0, 0, (1<<24)   //Timer 32kHz signal source sel : 0:from RC2M; 1:xosc32k 
        mem_set     TMR0PR,  32768/2 -1;
        mem_set     TMR0CNT,  0
        mem_set     TMR0CON,  (7<<8) | (1<<0)
 /*       
        bic_orr     PMAPCON0,  0x3ff,     ((6<<5))
        bic_orr     PAFMAP,   0,    (1<<7)
        bic_orr     PADIR,    (1<<7), 0 
        //bic_orr     PAPU,  0, (1<<1)
        mem_set     UART0BAUD,  2400000/115200 -1
        mem_set     UART0CON,  0x10
 */       

        table_init_end
        
//        l.jal check_xosc32k    
        //soft_delay 40000
#if (AX32_PLATFORM == FPGA)
//        init_pll    0x000, 144000000/32768, (144000000%32768)*2, 0
#else
//#define pll_192M    0x600, 96000000/32768,  (96000000%32768)*2,   192000000/96000000-1
//#define pll_264M    0x000, 132000000/32768, (132000000%32768)*2,  264000000/132000000-1
//#define pll_288M    0x000, 144000000/32768, (144000000%32768)*2,  288000000/144000000-1
//        init_pll    pll_288M
#endif
        //dllcon_init 0x30fc931, 0x100000
#endif

        table_init_begin
        
#if (AX32_PLATFORM == FPGA)

	#if (0==SDRAM_PSRAM_TYPE)
        mem_set     PLL1DIV,  ((200000000/32768)<<16)|((200000000%32768)*2)
        bic_orr     PLL1CON,  0,     (1<<1)
        bic_orr     PLL1CON,  0x7cc,    0x300
        bic_orr     PLL1CON,  0,      (1<<0)
        bic_orr     PLL1CON,  0,      (1<<5)

        mem_set     SDRACON0, 0x3ff
        bic_orr     CLKCON1,  (7<<5), (1<<5)   //ddr pll divide
        bic_orr     CLKCON0,  (1<<22),(1<<23)  //0:ddrpll 1:syspll
        //bic_orr     SDRACON1, 0, (1<<29)       //0: select 2MB 1: select 8MB
	#endif

#else

/*		
#if (AX32_PLATFORM == AX3292) //uart tx PE1
        bic_orr     PMAPCON0,  0x3ff,     ((24<<5))
        bic_orr     PEFMAP,   0,    (1<<1)
        bic_orr     PEDIR,    (1<<1), 0 
#else
		bic_orr     PMAPCON0,  0x3ff,     ((6<<5))
        bic_orr     PAFMAP,   0,    (1<<7)
        bic_orr     PADIR,    (1<<7), 0 
#endif

        //bic_orr     PAPU,  0, (1<<1)
        mem_set     UART0BAUD,  144000000/115200 -1
        mem_set     UART0CON,  0x10
        mem_set     UART0DAT,  'a'
   
        mem_set     PLL1DIV,  ((200000000/32768)<<16)|((200000000%32768)*2)
        bic_orr     PLL1CON,  0,     (1<<1)
        bic_orr     PLL1CON,  0x7cc,    0x300
        bic_orr     PLL1CON,  0,      (1<<0)
        bic_orr     PLL1CON,  0,      (1<<5)
        
        mem_set     SDRACON0, 0x3ff
        bic_orr     CLKCON1,  (7<<5), (1<<5)   //ddr pll divide
        bic_orr     CLKCON0,  (1<<22),(1<<23)  //0:ddrpll 1:syspll
        //bic_orr     SDRACON1, 0, (1<<29)       //0: select 2MB 1: select 8MB
*/
#endif
       //mem_set     UART0BAUD, COMBO_CLOCK / UART_BAUDRATE - 1

#if USE_ADKEY
		//bic_orr		PCON0,      29,  0         //ADC clock enable(default enable)
        mem_set     ADCBAUD,    boot_adc_baud 
        mem_set     ADCCON,     boot_adc_channel    //select channal
		bic_orr     ADCCON,     0,   (1<<4)|(1<<7)  //ADC enable | ADC run
#endif 

        table_init_end
#if (AX32_PLATFORM == FPGA)
        //sdram_init  0x183|(1<<5), 0x25000A50,0x22
		#if (0==SDRAM_PSRAM_SEL)
        sdram_init2   0x181|(1<<5), 0x2297fbf5,0x32
		#endif
#else
        //sdram_init  boot_cfg0, boot_cfg1,0x32
#endif
        
#if USE_ADKEY
        adkupd  config_end
#endif

        //morefunc _before_load_vma,_before_load_sec,_before_load_len,0
        //runfunc
		morefunc _exception_vma,_exception_sec,_exception_len,0
        runfunc
//////////////////////////////

        return0
config_end:
CONFIG_END
#endif
#if CHECK_KEY
check_key:
		
		l.movhi 	r3, hi(0x41fff)
		l.ori   	r3,r3,lo(0x41fff)
		l.lbz		r4, 0(r3)
		
		l.sfnei     r4,0x2e
		l.bf upgrade
		l.nop
#endif

#if (0==SDRAM_PSRAM_SEL)
run_func:
		l.movhi 	r7, hi(sdram_clock_tune)
		l.ori   	r7,r7,lo(sdram_clock_tune)
        l.jr        r7
#else
run_func:
		l.movhi 	r7, hi(psram_init)
		l.ori   	r7,r7,lo(psram_init)
        l.jr        r7
#endif

#if 0
check_xosc32k:
    l.sw -8(r1),r1
    l.sw -4(r1),r9

    l.ori r4,r0,0x9804            // TMR0CNT :0x9804       								    
    l.addi r6,r0,20               //loop cnt
    l.addi r2,r0,0                                             
    l.mtspr r0,r2,0x9804      
loop0:  
    soft_delay 14000         //50ms RC 2.4M
    l.mfspr r5,r4,0x0                     
    l.sfleui r5,600          //32768/2/1000*50  = 819;3/4
    l.bnf back0 
    l.addi r2,r0,0                        
    l.addi r6,r6,-1                       
    l.mtspr r0,r2,0x9804                  
    l.sfne r6,r2                          
    //l.addi r3,r0,'b'      
    //l.mtspr r0,r3,0x9088    // UART0DAT :0x9088      
    l.bf loop0  

back0: 
    //l.addi r3,r0,'a'      
    //l.mtspr r0,r3,0x9088
    l.lwz r1,-8(r1) 
    l.lwz r9,-4(r1) 

    l.jr         r9
    //l.nop 
    
#endif

#if USE_ADKEY

adkey_update:
        
		//l.movhi 	r3, hi(ADKEY_PRESSED_CNT)
		l.ori   	r3,r0,lo(ADKEY_PRESSED_CNT) //value <0xffff
		l.ori       r5, r0, 0
		
key_save_lastvalue:
        l.ori       r6, r5, 0
key_get_advalue:
		l.addi      r3, r3, -1
		l.sfnei		r3,0
		l.bnf       upgrade
		l.nop

		l.movhi 	r4, hi(ADCCON)
		l.ori   	r4,r4,lo(ADCCON)
		
check_pnd:
		l.lwz   r5, 0(r4)
		l.andi  r5,r5,0x40 //(1<<6)
		l.sfnei r5,0
		l.bf check_pnd
		l.nop

		l.movhi 	r4, hi(ADCDAT)
		l.ori   	r4,r4,lo(ADCDAT)
		l.lwz		r5, 0(r4)

		l.sfltui    r5,  lo(boot_adkey_downvalue) //value <0xffff
		l.bf back
		l.nop
		l.sfgtui    r5,  lo(boot_adkey_upvalue)   //value <0xffff
		l.bf back
		l.nop
   
		l.sfeqi		r3, (ADKEY_PRESSED_CNT-1)
		l.bf		key_save_lastvalue
		l.nop
		
		l.sfne		r6,r5
		l.bnf		key_get_advalue
		l.nop	
#endif
back:   
upgrade:
        l.jr         r9
        l.nop
        


		.global  .bootsect
.bootsect:

################################################################################
        .skip       512 - (. - __startup) - 2, 0
x55AA:
        .byte       0x55, 0xAA
################################################################################
