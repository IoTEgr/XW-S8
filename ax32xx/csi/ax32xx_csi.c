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
* File Name   : ax32xx_csi.c
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
#include "../inc/ax32xx.h"
#define  R_CSI_MODE              CSIMODE
#define  R_ISPCTRL               ISPCTRL
#define  R_CSI_INTEN             CSI_INTEN
#define  R_CSI_INTCLR            CSI_INTR //CSI_INTCLR
#define  R_CSI_INTSTATUS         CSI_INTR
#define  R_CSI_SIZE              CSI_IMAGE_SIZE
#define  R_CSI_LDMA_STRIDE       CSI_I2L_STRD

#define  R_CSI_INPUT_ADDR        CSI_DVP_BADR
#define  R_CSI_TEST_SIZE         CSI_TEST_SIZE
#define  R_CSI_TEST_SYNC         CSI_TEST_BLANK

#define  R_CSI_MJPEG_LINE        CSI_I2J_LBC
#define  R_CSI_MJPEG_SIZE        CSI_I2J_LBS
#define  R_CSI_MJPEG_YADDR       CSI_I2J_YBADR
#define  R_CSI_MJPEG_UVADDR      CSI_I2J_UVBADR

#define  R_CSI_LCD_YADDR         CSI_I2L_YBADR
#define  R_CSI_LCD_UVADDR        CSI_I2L_UVBADR
#define  R_CSI_LCD_CROPH         CSI_I2L_HCROP
#define  R_CSI_LCD_CROPV         CSI_I2L_VCROP 

#define  R_CSI_LCD_SCALER_WTABLE0   ISP_LS_HTAB0
#define  R_CSI_LCD_SCALER_WTABLE1   ISP_LS_HTAB1
#define  R_CSI_LCD_SCALER_WTABLE2   ISP_LS_HTAB2
#define  R_CSI_LCD_SCALER_WTABLE3   ISP_LS_HTAB3
#define  R_CSI_LCD_SCALER_WTABLE4   ISP_LS_HTAB4
#define  R_CSI_LCD_SCALER_WTABLE5   ISP_LS_HTAB5
#define  R_CSI_LCD_SCALER_WTABLE6   ISP_LS_HTAB6
#define  R_CSI_LCD_SCALER_WTABLE7   ISP_LS_HTAB7
#define  R_CSI_LCD_SCALER_WTABLE8   ISP_LS_HTAB8
#define  R_CSI_LCD_SCALER_WTABLE9   ISP_LS_HTAB9
#define  R_CSI_LCD_SCALER_WTABLE10  ISP_LS_HTAB10
#define  R_CSI_LCD_SCALER_WTABLE11  ISP_LS_HTAB11
#define  R_CSI_LCD_SCALER_WTABLE12  ISP_LS_HTAB12
#define  R_CSI_LCD_SCALER_WTABLE13  ISP_LS_HTAB13
#define  R_CSI_LCD_SCALER_WTABLE14  ISP_LS_HTAB14
#define  R_CSI_LCD_SCALER_WTABLE15  ISP_LS_HTAB15

#define  R_CSI_LCD_SCALER_HTABLE0   ISP_LS_VTAB0
#define  R_CSI_LCD_SCALER_HTABLE1   ISP_LS_VTAB1
#define  R_CSI_LCD_SCALER_HTABLE2   ISP_LS_VTAB2
#define  R_CSI_LCD_SCALER_HTABLE3   ISP_LS_VTAB3
#define  R_CSI_LCD_SCALER_HTABLE4   ISP_LS_VTAB4
#define  R_CSI_LCD_SCALER_HTABLE5   ISP_LS_VTAB5
#define  R_CSI_LCD_SCALER_HTABLE6   ISP_LS_VTAB6
#define  R_CSI_LCD_SCALER_HTABLE7   ISP_LS_VTAB7

#define  R_CSI_DMA_MODE          CSI_DMA_MODE

#define  R_CSI_SCALER_SIZE       ISP_LS_SIZE
#define  R_CSI_SCALER_WSTEP      ISP_LS_HSTP
#define  R_CSI_SCALER_HSTEP      ISP_LS_VSTP     


volatile u32 csifcnt;
u32 hal_csi_fcnt_get(void)
{
	u32 cnt = csifcnt;
	csifcnt = 0;
//	debg("csi_fcnt:%d\n",cnt);
	return cnt;
}


static u32 csi_errcnt;
static void (*ax32xxCSIISR[CSI_IRQ_MAX])(void);
void ax32xx_csiIRQHandler(void)
{
	volatile u32 intrcsi = R_CSI_INTSTATUS;
	int i;
//	debg("intrcsi:%x %x %x\n",intrcsi,CSI_DMA_MODE,R_CSI_INTEN);

	if(intrcsi & BIT(CSI_JFRA_END_INT)){
		//debg("CSI_JFRA_END_INT\n");
	}
	if(intrcsi & BIT(CSI_SEN_SIZE_ERR)){
		CSIMODE &= ~(3 << 26);
		CSIMODE |= (1 << 26);
		debg("CSI_SEN_SIZE_ERR:%X CSIMODE:%X\n",CSI_SIZE_STAT,CSIMODE);
	}
	if(intrcsi & BIT(CSI_SEN_SIZE_STAT_INT)){
		//debg("CSI_SIZE_INT:%X\n",CSI_SIZE_STAT);
		//debg("CSI_SCALER:%X %X %X %X %X %x\n",R_CSI_LCD_CROPH,R_CSI_LCD_CROPV,R_CSI_SCALER_SIZE,R_CSI_SCALER_WSTEP,R_CSI_SCALER_HSTEP,R_CSI_LDMA_STRIDE);
		csifcnt++;	
	}
	for(i=0;i<CSI_IRQ_ISP;i++)
	{
		if(intrcsi&(1<<i))
		{
			R_CSI_INTCLR = 1<<i;
			
			if(ax32xxCSIISR[i])
			   ax32xxCSIISR[i]();
			csi_errcnt = 0; 
		}
	}
	if(intrcsi & (0x1<<CSI_AE_HIDT_INT))
	{
		R_CSI_INTCLR = 0x3<<CSI_WP_DET_INT;
		if(ax32xxCSIISR[CSI_IRQ_ISP])
			ax32xxCSIISR[CSI_IRQ_ISP]();
		csi_errcnt = 0;
	}
    u32 error_mask = (7<<CSI_JE_CSI_ERR)|
                     (1<<CSI_ASYC_FIRO_ERR)|
                     (1<<CSI_J1BUFF_ERR)|
                     (7<<CSI_SEN_SIZE_ERR);
                     
	if(intrcsi & error_mask)
	{
		//BIT(18): JE_LINE_BUFF_ERR
		//BIT(17): JE_CSI_ERR
		//BIT(11): SYNC_FIFO_ERR
		//BIT(8):  WBUFF_ERR
		//BIT(6):  LCD_INTLBUF_ERR
		//BIT(5):  MJP_JINTLBUFF_ERR
		//BIT(4):  SENSOR_SIZE_ERR
		debg("ax32xx: <CSI_ERR:%x>\n",intrcsi & error_mask);
		
		if(csi_errcnt++ > 100){ //err cnt > 100 frame , must reset by WDT
			//csi_errcnt;  
			while(1);	
		}
        if(intrcsi & BIT(CSI_SEN_SIZE_ERR))
        {
            if(R_CSI_DMA_MODE & CSI_OUTPUT_LCDEN)
            {
                R_CSI_DMA_MODE &=~CSI_OUTPUT_LCDEN;//ldma dis
                ax32xx_pipCounterReset(PIP_SRC_ISP);
                R_CSI_DMA_MODE |= CSI_OUTPUT_LCDEN;//ldma en
            }
        }
		/*if(intrcsi & BIT(CSI_LBUFF_ERR)){
			R_CSI_INTEN &= ~BIT(CSI_LBUFF_ERR);
			R_CSI_DMA_MODE &= ~CSI_OUTPUT_LCDEN;
			
			R_CSI_INTCLR = BIT(CSI_LBUFF_ERR);
			ax32xx_pipCounterReset(PIP_SRC_ISP);
            
			R_CSI_DMA_MODE |= CSI_OUTPUT_LCDEN;
			R_CSI_INTEN |= BIT(CSI_LBUFF_ERR);
		}
		if(intrcsi & BIT(CSI_JBUFF_ERR)){
			ax32xx_mjpegEncodeEnable(0);
			void hal_mjpegStart(void);
			ax32xx_csiISRRegiser(CSI_IRQ_SEN_STATE_INT,hal_mjpegStart);
		}
		if(intrcsi & BIT(CSI_JE_LB_OVERFLOW)){
			ax32xx_mjpegEncodeEnable(0);
			void hal_mjpegStart(void);
			ax32xx_csiISRRegiser(CSI_IRQ_SEN_STATE_INT,hal_mjpegStart);
		}
		if(intrcsi & BIT(CSI_JE_CSI_ERR)){
		 // hardware auto lost frame,software doesn't need to do thing
		}*/
	}
	R_CSI_INTCLR = intrcsi;
}
/*******************************************************************************
* Function Name  : ax32xx_csiISRRegiser
* Description    : csi isr register
* Input          :  u8 type : irq type.CSI_IRQ_E
                       void (*isr)(void) : isr
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiISRRegiser(u8 type,void (*isr)(void))
{
	if(type<CSI_IRQ_MAX)
		ax32xxCSIISR[type] = isr;
}
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
void ax32xx_csiMJPEGFrameSet(u32 mjpeg_y,u32 mjpeg_uv,u32 lines,u32 width)
{
	R_CSI_MJPEG_LINE = lines&0x7ff;
	R_CSI_MJPEG_SIZE  = ((width+0x1f)/32)*32*lines;
	R_CSI_MJPEG_YADDR = mjpeg_y;
	R_CSI_MJPEG_UVADDR= mjpeg_uv;
	
	R_CSI_DMA_MODE &= ~CSI_OUTPUT_UVCEN;
	//R_CSI_DMA_MODE &= ~(7<<28);
	//R_CSI_DMA_MODE |= (CSI_BURST_32BYTE<<28);

	R_CSI_INTEN |= (1<<CSI_INT_EN)
				|(1<<CSI_JE_LB_OVERFLOW)	
				|(1<<CSI_JE_CSI_ERR)
				|(1<<CSI_ASYC_FIRO_ERR)	
				|(1<<CSI_JBUFF_ERR)
				|(1<<CSI_SEN_SIZE_ERR)
				|(1<<CSI_SEN_SIZE_STAT_INT)
				|(1<<CSI_JFRA_END_INT)
				|(1<<CSI_DVP_MEM_EMPTY)
				|(1<<CSI_STAT_CP_DONE)
				|(1<<CSI_WP_DET_INT)
				|(1<<CSI_AE_HIDT_INT)
				;	
}
void ax32xx_csiMjpegFrameSwitch(u32 mjpeg_y,u32 mjpeg_uv)
{
	R_CSI_MJPEG_YADDR = mjpeg_y;
	R_CSI_MJPEG_UVADDR= mjpeg_uv;
}
void ax32xx_csihv_tomjpg_Switch(u8 *Jy_buf,u8 *juv_buf)
{
	CSI_I2W_YBADR = (u32)Jy_buf;
	CSI_I2W_UVBADR= (u32)juv_buf;
}
void ax32xx_csihv_tomjp_dma_cfg(u8 *Jy_buf, u8 *juv_buf, u16 line, u16 w)
{
#if 1
	//CFG YUV cache
    CSI_I2W_LBC = line;
    CSI_I2W_LBS = (((w +0x1f)/32)*32 * line);//__IMG_H__必须跟32B对其
	CSI_I2W_YBADR = (u32)Jy_buf;//jyuvbuf.y_buf;
	CSI_I2W_UVBADR = (u32)juv_buf;//jyuvbuf.uv_buf;
	//debg("yadr:%x, uvadr: %x\n",CSI_JYDMA_ADR,CSI_JUVDMA_ADR);
	
	//
	CSI_DMA_MODE &= ~BIT(16); //uvc disable
	CSI_DMA_MODE &= ~(7 << 28);
	//CSI_DMA_MODE |= (CSI_BURST_32BYTE << 28)		//burst
					//|(1 << 31)
					;				//csi jdma en
	//DMA EN
	CSI_INTEN |= BIT(31)//_CSI_INT_EN_
					|BIT(18)//_CSI_LBUF_INT_EN_ 	
					|BIT(17)//_CSI_ERR_INT_EN_ 
					|BIT(11)//ASYC_FIFO_ERR_EN
					//|BIT(6)//_LCDBUF_ER_INT_EN_		
					|BIT(9)// CSI_J1FRA_END_INT
					|BIT(8)//CSI_J1BUFF_ERR
					|BIT(5)//_JBUF_ER_INT_EN_
					|BIT(4)//_SENSER_SIZE_ER
					|BIT(3)//_SENSER_RESOLIVING_TEST_EN
					|BIT(0)//_JFRAME_END_INT_EN_
					;	
	//debg("ladr:%x,%x\n",CSI_J1YDMA_ADR,CSI_J1UVDMA_ADR);
	//debg("CSI_JPACKET_SIZE:%x\n",CSI_JPACKET_SIZE);
#endif
}
/*******************************************************************************
* Function Name  : ax32xx_csi_only_FrameSet
* Description    : ax32xx_csi_only_FrameSet for ezxposure
* Input          : none 
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csi_only_FrameSet()
{
	u32 temp = 0;
	temp        |= (1<<CSI_INT_EN)	
				|(1<<CSI_JE_CSI_ERR)
				|(1<<CSI_ASYC_FIRO_ERR)	
				|(1<<CSI_SEN_SIZE_ERR)
				|(1<<CSI_SEN_SIZE_STAT_INT)
				|(1<<CSI_DVP_MEM_EMPTY)
				|(1<<CSI_STAT_CP_DONE)
				|(1<<CSI_WP_DET_INT)
				|(1<<CSI_AE_HIDT_INT)
	
				;	
	R_CSI_INTEN = temp;
	debg("CSI_INTEN:%x %x\n",temp,R_CSI_INTEN);
}
/*******************************************************************************
* Function Name  : ax32xx_csiYUVFrameSet
* Description    : csi yuv buffer set for uvc
* Input          :  u32 yaddr0 : y pixel address 0
                       u32 yaddr1 : y pixel address 1
                       u32 size     : frame size
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiYUVFrameSet(u32 yaddr0,u32 yaddr1,u32 size)
{
//    R_CSI_LCD_YADDR = yaddr0;
//	
//	R_CSI_DMA_MODE &= ~(7 << 28);
//	R_CSI_DMA_MODE &= ~CSI_OUTPUT_UVCSIZE; //size clr
//	
//	R_CSI_DMA_MODE = (7 << 21)		//lcd burst
//				|CSI_OUTPUT_UVCEN//_UVC_EN_
//				|CSI_OUTPUT_LCDEN//_CSI_LDMA_EN_
//				|CSI_OUTPUT_RELOAD//_CSI_LADR_RELOAD_
//				|(size & CSI_OUTPUT_UVCSIZE)
//				;	
//    R_CSI_LCD_YADDR = yaddr1;	
//	
//	R_CSI_INTEN = (1<<CSI_INT_EN)
//				|(1<<CSI_JE_LB_OVERFLOW)	
//				|(1<<CSI_JE_CSI_ERR)
//				|(1<<CSI_ASYC_FIRO_ERR)
//				|(1<<CSI_LBUFF_ERR)	
//				|(1<<CSI_SEN_SIZE_ERR)
//				|(1<<CSI_SEN_SIZE_STAT_INT)
//				|(1<<CSI_UVC_PACKET_INT)
//				|(1<<CSI_JFRA_END_INT)
//				;	
}
/*******************************************************************************
* Function Name  : ax32xx_csiIOConfig
* Description    : csi io config set
* Input          :  none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiIOConfig(void)
{
	ax32xx_gpioSFRSet(GPIO_MAP_CSI,CSI_POS);
    if(CSI_POS == CSI_POS1)
    {
        ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN10,GPIO_OUTPUT); // mCLK
        ax32xx_gpioDirSet(GPIO_PE,/*GPIO_PIN0|GPIO_PIN1|*/GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7|GPIO_PIN8|GPIO_PIN9|GPIO_PIN11|GPIO_PIN12|GPIO_PIN14,GPIO_INPUT);
        ax32xx_gpioMapSet(GPIO_PE,/*GPIO_PIN0|GPIO_PIN1|*/GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7|GPIO_PIN8|GPIO_PIN9|GPIO_PIN10|GPIO_PIN11|GPIO_PIN12|GPIO_PIN14,GPIO_FUNC_SFR);	
    }
    else if(CSI_POS == CSI_POS0)
    {
        
    }
	
}
/*******************************************************************************
* Function Name  : ax32xx_csiInit
* Description    : csi initial 
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiInit(void)
{
	ax32xx_csiIOConfig();
	
	//CLKCON0 &= ~((1<<24) | (1<<25));//select  pclk form sensor
	
	ax32xx_sysClkSet(SYS_CLK_DVP_CSI,1);
}
/*******************************************************************************
* Function Name  : ax32xx_csiMclkSet
* Description    : csi mclk set
* Input          : u32 clk : clock
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiMclkSet(u32 clk)
{
	REG32(CLKCON2) &= ~(1<<26);

	REG32(CLKCON2) |= (1<<26);	
	
	ax32xx_gpioSFRSet(GPIO_MAP_CSI,CSI_POS);
	ax32xx_gpioMapSet(GPIO_PE,GPIO_PIN10,GPIO_FUNC_SFR);  // set mclk output
	ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN10,GPIO_OUTPUT);    
	
#if 0
    //USB PHY PLL
	USB20CON0 |= 0xc0;//usb pll enable
	CLKCON0 |= BIT(5);
	
	CLKCON1 &= 0xffffe0ff ;
	CLKCON1 |= (240000000/clk - 1) << 8;  //senclk =pll/(div+1)
#else
    //SPLL
    CLKCON0 &= ~BIT(5);
	CLKCON1 &= 0xffffe0ff ;
	CLKCON1 |= (PLL_CLK/clk - 1) << 8;  //senclk =pll/(div+1)
#endif
}

void ax32xx_csi_set_mclk_free()
{
	ax32xx_gpioSFRSet(GPIO_MAP_CSI,CSI_POS);
	ax32xx_gpioMapSet(GPIO_PE,GPIO_PIN10,GPIO_FUNC_GPIO);  // set mclk input
	ax32xx_gpioDirSet(GPIO_PE,GPIO_PIN10,GPIO_INPUT);    
	
}
/*******************************************************************************
* Function Name  : ax32xx_csiSyncSet
* Description    : csi sync set
* Input          : u8 hsync_en : hsync
                      u8 vsync_en : vsync
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiSyncSet(u8 hsync_en,u8 vsync_en)
{
    R_CSI_MODE &= ~(CSI_MODE_VSYNC|CSI_MODE_HSYNC);
	if(hsync_en){
		R_CSI_MODE |= CSI_MODE_HSYNC;
	}
	if(vsync_en){
		R_CSI_MODE |= CSI_MODE_VSYNC;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_csiPrioritySet
* Description    : csi priority set
* Input          : u8 type : type .CSI_POLARITY_E
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiPrioritySet(u8 plo)
{
	R_CSI_MODE &= ~(CSI_MODE_PRIORITY);
	R_CSI_MODE |= (plo&0x03)*(CSI_MODE_PRIORITY / 3);
}
/*******************************************************************************
* Function Name  : ax32xx_csiTypeSet
* Description    : csi type set
* Input          : u8 type : type .CSI_TYPE_E
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiTypeSet(u8 type)
{
	R_CSI_MODE &= ~CSI_MODE_FORMAT;//0xc0   数据格式
	R_CSI_MODE &= ~CSI_MODE_DATA_BW;//c00	数据宽度
	if(type < 3){
		R_CSI_MODE |= type*(CSI_MODE_DATA_BW / 3);
	}else{
		R_CSI_MODE |= ((type-1)&0x3)*(CSI_MODE_FORMAT / 3);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_csiModeSet
* Description    : csi mode set
* Input          : u32 mode : mode flag
                      u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiModeSet(u32 mode,u8 en)
{
	if(en)
		R_CSI_MODE |= mode;
	else
		R_CSI_MODE &= ~mode;
		
}
/*******************************************************************************
* Function Name  : ax32xx_pclk_fir_Set
* Description    : pclk filter set
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_pclk_fir_Set(u8 en)
{
	if(en){
		CLKCON0 |= BIT(26);  //DVP filter use DDRPLL
	#if 0
		if((USB20CON0 &0xc0)!=0xc0){
			debg("USB PLL is not enable!!!");
			USB20CON0 |= 0x80;//osc enable
			USB20CON0 |= 0x40;//pll enable
			debg("Enable USB PLL for PCLK Filter!!!");
		}
	#else
		//usb20_phyreset_as_filter();
	#endif
		R_CSI_MODE |= en<<26;
	}
	else
		R_CSI_MODE &= ~(0xf<<26);
}
/*******************************************************************************
* Function Name  : ax32xx_pclk_analog_Set
* Description    : pclk filter set
* Input          : u8 en : step
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_pclk_analog_Set(u8 en)
{
	if(en){
		R_CSI_MODE |= en<<22;
	}
	else
		R_CSI_MODE &= ~(0xf<<22);
}
/*******************************************************************************
* Function Name  : ax32xx_pclk_inv_Set
* Description    : pclk filter set
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_pclk_inv_Set(u8 en)
{
	if(en){
		R_CSI_MODE |= CSI_MODE_PCLK_INV;
	}
	else
		R_CSI_MODE &= ~CSI_MODE_PCLK_INV;
}
/*******************************************************************************
* Function Name  : ax32xx_csi_clk_tun_Set
* Description    : pclk tun set
* Input          : u8 tun : tun val
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csi_clk_tun_Set(u8 tun)
{
	if(tun){
		CLKTUN |= (1 << 31) |((tun&0x1f) << 26); 
	}
}
/*******************************************************************************
* Function Name  : ax32xx_csiSizeSet
* Description    : csi size set
* Input          : u16 width : width
                      u16 height: height
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiSizeSet(u16 width,u16 height)
{
	R_CSI_SIZE = ((height-1)<<16)|(width-1);
}
/*******************************************************************************
* Function Name  : ax32xx_csiInputAddrSet
* Description    : csi input from sdram addr set
* Input          : u32 addr : input address
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiInputAddrSet(u32 addr)
{
	R_CSI_INPUT_ADDR = addr;
}
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
void ax32xx_csiTestModeSet(u16 width,u16 height,u16 hsync,u16 vsync,u16 test_typ)
{
	R_CSI_MODE &= ~CSI_MODE_TEST;
	R_CSI_MODE |= (test_typ&0xf) * (CSI_MODE_TEST / 15);
	R_CSI_TEST_SIZE = (height<<16)+width;
	R_CSI_TEST_SYNC = (vsync<<16)+hsync;
}
/*******************************************************************************
* Function Name  : ax32xx_csiDvpClkDivSet
* Description    : csi dvp gennerate clk_div set
* Input          : u16 clk_div_mode : clk_div
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiDvpClkDivSet(u16 clk_div_mode)
{
	R_CSI_MODE &= ~CSI_MODE_DVP_PCLK_DIV;
	R_CSI_MODE |= (clk_div_mode & 0x3) * (CSI_MODE_DVP_PCLK_DIV / 3);

}
/*******************************************************************************
* Function Name  : ax32xx_csiLCDFrameSet
* Description    : csi set output frame addr.if any of the r address is NULL,do not care it.
* Input          : u32 yaddr : y pixel buffer addr
                      u32 uvaddr: uv pixel buffer addr.
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiLCDFrameSet(u32 yaddr,u32 uvaddr)
{
	if(yaddr)
	    R_CSI_LCD_YADDR = yaddr;
	if(uvaddr)
	    R_CSI_LCD_UVADDR= uvaddr;
}
/*******************************************************************************
* Function Name  : ax32xx_csiLCDDmaEnable
* Description    : csi output for lcd control enable
* Input          : u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiLCDDmaEnable(u8 en)
{
	if(en)
	{
		R_CSI_INTCLR    = (1<<CSI_LBUFF_ERR)| //lbuff err
                          (1<<CSI_SEN_SIZE_ERR)| //
                          (1<<CSI_SEN_SIZE_STAT_INT)| //
                          (1<<CSI_LFRA_END_INT); //frame done
		R_CSI_INTEN    |= (1<<CSI_INT_EN)|//int en
                          (1<<CSI_LINE_SCALER_ERR)|//line scaler err int en
                          (1<<CSI_LBUFF_ERR)| //lbuff err
                          (1<<CSI_SEN_SIZE_ERR)| //
                          (1<<CSI_SEN_SIZE_STAT_INT)| //
                          (1<<CSI_LFRA_END_INT); //frame done
        R_CSI_DMA_MODE |= CSI_OUTPUT_LCDEN;//ldma en
	}
	else
	{
		R_CSI_DMA_MODE &=~CSI_OUTPUT_LCDEN;//ldma en

		R_CSI_INTEN  &=~((1<<CSI_LINE_SCALER_ERR)|//line scaler err int en
                        (1<<CSI_LBUFF_ERR)| //lbuff err int en
                        (0<<CSI_SEN_SIZE_ERR)| //sensor output size err int en
                        (0<<CSI_SEN_SIZE_STAT_INT)| //sensor output size statistics int en
                        (1<<CSI_LFRA_END_INT)); //frame done
		R_CSI_INTCLR  = (1<<CSI_LINE_SCALER_ERR)|//line scaler err pending
                        (1<<CSI_LBUFF_ERR)| //lbuff err
                        (1<<CSI_SEN_SIZE_ERR)| //
                        (1<<CSI_SEN_SIZE_STAT_INT)| //
                        (1<<CSI_LFRA_END_INT); //frame done
	}
}
/*******************************************************************************
* Function Name  : ax32xx_csiLCDDmaKick
* Description    : csi output for lcd control enable(manual mode)
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiLCDDmaKick(void)
{
    if(R_CSI_DMA_MODE & CSI_OUTPUT_LDMAMANEN)
    {
        R_CSI_DMA_MODE |= CSI_OUTPUT_LCDEN;
    }
}
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
void ax32xx_csiLCDScaler(u16 src_w,u16 src_h,u16 tar_w,u16 tar_h,u16 sx,u16 sy,u16 ex,u16 ey,u16 stride,u32 * anti_lsawtooth)
{
	if(((ex-sx)<tar_w)|| ((ey-sy)<tar_h)) // scaler can not do scaler up
		return ;
	R_CSI_LCD_CROPH = (ex<<16)|sx;
	R_CSI_LCD_CROPV = (ey<<16)|sy;
	
	ax32xx_ispModeSet(ISP_MODE_LCDCROP,1);
	
	R_CSI_SCALER_SIZE = (tar_h<<16)|tar_w;
	R_CSI_SCALER_WSTEP = ((ex-sx)<<16)/(tar_w>>1);
	R_CSI_SCALER_HSTEP = ((ey-sy)<<16)/(tar_h>>1);
	
    static u32 configed = 0;
    if(!configed)
    {
        R_CSI_LCD_SCALER_WTABLE0 = anti_lsawtooth[0];
        R_CSI_LCD_SCALER_WTABLE1 = anti_lsawtooth[1];
        R_CSI_LCD_SCALER_WTABLE2 = anti_lsawtooth[2];
        R_CSI_LCD_SCALER_WTABLE3 = anti_lsawtooth[3];
        R_CSI_LCD_SCALER_WTABLE4 = anti_lsawtooth[4];
        R_CSI_LCD_SCALER_WTABLE5 = anti_lsawtooth[5];
        R_CSI_LCD_SCALER_WTABLE6 = anti_lsawtooth[6];
        R_CSI_LCD_SCALER_WTABLE7 = anti_lsawtooth[7];
        R_CSI_LCD_SCALER_WTABLE8 = anti_lsawtooth[8];
        R_CSI_LCD_SCALER_WTABLE9 = anti_lsawtooth[9];
        R_CSI_LCD_SCALER_WTABLE10= anti_lsawtooth[10];
        R_CSI_LCD_SCALER_WTABLE11= anti_lsawtooth[11];
        R_CSI_LCD_SCALER_WTABLE12= anti_lsawtooth[12];
        R_CSI_LCD_SCALER_WTABLE13= anti_lsawtooth[13];
        R_CSI_LCD_SCALER_WTABLE14= anti_lsawtooth[14];
        R_CSI_LCD_SCALER_WTABLE15= anti_lsawtooth[15];//*/
        		
		R_CSI_LCD_SCALER_HTABLE0 = anti_lsawtooth[16];
        R_CSI_LCD_SCALER_HTABLE1 = anti_lsawtooth[17];
        R_CSI_LCD_SCALER_HTABLE2 = anti_lsawtooth[18];
        R_CSI_LCD_SCALER_HTABLE3 = anti_lsawtooth[19];
        R_CSI_LCD_SCALER_HTABLE4 = anti_lsawtooth[20];
        R_CSI_LCD_SCALER_HTABLE5 = anti_lsawtooth[21];
        R_CSI_LCD_SCALER_HTABLE6 = anti_lsawtooth[22];
        R_CSI_LCD_SCALER_HTABLE7 = anti_lsawtooth[23];
        configed = 1;
    }
	
    R_CSI_LDMA_STRIDE = stride;
    
//	R_CSI_SCALER_CTRL = (1<<2)|1;	// ls_en , ls_ks
	ax32xx_ispModeSet(ISP_MODE_LSEN,1);
	R_ISPCTRL |= ISP_CTRL_LS_KS;
}
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
void ax32xx_csiScaler(u16 src_w,u16 src_h,u16 tar_w,u16 tar_h,u16 sx,u16 sy,u16 ex,u16 ey)
{
	//R_CSI_DMA_MODE &= ~CSI_OUTPUT_MJPGEN;//�ر�hv sacle
	//ISPMODE&= ~BIT(19)|BIT(18); //SD_EN , wcrop_en
	//R_CSI_DMA_MODE &= ~CSI_OUTPUT_WIFIEN;//�ر�hv sacle
	ax32xx_ispModeSet(ISP_MODE_JSCALER_SEL,0); // disable csi to mjpeg scaler
	if((tar_w<=src_w) || (tar_h<=src_h))
	{
		if((sx > src_w) || (ex > src_w)){
			return;	
		}
		if((sy > src_h) || (ey > src_h)){
			return;	
		}		
		//R_CSI_LCD_CROPH = (ex<<16)|sx;
		//R_CSI_LCD_CROPV = (ey<<16)|sy;
		CSI_I2J_HCROP = (ex<<16)|sx;
		CSI_I2J_VCROP = (ey<<16)|sy;
		R_CSI_SCALER_SIZE = (tar_h<<16)|tar_w;
		R_CSI_SCALER_WSTEP = (src_w<<16)/(tar_w>>1);
		R_CSI_SCALER_HSTEP = (src_h<<16)/(tar_h>>1);	
//		R_CSI_SCALER_CTRL = (1<<2)|1;
		//R_ISPMODE |= ISP_MODE_LSEN;
		
		ax32xx_ispModeSet(ISP_MODE_JPEGCROP,1); //enable  csi chop
		//ax32xx_ispModeSet(ISP_MODE_JSCALER_SEL,1); //enable  csi to mjpeg scaler
		//ax32xx_ispModeSet(ISP_MODE_LSEN,1);     //enable line scaler
		//R_ISPCTRL |= ISP_CTRL_LS_KS;            //kick line scaler
		
		//R_CSI_DMA_MODE |= CSI_OUTPUT_MJPGEN;
	}
}
bool ax32xx_hvScaler(u32 src_w,  u32 src_h, u32 obj_w, u32 obj_h)
{
#if 1
	u8 wsacle,hsacle;
	if((src_w < obj_w) || (src_h < obj_h))
		return false;
	
	if((src_w / obj_w) == 2){ //w:2:1
		wsacle = 1;
	}
	else if((src_w / obj_w) == 4){//w:4:1
		wsacle = 3;
	}
	else{
		return false;	
	}
	
	if((src_h/obj_h) == 2){//h:2:1
		hsacle = 1;
	}
	else if((src_h/obj_h) == 4){//h:4:1
		hsacle = 3;
	}
	else if(((src_h*2/obj_h) == 3) && ((obj_h % 3) == 0)){//h:3:2
		hsacle = 0;
	}
	else if(((src_h/obj_h) == 3) && ((obj_h % 3) == 0)){//h:3:1
		hsacle = 2;
	}
	else{
		return false;	
	}
	debg("wsacle,hsacle:%x,%x",wsacle,hsacle);
	{
		//R_CSI_DMA_MODE &= ~CSI_OUTPUT_MJPGEN;//关闭hv sacle
		R_CSI_DMA_MODE &= ~CSI_OUTPUT_WIFIEN;
		//CSI_I2W_HCROP &= ~(3<<11);
		//CSI_I2W_VCROP &= ~(3<<10);
	
		CSI_I2W_HCROP = (obj_w << 16) //end
						|((wsacle & 0x03)<<11) //Horizontal Scale-Down steps 
						|(0 << 0);	//start
						
		CSI_I2W_VCROP = (obj_h << 16) //end
						|((hsacle & 0x03) << 10) //Vertical Scale-Down steps  
						|(0 << 0);	//start
		ISPMODE|= BIT(19)/*|BIT(18)*/; //SD_EN , wcrop_en
		
					;
		//R_CSI_DMA_MODE |= CSI_OUTPUT_WIFIEN;//打开hv sacle
		debg("csi_hv_sacle... \n");
	}
	return true;
#endif
}
/*******************************************************************************
* Function Name  : ax32xx_csiScalerCheck
* Description    : csi sclaer to lcd check
* Input          : none
* Output         : none
* Return         : 1 : scaler done
*******************************************************************************/
s32 ax32xx_csiScalerCheck(void)
{
	if(R_ISPCTRL&ISP_CTRL_LS_KS)
		return 0;
	else
		return 1;
}
/*******************************************************************************
* Function Name  : ax32xx_csiOutputSet
* Description    : csi output mode set
* Input          : u32 mode : mode.CSI_OUTPUT_E
                      u8 en      : 0-disable,1-enable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiOutputSet(u32 mode,u8 en)
{
	if(en)
		R_CSI_DMA_MODE |= mode;
	else
		R_CSI_DMA_MODE &= ~mode;
}
/*******************************************************************************
* Function Name  : ax32xx_csiEnable
* Description    : csi enable set
* Input          : u8 en : 0-disable,1-enable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_csiEnable(u8 en)
{
debg("close!\n");
	if(en)
	{
		R_CSI_INTEN |= (1<<CSI_INT_EN);
		R_CSI_MODE  |= CSI_MODE_ENABLE;
		ax32xx_intEnable(IRQ_CSI,1);
	}
	else
	{
		R_CSI_INTEN &= ~(1<<CSI_INT_EN);
		R_CSI_MODE  &= ~CSI_MODE_ENABLE;
		ax32xx_intEnable(IRQ_CSI,0);
	}
}
/*
void ax32xx_csiPrint(void)
{
	deg_Printf("R_CSI_MODE = %x\n",R_CSI_MODE);
	deg_Printf("R_CSI_INTEN = %x\n",R_CSI_INTEN);
	deg_Printf("R_CSI_INTCLR = %x\n",R_CSI_INTCLR);
	deg_Printf("R_CSI_INTSTATUS = %x\n",R_CSI_INTSTATUS);
	deg_Printf("R_CSI_SIZE = %x\n",R_CSI_SIZE);
	deg_Printf("R_CSI_MODE2 = %x\n",R_CSI_MODE2);
	deg_Printf("R_CSI_INPUT_ADDR = %x\n",R_CSI_INPUT_ADDR);
	deg_Printf("R_CSI_TEST_SIZE = %x\n",R_CSI_TEST_SIZE);
	deg_Printf("R_CSI_TEST_SYNC = %x\n",R_CSI_TEST_SYNC);
	deg_Printf("R_CSI_MJPEG_SIZE = %x\n",R_CSI_MJPEG_SIZE);
	deg_Printf("R_CSI_MJPEG_YADDR = %x\n",R_CSI_MJPEG_YADDR);
	deg_Printf("R_CSI_MJPEG_UVADDR = %x\n",R_CSI_MJPEG_UVADDR);
	deg_Printf("R_CSI_LCD_YADDR = %x\n",R_CSI_LCD_YADDR);
	deg_Printf("R_CSI_LCD_UVADDR = %x\n",R_CSI_LCD_UVADDR);
	deg_Printf("R_CSI_LCD_CROPH = %x\n",R_CSI_LCD_CROPH);
	deg_Printf("R_CSI_LCD_CROPV = %x\n",R_CSI_LCD_CROPV);
	deg_Printf("R_CSI_DMA_MODE = %x\n",R_CSI_DMA_MODE);


}

*/

void ax32xx_csiIOConfigUinit(void)
{
   hal_gpioInit(GPIO_PE,GPIO_PIN13|GPIO_PIN15|GPIO_PIN2|GPIO_PIN3|GPIO_PIN4|GPIO_PIN5|GPIO_PIN6|GPIO_PIN7|GPIO_PIN8|GPIO_PIN9|GPIO_PIN10|GPIO_PIN11|GPIO_PIN12|GPIO_PIN14,GPIO_INPUT,GPIO_PULL_DOWN);
}