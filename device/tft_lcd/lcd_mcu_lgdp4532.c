#include "tft_lcd.h"

#if DEV_LCD  == DEV_LCD_lgdp4532
#define  H_WBIC_480x272_NUM  5

#if H_WBIC_480x272_NUM == 0
	//水平
	#define WBIC_TAB0	   0x00007f00UL
	#define WBIC_TAB1	   0xff0b7bfaUL
	#define WBIC_TAB2	   0xfd1c6ff7UL
	#define WBIC_TAB3	   0xfa315df7UL
	#define WBIC_TAB4	   0xf84748f8UL
	#define WBIC_TAB5	   0xf75c32faUL
	#define WBIC_TAB6	   0xf76e1dfdUL
	#define WBIC_TAB7	   0xfa7a0cffUL
	//垂直
	#define HBIC_TAB0	   0x00007f00UL
	#define HBIC_TAB1	   0xff0b7bfaUL
	#define HBIC_TAB2	   0xfd1c6ff7UL
	#define HBIC_TAB3	   0xfa315df7UL
	#define HBIC_TAB4	   0xf84748f8UL
	#define HBIC_TAB5	   0xf75c32faUL
	#define HBIC_TAB6	   0xf76e1dfdUL
	#define HBIC_TAB7	   0xfa7a0cffUL//*///0%

#elif H_WBIC_480x272_NUM == 1 //3.0
	//水平
	#define WBIC_TAB0	   0x00F98DF9
	#define WBIC_TAB1	   0x000188F6
	#define WBIC_TAB2	   0x001178F6
	#define WBIC_TAB3	   0xFE2960F8
	#define WBIC_TAB4	   0xFB4445FB
	#define WBIC_TAB5	   0xF86029FE
	#define WBIC_TAB6	   0xF6781100
	#define WBIC_TAB7	   0xF6880100

	//垂直
	#define HBIC_TAB0	   0x00F98DF9
	#define HBIC_TAB1	   0x000188F6
	#define HBIC_TAB2	   0x001178F6
	#define HBIC_TAB3	   0xFE2861F8
	#define HBIC_TAB4	   0xFB4445FB
	#define HBIC_TAB5	   0xF86029FE
	#define HBIC_TAB6	   0xF6791000
	#define HBIC_TAB7	   0xF6890000

#elif H_WBIC_480x272_NUM == 2 //2.5
	//水平
	#define WBIC_TAB0	   0xFF057605
	#define WBIC_TAB1	   0xFD1174FD
	#define WBIC_TAB2	   0xFB2269F9
	#define WBIC_TAB3	   0xF9355AF7
	#define WBIC_TAB4	   0xF74849F7
	#define WBIC_TAB5	   0xF75A35F9
	#define WBIC_TAB6	   0xF96922FB
	#define WBIC_TAB7	   0xFD7312FD

	//垂直
	#define HBIC_TAB0	   0xFF047804
	#define HBIC_TAB1	   0xFE1173FD
	#define HBIC_TAB2	   0xFB216AF9
	#define HBIC_TAB3	   0xF9345BF7
	#define HBIC_TAB4	   0xF74849F7
	#define HBIC_TAB5	   0xF75A35F9
	#define HBIC_TAB6	   0xF96A21FB
	#define HBIC_TAB7	   0xFD7410FE

#elif H_WBIC_480x272_NUM ==  3 //2.0
	//水平
	#define WBIC_TAB0	   0xFA155B15
	#define WBIC_TAB1	   0xF9205B0B
	#define WBIC_TAB2	   0xF92C5604
	#define WBIC_TAB3	   0xFA394DFF
	#define WBIC_TAB4	   0xFC4443FC
	#define WBIC_TAB5	   0xFF4E38FA
	#define WBIC_TAB6	   0x04562CF9
	#define WBIC_TAB7	   0x0B5A21F9

	//垂直
	#define HBIC_TAB0	   0xFA145D14
	#define HBIC_TAB1	   0xF9205B0B
	#define HBIC_TAB2	   0xF92C5703
	#define HBIC_TAB3	   0xFA394EFE
	#define HBIC_TAB4	   0xFB4445FB
	#define HBIC_TAB5	   0xFE4F38FA
	#define HBIC_TAB6	   0x03572CF9
	#define HBIC_TAB7	   0x0B5B20F9
	
#elif H_WBIC_480x272_NUM ==  4 //1.75
	#define WBIC_TAB0	   0xFA1B4F1B
	#define WBIC_TAB1	   0xFB244D13
	#define WBIC_TAB2	   0xFC2D4A0C
	#define WBIC_TAB3	   0xFE364605
	#define WBIC_TAB4	   0x013F3E01
	#define WBIC_TAB5	   0x054636FE
	#define WBIC_TAB6	   0x0C4A2DFC
	#define WBIC_TAB7	   0x134D24FB
	
	#define HBIC_TAB0	   0xFA1B4F1B
	#define HBIC_TAB1	   0xFB244D13
	#define HBIC_TAB2	   0xFC2D4B0B
	#define HBIC_TAB3	   0xFD374605
	#define HBIC_TAB4	   0x003F4000
	#define HBIC_TAB5	   0x054637FD
	#define HBIC_TAB6	   0x0B4B2DFC
	#define HBIC_TAB7	   0x134E23FB
#elif H_WBIC_480x272_NUM == 5 //1.5
	//水平
	#define WBIC_TAB0	   0xFD204220
	#define WBIC_TAB1	   0xFE264219
	#define WBIC_TAB2	   0x012C3F13
	#define WBIC_TAB3	   0x04323C0D
	#define WBIC_TAB4	   0x08373808
	#define WBIC_TAB5	   0x0D3C3204
	#define WBIC_TAB6	   0x133F2C01
	#define WBIC_TAB7	   0x194226FE


	//垂直
	#define HBIC_TAB0	   0xFD204220
	#define HBIC_TAB1	   0xFE264219
	#define HBIC_TAB2	   0x002C4013
	#define HBIC_TAB3	   0x03323D0D
	#define HBIC_TAB4	   0x08383708
	#define HBIC_TAB5	   0x0D3C3303
	#define HBIC_TAB6	   0x13402C00
	#define HBIC_TAB7	   0x194226FE
#elif H_WBIC_480x272_NUM == 6  //1.25
	#define WBIC_TAB0	   0x02223922
	#define WBIC_TAB1	   0x0526371D
	#define WBIC_TAB2	   0x082A3518
	#define WBIC_TAB3	   0x0C2D3214
	#define WBIC_TAB4	   0x10302F10
	#define WBIC_TAB5	   0x14332C0C
	#define WBIC_TAB6	   0x18352A08
	#define WBIC_TAB7	   0x1D372605

	#define HBIC_TAB0	   0x02223922
	#define HBIC_TAB1	   0x0526371D
	#define HBIC_TAB2	   0x082A3518
	#define HBIC_TAB3	   0x0B2D3314
	#define HBIC_TAB4	   0x0F30310F
	#define HBIC_TAB5	   0x14332D0B
	#define HBIC_TAB6	   0x18352A08
	#define HBIC_TAB7	   0x1D372605
#else  //1.0
	//水平
	#define WBIC_TAB0	   0x0B223022
	#define WBIC_TAB1	   0x0D242F1F
	#define WBIC_TAB2	   0x10262D1C
	#define WBIC_TAB3	   0x13282B19
	#define WBIC_TAB4	   0x162A2916
	#define WBIC_TAB5	   0x192B2813
	#define WBIC_TAB6	   0x1C2D2610
	#define WBIC_TAB7	   0x1F2E250D

	//垂直
	#define HBIC_TAB0	   0x0A223122
	#define HBIC_TAB1	   0x0D242F1F
	#define HBIC_TAB2	   0x10262D1C
	#define HBIC_TAB3	   0x13282B19
	#define HBIC_TAB4	   0x152A2B15
	#define HBIC_TAB5	   0x192C2713
	#define HBIC_TAB6	   0x1C2D2610
	#define HBIC_TAB7	   0x1F2E250D
#endif
#define CMD(x)	{MCU_CMD,x>>8},{MCU_CMD,x}
#define DAT(x)	{MCU_DAT,x}
#define DELAY(x)	{DELAY_MS,x}
#define END		{LCD_TAB_END,LCD_TAB_END}
//LCD_INTI_TAB_SECTION static u32 lgdp4532_register_init_tab[][2] = 
//{
	BEGIN_INIT_TAB

	CMD(0x00),DAT(0x00),DAT(0x01),
	CMD(0x15),DAT(0x00),DAT(0x30),	
	CMD(0x11),DAT(0x00),DAT(0x40),
	CMD(0x10),DAT(0x16),DAT(0x28),
	CMD(0x12),DAT(0x00),DAT(0x00),
	CMD(0x13),DAT(0x10),DAT(0x47),	//1047 104d
	CMD(0x12),DAT(0x00),DAT(0x10),	
	CMD(0x10),DAT(0x26),DAT(0x20),	
	CMD(0x13),DAT(0x30),DAT(0x44),
//-----Gamma control-----------------------
	CMD(0x30),DAT(0x00),DAT(0x00),	
	CMD(0x31),DAT(0x04),DAT(0x02),	
	CMD(0x32),DAT(0x01),DAT(0x06),	
	CMD(0x33),DAT(0x05),DAT(0x03),	
	CMD(0x34),DAT(0x01),DAT(0x04),	
	CMD(0x35),DAT(0x03),DAT(0x01),	
	CMD(0x36),DAT(0x07),DAT(0x07),	
	CMD(0x37),DAT(0x03),DAT(0x05),	
	CMD(0x38),DAT(0x02),DAT(0x08),	
	CMD(0x39),DAT(0x0f),DAT(0x0b),
//-----Normal Setting-----------------------
	CMD(0x01),DAT(0x01),DAT(0x00),
	CMD(0x02),DAT(0x03),DAT(0x00),
	CMD(0x03),DAT(0x10),DAT(0x80),	//1080
	CMD(0x08),DAT(0x06),DAT(0x04),
	CMD(0x09),DAT(0x00),DAT(0x00),
	CMD(0x0A),DAT(0x00),DAT(0x00),//0000 08
	CMD(0x17),DAT(0x07),DAT(0x71),	
	CMD(0x41),DAT(0x00),DAT(0x02),	
	CMD(0x60),DAT(0x27),DAT(0x00),	
	CMD(0x61),DAT(0x00),DAT(0x01),
	CMD(0x90),DAT(0x01),DAT(0x99), //199 182
	CMD(0x93),DAT(0x00),DAT(0x01),
	CMD(0xa3),DAT(0x00),DAT(0x10),

	CMD(0x50),DAT(0x00),DAT(0x00),		//设置水平开始地址
	CMD(0x51),DAT(0x00),DAT(0xef),		//设置水平结束地址	
	CMD(0x52),DAT(0x00),DAT(0x00),		//设置水平开始地址
	CMD(0x53),DAT(0x01),DAT(0x3f),	
	CMD(0x21),DAT(0x01),DAT(0x3f),
	CMD(0x20),DAT(0x00),DAT(0xef),
//-----Display on-----------------------	
	CMD(0x07),DAT(0x00),DAT(0x01),	
	CMD(0x07),DAT(0x00),DAT(0x21),	
	CMD(0x07),DAT(0x00),DAT(0x23),	
	CMD(0x07),DAT(0x00),DAT(0x33),	
	CMD(0x07),DAT(0x01),DAT(0x33),	
	CMD(0x22),


END_INIT_TAB
//};

LCD_OP_SECTION lcddev_t lcdc_init_tab = 
{
	.name = "lgdp4532",
	.lcd_bus_type = LCD_BUS_I80,
	.bus_width = LCD_BUS_WIDTH_8,
	.red_width = LCD_BUS_WIDTH_5,
	.green_width = LCD_BUS_WIDTH_6,
	.blue_width = LCD_BUS_WIDTH_5,
	.scan_mode = LCD_ROTATE_90,//rotate 90
	.te_mode = 0xff,//te mode, 0xff:disable
	.bus_cmd = 0,
	//f(wr) = source_clk/div/2
	//f(wr) >= screen_w * screen_h * clk_per_pixel * 60
	.pclk_div = PLL_CLK/(320*240*2*60)/2,
    .even_order = 0,
    .odd_order = 0,
	.lcd_data_mode = (DATA_NOT_INVER<<31)|//data inversion mode
					 (RGB565<<24)|//data compress mode
					 (MODE24BIT_S<<20)|//fifo mode
					 (SDIR2_UP<<17)|//output cycle 2 shift direction
					 (0<<12)|//output cycle 2 shift bit
					 (SDIR1_UP<<11)|//output cycle 1 shift direction
					 (0<<6)| //output cycle 1 shift bit
					 (SDIR0_UP<<5)| //output cycle 0 shift direction
					 (8<<0), //output cycle 0 shift bit
	.screen_w = 240,
	.screen_h = 320,
	.video_x  = 0,
	.video_y  = 0,
	.video_w  = 320,
    .video_h  = 240,
	.osd_x = 0,
	.osd_y = 0,
	.osd_w = 0, // 0 : value will set to video_w  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK
	.osd_h = 0, // 0 : value will set to video_h  , use for 4:3 LCD +16:9 sensor show UPDOWN BLACK

	
//	.init_table = lgdp4532_register_init_tab,
	.clk_per_pixel = 2,
	
	.brightness = -12,
	.saturation = 7,
	.contrast   = 7,
	.contra_index = 8,

	.gamma_red = 3,
	.gamma_green=3,
	.gamma_blue=3,
	
	.lcd_lsawtooth = {{{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB},{WBIC_TAB,HBIC_TAB}}},
	.lcd_bsawtooth = {{{BCTAB},{BCTAB},{BCTAB}}},
	 INIT_TAB_INIT
};



#endif







