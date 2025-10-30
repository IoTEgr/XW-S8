/*
 * usb_host_tpbulk.h
 *
 *  Created on: 2013Äê10ÔÂ25ÈÕ
 *      Author: jingcunxu
 */

#ifndef USB_HOST_UVC_H_
#define USB_HOST_UVC_H_




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

typedef struct
{
	u8  addr;
    u8 *buffer;	// usb transfer buffer must be
    u16 payload;
    u8 episoin;
    u8 episoout;	
	u8 interval;
}ISOSTACK;

//ISOSTACK isostack;
extern ISOSTACK isostack;


typedef struct
{
	u16 bmhint;
	u8 bFormatIndex;
	u8 bFrameIndex;
	u32 dwFrameInterval;
	u16 wKeyFrameRate;
	u16 wPFrameRate;
	u16 wCompQuality;
	u16 wCompWindowSize;
	u16 wDelay;
	u32 dwMaxVideoFrameSize;
	u32 dwMaxPayloadTSize;
}VIDEO_COMMIT;
//VIDEO_COMMIT commit;
extern bool (*fp_fst_jpg_hal)(void);

typedef struct
{
	u8 fsynflg;
	BOOL (*fp_uvc_receive_jframe)(u8 *buf ,u32 *len);
	BOOL (*fp_iso_receive_dat)(u8 *buf ,u32 *len);
	
}UVCHAL;


extern VIDEO_COMMIT commit;
void cnvr(u8 *p);
typedef struct
{
	u32 jUcnt;
	u8 usta;
	u8 *jbuf;
	u32 jlen;
	u8 utgl;
}UVC_FSTACK;

typedef  struct _BUFQDT	QDT_BUF;
struct _BUFQDT
{
	u8 sta;
	u8  *pbuf;
	u32 buflen;
	u32 id;
	struct _BUFQDT *last; 
	struct _BUFQDT *next; 
	s32 u_tsync;
	s32 u_tsync_next;
};
typedef struct
{
	u8 semf;
	u8 *sf;
	u8 *ef;
	u8 *pcache;
	u32 cachelen;
	u32 cacheuse;
}CACHE;


typedef struct
{
	CACHE c_inf;
	QDT_BUF *p_qdt; 
	QDT_BUF *g_qdt;
	volatile s32 stack_size;
}C_STACK;
typedef struct 
{
	u8 expoint[2048];
	int exfilesize;	
}USENSR_ISP;
extern USENSR_ISP uvcisp;
bool  uvc_host_init(u8 addr, u8 epin, u8 epout, u16 payload, u8 interval);
bool usb_usensor_frame_read(u8 **p, u32 *len, u32 *id);
UVCHAL *uvc_actech_as_usensor(void);
void huvc_dcod_down(u8 err);
void usensor_detach(void);
void backrec_stop(void);
u8 *mjpBcodec_kick(void);
void hal_mjpB_dcdown(void);
u32 api_uvc_fcnt_get(void);
bool hal_mjpB_decoding(void);
void hal_huvc_strm_init(void);
void api_backsta_linking(void);
void api_usensor_1S_pollmonitor(void);
bool api_backrec_status(void);
//............for usb1.1 host ..........
UVCHAL *uvc_actech_as_usensor11(void);
bool  uvc_host11_init(u8 addr, u8 epin, u8 epout, u16 payload, u8 interval);
BOOL iso_receive_dat11(u8 *buf ,u32 *len);
BOOL iso_recive_payload_isr11(u8 *buf ,u32 *len);
void uvc_recevie_jframe_isr11(void);
bool  uvc_host_init(u8 addr, u8 epin, u8 epout, u16 payload, u8 interval);
bool  uvc_host11_init(u8 addr, u8 epin, u8 epout, u16 payload, u8 interval);
UVCHAL *uvc11_actech_as_usensor(void);
#endif /* USB_HOST_TPBULK_H_ */

