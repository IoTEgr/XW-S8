/*
 * usb_host_enum.h
 *
 *  Created on: 2013Äê10ÔÂ24ÈÕ
 *      Author: jingcunxu
 */

#ifndef USB_HOST_ENUM_H_
#define USB_HOST_ENUM_H_

#define 	__DEV_ADDR__	4
// USB specific request
typedef struct
{
    u8  mrequest;
    u8  request;
    u16 value;
    u16 index;
    u16 length;
} SETUP_PKG, *P_SETUP_PKG;
typedef struct
{
	u8 addr;
	u8 pidphase;
	u8 t_nkg;
	u8 payload;
}SETUP_TRAN;

typedef struct
{
	u8 addr;
    u8 pid;
    u8 payload;
    u16 len;
    u8 *buffer;	// usb transfer buffer must be
    u8 epbulkin;
    u8 epbulkout;
} XXGPKG, *P_XXGPKG;

typedef struct
{
    u8 max_lun;
    u8 cbw_lun;
//  u8 device_online;
    u8 usb_enum_sta;
//  u8 usb_dev_addr;
//  u8 config_index;
//  u8 usb_connect;
//  u8 pkt_per_sec;		// Ã¿¸öÊµ¼ÊSecotrµÄ Óë512µÄ±¶Êý¹ØÏµ
}USB_VA;

typedef union
{
    SETUP_PKG setup;
    u8 set_pkt[8];
} SETUPPKT;

//±ê×¼ÇëÇóÀàÐÍ
#define GET_STATUS      0x00
#define CLEAR_FEATURE   0x01
#define SET_FEATURE     0x03
#define SET_ADDRESS     0x05
#define GET_DESCRIPTOR  0x06
#define SET_DESCRIPTOR  0x07
#define GET_CONFIG      0x08
#define SET_CONFIG      0x09
#define GET_INTERFACE   0x0a
#define SET_INTERFACE   0x0b
#define SYNCH_FRAME     0x0c

//¸ßByte´ú±íÃèÊö·ûÀàÐÍ¼°±àºÅ¡¢µÍByte´ú±íË÷ÒýºÅ
#define DEVICE          0x0100
#define CONFIGURATION   0x0200
#define STRING          0x0300
#define INTERFACE       0x0400
#define ENDPOINT        0x0500


#define PID_SETUP       0xD0
#define PID_IN          0x90
#define PID_OUT         0x10

#define EP0				0
#define EP1				1
#define EP2				2
#define EP3				3


#define MASS_STORAGE_CLASS    0x08
#define UVC_CLASS    0x0e
#define UAC_CLASS    0x01

extern SETUPPKT   setup_pkt;
extern XXGPKG     usbstack;
extern USB_VA   usb_var;

extern u8 usb_dev_addr;
extern u8 bs_buf[64];

void delay_5ms(u32 n);
u8 enum_usb_dev();


typedef struct {
	u8 w_cmd,r_cmd,id,addr_num,data_num;
	u16 id_reg;//id 寄存器地址
	u16 const (*init)[2];
	u8 mclk;
} sensor_cmd_t;
extern const sensor_cmd_t * sensor_cmd_tab[];
typedef struct
{
	u8 *code;
	u32 clen;
	u32 inram;
	
	u32 (*pfun)[2];
	u32 npfun;
	
	u32 iensor;
	sensor_cmd_t const*psensor;
	
}EX_MAND;
extern EX_MAND exmand;
extern EX_MAND uisp;

u32 usb_enum_phase_get(void);
void usb_stack_init(void);
void husb_kick_ep0isr(void);
void husb_ep0isr(void);
void uhost_backsta_cmd(void);
bool check_backsta(void);
void usb_enum_phase_clr(void);
#endif /* USB_HOST_ENUM_H_ */
