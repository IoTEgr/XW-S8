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
#ifndef HAL_ISP_H
#define HAL_ISP_H


#define  	CMOS_USART_ONLINE_DBG          	0
#define  	CMOS_USB_ONLINE_DBG      		0
#define		COMMOM_ONLEIN_DBG				(0&&CMOS_USB_ONLINE_DBG)


void isp_process_check(void);
void isp_table_load(u32 gma_num,u32 contra_num,u32 rgbgma_num);
void isp_switch_senser_set_gain_exp();

void isp_all_model_cfg(u32 isp_all_mod);
bool exposal_adj(u32 yloga_avg,u32 yloga,u32 * p_br,u32 exp_ext_mod);
void awb_manual_gain_adapt(u32 awb_scene_mod);

void awb_scene_mod_set(u32 awb_scene_mod);
void sensor_compensate_exp_set(u32 compensate_mod);

/*******************************************************************************
* Function Name  : isp_get_scen_light
* Description    : get light level from sensor
* Input          : none
* Output         : none                                            
* Return         : level value(0~255)  0: max dark  255: max light
*******************************************************************************/
u8 isp_get_scen_light(void);



void sensor_frame_rate_adapt(u32 frequecy_mode,u32 frame_rate);
//==rotate : 0 is 0' , 3 is 180' , 0x1  is flip , 0x2 is mirror==
void hal_senser_rotate(u8 rotate);

extern bool (*fp_sensor_exposure)(u32 yloga_avg,u32 yloga,u32 * p_br,u32 exp_ext_mod);
void uart_to_isp(u8 *p);

void isp_init(void);


int hal_ispFrameCnt(u8 clear);

void hal_ispService(void);

/*******************************************************************************
* Function Name  : commom_isr
* Description    : these option hasn't double reg,so they must be update during frame blanking
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void hal_commom_isr(void);

/*******************************************************************************
* Function Name  : hal_isp_sof_loop
* Description    : if the modules are not adaptive,should put in hal_isp_sof_loog
* Input          : none
* Output         : none                                            
* Return         : none
*******************************************************************************/
void hal_isp_sof_loop();

/*******************************************************************************
* Function Name  : init_common_str
* Description	  : initial common_adapt structure
* Input		      :  
* Output		  : None
* Return		  : None
*******************************************************************************/
void init_common_str(void);

/*******************************************************************************
* Function Name  : hal_isp_debug_write
* Description    : isp online debug.
* Input          : 
* Output         : none                                            
* Return         : none
*******************************************************************************/
void hal_isp_debug_write(u8 *pbdata ,u32 trans_len, u32 param_3B);

/*******************************************************************************
* Function Name  : hal_isp_debug_read
* Description    : isp hal_isp_debug_read read
* Input          : u8 *pbdata ,u32 trans_len, u32 param_3B
* Output         : none                                            
* Return         : none
*******************************************************************************/
void hal_isp_debug_read(u8 *pbdata ,u32 trans_len, u32 param_3B);

/*******************************************************************************
* Function Name  : hal_Sensor_common_Set
* Description	  : sensor on line get sensor ID
* Input		      :  
* Output		  : None
* Return		  : None
*******************************************************************************/
void hal_Sensor_common_Set(u8 *pbdata ,u32 trans_len, u32 param_3B);


/*******************************************************************************
* Function Name  : CopyToMember
* Description	  : copy data to structure base on offset
* Input		      :  
* Output		  : None
* Return		  : None
*******************************************************************************/
void CopyToMember(void* ispModuleData, int offsetOfMember, int sizeOfMember, void* pbdata, int startPos, int trans_len);

/*******************************************************************************
* Function Name  : hal_Sensor_common_Get
* Description	  : sensor on line get sensor ID and so on
* Input		      :  
* Output		  : None
* Return		  : None
*******************************************************************************/
void hal_Sensor_common_Get(u8 *pbdata ,u32 trans_len, u32 param_3B);

/*******************************************************************************
* Function Name  : ygama_update
* Description	  : update yagama structure
* Input		      :  
* Output		  : None
* Return		  : None
*******************************************************************************/
void ygama_update(u8 *pbdata ,u32 trans_len, u32 param_3B);

/*******************************************************************************
* Function Name  : awb_update
* Description	  : update awb structure
* Input		      :  
* Output		  : None
* Return		  : None
*******************************************************************************/
void awb_update(u8 *pbdata ,u32 trans_len, u32 param_3B);

#define _EVSTEP_TAB_LEN_  132//68
extern const u16 Ratio_of_Evstep[_EVSTEP_TAB_LEN_];



extern int _sensor_resource_start_addr;
#define SEN_RES_FLASH_ADDR(x)         ((u32)(x)  + ((u32)&_sensor_resource_start_addr))
 	
#endif
