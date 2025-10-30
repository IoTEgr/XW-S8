
#ifndef USB_DEVICE_VIDEO_H_
#define USB_DEVICE_VIDEO_H_


#define UVC_VS_CONTROL_UNDEFINED 				0x00
#define UVC_VS_PROBE_CONTROL 					0x01
#define UVC_VS_COMMIT_CONTROL 					0x02
#define UVC_VS_STILL_PROBE_CONTROL 				0x03
#define UVC_VS_STILL_COMMIT_CONTROL 			0x04
#define UVC_VS_STILL_IMAGE_TRIGGER_CONTROL 		0x05
#define UVC_VS_STREAM_ERROR_CODE_CONTROL 		0x06
#define UVC_VS_GENERATE_KEY_FRAME_CONTROL 		0x07
#define UVC_VS_UPDATE_FRAME_SEGMENT_CONTROL 	0x08
#define UVC_VS_SYNCH_DELAY_CONTROL 				0x09

#define PU_CONTROL_UNDEFINED					0x00
#define PU_BACKLIGHT_COMPENSATION_CONTROL		0x01
#define PU_BRIGHTNESS_CONTROL					0x02
#define PU_CONTRAST_CONTROL						0x03
#define PU_GAIN_CONTROL							0x04
#define PU_POWER_LINE_FREQUENCY_CONTROL			0x05
#define PU_HUE_CONTROL							0x06
#define PU_SATURATION_CONTROL					0x07
#define PU_SHARPNESS_CONTROL					0x08
#define PU_GAMMA_CONTROL						0x09
#define PU_WHITE_BALANCE_TEMPERATURE_CONTROL	0x0A
#define PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL 0x0B

#define PROCESSING_UINT_CONTROL					1
#define PROCESSING_UINT_ID						0x02	//if configuration desperator change processing id ,this should change
#define VEDIO_CONTROL_INTERFACE_ID				0x00


#define UVC_VIDEO_PROBE_COMMIT_FRAME_INDEX_OFFSET		3
#define UVC_VIDEO_PROBE_COMMIT_FRAME_INTERVAL_OFFSET	4


#define UVC_SET_REQ				0x21
#define UVC_GET_REQ				0xA1
#define UVC_SET_CUR				0x01
#define UVC_GET_CUR				0x81
#define UVC_GET_MIN				0x82
#define UVC_GET_MAX				0x83
#define UVC_GET_RES				0x84
#define UVC_GET_LEN				0x85
#define UVC_GET_INFO			0x86
#define UVC_GET_DEF				0x87



//#define EP2_BUF_SIZE		(3072-20)        // occupies 3072 bytes (HS only)
//#define EP2_BUF_SIZE		(256)   //2988     // occupies 3072 bytes (HS only)



#define MAX_FRAME_INDEX				5
#define UVC_PAYLOAD_HEADER_SIZE		12


typedef unsigned char (*SET_REQ_CALLBACK_TYPE)(unsigned char);
//SET_REQ_CALLBACK_TYPE pSetReqCallback;
//volatile bool EP0WaitForSetReqData;

void uvc_video_probe_control(u8 request,u8 len);
void uvc_video_commit_control(u8 request,u8 len);
void uvc_still_probe_control(u8 request,u8 len);
void uvc_still_commit_control(u8 request,u8 len);

#if (PROCESSING_UINT_CONTROL == 1)
void uvc_pu_power_line_frequency_control(u8 request,u8 len);
void uvc_pu_backlight_compensation_control(u8 request,u8 len);
void uvc_pu_contrast_control(u8 request,u8 len);
void uvc_pu_saturation_control(u8 request,u8 len);
void uvc_pu_sharpness_control(u8 request,u8 len);
void uvc_pu_gamma_control(u8 request,u8 len);
void uvc_pu_brightness_control(u8 request,u8 len);
void uvc_pu_hue_control(u8 request,u8 len);
#endif

void uvc_header_fill(u8 toggle, u32 pts, u8 end, u8 *pbuf);
void uvc_var_init(void);
void uvc_stop(void);
void uvc_start(void);
void uvc_init(void);
u8 uvc_still_trigger_control(unsigned char request, unsigned char len);
void CsiHandleLastPacket(void);
void CsiBuffer0FullIntrHandler(void);
void CsiBuffer1FullIntrHandler(void);
void CsiEndOfFrameIntrHandler(void);

void uvc_header(u32 tgl, u8 *pbuf);
void csi_SetCutSize(u16 x_sta,u16 x_end,u16 y_sta,u16 y_end);


void uvc_fifoinf_init(void);
//extern bool (*fp_usb_ep0_tx)(u8* txbuf, u8 len);
//extern u8 volatile uvc_on;	


bool uvc_unit_ctl_ex_hal(u8 val, u8 rqu, u8 len);
bool uvc_unit_ctl_hal(u8 val, u8 rqu, u8 len);
bool uvc_probe_ctl_hal(u8 val, u8 rqu, u8 len);
void uvc_epx_cfg(void);	

void uvc_epx_cfg(void);
void uvc_isr_hal(void);

extern const u8 UVC_CONFIG_DESC_DATA[];
BOOL uvc_pkg_tx(u8 pkgtoggle, u8* addr, u16 len);
BOOL  uvc_pkg_kick(void);
void uvc_tx_hal(void);

#if UVC_MOD	 == UVC_MJP
	#define 	__UVC_CTL_EP__		0x82
	#define 	__UVC_EPX__			4
	#define 	__UVC_STRM_EP__		0x84
	#define 	__UVC_PKG_SZ__		2048//(2028)//(996*2)//EP2_BUF_SIZE	 (x*3-12)/4, *3
	#define 	__UVC_MIF_SZ__		(2028)//(996)//EP2_BUF_SIZE	
#else
	#define 	__UVC_CTL_EP__		0x82
	#define 	__UVC_EPX__			4
	#define 	__UVC_STRM_EP__		0x84
	#define 	__UVC_PKG_SZ__		(2560+12)//(2028)//(996*2)//EP2_BUF_SIZE	 (x*3-12)/4, *3
	#define 	__UVC_MIF_SZ__		(1024)//(996)//EP2_BUF_SIZE	

#endif
/**
 iso:36
iso:60
iso:84
iso:108
iso:132
iso:156
iso:180
iso:204
iso:228
iso:252
iso:276
iso:300
iso:324
iso:348
iso:372
iso:396
iso:420
iso:444
iso:468
iso:492
iso:516
iso:540
iso:564
iso:588
iso:612
iso:636
iso:660
iso:684
iso:708
iso:732
iso:756
iso:780
iso:804
iso:828
iso:852
iso:876
iso:900
iso:924
iso:948
iso:972
iso:996
iso:1020
iso:1044
iso:1068
iso:1092
iso:1116
iso:1140
iso:1164
iso:1188
iso:1212
iso:1236
iso:1260
iso:1284
iso:1308
iso:1332
iso:1356
iso:1380
iso:1404
iso:1428
iso:1452
iso:1476
iso:1500
iso:1524
iso:1548
iso:1572
iso:1596
iso:1620
iso:1644
iso:1668
iso:1692
iso:1716
iso:1740
iso:1764
iso:1788
iso:1812
iso:1836
iso:1860
iso:1884
iso:1908
iso:1932
iso:1956
iso:1980
iso:2004
iso:2028

*/
extern bool (*fp_uvc_unit_ctl_hal)(u8 val, u8 rqu, u8 len);
extern bool (*fp_uvc_probe_ctl_hal)(u8 val, u8 rqu, u8 len);
extern bool (*fp_uvc_unit_ctl_ex_hal)(u8 val, u8 rqu, u8 len);
extern bool (*fp_usb_ep0_tx)(u8* txbuf, u8 len);
extern bool (*fp_usb_iso_tx)(u8 epx, u32 adr, u16 len);

/*
#define SOL1		1
#define SOL1_W		1280
#define SOL1_H		720

#define SOL2		2
#define SOL2_W		800
#define SOL2_H		600

#define SOL3		3
#define SOL3_W		640
#define SOL3_H		480

#define SOL4		4
#define SOL4_W		720
#define SOL4_H		640

#define SOL5		5
#define SOL5_W		320
#define SOL5_H		240

#define DFLT_SOL	SOL1
*/
typedef struct
{
	u32 win_w;	
	u32 win_h;
}PROBE;



#endif
