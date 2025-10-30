#include "tft_lcd.h"


#define CMD(x)	 {SPI_DAT,x}
#define DAT(x)   {SPI_DAT,((x)|(1<<8))}
#define DELAY(x) {DELAY_MS,(x)}

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

#if DEV_LCD  == DEV_LCD_ST7701WV1604
//--------------------------- XJ040WV1604 ---------------------------------
BEGIN_INIT_TAB
    CMD(0x11),
//------------------------------------------Bank0 Setting----------------------------------------------------//
//------------------------------------Display Control setting----------------------------------------------//
    DELAY(100),

    CMD(0xff),
    DAT(0x77),
    DAT(0x01),
    DAT(0x00),
    DAT(0x00),
    DAT(0x10),
    //Set lines = 800
    CMD(0xC0),
    DAT(0x63),
    DAT(0x00),
    //Set VBP = 0x0A,VFP = 0x14
    CMD(0xC1),
    DAT(0x0A),
    DAT(0x14),
    //Set inversion
    CMD(0xC2),
    DAT(0x31),
    DAT(0x08), //set min pclks each line = 512 + 8 * 16 = 640(plck)
    //Set HS/VS/DE/PCLK
    CMD(0xC3),
    DAT(0x00),//enable DE mode,VS low active,HS low active,PCLK sample raise edge,DE high active
    DAT(0x10),//VBP for HV mode
    DAT(0x08),//HBP for HV mode
//-------------------------------------Gamma Cluster Setting-------------------------------------------//
    CMD(0xB0),
    DAT(0x00),
    DAT(0x11),
    DAT(0x19),
    DAT(0x0C),
    DAT(0x10),
    DAT(0x06),
    DAT(0x07),
    DAT(0x0A),
    DAT(0x09),
    DAT(0x22),
    DAT(0x04),
    DAT(0x10),
    DAT(0x0E),
    DAT(0x28),
    DAT(0x30),
    DAT(0x1C),

    CMD(0xB1),
    DAT(0x00),
    DAT(0x12),
    DAT(0x19),
    DAT(0x0D),
    DAT(0x10),
    DAT(0x04),
    DAT(0x06),
    DAT(0x07),
    DAT(0x08),
    DAT(0x23),
    DAT(0x04),
    DAT(0x12),
    DAT(0x11),
    DAT(0x28),
    DAT(0x30),
    DAT(0x1C),
//---------------------------------------End Gamma Setting----------------------------------------------//
//------------------------------------End Display Control setting----------------------------------------//
//-----------------------------------------Bank0 Setting End---------------------------------------------//
//-------------------------------------------Bank1 Setting---------------------------------------------------//
//-------------------------------- Power Control Registers Initial --------------------------------------//
    CMD(0xff),
    DAT(0x77),
    DAT(0x01),
    DAT(0x00),
    DAT(0x00),
    DAT(0x11),

    CMD(0xb0),
    DAT(0x7d),
//-------------------------------------------Vcom Setting---------------------------------------------------//
    CMD(0xb1),
    DAT(0x3E),
//-----------------------------------------End Vcom Setting-----------------------------------------------//
    CMD(0xb2),
    DAT(0x07),

    CMD(0xb3),
    DAT(0x80),

    CMD(0xb5),
    DAT(0x47),

    CMD(0xb7),
    DAT(0x8A),

    CMD(0xb8),
    DAT(0x21),

    CMD(0xC1),
    DAT(0x78),

    CMD(0xC2),
    DAT(0x78),

    CMD(0xD0),
    DAT(0x88),
//---------------------------------End Power Control Registers Initial -------------------------------//
    DELAY (100),
//---------------------------------------------GIP Setting----------------------------------------------------//
    CMD(0xe0),
    DAT(0x00),
    DAT(0x00),
    DAT(0x02),

    CMD(0xe1),
    DAT(0x04),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x05),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x20),
    DAT(0x20),

    CMD(0xe2),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),

    CMD(0xe3),
    DAT(0x00),
    DAT(0x00),
    DAT(0x33),
    DAT(0x00),

    CMD(0xe4),
    DAT(0x22),
    DAT(0x00),

    CMD(0xe5),
    DAT(0x04),
    DAT(0x34),
    DAT(0xAA),
    DAT(0xAA),
    DAT(0x06),
    DAT(0x34),
    DAT(0xAA),
    DAT(0xAA),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),

    CMD(0xe6),
    DAT(0x00),
    DAT(0x00),
    DAT(0x33),
    DAT(0x00),

    CMD(0xe7),
    DAT(0x22),
    DAT(0x00),

    CMD(0xe8),
    DAT(0x05),
    DAT(0x34),
    DAT(0xAA),
    DAT(0xAA),
    DAT(0x07),
    DAT(0x34),
    DAT(0xAA),
    DAT(0xAA),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),

    CMD(0xeb),
    DAT(0x02),
    DAT(0x00),
    DAT(0x40),
    DAT(0x40),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),

    CMD(0xed),
    DAT(0xfA),
    DAT(0x45),
    DAT(0x0B),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xB0),
    DAT(0x54),
    DAT(0xAf),
//---------------------------------------------End GIP Setting-----------------------------------------------//
//------------------------------ Power Control Registers Initial End-----------------------------------//
//------------------------------------------Bank1 Setting----------------------------------------------------//
    CMD(0xFF),
    DAT(0x77),
    DAT(0x01),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    //Set RGB order = RGB
    CMD(0x36),
    DAT(0x00),
    //Set pixel bits = 24bit/pixel
    CMD(0x3A),
    DAT(0x70),
    
    DELAY(100),

    CMD(0x29),
END_INIT_TAB

LCD_OP_SECTION const lcddev_t st7701_WV1604 = {
	.name = "st7701-XJ040WV1604",
	.lcd_bus_type = 0,
	.bus_width = 24,
	.red_width = 8,
	.green_width = 8,
	.blue_width = 8,
	.scan_mode = LCD_ROTATE_90,
	.pclk_div = PLL_CLK/24000000,//pclk div
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
	.video_w  = 800,
    .video_h  = 480,
//	.vsrc_w   = 720,
//    .vsrc_h   = 240,
	
	.clk_per_pixel = 1,

	.pclk_inv = 1,
	
    .vlw = 1,
	.vbp = 10,
	.vfp = 20,
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
    
	.brightness = 4,
	.saturation = 10,
	.contrast   = 8,
    .contra_index = 6,
	
	.gamma_red = 4,
	.gamma_green=4,
	.gamma_blue=5,
	
	.de_ccm = {0x100,0x0,0x0,
           0x0,0x100,0x0,
           0x0,0x0,0x100,0x0,0x0,0x0},
    .de_saj = {0x40,0x40,0x40,0x40,0x40},
    .lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
    .lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
	
	INIT_TAB_INIT
};
#endif

#if DEV_LCD  == DEV_LCD_ST7701FW1601
//--------------------------- XJ040WV1604 ---------------------------------
BEGIN_INIT_TAB
    CMD(0x11),
//------------------------------------------Bank0 Setting----------------------------------------------------//
//------------------------------------Display Control setting----------------------------------------------//
    DELAY(100),

    CMD(0xff),
    DAT(0x77),
    DAT(0x01),
    DAT(0x00),
    DAT(0x00),
    DAT(0x10),
    //Set lines = 854
    CMD(0xC0),
    DAT(0xE9),
    DAT(0x03),
    //Set VBP = 0x0A,VFP = 0x14
    CMD(0xC1),
    DAT(0x0A),
    DAT(0x14),
    //Set inversion
    CMD(0xC2),
    DAT(0x31),
    DAT(0x08), //set min pclks each line = 512 + 8 * 16 = 640(plck)
    //Set HS/VS/DE/PCLK
    CMD(0xC3),
    DAT(0x00),//enable DE mode,VS low active,HS low active,PCLK sample raise edge,DE high active
    DAT(0x10),//VBP for HV mode
    DAT(0x08),//HBP for HV mode
//-------------------------------------Gamma Cluster Setting-------------------------------------------//
    CMD(0xB0),
    DAT(0x00),
    DAT(0x12),
    DAT(0x1A),
    DAT(0x0D),
    DAT(0x11),
    DAT(0x07),
    DAT(0x0C),
    DAT(0x0A),
    DAT(0x09),
    DAT(0x26),
    DAT(0x05),
    DAT(0x11),
    DAT(0x10),
    DAT(0x2B),
    DAT(0x33),
    DAT(0x1B),

    CMD(0xB1),
    DAT(0x00),
    DAT(0x12),
    DAT(0x1A),
    DAT(0x0D),
    DAT(0x11),
    DAT(0x06),
    DAT(0x0B),
    DAT(0x07),
    DAT(0x08),
    DAT(0x26),
    DAT(0x03),
    DAT(0x11),
    DAT(0x0F),
    DAT(0x2B),
    DAT(0x33),
    DAT(0x1B),
    //---------------------------------------End Gamma Setting----------------------------------------------//
    //------------------------------------End Display Control setting----------------------------------------//
    //-----------------------------------------Bank0 Setting End---------------------------------------------//
    //-------------------------------------------Bank1 Setting---------------------------------------------------//
    //-------------------------------- Power Control Registers Initial --------------------------------------//
    CMD(0xff),
    DAT(0x77),
    DAT(0x01),
    DAT(0x00),
    DAT(0x00),
    DAT(0x11),

    CMD(0xb0),
    DAT(0x4D),
    //-------------------------------------------Vcom Setting---------------------------------------------------//
    CMD(0xb1),
    DAT(0x2E),//0x2E
    //-----------------------------------------End Vcom Setting-----------------------------------------------//
    CMD(0xb2),
    DAT(0x07),

    CMD(0xb3),
    DAT(0x80),

    CMD(0xb5),
    DAT(0x47),

    CMD(0xb7),
    DAT(0x8A),

    CMD(0xb8),
    DAT(0x21),

    CMD(0xC1),
    DAT(0x78),

    CMD(0xC2),
    DAT(0x78),

    CMD(0xD0),
    DAT(0x88),
    //---------------------------------End Power Control Registers Initial -------------------------------//
    DELAY (100),
    //---------------------------------------------GIP Setting----------------------------------------------------//
    CMD(0xe0),
    DAT(0x00),
    DAT(0x00),
    DAT(0x02),

    CMD(0xe1),
    DAT(0x04),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x05),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x20),
    DAT(0x20),

    CMD(0xe2),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),

    CMD(0xe3),
    DAT(0x00),
    DAT(0x00),
    DAT(0x33),
    DAT(0x00),

    CMD(0xe4),
    DAT(0x22),
    DAT(0x00),

    CMD(0xe5),
    DAT(0x04),
    DAT(0x5C),
    DAT(0xA0),
    DAT(0xA0),
    DAT(0x06),
    DAT(0x5C),
    DAT(0xA0),
    DAT(0xA0),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),

    CMD(0xe6),
    DAT(0x00),
    DAT(0x00),
    DAT(0x33),
    DAT(0x00),

    CMD(0xe7),
    DAT(0x22),
    DAT(0x00),

    CMD(0xe8),
    DAT(0x05),
    DAT(0x5C),
    DAT(0xA0),
    DAT(0xA0),
    DAT(0x07),
    DAT(0x5C),
    DAT(0xA0),
    DAT(0xA0),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),

    CMD(0xeb),
    DAT(0x02),
    DAT(0x00),
    DAT(0x40),
    DAT(0x40),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),

    CMD(0xed),
    DAT(0xfA),
    DAT(0x45),
    DAT(0x0B),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xff),
    DAT(0xB0),
    DAT(0x54),
    DAT(0xAf),
//---------------------------------------------End GIP Setting-----------------------------------------------//
//------------------------------ Power Control Registers Initial End-----------------------------------//
//------------------------------------------Bank1 Setting----------------------------------------------------//
    CMD(0xFF),
    DAT(0x77),
    DAT(0x01),
    DAT(0x00),
    DAT(0x00),
    DAT(0x00),
    //Set RGB order = RGB
    CMD(0x36),
    DAT(0x00),
    //Set pixel bits = 24bit/pixel
    CMD(0x3A),
    DAT(0x70),
    
    DELAY(100),

    CMD(0x29),
END_INIT_TAB


LCD_OP_SECTION const lcddev_t st7701_FW1601 = {
	.name = "st7701-XJ050FW1601",
	.lcd_bus_type = 0,
	.bus_width = 24,
	.red_width = 8,
	.green_width = 8,
	.blue_width = 8,
	.scan_mode = LCD_ROTATE_90,
	.pclk_div = 3,//PLL_CLK/26400000,//pclk div
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
    .osd_x = 0,
	.osd_y = 0,
	.osd_w = 0,	// 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 0, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
    
	.clk_per_pixel = 1,

	.pclk_inv = 1,
	
    .vlw = 1,
	.vbp = 20,
	.vfp = 10,
	.hlw = 1,
	.hbp = 3,
	.hfp = 40,
	
	.de_inv = 0,
	.hs_inv = 1,
	.vs_inv = 1,
	
	.spi_cpol  = 0,
	.spi_cpha  = 0,
	.spi_order = 0,
	.spi_bits  = 9,
    
	.brightness = -4,
	.saturation = 8,
	.contrast   = 7,
    .contra_index = 6,
	
	.gamma_red = 4,
	.gamma_green=4,
	.gamma_blue=5,
    
    .de_ccm = {0x100,0x0,0x0,
               0x0,0x100,0x0,
               0x0,0x0,0x100,0x0,0x0,0x0},
    .de_saj = {0x40,0x40,0x40,0x40,0x40},
    .lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
    .lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
    
    INIT_TAB_INIT
};
#endif