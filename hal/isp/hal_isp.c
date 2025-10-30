/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                  THE APPOTECH HAL
   **** **                 ISP HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_isp.h
* Author      : 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN ISP HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"
#include <stdlib.h>
typedef struct ISP_CTRL_S
{
	u8  isp_contan_load; 
	u32 *p_isp_ygama_load;
	u32 isp_ygama_br_mod;
	u32 *p_isp_rgbgama_load;
	u32 isp_rgbgama_br_mod;
}ISP_CTRL_T;

extern U32 get_time_for_tfdebg (void);

bool (*fp_sensor_exposure)(u32 yloga_avg,u32 yloga,u32 * p_br,u32 exp_ext_mod);
extern u8 color_compress[15][256];
extern u8 contra_tab[7][256];
static u32 sen_exp,sen_gain,vbr,vyloga,exp_gain_flag = 0;
extern u8 get_sen_mode();
static COMMON common_adapt;
static U32 isp_module_state;
static u32 commom_mode;

static volatile u8 fcnt;
void (*fp_stat_isr)(void);
void (*fp_awb_isr)(void);
void (*fp_ae_isr)(void);

static ISP_CTRL_T ispCtrl;
#if 0//HAL_CFG_ISP_SUPPORT

static ISP isp;
static __CSI_STAT__ csi_stat;

static STAT ispstat;
#endif
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void dely_ms(void)
{
 volatile	u32 i =	 0x7ffff;
	while(--i){
	/*
		if(JPFLG & BIT(4) ){//JEOLDADRD
				JPFLG_CLR |= BIT(4);//JEOLDADRDCLR(mast before upload addr!!!!)
				__FP__(fp_mjp_adrload_hal);	
		}*/
	
	}
}
int hal_ispFrameCnt(u8 clear)
{
	u8 temp = fcnt;

	if(clear)
		fcnt = 0;
	return temp;
}
void isp_isr_hal(void)
{
	
	__FP__(fp_stat_isr);
	__FP__(fp_awb_isr);
	isp_kick_stat();
	if(exp_gain_flag == 1){
		if(pointer_check(&(snser.p_sensor_cmd->p_fun_adapt.fp_exp_gain_wr))){
			(*snser.p_sensor_cmd->p_fun_adapt.fp_exp_gain_wr)(sen_exp,sen_gain);
			}
			//debg("auto br:%d  e:%d  g:%d vyloga:%d\n",vbr,sen_exp,sen_gain,vyloga);
		exp_gain_flag = 0;
	}
	if(COMMOM_ONLEIN_DBG){		
		hal_commom_isr();
	}
}
void isp_stat_isr_hal(void)
{
	if(_GET_ISP_MODE_(*isp.p_all_mod,_AE_POS_) == _ISP_FREE_ ){
		stat_cp_kick_st();
		stat_cp_done();
		_SET_ISP_MODE_(*isp.p_all_mod,_AE_POS_,_ISP_READY_);
	}
	
}

void isp_AWB_auto_adj(void)
{
	u32 i, Kr, Kg, Kb;
	u16 weight[4][8] = {{256,0,0,0,0,0,0,0},{192,64,0,0,0,0,0,0},
					   {80,80,64,32,0,0,0,0},{24,40,64,64,40,24,0,0}};//
	u8 cal_segs[4] = {1,2,4,8};
	u8 segs = cal_segs[isp.p_awb->seg_mode];
	ispstat.Wp_stat = _csi_wptbuf_;
	if(_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_)!= _ISP_AUTO_){
		return;
	}
	Kb=Kg=Kr=0;
	for(i=0;i<segs;i++){
		if(ispstat.Wp_stat[i*4] > 256*8/segs){
			isp.p_awb->seg_gain[i][0] = clip(ispstat.Wp_stat[i*4 + 2] / (ispstat.Wp_stat[i*4 + 3] >>8),0,1023);
			isp.p_awb->seg_gain[i][2] = clip(ispstat.Wp_stat[i*4 + 2] / (ispstat.Wp_stat[i*4 + 1] >>8),0,1023);
			//debg("kb:%d kg:%d kr:%d i:%d\n",isp.p_awb->seg_gain[i][2],isp.p_awb->seg_gain[i][1],isp.p_awb->seg_gain[i][0],i);
		}else{
			isp.p_awb->seg_gain[i][0] = isp.p_awb->seg_gain[(i-1)&(segs-1)][0];
			isp.p_awb->seg_gain[i][2] = isp.p_awb->seg_gain[(i-1)&(segs-1)][2];
		}
		Kb += (isp.p_awb->seg_gain[i][0]*weight[isp.p_awb->seg_mode][i] + 128)>>8;
		Kr += (isp.p_awb->seg_gain[i][2]*weight[isp.p_awb->seg_mode][i] + 128)>>8;//
		//debg("kr:%d kg:%d kb:%d i:%d wsum:%d\n",isp.p_awb->seg_gain[i][2],isp.p_awb->seg_gain[i][1],isp.p_awb->seg_gain[i][0],i,ispstat.Wp_stat[i*4 + 0]);
	}
	if(abs(isp.p_awb->bgain-Kb)+abs(isp.p_awb->rgain-Kr)>=8){
		isp.p_awb->bgain = clip((isp.p_awb->bgain*3 + Kb)/4,0,1023);
		isp.p_awb->ggain = 256;
		isp.p_awb->rgain = clip((isp.p_awb->rgain*3 + Kr)/4,0,1023);
		//debg("        kr:%d kg:%d kb:%d \n",isp.p_awb->rgain,isp.p_awb->ggain,isp.p_awb->bgain);
		isp_AWB_GAIN_adj(isp.p_awb->bgain,isp.p_awb->ggain,isp.p_awb->rgain);
	}
}

void awb_manual_gain_adapt(u32 awb_scene_mod)
{
	if(awb_scene_mod < 7){
		awb_scene_mod = (awb_scene_mod>1)?(awb_scene_mod-2):0;
		u32 * p_manu_awb_gain = isp.p_awb->manu_awb_gain;
		u32 manu_awb_gain = p_manu_awb_gain[awb_scene_mod];
		isp.p_awb->manu_bgain = (manu_awb_gain>>20)&0x3ff;
		isp.p_awb->manu_ggain = (manu_awb_gain>>10)&0x3ff;
		isp.p_awb->manu_rgain = manu_awb_gain&0x3ff;
	}else{
		debg("awb_scene_mod err !!!\n");
	}
	u32 i;
	for(i=0;i<8;i++){
		 isp.p_awb->seg_gain[i][0] = isp.p_awb->manu_bgain;
		 isp.p_awb->seg_gain[i][1] = isp.p_awb->manu_ggain;
		 isp.p_awb->seg_gain[i][2] = isp.p_awb->manu_rgain;
	}
}
void isp_AWB_manual_adj(void)
{
	awb_manual_gain_adapt(isp.p_awb->awb_scene_mod);
	isp_AWB_GAIN_adj(isp.p_awb->manu_bgain,isp.p_awb->manu_ggain,isp.p_awb->manu_rgain);
}
void isp_AWB_adj_cfg(u32 en,u8*p_wp_tab,u32 seg_mode,u32 rgmin,u32 rgmax,
				     u32 weight_in,u32 weight_mid,u32 ymin,u32 ymax,u32 hb_rate,u32 hb_class,u32 hr_rate,u32 hr_class,
					 u32 yuv_mod_en, u32 rg_start, u32 ycbcr_th, u32 *cb_th, u32 *cr_th, u32 *cbcr_th)
{
	if(en == _ISP_AUTO_){
		
		isp_whtpnt_stat_cfg(p_wp_tab,seg_mode,rgmin,rgmax,weight_in,weight_mid,ymin,ymax,hb_rate,hb_class,hr_rate,hr_class,
							yuv_mod_en, rg_start, ycbcr_th, cb_th, cr_th, cbcr_th);
		
		awb_manual_gain_adapt(isp.p_awb->awb_scene_mod);
	}
	if(fp_awb_isr == NULL){
	
		fp_awb_isr = isp_AWB_auto_adj;
	
		isp_AWB_manual_adj();
	}
}
void isp_AE_adj_cfg(u32 win_x0,u32 win_x1,u32 win_x2,u32 win_x3,u32 win_y0,u32 win_y1,
					   u32 win_y2,u32 win_y3,u32 weight_0_7,u32 weight_8_15,u32 weight_16_23,u32 weight_24)
{
	isp_hist_stat_cfg(win_x0,win_x1,win_x2,win_x3,win_y0,win_y1,win_y2,win_y3,weight_0_7,weight_8_15,weight_16_23,weight_24,_csi_histbuf_);
	fp_sensor_exposure = exposal_adj;
}
void isp_LSC_adj_cfg(u16 *p_lsc_tab,u8 first_time){
	_Sensor_Ident_ * p_sen_ident  = hal_csi_SeninitGet();
	if(first_time){
		if (1==get_sen_mode())
		{
			hal_spiFlashRead(SEN_RES_FLASH_ADDR(p_sen_ident->lsc_tab_adr2),(u32)p_lsc_tab,p_sen_ident->lsc_tab_size2);
		}
		else
		{
		hal_spiFlashRead(SEN_RES_FLASH_ADDR(p_sen_ident->lsc_tab_adr),(u32)p_lsc_tab,p_sen_ident->lsc_tab_size);
		}
	}
	isp_LSC_cfg(p_lsc_tab);
}
void awb_scene_mod_set(u32 awb_scene_mod)
{
	debg("awb_scene_mod:%d\n",awb_scene_mod);
	if(awb_scene_mod == 0){
		_SET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_,_ISP_AUTO_);
		isp.p_awb->awb_scene_mod = 1;
		isp_model_cfg(_AWB_POS_,(_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_)));
	}else if(awb_scene_mod < 6){
		_SET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_,_ISP_EN_);
		isp.p_awb->awb_scene_mod = awb_scene_mod+1;
		isp_model_cfg(_AWB_POS_,(_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_)));
		isp_AWB_manual_adj();
	}else{
		_SET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_,_ISP_DIS_);
		isp_model_cfg(_AWB_POS_,(_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_)));
	}
	
	
	isp_AWB_adj_cfg(_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_),(u8*)isp.p_awb->awb_tab,isp.p_awb->seg_mode,isp.p_awb->rgmin,isp.p_awb->rgmax,
					isp.p_awb->weight_in,isp.p_awb->weight_mid,isp.p_awb->ymin,isp.p_awb->ymax,isp.p_awb->hb_rate,isp.p_awb->hb_class,isp.p_awb->hr_rate,isp.p_awb->hr_class,
					isp.p_awb->yuv_mod_en, isp.p_awb->rg_start, isp.p_awb->ycbcr_th, isp.p_awb->cb_th, isp.p_awb->cr_th,isp.p_awb->cbcr_th);
}

void isp_contra_table_load(u32 contra_num)
{
	if(contra_num<_CONTRA_CLSASSES_){
		u32  contra_tab_flash_adr = SEN_RES_FLASH_ADDR(contra_tab);
		hal_spiFlashRead(contra_tab_flash_adr+contra_num*256,(u32)using_contrast,256);
	}
	if(!(data_check(using_contrast,0,32)&&data_check(&using_contrast[255],224,255))){
		debg("using_contrast resource_load err");
		//while(1);
	}
	return;	
}
void isp_rgbgma_table_load(u32 rgbgma_num0,u32 rgbgma_num1)
{
	_Sensor_Ident_ * p_sen_ident  = hal_csi_SeninitGet();
	u32  gam_tab_flash_adr,size;
	size = sizeof(isp.p_rgb_gama->using_rgbgama)/2;
	gam_tab_flash_adr = (p_sen_ident->rgb_gamma_tab_adr == NULL) ? (SEN_RES_FLASH_ADDR(gama)) : SEN_RES_FLASH_ADDR(p_sen_ident->rgb_gamma_tab_adr);
	hal_spiFlashRead(gam_tab_flash_adr+rgbgma_num0*size,(u32)isp.p_rgb_gama->using_rgbgama,size);
	hal_spiFlashRead(gam_tab_flash_adr+rgbgma_num1*size,(u32)isp.p_rgb_gama->using_rgbgama+size,size);
	if(!(data_check(isp.p_rgb_gama->using_rgbgama,0,32)&&data_check(&isp.p_rgb_gama->using_rgbgama[511],224,255))){
		//debg("isp.p_rgb_gama->using_rgbgama resource_load err");
		//while(1);
	}
	return;
}
void isp_gma_table_load(u32 gma_num0,u32 gma_num1){
	_Sensor_Ident_ * p_sen_ident  = hal_csi_SeninitGet();
	u32  gam_tab_flash_adr,size;
	size = sizeof(isp.p_ygama->using_ygama)/2;
	gam_tab_flash_adr = (p_sen_ident->ygamma_tab_adr == NULL) ? (SEN_RES_FLASH_ADDR(gama)) : SEN_RES_FLASH_ADDR(p_sen_ident->ygamma_tab_adr);
	hal_spiFlashRead(gam_tab_flash_adr+gma_num0*size,(u32) isp.p_ygama->using_ygama,size);
	hal_spiFlashRead(gam_tab_flash_adr+gma_num1*size,(u32) isp.p_ygama->using_ygama+size,size);
	if(!(data_check(isp.p_ygama->using_ygama,0,64<<(_YGAMA_VAL_BIT_ - 8))&&data_check(&isp.p_ygama->using_ygama[_YGAMA_STEPS_*2 - 1],224<<(_YGAMA_VAL_BIT_ - 8),255<<(_YGAMA_VAL_BIT_ - 8)))){
		//debg("isp.p_ygama->using_ygama resource_load err");
		//while(1);
	}
	return;
}

void isp_table_load(u32 gma_num,u32 contra_num,u32 rgbgma_num)
{
	isp_gma_table_load(gma_num,gma_num);
	isp_rgbgma_table_load(rgbgma_num,rgbgma_num);
	isp_contra_table_load(contra_num);
}
void sensor_isp_adapt()
{
	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	isp.p_all_mod = &(csiSensor->isp_all_mod);
	isp.p_ae = &(csiSensor->ae_adapt);
	isp.p_blc = &(csiSensor->blc_adapt);
	isp.p_ddc = &(csiSensor->ddc_adapt);
	isp.p_awb = &(csiSensor->awb_adapt);
	isp.p_ccm = &(csiSensor->ccm_adapt);
	isp.p_rgb_dgain = &csiSensor->rgbdgain_adapt;
	isp.p_ygama = &(csiSensor->ygama_adapt);
	isp.p_rgb_gama = &(csiSensor->rgbgama_adapt);
	isp.p_ch = &(csiSensor->ch_adapt);
	isp.p_vde = &(csiSensor->vde_adapt);
	isp.p_ee = &(csiSensor->ee_adapt);
	isp.p_ccf = &(csiSensor->cfd_adapt);
	isp.p_saj = &(csiSensor->saj_adapt);
	isp.p_md = &(csiSensor->md_adapt);

	void init_common_str();
	init_common_str();

}
void isp_para_init(void){
	sensor_isp_adapt();
}
void isp_para_config(bool gama_cfg_en){
	isp_BLC_cfg(0,0,isp.p_blc->blkl_gb,isp.p_blc->blkl_gr,isp.p_blc->blkl_r,isp.p_blc->blkl_b);
	isp_LSC_adj_cfg((u16*)lsc_tab,1);
	isp_DDC_cfg(isp.p_ddc->dpc_dn_en,isp.p_ddc->hot_num,isp.p_ddc->dead_num,isp.p_ddc->hot_th,isp.p_ddc->dead_th,isp.p_ddc->avg_th,
				isp.p_ddc->d_th_rate[7],isp.p_ddc->h_th_rate[7],isp.p_ddc->std_th,isp.p_ddc->std_th_rate,isp.p_ddc->indx_table);
	isp_AWB_adj_cfg(_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_),(u8*)isp.p_awb->awb_tab,isp.p_awb->seg_mode,isp.p_awb->rgmin,isp.p_awb->rgmax,
					isp.p_awb->weight_in,isp.p_awb->weight_mid,isp.p_awb->ymin,isp.p_awb->ymax,isp.p_awb->hb_rate,isp.p_awb->hb_class,isp.p_awb->hr_rate,isp.p_awb->hr_class,
					isp.p_awb->yuv_mod_en,isp.p_awb->rg_start, isp.p_awb->ycbcr_th, isp.p_awb->cb_th, isp.p_awb->cr_th,isp.p_awb->cbcr_th);
	isp_CCM_cfg( isp.p_ccm->ccm,isp.p_ccm->s41,isp.p_ccm->s42,isp.p_ccm->s43);
	isp_AE_adj_cfg(isp.p_ae->hgrm_adapt.ae_win_x0,isp.p_ae->hgrm_adapt.ae_win_x1,isp.p_ae->hgrm_adapt.ae_win_x2,isp.p_ae->hgrm_adapt.ae_win_x3,
					isp.p_ae->hgrm_adapt.ae_win_y0,isp.p_ae->hgrm_adapt.ae_win_y1,isp.p_ae->hgrm_adapt.ae_win_y2,isp.p_ae->hgrm_adapt.ae_win_y3,
					isp.p_ae->hgrm_adapt.weight_0_7,isp.p_ae->hgrm_adapt.weight_8_15,isp.p_ae->hgrm_adapt.weight_16_23,isp.p_ae->hgrm_adapt.weight_24);
	isp_RGB_DGAIN_adj(64,64,64,64,64,64,64,64,64);
	
	if(gama_cfg_en){
	    isp_table_load(isp.p_ygama->gma_num[7],isp.p_ygama->contra_num,isp.p_rgb_gama->gam_num[7]);
		isp_GAMMA_cfg(isp.p_ygama->using_ygama,isp.p_ygama->br_mod,using_contrast,isp.p_ygama->bofst,isp.p_ygama->lofst,NULL,NULL,isp.p_ygama->pad_num);
		isp_RGB_gam_cfg(isp.p_rgb_gama->using_rgbgama,isp.p_rgb_gama->br_mod,isp.p_rgb_gama->max_oft[7],isp.p_rgb_gama->rmin,isp.p_rgb_gama->rmax,isp.p_rgb_gama->gmin,isp.p_rgb_gama->gmax,isp.p_rgb_gama->bmin,isp.p_rgb_gama->bmax);
	}
	isp_CH_cfg(isp.p_ch->stage1_en,isp.p_ch->stage0_en,isp.p_ch->enhence,isp.p_ch->th1,isp.p_ch->th0,isp.p_ch->r_rate,isp.p_ch->g_rate,isp.p_ch->b_rate,isp.p_ch->sat);
	isp_VDE_cfg(isp.p_vde->contra,isp.p_vde->bright_oft,isp.p_vde->bright_k,isp.p_vde->hue,isp.p_vde->sat);
	isp_EE_cfg(isp.p_ee->ee_dn_th[7],isp.p_ee->ee_dn_slope[7],isp.p_ee->ee_sharp_slope[7],isp.p_ee->ee_th_adp[7],isp.p_ee->sharp_class[7],isp.p_ee->dn_class[7]);
	isp_CCF_cfg(isp.p_ccf->ccf_en,isp.p_ccf->wdc_en, isp.p_ccf->wclass, isp.p_ccf->rate, isp.p_ccf->wymin,isp.p_ccf->ymax,isp.p_ccf->mean_en,isp.p_ccf->th,isp.p_ccf->dn_class);
	isp_SAJ_cfg(isp.p_saj->sat[0],isp.p_saj->sat[1], isp.p_saj->sat[2], isp.p_saj->sat[3], isp.p_saj->sat[4], isp.p_saj->sat[5], isp.p_saj->sat[6],isp.p_saj->sat[7], isp.p_saj->sat[8],
			   isp.p_saj->sat[9], isp.p_saj->sat[10], isp.p_saj->sat[11], isp.p_saj->sat[12], isp.p_saj->sat[13],isp.p_saj->sat[14], isp.p_saj->sat[15], isp.p_saj->sat[16]);
//	md_init(isp.md->pixel_th,isp.md->num_th,isp.md->update_cnt,isp.md->win_h_start,isp.md->win_h_end,isp.md->win_v_start,isp.md->win_v_end);//*/	
    isp_stat_en(1,1);
}
void isp_init(void)
{   
	isp_para_init();
	isp_para_config(true);
	isp_all_model_cfg(*isp.p_all_mod);
	ispCtrl.isp_contan_load = 0xff;
	ispCtrl.p_isp_ygama_load = NULL;
	ispCtrl.p_isp_rgbgama_load = NULL;
	ax32xx_csiISRRegiser(CSI_IRQ_ISP,isp_isr_hal);
	fp_stat_isr = isp_stat_isr_hal; 
    fp_sensor_exposure = exposal_adj;
	deg_Printf("CSIMODE:0x%x\n",CSIMODE);
	deg_Printf("ISPMODE:0x%x\n",ISPMODE);
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

u32 dot_num_stat(u32 * phgrm,u16 y_start,u16 len){
	u32 i,dotsum = 0;
	for(i = 0;i<len;i++){
		dotsum += (phgrm[y_start + i]>>4);
	}
	return dotsum>>4;
}
u32 y_sum_stat(u32 * phgrm,u8 * p_ln,u16 y_start,u16 len){
	u32 i,ysum = 0;
	for(i = 0;i<len;i++){
		if(NULL != p_ln){// ?????? // ????
			ysum += p_ln[y_start + i] * (phgrm[y_start + i]>>4);
		}else{
			ysum += (y_start + i) * (phgrm[y_start + i]>>4);
		}
	}
	return ysum>>4;
}
void find_peak(u32 * phgrm,u32 y_start,u8 y_step,u8 width,u32 *p_ipeak,u32 *p_peak_num){
	u16 limit,i;
	u32 dot_num;
	if(y_start == 0){
		limit = 128;
	}else{
		limit = 256;
	}
	for(i = y_start; (i+width) <= limit; i += y_step){
		dot_num = dot_num_stat(phgrm,i,width);
		if(dot_num > (*p_peak_num)){
			*p_peak_num = dot_num;
			*p_ipeak = i;
		}
	}
}
void find_y_of_dotsum(u32 * phgrm,u32 *p_fog_y_low,u32 *p_fog_y_high){
	u32 i,dot_num = 0;
	for(i = 0; ((dot_num <= isp.p_rgb_gama->fog_dotnum)&&(i <= 255)); i++){//4000
		dot_num += (phgrm[i]);
	}
	*p_fog_y_low = i - 1;
	dot_num = 0; 
	for(i = 255; ((dot_num <= isp.p_rgb_gama->fog_dotnum)&&(i >= 1)); i--){
		dot_num += (phgrm[i]);
	}
	*p_fog_y_high = i + 1;
	return;
}
u32 get_isp_adapt_val(u32 * pstep_val,u32 index){
	u32 step,mod,val;
	index = clip(index,0,255);
	step = index / 32;
	mod = index % 32;
	if(step < 7){
		val = (pstep_val[step]*(32 - mod) + pstep_val[step+1]*mod)>>5; 
	}else{
		val = pstep_val[7];
	}
	return val;
}

u8 get_isp_adapt_val_u8(u8 * pstep_val,u8 index){
	u32 step,mod,val;
	index = clip(index,0,255);
	step = index / 32;
	mod = index % 32;
	if(step < 7){
		val = (pstep_val[step]*(32 - mod) + pstep_val[step+1]*mod)>>5; 
	}else{
		val = pstep_val[7];
	}
	return val;
}

bool soft_exp(STAT *pstat,u8 Br){
	u32 i,fog_y_low,fog_y_high;
	static u16 former_ylog = 256,contra_num = 0;
    static u32 ygama_num[2] = {0,0}, ygama_temp[2] = {0,0},rgbgama_num[2] = {0,0}, rgbgama_temp[2] = {0,0};
	find_y_of_dotsum(pstat->yhgrm,&fog_y_low,&fog_y_high);

	u32 rgb_dgain = get_isp_adapt_val(isp.p_rgb_dgain->dgain_rate,clip(Br *4,0,255)); 
	if(_GET_ISP_MODE_(*isp.p_all_mod,_DGAIN_POS_) == _ISP_AUTO_){
		u32 dgain[9];
		for(i = 0; i < 9; i ++){
			dgain[i] = rgb_dgain*isp.p_rgb_dgain->dgain[i]>>6;
		}
		isp_RGB_DGAIN_adj(dgain[0],dgain[1],dgain[2],dgain[3],dgain[4],dgain[5],dgain[6],dgain[7],dgain[8]);
	}

	if(_GET_ISP_MODE_(*isp.p_all_mod,_YGAMA_POS_) == _ISP_AUTO_){ 
		u32 dr_mode = clip(pstat-> dr_mode>>4,0,isp.p_ygama->contra_num); // ?????
		if(contra_num!= dr_mode){
			contra_num = dr_mode;
			ispCtrl.isp_contan_load = contra_num; // ispCtrl.isp_contan_load  ???????flash???? // ???ff??,????????ff
	    }
		// ???
		isp_GAMMA_cfg(isp.p_ygama->using_ygama,isp.p_ygama->br_mod&0x1c,using_contrast,isp.p_ygama->bofst,isp.p_ygama->lofst,&fog_y_low,&fog_y_high, isp.p_ygama->pad_num);
		ygama_temp[0]= get_isp_adapt_val(isp.p_ygama->gma_num,Br); // ?????????
		ygama_temp[1]= get_isp_adapt_val(isp.p_ygama->gma_num,Br+32);//??????????????
		if(((ygama_num[0]!=ygama_temp[0])||(ygama_num[1]!=ygama_temp[1]))){ // ?????????
			ygama_num[0] = ygama_temp[0];
			ygama_num[1] = ygama_temp[1];
			ispCtrl.p_isp_ygama_load = ygama_num; // ???????(????????index?brmode)
			ispCtrl.isp_ygama_br_mod = Br%32; // ???????
			debg("ygam:%d %d\n",ygama_num[0],dr_mode);
		}else{
			isp.p_ygama->br_mod = Br%32;
		}
	}else{ 
		fog_y_low = Y_GMA[fog_y_low];
		fog_y_high = Y_GMA[fog_y_high];
	}
	
	if((_GET_ISP_MODE_(*isp.p_all_mod,_RGB_GAMA_POS_) == _ISP_AUTO_)){
		fog_y_high+=64;
		u32 temp1 = (96-clip(Br,0,64))>>5; 
		u32 temp2 = ((isp.p_ccm->s41>>2) + (isp.p_ccm->s42>>1) + (isp.p_ccm->s43>>2)) / 4; 
		u32 temp = temp1+temp2*rgb_dgain / 64; 
		if(Br>28)
            fog_y_low=28;
        else
            fog_y_low=Br;
        
        isp.p_rgb_gama->rmin = isp.p_rgb_gama->gmin = isp.p_rgb_gama->bmin = fog_y_low;
	    // fog_y_low = clip(fog_y_low,Y_GMA[temp]/4,255) - (Y_GMA[temp]/4 - Y_GMA[temp - 3]/4); 
		// isp.p_rgb_gama->rmin = isp.p_rgb_gama->gmin = isp.p_rgb_gama->bmin = clip((isp.p_rgb_gama->bmin*5+fog_y_low*3)/8,0,isp.p_rgb_gama->fog_llimt); 
		isp.p_rgb_gama->rmax = isp.p_rgb_gama->gmax = isp.p_rgb_gama->bmax = clip((isp.p_rgb_gama->bmax*5+fog_y_high*3)/8,isp.p_rgb_gama->fog_hlimt,255);
		u32 max_oft = get_isp_adapt_val(isp.p_rgb_gama->max_oft,Br);
		isp_RGB_gam_cfg(isp.p_rgb_gama->using_rgbgama,isp.p_ygama->br_mod&0x1c,max_oft,isp.p_rgb_gama->rmin,isp.p_rgb_gama->rmax,isp.p_rgb_gama->gmin,isp.p_rgb_gama->gmax,isp.p_rgb_gama->bmin,isp.p_rgb_gama->bmax);
		rgbgama_temp[0]= get_isp_adapt_val(isp.p_rgb_gama->gam_num,Br); 
		rgbgama_temp[1]= get_isp_adapt_val(isp.p_rgb_gama->gam_num,Br+32); 
		if(((rgbgama_num[0]!=rgbgama_temp[0])||(rgbgama_num[1]!=rgbgama_temp[1]))){ //??????,??????
			rgbgama_num[0] = rgbgama_temp[0];
			rgbgama_num[1] = rgbgama_temp[1];
			ispCtrl.p_isp_rgbgama_load = rgbgama_num;
			ispCtrl.isp_rgbgama_br_mod = Br%32;
			debg("rgbgam:%d\n",rgbgama_num[0]);
		}else{
			isp.p_rgb_gama->br_mod = Br%32;
		}
	}
	if(CMOS_USB_ONLINE_DBG){
		hal_isp_sof_loop();
	}
	
	if((abs(former_ylog - pstat-> yloga_avg) > 0)){
		former_ylog = pstat->yloga_avg;
		return 1;
	}
	
	return 0;
}
#define  _Hgrm_EXPAN_  2
void hgrm_stat(STAT *pstat,u32 Br)
{
	u32 i,j;
	pstat->picsize = 1440000 >> 4;
	u32 weight_8_15 = isp.p_ae->hgrm_adapt.weight_8_15&(~0xf0000);
	weight_8_15 |= (get_isp_adapt_val(isp.p_ae->hgrm_adapt.hgrm_centre_weight,Br) & 0xf0000);
	isp_AE_adj_cfg(isp.p_ae->hgrm_adapt.ae_win_x0,isp.p_ae->hgrm_adapt.ae_win_x1,isp.p_ae->hgrm_adapt.ae_win_x2,isp.p_ae->hgrm_adapt.ae_win_x3,
					isp.p_ae->hgrm_adapt.ae_win_y0,isp.p_ae->hgrm_adapt.ae_win_y1,isp.p_ae->hgrm_adapt.ae_win_y2,isp.p_ae->hgrm_adapt.ae_win_y3,
					isp.p_ae->hgrm_adapt.weight_0_7,weight_8_15,isp.p_ae->hgrm_adapt.weight_16_23,isp.p_ae->hgrm_adapt.weight_24);
	for(i=0;i<128;i++){ 
		pstat-> yhgrm[(127-i)*_Hgrm_EXPAN_] = _csi_histbuf_[(127-i)]*get_isp_adapt_val(isp.p_ae->hgrm_adapt.hgrm_gray_weight,(127-i)*2)>>3;
		for(j=1;j<_Hgrm_EXPAN_;j++){
			pstat-> yhgrm[(127-i)*_Hgrm_EXPAN_+j] = 0;
		}
	}
	pstat->ptsum = dot_num_stat(pstat->yhgrm,0,256); 
	pstat->ysum = y_sum_stat( pstat->yhgrm,NULL,0,256);
}
void isp_calculate(STAT *pstat,u8 Br)
{
	#define _WIDTH_ 16
	u32 i;
	for(i=0;i<(isp.p_ae->exp_adapt.ylog_cal_fnum-1);i++){ 
		pstat-> ylog_buf[i] = pstat-> ylog_buf[i+1];
	}
	u32 high_dot = dot_num_stat(pstat-> yhgrm,224,32);
	u32 high_sum = y_sum_stat( pstat-> yhgrm,NULL,224,32);
	u32 low_dot = dot_num_stat(pstat-> yhgrm,0,8);
	u32 low_sum = y_sum_stat( pstat-> yhgrm,NULL,0,8);
	pstat-> dr_mode = ((clip(Br+1,128,256)-128)*low_dot / pstat->picsize + pstat-> dr_mode*3)/4;
	pstat-> ylog_buf[isp.p_ae->exp_adapt.ylog_cal_fnum-1] = (pstat->ysum *16 +high_sum*0 +low_sum*0)/ (pstat-> ptsum*16 + high_dot*0 + low_dot*0);
	pstat->yloga_avg = 0;
	for(i=0;i<isp.p_ae->exp_adapt.ylog_cal_fnum;i++){
		pstat->yloga_avg += pstat-> ylog_buf[i];
	}
	pstat->yloga_avg = pstat->yloga_avg / isp.p_ae->exp_adapt.ylog_cal_fnum;
	pstat->yloga = (pstat->yloga + pstat-> ylog_buf[isp.p_ae->exp_adapt.ylog_cal_fnum-1])>>1;
}
void isp_adjust(STAT *pstat,u32 *p_Br){
	if((pstat->yloga_avg == 256)){
		return;
	}
	u32 i;
	if(pointer_check(&fp_sensor_exposure)){ 
		(*fp_sensor_exposure)(pstat->yloga_avg,pstat->yloga,p_Br,isp.p_ae->exp_adapt.exp_ext_mod);//3
		soft_exp(pstat,*p_Br);
	}
	if(_GET_ISP_MODE_(*isp.p_all_mod,_BLC_POS_) == _ISP_AUTO_){
		int blk_r,blk_gr,blk_gb,blk_b,blk_rate;
		blk_rate = vyloga/isp.p_blc->step_len;
		blk_rate = clip(blk_rate,0,7);
		blk_rate = isp.p_blc->blk_rate[blk_rate];
		blk_r = isp.p_blc->blkl_r*blk_rate>>3;
		blk_gr = isp.p_blc->blkl_gr*blk_rate>>3;
		blk_gb = isp.p_blc->blkl_gb*blk_rate>>3;
		blk_b = isp.p_blc->blkl_b*blk_rate>>3;
		isp_BLC_cfg(0,0,blk_gb,blk_gr,blk_r,blk_b);
	}
	if(_GET_ISP_MODE_(*isp.p_all_mod,_CH_POS_) == _ISP_AUTO_){
		u32 ch_rate,ch_sat[17];
		u32 r_rate[6];
		u32 g_rate[6];
		u32 b_rate[6];
		ch_rate = vyloga/isp.p_ch->ch_step;
		ch_rate = clip(ch_rate,0,7);
		ch_rate = isp.p_ch->rate[ch_rate];
		for(i = 0;i < 17;i ++){
			ch_sat[i] = isp.p_ch->sat[i] * ch_rate >>4;
			
		}
		for(i = 0;i < 6;i++){
			r_rate[i] = isp.p_ch->r_rate[i] * ch_rate >>4;
			g_rate[i] = isp.p_ch->g_rate[i] * ch_rate >>4;
			b_rate[i] = isp.p_ch->b_rate[i] *ch_rate >>4;
		}
		isp_CH_cfg(isp.p_ch->stage1_en,isp.p_ch->stage0_en,isp.p_ch->enhence,isp.p_ch->th1,isp.p_ch->th0,r_rate,g_rate,b_rate,ch_sat);
	}
	if(_GET_ISP_MODE_(*isp.p_all_mod,_VDE_POS_) == _ISP_AUTO_){//vde?????  BR
		u32 sat_rate,sat[9];
		sat_rate = vyloga/isp.p_vde->vde_step;
		sat_rate = clip(sat_rate,0,7);
		sat_rate = isp.p_vde->sat_rate[sat_rate];
		for(i = 0;i< 9;i++){
			sat[i] = isp.p_vde->sat[i]*sat_rate>>4;
		}
		isp_VDE_cfg(isp.p_vde->contra,isp.p_vde->bright_oft,isp.p_vde->bright_k,isp.p_vde->hue,sat);
	}
	if(_GET_ISP_MODE_(*isp.p_all_mod,_EE_POS_) == _ISP_AUTO_){//ee ÊÇ·ñ×ÔÊÊÓ¦  BR
		u8 ee_dn_slope = get_isp_adapt_val_u8(isp.p_ee->ee_dn_slope,*p_Br);
		u8 ee_sharp_slope = get_isp_adapt_val_u8(isp.p_ee->ee_sharp_slope,*p_Br);
		u8 ee_th_adp = get_isp_adapt_val_u8(isp.p_ee->ee_th_adp,*p_Br);
		u8 ee_dn_th = get_isp_adapt_val_u8(isp.p_ee->ee_dn_th,*p_Br);
		u8 sharp_class = get_isp_adapt_val_u8(isp.p_ee->sharp_class,*p_Br);
		u8 dn_class = get_isp_adapt_val_u8(isp.p_ee->dn_class,*p_Br);
		isp_EE_cfg(ee_dn_th,ee_dn_slope,ee_sharp_slope,ee_th_adp,sharp_class,dn_class);
	}
	if(_GET_ISP_MODE_(*isp.p_all_mod,_DDC_POS_) == _ISP_AUTO_){//ddc ?????  br
		u32 d_th_rate = get_isp_adapt_val(isp.p_ddc->d_th_rate,*p_Br);
		u32 h_th_rate = get_isp_adapt_val(isp.p_ddc->h_th_rate,*p_Br);
		int index_adapt  = isp.p_ddc->indx_adapt[clip((vyloga/isp.p_ddc->ddc_step),0,7)];
		int indx_tab[8];
		for(i = 0; i < 8; i ++){
			indx_tab[i] = clip(((int)isp.p_ddc->indx_table[i]+index_adapt),0,7);
		}
//		debg(" %d %d %d %d %d %d %d %d %d\n",index_adapt,indx_tab[0],indx_tab[1],indx_tab[2]
//		,indx_tab[3],indx_tab[4],indx_tab[5],indx_tab[6],indx_tab[7]);
								
		isp_DDC_cfg(isp.p_ddc->dpc_dn_en,isp.p_ddc->hot_num,isp.p_ddc->dead_num,isp.p_ddc->hot_th,isp.p_ddc->dead_th,isp.p_ddc->avg_th,
				d_th_rate,h_th_rate,isp.p_ddc->std_th,isp.p_ddc->std_th_rate,(u32 *)indx_tab);
	}
	if(_GET_ISP_MODE_(*isp.p_all_mod,_SAJ_POS_) == _ISP_AUTO_){
		u8 sat_rate = vyloga/isp.p_saj->saj_step;
		sat_rate = clip(sat_rate,0,7);
		sat_rate = isp.p_saj->sat_rate[sat_rate];
		u8 saj[17];
		for(i = 0;i<17;i++){
			saj[i] = sat_rate * isp.p_saj->sat[i] >> 4;
		}
		isp_SAJ_cfg(saj[0],saj[1],saj[2],saj[3],saj[4],saj[5],saj[6],saj[7],saj[8],saj[9],saj[10],saj[11],saj[12],saj[13],saj[14],saj[15],saj[16]);
	}
}
void sensor_frame_rate_adapt_for_dark(u32 Br){
	static u32 down_fps_mode = 0;
	if(Br < 16){ 
		down_fps_mode = 1;
	}else if(Br > 32){
		down_fps_mode = 0;
	}
	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	if((down_fps_mode != csiSensor->hvb_adapt.down_fps_mode)&&(csiSensor->hvb_adapt.down_fps_mode<2)){
		csiSensor->hvb_adapt.down_fps_mode = down_fps_mode;
		sensor_frame_rate_adapt(0xff,0);
	}
}
void isp_process(STAT *pstat)
{
	if(pstat != &ispstat) 
	{
		debg("sasdf");while(1);
	}
	hgrm_stat(pstat,pstat->br);
	isp_calculate(pstat,pstat->br); 
	sensor_frame_rate_adapt_for_dark(pstat->br); 
	isp_adjust(pstat,&pstat->br); 
	#if MJPEG_SRC == MJPEG_SRC_RAM
	#endif
}
void isp_process_check(void){
	#if 0
	static int cnt = 0;
	cnt ++;
	if(cnt == 50){
		extern u32 hal_csi_fcnt_get(void);
		debg("f=%d\n",hal_csi_fcnt_get());
		cnt = 0;
	}
	#endif
	if(isp.p_all_mod){ 
		if(_GET_ISP_MODE_(*isp.p_all_mod,_AE_POS_) == _ISP_READY_) 
		{
			isp_process(&ispstat); 
			_SET_ISP_MODE_(*isp.p_all_mod,_AE_POS_,_ISP_FREE_);
		}
	}
}
void sensor_compensate_exp_set(u32 compensate_mod){
	u32  valve;
	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	valve = csiSensor->ae_adapt.exp_adapt.exp_nums>>16 ;
	if (valve){
		switch(compensate_mod){
			case  3 : csiSensor->vde_adapt.bright_oft = 0x80;break;
			case  2 : csiSensor->vde_adapt.bright_oft = 0x70;break;
			case  1: csiSensor->vde_adapt.bright_oft = 0x60;break;
			case  4: csiSensor->vde_adapt.bright_oft = 0xa0;break;
			case  5 : csiSensor->vde_adapt.bright_oft = 0xc0;break;
			default : csiSensor->vde_adapt.bright_oft = 0x80;break;	
			}
		}
	else{			
		csiSensor->ae_adapt.exp_adapt.exp_ext_mod += compensate_mod&0x7;
		csiSensor->ae_adapt.exp_adapt.exp_ext_mod = clip(csiSensor->ae_adapt.exp_adapt.exp_ext_mod,3,9) - 3;
		}
}




/*******************************************************************************
* Function Name  : isp_get_scen_light
* Description    : get light level from sensor
* Input          : none
* Output         : none                                            
* Return         : level value(0~255)  0: max dark  255: max light
*******************************************************************************/
u8 isp_get_scen_light(void){
	return (u8)ispstat.br;
}

void sensor_frame_rate_adapt(u32 frequecy_mode,u32 frame_rate){
	//frequecy_mode  0:50Hz  1:60Hz , 1~255 :down_fps
	static u32 frequecy,frame_rate_temp;
	u32 h_len,pclk,frame_rate_final;
	if(frequecy_mode < 2){
		frequecy = 50+10*frequecy_mode;
		frame_rate_temp = frame_rate;
	}
	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	pclk = csiSensor->hvb_adapt.pclk;
	csiSensor->hvb_adapt.step_max = frequecy*2 / frame_rate_temp + (u32)(csiSensor->hvb_adapt.down_fps_mode == 2);
	if(csiSensor->hvb_adapt.down_fps_mode ==1){
		csiSensor->hvb_adapt.step_max+=1;
		frame_rate_final = frequecy*2 / csiSensor->hvb_adapt.step_max;
	}else{
		frame_rate_final = frame_rate_temp;
	}
	h_len = pclk / (csiSensor->hvb_adapt.v_len * frame_rate_final);
	csiSensor->hvb_adapt.step_val = csiSensor->hvb_adapt.v_len*frame_rate_final/(frequecy*2);
	if(pointer_check(&(csiSensor->p_fun_adapt.fp_hvblank))){
		csiSensor->p_fun_adapt.fp_hvblank(h_len,csiSensor->hvb_adapt.v_len);
	}
	if(pointer_check(&(csiSensor->p_fun_adapt.fp_rotate))){
		csiSensor->p_fun_adapt.fp_rotate(csiSensor->rotate_adapt.rotate);
	}
	if(frequecy_mode < 2)
		debg("frame_rate set :%d %d %d %d\n",h_len,csiSensor->hvb_adapt.v_len,csiSensor->hvb_adapt.step_max,csiSensor->hvb_adapt.step_val);
}

//==rotate : 0 is 0' , 3 is 180' , 0x1  is mirror , 0x2 is flip==
void hal_senser_rotate(u8 rotate)
{
	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();

	if(pointer_check(&(csiSensor->p_fun_adapt.fp_rotate))){
		csiSensor->p_fun_adapt.fp_rotate(rotate);
	}
}



#define  ISP_SECTION_EXP_EVSTEP          __attribute__ ((section(".res_exp_evstep_tab")))  



ALIGNED(32) u16 const Ratio_of_Evstep[_EVSTEP_TAB_LEN_]={0,	
	256,269,282,294,307,321,335,350,365,381,398,416,434,453,473,494,
	516,538,562,587,613,640,668,698,728,761,794,829,866,904,944,985,
	1029,1074,1122,1171,1223,1277,1333,1392,1453,1517,1584,1654,1727,1803,1883,1966,
	2053,2143,2238,2337,2440,2547,2660,2777,2900,3028,3161,3301,3446,3598,3757,3923,
	4096,4277,4465,4662,4868,5083,5307,5541,5786,6041,6308,6586,6876,7180,7497,7827,
	8173,8533,8910,9303,9713,10142,10589,11056,11544,12053,12585,13140,13720,14326,14958,15617,
	16306,17026,17777,18561,19380,20235,21128,22060,23034,24050,25111,26219,27375,28583,29844,31161,
	32536,33971,35470,37035,38669,40375,42156,44016,45958,47985,50103,52313,54621,57031,59547,62174,
	64917,65535,65535};
void exp_gain_separate(u32 exp_gain,u32 step_val,u32 step_max,u32*p_exp,u32* p_sen_gain){
	u32 step = exp_gain / (step_val*256);
	#if 1		//flicker level
	if(step < 1){
		*p_exp = exp_gain / 256;
	}else if(step <= step_max){
		*p_exp = step * step_val;
	}else{
		*p_exp = step_max * step_val;
	}
	*p_sen_gain = exp_gain / (*p_exp);
	#else
	if(step < 2){
		*p_exp = exp_gain / 256;
	}else if(step <= step_max){
		*p_exp = step * step_val;
	}else{
		*p_exp = step_max * step_val;
	}
	*p_sen_gain = exp_gain / (*p_exp);
	#endif
}
u32 exp_br_cal(u32 ylog,u32 exp_gain,u32 br_k){
	#define  _BR_BASE_    16640//(780*16384 / (6*64))
	u32 br = 0,ratio,i;
    ratio = (ylog * br_k * _BR_BASE_ << 2) / (exp_gain>>6);
	if(ratio > 65535){
		br = 130;
		ratio >>= 8;
	}
	for(i = 0;i < _EVSTEP_TAB_LEN_-3;i++){
		if((ratio >= Ratio_of_Evstep[i])&&(ratio <Ratio_of_Evstep[i+1])){
			break;
		}
	} 
	br = clip(br+i,0,255);
	return br;
}

bool exposal_adj(u32 yloga_avg,u32 yloga,u32 *p_br,u32 exp_ext_mod)
{
	static u32 former_exp_gain,frame_number = 0;
	u32 ratio=0,ylog_obj,step_val,step_max;
	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	EXP * p_exp_adapt = &csiSensor->ae_adapt.exp_adapt;
	step_val = csiSensor->hvb_adapt.step_val;
	step_max = csiSensor->hvb_adapt.step_max;
	frame_number = (frame_number<((p_exp_adapt->exp_nums&0xf) - 1))?(frame_number+1):0;   
	if(frame_number!=1){
		return 0;
	}
	former_exp_gain = p_exp_adapt->exp_gain;
	*p_br = exp_br_cal(yloga ,p_exp_adapt->exp_gain,p_exp_adapt->k_br); 
	vbr = *p_br;
	vyloga = yloga;
	//debg("br:%d\n",*p_br);
	ylog_obj = get_isp_adapt_val(p_exp_adapt->exp_tag,*p_br);
	ylog_obj = (ylog_obj > 0x18)?(ylog_obj + exp_ext_mod *0x08 - 0x18):(exp_ext_mod *0x08);
	if((yloga_avg < (ylog_obj -  3 - ylog_obj/32)) || (yloga_avg > (ylog_obj + 3+ylog_obj/32))||0){
		u32 weight = (p_exp_adapt->exp_nums>>12)&0xf; 
		ratio = clip(((yloga<<5) / ylog_obj),1,256); 
		if(ratio > 32){  
			weight = (p_exp_adapt->exp_nums>>12)&0xf;
		}else{
			weight = (p_exp_adapt->exp_nums>>8)&0xf;
		}
		p_exp_adapt->exp_gain = ((p_exp_adapt->exp_gain<<5) / ratio * weight + p_exp_adapt->exp_gain*(16 - weight))>>4;
		p_exp_adapt->exp_gain = clip(p_exp_adapt->exp_gain,p_exp_adapt->exp_min*256,p_exp_adapt->gain_max*step_val*step_max);
		if((ratio >= 32 +((p_exp_adapt->exp_nums&0x0f0)>>2))||(ratio <= 32-((p_exp_adapt->exp_nums&0x0f0)>>3))){
			frame_number++;
			frame_number = (frame_number<(p_exp_adapt->exp_nums&0xf))?frame_number:0; 
		}
	}
	if(common_adapt.exp_gain!=p_exp_adapt->exp_gain){
			common_adapt.exp_gain = clip(p_exp_adapt->exp_gain,p_exp_adapt->exp_min*256,p_exp_adapt->gain_max*step_val*step_max);
			exp_gain_separate(common_adapt.exp_gain,step_val,step_max,&sen_exp,&sen_gain); 
			if(pointer_check(&(snser.p_sensor_cmd->p_fun_adapt.fp_exp_gain_wr))){
				(*snser.p_sensor_cmd->p_fun_adapt.fp_exp_gain_wr)(sen_exp,sen_gain);
			}
			//debg("common  br:%d o:%d eg:%d e:%d g:%d avg:%d a:%d tag:%d %d\n",*p_br,ylog_obj,p_exp_adapt->exp_gain,sen_exp,sen_gain,yloga_avg,yloga,common_adapt.exp_gain,ylog_obj);
	}else{
		// if((p_exp_adapt->exp_gain != former_exp_gain) && (exp_gain_flag == 0)){ // ???????
		// 	exp_gain_separate(p_exp_adapt->exp_gain,step_val,step_max,&sen_exp,&sen_gain); // exp?gain??
		// 	exp_gain_flag = 1;
//			if(pointer_check(&(snser.p_sensor_cmd->p_fun_adapt.fp_exp_gain_wr))){
//			(*snser.p_sensor_cmd->p_fun_adapt.fp_exp_gain_wr)(sen_exp,sen_gain);
//			}
			//debg("auto   br:%d o:%d eg:%d e:%d g:%d avg:%d a:%d tag:%d\n",*p_br,ylog_obj,p_exp_adapt->exp_gain,sen_exp,sen_gain,yloga_avg,yloga,common_adapt.exp_gain,ylog_obj);
		// }
	}
	(void)former_exp_gain;
	
	return 0;
}	

void isp_switch_senser_set_gain_exp()
{
	if(pointer_check(&(snser.p_sensor_cmd->p_fun_adapt.fp_exp_gain_wr)))
	{
		(*snser.p_sensor_cmd->p_fun_adapt.fp_exp_gain_wr)(sen_exp,sen_gain);
	}
}

void isp_all_model_cfg(u32 isp_all_mod)
{
	u32 i,val;
	for(i = 0;i<_OL_DB_POS;i++)
	{		
		val = _GET_ISP_MODE_(*isp.p_all_mod,i*_ISP_BIT_WIDTH_);
		isp_model_cfg(i*_ISP_BIT_WIDTH_,val);

	}	
}
void hal_ispService(void) 
{
	if(ispCtrl.isp_contan_load !=0xff)
	{
	    isp_contra_table_load(ispCtrl.isp_contan_load ); 
	    ispCtrl.isp_contan_load  = 0xff;
	}
	if(ispCtrl.p_isp_ygama_load !=NULL)
	{
		isp_gma_table_load(ispCtrl.p_isp_ygama_load[0], ispCtrl.p_isp_ygama_load[1]); 
		isp.p_ygama->br_mod = ispCtrl.isp_ygama_br_mod;
	    ispCtrl.p_isp_ygama_load  = NULL;
	}	
	if(ispCtrl.p_isp_rgbgama_load !=NULL)
	{
	    isp_rgbgma_table_load(ispCtrl.p_isp_rgbgama_load[0],ispCtrl.p_isp_rgbgama_load[0]);  
		isp.p_rgb_gama->br_mod = ispCtrl.isp_rgbgama_br_mod;
	    ispCtrl.p_isp_rgbgama_load  = NULL;
	}
}


void hal_isp_sof_loop()
{
	if(_GET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS)){
		u32 i,val;
		debg("ISPMODE:%x\n",ISPMODE);
		for(i = 0;i<_OL_DB_POS;i++)
		{	
			if(_GET_ISP_MODE_(isp_module_state,i*_ISP_BIT_WIDTH_)){
				val = _GET_ISP_MODE_(*isp.p_all_mod,i*_ISP_BIT_WIDTH_);
				switch (i*_ISP_BIT_WIDTH_){
					//case _AE_POS_:   AE cannot disable,but you can change its parameters
					case _BLC_POS_:         isp_BLC_cfg(0,0,isp.p_blc->blkl_gb,isp.p_blc->blkl_gr,isp.p_blc->blkl_r,isp.p_blc->blkl_b);
											ax32xx_ispModeSet(ISP_MODE_BLCEN,val);
											debg("usb BLC MODE:%d\n",val);
											break;
					case _LSC_POS_:         isp_LSC_adj_cfg((u16*)lsc_tab,0);
											ax32xx_ispModeSet(ISP_MODE_LSCEN,val);
											debg("usb LSC MODE:%d\n",val);
											break;
					case _AWB_POS_ :        isp_AWB_adj_cfg(_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_),(u8*)isp.p_awb->awb_tab,isp.p_awb->seg_mode,isp.p_awb->rgmin,isp.p_awb->rgmax,
												isp.p_awb->weight_in,isp.p_awb->weight_mid,isp.p_awb->ymin,isp.p_awb->ymax,isp.p_awb->hb_rate,isp.p_awb->hb_class,isp.p_awb->hr_rate,isp.p_awb->hr_class,
												isp.p_awb->yuv_mod_en,isp.p_awb->rg_start, isp.p_awb->ycbcr_th, isp.p_awb->cb_th, isp.p_awb->cr_th,isp.p_awb->cbcr_th);
												ax32xx_ispModeSet(ISP_MODE_AWBEN,val);
											debg("usb AWB MODE:%d\n",val);
											break;
					case _YGAMA_POS_:      isp_GAMMA_cfg(isp.p_ygama->using_ygama,isp.p_ygama->br_mod,using_contrast,isp.p_ygama->bofst,isp.p_ygama->lofst,NULL,NULL,isp.p_ygama->pad_num);
											ax32xx_ispModeSet(ISP_MODE_YGAMMAEN,val);
											debg("usb YGAMA MODE:%d\n",val);
											break;
					case _DDC_POS_:         isp_DDC_cfg(isp.p_ddc->dpc_dn_en,isp.p_ddc->hot_num,isp.p_ddc->dead_num,isp.p_ddc->hot_th,isp.p_ddc->dead_th,isp.p_ddc->avg_th,
													isp.p_ddc->d_th_rate[7],isp.p_ddc->h_th_rate[7],isp.p_ddc->std_th,isp.p_ddc->std_th_rate,isp.p_ddc->indx_table);
											ax32xx_ispModeSet(ISP_MODE_DPCEN,val);debg("DDC MODE:%d\n",val);
											break;
					
					case _CCM_POS_:       	isp_CCM_cfg( isp.p_ccm->ccm,isp.p_ccm->s41,isp.p_ccm->s42,isp.p_ccm->s43);
											ax32xx_ispModeSet(ISP_MODE_CCMEN,val);debg("CCM MODE:%d\n",val);
											break;
					
					case _DGAIN_POS_:    	isp_RGB_DGAIN_adj(isp.p_rgb_dgain->dgain[0],isp.p_rgb_dgain->dgain[1],isp.p_rgb_dgain->dgain[2],isp.p_rgb_dgain->dgain[3],isp.p_rgb_dgain->dgain[4],
											isp.p_rgb_dgain->dgain[5],isp.p_rgb_dgain->dgain[6],isp.p_rgb_dgain->dgain[7],isp.p_rgb_dgain->dgain[8]);
											ax32xx_ispModeSet(ISP_MODE_DGAINEN,val);debg("DEGAIN MODE:%d\n",val);
											break;
					
					case _RGB_GAMA_POS_:    isp_RGB_gam_cfg(isp.p_rgb_gama->using_rgbgama,isp.p_rgb_gama->br_mod,isp.p_rgb_gama->max_oft[7],isp.p_rgb_gama->rmin,isp.p_rgb_gama->rmax,isp.p_rgb_gama->gmin,isp.p_rgb_gama->gmax,isp.p_rgb_gama->bmin,isp.p_rgb_gama->bmax);
											ax32xx_ispModeSet(ISP_MODE_RGBGAMMAEN,val);debg("RGB_GAMA MODE:%d\n",val);
											break;
					
					case _CH_POS_:          isp_CH_cfg(isp.p_ch->stage1_en,isp.p_ch->stage0_en,isp.p_ch->enhence,isp.p_ch->th1,isp.p_ch->th0,isp.p_ch->r_rate,isp.p_ch->g_rate,isp.p_ch->b_rate,isp.p_ch->sat);
											ax32xx_ispModeSet(ISP_MODE_CHEN,val);debg("CH MODE:%d\n",val);
											break;
											
					case _VDE_POS_:         isp_VDE_cfg(isp.p_vde->contra,isp.p_vde->bright_oft,isp.p_vde->bright_k,isp.p_vde->hue,isp.p_vde->sat);
											ax32xx_ispModeSet(ISP_MODE_VDEEN,val);debg("VDE MODE:%d\n",val);
											break;
							
					case _EE_POS_:          isp_EE_cfg(isp.p_ee->ee_dn_th[7],isp.p_ee->ee_dn_slope[7],isp.p_ee->ee_sharp_slope[7],isp.p_ee->ee_th_adp[7],isp.p_ee->sharp_class[7],isp.p_ee->dn_class[7]);
											ax32xx_ispModeSet(ISP_MODE_SHARPEN,val);debg("EE MODE:%d &x\n",val);
											break;
					
					case _CFD_POS_:         isp_CCF_cfg(isp.p_ccf->ccf_en,isp.p_ccf->wdc_en, isp.p_ccf->wclass, isp.p_ccf->rate, isp.p_ccf->wymin,isp.p_ccf->ymax,isp.p_ccf->mean_en,isp.p_ccf->th,isp.p_ccf->dn_class);
											ax32xx_ispModeSet(ISP_MODE_CCFEN,val);debg("CFD MODE:%d\n",val);
											break;
					
					case _SAJ_POS_:         isp_SAJ_cfg(isp.p_saj->sat[0],isp.p_saj->sat[1], isp.p_saj->sat[2], isp.p_saj->sat[3], isp.p_saj->sat[4], isp.p_saj->sat[5], isp.p_saj->sat[6],isp.p_saj->sat[7], isp.p_saj->sat[8],
													isp.p_saj->sat[9], isp.p_saj->sat[10], isp.p_saj->sat[11], isp.p_saj->sat[12], isp.p_saj->sat[13],isp.p_saj->sat[14], isp.p_saj->sat[15], isp.p_saj->sat[16]);
											ax32xx_ispModeSet(ISP_MODE_SAJEN,val);debg("SAJ MODE:%d\n",val);
											break;
					
					
					default: break;//
				}
			}			
		}
		debg("ISPMODE:%x\n",ISPMODE);
		isp_module_state = 0;
		_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_DIS_);
	}
}

void hal_commom_isr(void)
{//???????????,????????????????
	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	if(_GET_BIT_(commom_mode,common_mode_max)){
		debg("common_mode:%d\n",commom_mode);
		u32 i;
		for(i = 0;i<common_mode_max;i++)
		{	
			if(!_GET_BIT_(commom_mode,i))
				continue;
			switch (i){				
				case pclk_fir_en_pos:		ax32xx_pclk_fir_Set(common_adapt.pclk_fir_en);	break;
				case pclk_fir_class_pos:	ax32xx_pclk_analog_Set(common_adapt.pclk_fir_class);break;
				case pclk_inv_en_pos:		ax32xx_pclk_inv_Set(common_adapt.pclk_inv_en);break;
				case csi_tun_pos:			ax32xx_csi_clk_tun_Set(common_adapt.csi_tun);break;	
				case hsyn_pos:				ax32xx_csiSyncSet(common_adapt.hsyn,common_adapt.vsyn);break;
				case vsyn_pos:				ax32xx_csiSyncSet(common_adapt.hsyn,common_adapt.vsyn);break;
				case colrarray_pos:			ax32xx_csiPrioritySet(common_adapt.colrarray);break;
				case rduline_pos:			break;
				case v_len_pos:				csiSensor->hvb_adapt.v_len = common_adapt.hvb.v_len;
											sensor_frame_rate_adapt(common_adapt.hvb.frequency,common_adapt.hvb.fps);
											debg("v_len:%d\n",common_adapt.hvb.v_len);break;
				case down_fps_mode_pos:		break;
				case fps_pos:				sensor_frame_rate_adapt(common_adapt.hvb.frequency,common_adapt.hvb.fps);break;
				case frequency_pos:			sensor_frame_rate_adapt(common_adapt.hvb.frequency,common_adapt.hvb.fps);break;
				case mclk_pos:				break;
				case pclk_pos:				debg("no suppot change pclk yet\n");break;
				case rotate_pos:			csiSensor->p_fun_adapt.fp_rotate(common_adapt.rotate);break;
				case AVDD_pos:				ax32xx_sysLDOSet(SYS_LDO_HSEN,common_adapt.AVDD,1);break;
				case DVDD_pos:				ax32xx_sysLDOSet(SYS_LDO_LSEN,common_adapt.DVDD,1);break;
				case VDDIO_pos:				debg("the board didn't support set VDDIO:%d\n",common_adapt.VDDIO);break;
				 /*
				case pclk_fir_en_pos:		debg("fir_en:%d\n",common_adapt.pclk_fir_en);	break;
				case pclk_fir_class_pos:	debg("fir_class:%d\n",common_adapt.pclk_fir_class);break;
				case pclk_inv_en_pos:		debg("inv_en:%d\n",common_adapt.pclk_inv_en);break;
				case csi_tun_pos:			debg("tun:%d\n",common_adapt.csi_tun);break;	
				case hsyn_pos:				debg("hsyn:%d\n",common_adapt.hsyn);break;
				case vsyn_pos:				debg("vsyn:%d\n",common_adapt.vsyn);break;
				case colrarray_pos:			debg("array:%d\n",common_adapt.colrarray);break;
				case rduline_pos:			debg("rduline:%d\n",common_adapt.rduline);break;
				case v_len_pos:				debg("v_len:%d\n",common_adapt.hvb.v_len);break;	
				case down_fps_mode_pos:		debg("down:%d\n",common_adapt.hvb.down_fps_mode);break;
				case fps_pos:				debg("fps:%d\n",common_adapt.hvb.fps);break;
				case frequency_pos:			debg("fre:%d\n",common_adapt.hvb.frequency);break;
				case mclk_pos:				debg("mclk:%d\n",common_adapt.mclk);break;
				case pclk_pos:				debg("no suppot change pclk yet:%d\n",common_adapt.hvb.pclk);break;
				case rotate_pos:			debg("rotate:%d\n",common_adapt.rotate);break;
				case AVDD_pos:				debg("avdd:%d\n",common_adapt.AVDD);break;
				case DVDD_pos:				debg("dvdd:%d\n",common_adapt.DVDD);break;
				case VDDIO_pos:				debg("the board didn't support set VDDIO:%d\n",common_adapt.VDDIO);break;*/
				default: break;//
			}
		}
		commom_mode = 0;
	}
}

void init_common_str(void)
{
	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	_Sensor_Ident_ *csiIdent =  hal_csi_SeninitGet();
	memcpy(&common_adapt.hvb,&csiSensor->hvb_adapt,sizeof(csiSensor->hvb_adapt));
	common_adapt.exp_gain = csiSensor->hvb_adapt.v_len*csiSensor->ae_adapt.exp_adapt.gain_max;
	common_adapt.gain_max = csiSensor->ae_adapt.exp_adapt.gain_max;
	common_adapt.id = csiIdent->id;
	common_adapt.mclk = csiSensor->mclk;
	common_adapt.pixelw = csiSensor->pixelw;
	common_adapt.pixelh = csiSensor->pixelh;
	common_adapt.type = csiSensor->typ;
	common_adapt.hsyn = csiSensor->hsyn;
	common_adapt.vsyn = csiSensor->vsyn;
	common_adapt.rduline = 0;
	common_adapt.colrarray = csiSensor->colrarray;
	common_adapt.pclk_fir_en = csiSensor->pclk_fir_en;
	common_adapt.pclk_inv_en = csiSensor->pclk_inv_en;
	common_adapt.csi_tun = csiSensor->csi_tun;
	memcpy(&common_adapt.name,&csiIdent->sensor_name,sizeof(common_adapt.name));
	common_adapt.exp_gain_en = 0;// 0:?????exp_gain  1:????exp_gain
	common_adapt.blk_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_BLC_POS_);
	common_adapt.lsc_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_LSC_POS_);
	common_adapt.ddc_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_DDC_POS_);
	common_adapt.awb_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_AWB_POS_);
	common_adapt.ccm_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_CCM_POS_);
	common_adapt.dgain_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_DGAIN_POS_);
	common_adapt.ygama_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_YGAMA_POS_);
	common_adapt.rgb_gama_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_RGB_GAMA_POS_);
	common_adapt.ch_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_CH_POS_);
	common_adapt.vde_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_VDE_POS_);
	common_adapt.ee_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_EE_POS_);
	common_adapt.cfd_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_CFD_POS_);
	common_adapt.saj_en = _GET_ISP_MODE_(csiSensor->isp_all_mod,_SAJ_POS_);
	
	common_adapt.AVDD = csiSensor->AVDD;
	common_adapt.DVDD = csiSensor->DVDD;
	common_adapt.VDDIO = csiSensor->VDDIO;
	common_adapt.pclk_fir_class = csiSensor->pclk_fir_class;
	common_adapt.rotate = csiSensor->rotate_adapt.rotate;
	isp_module_state = 0;
	commom_mode = 0;
	debg("isp_module_state:%x\n",isp_module_state);

}

void hal_isp_debug_read(u8 *pbdata ,u32 trans_len, u32 param_3B)
{
	int startPos = param_3B >> 8;
    int isp_mode = param_3B & 0xff;
	debg("\n");
	debg("isp_mode:%x len:%d 3B:%x\n",isp_mode,trans_len,param_3B);
    switch(isp_mode){
		
		case _AE_POS_		:	debg("read ae\n");
								debg("%d\n",isp.p_ae->exp_adapt.gain_max);
								memcpy(pbdata,(char *)isp.p_ae+startPos,trans_len); 
								break;
        case _BLC_POS_      :	debg("read blc\n");
								memcpy(pbdata,(char *)isp.p_blc+startPos,trans_len);
								break;
        case _DDC_POS_       : 	debg("read ddc\n");
								isp.p_ddc->ddc_class = isp.p_ddc->indx_table[0];
								debg("%d %d %d %d %d %d %d %d\n",isp.p_ddc->indx_adapt[0],isp.p_ddc->indx_adapt[1],isp.p_ddc->indx_adapt[2]
								,isp.p_ddc->indx_adapt[3],isp.p_ddc->indx_adapt[4],isp.p_ddc->indx_adapt[5],isp.p_ddc->indx_adapt[6],isp.p_ddc->indx_adapt[7]);
								memcpy(pbdata,(char *)isp.p_ddc+startPos,trans_len); 
								break;
        case _AWB_POS_       : 	debg("read awb\n");
								memcpy(pbdata,(char *)isp.p_awb+startPos,trans_len);//awb structure is too big,char data follow,must (char *)
								//uart_PrintfBuf(pbdata,sizeof(isp.p_awb));
								break;
        case _CCM_POS_       : 	memcpy(pbdata,(char *)isp.p_ccm+startPos,trans_len);  
								//uart_PrintfBuf(pbdata,sizeof(devSensor_Cmd.ccm_adapt));
								break;
        case _YGAMA_POS_     : 	debg("read ygama\n");
								memcpy(pbdata,(char *)isp.p_ygama+startPos,trans_len); 
								//uart_PrintfBuf(pbdata,trans_len);
								break;
        case _RGB_GAMA_POS_  : 	memcpy(pbdata,(char *)isp.p_rgb_gama+startPos,trans_len);  
								//uart_PrintfBuf(pbdata,sizeof(devSensor_Cmd.rgbgama_adapt));
								break;
        case _EE_POS_        : 	memcpy(pbdata,(char *)isp.p_ee+startPos,trans_len); 
								//uart_PrintfBuf(pbdata,sizeof(devSensor_Cmd.ee_adapt));
								break;
        case _LSC_POS_       : 	memcpy(pbdata,(char *)lsc_tab+startPos,trans_len);
								debg("len:%d 3B:%x\n",trans_len,param_3B);
								debg("lsc_tab:\n");
	                            debg("startPos=%d\n",startPos);
								debg("size:%d\n",trans_len);
								//uart_PrintfBuf((char *)lsc_tab+startPos,trans_len);
								break;
        case _VDE_POS_      : 	debg("vde read\n");
								memcpy(pbdata,(char *)isp.p_vde+startPos,trans_len); 
								//uart_PrintfBuf(pbdata,sizeof(devSensor_Cmd.vde_adapt));
								break;
		case _CH_POS_		:	debg("ch read\n");
								memcpy(pbdata,(char *)isp.p_ch+startPos,trans_len);
								break;
		case _SAJ_POS_		:	debg("saj read\n");
								memcpy(pbdata,(char *)isp.p_saj+startPos,trans_len);
								break;
        default :break;
    }

}

void hal_isp_debug_write(u8 *pbdata ,u32 trans_len, u32 param_3B){
	int isp_mode = param_3B & 0xff;
	int startPos = param_3B >> 8;
	debg("\nisp_mode:%x len:%d 3B:%x\n",isp_mode,trans_len,param_3B);
    switch(isp_mode){
		case _AE_POS_		:	debg("write ae\n");
								memcpy(isp.p_ae,pbdata+startPos,trans_len);
								debg("%d %d %d %d %d %d %d %d\n",isp.p_ae->exp_adapt.exp_tag[0],isp.p_ae->exp_adapt.exp_tag[1]\
								,isp.p_ae->exp_adapt.exp_tag[2],isp.p_ae->exp_adapt.exp_tag[3],isp.p_ae->exp_adapt.exp_tag[4]\
								,isp.p_ae->exp_adapt.exp_tag[5],isp.p_ae->exp_adapt.exp_tag[6],isp.p_ae->exp_adapt.exp_tag[7]);
								debg("%d\n",isp.p_ae->exp_adapt.gain_max);
								break;
        case _BLC_POS_  	: 	debg("wirte blc\n");
								memcpy(isp.p_blc,pbdata+startPos,trans_len);
								debg("%d %d %d %d\n",isp.p_blc->blkl_r,isp.p_blc->blkl_gr,isp.p_blc->blkl_gb,isp.p_blc->blkl_r);
								if(_GET_ISP_MODE_(*isp.p_all_mod,_BLC_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
								
		case _LSC_POS_       : 	debg("wirte lsc:\n");
								CopyToMember(lsc_tab, 0, sizeof(lsc_tab), pbdata, startPos, trans_len);
								int i;
								u16 *data = (u16 *)pbdata;
								for(i = startPos/2;i<(((startPos+trans_len)/2)<572?((startPos+trans_len)/2):572);i++){
									debg("%d\t ",*data);
									data ++;
								}
								if(_GET_ISP_MODE_(*isp.p_all_mod,_LSC_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
								
        case _DDC_POS_       : 	debg("write ddc\n");
								//debg("ddc_class:%d\n",isp.p_ddc->ddc_class);
								memcpy(isp.p_ddc,pbdata+startPos,trans_len);								
//								isp.p_ddc->indx_table[0] = isp.p_ddc->ddc_class;
//								isp.p_ddc->indx_table[1] = clip((isp.p_ddc->ddc_class-2),0,5);
//								debg("ddc_class:%d\n",isp.p_ddc->ddc_class);
								debg("%d %d %d %d %d %d %d %d\n",isp.p_ddc->indx_adapt[0],isp.p_ddc->indx_adapt[1],isp.p_ddc->indx_adapt[2]
								,isp.p_ddc->indx_adapt[3],isp.p_ddc->indx_adapt[4],isp.p_ddc->indx_adapt[5],isp.p_ddc->indx_adapt[6],isp.p_ddc->indx_adapt[7]);
								if(_GET_ISP_MODE_(*isp.p_all_mod,_DDC_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
        case _AWB_POS_       : 	debg("wirte awb:\n");
								memcpy(isp.p_awb,pbdata+startPos,trans_len);							
								_SET_ISP_MODE_(isp_module_state,_AWB_POS_,1);
								_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								break;
								
        case _CCM_POS_       : 	debg("write ccm\n");
								memcpy(isp.p_ccm,pbdata+startPos,trans_len); 
								debg("%d %d %d %d %d %d %d %d %d\n",isp.p_ccm->ccm[0],isp.p_ccm->ccm[1],isp.p_ccm->ccm[2]
								,isp.p_ccm->ccm[3],isp.p_ccm->ccm[4],isp.p_ccm->ccm[5]
								,isp.p_ccm->ccm[6],isp.p_ccm->ccm[7],isp.p_ccm->ccm[8]);
								debg("%d %d %d \n",isp.p_ccm->s41,isp.p_ccm->s42,isp.p_ccm->s43);
								_SET_ISP_MODE_(isp_module_state,_CCM_POS_,1);
								_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								break;
								
        case _DGAIN_POS_ 	 :	debg("write dgain\n");
								//	memcpy(isp.p);
								if(_GET_ISP_MODE_(*isp.p_all_mod,_DGAIN_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
								
		case _YGAMA_POS_     : 	debg("wirte ygama\n"); 
								ygama_update(pbdata,trans_len,param_3B);
								//memcpy(isp.p_ygama,pbdata+startPos,trans_len);
			
								if(_GET_ISP_MODE_(*isp.p_all_mod,_YGAMA_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
								
        case _RGB_GAMA_POS_  : 	debg("write rgb_gamma\n");
								//	memcpy(&(devSensor_Cmd.rgbgama_adapt),pbdata,sizeof(devSensor_Cmd.rgbgama_adapt));  
								if(_GET_ISP_MODE_(*isp.p_all_mod,_RGB_GAMA_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
								
        case _CH_POS_		 :	debg("write ch\n");
								memcpy(isp.p_ch,pbdata+startPos,trans_len);	
								debg("%d %d %d %d %d %d %d\n",isp.p_ch->rate[0],isp.p_ch->rate[1],isp.p_ch->rate[2]
								,isp.p_ch->rate[3],isp.p_ch->rate[4],isp.p_ch->rate[5],isp.p_ch->rate[6],isp.p_ch->rate[7]);
								debg("%d %d %d %d %d %d\n",isp.p_ch->r_rate[0],isp.p_ch->r_rate[1],isp.p_ch->r_rate[2],isp.p_ch->r_rate[3],isp.p_ch->r_rate[4],isp.p_ch->r_rate[5]);
								debg("%d %d %d %d %d %d\n",isp.p_ch->g_rate[0],isp.p_ch->g_rate[1],isp.p_ch->g_rate[2],isp.p_ch->g_rate[3],isp.p_ch->g_rate[4],isp.p_ch->g_rate[5]);
								debg("%d %d %d %d %d %d\n",isp.p_ch->b_rate[0],isp.p_ch->b_rate[1],isp.p_ch->b_rate[2],isp.p_ch->b_rate[3],isp.p_ch->b_rate[4],isp.p_ch->b_rate[5]);
								debg("%d %d %d %d %d %d\n",isp.p_ch->enhence[0],isp.p_ch->enhence[1],isp.p_ch->enhence[2]
								,isp.p_ch->enhence[3],isp.p_ch->enhence[4],isp.p_ch->enhence[5]);
								if(_GET_ISP_MODE_(*isp.p_all_mod,_CH_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
								
		case _EE_POS_        : 	debg("write ee\n");
								memcpy(isp.p_ee,pbdata+startPos,trans_len);
								for(i=0; i < 8; i++){
									isp.p_ee->sharp_class[i] = isp.p_ee->ee_class;
								}
								for(i=0; i < 8; i++){
									debg("%d ",isp.p_ee->sharp_class[i]);
								}
								debg("\n");
								if(_GET_ISP_MODE_(*isp.p_all_mod,_EE_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
		case _VDE_POS_      : 	debg("write vde\n");
								memcpy(isp.p_vde,pbdata+startPos,trans_len);
								debg("%d %d %d %d %d %d %d %d\n",isp.p_vde->sat_rate[0],isp.p_vde->sat_rate[1]
								,isp.p_vde->sat_rate[2],isp.p_vde->sat_rate[3],isp.p_vde->sat_rate[4],isp.p_vde->sat_rate[5]
								,isp.p_vde->sat_rate[6],isp.p_vde->sat_rate[7]);
								if(_GET_ISP_MODE_(*isp.p_all_mod,_VDE_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
								
		case _CFD_POS_       :// memcpy(&(devSensor_Cmd.cfd_adapt),pbdata,sizeof(devSensor_Cmd.cfd_adapt)); 
								
								if(_GET_ISP_MODE_(*isp.p_all_mod,_CFD_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;
								
        case _SAJ_POS_		:	debg("write saj\n");
								memcpy(isp.p_saj,pbdata+startPos,trans_len);
								debg("%d %d %d %d %d %d %d %d\n",isp.p_saj->sat_rate[0],isp.p_saj->sat_rate[1],isp.p_saj->sat_rate[2]
								,isp.p_saj->sat_rate[3],isp.p_saj->sat_rate[4],isp.p_saj->sat_rate[5],isp.p_saj->sat_rate[6],isp.p_saj->sat_rate[7]);
								if(_GET_ISP_MODE_(*isp.p_all_mod,_SAJ_POS_) == _ISP_EN_){
									_SET_ISP_MODE_(isp_module_state,isp_mode,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
								}
								break;


        default :break;
    }
	debg("isp write out\n");
}

//vde,exp_tag,gain_max,ee,ch,saj,ccm,ddc
void hal_Sensor_common_Get(u8 *pbdata ,u32 trans_len, u32 param_3B){
	int startPos = param_3B;
	debg("startPos:%d len:%d 3B:%d\n",startPos,trans_len,param_3B);
	debg("w:%x\n",common_adapt.pixelw);
	memcpy(pbdata,(char *)&common_adapt+startPos,trans_len);
}


void hal_Sensor_common_Set(u8 *pbdata ,u32 trans_len, u32 param_3B){
	int startPos = param_3B; 
	debg("Sensor_set\n");
	debg("\n");
	debg("startPos:%d len:%d 3B:%d\n",startPos,trans_len,param_3B);
	
	switch(startPos){
		case offsetof(COMMON,exp_gain):		CopyToMember(&common_adapt,offsetof(COMMON,exp_gain),
													sizeof(common_adapt.exp_gain),pbdata,startPos,trans_len);
													break;
		case offsetof(COMMON,gain_max):		CopyToMember(&common_adapt,offsetof(COMMON,gain_max),
													sizeof(common_adapt.gain_max),pbdata,startPos,trans_len);
											_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
											csiSensor->ae_adapt.exp_adapt.gain_max = common_adapt.gain_max;
													break;
		case offsetof(COMMON,mclk):			CopyToMember(&common_adapt,offsetof(COMMON,mclk),
													sizeof(common_adapt.mclk),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,mclk_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,pclk):			
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,pclk),
													sizeof(common_adapt.hvb.pclk),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,pclk_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,v_len):			
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,v_len),
													sizeof(common_adapt.hvb.v_len),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,v_len_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;									
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,down_fps_mode):			
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,down_fps_mode),
													sizeof(common_adapt.hvb.down_fps_mode),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,down_fps_mode_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;									
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,fps):			
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,fps),
													sizeof(common_adapt.hvb.fps),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,fps_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;									
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,frequency):			
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,frequency),
													sizeof(common_adapt.hvb.frequency),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,frequency_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;										
											
		case offsetof(COMMON,hsyn):			CopyToMember(&common_adapt,offsetof(COMMON,hsyn),
													sizeof(common_adapt.hsyn),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,hsyn_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,vsyn):			CopyToMember(&common_adapt,offsetof(COMMON,vsyn),
													sizeof(common_adapt.vsyn),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,vsyn_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;		
		case offsetof(COMMON,rduline):		CopyToMember(&common_adapt,offsetof(COMMON,rduline),
													sizeof(common_adapt.rduline),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,rduline_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,colrarray):	CopyToMember(&common_adapt,offsetof(COMMON,colrarray),
													sizeof(common_adapt.colrarray),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,colrarray_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,pclk_fir_en):	CopyToMember(&common_adapt,offsetof(COMMON,pclk_fir_en),
													sizeof(common_adapt.pclk_fir_en),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,pclk_fir_en_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,pclk_inv_en):	CopyToMember(&common_adapt,offsetof(COMMON,pclk_inv_en),
													sizeof(common_adapt.pclk_inv_en),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,pclk_inv_en_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,csi_tun):		CopyToMember(&common_adapt,offsetof(COMMON,csi_tun),
													sizeof(common_adapt.csi_tun),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,csi_tun_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;											
		case offsetof(COMMON,exp_gain_en):	CopyToMember(&common_adapt,offsetof(COMMON,exp_gain_en),
													sizeof(common_adapt.exp_gain_en),pbdata,startPos,trans_len);
													break;
		case offsetof(COMMON,blk_en):		CopyToMember(&common_adapt,offsetof(COMMON,blk_en),
													sizeof(common_adapt.blk_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_BLC_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_BLC_POS_)) == common_adapt.blk_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_BLC_POS_,common_adapt.blk_en);		
													break;
		case offsetof(COMMON,lsc_en):		CopyToMember(&common_adapt,offsetof(COMMON,lsc_en),
													sizeof(common_adapt.lsc_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_LSC_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_LSC_POS_)) == common_adapt.lsc_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_LSC_POS_,common_adapt.lsc_en);
													break;
		case offsetof(COMMON,ddc_en):		CopyToMember(&common_adapt,offsetof(COMMON,ddc_en),
													sizeof(common_adapt.ddc_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_DDC_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_DDC_POS_)) == common_adapt.ddc_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_DDC_POS_,common_adapt.ddc_en);
													break;
		case offsetof(COMMON,awb_en):		CopyToMember(&common_adapt,offsetof(COMMON,awb_en),
													sizeof(common_adapt.awb_en),pbdata,startPos,trans_len);	
											_SET_ISP_MODE_(isp_module_state,_AWB_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_)) == common_adapt.awb_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_,common_adapt.awb_en);
													break;
		case offsetof(COMMON,ccm_en):		CopyToMember(&common_adapt,offsetof(COMMON,ccm_en),
													sizeof(common_adapt.ccm_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_CCM_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_CCM_POS_)) == common_adapt.ccm_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_CCM_POS_,common_adapt.ccm_en);
													break;
		case offsetof(COMMON,dgain_en):		CopyToMember(&common_adapt,offsetof(COMMON,dgain_en),
													sizeof(common_adapt.dgain_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_DGAIN_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_DGAIN_POS_)) == common_adapt.dgain_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_DGAIN_POS_,common_adapt.dgain_en);
													break;
		case offsetof(COMMON,ygama_en):		CopyToMember(&common_adapt,offsetof(COMMON,ygama_en),
													sizeof(common_adapt.ygama_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_YGAMA_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_YGAMA_POS_)) == common_adapt.ygama_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_YGAMA_POS_,common_adapt.ygama_en);
													break;
		case offsetof(COMMON,rgb_gama_en):	CopyToMember(&common_adapt,offsetof(COMMON,rgb_gama_en),
													sizeof(common_adapt.rgb_gama_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_RGB_GAMA_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_RGB_GAMA_POS_)) == common_adapt.rgb_gama_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_RGB_GAMA_POS_,common_adapt.rgb_gama_en);
													break;
		case offsetof(COMMON,ch_en):		CopyToMember(&common_adapt,offsetof(COMMON,ch_en),
													sizeof(common_adapt.ch_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_CH_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_CH_POS_)) == common_adapt.ch_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_CH_POS_,common_adapt.ch_en);
													break;
		case offsetof(COMMON,vde_en):		CopyToMember(&common_adapt,offsetof(COMMON,vde_en),
													sizeof(common_adapt.vde_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_VDE_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_VDE_POS_)) == common_adapt.vde_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_VDE_POS_,common_adapt.vde_en);
													break;
		case offsetof(COMMON,ee_en):		CopyToMember(&common_adapt,offsetof(COMMON,ee_en),
													sizeof(common_adapt.ee_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_EE_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_EE_POS_)) == common_adapt.ee_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_EE_POS_,common_adapt.ee_en);
													break;
		case offsetof(COMMON,cfd_en):		CopyToMember(&common_adapt,offsetof(COMMON,cfd_en),
													sizeof(common_adapt.cfd_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_CFD_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_CFD_POS_)) == common_adapt.cfd_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_CFD_POS_,common_adapt.cfd_en);
													break;
		case offsetof(COMMON,saj_en):		CopyToMember(&common_adapt,offsetof(COMMON,saj_en),
													sizeof(common_adapt.saj_en),pbdata,startPos,trans_len);	
											_SET_ISP_MODE_(isp_module_state,_SAJ_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_SAJ_POS_)) == common_adapt.saj_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_SAJ_POS_,common_adapt.saj_en);
													break;		
		case offsetof(COMMON,AVDD):			CopyToMember(&common_adapt,offsetof(COMMON,AVDD),
													sizeof(common_adapt.AVDD),pbdata,startPos,trans_len);	
											_SET_BIT_(commom_mode,AVDD_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,DVDD):			CopyToMember(&common_adapt,offsetof(COMMON,DVDD),
													sizeof(common_adapt.DVDD),pbdata,startPos,trans_len);	
											_SET_BIT_(commom_mode,DVDD_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,VDDIO):		CopyToMember(&common_adapt,offsetof(COMMON,VDDIO),
													sizeof(common_adapt.VDDIO),pbdata,startPos,trans_len);	
											_SET_BIT_(commom_mode,VDDIO_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,pclk_fir_class):	CopyToMember(&common_adapt,offsetof(COMMON,pclk_fir_class),
													sizeof(common_adapt.pclk_fir_class),pbdata,startPos,trans_len);	
											_SET_BIT_(commom_mode,pclk_fir_en_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,rotate):		CopyToMember(&common_adapt,offsetof(COMMON,rotate),
													sizeof(common_adapt.rotate),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,rotate_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;											
			
		default:							debg("do not have this module\n");
											break;											
												
	}
	
	//double regs were used anytime during the vsyn signal is valid,so you shoud change these regs at the vsyn signal is invalid,
	//then these changed double regs will be valid at vsyn signal is valid next time.
	//tell soft_exp that someone(module) was modified
	_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);

	debg("isp_module_state:%x isp.p_all_mod:%x\n",isp_module_state,*isp.p_all_mod);
//	debg("blk_en:%d\n",common_adapt.blk_en);	
//	debg("lsc_en:%d\n",common_adapt.lsc_en);
//	debg("ddc_en:%d\n",common_adapt.ddc_en);
//	debg("awb_en:%d\n",common_adapt.awb_en);
//	debg("ccm_en:%d\n",common_adapt.ccm_en);
//	debg("dgain_en:%d\n",common_adapt.dgain_en);
//	debg("ygama_en:%d\n",common_adapt.ygama_en);
//	debg("rgb_gama_en:%d\n",common_adapt.rgb_gama_en);
//	debg("ch_en:%d\n",common_adapt.ch_en);
//	debg("vde_en:%d\n",common_adapt.vde_en);
//	debg("ee_en:%d\n",common_adapt.ee_en);
//	debg("cfd_en:%d\n",common_adapt.cfd_en);
//	debg("saj_en:%d\n",common_adapt.saj_en);	
//	debg("vddio:%d\n",common_adapt.VDDIO);
//	debg("exp_gain_en:%d\n",common_adapt.exp_gain_en);	
//	debg("exp_gain:%d\n",common_adapt.exp_gain);	
	
}

void CopyToMember(void* ispModuleData, int offsetOfMember, int sizeOfMember, void* pbdata, int startPos, int trans_len)
{
    int endPos = startPos + trans_len;

    if (startPos <= offsetOfMember)
    {
        if (endPos > offsetOfMember)
        {
            memcpy((char*)ispModuleData + offsetOfMember,
                (char*)pbdata + (offsetOfMember - startPos),
                endPos > offsetOfMember + sizeOfMember ? sizeOfMember : endPos - offsetOfMember);
        }
    }

    else if (startPos > offsetOfMember && startPos <= offsetOfMember + sizeOfMember)
    {
        memcpy((char*)ispModuleData + startPos,
            pbdata,
            endPos > offsetOfMember + sizeOfMember ?
        sizeOfMember : endPos - startPos);
    }
}


void ygama_update(u8 *pbdata ,u32 trans_len, u32 param_3B)
{
	int startPos = param_3B >> 8;
//	CopyToMember(isp.p_ygama, offsetof(YGAMA,br_mod), 
//			sizeof(isp.p_ygama->br_mod), pbdata, startPos, trans_len);
//	CopyToMember(isp.p_ygama, offsetof(YGAMA,contra_num), 
//			sizeof(isp.p_ygama->contra_num), pbdata, startPos, trans_len);
//	CopyToMember(isp.p_ygama, offsetof(YGAMA,bofst), 
//			sizeof(isp.p_ygama->bofst), pbdata, startPos, trans_len);								
//	CopyToMember(isp.p_ygama, offsetof(YGAMA,lofst), 
//			sizeof(isp.p_ygama->lofst), pbdata, startPos, trans_len);								
//	CopyToMember(isp.p_ygama, offsetof(YGAMA,lcpr_low), 
//			sizeof(isp.p_ygama->lcpr_low), pbdata, startPos, trans_len);								
//	CopyToMember(isp.p_ygama, offsetof(YGAMA,lcpr_high), 
//			sizeof(isp.p_ygama->lcpr_high), pbdata, startPos, trans_len);								
//	CopyToMember(isp.p_ygama, offsetof(YGAMA,lcpr_llimt), 
//			sizeof(isp.p_ygama->lcpr_llimt), pbdata, startPos, trans_len);								
//	CopyToMember(isp.p_ygama, offsetof(YGAMA,lcpr_hlimt), 
//			sizeof(isp.p_ygama->lcpr_hlimt), pbdata, startPos, trans_len);

	CopyToMember(isp.p_ygama, offsetof(YGAMA,pad_num), 
			sizeof(isp.p_ygama->pad_num), pbdata, startPos, trans_len);	
	CopyToMember(isp.p_ygama, offsetof(YGAMA,using_ygama), 
			sizeof(isp.p_ygama->using_ygama), pbdata, startPos, trans_len);

}

void awb_update(u8 *pbdata ,u32 trans_len, u32 param_3B)
{
	int startPos = param_3B >> 8;
	CopyToMember(isp.p_awb, offsetof(AWB,seg_mode), 
			sizeof(isp.p_awb->seg_mode), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,rg_start), 
			sizeof(isp.p_awb->rg_start), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,rgmin), 
			sizeof(isp.p_awb->rgmin), pbdata, startPos, trans_len);																
	CopyToMember(isp.p_awb, offsetof(AWB,rgmax), 
			sizeof(isp.p_awb->rgmax), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,weight_in), 
			sizeof(isp.p_awb->weight_in), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,weight_mid), 
			sizeof(isp.p_awb->weight_mid), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,ymin), 
			sizeof(isp.p_awb->ymin), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,ymax), 
			sizeof(isp.p_awb->ymax), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,hb_rate), 
			sizeof(isp.p_awb->hb_rate), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,hb_class), 
			sizeof(isp.p_awb->hb_class), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,hr_rate), 
			sizeof(isp.p_awb->hr_rate), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,hr_class), 
			sizeof(isp.p_awb->hr_class), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,yuv_mod_en), 
			sizeof(isp.p_awb->yuv_mod_en), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,cb_th), 
			sizeof(isp.p_awb->cb_th), pbdata, startPos, trans_len);								
	CopyToMember(isp.p_awb, offsetof(AWB,cr_th), 
			sizeof(isp.p_awb->cr_th), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,cbcr_th), 
			sizeof(isp.p_awb->cbcr_th), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,ycbcr_th), 
			sizeof(isp.p_awb->ycbcr_th), pbdata, startPos, trans_len);
	CopyToMember(isp.p_awb, offsetof(AWB,awb_tab), 
			sizeof(isp.p_awb->awb_tab), pbdata, startPos, trans_len);
	
}


