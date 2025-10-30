/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         ADC HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_csi.h
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
#ifndef HAL_CSI_H
     #define HAL_CSI_H

/*sensorµÄ¼Ä´æÆ÷¶ÁÐ´Ö¸Áî£¬ID£¬³õÊ¼»¯±íÖ¸Õë*/
typedef struct {
	u32 rotate;
} Rotate_Adapt;
typedef struct {
	u32 pclk;
	u32 v_len;
	u32 step_val;
	u32 step_max;
	u32 down_fps_mode;//0,1,hvb down_fps; 2: exp down_fps,0xff: turn off down_fps
	u8 fps;
	u8 frequency;
} Hvb_Adapt;

typedef struct 
{
	u32 exp_gain;
	u32 gain_max; 
	u32 id; 
	u32 mclk;
	Hvb_Adapt hvb;
	u16 pixelw;
	u16 pixelh;
	u8 type;//CSI_TYPE_RAW10,CSI_TYPE_RAW8
	u8 hsyn;
	u8 vsyn;
	u8 rduline;
	u8 colrarray;//0:_RGRG_ 1:_GRGR_,2:_BGBG_,3:_GBGB_
	u8 pclk_fir_en;
	u8 pclk_inv_en;
	u8 csi_tun;
	u8 name[20];//最后一个字母是 ^
	u8 exp_gain_en;
	u8 blk_en;
	u8 lsc_en;
	u8 ddc_en;
	u8 awb_en;
	u8 ccm_en;
	u8 dgain_en;
	u8 ygama_en;
	u8 rgb_gama_en;
	u8 ch_en;
	u8 vde_en;
	u8 ee_en;
	u8 cfd_en;
	u8 saj_en;
	u8 pclk_fir_class;
	u8 AVDD;
	u8 DVDD;
	u8 VDDIO;
	u8 rotate;
}COMMON;

typedef struct {
	void (*fp_rotate)(u32 rotate);
	void (*fp_hvblank)(u32 h_len,u32 v_len);
	void (*fp_exp_gain_wr)(u32 exp,u32 gain);
} P_Fun_Adapt;
typedef struct {
	u32 typ; //0: raw 1:yuv 2:mipi
	u32 mclk;
	u32 pclk_fir_en;
	u32 pclk_fir_class;
	u32 pclk_inv_en;
	u32 csi_tun;
	u32 pixelw; 
	u32 pixelh;
	u32 colrarray; 
	u32 hsyn; 
	u32 vsyn;
	u32 AVDD;
	u32 DVDD;
	U32 VDDIO;
	Rotate_Adapt rotate_adapt;
	Hvb_Adapt hvb_adapt;
	volatile u32 isp_all_mod;
	//sens_isp_adapt
	AE ae_adapt;
	BLC blc_adapt;
	DDC ddc_adapt;
	AWB awb_adapt;
	CCM ccm_adapt;
	RGB_DGAIN rgbdgain_adapt;
	YGAMA ygama_adapt; 	
	RGB_GAMA rgbgama_adapt;
	CH ch_adapt;
	VDE vde_adapt;
	EE ee_adapt;
	CCF cfd_adapt;
	SAJ saj_adapt;
	MD md_adapt;
	P_Fun_Adapt p_fun_adapt;
} _Sensor_Adpt_;
typedef struct
{
	void* sensor_struct_addr;
	u32 sensor_struct_size;
	void * sensor_init_tab_adr;
	u32 sensor_init_tab_size;
	void * sensor_init_tab_adr2;
	u32 sensor_init_tab_size2;
	void * rgb_gamma_tab_adr;
	u32 rgb_gamma_tab_size;
	void * ygamma_tab_adr;   
	u32 ygamma_tab_size;
	void * lsc_tab_adr;
	u32 lsc_tab_size;
	void * lsc_tab_adr2;
	u32 lsc_tab_size2;
	u32 awb_tab_size; 
	u8 sensor_name[4*5];
	u8 id,w_cmd,r_cmd,addr_num,data_num;
	u16 id_reg;
}_Sensor_Ident_;
typedef struct
{
	u8 online;
	u32 pclk;
	u32 iclk;
	_Sensor_Ident_ * p_sensor_ident;
	_Sensor_Adpt_ * p_sensor_cmd;
}SNSER;
typedef struct
{
	u32 header_items_addr;
	u32 header_items_total_size;
	u32 header_item_size;
	u32 align[13];
}_Sensor_Header_;

enum{
	pclk_fir_en_pos = 0,
	pclk_fir_class_pos,
	pclk_inv_en_pos,
	csi_tun_pos,
	hsyn_pos,
	vsyn_pos,
	colrarray_pos,
	rduline_pos,
	v_len_pos,
	down_fps_mode_pos,
	fps_pos,
	frequency_pos,
	mclk_pos,
	pclk_pos,
	rotate_pos,
	AVDD_pos,
	DVDD_pos,
	VDDIO_pos,
	common_mode_max
};

extern SNSER snser;

#define RESOLUTION_720P_W	640
#define RESOLUTION_720P_H	360


/*******************************************************************************
* Function Name  : hal_csiMCLKSet
* Description    : hal layer .csi set m-clk
* Input          : int clk : clk frq
* Output         : None
* Return         : none
*******************************************************************************/
void hal_csiMCLKSet(int clk);
/*******************************************************************************
* Function Name  : hal_csiRegister
* Description    : hal layer .csi register
* Input          : sensor_adpt *cmos_sensor : sensor op
* 				   _Sensor_Ident_ *sensor_ident
* Output         : None
* Return         : none
*******************************************************************************/
void hal_csiRegister(_Sensor_Adpt_ *sensor_op,_Sensor_Ident_ *sensor_ident);
/*******************************************************************************
* Function Name  : hal_csiResolutionGet
* Description    : hal layer .get csi resolution
* Input          : u16 *width : width
                      u16 *height : height
* Output         : None
* Return         : int 0 : success
                      int -1: fail
*******************************************************************************/
int hal_csiResolutionGet(u16 *width,u16 *height);
/*******************************************************************************
* Function Name  : hal_csiAdptGet
* Description    : hal layer .get csi adpt
* Input          : 
* Output         : None
* Return         : sensor_adpt *adpt
*******************************************************************************/
_Sensor_Adpt_ *hal_csiAdptGet(void);
/*******************************************************************************
* Function Name  : hal_csi_SeninitGet
* Description    : hal layer .get csi Seninit
* Input          : 
* Output         : None
* Return         : _Sensor_Ident_ *ident
*******************************************************************************/
_Sensor_Ident_ *hal_csi_SeninitGet(void);

#define  hal_csiEnable     ax32xx_csiEnable
void hal_csiHalf720P_ResolutionGet(u16 *width,u16 *height);





#endif
