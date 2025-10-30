#include "../../../ax32_platform_demo/application.h"
#include "../inc/btcom.h"
#include "../inc/btcom_inner.h"

#define BT_TIME_OUT 15500

// 全局数据
static FRAME_MANAGE_T *sFrmManager = NULL;
static FN_BTCMD_PROC sCmdProc = NULL;
static FRAME_BUF_T *serviceTempBuf = NULL;
static u8 serviceFlag = 0;
static u8 dot_save_next_data[16];
static u8 dot_data_tail = 0;
static u32 bt_timeout = 0;
Bt_BufManage_T Bt_BufManage;

static u8 flow_ctrl = 0; // 蓝牙数据流控启停标志
static u8 start_jpg_flag = 0;
static u8 dotUnit_flag = 0; // 点阵整合数据包标志位
/**
 * 打印串口数据
 *
 */
static void prvOutputStream(const u8 *data, int len)
{
	// example code
	char buf[100], *p;
	int i, outlen;
	u8 hi, lo;

	p = buf;
	outlen = len > 33 ? 33 : len;
	for (i = 0; i < outlen; i++)
	{
		hi = (data[i] >> 4) & 0x0F;
		lo = data[i] & 0x0F;
		*p++ = (hi >= 10) ? 'A' + (hi - 10) : '0' + hi;
		*p++ = (lo >= 10) ? 'A' + (lo - 10) : '0' + lo;
		*p++ = ' ';
	}
	*p = '\0';
	// debg("STREAM:len=%d,[%s]\n", len,buf);
}

/**
 * 串口数据接收
 *
 */

// u8 tmpdata[1024*300];
static u8 *BT_Pic_Addr = NULL;
static u32 dataLen = 0;

static void prvRxIsr(u8 data)
{

	if (sFrmManager != NULL)
	{
		// 先分配一个接收buffer
		if (sFrmManager->working == NULL)
		{
			btcomRecycleBuf(sFrmManager);
			if (btcomGetIdleBuf(sFrmManager) == false)
			{
				/*if(flow_ctrl==1){

				}else{
					flow_ctrl=1;
					Bt_Flow_Ctrl(flow_ctrl);
				}*/
				deg_Printf("[RxIsr]No buf left.flow control remain:%d\n", sFrmManager->remain_frameCount);
				return;
			}
		}

		// 超时检测
		static uint32 lasttick = 0;
		uint32 tick = XOSTimeGet();
		FRAME_BUF_T *buf = sFrmManager->working;

		// 设置超时时间，防止消耗数据过慢导致丢数据
		if (Bt_Get_Printer_Moto_Speed() > 70)
			bt_timeout = BT_TIME_OUT * 17 / 10;
		else if (Bt_Get_Printer_Moto_Speed() > 50)
			bt_timeout = BT_TIME_OUT * 15 / 10;
		else
			bt_timeout = BT_TIME_OUT;

		if (lasttick > 0 && tick > lasttick && (tick - lasttick) >= bt_timeout && buf->len > 0)
		{
			// deg_Printf("Timeout data: len:%d data:%x %d\n",buf->len,data,SysCtrl.stream_drop_data);
			// prvOutputStream(buf->data, buf->len);
			buf->len = 0;
		}
		lasttick = tick;

#if 1
		if (start_jpg_flag == 1)
		{
			// if (dataLen < JPG_LEN_MAX)	BT_Pic_Addr[dataLen++] = data;
		}
		else if (start_jpg_flag == 2)
		{
		}
#endif
		// 将存起来的ID和长度放回去
		if (dot_data_tail)
		{
			int k = 0;
			for (k = 0; k < 3; k++)
			{
				buf->data[k] = dot_save_next_data[k];
			}
			buf->len = k + 1;
			dot_data_tail = 0;
		}

		// 是否数据部份
		if (buf->len >= 6)
		{
			if (buf->data[2] == 0xD1)
			{ // 接收身份校验时把一个字节扩成两个字节
				buf->data[buf->len++] = data >> 4;
				buf->data[buf->len++] = data & 0x0f;
			}
			else
			{
				if (dotUnit_flag && buf->len >= 47)
				{ // 点阵整合包整合数据
					if ((buf->data[buf->len - 3] == 0x51) && (buf->data[buf->len - 2] == 0x78) && (buf->data[buf->len - 1] != 0xA2))
					{
						buf->total_len = buf->len - 3;
						buf->data[buf->len] = data;
						// 存ID和长度，下一次用
						int i = 0;
						for (i = 0; i < 3; i++)
						{
							dot_save_next_data[i] = buf->data[buf->len - 3 + i];
						}
						dot_data_tail = 1;

						// 特殊情况，不是A2指令，则处理最后一个整合包

						btcomPutBusy(sFrmManager);
						if (dotUnit_flag)
						{
							dotUnit_flag = 0; // push了整合的数据包之后开始新的一包
						}
						sFrmManager->remain_frameCount--;
						// deg_Printf("isr2[%d]\n",sFrmManager->remain_frameCount);
					}
					else if ((buf->data[buf->len - 6] == 0x51) && (buf->data[buf->len - 5] == 0x78) && (buf->data[buf->len - 4] == 0xA2))
					{
						// 是A2指令，则从已收到的数据里剔掉头
						buf->len = buf->len - 8;
						buf->data[buf->len++] = data;
					}
					else
						buf->data[buf->len++] = data;
				}
				else
					buf->data[buf->len++] = data;
			}
			// 判断是否接收完一帧数据
			if (buf->len >= buf->total_len)
			{
				// deg_Printf(".");
				btcomPutBusy(sFrmManager);
				if (dotUnit_flag)
				{
					dotUnit_flag = 0; // push了整合的数据包之后开始新的一包
				}
				sFrmManager->remain_frameCount--;
				// deg_Printf("isr1[%d]\n",sFrmManager->remain_frameCount);
				if (sFrmManager->remain_frameCount < 0)
					sFrmManager->remain_frameCount = 0;
				//				if((sFrmManager->remain_frameCount < (NUM_OF_FRAME*4/10))&& SysCtrl.stream_print){	//有一种情况例外：当中断存的很快，任务消化数据跟不上时，双方(任务和中断)会反复开关流控
				//					if(!get_flow_ctrl()){
				//						Bt_Flow_Ctrl(1);
				//						set_flow_ctrl(1);
				//					}
				//				}
			}
		}
		else if (buf->len == 5)
		{
			if (buf->data[2] == 0xBC)
			{
				// buf->data[buf->len++] = data;
				buf->data[buf->len++] = 0x00;
				buf->total_len = 8 + ((0x00 << 8) | (0x02)); // 2byte
			}
			else if (buf->data[2] == 0xD1) // 接收身份验证的长度实际只有8个字节，扩写后变成16字节，方便后续处理
			{
				buf->data[buf->len++] = 0x00;
				buf->total_len = 8 + ((0x00 << 8) | (0x10)); // 16byte
			}
			else if (dotUnit_flag && (buf->data[2] == 0xA2)) // 设置整合包长度
			{
				/*0x60 两包合成一包点阵数据,（此处更改后最好Bt_Get_Print_Line_Data也更改）*/
				buf->data[4] = 0x00;		 // 0x80;//0xA0;	//
				buf->data[buf->len++] = 0x6; // 0x07;//

				buf->total_len = 8 + ((buf->data[5] << 8) | (buf->data[4]));
			}
			else
			{
				buf->data[buf->len++] = data;
				// 计算帧长度
				buf->total_len = 8 + ((buf->data[5] << 8) | (buf->data[4]));
				if (buf->total_len > sizeof(buf->data))
				{
					deg_Printf("[RxIsr]length err(%d)\n", buf->total_len);
					buf->len = 0;
				}
			}
		}
		else if (buf->len >= 3)
		{
			buf->data[buf->len++] = data;
		}
		else if (buf->len == 2)
		{
			if (btcomCmdSupport(data))
			{
				// deg_Printf("data:%x\n",data);
				buf->data[buf->len++] = data;
				if (data == 0xA2)
				{
#if 1
					if (!dotUnit_flag) // 发现是第一包点阵就置位
					{
						dotUnit_flag = 1;
					}
#else
					if (SysCtrl.printer_print_mode == 0)
					{
						start_jpg_flag = 1;
						// memset(tmpdata,0,300*1024);
						// dataLen=0;
					}
#endif
				}
				else if (data == 0xA1)
				{
					if (SysCtrl.printer_print_mode == 0)
					{
						start_jpg_flag = 0;
						SysCtrl.ready_print = 1;
						SysCtrl.ready_print_time = XOSTimeGet();
						// ax32xx_sysDcacheWback((u32)BT_Pic_Addr,dataLen);
					}
				}
				else if (data == 0xBC)
				{
					buf->data[buf->len++] = 0x00;
				}
			}
			else
			{
				buf->len = 0;
				deg_Printf("[RxIsr]unknown cmd:%x\n", data);
			}
		}
		else if (buf->len == 1)
		{
			if (data == 0x78)
			{
				buf->data[buf->len++] = data;
			}
			else
			{
				buf->len = 0;
				deg_Printf("[RxIsr]second byte is not 0x78[0x%X]\n", data);
			}
		}
		else /*if (buf->len == 0)*/
		{
			if (data == 0x51)
			{
				buf->data[buf->len++] = data;
				//				buf->start_tick = XOSTimeGet();
			}
			// deg_Printf("first byte is [0x%X]\n", data);
		}
	}
	/*
		if(SysCtrl.stream_print)
			btcomService();*/
}

/**
 * 蓝牙通讯初始化
 *
 * fnCmdProc - 处理命令的回调程序
 *
 */
void btcomInit(FN_BTCMD_PROC fnCmdProc, unsigned char cmdData[], unsigned char cmdNum)
{
	// 创建接收buffer
	sFrmManager = btcomBufCreate();
	if (sFrmManager == NULL)
	{
		return;
	}

	Bt_BufManage = btdataBufCreate();

	//	BT_Pic_Addr = hal_sysMemMalloc(/*JPG_LEN_MAX*/20*1024, 64);
	//		if (BT_Pic_Addr == NULL) {
	//			btcomBufDestroy(sFrmManager);
	//			sFrmManager = NULL;
	//			return ;
	//		}
	dataLen = 0;

	// 命令处理callback
	sCmdProc = fnCmdProc;

	/*
	 * Step 1: initialize the LZO library
	 */
	if (lzo_init() != LZO_E_OK)
	{
		deg_Printf("internal error - lzo_init() failed !!!\n");
		deg_Printf("(this usually indicates a compiler bug - try recompiling\nwithout optimizations, and enable '-DLZO_DEBUG' for diagnostics)\n");
		return 3;
	}

	// 命令排序
	btcomParserInit(cmdData, cmdNum);

	// 注删蓝牙数据接收
	// hal_uartRXIsrRegister(prvRxIsr);
	// hal_uartInit();
	hal_uart1RXIsrRegister(prvRxIsr);
	hal_uart1Init();
}

/**
 * 蓝牙通讯反初始化
 *
 */
void btcomUninit(void)
{
	// 不再接收数据
	// hal_uartRXIsrRegister(NULL);
	hal_uart1RXIsrRegister(NULL);

	// 释放内存
	if (sFrmManager != NULL)
	{
		btcomBufDestroy(sFrmManager);
		sFrmManager = NULL;
	}
	//	if (BT_Pic_Addr != NULL) {
	//		hal_sysMemFree(BT_Pic_Addr);
	//		BT_Pic_Addr = NULL;
	//	}
	if (Bt_BufManage.al_addr != NULL)
	{
		hal_sysMemFree(Bt_BufManage.al_addr);
		Bt_BufManage.al_addr = NULL;
	}

	btcomParserUnInit();
	sCmdProc = NULL;
	hal_sysMemPrint();
}

/**
 * 蓝牙通讯任务处理
 *
 */
void btcomService(void)
{
	// 未进行蓝牙通讯
	if (sFrmManager == NULL)
	{
		return;
	}
	// 数据处理
	// if(!serviceFlag)
	{
		FRAME_BUF_T *buf = btcomTestBusyBuf(sFrmManager);
		//		serviceTempBuf = buf;
		//		serviceFlag = 1;
		//	}
		//	else{
		//		FRAME_BUF_T *buf = serviceTempBuf;
		//		serviceFlag = 0;

		if (buf != NULL)
		{
			if (SysCtrl.stream_drop_data)
				goto bt_dataDrop;
			u8 *data = buf->data + 6;
			int data_len = buf->total_len - 8;
#if 0
		u32 chksum;
		u8 crc;

		crc = buf->data[buf->total_len-2];
		chksum = btcomCheckSum(data, data_len, 0);
		if ((chksum&0xFF) == crc)
#endif
			{
				// prvOutputStream(buf->data,buf->total_len);	//debug
				if (sCmdProc != NULL)
				{
					sCmdProc(buf->data[2], data, data_len);
				}
			}
		bt_dataDrop:
			btcomMarkConsumed(buf);
			//		if(SysCtrl.stream_drop_data!=0){
			//			while(buf)
			//			{
			//
			//				btcomMarkConsumed(buf);
			//
			//				buf = btcomTestBusyBuf(sFrmManager);
			//			}
			//			SysCtrl.stream_drop_data=0;
			//			//deg_Printf("clean done\n");
			//		}

			if (buf)
			{

				sFrmManager->remain_frameCount++;
				if (sFrmManager->remain_frameCount > NUM_OF_FRAME)
					sFrmManager->remain_frameCount = NUM_OF_FRAME;
				if (sFrmManager->remain_frameCount > 57 && SysCtrl.stream_print)
				{
					/*if(flow_ctrl==0){

					}else{
						flow_ctrl = 0;
						Bt_Flow_Ctrl(flow_ctrl);
					}*/
				}
			}
			else
			{
				if (SysCtrl.stream_drop_data != 0)
					SysCtrl.stream_drop_data = 0;
			}
		}
		else
		{
			if (SysCtrl.stream_drop_data != 0)
				SysCtrl.stream_drop_data = 0;
		}
	}
}

/**
 * 指令处理结果的回复
 *
 * cmd - 指令
 * data - 回复的数据
 * len - 数据长度
 *
 * 返回:true - 支持；false - 不支持
 *
 */
void btcomCmdRsp(BTCOM_CMD_E cmd, const u8 *data, int len)
{
	const u8 *fragment[3];
	int frag_len[3];
	u8 header[6];
	u8 tail[2];
	int i, j;
	// deg_Printf("Rsp:0x%x len:%d\n",data[2],len);
	//  header
	header[0] = 0x51;
	header[1] = 0x78;
	header[2] = cmd;
	header[3] = 0x01;

	header[4] = len & 0xFF;
	header[5] = ((len) >> 8) & 0xFF;

	// tail
	tail[0] = (btcomCheckSum(data, len, 0) & 0xFF);
	tail[1] = 0xFF;

	fragment[0] = header;
	frag_len[0] = 6;
	fragment[1] = data;
	frag_len[1] = len;
	fragment[2] = tail;
	frag_len[2] = 2;
	for (i = 0; i < sizeof(fragment) / sizeof(fragment[0]); i++)
	{
		for (j = 0; j < frag_len[i]; j++)
		{
			// hal_uartSendData(fragment[i][j]);
			hal_uart1SendData(fragment[i][j]);
		}
	}
}

u8 *get_pic_add(void)
{
	Bt_BufManage_T *Manage;
	Manage = Get_btdataBufManage();
	if (Manage->buffer)
		return Manage->buffer;
	/*if(BT_Pic_Addr)
		return BT_Pic_Addr;*/
	return NULL;
}

u32 get_pic_w(void)
{
	return dataLen;
}

void set_pic_w(u32 pic_w)
{
	dataLen = pic_w;
}

u16 get_remainFreme(void)
{
	return sFrmManager->remain_frameCount;
}

/*
return val :1: enable flow ctrl  , 0:disable
*/
u8 get_flow_ctrl(void)
{
	return flow_ctrl;
}
void set_flow_ctrl(u8 ctrl)
{
	flow_ctrl = ctrl;
}

void btcomATcmdSend(const u8 *data, int len)
{
	const u8 *fragment;
	int frag_len;
	u8 header[6];
	u8 tail[2];
	int i, j;
	// deg_Printf("Rsp:0x%x len:%d\n",data[2],len);
	//  header
	/*
	header[0] = 0x51;
	header[1] = 0x78;
	header[2] = cmd;
	header[3] = 0x01;

	header[4] = len & 0xFF;
	header[5] = ((len)>>8) & 0xFF;

	// tail
	tail[0] = (btcomCheckSum(data, len, 0) & 0xFF);
	tail[1] = 0xFF;

	fragment[0] = header;
	frag_len[0] = 6;
	fragment[1] = data;
	frag_len[1] = len;
	fragment[2] = tail;
	frag_len[2] = 2;
*/
	fragment = data;
	frag_len = len;
	deg_Printf("len:%d\n", len);
	// for (i = 0; i < sizeof(fragment)/sizeof(fragment[0]); i++)
	{
		for (j = 0; j < frag_len; j++)
		{
			// hal_uartSendData(fragment[i][j]);
			hal_uart1SendData(fragment[j]);
		}
	}
}

// 注意清除时，可能存在一部分没接收完的(sFrmManager->working),所以需要先开启流控,等待数据都过来了，再清
void CleanISR_Btbuf(void)
{
	if (sFrmManager == NULL)
		return;

	FRAME_BUF_T *buf;

	// 清除还在接收的(sFrmManager->working)--------
	buf = sFrmManager->working;
	buf->len = 0;
	//-----------------------------------

	// 清除已经存进去的(sFrmManager->busy)--------
	buf = btcomTestBusyBuf(sFrmManager);
	while (buf)
	{
		btcomMarkConsumed(buf);
		buf = btcomTestBusyBuf(sFrmManager);
	}
	//-----------------------------------

	SysCtrl.stream_drop_data = 0;
}

void checkBtbuf(void)
{
	FRAME_BUF_T *buf;
	buf = sFrmManager->working;
	deg_Printf("working len:%d\n", buf->len);

	buf = btcomTestBusyBuf(sFrmManager);
	if (buf)
		deg_Printf("have buf\n");
	else
		deg_Printf("no buf\n");

	deg_Printf("queue remain:%d\n", Bt_BufManage.Btbuf_QueueRemain);
}
