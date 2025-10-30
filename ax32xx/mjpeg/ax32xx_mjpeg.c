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
* File Name   : ax32xx_mjpeg.c
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
#include "../inc/ax32xx.h"

#define  R_MJPEG_OUT_SADDR     BACK_JPOBAS
#define  R_MJPEG_OUT_EADDR     BACK_JPOEND
#define  R_MJPEG_IN_SADDR      BACK_JPIBAS
#define  R_MJPEG_IN_EADDR      BACK_JPIEND
//#define  R_MJPEG_CTRL          
#define  R_MJPEG_CTRL1         BACK_JPCON1
#define  R_MJPEG_CTRL2         BACK_JPCON2
#define  R_MJPEG_INTEN         BACK_JPINTCON
#define  R_MJPEG_INTCLR        BACK_JPFLG_CLR 
#define  R_MJPEG_INTSTATUS     BACK_JPFLG
#define  R_MJPEG_SIZE          BACK_JPSIZE0
#define  R_MJPEG_SIZE1         BACK_JPSIZE1
#define  R_MJPEG_XSCALER       BACK_JPXSCALE
#define  R_MJPEG_YSCALER       BACK_JPYSCALE
#define  R_MJPEG_HEIGHT        BACK_JPHCNT
#define  R_MJPEG_BIT           BACK_JPUBIT
#define  R_MJPEG_OUTPTR        BACK_JPOPTR
#define  R_MJPEG_OUTLOAD       BACK_JPOLTA
#define  R_MJPEG_INPTR         BACK_JPIPTR
#define  R_MJPEG_DRI           BACK_JPDRI
#define  R_MJPEG_DADDR         BACK_JPBSHADDR
#define  R_MJPEG_LINES         BACK_JPLS
#define  R_MJPEG_DTATS         BACK_JPES
#define  R_MJPEG_TABLE         BACK_JPCOM
#define  R_MJPEG_DMA1YBASE     BACK_JPDCYBASE
#define  R_MJPEG_DMA1UVBASE    BACK_JPDCUVBASE
#define  R_MJPEG_STRIDE        BACK_JPDMAOUTX32
#define  R_MJPEG_DCTIMER       BACK_DCTIMER 
#define  R_MJPEG_DEBLOCK	   BACK_DEBLOCK

#define  R_MJPEG_CCOEF0        BACK_JPBSCCOEF0
#define  R_MJPEG_CCOEF1        BACK_JPBSCCOEF1
#define  R_MJPEG_CCOEF2        BACK_JPBSCCOEF2
#define  R_MJPEG_CCOEF3        BACK_JPBSCCOEF3
#define  R_MJPEG_CCOEF4        BACK_JPBSCCOEF4
#define  R_MJPEG_CCOEF5        BACK_JPBSCCOEF5
#define  R_MJPEG_CCOEF6        BACK_JPBSCCOEF6
#define  R_MJPEG_CCOEF7        BACK_JPBSCCOEF7
#define  R_MJPEG_CCOEF10       BACK_JPBSCCOEF10
#define  R_MJPEG_CCOEF11       BACK_JPBSCCOEF11
#define  R_MJPEG_CCOEF12       BACK_JPBSCCOEF12
#define  R_MJPEG_CCOEF13       BACK_JPBSCCOEF13
#define  R_MJPEG_CCOEF14       BACK_JPBSCCOEF14
#define  R_MJPEG_CCOEF15       BACK_JPBSCCOEF15
#define  R_MJPEG_CCOEF16       BACK_JPBSCCOEF16
#define  R_MJPEG_CCOEF17       BACK_JPBSCCOEF17
#define  R_MJPEG_LINE_CCOEF0   BACK_LINE_JPBSCCOEF0
#define  R_MJPEG_LINE_CCOEF1   BACK_LINE_JPBSCCOEF1
#define  R_MJPEG_LINE_CCOEF2   BACK_LINE_JPBSCCOEF2
#define  R_MJPEG_LINE_CCOEF3   BACK_LINE_JPBSCCOEF3
#define  R_MJPEG_LINE_CCOEF4   BACK_LINE_JPBSCCOEF4
#define  R_MJPEG_LINE_CCOEF5   BACK_LINE_JPBSCCOEF5
#define  R_MJPEG_LINE_CCOEF6   BACK_LINE_JPBSCCOEF6
#define  R_MJPEG_LINE_CCOEF7   BACK_LINE_JPBSCCOEF7
#define  R_MJPEG_UVCCOEF0  	   BACK_UVJPBSCCOEF0
#define  R_MJPEG_UVCCOEF1  	   BACK_UVJPBSCCOEF1
#define  R_MJPEG_UVCCOEF2  	   BACK_UVJPBSCCOEF2
#define  R_MJPEG_UVCCOEF3  	   BACK_UVJPBSCCOEF3
#define  R_MJPEG_UVCCOEF4  	   BACK_UVJPBSCCOEF4
#define  R_MJPEG_UVCCOEF5  	   BACK_UVJPBSCCOEF5
#define  R_MJPEG_UVCCOEF6  	   BACK_UVJPBSCCOEF6
#define  R_MJPEG_UVCCOEF7  	   BACK_UVJPBSCCOEF7
#define  R_MJPEG_UVCCOEF10     BACK_UVJPBSCCOEF10
#define  R_MJPEG_UVCCOEF11     BACK_UVJPBSCCOEF11
#define  R_MJPEG_UVCCOEF12     BACK_UVJPBSCCOEF12
#define  R_MJPEG_UVCCOEF13     BACK_UVJPBSCCOEF13
#define  R_MJPEG_UVCCOEF14     BACK_UVJPBSCCOEF14
#define  R_MJPEG_UVCCOEF15     BACK_UVJPBSCCOEF15
#define  R_MJPEG_UVCCOEF16     BACK_UVJPBSCCOEF16
#define  R_MJPEG_UVCCOEF17     BACK_UVJPBSCCOEF17

#define  R_MJPEGA_OUT_SADDR     PRE_JPOBAS
#define  R_MJPEGA_OUT_EADDR     PRE_JPOEND
#define  R_MJPEGA_IN_SADDR      PRE_JPIBAS
#define  R_MJPEGA_IN_EADDR      PRE_JPIEND
//#define  R_MJPEG_CTRL          
#define  R_MJPEGA_CTRL1         PRE_JPCON1
#define  R_MJPEGA_CTRL2         PRE_JPCON2
#define  R_MJPEGA_INTEN         PRE_JPINTCON
#define  R_MJPEGA_INTCLR        PRE_JPFLG_CLR 
#define  R_MJPEGA_INTSTATUS     PRE_JPFLG
#define  R_MJPEGA_SIZE          PRE_JPSIZE0
#define  R_MJPEGA_SIZE1         PRE_JPSIZE1
#define  R_MJPEGA_XSCALER       PRE_JPXSCALE
#define  R_MJPEGA_YSCALER       PRE_JPYSCALE
#define  R_MJPEGA_HEIGHT        PRE_JPHCNT
#define  R_MJPEGA_BIT           PRE_JPUBIT
#define  R_MJPEGA_OUTPTR        PRE_JPOPTR
#define  R_MJPEGA_OUTLOAD       PRE_JPOLTA
#define  R_MJPEGA_INPTR         PRE_JPIPTR
#define  R_MJPEGA_DRI           PRE_JPDRI
#define  R_MJPEGA_DADDR         PRE_JPBSHADDR
#define  R_MJPEGA_LINES         PRE_JPLS
#define  R_MJPEGA_DTATS         PRE_JPES
#define  R_MJPEGA_TABLE         PRE_JPCOM
#define  R_MJPEGA_DMA1YBASE     PRE_JPDCYBASE
#define  R_MJPEGA_DMA1UVBASE    PRE_JPDCUVBASE
#define  R_MJPEGA_STRIDE        PRE_JPDMAOUTX32

#define  R_MJPEGA_CCOEF0        PRE_JPBSCCOEF0
#define  R_MJPEGA_CCOEF1        PRE_JPBSCCOEF1
#define  R_MJPEGA_CCOEF2        PRE_JPBSCCOEF2
#define  R_MJPEGA_CCOEF3        PRE_JPBSCCOEF3
#define  R_MJPEGA_CCOEF4        PRE_JPBSCCOEF4
#define  R_MJPEGA_CCOEF5        PRE_JPBSCCOEF5
#define  R_MJPEGA_CCOEF6        PRE_JPBSCCOEF6
#define  R_MJPEGA_CCOEF7        PRE_JPBSCCOEF7
#define  R_MJPEGA_CCOEF10       PRE_JPBSCCOEF10
#define  R_MJPEGA_CCOEF11       PRE_JPBSCCOEF11
#define  R_MJPEGA_CCOEF12       PRE_JPBSCCOEF12
#define  R_MJPEGA_CCOEF13       PRE_JPBSCCOEF13
#define  R_MJPEGA_CCOEF14       PRE_JPBSCCOEF14
#define  R_MJPEGA_CCOEF15       PRE_JPBSCCOEF15
#define  R_MJPEGA_CCOEF16       PRE_JPBSCCOEF16
#define  R_MJPEGA_CCOEF17       PRE_JPBSCCOEF17

#define  R_MJPEGA_LINE_CCOEF0   PRE_LINE_JPBSCCOEF0
#define  R_MJPEGA_LINE_CCOEF1   PRE_LINE_JPBSCCOEF1
#define  R_MJPEGA_LINE_CCOEF2   PRE_LINE_JPBSCCOEF2
#define  R_MJPEGA_LINE_CCOEF3   PRE_LINE_JPBSCCOEF3
#define  R_MJPEGA_LINE_CCOEF4   PRE_LINE_JPBSCCOEF4
#define  R_MJPEGA_LINE_CCOEF5   PRE_LINE_JPBSCCOEF5
#define  R_MJPEGA_LINE_CCOEF6   PRE_LINE_JPBSCCOEF6
#define  R_MJPEGA_LINE_CCOEF7   PRE_LINE_JPBSCCOEF7
#define  R_MJPEGA_UVCCOEF0  	PRE_UVJPBSCCOEF0
#define  R_MJPEGA_UVCCOEF1  	PRE_UVJPBSCCOEF1
#define  R_MJPEGA_UVCCOEF2  	PRE_UVJPBSCCOEF2
#define  R_MJPEGA_UVCCOEF3  	PRE_UVJPBSCCOEF3
#define  R_MJPEGA_UVCCOEF4  	PRE_UVJPBSCCOEF4
#define  R_MJPEGA_UVCCOEF5  	PRE_UVJPBSCCOEF5
#define  R_MJPEGA_UVCCOEF6  	PRE_UVJPBSCCOEF6
#define  R_MJPEGA_UVCCOEF7  	PRE_UVJPBSCCOEF7
#define  R_MJPEGA_UVCCOEF10     PRE_UVJPBSCCOEF10
#define  R_MJPEGA_UVCCOEF11     PRE_UVJPBSCCOEF11
#define  R_MJPEGA_UVCCOEF12     PRE_UVJPBSCCOEF12
#define  R_MJPEGA_UVCCOEF13     PRE_UVJPBSCCOEF13
#define  R_MJPEGA_UVCCOEF14     PRE_UVJPBSCCOEF14
#define  R_MJPEGA_UVCCOEF15     PRE_UVJPBSCCOEF15
#define  R_MJPEGA_UVCCOEF16     PRE_UVJPBSCCOEF16
#define  R_MJPEGA_UVCCOEF17     PRE_UVJPBSCCOEF17

#define  BIC_TAB00				0x40000000 //0
#define  BIC_TAB01				0x3ffa02ff //1
#define  BIC_TAB02				0x39f604ff //2
#define  BIC_TAB03				0x32f504ff //3
#define  BIC_TAB04				0x28f504ff //4
#define  BIC_TAB05				0x1df703ff //5
#define  BIC_TAB06				0x12fa0200 //6
#define  BIC_TAB07				0x08fd0100 //7
#define  BIC_TAB10				0x00000000 //10
#define  BIC_TAB11				0x0001fd08 //11
#define  BIC_TAB12				0x0002fa12 //12
#define  BIC_TAB13				0xff03f71d //13
#define  BIC_TAB14				0xff04f528 //14
#define  BIC_TAB15				0xff04f532 //15
#define  BIC_TAB16				0xff04f639 //16
#define  BIC_TAB17				0xff02fa3f //17

#define  BIC_UVTAB00			0x40000000//0
#define  BIC_UVTAB01			0x3ffa02ff //1
#define  BIC_UVTAB02			0x39f604ff //2
#define  BIC_UVTAB03			0x32f504ff //3
#define  BIC_UVTAB04			0x28f504ff //4
#define  BIC_UVTAB05			0x1df703ff //5
#define  BIC_UVTAB06			0x12fa0200 //6
#define  BIC_UVTAB07			0x08fd0100 //7
#define  BIC_UVTAB10			0x00000000 //10
#define  BIC_UVTAB11			0x0001fd08 //11
#define  BIC_UVTAB12			0x0002fa12 //12
#define  BIC_UVTAB13			0xff03f71d //13
#define  BIC_UVTAB14			0xff04f528 //14
#define  BIC_UVTAB15			0xff04f532 //15
#define  BIC_UVTAB16			0xff04f639 //16
#define  BIC_UVTAB17			0xff02fa3f //17

#define  BIC_LINETAB00			0x00004000 //0
#define  BIC_LINETAB01			0x00063dfd //1
#define  BIC_LINETAB02			0xfe0f37fc //2
#define  BIC_LINETAB03			0xfd192ffb //3
#define  BIC_LINETAB04			0xfc2424fc //4
#define  BIC_LINETAB05			0xfb2f19fd //5
#define  BIC_LINETAB06			0xfc370ffe //6
#define  BIC_LINETAB07			0xfd3d0600 //7
/*
#define  CCOEF0     0x00017F00
#define  CCOEF1   	0xFF0E7CF7
#define  CCOEF2   	0xFC2170F3
#define  CCOEF3   	0xF8365FF3
#define  CCOEF4   	0xF54B4BF5
#define  CCOEF5   	0xF35F36F8
#define  CCOEF6   	0xF37021FC
#define  CCOEF7   	0xF77C0EFF
 */
/*
#define  CCTAB0	   170012706UL 
#define  CCTAB1	   220474911UL 
#define  CCTAB2	   270937372UL 
#define  CCTAB3	   321399833UL 
#define  CCTAB4	   355084821UL 
#define  CCTAB5	   422324243UL 
#define  CCTAB6	   472720912UL 
#define  CCTAB7	   523117581UL
*/
/*
#define CCTAB0	   186789922UL
#define CCTAB1	   220475167UL
#define CCTAB2	   270937372UL
#define CCTAB3	   321399577UL
#define CCTAB4	   371861782UL
#define CCTAB5	   422258707UL
#define CCTAB6	   472720912UL
#define CCTAB7	   523117837UL
 */
/*********for decode deblock cofig****/
const int alpha_table_luma[16] =   {0, 3, 4, 5, 6, 7 ,8, 9,10,11,12,13,14,15,16,17};//列出一些有可能的配置
const int belta_table_luma[16] =   {0, 2, 3, 4, 5, 6 ,7, 8, 8, 9, 9,10,11,12,13,16};
const int c_table_luma[16] =       {0, 1, 2, 2, 3, 3 ,3, 4, 4, 4, 4, 5, 5, 5, 5, 8};
const int alpha_table_chroma[16] = {0, 3, 4, 5, 6, 7 ,8, 9,10,12,16,18,20,25,30,40};
const int belta_table_chroma[16] = {0, 2, 3, 4, 5, 6 ,7, 8, 9,10,11,12,13,14,15,16};
const int c_table_chroma[16] =     {0, 1, 2, 2, 3, 3 ,3, 4, 4, 4, 4, 5, 5, 5, 5, 8};
#define INDEX_LUMA 	3 //select cofigure para
#define INDEX_CHROMA 	3
u32 block_scaler_tab[3][8];//0 :all lcd   1:half lcd  2:small window
#define  debgreg(x)				uart_Printf(#x"=%X\n",x)
void ax32xx_mpegPrint(void)
{
    debgreg(BACK_JPCON1);
    debgreg(BACK_JPCON2);
    debgreg(BACK_JPCON3);
    debgreg(BACK_JPSIZE0);
    debgreg(BACK_JPOBAS);
    debgreg(BACK_JPOYADR);
    debgreg(BACK_JPOEND);
    debgreg(BACK_JPOUVADR);
    debgreg(BACK_JPOPTR);
    debgreg(BACK_JPOLTA);
    debgreg(BACK_JPIBAS);
    debgreg(BACK_JPIYADR);
    debgreg(BACK_JPIEND);
    debgreg(BACK_JPIUVADR);
    debgreg(BACK_JPIPTR);
    debgreg(BACK_JPUBIT);
    debgreg(BACK_JPHCNT);
    debgreg(BACK_JPDRI);
    debgreg(BACK_JPBSHADDR);
    debgreg(BACK_JPINTCON);
    debgreg(BACK_JPFLG);
    debgreg(BACK_JPLS);
    debgreg(BACK_JPES);
    debgreg(BACK_JPCOM);
    debgreg(BACK_JPTINF);
    debgreg(BACK_JPT0SIZE);
    debgreg(BACK_JPT1SIZE);
    debgreg(BACK_JPT2SIZE);
    debgreg(BACK_JPT0POS);
    debgreg(BACK_JPT1POS);
    debgreg(BACK_JPT2POS);
    debgreg(BACK_JPT0BASE);
    debgreg(BACK_JPT1BASE);
    debgreg(BACK_JPT2BASE);
    debgreg(BACK_JPSIZE1);
    debgreg(BACK_JPXSCALE);
    debgreg(BACK_JPYSCALE);
    debgreg(BACK_JPDCXCROP);
    debgreg(BACK_JPDCYCROP);
    debgreg(BACK_JPFLG_CLR);
    debgreg(BACK_JPPAUSECNT);
    debgreg(BACK_JPDCYBASE);
    debgreg(BACK_JPDCUVBASE);
    debgreg(BACK_JPDMAOUTX32);
	debgreg(BACK_JPELINECNT);
	debgreg(BACK_DCTIMER);
	debgreg(BACK_DEBLOCK);
}
void ax32xx_mjpB_reset(void)
{
	//static u8 jreset;
	//if(jreset) return ;
	//jreset = true;
	//PCON0 |= BIT(31);			//JPEG  clk enable
	//ax32xx_sysCpuNopDelay(3);
	PCON0 &= ~BIT(31);			//JPEG  clk enable
	//debg("src_w/h:%d,%d  obj_w/h:%d,%d\n",src_w,src_h,obj_w,obj_h);
	(RSTCON) &= ~BIT(2);
	ax32xx_sysCpuNopDelay(30);
	(RSTCON) |= BIT(2);
	ax32xx_sysCpuNopDelay(30);
}

static volatile u32 jpegErrStatus = 0,jpegFlag=0;
extern void jpegA_table_init(bool tp);

static void (*ax32xxMJPEGA_EncodeISR)(int flag);
static void (*ax32xxMJPEGB_EncodeISR)(int flag);
static void (*ax32xxMJPEGDecodeISR)(int flag);

void ax32xx_mjpegAIRQHandler(void)
{
	
	
	volatile u32 jflg = R_MJPEGA_INTSTATUS;
	//debg("jAflg:%x\n",jflg);	
	
	if(jflg & (BIT(3) /*| BIT(7)*/)){
		debg("jA: <Er:%x>\n",jflg);	
		//R_MJPEGA_INTCLR |= jflg;
	}
	if(jflg & BIT(6)){
		//debg("*");	
	}
	if(ax32xxMJPEGA_EncodeISR)
		ax32xxMJPEGA_EncodeISR(jflg);	//hal_mjpegCallback
		
	R_MJPEGA_INTCLR = jflg;
}
INT32S ax32xx_mjpeg_encode_check(void)
{
	u32 timeout = 0;
	while(1){	
		timeout++;
		if(timeout>=0x0fffff){
			debg("frame encode fail:%x\n",PRE_JPFLG);
			break;
		}
		volatile u32 jflg = PRE_JPFLG;
		//part farme down isr
		if(jflg & (BIT(0) | BIT(6))){
		
			//pause down
			if(jflg & BIT(0)){//ODMAPAUSE
				if(!(jflg & BIT(6))){
					debg("mallo j frame size err\n");	
					PRE_JPFLG_CLR |=  BIT(0)|BIT(6);
					return -1;
				}
			}
			
			//frame down
			if(jflg & BIT(6)){//FRAMDONE
				//debgstr("encode A frame done\n");	
				PRE_JPFLG_CLR |=  BIT(0)|BIT(6);
				return 0;
			}	
		}
		
		//addr reload(last reoload addr)
		if(jflg & BIT(4)){//JEOLDADRD

			PRE_JPFLG_CLR |= BIT(4);//JEOLDADRDCLR(mast before upload addr!!!!)
			//__FP__(fp_mjp_adrload_hal);
			
		}
		/**/
		if(jflg & (BIT(9) | BIT(10) | BIT(11))){//TIMEINF_LOAD_DOWN
			PRE_JPFLG_CLR |= BIT(9) | BIT(10) | BIT(11);
		}
		
		//err isr
		if(jflg & (BIT(3)|BIT(7))){
			if(jflg & BIT(3)){//OBDMAERROR
				PRE_JPFLG_CLR |= BIT(3);//ODMAERRORCLR
				debg("A-OBDMAERROR\n");
			}
			if(jflg & BIT(7)){//JEOFULL
				PRE_JPFLG_CLR |= BIT(7);//JEOFULL
				debg("A-JEOFULL\n");
				//debg("b,s:%x,%x\n",BACK_JPOBAS,BACK_JPOEND & ~(0xf << 28));
			}
			return -1;
			
		}
		//flg
		if(jflg & ~(BIT(0)| BIT(6) |BIT(4) | BIT(3) |  BIT(7)| BIT(9) | BIT(10) | BIT(11) | BIT(17))){
			//1  JPIDAM_RESUME 
			//2  YUV422_DMADONECLR  
			//5  JDRSTCLR   
			//8  JDHUFCLR 
			PRE_JPFLG_CLR |= jflg;
		}
	}
	return 0;
}
INT32S ax32xx_mjpegB_encode_check(void)
{
	u32 timeout = 0;
	while(1){	
		timeout++;
		if(timeout>=0x0fffff){
			debg("frame encode fail:%x\n",PRE_JPFLG);
			break;
		}
		volatile u32 jflg = BACK_JPFLG;
		//part farme down isr
		if(jflg & (BIT(0) | BIT(6))){
		
			//pause down
			if(jflg & BIT(0)){//ODMAPAUSE
				if(!(jflg & BIT(6))){
					debg("mallo j frame size err\n");	
					BACK_JPFLG_CLR |=  BIT(0)|BIT(6);
					ax32xx_mjpB_stop_2();
					return -1;
				}
			}
			
			//frame down
			if(jflg & BIT(6)){//FRAMDONE
				//debg("encode  frame done\n");	
				//ax32xx_mpegPrint();
				BACK_JPFLG_CLR |=  BIT(0)|BIT(6);
				ax32xx_mjpB_stop_2();
				return 0;
			}	
		}
		
		//addr reload(last reoload addr)
		if(jflg & BIT(4)){//JEOLDADRD

			BACK_JPFLG_CLR |= BIT(4);//JEOLDADRDCLR(mast before upload addr!!!!)
			//__FP__(fp_mjp_adrload_hal);
			
		}
		/**/
		if(jflg & (BIT(9) | BIT(10) | BIT(11))){//TIMEINF_LOAD_DOWN
			BACK_JPFLG_CLR |= BIT(9) | BIT(10) | BIT(11);
		}
		
		//err isr
		if(jflg & (BIT(3)|BIT(7))){
			if(jflg & BIT(3)){//OBDMAERROR
				BACK_JPFLG_CLR |= BIT(3);//ODMAERRORCLR
				debg("A-OBDMAERROR\n");
			}
			if(jflg & BIT(7)){//JEOFULL
				BACK_JPFLG_CLR |= BIT(7);//JEOFULL
				debg("A-JEOFULL\n");
				//debg("b,s:%x,%x\n",BACK_JPOBAS,BACK_JPOEND & ~(0xf << 28));
			}
			ax32xx_mjpB_stop_2();
			return -1;
			
		}
		//flg
		if(jflg & ~(BIT(0)| BIT(6) |BIT(4) | BIT(3) |  BIT(7)| BIT(9) | BIT(10) | BIT(11) | BIT(17))){
			//1  JPIDAM_RESUME 
			//2  YUV422_DMADONECLR  
			//5  JDRSTCLR   
			//8  JDHUFCLR 
			BACK_JPFLG_CLR |= jflg;
		}
	}
	ax32xx_mjpB_stop_2();
	return -1;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegIRQHandler
* Description    : jpeg irq handler
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void (*fp_jpegdc_ldone_hal)(void);
bool ax32xx_mjpB_employ(void)
{
	return (bool)fp_jpegdc_ldone_hal;
}
void ax32xx_mjpB_dcdown_reg(void (*fp_fun)(void))
{
	fp_jpegdc_ldone_hal = fp_fun;
}
//extern void (*fp_jpeg0_decode_done_hal)(void);
//extern void (*fp_jpeg0_decode_ch1_done_hal)(void);
//extern void (*fp_jpeg0_decode_input_pause_hal)(void);
//extern volatile u8 ecdwnflg;
//extern volatile u8 dcdwnflg;
//extern volatile u8 dcerflg;

void ax32xx_mjB_as_dcode(void)
{
	jpegFlag = 0;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegIRQHandler
* Description    : jpeg irq handler
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
extern void mjpBcodec_sacle_dwon(u32 flg);
void ax32xx_mjpegBIRQHandler(void)
{
	volatile u32 jflg = R_MJPEG_INTSTATUS;
	//if(jflg & ~(BIT(0)|BIT(10) | BIT(9) | BIT(8)| BIT(4) | BIT(7) | BIT(17))){
		//debg("jflg:%x\n",jflg);	
	//	R_MJPEG_INTCLR |= jflg;
	//}
	
	
//--------encode status--------------------	
	if(jpegFlag==0)
	{
		//debg("-",jflg);
		//debg("-");
		if(ax32xxMJPEGB_EncodeISR)//hal_mjpB_isr
			ax32xxMJPEGB_EncodeISR(jflg);
	}
	else
	{
		//debg("+");
		/*while(1)
		{
			
			if((jflg&(1<<2))&&(jflg&(1<<6)))
				break;
			if(jflg & (BIT(8) | BIT(5)|BIT(12)|BIT(13)))
				break;
			debg("**********[R_MJPEG_INTSTATUS:%x\n]***********",jflg);
			jflg = R_MJPEG_INTSTATUS;
		}*/
		ax32xx_mjpegDecodeStop();
		//mjpBcodec_sacle_dwon(jflg);
        //huffman err
        if(jflg & (1<<8))
        {
            jpegErrStatus = 1<<8;
			//usb_usensor_frame_free();
            debg("mjpBcodec: <huf err>\n");
        }
        //dri err
        if(jflg & (1<<5))
        {
            jpegErrStatus = 1<<5;
            debg("mjpBcodec: <dri err>\n");
        }
		if(jflg & (1<<12))
        {
            jpegErrStatus = 1<<12;
            debg("mjpBcodec: <FFD9  err>\n");
        }
		if(jflg & (1<<13))
        {
            jpegErrStatus = 1<<13;
            debg("mjpBcodec: <dc time over  err>\n");
        }
		//debg("mjpBcodec: end\n");
		/*
		if(jflg & ~(BIT(8)|BIT(5)|BIT(6)|BIT(2)|BIT(16))){
			debg("mjpBdc: <Err:%x>\n",jflg & ~(BIT(8)|BIT(5)|BIT(6)|BIT(2)|BIT(16)));	
		}
        //huffman err
        if(jflg & (1<<8))
        {
            jpegErrStatus = 1<<6;
			//usb_usensor_frame_free();
            debg("mjpBcodec: <huf err>\n");
        }
        //dri err
        if(jflg & (1<<5))
        {
            jpegErrStatus = 1<<5;
            debg("mjpBcodec: <dri err>\n");
        }
		
        //decode frame done,DMA0 lcd sacle
        if(jflg & (1<<6))
        {
		
        }
        //decode yuv422 done,DMA1 backmjpec
        if(jflg & (1<<2))
        {           
		}
		//decode end hal
		 * */
		if(ax32xxMJPEGDecodeISR) //deocode_done()
			ax32xxMJPEGDecodeISR(jflg);   
		
		
	}
	R_MJPEG_INTCLR = jflg;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeISRRegister
* Description    : jpeg encode isr register
* Input          : void (*isr)(int flag)
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEA_ncodeISRRegister(void (*isr)(int flag))
{
	ax32xxMJPEGA_EncodeISR = isr;
}
void ax32xx_mjpegEB_ncodeISRRegister(void (*isr)(int flag))
{
	ax32xxMJPEGB_EncodeISR = isr;
}
u8 ax32xx_mjpegEA_ncodeISR(void)
{
	if(ax32xxMJPEGA_EncodeISR)
		return 1;
	else 
		return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeISRRegister
* Description    : jpeg decode isr register
* Input          : void (*isr)(int flag)
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeISRRegister(void (*isr)(int flag))
{
	ax32xxMJPEGDecodeISR = isr;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeBufferSet
* Description    : jpeg encode set buffer
* Input          : u32 saddr : start address
*                  u32 eddr : end address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeBufferSet(u32 saddr,u32 eaddr)
{
	ax32xx_sysDcacheFlush(saddr,eaddr-saddr);
	
	R_MJPEGA_OUT_SADDR = saddr;
	R_MJPEGA_OUT_EADDR = (eaddr|(3<<28));
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeEnable
* Description    : jpeg encode enable
* Input          : u8 ne : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeEnable(u8 en)
{
	if(en)
	{
		//debg("R");
		CSI_INTR = BIT(18)|BIT(17);
		CSI_INTEN |= BIT(18)|BIT(17);
		ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN,1); // enable csi output for mjpeg
		//ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN,1); // enable  csi output for wifi
		R_MJPEGA_CTRL1 |= BIT(8); // mjpeg run
		
		ax32xx_intEnable(IRQ_JPGA,1); // enable jpegirq
	}
	else
	{
		//CSI_DMA_MODE &= ~BIT(19);//J1DMAOFF
		
		R_MJPEGA_CTRL1 |= BIT(9); // mjpeg stop
		R_MJPEGA_INTEN &=~(1<<11);//MJP_INIT_EN
		R_MJPEGA_CTRL1 |= BIT(9); // mjpeg stop
		ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN,0); // disable csi output for mjpeg
		ax32xx_csiOutputSet(CSI_OUTPUT_WIFIEN,0); // disable csi output for wifi
		R_MJPEGA_INTCLR |= 0xffffffff; // clear all int flag
		CSI_INTEN &= ~(BIT(18)|BIT(17));
		ax32xx_intEnable(IRQ_JPGA,0); // diable jpeg irq
	//	debg("D\n");
		//hal_mjpegQueuePrintf();
	}
}
void ax32xx_mjpegEncode_wifi_Enable(u8 en)
{
	if(en)
	{
		//debg("R");
		CSI_INTR = BIT(18)|BIT(17);
		CSI_INTEN |= BIT(18)|BIT(17);
		//ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN,1); // enable csi output for mjpeg
		ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN,1); // enable  csi output for wifi
		R_MJPEGA_CTRL1 |= BIT(8); // mjpeg run
		
		ax32xx_intEnable(IRQ_JPGA,1); // enable jpegirq
	}
	else
	{
	//	CSI_DMA_MODE &= ~BIT(20);//W1DMAOFF
		
		R_MJPEGA_CTRL1 |= BIT(9); // mjpeg stop
		R_MJPEGA_INTEN &=~(1<<11);//MJP_INIT_EN
		R_MJPEGA_CTRL1 |= BIT(9); // mjpeg stop
		ax32xx_csiOutputSet(CSI_OUTPUT_MJPGEN,0); // disable csi output for mjpeg
		ax32xx_csiOutputSet(CSI_OUTPUT_WIFIEN,0); // disable csi output for wifi
		R_MJPEGA_INTCLR |= 0xffffffff; // clear all int flag
		CSI_INTEN &= ~(BIT(18)|BIT(17));
		ax32xx_intEnable(IRQ_JPGA,0); // diable jpeg irq
	//	debg("D\n");
		//hal_mjpegQueuePrintf();
	}
}
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
void ax32xx_mjpegEncodeSizeSet(u32 src_w,u32 src_h,u32 tar_w,u32 tar_h)
{
	if(src_w>tar_w)
		src_w = tar_w;
	if(src_h>tar_h)
		src_h = tar_h;
	R_MJPEGA_SIZE1 = (src_w<<16)|src_h;
	R_MJPEGA_SIZE  = (tar_w<<16)|tar_h;
	
	R_MJPEGA_XSCALER = (0 << 18)  //div n(ecdoe not use)
				     |(src_w/tar_w << 16)//intergser
				     |((u16)(src_w*65536 /tar_w + (bool)(src_w*65536 %tar_w))  << 0);
	
	R_MJPEGA_YSCALER = (src_h/tar_h << 16)//intergser
				      |((u16)(src_h*65536 /tar_h + (bool)(src_h*65536 %tar_h))  << 0);	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeQuilitySet
* Description    : jpeg encode quility set
* Input          : u8 qulity 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeQuilitySet(u8 qulity)
{
	R_MJPEGA_CTRL1 &= ~(0x0f<<23);
	R_MJPEGA_CTRL1 |= (qulity & 0x0f)<<23;
}
volatile u32 javgsize;
u32 cal_jA_1s_size(void)
{
	u32 tp = javgsize;
	javgsize = 0;
	return tp;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeQadj
* Description    : jpeg encode quality ajust                      
* Input          : u32 jsize : last jpeg size
                      u32 min_size : limited min size
                      u32 max_size: limited max size
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeQadj(u32 jsize,u32 min_size,u32 max_size,uint8 flag)
{
	static s8 tq;
	if(jsize==0)
		return ;
	javgsize += jsize;
	s8 q = (R_MJPEGA_CTRL1 >> 23) & 0x0f;
	if(q == 0){ //q = 0 equ q = 8 
		q = 8;	
	}
	if(jsize > max_size){
		q++;	
	}
	if(jsize < min_size){
		q--;	
	}	
	//q -> (JPEG_Q_27 ~ JPEG_Q_81)

	if(flag){
		if( (q < 0x6) && (q > 0) ) // JPEG_Q_62
		{ 
			q = 0x6;	
		}
	}else{
		if(q < 0x03){ //JPEG_Q_81
			q = 0x03;	
		}

	}

	if(q > 0x0f){ //JPEG_Q_27
	q = 0x0f;	
	}
	if(tq != q){
		tq = q; 
		//debg("Q,S:%x,%dK\n",q, jsize/1024);
	#if 0
		u8 sbuf[]={'Q','=','0','0','\n', '\0'};
		sbuf[2] = q/10 + '0';
		sbuf[3] = q%10 + '0';
		LogPrint(sbuf,0);
	#endif
	}
	ax32xx_mjpegEncodeQuilitySet(q);
	/*
	if(jsize==0)
		return ;
//	#define JSIZE_LIMT_MAX		80 * 1024
//	#define JSIZE_LIMT_MIN		50 * 1024
	const u8 Q[]={
		JPEG_Q_27,//0
		JPEG_Q_28,//1
		JPEG_Q_31,//2
		JPEG_Q_36,//3
		JPEG_Q_40,//4
		JPEG_Q_42,//5
		JPEG_Q_50,//6
		JPEG_Q_62,//7
		JPEG_Q_75,//8
		JPEG_Q_81,//9
	};
	static s8 iq;
	u32 i;

	u8 qc = (R_MJPEGA_CTRL1 >> 23) & 0x0f;
	
	iq = 0;
	for(i = 0; i < sizeof(Q); i++)
	{
		if(qc == Q[i]){
			iq = i;	
			break;
		}
	}
	if(jsize > max_size){
		if(iq > 0){
			iq--;
		}
	}
	if(jsize < min_size){
		if(iq < (sizeof(Q)-1)){
			iq++;
		}		
	}
	
	ax32xx_mjpegEncodeQuilitySet(Q[iq]);*/ 	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeInfoSet
* Description    : jpeg encode info mode set
* Input          : u8 mode 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeInfoSet(u8 mode)
{
	R_MJPEGA_CTRL1 &= ~(3<<19);
	R_MJPEGA_CTRL1 |= (mode & 3)<<19;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeLoadAddrGet
* Description    : jpeg encode get load addr
* Input          : u8 mode 
* Output         : none
* Return         : none
*******************************************************************************/
u32 ax32xx_mjpegEncodeLoadAddrGet(void)
{
	//debg("R_MJPEGA_OUTLOAD:%x\n",R_MJPEGA_OUTLOAD);
	return R_MJPEGA_OUTLOAD;
}

void ax32xx_mjpeg_inlinebuf_init(u8* ybuf, u8 *uvbuf)
{
	/*u32 i;
	
	for(i = 0 ; i < 640*360; i++){
		ybuf[i] = 0X80;
	}
	for(i = 0 ; i < 640*360/2; i++){
		uvbuf[i] = 0x00;
	}
	*/
	//PRE_JPELINECNT = 0;
	PRE_JPIBAS = (u32)ybuf;
	PRE_JPIEND = (u32)uvbuf;
	//debg("JIN %x,%x\n",PRE_JPIBAS,PRE_JPIEND);
	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegEncodeInit
* Description    : jpeg encode initial
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegEncodeInit(bool manual, bool tp)
{
	ax32xx_sysReset(SYS_RESET_JPGA,1); // reset mjpeg modlule
	ax32xx_sysCpuNopDelay(20);
	ax32xx_sysReset(SYS_RESET_JPGA,0); // normal set mjpeg
    ax32xx_sysClkSet(26,1);  // enable mjpeg clk
    
    jpegA_table_init(tp);
/*
    R_MJPEGA_CCOEF0 = CCOEF0;
    R_MJPEGA_CCOEF1 = CCOEF1;
    R_MJPEGA_CCOEF2 = CCOEF2;
    R_MJPEGA_CCOEF3 = CCOEF3;
    R_MJPEGA_CCOEF4 = CCOEF4;
    R_MJPEGA_CCOEF5 = CCOEF5;
    R_MJPEGA_CCOEF6 = CCOEF6;  
    R_MJPEGA_CCOEF7 = CCOEF7;*/
	R_MJPEGA_CCOEF0  = BIC_TAB00;
	R_MJPEGA_CCOEF1  = BIC_TAB01;
	R_MJPEGA_CCOEF2  = BIC_TAB02;
	R_MJPEGA_CCOEF3  = BIC_TAB03;
	R_MJPEGA_CCOEF4  = BIC_TAB04;
	R_MJPEGA_CCOEF5  = BIC_TAB05;
	R_MJPEGA_CCOEF6  = BIC_TAB06;
	R_MJPEGA_CCOEF7  = BIC_TAB07;
	R_MJPEGA_CCOEF10 = BIC_TAB10;
	R_MJPEGA_CCOEF11 = BIC_TAB11;
	R_MJPEGA_CCOEF12 = BIC_TAB12;
	R_MJPEGA_CCOEF13 = BIC_TAB13;
	R_MJPEGA_CCOEF14 = BIC_TAB14;
	R_MJPEGA_CCOEF15 = BIC_TAB15;
	R_MJPEGA_CCOEF16 = BIC_TAB16;
	R_MJPEGA_CCOEF17 = BIC_TAB17;
	R_MJPEGA_LINE_CCOEF0 = BIC_LINETAB00;
	R_MJPEGA_LINE_CCOEF1 = BIC_LINETAB01;
	R_MJPEGA_LINE_CCOEF2 = BIC_LINETAB02;
	R_MJPEGA_LINE_CCOEF3 = BIC_LINETAB03;
	R_MJPEGA_LINE_CCOEF4 = BIC_LINETAB04;
	R_MJPEGA_LINE_CCOEF5 = BIC_LINETAB05;
	R_MJPEGA_LINE_CCOEF6 = BIC_LINETAB06;
	R_MJPEGA_LINE_CCOEF7 = BIC_LINETAB07;
	R_MJPEGA_UVCCOEF0  = BIC_UVTAB00;
	R_MJPEGA_UVCCOEF1  = BIC_UVTAB01;
	R_MJPEGA_UVCCOEF2  = BIC_UVTAB02;
	R_MJPEGA_UVCCOEF3  = BIC_UVTAB03;
	R_MJPEGA_UVCCOEF4  = BIC_UVTAB04;
	R_MJPEGA_UVCCOEF5  = BIC_UVTAB05;
	R_MJPEGA_UVCCOEF6  = BIC_UVTAB06;
	R_MJPEGA_UVCCOEF7  = BIC_UVTAB07;
	R_MJPEGA_UVCCOEF10 = BIC_UVTAB10;
	R_MJPEGA_UVCCOEF11 = BIC_UVTAB11;
	R_MJPEGA_UVCCOEF12 = BIC_UVTAB12;
	R_MJPEGA_UVCCOEF13 = BIC_UVTAB13;
	R_MJPEGA_UVCCOEF14 = BIC_UVTAB14;
	R_MJPEGA_UVCCOEF15 = BIC_UVTAB15;
	R_MJPEGA_UVCCOEF16 = BIC_UVTAB16;
	R_MJPEGA_UVCCOEF17 = BIC_UVTAB17;

	R_MJPEGA_CTRL1 &= ~(0x0f << 23);
	R_MJPEGA_CTRL1 &= ~(0x03 << 19);
	R_MJPEGA_CTRL1 =	0//BIT(27)//JEOAUTOLD_EN
				|BIT(13)//ENCHOLD_EN
				//|BIT(JPBITSCHRLD_EN)
				//|BIT(JPBTSCH_EN)
				//|((quality & 0x0f)<< 23)
				|BIT(22)//DCTIDCT
				//|BIT(10)//ENCODST
				//|BIT(INFOMODE)
				//|BIT(HEADERSKIP)
				//|((mod & 0x03) << 19)//INFMOD:(0:no 1:8byte, 2:28buye)
				|BIT(0)//JEODMAEN
				//|BIT(DRISKIP)
				|BIT(11)//JESTSYNCEN
				|BIT(14)//WR_CRITICAL_EN
				|BIT(15)//RD_CRITICAL_EN
				;
	/*
				0//(1<<27)//JEOAUTOLD_EN
				|(1<<26)//ENCHOLD_EN
				//|(1<<21)
				//|(1<<PBTSCH_EN)
				//|((quality & 0x0f)<< 12)
				|(1<<11)//DCTIDCT
				//|(1<<10)//ENCODST
				//|(1<<INFOMODE)
				//|(1<<HEADERSKIP)
				//|((mode & 0x03) << 8)//INFMOD:(0:no 1:8byte, 2:28buye)
				|(1<<5)//JEODMAEN
				//|(1<<DRISKIP)
				|(1<<4)//JESTSYNCEN
				|(1<<29)//
				|(1<<28)//
				;
	*/
	R_MJPEGA_INTEN =BIT(31)//JPEG_INT_EN
				|BIT(6)//FRAMEDONE_INT_EN
				|BIT(4)//JEOLDARD_INT_EN
				//|BIT(JEO_FULL_INT_EN)
				|BIT(2)//DMA_OBERROR_INT_EN
				|BIT(0)//JPO_DMA_INT_EN
				//|BIT(11)//JPTINF0_INTEN 
				//|BIT(10)//JPTINF1_INTEN 
				//|BIT(9)//JPTINF2_INTEN 
				;
	/*
	(1<<8)//JPEG_INT_EN
				|(1<<7)//FRAMEDONE_INT_EN
				|(1<<4)//JEOLDARD_INT_EN
				//|(1<<JEO_FULL_INT_EN)
				|(1<<2)//DMA_OBERROR_INT_EN
				|(1<<0)//JPO_DMA_INT_EN
				|(1<<11);
				;
	*/
	PRE_JPELINECNT = 0;
	(manual)?(PRE_JPCON1 |= BIT(10)):(PRE_JPCON1 &= ~BIT(10));
	#if PSRAM_BURST16_EN
	PRE_JPCON1 |= BIT(21);	// JT529X  bit21 is BURST8/16 setting, dma read (default 0 is burst8)
	#endif
	R_MJPEGA_INTCLR = -1;
	R_MJPEGA_HEIGHT = (768<<16)|(600);	
}
void ax32xx_mjpeg_manual_on(void) //jpg encode start
{
	//PRE_JPCON1 |= BIT(10);
	//SetIntSrc(_CSI_INT_);	
	
	

	PRE_JPELINECNT  = 0;
	PRE_JPCON1 |= BIT(8); //JERUN
	PRE_JPELINECNT = 8;
	//Delay_MS(10);
	//debg("j -mon:%x,%x\n",PRE_JPCON1,PRE_JPINTCON);
	PRE_JPELINECNT = 800;
}

void ax32xx_mjpeg_manual_stop(void)	//encode start
{
	//PRE_JPCON1 |= BIT(10);
	//SetIntSrc(_CSI_INT_);	
	//PRE_JPELINECNT  = 0;
	PRE_JPCON1 |= BIT(9); //JESTOP
	//PRE_JPELINECNT = 8;
	//Delay_MS(10);
	//debg("j -mon:%x,%x\n",PRE_JPCON1,PRE_JPINTCON);
	//PRE_JPELINECNT = 800;
}

/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeScalerCal
* Description    : jpeg decode scaler caculate
* Input          : u32 src_w : source width
*                  u32 tar_w : source height
* Output         : none
* Return         : none
*******************************************************************************/
s32 ax32xx_mjpegDecodeScalerCal(u32 src_w,u32 tar_w)
{
	const u8 pre_xscale_table[] = {0,0,1,2,2};
	const u8 pre_xscale_table1[] = {0,1,2,2,3,3,3,3,3};
	u32 res = 0;
	u32 prescale0 = 0,prescale1 = 0,temp0 = src_w,dst_w4 = tar_w * 4;
	
	if(src_w > 1920)
	{
		prescale0 = pre_xscale_table[(src_w + 1920 - 1)/1920];
		temp0 = (src_w + (1 << prescale0) - 1)/(1 << prescale0);
	}
	
	if(temp0 >= dst_w4)
	{
		prescale1 = pre_xscale_table1[temp0 / dst_w4];
	}	
	
	temp0 = (prescale0 + prescale1);
	res = (temp0 << 16)|((src_w + (1 << temp0) - 1)/(1 << temp0));
	
	return res;	
}
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
s32 ax32xx_mjpegDecodeSetSize(u32 src_w,u32 src_h,u32 tar_w,u32 tar_h,u32 stride)
{
    static u32 sum0 = 0;
    static u32 jpcon2,jpsize0,jpsize1,jpxscale,jpyscale;
    
	u32 pre_xscale,scale_src_width,src_w1;
	u32 xi,xf,yi,yf;
	
	if(src_w > 4032)
		return -1;

    src_w1 = src_w;

    //避免重复计算
    u32 sum = src_w + src_h + tar_w + tar_h;
    if(sum0 != sum)
    {
        sum0 = sum;
        //避免prescale
        if((src_w / 4) == tar_w)
        {
            //保存原始尺寸
            R_MJPEG_CTRL2 =
            jpcon2 = (1<<28)|
                     (src_w<<16)|
                     (src_h<<0);
            src_w -= 2;
        }
        else
            R_MJPEG_CTRL2 = jpcon2 = 0;

        pre_xscale = ax32xx_mjpegDecodeScalerCal(src_w,tar_w);
        scale_src_width = pre_xscale & 0xffff;
        xi = scale_src_width / tar_w;
        xf = (((scale_src_width % tar_w)<<16) + tar_w - 1) / tar_w;
        yi = src_h / tar_h;
        yf = (((src_h % tar_h)<<16) + tar_h - 1) / tar_h;
        
        pre_xscale >>= 16;
        
        R_MJPEG_SIZE   = (src_w<<16)|//decoder src width
                         (src_h<<0); //decoder src height

        R_MJPEG_SIZE1  = (tar_w<<16)|//decoder dst width
                         (tar_h<<0); //decoder dst height

        R_MJPEG_XSCALER= (pre_xscale<<18)|//x scale div
                         (xi<<16)|//integrate scale
                         (xf<<0); //fractional of scale

        R_MJPEG_YSCALER= (yi<<16)|//integrate scale
                         (yf<<0); //fractional of scale
        
        jpsize0  = R_MJPEG_SIZE;
        jpsize1  = R_MJPEG_SIZE1;
        jpxscale = R_MJPEG_XSCALER;
        jpyscale = R_MJPEG_YSCALER;
    }
    else
    {
        R_MJPEG_CTRL2   = jpcon2;
        R_MJPEG_SIZE    = jpsize0;
        R_MJPEG_SIZE1   = jpsize1;
        R_MJPEG_XSCALER = jpxscale;
        R_MJPEG_YSCALER = jpyscale;
    }
    
    if(stride != 0xffffffff)
    {
        R_MJPEG_STRIDE = (src_w1<<16)|//DMA1 stride
                         ((!!stride)<<12)|//decoder yuv420 manual stride en
                         (stride<<0); //decoder stride of yuv420 output
    }
	return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeOutputSet
* Description    : jpeg decode output buffer set
* Input          : u32 yaddr : yaddress
*                  u32 uvaddr: uv address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeOutputSet(u32 yaddr,u32 uvaddr)
{
	R_MJPEG_OUT_SADDR = yaddr;
	R_MJPEG_OUT_EADDR = uvaddr;
	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeInputSet
* Description    : jpeg decode input set
* Input          : u32 saddr : start address
*                  u32 eaddr : end address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInputSet(u32 saddr,u32 eaddr)
{
	R_MJPEG_IN_SADDR = saddr&(~3);
	R_MJPEG_IN_EADDR = eaddr;
	R_MJPEG_BIT      = (saddr&3)<<3;
	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeDriSet
* Description    : jpeg decode dri set
* Input          : u32 dri
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeDriSet(u32 dri)
{
	R_MJPEG_DRI = dri;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeCompressSet
* Description    : jpeg decode compress set
* Input          : u32 table : compress value
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeCompressSet(u32 table)
{
	R_MJPEG_TABLE = table;
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeInitTable
* Description    : jpeg decode huffman table
* Input          : u32 tab : table address
*                  u32 len : reserved,do not care this value
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInitTable(u32 tab,u32 len)
{
	u8 *dst = (u8 *)0x66400;
	u8 *src = (u8 *)tab;
	u32 i;
	
	for(i=0;i<796;i++)
	{
		*dst++ = *src++;
	}
	ax32xx_sysDcacheWback((u32)dst,796);
}
/*******************************************************************************
* Function Name  : ax32xx_mjepgDecodeInit
* Description    : jpeg decode initial
* Input          : u8 yuvfmt : 0:yuv420,1:yuv422
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeInit(u8 yuvfmt)
{
//    ax32xx_sysReset(SYS_RESET_JPGB,1); // reset mjpeg modlule
//    ax32xx_sysCpuNopDelay(10);
//    ax32xx_sysReset(SYS_RESET_JPGB,0); // reset mjpeg modlule
	ax32xx_sysClkSet(SYS_CLK_JPGB,1);
	R_MJPEG_CTRL1 =  (R_MJPEG_CTRL1 & ~(1<<16))|
                     (0<<31)| //linebuf_hold_en ,when the 1792<src_size<1920 and not use sample function
					 (1<<30)|//deblock en 
					 (1<<29)|//jpdelline en
                     (0<<28)|//encoder
                     (0<<27)|//encoder
                     (0<<23)|//encoder quantize select
                     (0<<22)|//codec DCI/IDCT select,0:IDCT
                     (0<<19)|//encoder add 8/28 byte AVI chunk header before .jpg header
                     (0<<18)|//encoder generate JPEG head en
                     (0<<17)|//encoder DRI enable
                     ((!!yuvfmt)<<16)|//decoder yuv420/yuv422 select
                     (0<<15)|//dma read critical en
                     (0<<14)|//dma write critical en
                     (0<<13)|//encoder hold en
                     (0<<12)|//encoder
                     (0<<11)|//encoder sync to csi frame start
                     (0<<10)|//encoder manual mode en
                     (0<<9)| //encoder stop and reset
                     (0<<8)| //encoder start
                     (0<<7)| //decoder stop and reset
                     (0<<6)| //decoder start
                     (1<<5)| //decoder end-marker select
                     (0<<4)| //encoder output auto reload en
                     (0<<3)| //encoder
                     (0<<2)| //decoder output DMA(no block-scaler) en<!JUST FOR BACK_JPEG>
                     (1<<1)| //input DMA en
                     (1<<0); //output DMA en
	R_MJPEG_INPTR   = 0;
	R_MJPEG_INTEN   = (1<<31)|//codec jpeg int en
					  (0<<13)| //dc time over int en
                      (0<<11)|//time-info int en
                      (0<<10)|//time-info int en
                      (0<<9)| //time-info int en
                      (1<<8)| //decoder huffman err int en
                      (0<<7)| //encoder
                      (1<<6)| //codec frame done int en
                      (1<<5)| //decoder DRI err int en
                      (0<<4)| //encoder reload output addr int en
                      (0<<3)| //output buffer error int en
                      (0<<2)| //decoder DMA(no block-scaler) frame done int en
                      (0<<1)| //decode DMA input int en,input one packet,one interrupt
                      (0<<0); //encode DMA output int en,output one packet,one interrupt
	
    R_MJPEG_INTCLR = (1<<11)|//time-info
                     (1<<10)|//time-info
                     (1<<9)| //time-info
                     (1<<8)| //decoder huffman err flag
                     (1<<7)| //encoder
                     (1<<6)| //codec frame done flag
                     (1<<5)| //decoder DRI err flag
                     (1<<4)| //encoder
                     (1<<3)| //output buffer error
                     (1<<2)| //decoder DMA(no block-scaler) frame done
                     (0<<1)| //decoder input dma resume en
                     (0<<0); //encoder output dma resume en
				
/*	R_MJPEG_CCOEF0 = CCTAB0;
	R_MJPEG_CCOEF1 = CCTAB1;
	R_MJPEG_CCOEF2 = CCTAB2;
	R_MJPEG_CCOEF3 = CCTAB3;
	R_MJPEG_CCOEF4 = CCTAB4;
	R_MJPEG_CCOEF5 = CCTAB5; 
	R_MJPEG_CCOEF6 = CCTAB6;
	R_MJPEG_CCOEF7 = CCTAB7;
*/
	R_MJPEG_CCOEF0  = BIC_TAB00;
	R_MJPEG_CCOEF1  = BIC_TAB01;
	R_MJPEG_CCOEF2  = BIC_TAB02;
	R_MJPEG_CCOEF3  = BIC_TAB03;
	R_MJPEG_CCOEF4  = BIC_TAB04;
	R_MJPEG_CCOEF5  = BIC_TAB05;
	R_MJPEG_CCOEF6  = BIC_TAB06;
	R_MJPEG_CCOEF7  = BIC_TAB07;
	R_MJPEG_CCOEF10 = BIC_TAB10;
	R_MJPEG_CCOEF11 = BIC_TAB11;
	R_MJPEG_CCOEF12 = BIC_TAB12;
	R_MJPEG_CCOEF13 = BIC_TAB13;
	R_MJPEG_CCOEF14 = BIC_TAB14;
	R_MJPEG_CCOEF15 = BIC_TAB15;
	R_MJPEG_CCOEF16 = BIC_TAB16;
	R_MJPEG_CCOEF17 = BIC_TAB17;
	R_MJPEG_LINE_CCOEF0 = BIC_LINETAB00;
	R_MJPEG_LINE_CCOEF1 = BIC_LINETAB01;
	R_MJPEG_LINE_CCOEF2 = BIC_LINETAB02;
	R_MJPEG_LINE_CCOEF3 = BIC_LINETAB03;
	R_MJPEG_LINE_CCOEF4 = BIC_LINETAB04;
	R_MJPEG_LINE_CCOEF5 = BIC_LINETAB05;
	R_MJPEG_LINE_CCOEF6 = BIC_LINETAB06;
	R_MJPEG_LINE_CCOEF7 = BIC_LINETAB07;
	R_MJPEG_UVCCOEF0  = BIC_UVTAB00;
	R_MJPEG_UVCCOEF1  = BIC_UVTAB01;
	R_MJPEG_UVCCOEF2  = BIC_UVTAB02;
	R_MJPEG_UVCCOEF3  = BIC_UVTAB03;
	R_MJPEG_UVCCOEF4  = BIC_UVTAB04;
	R_MJPEG_UVCCOEF5  = BIC_UVTAB05;
	R_MJPEG_UVCCOEF6  = BIC_UVTAB06;
	R_MJPEG_UVCCOEF7  = BIC_UVTAB07;
	R_MJPEG_UVCCOEF10 = BIC_UVTAB10;
	R_MJPEG_UVCCOEF11 = BIC_UVTAB11;
	R_MJPEG_UVCCOEF12 = BIC_UVTAB12;
	R_MJPEG_UVCCOEF13 = BIC_UVTAB13;
	R_MJPEG_UVCCOEF14 = BIC_UVTAB14;
	R_MJPEG_UVCCOEF15 = BIC_UVTAB15;
	R_MJPEG_UVCCOEF16 = BIC_UVTAB16;
	R_MJPEG_UVCCOEF17 = BIC_UVTAB17;
	
	R_MJPEG_DEBLOCK = ((c_table_chroma[INDEX_CHROMA]&0xf)<<28)|      //c_chroma
					  ((c_table_luma[INDEX_LUMA]&0xf)<<24)|   	   //c_luma
					  ((belta_table_chroma[INDEX_CHROMA]&0x3f)<<18)| //belta_chroma
				      ((alpha_table_chroma[INDEX_CHROMA]&0x3f)<<12)| //alpha_chroma
				      ((belta_table_luma[INDEX_LUMA]&0x3f)<<6)|      //belta_luma
				      (alpha_table_luma[INDEX_LUMA]&0x3f);           //alpha_luma
	/*	
	R_MJPEG_INTEN |= (1<<13) ; //dc time over int en
	JPDCTIMER = (1<<31)|  //DC TIMER CHECK EN
				(((45*(COMBO_CLOCK/1000))&(0x7fffffff))<<0);// dc timer count : 45ms
*/
	
    jpegFlag = 1;
    jpegErrStatus = 0;		
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeDCTimeSet
* Description    : jpeg decode dc time over set
* Input          : u8 en,u32 src_w,u32 src_h
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeDCTimeSet(u8 en, u32 src_w,u32 src_h)
{
	int time_count = 0; //ms
/*
	switch(src_h)
	{
	// these time count setting will be modifyed after test
		case 480: 
		case 720:   time_count = 45; break;
		case 960: 
		case 1080:  time_count = 80; break;
		case 1440: 
		case 2160:  time_count = 150; break;
		case 3000:  time_count = 300; break;
		default: //
			time_count = 100; break;
	}
*/
	if(src_h <= 720)
	{
		time_count = 150;
	}
	else if(src_h <= 1080)
	{
		time_count = 280;
	}
	else if(src_h <= 2160)
	{
		time_count = 350;
	}
	else
	{
		time_count = 500;
	}

	if(en)
	{
		R_MJPEG_INTEN |= (1<<13) ; //dc time over int en
		
		R_MJPEG_DCTIMER = (1<<31)|  //DC TIMER CHECK EN
					(((time_count*(COMBO_CLOCK/1000))&(0x7fffffff))<<0);// dc timer count : 45ms
	}
	else{
		R_MJPEG_INTEN &= ~(1<<13) ; //dc time over int dis
		R_MJPEG_DCTIMER &= ~(1<<31);       //DC TIMER CHECK dis
	}
	
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeEnable
* Description    : jpeg decode enable
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeEnable(u8 en)
{
	if(en)
	{
		R_MJPEG_CTRL1  |= (1<<29)|//decoder
                         (0<<28)|//encoder
                         (0<<27)|//encoder
                         (0<<23)|//encoder quantize select
                         (0<<22)|//codec DCI/IDCT select,0:IDCT
                         (0<<19)|//encoder add 8/28 byte AVI chunk header before .jpg header
                         (0<<18)|//encoder generate JPEG head en
                         (0<<17)|//encoder DRI enable
                         (0<<16)|//decoder yuv420/yuv422 select
                         (0<<15)|//dma read critical en
                         (0<<14)|//dma write critical en
                         (0<<13)|//encoder hold en
                         (0<<12)|//encoder
                         (0<<11)|//encoder sync to csi frame start
                         (0<<10)|//encoder manual mode en
                         (0<<9)| //encoder stop and reset
                         (0<<8)| //encoder start
                         (0<<7)| //decoder stop and reset
                         (1<<6)| //decoder start
                         (1<<5)| //decoder end-marker select
                         (0<<4)| //encoder output auto reload en
                         (0<<3)| //encoder
                         (0<<2)| //decoder output DMA(no block-scaler) en<!JUST FOR BACK_JPEG>
                         (1<<1)| //input DMA en
                         (1<<0); //output DMA en
		jpegFlag = 1;
		ax32xx_intEnable(IRQ_JPGB,1);	
        	
	}
	else
	{
		ax32xx_intEnable(IRQ_JPGB,0);	
		jpegFlag = 0;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeEnable
* Description    : jpeg decode enable
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeKick(void)
{
		R_MJPEG_CTRL1  |= (1<<29)|//decoder
                         (0<<28)|//encoder
                         (0<<27)|//encoder
                         (0<<23)|//encoder quantize select
                         (0<<22)|//codec DCI/IDCT select,0:IDCT
                         (0<<19)|//encoder add 8/28 byte AVI chunk header before .jpg header
                         (0<<18)|//encoder generate JPEG head en
                         (0<<17)|//encoder DRI enable
                         (0<<16)|//decoder yuv420/yuv422 select
                         (0<<15)|//dma read critical en
                         (0<<14)|//dma write critical en
                         (0<<13)|//encoder hold en
                         (0<<12)|//encoder
                         (0<<11)|//encoder sync to csi frame start
                         (0<<10)|//encoder manual mode en
                         (0<<9)| //encoder stop and reset
                         (0<<8)| //encoder start
                         (0<<7)| //decoder stop and reset
                         (1<<6)| //decoder start
                         (1<<5)| //decoder end-marker select
                         (0<<4)| //encoder output auto reload en
                         (0<<3)| //encoder
                         (0<<2)| //decoder output DMA(no block-scaler) en<!JUST FOR BACK_JPEG>
                         (1<<1)| //input DMA en
                         (1<<0); //output DMA en
	ax32xx_intEnable(IRQ_JPGB,0);	
	jpegFlag = 0;	
}

/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeStop
* Description    : jpeg decode stop
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeStop(void)
{
	R_MJPEG_CTRL1 |= (1<<7);
	R_MJPEG_CTRL1  = 0;
    
    R_MJPEG_INTCLR = (1<<11)|//time-info
                     (1<<10)|//time-info
                     (1<<9)| //time-info
                     (1<<8)| //decoder huffman err flag
                     (1<<7)| //encoder
                     (1<<6)| //codec frame done flag
                     (1<<5)| //decoder DRI err flag
                     (1<<4)| //encoder
                     (1<<3)| //output buffer error
                     (1<<2)| //decoder DMA(no block-scaler) frame done
                     (0<<1)| //decoder input dma resume en
                     (0<<0); //encoder output dma resume en
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeQDTCfg
* Description    : jpeg decode config qdt table
* Input          : u8 *qdt[3] : table
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_mjpegDecodeQDTCfg(u8 *qdt[2])
{
	u8 i;
	u8 *dst = (u8 *)0x66400;

	for(i=0;i<2;i++)
	{
		memcpy(dst,qdt[i],64);
		dst+=64;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeStatus
* Description    : jpeg decode get status
* Input          : none
* Output         : none
* Return         : s32 : 1-busy,0-idle
*******************************************************************************/
s32 ax32xx_mjpegDecodeStatus(void)
{
	//debg("R_MJPEG_INTSTATUS&:%x\n",R_MJPEG_INTSTATUS & (3 << 16));
	if(R_MJPEG_INTSTATUS&(3<<16))
		return 1;  // busy
	else
		return 0;  // idle
}
/*******************************************************************************
* Function Name  : ax32xx_mjpegDecodeCheck
* Description    : jpeg decode error check
* Input          : none
* Output         : none
* Return         : s32 : 0-no error.other-error
*******************************************************************************/
s32 ax32xx_mjpegDecodeCheck(void)
{
	s32 temp;
	temp = jpegErrStatus;
	jpegErrStatus = 0;
	return temp;
}

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
void ax32xx_mjpegDecodeODma1Cfg(u8 * y_addr,u8 * uv_addr,bool dma_en)
{
    R_MJPEG_DMA1YBASE   = (u32)y_addr;
    R_MJPEG_DMA1UVBASE  = (u32)uv_addr;
    
    if(!dma_en)
    {
        //disable dma1
        R_MJPEG_CTRL1   &=~(1<<2);
        R_MJPEG_INTEN   &=~(1<<2);
        R_MJPEG_INTCLR   = (1<<2);
    }
    else
    {
        if(ax32xx_mjpegDecodeStatus())
        {
        }
        else
        {
            R_MJPEG_CTRL1 |= (1<<2);//decoder output DMA(no block-scaler) en<!JUST FOR BACK_JPEG>
            R_MJPEG_INTEN |= (1<<2);
        }
    }
}

#if 0
void ax32xx_mpegEncodePrint(void)
{
	
  
	debgreg(PRE_JPCON1);
    debgreg(PRE_JPCON2);
    debgreg(PRE_JPCON3);
    debgreg(PRE_JPSIZE0);
    debgreg(PRE_JPOBAS);
    debgreg(PRE_JPOYADR);
    debgreg(PRE_JPOEND);
    debgreg(PRE_JPOUVADR);
    debgreg(PRE_JPOPTR);
    debgreg(PRE_JPOLTA);
    debgreg(PRE_JPIBAS);
    debgreg(PRE_JPIYADR);
    debgreg(PRE_JPIEND);
    debgreg(PRE_JPIUVADR);
    debgreg(PRE_JPIPTR);
    debgreg(PRE_JPUBIT);
    debgreg(PRE_JPHCNT);
    debgreg(PRE_JPDRI);
    debgreg(PRE_JPBSHADDR);
    debgreg(PRE_JPINTCON);
    debgreg(PRE_JPFLG);
    debgreg(PRE_JPLS);
    debgreg(PRE_JPES);
    debgreg(PRE_JPCOM);
    debgreg(PRE_JPTINF);
    debgreg(PRE_JPT0SIZE);
    debgreg(PRE_JPT1SIZE);
    debgreg(PRE_JPT2SIZE);
    debgreg(PRE_JPT0POS);
    debgreg(PRE_JPT1POS);
    debgreg(PRE_JPT2POS);
    debgreg(PRE_JPT0BASE);
    debgreg(PRE_JPT1BASE);
    debgreg(PRE_JPT2BASE);
    debgreg(PRE_JPSIZE1);
    debgreg(PRE_JPXSCALE);
    debgreg(PRE_JPYSCALE);
    debgreg(PRE_JPDCXCROP);
    debgreg(PRE_JPDCYCROP);
    debgreg(PRE_JPFLG_CLR);
    debgreg(PRE_JPPAUSECNT);
    debgreg(PRE_JPDCYBASE);
    debgreg(PRE_JPDCUVBASE);
    debgreg(PRE_JPDMAOUTX32);
	debgreg(PRE_JPELINECNT);
	debgreg(PRE_DCTIMER);

}

const u32 bicCtab[]=
{
	0x00017F00, 
	0xFF0E7CF7, 
	0xFC2170F3, 
	0xF8365FF3, 
	0xF54B4BF5, 
	0xF35F36F8, 
	0xF37021FC, 
	0xF77C0EFF,
};
#endif
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
extern void ax32xx_mjpB_tab_init(void);
void ax32xx_mjpB_Ctl_init(u8 mod, u8 quality, u32 src_w,  u32 src_h, u32 obj_w, u32 obj_h)
{
	ax32xx_mjpB_reset();
	//humfamn, quanta tab cfg
	ax32xx_mjpB_tab_init();
	
	//scale cfg
	if(src_w > obj_w){
		src_w = obj_w;
	}
	if(src_h > obj_h){
		src_h = obj_h;	
	}
	BACK_JPSIZE1 = (src_w << 16) | src_h;//src size
	BACK_JPSIZE0 = (obj_w << 16) | obj_h;//obj size
	
	BACK_JPXSCALE = (0 << 18)  //div n(ecdoe not use)
				|(src_w/obj_w << 16)//intergser
				|((u16)(src_w*65536 /obj_w + (bool)(src_w*65536 %obj_w))  << 0);
	
	BACK_JPYSCALE = (src_h/obj_h << 16)//intergser
				|((u16)(src_h*65536 /obj_h + (bool)(src_h*65536 %obj_h))  << 0);
				
/*	BACK_JPBSCCOEF0 = bicCtab[0];
	BACK_JPBSCCOEF1 = bicCtab[1];
	BACK_JPBSCCOEF2 = bicCtab[2];
	BACK_JPBSCCOEF3 = bicCtab[3];
	BACK_JPBSCCOEF4 = bicCtab[4];
	BACK_JPBSCCOEF5 = bicCtab[5];
	BACK_JPBSCCOEF6 = bicCtab[6];
	BACK_JPBSCCOEF7 = bicCtab[7];
	 */
	R_MJPEG_CCOEF0  = BIC_TAB00;
	R_MJPEG_CCOEF1  = BIC_TAB01;
	R_MJPEG_CCOEF2  = BIC_TAB02;
	R_MJPEG_CCOEF3  = BIC_TAB03;
	R_MJPEG_CCOEF4  = BIC_TAB04;
	R_MJPEG_CCOEF5  = BIC_TAB05;
	R_MJPEG_CCOEF6  = BIC_TAB06;
	R_MJPEG_CCOEF7  = BIC_TAB07;
	R_MJPEG_CCOEF10 = BIC_TAB10;
	R_MJPEG_CCOEF11 = BIC_TAB11;
	R_MJPEG_CCOEF12 = BIC_TAB12;
	R_MJPEG_CCOEF13 = BIC_TAB13;
	R_MJPEG_CCOEF14 = BIC_TAB14;
	R_MJPEG_CCOEF15 = BIC_TAB15;
	R_MJPEG_CCOEF16 = BIC_TAB16;
	R_MJPEG_CCOEF17 = BIC_TAB17;
	R_MJPEG_LINE_CCOEF0 = BIC_LINETAB00;
	R_MJPEG_LINE_CCOEF1 = BIC_LINETAB01;
	R_MJPEG_LINE_CCOEF2 = BIC_LINETAB02;
	R_MJPEG_LINE_CCOEF3 = BIC_LINETAB03;
	R_MJPEG_LINE_CCOEF4 = BIC_LINETAB04;
	R_MJPEG_LINE_CCOEF5 = BIC_LINETAB05;
	R_MJPEG_LINE_CCOEF6 = BIC_LINETAB06;
	R_MJPEG_LINE_CCOEF7 = BIC_LINETAB07;
	R_MJPEG_UVCCOEF0  = BIC_UVTAB00;
	R_MJPEG_UVCCOEF1  = BIC_UVTAB01;
	R_MJPEG_UVCCOEF2  = BIC_UVTAB02;
	R_MJPEG_UVCCOEF3  = BIC_UVTAB03;
	R_MJPEG_UVCCOEF4  = BIC_UVTAB04;
	R_MJPEG_UVCCOEF5  = BIC_UVTAB05;
	R_MJPEG_UVCCOEF6  = BIC_UVTAB06;
	R_MJPEG_UVCCOEF7  = BIC_UVTAB07;
	R_MJPEG_UVCCOEF10 = BIC_UVTAB10;
	R_MJPEG_UVCCOEF11 = BIC_UVTAB11;
	R_MJPEG_UVCCOEF12 = BIC_UVTAB12;
	R_MJPEG_UVCCOEF13 = BIC_UVTAB13;
	R_MJPEG_UVCCOEF14 = BIC_UVTAB14;
	R_MJPEG_UVCCOEF15 = BIC_UVTAB15;
	R_MJPEG_UVCCOEF16 = BIC_UVTAB16;
	R_MJPEG_UVCCOEF17 = BIC_UVTAB17;
	//debg("> Jsacle: %x,%x,%x,%x\n",JPSIZE,JPSIZE1,JPXSCALE,JPYSCALE);
	
	BACK_JPCON1 &= ~(0x0f << 23);//Q
	BACK_JPCON1 &= ~(0x03 << 19);//jpg & mjpg mod
	

	BACK_JPCON1 = 0//BIT(27)//JEOAUTOLD_EN
				|BIT(13)//ENCHOLD_EN
				//|BIT(JPBITSCHRLD_EN)
				//|BIT(JPBTSCH_EN)
				|((quality & 0x0f)<< 23)
				|BIT(22)//DCTIDCT
				//|BIT(10)//ENCODST
				//|BIT(INFOMODE)
				//|BIT(HEADERSKIP)
				|((mod & 0x03) << 19)//INFMOD:(0:no 1:8byte, 2:28buye)
				|BIT(0)//JEODMAEN
				//|BIT(DRISKIP)
				|BIT(11)//JESTSYNCEN
				|BIT(14)//WR_CRITICAL_EN
				|BIT(15)//RD_CRITICAL_EN
				|BIT(10) //JPENCMAN (only manual)
				;

	//int cfg			
	BACK_JPINTCON = BIT(31)//JPEG_INT_EN
				|BIT(6)//FRAMEDONE_INT_EN
				|BIT(4)//JEOLDARD_INT_EN
				//|BIT(JEO_FULL_INT_EN)
				|BIT(2)//DMA_OBERROR_INT_EN
				|BIT(0)//JPO_DMA_INT_EN
				//|BIT(11)//JPTINF0_INTEN 
				//|BIT(10)//JPTINF1_INTEN 
				//|BIT(19)//JPTINF2_INTEN 
				;
	//clr all pending
	BACK_JPFLG_CLR = -1;
	
	BACK_JPHCNT = (768 << 16) //high
			|(600 << 0) //low
			;
	//debg("BACK_JPCON1:%x,%x\n",BACK_JPCON1,BACK_JPINTCON);
}
void ax32xx_mjpB_Linebuf_cfg(u8* ybuf, u8 *uvbuf)
{
	/*u32 i;
	
	for(i = 0 ; i < 640 *  480; i++){
		ybuf[i] = 0x80;
	}
	for(i = 0 ; i < 640 *  480/2; i++){
		uvbuf[i] = 0x00;
	}
	*/
	//BACK_JPELINECNT = 0;
	BACK_JPIBAS = (u32)ybuf;
	BACK_JPIEND = (u32)uvbuf;
	
	//debg("Jline:%x,%x\n",BACK_JPIBAS,BACK_JPIEND);
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpB_dma_cfg(u32 s, u32 e)
{
//	debg("jdma:%x,%x\n",s,e);
//	mjp_dmaadrr_check(s,e);
	//dcache_flush(s,e-s);
	BACK_JPOBAS = s ;
	BACK_JPOEND = (e | (0x03 << 28));
	
}
void ax32xx_mjpB_stop(void)
{
	BACK_JPCON1 |= BIT(9); 			//JESTOP
	BACK_JPINTCON &= ~BIT(11); 		//MJP_INIT_EN
	BACK_JPCON1 |= BIT(9); 			//JESTOP
	
	//mdf 2017/7/6
	//CSI_DMA_MODE &= ~BIT(31); 	//_CSI_MJP_OFF
	BACK_JPFLG_CLR = ~0;				//MJP_INT_PENDING	
	ax32xx_intEnable(IRQ_JPGB,0);		
	//debg("\nstop\n");
	//debg("j:%x",BACK_JPFLG);	
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpB_on(void)
{
	//CSIMODE  |= BIT(31);//CSI_DCEN
	//SetIntSrc(_CSI_INT_);	
	//mjpBec_recfg();
	BACK_JPCON1 |= BIT(8); //JERUN
	BACK_JPELINECNT = 8;
	//Delay_MS(10);
	//debg("j -mon:%x,%x\n",PRE_JPCON1,PRE_JPINTCON);
	BACK_JPELINECNT = 800;	
	ax32xx_intEnable(IRQ_JPGB,1);
	//debg("mjpB: <manual on>\n");	
}
void ax32xx_mjpB_stop_2(void)
{
	BACK_JPCON1 |= BIT(9); 			//JESTOP
	BACK_JPINTCON &= ~BIT(11); 		//MJP_INIT_EN
	BACK_JPCON1 |= BIT(9); 			//JESTOP
	
	//mdf 2017/7/6
	//CSI_DMA_MODE &= ~BIT(31); 	//_CSI_MJP_OFF
	BACK_JPFLG_CLR = ~0;				//MJP_INT_PENDING	
		
	//debg("\nstop\n");
	//debg("j:%x",BACK_JPFLG);	
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_mjpB_on_2(void)
{
	//CSIMODE  |= BIT(31);//CSI_DCEN
	//SetIntSrc(_CSI_INT_);	
	//mjpBec_recfg();
	BACK_JPCON1 |= BIT(8); //JERUN
	BACK_JPELINECNT = 8;
	//Delay_MS(10);
	//debg("j -mon:%x,%x\n",PRE_JPCON1,PRE_JPINTCON);
	BACK_JPELINECNT = 800;	
	//debg("mjpB: <manual on>\n");	
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool jpeg1_decode_check(void)
{
	//debgstr("decode_isr_test\n");
	u32 timeout=0;
	while(1)
	{
		timeout++;
		if(timeout>=0x0fffff){
			debg("frame decode fail:%x\n",BACK_JPFLG);
			break;
		}
		volatile u32 status = BACK_JPFLG;
		if(status & (BIT(8) | BIT(5)|BIT(12)|BIT(13))){
			
			
			if(status & BIT(8)){
				debg("huf err:%x\n",status);
			}
			if(status & BIT(5)){
				debg("dri err:%x\n",status);
			}	
			if(status & BIT(12)){
				debg("FFD9 err:%x\n",status);
			}
			if(status & BIT(13)){
				debg("dc time over:%x\n",status);
			}		
			ax32xx_mjpegDecodeStop();	
			//usb_usensor_frame_free();
		
			BACK_JPFLG_CLR |= BIT(6) | BIT(8)| BIT(5) |BIT(1) |BIT(2)|BIT(13)|BIT(12);
			break;
		}
		
		
		
		if(status & BIT(6))
		{
			//JPFLG_CLR = BIT(6);
			//debg("frame decode done\n");
			//ax32xx_mpegPrint();
			ax32xx_mjpegDecodeStop();
			//debgstr("> dec done \n");
			BACK_JPFLG_CLR |= BIT(6);
			return true;
			//debg("1");	
		
		}
	}
	return false;
}
