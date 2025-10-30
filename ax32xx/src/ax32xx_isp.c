/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX ISP
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_isp.h
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
#define  R_CSI_MODE                   CSIMODE
#define  R_ISPMODE                    ISPMODE
#define  R_ISPCTRL                    ISPCTRL
#define  R_ISP_BLC_PARA0              ISP_BLC_PARA0
#define  R_ISP_BLC_PARA1              ISP_BLC_PARA1
#define  R_ISP_DPC_PARA0              ISP_DPC_PARA0
#define  R_ISP_DPC_PARA1              ISP_DPC_PARA1
#define  R_ISP_DPC_PARA2              ISP_DPC_PARA2
#define  R_ISP_DPC_PARA3              ISP_DPC_PARA3
#define  R_ISP_DPC_PARA4              ISP_DPC_PARA4
#define  R_ISP_CCM_PARA0              ISP_CCM_PARA0
#define  R_ISP_CCM_PARA1              ISP_CCM_PARA1
#define  R_ISP_CCM_PARA2              ISP_CCM_PARA2
#define  R_ISP_CCM_PARA3              ISP_CCM_PARA3
#define  R_ISP_DGAIN_PARA0            ISP_DGAIN_PARA0
#define  R_ISP_DGAIN_PARA1            ISP_DGAIN_PARA1
#define  R_ISP_DGAIN_PARA2            ISP_DGAIN_PARA2
#define  R_ISP_CH_PARA0               ISP_CH_PARA0
#define  R_ISP_CH_PARA1               ISP_CH_PARA1
#define  R_ISP_CH_PARA2               ISP_CH_PARA2
#define  R_ISP_CH_PARA3               ISP_CH_PARA3
#define  R_ISP_CH_PARA4               ISP_CH_PARA4
#define  R_ISP_CH_PARA5               ISP_CH_PARA5
#define  R_ISP_CH_PARA6               ISP_CH_PARA6
#define  R_ISP_CH_PARA7               ISP_CH_PARA7
#define  R_ISP_CH_PARA8               ISP_CH_PARA8
#define  R_ISP_CH_PARA9               ISP_CH_PARA9
#define  R_ISP_AE_WINX01              ISP_AE_WINX01
#define  R_ISP_AE_WINX23              ISP_AE_WINX23
#define  R_ISP_AE_WINY01              ISP_AE_WINY01
#define  R_ISP_AE_WINY23              ISP_AE_WINY23
#define  R_ISP_AE_WEIGHT0             ISP_AE_WEIGHT0
#define  R_ISP_AE_WEIGHT1             ISP_AE_WEIGHT1////
#define  R_ISP_AE_WEIGHT2             ISP_AE_WEIGHT2
#define  R_ISP_AE_WEIGHT3             ISP_AE_WEIGHT3////
#define  R_ISP_TAT_COPY_ADR           CSI_STAT_BADR////
#define  R_ISP_AWB_GAIN               ISP_AWB_GAIN
#define  R_ISP_AWB_PARA0 			  ISP_AWB_PARA0
#define  R_ISP_WP_PARA0               ISP_WP_PARA0
#define  R_ISP_WP_PARA1               ISP_WP_PARA1
#define  R_ISP_WP_PARA2               ISP_WP_PARA2
#define  R_ISP_WP_PARA3               ISP_WP_PARA3
#define  R_ISP_WP_PARA4               ISP_WP_PARA4
#define  R_ISP_WP_PARA5               ISP_WP_PARA5
#define  R_ISP_WP_PARA6               ISP_WP_PARA6
#define  R_ISP_WP_PARA7               ISP_WP_PARA7
#define  R_ISP_MD_PARA0               ISP_MD_PARA0
#define  R_ISP_MD_PARA1               ISP_MD_PARA1
#define  R_ISP_MD_PARA2               ISP_MD_PARA2
#define  R_ISP_VDE_PARA0              ISP_VDE_PARA0
#define  R_ISP_VDE_PARA1              ISP_VDE_PARA1
#define  R_ISP_VDE_PARA2              ISP_VDE_PARA2
#define  R_ISP_SHARP_PARA             ISP_SHARP_PARA
#define  R_ISP_SHARP_EEM0             ISP_SHARP_EEM0
#define  R_ISP_SHARP_EEM1             ISP_SHARP_EEM1
#define  R_ISP_SHARP_EEM2             ISP_SHARP_EEM2
#define  R_ISP_SHARP_EEM3             ISP_SHARP_EEM3
#define  R_ISP_SHARP_EEM4             ISP_SHARP_EEM4
#define  R_ISP_SHARP_DNM0             ISP_SHARP_DNM0
#define  R_ISP_SHARP_DNM1             ISP_SHARP_DNM1
#define  R_ISP_SHARP_DNM2             ISP_SHARP_DNM2
#define  R_ISP_SHARP_DNM3             ISP_SHARP_DNM3
#define  R_ISP_SHARP_DNM4             ISP_SHARP_DNM4
#define  R_ISP_CCF_PARA               ISP_CCF_PARA
#define  R_ISP_CCF_CDM                ISP_CCF_CDM
#define  R_ISP_SAJ_PARA0              ISP_SAJ_PARA0
#define  R_ISP_SAJ_PARA1              ISP_SAJ_PARA1
#define  R_ISP_SAJ_PARA2              ISP_SAJ_PARA2


ALIGNED(32) __CSI_STAT__ csi_stat __attribute__ ((section(".sram_comm")));
//map to 
ALIGNED(32) ISP isp;
STAT ispstat ;//__attribute__ ((section(".sdc_buf")));
/*******************************************************************************
* Function Name  : ax32xx_ispModeSet
* Description    : csi mode set
* Input          : u32 mode : mode flag
                      u8 en : 1-enable,0-disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_ispModeSet(u32 mode,u8 en)
{
	if(en)
		R_ISPMODE |= mode;
	else
		R_ISPMODE &= ~mode;
}

#if 0//<---HAL_CFG_ISP_SUPPORT

void isp_stat_en(bool en_ae, bool en_wpt)
{
    R_ISPMODE |=  (en_wpt << 15)|  //_Wpt_POINT_EN_
				(en_ae << 14)  	   //_ae_HGRAM_EN_
				;
	isp_kick_stat();
}
/*******************************************************************************
* Function Name  : isp_BLC_cfg
* Description    : cfg the blc
* Input          : U32 smooth_th, u32 smooth_c,int blkl_gb,int blkl_gr,int blkl_r,int blkl_b   
* Output         : None
* Return         : None
*******************************************************************************/
void isp_BLC_cfg(U32 smooth_th, u32 smooth_c,int blkl_gb,int blkl_gr,int blkl_r,int blkl_b){
	R_ISP_BLC_PARA0 = ((smooth_th&0xf)<<26)|((blkl_gr&0x3ff)<<16)|((smooth_c&0xf)<<10)|(blkl_r&0x3ff);
	R_ISP_BLC_PARA1 = ((blkl_b&0x3ff)<<16)|(blkl_gb&0x3ff);
}
/*******************************************************************************
* Function Name  : isp_LSC_GbGr_cfg
* Description    : cfg lens shading correction and gbgr correction
* Input          : u32 lsc_block_sel : 
* 				   void * p_lsc_tab  :lsc table
*                  u32 gbgr_cor_th : gbgr_cor_th
* Output         : None
* Return         : None
*******************************************************************************/
ALIGNED(32)  u16 lsc_tab[572]; 
#define _LSC_TAB_RAM_ADDR__ 0x85000
void isp_LSC_cfg(void *p_lsc_tab)
{
	//p_lsc_tab = (void *)lsc_tab;
	if(p_lsc_tab){
		half_word_memcpy((u16*)_LSC_TAB_RAM_ADDR__,p_lsc_tab,572*2);
	}
}
/*******************************************************************************
* Function Name  : isp_DDC_cfg
* Description    : config isp DDC
* Input          : u32 dpc_dn_en,u32 hot_num,u32 dead_num,u32 hot_th,u32 dead_th,u32 avg_th,
				  u32 d_th_rate,u32 h_th_rate,u32* std_th,u32 std_th_rate,u32 *indx_table
* Output         : None
* Return         : None
*******************************************************************************/
void isp_DDC_cfg(u32 dpc_dn_en,u32 hot_num,u32 dead_num,u32 hot_th,u32 dead_th,u32 avg_th,
				u32 d_th_rate,u32 h_th_rate,u32* std_th,u32 std_th_rate,u32 *indx_table)
{
	R_ISP_DPC_PARA0 = ((dpc_dn_en&0x1)<<30)|((hot_num&0x7)<<27)|((dead_num&0x7)<<24)|((hot_th&0x3ff)<<12)|(dead_th&0x3ff);
	R_ISP_DPC_PARA1 = ((indx_table[7]&0x7)<<20)|((std_th_rate&0xf)<<16)|((avg_th&0xff)<<8)|((d_th_rate&0xf)<<4)|(h_th_rate&0xf);
	R_ISP_DPC_PARA2 = ((std_th[2]&0x1ff)<<20)|((std_th[1]&0x1ff)<<10)|(std_th[0]&0x1ff);
	R_ISP_DPC_PARA3 = ((std_th[5]&0x1ff)<<20)|((std_th[4]&0x1ff)<<10)|(std_th[3]&0x1ff);
	R_ISP_DPC_PARA4 = ((std_th[6]&0x1ff)<<21)|((indx_table[6]&0x7)<<18)|((indx_table[5]&0x7)<<15)|((indx_table[4]&0x7)<<12) \
					  |((indx_table[3]&0x7)<<9)|((indx_table[2]&0x7)<<6)|((indx_table[1]&0x7)<<3)|(indx_table[0]&0x7);
}
/*******************************************************************************
* Function Name  : isp_AWB_GAIN_adj
* Description    : awb gain adjust
* Input          : u32 Kb : bgain 
* 				   u32 Kg : ggain  
*                  u32 Kr : rgain 
* Output         : None
* Return         : None
*******************************************************************************/
void isp_AWB_GAIN_adj(u32 Kb, u32 Kg, u32 Kr)
{
	R_ISP_AWB_GAIN = ((Kb&0x3ff) << 20) | ((Kg&0x3ff) << 10) | ((Kr&0x3ff) << 0);
}
#define __WP_TABLE_ADDR__ 0x84C00

/*******************************************************************************
* Function Name  : isp_whtpnt_stat_cfg
* Description    : cfg wp stat
* Input          : u8 *p_wp_tab u32 seg_mode,u32 rgmin,u32 rgmax,u32 weight_in,u32 weight_mid,u32 ymin,u32 ymax
* 				   u32 hb_rate,u32 hb_class,u32 hr_rate,u32 hr_class, u32 yuv_mod_en, u32 rg_start, u32 ycbcr_th,
* 				   u32 *cb_th, u32 *cr_th, u32 *cbcr_th
* Output         : None
* Return         : None
*******************************************************************************/
void isp_whtpnt_stat_cfg(u8 *p_wp_tab,u32 seg_mode,u32 rgmin,u32 rgmax,u32 weight_in,u32 weight_mid,u32 ymin,u32 ymax,
						 u32 hb_rate,u32 hb_class,u32 hr_rate,u32 hr_class, u32 yuv_mod_en, u32 rg_start, u32 ycbcr_th,
 				   u32 *cb_th, u32 *cr_th, u32 *cbcr_th)
{
	R_ISP_AWB_PARA0 = ((u8)hb_rate<<12)|((hb_class&0x3)<<10)|((u8)hr_rate<<2)|(hr_class&0x3);
	R_ISP_WP_PARA0 = ((yuv_mod_en&0x1)<<28)|((seg_mode&0x3)<<26)|((weight_mid&0x7)<<23)|((weight_in&0x7)<<20)|((rgmin&0x3ff)<<10)|(rgmax&0x3ff);
	R_ISP_WP_PARA1 = ((cb_th[7]&0x7f)<<24)|((u8)ycbcr_th<<16)|((u8)ymin<<8)|(u8)ymax;
	R_ISP_WP_PARA2 = ((cb_th[3]&0x7f)<<21)|((cb_th[2]&0x7f)<<14)|((cb_th[1]&0x7f)<<7)|(cb_th[0]&0x7f);
	R_ISP_WP_PARA3 = ((rg_start&0x1ff)<<21)|((cb_th[6]&0x7f)<<14)|((cb_th[5]&0x7f)<<7)|(cb_th[4]&0x7f);
	R_ISP_WP_PARA4 = ((cr_th[3]&0x7f)<<21)|((cr_th[2]&0x7f)<<14)|((cr_th[1]&0x7f)<<7)|(cr_th[0]&0x7f);
	R_ISP_WP_PARA5 = ((cr_th[7]&0x7f)<<21)|((cr_th[6]&0x7f)<<14)|((cr_th[5]&0x7f)<<7)|(cr_th[4]&0x7f);
	R_ISP_WP_PARA6 = ((u8)cbcr_th[3]<<24)|((u8)cbcr_th[2]<<16)|((u8)cbcr_th[1]<<8)|((u8)cbcr_th[0]);
	R_ISP_WP_PARA7 = ((u8)cbcr_th[7]<<24)|((u8)cbcr_th[6]<<16)|((u8)cbcr_th[5]<<8)|((u8)cbcr_th[4]);
	if(p_wp_tab && (!(R_ISPCTRL & ISP_CTRL_AWB_TAB_KS))){
		word_memcpy((u32*)__WP_TABLE_ADDR__,p_wp_tab,128);
		R_ISPCTRL |= ISP_CTRL_AWB_TAB_KS;
	}
	//debg("R_ISP_AWB_PARA0:%x\n",R_ISP_AWB_PARA0);
}
/*******************************************************************************
* Function Name  : isp_CCM_cfg
* Description    : u32 c11,u32 c12,u32 c13,u32 c21,u32 c22,u32 c23,u32 c31,u32 c32,u32 c33,
* 				   int s41,int s42,int s43
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void isp_CCM_cfg(u16 *ccm,short s41,short s42,short s43)
{
	R_ISP_CCM_PARA0 = ((ccm[2]&0x3ff)<< 20) |((ccm[1]&0x3ff)<< 10) | (ccm[0]&0x3ff) ;
	R_ISP_CCM_PARA1 = ((ccm[5]&0x3ff)<< 20) |((ccm[4]&0x3ff)<< 10) | (ccm[3]&0x3ff) ;
	R_ISP_CCM_PARA2 = ((ccm[8]&0x3ff)<< 20) |((ccm[7]&0x3ff)<< 10) | (ccm[6]&0x3ff) ;
	
	R_ISP_CCM_PARA3 = ((s43&0x7f)<< 20) |((s42&0x7f)<< 10) | (s41&0x7f) ;
}
/*******************************************************************************
* Function Name  : isp_awbgain_adj
* Description    : awb gain adjust
* Input          : u32 dgain : dgain 
* Output         : None
* Return         : None
*******************************************************************************/
void isp_RGB_DGAIN_adj(u32 dgain0,u32 dgain1,u32 dgain2,u32 dgain3,u32 dgain4,u32 dgain5,u32 dgain6,u32 dgain7,u32 dgain8)
{
	u32 tmp;
	R_ISP_DGAIN_PARA0 = ((u8)dgain3 << 24) | ((u8)dgain2 << 16) | ((u8)dgain1 << 8) | ((u8)dgain0 << 0);
	R_ISP_DGAIN_PARA1 = ((u8)dgain7 << 24) | ((u8)dgain6 << 16) | ((u8)dgain5 << 8) | ((u8)dgain4 << 0);	
	tmp = R_ISP_DGAIN_PARA2;
	tmp &= ~0xff;
	tmp |= (u8)dgain8;
	R_ISP_DGAIN_PARA2 = tmp;
	//debg("dgain:%x %x %x\n",R_ISP_DGAIN_PARA0,R_ISP_DGAIN_PARA1,R_ISP_DGAIN_PARA2);
}
/*******************************************************************************
* Function Name  : isp_hist_stat_cfg
* Description    : cfg isp ae globel hist stat
* Input          : u32 win_x0,u32 win_x1,u32 win_x2,u32 win_x3,
* 				   u32 win_y0,u32 win_y1,u32 win_y2,u32 win_y3,
* 				   u32 hb_rate,u32 hb_class,u32 hr_rate,u32 hr_class
* 				   u32 weight0_7,u32 weight8_15,u32 weight16_23,u32 weight24,u32 *p_csi_hist
* Output         : None
* Return         : None
*******************************************************************************/
void isp_hist_stat_cfg(u32 win_x0,u32 win_x1,u32 win_x2,u32 win_x3,u32 win_y0,u32 win_y1,
					   u32 win_y2,u32 win_y3,u32 weight0_7,u32 weight8_15,u32 weight16_23,u32 weight24,u32 *p_csi_hist)
{
	R_ISP_AE_WINX01 = ((u16)win_x1<<16)|(u16)win_x0;
	R_ISP_AE_WINX23 = ((u16)win_x3<<16)|(u16)win_x2;
	R_ISP_AE_WINY01 = ((u16)win_y1<<16)|(u16)win_y0;
	R_ISP_AE_WINY23 = ((u16)win_y3<<16)|(u16)win_y2;
	R_ISP_AE_WEIGHT0 = weight0_7;
	R_ISP_AE_WEIGHT1 = weight8_15;
	R_ISP_AE_WEIGHT2 = weight16_23;
	R_ISP_AE_WEIGHT3 = weight24;
	R_ISP_TAT_COPY_ADR  = (u32)p_csi_hist;
}
#define __RGAMM_RAM_ADDR__ 0x84600
#define __GGAMM_RAM_ADDR__ 0x84800
#define __BGAMM_RAM_ADDR__ 0x84A00
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

ALIGNED(32) u8 using_contrast[256];
#define __YGAMM_RAM_ADDR__   0x84400
u16 Y_GMA[_YGAMA_STEPS_];
u8 RGB_GMMA[3][_RGB_GAMA_STEPS_];
void isp_GAMMA_cfg(u16* p_ygama,u8 mod,u8* p_contra,u32 ymin, u32 ymax,u32 *p_fog_y_low,u32 *p_fog_y_high, u32 pad_num)
{
	//ÖØ¹¹GAMMA ((x-b)*a)^r
	u32 i,hue;
	ymin<<= (_YGAMA_STEPS_BIT_-8);
	ymax<<= (_YGAMA_STEPS_BIT_-8);
	
	for(i = 0; i < _YGAMA_STEPS_; i++){
		if(i < ymin){
			hue = 0;
		}else if(i > ymax){
			hue = _YGAMA_STEPS_ -1;
		}else{
			hue =  ((i - ymin) * (_YGAMA_STEPS_-1))/ (ymax- ymin) ;///(255 * i)/(l_ofset - b_ofset) - 255 * b_ofset/(l_ofset - b_ofset);
		}
		u32 temp = (p_ygama[hue]*(32 - mod) + p_ygama[hue + _YGAMA_STEPS_]*mod)>>5;
		Y_GMA[i] = temp;//p_contra[temp];
	}
	//debg("Y_GMA: %d %d %d %d %d %d\n",Y_GMA[0],Y_GMA[254],Y_GMA[255],p_ygama[250],p_ygama[254],p_ygama[255]);
	if(p_fog_y_low&&p_fog_y_high){
		*p_fog_y_low = Y_GMA[(*p_fog_y_low)<<(_YGAMA_STEPS_BIT_ - 8)];
		*p_fog_y_low >>= (_YGAMA_VAL_BIT_  - 8);
		*p_fog_y_high = Y_GMA[(*p_fog_y_high)<<(_YGAMA_STEPS_BIT_ - 8)];
		*p_fog_y_high>>= (_YGAMA_VAL_BIT_  - 8);
	}
	if((R_ISPCTRL & ISP_CTRL_YGAMMA_KS) == 0)
    {
		word_memcpy((u32*)__YGAMM_RAM_ADDR__,Y_GMA,sizeof(Y_GMA));
		R_ISP_DGAIN_PARA2 &=~(0x7<<8);
		R_ISP_DGAIN_PARA2 |= (pad_num&0x7)<<8;
        R_ISPCTRL |= ISP_CTRL_YGAMMA_KS; //kick gamma
		//u32*p = (u32*)__YGAMM_RAM_ADDR__;
		//debg("Y_GMA: %x %x %x %x\n",p[0],p[1],p[510],p[511]);
    }
}
/*******************************************************************************
* Function Name  : isp_RGB_gam_cfg
* Description    : cfg isp ae globel hist stat
* Input          : u16 *p_rgbgam,u8 mod,u32 max_oft,
* 				   u32 rmin,u32 rmax,u32 gmin,u32 gmax,u32 bmin,u32 bmax
* Output         : None
* Return         : None
*******************************************************************************/
void isp_RGB_gam_cfg(u8 *p_rgbgam,u8 mod,u32 max_oft,u32 rmin,u32 rmax,u32 gmin,u32 gmax,u32 bmin,u32 bmax)
{

	u32 rgbmax[3] = {rmax << (_RGB_GAMA_STEPS_BIT_-8),gmax << (_RGB_GAMA_STEPS_BIT_-8),bmax << (_RGB_GAMA_STEPS_BIT_-8)};
	u32 rgbmin[3] = {rmin << (_RGB_GAMA_STEPS_BIT_-8),gmin << (_RGB_GAMA_STEPS_BIT_-8),bmin << (_RGB_GAMA_STEPS_BIT_-8)};
		
	u32 i,j,hue;
	for(j = 0; j < 3; j++){  //R,G,B
		for(i = 0; i < _RGB_GAMA_STEPS_; i++){
			if(i <= rgbmin[j]){
				hue = 0;
			}
			else if(i >= rgbmax[j]){
				hue = _RGB_GAMA_STEPS_-1;
			}else{
				//hue = 255 - 255 * (rgbmax[j]-i)/ (rgbmax[j]- rgbmin[j]);
				hue = ((i - rgbmin[j]) * (_RGB_GAMA_STEPS_-1))/ (rgbmax[j]- rgbmin[j]);
			}
			u32 temp  = (p_rgbgam[hue]*(32 - mod) + p_rgbgam[hue+_RGB_GAMA_STEPS_]*mod)>>5;
			temp += (max_oft*(_RGB_GAMA_MAX_VAL_-temp) / (_RGB_GAMA_MAX_VAL_+1));
			RGB_GMMA[j][i] = temp ;
			//RGB_GMMA[j][i]  = i;
		}
	}
	if((R_ISPCTRL & ISP_CTRL_RGAMMA_KS) == 0)
    {
        word_memcpy((u32 *) __RGAMM_RAM_ADDR__,(u32 *)RGB_GMMA[0],sizeof(RGB_GMMA)/3);
		word_memcpy((u32 *) __GGAMM_RAM_ADDR__,(u32 *)RGB_GMMA[1],sizeof(RGB_GMMA)/3);
		word_memcpy((u32 *) __BGAMM_RAM_ADDR__,(u32 *)RGB_GMMA[2],sizeof(RGB_GMMA)/3);
        R_ISPCTRL |= ISP_CTRL_RGAMMA_KS;   //Kick start bit
	}
	//u32 * p = (u32 *) __RGAMM_RAM_ADDR__;
	//debg("RGBGMA: %x %x %x %x %x %x\n",p[0],p[63],p[128],p[191],p[256],p[319]);
	//debg("RGBGMA: %x %x %x %x\n",RGB_GMMA[0][252],RGB_GMMA[0][253],RGB_GMMA[0][254],RGB_GMMA[0][255]);
	//debg("\nRGBGAM ON\n");
}
/*******************************************************************************
* Function Name  : isp_CH_cfg
* Description    : cfg color enhance
* Input          : u32 stage1_en,u32 stage0_en,u32*p_enhence,u32*p_th1,u32*p_th0,
* 				   u32*p_r_rate,u32*p_g_rate,u32*p_b_rate , u32*p_sat
* Output         : None
* Return         : None
*******************************************************************************/
void isp_CH_cfg(u32 stage1_en,u32 stage0_en,u32*p_enhence,u32*p_th1,u32*p_th0,u32*p_r_rate,u32*p_g_rate,u32*p_b_rate,u32*p_sat)
{
	R_ISP_CH_PARA0 = ((p_enhence[0]&0x1)<<28) | ((p_th1[0]&0x1ff)<<19) | ((p_th0[0]&0x1ff)<<10) | ((p_g_rate[0]&0x1f)<<5) | (p_r_rate[0]&0x1f);
	R_ISP_CH_PARA1 = ((p_enhence[1]&0x1)<<28) | ((p_th1[1]&0x1ff)<<19) | ((p_th0[1]&0x1ff)<<10) | ((p_g_rate[1]&0x1f)<<5) | (p_r_rate[1]&0x1f);
	R_ISP_CH_PARA2 = ((p_enhence[2]&0x1)<<28) | ((p_th1[2]&0x1ff)<<19) | ((p_th0[2]&0x1ff)<<10) | ((p_g_rate[2]&0x1f)<<5) | (p_r_rate[2]&0x1f);
	R_ISP_CH_PARA3 = ((p_enhence[3]&0x1)<<28) | ((p_th1[3]&0x1ff)<<19) | ((p_th0[3]&0x1ff)<<10) | ((p_g_rate[3]&0x1f)<<5) | (p_r_rate[3]&0x1f);
	R_ISP_CH_PARA4 = ((p_enhence[4]&0x1)<<28) | ((p_th1[4]&0x1ff)<<19) | ((p_th0[4]&0x1ff)<<10) | ((p_g_rate[4]&0x1f)<<5) | (p_r_rate[4]&0x1f);
	R_ISP_CH_PARA5 = ((p_enhence[5]&0x1)<<28) | ((p_th1[5]&0x1ff)<<19) | ((p_th0[5]&0x1ff)<<10) | ((p_g_rate[5]&0x1f)<<5) | (p_r_rate[5]&0x1f);
	
	//debg("R_ISP_CH_PARA3:%x %x %x %x %x %x\n",R_ISP_CH_PARA3,p_enhence[5],p_th1[5],p_th0[5],p_g_rate[5],p_r_rate[5]);
	R_ISP_CH_PARA6 = ((stage1_en&0x1)<<31)  | ((stage0_en&0x1)<<30) | ((p_b_rate[5]&0x1f)<<25)  | ((p_b_rate[4]&0x1f)<<20) 
	                 | ((p_b_rate[3]&0x1f)<<15) | ((p_b_rate[2]&0x1f)<<10) | ((p_b_rate[1]&0x1f)<<5) | (p_b_rate[0]&0x1f);
	R_ISP_CH_PARA7 = ((p_sat[5]&0x1f)<<25) | ((p_sat[4]&0x1f)<<20) | ((p_sat[3]&0x1f)<<15) | ((p_sat[2]&0x1f)<<10) | ((p_sat[1]&0x1f)<<5) | (p_sat[0]&0x1f);
	R_ISP_CH_PARA8 = ((p_sat[11]&0x1f)<<25) | ((p_sat[10]&0x1f)<<20) | ((p_sat[9]&0x1f)<<15) | ((p_sat[8]&0x1f)<<10) | ((p_sat[7]&0x1f)<<5) | (p_sat[6]&0x1f);
	R_ISP_CH_PARA9 = ((p_sat[16]&0x1f)<<20) | ((p_sat[15]&0x1f)<<15) | ((p_sat[14]&0x1f)<<10) | ((p_sat[13]&0x1f)<<5) | (p_sat[12]&0x1f);
}
/*******************************************************************************
* Function Name  : isp_VDE_cfg
* Description    : cfg VDE
* Input          : u32 contra,u32 bright_oft, u32 bright_k, u32 hue,u32*p_sat
* Output         : None
* Return         : None
*******************************************************************************/
void isp_VDE_cfg(u32 contra,u32 bright_oft, u32 bright_k, u32 hue,u32*p_sat)
{
	R_ISP_VDE_PARA0 = ((u8)contra <<  24)|((u8)bright_oft << 16)|((u8)bright_k << 8) | (u8)hue;
	R_ISP_VDE_PARA1 = ((p_sat[3]&0xff)<<24) | ((p_sat[2]&0xff)<<16) | ((p_sat[1]&0xff)<<8) | (p_sat[0]&0xff);
	R_ISP_VDE_PARA2 = ((p_sat[7]&0xff)<<24) | ((p_sat[6]&0xff)<<16) | ((p_sat[5]&0xff)<<8) | (p_sat[4]&0xff);
	R_ISP_DGAIN_PARA2 &= ~(0xff<<11);
	R_ISP_DGAIN_PARA2 |= ((p_sat[8]&0xff)<<11);
}


const u32 LOG_TAB[25] = {
	/*-1,-2,-2,-2,-1,
	-2, 0, 4, 0,-2,
	-2, 4, 12,4,-2,
	-2, 0, 4, 0,-2,
	-1,-2,-2,-2,-1*/
	/*0,0,0,0,0,
	0,-2,-2,-2,0,
	0,-2, 8,-2,0,
	0,-2,-2,-2,0,
	0,0,0,0,0,*/
	-1,-2,-2,-2,-1,
	-2, 1, 3, 1,-2,
	-2, 3, 20,3,-2,
	-2, 1, 3, 1,-2,
	-1,-2,-2,-2,-1
};
const u32 GAOS5X5_TAB[][25] = {
	{0,0,1,0,0,
	0,2,4,2,0,
	1,4,24,4,1,
	0,2,4,2,0,
	0,0,1,0,0},//*/
	/*{0,0,0,0,0,
	0,0,0,0,0,
	0,0,1,0,0,
	0,0,0,0,0,
	0,0,0,0,0},*/
	/*{-1,-2,-2,-2,-1,
	-2, 0, 4, 0,-2,
	-2, 4, 17,4,-2,
	-2, 0, 4, 0,-2,
	-1,-2,-2,-2,-1},//*/
};
const u32 GAOS3X3_TAB[][9] = {
	{21,10,21,
	 10,4,10,
	 21,10,21},//sum must be 128
};
/*******************************************************************************
* Function Name  : isp_EE1_cfg
* Description    : cfg EE0 
* Input          : u32 sharp_slope_w,u32 sharp_th,u32 sharp_th_adp,u32 sharp_class, u32 dn_class
* Output         : None
* Return         : None
*******************************************************************************/
void isp_EE_cfg(u8 ee_dn_th,u8 ee_dn_slope,u8 ee_sharp_slope,u8 ee_th_adp,u8 sharp_class,u8 dn_class)
{
	u32 * p_ee_mk = (u32 *)LOG_TAB;
	u32 * p_dn_mk = (u32 *)GAOS5X5_TAB[dn_class];
	//sharp_class = 0x0;
	R_ISP_SHARP_PARA &= ~0xffff;
	R_ISP_SHARP_PARA |= ((ee_dn_th&0x3f)<<10)|((ee_dn_slope&0x7)<<7)|((ee_sharp_slope&0x7)<<4)|(ee_th_adp&0xf);
	R_ISP_SHARP_EEM0 = ((p_ee_mk[4]&0x3f)<<24)|((p_ee_mk[3]&0x3f)<<18)|((p_ee_mk[2]&0x3f)<<12)|((p_ee_mk[1]&0x3f)<<6)|(p_ee_mk[0]&0x3f);
	R_ISP_SHARP_EEM1 = ((p_ee_mk[9]&0x3f)<<24)|((p_ee_mk[8]&0x3f)<<18)|((p_ee_mk[7]&0x3f)<<12)|((p_ee_mk[6]&0x3f)<<6)|(p_ee_mk[5]&0x3f);
//	R_ISP_SHARP_EEM2 = ((p_ee_mk[14]&0x3f)<<24)|((p_ee_mk[13]&0x3f)<<18)|(((31 - clip(sharp_class,0,18))&0x3f)<<12)|((p_ee_mk[11]&0x3f)<<6)|(p_ee_mk[10]&0x3f);
	R_ISP_SHARP_EEM2 = ((p_ee_mk[14]&0x3f)<<24)|((p_ee_mk[13]&0x3f)<<18)|((clip((31-sharp_class),16,31)&0x3f)<<12)|((p_ee_mk[11]&0x3f)<<6)|(p_ee_mk[10]&0x3f);
	R_ISP_SHARP_EEM3 = ((p_ee_mk[19]&0x3f)<<24)|((p_ee_mk[18]&0x3f)<<18)|((p_ee_mk[17]&0x3f)<<12)|((p_ee_mk[16]&0x3f)<<6)|(p_ee_mk[15]&0x3f);
	R_ISP_SHARP_EEM4 = ((p_ee_mk[24]&0x3f)<<24)|((p_ee_mk[23]&0x3f)<<18)|((p_ee_mk[22]&0x3f)<<12)|((p_ee_mk[21]&0x3f)<<6)|(p_ee_mk[20]&0x3f);	
	R_ISP_SHARP_DNM0 = ((p_dn_mk[4]&0x3f)<<24)|((p_dn_mk[3]&0x3f)<<18)|((p_dn_mk[2]&0x3f)<<12)|((p_dn_mk[1]&0x3f)<<6)|(p_dn_mk[0]&0x3f);
	R_ISP_SHARP_DNM1 = ((p_dn_mk[9]&0x3f)<<24)|((p_dn_mk[8]&0x3f)<<18)|((p_dn_mk[7]&0x3f)<<12)|((p_dn_mk[6]&0x3f)<<6)|(p_dn_mk[5]&0x3f);
	R_ISP_SHARP_DNM2 = ((p_dn_mk[14]&0x3f)<<24)|((p_dn_mk[13]&0x3f)<<18)|((p_dn_mk[12]&0x3f)<<12)|((p_dn_mk[11]&0x3f)<<6)|(p_dn_mk[10]&0x3f);
	R_ISP_SHARP_DNM3 = ((p_dn_mk[19]&0x3f)<<24)|((p_dn_mk[18]&0x3f)<<18)|((p_dn_mk[17]&0x3f)<<12)|((p_dn_mk[16]&0x3f)<<6)|(p_dn_mk[15]&0x3f);
	R_ISP_SHARP_DNM4 = ((p_dn_mk[24]&0x3f)<<24)|((p_dn_mk[23]&0x3f)<<18)|((p_dn_mk[22]&0x3f)<<12)|((p_dn_mk[21]&0x3f)<<6)|(p_dn_mk[20]&0x3f);
}
/*******************************************************************************
* Function Name  : isp_CCF_cfg
* Description    : cfg CCF 
* Input          : u32 ccf_wdc_en,u32 ccf_wdc_en, u32 ccf_wclass, u32 ccf_rate, u32 ccf_wymin,u32 ccf_ymax,u32 ccf_mean_en,u32 ccf_th,u32 c1_tab_indx
* Output         : None
* Return         : None
*******************************************************************************/
void isp_CCF_cfg(u32 ccf_ccf_en,u32 ccf_wdc_en, u32 ccf_wclass, u32 ccf_rate, u32 ccf_wymin,u32 ccf_ymax,u32 ccf_mean_en,u32 ccf_th, u32 dn_class)
{
	u32 * p_cdn_mk = (u32 *)GAOS3X3_TAB[dn_class];
	//debg("isp_CCF_cfg %x %x %x %x\n",dn_class,p_cdn_mk[0],p_cdn_mk[1],p_cdn_mk[4]);
	R_ISP_CCF_PARA = (((ccf_ccf_en&0x1)<<31)|(ccf_wdc_en&0x1)<<30)|((ccf_wclass&0x3)<<28)|((ccf_rate&0xf)<<24)|(((u8)ccf_wymin|0x80)<<16)|
					 ((u8)ccf_ymax<<8)|((ccf_mean_en&0x1)<<7)|((ccf_th&0x7f)<<0);
	R_ISP_CCF_CDM  = ((p_cdn_mk[5]&0x1f)<<25)|((p_cdn_mk[4]&0x1f)<<20)|((p_cdn_mk[3]&0x1f)<<15)|((p_cdn_mk[2]&0x1f)<<10)
					|((p_cdn_mk[1]&0x1f)<<5)|((p_cdn_mk[0]&0x1f)<<0);
	R_ISP_SHARP_PARA |= ((p_cdn_mk[8]&0x1f)<<26)|((p_cdn_mk[7]&0x1f)<<21)|((p_cdn_mk[6]&0x1f)<<16);
}
/*******************************************************************************
* Function Name  : isp_SAJ_cfg
* Description    : cfg SAJ
* Input          : u32 contra,u32 bright_oft, u32 bright_k, u32 hue,u32*p_sat
* Output         : None
* Return         : None
*******************************************************************************/
void isp_SAJ_cfg(u8 sj_sr0, u8 sj_sr1, u8 sj_sr2, u8 sj_sr3, u8 sj_sr4, u8 sj_sr5, u8 sj_sr6,u8 sj_sr7, u8 sj_sr8,
				 u8 sj_sr9, u8 sj_sr10, u8 sj_sr11, u8 sj_sr12, u8 sj_sr13,u8 sj_sr14, u8 sj_sr15, u8 sj_sr16)
{
	R_ISP_SAJ_PARA0 = ((sj_sr5&0x1f)<<25)|((sj_sr4&0x1f)<<20)|((sj_sr3&0x1f)<<15)|((sj_sr2&0x1f)<<10)|((sj_sr1&0x1f)<<5)|(sj_sr0&0x1f);
	R_ISP_SAJ_PARA1 = ((sj_sr11&0x1f)<<25)|((sj_sr10&0x1f)<<20)|((sj_sr9&0x1f)<<15)|((sj_sr8&0x1f)<<10)|((sj_sr7&0x1f)<<5)|(sj_sr6&0x1f);
	R_ISP_SAJ_PARA2 = ((sj_sr16&0x1f)<<20)|((sj_sr15&0x1f)<<15)|((sj_sr14&0x1f)<<10)|((sj_sr13&0x1f)<<5)|(sj_sr12&0x1f);
}
void isp_kick_stat(void)
{
	R_ISPCTRL |= ISP_CTRL_AE_STAT_CLR;//_ISP_AE_KICK_STAT_
	R_ISPCTRL |= ISP_CTRL_AWB_STAT_CLR;//_ISP_WP_KICK_STAT_
}
void stat_cp_kick_st(void){
	R_ISPCTRL |= ISP_CTRL_CP_STAT_KS;
}
void stat_cp_done(void){
	while(!(CSI_INTR & BIT(13)));
	CSI_INTR = BIT(13);
}
void isp_model_cfg(u32 model_num,u32 val){
	bool en;
	if((val==_ISP_EN_)||(val==_ISP_AUTO_)){
		en = true;
	}else{
		en = false;
	}
	//return;
	switch (model_num){
		case _BLC_POS_:         ax32xx_ispModeSet(ISP_MODE_BLCEN,en);debg("BLC MODE:%d\n",val);break;
		case _LSC_POS_:         ax32xx_ispModeSet(ISP_MODE_LSCEN,en);debg("LSC MODE:%d\n",val);break;
		case _DDC_POS_:         ax32xx_ispModeSet(ISP_MODE_DPCEN,en);debg("DDC MODE:%d\n",val);break;
		case _AWB_POS_ :        ax32xx_ispModeSet(ISP_MODE_AWBEN,en);debg("AWB MODE:%d\n",val);break;
		case _CCM_POS_:         ax32xx_ispModeSet(ISP_MODE_CCMEN,en);debg("CCM MODE:%d\n",val);break;
		case _YGAMA_POS_:       ax32xx_ispModeSet(ISP_MODE_YGAMMAEN,en);debg("YGAMA MODE:%d\n",val);break;
		case _DGAIN_POS_:       ax32xx_ispModeSet(ISP_MODE_DGAINEN,en);debg("DEGAIN MODE:%d\n",val);break;
		case _RGB_GAMA_POS_:    ax32xx_ispModeSet(ISP_MODE_RGBGAMMAEN,en);debg("RGB_GAMA MODE:%d\n",val);break;
	    case _CH_POS_:          ax32xx_ispModeSet(ISP_MODE_CHEN,en);debg("CH MODE:%d\n",val);break;
		case _VDE_POS_:         ax32xx_ispModeSet(ISP_MODE_VDEEN,en);debg("VDE MODE:%d\n",val);break;
		case _EE_POS_:          ax32xx_ispModeSet(ISP_MODE_SHARPEN,en);debg("EE MODE:%d &x\n",val,R_CSI_MODE);break;
		case _CFD_POS_:         ax32xx_ispModeSet(ISP_MODE_CCFEN,en);debg("CFD MODE:%d\n",val);break;
		case _SAJ_POS_:         ax32xx_ispModeSet(ISP_MODE_SAJEN,en);debg("SAJ MODE:%d\n",val);break;
		default: break;//
	}
}
#endif

