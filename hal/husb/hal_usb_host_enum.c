/*
 * usb_host_enum.c
 *
 *  Created on: 2013Äê10ÔÂ24ÈÕ
 *      Author: jingcunxu
 */
#include "../inc/hal.h"


//#define husb_debgbuf(a,b)	debgbuf(a,b)
#define husb_debgbuf(a,b)

#define 	ep0fifo 		_ep0_fifo_		//must in ram addr
typedef  enum 
{
	_OK_,
	_STALL_,
	_PIDER_,
	_PLDER_,
	_TOUT_,
	_UNKWN_,
	
}TSTA;

typedef enum 
{
	PID_SETUP_PHASE,	
	PID_IN_PHASE,
	PID_OUT_PHASE,
	SETUP_END_PHASE,
}PID_PHASE;

#define __INTFS_STACK__		16
SDEV_DESC dev;
SCFG_DESC cfg;
SINTF_DESC intfs[__INTFS_STACK__];
SEP_DESC edpt[__INTFS_STACK__];	


/**
 * msc desc struct (not use)
 */
typedef struct
{
	u8 ctyp;
	u8 cv;
	u8 intfs;
	u8 altset;
	u8 endpts;
	
	u8 epin;
	u8 attrin;
	u16 inpload;
	
	u8 epout;
	u8 attrout;
	u16 outpload;

}MSCDEV;
MSCDEV msc;

/**
 * uvc desc struct
 */
typedef struct
{
	u8 ctyp;
	
	u8 cv;
	u8 epctl;
	u8 ctl_intfs;
	u8 ctl_altset;
	u8 ctl_pload;
	u8 ctl_ttyp;
	u8 ctl_interval;
	
	u8 epstrm;
	u8 strm_intfs;
	u8 strm_altset;
	u16 strm_pload;
	u8 strm_ttyp;
	u8 strm_interval;
}UVCDEV;
UVCDEV uvc;

/**
 * usb cmd state struct
 */
typedef struct
{
	u8 enum_phase;
	u8 pid_phase;
	u16 inlen;
	u8 uaddr;
}ENUM_STA;
ENUM_STA  usbsta;
VIDEO_COMMIT commit;

u8 desc[2048]; 	//all desc buff
u8 pcommit[64];	//uvc commit buff
u8 backstabuf[4]; //car backtate
u8 iicpr[4];
//static u8 T_enum_phase;
extern u8 usensor_237a_2_patch(void);

/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/


void cnvr(u8 *p)
{
	commit.bmhint = (p[1] << 8) + p[0];
	commit.bFormatIndex = p[2];
	commit.bFrameIndex = p[3];
	commit.dwFrameInterval = (p[7] << 24) +(p[6] << 16) +(p[5] << 8) +(p[4] << 0) ;
	commit.wKeyFrameRate = (p[9] << 8) + p[8];
	commit.wPFrameRate = (p[11] << 8) + p[10];
	commit.wCompQuality = (p[13] << 8) + p[12];
	commit.wCompWindowSize = (p[15] << 8) + p[14];
	commit.wDelay = (p[17] << 8) + p[16];
	commit.dwMaxVideoFrameSize=(p[21] << 24) +(p[20] << 16) +(p[19] << 8) +(p[18] << 0) ;
	commit.dwMaxPayloadTSize = (p[25] << 24) +(p[24] << 16) +(p[23] << 8) +(p[22] << 0) ;
	
	debg(".bmhint:%x\n",		commit.bmhint);
	debg(".bFormatIndex:%x\n",	commit.bFormatIndex);
	debg(".bFrameIndex:%x\n",	commit.bFrameIndex);
	debg(".dwFrameInterval:%x\n",commit.dwFrameInterval);
	debg(".wKeyFrameRate:%x\n",	commit.wKeyFrameRate);
	debg(".wPFrameRate:%x\n",	commit.wPFrameRate);
	debg(".wCompQuality:%x\n",	commit.wCompQuality);
	debg(".wCompWindowSize:%x\n",commit.wCompWindowSize);
	debg(".wDelay:%x\n",		commit.wDelay);
	debg(".dwMaxVideoFrameSize:%x\n",commit.dwMaxVideoFrameSize);
	debg(".dwMaxPayloadTSize:%x\n",commit.dwMaxPayloadTSize);	
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
bool husb_desc_devide(u8 *alldesc)
{
	u8 *pdesc = alldesc;
	u32 dclen = cfg.t_length;
	
	u32 iinfs = 0;
	u32 iedpt = 0;
	debg("dclen:%x\n",dclen);
	
	while(dclen){
		
		u32 stplen = pdesc[0];
		
		//configuration desc
		if(pdesc[1] == 0x02){ 
			debg("[  configuration desc ]\n");
			husb_debgbuf(pdesc, pdesc[0]);
			
			cfg.num_intf = pdesc[4];	
			cfg.cv = pdesc[5];								
		} 
		
		//interface desc
		if(pdesc[1] == 0x04){ 
			debg("[  interface desc ]\n");
			husb_debgbuf(pdesc, pdesc[0]);
			
			intfs[iinfs].type = pdesc[1];
			intfs[iinfs].num = pdesc[2];
			intfs[iinfs].alt_tring = pdesc[3];
			intfs[iinfs].end_points = pdesc[4];
			intfs[iinfs].iclass = pdesc[5];
			intfs[iinfs].sub = pdesc[6];
			iinfs++;
		} 
		//endpoint desc
		if(pdesc[1] == 0x05){  
			debg("[  endpoint desc ]\n");
			husb_debgbuf(pdesc, pdesc[0]);

			edpt[iedpt].type = pdesc[1];
			edpt[iedpt].ep_addr = pdesc[2];
			edpt[iedpt].attr = pdesc[3];
			edpt[iedpt].pay_load = (pdesc[5] << 8)+ pdesc[4]; 
			edpt[iedpt].interval  = pdesc[6];
			iedpt++;
		} 
		//seek desc
		pdesc  += stplen;
		dclen -=  stplen;
	}
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
bool husb_dev_analysis(void)
{
	{
		u32 iinfs = 0;
		u32 iedpt = 0;
		
		for(; iinfs < __INTFS_STACK__; iinfs++){
			
			SINTF_DESC *pintfs = &intfs[iinfs];
			if((pintfs -> type == 0))
				break;
			husb_debgbuf(pintfs, 9);	
			
			//mass storage
			if(pintfs -> iclass == MASS_STORAGE_CLASS){
				
				msc.intfs = pintfs -> num;
				msc.altset = pintfs -> alt_tring;
				msc.endpts = pintfs -> end_points;
				msc.ctyp = 	pintfs -> iclass;
				
				u32 i;
				for(i = 0; i < pintfs -> end_points; i++,iedpt++){
					SEP_DESC *pendpt = &edpt[iedpt];
					if(pendpt -> type){
						//in
						if(pendpt -> ep_addr & 0x80){
							msc.epin = pendpt -> ep_addr;
							msc.attrin = pendpt->attr;
							msc.inpload = pendpt -> pay_load;
						}
						//out
						else{
							msc.epout = pendpt -> ep_addr;
							msc.attrout = pendpt->attr;
							msc.outpload = pendpt -> pay_load;						
						}							
					}
				}
			}
			
			//uvc add......
			else if(pintfs -> iclass == UVC_CLASS){
				//uvc.altset = pintfs -> alt_tring;
				//uvc.endpts = pintfs -> end_points;
				uvc.ctyp = 	pintfs -> iclass;	

				u32 i;
				for(i = 0; i < pintfs -> end_points; i++,iedpt++){
					
					SEP_DESC *pendpt = &edpt[iedpt];
					if(pintfs -> sub == 0x01){
						uvc.ctl_intfs = pintfs->num;
						uvc.ctl_altset = pintfs->alt_tring;
						uvc.epctl = pendpt -> ep_addr;
						uvc.ctl_ttyp = pendpt -> attr;
						uvc.ctl_pload = pendpt -> pay_load;
						uvc.ctl_interval = pendpt -> interval;
					}
					if(pintfs -> sub == 0x02){
						uvc.strm_intfs= pintfs->num;
						uvc.strm_altset = pintfs->alt_tring;
						uvc.epstrm= pendpt -> ep_addr;
						uvc.strm_ttyp = pendpt -> attr;
						uvc.strm_pload= pendpt -> pay_load;
						uvc.strm_interval = pendpt -> interval;
					}
				}
			}
			//uac add......
			else if(pintfs -> iclass == UAC_CLASS){
				
				u32 i;
				for(i = 0; i < pintfs -> end_points; i++,iedpt++){
				}
			}
			
			// other dev
			else{
				
				u32 i;
				for(i = 0; i < pintfs -> end_points; i++,iedpt++){
				}
			}
			
		}
	}	
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
void husb_pidsetup(u8 *psetup, u16 len, u8 uaddr)
{
	//debg("uaddr:%X\n",uaddr);
	u32 i;
	u8 t_epx = USB20_Index;
	USB20_Index = 0;
	USB20_FAddr = uaddr;
	USB20E0ADR =  (u32)ep0fifo; //reset USB20_FIFO0 to ep0fifo	
	
	for (i = 0; i < len; i++){
		USB20_FIFO0 = psetup[i]; 
	}	
	husb_debgbuf(psetup, 8);
	USB20_CSR0 = 0x0A; 
	USB20_Index = t_epx;
}
void husb_pidin_kick(u8 uaddr)
{	
	u8 t_epx = USB20_Index;
	USB20_Index = 0;
	USB20_FAddr = uaddr;
	USB20E0ADR =  (u32)ep0fifo; //reset USB20_FIFO0 to ep0fifo
	USB20_CSR0 = 0x40 | 0x20;
	USB20_Index = t_epx;
}
void husb_pidin_next(u8 uaddr)
{
	USB20_CSR0 &= ~0x01; // Clear INPktRdy bit
	
	u8 t_epx = USB20_Index;
	USB20_Index = 0;
	USB20_FAddr = uaddr;
	USB20E0ADR =  (u32)ep0fifo; //reset USB20_FIFO0 to ep0fifo
	USB20_CSR0 = 0x20;
	USB20_Index = t_epx;
}
void husb_pidout(u8 *pout, u16 tlen, u8 uaddr)
{
	u8 t_epx = USB20_Index;
	USB20_Index = 0;
	USB20_FAddr = uaddr;
	if((tlen !=0)&&(pout)){
		u32 i;
		for (i = 0; i < tlen; i++){
			USB20_FIFO0 = pout[i]; 
		}	
		husb_debgbuf(pout, tlen);
		USB20_CSR0 = 0x00 | 0x02;
	}
	else 
		USB20_CSR0 = 0x40 | 0x02;
	USB20_Index = t_epx;
}

u8 husb_pidin_get(u8 *pin, u16 *len, u8 uaddr)
{
	*len = 0;
	u8 t_epx = USB20_Index;
	USB20_Index = 0;
	USB20_FAddr = uaddr;
	
	U8 intr = USB20_CSR0;
	//send stall
	if (intr & 0x10){
		deg("hep0: <send stall er>\n");
		USB20_Index = t_epx;
		return _STALL_;
	}
	
	// STALL detected
	if (intr & 0x04){
		deg("hep0: <stall detected>\n");
		USB20_CSR0 &= ~0x04;
		USB20_Index = t_epx;
		return _STALL_;                                // for unsupported request.
	}
	
	//in
	if ((intr & 0x01) == 0x01){
		u32 i;
		u32 rlen = USB20_OutCount1;
		for (i = 0; i < rlen; i++){
			if(pin)
				pin[i] = USB20_FIFO0;
			else 
				USB20_FIFO0;
		}
		if(pin)
			husb_debgbuf(pin,rlen);
		*len = rlen;
		USB20_CSR0 &= ~0x01; 
		USB20_Index = t_epx;
		return _OK_;  
	}
	if((intr & 0x02) == 0x00){
		//deg("out end\n");
		USB20_Index = t_epx;
		return _OK_;
	}
	debg("?");
	return 	_UNKWN_;	
}

/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
//u8 setuptemp[8] __attribute__ ((section(".sram_comm"))); 
void husb11_pidsetup(u8 *psetup, u16 len, u8 uaddr)
{
	u32 i;
	u8 t_epx = usb11_registers_read(Index);//USB_Index;
	usb11_registers_write(Index,0x0);//USB_Index = 0;
	usb11_registers_write(FAddr,uaddr);//USB_FAddr = uaddr;
	USB11E0ADR =(u32)uaddr; //mv_usb_dev.ep0_buf;//U20E0ADR =  (u32)ep0fifo; //reset USB_FIFO0 to ep0fifo	
	for (i = 0; i < len; i++){
		//debg("psetup[i]:%X\n",psetup[i]);
		usb11_registers_write(FIFO0,psetup[i]);//USB_FIFO0 = psetup[i]; 
	}
	husb_debgbuf(psetup, 8);
	usb11_registers_write(CSR0,0x0a);//USB_CSR0 = 0x0A; 
	usb11_registers_write(Index,t_epx);//USB_Index = t_epx;
	
}
void husb11_pidin_kick(u8 uaddr)
{	
	u8 t_epx = usb11_registers_read(Index);//u8 t_epx = USB_Index;
	usb11_registers_write(Index,0x0);//USB_Index = 0;
	usb11_registers_write(FAddr,uaddr);//USB_FAddr = uaddr;
	USB11E0ADR =(u32)uaddr;//mv_usb_dev.ep0_buf;//U20E0ADR =  (u32)ep0fifo; //reset USB_FIFO0 to ep0fifo
	usb11_registers_write(CSR0,0x40|0x20);//USB_CSR0 = 0x40 | 0x20;
	usb11_registers_write(Index,t_epx);//USB_Index = t_epx;
}
void husb11_pidin_next(u8 uaddr)
{
	//USB_CSR0 &= ~0x01; // Clear INPktRdy bit
	usb11_registers_write(CSR0, usb11_registers_read(CSR0)&(~0x01));
	u8 t_epx =  usb11_registers_read(Index);//USB_Index;
	usb11_registers_write(Index,0x0);//USB_Index = 0;
	usb11_registers_write(FAddr,uaddr);//USB_FAddr = uaddr;
	USB11E0ADR = (u32)uaddr;//mv_usb_dev.ep0_buf;//U20E0ADR =  (u32)ep0fifo; //reset USB_FIFO0 to ep0fifo
	usb11_registers_write(CSR0,0x20);//USB_CSR0 = 0x20;
	usb11_registers_write(Index,t_epx);//USB_Index = t_epx;
	
}
void husb11_pidout(u8 *pout, u16 tlen, u8 uaddr)
{
	u8 t_epx =  usb11_registers_read(Index);//USB_Index;
	usb11_registers_write(Index,0x0);//USB_Index = 0;
	usb11_registers_write(FAddr,uaddr);//USB_FAddr = uaddr;
	if((tlen != 0)&&(pout)){
		u32 i;
		for (i = 0; i < tlen; i++){
			usb11_registers_write(FIFO0, pout[i]);
		}
		usb11_registers_write(CSR0,0x00|0x02);//USB_CSR0 = 0x40 | 0x02;
		husb_debgbuf(pout, tlen);
	}
	else 
		usb11_registers_write(CSR0,0x40|0x02);//USB_CSR0 = 0x40 | 0x02;
		
	usb11_registers_write(Index,t_epx);	
}

u8 husb11_pidin_get(u8 *pin, u16 *len, u8 uaddr)
{
	*len = 0;
	u8 t_epx = usb11_registers_read(Index);
	usb11_registers_write(Index,0x0);//USB_Index = 0;
	usb11_registers_write(FAddr,uaddr);//USB_FAddr = uaddr;
	
	U8 intr =  usb11_registers_read(CSR0);//USB_CSR0;
	//debg("CSR0:%x\n",intr);
	//send stall
	if (intr & 0x10){
		//debg("CSR0:%x\n",intr);
		debg("hep0: <no response!>\n");
		usb11_registers_write(Index,t_epx);//USB_Index = t_epx;
		return _STALL_;
	}
	
	// STALL detected
	if (intr & 0x04){
		debg("hep0: <stall detected>\n");
		usb11_registers_write(CSR0, usb11_registers_read(CSR0)&(~0x04));//USB_CSR0 &= ~0x04;
		usb11_registers_write(Index,t_epx);//USB_Index = t_epx;
		return _STALL_;                                // for unsupported request.
	}
	
	//in
	if ((intr & 0x01) == 0x01){
		u32 i;
		u32 rlen = usb11_registers_read(RxCount1);//USB_OutCount1;
		for (i = 0; i < rlen; i++){
			if(pin)
				pin[i] = usb11_registers_read(FIFO0);
			else 
				usb11_registers_read(FIFO0);
		}	
		*len = rlen;
		
		
		usb11_registers_write(CSR0, usb11_registers_read(CSR0)&(~0x01));//USB_CSR0 &= ~0x01; 
		usb11_registers_write(Index,t_epx);//USB_Index = t_epx;
		return _OK_;  
	}
	if((intr & 0x02) == 0x00){
		debg("out end\n");
		usb11_registers_write(Index,t_epx);//USB_Index = t_epx;
		return _OK_;
	}
	debg("?");
	return 	_UNKWN_;	
	
}



/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/


u8 get_desc(SETUPPKT *psetup, u8 *desc)
{
	static u8* prev;
	//send usb cmd	
	if(usbsta.pid_phase == 0){

		//usbsta.inlen = psetup->setup.length ;
		if(usb_host_type == 2)
			husb_pidsetup((u8*)psetup, 8, usbsta.uaddr);
		else if (usb_host_type == 1)
			husb11_pidsetup((u8*)psetup, 8, usbsta.uaddr);
		usbsta.pid_phase = 1;
		prev = desc; //pointer to desc
		usbsta.inlen = psetup->setup.length;
		return -1;
	}
	if(usbsta.pid_phase == 1){
		if(usb_host_type == 2)
			husb_pidin_kick(usbsta.uaddr);
		else if (usb_host_type == 1)
			husb11_pidin_kick(usbsta.uaddr);
		usbsta.pid_phase = 2;
		return -1;	
	}
	
	//usb in
	if(usbsta.pid_phase == 2){
		
		//in
		u16 getlen = 0;
		if(usb_host_type == 2){
			if(_OK_ != husb_pidin_get(prev,&getlen, usbsta.uaddr)){
				usbsta.pid_phase = 0;
				return false;
			}
		}
		else if(usb_host_type == 1){
			if(_OK_ != husb11_pidin_get(prev,&getlen, usbsta.uaddr)){
				usbsta.pid_phase = 0;
				return false;
			}
		}
		//debg("inlen:%x,%x\n",usbsta.inlen,getlen);
		if(usbsta.inlen > getlen){
			 usbsta.inlen -= getlen;
			 prev += getlen;
			 if(usb_host_type == 2)
				husb_pidin_next(usbsta.uaddr);	
			else if(usb_host_type == 1)
				husb11_pidin_next(usbsta.uaddr);	
			usbsta.pid_phase = 2;
			return -1;
		}
		usbsta.pid_phase = 3;
	}
	//out
	if(usbsta.pid_phase == 3){
		if(usb_host_type == 2)
			husb_pidout(NULL,0,usbsta.uaddr);
		else if(usb_host_type == 1)
			husb11_pidout(NULL,0,usbsta.uaddr);
		usbsta.pid_phase = 4;
		return -1;		
	}
	if(usbsta.pid_phase == 4){
		usbsta.pid_phase = 0;
		return true; 
	}
	usbsta.pid_phase = 0;
	return false;
}

u8 set_desc(SETUPPKT *psetup)
{
	//send usb cmd	
	if(usbsta.pid_phase == 0){
		usbsta.inlen = psetup->setup.length ;
		if(usb_host_type == 2)
			husb_pidsetup((u8*)psetup, 8, usbsta.uaddr);
		else  if(usb_host_type == 1)
			husb11_pidsetup((u8*)psetup, 8, usbsta.uaddr);
		usbsta.pid_phase = 1;
		return -1;
	}
	if(usbsta.pid_phase == 1){
		if(usb_host_type == 2)
			husb_pidin_kick(usbsta.uaddr);
		else if(usb_host_type == 1)
			husb11_pidin_kick(usbsta.uaddr);
		usbsta.pid_phase = 2;
		return -1;	
	}
	
	if(usbsta.pid_phase == 2){
		u16 inlen;
		if(usb_host_type == 2)
			husb_pidin_get(NULL,&inlen, usbsta.uaddr);	
		else if(usb_host_type == 1)
			husb11_pidin_get(NULL,&inlen, usbsta.uaddr);	
		usbsta.pid_phase = 0;
		return true;
	}
	usbsta.pid_phase = 0;
	return false;
}
u8 set_dat(SETUPPKT *psetup,u8 *desc)
{
	//send usb cmd	
	if(usbsta.pid_phase == 0){
		usbsta.inlen = psetup->setup.length ;
		if(usb_host_type == 2)
			husb_pidsetup((u8*)psetup, 8, usbsta.uaddr);
		else if(usb_host_type == 1)
			husb11_pidsetup((u8*)psetup, 8, usbsta.uaddr);
		usbsta.pid_phase = 1;
		return -1;
	}
	if(usbsta.pid_phase == 1){
		if(usb_host_type == 2)
			husb_pidout(desc,usbsta.inlen,usbsta.uaddr);
		else if(usb_host_type == 1)
			husb11_pidout(desc,usbsta.inlen,usbsta.uaddr);
		usbsta.pid_phase = 2;
		return -1;
	}
	if(usbsta.pid_phase == 2){
		if(usb_host_type == 2)
			husb_pidin_kick(usbsta.uaddr);	
		else if(usb_host_type == 1)
			husb11_pidin_kick(usbsta.uaddr);
		usbsta.pid_phase = 3;
		return -1;	
	}
	
	if(usbsta.pid_phase == 3){
		u16 inlen;
		if(usb_host_type == 2)
			husb_pidin_get(NULL,&inlen, usbsta.uaddr);
		else if(usb_host_type == 1)
			husb11_pidin_get(NULL,&inlen, usbsta.uaddr);
		usbsta.pid_phase = 0;	
		return true;
	}
	usbsta.pid_phase = 0;
	return false;
}




void usb_stack_init(void)
{
	//usb stack init
	memset((u8 *)&uvc,-1,sizeof(uvc));
	memset((u8 *)&msc,-1,sizeof(msc));
	memset((u8 *)&dev,0,sizeof(dev));
	memset((u8 *)&cfg,0,sizeof(cfg));
	memset((u8 *)&intfs,0,sizeof(intfs));
	memset((u8 *)&edpt,0,sizeof(edpt));	
	memset((u8 *)&usbsta,0,sizeof(usbsta));	
	memset((u8 *)&backstabuf,-1,sizeof(backstabuf));
	
	//usb speed connect
	//T_enum_phase = -1;
	
}



/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
u8 husb_1st_get_devdsc(void)
{
	setup_pkt.setup.mrequest     = 0x80;
	setup_pkt.setup.request      = GET_DESCRIPTOR;
	setup_pkt.setup.value = DEVICE;
	setup_pkt.setup.index = 0;
	setup_pkt.setup.length = 8;	
	return get_desc(&setup_pkt, (u8* )&dev);
}
u8 husb_set_devaddr(void)
{
	setup_pkt.setup.mrequest     = 0x00;
	setup_pkt.setup.request      = SET_ADDRESS;
	setup_pkt.setup.value = __DEV_ADDR__;
	setup_pkt.setup.index = 0;
	setup_pkt.setup.length = 0;	
	u8 ret = set_desc(&setup_pkt);
	if(true == ret){
		usbsta.uaddr = __DEV_ADDR__;
	}	
	return ret;
}
extern void usensor_exsrc_update_A1(void);
extern void usensor_uispsrc_update_A1(void);
extern void usensor_exsrc_update_A2(void);
extern void usensor_uispsrc_update_A2(void);
u8 husb_all_get_devdsc(void)
{
/*
	setup_pkt.setup.mrequest     = 0x80;
	setup_pkt.setup.request      = GET_DESCRIPTOR;
	setup_pkt.setup.value = DEVICE;
	setup_pkt.setup.index = 0;
	setup_pkt.setup.length = dev.length;
	u8 ret = get_desc(&setup_pkt, (u8* )&dev);
	if(ret == true){
		//update 237X exmand
		if((dev.id_vendor == 0x1902)){
			if(dev.id_product == 0x3233){
				usensor_exsrc_update_A1();
				usensor_uispsrc_update_A1();				
			}
			if(dev.id_product == 0x327){
				usensor_exsrc_update_A2();
				usensor_uispsrc_update_A2();					
			}
		}
		
	}
	return ret;
*/
	return 0;
}
u8 husb_1st_get_cfg_desc(void)
{
	setup_pkt.setup.mrequest    = 0x80;
	setup_pkt.setup.request     = GET_DESCRIPTOR;
	setup_pkt.setup.value 		= CONFIGURATION;
	setup_pkt.setup.index 		= 0;
	setup_pkt.setup.length 		= 9;
	return get_desc(&setup_pkt, (u8* )&cfg);	
}
u8 husb_all_get_cfg_desc(void)
{
	setup_pkt.setup.mrequest    = 0x80;
	setup_pkt.setup.request     = GET_DESCRIPTOR;
	setup_pkt.setup.value 		= CONFIGURATION;
	setup_pkt.setup.index 		= 0;
	setup_pkt.setup.length 		= cfg.t_length;
	u8 ret = get_desc(&setup_pkt, desc);	
	if(true == ret){
		//decode desc
		if(!husb_desc_devide(desc)){
			return false;	
		}
		if(!husb_dev_analysis()){
			return false;
		}
		if(msc.ctyp == MASS_STORAGE_CLASS){
			debg("msc: VID,PID:%x,%x\n",dev.id_vendor, dev.id_product);
			debg("msc: -msc:<cv:%x, intfs:%x, altset:%x,endpts:%x>\n",msc.cv, msc.intfs, msc.altset, msc.endpts);
			debg("msc: -epin:<epin:%x, attrin:%x, inpload:%x\n",msc.epin, msc.attrin, msc.inpload);
			debg("msc: -epout:<epout:%x, attrout:%x, outpload:%x\n",msc.epout, msc.attrout, msc.outpload);
			
		}else if(uvc.ctyp == UVC_CLASS){
			debg("uvc: -VID,PID:%x,%x\n",dev.id_vendor, dev.id_product);
			debg("uvc: -ctl_intfs:%x, epctl:%x, ctl_pload: %x, ctl_altset:%x \n",uvc.ctl_intfs ,uvc.epctl,uvc.ctl_pload,uvc.ctl_altset);
			debg("uvc: -strm_intfs:%x, epstrm:%x, strm_pload: %x, strm_altset:%x \n",uvc.strm_intfs ,uvc.epstrm,uvc.strm_pload, uvc.strm_altset);
		}		
	}
	return ret;
}
u8 husb_set_cfg(void)
{
	setup_pkt.setup.mrequest    = 0x00;
	setup_pkt.setup.request     = SET_CONFIG;
	setup_pkt.setup.value 		= 1;
	setup_pkt.setup.index 		= 0;
	setup_pkt.setup.length 		= 0;
	return set_desc(&setup_pkt)	;
}
u8 husb_set_intfs_0(void)
{
	setup_pkt.setup.mrequest    = 0x01;
	setup_pkt.setup.request     = SET_INTERFACE;
	setup_pkt.setup.value 		= 0;
	setup_pkt.setup.index 		= uvc.strm_intfs;
	setup_pkt.setup.length 		= 0;
	return set_desc(&setup_pkt);
	
}
u8 husb_set_intfs_1(void)
{
	setup_pkt.setup.mrequest    = 0x01;
	setup_pkt.setup.request     = SET_INTERFACE;
	setup_pkt.setup.value 		= 1;
	setup_pkt.setup.index 		= uvc.strm_intfs;
	setup_pkt.setup.length 		= 0;	
	u8 ret= set_desc(&setup_pkt);
	if(ret == true)
	{
		if(usb_host_type ==2){
			if(uvc_host_init(usbsta.uaddr, uvc.epstrm,-1,uvc.strm_pload,uvc.strm_interval)){
				u_dev.puvc = uvc_actech_as_usensor();	
				usensor_actech();
			}	
		}
		else if (usb_host_type == 1){
			if(uvc_host11_init(usbsta.uaddr, uvc.epstrm,-1,uvc.strm_pload,uvc.strm_interval)){
				u_dev.puvc = uvc11_actech_as_usensor();	
				usensor_actech();
			}
		}
	}
	return ret;
}
u8 hsub_get_pcommit_cs100(void)
{
	setup_pkt.setup.mrequest    = 0xa1;
	setup_pkt.setup.request     = UVC_GET_CUR;
	setup_pkt.setup.value 		= 0x0100;	//cs
	setup_pkt.setup.index 		= 1;		//uid
	setup_pkt.setup.length 		= 0x1a;		//len
	return get_desc(&setup_pkt,pcommit);
}	

u8 hsub_set_pcommit_cs100(void)
{
	setup_pkt.setup.mrequest    = 0x21;
	setup_pkt.setup.request     = UVC_SET_CUR;
	setup_pkt.setup.value 		= 0x0100;
	setup_pkt.setup.index 		= 1;
	setup_pkt.setup.length 		= 0x1a;	
	return set_dat(&setup_pkt,pcommit);
}	
u8 hsub_set_pcommit_cs200(void)
{
	setup_pkt.setup.mrequest    = 0x21;
	setup_pkt.setup.request     = UVC_SET_CUR;
	setup_pkt.setup.value 		= 0x0200;
	setup_pkt.setup.index 		= 1;
	setup_pkt.setup.length 		= 0x1a;	
	return set_dat(&setup_pkt,pcommit);	
}
u8 husb_backsta_check(void)
{
	setup_pkt.setup.mrequest    = 0xa1;
	setup_pkt.setup.request     = 0x81;
	setup_pkt.setup.value 		= 0x0100;

	if(dev.id_vendor == 0x1902){
		setup_pkt.setup.index 		= 0x0300;
		setup_pkt.setup.length 		= 0x04;			
	}
	else{
		setup_pkt.setup.index 		= 0x0200;
		setup_pkt.setup.length 		= 0x02;
	}
	
	u8 ret = get_desc(&setup_pkt,pcommit);
	if(ret == true){
		//card sta
		backstabuf[0] = pcommit[0];
		backstabuf[1] = pcommit[1];
		backstabuf[2] = pcommit[2];
		backstabuf[3] = pcommit[3];
		//debg(">:%x,%x,%x,%x\n",backstabuf[0],backstabuf[1],backstabuf[2],backstabuf[3]);
	}	
	return 	ret;	
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
volatile u8 busy[4];
u8 husb_ex_237a_iicbusy(void)
{
/*
	if(dev.id_vendor != 0x1902){
		return true;	
	}	
	setup_pkt.setup.mrequest    = 0xa1;
	//setup_pkt.setup.request     = 0x81;
    if(1 == usensor_237a_2_patch())
		setup_pkt.setup.value 		= 0xba60;//237a_2.c新版补丁
	else
		setup_pkt.setup.value 	= 0x02d0;//237a_2.c旧版补丁

	setup_pkt.setup.index 		= 0xfc;
	setup_pkt.setup.length 		= 0x04;
	u8 ret = get_desc(&setup_pkt,(u8*)busy);
	if(ret == true){
		debg("busy:%x,%x,%x,%x\n",busy[0],busy[1],busy[2],busy[3]);
		if((busy[0]) && (dev.id_product == 0x327)){
			usbsta.enum_phase -= 1;
			delay_5ms(10);
		}
	}

	return ret;
*/
	return 0;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
EX_MAND exmand,uisp;
u8 husb_ex_237a_iiprcfg(void)
{
	if(dev.id_vendor != 0x1902){
		return true;	
	}
	if(exmand.psensor == NULL){
		return true;	
	}
	
	u8 adrhum = exmand.psensor->addr_num;//1
	u8 dathum = exmand.psensor->data_num;//1
	u8 wid = exmand.psensor->w_cmd; //(0x68 << 1) | 0;
	u8 rid = exmand.psensor->r_cmd;//(0x68 << 1) | 1;
	setup_pkt.setup.mrequest    = 0xa1;
	setup_pkt.setup.request     = 0x00;
	setup_pkt.setup.value 		= (dathum<<8) + adrhum;
	setup_pkt.setup.index 		= 0x00fe;
	setup_pkt.setup.length 		= (rid<<8)|wid;
	u8 ret = set_desc(&setup_pkt);
	if(true == ret){
		debg("anum,dnum,wid,rid:%x,%x,%x,%x\n",adrhum,dathum,wid,rid);
	}	
	return ret;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
u8 sensor_id[4];
u8 husb_ex_237a_sensorid(void)
{
#if 0
	if(dev.id_vendor != 0x1902){
		return true;	
	}
	
	volatile u16 snsor_idreg = exmand.psensor->id_reg; //0x01;
	setup_pkt.setup.mrequest    = 0xa1;
	setup_pkt.setup.request     = 0x80;
	if(exmand.psensor->addr_num == 1){
		setup_pkt.setup.value 		= snsor_idreg;	
	}else{
		setup_pkt.setup.value 		=((snsor_idreg & 0xff)<<8)+ (u8)(snsor_idreg>>8);	
	}
	
	setup_pkt.setup.index 		= 0x00ff;
	setup_pkt.setup.length 		= 0;
	u8 ret = get_desc(&setup_pkt, sensor_id);	
	if(true == ret){
		debg("snsor check:%x,\n",sensor_id[0]);
		
		if(exmand.psensor->id != sensor_id[0]){
			exmand.iensor++;	
			if(sensor_cmd_tab[exmand.iensor] != NULL){
				exmand.psensor = sensor_cmd_tab[exmand.iensor];	
				if(usbsta.enum_phase >= 2)
					usbsta.enum_phase -= 2;
			}else{
				exmand.psensor = NULL;	
			}
		}else{
				if(dev.id_product == 0x327){
					debg("a2mclk = 192/(%d+1) M\n",exmand.psensor->mclk);
					if(0 == usensor_237a_2_patch())
						exmand.code[53] = exmand.psensor->mclk; //mdfmclk
					else
						exmand.code[161] = exmand.psensor->mclk;//mdfmclk
				
				}else{
					debg("a1mclk = 192/(%d+1) M\n",exmand.psensor->mclk);
					exmand.code[233] = exmand.psensor->mclk; //mdfmclk					
				}
		}
	}
	return ret;	
#endif
	return 0;
}
u8 husb_ex_237a_iiprcheck(void)
{
#if 0
	if(dev.id_vendor != 0x1902){
		return true;	
	}

	setup_pkt.setup.mrequest    = 0xa1;
	setup_pkt.setup.request     = 0x80;
	setup_pkt.setup.value 		= 0;
	setup_pkt.setup.index 		= 0x00fe;
	setup_pkt.setup.length 		= 0;
	u8 ret = get_desc(&setup_pkt, iicpr);	
	if(true == ret){
		debg("iic check:%x,%x,%x,%x\n",iicpr[0],iicpr[1],iicpr[2],iicpr[3]);
	}
	return ret;	
#endif
	return 0;
}

u8 husb_ex_237a_iiccmd(void)
{
#if 0
	if(dev.id_vendor != 0x1902){
		return true;	
	}
	
	if(exmand.psensor == NULL){
		return true;	
	}
	
	//poll all cmd
	static u32 icmd; 
	u8 endflg = false;
	u8 ret;
	
	for(;;){
		u8 cmd = exmand.psensor->init[icmd][0]; 	
		u8 dat = exmand.psensor->init[icmd][1]; 
		u8 cmd_next =  exmand.psensor->init[icmd+1][0]; 	
		u8 dat_next =  exmand.psensor->init[icmd+1][1]; 
		//if((icmd+1) >= exmand.niiccmd){//
		if((cmd_next== 0xff) && (dat_next == 0xff)){
			endflg = true;	
		}
		
		setup_pkt.setup.mrequest    = 0xa1;
		setup_pkt.setup.request     = 0x00;
		if(exmand.psensor->addr_num == 1){
		setup_pkt.setup.value 		= (dat<<8) + cmd;
		setup_pkt.setup.index 		= 0x00ff;
		setup_pkt.setup.length 		= 0;
		}else{
			setup_pkt.setup.value 		= ((cmd & 0xff)<<8)+ (u8)(cmd>>8);
			setup_pkt.setup.index 		= 0x00ff;
			setup_pkt.setup.length 		= dat;			
		}
		ret = set_desc(&setup_pkt);
		if(true == ret){
			debg("[  cmd,dat:%x,%x]\n",cmd,dat);
			if(endflg == true){
				icmd = 0;

				break;	
			}else{
				icmd++;
				continue;
			}	
		}else{
			break;
		}
	}	
	return ret;//-1,0,1
#endif
	return 0;
}

//extern const u8 driver_237a_1[]; 
u8 husb_ex_237a_ispdriver_down(void)
{
#if 0
	if((dev.id_vendor != 0x1902) || (dev.id_product != 0x3233)){
		return true;	
	}
	
	u32 adr_codinram = uisp.inram;
	
	u8 *pcod = uisp.code;
	u32 npkg = (uisp.clen + 63)/64;
	u16 pkglegth = 0x40;
	static u32 icodpkg;
	u8 endflg = false;
	u8 ret;
	
	for(;;){
		if((icodpkg + 1) >= npkg){
			endflg = true;	
		}
		if(uisp.clen - icodpkg*64 < pkglegth )
			pkglegth = uisp.clen - icodpkg*64;
		setup_pkt.setup.mrequest    = 0x21;
		//setup_pkt.setup.request     = 0x01;
		setup_pkt.setup.value 		= adr_codinram + icodpkg * 64; //code ram addr
		setup_pkt.setup.index 		= 0xfd; 					//ex code cmd 
		setup_pkt.setup.length 		= pkglegth;						//max len code per pkg
		ret = set_dat(&setup_pkt,(pcod + icodpkg*64));			//code 
		if(true == ret){
			if(endflg == true){
				debg("[  isp driver down ok: -code:%d,%d,%x]\n",uisp.clen, icodpkg+1, adr_codinram);
				icodpkg = 0;
				break;	
			}else{
				icodpkg++;
				continue;
			}
		}else{
			break;	
		}		
	}
	return ret;
#endif
	return 0;
}
u8 husb_ex_237a_ispexmand_down(void)
{
#if 0
	if((dev.id_vendor != 0x1902) || (dev.id_product != 0x3233)){
		return true;	
	}
	
	u32 adr_codinram = 0x2d60;
	u8 *pcod = uvcisp.expoint; //ex code
	u32 npkg = (uvcisp.exfilesize + 63)/64;
	static u32 icodpkg;
	
	u8 endflg = false;
	u8 ret;
	u16 pkglegth = 0x40;
	
	for(;;){
		if((icodpkg + 1) >= npkg){
			endflg = true;	
		}
		if(uvcisp.exfilesize - icodpkg*64 < pkglegth )
			pkglegth = uvcisp.exfilesize - icodpkg*64;
		setup_pkt.setup.mrequest    = 0x21;
	//	setup_pkt.setup.request     = 0x01;
		setup_pkt.setup.value 		= adr_codinram + icodpkg * 64; //code ram addr
		setup_pkt.setup.index 		= 0xfd; 					//ex code cmd 
		setup_pkt.setup.length 		= pkglegth;						//max len code per pkg
		ret = set_dat(&setup_pkt,(pcod + icodpkg*64));			//code 
		if(true == ret){
			if(endflg == true){
				debg("[  isp exmand code down ok: -code:%d]\n",icodpkg+1);
				icodpkg = 0;
				break;	
			}else{
				icodpkg++;
				continue;
			}
		}else{
			break;	
		}		
	}
	return ret;
#endif
	return 0;
}
u8 husb_ex_237a_isppointer_mdify(void)
{
#if 0
	if((dev.id_vendor != 0x1902) || (dev.id_product != 0x3233)){
		return true;	
	}
	
	u32 adr_funpointer = uisp.pfun[0][0];//0x0D1C;	//function pointer adr	
	u32 cod_funpointer = uisp.pfun[0][1];//0x0d60;		//function pointer to code addr
	
	u8 txbuf[4];
	txbuf[0] = (u8)(cod_funpointer>>0);
	txbuf[1] = (u8)(cod_funpointer>>8);
	txbuf[2] = 0;
	txbuf[3] = 0;
	setup_pkt.setup.mrequest    = 0x21;
	//setup_pkt.setup.request     = 0x00;
	setup_pkt.setup.value 		= (u16)adr_funpointer;
	setup_pkt.setup.index 		= 0xfd,
	setup_pkt.setup.length 		= 4;	
	
	u8 ret = set_dat(&setup_pkt,txbuf);	
	if(true == ret){
		debg("[  isp driver function modify pointer addr ok]\n");	
	}
	return ret;
#endif
	return 0;
}
u8 husb_ex_237a_isppointer_run(void)
{
#if 0
	if((dev.id_vendor != 0x1902) || (dev.id_product != 0x3233)){
		return true;	
	}
	
	u32 isplen = uvcisp.exfilesize;
	
	setup_pkt.setup.mrequest    = 0x21;
	//setup_pkt.setup.request     = 0x00;
	setup_pkt.setup.value 		= (u16)isplen;  
	setup_pkt.setup.index 		= 0xfa,      
	setup_pkt.setup.length 		= 0;	
	u8 ret = set_desc(&setup_pkt);	
	if(true == ret){
		debg("[  isp function run ok:%x]\n",isplen);	
	}
	return ret;
#endif
	return 0;
}
//extern const u8 exmand_code[]; 
u8 husb_ex_237a_exmandcod_down(void)
{
#if 0
	if(dev.id_vendor != 0x1902){
		return true;	
	}
		
	u32 adr_codinram = exmand.inram;
	u8 *pcod = exmand.code;
	u32 npkg = (exmand.clen + 63)/64;
	static u32 icodpkg;
	//debg("icodpkg:%d\n",icodpkg);
	u8 endflg = false;
	u8 ret;
	u16 pkglength = 0x40;
	for(;;){
		if((icodpkg + 1) >= npkg){
			endflg = true;	
		}
		if(exmand.clen - icodpkg*64 < pkglength )
			pkglength = exmand.clen- icodpkg*64;
		setup_pkt.setup.mrequest    = 0x21;
	//	setup_pkt.setup.request     = 0x01;
		setup_pkt.setup.value 		= adr_codinram + icodpkg * 64; //code ram addr
		setup_pkt.setup.index 		= 0xfd; 					//ex code cmd 
		setup_pkt.setup.length 		= pkglength;						//max len code per pkg
		ret = set_dat(&setup_pkt,(pcod + icodpkg*64));			//code 
		if(true == ret){
			if(endflg == true){
				debg("[  exmand code down ok: -code:%d,%d,%x]\n",exmand.clen,npkg,adr_codinram);
				icodpkg = 0;
				break;	
			}else{
				icodpkg++;
				//delay_5ms(50);
				continue;
			}
		}else{
			break;	
		}		
	}
	return ret;
#endif
	return 0;
}


u8 husb_ex_237a_funpointer_mdify(void)
{
#if 0
	if((dev.id_vendor != 0x1902)){
		return true;	
	}
	
	static u32 ifun;
	u8 endflg = false;
	u8 ret;
	for(;;){
		if((ifun+1)>= exmand.npfun){
			endflg = true;
		}
		
		u32 adr_funpointer = exmand.pfun[ifun][0]; //0x0D1C;	//function pointer adr	
		u32 cod_funpointer = exmand.pfun[ifun][1];//0x0d60;		//function pointer to code addr
		
		u8 txbuf[4];
		txbuf[0] = (u8)(cod_funpointer>>0);
		txbuf[1] = (u8)(cod_funpointer>>8);
		txbuf[2] = (u8)(cod_funpointer>>16);
		txbuf[3] = (u8)(cod_funpointer>>24);
		setup_pkt.setup.mrequest    = 0x21;
		//setup_pkt.setup.request     = 0x00;
		setup_pkt.setup.value 		= adr_funpointer;
		setup_pkt.setup.index 		= 0xfd,
		setup_pkt.setup.length 		= 4;	
		
		ret = set_dat(&setup_pkt,txbuf);	
		if(true == ret){
			debg("[  exmand  function pointer modify ok:%x,%x]\n",adr_funpointer,cod_funpointer);	
			if(endflg == true){
				ifun = 0;
				break;
			}else{
				ifun++;
				continue;
			}
			
		}else{
			break;	
		}			
	}	

	return ret;
#endif
	return 0;
}

u8 husb_ex_237a_funpointer_run(void)
{
#if 0
	if(dev.id_vendor != 0x1902){
		return true;	
	}
	if((dev.id_product == 0x3233)){
		return true;	
	}
	setup_pkt.setup.mrequest    = 0x21;
	//setup_pkt.setup.request     = 0x00;
	setup_pkt.setup.value 		= 0x0000;  
	setup_pkt.setup.index 		= 0xfa,      
	setup_pkt.setup.length 		= 0;	
	u8 ret = set_desc(&setup_pkt);	
	if(true == ret){
		debg("[  exmand code function run ok]\n");	
	}
	return ret;
#endif
	return 0;
}

typedef u8 (*pHUSBFUN)(void);
pHUSBFUN phusbfun[128];
pHUSBFUN pfun;
//user mod husb que
const pHUSBFUN user_husbfun[128]={
	husb_1st_get_devdsc,	
	husb_set_devaddr,
	husb_all_get_devdsc,
	husb_1st_get_cfg_desc,
	husb_all_get_cfg_desc,
	husb_set_cfg,
	husb_set_intfs_0,
	
	hsub_get_pcommit_cs100,
	hsub_set_pcommit_cs100,
	hsub_get_pcommit_cs100,
	hsub_set_pcommit_cs200,
	
	husb_ex_237a_exmandcod_down,
	husb_ex_237a_funpointer_mdify,
	//husb_ex_237a_funpointer_run,
	
	//husb_ex_237a_iiprcheck,
	husb_ex_237a_iicbusy,
	husb_ex_237a_iiprcfg,
	husb_ex_237a_sensorid,
	
	husb_ex_237a_exmandcod_down,//mclk cfg
	
	husb_ex_237a_iiccmd,
	husb_set_intfs_1,

	NULL,
};

// isp mod husb que
const pHUSBFUN isp_husbfun[128]={
	husb_1st_get_devdsc,	
	husb_set_devaddr,
	husb_all_get_devdsc,
	husb_1st_get_cfg_desc,
	husb_all_get_cfg_desc,
	husb_set_cfg,
	husb_set_intfs_0,

	husb_ex_237a_ispdriver_down, 	//down driver
	husb_ex_237a_ispexmand_down, 	//down eeprom dat
	husb_ex_237a_isppointer_mdify,	//function pointer set 
	husb_ex_237a_isppointer_run,	//function ponter run
	
	NULL,
};
 

void husb_kick_ep0isr(void)
{
#if HAL_CFG_SELF_TEST
	debg("usb type:%d\n",usb_host_type);
	usbsta.pid_phase=0;
	usbsta.enum_phase=0;
	pfun = (uvcisp.exfilesize)?isp_husbfun[usbsta.enum_phase]: user_husbfun[usbsta.enum_phase]; 
	if(NULL == pfun){
		return;	
	}
	//debg("[pid_phase:%d , enum_phase:%d]\n",usbsta.pid_phase,usbsta.enum_phase);
	pfun();
#endif
}
void husb_ep0isr(void)
{
	u8 ret ;
#if HAL_CFG_SELF_TEST
next:
#endif
	//debg("[pid_phase:%d , enum_phase:%d]\n",usbsta.pid_phase,usbsta.enum_phase);
	ret = pfun();
#if HAL_CFG_SELF_TEST
	if(true == ret){
		usbsta.pid_phase = 0;
		usbsta.enum_phase++;
		pfun = (uvcisp.exfilesize)?isp_husbfun[usbsta.enum_phase]: user_husbfun[usbsta.enum_phase];
		if(NULL == pfun){
			return;	
		}
		//pfun();//kick
		goto next;
	}
#endif
	if(false == ret){
		//husb_kick_ep0isr();
		if(usb_host_type ==2)
			usb_host_remove();
		else if (usb_host_type ==1)
			usb11_host_remove();
	}
}


















/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------
u8 uhost_check_cmdend(void)
{
	//debg(">:%d\n",usbsta.enum_phase);
	if((usbsta.enum_phase >= (T_enum_phase)) && (usbsta.pid_phase == 0)){
		return true;	
	}	
	return false;
}

#define USE_AG 0
#define USE_LT 1*/
void uhost_backsta_cmd(void)
{
	
/*	if(!uhost_check_cmdend()){
		return;	
	}
	usbsta.enum_phase = T_enum_phase+1;
	usbsta.pid_phase = 0;
	setup_pkt.setup.mrequest    = 0xa1;
	setup_pkt.setup.request     = 0x81;
	setup_pkt.setup.value 		= 0x0100;

	if(dev.id_vendor == 0x1902){
		setup_pkt.setup.index 		= 0x0300;
		setup_pkt.setup.length 		= 0x04;			
	}
	else{
		setup_pkt.setup.index 		= 0x0200;
		setup_pkt.setup.length 		= 0x02;
	}
//#endif
	
	get_desc(&setup_pkt,pcommit);
*/
	usbsta.pid_phase = 0;
	usbsta.enum_phase --;
		
	pfun = husb_backsta_check;
	pfun();
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
bool check_backsta(void)
{
	if(dev.id_vendor == 0x1902){
		if(backstabuf[2] == 0xfc){
			return true;	
		}else{
			return false;	
		}		
	}else{
		if((backstabuf[0] == 0x00) && (backstabuf[1] == 0x01)){ //00,01
			return true;	
		}
		else if((backstabuf[0] == 0x79) && (backstabuf[1] == 0x00)){ //79,01
			return true;	
		}
		else{
			return false;	
		}		
	}
}

u32 usb_enum_phase_get(void)
{
	return usbsta.enum_phase;	
}

void usb_enum_phase_clr(void)
{
	usbsta.enum_phase = 0;	
}
//-VID,PID:1b3f,8301 灵通
//-VID,PID:58f,3842 安国

