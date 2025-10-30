/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         OSD DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_osd.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN OSD HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"


#define  R_DE_DISPLAY_CTRL   DE_DISPLAY_CTRL
#define  R_DE_FRAME_CTRL     DE_FRAME_CTRL
#define  R_DE_CSC            DE_CSC_BCS_CFG
#define  R_DE_CRITICAL       DE_CRITICAL_CTRL
#define  R_DE_INT            DE_INT_CTRL
#define  R_DE_STATUS         DE_STATUS
#define  R_DE_SCRENN_SIZE    DE_SCREEN_CFG
#define  R_DE_BACKGROUND     DE_BK_CFG
#define  R_DE_CP_TARGET_CFG  DE_CP_TARGET_CFG

#define  R_VIDEO_Y_ADDR      DE_VIDEO_Y_FB_CFG
#define  R_VIDEO_Y_STRIDE    DE_VIDEO_Y_STRIDE_CFG
#define  R_VIDEO_UV_ADDR     DE_VIDEO_UV_FB_CFG
#define  R_VIDEO_UV_STRIDE   DE_VIDEO_UV_STRIDE_CFG
#define  R_VIDEO_SIZE        DE_VIDEO_SIZE_CFG
#define  R_VIDEO_POS         DE_VIDEO_COOR_CFG
#define  R_VIDEO_SCALE       DE_VIDEO_SCALE_COF_CFG
#define  R_VIDEO_Y_CUR_ADDR  DE_VIDEO_Y_CUR_FB

#define  R_OSD_FBADDR0       DE_OSD0_FB_CFG
#define  R_OSD_SIZE0         DE_OSD0_SIZE_CFG
#define  R_OSD_COOR0         DE_OSD0_COOR_CFG
#define  R_OSD_STRIDE0       DE_OSD0_STRIDE_CFG
#define  R_OSD_PALETTE0      0x50000
#define  R_OSD_FBADDR1       DE_OSD1_FB_CFG
#define  R_OSD_SIZE1         DE_OSD1_SIZE_CFG
#define  R_OSD_COOR1         DE_OSD1_COOR_CFG
#define  R_OSD_STRIDE1       DE_OSD1_STRIDE_CFG
#define  R_OSD_PALETTE1      (R_OSD_PALETTE0+2014)
#define  R_OSD_ALPHA         DE_OSD_ALPHA_CFG
#define  OSD_PALETTE_SIZE    1024


   

	
static void (*ax32xx_deISR)(void);
/*******************************************************************************
* Function Name  : ax32xx_deWait
* Description    : de wait
* Input          :  none
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deWait(void)
{
	volatile int i = 0xf000;
	while((R_DE_FRAME_CTRL & BIT(1)) != 0)
	{
		if((R_DE_STATUS & BIT(9)) == 0)
			break;
		i--;
		if(i==0)
			break;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_deUpdate
* Description    : de update
* Input          :  none
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deUpdate(void)
{
	R_DE_FRAME_CTRL |= (1<<1);
}
/*******************************************************************************
* Function Name  : ax32xx_deGetUpdateStatus
* Description    : get de update status
* Input          :  none
* Output         : bit 1 status
* Return         : 
*******************************************************************************/
u8 ax32xx_deGetUpdateStatus(void)
{
	return (R_DE_FRAME_CTRL & (1<<1))>>1;
}
/*******************************************************************************
* Function Name  : ax32xx_deIRQHandler
* Description    : initial de irq handler
* Input          :  none
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deIRQHandler(void)
{
	if(DE_STATUS & BIT(0))
	{
		DE_STATUS &=~BIT(0);
		if(ax32xx_deISR)
			ax32xx_deISR();
	}
}
/*******************************************************************************
* Function Name  : ax32xx_deISRRegister
* Description    : register de isr 
* Input          :  void (*isr)(void) : call back
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deISRRegister(void (*isr)(void))
{
	ax32xx_deISR = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_deInit
* Description    : initial de for video/osd0/osd1 layers
* Input          :  u16 width : display width
                       u16 height: display height
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deInit(u16 width,u16 height)
{
	ax32xx_sysClkSet(SYS_CLK_DE,1);
//	ax32xx_deWait();
	
	R_DE_CRITICAL = (0<<16)|//osd arbit number
					   (0<<10)|//critical en
					   (0<<9)| //critical mode
					   (15<<0); //critical burst cycle
			   
	R_DE_INT  = (0<<0); //frame end int en
	
	R_DE_STATUS  = (0<<9)| //de busy flag
			     (0<<0); //frame end pending	


    R_DE_SCRENN_SIZE  = ((height - 1)<<16)|((width - 1)<<0); //screen width
	R_DE_DISPLAY_CTRL = 0;			 

//	ax32xx_deUpdate();

	R_DE_FRAME_CTRL  = (1<<1)|//double-buffer register status
			               (1<<0);//de en

	ax32xx_intEnable(IRQ_DE,0);
}
/*******************************************************************************
* Function Name  : ax32xx_deSetBackground
* Description    : set video layer background color
* Input          :  u8 red : red value 
                       u8 green:green value 
                       u8 blue  : blue value 
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deSetBackground(u8 red,u8 green,u8 blue)
{
//    ax32xx_deWait();	
	R_DE_BACKGROUND  = (red<<16)|(green<<8)|(blue<<0);
//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_deSetCSC
* Description    : set video layer csc effect
* Input          :  u16 brightness : brightness
                       u16 saturation:saturation
                       u16 contrast  : contrast
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_deSetCSC(u16 brightness,u16 saturation,u16 contrast)
{
//	ax32xx_deWait();	
	R_DE_CSC  = (brightness<<16)|((saturation & 0x0f)<<8)|((contrast & 0x0f)<<0); //contrast
    R_DE_DISPLAY_CTRL &= ~(1<<12);	
//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetDither
* Description    : set video layer color dither width
* Input          :  u8 red : red color width
                       u8 green:green color width
                       u8 blue  : blue color width
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_videoSetDither(u8 red,u8 green,u8 blue)
{
//	ax32xx_deWait();	
	R_DE_DISPLAY_CTRL &= ~(0x0f<<15);
	R_DE_DISPLAY_CTRL |= ((((red == 5)||(red == 6))<<18)|
                          ((red != 5)<<17)|
                          ((green != 5)<<16)|
                          ((blue != 5)<<15));//dither B width	
//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetScale
* Description    : set video layer scale
* Input          :  u8 tap0 : tap0
                       u8 tap1 : tap1
                       u8 tap2 : tap2
                       u8 tap3 : tap3
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_videoSetScale(u8 tap3,u8 tap2,u8 tap1,u8 tap0)
{
//	ax32xx_deWait();	
	R_VIDEO_SCALE = (tap3<<24)|//tap3 0xfa
                    (tap2<<16)|//tap2 0x26
                    (tap1<<8)| //tap1 0x26
                    (tap0<<0); //tap0 0xfa
	R_DE_DISPLAY_CTRL |= 1<<13;					 
//	ax32xx_deUpdate();		
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetScaleLine
* Description    : set video layer scale specific line
* Input          : u16 line:line number
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_videoSetScaleLine(u16 line,u8 enable)
{
//	ax32xx_deWait();	
    if(enable)
    {
        R_DE_DISPLAY_CTRL &=~(0x3ff<<22);
        R_DE_DISPLAY_CTRL |= (line<<22)|(1<<21);
    }
    else
        R_DE_DISPLAY_CTRL &=~(1<<21);
//	ax32xx_deUpdate();		
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetScanMode
* Description    : set video layer scan mode.scan mode is rotate
* Input          :  u8 scan_mode : scan mode 
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_videoSetScanMode(u8 scan_mode)
{
	if(scan_mode<7)
	{
//		ax32xx_deWait();	
		R_DE_DISPLAY_CTRL &=~(7<<8);
		R_DE_DISPLAY_CTRL |= (scan_mode & 7)<<8;	
//		ax32xx_deUpdate();
	}
}
/*******************************************************************************
* Function Name  : ax32xx_videoGetYAddr
* Description    : set video layer current y buffer addr
* Input          :  
* Output         : None
* Return         : buffer addr
*******************************************************************************/
u32 ax32xx_videoGetYAddr(void)
{
	return (R_VIDEO_Y_CUR_ADDR&0xffffff);
}
/*******************************************************************************
* Function Name  : ax32xx_videoGetUVAddr
* Description    : set video layer current uv buffer addr
* Input          :  
* Output         : None
* Return         : buffer addr
*******************************************************************************/
u32 ax32xx_videoGetUVAddr(void)
{
	return (R_VIDEO_UV_ADDR&0xffffff);
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetAddr
* Description    : set video layer buffer addr
* Input          :  u32 y_addr : ystride
                       u32 uv_addr : uv_addr
                       u8 update  : 1-update now,0-update late
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetAddr(u32 y_addr,u32 uv_addr,u8 update)
{
//	if(update)
//		ax32xx_deWait();	
	R_VIDEO_Y_ADDR = (y_addr<<0);//video y addr,64byte align
	
	R_VIDEO_UV_ADDR = (uv_addr<<0);//video uv addr ,64byte align



//	if(update)
//	   ax32xx_deUpdate();	
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetStride
* Description    : set video layer stride
* Input          :  u32 ystride : ystride
                       u32 uvstride : uvstride
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetStride(u32 ystride,u32 uvstride)
{
//	ax32xx_deWait();	
	DE_VIDEO_Y_STRIDE_CFG = ((ystride>>2)<<0);//video y stride,unit:word
	DE_VIDEO_UV_STRIDE_CFG = ((uvstride>>2)<<0);//video uv stride
//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetSize
* Description    : set video layer size
* Input          :  u16 width : width
                       u16 height : height
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetSize(u16 width,u16 height)
{
//	ax32xx_deWait();	
	R_VIDEO_SIZE  = ((height - 1)<<16)|((width - 1)<<0); //video width	
//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoSetPosition
* Description    : set video position
* Input          :  s16 x : x
                       s16 y : y
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoSetPosition(s16 x,s16 y)
{
//	ax32xx_deWait();	
	R_VIDEO_POS  = (y<<16)|(x<<0); //video start-x	
//	ax32xx_deUpdate();
}
/*******************************************************************************
* Function Name  : ax32xx_videoCopyCfg
* Description    : set video copy function,src size must be 320x240
* Input          : s16 dst_w : dest width,range (320,640]
*                  s16 dst_h : dest height,range (320,640]
*                  bool enable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoCopyCfg(s16 dst_w,s16 dst_h,bool enable)
{
    if(enable)
    {
        R_DE_DISPLAY_CTRL   |= (1<<4);
        R_DE_CP_TARGET_CFG = (dst_h<<16)|
                             (dst_w<<0);
    }
    else
    {
        R_DE_DISPLAY_CTRL   &=~(1<<4);
    }
}
/*******************************************************************************
* Function Name  : ax32xx_videoEnable
* Description    : video enable
* Input          :  u8 en: 0-disable,1-enable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoEnable(u8 en)
{
//	ax32xx_deWait();
	if(en)
		R_DE_DISPLAY_CTRL |= 1;
	else
		R_DE_DISPLAY_CTRL &= ~1;
//	ax32xx_deUpdate();
}

/*******************************************************************************
* Function Name  : ax32xx_degamma_cfg
* Description    : de video gamma table for three channle color
* Input          :  u32 lut : table addr
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_degamma_cfg(u32 *dst,u32 *src,u32 size)
{
	memcpy(dst,src,size);
}



/*******************************************************************************
* Function Name  : ax32xx_videoGammaRed
* Description    : de video gamma table for red color
* Input          :  u32 lut : table addr
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoGammaRed(u32 lut)
{
	u32 *src,*dst,i;

	dst = (u32 *)0x50800;
	src = (u32 *)lut;
	for(i=0;i<128;i++)
	{
		*dst++ = *src++;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_videoGammaRed
* Description    : de video gamma table for green color
* Input          :  u32 lut : table addr
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoGammaGreen(u32 lut)
{
    u32 *src,*dst,i;

	dst = (u32 *)(0x50800+4*128);
	src = (u32 *)lut;
	for(i=0;i<128;i++)
	{
		*dst++ = *src++;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_videoGammaRed
* Description    : de video gamma table for blue color
* Input          :  u32 lut : table addr
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoGammaBlue(u32 lut)
{
    u32 *src,*dst,i;

	dst = (u32 *)(0x50800+8*128);
	src = (u32 *)lut;
	for(i=0;i<128;i++)
	{
		*dst++ = *src++;
	}
}

/*******************************************************************************
* Function Name  : ax32xx_deSetGAMA
* Description    : set GAMA table by index
* Input          :  u8 gamma_index :gamma table index
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_deSetGAMA(u32 gred,u32 ggreen,u32 gblue)
{	
	if((!gred)||(!ggreen)||(!gblue))
		return ;
	ax32xx_videoGammaRed(gred);
	ax32xx_videoGammaGreen(ggreen);
	ax32xx_videoGammaBlue(gblue);

	ax32xx_videoGammaEnable(1);
}
/*******************************************************************************
* Function Name  : ax32xx_deSet_DE_CCM
* Description    : set de_ccm
* Input          :  u32 *p_ccm:pointer of ccm matrix 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_deSet_DE_CCM(u32 *p_ccm)
{	
	if(p_ccm[0] == 0)
		return ;
	DE_CCMC1_CFG = ((p_ccm[2]&0x3ff)<<20)|((p_ccm[1]&0x3ff)<<10)|(p_ccm[0]&0x3ff);
	DE_CCMC2_CFG = ((p_ccm[5]&0x3ff)<<20)|((p_ccm[4]&0x3ff)<<10)|(p_ccm[3]&0x3ff);
	DE_CCMC3_CFG = ((p_ccm[8]&0x3ff)<<20)|((p_ccm[7]&0x3ff)<<10)|(p_ccm[6]&0x3ff);
	DE_CCMOFFSET_CFG = ((p_ccm[11]&0x1f)<<10)|((p_ccm[10]&0x1f)<<5)|(p_ccm[9]&0x1f);
	
	DE_DISPLAY_CTRL |= 1<<20; //ccm enable
}

/*******************************************************************************
* Function Name  : ax32xx_deSet_DE_SAJ
* Description    : set de_saj
* Input          :  u32 *p_saj:pointer of saj 
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_deSet_DE_SAJ(u32 *p_saj)
{	
	if(p_saj[4] == 0)
		return ;
	DE_SATCOEF1_CFG = (p_saj[4]);
	DE_SATCOEF0_CFG = (p_saj[3]<<24)|(p_saj[2]<<16)|(p_saj[1]<<8)|(p_saj[0]);
	
	DE_DISPLAY_CTRL |= 1<<19; //sat enable
}
/*******************************************************************************
* Function Name  : ax32xx_videoGammaEnable
* Description    : enable de video gamma table
* Input          :  u8 en : 1-enable,0-disable
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_videoGammaEnable(u8 en)
{
//	ax32xx_deWait();
	if(en)
		R_DE_DISPLAY_CTRL |= (1<<14);
	else
		R_DE_DISPLAY_CTRL &=~(1<<14);
//	ax32xx_deUpdate();	
}

/*******************************************************************************
* Function Name  : ax32xx_osdEnable
* Description    : osd enable
* Input          : osd : osd id,OSD0,OSD1
                      u8 en: 0-disable,1-enable
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdEnable(u8 osd,u8 en)
{
	if(osd>=OSD_MAX)
		return -1;
//	ax32xx_deWait();
	if(en)
	{
		DE_DISPLAY_CTRL |= 1<<(osd+1);
	}
	else
	{
		DE_DISPLAY_CTRL &= ~(1<<(osd+1));
	}
//    ax32xx_deUpdate();
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetSize
* Description    : osd set size
* Input          : osd : osd id,OSD0,OSD1
                      u16 width : width
                      u16 height: height
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdSetSize(u8 osd,u16 width,u16 height)
{
	if(osd>=OSD_MAX)
		return -1;


//	ax32xx_deWait();
    switch(osd)
    {
		case OSD0 : 
			   R_OSD_SIZE0 = ((height-1)<<16)|(width-1);
			   break;
		case OSD1 : 
			   R_OSD_SIZE1 = ((height-1)<<16)|(width-1);
			   break;
    } 

	
//    ax32xx_deUpdate();
	return 0;	
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetPosition
* Description    : osd set position
* Input          : osd : osd id,OSD0,OSD1
                      u16 x : x
                      u16 y: y
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdSetPosition(u8 osd,s16 x,s16 y)
{
	if(osd>=OSD_MAX)
		return -1;


//	ax32xx_deWait();
    switch(osd)
    {
		case OSD0 : R_OSD_COOR0 = (y<<16)|(x);break;
		case OSD1 : R_OSD_COOR1 = (y<<16)|(x);break;
    } 

 //   ax32xx_deUpdate();
	return 0;	
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetPalette
* Description    : osd set palette
* Input          : osd : osd id,OSD0,OSD1
                      u32 addr : addr
* Output         : None
* Return         : None
*******************************************************************************/
s32 ax32xx_osdSetPalette(u8 osd,u32 addr)
{
	if(osd>=OSD_MAX)
		return -1;

	switch(osd)
	{
		case OSD0 : memcpy((u32 *)R_OSD_PALETTE0,(u32 *)addr,OSD_PALETTE_SIZE);break;
		case OSD1 : memcpy((u32 *)R_OSD_PALETTE1,(u32 *)addr,OSD_PALETTE_SIZE);break;
	}
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetAddr
* Description    : osd set fb addr
* Input          : osd : osd id,OSD0,OSD1
                   u32 addr : addr
                   u32 data_len : 
* Output         : None
* Return         : 
*******************************************************************************/
s32 ax32xx_osdSetAddr(u8 osd,u32 addr,u32 data_len)
{
	if(osd>=OSD_MAX)
		return -1;

	switch(osd)
	{
		case OSD0 : 
            if(!data_len)
                data_len = ((R_OSD_SIZE0 >> 16) + 1) * ((R_OSD_SIZE0 & 0xffff) + 1);
            R_OSD_FBADDR0 = addr;
            R_OSD_STRIDE0 = ((data_len + 7) & ~7) - 1;
            break;
		case OSD1 : 
            if(!data_len)
                data_len = ((R_OSD_SIZE1 >> 16) + 1) * ((R_OSD_SIZE1 & 0xffff) + 1);
            R_OSD_FBADDR1 = addr;
            R_OSD_STRIDE1 = ((data_len + 7) & ~7) - 1;
            break;
	}
	return 0;	
}
/*******************************************************************************
* Function Name  : ax32xx_osdGetAddr
* Description    : get osd layer current buffer addr
* Input          : osd : osd id,OSD0,OSD1
* Output         : None
* Return         : buffer addr
*******************************************************************************/
u32 ax32xx_osdGetAddr(u8 osd)
{
	if(osd>=OSD_MAX)
		return -1;
    
    u32 addr = -1;
    
    if(osd == OSD0)
        addr = R_OSD_FBADDR0 & 0xffffff;
    else if(osd == OSD1)
        addr = R_OSD_FBADDR1 & 0xffffff;

	return addr;	
}
/*******************************************************************************
* Function Name  : ax32xx_osdSetAlpha
* Description    : osd set set alpha type
* Input          : osd : osd id,OSD0,OSD1
                      u8 type : ALPHA_NORMAL,ALPHA_GLOBAL
                      u8 value : gblobal alpha value
* Output         : None
* Return         : 
*******************************************************************************/
s32 ax32xx_osdSetAlpha(u8 osd,u8 type,u8 value)
{
	if(osd>=OSD_MAX)
		return -1;	
	R_OSD_ALPHA &= ~(0x2f<<(osd*16));
	value |= ((type&1)<<5);
	R_OSD_ALPHA |= (value&0x2f<<(osd*16));

	return 0;
}

static const u32 def_args[] = {
    0x40000000,0,//Y_WTAB0
    0x40000000,0,//Y_WTAB1
    0x40000000,0,//Y_WTAB2
    0x40000000,0,//Y_WTAB3
    0x40000000,0,//Y_WTAB4
    0x40000000,0,//Y_WTAB5
    0x40000000,0,//Y_WTAB6
    0x40000000,0,//Y_WTAB7
    0x00004000,0xff083efb,0xfc123af8,0xfa1e31f7,//YHTAB0~3
    0xf82729f8,0xf7311efa,0xf83a12fc,0xfb3e08ff,//YHTAB4~7
    0x40000000,0,//UV_WTAB0
    0x40000000,0,//UV_WTAB1
    0x40000000,0,//UV_WTAB2
    0x40000000,0,//UV_WTAB3
    0x40000000,0,//UV_WTAB4
    0x40000000,0,//UV_WTAB5
    0x40000000,0,//UV_WTAB6
    0x40000000,0,//UV_WTAB7
};
/*******************************************************************************
* Function Name  : ax32xx_osdSetUpscaler
* Description    : osd set set alpha type
* Input          : 
* Output         : None
* Return         : 
*******************************************************************************/
void ax32xx_osdSetUpscaler(u16 src_w,u16 src_h,u16 dst_w,u16 dst_h,u32 * args)
{
#if SDRAM_PSRAM_SEL
//==JT529X remove upscaler==
	if(!args)
		args = (u32 *)def_args;

	DE_YWTABLE0L_CFG  = args[0 ];
	DE_YWTABLE0H_CFG  = args[1 ];
	DE_YWTABLE1L_CFG  = args[2 ];
	DE_YWTABLE1H_CFG  = args[3 ];
	DE_YWTABLE2L_CFG  = args[4 ];
	DE_YWTABLE2H_CFG  = args[5 ];
	DE_YWTABLE3L_CFG  = args[6 ];
	DE_YWTABLE3H_CFG  = args[7 ];
	DE_YWTABLE4L_CFG  = args[8 ];
	DE_YWTABLE4H_CFG  = args[9 ];
	DE_YWTABLE5L_CFG  = args[10];
	DE_YWTABLE5H_CFG  = args[11];
	DE_YWTABLE6L_CFG  = args[12];
	DE_YWTABLE6H_CFG  = args[13];
	DE_YWTABLE7L_CFG  = args[14];
	DE_YWTABLE7H_CFG  = args[15];
	DE_YHTABLE0_CFG   = args[16];
	DE_YHTABLE1_CFG   = args[17];
	DE_YHTABLE2_CFG   = args[18];
	DE_YHTABLE3_CFG   = args[19];
	DE_YHTABLE4_CFG   = args[20];
	DE_YHTABLE5_CFG   = args[21];
	DE_YHTABLE6_CFG   = args[22];
	DE_YHTABLE7_CFG   = args[23];
	DE_UVWTABLE0L_CFG = args[24];
	DE_UVWTABLE0H_CFG = args[25];
	DE_UVWTABLE1L_CFG = args[26];
	DE_UVWTABLE1H_CFG = args[27];
	DE_UVWTABLE2L_CFG = args[28];
	DE_UVWTABLE2H_CFG = args[29];
	DE_UVWTABLE3L_CFG = args[30];
	DE_UVWTABLE3H_CFG = args[31];
	DE_UVWTABLE4L_CFG = args[32];
	DE_UVWTABLE4H_CFG = args[33];
	DE_UVWTABLE5L_CFG = args[34];
	DE_UVWTABLE5H_CFG = args[35];
	DE_UVWTABLE6L_CFG = args[36];
	DE_UVWTABLE6H_CFG = args[37];
	DE_UVWTABLE7L_CFG = args[38];
	DE_UVWTABLE7H_CFG = args[39];

	DE_WSTEP_CFG	   = (src_w<<16)/(dst_w);
	DE_HSTEP_CFG	   = (src_h<<16)/(dst_h);
	DE_VIDEO_SIZE_CFG  = ((src_h - 1)<<16)|//video height
						 ((src_w - 1)<<0); //video width
	DE_SCA_SRCSIZE_CFG = ((src_h - 1)<<16)|//src height
						 ((src_w - 1)<<0); //src width
	DE_SCA_DSTSIZE_CFG = ((dst_h - 1)<<16)|//dst height
						 ((dst_w - 1)<<0); //dst width

	#if PSRAM_BURST16_EN
	DE_DISPLAY_CTRL   &= ~(1<<7);// bit7 : 0 : is burst16 , 1: is burst8
	#else
	DE_DISPLAY_CTRL   |= (1<<7);// bit7 : 0 : is burst16 , 1: is burst8
	#endif

#else

    if(!args)
        args = (u32 *)def_args;
        
    DE_YWTABLE0L_CFG  = args[0 ];
    DE_YWTABLE0H_CFG  = args[1 ];
    DE_YWTABLE1L_CFG  = args[2 ];
    DE_YWTABLE1H_CFG  = args[3 ];
    DE_YWTABLE2L_CFG  = args[4 ];
    DE_YWTABLE2H_CFG  = args[5 ];
    DE_YWTABLE3L_CFG  = args[6 ];
    DE_YWTABLE3H_CFG  = args[7 ];
    DE_YWTABLE4L_CFG  = args[8 ];
    DE_YWTABLE4H_CFG  = args[9 ];
    DE_YWTABLE5L_CFG  = args[10];
    DE_YWTABLE5H_CFG  = args[11];
    DE_YWTABLE6L_CFG  = args[12];
    DE_YWTABLE6H_CFG  = args[13];
    DE_YWTABLE7L_CFG  = args[14];
    DE_YWTABLE7H_CFG  = args[15];
    DE_YHTABLE0_CFG   = args[16];
    DE_YHTABLE1_CFG   = args[17];
    DE_YHTABLE2_CFG   = args[18];
    DE_YHTABLE3_CFG   = args[19];
    DE_YHTABLE4_CFG   = args[20];
    DE_YHTABLE5_CFG   = args[21];
    DE_YHTABLE6_CFG   = args[22];
    DE_YHTABLE7_CFG   = args[23];
    DE_UVWTABLE0L_CFG = args[24];
    DE_UVWTABLE0H_CFG = args[25];
    DE_UVWTABLE1L_CFG = args[26];
    DE_UVWTABLE1H_CFG = args[27];
    DE_UVWTABLE2L_CFG = args[28];
    DE_UVWTABLE2H_CFG = args[29];
    DE_UVWTABLE3L_CFG = args[30];
    DE_UVWTABLE3H_CFG = args[31];
    DE_UVWTABLE4L_CFG = args[32];
    DE_UVWTABLE4H_CFG = args[33];
    DE_UVWTABLE5L_CFG = args[34];
    DE_UVWTABLE5H_CFG = args[35];
    DE_UVWTABLE6L_CFG = args[36];
    DE_UVWTABLE6H_CFG = args[37];
    DE_UVWTABLE7L_CFG = args[38];
    DE_UVWTABLE7H_CFG = args[39];
    
    DE_WSTEP_CFG       = (src_w<<16)/(dst_w);
    DE_HSTEP_CFG       = (src_h<<16)/(dst_h);

    DE_VIDEO_SIZE_CFG  = ((src_h - 1)<<16)|//video height
                         ((src_w - 1)<<0); //video width

    DE_SCA_SRCSIZE_CFG = ((src_h - 1)<<16)|//src height
                         ((src_w - 1)<<0); //src width
    DE_SCA_DSTSIZE_CFG = ((dst_h - 1)<<16)|//dst height
                         ((dst_w - 1)<<0); //dst width

    DE_DISPLAY_CTRL   |= (1<<7);//upscaler en
#endif

}
/*******************************************************************************
* Function Name  : ax32xx_osdEncodeOneCode
* Description    : create a compressed code
* Input          : u8 data : 
                   u32 cnt : need encode 
                   u32 *used : used cnt
* Output         : encoded code
* Return         : none
*******************************************************************************/
u32 ax32xx_osdEncodeOneCode(u8 data,u32 cnt,u32 *used)
{
    const u32 max = (cnt > 0x10000) ? 0x10000 : cnt;
    if(used) *used = max;
    return (0xFF000000 | (data << 16) | (max - 1));
}
