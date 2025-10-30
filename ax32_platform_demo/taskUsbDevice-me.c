/****************************************************************************
**
 **                              TASK 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                 TASK USB
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : task_usb.c
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

#define  USB_ICON_MASS      RES_USBMODE_MSC
#define  USB_ICON_CAM       RES_USBMODE_PCCAM
extern msgDealInfor usbDeviceMsgDeal[];
//static bool is_SDCard;

static int LcdOff_cnt = 0;
static int PowerOff_cnt = 0;
static int batcnt = 0;
int usb_bat_flag = 0;
int usb_bat_draw_first_pic = 0;
int usb_pwroff_logo_flag = 0;
static u32 batPicTable[] =
{
	RES_USBBAT_10,
	//RES_USBBAT_20,
	RES_USBBAT_30,
	//RES_USBBAT_40,
	RES_USBBAT_50,
	//RES_USBBAT_60,
	RES_USBBAT_70,
	//RES_USBBAT_80,
	//RES_USBBAT_90,
	RES_USBBAT_100
};

#if 0
static void usbModelDel(uint32 mode)
{
	//deg_Printf("usb task enter.0x%x\n",arg);
//	dispLayerInit(DISP_LAYER_VIDEO); 
//	dispLayerUninit(DISP_LAYER_VIDEO);
	dispLayerUninit(DISP_LAYER_OSD0);	
	if(SysCtrl.sdcard == SDC_STAT_NORMAL)
		mode = 0;	// 0 : card reader  , 1 : PC cam   , 2 card reader + PCcam
	else
		mode = 1;	// 0 : card reader  , 1 : PC cam   , 2 card reader + PCcam
	//set lcd video layer color
//	hal_lcdSetBufYUV(hal_lcdGetShowingBuffer(),0,0x80,0x80);
	dispLayerSetPIPMode(DISP_PIP_DISABLE);	// mark add.2017/10/16	. can not show MASS & CAM when prev task is video task or photo task.
	if(mode==0)
		logo_image_show(USB_ICON_MASS);
	else
		logo_image_show(USB_ICON_CAM);
	
//==set label==
	#if MSC_LABLE_EN
	if(SDC_STAT_NORMAL==SysCtrl.sdcard)
	{
		f_setlabel(MSC_LABEL);
	}
	#endif

	if(mode) // pc-cam   
	{
	   VIDEO_ARG_T arg;
	   arg.resolution.width = devSensorOp->pixelw;
	   arg.resolution.height= devSensorOp->pixelh;
	   deg_Printf("pixelw:%d,pixelh:%d\n",devSensorOp->pixelw,devSensorOp->pixelh);
	   videoRecordInit(&arg); // enable csi&mjpeg 
	   hal_csiEnable(1);
	}

	while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY); 
	deg_Printf("mode:%d\n",mode);
	hal_usbdInit(mode);
}
#endif

extern u32 csi_en_flag;
static void taskUsbDeviceOpen(uint32 arg)
{
	uint32 mode;
	deg_Printf("usb task enter.0x%x\n",arg);
	csi_en_flag = 1;
//	dispLayerInit(DISP_LAYER_VIDEO); 
//	dispLayerUninit(DISP_LAYER_VIDEO);

	if((SysCtrl.usb!=USB_STAT_NULL)&&(usb_bat_draw_first_pic==0))
	{
		if(usb_pwroff_logo_flag == 1)
		{
			usb_pwroff_logo_flag = 0;
			dispLayerSetPIPMode(DISP_PIP_DISABLE); ////app_lcdCsiVideoShowStop();
			layout_logo_show(1,1,0); 
		}
		usb_bat_draw_first_pic = 1;
		batcnt = 0;
		LcdOff_cnt = 0;
		usb_bat_flag = 1;
		//logo_image_show(RES_USBBAT_10);
		msgFuncRegister(usbDeviceMsgDeal);
	}
	else
	{
		dispLayerUninit(DISP_LAYER_VIDEO);
		dispLayerUninit(DISP_LAYER_OSD0);	
		if(SysCtrl.sdcard == SDC_STAT_NORMAL)
			mode = 0;	// 0 : card reader  , 1 : PC cam   , 2 card reader + PCcam
		else
			mode = 1;	// 0 : card reader  , 1 : PC cam   , 2 card reader + PCcam
		//set lcd video layer color
	//	hal_lcdSetBufYUV(hal_lcdGetShowingBuffer(),0,0x80,0x80);
		dispLayerSetPIPMode(DISP_PIP_DISABLE);	// mark add.2017/10/16	. can not show MASS & CAM when prev task is video task or photo task.
		if(mode==0)
			logo_image_show(USB_ICON_MASS);
		else
			logo_image_show(USB_ICON_CAM);
		
	//==set label==
		#if MSC_LABLE_EN
		if(SDC_STAT_NORMAL==SysCtrl.sdcard)
		{
			f_setlabel(MSC_LABEL);
		}
		#endif

		if(mode) // pc-cam   
		{
		   VIDEO_ARG_T arg;
		   arg.resolution.width = devSensorOp->pixelw;
		   arg.resolution.height= devSensorOp->pixelh;
		   deg_Printf("pixelw:%d,pixelh:%d\n",devSensorOp->pixelw,devSensorOp->pixelh);
		   videoRecordInit(&arg); // enable csi&mjpeg 
		  // hal_csiEnable(1);
		}

		while(audioPlaybackGetStatus() == MEDIA_STAT_PLAY); 
		deg_Printf("mode:%d\n",mode);
		hal_usbdInit(mode);
		msgFuncRegister(usbDeviceMsgDeal);
	}
}
static void taskUsbDeviceClose(uint32 arg)
{
	csi_en_flag = 0;
	hal_usbdUninit();
	hal_mjpegEncodeUninit();
	dispLayerInit(DISP_LAYER_OSD0);
}
static void taskUsbDeviceService(uint32 arg)
{
	if(false == hal_usbdProcess())
	{
		deg_Printf("usb update\n");
		boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0); // back light off
		taskStart(TASK_USB_UPDATE,0);
		//taskStart(TASK_POWER_OFF,0);
	}
/*
	static u32 t;
	if(XOSTimeGet()>t+500)
	{
		deg_Printf(".");
		t=XOSTimeGet();
	}
*/
}
sysTask taskUsbDevice=
{
	"usb device",
	/*NULL*/0,
	taskUsbDeviceOpen,
	taskUsbDeviceClose,
	taskUsbDeviceService,
};


static int usbDeviceMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int usbDeviceMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	/*
		if(videoRecordFrameEnGet())
		{
			u32 yuv_init_time = XOSTimeGet();
			if((SysCtrl.frame_index < 0)&&(SysCtrl.frame_index_last<0))
				SysCtrl.frame_index = SysCtrl.frame_index_last = 0;
			else if(SysCtrl.frame_index < 0)
				SysCtrl.frame_index = SysCtrl.frame_index_last;
			else
			{
				if(SysCtrl.frame_index > 0)
				{
					SysCtrl.frame_index--;
					SysCtrl.frame_index_last--;
				}
				else
				{
					SysCtrl.frame_index = ICON_FRAME_NUM -1;
					SysCtrl.frame_index_last = ICON_FRAME_NUM -1;
				}
			}
			if( SysCtrl.frame_index >= ICON_FRAME_NUM)
				SysCtrl.frame_index = SysCtrl.frame_index_last= 0;
			deg_Printf("frame_index:%d\n",SysCtrl.frame_index);

			if(hal_custom_frame_create(R_FRAME[SysCtrl.frame_index])<0)
			{
				SysCtrl.frame_index_last = -1;
				SysCtrl.frame_index = -1;
				videoRecordCmdSet(CMD_COM_FRAME,0);
				deg_Printf(">>---frame image is not prepared sucessfully,will not display frame--<<\n");
			}
			else{
				//SysCtrl.frame_index = SysCtrl.frame_index_last;
				videoRecordCmdSet(CMD_COM_FRAME,1);
				deg_Printf(">>------frame image %d is ready, init time:%dms----<<\n", SysCtrl.frame_index,XOSTimeGet()-yuv_init_time);
			}
			dispLayerSetPIPMode(SysCtrl.pip_mode);
		}
		else
		{
			if(SysCtrl.spec_color_index >0)
			{
				SysCtrl.spec_color_index--;
			}
			else
			{
				SysCtrl.spec_color_index=5;
			}
			cmos_spec_color(SysCtrl.spec_color_index);
		}
		*/
	}
	return 0;
}
static int usbDeviceMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
//	static u32 test = 0;
//    static u32 addr = 0;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	/*
		if(videoRecordFrameEnGet())
		{
			u32 yuv_init_time = XOSTimeGet();
			if((SysCtrl.frame_index < 0)&&(SysCtrl.frame_index_last<0))
				SysCtrl.frame_index = SysCtrl.frame_index_last = 0;
			else if(SysCtrl.frame_index < 0)
				SysCtrl.frame_index = SysCtrl.frame_index_last;
			else
			{
				SysCtrl.frame_index++;
				SysCtrl.frame_index_last++;
			}
			if( SysCtrl.frame_index >= ICON_FRAME_NUM)
				SysCtrl.frame_index = SysCtrl.frame_index_last= 0;
			deg_Printf("frame_index:%d\n",SysCtrl.frame_index);
			
			if(hal_custom_frame_create(R_FRAME[SysCtrl.frame_index])<0)
			{
				SysCtrl.frame_index_last = -1;
				SysCtrl.frame_index = -1;
				videoRecordCmdSet(CMD_COM_FRAME,0);
				deg_Printf(">>---frame image is not prepared sucessfully,will not display frame--<<\n");
			}
			else{
				//SysCtrl.frame_index = SysCtrl.frame_index_last;
				videoRecordCmdSet(CMD_COM_FRAME,1);
				deg_Printf(">>------frame image %d is ready, init time:%dms----<<\n", SysCtrl.frame_index,XOSTimeGet()-yuv_init_time);
			}
			dispLayerSetPIPMode(SysCtrl.pip_mode);
		}
		else
		{
			SysCtrl.spec_color_index++;
			if(SysCtrl.spec_color_index > 5)
			{
				SysCtrl.spec_color_index = 0;
			}
			cmos_spec_color(SysCtrl.spec_color_index);
		}
		*/
	}

	return 0;
}

static int usbDeviceMsgSensorChange(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		sensor_change();
	}
	return 0;
}

static int usbDeviceSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int usbDeviceSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.usb == USB_STAT_NULL)
		taskStart(TASK_POWER_OFF,0);
	return 0;
}
static int usbDeviceSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	int lcdOn = 0;
//	int j = 0;
//	int curBatADC;
	
	if(SysCtrl.usb != USB_STAT_DCIN)
		return -1;

//	curBatADC = getCurBatteryADC();
	//deg_Printf("curBatADC = %d \n",curBatADC);
	deamon_auto_poweroff(1);

	if(0){//curBatADC >= 840
		jpg_decode_show(batPicTable[5]);
	}else{
		if(batcnt>5){
			batcnt = 0;
		}
		jpg_decode_show(batPicTable[batcnt]);
		batcnt++;
	}

	boardIoctrl(SysCtrl.bfd_lcd,IOGET_LCD_BKLIGHT,(u32)&lcdOn);
	if(configGet(CONFIG_ID_BAT_CHECK_FLAG)!=R_ID_STR_COM_ON)
	{
		configSet(CONFIG_ID_BAT_CHECK_FLAG,R_ID_STR_COM_ON);
		userConfigSave();
	}
	if(lcdOn==1){
		LcdOff_cnt++;
		if(LcdOff_cnt>=20){
			LcdOff_cnt = 0;
			boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0);
		
		}
	}	

	PowerOff_cnt++;
	if(PowerOff_cnt>=600){
		
		taskStart(TASK_POWER_OFF,0);
	}
	return 0;
}

msgDealInfor usbDeviceMsgDeal[]=
{
	{SYS_EVENT_SDC,usbDeviceSysMsgSD},
	{SYS_EVENT_USB,usbDeviceSysMsgUSB},
	{SYS_EVENT_500MS,usbDeviceSysMsg500MS},
	{KEY_EVENT_OK,usbDeviceMsgOk},
	{KEY_EVENT_UP,usbDeviceMsgUp},
	{KEY_EVENT_DOWN,usbDeviceMsgDown},
	{KEY_EVENT_SENSOR_CHANGE,usbDeviceMsgSensorChange},

	{EVENT_MAX,NULL},
};

