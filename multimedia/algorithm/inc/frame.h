#include "../../media.h"




typedef struct _MJQDT
{
	u8 sta;
	s8 endf;
	s32 tgl;
	u32 fsize;
	u8	*pbuf;
	u32 buflen;
	u8	*dribuf;
	s32 t_sync;
	s32 t_sync_next;
	struct _MJQDT *phead; //JPG?a绡撴綖?QDT
	struct _MJQDT *plink; //JPG??绡撴幊???QDT
	struct _MJQDT *next;  //??绡撴幊???QDT
}QDT_MJP;

QDT_MJP *que_jrd();
INT32S hal_stream_checkFrame(QDT_MJP *phal);
INT32S hal_stream_writeFrame(int fd,QDT_MJP *phal);



void hal_stream_qdt_init(u8*addr,u32 bufLen,u32 len);

void hal_stream_qdt_uninit();

QDT_MJP *hal_stream_qdt_malloc(u8 type);

void hal_stream_qdt_reles(QDT_MJP *qdt);


QDT_MJP *hal_strem_drop_deal(QDT_MJP *header);



QDT_MJP *hal_stream_data_hal(s8 sgn,u32 len,s32 sync,s32 sync_next,u8* drop);


void hal_steam_set_pjchain(QDT_MJP *qdt);

void hal_strem_drop(QDT_MJP *pdel);


