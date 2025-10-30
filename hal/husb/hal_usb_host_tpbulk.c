/*
 * usb_host_tpbulk.c
 *
 *  Created on: 2013Äê10ÔÂ25ÈÕ
 *      Author: jingcunxu
 */

#include "../inc/hal.h"

#define _USE_DMA_MOD_		1
//#define _DEBG_SCSI_(a,b)	debgbuf(a,b)
#define _DEBG_SCSI_(...)


DISKHAL diskhal;

MSCSTACK mscstack;
TPBLK_STRUC  tpbulk_block;

#define msc_rx_fifo		_msc_rxfifo_
#define msc_tx_fifo		_msc_txfifo_
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

u8  msc_init(u8 addr, u8 epin, u8 epout, u16 payload, u8 lun)
{

	u32 MSC_TX_FIFO = (u32)&msc_tx_fifo ;
	u32 MSC_RX_FIFO = (u32)&msc_rx_fifo ;

	if((MSC_TX_FIFO < ufifo_s) || (MSC_TX_FIFO > ufifo_e)){
		debg("msc_tx_fifo set err\n");
		return false;
	}
	if((MSC_RX_FIFO < ufifo_s) || (MSC_RX_FIFO > ufifo_e)){
		debg("msc_rx_fifo set err\n");	
		return false;
	}

	memset((u8 *)&diskhal, sizeof(diskhal),0);
	
	diskhal.maxlun = lun + 1;
	mscstack.addr = addr;
	mscstack.epbulkin = epin & 0x7f;
	mscstack.epbulkout = epout;
	mscstack.payload = payload;
	if(payload == 0) 
		return false;
	//debg("ep:in/out%x,%x\n",mscstack.epbulkin,mscstack.epbulkout);

	(USB20_Index)= BULKRX_EP;//mscstack.epbulkin;//EP_BULKTX;
	(USB20_InCSR1)=BIT(6) | BIT(3);
	(USB20_InCSR2)=0;
	(USB20_InMaxPl)=0x00;
	(USB20_InMaxPh)=0x04;//
	(USB20_OutCSR1)=BIT(7) | BIT(4);
	(USB20_OutCSR2) = 0;
	(USB20_OutMaxPl)=0x00;
	(USB20_OutMaxPh)=0x04;//

	(USB20_Index)= BULKTX_EP;//mscstack.epbulkout;//EP_BULKTX;
	(USB20_InCSR1)=BIT(6) | BIT(3);
	(USB20_InCSR2)=0;
	(USB20_InMaxPl)=0x00;
	(USB20_InMaxPh)=0x04;//
	(USB20_OutCSR1)=BIT(7) | BIT(4);
	(USB20_OutCSR2) = 0;
	(USB20_OutMaxPl)=0x00;
	(USB20_OutMaxPh)=0x04;//
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
u8 epbulk_send_dat(u8 *wbuf, u32 len)
{
	u16 tLen;
  	u8 i;
    mscstack.buffer = wbuf;//(u8 *)&tpbulk_block;
	
	//(*(volatile __sfr unsigned long*)(&USB20E1TADR + 8*(mscstack.epbulkout-1))) = (u32)msc_txbuf;
	
    USB20_Index = BULKTX_EP; //mscstack.epbulkout;// select endpiont
    USB20_TxType = ( TT_BULK | mscstack.epbulkout);
    USB20_FAddr = mscstack.addr;
	
	while(1){
		USB20E1TADR = (u32)msc_tx_fifo;//(u32)mscstack.buffer;
		
		tLen = (usb_type==HIGH_SPEED)? 512: 64;
		tLen= (len > tLen)? tLen:len;
		
	#if _USE_DMA_MOD_
		ax32xx_sys_dma_memkick(msc_tx_fifo, mscstack.buffer, tLen);
		ax32xx_sys_dma_memdown();
		USB20CON1 = BIT(13 + BULKTX_EP) |  tLen;	//set fifo pointer
		if(tLen < 512){
			_DEBG_SCSI_(msc_tx_fifo,tLen);
		}
	#else
		for(i=0;i< tLen ;i++){
			USB_FIFO1 = mscstack.buffer[i];
			//*(volatile __sfr unsigned long*)(&USB_FIFO1 + 16*(mscstack.epbulkout-1)) = mscstack.buffer[i];
		}
		if(tLen < 512){
			_DEBG_SCSI_(msc_tx_fifo,tLen);exception_trigger();//while(1);
		}
	#endif

		USB20_InCSR1 |=0x01;//enable ep1 send data
		USB20_InCSR1 &=~0x04;
		while (1)
		{
			i = USB20_InCSR1;
			if (i & 0x04){
				deg("INERR1\r\n");
				USB20_InCSR1=0;
				return 0;
			}

			if (i & 0x20) {
				deg("INERR2\r\n");
				return 0;     //½â¾ö¶Á¿¨Æ÷¿¨°ÎµôºóËÀÑ­»·µÄÎÊÌâ
			}
			if (!(i & 0x02)){
				//deg("cmd ok\r\n");
				break;
			}
		}	
	
		len -= tLen;
		mscstack.buffer += tLen;
		if(len == 0){
			break;	
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
u8 epbulk_receive_dat(u8 *buffer,u16 len)
{
    u8  intr;
    u16 rLen;
	USB20E1RADR = (u32)msc_rx_fifo;;
	//*(volatile __sfr unsigned long*)(&USB20E1RADR + 8*(mscstack.epbulkin-1)) = (u32)msc_rxbuf;
	
    USB20_Index = BULKRX_EP;//mscstack.epbulkin;//select ep1
    USB20_RxType = (TT_BULK|mscstack.epbulkin);//SET BULK AND ENDPIONT
    USB20_FAddr = mscstack.addr;
	
	
    while (1)
    {
    	USB20_OutCSR1 |= 0x20;//request pkt,send a in packet
        while (1)
        {
            intr = USB20_OutCSR1;
			//debg("3");
            if (intr & 0x04){
            	USB20_OutCSR1=0;
            	deg("cbw_rx-err2\r\n");
                return 0;
            }

            if (intr & 0x40){
            	deg("cbw_rx-err3\r\n");
                return 0;
            }

            if (intr & 0x01){
                rLen = (USB20_OutCount2 << 8) + USB20_OutCount1;
				
#if _USE_DMA_MOD_
				ax32xx_sys_dma_memkick(buffer,msc_rx_fifo,rLen);
				ax32xx_sys_dma_memdown();
				if(rLen < 512){
					_DEBG_SCSI_(msc_rx_fifo,rLen);
				}
#else
				u32 i;
                for(i=0; i<rLen; i++){
					buffer[i] = USB_FIFO1;//*(volatile __sfr unsigned long*)(&USB_FIFO1 + 16*(mscstack.epbulkin-1)); 
				}
				if(rLen < 512){
					_DEBG_SCSI_(buffer,rLen);
				}
#endif
				
                (len > rLen)?( len -= rLen):(len = 0);
                buffer += rLen;
				//debg("rLen, len:%x,%x\n",rLen,len);
				USB20E1RADR = (u32)msc_rx_fifo;;
            	USB20_OutCSR1 |= 0x10;//flush the next packet to be read from the endpoint Rx FIFO.
				
                if (!len)
                    return 1;
                else
                    break;
            }
        }
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
/*-----------------------------------------------------------------------------------*/
void cbw_init(void)
{
     memset(((u8 *)&tpbulk_cbw) ,0,31);
     tpbulk_cbw.cbw_signature = CBW_SIGNATURE;
     tpbulk_cbw.cbw_tag = CBW_TAG;
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
u8 spc_inquiry(u8 *quiry)
{
	debg("====  spc_inquiry  ====\n");
	cbw_init();
    tpbulk_cbw.cbw_data_xferlen = 0x00000024;
    tpbulk_cbw.cbw_flag=0x80;
    tpbulk_cbw.cbw_lun=usb_var.cbw_lun;
    tpbulk_cbw.cbw_cbdlen = 0x06;
    tpbulk_cbw.operation_code = INQUIRY;
    tpbulk_cbw.cbw_cblba[2]   = 0x24;

    //send cmd
    if (!epbulk_send_dat((u8 *)&tpbulk_block, 31)){
        return false;
    }
	/*
	if(!epbulk_send_dat((u8 *)&tpbulk_block, 31)){
		return false;
	}
	*/
    //receive device data
    if (!epbulk_receive_dat(quiry,tpbulk_cbw.cbw_data_xferlen)){
        return false;
    }
    //receive device status
    if (!epbulk_receive_dat((u8 *)&tpbulk_block,0)){
        return false;
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
u8 spc_test_unit_rdy(u8 *online)
{
	debg("====  spc_test_unit_rdy  ====\n");
    cbw_init();
    tpbulk_cbw.cbw_lun = usb_var.cbw_lun;
    tpbulk_cbw.cbw_cbdlen = 0x0C; //sizeof(INQUIRY_SPC);

    if (!epbulk_send_dat((u8 *)&tpbulk_block, 31)){
        return 0;
    }

    if (!epbulk_receive_dat((u8 *)&tpbulk_block,0)){
        return 0;
    }
	*online = !((bool )tpbulk_csw.csw_status);
    return 1;
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
u8 spc_request_sense(void)
{
	debg("====  spc_request_sense  ====\n");
	cbw_init();
	tpbulk_cbw.cbw_data_xferlen = 0x00000012;
	tpbulk_cbw.cbw_flag = 0x80;
	tpbulk_cbw.cbw_lun = usb_var.cbw_lun;
	tpbulk_cbw.cbw_cbdlen = 0x0C;	//sizeof(REQUEST_SENSE_SPC);
	tpbulk_cbw.operation_code     = 0x03;
	tpbulk_cbw.cbw_cblba[2]       = 0x12;

	if (!epbulk_send_dat((u8 *)&tpbulk_block, 31)){
		return false;
	}

	if (!epbulk_receive_dat((u8 *)&tpbulk_block,0)){
		return false;
	}

	if (!epbulk_receive_dat((u8 *)&tpbulk_block,0)){
		return false;
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
u8 rbc_read_capacity(u32 *cap, u32 *slen)
{
	debg("====  rbc_read_capacity  ====\n");
	
	u8 rx[8];
	*cap    = 0;
	*slen = 0;
	
	cbw_init();
	tpbulk_cbw.cbw_data_xferlen = 8;
	tpbulk_cbw.cbw_lun=usb_var.cbw_lun;
	tpbulk_cbw.cbw_flag=0x80;
	tpbulk_cbw.cbw_cbdlen = 0x0a;
	tpbulk_cbw.operation_code = READ_CAPACITY;

	if (!epbulk_send_dat((u8 *)&tpbulk_block, 31)){
		return false;
	}

	if (!epbulk_receive_dat(rx,tpbulk_cbw.cbw_data_xferlen)){
		return false;
	}

	if (!epbulk_receive_dat((u8 *)&tpbulk_block,0))	{
		return false;
	}		
	
	*cap = (rx[0] << 24) + (rx[1] << 16) + (rx[2] << 8) + (rx[3] << 0);
	*slen = (rx[4] << 24) + (rx[5] << 16) + (rx[6] << 8) + (rx[7] << 0);
	//debg("%x,%x\n",*cap,*slen);
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
s32 rbc_read_lba(u8 *buffer,u32 lba,u16 len)
{
	cbw_init();
	tpbulk_cbw.cbw_data_xferlen =  len << 9;
	tpbulk_cbw.cbw_flag         = 0x80;          //bmCBWFlags
	tpbulk_cbw.cbw_lun          = usb_var.cbw_lun;//bCBWLUN
	tpbulk_cbw.cbw_cbdlen       = 10;            //bCBWCBLength

	//CBWCB
	tpbulk_cbw.operation_code   = READ_DATA;
	tpbulk_cbw.cbw_cblba[3]     = (lba)&0xff;
	tpbulk_cbw.cbw_cblba[2]     = (lba>>8)&0xff;
	tpbulk_cbw.cbw_cblba[1]     = (lba>>16)&0xff;
	tpbulk_cbw.cbw_cblba[0]     = (lba>>24)&0xff;

	tpbulk_cbw.cbw_cblength     =  ((len&0xff)<<8)|((len>>8)&0xff);       //´«ÊäµÄÁ¬ÐøLBA³¤¶È

	//send read cmd
	if (!epbulk_send_dat((u8 *)&tpbulk_block, 31)){
		return 0;
	}
	//receive data
    if ((epbulk_receive_dat(buffer,len<<9)) == 0){
        return 0;
    }

    //receive status
    if (!epbulk_receive_dat((u8 *)&tpbulk_block,0)){
		return 0;
	}

    return 1;
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

s32 rbc_write_lba(u8 *buffer,u32 lba,u16 len)
{
//    u8  status;
    memset(((u8 *)&tpbulk_cbw) ,0x00,31);
    //CBW
    tpbulk_cbw.cbw_signature   =CBW_SIGNATURE;    //dCBWSignature
    tpbulk_cbw.cbw_tag         =CBW_TAG;          //dCBWTag
    tpbulk_cbw.cbw_data_xferlen = len << 9;
    tpbulk_cbw.cbw_flag        =0x00;             //bmCBWFlags
    tpbulk_cbw.cbw_lun         =usb_var.cbw_lun;   //bCBWLUN
    tpbulk_cbw.cbw_cbdlen      =10;               //bCBWCBLength
    //CBWCB
    tpbulk_cbw.operation_code    = WRITE_DATA;            
    tpbulk_cbw.cbw_cblba[3]      = (lba)&0xff;
    tpbulk_cbw.cbw_cblba[2]      = (lba>>8)&0xff;
    tpbulk_cbw.cbw_cblba[1]      = (lba>>16)&0xff;
    tpbulk_cbw.cbw_cblba[0]      = (lba>>24)&0xff;

    tpbulk_cbw.cbw_cblength      = ((len&0xff)<<8)|((len>>8)&0xff);        //´«ÊäµÄÁ¬ÐøLBA³¤¶È

    if (!epbulk_send_dat((u8 *)&tpbulk_block, 31)){
    	deg("rbc_write_lba err1\n");
        return 0;
    }

    if (!epbulk_send_dat(buffer,len<<9)){
    	deg("rbc_write_lba err2\r\n");
        return 0;
    }

    if (!epbulk_receive_dat((u8 *)&tpbulk_block,0))
    {
    	deg("rbc_write_lba err3\r\n");
    	return 0;
    }

    return 1;
}


s32 udisk_actech(void)
{
	//udisk > 1M
	if(diskhal.cap > 1*2048){
		return true;	
	}
	else{
		return false;	
	}
}
s32 udisk_state(void)
{
	return 	diskhal.online;
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
DISKHAL *enum_mass_dev(void)
{
	deg("*******************************\n");
	deg("[ enum_mass_dev ]\n");
	deg("*******************************\n");
	if(!spc_inquiry(diskhal.inquiry)){
		debg("disk: <inquiry err>\n");
		return NULL;	
	}
	
	u32 i;
	for(i = 0; i < 100; i++){
		if(!spc_test_unit_rdy(&diskhal.online)){
			debg("disk: <check state err>\n");
			return NULL;			
		}
		if(diskhal.online){
			break;	
		}
		if(!spc_request_sense()){
			debg("disk: <sense err>\n");
		}			
	}


	if(!rbc_read_capacity(&diskhal.cap, &diskhal.slen)){
		debg("disk: <capacity err>\n");
		return NULL;		
	}

	
	debg("disk:<cap:%dMB, slen:%d, online:%X>\n",diskhal.cap/2048, diskhal.slen,diskhal.online);
	diskhal.fp_udisk_rd = rbc_read_lba;
	diskhal.fp_udisk_wr = rbc_write_lba;
	diskhal.fp_udisk_actech = udisk_actech;
	diskhal.fp_udisk_state = udisk_state;

	/*
	u8 testbuf[2048];
	my_memset(testbuf, 0x55, 1024);
	rbc_write_lba(testbuf, 0, 2);
	my_memset(testbuf, 0xaa, 1024);
	rbc_write_lba(testbuf, 2, 2);
	
	rbc_read_lba(testbuf, 0, 4);
	debgbuf(testbuf, 2048);
	rbc_read_lba(testbuf, 4, 4);
	debgbuf(testbuf, 2048);	
	*/
	return &diskhal;;
}
