/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX MJPEG
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_mjpeg.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef  AX32XX_MJPEG_H
    #define  AX32XX_MJPEG_H

enum
{
	JPEG_Q_27 = 0xf,// 1.875
	JPEG_Q_28 = 0xe,// 1.75
	JPEG_Q_31 = 0xD,// 1.625
	JPEG_Q_33 = 0xC,// 1.5
	JPEG_Q_36 = 0xB,// 1.375
	JPEG_Q_40 = 0xA, // 1.25
	JPEG_Q_42 = 0x9,//1.125
	JPEG_Q_50 = 0x0,//0.00	
	JPEG_Q_62 = 0x6,//0.75
	JPEG_Q_75 = 0x4,//0.50
	JPEG_Q_81 = 0x3,//0.375 
	JPEG_Q_90 = 0x2,
	JPEG_Q_95 = 0x1,
	JPEG_Q_AUTO = 0xff,
};


typedef enum
{
	MJPEG_IRQ_OUTPAUSE=0,
	MJPEG_IRQ_INPAUSE,
	MJPEG_IRQ_OUTERR = 3,
	MJPEG_IRQ_OUTFULL = 7,	
	MJPEG_IRQ_OUTLOAD = 4,
	MJPEG_IRQ_ERROR,
	MJPEG_IRQ_HUFFERR,
	MJPEG_IRQ_FRAMEEND = 6,
	MJPEG_IRQ_TINFO2END = 11,
	MJPEG_IRQ_TINFO1END = 10,
	MJPEG_IRQ_TINFO0END = 9,
	
	MJPEG_IRQ_DECODE=16,
	MJPEG_IRQ_ENCODE,
	MJPEG_IRQ_MAX
}MJPEG_IRQ_E;

void ax32xx_mjpegAIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_mjpegIRQHandler
* Description    : jpeg irq handler
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegBIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeISRRegister
* Description    : jpeg encode isr register
* Input          : void (*isr)(int flag)
* Output         : none
* Return         : none
*******************************************************************************/
//void ax32xx_mjpegEncodeISRRegister(void (*isr)(int flag));
void ax32xx_mjpegEA_ncodeISRRegister(void (*isr)(int flag));
void ax32xx_mjpegEB_ncodeISRRegister(void (*isr)(int flag));
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeISRRegister
* Description    : jpeg decode isr register
* Input          : void (*isr)(int flag)
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeISRRegister(void (*isr)(int flag));
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeBufferSet
* Description    : jpeg encode set buffer
* Input          : u32 saddr : start address
*                  u32 eddr : end address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeBufferSet(u32 saddr,u32 eaddr);
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeEnable
* Description    : jpeg encode enable
* Input          : u8 ne : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeEnable(u8 en);
void ax32xx_mjpegEncode_wifi_Enable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeSizeSet
* Description    : jpeg encode size set
* Input          : u32 src_w : source width
*                  u32 src_h : source height
*                  u32 tar_w : target width
*                  u32 tar_h : target height
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeSizeSet(u32 src_w,u32 src_h,u32 tar_w,u32 tar_h);
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeQuilitySet
* Description    : jpeg encode quility set
* Input          : u8 qulity 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeQuilitySet(u8 qulity);
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeInfoSet
* Description    : jpeg encode info mode set
* Input          : u8 mode 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeInfoSet(u8 mode);
/*******************************************************************************
* Function Name  : ax32xx_mjpegEnccodeLoadAddrGet
* Description    : jpeg encode get load addr
* Input          : u8 mode 
* Output         : none
* Return         : none
*******************************************************************************/
u32 ax32xx_mjpegEncodeLoadAddrGet(void);
void ax32xx_mjpeg_inlinebuf_init(u8* ybuf, u8 *uvbuf);
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeInit
* Description    : jpeg encode initial
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeInit(bool manual, bool tp);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeScalerCal
* Description    : jpeg decode scaler caculate
* Input          : u32 src_w : source width
*                  u32 tar_w : source height
* Output         : none
* Return         : none
*******************************************************************************/
s32 ax32xx_mjpegDecodeScalerCal(u32 src_w,u32 tar_w);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeSetSize
* Description    : jpeg decode size set
* Input          : u32 src_w : source width
*                  u32 src_h : source height
*                  u32 tar_w : target width
*                  u32 tar_h : target height
*                  u32 stride: 0:disable stride function
*                             -1:don't change stride
*                              other:target stride,for dma0
* Output         : none
* Return         : none
*******************************************************************************/
s32 ax32xx_mjpegDecodeSetSize(u32 src_w,u32 src_h,u32 tar_w,u32 tar_h,u32 stride);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeOutputSet
* Description    : jpeg decode output buffer set
* Input          : u32 yaddr : yaddress
*                  u32 uvaddr: uv address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeOutputSet(u32 yaddr,u32 uvaddr);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeInputSet
* Description    : jpeg decode input set
* Input          : u32 saddr : start address
*                  u32 eaddr : end address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInputSet(u32 saddr,u32 eaddr);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeDriSet
* Description    : jpeg decode dri set
* Input          : u32 dri
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeDriSet(u32 dri);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeCompressSet
* Description    : jpeg decode compress set
* Input          : u32 table : compress value
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeCompressSet(u32 table);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeInitTable
* Description    : jpeg decode huffman table
* Input          : u32 tab : table address
*                  u32 len : reserved,do not care this value
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInitTable(u32 tab,u32 len);
/*******************************************************************************
* Function Name  : ax32xx_mjepgDecodeInit
* Description    : jpeg decode initial
* Input          : u8 yuvfmt : 0:yuv420,1:yuv422
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInit(u8 yuvfmt);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeDCTimeSet
* Description    : jpeg decode dc time over set
* Input          : u8 en,u32 src_w,u32 src_h
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeDCTimeSet(u8 en, u32 src_w,u32 src_h);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeEnable
* Description    : jpeg decode enable
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeEnable(u8 en);
void ax32xx_mjpegDecodeKick(void);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeStop
* Description    : jpeg decode stop
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeStop(void);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeQDTCfg
* Description    : jpeg decode config qdt table
* Input          : u8 *qdt[3] : table
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeQDTCfg(u8 *qdt[3]);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeStatus
* Description    : jpeg decode get status
* Input          : none
* Output         : none
* Return         : s32 : 1-busy,0-idle
*******************************************************************************/
s32 ax32xx_mjpegDecodeStatus(void);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeCheck
* Description    : jpeg decode error check
* Input          : none
* Output         : none
* Return         : s32 : 0-no error.other-error
*******************************************************************************/
s32 ax32xx_mjpegDecodeCheck(void);
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeODma1Cfg
* Description    : config output dma1(no block-scaler),call this function before init jpeg-decoder
*                  ax32xx_mjpegDecodeStop will disable DMA1
* Input          : u8 * y_addr
*                  u8 * uv_addr
*                  bool dma_en
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeODma1Cfg(u8 * y_addr,u8 * uv_addr,bool dma_en);
void ax32xx_mpegEncodePrint(void);
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeQadj
* Description    : jpeg encode quality ajust                      
* Input          : u32 jsize : last jpeg size
                      u32 min_size : limited min size
                      u32 max_size: limited max size
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeQadj(u32 jsize,u32 min_size,u32 max_size,uint8 flag);
u8 ax32xx_mjpegEA_ncodeISR(void);
void ax32xx_mjpB_stop(void);
void ax32xx_mjpB_reset(void);
void ax32xx_mjpB_dma_cfg(u32 s, u32 e);
void ax32xx_mjB_as_dcode(void);
void ax32xx_mjpB_Ctl_init(u8 mod, u8 quality, u32 src_w,  u32 src_h, u32 obj_w, u32 obj_h);
void ax32xx_mjpB_Linebuf_cfg(u8* ybuf, u8 *uvbuf);
void ax32xx_mjpB_dcdown_reg(void (*fp_fun)(void));
void ax32xx_mjpB_on(void);
void ax32xx_mjpB_on_2(void);
void ax32xx_mjpB_stop_2(void);
void ax32xx_mjpeg_manual_on(void);
void ax32xx_mjpeg_manual_stop(void);

INT32S ax32xx_mjpeg_encode_check(void);
INT32S ax32xx_mjpegB_encode_check(void);
bool jpeg1_decode_check(void);
#endif
