#include "tft_lcd.h"

#if DEV_LCD  == DEV_LCD_HX8357B

#define  STRENGTH  5

#if STRENGTH == 0
#define HBIC_TAB0    0x01ff41ff
#define HBIC_TAB1    0x000640fa
#define HBIC_TAB2    0xfe103bf7
#define HBIC_TAB3    0xfb1c32f7
#define HBIC_TAB4    0xf82729f8
#define HBIC_TAB5    0xf7321cfb
#define HBIC_TAB6    0xf73b10fe
#define HBIC_TAB7    0xfa400600
#define WBIC_TAB0    0x40ff0100,0x000001ff
#define WBIC_TAB1    0x3efa0200,0x00000006
#define WBIC_TAB2    0x37f80201,0x0000fe10
#define WBIC_TAB3    0x2df80201,0x0101fb1b
#define WBIC_TAB4    0x26f90101,0x0101f924
#define WBIC_TAB5    0x1bfb0101,0x0102f82d
#define WBIC_TAB6    0x10fe0000,0x0102f837
#define WBIC_TAB7    0x06000000,0x0002fa3e

#elif STRENGTH == 1
#define HBIC_TAB0    0xff033b03
#define HBIC_TAB1    0xfd0b3afe
#define HBIC_TAB2    0xfb1437fa
#define HBIC_TAB3    0xf91f30f8
#define HBIC_TAB4    0xf82729f8
#define HBIC_TAB5    0xf8301ff9
#define HBIC_TAB6    0xfa3714fb
#define HBIC_TAB7    0xfe3a0bfd
#define WBIC_TAB0    0x3b03ff00,0x0100ff03
#define WBIC_TAB1    0x37ff0000,0x0101fd0b
#define WBIC_TAB2    0x33fb0100,0x0101fb14
#define WBIC_TAB3    0x2cf90200,0x0102f91d
#define WBIC_TAB4    0x26f80200,0x0002f826
#define WBIC_TAB5    0x1df90201,0x0002f92c
#define WBIC_TAB6    0x14fb0101,0x0001fb33
#define WBIC_TAB7    0x0bfd0101,0x0000ff37

#elif STRENGTH == 2
#define HBIC_TAB0    0xfd083308
#define HBIC_TAB1    0xfb0f3402
#define HBIC_TAB2    0xf91732fe
#define HBIC_TAB3    0xf9202cfb
#define HBIC_TAB4    0xf92628f9
#define HBIC_TAB5    0xfb2c20f9
#define HBIC_TAB6    0xfe3217f9
#define HBIC_TAB7    0x02340ffb
#define WBIC_TAB0    0x3508fc01,0x0101fc08
#define WBIC_TAB1    0x3302fe01,0x0002fb0f
#define WBIC_TAB2    0x2fff0000,0x0002f917
#define WBIC_TAB3    0x2bfb0000,0x0002f91f
#define WBIC_TAB4    0x26fa0100,0x0001fa24
#define WBIC_TAB5    0x1ff90200,0x0000fb2b
#define WBIC_TAB6    0x17f90200,0x0000ff2f
#define WBIC_TAB7    0x0ffb0200,0x01fe0233

#elif STRENGTH == 3
#define HBIC_TAB0    0xfb0c2d0c
#define HBIC_TAB1    0xfa122d07
#define HBIC_TAB2    0xfa182c02
#define HBIC_TAB3    0xfb1f2600
#define HBIC_TAB4    0xfd2224fd
#define HBIC_TAB5    0x00261ffb
#define HBIC_TAB6    0x022c18fa
#define HBIC_TAB7    0x072d12fa
#define WBIC_TAB0    0x2e0dfb01,0x0001fb0d
#define WBIC_TAB1    0x2e07fc01,0x0001fa13
#define WBIC_TAB2    0x2b03fd01,0x0001fa19
#define WBIC_TAB3    0x2700fe01,0x0000fb1f
#define WBIC_TAB4    0x25fd0001,0x0100fd1f
#define WBIC_TAB5    0x1ffb0000,0x01fe0027
#define WBIC_TAB6    0x19fa0100,0x01fd032b
#define WBIC_TAB7    0x13fa0100,0x01fc072e

#elif STRENGTH == 4
#define HBIC_TAB0    0xfb0f270f
#define HBIC_TAB1    0xfc14250b
#define HBIC_TAB2    0xfd182407
#define HBIC_TAB3    0xff1c2203
#define HBIC_TAB4    0x00202000
#define HBIC_TAB5    0x03221cff
#define HBIC_TAB6    0x072418fd
#define HBIC_TAB7    0x0b2514fc
#define WBIC_TAB0    0x2711fb00,0x0100fb11
#define WBIC_TAB1    0x260cfb00,0x0100fc16
#define WBIC_TAB2    0x2508fb01,0x01fffd1a
#define WBIC_TAB3    0x2304fc01,0x01feff1e
#define WBIC_TAB4    0x2201fd01,0x01fd0120
#define WBIC_TAB5    0x1efffe01,0x01fc0423
#define WBIC_TAB6    0x1afdff01,0x01fb0825
#define WBIC_TAB7    0x16fc0001,0x00fb0c26

#elif STRENGTH == 5
#define HBIC_TAB0    0xfe112011
#define HBIC_TAB1    0xff141f0e
#define HBIC_TAB2    0x00171f0a
#define HBIC_TAB3    0x02191e07
#define HBIC_TAB4    0x051b1b05
#define HBIC_TAB5    0x071e1902
#define HBIC_TAB6    0x0a1f1700
#define HBIC_TAB7    0x0e1f14ff
#define WBIC_TAB0    0x2113fefe,0x01fefe13
#define WBIC_TAB1    0x2010fdff,0x01fdff17
#define WBIC_TAB2    0x220cfcff,0x01fc001a
#define WBIC_TAB3    0x1f09fc00,0x00fc031d
#define WBIC_TAB4    0x2006fc00,0x00fc061c
#define WBIC_TAB5    0x1d03fc00,0x00fc091f
#define WBIC_TAB6    0x1a00fc01,0xfffc0c22
#define WBIC_TAB7    0x17fffd01,0xfffd1020

#elif STRENGTH == 6
#define HBIC_TAB0    0x01121b12
#define HBIC_TAB1    0x03131b0f
#define HBIC_TAB2    0x0515190d
#define HBIC_TAB3    0x0616190b
#define HBIC_TAB4    0x08181808
#define HBIC_TAB5    0x0b191606
#define HBIC_TAB6    0x0d191505
#define HBIC_TAB7    0x0f1b1303
#define WBIC_TAB0    0x1a1402fd,0x00fd0214
#define WBIC_TAB1    0x1b1200fd,0x00fc0416
#define WBIC_TAB2    0x190f00fd,0xfffd0619
#define WBIC_TAB3    0x190dfffd,0xfefd081b
#define WBIC_TAB4    0x1c0afefe,0xfefe0a18
#define WBIC_TAB5    0x1b08fdfe,0xfdff0d19
#define WBIC_TAB6    0x1906fdff,0xfd000f19
#define WBIC_TAB7    0x1604fc00,0xfd00121b

#elif STRENGTH == 7
#define HBIC_TAB0    0x06111811
#define HBIC_TAB1    0x07121710
#define HBIC_TAB2    0x0913160e
#define HBIC_TAB3    0x0a14150d
#define HBIC_TAB4    0x0b15150b
#define HBIC_TAB5    0x0d15140a
#define HBIC_TAB6    0x0e161309
#define HBIC_TAB7    0x10171207
#define WBIC_TAB0    0x141206ff,0xfeff0612
#define WBIC_TAB1    0x131005ff,0xfe000813
#define WBIC_TAB2    0x140f04fe,0xfe000914
#define WBIC_TAB3    0x140e02fe,0xfe000b15
#define WBIC_TAB4    0x160c01fe,0xfe010c14
#define WBIC_TAB5    0x150b00fe,0xfe020e14
#define WBIC_TAB6    0x140900fe,0xfe040f14
#define WBIC_TAB7    0x130800fe,0xff051013

#endif


#define CMD(x)     {MCU_CMD,x}
#define DAT(x)   {MCU_DAT,x}
#define DLY(x)	{DELAY_MS,x}

#define  CODE_VERSION    0x31

BEGIN_INIT_TAB
    #if CODE_VERSION == 0x10  // v1
    CMD(0x11), //Sleep Out
    DLY(150),

    CMD(0xB0), 
    DAT(0x00), 
    DLY(120),

    CMD(0xB4), //Set RM, DM
    DAT(0x00), //

    CMD(0xC1), 
    DAT(0x00),
    DAT(0x10),// 0x11
    DAT(0x22),

    CMD(0xC5), 
    DAT(0x0f),

    CMD(0xC8), //Set Gamma
    DAT(0x00),
    DAT(0x57),
    DAT(0x04),
    DAT(0x60),
    DAT(0x00),
    DAT(0x1e),
    DAT(0x37),
    DAT(0x02),
    DAT(0x77),
    DAT(0x06),
    DAT(0x0f),
    DAT(0x00),

    CMD(0xD0), //Set Power
    DAT(0x44), //DDVDH
    DAT(0x41),
    DAT(0x06), //VREG1

    CMD(0xD1), //Set VCOM
    DAT(0x1c), //VCOMH    0x3a
    DAT(0x0c), //VCOML    0x9

    CMD(0xD2), //Set NOROW
    DAT(0x05), //SAP
    DAT(0x12), //DC10/00

    CMD(0xE9), //Set Panel
    DAT(0x01),

    CMD(0xEA), //Set STBA
    DAT(0x03),
    DAT(0x00),
    DAT(0x00),

    CMD(0x3a), //
    DAT(0x55), 

    CMD(0x36), //     
    DAT(0x20), //0x40

    CMD(0X2A),
    DAT(0X00),
    DAT(0X00),
    DAT(0x01),
    DAT(0xDF),

    CMD(0X2B),
    DAT(0X00),
    DAT(0X00),
    DAT(0x01),
    DAT(0x3F),


    CMD(0x21), 
    CMD(0x29), //Display On
    DLY(10),
    CMD(0x2c),
    #elif CODE_VERSION == 0x20  // v2
    CMD(0x11), //Sleep Out
    DLY(120), 
        
    CMD(0xB0), 
    DAT(0x00), 
    DLY(120),
        
        
    CMD(0xB4), //Set RM, DM
    DAT(0x00), //
    //---------add v1
    CMD(0xC1), 
    DAT(0x00),
    DAT(0x10),// 0x11
    DAT(0x22),

    CMD(0xC5), 
    DAT(0x0f),
        
    CMD(0xC8), //Set Gamma
    DAT(0x00),
    DAT(0x57),
    DAT(0x04),
    DAT(0x60),
    DAT(0x00),
    DAT(0x1E),
    DAT(0x37),
    DAT(0x02),
    DAT(0x77),
    DAT(0x06),
    DAT(0x0F),
    DAT(0x00),

    CMD(0xD0), //Set Power
    DAT(0x44), //DDVDH
    DAT(0x41),
    DAT(0x08), //VREG1

    CMD(0xD1), //Set VCOM
    DAT(0x28), //VCOMH   28
    DAT(0x14), //VCOML   14

    CMD(0xD2), //Set NOROW
    DAT(0x05), //SAP
    DAT(0x12), //DC10/00

    CMD(0xE9), //Set Panel
    DAT(0x01),

    CMD(0xEA),  //Set STBA
    DAT(0x03),
    DAT(0x00),
    DAT(0x00),

    CMD(0x3A), //Set Panel
    DAT(0x55),

    CMD(0x36), //Set Panel
    DAT(0x20),

    CMD(0X2A),
    DAT(0X00),
    DAT(0X00),
    DAT(0x01),
    DAT(0xDF),

    CMD(0X2B),
    DAT(0X00),
    DAT(0X00),
    DAT(0x01),
    DAT(0x3F),
    CMD(0x29), //Display On
    DLY(5),
    CMD(0x2c),
    #elif CODE_VERSION == 0x21
    CMD(0x11), //Sleep Out
    DLY(160),

    CMD(0xB0),
    DAT(0x00),
    DLY(160),

    CMD(0xB4), //Set RM, DM
    DAT(0x00), //
    ///---------v1
    //CMD(0xC1), 
    //DAT(0x00),
    //DAT(0x10),// 0x11
    //DAT(0x22),

    CMD(0xC5), 
    DAT(0x0f),
    #if 1

    CMD(0xC8), //Set Gamma
    DAT(0x00),//1
    DAT(0x46),//2
    DAT(0x14),//3
    DAT(0x21),//4
    DAT(0x00),//5
    DAT(0x0E),//6
    DAT(0x36),//7
    DAT(0x13),//8
    DAT(0x77),//9
    DAT(0x06),//10
    DAT(0x07),//11
    DAT(0x00),//12

    #else
    CMD(0xC8), //Set Gamma
    DAT(0x00),//1
    DAT(0x57),//2
    DAT(0x04),//3
    DAT(0x60),//4
    DAT(0x00),//5
    DAT(0x1E),//6
    DAT(0x37),//7
    DAT(0x02),//8
    DAT(0x77),//9
    DAT(0x06),//10
    DAT(0x0F),//11
    DAT(0x00),//12
    #endif

    CMD(0xD0), //Set Power
    DAT(0x40), //DDVDH
    DAT(0x45),
    DAT(0x0a), //VREG1

    CMD(0xD1), //Set VCOM
    //DAT(0x4a), //VCOMH   28
    //DAT(0x1d), //VCOML   14
    DAT(0x4a), //VCOMH   28
    DAT(0x1f), //VCOML   14

    CMD(0xD2), //Set NOROW
    DAT(0x05), //SAP
    DAT(0x12), //DC10/00

    CMD(0xE9), //Set Panel
    DAT(0x01),

    CMD(0xEA),  //Set STBA
    DAT(0x03),
    DAT(0x00),
    DAT(0x00),

    CMD(0x3A), //Set Panel
    DAT(0x55),

    CMD(0x36), //Set Panel
    DAT(0x20),

    CMD(0X2A),
    DAT(0X00),
    DAT(0X00),
    DAT(0x01),
    DAT(0xDF),

    CMD(0X2B),
    DAT(0X00),
    DAT(0X00),
    DAT(0x01),
    DAT(0x3F),

    CMD(0x29), //Display On
    DLY(20),

    CMD(0x2c), //Display On
    DLY(2),
	#elif CODE_VERSION == 0x31// add for te test
		
	CMD(0x11),//
	DLY(200),
	CMD(0xB0),
	DAT(0x00),

	CMD(0xB3),
	DAT(0x02),
	DAT(0x00),
	DAT(0x00),
	DAT(0x00),

	//C_out16(0xb4,0x00),
	CMD(0xB4),
	DAT(0x00),//0x00

	CMD(0xC6),
	DAT(0x02),

	CMD(0xC0),    //set PANEL 
	DAT(0x14),   //DAT //REV,SM,GS 
	DAT(0x3b),   //DAT 
	DAT(0x00),   //DAT
	DAT(0x02),   //DAT 
	DAT(0x11),   //DAT 

	CMD(0xC1),   
	DAT(0x10),   //Bit4:0-->frame inversion,1-->line inversion
	DAT(0x10),   
	DAT(0x22), 

	CMD(0xC5),//Freme rate
	DAT(0x07),  //0x06==54fps  0x07==59fps

	CMD(0xC8),   //GAMMA
	DAT(0x00), //DAT 
	DAT(0x25), //DAT  
	DAT(0x11), //DAT 
	DAT(0x21), //DAT
	DAT(0x09), //DAT
	DAT(0x0C), //DAT  
	DAT(0x66), //DAT 
	DAT(0x25), //DAT 
	DAT(0x77), //DAT 
	DAT(0x12), //DAT
	DAT(0x06), //DAT 
	DAT(0x12), //DAT 

	CMD (0xD0),   //set power
	DAT(0x43), //DAT//DDVDH,VCI1,VCL 
	DAT(0x41), //DAT//VGH,VGL 
	DAT(0x0A), //DAT//VREG1  01--4.125V,06--4.625V

	CMD (0xD1),   //set vcom
	DAT(0x35), //DAT//VCOMH  //25
	DAT(0x09), //DAT//VCOML  //09
	 
	CMD (0xD2),    //set NOROW
	DAT(0x05), //DAT //SAP
	DAT(0x12), //DAT //DC10/00

	CMD(0xEA),
	DAT(0x03), 
	DAT(0x00), 
	DAT(0x00), 

	CMD (0xEE),   //SET EQ
	DAT(0x00), //DAT 
	DAT(0x00), //DAT    
	DAT(0x00), //DAT 
	DAT(0x00), //DAT 

	CMD (0xED),   //SET DIR TIM--Source/gate/vcom timing set 
	DAT(0x00), //DAT 
	DAT(0x00), //DAT 
	DAT(0xA2), //DAT 
	DAT(0xA2), //DAT 
	DAT(0xA2), //DAT 
	DAT(0xA2), //DAT 
	DAT(0x00), //DAT 
	DAT(0x00), //DAT 
	DAT(0x00), //DAT 
	DAT(0x00), //DAT 
	DAT(0xAE), //DAT 
	DAT(0xAE), //DAT 
	DAT(0x01), //DAT 
	DAT(0xA2), //DAT 
	DAT(0x00), //DAT 			   

	CMD(0x53),   
	DAT(0xff), //DAT 

	CMD(0x35),///TE on
	DAT(0x00),

	CMD(0x3A), //Set Panel
	DAT(0x55),

	#if 0 //  TE OFF window=480X320
	CMD(0x36), //Set Panel
	DAT(0xa8),
	CMD(0X2A),
	DAT(0X00),
	DAT(0X00),
	DAT(0x01),
	DAT(0xDF),

	CMD(0X2B),
	DAT(0X00),
	DAT(0X00),
	DAT(0x01),
	DAT(0x3F),
	#elif 1//  TE ON  && window==320X480
	CMD(0x36), //Set Panel
	DAT(0x08),
	
	CMD(0X2B),
	DAT(0X00),
	DAT(0X00),
	DAT(0x01),
	DAT(0xDF),

	CMD(0X2A),
	DAT(0X00),
	DAT(0X00),
	DAT(0x01),
	DAT(0x3F),
	#endif
	CMD(0x29), //Display On
	DLY(20),
	CMD(0x2c), //Display On
		
    #endif
END_INIT_TAB

LCD_OP_SECTION lcddev_t hx8357b = 
{
    .name = "HX8357B",
    .lcd_bus_type = LCD_BUS_I80,
    .bus_width = LCD_BUS_WIDTH_8,
    .red_width = LCD_BUS_WIDTH_5,
    .green_width = LCD_BUS_WIDTH_6,
    .blue_width = LCD_BUS_WIDTH_5,
    .scan_mode = LCD_ROTATE_90,
    .te_mode =  1,//0xff,//te mode, 0xff:disable
    //f(wr) = source_clk/div/2
    //f(wr) >= screen_w * screen_h * clk_per_pixel * 60
    .pclk_div = PLL_CLK/(320*480*2*30)/2,// when enable TE,it should not set too fast
    .even_order = RGB,
    .odd_order = RGB,
    .lcd_data_mode = (DATA_NOT_INVER<<31)|//data inversion mode
                     (RGB565<<24)|//data compress mode
                     (MODE24BIT_S<<20)|//fifo mode
                     (SDIR2_UP<<17)|//output cycle 2 shift direction
                     (0<<12)|//output cycle 2 shift bit
                     (SDIR1_UP<<11)|//output cycle 1 shift direction
                     (0<<6)| //output cycle 1 shift bit
                     (SDIR0_UP<<5)| //output cycle 0 shift direction
                     (8<<0), //output cycle 0 shift bit
    .screen_w = 320,//480,
    .screen_h = 480,//320,
    .video_x  = 0,
    .video_y  = 0,
    .video_w  = 480,
    .video_h  = 320,
    .osd_x = 0,
	.osd_y = 0,
	.osd_w = 0,	// 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 0, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK

    .clk_per_pixel = 2,
    /*
    .brightness = 0,
    .saturation = 10,
    .contrast   = 9,
    .contra_index = 4,

    .gamma_red = 4,
    .gamma_green=4,
    .gamma_blue=4,
    */

    .brightness = -16, // 5
    .saturation = 9, // 11
    .contrast   = 7, // 10
    .contra_index = 9, // 6

    .gamma_red = 0,
    .gamma_green=0,
    .gamma_blue=0,
    
    .de_ccm = {0x100,0x0,0x0,
               0x0,0x100,0x0,
               0x0,0x0,0x100,0x0,0x0,0x0},
    .de_saj = {0x40,0x40,0x40,0x40,0x40},
    .lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
    .lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
    
    INIT_TAB_INIT
};

#endif




























