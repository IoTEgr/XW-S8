/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                  THE APPOTECH HAL
   **** **                 MJPEG HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_mjpeg.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN MJPEG HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"
typedef struct
{
/*
	u8 y_buf[640 * 480] __attribute__ ((aligned(32)));	
	u8 uv_buf[640 * 480/2] __attribute__ ((aligned(32)));
*/
	u8 y_buf[8] __attribute__ ((aligned(32)));	
	u8 uv_buf[8] __attribute__ ((aligned(32)));

}JBLINE_BUF;

//u8 uvc_jbuf[512*1024] __attribute__ ((aligned(64))); 
JBLINE_BUF jBLbuf;
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool  usensor_actech_to_mjpBcodec(void)
{
		static volatile u8 *pbuf;
		static volatile u32 plen;
		static volatile u32 plid;
		//debg("usensor_actech_to_mjpBcodec\n");
		if(usb_usensor_frame_read((u8 **)&pbuf,(u32 *)&plen ,(u32 *)&plid)){
			//debg("usb_usensor_frame_read\n");
			if(hal_mjpegDecodeParse((u8 *)pbuf,320,240)){
				debg("usensor: <1st mjp decode fail>\n");	
				huvc_dcod_down(1);
				//usb_usensor_frame_free();
				return false;	
			}
			//usb_usensor_frame_free();
			huvc_dcod_down(0);
			debg("usensor: <1st mjp decode ok>\n");
			return true;
		}		
		return false;
}

typedef struct
{
	bool actech;
	bool astern;
	bool usensor_in;
	bool lowpower;
	u32 usensor_width;
	u32 usensor_height;
	
}BACKREC_ADPT;

BACKREC_ADPT backrecadpt; 
bool backrec_adpt(void)
{
	return  backrecadpt.actech;
}
void backrec_adpt_set(bool stat)
{
	backrecadpt.actech = stat;
}
void backrec_sensor(bool stat)
{
	backrecadpt.usensor_in =stat ;
}
bool backrec_sensor_stat(void)
{
	return backrecadpt.usensor_in ;
}

//unlink usensor, unlinck uvc dev has lincking
bool (*fp_mjpBcodec_linking)(void);
void api_usensor_reg_to_mjpBcodec(bool (*callback)(void))
{
	fp_mjpBcodec_linking = callback;	
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usensor_actech(void)
{
//	backrecadpt.actech = true;
	backrecadpt.usensor_width  = 640;
	backrecadpt.usensor_height = 480; 

	//hal_huvc_strm_init(uvc_jbuf, sizeof(uvc_jbuf));
	hal_huvc_strm_init();
	//register to mjp,lcd
	api_usensor_reg_to_mjpBcodec(usensor_actech_to_mjpBcodec);
	debg("usensor: <actech ok>\n");

}
void api_backrec_linking(void)
{
	if(fp_mjpBcodec_linking){
		if(true == (*fp_mjpBcodec_linking)())	
		{ //usensor_actech_to_mjpBcodec
			backrecadpt.actech = true;
			backrecadpt.usensor_in = true;
			backrecadpt.lowpower =false;
			api_usensor_reg_to_mjpBcodec(NULL);
			debg("usensor: <linking to lcd ok>\n");
		}
	}	
}
void api_backrec_relinking(void){
	api_usensor_reg_to_mjpBcodec(usensor_actech_to_mjpBcodec);
}
bool api_backrec_status(void)
{
    return  backrecadpt.actech;
}
bool api_astern_status(void)
{
	return backrecadpt.astern;
}
void api_astern_set_status(bool sta)
{
	backrecadpt.astern = sta;
}

static u8 reverse_mod;
static u8 reverse_sta;
extern u8 dispLayer_getABwin(void);
void save_fst_reverse_mod(void)
{
	reverse_mod = dispLayer_getABwin();
}

void api_backsta_linking(void)
{
	static s8 sync,astern=0;
	if(backrecadpt.actech){
		/**/
		if(sync == false){
			sync = true;
			//reverse_mod = dispLayer_getABwin();	
			return;
		}
		uhost_backsta_cmd();
		//u8 backstabuf[4];
		//get_usensorsta(backstabuf);
		if(reverse_sta != check_backsta())
		{
			if(check_backsta() == true)
			{
				//save_fst_reverse_mod();
				if(astern == 0)
				{
					backrecadpt.astern = true;
					//layout_astern_enter(); //osd show back line
					//dispLayer_ABwin(/*DISP_PIP_BACK*/1);
				}
				astern = 1;
			}
			else
			{
				if(astern)
				{
					backrecadpt.astern = false;
					//layout_astern_exit(); //osd off back line
					//dispLayer_ABwin(/*DISP_PIP_BACK_RIGHTTOP*/reverse_mod);	
				}
				astern = 0;
			}			
		}
		reverse_sta = check_backsta();
		
	}else{
		reverse_sta = -1;
		sync = false;	
	}
}
void usensor_detach(void)
{
	usb_enum_phase_clr();
	backrecadpt.actech = false;
	backrecadpt.usensor_in = false;
	backrecadpt.usensor_width  = 0;
	backrecadpt.usensor_height = 0;
	api_mjp_avi_stop();
	api_usensor_reg_to_mjpBcodec(NULL);
	//layout_astern_exit(); //osd off back line
}
extern void dispLayerPIPOnlyFront(void);
//extern int layout_astern_exit(void);
void backrec_stop(void)
{
	if(!backrecadpt.actech){
		return;
	}
	ax32xx_intEnable(IRQ_USB20,0);
	usensor_detach();
	backrecadpt.lowpower=true; //stop file start
	dispLayerPIPOnlyFront();
	//layout_astern_exit(); //osd off back line
	//off back record vdd
#if USENSOR_PWRCTRL_EN
		hal_gpioWrite(USB2_PWR_CH,USB2_PWR_PIN,GPIO_LOW);	
#endif
	uvc_host_dev_detech();
}
bool backrec_lowerpower(void)
{
	return backrecadpt.lowpower;
}

typedef enum
{
	_JDC_FREE_ = 0,
	_JDC_BUSY_ = 1,	
	_JDC_DOWN_ = -1,
	_JDC_ERR_  = -2,		
}JBDC_STA;
volatile s8 jbdcsta;
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool hal_mjpB_decoding(void)
{
	if(jbdcsta == _JDC_BUSY_){
		return 1;	
	}	
	return 0;
}
void hal_mjpB_dcdown(void)
{
	jbdcsta = _JDC_DOWN_;	
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
volatile u8 debg_jdec;
extern void (*fp_jpegdc_ldone_hal)(void);
extern bool hal_mjpB_ecoding(void);
u8 *mjpBcodec_kick(void)
{	
	static volatile u8 *pbuf;
	static volatile u32 plen;
	static volatile u32 plid;	
	if(backrecadpt.actech &&!hal_mjpB_decoding() && !hal_mjpB_ecoding()){
		
		if(usb_usensor_frame_read((u8 **)&pbuf,(u32 *)&plen ,(u32 *)&plid)){	
		//debg monitor
		if(fp_jpegdc_ldone_hal == NULL)	{
			debg_jdec = 0;
		}else{
			
			debg_jdec++;
			if(debg_jdec >= 2){
				debg("jcodec: <logic err:%x>\n",debg_jdec);exception_trigger();while(1);	
			}	
		}
			//debg("{");
			jbdcsta = _JDC_BUSY_; 
			hal_mjpB_ecbusy();
			//debg("s:%x",jbdcsta);
			//debg("{%dK",plen/1024);
			ax32xx_mjpB_reset();
			ax32xx_mjpegDecodeODma1Cfg(jBLbuf.y_buf,jBLbuf.uv_buf, true);
			return (u8 *)pbuf;
		}
	}
	return NULL;
}
void mjpBcodec_sacle_dwon(u32 flg)
{
	//debg("}");//debg("d:%x",(bool)~jbdcsta);	
	if(flg & (BIT(8) | BIT(5)|BIT(12)|BIT(13))){
		huvc_dcod_down(1);//usb_usensor_frame_free();
		jbdcsta = _JDC_ERR_;
		//debg("x");
		//if(fp_jpegdc_ldone_hal)
		hal_mjpB_ecdown();
		return;
	}
	//decode frame done,DMA0 lcd sacle
	if(flg & (1<<6)){
		if(fp_jpegdc_ldone_hal == NULL){
			//usb_usensor_frame_free();
			huvc_dcod_down(0);
		}
		jbdcsta = _JDC_DOWN_;
	}
	//decode yuv422 done,DMA1 backmjpec
	if(flg & (1<<2)){
		//debg("e");
		if(fp_jpegdc_ldone_hal){
			
			(*fp_jpegdc_ldone_hal)(); //call hal_mjpB_manualstart	
			//debg("%x",hal_mjpB_ecoding());
		}else{
			//debg("#");
			hal_mjpB_ecdown();	
		}
	}
	else{
		if(fp_jpegdc_ldone_hal){
			huvc_dcod_down(1);//usb_usensor_frame_free();
			jbdcsta = _JDC_ERR_;
			hal_mjpB_ecdown();	
		}
	}
	if(flg & ~(BIT(8) | BIT(5) | BIT(6) | BIT(2) | BIT(16)|BIT(12)|BIT(13))){
		debg("dc:%x\n",flg & ~(BIT(8) | BIT(5) | BIT(6) | BIT(2) |BIT(16)|BIT(12)|BIT(13)));
	}
	
}
void pt_uvcframe(u32 f)
{
	//u32 ja = cal_jA_1s_size()>>20;
	//u32 jb = cal_jB_1s_size()>>20;
	//u8  jaf = hal_jA_fcnt_get();
	//u8  jbf = hal_jB_fcnt_get();
	//u8 csif = hal_csi_fcnt_get();
	
	/*
	u8 sbuf[]={'u',':','0','0','\0'};
	sbuf[2] = f/10+'0';
	sbuf[3] = f%10+'0';
	
	
	sbuf[2] = ja/1000+'0';
	sbuf[3] = (ja%1000)/100+'0';
	sbuf[4] = (ja%100)/10+'0';
	sbuf[5] = (ja%10)+'0';
	sbuf[7] = jaf/10+'0';
	sbuf[8] = jaf%10+'0';
	sbuf[9] = jb/1000+'0';
	sbuf[10] = (jb%1000)/100+'0';
	sbuf[11] = (jb%100)/10+'0';
	sbuf[12] = (jb%10)+'0';
	 */
	//LogPrint(sbuf,0);

}

void api_usensor_1S_pollmonitor(void)
{
	static u32 cnt_out;
	if(backrecadpt.actech == true){
		u32 f = api_uvc_fcnt_get();
		pt_uvcframe(f);
		if(f == 0){
			cnt_out ++;
			//debg("ERR: <usensor no tranfer cnt%d>\n",cnt_out);
			//sysLog("no Transfer");
			if(cnt_out >= 6){
				sysLog("No Transfer");
				cnt_out =0;
				debg("ERR: <usensor no tranfer !!!>\n");
				if(usb_host_type ==2)
					usb_host_remove();
				if(usb_host_type ==1)
					usb11_host_remove();
			}
		}
		else
			cnt_out = 0;
	}
	
}

