/******************** (C) COPYRIGHT 2013 BUILDWIN ************************
* File Name          : uart.c
* Author             : pengtao_zhang
* Version            : V0719a
* Date               : 07/19/2013 ;mm/dd/yyyy
* Description        : This file provides all the uart firmware functions.
***************************************************************************/
//#include "../../mcu/mcu_include.h"
#include "../inc/hal.h"
#include "hal_usb_dev_enum.h"
#include "hal_usb_uvc.h"

bool (*fp_uvc_unit_ctl_hal)(u8 val, u8 rqu, u8 len);
bool (*fp_uvc_probe_ctl_hal)(u8 val, u8 rqu, u8 len);
bool (*fp_uvc_unit_ctl_ex_hal)(u8 val, u8 rqu, u8 len);
bool (*fp_usb_ep0_tx)(u8* txbuf, u8 len);
bool (*fp_usb_iso_tx)(u8 epx, u32 adr, u16 len);

#define write_usb_reg(a,b) (a = b)
//#define CSR0 USB20_CSR0

#define __RQU_TYPES__	14
/*#define RC_UNDEFINED    0x00 */
#define SET_CUR  		0x01 
#define SET_CUR_ALL  	0x11
#define GET_CUR  		0x81 
#define GET_MIN  		0x82 
#define GET_MAX  		0x83 
#define GET_RES 		0x84 
#define GET_LEN  		0x85 
#define GET_INFO  		0x86 
#define GET_DEF 		0x87 
#define GET_CUR_ALL 	0x91 
#define GET_MIN_ALL 	0x92 
#define GET_MAX_ALL  	0x93 
#define GET_RES_ALL  	0x94 
#define GET_DEF_ALL  	0x97 

#define __PU_TYPES__	0x14
#define CT_CONTROL_UNDEFINED 0x00			
#define PU_BACKLIGHT_COMPENSATION_CONTROL 0x01
#define PU_BRIGHTNESS_CONTROL  0x02 
#define PU_CONTRAST_CONTROL  0x03 
#define PU_GAIN_CONTROL  0x04 
#define PU_POWER_LINE_FREQUENCY_CONTROL  0x05 
#define PU_HUE_CONTROL  0x06 
#define PU_SATURATION_CONTROL  0x07 
#define PU_SHARPNESS_CONTROL  0x08 
#define PU_GAMMA_CONTROL  0x09 
#define PU_WHITE_BALANCE_TEMPERATURE_CONTROL  0x0A 
#define PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL  0x0B 
#define PU_WHITE_BALANCE_COMPONENT_CONTROL  0x0C 
#define PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL  0x0D 
#define PU_DIGITAL_MULTIPLIER_CONTROL  0x0E 
#define PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL  0x0F 
#define PU_HUE_AUTO_CONTROL  0x10 
#define PU_ANALOG_VIDEO_STANDARD_CONTROL  0x11 
#define PU_ANALOG_LOCK_STATUS_CONTROL  0x12 
#define PU_CONTRAST_AUTO_CONTROL  0x13 


typedef struct
{
	u8 type;
	u8 len;
	u8 val[8];
}RQU_TYPE;
	
typedef struct
{
	RQU_TYPE item[__RQU_TYPES__];
	
}RQU_CODE;


const RQU_CODE pu_rqu[__PU_TYPES__]=
{
	
//PU_CONTROL_UNDEFINED  0x00 
	{
		{
			{SET_CUR,		2, {-1,0x00}},  		 
			{SET_CUR_ALL,  	2, {-1,0x00}},
			{GET_CUR, 		2, {-1,0x00}},
			{GET_MIN,  		2, {-1,0x00}},
			{GET_MAX,  		2, {-1,0x00}},
			{GET_RES, 		2, {-1,0x00}},
			{GET_LEN,  		2, {-1,0x00}},
			{GET_INFO,  	1, {-1,0x00}},
			{GET_DEF, 		2, {-1,0x00}},
			{GET_CUR_ALL, 	2, {-1,0x00}},
			{GET_MIN_ALL, 	2, {-1,0x00}},
			{GET_MAX_ALL,  	2, {-1,0x00}},
			{GET_RES_ALL,  	2, {-1,0x00}},
			{GET_DEF_ALL,  	2, {-1,0x00}},
		}
	},	

//PU_BACKLIGHT_COMPENSATION_CONTROL  0x01 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x02,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_BRIGHTNESS_CONTROL  0x02 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x40,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_CONTRAST_CONTROL  0x03 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x04,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_GAIN_CONTROL  0x04 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x02,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_POWER_LINE_FREQUENCY_CONTROL  0x05 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x02,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_HUE_CONTROL  0x06 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x02,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_SATURATION_CONTROL  0x07 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x02,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_SHARPNESS_CONTROL  0x08 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x02,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_GAMMA_CONTROL  0x09 
	{
		{
			{SET_CUR,		2, {0x00,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		2, {0x08,0x00}},
			{GET_MAX,  		2, {0x10,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {0x02,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		2, {0x0a,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},	
//PU_WHITE_BALANCE_TEMPERATURE_CONTROL  0x0A 
	{
		{
			{SET_CUR,		2, {0x0a,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		8, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
			{GET_MAX,  		8, {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}},
			{GET_RES, 		8, {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
			{GET_LEN,  		2, {0x08,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		8, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},
//PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL  0x0B 
	{
		{
			{SET_CUR,		2, {0x0a,0x00}},  		 
			{SET_CUR_ALL,  	2, {0x00,0x00}},
			{GET_CUR, 		2, {0x0a,0x00}},
			{GET_MIN,  		8, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
			{GET_MAX,  		8, {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}},
			{GET_RES, 		8, {0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
			{GET_LEN,  		2, {0x08,0x00}},
			{GET_INFO,  	1, {0x03,0x00}},
			{GET_DEF, 		8, {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}},
			{GET_CUR_ALL, 	2, {0x00,0x00}},
			{GET_MIN_ALL, 	2, {0x00,0x00}},
			{GET_MAX_ALL,  	2, {0x00,0x00}},
			{GET_RES_ALL,  	2, {0x00,0x00}},
			{GET_DEF_ALL,  	2, {0x00,0x00}},
		}
	},
//PU_WHITE_BALANCE_COMPONENT_CONTROL  0x0C 
//PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL  0x0D 
//PU_DIGITAL_MULTIPLIER_CONTROL  0x0E 
//PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL  0x0F 
//PU_HUE_AUTO_CONTROL  0x10 
//PU_ANALOG_VIDEO_STANDARD_CONTROL  0x11 
//PU_ANALOG_LOCK_STATUS_CONTROL  0x12 
//PU_CONTRAST_AUTO_CONTROL  0x13 
 	
};
const RQU_CODE pu_rqu_ex[3]=
{
//PU_CONTROL_UNDEFINED  0x00 
	{
		{
			{SET_CUR,		2, {-1,0x00}},  		 
			{SET_CUR_ALL,  	2, {-1,0x00}},
			{GET_CUR, 		2, {-1,0x00}},
			{GET_MIN,  		2, {-1,0x00}},
			{GET_MAX,  		2, {-1,0x00}},
			{GET_RES, 		2, {-1,0x00}},
			{GET_LEN,  		2, {-1,0x00}},
			{GET_INFO,  	1, {-1,0x00}},
			{GET_DEF, 		2, {-1,0x00}},
			{GET_CUR_ALL, 	2, {-1,0x00}},
			{GET_MIN_ALL, 	2, {-1,0x00}},
			{GET_MAX_ALL,  	2, {-1,0x00}},
			{GET_RES_ALL,  	2, {-1,0x00}},
			{GET_DEF_ALL,  	2, {-1,0x00}},
		}
	},		
//?? 0x01
	{
		{
			{SET_CUR,		2, {-1,0x00}},  		 
			{SET_CUR_ALL,  	2, {-1,0x00}},
			{GET_CUR, 		2, {-1,0x00}},
			{GET_MIN,  		2, {-1,0x00}},
			{GET_MAX,  		2, {-1,0x00}},
			{GET_RES, 		2, {-1,0x00}},
			{GET_LEN,  		2, {-1,0x00}},
			{GET_INFO,  	1, {-1,0x00}},
			{GET_DEF, 		2, {-1,0x00}},
			{GET_CUR_ALL, 	2, {-1,0x00}},
			{GET_MIN_ALL, 	2, {-1,0x00}},
			{GET_MAX_ALL,  	2, {-1,0x00}},
			{GET_RES_ALL,  	2, {-1,0x00}},
			{GET_DEF_ALL,  	2, {-1,0x00}},
		}
	},	
//?? 0x02
	{
		{
			{SET_CUR,		2, {-1,	0x00}},  		 
			{SET_CUR_ALL,  	2, {-1,	0x00}},
			{GET_CUR, 		2, {-1,	0x00}},
			{GET_MIN,  		2, {0x00,0x00}},
			{GET_MAX,  		2, {0xff,0x00}},
			{GET_RES, 		2, {0x01,0x00}},
			{GET_LEN,  		2, {-1,0x00}},
			{GET_INFO,  	1, {3,0x00}},
			{GET_DEF, 		2, {0x73,0x00}},
			{GET_CUR_ALL, 	2, {-1,0x00}},
			{GET_MIN_ALL, 	2, {-1,0x00}},
			{GET_MAX_ALL,  	2, {-1,0x00}},
			{GET_RES_ALL,  	2, {-1,0x00}},
			{GET_DEF_ALL,  	2, {-1,0x00}},
		}
	},	
	
};



u8  uvc_video_probe_commit_value[26]=
{
#if UVC_MOD	== UVC_MJP

	#if 0
	0x1f,0x00, 					//bmHint
	0x01,						// bFormatIndex 
	0x01,						//bFrameIndex  
	0x15, 0x16, 0x05, 0x00, 	// byte of dwFrameInterval 333333 us 
	0x00, 0x00, 				//wKeyFrameRate
	
	0x00, 0x00,					//wPFrameRate 
	0x00, 0x00,					//wCompQuality
	0x00, 0x00, 				//wCompWindowSize
	0xff, 0x00, 				//wDelay
	#if __VGA__
		0x00 ,0x60,0x09,0x00, 		//dwMaxVideoFrameSize
	#else
		0x00,0x20,0x1c,0x00,
	#endif
	(__UVC_PKG_SZ__ &0xff), (__UVC_PKG_SZ__ >>8),  				//dwMaxPayloadTransferSize
	0x00, 0x00					//dwMaxPayloadTransferSize
	#else
#if UVC_VGA
	0x01, 0x00, DFLT_SOL_VGA, 0x01, 0x15, 0x16, 0x05, 0x00,
#else
	0x01, 0x00, DFLT_SOL, 0x01, 0x15, 0x16, 0x05, 0x00,
#endif
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x20, 0x1C, 0x00, 0x00, 0x0C,
	0x00, 0x00,	
	#endif
#else
	0x00,0x00, 					//bmHint
	0x01,						// bFormatIndex 
	0x01,						//bFrameIndex  
	0x15, 0x16, 0x05, 0x00, 	// byte of dwFrameInterval 333333 us 
	0x00, 0x00, 				//wKeyFrameRate
	
	0x00, 0x00,					//wPFrameRate 
	0x00, 0x00,					//wCompQuality
	0x00, 0x00, 				//wCompWindowSize
	0x00, 0x00, 				//wDelay
	#if __VGA__
		0x00 ,0x60,0x09,0x00, 		//dwMaxVideoFrameSize
	#else
		0x00,0x20,0x1c,0x00,
	#endif
	(__UVC_MIF_SZ__*3 &0xff), (__UVC_MIF_SZ__*3 >>8),  				//dwMaxPayloadTransferSize
	0x00, 0x00	
#endif
};

u8 UVC_Still_Probe_Commit[11] =
{
#if UVC_MOD	== UVC_MJP
	0x01,						//bFormatIndex 
	0x01,						//bFrameIndex
	0x00,						// bCompressionIndex
	#if __VGA__
		0x00 ,0x60,0x09,0x00, 		//dwMaxVideoFrameSize
	#else
		0x00,0x2c,0x1c,0x00,
	#endif
	(__UVC_PKG_SZ__&0xff), (__UVC_PKG_SZ__>>8),  				//dwMaxPayloadTransferSize
	0x00, 0x00					//dwMaxPayloadTransferSize
#else
	0x01,						//bFormatIndex 
	0x01,						//bFrameIndex
	0x00,						// bCompressionIndex
	#if __VGA__
		0x00 ,0x60,0x09,0x00, 		//dwMaxVideoFrameSize
	#else
		0x00,0x2c,0x1c,0x00,
	#endif
	(__UVC_MIF_SZ__*3 &0xff), (__UVC_MIF_SZ__*3 >>8),  				//dwMaxPayloadTransferSize
	0x00, 0x00					//dwMaxPayloadTransferSize
#endif
};

/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool uvc_unit_ctl_ex_hal(u8 val, u8 rqu, u8 len)
{
	u8 i ;
	pSetReqCallback = NULL;
	if(val < __PU_TYPES__){
	
		for(i = 0; i < __RQU_TYPES__; i++){
			if(pu_rqu_ex[val].item[i].type == rqu){
				u8 *pval = (u8* )pu_rqu_ex[val].item[i].val;
				u8 len   = pu_rqu_ex[val].item[i].len;
				(*fp_usb_ep0_tx)(pval, len);
				return true;
			}	
		}
		
		debg("__Err__:rqu:%x,%x\n",rqu,val);	
		return false;		
	}	
	else{
		//debg("__Err__: val:%x\n",val);	
		return false;
	}
}


/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool uvc_unit_ctl_hal(u8 val, u8 rqu, u8 len)
{	
	u8 i ;
	pSetReqCallback = NULL;
	//PUè¯·æ±index
	if(val < __PU_TYPES__){
	
		for(i = 0; i < __RQU_TYPES__; i++){
			if(pu_rqu[val].item[i].type == rqu){
				u8 *pval = (u8* )pu_rqu[val].item[i].val;
				u8 len   = pu_rqu[val].item[i].len;
				(*fp_usb_ep0_tx)(pval, len);
				return true;
			}	
		}
		//debg("__Err__:rqu:%x\n",val);	
		return false;		
	}	
	else{
		//debg("__Err__: val:%x\n",val);	
		return false;			
	}
}



/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool uvc_video_probe_control_callback(u8* rxbuf)
{
	u8 i;
	for(i = 0; i < 22; i++){
		uvc_video_probe_commit_value[i] = rxbuf[i];
	}	
	//debg("set26:");debgbuf(rxbuf,26);
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void uvc_video_probe_control(u8 request,u8 len)
{
	if (request == UVC_SET_CUR){
		pSetReqCallback = uvc_video_probe_control_callback;
		usb_dev.ep0_state = RECEIVING_STATE;
		write_usb_reg(USB20_CSR0, BIT(6));           //æ¸é¤RxRdy	
	}
	else if((request == UVC_GET_CUR) || (request == UVC_GET_MIN) || (request == UVC_GET_MAX) || (request == UVC_GET_DEF)){
		//debg("ack26:");debgbuf(uvc_video_probe_commit_value,sizeof(uvc_video_probe_commit_value));
		(*fp_usb_ep0_tx)((u8* )uvc_video_probe_commit_value,sizeof(uvc_video_probe_commit_value));
		//debgbuf(uvc_video_probe_commit_value,sizeof(uvc_video_probe_commit_value));
		
	}
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
BOOL uvc_video_commit_control_callback(u8* rxbuf)
{
	u8 i;
	for(i = 0; i < 22; i++){
		uvc_video_probe_commit_value[i] = rxbuf[i];
	}
	//debg("set26:");debgbuf(rxbuf,26);
	return true;	
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void uvc_video_commit_control(u8 request,u8 len)
{
	if (request == UVC_SET_CUR){
		pSetReqCallback = uvc_video_commit_control_callback;
		usb_dev.ep0_state = RECEIVING_STATE;
		write_usb_reg(USB20_CSR0, BIT(6));           //æ¸é¤RxRdy	
	}
	else if((request == UVC_GET_CUR) || (request == UVC_GET_MIN) || (request == UVC_GET_MAX) || (request == UVC_GET_DEF)){
		(*fp_usb_ep0_tx)((u8* )uvc_video_probe_commit_value,sizeof(uvc_video_probe_commit_value));
	}
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
BOOL uvc_still_probe_control_callback(u8* rxbuf)
{
	u8 i;
	for(i = 0; i < 7; i++){
		UVC_Still_Probe_Commit[i] = rxbuf[i];
	}
	//debg("set 11:");debgbuf(rxbuf,11);
	return true;
}

/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void uvc_still_probe_control(u8 request,u8 len)
{
	if (request == UVC_SET_CUR){
		pSetReqCallback = uvc_still_probe_control_callback;
		usb_dev.ep0_state = RECEIVING_STATE;
		write_usb_reg(USB20_CSR0, BIT(6));           //æ¸é¤RxRdy	
	}
	else if((request == UVC_GET_CUR) || (request == UVC_GET_MIN) || (request == UVC_GET_MAX) || (request == UVC_GET_DEF)){
		//debg("ack11:");debgbuf(UVC_Still_Probe_Commit,sizeof(UVC_Still_Probe_Commit));
		(*fp_usb_ep0_tx)((u8* )UVC_Still_Probe_Commit,sizeof(UVC_Still_Probe_Commit));
	}
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool uvc_still_commit_control_callback(u8* rxbuf)
{
	u8 i;
	for(i = 0; i < 7; i++){
		UVC_Still_Probe_Commit[i] = rxbuf[i];
	}	
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void uvc_still_commit_control(u8 request,u8 len)
{
	if (request == UVC_SET_CUR){
		pSetReqCallback = uvc_still_commit_control_callback;
		usb_dev.ep0_state = RECEIVING_STATE;
		write_usb_reg(USB20_CSR0, BIT(6));           //æ¸é¤RxRdy	
	}
	else if((request == UVC_GET_CUR) || (request == UVC_GET_MIN) || (request == UVC_GET_MAX) || (request == UVC_GET_DEF)){
		(*fp_usb_ep0_tx)((u8* )UVC_Still_Probe_Commit,sizeof(UVC_Still_Probe_Commit));
	}
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool uvc_probe_ctl_hal(u8 val, u8 rqu, u8 len)
{
		pSetReqCallback = NULL;
		switch(val)
		{
			case /*1*/UVC_VS_PROBE_CONTROL:
				//debg("UVC_VS_PROBE_CONTROL\n");
				uvc_video_probe_control(rqu,len);
				return true;
			case /*2*/UVC_VS_COMMIT_CONTROL:
				//deg_Printf("UVC_VS_COMMIT_CONTROL\n");
				uvc_video_commit_control(rqu,len);
				return true;
			case /*3*/UVC_VS_STILL_PROBE_CONTROL:
				//deg_Printf("UVC_VS_STILL_PROBE_CONTROL\n");
				uvc_still_probe_control(rqu,len);
				return true;
			case /*4*/UVC_VS_STILL_COMMIT_CONTROL:
				//deg_Printf("UVC_VS_STILL_COMMIT_CONTROL\n");
				uvc_still_commit_control(rqu,len);
				return true;
			case /*5*/UVC_VS_STILL_IMAGE_TRIGGER_CONTROL:
				//deg_Printf("UVC_VS_STILL_IMAGE_TRIGGER_CONTROL\n");
				//uvc_still_trigger_control(rqu,len);
				return false;
		}	
	return false;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void uvc_epx_cfg(void)
{
	/*
	USB20_Index = __UVC_EPX__;		
	USB20_InMaxPl  = 0xe4;
	USB20_OutMaxPl = 0xe4;
	USB20_InMaxPh  = 0x1b;
	USB20_OutMaxPh = 0x1b;
	USB20_OutCSR2 = 0x60; //enable iso
	USB20_InCSR2 = 0x60; //enable iso	
	*/
	USB20_Index = __UVC_EPX__;		
	USB20_InMaxPl  = (u8)(__UVC_MIF_SZ__ & 0xff);//0xe4;
	USB20_OutMaxPl = (u8)(__UVC_MIF_SZ__ & 0xff);//0xe4;
	USB20_InMaxPh  = (u8)(__UVC_MIF_SZ__ >> 8) | BIT(4)| BIT(3);//0x1b;
	USB20_OutMaxPh = (u8)(__UVC_MIF_SZ__ >> 8) | BIT(4)| BIT(3);//0x1b;
	USB20_OutCSR2 = 0x60; //enable iso
	USB20_InCSR2 = 0x60; //enable iso	
	fp_usb_iso_tx = usb_iso_tx;
//extern bool (*uvc_on_hal)(void);
//extern void (*uvc_off_hal)(void);
//void mjp_actech_as_jpguvc(u32 hsize, u32 vsize);
//void mjp_stop(void);

}

//volatile u8 mjpflg;
//volatile u8 toggle;
//volatile u8 endframe;
//volatile u8 still;
//STRMHAL *uvcjhal; 
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
#if UVC_VGA
PROBE const probe[2]={

	{SOL1_VGA_W, SOL1_VGA_H},//{640, 480},
	{SOL2_VGA_W, SOL2_VGA_H},//{320, 240},
};
#else
PROBE const probe[5]={
	{SOL1_W, SOL1_H},//{1280, 720},
	{SOL2_W, SOL2_H},//{800, 600},
	{SOL3_W, SOL3_H},//{640, 480},
	{SOL4_W, SOL4_H},//{720, 480},
	{SOL5_W, SOL5_H},//{320, 240},
};
#endif
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void uvc_on(void)
{
#if UVC_MOD	== UVC_MJP
	u8 winid = uvc_video_probe_commit_value[3];
	winid -=1 ;	
	#if UVC_VGA
	if(winid <= 1){
		//deg_Printf("")
		hal_mjpegUVCStart(probe[winid].win_w,probe[winid].win_h,JPEG_Q_50,0);//jpg_q[2]);
	}
	#else
	if(winid <= 4){
		hal_mjpegUVCStart(probe[winid].win_w,probe[winid].win_h,0,0);//jpg_q[2]);
		//set_wave();
	}
	#endif
	debg("w,h:%d,%d\n",probe[winid].win_w,probe[winid].win_h);
#else
	hal_mjpegUVCYUVStart(640,480);
#endif
}
void uvc_off(void)
{	
	ax32xx_mjpegEncodeEnable(0);
//	mjp_stop();	
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
BOOL uvc_pkg_tx(u8 pkgtoggle, u8* addr, u16 len)
{
	u8 *pbuf = addr;
	memset(pbuf,0,0x0c);
	pbuf[0] = 0x0c;
	pbuf[1] = 0x8c | pkgtoggle;
	return (*fp_usb_iso_tx)(__UVC_EPX__,(u32)pbuf,len);	

}
volatile u32 ptgl;
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uvc_header(u32 tgl, u8 *pbuf)
{
	static volatile u8 toggle;
	
	
	//my_memset(pbuf,0,0x0c);
	ptgl = tgl;//全局保存
	if(0x00 == tgl)
	{
		toggle ^= 0x01;
	}
	
	
#if 1
	pbuf[0] = 0x0c;	
	pbuf[1] = 0x80 | toggle;// | BIT(2) | BIT(3);;
#else
    volatile u32 ptime,pstsync,softcnt;
	ptime = ax32xx_timerTickCount();
	if(0x00 == tgl)
		pstsync = ptime;
	softcnt = get_u32softcnt();
	pbuf[0] = 0x0c;	
	pbuf[1] = 0x80 | toggle | BIT(2) | BIT(3);;
	
	pbuf[2] = (pstsync >> 0) & 0xff;
	pbuf[3] = (pstsync >> 8) & 0xff;
	pbuf[4] = (pstsync >> 16) & 0xff;
	pbuf[5] = (pstsync >> 24) & 0xff;
	
	pbuf[6] = (ptime >> 0) & 0xff;
	pbuf[7] = (ptime >> 8) & 0xff;
	pbuf[8] = (ptime >> 16) & 0xff;
	pbuf[9] = (ptime >> 24) & 0xff;

	pbuf[10] = (softcnt >> 0) & 0xff;
	pbuf[11] = (softcnt >> 8) & 0xff;
#endif 
}
void uvc_header_fill(u8 toggle, u32 pts, u8 end, u8 *pbuf)
{
	u32 ptime = 0x01c9c380;//0x80, 0xC3, 0xC9, 0x01
	u16 softcnt = get_u32softcnt();
#if 0
	pbuf[0] = 0x0c;	
	pbuf[1] = 0x80 | (toggle & 0x01);// | BIT(2) | BIT(3);;
#else
	pbuf[0] = 0x0c;	
	pbuf[1] = /*0x80 | */ BIT(2) | BIT(3) | (end << 1) | (toggle & 0x01);
	
	pbuf[2] = (pts >> 0) & 0xff;
	pbuf[3] = (pts >> 8) & 0xff;
	pbuf[4] = (pts >> 16) & 0xff;
	pbuf[5] = (pts >> 24) & 0xff;
	
	pbuf[6] = (ptime >> 0) & 0xff;
	pbuf[7] = (ptime >> 8) & 0xff;
	pbuf[8] = (ptime >> 16) & 0xff;
	pbuf[9] = (ptime >> 24) & 0xff;

	pbuf[10] = (softcnt >> 0) & 0xff;
	pbuf[11] = (softcnt >> 8) & 0xff;
#endif 
}
			
u32 uvc_get_tgl(void)
{
	return 	ptgl;
}
