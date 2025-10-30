
/******************** (C) COPYRIGHT 2013 BUILDWIN ************************
* File Name          : uart.c
* Author             : pengtao_zhang
* Version            : V0719a
* Date               : 07/19/2013 ;mm/dd/yyyy
* Description        : This file provides all the uart firmware functions.
***************************************************************************/
#include "../inc/hal.h"
//#include "usb_dev_enum.h"
//#include "usb_msc.h"
//#include "usb_uac.h"
//#include "usb_uvc.h"
#define  USBD_UVC_PKGSIZE		(996)

extern s32 usb_device_check(void);


bool usb_uvciso_kick(u32 adr, u16 len)
{
	return _usb_iso_kick(__UVC_EPX__,adr,len);
}
bool usb_uvciso_wait_txdown(void)
{
	return _iso_wait_txdown(__UVC_EPX__);
}
bool usb_uaciso_kick(u32 adr, u16 len)
{
	return _usb_iso_kick(UAC_EPX,adr,len);
}
bool usb_uaciso_wait_txdown(void)
{
	return _iso_wait_txdown(UAC_EPX);
}

bool usb_bulk_tx(u8 epx, u32 adr, u16 len)//
{
	return _usb_bulk_tx(epx,adr,len,__MSC_SIZE__);
}
bool usb_bulk_rx(u8 epx, u32 adr, u16 len)
{
	return _usb_bulk_rx(epx,adr,len,__MSC_SIZE__);
}
void uac_epx_cfg(void);


void hal_usbdReset(void)
{
	
	enum_epx_cfg(0);
	msc_epx_cfg();
	uvc_epx_cfg();
	uac_epx_cfg();
}

void uac_on(void)
{
	hal_auadcStartForUAC(16000,100);
}
void uac_off(void)
{
	hal_auadcStop();
}


void usb_fun_reg(void)
{
    ax32xx_usbCallbackRegister(USB_IRQ_RESET,hal_usbdReset);
	ax32xx_usbCallbackRegister(USB_IRQ_EP0,usb_ep0_hal);
	fp_usb_isr = ax32xx_usbIRQHanlder;
	
	//reg for void usb_burst_reset(void)
//	fp_enum_epx_cfg= enum_epx_cfg;
//	fp_msc_epx_cfg = msc_epx_cfg;
//	fp_uvc_epx_cfg = uvc_epx_cfg;
//	fp_uac_epx_cfg = uac_epx_cfg;
	
	//reg for void usb_isr(void)
	
	
//-------------mass storage---------------------	
	//reg for void msc_epx_cfg(void)
	fp_disk_rd = hal_sdRead;
	fp_disk_wr = hal_sdWrite;
	fp_usb_bulk_tx = usb_bulk_tx;
	fp_usb_bulk_rx = usb_bulk_rx;	
	fp_disk_online = hal_sdExist; 
	fp_disk_cap = hal_sdCapacity;
	fp_disk_free = hal_sdUnlock;
	fp_disk_busy = hal_sdLock;
	fp_disk_stop = hal_sdStop;
//-------------uvc-------------------------------
	//reg for void uvc_epx_cfg(void)
	fp_uvc_on_hal = uvc_on;//mjp_actech_as_jpguvc;
	fp_uvc_off_hal = uvc_off;//mjp_stop;
	fp_uac_on_hal = uac_start;//uac_on;//mjp_actech_as_jpguvc;
	fp_uac_off_hal = uac_stop;//uac_off;//mjp_stop;
}
extern void usb_dev_enum_reg(u8 tpe);

void hal_usbdInit(u8 mod)
{
	usb_dev_enum_reg(mod);
	usb_fun_reg();
	usb_phyreset_as_typeb();
	usb_burst_reset();
	hal_usbdReset();
	ax32xx_intEnable(IRQ_USB20,1);	
}
/*******************************************************************************
* Function Name  : hal_usbdunInit
* Description    : hal layer.usb device uninitial 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void hal_usbdUninit(void)
{
	uac_off();
	uvc_off();
	ax32xx_intEnable(IRQ_USB20,0);
	REG32(USB20CON0)=0;
	/*
	delay_5ms(900);
	memset((u8 *)&usb_dev,0,sizeof(usb_dev));
	memset((u8 *)&usb_rqu,0,sizeof(usb_rqu));
	*/
}
/*******************************************************************************
* Function Name  : hal_usbdEnable
* Description    : hal layer.usb device enable 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
void hal_usbdEnable(u8 en)
{
	ax32xx_intEnable(IRQ_USB20,en);	
}
/*******************************************************************************
* Function Name  : hal_usbdUVCProcess
* Description    : hal layer . process for uvc in mjpeg mode
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_usbdUVCProcess(void)
{
	hal_uac_send();
	static u8 ftgl;
	static u32 PTS;
	
	u32 buffer,len;
	s32 syn_cnt, syn_cnt_next;	
	
	if(!uvc_ready())
	{
		ftgl = 0;
		PTS = -1;
		return ;
	}
	
	if(hal_mjpegRawBufferGet(NULL,&buffer,&len,&syn_cnt,&syn_cnt_next)==NULL)
		return ;

		
		u8 *fbuf = (u8 *)buffer; 
		u32 flen = len;
		//u8 *txbuf ;
		//u32 tlen;

		ax32xx_sysDcacheFlush((u32)fbuf,16);
		if((fbuf[0] != 0xff) || (fbuf[1] != 0xd8))
		{
			debg("u: <header err>\n");	
			hal_mjpegRawBufferfree();
			return ;
		}
		if(flen  <= USBD_UVC_PKGSIZE)
		{
			debg("u: <j err:%x >\n",flen);	
			hal_mjpegRawBufferfree();
			return ;
		}
		u8 *txbuf ,*txfifo;
		u32 tlen;		
		ftgl ^= 1; 	//uvc tgl
		PTS++;	 	//uvc pts	
		
		txbuf = _uvc_txbuf_[0];
		txfifo = _uvc_txfifo_;
		while(flen){
			(flen >= USBD_UVC_PKGSIZE)? (tlen = USBD_UVC_PKGSIZE):(tlen = flen); 
			uvc_header_fill(ftgl, PTS, 0, txfifo);
			ax32xx_mcpy1_sdram2gram(txbuf, fbuf, tlen);
			ax32xx_mcpy_gram2ufifo(txfifo+12, txbuf,tlen);

			usb_uvciso_kick((u32)txfifo, tlen+12);
			if(!usb_uvciso_wait_txdown()) return;
			flen -= tlen;
			fbuf += tlen;
			hal_uac_send();
		}
		
		//hal_mjpegQueueFree(uvcjhal);
		uvc_header_fill(ftgl & 0x01, PTS, 1, txfifo);
		usb_uvciso_kick((u32)txfifo, 12);	
		if(!usb_uvciso_wait_txdown()) return;
		hal_mjpegRawBufferfree();
		
        
/*		static u8 bftgl;
		ftgl ^= 1; 	//uvc tgl
		PTS++;	 	//uvc pts	
		
		//first tx
		(flen >= USBD_UVC_PKGSIZE)? (tlen = USBD_UVC_PKGSIZE):(tlen = flen); 
		txbuf = _uvc_txbuf_[bftgl];
		ax32xx_dmaMemKick(txbuf+12, fbuf, tlen);
		ax32xx_dmaMemDown();
		
		//pkg tx
		while(flen)
		{
			uvc_header_fill(ftgl, PTS, 0, _uvc_txfifo_);
			usb_uvciso_kick((u32)txbuf, tlen + 12);
			
			(flen >= tlen)? (tlen = USBD_UVC_PKGSIZE):(tlen = flen); 
			flen -= tlen;
			fbuf += tlen;
			
			if(tlen){
				txbuf = _uvc_txbuf_[bftgl ^= 1];
				ax32xx_dmaMemKick(txbuf+12, fbuf, tlen);
				ax32xx_dmaMemDown();	
			}
			if(!usb_uvciso_wait_txdown()){
				break;
			}	
		}
		uvc_header_fill(ftgl & 0x01, PTS, 1, _uvc_txfifo_);
		usb_uvciso_kick((u32)_uvc_txfifo_, 12);	
		hal_mjpegRawBufferfree();
*/

}

/*******************************************************************************
* Function Name  : hal_usbdProcess
* Description    : hal layer.usb device process 
* Input          :  
* Output         : None
* Return         : none
*******************************************************************************/
bool hal_usbdProcess(void)
{
	if(false == rbc_process())
	{
		return false;
	}
#if UVC_MOD	== UVC_MJP
	hal_usbdUVCProcess();	
#endif
	//hal_auadcUACProcess();
	return true;
}
/*******************************************************************************
* Function Name  : usb_detect
* Description    : hal layer.usb device check for pc conected or not
* Input          :  
* Output         : None
* Return         : 0 : pc connected
                      <0: no pc connected
*******************************************************************************/
s32 hal_usbdCheck(void)
{
	return usb_device_check();
}
