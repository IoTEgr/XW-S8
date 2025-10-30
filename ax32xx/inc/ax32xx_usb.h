/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         USB DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_usb.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN USB HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_USB_H
    #define AX32XX_USB_H




/*Common USB registers */
#define		FAddr			0x00	/* Function address register. */
#define		Power			0x01	/* Power management register. */
#define		IntrTx1			0x02	/* Interrupt register for Endpoint 0 plus Tx Endpoint0 1 to 7. */
#define		IntrTx2			0x03	/* Interrupt register for Endpoint 8 to 15. */
#define 	IntrRx1			0x04	/* Interrupt register for Rx Endpoint 1 to 7. */
#define		IntrRx2			0x05	/* Interrupt register for Rx Endpoint 8 to 15. */
#define		IntrUSB			0x06	/* Interrupt register for common USB interrupts. */
#define		IntrTx1E		0x07	/* Interrupt enable register for IntrTx1. */
#define		IntrTx2E		0x08	/* Interrupt enable register for IntrTx2. */
#define		IntrRx1E		0x09	/* Interrupt enable register for IntrRx1. */
#define		IntrRx2E		0x0A	/* Interrupt enable register for IntrRx2. */
#define		IntrUSBE		0x0B	/* Interrupt enable register for IntrUSB. */
#define		Frame1			0x0C	/* Frame number bits 0 to 7. */
#define		Frame2			0x0D	/* Frame number bits 8 to 15. */
#define		Index			0x0E	/* Index register for selecting the endpoint status and control register. */
#define		DevCtl			0x0F	/* USB device control register. */

#define		TxMaxP			0x10	/* Maximum packet size for host OUT endpoint or peripheral IN endpoint. */
#define		CSR0			0x11	/* Control Status register for Endpoint0. */
#define		TxCSR1			0x11	/* Control Status register 1 for host OUT endpoint or peripheral IN endpoint. */
#define		TxCSR2			0x12	/* Control Status register 2 for host OUT endpoint or peripheral IN endpoint. */
#define		RxMaxP			0x13	/* Maximum packet size for host IN endpoint or peripheral OUT endpoint. */
#define		RxCSR1			0x14	/* Control Status register 1 for host IN endpoint or peripheral OUT endpoint. */
#define		RxCSR2			0x15	/* Control Status register 2 for host IN endpoint or peripheral OUT endpoint. */
#define		Count0			0x16	/* Number of received bytes in Endpoint 0 FIFO. */
#define		RxCount1		0x16	/* Number of bytes in host IN endpoint FIFO or peripheral OUT FIFO(lower byte). */
#define		RxCount2		0x17	/* Number of bytes in host IN endpoint FIFO or peripheral OUT FIFO(upper byte). */
#define		TxType			0x18	/* Sets the transation protocol and peripheral endpoint number for the host OUT endpoint. */
#define		TxInterval		0x19	/* Sets the polling interval for an OUT Interrupt endpoint, in ms. */
#define		RxType			0x1A	/* Sets the transation protocol and peripheral endpoint number for the host IN endpoint. */
#define		RxInterval		0x1B	/* Sets the polling interval for an IN Interrupt endpoint, in ms. */

#define		FIFO0			0x20	/* FIFO for endpoint 0 */
#define		FIFO1			0x21	/* FIFO for endpoint 1 */
#define		FIFO2			0x22	/* FIFO for endpoint 2 */
#define		FIFO3			0x23	/* FIFO for endpoint 3 */
#define		FIFO4			0x24	/* FIFO for endpoint 4 */
#define		FIFO5			0x25	/* FIFO for endpoint 5 */
#define		FIFO6			0x26	/* FIFO for endpoint 6 */
#define		FIFO7			0x27	/* FIFO for endpoint 7 */
#define		FIFO8			0x28	/* FIFO for endpoint 8 */
#define		FIFO9			0x29	/* FIFO for endpoint 9 */
#define		FIFO10			0x2A	/* FIFO for endpoint A */
#define		FIFO11			0x2B	/* FIFO for endpoint B */
#define		FIFO12			0x2C	/* FIFO for endpoint C */
#define		FIFO13			0x2D	/* FIFO for endpoint D */
#define		FIFO14			0x2E	/* FIFO for endpoint E */
#define		FIFO15			0x2F	/* FIFO for endpoint F */

//----------------------------------------------------------------------------------------------------




#define USB_TIMEOUT 	80000

#define MAX_EPX			5
#define EP0				0
#define EP1     		1
#define EP2				2
#define EP3				3
#define EP4				4

#define EP0_FIFO		64
#define EP1_FIFO    	1024
#define EP2_FIFO    	1024
#define EP3_FIFO    	1024
#define EP4_FIFO    	4096


//USB20_Power
#define USB_SUSPENDM_EN		(1<<0)
#define USB_SUSPEND_MODE	(1<<1)//in host send suspend signal to target or be a device to into suspend mode
#define USB_RESUME_MODE		(1<<2)//the same to suspend mode
#define USB_RESET_MODE		(1<<3)//read/write in host but read only in peripheral
#define USB_HS_MODE			(1<<4)
#define USB_HS_ENAB			(1<<5)//1:HIGH speed 0:full speed
#define USB_SOFT_CONN		(1<<6)
#define USB_ISO_UPDATE		(1<<7)



//USB20_IntrUSB (read only)
//USB20_IntrUSBE¶ÔÓ¦ÉèÖÃbit
#define USB_SUSPEND			(1<<0)
#define USB_RESUME			(1<<1)
#define USB_RESET			(1<<2)
#define USB_BABBLE			(1<<2)
#define USB_SOF				(1<<3)
#define USB_CONN			(1<<4)//device connection is detected,only valid in host mode
#define USB_DISCON			(1<<5)
#define USB_SESS_REQ		(1<<6)
#define USB_VBUS_ERR		(1<<7)


#define USB_INEP_INT(n)		(1<<n)
#define USB_OUTEP_INT(n)	(1<<n)//0 not include

/**
* FIFO 属性操作类
*/
typedef struct
{
	u16 size;
	u8 *fifo;
	void (*ptx)(u8 epx, u32 adr, u16 len);
	void (*prx)(u8 epx, u32 adr, u16 len);
	
}FIFO_CFG;

extern const FIFO_CFG g_fifo_inf[MAX_EPX];


typedef enum
{
	USB_IRQ_RESET=0,
	USB_IRQ_EP0,

	USB_IRQ_MAX
}USB_IRQ_E;

/*
enum
{
	__DISK__ = 0,
	__DISK_UVC__ = 1,
	__DISK_UAC__ = 1,
	__COMB__ = 1,
};
*/
extern void (*fp_enum_epx_cfg)(u8 devtyp); 
extern void (*fp_msc_epx_cfg)(void);
extern void (*fp_uac_epx_cfg)(void);
extern void (*fp_uvc_epx_cfg)(void);
extern void (*fp_usb_reset_hal)(void);
extern void (*fp_usb_ep0_hal)(void);

bool usb_iso_tx(u8 epx, u32 adr, u16 len);
void usb_burst_reset(void);

extern u8 (*fp_host_connect_hal)(void);
extern u8 (*fp_host_remove_hal)(void);
void ax32xx_usb_hostIRQHanlder(void);

bool _usb_bulk_tx(u8 epx, u32 adr, u16 len,u16 blkszie);
bool _usb_bulk_rx(u8 epx, u32 adr, u16 len,u16 blkszie);
bool _usb_iso_kick(u8 epx, u32 adr, u16 len);
void usb_phyreset_as_typeb(void);
bool _iso_wait_txdown(u8 epx);
u32 get_u32softcnt(void);
/*******************************************************************************
* Function Name  : ax32xx_usbIRQHanlder
* Description    : USB irq handler
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_usbIRQHanlder(void);
/*******************************************************************************
* Function Name  : ax32xx_usbCallbackRegister
* Description    : USB CALLBACK REGISTER
* Input          : u8 type : USB_IRQ_E
                      void (*callback)(void) : callback
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_usbCallbackRegister(u8 type,void (*callback)(void));
/*******************************************************************************
* Function Name  : usb_phyreset_as_typea
* Description    : usb type set
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/		
void usb_phyreset_as_typea(void);
void usb_host_reset(void);
/*******************************************************************************
* Function Name  : ax32xx_usbPhyPllEnable
* Description    : enable usb phy pll(96M/120M/240M/480M)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_usbPhyPllEnable(void);

extern u8 (*fp_epout_isr_hal)(void);

extern const u32 ExTADR[6];
extern const u32 ExRADR[6];
#define __U20ExTADR__(x)  *((volatile __sfr u32 *)(ExTADR[x]))
#define __U20ExRADR__(x)  *((volatile __sfr u32 *)(ExRADR[x]))

#define 	__MSCTADR__		USB20E1TADR
#define 	__MSCRADR__		USB20E1RADR
//u8 (*fp11_epout_isr_hal)(void);
extern u8 (*fp_host11_connect_hal)(void);
extern u8 (*fp_host11_remove_hal)(void);
/*******************************************************************************
* Function Name  : usb11_registers_write
* Description    : usb11_registers_write
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//usb1.1 SIE registers write
void usb11_registers_write(u8 addr, u8 reg_data);
/*******************************************************************************
* Function Name  : usb11_registers_read
* Description    : usb11_registers_read
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//usb1.1 SIE registers write
u8 usb11_registers_read(u8 addr);
/*******************************************************************************
* Function Name  : usb11_host_reset
* Description    : usb11_host_reset
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb11_host_reset(void);
/*******************************************************************************
* Function Name  : usb_memcpy
* Description    : usb_memcpy
* Input          : dir:1:gram->usb11_ram, 0: usb11_ram->gram;
* Output         : None
* Return         : None
*******************************************************************************/
void usb_memcpy(uint8_t dir,uint32_t dst,uint32_t src,uint32_t len);
/*******************************************************************************
* Function Name  : usb_phyreset_as_typea11
* Description    : usb_phyreset_as_typea11
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_phyreset_as_typea11(void);
void ax32xx_usb11_hostIRQHanlder(void);
s32 usb20_host_check(void);
s32 usb11_host_check(void);
void usb20_phyreset_as_filter(void);
#endif /* USB_H_ */
