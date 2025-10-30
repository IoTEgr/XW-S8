/*
 * usb_host_tpbulk.c
 *
 *  Created on: 2013Äê10ÔÂ25ÈÕ
 *      Author: jingcunxu
 */

#include "../inc/hal.h"
#define uvc_rx_fifo		_uvc_txfifo_
UVCHAL uvchal;				//uvc stream  function
UVC_FSTACK uvc_fstack;		//uvc paylod stack
ISOSTACK isostack;

/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u32 api_uvc_fcnt_get(void)
{
	u32 cnt = uvc_fstack.jUcnt;
	uvc_fstack.jUcnt = 0;
	return cnt;
}




/*
 * buff 管理：
 * 3个buff: 备帧，好帧/坏帧，新帧
 * 1、填充帧完成，标记成新帧
 * 2、解码完成后，标记成好帧/坏帧
 * 		解码完成后如果是好帧，以前的备帧释放
 * 		解码完成后如果是坏帧，以前的备帧保留，坏帧释放
 **/
//#define _UVC_JPG_MAXSIZE_ (100 * 1024) 
#define _UVC_JPG_MAXSIZE_ (8) 

enum 
{
	NULL_FRAME = 0, //can use for filling jpg
	NEW_FRAME, 		//filling jpg 
	RDY_FRAME, 		//fill jpg ready
	DCD_FRAME, 		//jpg decodding 	
	BAK_FRAME, 		//use for insert frame
};	
typedef struct
{
	u8 sta; 
	u8* buff;	
	u32 size;
}UVC_CACHE;
//static u8 uvcjbuf[3][_UVC_JPG_MAXSIZE_]; 		//cache hal
static u8** uvcjbuf;
static UVC_CACHE uvccache[3];			//cache buff

static UVC_CACHE *uvcfillhal, *uvcbackhal, *uvcdcdhal;
static u8 (*fp11_epout_isr_hal)(void);

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
void huvc_cache_statck_init(void)
{
	u32 i;
	for(i = 0; i < 3; i++){
		uvccache[i].sta = NULL_FRAME;
		uvccache[i].buff = uvcjbuf[i];
	}
	uvcfillhal = NULL;
	uvcbackhal = NULL;
	uvcdcdhal  = NULL;
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
UVC_CACHE *huvc_get_jfillhal(void)
{
	u32 i;
	UVC_CACHE *hal = NULL; 
	
	//normal case
	for(i = 0; i < 3; i++){
		if(uvccache[i].sta == NULL_FRAME){
			hal = &uvccache[i];
			return hal;
		}
	}
	
	//this case is  fCSI < fUVC or mjpB ecode block 
	for(i = 0; i < 3; i++){
		if(uvccache[i].sta == RDY_FRAME){
			hal = &uvccache[i];
			//debg("warning : uvc frame drop\n");
			return hal;
		}		
	}
	return hal;
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
UVC_CACHE  *huvc_get_jdcodhal(void)
{
	u32 i;
	UVC_CACHE *hal = NULL;
	
	//normal case
	for(i = 0; i < 3; i++){
		if(uvccache[i].sta == RDY_FRAME){
			hal = &uvccache[i];
			return hal;
		}
	}
	
	//this case is  fCSI > fUVC or mjpB ecode block 
	for(i = 0; i < 3; i++){
		if(uvccache[i].sta == BAK_FRAME){
			//debg("f");
			hal = &uvccache[i];
			return hal;
		}
	}
	return hal;
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
u8 *huvc_jfill_loadadr(void)
{
	UVC_CACHE *hal = huvc_get_jfillhal();
	if(hal == NULL){
		debg("malloc uvc jfillhal err\n");
		LogPrint("malloc uvc jfillhal err",0);
		while(1);	
	}
	hal-> sta  = NEW_FRAME; 
	hal-> size = 0;
	uvcfillhal = hal;
	return hal->buff;	
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
bool huvc_jdcod_load(u8 **p, u32 *len)
{
	UVC_CACHE *hal  = huvc_get_jdcodhal();
	if(hal == NULL){
		return false;
	}
	uvcdcdhal = hal;
	hal-> sta  = DCD_FRAME; 
	
	*p = hal->buff;
	*len = hal->size;
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
void huvc_jfill_down(u8 *buf, u32 size)
{
	if(uvcfillhal == NULL){
		debg("ERR: <uvcfillhal = NULL>\n");exception_trigger();while(1);	
	}
	if(uvcfillhal->buff != buf){
		debg("ERR: <jpg buff not match\n");exception_trigger();while(1);
	}
	if(size == 0){
		debg("ERR: <jpg size = 0\n");exception_trigger();while(1);
	}
	uvcfillhal->sta = RDY_FRAME; //NULL_FRAME/RDY_FRAME ->  NEW_FRAME -> RDY_FRAME
	uvcfillhal->size = size;
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
void huvc_dcod_down(u8 err)
{
	if(uvcdcdhal == NULL){
		//debg("waring: <uvcdcdhal = NULL>\n");return;
	}
	
	if(err){
		uvcdcdhal -> sta = NULL_FRAME;	//decode err: DCD_FRAME-> NULL_FRAME
		uvcbackhal = uvcbackhal;		//uvcbackhal remaining
		uvcdcdhal = NULL;
	}else{
		uvcbackhal ->sta = NULL_FRAME; //decode ok: BAK_FRAME-> NULL_FRAME, 
		uvcbackhal = uvcdcdhal;			
		uvcbackhal ->sta = BAK_FRAME; //DCD_FRAME -> BAK_FRAME
		uvcdcdhal = NULL;
	}
}

void hal_huvc_strm_init(void)
{
	huvc_cache_statck_init();
}

#if 0
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void _c_stack_setup(C_STACK *c, QDT_BUF que[], u32 que_dep, u8 *linkbuf, u32 linksize)
{
	u32 i;	
	if((c == NULL) || (que == NULL) || (!que_dep)){
		debg("stack: <parameter err>\n");exception_trigger();while(1);	
	}
	//c->que = que;
	
	//init que item
	for(i = 0; i < que_dep; i++){
		que[i].sta = 0;
		que[i].pbuf = NULL;
		que[i].buflen =  0;
		que[i].last =(i == 0)?&que[que_dep-1] : &que[i-1];
		que[i].next = ((i + 1) == que_dep)?&que[0] : &que[i+1];
	}
	

	//init p_qdt,g_qdt 
	c->p_qdt = c->g_qdt = &que[0];
	
	//init cache information
	c->c_inf.semf = 0;
	c->c_inf.pcache = linkbuf; //ç»™å®šçš„BUUFF
	c->c_inf.cachelen = linksize; //ç»™å®šçš„é•¿åº?
	c->c_inf.sf = linkbuf;
	c->c_inf.ef = &linkbuf[linksize];
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8* _cache_pre_malloc(C_STACK *c, u32 mlen)
{
	u8 *malloc_buf ;
	
	QDT_BUF *p = c -> p_qdt->last;
	QDT_BUF *g = c -> g_qdt;
	
	volatile u32 sf = (u32)c->c_inf.sf;
	volatile u32 ef = (u32)c->c_inf.ef;
	
	while(1){
		//first macllo
		if((p->pbuf == NULL) && (g->pbuf == NULL)){
			malloc_buf = (u8*)sf;
			break;
		}
		
		//not first macllo
		u32 pt = (u32)(p ->pbuf + p ->buflen);
		u32 gt = (u32)(g ->pbuf);
		
		if(pt > gt){
			if((ef - pt) >= mlen){
				malloc_buf = (u8*)pt;
				break;
			}else{
				if((gt - sf) > mlen){
					malloc_buf = (u8*)sf;
					break;
				}else{
					//debg("gt-sf:%x,%x,%x\n",gt-sf, mlen,g);
					malloc_buf = NULL;
					break;
				}	
			}
		}
		if(pt < gt){
			if((gt - pt) > mlen){
				malloc_buf =  (u8*)pt;	
				break;
			}else{
				//debg("gt-pt:%x,%x,%x\n",gt-pt, mlen,g);
				malloc_buf = NULL;
				break;
			}	
		}
		
		//cache full
		if(pt == gt){
			//debg("gt=pf\n");
			malloc_buf = NULL;
			break;
		}	
	}
	if(malloc_buf == NULL)
		return NULL;
	
	c -> p_qdt->sta = 0;
	c -> p_qdt->pbuf = malloc_buf;
	c -> p_qdt->buflen = mlen;
	c -> p_qdt = c -> p_qdt->next;
	c-> c_inf.semf++;
	//if(c-> c_inf.semf > 2)
	//	debg("uvc: <-sem:%x>\n",c-> c_inf.semf);
	return malloc_buf;
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool _cache_put(C_STACK *c, u8 *p, u32 len)
{
	//#define pt  c->p_qdt
	if(c-> c_inf.semf == 0){
		//debg("uvc: <cache_put err>\n");//while(1);
		return false;
	}
	QDT_BUF *pt = c->p_qdt;
	
	u8 i;
	for(i = 0; i < c-> c_inf.semf; i++)
		pt = pt->last;
	//sem--
//	if(c-> c_inf.semf >2)
//		debg("uvc: <+sem:%x>\n",c-> c_inf.semf);
	c-> c_inf.semf--;
	
	if((pt -> pbuf != NULL) && (pt->buflen != 0) && (pt->sta == 0)){
		pt-> sta = true;
		//pt -> pbuf = p;
		pt -> buflen = len;	
		return true;
	}else{
		//debg("qdt: <list full>\n");while(1);
		return false;	
	}
	//#undef pt
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool _cache_get(C_STACK *c, u8 **p, u32 *len)
{
	#define gt  c->g_qdt

	
	if((gt->pbuf != NULL) && (gt->buflen != 0)&& (gt->sta == true)){
		*p = gt ->pbuf;	
		*len = gt->buflen;
		
		//gt-> sta = 0;
		//gt-> pbuf = NULL;
		//gt-> buflen = 0;
		//gt = gt->next;
		//debg("V");
		return true;
		
	}else{
		//debg("x");
		return false;	
	}
	
	#undef gt
}
bool _cache_free(C_STACK *c)
{
	#define gt  c->g_qdt
	if((gt->pbuf != NULL) && (gt->buflen != 0)&& (gt->sta == true)){
		//if((gt->pbuf == p) && (gt->buflen == len))
		{
			gt-> sta = 0;
			gt-> pbuf = NULL;
			gt-> buflen = 0;
			gt = gt->next;		
			return true;
		}//else{
			//debg("free obj is not match\n");	
		//}
	}else{
		debg("free obj is null\n");
		
	}
	return false;
	#undef gt
}



#define UVC_QDT_ITEM   128
static QDT_BUF	uvc_qdt[UVC_QDT_ITEM];
static C_STACK uvc_cstack;
static u8 *uvc_ubuf;
static u32 uvc_ulen;
static u8  uvc_udrop;
/*******************************************************************************
* Function Name  : sensor_ClockInit
* Description    : set sensor clock 
* Input          : u32SenClk: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
void hal_huvc_strm_init(u8 *linkbuf, u32 linksize)
{
	_c_stack_setup(&uvc_cstack, uvc_qdt, UVC_QDT_ITEM, linkbuf, linksize);
	uvc_ulen = linksize/2;
	uvc_udrop = false;
	//debg("linkbuf:%x\n",linkbuf);
}

/*******************************************************************************
* Function Name  : sensor_ClockInit
* Description    : set sensor clock 
* Input          : u32SenClk: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
u8* hal_huvc_adrreload_hal(void)
{
	u32 jmallolen = (uvc_ulen > 50 * 1024)? (uvc_ulen* 2) : (50 *1024*2);
	u8* p = _cache_pre_malloc(&uvc_cstack, jmallolen);
	if(p != NULL){
		uvc_ubuf = p;
		uvc_udrop = false;
	}else{
		//debg("U:%x,%x,%x\n",BACK_JPFLG & (3 << 16),hal_mjpB_decoding(),hal_mjpB_ecoding());	 
		//ax32xx_mpegPrint();
		uvc_ubuf =uvc_ubuf; //use last
		uvc_udrop = true;
	}
	//debg("m:%x\n",uvc_ubuf);
	return uvc_ubuf;
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_huvc_frame_hal(u8 *p, u32 len)
{
	uvc_ubuf = p;
	uvc_ulen = len;
	
	//debg("p:%x,%x\n",p,len);
	if(!uvc_udrop)
		_cache_put(&uvc_cstack,uvc_ubuf, uvc_ulen);

}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_usensor_frame_free(void)
{
	return _cache_free(&uvc_cstack);

}
#endif
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool usb_usensor_frame_read(u8 **p, u32 *len, u32 *id)
{
	u8 *pbuf;
	u32 plen;
	//if(!_cache_get(&uvc_cstack, &pbuf, &plen)){
	if(!huvc_jdcod_load(&pbuf, &plen)){
		return false;	
	}
	ax32xx_sysDcacheFlush((u32)pbuf,16);
	if((!plen) || (pbuf == NULL)){
		debg("uj: <len = 0>\n");
		//usb_usensor_frame_free();
		huvc_dcod_down(1);
		return false;	
	}
	if((pbuf[0] != 0xff) || (pbuf[1] != 0xd8)){
		debg("uj: <header err>:%x,%x\n",pbuf,plen);	
		//usb_usensor_frame_free();
		huvc_dcod_down(1);
		return false;
	}
	*p = pbuf;	
	*len = 	plen;
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
BOOL iso_receive_dat(u8 *buf ,u32 *len)
{
    u8  intr;
    u32 rLen;

	_U20E2RADR_ = (u32)uvc_rx_fifo;;	
	USB20_Index = ISOTX_EP;							//select use ep
	USB20_RxType = (TT_ISOCHRONOUS|isostack.episoin);	//select send ep
	USB20_FAddr = isostack.addr;						//select send addr
	
    while (1)
    {
    	USB20_OutCSR1 |= 0x20;//request pkt,send a in packet
        while (1){
            intr = USB20_OutCSR1;
            if (intr & 0x04){
            	USB20_OutCSR1=0;
            	deg("cbw_rx-err2\r\n");
				*len = 0;
                return false;
            }
            if (intr & 0x40){
            	deg("cbw_rx-err3\r\n");
                return false;
            }

            if (intr & 0x01)    // Get one packet
            {
                rLen = (USB20_OutCount2<<8)+USB20_OutCount1;
				if(rLen > __UVC_PKG_SZ__){debg("husb isopkg > 3072");exception_trigger();while(1);};
#define USE_ISO_DMA		1
#if USE_ISO_DMA
				if(rLen){
					//dma_memkick(buf,uvc_rx_fifo,rLen);
					//dma_memdown();
					//debg(":%x\n",buf);
					ax32xx_mcpy_gram2ufifo(buf, uvc_rx_fifo,rLen);
				}
#else
				u32 i;
                for(i=0; i<rLen; i++){
					buf[i]=USB_FIFO2;
				}
				//debg(":%x\n",rLen);
#endif
				*len = rLen; 
				USB20_OutCSR1 = 0x30;//flush the next packet to be read from the endpoint Rx FIFO.
				return 1;
				
            }
				//USB20E2RADR = (u32)uvc_rx_fifo;;
            	//USB20_OutCSR1 = 0x20;//flush the next packet to be read from the endpoint Rx FIFO.
        }
    }
#undef USE_ISO_DMA
}

BOOL iso_recive_payload_isr(u8 *buf ,u32 *len)
{
	bool ret = true;
	while (1){
		u8 intr = USB20_OutCSR1;
		if (intr & 0x04){
			USB20_OutCSR1=0;
			deg("cbw_rx-err2\r\n");
			*len = 0;
			ret = false;
			break;
		}
		if (intr & 0x40){
			deg("cbw_rx-err3\r\n");
			*len = 0;
			ret = false;
			break;
		}

		if (intr & 0x01)    // Get one packet
		{
			u32 rLen = (USB20_OutCount2<<8)+USB20_OutCount1;
			//if(rLen > 1024){debg("xxxx");while(1);}
#define USE_ISO_DMA		1
#if USE_ISO_DMA
			if(rLen){
				ax32xx_mcpy_gram2ufifo(buf, uvc_rx_fifo,rLen);
			}
#else
			u32 i;
			for(i=0; i<rLen; i++){
				buf[i]=USB_FIFO2;
			}
#endif
			*len = rLen; 
			break;
		}
	}	

		
	_U20E2RADR_ = (u32)uvc_rx_fifo;;	
	USB20_Index = ISOTX_EP;							//select use ep
	USB20_RxType = (TT_ISOCHRONOUS|isostack.episoin);	//select send ep
	USB20_FAddr = isostack.addr;						//select send addr
	USB20_OutCSR1 = 0x30;//flush the next packet to be read from the endpoint Rx FIFO.
#undef USE_ISO_DMA	
    return ret;	
}

/*
void uvc_receive_jframe(u8 *jbuf ,u32 *jlen)
{
	
	static u8 jtglsyc;
	
	u8 payload[3072];
	u32 paylen;
	iso_receive_dat(payload ,&paylen);
	if(paylen){
		continue;	
	}
	if(payload[0] != 0x0c){
		continue;	
	}
	if(!fsynflg){
		if(payload[1] & BIT(0) != jtglsyc){
			continue;	
		}		
	}
	if(payload[1] & BIT(0) != jtglsyc){
		continue;	
	}
	memcpy(jbuf, &payload[12] , paylen-= 12);
	jbuf += paylen-= 12;
	if(payload[1] & BIT(1)){
		break;
	}
*/
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
void uvc_fstatck_init(void)
{
	memset((u8 *)&uvc_fstack, 0, sizeof(uvc_fstack));
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
#define _UVC_HEADER_	12	
enum
{
	ASYN = 0,
	SYNF,
	SYNC,
};
void uvc_recevie_jframe_isr(void)
{
	#define NEXT_TGL  ((payload[1] & BIT(0)) ^ 1)
	#define CURT_TGL  (payload[1] & BIT(0))
	
	u8 *payload = _uvc_payld_[0];
	u32 paylen;	
	iso_recive_payload_isr(payload,&paylen);

	//err paylen
	if(!paylen){
		return;	
	}
	
	//err header
	if(payload[0] ^ _UVC_HEADER_){
		if(uvc_fstack.jbuf != NULL){
			huvc_jfill_down(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);//hal_huvc_frame_hal(uvc_fstack.jbuf-uvc_fstack.jlen, uvc_fstack.jlen);	
		}
		uvc_fstack.usta = ASYN;
		//uvc_fstatck_init();
		debg("warning: <paylod header != 0x0c>\n");
		return ;	
	}
	
	//err pload
	if(payload[1] & (BIT(6) | BIT(5))){
		if(uvc_fstack.jbuf != NULL){
			huvc_jfill_down(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);//hal_huvc_frame_hal(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);	
		}
		
		//uvc_fstatck_init();
		uvc_fstack.usta = ASYN;
		debg("warning: <bend frame >\n");
		return;
	}	
	
	//strart sync
	if(uvc_fstack.usta == ASYN){
		uvc_fstack.utgl = NEXT_TGL;
		uvc_fstack.usta = SYNF;
		uvc_fstack.jbuf = NULL;
		uvc_fstack.jlen = 0;
		//debg("fsync=0\n");
	}
	//synced
	if(uvc_fstack.usta == SYNF){
		if(uvc_fstack.utgl == CURT_TGL){
			uvc_fstack.usta = SYNC;
			
			//check jpg header
			if(paylen > _UVC_HEADER_){
				if((payload[_UVC_HEADER_] != 0xff) ||(payload[_UVC_HEADER_+1] != 0xd8)){
						debg("warning: <1st jpg header:%x,%x,\n",payload[_UVC_HEADER_],payload[_UVC_HEADER_+1]);
						LogPrint("1st jpg header err",0);
						uvc_fstack.usta = ASYN;
						return;
				}
			}
			
			//sync first jpg payload & malloc  buff
			uvc_fstack.jbuf = huvc_jfill_loadadr();//hal_huvc_adrreload_hal();
			uvc_fstack.jlen = 0;
		}
	}
	if(uvc_fstack.usta == SYNC){
		
		if((uvc_fstack.utgl != CURT_TGL)){
			
			//frame togle
			huvc_jfill_down(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);//hal_huvc_frame_hal(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);
			
			//check jpg header
			if(paylen > _UVC_HEADER_){
				if((payload[_UVC_HEADER_] != 0xff) ||(payload[_UVC_HEADER_+1] != 0xd8)){
						debg("warning: <nst jpg header:%x,%x,\n",payload[_UVC_HEADER_],payload[_UVC_HEADER_+1]);	
						LogPrint("nst jpg header err",0);
						uvc_fstack.usta = ASYN;
						return;
				}
			}
			
			
			//malloc next buff
			uvc_fstack.jbuf = huvc_jfill_loadadr();//hal_huvc_adrreload_hal();//uvc_cache_pre_malloc(100*1024);
			uvc_fstack.jlen = 0;
			
			uvc_fstack.usta = SYNC;
			uvc_fstack.utgl = CURT_TGL;
			uvc_fstack.jUcnt++;
		}		
	}
	if((uvc_fstack.usta == SYNC) && (uvc_fstack.jbuf) && (paylen >= _UVC_HEADER_)){
		ax32xx_mcpy1_sdram2gram_2(uvc_fstack.jbuf, &payload[_UVC_HEADER_] , paylen- _UVC_HEADER_);
		uvc_fstack.jbuf += paylen- _UVC_HEADER_;
		uvc_fstack.jlen += paylen- _UVC_HEADER_;
	}
	if(uvc_fstack.jlen > _UVC_JPG_MAXSIZE_){
		debg("ERR: <uvc_fstack.jlen overflow>\n");
		//LogPrint("uvc satck err",0);
		//exception_trigger();
		//while(1);	
		usb_host_remove();
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

bool  uvc_host_init(u8 addr, u8 epin, u8 epout, u16 payload, u8 interval)
{
	u32 UVC_RX_FIFO = (u32)&uvc_rx_fifo ;
	
	if((UVC_RX_FIFO < ufifo_s) || (UVC_RX_FIFO > ufifo_e)){
		debg("uvc_rx_fifo set err\n");
		return false;
	}

	isostack.addr = addr;
	isostack.episoin= epin & 0x7f;
	isostack.episoout = epout;
	isostack.payload = payload;
	isostack.interval = interval;
	if(payload == 0) 
		return false;
	debg(".addr :%x\n",isostack.addr);
	debg(".episoin:%x\n",isostack.episoin);
	debg(".episoout:%x\n",isostack.episoout);
	debg(".payload:%x\n",isostack.payload);
	debg(".interval:%x\n",isostack.interval);
	//debg("ep:in/out%x,%x\n",mscstack.epbulkin,mscstack.epbulkout);


	(USB20_Index)= ISOTX_EP;//mscstack.epbulkin;//EP_BULKTX;
	(USB20_InCSR1)=BIT(6) | BIT(3);
	(USB20_InCSR2)=0;
	(USB20_InMaxPl)=0x00;
	(USB20_InMaxPh)=0x0e;//
	(USB20_OutCSR1)=BIT(7) | BIT(4);
	(USB20_OutCSR2) = 0;
	(USB20_OutMaxPl)=0x00;
	(USB20_OutMaxPh)=0x0e;//

	(USB20_Index)= ISORX_EP;//mscstack.epbulkout;//EP_BULKTX;
	(USB20_InCSR1)=BIT(6) | BIT(3);
	(USB20_InCSR2)=0;
	(USB20_InMaxPl)=0x00;
	(USB20_InMaxPh)=0x0e;//
	(USB20_OutCSR1)=BIT(7) | BIT(4);
	(USB20_OutCSR2) = 0;
	(USB20_OutMaxPl)=0x00;
	(USB20_OutMaxPh)=0x0e;//
	//jtglsyc = -1;
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
u8 isobuf[1024] __attribute__ ((section(".sram_comm"))); 
UVCHAL *uvc_actech_as_usensor(void)
{
	debg("uvc20_actech_as_usensor\n");
	uvchal.fp_iso_receive_dat = iso_receive_dat;
	//register to iso out isr
	fp_epout_isr_hal =(u8(*)(void))uvc_recevie_jframe_isr; 

	//kick
	_U20E2RADR_ = (u32)uvc_rx_fifo;;	
	USB20_Index = ISOTX_EP;							//select use ep
	USB20_RxType = (TT_ISOCHRONOUS|isostack.episoin);	//select send ep
	USB20_FAddr = isostack.addr;						//select send addr	

	uvc_fstatck_init();
	 
	//kick start
	USB20_IntrOutel |= BIT(2);						//isr en
	USB20_OutCSR1 |= 0x20;//request pkt,send a in packet	
	return &uvchal;
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
UVCHAL *uvc11_actech_as_usensor(void)
{
	debg("uvc11_actech_as_usensor\n");
	uvchal.fp_iso_receive_dat = iso_receive_dat11;
	//register to iso out isr
	fp11_epout_isr_hal = (u8 (*)(void))uvc_recevie_jframe_isr11; 

	//kick
	USB11E3RADR = (u32)512;
	usb11_registers_write(Index,ISOTX_EP);//USB_Index = ISOTX_EP;							//select use ep
	usb11_registers_write(RxType,(TT_ISOCHRONOUS|isostack.episoin));//USB_RxType = (TT_ISOCHRONOUS|isostack.episoin);	//select send ep
	usb11_registers_write(FAddr, isostack.addr);//USB_FAddr = isostack.addr;						//select send addr	

	uvc_fstatck_init();
	 
	//kick start
	usb11_registers_write(IntrRx1E,BIT(2)|BIT(3));//USB_IntrOutel |= BIT(2);						//isr en
	usb11_registers_write(RxCSR1,0x20);//USB_OutCSR1 |= 0x20;//request pkt,send a in packet	
	
	return &uvchal;
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

bool  uvc_host11_init(u8 addr, u8 epin, u8 epout, u16 payload, u8 interval)
{
	/*u32 UVC_RX_FIFO = (u32)&uvc_rx_fifo ;
	
	if((UVC_RX_FIFO < ufifo_s) || (UVC_RX_FIFO > ufifo_e)){
		debg("uvc_rx_fifo set err\n");
		return false;
	}
*/
	isostack.addr = addr;
	isostack.episoin= epin & 0x7f;
	isostack.episoout = epout;
	isostack.payload = payload;
	isostack.interval = interval;
	if(payload == 0) 
		return false;
	debg(".addr :%x\n",isostack.addr);
	debg(".episoin:%x\n",isostack.episoin);
	debg(".episoout:%x\n",isostack.episoout);
	debg(".payload:%x\n",isostack.payload);
	debg(".interval:%x\n",isostack.interval);
	//debg("ep:in/out%x,%x\n",mscstack.epbulkin,mscstack.epbulkout);

	usb11_registers_write(Index,ISOTX_EP);//(USB_Index)= ISOTX_EP;//mscstack.epbulkin;//EP_BULKTX;
	usb11_registers_write(TxCSR1,BIT(6)|BIT(3));//(USB_InCSR1)=BIT(6) | BIT(3);
	usb11_registers_write(TxCSR2,0x0);//(USB_InCSR2)=0;
	usb11_registers_write(TxMaxP,250);//(USB_InMaxPl)=0x00;
	//(USB_InMaxPh)=0x0e;//
	usb11_registers_write(RxCSR1,BIT(7) | BIT(4));//(USB_OutCSR1)=BIT(7) | BIT(4);
	usb11_registers_write(RxCSR2,0x0);//(USB_OutCSR2) = 0;
	usb11_registers_write(RxMaxP,250);//(USB_OutMaxPl)=0x00;
	//(USB_OutMaxPh)=0x0e;//

	usb11_registers_write(Index,ISORX_EP);//(USB_Index)= ISORX_EP;//mscstack.epbulkout;//EP_BULKTX;
	usb11_registers_write(TxCSR1,BIT(6)|BIT(3));//(USB_InCSR1)=BIT(6) | BIT(3);
	usb11_registers_write(TxCSR2,0x0);//(USB_InCSR2)=0;
	usb11_registers_write(TxMaxP,250);//(USB_InMaxPl)=0x00;
	//(USB_InMaxPh)=0x0e;//
	usb11_registers_write(RxCSR1,BIT(7) | BIT(4));//(USB_OutCSR1)=BIT(7) | BIT(4);
	usb11_registers_write(RxCSR2,0x0);//(USB_OutCSR2) = 0;
	usb11_registers_write(RxMaxP,250);//(USB_OutMaxPl)=0x00;
	//(USB_OutMaxPh)=0x0e;//
	//jtglsyc = -1;
	
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
BOOL iso_receive_dat11(u8 *buf ,u32 *len)
{
    u8  intr;
    u32 rLen;
	USB11E3RADR = (u32)512;	
	usb11_registers_write(Index,ISOTX_EP);				//select use ep
	usb11_registers_write(RxType,(TT_ISOCHRONOUS|isostack.episoin));	//select send ep
	usb11_registers_write(FAddr, isostack.addr);						//select send addr
	usb11_registers_write(RxCSR1,0x30);//flush the next packet to be read from the endpoint Rx FIFO.

    while (1)
    {
		usb11_registers_write(RxCSR1,usb11_registers_read(RxCSR1)|0x20);//request pkt,send a in packet
        while (1){
            intr = usb11_registers_read(RxCSR1);
            if (intr & 0x04){
            	usb11_registers_write(RxCSR1,0x0);
            	deg("cbw_rx-err2\r\n");
				*len = 0;
                return false;
            }
            if (intr & 0x40){
            	deg("cbw_rx-err3\r\n");
                return false;
            }

            if (intr & 0x01)    // Get one packet
            {
                rLen = (usb11_registers_read(RxCount2)<<8) + usb11_registers_read(RxCount1);
				if(rLen > __UVC_PKG_SZ__){debg("husb isopkg > 3072");exception_trigger();while(1);};
#define USE_ISO_DMA		1
#if USE_ISO_DMA
				if(rLen){
					//debg("*");
					usb_memcpy(0,(u32)isobuf,(u32)USB11E3RADR,rLen);
					//dma_memkick(buf,uvc_rx_fifo,rLen);
					//dma_memdown();
					//debg(":%x\n",buf);
					ax32xx_mcpy_gram2ufifo(buf, isobuf,rLen);
				}
#else
				u32 i;
                for(i=0; i<rLen; i++){
					buf[i]=usb11_registers_read(FIFO3);
				}
				//debg(":%x\n",rLen);
#endif
				*len = rLen; 
				usb11_registers_write(RxCSR1,0x30);//flush the next packet to be read from the endpoint Rx FIFO.
				return 1;
				
            }
				//USB20E2RADR = (u32)uvc_rx_fifo;;
            	//USB20_OutCSR1 = 0x20;//flush the next packet to be read from the endpoint Rx FIFO.
        }
    }
#undef USE_ISO_DMA
}

BOOL iso_recive_payload_isr11(u8 *buf ,u32 *len)
{
	bool ret = true;
	while (1){
		u8 intr = usb11_registers_read(RxCSR1);
		if (intr & 0x04){
			usb11_registers_write(RxCSR1,0x0);
			deg("cbw_rx-err2\r\n");
			*len = 0;
			ret = false;
			break;
		}
		if (intr & 0x40){
			deg("cbw_rx-err3\r\n");
			*len = 0;
			ret = false;
			break;
		}

		if (intr & 0x01)    // Get one packet
		{
			u32 rLen = (usb11_registers_read(RxCount2)<<8) + usb11_registers_read(RxCount1);
			//if(rLen > 1024){debg("xxxx");while(1);}
#define USE_ISO_DMA		1
#if USE_ISO_DMA
			if(rLen){
				//debg("*");
				usb_memcpy(0,(u32)buf,(u32)USB11E3RADR,rLen);
				
				//debg("isobuf:  ");
				//debgbuf(isobuf,12);
			/*if(buf[1]==0x8D)
					debg("*");
				else if(buf[1]==0x8c)
					debg("+");*/
				//ax32xx_mcpy1_sdram2gram(buf, isobuf,rLen);
			}
#else
			u32 i;
			for(i=0; i<rLen; i++){
				buf[i]=usb11_registers_read(FIFO3);
					
			}
			
					//debg("%x ", buf[1]);
#endif
			*len = rLen; 
			break;
		}
	}	

		
	USB11E3RADR = (u32)512;	
	usb11_registers_write(Index,ISOTX_EP);				//select use ep
	usb11_registers_write(RxType,(TT_ISOCHRONOUS|isostack.episoin));	//select send ep
	usb11_registers_write(FAddr, isostack.addr);						//select send addr
	usb11_registers_write(RxCSR1,0x30);//flush the next packet to be read from the endpoint Rx FIFO.
#undef USE_ISO_DMA	
    return ret;	
}
void uvc_recevie_jframe_isr11(void)
{
	#define NEXT_TGL  ((payload[1] & BIT(0)) ^ 1)
	#define CURT_TGL  (payload[1] & BIT(0))
	
	u8 *payload = _uvc_payld_[0];
	u32 paylen;	
	iso_recive_payload_isr11(payload,&paylen);
	//debg("payload: ");
	//debgbuf(payload,12);
	//err paylen
	if(!paylen){
		return;	
	}
	//if(payload[1]==0x8d)
		
	//debgbuf(payload,paylen);
	//return;
	//err header
	if(payload[0] ^ _UVC_HEADER_){
		if(uvc_fstack.jbuf != NULL){
			huvc_jfill_down(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);//hal_huvc_frame_hal(uvc_fstack.jbuf-uvc_fstack.jlen, uvc_fstack.jlen);	
		}
		uvc_fstack.usta = ASYN;
		//uvc_fstatck_init();
		debg("warning: USB11<paylod header != 0x0c>\n");
		return ;	
	}
	
	//err pload
	if(payload[1] & (BIT(6) | BIT(5))){
		if(uvc_fstack.jbuf != NULL){
			huvc_jfill_down(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);//hal_huvc_frame_hal(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);	
		}
		
		//uvc_fstatck_init();
		uvc_fstack.usta = ASYN;
		debg("warning: <bend frame >\n");
		return;
	}	
	
	//strart sync
	if(uvc_fstack.usta == ASYN){
		uvc_fstack.utgl = NEXT_TGL;
		uvc_fstack.usta = SYNF;
		uvc_fstack.jbuf = NULL;
		uvc_fstack.jlen = 0;
		//debg("fsync=0\n");
	}
	//synced
	if(uvc_fstack.usta == SYNF){
		if(uvc_fstack.utgl == CURT_TGL){
			uvc_fstack.usta = SYNC;
			
			//check jpg header
			if(paylen > _UVC_HEADER_){
				if((payload[_UVC_HEADER_] != 0xff) ||(payload[_UVC_HEADER_+1] != 0xd8)){
						debg("warning: <1st jpg header:%x,%x,\n",payload[_UVC_HEADER_],payload[_UVC_HEADER_+1]);
						LogPrint("1st jpg header err",0);
						uvc_fstack.usta = ASYN;
						return;
				}
			}
			
			//sync first jpg payload & malloc  buff
			uvc_fstack.jbuf = huvc_jfill_loadadr();//hal_huvc_adrreload_hal();
			uvc_fstack.jlen = 0;
		}
	}
	if(uvc_fstack.usta == SYNC){
		
		if((uvc_fstack.utgl != CURT_TGL)){
			
			//frame togle
			//debg("-");
			huvc_jfill_down(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);//hal_huvc_frame_hal(uvc_fstack.jbuf - uvc_fstack.jlen, uvc_fstack.jlen);
			
			//check jpg header
			if(paylen > _UVC_HEADER_){
				if((payload[_UVC_HEADER_] != 0xff) ||(payload[_UVC_HEADER_+1] != 0xd8)){
						debg("warning: <nst jpg header:%x,%x,\n",payload[_UVC_HEADER_],payload[_UVC_HEADER_+1]);	
						LogPrint("nst jpg header err",0);
						uvc_fstack.usta = ASYN;
						return;
				}
			}
			
			
			//malloc next buff
			uvc_fstack.jbuf = huvc_jfill_loadadr();//hal_huvc_adrreload_hal();//uvc_cache_pre_malloc(100*1024);
			uvc_fstack.jlen = 0;
			
			uvc_fstack.usta = SYNC;
			uvc_fstack.utgl = CURT_TGL;
			uvc_fstack.jUcnt++;
		}		
	}
	
	if((uvc_fstack.usta == SYNC) && (uvc_fstack.jbuf) && (paylen >= _UVC_HEADER_)){
		ax32xx_mcpy1_sdram2gram_2(uvc_fstack.jbuf, &payload[_UVC_HEADER_] , paylen- _UVC_HEADER_);
		uvc_fstack.jbuf += paylen- _UVC_HEADER_;
		uvc_fstack.jlen += paylen- _UVC_HEADER_;
	}
	if(uvc_fstack.jlen > _UVC_JPG_MAXSIZE_){
		debg("ERR: <uvc_fstack.jlen overflow>\n");
		//LogPrint("uvc satck err",0);
		//exception_trigger();
		//while(1);	
		usb11_host_remove();
	}
}
