#include "application.h"
#include "../multimedia/btcom/inc/btcom.h"
#include "../multimedia/btcom/inc/btcom_user.h"
typedef void (*BTCMD_CALLBACK)(const u8 *data, int len);

typedef struct
{
	BTCOM_CMD_E cmd;
	BTCMD_CALLBACK fnCmdProc;
} BTCMD_PROC_T;
u32 auto_power;
// 全局数据
u8 sCmdParser[] = {
	BTCOM_GET_STATE,		   // = 0xA3,
	BTCOM_SET_QUALITY,		   // = 0xA4,
	BTCOM_SEND_BITMAP,		   // = 0xA5,
	BTCOM_FEED,				   // = 0xA1,
	BTCOM_SEND_LINE_DATA,	   // = 0xA2,
	BTCOM_GET_DEVINFO,		   // = 0xA8,
	BTCOM_FLOW_CONTROL,		   // = 0xAE,
	BTCOM_SET_ENERGE25,		   // = 0xAF,
	BTCOM_SEND_LINE_DATA_ZIP,  // = 0xBF,
	BTCOM_SET_PRINT_SPEED,	   // = 0xBE,
	BTCOM_SET_MOTOR_STEP_TIME, // = 0xBD,
	// BTCOM_PRINT_IGNORE,        // = 0xBC,
	BTCOM_SET_LED_SCREEN,	  // = 0xA6,
	BTCOM_SET_FLAG_BIT,		  // = 0xBC,
	BTCOM_WRITE_DEVICE_ID,	  // = 0xBB,
	BTCOM_APP_IDENTITY,		  // = 0xD1,
	BTCOM_SEND_GRAY_DATA_ZIP, // = 0xCF,
	BTCOM_SET_DENSITY,		  // = 0xF2,
};

// must sorted by cmd !!!!
static const BTCMD_PROC_T s_btcmd_procs[] = {
	{BTCOM_FEED, Bt_Feed},										// = 0xA1,
	{BTCOM_SEND_LINE_DATA, Bt_Get_Print_Line_Data},				// = 0xA2,
	{BTCOM_GET_STATE, /*Bt_Get_Printer_State*/ Bt_Printer_Set}, // = 0xA3,
	{BTCOM_SET_QUALITY, Bt_Set_Quality},						// = 0xA4,
	{BTCOM_SEND_BITMAP, NULL},									// = 0xA5,
	{BTCOM_SET_LED_SCREEN, Bt_Set_Lcd_Screen},					// = 0xA6,
	{BTCOM_GET_DEVINFO, NULL},									// = 0xA8,
	{BTCOM_FLOW_CONTROL, NULL},									// = 0xAE,
	{BTCOM_SET_ENERGE25, Bt_Set_Energer},						// = 0xAF,
	{BTCOM_WRITE_DEVICE_ID, Bt_WriteDevice_ID},					// = 0xBB,
	//{BTCOM_PRINT_IGNORE,       NULL}, // = 0xBC,
	{BTCOM_SET_FLAG_BIT, Bt_Set_Printer_State},		 // = 0xBC,
	{BTCOM_SET_MOTOR_STEP_TIME, Bt_Motor_Step_Time}, // = 0xBD,
	{BTCOM_SET_PRINT_SPEED, NULL},					 // = 0xBE,
	{BTCOM_SEND_LINE_DATA_ZIP, NULL},				 // = 0xBF,
	{BTCOM_SEND_GRAY_DATA_ZIP, Bt_GetGrayZip_Data},	 // = 0xCF,
	{BTCOM_APP_IDENTITY, Bt_App_Ientity},			 // = 0xD1,
	{BTCOM_SET_DENSITY, NULL},						 // = 0xF2,
};

static void appCmdProc(BTCOM_CMD_E cmd, const u8 *data, int len)
{
	int begin, end, mid;
	BTCMD_CALLBACK fnProc = NULL;

	// example code
	begin = 0;
	end = sizeof(s_btcmd_procs) / sizeof(s_btcmd_procs[0]) - 1;
	while (begin <= end)
	{
		mid = (begin + end) / 2;
		if (cmd == s_btcmd_procs[mid].cmd)
		{
			fnProc = s_btcmd_procs[mid].fnCmdProc;
			break;
		}
		else if (cmd > s_btcmd_procs[mid].cmd)
		{
			begin = mid + 1;
		}
		else
		{
			end = mid - 1;
		}
	}

	if (fnProc != NULL)
	{
		fnProc(data, len);
	}
}

void taskAppOpen(uint32 arg)
{
	deg_Printf("App open remain:0x%x\n", hal_sysMemRemain());

	btcomInit(appCmdProc, sCmdParser, sizeof(sCmdParser) / sizeof(sCmdParser[0]));
	uiOpenWindow(&AppWindow, 0);
	auto_power = configGet(CONFIG_ID_AUTOOFF);

	configSet(CONFIG_ID_AUTOOFF, R_ID_STR_TIM_60MIN);
	deg_Printf("b:%d a:%d\n", auto_power, configGet(CONFIG_ID_AUTOOFF));

	// hal_sysMemPrint();
}

void taskAppService(uint32 arg)
{
	if (!SysCtrl.stream_print)
		btcomService();
}
void taskAppClose(uint32 arg)
{
	deg_Printf("App exit.\n");
	btcomUninit();
	configSet(CONFIG_ID_AUTOOFF, auto_power);
	if (SysCtrl.printer_print_mode)
		configSet(CONFIG_ID_PRINTER_MODE, R_ID_STR_SET_PRINT_DOT);
	else
		configSet(CONFIG_ID_PRINTER_MODE, R_ID_STR_SET_PRINT_GRAY);

	userConfigSave();
	configSys(CONFIG_ID_AUTOOFF);
}
sysTask taskApp =
	{
		"App",
		(u32)NULL,
		taskAppOpen,
		taskAppClose,
		taskAppService,
};
