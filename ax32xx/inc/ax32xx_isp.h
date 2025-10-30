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
#ifndef  AX32XX_ISP_H
    #define  AX32XX_ISP_H
#define _FRAME_NUMS_  2  //_FRAME_NUMS_ must >=5 when stream-lined	
#define _YGAMA_CLSASSES_     15
#define _RGBGAMA_CLSASSES_  18
#define _CONTRA_CLSASSES_   7

#define _YGAMA_STEPS_BIT_      8
#define _YGAMA_STEPS_          (1<<(_YGAMA_STEPS_BIT_))
#define _YGAMA_VAL_BIT_        10
#define _YGAMA_MAX_VAL_        ((1<<(_YGAMA_VAL_BIT_)) - 1)

#define _RGB_GAMA_STEPS_BIT_   8
#define _RGB_GAMA_STEPS_       (1<<(_RGB_GAMA_STEPS_BIT_))
#define _RGB_GAMA_VAL_BIT_     8
#define _RGB_GAMA_MAX_VAL_     ((1<<(_RGB_GAMA_VAL_BIT_)) - 1)

#define _SET_ISP_MODE_(x,model,val)  do{x = ((x)&(~(0x03<<(model)))); x = ((x)|(((val)&(_ISP_MODE_BW_))<<(model)));}while(0);	
#define _GET_ISP_MODE_(x,model)  (((x)>>model)&_ISP_MODE_BW_)

#define _SET_BIT_(x,model,val)		do{x = ((x)&(~(1<<(model)))); x = ((x)|(((val)&(0x1))<<(model)));}while(0);
#define	_GET_BIT_(x,model)			(((x)>>model)&0x01)

typedef enum
{
	ISP_MODE_BLCEN      =0x00000001, 
	ISP_MODE_LSCEN      =0x00000002,
	ISP_MODE_DPCEN      =0x00000004,
	ISP_MODE_AWBEN      =0x00000008,
	ISP_MODE_CFAEN      =0x00000010,
	ISP_MODE_CCMEN      =0x00000020,
	ISP_MODE_DGAINEN    =0x00000040,
	ISP_MODE_YGAMMAEN   =0x00000080,
	ISP_MODE_RGBGAMMAEN =0x00000100,
	ISP_MODE_CHEN       =0x00000200,
	ISP_MODE_VDEEN      =0x00000400,
	ISP_MODE_SHARPEN    =0x00000800,
	ISP_MODE_CCFEN      =0x00001000,
	ISP_MODE_SAJEN      =0x00002000,
	ISP_MODE_AE_STATEN  =0x00004000,
	ISP_MODE_AWB_STATEN =0x00008000,
	ISP_MODE_JPEGCROP   =0x00010000,
	ISP_MODE_LCDCROP    =0x00020000,
	ISP_MODE_WIFICROP   =0x00040000,
	ISP_MODE_SDEN		=0x00080000,
	ISP_MODE_CSI_SAVEEN	=0x00100000,
	ISP_MODE_JSCALER_SEL=0x00200000,
	ISP_MODE_LSEN		=0x00400000,
		//==JT529X add cfg==
	ISP_MODE_LCD_BURST16_SEL =0x00800000,
	ISP_MODE_WIFI_BURST16_SEL	=0x01000000,
	ISP_MODE_JPEG_BURST16_SEL	=0x02000000,
}ISP_MODE_E;

#define _ISP_BIT_WIDTH_ 2
#define _ISP_MODE_BW_ 0x03
#define _AE_POS_          (0*_ISP_BIT_WIDTH_)
#define _BLC_POS_         (1*_ISP_BIT_WIDTH_)
#define _LSC_POS_    	  (2*_ISP_BIT_WIDTH_)
#define _DDC_POS_         (3*_ISP_BIT_WIDTH_)
#define _AWB_POS_         (4*_ISP_BIT_WIDTH_)
#define _CCM_POS_         (5*_ISP_BIT_WIDTH_)
#define _DGAIN_POS_       (6*_ISP_BIT_WIDTH_)
#define _YGAMA_POS_       (7*_ISP_BIT_WIDTH_)
#define _RGB_GAMA_POS_    (8*_ISP_BIT_WIDTH_)
#define _CH_POS_          (9*_ISP_BIT_WIDTH_)
#define _VDE_POS_         (10*_ISP_BIT_WIDTH_)
#define _EE_POS_          (11*_ISP_BIT_WIDTH_)
#define _CFD_POS_         (12*_ISP_BIT_WIDTH_)
#define _SAJ_POS_         (13*_ISP_BIT_WIDTH_)
#define _OL_DB_POS        (15*_ISP_BIT_WIDTH_)

#define _ISP_DIS_        0x00
#define _ISP_EN_         0x01
#define _ISP_AUTO_       0x02

#define _ISP_FREE_        0x00
#define _ISP_READY_       0x01
#define _ISP_OFF_         0x03

/*
#define _ISP_MODE_DEFAULT_     (_ISP_FREE_<<_AE_POS_       |_ISP_EN_<<_BLC_POS_        |_ISP_DIS_<<_LSC_POS_         \
								|_ISP_AUTO_<<_DDC_POS_ 	   |_ISP_AUTO_<<_AWB_POS_      |_ISP_EN_<<_CCM_POS_         \
								|_ISP_AUTO_<<_DGAIN_POS_     |_ISP_AUTO_<<_YGAMA_POS_    |_ISP_AUTO_<<_RGB_GAMA_POS_  \
								|_ISP_AUTO_<<_CH_POS_        |_ISP_AUTO_<<_VDE_POS_      |_ISP_AUTO_<<_EE_POS_        \
								|_ISP_AUTO_<<_CFD_POS_     |_ISP_AUTO_<<_SAJ_POS_)
*/

#define _ISP_MODE_DEFAULT_     (_ISP_FREE_<<_AE_POS_       |_ISP_EN_<<_BLC_POS_        |_ISP_EN_<<_LSC_POS_         \
								|_ISP_AUTO_<<_DDC_POS_ 	   |_ISP_AUTO_<<_AWB_POS_      |_ISP_DIS_<<_CCM_POS_         \
								|_ISP_AUTO_<<_DGAIN_POS_     |_ISP_AUTO_<<_YGAMA_POS_    |_ISP_AUTO_<<_RGB_GAMA_POS_  \
								|_ISP_DIS_<<_CH_POS_        |_ISP_DIS_<<_VDE_POS_      |_ISP_AUTO_<<_EE_POS_        \
								|_ISP_DIS_<<_CFD_POS_     |_ISP_DIS_<<_SAJ_POS_)

							   



typedef	struct
{
	u32 ylog_cal_fnum;
	u32 exp_tag[8];
	u32 exp_ext_mod;
	u32 exp_gain;
	u32 k_br;
	u32 exp_min;
	u32 gain_max;
	u32 exp_nums;
	u32 gain_max_save;
}EXP;
typedef	struct
{
	u32 allow_miss_dots;
	u32 ae_win_x0;
	u32 ae_win_x1;
	u32 ae_win_x2;
	u32 ae_win_x3;
	u32 ae_win_y0;
	u32 ae_win_y1;
	u32 ae_win_y2;
	u32 ae_win_y3;
	u32 weight_0_7;   
	u32 weight_8_15;  
	u32 weight_16_23;  
	u32 weight_24;  
	u32 hgrm_centre_weight[8]; 
	u32 hgrm_gray_weight[8];  
}HGRM;
typedef	struct
{
	EXP exp_adapt;
	HGRM hgrm_adapt;
}AE;



typedef	struct
{
	int blkl_r;
	int blkl_gr;
	int blkl_gb;
	int blkl_b;
	u8 blk_rate[8];
	u8 step_len;
}BLC;


typedef	struct
{
	u32 hot_num;//5*5?D�䨮����24-hot_num?����??a��?hot_point
	u32 dead_num;//5*5?D�䨮����24-dead_num?����??a��?hot_point
	u32 hot_th;
	u32 dead_th;
	u32 avg_th;
	u32 d_th_rate[8];
	u32 h_th_rate[8];
	u32 dpc_dn_en;
	u32 indx_table[8];
	int indx_adapt[8];
	u32 std_th[7];
	u32 std_th_rate;
	u32 ddc_step;
	u32 ddc_class;
}DDC;
typedef	struct
{
	u32 seg_mode;
	u32 rg_start;
	u32 rgmin;
	u32 rgmax; // 256 -> 1 gain  500 /256 =about 1.9 gain
	u32 weight_in;
	u32 weight_mid;
	u32 ymin;
	u32 ymax;
	u32 hb_rate;
	u32 hb_class;
	u32 hr_rate;
	u32 hr_class;
	u32 awb_scene_mod;
	u32 manu_awb_gain[5];
	u32 yuv_mod_en;
	u32 cb_th[8];
	u32 cr_th[8];
	u32 cbcr_th[8];
	u32 ycbcr_th;
	u32 manu_rgain;
	u32 manu_ggain;
	u32 manu_bgain;
	u32 rgain;
	u32 ggain;
	u32 bgain;
	u16 seg_gain[8][3];
	u8 awb_tab[128];
}AWB; //raw -> calculate

typedef	struct
{
	u16 ccm[9];
	short s41;
	short s42;
	short s43;
}CCM;

typedef struct
{
	u32 dgain[9];
	u32 dgain_rate[8];
}RGB_DGAIN;

typedef	struct
{
	u32 br_mod;
	u32 gma_num[8];
	u32 contra_num;
	u32 bofst;
	u32 lofst;
	u32 lcpr_low; 
	u32 lcpr_high;
	u32 lcpr_llimt;
	u32 lcpr_hlimt;
	u32 pad_num;
	/*ALIGNED(32)*/ u16 using_ygama[_YGAMA_STEPS_*2]; 
}YGAMA;
typedef	struct
{
	u32 br_mod;
	u32 gam_num[8];
	u32 max_oft[8];
	u32 rmin;
	u32 rmax;
	u32 gmin;
	u32 gmax;
	u32 bmin;
	u32 bmax;
	u32 fog_llimt;
	u32 fog_hlimt;
	u32 fog_dotnum;
	ALIGNED(32) u8 using_rgbgama[_RGB_GAMA_STEPS_*2];
}RGB_GAMA;
typedef	struct
{
	u32 stage0_en;//enable r g b
	u32 stage1_en;//enable y c m
	u32 enhence[6];//enhance channel  r b g y c m
	u32 th1[6];//you can set hue width
	u32 th0[6];
	//m_x r_x y_x b_x g_r r_x
	u32 r_rate[6];//combining with sat[],you can enhance or weaken
	u32 g_rate[6];
	u32 b_rate[6];
	u32 sat[17];//16?a1
	u32 rate[8];
	u32 ch_step;
}CH;
typedef	struct
{
	u32 contra;
	u32 bright_k; // 80 -> 1 gain
	u32 bright_oft; // bright_oft * bright_K 
	u32 hue;   
	u32 sat[9];
	u32 sat_rate[8];
	u32 vde_step;
}VDE;
typedef	struct
{
	u8 ee_class;
	
	u8 ee_dn_slope[8];
	u8 ee_sharp_slope[8];
	u8 ee_th_adp[8];
	u8 ee_dn_th[8]; 
	u8 sharp_class[8];
	u8 dn_class[8];
	u8 ee_step;
}EE;
typedef	struct
{
	u32 rate; // (1)  rate / 16 
	u32 ymax;//  (1)  3*3
	u32 th; //   (1)  |cb -cr|
	u32 wdc_en;  // 
	u32 wclass; //ccf_start: wymin - (16<<wclass)   reduce saturation
	u32 wymin; // (2) 
	u32 mean_en; // (3)
	u32 dn_class;
	u32 ccf_en;
}CCF;  
typedef	struct
{
	u8 sat[17];
	u8 sat_rate[8];
	u8 saj_step;
}SAJ;  
typedef	struct
{
	u32 pixel_th;
	u32 num_th;
	u32 update_cnt;
	u32 win_h_start;
	u32 win_h_end;
	u32 win_v_start;
	u32 win_v_end;
}MD;


typedef	struct
{
	volatile  u32*  p_all_mod;
	AE *p_ae;
	//sensor_isp_adapt
	BLC *p_blc;
	DDC *p_ddc;
	AWB *p_awb;
	CCM *p_ccm;
	RGB_DGAIN *p_rgb_dgain;
	YGAMA *p_ygama; 	
	RGB_GAMA *p_rgb_gama;
	CH *p_ch;
	VDE *p_vde;
	EE *p_ee;
	CCF *p_ccf;
	SAJ *p_saj;
	MD *p_md;
}ISP;
typedef struct
{	
	u32 picsize;
	u32* Wp_stat;

	u32 rmin;
	u32 rmax;

	u32 gmin;
	u32 gmax;
	
	u32 bmin;
	u32 bmax;	
	
	u32 ymin;
	u32 ymax;
	u32 br;
	u32 dr_mode;	
	u32 yloga_avg;   //exp -> frame_num light Avg ADD 
	u32 yloga; //  two frame current 
	u32 ylog_buf[32];
	
	u32 ptsum;
	u32 ysum;
	u32 yhgrm_before[256];
	u32 yhgrm[256];
}STAT;

extern ISP isp;
extern STAT ispstat;

extern u8 gama[_RGBGAMA_CLSASSES_][256];// __attribute__ ((section(".res_isp_tab"))) ;
extern u8 color_compress[_YGAMA_CLSASSES_][256];// __attribute__ ((section(".res_isp_tab"))) ;
extern u8 contra_tab[_CONTRA_CLSASSES_][256];// __attribute__ ((section(".res_isp_tab"))) ;

extern u16 Y_GMA[_YGAMA_STEPS_];


extern u8 using_contrast[256];
extern u16 lsc_tab[572]; 
void ax32xx_ispModeSet(u32 mode,u8 en);
void isp_stat_isr_hal(void);

void isp_BLC_cfg(U32 smooth_th, u32 smooth_c,int blkl_gb,int blkl_gr,int blkl_r,int blkl_b);
void isp_LSC_adj_cfg(u16 *p_lsc_tab,u8 first_time);
void isp_LSC_cfg(void *p_lsc_tab);
void isp_DDC_cfg(u32 dpc_dn_en,u32 hot_num,u32 dead_num,u32 hot_th,u32 dead_th,u32 avg_th,
				u32 d_th_rate,u32 h_th_rate,u32* std_th,u32 std_th_rate,u32 *indx_table);
void isp_whtpnt_stat_cfg(u8 *p_wp_tab,u32 seg_mode,u32 rgmin,u32 rgmax,u32 weight_in,u32 weight_mid,u32 ymin,u32 ymax,
						 u32 hb_rate,u32 hb_class,u32 hr_rate,u32 hr_class,u32 yuv_mod_en, u32 rg_start, u32 ycbcr_th,
						 u32 *cb_th, u32 *cr_th, u32 *cbcr_th);
void isp_AWB_adj_cfg(u32 en,u8*p_wp_tab,u32 seg_mode,u32 rgmin,u32 rgmax,
				     u32 weight_in,u32 weight_mid,u32 ymin,u32 ymax,u32 hb_rate,u32 hb_class,u32 hr_rate,u32 hr_class,
					 u32 yuv_mod_en, u32 rg_start, u32 ycbcr_th, u32 *cb_th, u32 *cr_th, u32 *cbcr_th);
void isp_AWB_GAIN_adj(u32 Kb, u32 Kg, u32 Kr);
void isp_CCM_cfg(u16 *ccm,short s41,short s42,short s43);
void isp_RGB_DGAIN_adj(u32 dgain0,u32 dgain1,u32 dgain2,u32 dgain3,u32 dgain4,u32 dgain5,u32 dgain6,u32 dgain7,u32 dgain8);
void isp_hist_stat_cfg(u32 win_x0,u32 win_x1,u32 win_x2,u32 win_x3,u32 win_y0,u32 win_y1,
					   u32 win_y2,u32 win_y3,u32 weight0_7,u32 weight8_15,u32 weight16_23,u32 weight24,u32 *p_csi_hist);
void isp_GAMMA_cfg(u16* p_ygama,u8 mod,u8* p_contra,u32 ymin, u32 ymax,u32 *p_fog_y_low,u32 *p_fog_y_high, u32 pad_num);
void isp_RGB_gam_cfg(u8 *p_rgbgam,u8 mod,u32 max_oft,u32 rmin,u32 rmax,u32 gmin,u32 gmax,u32 bmin,u32 bmax);
void isp_CH_cfg(u32 stage1_en,u32 stage0_en,u32*p_enhence,u32*p_th1,u32*p_th0,u32*p_r_rate,u32*p_g_rate,u32*p_b_rate,u32*p_sat);
void isp_VDE_cfg(u32 contra,u32 bright_oft, u32 bright_k, u32 hue,u32*p_sat);
void isp_EE_cfg(u8 ee_dn_th,u8 ee_dn_slope,u8 ee_sharp_slope,u8 ee_th_adp,u8 sharp_class,u8 dn_class);
void isp_CCF_cfg(u32 ccf_ccf_en,u32 ccf_wdc_en, u32 ccf_wclass, u32 ccf_rate, u32 ccf_wymin,u32 ccf_ymax,u32 ccf_mean_en,u32 ccf_th,u32 dn_class);
void isp_SAJ_cfg(u8 sj_sr0, u8 sj_sr1, u8 sj_sr2, u8 sj_sr3, u8 sj_sr4, u8 sj_sr5, u8 sj_sr6,u8 sj_sr7, u8 sj_sr8,
				 u8 sj_sr9, u8 sj_sr10, u8 sj_sr11, u8 sj_sr12, u8 sj_sr13,u8 sj_sr14, u8 sj_sr15, u8 sj_sr16);


void isp_kick_stat(void);
void stat_cp_kick_st(void);
void stat_cp_done(void);
void isp_stat_en(bool en_ae, bool en_wpt);
void isp_model_cfg(u32 model_num,u32 val);
#endif
