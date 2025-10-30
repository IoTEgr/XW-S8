#ifndef __BTCOM_USER__
#define __BTCOM_USER__

#define AT_RESET            "AT+RESET\n"          //复位
#define AT_FACTORYRESET     "AT+FACTORYRESET\n"   //恢复出厂设置
#define AT_ATMODE           "AT+ATMODE\n"         //进入AT模式
//----check
#define AT_CHECK_BTIEEENAME "AT+IEEENAME?"      //查询蓝牙模块的蓝牙打印设备名
#define AT_CHECK_BTPIN 	    "AT+PIN?\n"           //查询蓝牙PIN码
#define AT_CHECK_BTNAME 	"AT+NAME?"          //查询蓝牙模块的蓝牙广播名
#define AT_CHECK_BAUD 	    "AT+BAUD?\n"          //查询蓝牙的串口波特率
#define AT_CHECKMODE 	    "AT+CHECKMODE"      //查询蓝牙模式
#define AT_CHECKLBDADDR 	"AT+LBDADDR?"       //查询蓝牙MAC地址
//----set
#define AT_SET_BTNAME           "AT+NAME="      //设置蓝牙模块的蓝牙广播名
#define AT_SET_BTPIN            "AT+PIN="       //设置蓝牙PIN码
#define AT_SET_BAUD             "AT+BAUD="      //设置蓝牙的串口波特率
#define AT_SET_BTIEEENAME       "AT+IEEENAME="  //设置蓝牙模块的蓝牙打印设备名

/**
 * cmd：A3. return printer state to app
 *
 */
void Bt_Get_Printer_State(const u8 *data, int len);

/**
 * cmd：A4.  low 4 bit set print level :range 1~5.
 *
 */
void Bt_Set_Quality(const u8 *data, int len);

void Bt_Get_Print_Line_Data(const u8 *data, int len);

void Bt_Set_Lcd_Screen(const u8 *data, int len);

void Bt_Feed(const u8 *data, int len);

void Bt_Set_Printer_State(const u8 *data, int len);

void Bt_App_Ientity(const u8 *data, int len);

u16 reset_pic_data(void);

void encodeAction(char * Rc);

void string_to_hex_manual(const char *str,u8 *out_data);

void Bt_GetGrayZip_Data(const u8 *data, int len);

void Bt_Flow_Ctrl(u8 en);

void Bt_Set_Energer(const u8 *data, int len);

u8 *get_dot_tmp_add(void);

void Bt_Motor_Step_Time(const u8 *data, int len);

void Bt_WriteDevice_ID(u8 *data,int len);


void Bt_Printer_Set(const u8 *data, int len);

u32 Bt_Get_Printer_Moto_Speed(void);


#endif  // __BTCOM_USER__

