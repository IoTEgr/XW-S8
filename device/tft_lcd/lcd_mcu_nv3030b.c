#include "tft_lcd.h"

#if DEV_LCD  == DEV_LCD_NV3030B

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

#define CMD(x)    {MCU_CMD,x}
#define DAT(x)    {MCU_DAT,x}
#define DLAY(x)    {DELAY_MS,x}

BEGIN_INIT_TAB
//BOE	
CMD(0x11),// exit sleep                                                                                                
DLAY(200), 


CMD(0xFD),  
DAT(0x06),  
DAT(0x08), 

CMD(0x61),  
DAT(0x07), 
DAT(0x04), 

CMD(0x73),  
DAT(0x70), 

CMD(0xFD),  
DAT(0xFA),  
DAT(0xFC), 

CMD(0xFD),  
DAT(0x06),  
DAT(0x08), 

CMD(0x61),  
DAT(0x07), 
DAT(0x04), 

CMD(0x73),  
DAT(0x70), 

CMD(0xFD),  
DAT(0xFA),  
DAT(0xFC), 

CMD(0xfd),//private_access
DAT(0x06),	
DAT(0x08),

CMD(0x61),//	
DAT(0x07),//
DAT(0x04),//
//bias
CMD(0x62),//bias setting	
DAT(0x00),//01
DAT(0x44),//04	44
DAT(0x40),//44	65	40

//VSP
CMD(0x65),//Pump1=4.7MHz //PUMP1 VSP
DAT(0x08),//D6-5:pump1_clk[1:0] clamp  28	2b
DAT(0x10),//6.26	
DAT(0x21),

//VSN
CMD(0x66),	//pump=2 AVCL		  
DAT(0x08), //clamp  08	 0b	 09
DAT(0x10),	//10
DAT(0x21),

//add source_neg_time
CMD(0x67),//pump_sel
DAT(0x21),//21	20
DAT(0x40),

//gamma vap/van
CMD(0x68),//gamma vap/van
DAT(0x9F),//78-90-af	 9f
DAT(0x30),//	
DAT(0x2a),// 
DAT(0x21),//

CMD(0xb1),//frame rate 	
DAT(0x0f),//0x0f fr_h[5:0]
DAT(0x02),//0x02 fr_v[4:0]
DAT(0x00),//0x04 fr_div[2:0]

CMD(0xB4),	
DAT(0x01),	//1dot 

////porch
CMD(0xB5),	
DAT(0x02),//0x02 vfp[6:0]	
DAT(0x02),//0x02 vbp[6:0]	
DAT(0x0a),//0x0A hfp[6:0]	
DAT(0x14),//0x14 hbp[6:0]

CMD(0xB6),         //display function                                                                                                     
DAT(0x44),   //rev  sm                                                                                                   
//DAT(0x01), 	//gs  norblack
DAT(0x70), 	//gs  norblack
DAT(0x9f),                                                                                                        
DAT(0x00),
DAT(0x02),    
//source
CMD(0xE6),	
DAT(0x00),	
DAT(0xff),//SC_EN_START[7:0]  f0	

CMD(0xE7),	
DAT(0x01),//CS_START[3:0]  01 	
DAT(0x04),//scdt_inv_sel cs_vp_en	
DAT(0x03),//CS1_WIDTH[7:0]	12
DAT(0x03),//CS2_WIDTH[7:0]	12
DAT(0x00),//PREC_START[7:0]  06 	
DAT(0x12),//PREC_WIDTH[7:0]  12


CMD(0xE8),	//source
DAT(0x00), //VCMP_OUT_EN	 81-vcmp/vref_output pad
DAT(0x50),	//chopper_sel[6:4]
DAT(0x00), //gchopper_sel[6:4]

////gate 
CMD(0xEc),	
DAT(0x52),//47

////gamme sel 						
CMD(0xdf),//	
DAT(0x11),//gofc_gamma_en_sel=1

////gamma_test1	A1#_wangly
CMD(0xe0), //gmama set 2.2
DAT(0x04), //PKP0[4:0] 
DAT(0x05), //PKP1[4:0] 	
DAT(0x0c), //PKP2[4:0]  		
DAT(0x0f), //PKP3[4:0]  		 
DAT(0x0e), //PKP4[4:0]   
DAT(0x10), //PKP5[4:0]  	 
DAT(0x0f), //PKP6[4:0]       
DAT(0x16), //PKP6[4:0]   
CMD(0xe3),
DAT(0x16), //PKN0[4:0]   
DAT(0x10), //PKN1[4:0]   
DAT(0x10), //PKN2[4:0]     
DAT(0x0d), //PKN3[4:0]   
DAT(0x0e), //PKN4[4:0]    	 
DAT(0x09), //PKN5[4:0]    		
DAT(0x04), //PKN6[4:0]    
DAT(0x05), //PKN6[4:0]    		 
CMD(0xe1),
DAT(0x2f), //PRP0[6:0]  	
DAT(0x76), //PRP1[6:0]  	 
CMD(0xe4),												 
DAT(0x75), //PRN0[6:0]       
DAT(0x2f), //PRN1[6:0]  
CMD(0xe2),
DAT(0x26), //VRP0[5:0] 
DAT(0x1f), //VRP1[5:0] 	 
DAT(0x23), //VRP2[5:0]  
DAT(0x36), //VRP3[5:0] 	  
DAT(0x38), //VRP4[5:0]  		 
DAT(0x3f), //VRP5[5:0]  		
CMD(0xe5),
DAT(0x3f), //VRN0[5:0]     
DAT(0x3a), //VRN1[5:0]  	
DAT(0x36), //VRN2[5:0]  	    
DAT(0x20), //VRN3[5:0]  
DAT(0x27), //VRN4[5:0]  	 
DAT(0x26), //VRN5[5:0]  	

CMD(0xF6),
DAT(0x01),
DAT(0x30),//epf[1:0]
DAT(0x00),
DAT(0x00),//SPI2L: 40

/*CMD(0xF1),	
DAT(0x01),//te
DAT(0x01),
DAT(0x02),*/

CMD(0xfd),	
DAT(0xfa),	
DAT(0xfc),
	
CMD(0x3a),
DAT(0x55),

CMD(0x36),
DAT(0x00),

CMD(0x21),

CMD(0x35),
DAT(0x00),  //   00       


CMD(0x2a),  //0-239
DAT(0x00),
DAT(0x00),
DAT(0x00),
DAT(0xef),

CMD(0x2b),	//0-319
DAT(0x00),
DAT(0x00),
DAT(0x01),
DAT(0x3f),
                                                                                                              
CMD(0x29), // display on        
DLAY(10),
CMD(0x2c),
END_INIT_TAB

LCD_OP_SECTION lcddev_t nv3030b = 
{
    .name = "nv3030b",
    .lcd_bus_type = LCD_BUS_I80,
    .bus_width = LCD_BUS_WIDTH_8,
    .red_width = LCD_BUS_WIDTH_5,
    .green_width = LCD_BUS_WIDTH_6,
    .blue_width = LCD_BUS_WIDTH_5,
    .scan_mode = LCD_ROTATE_90,//LCD_ROTATE_270,//rotate 90
    .te_mode = 1,//te mode, 0xff:disable
    //.bus_cmd = 1,
    //f(wr) = source_clk/div/2
    //f(wr) >= screen_w * screen_h * clk_per_pixel * 60
    .pclk_div = PLL_CLK/(320*240*2*80)/2,
    .even_order = 0,
    .odd_order = 0,
    .lcd_data_mode = (1<<31)|//data inversion mode
                     (2<<24)|//data compress mode
                     (1<<20)|//fifo mode
                     (0<<17)|//output cycle 2 shift direction
                     (0<<12)|//output cycle 2 shift bit
                     (0<<11)|//output cycle 1 shift direction
                     (0<<6)| //output cycle 1 shift bit
                     (0<<5)| //output cycle 0 shift direction
                     (8<<0), //output cycle 0 shift bit
    .screen_w = 240,//240,//240,
    .screen_h = 320,//320,
    .video_x  = 0,
    .video_y  = 30,
    .video_w  = 320,//240,//320,
    .video_h  = 180,//240,//240,
	.osd_x = 0,
	.osd_y = 0,
	.osd_w = 320,//320, // 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 240,//240, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK

    .clk_per_pixel = 2,
    
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
};



#endif


