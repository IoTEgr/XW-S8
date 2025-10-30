#include "../../../ax32_platform_demo/application.h"
#include "../inc/btcom.h"
#include "../inc/btcom_inner.h"
#include "../inc/btcom_user.h"
#include "../inc/btcom_stream.h"

#define APP_IDENTITY		1	//app身份验证开关(不需要时关掉)

//---------------------Variable-----------------//
static u32 ad_tmVal = 0;	//温度平均值
static u32 tm_countSum=0;	//温度的和
static s32 p_tm_hot_time=0;	//recode pre hot_time val


//---------------------Identity APP Code-----------------//
#if APP_IDENTITY
 // 将十六进制字符转换为整数值
 int hexToInt(char c) {
	 if (isdigit(c)) return c - '0';
	 return tolower(c) - 'a' + 10;
 }

 // 对字符串进行右移操作
 void rightShift(char *str) {
	 int len = strlen(str);
	 int i;
	 char *tmpLast=str[len - 1];	//save last 
	 for (i = len - 1; i > 0; --i) {
		 str[i] = str[i - 1];
	 }
	 str[0] = tmpLast;
	 //deg_Printf("[%s] %s \n",__func__,str);
 }
 
 // 编码函数
 char* encodeWithKeyAndRc(const char *key, const char *rc) {
	 // 动态分配内存以存储结果字符串
	char *result1 = (char *)hal_sysMemMalloc(64,32);
	memset(result1,0,64);
	 if (!result1) return NULL;
 	int i;
	//deg_Printf("Bi2s:");
	 for (i = 0; i < strlen(key); ++i) {
		 int keyInt = hexToInt(key[i]);
		 int rcInt = hexToInt(rc[i]);
		 int resultInt = keyInt * rcInt;
 
		 // 循环直到找到第一个非零位
		 int tmpResult;
		 do {
			 tmpResult = resultInt % 16;
			 resultInt /= 16;
		 } while (tmpResult == 0);
 
		 // 将结果添加到字符串
		// sprintf(&result[strlen(result)], "%X", tmpResult);
		if(tmpResult > 9 )
		{
			char *tmpstr = NULL;
			if(tmpResult == 10)
				tmpstr = "A";
			else if(tmpResult == 11)
				tmpstr = "B";
			else if(tmpResult == 12)
				tmpstr = "C";
			else if(tmpResult == 13)
				tmpstr = "D";
			else if(tmpResult == 14)
				tmpstr = "E";
			else if(tmpResult == 15)
				tmpstr = "F";
			strcat(result1,tmpstr);
		}else
		 	intToString(&result1[strlen(result1)], 16, tmpResult);
	 }
	 return result1;
 }

 // 主编码函数
 void encodeAction(char * Rc) 
{
	 //XW身份验证Key ,勿动!!
	 const char *key = "269C83C1E32421E7";
	 bts_Printf("show Rc:%s\n",Rc);
	 const char *rc = Rc;
	 char *result = (char *)hal_sysMemMalloc(64,32);
	 if (!result) return;
 
	 strcpy(result, rc);
 	int i,j=0;
	 for (i = 1; i <= 10; ++i) {
		 if (i > 1) {
			 rightShift(result);
		 }
		 char *encoded = encodeWithKeyAndRc(key, result);
		 bts_Printf("result%d = %s\n", i, encoded);
		 strcpy(result,encoded);
		 hal_sysMemFree(encoded); // 释放之前分配的内存
	 }
	 bts_Printf("key = %s \n rc = %s \nresult = %s\n", key, rc,result);

	 u8 *hex_data =(char *)hal_sysMemMalloc(64,32);

	 //转成Hex
	 string_to_hex_manual(result,hex_data);
	 u8 *send_data;
	 bts_Printf("\n");
	 bts_Printf("X out_data:");
	 
	 //将16个字节缩成8个字节
	 j=0;
	 for ( i = 0; i < 16; i++) {
	 	send_data[j]=((hex_data[i]<<4)|(hex_data[++i]&0x0f));
		 bts_Printf(" %x", send_data[j]);
		j++;
    }

	//将版本号打包发送,4字节
	u8 Version_Num[4];
	strncpy(Version_Num,SOFTWARE_VERSION,4);
			
	for(i=8,j=0;i<12;i++,j++){
		send_data[i]=Version_Num[j];
		bts_Printf(" %x", send_data[i]);
 	}
	 bts_Printf("\n");
	 
	//发送
	btcomCmdRsp(BTCOM_APP_IDENTITY,send_data,12);
	 
	hal_sysMemFree(hex_data); // 释放字符串的内存
	hal_sysMemFree(result); 
 }

char *switch_data(const u8 *data,int len)
{
	//--
	char *sw_data =(char *)hal_sysMemMalloc(64,32);
	memset(sw_data,0,64);
	int i;
	for(i=0;i<len;i++)
	{
		if(data[i] > 9 )
		{
			char *tmpstr = NULL;
			if(data[i] == 10)
				tmpstr = "A";
			else if(data[i] == 11)
				tmpstr = "B";
			else if(data[i] == 12)
				tmpstr = "C";
			else if(data[i] == 13)
				tmpstr = "D";
			else if(data[i] == 14)
				tmpstr = "E";
			else if(data[i] == 15)
				tmpstr = "F";
			strcat(sw_data,tmpstr);
		}else
		 	intToString(&sw_data[strlen(sw_data)], 16, data[i]);
		 bts_Printf(" %d",data[i]);
	}
	bts_Printf("\nswdata[%s]\n",sw_data);
	return sw_data;
}

 void string_to_hex_manual(const char *str,u8 *out_data) {
	 while (*str) {
		 if (isdigit(*str)) {
		 	*out_data = *str - '0';
			 //deg_Printf("-%x", *out_data); // 数字字符直接转换
		 } else if (*str >= 'A' && *str <= 'Z') {
		 	*out_data = *str - 'A' + 10;
			// deg_Printf("[%x]", *out_data); // 大写字母转换
		 } else if (*str >= 'a' && *str <= 'z') {
		 	*out_data =  *str - 'a' + 10;
			// deg_Printf("=%x", *out_data); // 小写字母转换
		 } else if (*str == '+') {
            *out_data = 0x2B;  // '+' 转换为 0x2B
            // deg_Printf("+%x", *out_data);
        } else if (*str == '=') {
            *out_data = 0x3D;  // '=' 转换为 0x3D
            // deg_Printf("=%x", *out_data);
        } else if (*str == '?') {
            *out_data = 0x3F;  // '?' 转换为 0x3F
            // deg_Printf("?%x", *out_data);
        } else if (*str == '\r') {
            *out_data = 0x0D;  // '\r' 转换为 0x0D
            // deg_Printf("\\r%x", *out_data);
        } else if (*str == '\n') {
            *out_data = 0x0A;  // '\n' 转换为 0x0A
            // deg_Printf("\\n%x", *out_data);
        }else {
			 deg_Printf("Invalid character: %c\n", *str);
			 return;
		 }
		 str++;
		 out_data++;
	 }
	 deg_Printf("\n");
 }

 void stringToHex(const char *input, u8 *output, int *outputLength) {
	int i;
	 *outputLength = strlen(input);
	 for (i = 0; i < *outputLength; i++) {
		 output[i] = (int)input[i];
	 }
 }
#endif
 //-----------------------------------------------//

/*
*
*读写温度ADC平均值
*/
u32 Bt_Stream_getADtm(void)
{
	return ad_tmVal;	
}

void Bt_Stream_setADtm(u32 val)
{
	ad_tmVal=val;	
}

/*
*
*累加计算温度
*/
u32 Bt_Stream_Adjtm(u32 oldVal,int count,u16 h,u32 r_val)
{
	u32 adcVaule = 0;

	r_val = (u32)hal_adcGetChannel2(ADC_CH_PA6,r_val);
	if(r_val){
		*(u32*)oldVal = adcVaule = r_val;
		//deg_Printf("the r_val == %d\n",r_val);
	}else{
		adcVaule = *(u32*)oldVal;
		//deg_Printf("the r_val *********\n");
	}
	
	tm_countSum += adcVaule;
	if(count==h-1)	//if(i==PRINTER_W-1)
	{
		tm_countSum=tm_countSum/(h/4);
		Bt_Stream_setADtm(tm_countSum);
		tm_countSum = 0;	//reset
	}

	return r_val;
}

/*
*
*复位温度参数,为下一次做准备
*/
void Bt_Stream_HottimeRest(void)
{
	p_tm_hot_time = 0;
}

/*
*up/low_limit	:上/下限
*change_range	:每次的变化范围
*设置hot_time变化的上下限
*/
void Bt_Stream_HottimeAdj(S32 hot_time,u8 change_range,u8 up_limit,u8 low_limit)
{
	if(p_tm_hot_time!=0)	//设置补偿上下限, (p-x < tm < p+x)
	{	
		if(hot_time >= (p_tm_hot_time+up_limit))
		{
			hot_time = (p_tm_hot_time+change_range);
		}else if(hot_time <= (p_tm_hot_time-low_limit))
		{
			hot_time = (p_tm_hot_time-change_range);
		}
	}
	p_tm_hot_time=hot_time;
}

/*
*
*清理已入队的蓝牙数据
*/
void Bt_Stream_Clean_Useless(void)
{
	u32 buff,size;
	s32 syn_cnt, syn_cnt_next;
	Bt_BufManage_T* Manage = Get_btdataBufManage();
	while(1)
	{
		if(hal_streamOut(&Manage->vid_s,&buff,&size,&syn_cnt,&syn_cnt_next)<0)
		{
			break;
		}
		//hal_streamfree(&Manage->vid_s);
		Bt_Stream_Free(&Manage->vid_s);
	}
	Manage->Btbuf_QueueRemain = BT_BUFFER_NUM;
	btcomService();
	bts_Printf("bt stream data clean finish, reinit queuenum:%d\n",Manage->Btbuf_QueueRemain);
}

/*
*
*蓝牙数据入队(不带校验)
*/
int Bt_Data_Streamin(const u8 *data,int len)
{
	Bt_BufManage_T* Manage = Get_btdataBufManage();
	u32 addr;
	u8* str;
	u8 ret=0;
	int length;
	
	length  = (len+0x3f)&(~0x3f);
	addr = hal_streamMalloc(&Manage->vid_s,length);
	Manage->curBuffer = addr;
	if(addr==NULL){
		deg_Printf("bt_manageBuf err/n");
		//hal_streamIn_Drop(&Manage->vid_s);
		return -1;
	}
	// else{
	// 	deg_Printf("bt_manageBuf malloc succ manager->buffer[0x%x] manager->curBuffer[0x%x]/n",manager_data.buffer,manager_data.curBuffer);
	// 	manager_data.Btbuf_QueueRemain--;
	// }

	memcpy((u8*)Manage->curBuffer,data,len);
	
	ax32xx_sysDcacheWback(Manage->curBuffer,length);
	ret=hal_streamIn(&Manage->vid_s,Manage->curBuffer,length,data[len-1],0);
	u8*data1 = (u8*)Manage->curBuffer;
	//bts_Printf("******[%x][%x][%x][%x][%x][%x]-[%x][%x][%x][%x][%x][%x]****\n",data1[0],data1[1],data1[2],data1[3],data1[4],data1[5],data1[len-5],data1[len-4],data1[len-3],data1[len-2],data1[len-1],data1[len]);

	if(ret){
		deg_Printf("streamin fail _%s_ [%d]",__func__,__LINE__);
	}else if(ret == 0){
		//bts_Printf("*********************[%d]-[%x]***************\n",len,Manage->curBuffer);
		//bts_Printf("*****************-%d in &**************************\n",ret);
	}
	//addr = hal_streamMalloc(&Manage->vid_s,BT_BUFFER_EACH_LEN);
	if(addr)
	{
		Manage->curBuffer = addr;
		Manage->Btbuf_QueueRemain--;
		if(Manage->Btbuf_QueueRemain<0)
			Manage->Btbuf_QueueRemain = 0;
		if(Manage->Btbuf_QueueRemain <= (BT_BUFFER_NUM*4/10))	//when remain num < 30% ,flow control
		{
			if(!get_flow_ctrl())	//enable flow control ,tell app stop sending data
			{
				Bt_Flow_Ctrl(1);
				set_flow_ctrl(1);
				bts_Printf("NedFlowCtrl!\n");
			}
		}
		
		//mjpegEncCtrl.drop = 0;
		bts_Printf("malloc succ\n");
		return 0;
	}
	else
	{
		//mjpegEncCtrl.drop = 1;
		hal_streamIn_Drop(&Manage->vid_s);
		deg_Printf("streamin drop!\n");
	}

	
}

/*
*
*蓝牙数据入队(带校验)
*/
void Bt_Stream_In(const u8 *data, int len)
{
	u8 crc_cmp=btcomCheckSum(data, len, 0);	//CRC校验，检测数据是否有错（需耗时间，不追求数据正确可以注释）
	//deg_Printf("[%d]-crc_cmp:%x = %x , ",LineNum,crc_cmp,data[len]);
	bts_Printf("ZL:%d ,uZL:%d ,Len:%d ,",(data[3]<<8)|data[2],(data[1]<<8)|data[0],len);
	bts_Printf("[%x][%x][%x][%x][%x][%x]-[%x][%x][%x][%x][%x][%x]\n",data[0],data[1],data[2],data[3],data[4],data[5],data[len-5],data[len-4],data[len-3],data[len-2],data[len-1],data[len]);


	if(data[len]==crc_cmp)
		Bt_Data_Streamin(data,len);
}

/*
*
*蓝牙数据出队
*return val:1:正常取到数据 3:队列无数据退出 4:超时退出 5:继续轮询
*/
u8 Bt_Stream_Out(Bt_BufManage_T *Manage,u32 *buff,u32 *size,u32 *tickTime)
{
	s32 syn_cnt, syn_cnt_next;
	u8* str;
	Bt_BufManage_T* Manage1 = Get_btdataBufManage();
	if(hal_streamOut(&Manage1->vid_s,buff,size,&syn_cnt,&syn_cnt_next)<0)
	{
		if(SysCtrl.stop_print){
			deg_Printf("stop_Print:%d %d\n",SysCtrl.stop_print,__LINE__);
			return 3;
		}
		if(XOSTimeGet() - *tickTime > TIME_OUT){
			deg_Printf("[%s]:errtimeout\n",__func__);
			return 4;
		}

		return 5;
	}else {
		 u8* str1 = *buff;
		//  bts_Printf("[%x][%x][%x][%x][%x][%x]-[%x][%x][%x][%x][%x][%x][%x]\n",str1[0],str1[1],str1[2],str1[3],str1[4],str1[5],str1[*size-5],str1[*size-4],str1[*size-3],str1[*size-2],str1[*size-1],str1[*size],str1[*size+1]);

		// if(SysCtrl.printer_print_mode==0)//gray mode ,need crc
		// {
		// 	str = *buff;
		// 	u8 crc_cmp = btcomCheckSum(str, *size, 0);
			
		// 	bts_Printf("Out_size:%d addr:0x%x ,",*size,buff[0]);			
		// 	bts_Printf("ZL:%d ,uZL:%d .",str[2]|(str[3]<<8),str[0]|(str[1]<<8));
		// 	bts_Printf("[%x][%x][%x][%x][%x][%x]-[%x][%x][%x][%x][%x][%x][%x]\n",str[0],str[1],str[2],str[3],str[4],str[5],str[*size-5],str[*size-4],str[*size-3],str[*size-2],str[*size-1],str[*size],str[*size+1]);
		// 	bts_Printf("Out-crc_cmp:%x = %x syn[%x],",crc_cmp,str[((str[3]<<8)|str[2])+4],syn_cnt);
		// 	bts_Printf("ziplen:%d ziplen+4:%d \n",((str[3]<<8)|str[2]),((str[3]<<8)|str[2])+4);
		// }
		// bts_Printf("dataSucc:%d\n",*size);
		Bt_Stream_Free(&Manage1->vid_s);
		bts_Printf("size:%d addr:0x%x\n",*size,buff);
		ax32xx_sysDcacheWback((u32)buff,*size);
		return 1;
	}
}

/*
*
*蓝牙数据释放
*/
u8 Bt_Stream_Free(Stream_Head_T *head)
{
	hal_streamfree(head);
}

/*
*蓝牙数据解压
*return val: 0:success  -1:fail
*/
u8  Bt_Stream_Decompress(Bt_BufManage_T *Manage,u32 src_buf,u8 *outStr,u8 *dest_buf,u8 *dest_len)
{
	u16 zipLen,unZipLen,i,j;
	u32 out_len=0;
	u8 *str = src_buf;
	u8 *outStr1 = dest_buf;
	
	zipLen = (str[3]<<8)|str[2];
	unZipLen = (str[1]<<8)|str[0];
	bts_Printf("the len == %d,%d addr:0x%x\n",zipLen,unZipLen,src_buf);
	
	int r = lzo1x_decompress(str+4,zipLen,outStr,&out_len,NULL);
	if (r == LZO_E_OK ){
		//bts_Printf("decompressed %d bytes back into %d bytes\n",
			//(unsigned long) zipLen, (unsigned long) out_len);
		
	}
	else
	{
		// this should NEVER happen
		deg_Printf("internal error - decompression failed: %d\n", r);
		Manage->Btbuf_QueueRemain++;
		return -1;
	}
	
	u8 h1  = unZipLen/192;
	for(i=0;i<h1;i++){
	for(j=0;j< 192;j++){
		*(outStr1+i*384+j*2) = 255-(*(outStr+i*192+j)&0x0f)<<4;
		*(outStr1+i*384+j*2+1) = 255-(*(outStr+i*192+j)&0xf0); 
		}
	}
	
	*dest_len=h1;
	
	//bts_Printf("[%s]:outlen:%d outStr1:0x%x dest:0x%x\n",__func__,out_len,outStr1,dest_buf);
	return 0;
}

