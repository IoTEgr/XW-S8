
#include "tft_lcd.h"

#define CMD(x)    {SPI_DAT,x}
#define DAT(x)    {SPI_DAT,((x)|(1<<8))}

#define  H_WBIC_480x272_NUM  1

#if H_WBIC_480x272_NUM == 0
//strength; 0
#define HBIC_TAB0    0x01ff41ff
#define HBIC_TAB1    0x000640fa
#define HBIC_TAB2    0xfe103bf7
#define HBIC_TAB3    0xfb1c32f7
#define HBIC_TAB4    0xf82729f8
#define HBIC_TAB5    0xf7321cfb
#define HBIC_TAB6    0xf73b10fe
#define HBIC_TAB7    0xfa400600

//strength; 0
#define WBIC_TAB0    0x40ff0100,0x000001ff
#define WBIC_TAB1    0x3efa0200,0x00000006
#define WBIC_TAB2    0x37f80201,0x0000fe10
#define WBIC_TAB3    0x2df80201,0x0101fb1b
#define WBIC_TAB4    0x26f90101,0x0101f924
#define WBIC_TAB5    0x1bfb0101,0x0102f82d
#define WBIC_TAB6    0x10fe0000,0x0102f837
#define WBIC_TAB7    0x06000000,0x0002fa3e

#elif H_WBIC_480x272_NUM == 1
//strength; 1
#define HBIC_TAB0    0xff033b03
#define HBIC_TAB1    0xfd0b3afe
#define HBIC_TAB2    0xfb1437fa
#define HBIC_TAB3    0xf91f30f8
#define HBIC_TAB4    0xf82729f8
#define HBIC_TAB5    0xf8301ff9
#define HBIC_TAB6    0xfa3714fb
#define HBIC_TAB7    0xfe3a0bfd

//strength; 1
#define WBIC_TAB0    0x3b03ff00,0x0100ff03
#define WBIC_TAB1    0x37ff0000,0x0101fd0b
#define WBIC_TAB2    0x33fb0100,0x0101fb14
#define WBIC_TAB3    0x2cf90200,0x0102f91d
#define WBIC_TAB4    0x26f80200,0x0002f826
#define WBIC_TAB5    0x1df90201,0x0002f92c
#define WBIC_TAB6    0x14fb0101,0x0001fb33
#define WBIC_TAB7    0x0bfd0101,0x0000ff37

#elif H_WBIC_480x272_NUM == 2
//strength; 2
#define HBIC_TAB0    0xfd083308
#define HBIC_TAB1    0xfb0f3402
#define HBIC_TAB2    0xf91732fe
#define HBIC_TAB3    0xf9202cfb
#define HBIC_TAB4    0xf92628f9
#define HBIC_TAB5    0xfb2c20f9
#define HBIC_TAB6    0xfe3217f9
#define HBIC_TAB7    0x02340ffb

//strength; 2
#define WBIC_TAB0    0x3508fc01,0x0101fc08
#define WBIC_TAB1    0x3302fe01,0x0002fb0f
#define WBIC_TAB2    0x2fff0000,0x0002f917
#define WBIC_TAB3    0x2bfb0000,0x0002f91f
#define WBIC_TAB4    0x26fa0100,0x0001fa24
#define WBIC_TAB5    0x1ff90200,0x0000fb2b
#define WBIC_TAB6    0x17f90200,0x0000ff2f
#define WBIC_TAB7    0x0ffb0200,0x01fe0233

#elif H_WBIC_480x272_NUM == 3
//strength; 3
#define HBIC_TAB0    0xfb0c2d0c
#define HBIC_TAB1    0xfa122d07
#define HBIC_TAB2    0xfa182c02
#define HBIC_TAB3    0xfb1f2600
#define HBIC_TAB4    0xfd2224fd
#define HBIC_TAB5    0x00261ffb
#define HBIC_TAB6    0x022c18fa
#define HBIC_TAB7    0x072d12fa

//strength; 3
#define WBIC_TAB0    0x2e0dfb01,0x0001fb0d
#define WBIC_TAB1    0x2e07fc01,0x0001fa13
#define WBIC_TAB2    0x2b03fd01,0x0001fa19
#define WBIC_TAB3    0x2700fe01,0x0000fb1f
#define WBIC_TAB4    0x25fd0001,0x0100fd1f
#define WBIC_TAB5    0x1ffb0000,0x01fe0027
#define WBIC_TAB6    0x19fa0100,0x01fd032b
#define WBIC_TAB7    0x13fa0100,0x01fc072e

#elif H_WBIC_480x272_NUM == 4
//strength; 4
#define HBIC_TAB0    0xfb0f270f
#define HBIC_TAB1    0xfc14250b
#define HBIC_TAB2    0xfd182407
#define HBIC_TAB3    0xff1c2203
#define HBIC_TAB4    0x00202000
#define HBIC_TAB5    0x03221cff
#define HBIC_TAB6    0x072418fd
#define HBIC_TAB7    0x0b2514fc

//strength; 4
#define WBIC_TAB0    0x2711fb00,0x0100fb11
#define WBIC_TAB1    0x260cfb00,0x0100fc16
#define WBIC_TAB2    0x2508fb01,0x01fffd1a
#define WBIC_TAB3    0x2304fc01,0x01feff1e
#define WBIC_TAB4    0x2201fd01,0x01fd0120
#define WBIC_TAB5    0x1efffe01,0x01fc0423
#define WBIC_TAB6    0x1afdff01,0x01fb0825
#define WBIC_TAB7    0x16fc0001,0x00fb0c26

#elif H_WBIC_480x272_NUM == 5
//strength; 5
#define HBIC_TAB0    0xfe112011
#define HBIC_TAB1    0xff141f0e
#define HBIC_TAB2    0x00171f0a
#define HBIC_TAB3    0x02191e07
#define HBIC_TAB4    0x051b1b05
#define HBIC_TAB5    0x071e1902
#define HBIC_TAB6    0x0a1f1700
#define HBIC_TAB7    0x0e1f14ff

//strength; 5
#define WBIC_TAB0    0x2113fefe,0x01fefe13
#define WBIC_TAB1    0x2010fdff,0x01fdff17
#define WBIC_TAB2    0x220cfcff,0x01fc001a
#define WBIC_TAB3    0x1f09fc00,0x00fc031d
#define WBIC_TAB4    0x2006fc00,0x00fc061c
#define WBIC_TAB5    0x1d03fc00,0x00fc091f
#define WBIC_TAB6    0x1a00fc01,0xfffc0c22
#define WBIC_TAB7    0x17fffd01,0xfffd1020

#elif H_WBIC_480x272_NUM == 6
//strength; 6
#define HBIC_TAB0    0x01121b12
#define HBIC_TAB1    0x03131b0f
#define HBIC_TAB2    0x0515190d
#define HBIC_TAB3    0x0616190b
#define HBIC_TAB4    0x08181808
#define HBIC_TAB5    0x0b191606
#define HBIC_TAB6    0x0d191505
#define HBIC_TAB7    0x0f1b1303

//strength; 6
#define WBIC_TAB0    0x1a1402fd,0x00fd0214
#define WBIC_TAB1    0x1b1200fd,0x00fc0416
#define WBIC_TAB2    0x190f00fd,0xfffd0619
#define WBIC_TAB3    0x190dfffd,0xfefd081b
#define WBIC_TAB4    0x1c0afefe,0xfefe0a18
#define WBIC_TAB5    0x1b08fdfe,0xfdff0d19
#define WBIC_TAB6    0x1906fdff,0xfd000f19
#define WBIC_TAB7    0x1604fc00,0xfd00121b

#elif H_WBIC_480x272_NUM == 7
//strength; 7
#define HBIC_TAB0    0x06111811
#define HBIC_TAB1    0x07121710
#define HBIC_TAB2    0x0913160e
#define HBIC_TAB3    0x0a14150d
#define HBIC_TAB4    0x0b15150b
#define HBIC_TAB5    0x0d15140a
#define HBIC_TAB6    0x0e161309
#define HBIC_TAB7    0x10171207

//strength; 7
#define WBIC_TAB0    0x141206ff,0xfeff0612
#define WBIC_TAB1    0x131005ff,0xfe000813
#define WBIC_TAB2    0x140f04fe,0xfe000914
#define WBIC_TAB3    0x140e02fe,0xfe000b15
#define WBIC_TAB4    0x160c01fe,0xfe010c14
#define WBIC_TAB5    0x150b00fe,0xfe020e14
#define WBIC_TAB6    0x140900fe,0xfe040f14
#define WBIC_TAB7    0x130800fe,0xff051013

#endif

#if DEV_LCD  == DEV_LCD_ILI9806E02

BEGIN_INIT_TAB
    CMD(0xFF),
    DAT(0xFF),
    DAT(0x98),
    DAT(0x06),
    DAT(0x04),
    DAT(0x01),

    CMD(0x08),
    DAT(0x10),

    // 0:DE mode ,1:SYNC mode
    CMD(0x20),
    DAT(0x01),

    // B0: 0:DE Low enable,1:DE High enable
    // B1: 0:PCLK fetched rising,1:PCLK fetched falling
    // B2: 0:HS Low enable,1:HS High enable
    // B3: 0:VS Low enable,1:VS High enable
    CMD(0x21),
    DAT(0x0D),

    //1: 480 X 854
    //2: 480 X 800
    CMD(0x30),
    DAT(0x01),

    CMD(0x31),
    DAT(0x02),

    CMD(0x40),
    DAT(0x15),

    CMD(0x41),
    DAT(0x33),

    CMD(0x42),
    DAT(0x11),

    CMD(0x43),
    DAT(0x09),

    CMD(0x44),
    DAT(0x02),

    CMD(0x50),
    DAT(0x60),

    CMD(0x51),
    DAT(0x60),

    CMD(0x52),
    DAT(0x00),

    CMD(0x53),
    DAT(0x40),

    CMD(0x57),
    DAT(0x50),

    CMD(0x60),
    DAT(0x07),

    CMD(0x61),
    DAT(0x00),

    CMD(0x62),
    DAT(0x08),

    CMD(0x63),
    DAT(0x00),

//++++++++++++++++++ Gamma Setting ++++++++++++++++++//
    CMD(0xA0),
    DAT(0x00),

    CMD(0xA1),
    DAT(0x06),

    CMD(0xA2),
    DAT(0x11),

    CMD(0xA3),
    DAT(0x10),

    CMD(0xA4),
    DAT(0x0D),

    CMD(0xA5),
    DAT(0x19),

    CMD(0xA6),
    DAT(0x0A),

    CMD(0xA7),
    DAT(0x09),

    CMD(0xA8),
    DAT(0x03),

    CMD(0xA9),
    DAT(0x08),

    CMD(0xAA),
    DAT(0x07),

    CMD(0xAB),
    DAT(0x05),

    CMD(0xAC),
    DAT(0x0C),

    CMD(0xAD),
    DAT(0x2E),

    CMD(0xAE),
    DAT(0x29),

    CMD(0xAF),
    DAT(0x00),

    CMD(0xC0),
    DAT(0x00),

    CMD(0xC1),
    DAT(0x06),

    CMD(0xC2),
    DAT(0x11),

    CMD(0xC3),
    DAT(0x12),

    CMD(0xC4),
    DAT(0x08),

    CMD(0xC5),
    DAT(0x19),

    CMD(0xC6),
    DAT(0x09),

    CMD(0xC7),
    DAT(0x08),

    CMD(0xC8),
    DAT(0x04),

    CMD(0xC9),
    DAT(0x08),

    CMD(0xCA),
    DAT(0x04),

    CMD(0xCB),
    DAT(0x02),

    CMD(0xCC),
    DAT(0x0C),

    CMD(0xCD),
    DAT(0x2E),

    CMD(0xCE),
    DAT(0x29),

    CMD(0xCF),
    DAT(0x00),

    CMD(0xFF),
    DAT(0xFF),
    DAT(0x98),
    DAT(0x06),
    DAT(0x04),
    DAT(0x06),

    CMD(0x00),
    DAT(0x21),

    CMD(0x01),
    DAT(0x06),

    CMD(0x02),
    DAT(0x00),

    CMD(0x03),
    DAT(0x00),

    CMD(0x04),
    DAT(0x01),

    CMD(0x05),
    DAT(0x01),

    CMD(0x06),
    DAT(0x80),

    CMD(0x07),
    DAT(0x02),

    CMD(0x08),
    DAT(0x05),

    CMD(0x09),
    DAT(0x00),

    CMD(0x0A),
    DAT(0x00),

    CMD(0x0B),
    DAT(0x00),

    CMD(0x0C),
    DAT(0x01),

    CMD(0x0D),
    DAT(0x01),

    CMD(0x0E),
    DAT(0x00),

    CMD(0x0F),
    DAT(0x00),

    CMD(0x10),
    DAT(0xF0),

    CMD(0x11),
    DAT(0xF4),

    CMD(0x12),
    DAT(0x00),

    CMD(0x13),
    DAT(0x00),

    CMD(0x14),
    DAT(0x00),

    CMD(0x15),
    DAT(0xC0),

    CMD(0x16),
    DAT(0x08),

    CMD(0x17),
    DAT(0x00),

    CMD(0x18),
    DAT(0x00),

    CMD(0x19),
    DAT(0x00),

    CMD(0x1A),
    DAT(0x00),

    CMD(0x1B),
    DAT(0x00),

    CMD(0x1C),
    DAT(0x00),

    CMD(0x1D),
    DAT(0x00),

    CMD(0x20),
    DAT(0x02),

    CMD(0x21),
    DAT(0x13),

    CMD(0x22),
    DAT(0x45),

    CMD(0x23),
    DAT(0x67),

    CMD(0x24),
    DAT(0x01),

    CMD(0x25),
    DAT(0x23),

    CMD(0x26),
    DAT(0x45),

    CMD(0x27),
    DAT(0x67),

    CMD(0x30),
    DAT(0x13),

    CMD(0x31),
    DAT(0x22),

    CMD(0x32),
    DAT(0x22),

    CMD(0x33),
    DAT(0x22),

    CMD(0x34),
    DAT(0x22),

    CMD(0x35),
    DAT(0xBB),

    CMD(0x36),
    DAT(0xAA),

    CMD(0x37),
    DAT(0xDD),

    CMD(0x38),
    DAT(0xCC),

    CMD(0x39),
    DAT(0x66),

    CMD(0x3A),
    DAT(0x77),

    CMD(0x3B),
    DAT(0x22),

    CMD(0x3C),
    DAT(0x22),

    CMD(0x3D),
    DAT(0x22),

    CMD(0x3E),
    DAT(0x22),

    CMD(0x3F),
    DAT(0x22),

    CMD(0x40),
    DAT(0x22),

    CMD(0x52),
    DAT(0x10),

    CMD(0x53),
    DAT(0x12),

    CMD(0xFF),
    DAT(0xFF),
    DAT(0x98),
    DAT(0x06),
    DAT(0x04),
    DAT(0x07),

    CMD(0x17),
    DAT(0x32),

    CMD(0x02),
    DAT(0x77),

    CMD(0xE1),
    DAT(0x79),

    CMD(0x26),
    DAT(0xB2),

    CMD(0xFF),
    DAT(0xFF),
    DAT(0x98),
    DAT(0x06),
    DAT(0x04),
    DAT(0x00),

    CMD(0x36),
    DAT(0x0A),

    CMD(0x3A),
    DAT(0x55),

    CMD(0x29),

    CMD(0x11),
    
    {DELAY_MS,50},
END_INIT_TAB

LCD_OP_SECTION lcddev_t ili9806eFW02 = {
    .name = "ili9806e-XJ040FW02",
    .lcd_bus_type = LCD_BUS_RGB,
    .bus_width = LCD_BUS_WIDTH_8*3,
    .red_width = LCD_BUS_WIDTH_5,
    .green_width = LCD_BUS_WIDTH_6,
    .blue_width = LCD_BUS_WIDTH_5,
    .scan_mode = LCD_ROTATE_0,
    .pclk_div = 4,//pclk div
    .even_order = 0,
    .odd_order = 0,
    .lcd_data_mode = (0<<31)|//data inversion mode
                     (0<<24)|//data compress mode
                     (0<<20)|//fifo mode
                     (0<<17)|//output cycle 2 shift direction
                     (0<<12)|//output cycle 2 shift bit
                     (0<<11)|//output cycle 1 shift direction
                     (0<<6)| //output cycle 1 shift bit
                     (0<<5)| //output cycle 0 shift direction
                     (0<<0), //output cycle 0 shift bit
    .screen_w = 480,
    .screen_h = 854,
    .video_x  = 0,
    .video_y  = 0,
    .video_w  = 854,
    .video_h  = 480,
    
    .clk_per_pixel = 1,

    .pclk_inv = 1,
    
    .vlw = 1,
    .vbp = 20,
    .vfp = 9,//real vfp=vfp+1(line)
    .hlw = 1,
    .hbp = 5,
    .hfp = 86,//real hfp=.hfp(pixel) + 1(pclk)
    
    .de_inv = 0,
    .hs_inv = 1,
    .vs_inv = 1,
    
    .spi_cpol  = 0,
    .spi_cpha  = 0,
    .spi_order = 0,
    .spi_bits  = 9,
    
    .de_ccm = {0x100,0x0,0x0,
               0x0,0x100,0x0,
               0x0,0x0,0x100,0x0,0x0,0x0},
    .de_saj = {0x40,0x40,0x40,0x40,0x40},
    .lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
    .lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
    
    INIT_TAB_INIT
};
#endif

#if DEV_LCD == DEV_LCD_ILI9806E1540
//--------------------------- WV1540 ---------------------------------
BEGIN_INIT_TAB
    CMD(0xFF),
    DAT(0xFF),
    DAT(0x98),
    DAT(0x06),
    DAT(0x04),
    DAT(0x01),
    // Change to Page 

    CMD(0x08),
    DAT(0x10),
    // output SDA
    
    CMD(0x20),
    DAT(0x01),
    // 0:DE mode ,1:SYNC mode
    
    CMD(0x21),
    DAT(0x0D),
    // DE = 1 Active

    CMD(0x27),
    DAT(0x3),
    // HBP(7:0)
    CMD(0x28),
    DAT(0x00),
    // HBP(9:8)

    CMD(0x30),
    DAT(0x02),
    // 480 X 800

    CMD(0x31),
    DAT(0x02),
    // 2-dot Inversion    

    CMD(0x40),
    DAT(0x12),
    // DDVDH/L BT 17 -10 5.94 -5.47 -2.3 1.6 43.4 21.88 0.17

    CMD(0x41),
    DAT(0x33),
    // DDVDH/L CLAMP 

    CMD(0x42),
    DAT(0x01),
    // VGH/VGL 

    CMD(0x43),
    DAT(0x03),
    // VGH/VGL     

    CMD(0x44),
    DAT(0x09),
    // VGH/VGL 

    CMD(0x50),
    DAT(0x58),
    // VGMP 4.8

    CMD(0x51),
    DAT(0x58),
    // VGMN 4.8

    CMD(0x52),
    DAT(0x00),
    // LOW VOLTAGE DETECTION

    CMD(0x53),
    DAT(0x48),

    CMD(0x60),
    DAT(0x07),
    // SDTI

    CMD(0x61),
    DAT(0x00),
    // CRTI 00

    CMD(0x62),
    DAT(0x09),
    // EQTI 07

    CMD(0x63),
    DAT(0x00),
    // PCTI  00

    //++++++++++++++++++ Gamma Setting ++++++++++++++++++//

    CMD(0xA0),
    DAT(0x00),
    // Gamma 0 

    CMD(0xA1),
    DAT(0x0c),
    // Gamma 4 

    CMD(0xA2),
    DAT(0x13),
    // Gamma 8

    CMD(0xA3),
    DAT(0x0c),
    // Gamma 16

    CMD(0xA4),
    DAT(0x04),
    // Gamma 24

    CMD(0xA5),
    DAT(0x09),
    // Gamma 52

    CMD(0xA6),
    DAT(0x07),
    // Gamma 80

    CMD(0xA7),
    DAT(0x06),
    // Gamma 108

    CMD(0xA8),
    DAT(0x06),
    // Gamma 147

    CMD(0xA9),
    DAT(0x0A),
    // Gamma 175

    CMD(0xAA),
    DAT(0x11),
    // Gamma 203

    CMD(0xAB),
    DAT(0x08),
    // Gamma 231

    CMD(0xAC),
    DAT(0x0C),
    // Gamma 239

    CMD(0xAD),
    DAT(0x1B),
    // Gamma 247

    CMD(0xAE),
    DAT(0x0D),
    // Gamma 251

    CMD(0xAF),
    DAT(0x00),
    // Gamma 255
    //==============Nagitive

    CMD(0xC0),
    DAT(0x00),
    // Gamma 0 

    CMD(0xC1),
    DAT(0x0C),
    // Gamma 4

    CMD(0xC2),
    DAT(0x13),
    // Gamma 8

    CMD(0xC3),
    DAT(0x0C),
    // Gamma 16

    CMD(0xC4),
    DAT(0x04),
    // Gamma 24

    CMD(0xC5),
    DAT(0x09),
    // Gamma 52

    CMD(0xC6),
    DAT(0x07),
    // Gamma 80

    CMD(0xC7),
    DAT(0x06),
    // Gamma 108

    CMD(0xC8),
    DAT(0x06),
    // Gamma 147

    CMD(0xC9),
    DAT(0x0A),
    // Gamma 175

    CMD(0xCA),
    DAT(0x11),
    // Gamma 203

    CMD(0xCB),
    DAT(0x08),
    // Gamma 231

    CMD(0xCC),
    DAT(0x0C),
    // Gamma 239

    CMD(0xCD),
    DAT(0x1B),
    // Gamma 247

    CMD(0xCE),
    DAT(0x0D),
    // Gamma 251

    CMD(0xCF),
    DAT(0x00),
    // Gamma 255

    //+++++++++++++++++++++++++++++++++++++++++++++++++++//

    CMD(0xFF),
    DAT(0xFF),
    DAT(0x98),
    DAT(0x06),
    DAT(0x04),
    DAT(0x06),

    CMD(0x00),
    DAT(0xA0),

    CMD(0x01),
    DAT(0x05),

    CMD(0x02),
    DAT(0x00),

    CMD(0x03),
    DAT(0x00),

    CMD(0x04),
    DAT(0x01),

    CMD(0x05),
    DAT(0x01),

    CMD(0x06),
    DAT(0x88),

    CMD(0x07),
    DAT(0x04),

    CMD(0x08),
    DAT(0x01),

    CMD(0x09),
    DAT(0x90),

    CMD(0x0A),
    DAT(0x04),

    CMD(0x0B),
    DAT(0x01),

    CMD(0x0C),
    DAT(0x01),

    CMD(0x0D),
    DAT(0x01),

    CMD(0x0E),
    DAT(0x00),

    CMD(0x0F),
    DAT(0x00),

    CMD(0x10),
    DAT(0x55),

    CMD(0x11),
    DAT(0x50),

    CMD(0x12),
    DAT(0x01),

    CMD(0x13),
    DAT(0x85),

    CMD(0x14),
    DAT(0x85),

    CMD(0x15),
    DAT(0xC0),

    CMD(0x16),
    DAT(0x0B),

    CMD(0x17),
    DAT(0x00),

    CMD(0x18),
    DAT(0x00),

    CMD(0x19),
    DAT(0x00),

    CMD(0x1A),
    DAT(0x00),

    CMD(0x1B),
    DAT(0x00),

    CMD(0x1C),
    DAT(0x00),

    CMD(0x1D),
    DAT(0x00),

    CMD(0x20),
    DAT(0x01),

    CMD(0x21),
    DAT(0x23),

    CMD(0x22),
    DAT(0x45),

    CMD(0x23),
    DAT(0x67),

    CMD(0x24),
    DAT(0x01),

    CMD(0x25),
    DAT(0x23),

    CMD(0x26),
    DAT(0x45),

    CMD(0x27),
    DAT(0x67),

    CMD(0x30),
    DAT(0x02),

    CMD(0x31),
    DAT(0x22),

    CMD(0x32),
    DAT(0x11),

    CMD(0x33),
    DAT(0xAA),

    CMD(0x34),
    DAT(0xBB),

    CMD(0x35),
    DAT(0x66),

    CMD(0x36),
    DAT(0x00),

    CMD(0x37),
    DAT(0x22),

    CMD(0x38),
    DAT(0x22),

    CMD(0x39),
    DAT(0x22),

    CMD(0x3A),
    DAT(0x22),

    CMD(0x3B),
    DAT(0x22),

    CMD(0x3C),
    DAT(0x22),

    CMD(0x3D),
    DAT(0x22),

    CMD(0x3E),
    DAT(0x22),

    CMD(0x3F),
    DAT(0x22),

    CMD(0x40),
    DAT(0x22),

    CMD(0x52),
    DAT(0x10),

    CMD(0x53),
    DAT(0x12),
    ////****************************************************************************//

    CMD(0xFF),
    DAT(0xFF),
    DAT(0x98),
    DAT(0x06),
    DAT(0x04),
    DAT(0x07),
    
    CMD(0x18),
    DAT(0x1D),       

    CMD(0x17),
    DAT(0x32),   

    CMD(0x02),
    DAT(0x77),

    CMD(0xE1),
    DAT(0x79),

    ////****************************************************************************//

    CMD(0xFF),
    DAT(0xFF),
    DAT(0x98),
    DAT(0x06),
    DAT(0x04),
    DAT(0x00),
    {DELAY_MS,1},

    CMD(0x36),
    DAT(0x00),

    CMD(0x3A),
    DAT(0x55),


    CMD(0x29),

    CMD(0x11),
    
    {DELAY_MS,50},
END_INIT_TAB

BEGIN_UNINIT_TAB
    CMD(0x28),
    CMD(0x10),
END_UNINIT_TAB

//无白色边框的屏
LCD_OP_SECTION lcddev_t ili9806eWV1540 = 
{
    .name = "ili9806e-XJ040WV1540",
    .lcd_bus_type = LCD_BUS_RGB,
    .bus_width = LCD_BUS_WIDTH_8*3,
    .red_width = LCD_BUS_WIDTH_5,
    .green_width = LCD_BUS_WIDTH_6,
    .blue_width = LCD_BUS_WIDTH_5,
    .scan_mode = LCD_ROTATE_0,
    .pclk_div = 3,//pclk div
    .even_order = 0,
    .odd_order = 0,
    .lcd_data_mode = (0<<31)|//data inversion mode
                     (0<<24)|//data compress mode
                     (0<<20)|//fifo mode
                     (0<<17)|//output cycle 2 shift direction
                     (0<<12)|//output cycle 2 shift bit
                     (0<<11)|//output cycle 1 shift direction
                     (0<<6)| //output cycle 1 shift bit
                     (0<<5)| //output cycle 0 shift direction
                     (0<<0), //output cycle 0 shift bit
    .screen_w = 480,
    .screen_h = 800,
    .video_x  = 0,
    .video_y  = 0,
    .video_w  = 480,
    .video_h  = 272,
	.osd_x = 0,
	.osd_y = 0,
	.osd_w = 0, // 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 0, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK

    .clk_per_pixel = 1,

    .pclk_inv = 1,
    
    .vlw = 1,
    .vbp = 20,
    .vfp = 10,//真正的vfp=vbp+1(line)
    .hlw = 1,
    .hbp = 3,
    .hfp = 140,
    
    .de_inv = 0,
    .hs_inv = 1,
    .vs_inv = 1,
    
    .spi_cpol  = 0,
    .spi_cpha  = 0,
    .spi_order = 0,
    .spi_bits  = 9,
    
    .brightness = -12,
    .saturation = 7,
    .contrast   = 7,
    .contra_index = 8,

    .gamma_red = 3,
    .gamma_green=3,
    .gamma_blue=3,
    
    .de_ccm = {0x100,0x0,0x0,
               0x0,0x100,0x0,
               0x0,0x0,0x100,0x0,0x0,0x0},
    .de_saj = {0x40,0x40,0x40,0x40,0x40},
    .lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
    .lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
    
    INIT_TAB_INIT
    UNINIT_TAB_INIT
};

#endif

