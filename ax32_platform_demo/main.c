
/****************************************************************************
**
**                              MAIN
** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
**** **                  MAIN ENTER  OF SYSTEM
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : main.c
* Author      : Mark.Douglas
* Version     : V100
* Date        : 09/22/2016
* Description :
*
* History     :
* 2016-09-22  :
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "application.h"
#include "../hal/inc/hal_uart.h"
#include "version.h"

System_Ctrl_T SysCtrl;

#if (4 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0, RES_FRAME1, RES_FRAME2, RES_FRAME3};

#elif (18 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0, RES_FRAME1, RES_FRAME2, RES_FRAME3, RES_FRAME4,
								  RES_FRAME5, RES_FRAME6, RES_FRAME7, RES_FRAME8, RES_FRAME9,
								  RES_FRAME10, RES_FRAME11, RES_FRAME12, RES_FRAME13,
								  RES_FRAME14, RES_FRAME15, RES_FRAME16, RES_FRAME17};
#elif (9 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0, RES_FRAME1, RES_FRAME2, RES_FRAME3, RES_FRAME4,
								  RES_FRAME5, RES_FRAME6, RES_FRAME7, RES_FRAME8};
#elif (10 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0, RES_FRAME1, RES_FRAME2, RES_FRAME3, RES_FRAME4,
								  RES_FRAME5, RES_FRAME6, RES_FRAME7, RES_FRAME8, RES_FRAME9};
#elif (11 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0, RES_FRAME1, RES_FRAME2, RES_FRAME3, RES_FRAME4,
								  RES_FRAME5, RES_FRAME6, RES_FRAME7, RES_FRAME8, RES_FRAME9,
								  RES_FRAME10};
#elif (20 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0, RES_FRAME1, RES_FRAME10, RES_FRAME11, RES_FRAME12,
								  RES_FRAME13, RES_FRAME14, RES_FRAME15, RES_FRAME16, RES_FRAME17,
								  RES_FRAME18, RES_FRAME19, RES_FRAME2, RES_FRAME3, RES_FRAME4,
								  RES_FRAME5, RES_FRAME6, RES_FRAME7, RES_FRAME8, RES_FRAME9};
#elif (22 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {
	RES_FRAME0, RES_FRAME1, RES_FRAME2, RES_FRAME3, RES_FRAME4,
	RES_FRAME5, RES_FRAME6, RES_FRAME7, RES_FRAME8, RES_FRAME9,
	RES_FRAME10, RES_FRAME11, RES_FRAME12, RES_FRAME13, RES_FRAME14,
	RES_FRAME15, RES_FRAME16, RES_FRAME17, RES_FRAME18, RES_FRAME19,
	RES_FRAME20, RES_FRAME21 /*,RES_FRAME22,RES_FRAME23,RES_FRAME24,
	  RES_FRAME25,RES_FRAME26*/
};

#elif (27 == ICON_FRAME_NUM)
INT32S R_FRAME[ICON_FRAME_NUM] = {RES_FRAME0, RES_FRAME1, RES_FRAME2, RES_FRAME3, RES_FRAME4,
								  RES_FRAME5, RES_FRAME6, RES_FRAME7, RES_FRAME8, RES_FRAME9,
								  RES_FRAME10, RES_FRAME11, RES_FRAME12, RES_FRAME13, RES_FRAME14,
								  RES_FRAME15, RES_FRAME16, RES_FRAME17, RES_FRAME18, RES_FRAME19,
								  RES_FRAME20, RES_FRAME21, RES_FRAME22, RES_FRAME23, RES_FRAME24,
								  RES_FRAME25, RES_FRAME26};

#else // 20

#endif

static MSG_T sysDeviceQStack[SYSDEVICE_Q_SIZE];
extern sysTask taskPowerOff;
extern sysTask taskVideoRecorde;
extern sysTask taskPhotoEncode;
extern sysTask taskPlayBack;
extern sysTask taskAudioPlayer;
extern sysTask taskUsbDevice;
extern sysTask taskMainMenu;
extern sysTask taskGameMenu;
extern sysTask taskGame;
extern sysTask taskSettingMenu;
#if BT_MODE // MAIN_MENU_UI
extern sysTask taskApp;
#endif

#if 0 //(MXWAPP_PRINTER_SUPPORT == 1)
XWork_T *  Mxw_liukong_handle=NULL;
#endif

extern msgDealInfor sysUnshieldedMsgDeal[];
void registerTask(void)
{
	taskRegister(TASK_POWER_OFF, &taskPowerOff);
	taskRegister(TASK_VIDEO_RECORD, &taskVideoRecorde);
	taskRegister(TASK_PHOTO_ENCODE, &taskPhotoEncode);
	taskRegister(TASK_PLAY_BACK, &taskPlayBack);
	taskRegister(TASK_AUDIO_PLAYER, &taskAudioPlayer);
	taskRegister(TASK_USB_DEVICE, &taskUsbDevice);
	taskRegister(TASK_MAIN_MENU, &taskMainMenu);
	taskRegister(TASK_GAME_MENU, &taskGameMenu);
	taskRegister(TASK_GAME, &taskGame);
	taskRegister(TASK_SETTING_MENU, &taskSettingMenu);
#if BT_MODE // MAIN_MENU_UI
	taskRegister(TASK_APP, &taskApp);
#endif
}

bool sysPowerOn = false;
/*******************************************************************************
 * Function Name  : init
 * Description    : initial system hardware & task
 * Input          : *
 * Output         : None
 * Return         : None
 *******************************************************************************/
extern int uninit(void); // taskPowerOff.c
extern void recordTimeCount1S(void);
XWork_T *recordIncrease1S = NULL;
extern int usb_bat_draw_first_pic;

#if (MXWAPP_PRINTER_SUPPORT == 1)
void app_resume_cam_set(void)
{
	dispLayerSetPIPMode(DISP_PIP_DISABLE); // app_lcdCsiVideoShowStop();
	hal_sysDelayMS(100);
	dispLayerSetPIPMode(SysCtrl.pip_mode); // app_lcdCsiVideoShowStart();
}
#endif

int init(void)
{
	// debgchar('0');
	debg("****************CLKTUN:%x, OUT %x, IN %x***************\n", CLKTUN, (CLKTUN >> 7) & 0x1f, CLKTUN & 0x1f);
	debg("****************DLLCON:%x, CH1 %x, CH2 %x***************\n", DLLCON, (DLLCON >> 25) & 0xf, (DLLCON >> 21) & 0xf);
	debg("CHIP_ID:%x\n", CHIP_ID);
	deg_Printf("read 720P or VGA:[%d]/t read watermark x[%d] y[%d]", RESOLUTION, WATERMAKE_SET_X_POS, WATERMAKE_SET_Y_POS);
	hal_vddWKOEnable(1); // hold power
	hal_wkiWakeup(0);
	SysCtrl.PE0_stat = 0;
	hal_sysInit(); // initial system for free run
#if HAL_CFG_EN_DBG == 1
	hal_uartInit(); // 注意作为输入的时候不要有串口打印'
#else
	hal_gpioInit(GPIO_PA, GPIO_PIN7, GPIO_INPUT, GPIO_PULL_FLOATING);
#endif
	uart_Printf("[V%d.%d]build time:%s\n", VERSION_MAIN, VERSION_SUB, VERSION_TIME);
	//----------initial system work queue.work queues are isr callback function
	taskInit();
	registerTask();
	XOSInit();
	//----------start system work queue for system check & deamon service
	hal_timerStart(HAL_CFG_OS_TIMER, X_CFG_TICK, XOSTickService); // 1000 hz
#if HAL_CFG_ISP_SUPPORT
	XWorkCreate(15 * X_TICK, isp_process_check); // isp process
#endif
	recordIncrease1S = XWorkCreate(1000 * X_TICK, recordTimeCount1S);
	deg_Printf("system power on\n");
	memset(&SysCtrl, 0, sizeof(System_Ctrl_T));

	/* Debug: print SysCtrl address and offsets of key fields to detect layout/ABI issues */
	taskInfor(1);

	// DAC_PA_MUTE_INIT();
	// DAC_PA_MUTE_OFF();
	//----------board initial,LCD,LED,SPI,ADC,DAC....
	boardInit(NULL); // initial main board
					 //----------get board device ioctrl handler

	SysCtrl.bfd_battery = boardOpen(DEV_NAME_BATTERY);
	//    SysCtrl.bfd_gsensor = boardOpen(DEV_NAME_GSENSOR);
	SysCtrl.bfd_usb = boardOpen(DEV_NAME_USB);
	SysCtrl.bfd_key = boardOpen(DEV_NAME_ADKEY);
	SysCtrl.bfd_sdcard = boardOpen(DEV_NAME_SDCARD);
	SysCtrl.bfd_led = boardOpen(DEV_NAME_LED);
	SysCtrl.bfd_lcd = boardOpen(DEV_NAME_LCD);
	// SysCtrl.bfd_ir 		= boardOpen(DEV_NAME_IR);
	SysCtrl.bfd_usensor = -1; // boardOpen(DEV_NAME_USENSOR);
	SysCtrl.powerflag = POWERON_FLAG_MAX;

	if (SysCtrl.bfd_battery < 0)
	{
		// uninit();
	}
#if HALL_EN
	SysCtrl.bfd_hall = boardOpen(DEV_NAME_HALL);
#endif

	//-----------welcome
	deg_Printf("\n");
	deg_Printf(">>---------------------------------------------<<\n");
	deg_Printf(">>- Welcome.BuildWin SZ LTD.CO-----------------<<\n");
	deg_Printf(">>- VIDEO PROJECT TEAM.VISION------------------<<\n");
	deg_Printf(">>- %s --------------------<<\n", SYSTEM_VERSION);
	deg_Printf(">>---------------------------------------------<<\n");
	//----------nv fs for read only resource
	nv_init();
	//----------user menu configure set
	userConfigInitial();

	//----------power on flag check
	int ret, temp;
// if(SysCtrl.powerflag==POWERON_FLAG_KEY) // power key is not hold
// {
// 	temp = 0;
// 	boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
// 	if(temp==0)
// 	{
// 		//uninit();
// 	}
// }
// hal_csiEnable(0);
// XOSTimeDly(50);  //等待一会儿让数据流结束
// 	_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
// 	csiSensor->p_fun_adapt.fp_rotate(1);//打开旋转

// 	hal_csiEnable(1);
#if (1 == SENSOR_NUM)
	printer_init();
#endif
	SysCtrl.printer_en = configGet(CONFIG_ID_PRINTER_EN);
	SysCtrl.printer_level = configGet(CONFIG_ID_PRINTER_DENSITY);
	SysCtrl.printer_print_mode = configValue2Int(CONFIG_ID_PRINTER_MODE);
	SysCtrl.printer_near_far = configValue2Int(CONFIG_ID_PRINTER_NEARFAR);
	SysCtrl.volume_level = configGet(CONFIG_ID_VOLUME);
	SysCtrl.credit_flag = open_printer_get_credit() || masf_image_credit_check();
	deg_Printf("APP credit_flag:%d\n", SysCtrl.credit_flag);
	deg_Printf("SysCtrl.printer_en=%d\n", SysCtrl.printer_en);
	deg_Printf("SysCtrl.printer_level=%d\n", SysCtrl.printer_level);
	deg_Printf("SysCtrl.printer_print_mode=%d\n", SysCtrl.printer_print_mode);
	deg_Printf("SysCtrl.volume_level=%d\n", SysCtrl.volume_level);

#if 0 //(MXWAPP_PRINTER_SUPPORT == 1)
	hal_appPrintMalloc();
	Mxw_liukong_handle = XWorkCreate(100*X_TICK,liukongCTLTimeout1);//10ms
#endif
	if (configGet(CONFIG_ID_IR_LED) == R_ID_STR_COM_ON)
	{
		Bt_Control_Led_OnOff(1);
	}
	else
	{
		Bt_Control_Led_OnOff(0);
	}
	hal_dacSetVolume(100);

#if ENABLE_FLASH_PHOTO
	spi_udisk_init(FLASH_CAPACITY); // flash photo init
#endif

	boardIoctrl(SysCtrl.bfd_usb, IOCTRL_USB_CHECK, (INT32U)&temp);
	if (temp == 0)
	{
		usb_bat_draw_first_pic = 1;
		layout_logo_show(1, 0, 1); // power on.music en,do not wait music end	//---->show logo here,can speed start logo show.make user feeling system starting faster
								   // layout_logo_show(0,0,1);  // power on.music en,do not wait music end	//---->show logo here,can speed start logo show.make user feeling system starting faster
	}
	else
	{
		layout_logo_show(0, 0, 2);
	}

	yuv_rgb_table();

	//--------initial resource manager for fs
	managerInit();
	//--------initial font  & ui & configure
	fontInit();
	iconInit();
	//---------update time RTC time

	//	deg_Printf("main : backlight on.%d ms\n",XOSTimeGet()-tick);
	//	int fd = boardOpen(DEV_NAME_BATTERY);
	boardNameIoctrl("cmos-sensor", IOCTRL_CMOS_INIT, 0);
	//    deg_Printf("main : csi end.%d ms\n",XOSTimeGet()-tick);
	SysCtrl.t_wait = XOSTimeGet();
	DATE_TIME_T *rtcTime = hal_rtcTimeGet();
	if (/*userConfigInitial() || */ (rtcTime->year < 2020)) // user configure reset
	{
#if DATETIME_LOAD_AUTO > 0
		rtcTime->year = configGet(CONFIG_ID_YEAR);
		rtcTime->month = configGet(CONFIG_ID_MONTH);
		rtcTime->day = configGet(CONFIG_ID_MDAY);
		rtcTime->hour = configGet(CONFIG_ID_HOUR);
		rtcTime->min = configGet(CONFIG_ID_MIN);
		rtcTime->sec = configGet(CONFIG_ID_SEC);
#else
		rtcTime->year = atoi(VERSION_YEAR);
		rtcTime->month = atoi(VERSION_MONTH);
		rtcTime->day = atoi(VERSION_DAY);
		rtcTime->hour = atoi(VERSION_HOUR);
		rtcTime->min = atoi(VERSION_MIN);
		rtcTime->sec = atoi(VERSION_SEC);
#endif
		hal_rtcTimeSet(rtcTime); // default time from VERSION_TIME
	}
	SysCtrl.pip_mode = 0;
	SysCtrl.f_update = 0;
	SysCtrl.avi_list = -1;
	SysCtrl.jpg_list = -1;
	SysCtrl.wav_list = -1;
	SysCtrl.powerOnTime = 0;
	;
	SysCtrl.f_keysound = configValue2Int(CONFIG_ID_KEYSOUND);
	SysCtrl.sysQ = XMsgQCreate(sysDeviceQStack, SYSDEVICE_Q_SIZE);
	SysCtrl.bat_charg_flag = 1;
	/*
		SysCtrl.ir_setting= configValue2Int(CONFIG_ID_IR_LED);
		if(2==SysCtrl.ir_setting)
		{
			boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);
		}
	*/
	SysCtrl.sdcard = SDC_STAT_NULL; // SDC_STAT_UNSTABLE;
	if (configGet(CONFIG_ID_BAT_CHECK_FLAG) == R_ID_STR_COM_ON)
	{
		SysCtrl.battery = BATTERY_STAT_4; // DEFAULT VALUE
		deg_Printf("show value: use DEFAULT\n");
		deg_Printf("density:[%d]\n", configValue2Int(CONFIG_ID_PRINTER_DENSITY));
	}
	else
	{
		SysCtrl.battery = configValue2Int(CONFIG_ID_BAT_OLD);
		deg_Printf("show value: sys[%d]  2value:oldbat[%d] flag[%d]\n", SysCtrl.battery, configValue2Int(CONFIG_ID_BAT_OLD), configValue2Int(CONFIG_ID_BAT_CHECK_FLAG));
	}
	/*configSet(CONFIG_ID_BAT_OLD,R_ID_STR_COM_LEVEL_6);
	//configSys(CONFIG_ID_BAT_OLD);
	userConfigSave();
	deg_Printf("set old bat [%d]\n",configValue2Int(CONFIG_ID_BAT_OLD));*/
	//--------initial fs
	fs_init();
	//--------board check ,the first time.
	taskSysScanDev(0); // check board state
					   //--------wait power on music end.
	ret = XOSTimeGet();
	volatile int i = 0x08fffff;
	i = 0x08fffff;
	while ((audioPlaybackGetStatus() == MEDIA_STAT_PLAY)) // wait music end
	{
		i--;
		if (i == 0)
			break;
		if ((i & 0xffff) == 0)
			deg_Printf(".");
	}
	deg_Printf("pass music\n");
	while (XOSTimeGet() - ret <= 1500) // for check usb
	{
		XOSTimeDly(2);
		taskSysScanDev(0); // check board state
		hal_wdtClear();
	}
	deg_Printf("pass check usb\n");

	layout_keysound_load(NULL);
	deg_Printf("pass layout_keysound_load\n");

	INT16U width, height;
	dispLayerInit(DISP_LAYER_OSD0);							  // enable ui display layer
	dispLayerGetResolution(DISP_LAYER_OSD0, &width, &height); // get osd size
	initDrawBuffWH(width, height);							  // intitial ui manager
	deg_Printf("pass initDrawBuffWH\n");
	R_loadResource((void *)User_Icon_Table, R_ICON_MAX); // load icon table
	winInit();
	deg_Printf("pass winInit\n");

	// if all device is working , baterry may low, must off sysytem
	return 0;
}

/*******************************************************************************
 * Function Name  : main
 * Description    :
 * Input          : *
 * Output         : None
 * Return         : None
 *******************************************************************************/
int main(void)
{

	usb_phyreset_as_typeb();
	//--------------power on--------
	init(); // system power on configure
	configSystem();
	//----fireware upgrade
	upgrade();
	/*

	TASK_POWER_OFF	has highest priority,if other place start TASK_POWER_OFF task,
	the task started here will be ignored;
	*/
	// hal_csiEnable(0);
	// XOSTimeDly(50);  //等待一会儿让数据流结束
	// _Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
	// csiSensor->p_fun_adapt.fp_rotate(1);//打开旋转
	// hal_csiEnable(1);
	hal_custom_frame_init();

	if (USB_STAT_NULL == SysCtrl.usb)
	{
		taskStart(TASK_MAIN_MENU, 0);
	}
	else
	{
		taskStart(TASK_USB_DEVICE, USB_DEV_PCCAMERA);
	}

	unshieldedMsgFuncRegister(sysUnshieldedMsgDeal);
	// LDOCON |= (1<<12);
	// ax32xx_sysLDOSet(SYS_LDO_LSEN,SYS_VOL_V2_8,1);
	sysPowerOn = true;
	XMsgQFlush(SysCtrl.sysQ);
	XMsgQPost(SysCtrl.sysQ, (void *)makeEvent(SYS_EVENT_SDC, SysCtrl.sdcard));
	XMsgQPost(SysCtrl.sysQ, (void *)makeEvent(SYS_EVENT_BAT, SysCtrl.battery));
	if (SysCtrl.usb != USB_STAT_NULL)
		XMsgQPost(SysCtrl.sysQ, (void *)makeEvent(SYS_EVENT_USB, SysCtrl.usb));
	taskService();
	// self_test();
	return 2; // for usb upgrade
}
/*******************************************************************************
 * Function Name  :  systemService
 * Description    : system event service for event and key get
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void systemService(void)
{
	hal_wdtClear();
	taskSysScanDev(1);
	msgDeal();
	drawUIService(false);

	// uart1Test();
}

void sendDrawUIMsg(void)
{
	msgDealByType(SYS_DRAW_UI, winGetCurrent(), 0, NULL);
};
