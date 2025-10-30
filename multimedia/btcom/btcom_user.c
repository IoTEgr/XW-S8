#include "../../ax32_platform_demo/application.h"
#include "inc/btcom.h"
#include "inc/btcom_inner.h"
#include "inc/btcom_stream.h"
#include "inc/btcom_user.h"

static u32 count_printLine = 0;
static u32 Pic_Data_offset = 0;
static u16 stream_count_line = 0;
static u32 device_id[6] = {
	CONFIG_ID_DEVICE_ID1,
	CONFIG_ID_DEVICE_ID2,
	CONFIG_ID_DEVICE_ID3,
	CONFIG_ID_DEVICE_ID4,
	CONFIG_ID_DEVICE_ID5,
	CONFIG_ID_DEVICE_ID6,
};

#define BT_USER_DEBUG 0
#if BT_USER_DEBUG
#define bt_Printf(...) uart_Printf(__VA_ARGS__)
#else
#define bt_Printf(...)
#endif

/**
 * cmd：A4.  low 4 bit set print level :range 1~5.
 *
 */
void Bt_Set_Quality(const u8 *data, int len)
{
	if (data == NULL)
		return;

	//	int i;
	u16 Printer_Q;

	bt_Printf("set Quality:data :%x len:%d\n", data, len);

	count_printLine = 0; // reset line count
	set_pic_w(count_printLine);
	Pic_Data_offset = 0;

	Printer_Q = data[len - 1] & 0x0F;

	bt_Printf("set Quality: Q:%x\n", Printer_Q);

#if 1
	switch (Printer_Q)
	{
	case 1:
		SysCtrl.printer_level = 0;
		break;
	case 2:
		SysCtrl.printer_level = 1;
		break;
	case 3:
		SysCtrl.printer_level = 2;
		break;
	case 4:
		SysCtrl.printer_level = 3;
		break;
	case 5:
		SysCtrl.printer_level = 4;
		break;
	default:
		SysCtrl.printer_level = 4;
		break;
	}

	bt_Printf("show printer level:%d  %d\n", SysCtrl.printer_level, configValue2Int(/*R_ID_STR_COM_LEVEL_4*/ CONFIG_ID_BAT_OLD));
#endif
}

extern int usb_prinr;

u8 Rsp_Get_Printer_State()
{
	// 低电 > 开盖 > 缺纸 > 过热 > 正在打印
	u8 ret = 0;

	if (SysCtrl.battery <= 1) // low power
	{
		ret = 0x08;
	}
	else if (SysCtrl.paper_check == 1)
	{
		// 0x01 缺纸；0x02 纸仓开
		ret = 0x01;
	}
	else if (hal_adcGetChannel(ADC_CH_PA6) <= 316) // over heat
	{
		ret = 0x04;
	}
	else if (hal_wki1Read())
	{
		ret = 0x10;
	}
	else
	{
		if (SysCtrl.stop_print == 0)
			ret = 0x80; // when printing state
		else
			ret = 0x00; // when Idle state
	}

	deg_Printf("%s-hottime:%d,stop_print\n", __func__, hal_adcGetChannel(ADC_CH_PA6), SysCtrl.stop_print);
	if (ret != 0x80)
	{
		SysCtrl.ready_print = 0;	  // disable Bt printe flag (蓝牙打印使能位置零)
		SysCtrl.stream_drop_data = 1; // drop the Bt data (丢弃已存储的蓝牙数据帧)
		Bt_Stream_Clean_Useless();	  // drop the stream data (丢弃已存储的流队列数据)
	}
	bt_Printf("read ret 0x%x\n", ret);
	return ret;
}

/**
 * cmd：AF.
 *
 */
void Bt_Set_Energer(const u8 *data, int len)
{
	/*
	4605  5418  6375  7500   8625  9918  11406
	  1	   2     3	 	4	   5	 6	  7

	#define DENSITY_CMP 12000 //作用近似于最大值，与app发来的数据比较（app发来值：4605~11406）
	*/

	s16 printer_level = (data[1] << 8 | data[0]);
	bt_Printf("set energer-data:0x%x 0x%x %d value[%d]\n", data[0], data[1], len, printer_level);
	configSet(CONFIG_ID_PRINTER_DENSITY_L, printer_level / 100);
	configSet(CONFIG_ID_PRINTER_DENSITY_H, printer_level % 100);
	SysCtrl.printer_level = printer_level;
	/*printer_level=(DENSITY_CMP-printer_level)/1000;

	switch (printer_level)
	{
		case 7: SysCtrl.printer_level = 0;
			break;
		case 6: SysCtrl.printer_level = 1;
			break;
		case 5: SysCtrl.printer_level = 2;
			break;
		case 4: SysCtrl.printer_level = 3;
			break;
		case 3: SysCtrl.printer_level = 4;
			break;
		case 2: SysCtrl.printer_level = 5;
			break;
		case 1: SysCtrl.printer_level = 6;
			break;
		case 0: SysCtrl.printer_level = 6;
			break;
		default:
			SysCtrl.printer_level = 4;
			break;
	}*/
	bt_Printf("cmd AF set-level:%d printer_level:%d\n", SysCtrl.printer_level, printer_level);
	bt_Printf("H L [%d][%d]\n", configGet(CONFIG_ID_PRINTER_DENSITY_H), configGet(CONFIG_ID_PRINTER_DENSITY_L));
}

/*
 *
 *return :电池当前模拟电压值*10
 */
u8 Bt_CalculateVal(void)
{
	u16 ad_val;
	u32 temp, voltage;
	ad_val = board_bat_valGet(SysCtrl.battery);
	// 根据AD值计算电池当前模拟电压值，因不能用小数点所以*100计算
	bt_Printf("adVal:%d\n", ad_val);
	temp = (ad_val * 100 * (330)) / (1024 * 100);
	bt_Printf("temp:%d\n", temp);
	voltage = (temp * 590) / 200;

	// 因只需电压值*10 ，所以/10，且估算有0.2v的压降
	voltage = (voltage / 10) + 2;
	bt_Printf("voltage:%d [0x%x]\n", voltage, voltage);
	return voltage;
}

/**
 * get printer state and send
 *
 */
void Bt_Get_Printer_State(const u8 *data, int len)
{
	u8 dataResp[3];
	int _len;

	// get state
	dataResp[0] = Rsp_Get_Printer_State(); // 打印机状态
	dataResp[1] = 0x00;					   // 待定
	dataResp[2] = Bt_CalculateVal();	   // 电量模拟电压值*10

	bt_Printf("read datarsp[%x][%x][%x]\n", dataResp[0], dataResp[1], dataResp[2]);
	_len = ARRAY_NUM(dataResp);
	// send

	btcomCmdRsp(BTCOM_GET_STATE, dataResp, _len);
	bt_Printf("len:%d\n", _len);
	SysCtrl.paper_check = 0;
}

/**
 * cmd：A3. return printer state to app
 *
 */
void Bt_Printer_Set(const u8 *data, int len)
{
	if (SysCtrl.stop_print == 0)
		SysCtrl.stop_print = 1; // stop
	if (!SysCtrl.stream_print)
		Bt_Get_Printer_State(data, len);
	deg_Printf("--%s-- %d\n", __func__, SysCtrl.stream_print);
}

/**
 * cmd：A2. get line data
 *点阵数据来得快，一包一行，所以收到一包就可以开始打印了
 */
void Bt_Get_Print_Line_Data(const u8 *data, int len)
{
	if (SysCtrl.printer_print_mode != 1)
		SysCtrl.printer_print_mode = 1;

	Bt_Data_Streamin(data, len); // 由于点阵数据可能整合，所以校验可能不适用

	count_printLine = count_printLine + 2;
	set_pic_w(count_printLine);

	if (get_pic_w() == 2)
	{ // 缓了1包
		XMsgQPost(SysCtrl.sysQ, (void *)makeEvent(SYS_EVENT_1S, 0));
		SysCtrl.ready_print = 1;
		SysCtrl.stream_print = 1;
		SysCtrl.stop_print = 0;
		SysCtrl.ready_print_time = XOSTimeGet();
	}

	bt_Printf(" line:%d len:%d \n", count_printLine, len);
}

/**
 * cmd：A6. return printer state to app
 *
 */
void Bt_Set_Lcd_Screen(const u8 *data, int len)
{
}

/*
 *向APP发送流控指令
 * 1: 开启流控  , 0:关闭流控
 */
void Bt_Flow_Ctrl(u8 en)
{
	u8 send_data;
	int len;
	if (en)
	{
		send_data = 0x10;
	}
	else
	{
		send_data = 0x00;
	}
	len = sizeof(send_data);
	// deg_Printf("read datarsp:0x%x %x -%d\n",send_data,&send_data,len);
	btcomCmdRsp(BTCOM_FLOW_CONTROL, &send_data, len);
}
/**
 * cmd：A1. feed paper
 *走纸指令，收到后待队列数据清空，会停止打印
 */
void Bt_Feed(const u8 *data, int len)
{
	deamon_auto_poweroff(1);
	deamon_screen_save(1);

	SysCtrl.ready_print = 1;
	SysCtrl.stop_print = 1;
	SysCtrl.ready_print_time = XOSTimeGet();

	bt_Printf(" ready print:%d ready time:%d\n", SysCtrl.ready_print, SysCtrl.ready_print_time);
}

void Bt_Set_Printer_State(const u8 *data, int len)
{
	deamon_auto_poweroff(1);
	deamon_screen_save(1);

	bt_Printf("in cmd BC,show len:%d\n", len);
	int i;
	bt_Printf("in cmd BC,data: ");
	for (i = 0; i < len; i++)
	{
		bt_Printf("%x ", data[i]);
		if (i == 0)
		{
			switch (data[i])
			{
			case 1:
				SysCtrl.printer_level = 1;
				break;
			case 2:
				SysCtrl.printer_level = 4;
				break;
			case 3:
				SysCtrl.printer_level = 7;
				break;

			default:
				SysCtrl.printer_level = 4;
				break;
			}
		}
		else if (i == 1)
		{
			switch (data[i])
			{
			case 1:
				SysCtrl.printer_print_mode = 1; // data 1:dot matrix
				break;
			case 2:
				SysCtrl.printer_print_mode = 0; // gray
				break;

			default:
				SysCtrl.printer_print_mode = 0;
				break;
			}
		}
	}
	bt_Printf("\n");

	bt_Printf("level:%d . mode:%d\n", SysCtrl.printer_level, SysCtrl.printer_print_mode);
}

/*
 *cmd：BD.
 *马达速度相当于打印数据的消耗速度
 */
void Bt_Motor_Step_Time(const u8 *data, int len)
{
	bt_Printf("1--_[%s]_ 0x%x len:%d stremp:%d\n", __func__, data[0], len, SysCtrl.stream_print);
	bt_Printf("2--mote_step_time :%d [%d]\n", data[0], data[0] / 4);
	u32 writeConfig = data[0];
	if (!SysCtrl.stream_print) // 在未开始流打印时设置moto speed，流打印进行时忽略
	{
		if (writeConfig > 100)
			writeConfig = 100;
		else if (writeConfig < 15)
			writeConfig = 15;
		configSet(CONFIG_ID_PRINTER_MOTE_SPEED, writeConfig); // 设置马达速度
		bt_Printf("3--read cfg moto speed[%d]\n", configGet(CONFIG_ID_PRINTER_MOTE_SPEED));
	}
}

/*
 *cmd：D1.
 *
 */
void Bt_App_Ientity(const u8 *data, int len)
{
	int i;
	bt_Printf("data:[");
	for (i = 0; i < len; i++)
	{
		bt_Printf("0x%x ", data[i]);
	}
	bt_Printf("]\n");

	// switch to str
	char *Rc_code = switch_data(data, len);

	// decode and send Rsp
	encodeAction(Rc_code);

	hal_sysMemFree(Rc_code);
}
/*
 *cmd：CF.
 *灰度数据20行一包，可以先缓几包以达到流畅的效果（追求速度的可以少缓）
 */
void Bt_GetGrayZip_Data(const u8 *data, int len)
{
	bt_Printf("Ziplen:%d cline:[%d] remain[%d] streamp:%d\n", len, stream_count_line, get_remainFreme(), SysCtrl.stream_print);
	if (SysCtrl.printer_print_mode != 0)
		SysCtrl.printer_print_mode = 0;

	Bt_Stream_In(data, len);
	u16 x = 0;
	if (SysCtrl.credit_flag)
		x = 400; // 授权后，打印速度变快，多缓
	else
		x = 40;

	if (stream_count_line == x)
	{ // 提前缓了2包
		XMsgQPost(SysCtrl.sysQ, (void *)makeEvent(SYS_EVENT_1S, 0));
		SysCtrl.ready_print = 1;
		SysCtrl.stream_print = 1;
		SysCtrl.stop_print = 0;
		SysCtrl.ready_print_time = XOSTimeGet();
	}

	stream_count_line = stream_count_line + 20;
	set_pic_w(stream_count_line);
}
/*
 *cmd:BB.
 *
 */
void Bt_WriteDevice_ID(u8 *data, int len)
{
	bt_Printf("__%s__[%d] 0x%x\n", __func__, len, data[0]);
	int i;
	u8 Device_Id1[6];
	if (len > 1)
	{ // keep

		configSet(CONFIG_ID_DEVICE_ID1, data[1]);
		configSet(CONFIG_ID_DEVICE_ID2, data[2]);
		configSet(CONFIG_ID_DEVICE_ID3, data[3]);
		configSet(CONFIG_ID_DEVICE_ID4, data[4]);
		configSet(CONFIG_ID_DEVICE_ID5, data[5]);
		configSet(CONFIG_ID_DEVICE_ID6, data[6]);
	}
	else
	{ // send
		for (i = 0; i < 6; i++)
		{
			Device_Id1[i] = (u8)configGet(device_id[i]);
		}
		if ((Device_Id1[0] == 00) &&
			(Device_Id1[1] == 00) &&
			(Device_Id1[2] == 00) &&
			(Device_Id1[3] == 00) &&
			(Device_Id1[4] == 00) &&
			(Device_Id1[5] == 00))
		{ // never keep before
			memset(Device_Id1, 0, sizeof(Device_Id1));
		}

		len = 6;
		btcomCmdRsp(BTCOM_WRITE_DEVICE_ID, Device_Id1, len);
	}

	// bt Debug
	bt_Printf("Device_Id:");
	for (i = 0; i < len; i++)
	{
		bt_Printf(" [%x] %d", Device_Id1[i], i);
	}

	bt_Printf("\n");
}

u16 reset_pic_data(void)
{
	set_pic_w(0);
	Pic_Data_offset = 0;
	count_printLine = 0;
	stream_count_line = 0;
	bt_Printf("count_printLine[%d] and offset [%d]already reset!\n", count_printLine, Pic_Data_offset);
	return 0;
}

u32 Bt_Get_Printer_Moto_Speed(void)
{
	return configGet(CONFIG_ID_PRINTER_MOTE_SPEED);
}

void bt_AT_Send(void)
{
	int i;
	char *result = NULL;
	deg_Printf("1-AT_SET_BTNAME:%s\n", AT_SET_BTNAME);
	// string_to_hex_manual(AT_SET_BTNAME,result);
	memset(result, 0, 32);

	// strcat(result,AT_SET_BTNAME);
	strcat(result, AT_CHECKMODE);
	strcat(result, "\r");
	// strcat(result,"JJRRXX\r");
	for (i = 0; i < strlen(result); i++)
	{
		deg_Printf("%x ", result[i]);
	}
	deg_Printf("\n");
	btcomATcmdSend(result, strlen(result));
}
