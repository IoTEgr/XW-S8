/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         LCD HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_lcd.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN LCD HAL LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#ifndef HAL_LCD_H
     #define HAL_LCD_H

enum init_tab_token_e {
	MCU_CMD,
	MCU_DAT,
	SPI_DAT,
	DELAY_MS,
	LCD_TAB_END  = -1UL,
};

enum vch_id_e {
    VCH_JPG,
    VCH_ISP,
    VCH_TOP,
    VCH_BOT,
};

enum vch_status_e {
    VCH_DIS = 0,
    VCH_EN,
};

enum RGB_COLOR{
	RGB=0,
	RBG,
	BRG,
	GRB,
	GBR,
	BGR
};

/***************BITS of LCDDMOD0******************/

enum DATA_INV{//BIT[31]    data inversion mode
	DATA_NOT_INVER=0,
	DATA_INVER
};

enum COMPMODE{//BIT[26:24] data compress mode
	RGB888=0,
	RGB666,
	RGB565,
	RGB444,
	RGB453,
	RGB332
// others = reserved
};

enum FIFOMODE{//BIT[23:20]  FIFO mode for data organization
	MODE24BIT_P=0, // 24bit parallel mode 
	MODE24BIT_S,	// 24bit shift mode 
	MODE16BIT_R,	// 16bit red extend mode
	MODE16BIT_B,	// 16bit blue extend mode
	MODE16BIT_C,	// 16bit compact mode
	MODE16BIT_S,	//16bit shift mode
	MODE8BIT_S,	//8bit serial mode 
	MODE7			//mode 7,no description
// others = reserved
};

enum SDIR2{// BIT[17] output cycle 2 shift direction
	SDIR2_UP=0,
	SDIR2_DOWN
};

//BIT[16:12]:SCNT2,output cycle 2 shift bit count, 0 to 23,too many to list

enum SDIR1{// BIT[11] output cycle 1 shift direction
	SDIR1_UP=0,
	SDIR1_DOWN
};

//BIT[10:6]:SCNT2,output cycle 1 shift bit count,0 to 23,too many to list

enum SDIR0{// BIT[5] output cycle 0 shift direction
	SDIR0_UP=0,
	SDIR0_DOWN
};

//BIT[4:0]:SCNT2,output cycle 0 shift bit count,0 to 23,too many to list

/***********end of BITS of LCDDMOD0******************/

typedef struct _lcd_lsawtooth_s {
	u32 anti_lsawtooth[3][24];//0: all lcd  1:half lcd 2:small window
} lcd_lsawtooth_t;

typedef struct _lcd_bsawtooth_s {
	u32 anti_bsawtooth[3][8];//0: all lcd  1:half lcd 2:small window
} lcd_bsawtooth_t;

typedef struct _lcd_desc_s {
	char name[32];
	u8 lcd_bus_type;
	u8 bus_width;
	u8 bus_cmd;
	u8 red_width;
	u8 green_width;
	u8 blue_width;

	u8 scan_mode;
	//only mcu-bus use
	//-1,te disable.
	//0,te just interrupt,no kick TX.用户(写APP的人)自己编写TE中断服务函数,自己控制kick tx.
	//1,te interrupt and auto kick TX,中断被内部使用,用户不应该操作TE中断
	u8 te_mode;
	//for mcu data-bus, cs_setup and cs_hold eq 0 default,so freq(wr) = src_clk/div/2.
	//for rgb bus,freq(pclk) = src_clk/div;
	u8 pclk_div;

	//the 'lcd_data_mode' eq LCDDMOD
	u32 lcd_data_mode;
	u32 lcd_data_mode1;
    
	u16 screen_w,screen_h;
    //video layer在屏幕上的坐标和尺寸
	u16 video_x,video_y,video_w,video_h;
	u16 osd_x,osd_y,osd_w,osd_h;
    //upscaler源尺寸
    u16 vsrc_w,vsrc_h;
	
	u32 (*init_table)[2],
        (*uninit_table)[2],
        init_table_size,
        uninit_table_size;
    
	u8 clk_per_pixel;
    //0:RGB 1:RBG 2:BRG 3:GRB 4:GBR 5:BGR
    //first line : line number = 0
    u8 even_order;
	u8 odd_order;
	//------rgb-bus----------
	u8 pclk_inv;
	//unit:line,real vfp=vfp+1(line)
	u8 vlw,vbp,vfp;
	//unit:pclk
	u8 hlw;
	//unit:pixel,real hfp=.hfp(pixel) + 1(pclk)
	u8 hbp,hfp;

	u8 de_inv;//de默认是高电平有效,如果de_inv是1，则将其反转,0xff:disable
	u8 hs_inv;//hs_inv同de_inv
	u8 vs_inv;//vs_inv同de_inv
	
	u8 spi_cpol; //CPOL = 0,CLK is "Low" when inactive
	u8 spi_cpha; //CPHA = 0,sample edge is first edge
	u8 spi_order;//ORDER = 0,MSB
	u8 spi_bits; //number of data bits
	
	u32 contrast;
	
	s32 brightness;
	u32 saturation;	
	u32 contra_index;

	u32 gamma_red;
	u32 gamma_green;
	u32 gamma_blue;
	
	u32 de_ccm[12];
	u32 de_saj[5];
	lcd_lsawtooth_t  lcd_lsawtooth;
	lcd_bsawtooth_t  lcd_bsawtooth;
} lcddev_t;

typedef struct{
	char name[32];
	u16 screen_w;
	u16 screen_h;
}lcd_common_t;

typedef struct{
	u32 contrast;
	s32 brightness;
	u32 saturation;
}lcd_vde_t;

typedef struct{
	u32 contra_index;
	u32 gamma_red;
	u32 gamma_green;
	u32 gamma_blue;
}lcd_gamma_t;


typedef struct{
	lcd_common_t lcd_common;
	lcd_vde_t lcd_vde;
	lcd_gamma_t lcd_gamma;
	u32 de_ccm[12];
	u32 de_saj[5];
	lcd_lsawtooth_t  lcd_lsawtooth;
}usb_lcddev_t;


typedef struct lcd_buffer_s 
{
	struct lcd_buffer_s * next;
	u32 lcd_buffer_stride;
	u8 * p_lcd_y_buffer;//指向buffer起始
	u8 * p_lcd_uv_buffer;
	u8 * _p_lcd_y_buffer_;//供de使用,de不一定使用起始地址，例如ratate
	u8 * _p_lcd_uv_buffer_;
    u8 status;
    u8 id;
} lcd_buffer_t;
typedef struct
{
	u32 lcd_order;
	u32 lcd_struct_adr;
	u32 lcd_struct_len;
	u32 lcd_gamma_tab_adr;
	u32 lcd_gamma_tab_len;
	u32 lcd_gamma_tab_unit;
	u32 lcd_contra_tab_adr;
	u32 lcd_contra_tab_len;
	u32 lcd_contra_tab_unit;
	u32 align[5];
}_Lcd_Header_;
#define LCD_HEADER_SECTION        SECTION(".lcd_res.header")
extern int _lcd_resource_start_addr;
#define LCD_RES_FLASH_ADDR(x)         ((u32)(x) + ((u32)&_lcd_resource_start_addr))
/*******************************************************************************
* Function Name  : hal_lcdGetShowingBuffer
* Description    : hardware layer ,get lcd displaying buffer
* Input          : none
* Output         : None
* Return         : disp_frame_t *p_lcd_buffer : buffer frame addr
*******************************************************************************/
disp_frame_t * hal_lcdGetShowingBuffer(void);
/*******************************************************************************
* Function Name  : hal_lcdGetIdleBuffer
* Description    : hardware layer ,get lcd idle buffer
* Input          : none
* Output         : None
* Return         : disp_frame_t *p_lcd_buffer : buffer frame addr
*******************************************************************************/
disp_frame_t * hal_lcdGetIdleBuffer(void);
/*******************************************************************************
* Function Name  : hal_lcdSetFrame
* Description    : hardware layer ,frame to lcd show
* Input          : s16 x : position x
                      s16 y : position y
                      u16 w : width
                      u16 h : height
                      void *p_lcd_buffer : buffer frame addr
                      u8 scan_mode : scan mode.rotate
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdSetFrame(s16 x,s16 y,u16 w,u16 h,void *p_lcd_buffer,u8 scan_mode);
/*******************************************************************************
* Function Name  : hal_lcdGetResolution
* Description    : hardware layer ,get lcd resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetResolution(u16 *width,u16 *height);
/*******************************************************************************
* Function Name  : hal_lcdGetOSDShowingResolution
* Description    : hardware layer ,get lcd showing resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetOSDShowingResolution(u16 *width,u16 *height);
/*******************************************************************************
* Function Name  : hal_lcdGetOSDShowingPosition
* Description    : hardware layer ,get lcd showing position
* Input          : u16 *x : y
                      u16 *x : y
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetOSDShowingPosition(u16 *x,u16 *y);
/*******************************************************************************
* Function Name  : hal_lcdGetShowingRotate
* Description    : hardware layer ,get lcd showing rotate degree
* Input          : none
* Output         : None
* Return         : int : degree.LCD_ROTATE_0,LCD_ROTATE_90,LCD_ROTATE_180,LCD_ROTATE_270
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetShowingRotate(void);
/*******************************************************************************
* Function Name  : hal_lcdRegister
* Description    : hardware layer ,lcd device register
* Input          : lcddev_t *lcd_op : lcd device node
* Output         : None
* Return         : int 0: success
*******************************************************************************/
s32 hal_lcdRegister(lcddev_t *lcd_op);
/*******************************************************************************
* Function Name  : hal_lcdFrameEndCallBack
* Description    : register callback for lcd frame end
* Input          :void (*callback)(void) : call back
* Output         : None
* Return         : int 0: success
*******************************************************************************/
s32 hal_lcdFrameEndCallBack(void (*callback)(void));
/*******************************************************************************
* Function Name  : hal_lcdPreviewStart
* Description    : hardware layer ,lcd preview csi frame strat
* Input          : s16 width : sensor frame width
                      u16 height: sensor frame height
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_lcdPreviewStart(u16 width,u16 height);
/*******************************************************************************
* Function Name  : hal_lcdPreviewStop
* Description    : hardware layer ,lcd preview csi frame stop
* Input          : none
* Output         : None
* Return         : 
*******************************************************************************/
s32 hal_lcdPreviewStop(void);
/*******************************************************************************
* Function Name  : hal_lcdSetShowingSize
* Description    : hardware layer ,set lcd showing size
* Input          : u16 srcw : source graph width
                      u16 srch : source graph height
                      u16 width : showing width
                      u16 height: showing height
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_lcdSetShowingSize(u16 srcw,u16 srch,u16 width,u16 height);
/*******************************************************************************
* Function Name  : hal_lcdSetRoate
* Description    : hardware layer ,set lcd rotate degree
* Input          : u8 rotate : rotate degree
* Output         : None
* Return         : 
*******************************************************************************/
s32 hal_lcdSetRoate(u8 rotate);

/*******************************************************************************
* Function Name  : lcd_isp_config
* Description    : lcd isp config
* Input          : lcddev_t *p_lcddev : lcd op node
* Output         : none                                            
* Return         : none
*******************************************************************************/
void lcd_isp_config(lcddev_t *p_lcddev);

/*******************************************************************************
* Function Name  : usb_lcd_res_load
* Description    : usb_lcd_res_load
* Input          : lcddev_t *new_lcddev,lcddev_t *p_devLcd
* Output         : none                                            
* Return         : none
*******************************************************************************/
void usb_lcd_cfg(lcddev_t *new_lcddev,lcddev_t *p_devLcd);

/*******************************************************************************
* Function Name  : hal_lcdSetBufYUV
* Description    : memset buffer color,but U must equ V
* Input          : buffer: lcd buffer pointer
                   y: 
                   u: must u = v
                   v: must u = v
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdSetBufYUV(disp_frame_t * buffer,u8 y,u8 u,u8 v);
/*******************************************************************************
* Function Name  : hal_lcdPreSetPIPEnable
* Description    : set lcd pip enable/disbale
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdPreSetPIPEnable(u8 enable);
/*******************************************************************************
* Function Name  : hal_lcdSetPIPEnable
* Description    : set lcd pip enable/disbale,take effect immediately
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdSetPIPEnable(u8 enable);
/*******************************************************************************
* Function Name  : hal_lcdSetVCH
* Description    : set lcd video channles
* Input          : u8 src:video channle source,enum {VCH_JPG,VCH_ISP}
*                  u8 layer:video channle layer,enum {VCH_TOP,VCH_BOT}
*                  u16 x:if x == -1,means don't change this parameter
*                  u16 y:
*                  u16 w:
*                  u16 h:
*                  u8 vch_en:channle enable,enum {VCH_EN,VCH_DIS},if vch_en == -1,means don't change
* Output         : None
* Return         : int 0: success
                    <0: fail
*******************************************************************************/
int hal_lcdSetVCH(u8 src,u8 layer,u16 x,u16 y,u16 w,u16 h,u8 vch_en);
void hal_lcdDealBuffer(disp_frame_t * p_lcd_buffer);

/*******************************************************************************
* Function Name  : hal_lcdGetBufferResolution
* Description    : hardware layer ,get lcd showing resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetBufferResolution(u16 *width,u16 *height);
s32 hal_lcdGetVideoPos(u16 *x,u16 *y);
/*******************************************************************************
* Function Name  : hal_lcdLCMPowerOff
* Description    : lcd module power off sequence
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdLCMPowerOff(void);
void hal_lcdLCMDisplayOff(void);
void hal_lcdLCMDisplayOn(void);

/*******************************************************************************
* Function Name  : hal_lcdVideoShowCheck
* Description    : check video show state
* Input          :
* Output         : None
* Return         : int 0: success
*******************************************************************************/
s32 hal_lcdVideoShowCheck(void);

/*******************************************************************************
* Function Name  : hal_lcd_vde_cfg
* Description    : config lcd's vde para
* Input          : lcddev_t *p_devLcd
* Output         : none                                            
* Return         : none
*******************************************************************************/
void hal_lcd_vde_cfg(lcddev_t *p_lcddev);

/*******************************************************************************
* Function Name  : hal_lcd_gamma_cfg
* Description    : config lcd's gamma and contrast.use the function after lcdContra lcdGamma 
* Input          : _Lcd_Header_ * p_devLcdHeader,lcddev_t *p_lcddev,lcd_gamma_t *usb_lcddev
* Output         : none                                            
* Return         : none
*******************************************************************************/
void hal_lcd_gamma_cfg(_Lcd_Header_ * p_devLcdHeader,lcddev_t *p_lcddev,lcd_gamma_t *p_lcd_gamma);

/*******************************************************************************
* Function Name  : hal_lcd_lsawtooth
* Description    : anti-aliasing on lcd screen
* Input          :u32 *anti_lsawtooth
* Output         : None
* Return         : int 0: success
*******************************************************************************/
void hal_lcd_lsawtooth(u32 *anti_lsawtooth);

/*******************************************************************************
* Function Name  : hal_lcdSetCsiCrop
* Description    : set csi LDMA crop
* Input          : u16 crop_sx,u16 crop_ex,u16 crop_sy,u16 crop_ey
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdSetCsiCrop(u16 crop_sx,u16 crop_sy,u16 crop_ex,u16 crop_ey);
void hal_lcd_get_next_frame();


#endif
