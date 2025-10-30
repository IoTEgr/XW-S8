#include "../../multimedia.h"

struct _MSG{
	void *pmsg;
	struct _MSG *next;	
};
typedef struct _MSG QUE_S;
static QDT_MJP*qdt_mjp;
static u8 qdtSize = 0; 
QDT_MJP *pjchain;
QDT_MJP *qdt_jhead;
QUE_S *que_v;
QUE_S *pque_vput;
QUE_S *pque_vget;
QUE_S *que_j;
QUE_S *pque_jput,*pque_jget;


void que_init(QUE_S que[], u16 item)
{
	u16 i;
	for(i = 0; i < item; i++){
		que[i].pmsg = NULL;
		que[i].next = ((i + 1) == item)?&que[0] : &que[i + 1];		
	}	
}

void que_jinit(void)
{
	que_j  = (QUE_S *)hal_sysMemMalloc(qdtSize*sizeof(QUE_S),64);
	pque_jput = pque_jget = &que_j[0];
	que_init(que_j,qdtSize);
}

QDT_MJP *que_jrd()
{
	HAL_CRITICAL_INIT();
	QDT_MJP * p = NULL;
	
	HAL_CRITICAL_ENTER();
	if(NULL != pque_jget -> pmsg)
	{
		p = pque_jget -> pmsg;
		p -> sta =  __TYP_MJP__|__READY__;
		pque_jget -> pmsg = NULL;
		pque_jget = pque_jget -> next;
	}
	
	/*
	if(p == NULL){
		debg("-que rd err\n");while(1);	
	}
	 */
	HAL_CRITICAL_EXIT();
	return p;
}

/*******************************************************************************
* Function Name  : sensor_ClockInit
* Description    : set sensor clock
* Input          : u32SenClk: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
void que_jwr(QDT_MJP *qdt)
{
	if(qdt == NULL){
		debg("-que wr err\n");while(1);	
	}
	if((qdt-> sta & __READY__) != __READY__){
		debg("-qdt add err\n");while(1);	
	}
	//debg("jmsg:%x\n", qdt);
	
	if(NULL == pque_jput -> pmsg){
		 qdt -> sta = __TYP_MJP__|__READY__;		//BUSY 
		 pque_jput -> pmsg = qdt;
		 pque_jput = pque_jput->next;
	}else{
		debg("-que full\n");	//while(1);	
	}
}
/*******************************************************************************
* Function Name  : sensor_ClockInit
* Description    : set sensor clock 
* Input          : u32SenClk: senclk frequency
* Output         : None
* Return         : None
*******************************************************************************/
u32 get_jframes_inque(void)
{
	u32 frames = 0;
	u16 i;
	for(i = 0; i < /*MJQDT_ITEM*/qdtSize; i++){
		if(que_j[i].pmsg != NULL){
			frames++;		
		}
	}
	return frames;
}

static u8 drop_flag = 0;

void hal_stream_qdt_init(u8*addr,u32 bufLen,u32 len)
{
	u32 i;


	qdtSize = bufLen/len;

	if(qdtSize){
		qdt_mjp = (QDT_MJP*)hal_sysMemMalloc(qdtSize*sizeof(QDT_MJP),64);
	}
	
	for(i = 0; i < qdtSize; i++){
		qdt_mjp[i].sta = __FREE__;
		qdt_mjp[i].tgl = __FREE__;
		qdt_mjp[i].pbuf = (u8 *)(addr + i * len);//鐢宠绌洪棿
		qdt_mjp[i].buflen =  len;
		qdt_mjp[i].phead = NULL;
		qdt_mjp[i].plink = NULL; 
		qdt_mjp[i].next = ((i + 1) == qdtSize)?&qdt_mjp[0] : &qdt_mjp[i+1];
		//deg_Printf("the size == %d the mjpbuf[%d] == %x,buflen == %x\n",qdtSize,i,qdt_mjp[i].pbuf,qdt_mjp[i].buflen);
	}
	que_jinit();
	drop_flag = 0;
	return;
}

void hal_stream_qdt_uninit()
{
	if(qdt_mjp){
		hal_sysMemFree(qdt_mjp);
	}

	if(que_j){
		hal_sysMemFree(que_j);
	}
	return;
}

QDT_MJP *hal_stream_qdt_malloc(u8 type)
{
	static QDT_MJP *p_mjp;
	QDT_MJP *p;
	u32 i;
	
	if(NULL == p_mjp)
		p_mjp = &qdt_mjp[0];	
	
	p = NULL;
	for(i = 0; i < qdtSize; i++){

		if(__FREE__ == p_mjp -> sta){
			p = p_mjp;
			p -> sta = type | __BUSY__;	
			p_mjp = p_mjp -> next;
			break;
		}
		p_mjp = p_mjp -> next;	
	}

	return p;
}
void hal_steam_set_pjchain(QDT_MJP *qdt)
{
	if(qdt)
		pjchain = qdt;
}
void hal_stream_qdt_reles(QDT_MJP *qdt)
{
	//if(__BUSY__ == (qdt -> sta & __READY__)){
		qdt -> sta = __FREE__;
		qdt -> tgl = -1;	
		qdt -> plink = NULL;
		qdt -> phead = NULL;	
	//}	
}

QDT_MJP *hal_strem_drop_deal(QDT_MJP *header)
{
	QDT_MJP *pdel; 
	QDT_MJP *pnext;
	
	//閲婃斁涓€甯?
	pdel = que_jrd();
	if(pdel != NULL){
		//debg("-r2:%x\n",pdel->pbuf);
		while(1){
			pnext = pdel->plink;
			//if(pdel->sta == (__TYP_MJP__|__READY__)){//璇嗗埆MJPG鏁版嵁鍖?
				hal_stream_qdt_reles(pdel);
			//}
			//pdel = phal->plink;
			if(pnext == NULL) //閲婃斁瀹屾瘯
				break;
			pdel = pnext;
		}
		//pt_chain();
		return hal_stream_qdt_malloc(__TYP_MJP__);
	}
#if 0	
	debg("*");
	//pt_chain();
	jpeg_enc_Stop();
	//mjp_pause();
	if(header != NULL){
		pdel = header;
		while(1){
			pnext = pdel->plink;
			//if(pdel->sta == (__TYP_MJP__|__READY__)){//璇嗗埆MJPG鏁版嵁鍖?
				qdt_reles(pdel);
			//}
			//pdel = phal->plink;
			if(pnext == NULL) //閲婃斁瀹屾瘯
				break;
			pdel = pnext;
		}	
		//mjp_restart();
		//pt_chain();
		pasue_rec = 1;
		jpeg_enc_Start(rec_mod,u8SensorType,2);
		pasue_rec = 0;
		return NULL;//qdt_malloc(__TYP_MJP__);
	}
	pt_chain();
	//debg("reles fail\n");while(1);
	stop_rec = TRUE;
#endif	
	debg("........................................................\n");
	return NULL;
}



QDT_MJP *hal_stream_data_hal(s8 sgn,u32 len,s32 sync,s32 sync_next,u8* drop)
{	
	volatile u8 qtd_typ = 0;//鎷嶇収鍙戞秷鎭ā寮?
	QDT_MJP *pdel;
	QDT_MJP *pnext;
	/*if(rec_mod != 1){
		if((sys_ctl.jpeg_picture_size == JPEG_SIZE_2560_1440) ||((0 == u8SensorType) && (sys_ctl.jpeg_picture_size == JPEG_SIZE_1920_1080)))
		{
			debg("L\n");
			qtd_typ = 1;
		}
	}*/

	if(drop_flag){
		if(sgn == -1){
			drop_flag = 0;
			return pjchain;
		}else{
			return pjchain;
		}
	}
		
	QDT_MJP *pnew = hal_stream_qdt_malloc(__TYP_MJP__);
	
	if(pnew == NULL){
		//deg_Printf("the qdt malloc null\n");
		//debg("D");
		/*pnew = hal_strem_drop_deal(qdt_jhead);	
		if(pnew == NULL){
			//debg("j malloc err");//while(1);	
			return NULL;
		}*/
		/*if(sgn!=-1)*/{
			if(qdt_jhead){
				pdel = qdt_jhead;
				//debg("-r2:%x\n",pdel->pbuf);
				while(1){
					pnext = pdel->plink;
					//if(pdel->sta == (__TYP_MJP__|__READY__)){//璇嗗埆MJPG鏁版嵁鍖?
						hal_stream_qdt_reles(pdel);
					//}
					//pdel = phal->plink;
					if(pnext == NULL) //閲婃斁瀹屾瘯
						break;
					pdel = pnext;
				}
			}
		}
		*drop = 1;
		if(sgn != -1)
		  drop_flag = 1;
		else
		  drop_flag = 0;
		
		return pjchain;
	}

	pjchain -> sta = __TYP_MJP__|__READY__;
	pjchain -> tgl = sgn;	
	//鍖呭ご
	if(sgn == 0){ 
		qdt_jhead = pjchain;
		pjchain ->  phead = qdt_jhead;
		pjchain ->	fsize = len;//pjchain ->buflen;
		pjchain ->  plink = pnew;
		pjchain ->  t_sync = sync;
		pjchain ->  t_sync_next = sync_next;
	}
	//鍖呭熬
	else if(sgn == -1){
		pjchain ->  phead = NULL;
		pjchain ->	fsize = len;//REG32(JPOLTA) - (u32)pjchain->pbuf;//pjchain ->buflen; 
		pjchain ->  plink = NULL;
		pjchain ->  t_sync = sync;
		pjchain ->  t_sync_next = sync_next;
		
		if(!qtd_typ){
			if(qdt_jhead == NULL){//澶勭悊鍙湁1涓猵layload 鐨凧PG
				qdt_jhead = pjchain;
				pjchain ->  phead = pjchain;
			}
			que_jwr(qdt_jhead);//娉ㄥ唽鍒皅ue
			qdt_jhead = NULL;
		}
	}
	//涓棿鍖?
	else{
		pjchain ->  phead = NULL;
		pjchain ->	fsize = len;//pjchain ->buflen;
		pjchain ->  plink = pnew;
		pjchain ->  t_sync = sync;
		pjchain ->  t_sync_next = sync_next;
		
	}
	//if(qtd_typ)
		//que_jwr(pjchain);//娉ㄥ唽鍒皅ue

	//鐢宠鏂扮殑buff
	pjchain = pnew; //pnew == NULL
	return pjchain;
	//mjp_dma_cfg(pjchain);
	//debg("p:%x\n",pjchain);
	
}

INT32S hal_stream_checkFrame(QDT_MJP *phal)
{
	u32 fsize = 0,tempSize = 0;
	QDT_MJP *pcheck = phal;
	s8 togl = 0;
	while(1){
		//debg(":%x,%x\n",pcheck -> tgl,pcheck->sta);
		if(pcheck->sta == (__TYP_MJP__|__READY__)){//Identify MJPG packets
			fsize +=  ((pcheck -> fsize ));//Do not write the rest//pcheck -> fsize;//pcheck -> buflen;//
			if(pcheck -> tgl == -1){
				//fsize +=  ((0x1ff)/512)*512;//Do not write the rest//pcheck -> fsize;//pcheck -> buflen;//
				tempSize = fsize;
				
				fsize = (fsize + 0x1ff)&(~0x1ff);//local playback err , last section must be aligned
				if(tempSize != fsize){
					memset(pcheck->pbuf+pcheck -> fsize,0,fsize-tempSize);
					pcheck -> fsize += fsize-tempSize;
				}
				break;
			}
			if(pcheck -> tgl != togl){
				//debg("tgle err:%x,%x\n",pcheck -> tgl,togl);while(1);
				deg_Printf("tgle err \n");
				return  -1;
			}
			togl++;
		}
		else{
			/*
			debg("qtd check sta err\n");
			pt_chain();
			jpeg_enc_Stop();
			*jsize = 0;*/
			fsize = 0;
			//return fsize;
			deg_Printf("check sta error\n");
			return -1;
		}
		pcheck = pcheck->plink;
	}
	return fsize;
}

INT32S hal_stream_writeFrame(int fd,QDT_MJP *phal)
{
		u32 fsize = 0;
		INT32S res;
	
		QDT_MJP *p = phal;//phal-> phead;
		QDT_MJP *pnext;
		s8 enflg;
		fsize = 0;
			//debg("b");
		while(1){
			pnext = p ->plink;
			
			//鍐欏叆鏁版嵁
			//u32 nwrite;
			if(p->sta == (__TYP_MJP__|__READY__)){//璇嗗埆MJPG鏁版嵁鍖?
				//debg("%d",p -> tgl);
				enflg = p -> tgl;
				u32 len = (p -> fsize);//鍓╀綑閮ㄥ垎涓嶅啓/*len:鍥剧墖鏂囦欢澶у皬*/
				res = write(fd, p -> pbuf, len);
				if(res!=len)
				{
					deg_Printf("-wrj fat err\n");
					return -1;	
				}
				fsize += res;
				if(p -> sta == (__TYP_MJP__|__READY__))
					hal_stream_qdt_reles(p);
				else
					debg("!");
				
			}
			
			//閫€鍑?
			if((enflg == -1) && (pnext == NULL))
				break;
			//p = p -> plink;	
			p = pnext;
			
		}
		return fsize;
}


void hal_strem_drop(QDT_MJP *pdel)
{ 
	QDT_MJP *pnext;
	

	if(pdel != NULL){
		//debg("-r2:%x\n",pdel->pbuf);
		while(1){
			pnext = pdel->plink;
			//if(pdel->sta == (__TYP_MJP__|__READY__)){//璇嗗埆MJPG鏁版嵁鍖?
				hal_stream_qdt_reles(pdel);
			//}
			//pdel = phal->plink;
			if(pnext == NULL) //閲婃斁瀹屾瘯
				break;
			pdel = pnext;
		}
	}

	debg(" drop vids........................................................\n");
	return ;
}

