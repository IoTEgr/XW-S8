
#ifndef __USB_UAC_H
#define __USB_UAC_H
#define 	__PCM_SAMPL__	32000	
#define 	UAC_EPX			3
#define 	__UAC_STRM_EP__		0x83
#define		UAC_FIFO		EP3_FIFO
#define 	UAC_PKG_SZ		32 * 2 //32k 

#define UAC_FEATURE_UNIT		0x05//0x02 Ray fix

#define UAC_STREAMING_ENDPOINT	0x83

#define UAC_SET_REQ				0x21
#define UAC_SET_ENDPOINT		0x22
#define UAC_GET_REQ				0xA1
#define UAC_GET_ENDPOINT		0xA2

#define UAC_REQUEST_CODE_UNDEFINED 	0x00
#define UAC_SET_CUR  				0x01
#define UAC_GET_CUR  				0x81
#define UAC_SET_MIN  				0x02
#define UAC_GET_MIN  				0x82
#define UAC_SET_MAX  				0x03
#define UAC_GET_MAX  				0x83
#define UAC_SET_RES  				0x04
#define UAC_GET_RES  				0x84
#define UAC_SET_MEM  				0x05
#define UAC_GET_MEM  				0x85
#define UAC_GET_STAT 				0xFF

#define UAC_FU_CONTROL_UNDEFINED 			0x00
#define UAC_FU_MUTE_CONTROL 				0x01
#define UAC_FU_VOLUME_CONTROL 				0x02
#define UAC_FU_BASS_CONTROL 				0x03
#define UAC_FU_MID_CONTROL 					0x04
#define UAC_FU_TREBLE_CONTROL 				0x05
#define UAC_FU_GRAPHIC_EQUALIZER_CONTROL 	0x06
#define UAC_FU_AUTOMATIC_GAIN_CONTROL 		0x07
#define UAC_FU_DELAY_CONTROL 				0x08
#define UAC_FU_BASS_BOOST_CONTROL 			0x09
#define UAC_FU_LOUDNESS_CONTROL 			0x0A

#define UAC_EP_CONTROL_UNDEFINED 		0x00
#define UAC_EP_SAMPLING_FREQ_CONTROL 	0x01
#define UAC_EP_PITCH_CONTROL 			0x02


#define	USB_REQUEST_ERROR		0
#define	USB_DATAIN_STAGE		1
#define	USB_STATUS_STAGE		1
#define	USB_REQUEST_OVER		1

#define UacReqSelector			((unsigned char)(ep0rqu.wValue >> 8))
#define UacReqInterface			((unsigned char)(ep0rqu.wIndex))
#define UacReqTermUnitId		((unsigned char)(ep0rqu.wIndex >> 8))
#define UacReqLen				((unsigned char)(ep0rqu.wLength))
#define	UacEndPointNumber		((unsigned char)(ep0rqu.wIndex))

#define UACBUFF 10
struct _UAC
{
	u8 buff_num;			/*buffer ×ÜÊý*/
	u8 ready;				/*×¼±¸ºÃµÄbuffÊýÄ¿*/
	u8 pADCW;				/*ADCÕýÔÚÊ¹ÓÃµÄBUFFºÅ*/
	u8 pUSBR;				/*USBÕýÔÚÊ¹ÓÃµÄBUFFºÅ*/
	u32 agcflg;				/*ADC half\fullÖÐ¶Ï±êÖ¾Î»£¬Ö»Ê¹ÓÃµ¥BUFFÊ±Ê¹ÓÃ*/
	u32 addr_lst[UACBUFF];	/*µØÖ·ÁÐ±í*/
};
typedef bool TypeUsbStage;

#if 1
typedef unsigned char (*SET_REQ_CALLBACK_TYPE)(unsigned char);

extern u8 const audioSample[64];
extern volatile bool EP0WaitForSetReqData;
//extern SET_REQ_CALLBACK_TYPE pSetReqCallback;
#endif

/*
typedef enum {
	ADC_48K,
	ADC_44_1K,
	ADC_32K,
	ADC_24K,
	ADC_22_05K,
	ADC_16K,
	ADC_12K,
	ADC_11_025K,
	ADC_8K,
} ADC_SAMPLE_RATE;
 */
//extern void USB_EP3DataIn(u8 *Pbuf,u16 Length);
TypeUsbStage HandleUacFeatUnitReq(unsigned char reqType, unsigned char request, unsigned char reqSelector, unsigned char len);
TypeUsbStage UacHandleRequestToStreamingEndpoint(unsigned char reqType, unsigned char request, unsigned char reqSelector, unsigned char len);
void uac_var_init(void);
void dealforep3isr(void);
void USB_start_adc(void);
u32 Get_ep3_auido_adc(void);
void Ep3_audio_adc_init(void);
void UAC_buff_init(struct _UAC *pUAC,u8 buffnum,u8 * start,u8 buffsize);
//void UAC_adc_init(u8 * auadc_buff_addr,u32 buff_size,ADC_SAMPLE_RATE sample);
void UAC_half_hook(struct _UAC *pUAC);
//static void UAC_full_hook(struct _UAC *pUAC);
//static void UAC_half_hook1(void);
//static void UAC_full_hook1(void);
//static void UAC_half_hook2(void);
//static void UAC_full_hook2(void);
void UAC_adc_isr(void);
void UAC_adc_Stop(void);


extern const u8 UAC_CONFIG_DESC_DATA[];
bool uac_unit_ctl_hal(u8 val, u8 rqu, u8 len);
//extern bool (*fp_uac_unit_ctl_hal)(u8 val, u8 rqu, u8 len);	
//extern bool (*fp_UacHandleToStreaming)(u8 val, u8 rqu, u8 len);
void uac_epx_cfg(void);
bool UacHandleToStreaming(u8 val, u8 rqu, u8 len);

void uac_start(void);
void uac_stop(void);
void uac_isr_hal(void);

extern bool (*fp_uac_unit_ctl_hal)(u8 val, u8 rqu, u8 len);
extern bool (*fp_UacHandleToStreaming)(u8 val, u8 rqu, u8 len);

void uac_epx_cfg(void);

void uvc_on(void);
void uvc_off(void);
extern volatile u8 uaconflg;
#endif /* DAC_H_ */

