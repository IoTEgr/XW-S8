/*
 * usb_device_enum.h
 *
 *  Created on: 2013Äê11ÔÂ7ÈÕ
 *      Author: jingcunxu
 */

#ifndef USB_DEVICE_ENUM_H_
#define USB_DEVICE_ENUM_H_

#define		VID			0x1902
#define		PID			0x3333//0x3251     //PID  MS+UVC:0x3231; MS:0x3241; UVC:0x3251


#define		EP0_IDLE_STATE				0
#define		EP0_RX_STATE			1
#define		EP0_TX_STATE			2

//String Index
#define ID_LANGUAGE                     0x00
#define ID_MANUFACTURE                  0x01   	
#define ID_PRODUCT                      0x02	
#define ID_SERIAL			            0x03

#define USB_UAC_SUPPORT			1		//audio support
#define USB_UDISK_SUPPORT		1		//usb cardreader support
#define USB_UVC_SUPPORT			1		//vedio support

#define 	EP0_BUF_SIZE			64          // occupies 64 bytes

//Setup Command Const
#define SC_GET_STATUS			        0x00	//; Setup command: Get Status
#define SC_CLEAR_FEATURE		        0x01	//; Setup command: Clear Feature
#define SC_RESERVED			            0x02	//; Setup command: Reserved
#define SC_SET_FEATURE			        0x03	//; Setup command: Set Feature
#define SC_SET_ADDRESS			        0x05	//; Setup command: Set Address
#define SC_GET_DESCRIPTOR		        0x06	//; Setup command: Get Descriptor
#define SC_SET_DESCRIPTOR		        0x07	//; Setup command: Set Descriptor
#define SC_GET_CONFIGURATION		    0x08	//; Setup command: Get Configuration
#define SC_SET_CONFIGURATION		    0x09	//; Setup command: Set Configuration
#define SC_GET_INTERFACE		        0x0a	//; Setup command: Get Interface
#define SC_SET_INTERFACE		        0x0b	//; Setup command: Set Interface
#define SC_SYNC_FRAME			        0x0c	//; Setup command: Sync Frame
#define SC_ANCHOR_LOAD			        0xa0	//; Setup command: Anchor load

#define DEVICE_DESCRIPTOR               0x01	//; Get device descriptor: Device
#define CONFIGURATION_DESCRIPTOR	    0x02	//; Get device descriptor: Configuration
#define STRING_DESCRIPTOR			    0x03	//; Get device descriptor: String
#define	INTERFACE_DESCRIPTOR			0x04
#define	ENDPOINT_DESCRIPTOR				0x05
#define DEVICE_QUALIFIER_DESCRIPTOR     0x06
#define OTHER_SPEED_CONFIG_DESCRIPTOR   0x07



#define UVC_5FPS_LEB0	0x80		// 5 fps, little endian byte 0 (lowest order byte)
#define UVC_5FPS_LEB1	0x84		// 5 fps, little endian byte 1
#define UVC_5FPS_LEB2	0x1E		// 5 fps, little endian byte 2
#define UVC_5FPS_LEB3	0x00		// 5 fps, little endian byte 3 (highest order byte)

#define UVC_15FPS_LEB0	0x2B		// 15 fps, little endian byte 0 (lowest order byte)
#define UVC_15FPS_LEB1	0x2C		// 15 fps, little endian byte 1
#define UVC_15FPS_LEB2	0x0A		// 15 fps, little endian byte 2
#define UVC_15FPS_LEB3	0x00		// 15 fps, little endian byte 3 (highest order byte)

#define UVC_20FPS_LEB0	0x20		// 15 fps, little endian byte 0 (lowest order byte)
#define UVC_20FPS_LEB1	0xA1		// 15 fps, little endian byte 1
#define UVC_20FPS_LEB2	0x07		// 15 fps, little endian byte 2
#define UVC_20FPS_LEB3	0x00		// 15 fps, little endian byte 3 (highest order byte)

#define UVC_30FPS_LEB0	0x15		// 30 fps, little endian byte 0 (lowest order byte)
#define UVC_30FPS_LEB1	0x16		// 30 fps, little endian byte 1
#define UVC_30FPS_LEB2	0x05		// 30 fps, little endian byte 2
#define UVC_30FPS_LEB3	0x00		// 30 fps, little endian byte 3 (highest order byte)

#define AUDIO_PKT_SIZE  64


//USB20_CSR0
#define EP0_TXPKTRDY 			(1<<0) 
#define EP0_RXPKTRDY 			(1<<1) 
#define EP0_SEND_STALL_FLAG 	(1<<2) 
#define EP0_DATA_END 			(1<<3) 
#define EP0_SETUP_END 			(1<<4) 
#define EP0_SEND_STALL 			(1<<5) 
#define EP0_SERVICED_RXPKTRDY 	(1<<6) 
#define EP0_SERVICED_SETUP_END 	(1<<7)






// Startup Command Request Packet
typedef struct
{
    u8 bmRequestType;
    u8 bRequest;
    u16 wValue;
    u16 wIndex;
    u16 wLength;
}EP0_REQ;


// USB Variable
typedef struct
{
	//u32 ep0_buf[EP0_BUF_SIZE];	// to be modified by john
	u8 *pDesc_Data;
	u8 *pEP2_Data;
	u16 yEp0_Desc_Len;
	u8 bSetAddress;
	u8 y8Ep0_State;
	volatile u8 bEPin_stall;
	volatile u8 bEPout_stall;

    //u32  ep2_buf[EP2_BUF_SIZE];// to be modified by john
	u8 *ep2_Data;
    u32 yEp2_Desc_Len;

    u8* ep0_buf;   // EP0 buffer

	//u8* ep1_bufIN_A;    // EP1 IN buffer 0
	//u8* ep1_bufIN_B;    // EP1 IN buffer 1
	//u8* ep1_bufOUT_A;   // EP1 OUT buffer 0
	//u8* ep1_bufOUT_B;   // EP1 OUT buffer 1

    //u8* ep2_buf_A;   // EP2 ping-pong 1
	//u8* ep2_buf_B;     // EP2 ping-pong 2
	//u8* ep3_buf_A;     // EP3 ping-pong 1
	//u8* ep3_buf_B;     // EP3 ping-pong 2
}USB_VAR_t;




//USB_VAR_t 	usb_varm;
extern u8 ep0_current_state;


extern void usb_ep0_isr();
void usb_device_init(void);
void usb_device_reset_int(void);
void usb_epx_tx_dat(u8 epx,u8 *wbuf,u16 len);
//extern unsigned char volatile usb_dma_occupy;



#define EP1_BUF_SIZE		64
#define MAX_EP_D                3      //最大的EP号
//EP0状态机
enum {
    IDLE_STATE,
    TRANSER_OVER_STATE,
    TRANSFERING_STATE,
    RECEIVING_STATE
};
//接收者
enum {
    REQUEST_TO_DEVICE,
    REQUEST_TO_INTERFACE,
    REQUEST_TO_ENDPOINT
};
//USB当前状态机
enum {
    USB_DEFAULT_STATE,
    USB_ADDRESS_STATE,
    USB_CONFIG_STATE
};
enum {
    ENDPOINT_STALL,
    REMOTE_WAKEUP,
    TEST_MODE1
};

typedef struct {
	u8 connect;
    u8 type;                        //设备类型
    u8 set_addr;                    //设地址
    u8 cfg_value;                   //Config Value, 用于Set Config与Get Config
    u8 cur_state;                   //USB当前状态机
    u8 ep0_state;                   //EP0的状态机
    u8 ep0_pkt;                     //EP0包大小
    u8 *ep0_ptr;                    //当前发送指针
    u16 ep0_len;                    //当前发送剩余
    u8 uvc_tx_interface;           //ISOC，MIC发送的接口
    u8 uvc_tx_enable;              //ISOC使能
    u8 uvc_tx_enable_int;          //在中断中配置ISOC使能
	
    u8 uac_tx_interface;           //ISOC，MIC发送的接口
    u8 uac_tx_enable;              //ISOC使能
    u8 uac_tx_enable_int;          //在中断中配置ISOC使能
	
	u8 *ep0_buf;

    volatile u8 ep1_tx_stall;       //ep1 tx stall标志
    volatile u8 ep1_rx_stall;       //ep1 rx stall标志
    volatile u8 ep1_pipe_stall;     //ep1 Stall的标志
} USB_DEV;
extern USB_DEV usb_dev;
typedef struct {
	u8 rtype;
	u8 request;
	u16 value;
	u16 index;
	u16 length;
}USB_RQU;

extern USB_RQU usb_rqu;
void ep0_isr_hal(void);
void usb_stall_all(u8 epin, u8 epout);
bool usb_ep0_tx(u8* txbuf, u8 len);
void usb_stall_ep(u8 epnum);
extern bool (*pSetReqCallback)(u8 *rxbuf);
bool usbdev_rqu(void);
void usbdev_reset(void);
#define 	USBDEV_NUM		2

bool uvc_ready(void);
void enum_epx_cfg(u8 devtyp);
void usb_ep0_hal(void);

extern void (*fp_uvc_on_hal)(void);
extern void (*fp_uac_on_hal)(void);
extern void (*fp_uvc_off_hal)(void);
extern void (*fp_uac_off_hal)(void);
#endif /* USB_DEVICE_ENUM_H_ */
