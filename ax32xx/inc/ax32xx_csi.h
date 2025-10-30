/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         CSI DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_csi.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN CSI HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  AX32XX_CSI_H
	#define  AX32XX_CSI_H

typedef struct
{
	u32 csi_histbuf[128];
	u32 csi_wpbuf[32];
}__CSI_STAT__;
extern __CSI_STAT__ csi_stat;
#define _csi_histbuf_	csi_stat.csi_histbuf
#define _csi_wptbuf_	csi_stat.csi_wpbuf
typedef enum
{
	CSI_TYPE_RAW8=0, // raw8 data 
	CSI_TYPE_RAW10,   // raw10 data 
	CSI_TYPE_RAW12,   // raw12 data 
	CSI_TYPE_YUV422, // YUV422 0-255
	CSI_TYPE_YUVR422,// YUV 422 16-235,16-240
	CSI_TYPE_COLOR_BAR = 0xff,// YUV 422 16-235,16-240
}CSI_TYPE_E;

typedef enum
{
	ISP_CTRL_AWB_STAT_CLR=0x00000001, 
	ISP_CTRL_AE_STAT_CLR =0x00000002,
	ISP_CTRL_CP_STAT_KS  =0x00000004,
	ISP_CTRL_AWB_TAB_KS	 =0x00010000,
	ISP_CTRL_YGAMMA_KS	 =0x00020000,
	ISP_CTRL_RGAMMA_KS	 =0x00040000,
	ISP_CTRL_LS_KS		 =0x00080000,
}ISP_CTRL_E;

typedef enum
{
    CSI_PRIORITY_RGRG=0,
    CSI_PRIORITY_GRGR,
    CSI_PRIORITY_BGBG,
    CSI_PRIORITY_GBGB,

	CSI_PRIORITY_CBY0CRY1=0,
	CSI_PRIORITY_CRY0CBY1,
	CSI_PRIORITY_Y0CBY1CR,
	CSI_PRIORITY_Y0CRY1CB,
}CSI_POLARITY_E;

typedef enum
{
	CSI_MODE_ENABLE          =0x00000001,
	CSI_MODE_VSYNC           =0x00000002,
	CSI_MODE_HSYNC           =0x00000004,
	CSI_MODE_PCLK_INV        =0x00000008,
	CSI_MODE_PCLK_FIR        =0x00000010,
	CSI_MODE_SAMPLE          =0x00000020,
	CSI_MODE_FORMAT          =0x000000C0,
	CSI_MODE_PRIORITY        =0x00000300,
	CSI_MODE_DATA_BW         =0x00000C00,
	CSI_MODE_TEST            =0x0000F000,
	CSI_MODE_TEST_EN         =0x00010000,
	CSI_MODE_DVP_PCLK_DIV    =0x00060000,
	CSI_MODE_DVP_EN          =0x00080000,
}CSI_MODE_E;


typedef enum
{
	CSI_OUTPUT_UVCSIZE   =0x00003fff,
	CSI_OUTPUT_UVCPRIO   =0x0000c000,
	CSI_OUTPUT_UVCEN     =0x00010000,
	CSI_OUTPUT_RELOAD    =0x00020000,
	CSI_OUTPUT_LCDEN     =0x00040000,
	CSI_OUTPUT_MJPGEN    =0x00080000,
	CSI_OUTPUT_WIFIEN    =0x00100000,
	CSI_OUTPUT_LDMAMANEN =0x00200000,
}CSI_OUTPUT_E;
typedef enum
{
	CSI_BURST_4BYTE=0,
	CSI_BURST_8BYTE,
	CSI_BURST_12BYTE,
	CSI_BURST_16BYTE,
	CSI_BURST_20BYTE,
	CSI_BURST_24BYTE,
	CSI_BURST_28BYTE,
	CSI_BURST_32BYTE
}CSI_BURST_E;
typedef enum
{
	CSI_LCDSCALER_1_1=0, // 1/1
	CSI_LCDSCALER_1_2,   // 1/2
	CSI_LCDSCALER_1_3,   // 1/3
	CSI_LCDSCALER_REV
}CSI_LCDSCALER_E;
typedef enum
{
	CSI_UVCOUT_CRY1CBY0=0,
	CSI_UVCOUT_CBY1CRY0,
	CSI_UVCOUT_Y1CRY0CB,
	CSI_UVCOUT_Y1CBY0CR
}CSI_UVCOUT_E;
typedef enum
{
	CSI_JFRA_END_INT=0,   //0
	CSI_LFRA_END_INT,     //1
	CSI_UVC_PACKET_INT,   //2
	CSI_SEN_SIZE_STAT_INT,//3
	CSI_SEN_SIZE_ERR,     //4
	CSI_JBUFF_ERR,        //5
    CSI_LBUFF_ERR,        //6
	CSI_MOTION_DETECT_INT,//7
	CSI_J1BUFF_ERR,       //8
	CSI_J1FRA_END_INT,    //9
	CSI_DVP_FRAME_DONE,   //10
	CSI_ASYC_FIRO_ERR,    //11
	CSI_DVP_MEM_EMPTY,    //12
	CSI_STAT_CP_DONE,     //13
	CSI_WP_DET_INT,       //14
	CSI_AE_HIDT_INT,      //15
	CSI_WDR_STAT_INT,     //16
	CSI_JE_CSI_ERR,       //17
	CSI_JE_LB_OVERFLOW,   //18
	CSI_LINE_SCALER_ERR,  //19
	CSI_INT_EN = 31
}CSI_INT_E;
typedef enum
{
	CSI_IRQ_JPG_FRAME_END=0,//0
	CSI_IRQ_LCD_FRAME_END,  //1
	CSI_IRQ_UVC_PACKE_END,  //2
	CSI_IRQ_SEN_STATE_INT,  //3
	CSI_IRQ_SEN_STATE_ERR,  //4
	CSI_IRQ_JBF_DMAWR_ERR,  //5
	CSI_IRQ_LBF_DMAWR_ERR,  //6
	CSI_IRQ_MDT_MOTION_DET, //7
	CSI_IRQ_J1BUFF_ERR,     //8
	CSI_IRQ_VXY_FRAME_EN ,  //9
	CSI_IRQ_ISP,            //10
	CSI_IRQ_MAX	
}CSI_IRQ_E;





/*******************************************************************************
* Function Name  : ax32xx_csiIRQHandler
* Description    : csi isr handler
* Input          :  none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_csiISRRegiser
* Description    : csi isr register
* Input          :  u8 type : irq type.CSI_IRQ_E
                       void (*isr)(void) : isr
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiISRRegiser(u8 type,void (*isr)(void));
/*******************************************************************************
* Function Name  : ax32xx_csiMJPEGFrameSet
* Description    : csi frame set for mjpeg
* Input          :  u32 mjpeg_y : y pixel address 
                       u32 mjpeg_uv : uv pixel address 
                       u32 lines     : lines
                       u32 width   : width
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiMJPEGFrameSet(u32 mjpeg_y,u32 mjpeg_uv,u32 lines,u32 width);
void ax32xx_csiMjpegFrameSwitch(u32 mjpeg_y,u32 mjpeg_uv);
/*******************************************************************************
* Function Name  : ax32xx_csi_only_FrameSet
* Description    : csi frame set 
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csi_only_FrameSet();
/*******************************************************************************
* Function Name  : ax32xx_csiYUVFrameSet
* Description    : csi yuv buffer set for uvc
* Input          :  u32 yaddr0 : y pixel address 0
                       u32 yaddr1 : y pixel address 1
                       u32 size     : frame size
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiYUVFrameSet(u32 yaddr0,u32 yaddr1,u32 size);
/*******************************************************************************
* Function Name  : ax32xx_csiIOConfig
* Description    : csi io config set
* Input          :  none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiIOConfig(void);
/*******************************************************************************
* Function Name  : ax32xx_csiInit
* Description    : csi initial 
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiInit(void);
/*******************************************************************************
* Function Name  : ax32xx_csiMclkSet
* Description    : csi mclk set
* Input          : u32 clk : clock
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiMclkSet(u32 clk);
void ax32xx_csi_set_mclk_free();

/*******************************************************************************
* Function Name  : ax32xx_csiSyncSet
* Description    : csi sync set
* Input          : u8 hsync_en : hsync
                      u8 vsync_en : vsync
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiSyncSet(u8 hsync_en,u8 vsync_en);
/*******************************************************************************
* Function Name  : ax32xx_csiPrioritySet
* Description    : csi priority set
* Input          : u8 type : type .CSI_POLARITY_E
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiPrioritySet(u8 plo);
/*******************************************************************************
* Function Name  : ax32xx_csiTypeSet
* Description    : csi type set
* Input          : u8 type : type .CSI_TYPE_E
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiTypeSet(u8 type);
/*******************************************************************************
* Function Name  : ax32xx_csiModeSet
* Description    : csi mode set
* Input          : u32 mode : mode flag
                      u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiModeSet(u32 mode,u8 en);
/*******************************************************************************
* Function Name  : ax32xx_pclk_fir_Set
* Description    : pclk filter set
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_pclk_fir_Set(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_pclk_analog_Set
* Description    : pclk filter set
* Input          : u8 en : step
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_pclk_analog_Set(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_pclk_inv_Set
* Description    : pclk inv set
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_pclk_inv_Set(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_csi_clk_tun_Set
* Description    : pclk tun set
* Input          : u8 tun : tun val
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csi_clk_tun_Set(u8 tun);
/*******************************************************************************
* Function Name  : ax32xx_csiSizeSet
* Description    : csi size set
* Input          : u16 width : width
                      u16 height: height
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiSizeSet(u16 width,u16 height);
/*******************************************************************************
* Function Name  : ax32xx_csiInputAddrSet
* Description    : csi input from sdram addr set
* Input          : u32 addr : input address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiInputAddrSet(u32 addr);
/*******************************************************************************
* Function Name  : ax32xx_csiTestModeSet
* Description    : csi test mode size set
* Input          : u16 width : width
                      u16 height: height
                      u16 hsync : hsync
                      u16 vsync : vsync
					  u16 test_typ :test_typ
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiTestModeSet(u16 width,u16 height,u16 hsync,u16 vsync,u16 test_typ);
/*******************************************************************************
* Function Name  : ax32xx_csiDvpClkDivSet
* Description    : csi dvp gennerate clk_div set
* Input          : u16 clk_div_mode : clk_div
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiDvpClkDivSet(u16 clk_div_mode);
/*******************************************************************************
* Function Name  : ax32xx_csiLCDFrameSet
* Description    : csi set output frame addr.if any of the r address is NULL,do not care it.
* Input          : u32 yaddr : y pixel buffer addr
                      u32 uvaddr: uv pixel buffer addr.
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiLCDFrameSet(u32 yaddr,u32 uvaddr);
/*******************************************************************************
* Function Name  : ax32xx_csiLCDDmaEnable
* Description    : csi output for lcd control enable
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiLCDDmaEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_csiLCDDmaKick
* Description    : csi output for lcd control enable(manual mode)
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiLCDDmaKick(void);
/*******************************************************************************
* Function Name  : ax32xx_csiLCDScaler
* Description    : csi scaler for lcd
* Input          : u16 src_w : source width
                   u16 src_h : source height
                   u16 tar_w : target width
                   u16 tar_h : target height
                   u16 sx    : crop start x position
                   u16 sy    : crop start y position
                   u16 ex    : crop end x position
                   u16 ey    : crop end y position
                   u16 stride: warning:stride >= width
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiLCDScaler(u16 src_w,u16 src_h,u16 tar_w,u16 tar_h,u16 sx,u16 sy,u16 ex,u16 ey,u16 stride,u32 * anti_lsawtooth);
/*******************************************************************************
* Function Name  : ax32xx_csiScaler
* Description    : csi scaler down set for in line scaler
* Input          : u16 src_w : source width
                      u16 src_h : source height
                      u16 tar_w : target width
                      u16 tar_h : target height
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiScaler(u16 src_w,u16 src_h,u16 tar_w,u16 tar_h,u16 sx,u16 sy,u16 ex,u16 ey);
/*******************************************************************************
* Function Name  : ax32xx_hvScaler
* Description    : 
* Input          : u16 src_w : source width
                      u16 src_h : source height
                      u16 tar_w : target width
                      u16 tar_h : target height
* Output         : none
* Return         : none
*******************************************************************************/
bool ax32xx_hvScaler(u32 src_w,  u32 src_h, u32 obj_w, u32 obj_h);
void ax32xx_csihv_tomjp_dma_cfg(u8 *Jy_buf, u8 *juv_buf, u16 line, u16 w);
/*******************************************************************************
* Function Name  : ax32xx_csiScalerCheck
* Description    : csi sclaer to lcd check
* Input          : none
* Output         : none
* Return         : 1 : scaler done
*******************************************************************************/
s32 ax32xx_csiScalerCheck(void);
/*******************************************************************************
* Function Name  : ax32xx_csiOutputSet
* Description    : csi output mode set
* Input          : u32 mode : mode.CSI_OUTPUT_E
                      u8 en      : 0-disable,1-enable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiOutputSet(u32 mode,u8 en);
/*******************************************************************************
* Function Name  : ax32xx_csiEnable
* Description    : csi enable set
* Input          : u8 en : 0-disable,1-enable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiEnable(u8 en);
void ax32xx_csihv_tomjpg_Switch(u8 *Jy_buf,u8 *juv_buf);
void ax32xx_csiIOConfigUinit();

#endif
