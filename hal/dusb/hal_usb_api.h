
#ifndef __USB_API_H__
#define __USB_API_H__


//UVC定义了5种分辨率
//1280,720
#define SOL1		1
#define SOL1_W		1280
#define SOL1_H		720
//800,600
#define SOL2		2
#define SOL2_W		1280
#define SOL2_H		960
//640,480
#define SOL3		3
#define SOL3_W		640
#define SOL3_H		480
//720,480
#define SOL4		4
#define SOL4_W		640
#define SOL4_H		360

//320,240
#define SOL5		5
#define SOL5_W		320
#define SOL5_H		240

#define DFLT_SOL	SOL3
#define MAX_SOL		5

//===for vga uvc===
//1280,960
#define SOL1_VGA		1
#define SOL1_VGA_W		640
#define SOL1_VGA_H		480
//640,480
#define SOL2_VGA		2
#define SOL2_VGA_W		640
#define SOL2_VGA_H		480

#define DFLT_SOL_VGA	SOL1_VGA
#define MAX_SOL_VGA		2


#define UVC_VGA  	1		// cmos  senser  type 0: is 720P , 1 : is VGA
//usb dev select 
//you can select USB_MSC,USB_UVAC,USB_COMB
//#define USB_MSC			0
//#define USB_UVAC		1
//#define USB_COMB		2
//#define  USB_DEV_TYP	  USB_COMB


//uvc select UVC_MJP,UVC_YUV
#define UVC_MJP			0
#define UVC_YUV			1
#define UVC_MOD			UVC_MJP //不要动此定义，目前只能手动定义数据流类型
void usb_init_typeB(void);
bool usb_iso_kick(u32 adr, u16 len);
void usb_iso_wait_txdown(void);
bool usb_bulk_rx(u8 epx, u32 adr, u16 len);
bool usb_bulk_tx(u8 epx, u32 adr, u16 len);

/*******************************************************************************
* Function Name  : hal_usbdInit
* Description    : hal layer.usb device initial 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void hal_usbdInit(u8 mod);
/*******************************************************************************
* Function Name  : hal_usbdunInit
* Description    : hal layer.usb device uninitial 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void hal_usbdUninit(void);
/*******************************************************************************
* Function Name  : hal_usbdEnable
* Description    : hal layer.usb device enable 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void hal_usbdEnable(u8 en);
/*******************************************************************************
* Function Name  : hal_usbdProcess
* Description    : hal layer.usb device process 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
bool hal_usbdProcess(void);
/*******************************************************************************
* Function Name  : usb_detect
* Description    : hal layer.usb device check for pc conected or not
* Input          :  
* Output         : None
* Return         : 0 : pc connected
                      <0: no pc connected
*******************************************************************************/
s32 hal_usbdCheck(void);

void api_backrec_relinking(void);
bool api_backrec_status(void);
void api_backrec_linking(void);
bool usb_uvciso_wait_txdown(void);
bool api_astern_status(void);
void api_astern_set_status(bool sta);
bool usb_uvciso_kick(u32 adr, u16 len);

bool usb_uaciso_kick(u32 adr, u16 len);

bool usb_uaciso_wait_txdown(void);
#endif