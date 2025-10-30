#ifndef __BTCOM_INNER__
#define __BTCOM_INNER__
#include "../../../hal/inc/hal_stream.h"

// 最大帧数据
#define LEN_OF_FRAME (4*1024 + 20)
#define NUM_OF_FRAME (75)

// 每帧数据缓存
struct _FRAME_BUF_T {
	struct _FRAME_BUF_T *next;
	struct _FRAME_BUF_T *prev;
	u8 data[LEN_OF_FRAME];
	u16 len;
	u16 total_len;
};
typedef struct _FRAME_BUF_T FRAME_BUF_T;

// 帧数据的使用
typedef struct {
	FRAME_BUF_T buf[NUM_OF_FRAME];
	FRAME_BUF_T *free;
	FRAME_BUF_T *busy;
	FRAME_BUF_T *working;
	volatile u16 remain_frameCount;	//记录未使用数据帧
} FRAME_MANAGE_T;

// 帧数据的解析器
typedef bool (*FN_FRAME_PARSER)(const u8 *data, int len);

// 命令/解析管理
typedef struct {
	u8 cmd;
	FN_FRAME_PARSER fnParser;
} CMD_PARSER_T;

#define BT_BUFFER_NUM		35
#define BT_BUFFER_EACH_LEN	4*1024
	
/*typedef*/ struct Bt_Rec_Buffer
{
	u8 BT_buf[BT_BUFFER_NUM][BT_BUFFER_EACH_LEN];
};

typedef struct _Bt_BufManage_T
{
	struct Bt_Rec_Buffer *buffer;

	u32 curBuffer;
	u8 Btbuf_Set[BT_BUFFER_NUM];	//Btbuf ready or not
	u8 Btbuf_Used[BT_BUFFER_NUM];	//Btbuf used or not

	u8 Bt_RecEnd;//结束标志
	//长度
	//链表结构
	Stream_Head_T vid_s;
	Stream_Node_T Node[BT_BUFFER_NUM];
	volatile s16 Btbuf_QueueRemain;	//queue num remain
	u32 al_addr;	//对齐前的地址
}Bt_BufManage_T;


/**
 * 解析器初始化
 *
 */
void btcomParserInit(u8 cmdData[],u8 cmdNum);

void btcomParserUnInit(void);

/**
 * 判断是否支持给定命令
 *
 * cmd - 给定命令
 *
 * 返回:true - 支持；false - 不支持
 *
 */
bool btcomCmdSupport(u8 cmd);


/**
 * 计算checksum
 *
 * data - 要校验的数据
 * len - 数据长度
 * init_crc - crc初始值
 *
 * 返回:checksum
 *
 */
u8 btcomCheckSum(const u8 *data,int len,u8 init_crc);


/******************************************
 **************  btcom_buf.c *****************
 ******************************************/

/**
 * 创建保存帧数据的缓存
 *
 */
FRAME_MANAGE_T *btcomBufCreate(void);

/**
 * 销毁数据缓存
 *
 */
void btcomBufDestroy(FRAME_MANAGE_T *manager);

/**
 * 选一个空闲的buffer接收数据
 *
 * TRUE-成功；FALSE-失败
 */
bool btcomGetIdleBuf(FRAME_MANAGE_T *manager);

/**
 * 查找是否有数据
 *
 */
FRAME_BUF_T *btcomTestBusyBuf(FRAME_MANAGE_T *manager);

/**
 * 标记数据已处理，可回收
 *
 */
void btcomMarkConsumed(FRAME_BUF_T *item);

/**
 * 回收buffer
 *
 */
void btcomRecycleBuf(FRAME_MANAGE_T *manager);

/**
 * 数据放入待处理链表
 *
 */
void btcomPutBusy(FRAME_MANAGE_T *manager);

/**
 * 打印buffer使用情况
 *
 */
void btcomCheckManager(FRAME_MANAGE_T *manager);

Bt_BufManage_T btdataBufCreate(void);

#endif // __BTCOM_INNER__

