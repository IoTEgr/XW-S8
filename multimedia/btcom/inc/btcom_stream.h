#ifndef __BTCOM_STREAM__
#define __BTCOM_STREAM__

#define TIME_OUT 1000*10

#define BTSTREAM_DEBUG 		0

#if BTSTREAM_DEBUG
#define bts_Printf(...)       uart_Printf(__VA_ARGS__)
#else
#define bts_Printf(...)
#endif

#if 1
char *switch_data(const u8 *data,int len);

void encodeAction(char * Rc);
void string_to_hex_manual(const char *str,u8 *out_data);
#endif

/*
*
*opreate the ADtm
*/
u32 Bt_Stream_getADtm(void);
void Bt_Stream_setADtm(u32 val);

u32 Bt_Stream_Adjtm(u32 oldVal,int count,u16 h,u32 r_val);

/*
*
*复位温度参数,为下一次做准备
*/
void Bt_Stream_HottimeRest(void);

/*
*up/low_limit	:上/下限
*change_range	:每次的变化范围
*设置hot_time的变化
*/
void Bt_Stream_HottimeAdj(S32 hot_time,u8 change_range,u8 up_limit,u8 low_limit);

/*
*
*清理已入队的蓝牙数据
*/
void Bt_Stream_Clean_Useless(void);


/*
*
*蓝牙数据入队(不带校验)
*/
int Bt_Data_Streamin(const u8 *,int len);

/*
*
*蓝牙数据入队(带校验)
*/
void Bt_Stream_In(const u8 *data, int len);

/*
*
*蓝牙数据出队
*return val:1:正常取到数据 3:队列无数据退出 4:超时退出 5:继续轮询
*/
u8 Bt_Stream_Out(Bt_BufManage_T *Manage,u32 *buff,u32 *size,u32 *tickTime);

/*
*
*蓝牙数据释放
*/
u8 Bt_Stream_Free(Stream_Head_T *head);

/*
*蓝牙数据解压
*return val: 0:success  -1:fail
*/
u8  Bt_Stream_Decompress(Bt_BufManage_T *Manage,u32 src_buf,u8 *outStr,u8 *dest_buf,u8 *dest_len);


#endif

