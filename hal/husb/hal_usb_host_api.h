/*
 * usb_host.h
 *
 *  Created on: 2013?ê10??24è?
 *      Author: jingcunxu
 */
#ifndef USB_HOST_H_
#define USB_HOST_H_

#include "hal_usb_host_tpbulk.h"
#include "hal_usb_host_uvc.h"



#define MAX_EP 	2
#define MAX_INTFS 	2
//volatile u8 usb_host_type; 
typedef struct
{
    u16  vid, pid;       // Vendor ID and Product ID
    u8  bclass;
    u8  num_of_eps;      // actual number endpoint from slave
    u8  id_mft;          // Manufacturer ID
    u8  id_pdt;          // Product ID
    u8  id1;
    u8  id2;
    u8  ep_addr[MAX_EP];   // bool 7 = 1 = use PID_IN,
    u8  attr[MAX_EP];     // ISO | Bulk | Interrupt | Control
    u16 pay_load[MAX_EP];  // ISO range: 1-1023, Bulk: 1-64, etc
    u16 interval[MAX_EP]; // polling interval (for LS)
    u8  data1[MAX_EP];    // DataToggle value
} USBDEV, *PUSBDEV;


// Standard Device Descriptor
typedef struct
{
    u8  length;
    u8  descriptor_type;
    u16 bcd_usb;
    u8  device_class;
    u8  device_subclass;
    u8  device_protocol;
    u8  maxpacket_size0;
    u16 id_vendor;
    u16 id_product;
    u16 bcd_device;
    u8  manufacturer;
    u8  product;
    u8  serial_number;
    u8  num_configurations;
} SDEV_DESC, *P_DEV_DESC;


// Standard Configuration Descriptor
typedef struct
{
    u8  length;                // Size of descriptor in u8
    u8  type;                  // Configuration
    u16 t_length;              // Total length
    u8  num_intf;              // Number of interface
    u8  cv;                    // bConfigurationValue
    u8  index;                 // iConfiguration
    u8  attr;                  // Configuration Characteristic
    u8  max_power;             // Power config
} SCFG_DESC, *P_SCFG_DESC;



// Standard Interface Descriptor
typedef struct
{
    u8 length;
    u8 type;
    u8 num;
    u8 alt_tring;
    u8 end_points;
    u8 iclass;
    u8 sub;
    u8 proto;
    u8 index;
} SINTF_DESC, *P_INTF_DESC;


// Standard EndPoint Descriptor
typedef struct
{
    u8  length;
    u8  type;
    u8  ep_addr;
    u8  attr;
    u16 pay_load;               // low-speed this must be 0x08
    u8  interval;
} SEP_DESC, *P_EP_DESC;


extern USBDEV  		udev;
extern P_DEV_DESC  	p_dev;//éè±??èê?·?
extern P_SCFG_DESC 	p_cfg;//?????èê?·?
extern P_INTF_DESC 	p_ifc;//?ó?ú?èê?·?
extern P_EP_DESC  		p_enp;//??μ??èê?·?


#define FULL_SPEED		1
#define HIGH_SPEED		0

extern u8 usb_type;


void usb_detect_init();
u8 usb_read(u8 *rbuf,u32 lba,u32 offset);
u8 usb_write(u8 *wbuf,u32 lba,u32 offset);
u8 usb_host_enum(void);
void usb_irq_isr(u32 usb_device_task);
void hal_husb_init_typeA(void);
extern u8 (*fp_host_connect_hal)(void);
extern u8 (*fp_host_remove_hal)(void);
extern u8 usb_host_enum(void);
u8 usb_host_remove(void);



typedef struct
{
	DISKHAL *pudisk;
	UVCHAL *puvc;
}UDEV;
//UDEV u_dev;
extern UDEV u_dev;

extern volatile u8 dcerflg;
extern volatile u8 dcdwnflg;
void backrec_sensor(bool stat);
bool backrec_sensor_stat(void);
bool backrec_lowerpower(void);
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern u8 (*fp_host11_connect_hal)(void);
extern u8 (*fp_host11_remove_hal)(void);
void usb11_host_fun_reg(void);
void hal_husb11_init_typeA(void);
u8 usb11_host_enum(void);
u8 usb11_host_remove(void);
u8 usb_host_detect(void);
void usb_host_type_set(u8 type);
void usensor_actech(void);
void uvc_host_dev_detech(void);
void backrec_adpt_set(bool stat);
#endif /* USB_HOST_H_ */

