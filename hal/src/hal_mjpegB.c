/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                  THE APPOTECH HAL
   **** **                 MJPEG HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_mjpeg.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN MJPEG HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"

typedef struct MJPEG_ENC_OP_B
{
	void *pusensor_adpt;
	u8 type;
	u8 drop;
	
#if HAL_CFG_MJPEG_QULITY_AUTO>0
	s8 qulity;
    u8 q_cnt;
	u8 q_auto;
	s8 q_dir;
	u32 q_csize;
#endif
	u16 usensor_width;
	u16 usensor_height;
    u16 mjpeg_width;
	u16 mjpeg_height;
	u32 jfcnt;
	//must fix
    u32 ybuffer;
	u32 uvbuffer;
	u32 mjpbuf;
	u32 mjpsize;

	u32 curBuffer;
	u32 curLen;
	Stream_Head_T vids;
	
	u8 frame_enable;
	//Stream_Node_T mjpegNode[MJPEG_ITEM_NUM];
}MJPEGB_ENC_OP_T;
static MJPEGB_ENC_OP_T  mjpegBEncCtrl;

volatile u32 jBavgsize;
u32 cal_jB_1s_size(void)
{
	u32 tp = jBavgsize;
	jBavgsize = 0;
	return tp;
}
u32 hal_jB_fcnt_get(void)
{
	u32 cnt = mjpegBEncCtrl.jfcnt;
	mjpegBEncCtrl.jfcnt = 0;
	debg("jB_fcnt:%d\n",cnt);
	return cnt;
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void c_stack_setup(C_STACK *c, QDT_BUF que[], u32 que_dep, u8 *linkbuf, u32 linksize)
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
		que[i].u_tsync =0;
	}
	

	//init p_qdt,g_qdt 
	c->p_qdt = c->g_qdt = &que[0];
	
	c->stack_size=0;
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
u8* cache_pre_malloc(C_STACK *c, u32 mlen)
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
			if(ef >= pt + mlen){
				malloc_buf = (u8*)pt;
				break;
			}else{
				if(gt  >= sf + mlen){
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
			if(gt >= pt + mlen){
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
			//debg("B[sf:%x,ef:%x,pt:%x,gt:%x]",sf,ef,(u32)(p ->pbuf + p ->buflen),(u32)(g ->pbuf));
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
	//debg(".sem:%x\n",c-> c_inf.semf);
	return malloc_buf;
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool cache_put(C_STACK *c, u8 *p, u32 len, u32 id, s32 u_tsync,s32 u_tsync_next)
{
	//#define pt  c->p_qdt
	if(c-> c_inf.semf == 0){
		debg("cache_put err\n");//while(1);
		return false;
	}
	QDT_BUF *pt = c->p_qdt;
	
	u8 i;
	for(i = 0; i < c-> c_inf.semf; i++)
		pt = pt->last;
	//sem--
	c-> c_inf.semf--;
	
	if((pt -> pbuf != NULL) && (pt->buflen != 0) && (pt->sta == 0)){
		pt-> sta = true;
		//pt -> pbuf = p;
		pt -> buflen = len;	
		pt -> id = id;
		pt -> u_tsync = u_tsync;
		pt -> u_tsync_next = u_tsync_next;
		c->stack_size+=len;
		return true;
	}else{
		debg("qdt: <list full>\n");exception_trigger();while(1);
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
void cache_pre_malloc_drop(C_STACK *c)
{	
	if(c->p_qdt->last->sta==1){
		c->p_qdt->last->sta = 0;
		c->stack_size -= c->p_qdt->last->buflen;
	}
	c->c_inf.semf++;	
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool cache_get(C_STACK *c, u8 **p, u32 *len, u32 *id, s32 *u_tsync, s32 *u_tsync_next)
{
	#define gt  c->g_qdt
	if((gt->pbuf != NULL) && (gt->buflen != 0)&& (gt->sta == true)){
		*p = gt ->pbuf;	
		*len = gt->buflen;
		*id = gt -> id;
		if(u_tsync)
			*u_tsync=gt -> u_tsync;
		if(u_tsync_next)
			*u_tsync_next=gt -> u_tsync_next;
		//gt-> sta = 0;
		//gt-> pbuf = NULL;
		//gt-> buflen = 0;
		//gt = gt->next;
		return true;
		
	}else{
		//debg("N");
		return false;	
	}
	#undef gt
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool cache_free(C_STACK *c)
{
	#define gt  c->g_qdt
	HAL_CRITICAL_INIT();
	
	if((gt->pbuf != NULL) && (gt->buflen != 0)&& (gt->sta == true)){
		//*p = gt ->pbuf;	
		//*len = gt->buflen;
		HAL_CRITICAL_ENTER();
		c->stack_size-=gt-> buflen;
		gt-> sta = 0;
		gt-> pbuf = NULL;
		gt-> buflen = 0;
		gt -> id = 0;
		gt -> u_tsync=0;
		gt -> u_tsync_next=0;
		gt = gt->next;
		HAL_CRITICAL_EXIT();
		return true;
		
	}else{
		//debg("N");
		return false;	
	}
	#undef gt
}
/*******************************************************************************
* Function Name  : sensor_ClockInit
* Description    : set sensor clock 
* Input          : u32SenClk: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
#define uSENSOR_QDT_ITEM			256
static QDT_BUF	usensor_qdt[uSENSOR_QDT_ITEM];
static C_STACK	usensor_cstack;
static u8 *usensor_jbuf;
static u32 usensor_jlen;
static u8  usensor_drop;
volatile u32 usensor_jid;
void hal_mjpB_usensor_strm_init(u8 *joutbuf, u32 joutlen)
{
	c_stack_setup(&usensor_cstack, usensor_qdt, uSENSOR_QDT_ITEM, joutbuf, joutlen);
	usensor_jlen = joutlen/2;
	usensor_drop = false;
}
/*******************************************************************************
* Function Name  : sensor_ClockInit
* Description    : set sensor clock 
* Input          : u32SenClk: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpB_usensor_adrreload_hal(void)
{
	u32 jmallolen = (usensor_jlen > 50L*1024)? (usensor_jlen * 2) : (50L*1024*2);
	u8* p = cache_pre_malloc(&usensor_cstack, jmallolen);
	if(p != NULL){
		usensor_jbuf = p;
		usensor_drop = false;
	}else{
		deg_Printf("B");
		//deg_Printf("[B:%d]",usensor_cstack.stack_size*100/mjpegBEncCtrl.mjpsize);
		//sysLog("D:B");	 
		cache_pre_malloc_drop(&usensor_cstack);
		usensor_jbuf =usensor_jbuf; //use last
		//usensor_drop = true;
	}
	//debg("m:%x\n",usensor_jbuf);
	ax32xx_mjpB_dma_cfg((u32)usensor_jbuf, (u32 )(usensor_jbuf + jmallolen));
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpB_usensor_fram_isr(void)
{
	if(BACK_JPOLTA <= (u32)usensor_jbuf){
		debg("uvc: <j len err>");	
		sysLog("uvc: <j len err>");
	}
	usensor_jlen = BACK_JPOLTA - (u32)usensor_jbuf;
	usensor_jlen = (usensor_jlen + 0x1ff) &(~0x1ff); //512 filld
	jBavgsize += usensor_jlen;
	//debg("e:%x ",usensor_jlen);
	//debg("b,s:%x,%x\n",BACK_JPOBAS,BACK_JPOEND & ~(0xf << 28));
	
	if(!usensor_drop)
	{	
		s32 u_tsync;
		s32 u_tsync_next;
		u_tsync=hal_audc_stamp_out();
		u_tsync_next = hal_audc_stamp_next();
		cache_put(&usensor_cstack,usensor_jbuf, usensor_jlen, usensor_jid,u_tsync,u_tsync_next);
		
	}
	usensor_jid++;
}
/*******************************************************************************
* Function Name  : sensor_ClockInit
* Description    : set sensor clock 
* Input          : u32SenClk: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
bool hal_mjpB_usensor_fram_free(void)
{
	return cache_free(&usensor_cstack);	
}
/*******************************************************************************
* Function Name  : sensor_ClockInit
* Description    : set sensor clock 
* Input          : u32SenClk: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
bool hal_mjpB_usensor_fram_rd(u8 **p, u32 *len, u32 *id,s32 *u_sync,s32 *u_sync_next)
{
	u8 *pbuf;
	u32 plen;
	s32 u_tsync;
	s32 u_tsync_next;
	*p = NULL;	
	*len = 0;
	*u_sync=0;
	*u_sync_next=0;
	if(!cache_get(&usensor_cstack, &pbuf, &plen,id,&u_tsync,&u_tsync_next)){

		return 	false;
	}	
	if((!plen) || (pbuf == NULL)){
		debg("jB: <len=%x,buf=%x>\n",plen,pbuf);
		hal_mjpB_usensor_fram_free();
	
		return false;	
	}
	ax32xx_sysDcacheFlush((u32)pbuf,16);
	if((pbuf[0] != 0xff) || (pbuf[1] != 0xd8)){
		if((pbuf[8] != 0xff) || (pbuf[9] != 0xd8)){
			debg("uj: <header err>:%x,%x\n",pbuf,plen);	
			hal_mjpB_usensor_fram_free();

			return false;
		}
		
	}
	*p = pbuf;	
	*len = 	plen;
	if(u_sync)
		*u_sync=u_tsync;
	if(u_sync_next)
		*u_sync_next=u_tsync_next;	
	return true;
}

/**/


enum
{
	PHOTO = 0,
	AVI = 1,
};
typedef struct
{
	u8 y_buf[640 * 480] __attribute__ ((aligned(32)));	
	u8 uv_buf[640 * 480/2] __attribute__ ((aligned(32)));
	
}JBLINE_BUF;;
typedef u8 _MJBUF_	;

//must fix 640*480*1.5  must define as static
extern JBLINE_BUF jBLbuf;
//u8 mjbbuf[512 * 1024];


volatile u32 fBcnt;

u32 hal_get_fBcnt(void)
{
	u32 ret = fBcnt;
	fBcnt = 0;
	return ret;	
}

static void (*fp_mjpB_framedown)(void);
void hal_mjpB_avi_fdown_err(void);
void hal_mjpB_isr(u32 jflg)
{

	//debg("jb:%x\n",jflg);
	//part farme down isr
	if(jflg & (BIT(0) | BIT(6))){
	
		//pause down
		if(jflg & BIT(0)){//ODMAPAUSE
			if(!(jflg & BIT(6))){
				debg("mallo B j frame size err\n");	
				sysLog("mallo B j frame size err\n");
				hal_mjpB_avi_fdown_err();
			}
		}
		
		//frame down
		if(jflg & BIT(6)){//FRAMDONE
			fBcnt++;
			//debg("}\n");
			hal_mjpB_usensor_fram_isr();
			
			if(fp_mjpB_framedown){
				__FP__(fp_mjpB_framedown);
			}
		}	
		
	}
	
	//addr reload(last reoload addr)
	if(jflg & BIT(4)){//JEOLDADRD
		//debg("&");
		//__FP__(fp_mjp_adrload_hal);
		
	}

	if(jflg & (BIT(9) | BIT(10) | BIT(11))){//TIMEINF_LOAD_DOWN
		//BACK_JPFLG_CLR |= BIT(9) | BIT(10) | BIT(11);
	}
	
	//err isr
	if(jflg & (BIT(3)|BIT(7))){
		if(jflg & BIT(3)){//OBDMAERROR
			//BACK_JPFLG_CLR |= BIT(3);//ODMAERRORCLR
			debg("B-OBDMAERROR\n");
			sysLog("B-OBDMAERROR\n");
		}
		if(jflg & BIT(7)){//JEOFULL
			//BACK_JPFLG_CLR |= BIT(7);//JEOFULL
			debg("B-JEOFULL\n");
			sysLog("B-JEOFULL\n");
			//debg("b,s:%x,%x\n",BACK_JPOBAS,BACK_JPOEND & ~(0xf << 28));
		}
		hal_mjpB_avi_fdown_err();
		
	}
	//flg
	if(jflg & ~(BIT(0)| BIT(6) |BIT(4) | BIT(3) |  BIT(7)| BIT(9) | BIT(10) | BIT(11) | BIT(17))){
		debg("B-jflg:%x\n",jflg);	
		//1  JPIDAM_RESUME 
		//2  YUV422_DMADONECLR  
		//5  JDRSTCLR   
		//8  JDHUFCLR 
		//BACK_JPFLG_CLR |= jflg;
	}	
}

volatile s8 jbecsta;

typedef enum
{
	_JEC_FREE_ = 0,
	_JEC_BUSY_ = 1,	
	_JEC_DOWN_ = -1,
	_JEC_ERR_  = -2,		
}JBEC_STA;
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
JBEC_STA hal_mjpBec_sta(void)
{
	return 	jbecsta;
}
bool hal_mjpB_ecoding(void)
{
	if(jbecsta == _JEC_BUSY_){
		return 1;	
	}
	return 0;
}
void hal_mjpB_ecdown(void)
{
	extern volatile u8 debg_jdec;
	debg_jdec --;
			
	jbecsta = _JEC_DOWN_;
	
}
void hal_mjpB_ecdown_err(void)
{
	extern volatile u8 debg_jdec;
	debg_jdec --;
			
	jbecsta = _JEC_ERR_;
	
}
void hal_mjpB_ecbusy(void)
{
	jbecsta = _JEC_BUSY_;
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpBec_recfg(void)
{
	jbecsta = _JEC_BUSY_;
	ax32xx_mjB_as_dcode();
	ax32xx_mjpB_Ctl_init(mjpegBEncCtrl.type, mjpegBEncCtrl.qulity, 
	mjpegBEncCtrl.usensor_width, 
	mjpegBEncCtrl.usensor_height, 
	mjpegBEncCtrl.mjpeg_width, 
	mjpegBEncCtrl.mjpeg_height
	);
	ax32xx_recfg_mjpb_tminf();
	if(mjpegBEncCtrl.frame_enable==1)
	{
		//deg_Printf("----------usensor frame_enable----------\n");
		//hal_custom_frame_add_umjpeg((u32)jBLbuf.y_buf);
	}
	ax32xx_mjpB_Linebuf_cfg(jBLbuf.y_buf,jBLbuf.uv_buf);	
	hal_mjpB_usensor_adrreload_hal();
	mjpegBEncCtrl.jfcnt++;
}

/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpB_photo_fdown(void)
{
	//jbecsta = _JEC_DOWN_;
	hal_mjpB_ecdown();
	ax32xx_mjpB_dcdown_reg(NULL);
	huvc_dcod_down(0);//usb_usensor_frame_free();
	ax32xx_mjpB_stop();	
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpB_avi_fdown(void)
{
	
	//ax32xx_reg_jdclinedown(NULL);
	//jbecsta = _JEC_DOWN_;
	hal_mjpB_ecdown();
	huvc_dcod_down(0);//usb_usensor_frame_free();
	ax32xx_mjpB_stop();
}
void hal_mjpB_avi_fdown_err(void)
{
	sysLog("fdown err\n");
	debg("fdown err\n");
	hal_mjpB_ecdown_err();
	huvc_dcod_down(1);//usb_usensor_frame_free();
	ax32xx_mjpB_stop();
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void api_mjp_avi_stop(void)
{
	if(mjpegBEncCtrl.mjpbuf == 0)
		return;
		
	jbecsta = _JEC_FREE_;
	huvc_dcod_down(0);
	ax32xx_mjpB_dcdown_reg(NULL); 	//disconnect mjpec
//	ax32xx_mjpB_stop();				//stop mjpec

	hal_mjpegBMemUninit();
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpB_manualstart(void)
{
	hal_mjpBec_recfg();
	ax32xx_mjpB_on();
	//debg("s");
}
static s8 hal_mjpB_QualityCheck(u8 quality)
{
	if(quality > 0x0f){
		debg("mjpb: <set quality err>\n");	
	}
	return JPEG_Q_62; //fix Q= 31;
}
int hal_mjpBMemInit(void)
{
	/*
	s32 size;

	if(mjpegBEncCtrl.ybuffer && mjpegBEncCtrl.mjpbuf)
		return 0;

	if(mjpegBEncCtrl.ybuffer==0)
	{
		mjpegBEncCtrl.ybuffer = (u32)jBLbuf.y_buf;//(u32)hal_sysMemMalloc(HAL_CFG_MJPEG_HEIGHT*HAL_CFG_MJPEG_WIDTH*3/2,64);
		if(mjpegBEncCtrl.ybuffer==0)
			return -1;
		mjpegBEncCtrl.uvbuffer = (u32)jBLbuf.uv_buf;//mjpegEncCtrl.ybuffer+HAL_CFG_MJPEG_HEIGHT*HAL_CFG_MJPEG_WIDTH;
	}
	
//	MJPEG_ITEM_SIZE = 32768;
    if(mjpegBEncCtrl.mjpbuf==0)
    {
	#if SDRAM_SIZE == SDRAM_SIZE_8M	
	    size = hal_sysMemRemain()-128*1024;
	#else
	    size = hal_sysMemRemain()-8*1024;
		if(size>1024*1024) // remain space for other using
		{
			size -= (size>>7);
		}
	#endif	
		size &=~0x3f;
		mjpegBEncCtrl.mjpbuf = (u32)hal_sysMemMalloc(size,64);
		if(mjpegBEncCtrl.mjpbuf == 0)
			return -2;
		mjpegBEncCtrl.mjpsize = size;
		mjpegBEncCtrl.mjpbuf = mjbbuf;
		mjpegBEncCtrl.mjpsize = sizeof(mjbbuf);
		debg("HAL : [MJPEG]<INFO> mbuffer addr = 0x%x,size = 0x%x\n",mjpegBEncCtrl.mjpbuf,mjpegBEncCtrl.mjpsize);
	}
	*/
		s32 size;
	    if(/*mjpegBEncCtrl.ybuffer && */mjpegBEncCtrl.mjpbuf)
		   return 0;
		mjpegBEncCtrl.frame_enable = 0;
	#if SDRAM_SIZE == SDRAM_SIZE_8M	
	    size = hal_sysMemRemain()-128*1024-8*1024;
	   
	#else
	    size = hal_sysMemRemain()-8*1024;
		if(size>1024*1024) // remain space for other using
		{
			size -= (size>>7);
		}/**/
	#endif	
		size &=~0x1ff;
		
		mjpegBEncCtrl.mjpbuf = (INT32U)hal_sysMemMalloc(size,64);//mjbbuf;
		if(mjpegBEncCtrl.mjpbuf==0)
			return -1;
		mjpegBEncCtrl.mjpsize = size;//sizeof(mjbbuf);	
	
		deg_Printf("HAL : [MJPEGB]<INFO> mbuffer addr = 0x%x,size = 0x%x\n",mjpegBEncCtrl.mjpbuf,mjpegBEncCtrl.mjpsize);
    return 0;
}
/*******************************************************************************
* Function Name  : hal_mjpegBMemUninit
* Description    : hal layer .mjpeg memory uninitial
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void hal_mjpegBMemUninit(void)
{
	if(mjpegBEncCtrl.mjpbuf)
		hal_sysMemFree((void *)mjpegBEncCtrl.mjpbuf);
	mjpegBEncCtrl.mjpbuf = 0;
	mjpegBEncCtrl.frame_enable = 0;

}

/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int mjpB_actech_as_photo(u16 win_w, u16 win_h, u8 quality, u8 frame_enable)
{	
	
	/*
	if(!mjpegBEncCtrl.pusensor_adpt){
		return 1;	
	}
	*/
	if(hal_mjpBMemInit()<0)
	{
		deg_Printf("image encode : memory malloc fail\n");
		return -1;
	}
	mjpegBEncCtrl.type = PHOTO;
	mjpegBEncCtrl.mjpeg_width = win_w; 
	mjpegBEncCtrl.mjpeg_height = win_h;  
	mjpegBEncCtrl.usensor_width = 640;
	mjpegBEncCtrl.usensor_height= 480;
	mjpegBEncCtrl.frame_enable = frame_enable;
	mjpegBEncCtrl.qulity = hal_mjpB_QualityCheck(quality);
	//ax32xx_mjpB_Ctl_init(mjpegBEncCtrl.type, quality, mjpegBEncCtrl.usensor_width, mjpegBEncCtrl.usensor_height, win_w, win_h);
	//ax32xx_mjpB_Linebuf_cfg(jBLbuf.y_buf,jBLbuf.uv_buf);	
	hal_mjpB_usensor_strm_init((u8* )mjpegBEncCtrl.mjpbuf, mjpegBEncCtrl.mjpsize);

	ax32xx_mjpegEB_ncodeISRRegister((void *)hal_mjpB_isr);
	fp_mjpB_framedown = hal_mjpB_photo_fdown;
	ax32xx_mjpB_dcdown_reg(hal_mjpB_manualstart); //link to mjpbstart	
	return 0;
}

/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int hal_mjpB_actech_as_avi(u16 win_w, u16 win_h, u8 quality)
{	
	//while(hal_mjpB_decoding());
	/*
	//usnesor online;
	if(!mjpegBEncCtrl.pusensor_adpt){
		return 1;	
	}
	*/
	if(hal_mjpBMemInit()<0)
	{
		deg_Printf("image encode : memory malloc fail\n");while(1);
		return -1;
	}
	
	mjpegBEncCtrl.type = AVI;
	mjpegBEncCtrl.mjpeg_width = win_w; 
	mjpegBEncCtrl.mjpeg_height = win_h;  
	mjpegBEncCtrl.usensor_width = 640;
	mjpegBEncCtrl.usensor_height= 480;
	
	//mjpdc pr check
	mjpegBEncCtrl.qulity = hal_mjpB_QualityCheck(quality);
	//ax32xx_mjpB_Ctl_init(mjpegBEncCtrl.type, mjpegBEncCtrl.qulity, mjpegBEncCtrl.usensor_width, mjpegBEncCtrl.usensor_height, win_w, win_h);
	//ax32xx_mjpB_Linebuf_cfg(jBLbuf.y_buf,jBLbuf.uv_buf);	
	hal_mjpB_usensor_strm_init((u8* )mjpegBEncCtrl.mjpbuf, mjpegBEncCtrl.mjpsize);

	ax32xx_mjpegEB_ncodeISRRegister((void *)hal_mjpB_isr);	
	fp_mjpB_framedown = hal_mjpB_avi_fdown;//mjpB_stop;//mjp_on;//mjp_stop; 
	ax32xx_mjpB_dcdown_reg(hal_mjpB_manualstart);//link to mjpbstart
	return 0;
}
/*******************************************************************************
* Function Name  : hal_mjpB_nextsync
* Description    : hal layer.get next mjpB frame sync
* Input          : None
* Output         : None
* Return         : next frame tsync
*******************************************************************************/
s32 hal_mjpB_nextsync(void)
{
	s32 next_sync;
	next_sync = usensor_cstack.g_qdt->u_tsync_next;
	return next_sync;
}
/*******************************************************************************
* Function Name  : hal_mjpegRawBuffer_prefull
* Description    : hal layer.mjpeg buffer pre full(2/3) just
* Input          : None
* Output         : None
* Return         : true:mjpeg buffer is pre_full
*******************************************************************************/
bool hal_mjpB_Buffer_prefull(void)
{
	s32 size;
	size = usensor_cstack.stack_size;
	if(size<0)
		return false;
	if(size>(mjpegBEncCtrl.mjpsize/2))
		return true;
	return false;
}
