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
* File Name   : hal_lcd.c
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

#include "../inc/hal.h"
#include "../../ax32_platform_demo/application.h"

enum buffer_pip_sta_e {
    LB_IDLE   = (0<<0),
    LB_READY0 = (1<<2),
    LB_READY1 = (1<<3),
};

//lcd video-layer input channel
typedef struct _lcd_vch_s {
    u8 status;            //通道使能
	u8 config_enable;	  //1:使能配置
    u8 src;               //数据来源
    u8 layer;
	u16 x,y; //video起始坐标作为原点
	u16 h,w;
    struct _lcd_vch_s * bak;
} lcd_vch_t;

typedef struct _lcd_config_s 
{
    u8 pip_en;
    u8 need_rotate;
    lcd_vch_t * vchs[4];
    disp_frame_t * display_preset;
    disp_frame_t * displaying;
    disp_frame_t * csi_using;//csi ldma正在使用的buffer,该buffer将作为rotate/DE的输入,
                             //所以由rotate/DE负责free
    lcddev_t *  p_lcddev;
	// for LDMA crop
	u16 crop_sx,crop_ex,crop_sy,crop_ey;

} lcd_config_t;

static lcd_config_t lcd_config;
static lcd_vch_t vch_backup[2] = {
    {
    .status = VCH_DIS,
	.config_enable = 0,
    .src = VCH_ISP,
    .layer = VCH_TOP,
    .bak = NULL,
    },
    {
    .status = VCH_DIS,
	.config_enable = 0,
    .src = VCH_JPG,
    .layer = VCH_BOT,
    .bak = NULL,
    },
};
static lcd_vch_t vch[2] = {
    {
    .status = VCH_DIS,
	.config_enable = 0,
    .src = VCH_ISP,
    .layer = VCH_TOP,
    .bak = &vch_backup[0],
    },
    {
    .status = VCH_DIS,
	.config_enable = 0,
    .src = VCH_JPG,
    .layer = VCH_BOT,
    .bak = &vch_backup[1],
    },
};
/*******************************************************************************
* Function Name  : hal_lcdMemoryInit
* Description    : hardware layer ,lcd memory inital
* Input          : lcddev_t * p_lcddev : lcd  node
* Output         : lcddev_t *p_lcddev
* Return         : none
*******************************************************************************/
static void hal_lcdMemoryInit(lcddev_t *p_lcddev)
{
	INT16U lcdW=0,lcdH=0;
    u32 osd_w = (p_lcddev->osd_w + 3) & ~3;
    u32 osd_h = (p_lcddev->osd_h + 3) & ~3;
	hal_lcdGetResolution(&lcdW,&lcdH);
    hal_dispframeInit(lcdW,lcdH,
                      osd_w,osd_h,
                      p_lcddev->scan_mode);
}
/*******************************************************************************
* Function Name  : hal_lcdParaRGB
* Description    : hardware layer ,rgb type initial paramater
* Input          : lcddev_t * p_lcddev : lcd  node
* Output         : None
* Return         : none
*******************************************************************************/
static void hal_lcdParaRGB(lcddev_t * p_lcddev,void * para_tab)
{
	u32 (*tab)[2] = para_tab;
	
	if(tab)
	{
		ax32xx_lcdSPIMode(p_lcddev->spi_cpol,p_lcddev->spi_cpha,p_lcddev->spi_order,p_lcddev->spi_bits);
		ax32xx_lcdSPIInit();
		
		for(;tab[0][0] != LCD_TAB_END;tab++)
		{
			if(tab[0][0] == SPI_DAT)
				ax32xx_lcdSPISendData(tab[0][1]);
			else if(tab[0][0] == DELAY_MS)
				ax32xx_sysCpuMsDelay(tab[0][1]);
		}
		
		ax32xx_lcdSPIUninit();
	}
}
/*******************************************************************************
* Function Name  : hal_lcdParaMCU
* Description    : hardware layer ,mcu type initial paramater
* Input          : lcddev_t * p_lcddev : lcd  node
* Output         : None
* Return         : none
*******************************************************************************/
static void hal_lcdParaMCU(lcddev_t * p_lcddev,void * para_tab)
{
	u32 (*tab)[2] = para_tab;
    
	if(tab==NULL)
		return ;
	if(p_lcddev->bus_cmd==0)
    {
		for(;tab[0][0] != LCD_TAB_END;tab++)
		{
			if(tab[0][0] == MCU_CMD)
				ax32xx_lcdMcuSendCmd(tab[0][1]);
			else if(tab[0][0] == MCU_DAT)
				ax32xx_lcdMcuSendData(tab[0][1]);
			else if(tab[0][0] == DELAY_MS)
				ax32xx_sysCpuMsDelay(tab[0][1]);
		}

		//=====mcu lcd test=====
		/*
		u16 i,j;
		deg_Printf("====mcu lcd test====\n");
		for(j=0;j<320;j++)
		{
			for(i=0;i<120;i++)
			{
				ax32xx_lcdMcuSendData(0xff);
				ax32xx_lcdMcuSendData(0x00);
				ax32xx_lcdMcuSendData(0x00);
			}
			for(i=120;i<240;i++)
			{
				ax32xx_lcdMcuSendData(0x00);
				ax32xx_lcdMcuSendData(0xff);
				ax32xx_lcdMcuSendData(0x00);
			}
		}

		u32 time=XOSTimeGet();
		while(1)
		{
			hal_wdtClear();
			if(XOSTimeGet()>time+8000)
			{
				break;
			}
		}
		*/
		//==end mcu lcd test====
	}
	else
	{
		for(;tab[0][0] != LCD_TAB_END;tab++)
		{
			if(tab[0][0] == MCU_CMD)
				ax32xx_lcdMcuSendCmd16(tab[0][1]);
			else if(tab[0][0] == MCU_DAT)
				ax32xx_lcdMcuSendData16(tab[0][1]);
			else if(tab[0][0] == DELAY_MS)
				ax32xx_sysCpuMsDelay(tab[0][1]);
		}
	}
}
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
void hal_lcdSetBufYUV(disp_frame_t * buffer,u8 y,u8 u,u8 v)
{
    if(buffer)
    {
        u32 y_size  = buffer->h * buffer->stride;
        u32 uv_size = y_size / 2;
        memset(buffer->y_addr,y,y_size);
        ax32xx_sysDcacheWback((u32)buffer->y_addr,y_size);
        memset(buffer->uv_addr,u,uv_size);
        ax32xx_sysDcacheWback((u32)buffer->uv_addr,uv_size);
    }
}

ALIGNED(32) static u8 lcdGamma[3][512];
ALIGNED(32) static u8 lcdContra[256];
/*******************************************************************************
* Function Name  : hal_lcd_isp_init
* Description    : hardware layer ,initial lcd panel
* Input          : lcddev_t *p_lcddev : lcd op node
* Output         : None
* Return         : None
*******************************************************************************/
static void hal_lcd_isp_init(lcddev_t *p_lcddev)
{
	if(p_lcddev == NULL)
		return;
	ax32xx_deSetCSC(p_lcddev->brightness,p_lcddev->saturation,p_lcddev->contrast);
	ax32xx_deSetGAMA((u32)lcdGamma[0],(u32)lcdGamma[1],(u32)lcdGamma[2]);
}
/*******************************************************************************
* Function Name  : lcd_isp_res_load
* Description    : lcd isp_resource load
* Input          : lcddev_t *p_lcddev : lcd op node
* Output         : none                                            
* Return         : none
*******************************************************************************/
static void lcd_isp_res_load(lcddev_t *p_devLcd){
	u32 i=0,j=0,len,tab_nums;
    _Lcd_Header_ * lcd_header_get(void);
	_Lcd_Header_ * p_devLcdHeader = lcd_header_get();
	if(p_devLcd) {  // load gamma table
		len = p_devLcdHeader->lcd_contra_tab_unit;
		tab_nums = p_devLcdHeader->lcd_contra_tab_len / len;
		if(p_devLcd->contra_index<tab_nums)
			hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_contra_tab_adr+p_devLcd->contra_index*len),(u32)lcdContra,len);
		len = p_devLcdHeader->lcd_gamma_tab_unit;
		tab_nums = p_devLcdHeader->lcd_gamma_tab_len / len;
		if(p_devLcd->gamma_red < tab_nums)
			hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_red*len),(u32)lcdGamma[0],len);
		if(p_devLcd->gamma_green < tab_nums)
			hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_green*len),(u32)lcdGamma[1],len);
		if(p_devLcd->gamma_blue < tab_nums)
			hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_blue*len),(u32)lcdGamma[2],len);
		for(i=0;i<3;i++){
			for(j=0;j<p_devLcdHeader->lcd_gamma_tab_unit;j++){
				s32 temp;
				temp = lcdGamma[i][j];
				temp = temp + (-p_devLcd->brightness)*(temp+1) / 256;
				temp = clip(temp,0,255);
				lcdGamma[i][j] = lcdContra[temp];
			}
		}
	}
}

/*******************************************************************************
* Function Name  : hal_lcd_vde_cfg
* Description    : config lcd's vde para
* Input          : lcddev_t *p_devLcd
* Output         : none                                            
* Return         : none
*******************************************************************************/
void hal_lcd_vde_cfg(lcddev_t *p_lcddev)
{
	if(p_lcddev == NULL)
		return;
	ax32xx_deSetCSC(p_lcddev->brightness,p_lcddev->saturation,p_lcddev->contrast);
}

/*******************************************************************************
* Function Name  : hal_lcd_gamma_cfg
* Description    : config lcd's gamma and contrast.use the function after lcdContra lcdGamma 
* Input          : lcddev_t *p_devLcd
* Output         : none                                            
* Return         : none
*******************************************************************************/
void hal_lcd_gamma_cfg(_Lcd_Header_ * p_devLcdHeader,lcddev_t *p_lcddev,lcd_gamma_t *p_lcd_gamma)
{
	u32 len,tab_nums;
	if((NULL == p_devLcdHeader) || (NULL == p_lcddev) || (NULL == p_lcd_gamma))
		return;
	len = p_devLcdHeader->lcd_contra_tab_unit;
	tab_nums = p_devLcdHeader->lcd_contra_tab_len / len;
	if((p_lcddev->contra_index<tab_nums) && (p_lcddev->contra_index != p_lcd_gamma->contra_index)){
		p_lcddev->contra_index = p_lcd_gamma->contra_index;
		hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_contra_tab_adr+p_lcddev->contra_index*len),(u32)lcdContra,len);
	}
	len = p_devLcdHeader->lcd_gamma_tab_unit;
	tab_nums = p_devLcdHeader->lcd_gamma_tab_len / len;
	if((p_lcddev->gamma_red < tab_nums) && (p_lcddev->gamma_red != p_lcd_gamma->gamma_red)){
		p_lcddev->gamma_red = p_lcd_gamma->gamma_red;
		debg("red:%d\n",p_lcddev->gamma_red);
		hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_lcddev->gamma_red*len),(u32)lcdGamma[0],len);
	}
	if((p_lcddev->gamma_green < tab_nums) && (p_lcddev->gamma_green != p_lcd_gamma->gamma_green)){
		p_lcddev->gamma_green = p_lcd_gamma->gamma_green;
		debg("green:%d\n",p_lcddev->gamma_green);
		hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_lcddev->gamma_green*len),(u32)lcdGamma[1],len);
	}
	if((p_lcddev->gamma_blue < tab_nums) && (p_lcddev->gamma_blue != p_lcd_gamma->gamma_blue)){
		p_lcddev->gamma_blue = p_lcd_gamma->gamma_blue;
		debg("blue:%d\n",p_lcddev->gamma_blue);
		hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_lcddev->gamma_blue*len),(u32)lcdGamma[2],len);
	}
	ax32xx_deSetGAMA((u32)lcdGamma[0],(u32)lcdGamma[1],(u32)lcdGamma[2]);
}

/*******************************************************************************
* Function Name  : usb_lcd_cfg
* Description    : usb_lcd_cfg
* Input          : lcddev_t *new_lcddev,lcddev_t *p_devLcd
* Output         : none                                            
* Return         : none
*******************************************************************************/
void usb_lcd_cfg(lcddev_t *new_lcddev,lcddev_t *p_devLcd){
	u32 i=0,j=0,len,tab_nums;
    _Lcd_Header_ * lcd_header_get(void);
	_Lcd_Header_ * p_devLcdHeader = lcd_header_get();
	if(p_devLcd) {  // load gamma table
		len = p_devLcdHeader->lcd_contra_tab_unit;
		tab_nums = p_devLcdHeader->lcd_contra_tab_len / len;
		if(new_lcddev->contra_index != p_devLcd->contra_index){
			if(new_lcddev->contra_index<tab_nums){
				p_devLcd->contra_index = new_lcddev->contra_index;
				hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_contra_tab_adr+p_devLcd->contra_index*len),(u32)lcdContra,len);
			}
		}
		len = p_devLcdHeader->lcd_gamma_tab_unit;
		tab_nums = p_devLcdHeader->lcd_gamma_tab_len / len;
		if(new_lcddev->gamma_red != p_devLcd->gamma_red){
			if(new_lcddev->gamma_red < tab_nums){
				hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_red*len),(u32)lcdGamma[0],len);
				p_devLcd->gamma_red = new_lcddev->gamma_red;
			}
			
		}
		
		if(new_lcddev->gamma_green != p_devLcd->gamma_green){
			if(new_lcddev->gamma_green < tab_nums){
				hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_green*len),(u32)lcdGamma[1],len);
				p_devLcd->gamma_green = new_lcddev->gamma_green;
			}
			
		}
		if(new_lcddev->gamma_blue != p_devLcd->gamma_blue){
			if(new_lcddev->gamma_blue < tab_nums){
				hal_spiFlashRead(LCD_RES_FLASH_ADDR(p_devLcdHeader->lcd_gamma_tab_adr+p_devLcd->gamma_blue*len),(u32)lcdGamma[2],len);
				p_devLcd->gamma_blue = new_lcddev->gamma_blue;
			}
			
		}
		p_devLcd->contrast = new_lcddev->contrast;
		p_devLcd->saturation = new_lcddev->saturation;
		p_devLcd->brightness = new_lcddev->brightness;
		for(i=0;i<3;i++){
			for(j=0;j<p_devLcdHeader->lcd_gamma_tab_unit;j++){
				s32 temp;
				temp = lcdGamma[i][j];
				temp = temp + (-p_devLcd->brightness)*(temp+1) / 256;
				temp = clip(temp,0,255);
				lcdGamma[i][j] = lcdContra[temp];
			}
		}
		hal_lcd_isp_init(p_devLcd);
	}
}
/*******************************************************************************
* Function Name  : lcd_isp_config
* Description    : lcd isp config
* Input          : lcddev_t *p_lcddev : lcd op node
* Output         : none                                            
* Return         : none
*******************************************************************************/
void lcd_isp_config(lcddev_t *p_lcddev){
	lcd_isp_res_load(p_lcddev);
	hal_lcd_isp_init(p_lcddev);
}
/*******************************************************************************
* Function Name  : hal_lcdTeMode1ISR
* Description    : TE or timer isr,JUST for te_mode == 1
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
static void hal_lcdTeMode1ISR(void)
{
    if(ax32xx_deGetUpdateStatus())
    {
		ax32xx_lcdKick();
    }
}
/*******************************************************************************
* Function Name  : hal_lcdFrameEndIsr
* Description    : lcdc frame done isr
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void hal_lcdFrameEndIsr(void)
{
	static u16 x=0,y=0,w=0,h=0;
    disp_frame_t * p_lcd_buffer = lcd_config.display_preset;
    
    if(p_lcd_buffer == lcd_config.displaying)
    {
        lcd_config.display_preset = NULL;
        return;
    }
    
    if(p_lcd_buffer)
    {
        if(p_lcd_buffer->posX!=x||p_lcd_buffer->posY!=y||p_lcd_buffer->w!=w||p_lcd_buffer->h!=h)
        {
        	x=p_lcd_buffer->posX;
			y=p_lcd_buffer->posY;
			w=p_lcd_buffer->w;
			h=p_lcd_buffer->h;
	        ax32xx_videoSetStride(p_lcd_buffer->stride,p_lcd_buffer->stride);
	    	ax32xx_videoSetScanMode(p_lcd_buffer->scan_mode);
			ax32xx_videoSetPosition(p_lcd_buffer->posX,p_lcd_buffer->posY);
			ax32xx_osdSetUpscaler(p_lcd_buffer->w,p_lcd_buffer->h,p_lcd_buffer->w,p_lcd_buffer->h,NULL);
			debg("frame[%d],x:%d,y:%d,w:%d,h:%d\n",p_lcd_buffer->id,p_lcd_buffer->posX,p_lcd_buffer->posY,p_lcd_buffer->w,p_lcd_buffer->h);
        }
        ax32xx_videoSetAddr((u32)p_lcd_buffer->_y_addr,(u32)p_lcd_buffer->_uv_addr,1);
		
        ax32xx_deUpdate();
        hal_dispframeFree(lcd_config.displaying);
        lcd_config.displaying = p_lcd_buffer;
        lcd_config.display_preset = NULL;
    }
}

/*******************************************************************************
* Function Name  : hal_lcdInit
* Description    : hardware layer ,initial lcd panel
* Input          : lcddev_t *p_lcddev : lcd op node
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
static s32 hal_lcdInit(lcddev_t *p_lcddev)
{
	INT16U lcdW=0,lcdH=0;
	if(p_lcddev == NULL)
		return -1;
#if HAL_CFG_EN_LCD > 0
	lcd_config.p_lcddev   = p_lcddev;
	lcd_config.need_rotate= (p_lcddev->scan_mode == LCD_ROTATE_90) || 
								(p_lcddev->scan_mode == LCD_ROTATE_270);
	hal_lcdGetResolution(&lcdW,&lcdH);
	if(!p_lcddev->video_w||!p_lcddev->video_h||p_lcddev->video_x>lcdW||p_lcddev->video_y>lcdH)
	{
		p_lcddev->video_x = 0;
		p_lcddev->video_y = 0;
		p_lcddev->video_w = lcdW;
	    p_lcddev->video_h = lcdH;
	}
    //if(!p_lcddev->vsrc_w) p_lcddev->vsrc_w = p_lcddev->video_w;
    //if(!p_lcddev->vsrc_h) p_lcddev->vsrc_h = p_lcddev->video_h;
	if(!p_lcddev->osd_w||!p_lcddev->osd_h||p_lcddev->osd_x>lcdW||p_lcddev->osd_y>lcdH)
	{
		p_lcddev->osd_x = p_lcddev->video_x;
		p_lcddev->osd_y = p_lcddev->video_y;
		p_lcddev->osd_w = p_lcddev->video_w;
    	p_lcddev->osd_h = p_lcddev->video_h;
	}
	if(p_lcddev->video_x+p_lcddev->video_w>lcdW)
		p_lcddev->video_w=lcdW-p_lcddev->video_x;
	if(p_lcddev->video_y+p_lcddev->video_h>lcdH)
		p_lcddev->video_h=lcdH-p_lcddev->video_y;
	if(p_lcddev->osd_x+p_lcddev->osd_w>lcdW)
		p_lcddev->osd_w=lcdW-p_lcddev->osd_x;
	if(p_lcddev->osd_y+p_lcddev->osd_h>lcdH)
		p_lcddev->osd_h=lcdH-p_lcddev->osd_y;
    hal_lcdMemoryInit(p_lcddev);

    lcd_config.pip_en     = 0;    
    lcd_config.displaying = hal_dispframeMalloc(FTYPE_VIDEO_DISPLAY);
	lcd_config.vchs[VCH_ISP] = &vch[0];
	lcd_config.vchs[VCH_JPG] = &vch[1];
	lcd_config.vchs[VCH_TOP] = &vch[0];
	lcd_config.vchs[VCH_BOT] = &vch[1];
    //isp video channle
    lcd_config.vchs[VCH_ISP]->x = 
    lcd_config.vchs[VCH_ISP]->y = 0;
    lcd_config.vchs[VCH_ISP]->w = p_lcddev->video_w;
    lcd_config.vchs[VCH_ISP]->h = p_lcddev->video_h;
    lcd_config.vchs[VCH_ISP]->bak->x = 
    lcd_config.vchs[VCH_ISP]->bak->y = 0;
    lcd_config.vchs[VCH_ISP]->bak->w = p_lcddev->video_w;
    lcd_config.vchs[VCH_ISP]->bak->h = p_lcddev->video_h;
    lcd_config.vchs[VCH_ISP]->bak->status = 1;
    lcd_config.vchs[VCH_ISP]->bak->bak = lcd_config.vchs[VCH_ISP]->bak;
    lcd_config.vchs[VCH_JPG]->bak->bak = lcd_config.vchs[VCH_JPG]->bak;
    lcd_config.vchs[VCH_ISP]->bak->config_enable = 1;
    lcd_config.vchs[VCH_JPG]->bak->config_enable = 1;

    lcd_config.crop_sx = 0;
    lcd_config.crop_ex = p_lcddev->video_w;
    lcd_config.crop_sy = 0;
    lcd_config.crop_ey = p_lcddev->video_h;

    ax32xx_deWait();
	ax32xx_deInit(p_lcddev->screen_w,p_lcddev->screen_h);  // initial de
	ax32xx_deSetBackground(0,0,0);   //black
	ax32xx_videoSetDither(p_lcddev->red_width,p_lcddev->green_width,p_lcddev->blue_width);
	ax32xx_videoSetPosition(0,0);
	ax32xx_videoSetAddr((u32)lcd_config.displaying->_y_addr,(u32)lcd_config.displaying->_uv_addr,1);
	ax32xx_videoSetStride(lcd_config.displaying->stride,lcd_config.displaying->stride);
    ax32xx_videoSetScanMode(lcd_config.displaying->scan_mode);
	ax32xx_videoSetScale(0x00,0x00,0x40,0x00);
	ax32xx_osdSetUpscaler(lcd_config.displaying->w,lcd_config.displaying->h,lcd_config.displaying->w,lcd_config.displaying->h,NULL);
    
	ax32xx_videoEnable(1);
//	lcd_isp_config(p_lcddev);
//	ax32xx_deSet_DE_CCM(p_lcddev->de_ccm);
//	ax32xx_deSet_DE_SAJ(p_lcddev->de_saj);
    ax32xx_deUpdate();
	if(p_lcddev->lcd_bus_type==1) // mcu
	{
		debg("[LCD mcu init]\n");
		ax32xx_lcdMCUIOConfig(p_lcddev->bus_width,p_lcddev->te_mode!=0xff);
        ax32xx_lcdInit();
        ax32xx_lcdIRQEnable(LCD_IRQ_DEND,1);
        ax32xx_lcdPanelMode(1);
		ax32xx_lcdPreLineSet(7);
        ax32xx_lcdSignalSet(0,0,1,0,0,1,0,0);
		ax32xx_lcdBusWidth(p_lcddev->bus_width);  // must after ax32xx_lcdSignalSet
        ax32xx_lcdClkSet(/*p_lcddev->pclk_div*/56);	// low speed for lcd init
		ax32xx_lcdSyncSet(0,0,0,0);
		ax32xx_lcdDESignalSet(0,0,0,0);
		ax32xx_lcdPositionSet(0,0);
		ax32xx_lcdResolutionSet(p_lcddev->screen_w,p_lcddev->screen_h);
		ax32xx_lcdWindowSizeSet(p_lcddev->screen_w-1,p_lcddev->screen_h-1);
        ax32xx_lcdDataModeSet(p_lcddev->lcd_data_mode,p_lcddev->lcd_data_mode1,p_lcddev->even_order,p_lcddev->odd_order);
        ax32xx_lcdClkNumberSet(p_lcddev->screen_w*p_lcddev->screen_h*p_lcddev->clk_per_pixel-1);
        ax32xx_lcdEndLineSet(0,0);
        ax32xx_lcdEnable(1);

		hal_lcdParaMCU(p_lcddev,p_lcddev->init_table);

		ax32xx_lcdClkSet(p_lcddev->pclk_div);
		ax32xx_sysCpuMsDelay(1);


        if(p_lcddev->te_mode == 0)//just interrupt,no auto kick TX
        {
            ax32xx_lcdTeMode(0,1);
            ax32xx_lcdIRQEnable(LCD_IRQ_TE,1);
        }
        else if(p_lcddev->te_mode == 1)//auto kick TX
        {
            ax32xx_lcdISRRegister(LCD_IRQ_TE,hal_lcdTeMode1ISR);
            ax32xx_lcdIRQEnable(LCD_IRQ_TE,1);
            ax32xx_lcdTeMode(0,1);
        }
        else if(p_lcddev->te_mode == 0xff)
        {
            //TODO:
            //使用定时器中断，调用hal_lcdTeMode1ISR，每秒50~60次即可
            ax32xx_lcdKick();
        }
	}
    else
    {
        ax32xx_lcdRGBIOConfig(p_lcddev->bus_width,p_lcddev->de_inv != 0xff);

		hal_lcdParaRGB(p_lcddev,p_lcddev->init_table);
        ax32xx_lcdInit();
		ax32xx_lcdIRQEnable(LCD_IRQ_FEND,1);
        ax32xx_lcdPanelMode(0);
		ax32xx_lcdPreLineSet(7);
		ax32xx_lcdSignalSet(p_lcddev->vlw,p_lcddev->pclk_inv&1,p_lcddev->de_inv&1,p_lcddev->hs_inv&1,p_lcddev->vs_inv,p_lcddev->de_inv != 0xff,p_lcddev->hs_inv != 0xff,p_lcddev->vs_inv != 0xff);
        ax32xx_lcdBusWidth(p_lcddev->bus_width); // must after ax32xx_lcdSignalSet
        if(p_lcddev->init_table)
			ax32xx_lcdBusEnable(0x01,0);  // disable d0
        ax32xx_lcdClkSet(p_lcddev->pclk_div);
		ax32xx_lcdSyncSet(0,p_lcddev->hlw,0,p_lcddev->vlw);
		u32 temp1,temp2,temp3,temp4;
		temp1 = p_lcddev->hlw + p_lcddev->hbp * p_lcddev->clk_per_pixel;
		temp2 = p_lcddev->vlw + p_lcddev->vbp;
		temp3 = p_lcddev->screen_w * p_lcddev->clk_per_pixel;
		temp4 = p_lcddev->screen_h - 1;
		ax32xx_lcdDESignalSet(temp1,temp1+temp3,temp2,temp2+temp4);
        ax32xx_lcdPositionSet(temp1,temp2);
		ax32xx_lcdResolutionSet(p_lcddev->screen_w,p_lcddev->screen_h);
		ax32xx_lcdWindowSizeSet(temp1+temp3+p_lcddev->hfp * p_lcddev->clk_per_pixel,temp2+temp4+p_lcddev->vlw + p_lcddev->vfp);
        ax32xx_lcdDataModeSet(p_lcddev->lcd_data_mode,p_lcddev->lcd_data_mode1,p_lcddev->even_order,p_lcddev->odd_order);
        ax32xx_lcdClkNumberSet(0);
        ax32xx_lcdEndLineSet(temp2+p_lcddev->screen_h,temp2+p_lcddev->screen_h+2);
		ax32xx_lcdEnable(1);
	}
	
    hal_lcdFrameEndCallBack(hal_lcdFrameEndIsr);
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : hal_lcdLCMPowerOff
* Description    : lcd module power off sequence
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdLCMPowerOff(void)
{
    if(lcd_config.p_lcddev && lcd_config.p_lcddev->uninit_table)
    {
        if(lcd_config.p_lcddev->lcd_bus_type)
        {
            ax32xx_lcdEnable(0);
            ax32xx_lcdEnable(1);
            hal_lcdParaMCU(lcd_config.p_lcddev,lcd_config.p_lcddev->uninit_table);
        }
        else
            hal_lcdParaRGB(lcd_config.p_lcddev,lcd_config.p_lcddev->uninit_table);
            
    }
}
/*******************************************************************************
* Function Name  : hal_lcdPreviewStart
* Description    : hardware layer ,lcd preview csi frame strat
* Input          : s16 width : sensor frame width
                      u16 height: sensor frame height
* Output         : None
* Return         : None
*******************************************************************************/
s32 hal_lcdPreviewStart(u16 width,u16 height)
{
#if HAL_CFG_EN_LCD
    if(!lcd_config.csi_using)
    {
        lcd_config.csi_using = hal_lcdGetIdleBuffer();
    }

	if(NULL==lcd_config.csi_using)
	{
		deg_Printf("lcd_config.csi_using null\n");
	}
    //TODO:能否沿用csi_using?
    disp_frame_t * p_lcd_buffer = lcd_config.csi_using;
	
	if(lcd_config.p_lcddev&&lcd_config.csi_using)
    	hal_dispframeVideoCfg(lcd_config.csi_using,lcd_config.p_lcddev->video_x,lcd_config.p_lcddev->video_y,lcd_config.p_lcddev->video_w,lcd_config.p_lcddev->video_h);
    if(lcd_config.p_lcddev)
    {
    	deg_Printf("lcdPrev: sx:%d sy:%d ex:%d ey:%\n",lcd_config.crop_sx,lcd_config.crop_sy,lcd_config.crop_ex,lcd_config.crop_ey);
        ax32xx_csiLCDScaler(width,height,
                            lcd_config.vchs[VCH_ISP]->bak->w,
                            lcd_config.vchs[VCH_ISP]->bak->h,
                            #if (0 == LCD_CROP_SCALE)
                            0,0,width,height,
                            #else
							lcd_config.crop_sx,
							lcd_config.crop_sy,
							lcd_config.crop_ex,
							lcd_config.crop_ey,
							#endif
                            p_lcd_buffer->stride,
                            (u32*)lcd_config.p_lcddev->lcd_lsawtooth.anti_lsawtooth);
    }
    ax32xx_csiOutputSet(CSI_OUTPUT_RELOAD,0);
    ax32xx_csiOutputSet(CSI_OUTPUT_LDMAMANEN,1);
    ax32xx_csiLCDFrameSet((u32)p_lcd_buffer->y_addr,(u32)p_lcd_buffer->uv_addr);

    int i = 0xfffff;
	AX32XX_WAIT(!ax32xx_csiScalerCheck(),i);
    ax32xx_csiLCDDmaEnable(1);
#endif
    
	return 0;
}
/*******************************************************************************
* Function Name  : hal_lcdPreviewStop
* Description    : hardware layer ,lcd preview csi frame stop
* Input          : none
* Output         : None
* Return         : 
*******************************************************************************/
s32 hal_lcdPreviewStop(void)
{
    ax32xx_csiLCDDmaEnable(0);
    ax32xx_pipCounterReset(PIP_SRC_ISP);
    
    hal_dispframeFree(lcd_config.csi_using);
    lcd_config.csi_using = NULL;
	return 0;	
}
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
s32 hal_lcdSetShowingSize(u16 srcw,u16 srch,u16 width,u16 height)
{
#if HAL_CFG_EN_LCD > 0		
//    if(lcd_config.config_enable == 0)
//	{
//		lcd_config.video_w = (width == 0xffff)?lcd_config.video_w:width;
//		lcd_config.video_h = (height == 0xffff)?lcd_config.video_h:height;
//		lcd_config.video_x = (lcd_config.screen_w - lcd_config.video_w) / 2;
//		lcd_config.video_y = (lcd_config.screen_h - lcd_config.video_h) / 2;

//		ax32xx_csiLCDScaler(srcw,srch,
//                            lcd_config.video_w,lcd_config.video_h,
//                            0,0,srcw,srch,
//                            lcd_config.lcd_buffer_stride);

//		lcd_config.config_enable = 1; // this setting will be set in csi frame end irq
//	}
#endif
	return 0;
}
/*******************************************************************************
* Function Name  : hal_lcdSetRoate
* Description    : hardware layer ,set lcd rotate degree
* Input          : u8 rotate : rotate degree
* Output         : None
* Return         : 
*******************************************************************************/
s32 hal_lcdSetRoate(u8 rotate)
{
//    if(rotate>=7)
//		return -1;
//	if((lcd_config.scan_mode != rotate))
//	{
//		lcd_config.scan_mode = rotate;
//		lcd_config.config_enable = 1;  // set in irq
//	}	

	return 0;
}
/*******************************************************************************
* Function Name  : hal_lcdSetVideoBuffer
* Description    : hardware layer ,set lcd display buffer
* Input          : disp_frame_t *p_lcd_buffer : buffer frame addr
* Output         : None
* Return         : None
*******************************************************************************/
static void hal_lcdSetVideoBuffer(disp_frame_t * p_lcd_buffer)
{
    ax32xx_intEnable(IRQ_LCDC,0);
    
    if(lcd_config.display_preset)
        hal_dispframeFree(lcd_config.display_preset);
        
    lcd_config.display_preset = p_lcd_buffer;
    
    if(lcd_config.p_lcddev->te_mode == 1)
    {
        //for te interrupt,kick send
        ax32xx_deUpdate();
    }
    
    ax32xx_intEnable(IRQ_LCDC,1);
}
/*******************************************************************************
* Function Name  : hal_lcdDealBuffer
* Description    : hardware layer ,set lcd display buffer
* Input          : disp_frame_t *p_lcd_buffer : buffer frame addr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdDealBuffer(disp_frame_t * p_lcd_buffer)
{
    if(p_lcd_buffer->scan_mode == LCD_ROTATE_90)
    {
        //get rotate dst buffer
        disp_frame_t * dst = hal_dispframeMalloc(FTYPE_VIDEO_DISPLAY);
        if(dst)
        {
            hal_rotateAdd(ROTATE_CH0,p_lcd_buffer,dst);
        }
        else
            hal_dispframeFree(p_lcd_buffer);
    }
    else
    {
        hal_lcdSetVideoBuffer(p_lcd_buffer);
    }
}
/*******************************************************************************
* Function Name  : hal_lcdGetShowingBuffer
* Description    : hardware layer ,get lcd displaying buffer
* Input          : none
* Output         : None
* Return         : disp_frame_t *p_lcd_buffer : buffer frame addr
*******************************************************************************/
disp_frame_t * hal_lcdGetShowingBuffer(void)
{
    disp_frame_t * res = NULL;
#if HAL_CFG_EN_LCD > 0
    res = lcd_config.displaying;
    if(!(res && (((u32)res->_y_addr & 0xffffff) == ax32xx_videoGetYAddr())))
        res = NULL;
        
    if(!res)
        debg("[err] get busy buffer\n");
#endif
	return res;
}
/*******************************************************************************
* Function Name  : hal_lcdGetIdleBuffer
* Description    : hardware layer ,get lcd idle buffer,得到的buffer将用于csi output/mjpeg output
* Input          : none
* Output         : None
* Return         : disp_frame_t *p_lcd_buffer : buffer frame addr
*******************************************************************************/
disp_frame_t * hal_lcdGetIdleBuffer(void)
{
    disp_frame_t * res = NULL;
#if HAL_CFG_EN_LCD > 0
    if(lcd_config.need_rotate)
        res = hal_dispframeMalloc(FTYPE_VIDEO_ROTATE);
    else
        res = hal_dispframeMalloc(FTYPE_VIDEO_DISPLAY);
    
//    if(!res)
//        debg("[err] get idle buffer\n");
#endif	
	return res;
}
/*******************************************************************************
* Function Name  : hal_lcdSetFrame
* Description    : hardware layer ,frame to lcd show
* Input          : s16 x : position x
                      s16 y : position y
                      u16 w : width
                      u16 h : height
                      disp_frame_t *p_lcd_buffer : buffer frame addr
                      u8 scan_mode : scan mode.rotate
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdSetFrame(s16 x,s16 y,u16 w,u16 h,void *p_lcd_buffer,u8 scan_mode)
{
#if HAL_CFG_EN_LCD > 0		
    hal_lcdDealBuffer((disp_frame_t *)p_lcd_buffer);
#endif	
	return 0;
}
/*******************************************************************************
* Function Name  : vch_debug
* Description    : print vch info
* Input          : lcd_vch_t * p_lcd_vch
* Output         : None
* Return         : None
*******************************************************************************/
void vch_debug(lcd_vch_t * p_lcd_vch)
{
#if HAL_CFG_EN_LCD > 0	
    if(p_lcd_vch->bak == p_lcd_vch)
        debg("\n---backup\n");
    else
        debg("\n---master\n");
    debgreg(p_lcd_vch);
    debgreg(p_lcd_vch->src);
    debgreg(p_lcd_vch->layer);
    debgreg(p_lcd_vch->status);
    debgreg(p_lcd_vch->config_enable);
    debgreg(p_lcd_vch->x);
    debgreg(p_lcd_vch->y);
    debgreg(p_lcd_vch->w);
    debgreg(p_lcd_vch->h);
    debgreg(p_lcd_vch->bak);
#endif
}
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
int hal_lcdSetVCH(u8 src,u8 layer,
                  u16 x,u16 y,u16 w,u16 h,
                  u8 vch_en)
{
#if HAL_CFG_EN_LCD > 0	
    lcd_vch_t * p_lcd_vch = lcd_config.vchs[src]->bak;
    
    if(p_lcd_vch)
    {
        p_lcd_vch->config_enable = 0;
        if(x != 0xffff)    p_lcd_vch->x = x & ~7;
        if(y != 0xffff)    p_lcd_vch->y = y & ~1;
        if(w != 0xffff)    p_lcd_vch->w = w;
        if(h != 0xffff)    p_lcd_vch->h = h;
        if(layer != 0xff)  p_lcd_vch->layer = layer;
        if(vch_en != 0xff) p_lcd_vch->status = vch_en;
        p_lcd_vch->config_enable = 1;
    }

#endif	
    return 0;
}
/*******************************************************************************
* Function Name  : hal_lcdUpdataVCH
* Description    : set lcd video channles
* Input          : None
* Output         : None
* Return         : int 0: success
                    <0: fail
*******************************************************************************/
int hal_lcdUpdataVCH(void)
{
#if HAL_CFG_EN_LCD > 0	
    lcd_vch_t * isp_vch = lcd_config.vchs[VCH_ISP];
    lcd_vch_t * jpg_vch = lcd_config.vchs[VCH_JPG];
    lcd_vch_t * isp_vch_bak = isp_vch->bak;
    lcd_vch_t * jpg_vch_bak = jpg_vch->bak;
    
    if(isp_vch && jpg_vch && isp_vch_bak && jpg_vch_bak)
    {
        //只有isp/jpg同时使能配置，才能使配置生效
        if(isp_vch_bak->config_enable && jpg_vch_bak->config_enable)
        {
            memcpy(isp_vch,isp_vch_bak,sizeof(*isp_vch_bak));
            memcpy(jpg_vch,jpg_vch_bak,sizeof(*jpg_vch_bak));
            lcd_config.vchs[isp_vch->layer] = isp_vch;
            lcd_config.vchs[jpg_vch->layer] = jpg_vch;
            
            isp_vch_bak->config_enable = 
            jpg_vch_bak->config_enable = 0;
        }
        return 0;
    }
    else
        return -1;


#endif	
    return 0;
}
/*******************************************************************************
* Function Name  : hal_lcdPreSetPIPEnable
* Description    : prepare set lcd pip enable/disbale,
*                  take effect when next csi frame done
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdPreSetPIPEnable(u8 enable)
{
    lcd_config.pip_en = enable;
}
/*******************************************************************************
* Function Name  : hal_lcdSetPIPEnable
* Description    : set lcd pip enable/disbale,take effect immediately
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdSetPIPEnable(u8 enable)
{
    lcd_config.pip_en = enable;
    ax32xx_pipEnable(enable);
}
/*******************************************************************************
* Function Name  : hal_lcdGetResolution
* Description    : hardware layer ,get lcd resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetResolution(u16 *width,u16 *height)
{
#if HAL_CFG_EN_LCD > 0		
	if(lcd_config.p_lcddev == NULL)
		return -1;
	if(lcd_config.need_rotate)
	{
		if(width)
			*width  = lcd_config.p_lcddev->screen_h;
		if(height)
			*height = lcd_config.p_lcddev->screen_w;
		return 0;
	}
	if(width)
		*width  = lcd_config.p_lcddev->screen_w;
	if(height)
		*height = lcd_config.p_lcddev->screen_h;
#endif
	return 0;
}
/*******************************************************************************
* Function Name  : hal_lcdGetOSDShowingResolution
* Description    : hardware layer ,get lcd showing resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetOSDShowingResolution(u16 *width,u16 *height)
{
#if HAL_CFG_EN_LCD > 0		
	if(lcd_config.p_lcddev == NULL)
		return -1;
	if(width)
		*width = lcd_config.p_lcddev->osd_w;
	if(height)
		*height = lcd_config.p_lcddev->osd_h;
#endif
	return 0;	
}
/*******************************************************************************
* Function Name  : hal_lcdGetOSDShowingPosition
* Description    : hardware layer ,get lcd showing position
* Input          : u16 *x : y
                      u16 *x : y
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetOSDShowingPosition(u16 *x,u16 *y)
{
#if HAL_CFG_EN_LCD > 0		
	if(lcd_config.p_lcddev == NULL)
		return -1;
	if(x)
		*x = lcd_config.p_lcddev->osd_x;
	if(y)
		*y = lcd_config.p_lcddev->osd_y;
#endif
	return 0;		
}
/*******************************************************************************
* Function Name  : hal_lcdGetShowingRotate
* Description    : hardware layer ,get lcd showing rotate degree
* Input          : none
* Output         : None
* Return         : int : degree.LCD_ROTATE_0,LCD_ROTATE_90,LCD_ROTATE_180,LCD_ROTATE_270
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetShowingRotate(void)
{
	if(lcd_config.p_lcddev == NULL)
		return -1;

	return lcd_config.p_lcddev->scan_mode;	
}
/*******************************************************************************
* Function Name  : hal_lcdGetBufferResolution
* Description    : hardware layer ,get lcd showing resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0: success
                          <0: fail
*******************************************************************************/
s32 hal_lcdGetBufferResolution(u16 *width,u16 *height)
{
#if HAL_CFG_EN_LCD > 0		
	if(lcd_config.p_lcddev == NULL)
		return -1;
	if(width)
		*width = lcd_config.p_lcddev->video_w;
	if(height)
		*height = lcd_config.p_lcddev->video_h;
#endif
	return 0;	
}
s32 hal_lcdGetVideoPos(u16 *x,u16 *y)
{
#if HAL_CFG_EN_LCD > 0		
	if(lcd_config.p_lcddev == NULL)
		return -1;
	if(x)
		*x = lcd_config.p_lcddev->video_x;
	if(y)
		*y = lcd_config.p_lcddev->video_y;
#endif
	return 0;	
}

void deocode_done(int flag)
{
    if(!lcd_config.csi_using) return;
    
    if(flag & ((1<<8)|(1<<5))) // error
    {
        ax32xx_pipCounterReset(PIP_SRC_JPG);
    }
    else
    {
        if(flag & (1<<6))
            lcd_config.csi_using->pip_sta |= LB_READY1;
    }
}
/*******************************************************************************
* Function Name  : hal_lcdSwitchFrame2
* Description    : hardware layer ,switch frame when csi frame end
*                  <!note>:
*                  1.如代码中所示:
*                       videoSetAddr(ready);//将video切到ready,将在lcdc发送下一帧时生效,
*                                           //在生效前一直使用ready的前一个buffer
*                       jpg_dst_buf = next;
*                       jpegDecode(jpg_dst_buf,...);//启动解码，解码到jpg_dst_buf
*                     当video只有2个buffer时,
*                     "ready的前一个buffer" <=> "video正在使用的buffer" <=> next,
*                     *所以jpg有可能写入正在使用的video buffer,导致画面花或闪
*                     *解决方法:
*                     a.video buffer num = 3
*                     b.必须在video已经切到ready后才能启动decode
*                  2.如代码中所示:
*                       SetPIP(top_layer,x,y,w,h,...);//配置pip top layer
*                       SetScaler(dst_w,dst_h,...);//配置linescaler的目标尺寸,将在csi下一帧开始时
*                                                  //生效,当isp == pip top layer时,pip硬件内部
*                                                  //使用linescaler目标尺寸作为top layer size,
*                                                  //所以在csi下一帧开始之前,pip将使用旧的目标尺寸
*                       jpegDecode(jpg_dst_buf,...);//如果isp == pip top layer,则jpg == bottom
*                                                   //range(top layer coor,top layer size)将被pip保护
*                       *jpg decoder启动后立即开始DMA，但pip仍然使用了旧的top layer size,这将造成
*                        有一帧出现错误图像,画面会闪一下
*                       *解决方法:
*                        if(... && ax32xx_csiScalerCheck())//检查linescaler是否已生效
*                           jpegDecode(jpg_dst_buf,...);
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void hal_lcdSwitchFrame2(void)
{
    disp_frame_t * ready = lcd_config.csi_using;
    disp_frame_t * next = NULL;

    if(!ready)
    {
        //never run here
        //debg("[err] csi using null\n");
        return;
    }
	
    //debgchar('-');
    hal_lcdUpdataVCH();
    
    lcd_vch_t * isp_vch = lcd_config.vchs[VCH_ISP];
    lcd_vch_t * jpg_vch = lcd_config.vchs[VCH_JPG];
    lcd_vch_t * top_vch = lcd_config.vchs[VCH_TOP];
    u32 csi_y_ofs  = isp_vch->x + (isp_vch->y * ready->stride);
    u32 csi_uv_ofs = isp_vch->x + (isp_vch->y / 2 * ready->stride);
    u8 not_skip = 1;

    //config pip,必须在csi没有DMA，且JPGB也没有DMA的情况下修改pip配置
    if(top_vch->config_enable && (top_vch->status == VCH_EN))
    {
        //ax32xx_csiLCDDmaEnable(0);
        ax32xx_pipConfig(top_vch->src,
                         top_vch->x,top_vch->y,
                         top_vch->w,top_vch->h,
                         lcd_config.pip_en);
        ax32xx_videoSetScaleLine(top_vch->y + top_vch->h - 1,1);
        //ax32xx_csiLCDDmaEnable(1);
    }
    //config linescaler
    //if(XOSTimeGet()%500==0)
		//deg_Printf("lcdSw: sx:%d sy:%d ex:%d ey:%d\n",lcd_config.crop_sx,lcd_config.crop_sy,lcd_config.crop_ex,lcd_config.crop_ey);
    if(isp_vch->config_enable)
    {
        not_skip = 0;//skip this frame
        u16 csi_w,csi_h;
        hal_csiResolutionGet(&csi_w,&csi_h);
        ax32xx_csiLCDScaler(csi_w,csi_h,
                            isp_vch->w,isp_vch->h,
                            #if (0 == LCD_CROP_SCALE)
                            0,0,csi_w,csi_h,
                            #else
                            lcd_config.crop_sx,
                            lcd_config.crop_sy,
                            lcd_config.crop_ex,
                            lcd_config.crop_ey,
                            #endif
                            ready->stride,
                            (u32*)lcd_config.p_lcddev->lcd_lsawtooth.anti_lsawtooth);
    }
    
    ready->pip_sta |= LB_READY0;
    u8  buf_ready = LB_READY0;
    if (jpg_vch->status == VCH_EN)
    {
        buf_ready |= LB_READY1;
    }
    
    if(((ready->pip_sta & buf_ready) == buf_ready) && not_skip)
    {
    	if(SysCtrl.photo_task)	// photo task use software handle buf
    	{
			//uart_Printf("1");
    		if(2==SysCtrl.photo_software_handle)	// effect handle finish
    		{
				SysCtrl.photo_software_handle=0;
				//uart_Printf("3");
				yuv420_software_set_handle_frame(ready);
    		}
			else
			{
				//uart_Printf("5");
				disp_frame_t * handle_frame=yuv420_software_get_handle_frame();
				if(NULL==handle_frame)
				{
					yuv420_software_set_handle_frame(ready);
					SysCtrl.photo_software_handle=0;
					//uart_Printf("6");
				}
			}

			lcd_config.csi_using = NULL;
			isp_vch->config_enable = 
			jpg_vch->config_enable = 0;
			return ;

    	}
		else
		{
			//uart_Printf("2");
			next = hal_lcdGetIdleBuffer();
			if(next)
			{

				//update display buffer
	            hal_lcdDealBuffer(ready);

	            //config next frame
	    		hal_dispframeVideoCfg(next,lcd_config.p_lcddev->video_x,lcd_config.p_lcddev->video_y,lcd_config.p_lcddev->video_w,lcd_config.p_lcddev->video_h);
	            ax32xx_csiLCDFrameSet((u32)&next->y_addr[csi_y_ofs],
	                                  (u32)&next->uv_addr[csi_uv_ofs]);
	            //updata using
	            lcd_config.csi_using = next;
			}
		}
    }
    
    if(!next)
    {
        ax32xx_csiLCDFrameSet((u32)&ready->y_addr[csi_y_ofs],
                              (u32)&ready->uv_addr[csi_uv_ofs]);
        ready->pip_sta = LB_IDLE;
    }

/*
    if((jpg_vch->status == VCH_EN) && 
        !hal_BackRecDecodeStatusCheck() && 
        ax32xx_csiScalerCheck())
    {
		
        u8 * jbuf = mjpBcodec_kick();
        if(jbuf)
        {
            ax32xx_mjpegDecodeISRRegister(deocode_done);
            u8 * p_y = &lcd_config.csi_using->y_addr[jpg_vch->x + 
                       (jpg_vch->y * lcd_config.csi_using->stride)];
            u8 * p_uv= &lcd_config.csi_using->uv_addr[jpg_vch->x + 
                       (jpg_vch->y / 2 * lcd_config.csi_using->stride)];

            hal_mjpegDecodeOneFrame_Fast(jbuf,p_y,p_uv,
                                         jpg_vch->w,jpg_vch->h,
                                         lcd_config.csi_using->stride);
        }
    }
*/

    isp_vch->config_enable = 
    jpg_vch->config_enable = 0;
    //if(!SysCtrl.lcd_frame_stop)
    ax32xx_csiLCDDmaKick();
}


void hal_lcd_get_next_frame()
{
	disp_frame_t * next = hal_lcdGetIdleBuffer();
	if(next)
	{
		//uart_Printf("n");
		//config next frame
		hal_dispframeVideoCfg(next,lcd_config.p_lcddev->video_x,lcd_config.p_lcddev->video_y,lcd_config.p_lcddev->video_w,lcd_config.p_lcddev->video_h);
		lcd_vch_t * isp_vch = lcd_config.vchs[VCH_ISP];
		u32 csi_y_ofs  = isp_vch->x + (isp_vch->y * next->stride);
		u32 csi_uv_ofs = isp_vch->x + (isp_vch->y / 2 * next->stride);
		ax32xx_csiLCDFrameSet((u32)&next->y_addr[csi_y_ofs],
							  (u32)&next->uv_addr[csi_uv_ofs]);
		//updata using
		lcd_config.csi_using = next;
		ax32xx_csiLCDDmaKick();
	}
	else
	{
		uart_Printf("buf full\n");
	}

}
/*******************************************************************************
* Function Name  : hal_lcdRegister
* Description    : hardware layer ,lcd device register
* Input          : lcddev_t *lcd_op : lcd device node
* Output         : None
* Return         : int 0: success
*******************************************************************************/
s32 hal_lcdRegister(lcddev_t *lcd_op)
{
	if(!lcd_op)
		return -1;
	
#if HAL_CFG_EN_LCD > 0		

	hal_lcdInit(lcd_op);
    hal_rotateInit();
    
	ax32xx_csiISRRegiser(CSI_IRQ_LCD_FRAME_END,hal_lcdSwitchFrame2);
#endif

    return 0;
}
/*******************************************************************************
* Function Name  : lcd_struct_get
* Description    : lcd_struct_get
* Input          : 
* Output         : lcddev_t * p_lcd_struct                                           
* Return         : none
*******************************************************************************/
lcddev_t * lcd_struct_get(void)
{
	return lcd_config.p_lcddev;
}
/*******************************************************************************
* Function Name  : hal_lcdFrameEndCallBack
* Description    : register callback for lcd frame end
* Input          :void (*callback)(void) : call back
* Output         : None
* Return         : int 0: success
*******************************************************************************/
s32 hal_lcdFrameEndCallBack(void (*callback)(void))
{
    if(lcd_config.p_lcddev->lcd_bus_type == 1)
    {
        ax32xx_lcdISRRegister(LCD_IRQ_DEND,callback);
    }
	else
	{
		ax32xx_lcdISRRegister(LCD_IRQ_FEND,callback);
	}
	return 0;
}


/*******************************************************************************
* Function Name  : hal_lcd_lsawtooth
* Description    : anti-aliasing on lcd screen
* Input          :u32 *anti_lsawtooth
* Output         : None
* Return         : int 0: success
*******************************************************************************/
void hal_lcd_lsawtooth(u32 *anti_lsawtooth)
{
	ax32xx_lcd_lsawbtooth(anti_lsawtooth);
}

/*******************************************************************************
* Function Name  : hal_lcdSetCsiCrop
* Description    : set csi LDMA crop
* Input          : u16 crop_sx,u16 crop_ex,u16 crop_sy,u16 crop_ey
* Output         : None
* Return         : None
*******************************************************************************/
void hal_lcdSetCsiCrop(u16 crop_sx,u16 crop_sy,u16 crop_ex,u16 crop_ey)
{
    lcd_config.crop_sx = crop_sx;
    lcd_config.crop_sy = crop_sy;
    lcd_config.crop_ex = crop_ex;
    lcd_config.crop_ey = crop_ey;
    
    hal_lcdSetVCH(VCH_ISP,-1,-1,-1,-1,-1,-1);
    hal_lcdSetVCH(VCH_JPG,-1,-1,-1,-1,-1,-1);
}


