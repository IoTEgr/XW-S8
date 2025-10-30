#include "uiSelfTestRes.c"

static int selfTestKeyMsgOk(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	}
	return 0;
}
static int selfTestKeyMsgUp(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int selfTestKeyMsgDown(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{

	}
	return 0;
}
static int selfTestKeyMsgMenu(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
	
	}
	return 0;
}
static int selfTestKeyMsgMode(winHandle handle,uint32 parameNum,uint32* parame)
{
	uint32 keyState=KEY_STATE_INVALID;
	if(parameNum==1)
		keyState=parame[0];
	if(keyState==KEY_PRESSED)
	{
		
	}
	return 0;
}
static int selfTestSysMsgSD(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.sdcard == SDC_STAT_NULL)
	{
		winDestroy(&handle);
	}
	return 0;
}
static int selfTestSysMsgUSB(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
static int selfTestSysMsgBattery(winHandle handle,uint32 parameNum,uint32* parame)
{
	if(SysCtrl.battery == BATTERY_STAT_1)
	{
	}
	else if(SysCtrl.battery == BATTERY_STAT_0)
	{
	}
	return 0;
}
extern void upgrade_uvc(void);
static int selfTestOpenWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("selfTest Open Win!!!\n");
	upgrade_uvc();
	hal_auadcMemInit();
	hal_auadcStartForWAV(16000,100);
	static u8 sbuf[]={'0','0','-','0','0','\0'};
	sbuf[0] = ((CLKTUN>>11)&0X1) + '0';
	if(((CLKTUN>>7)&0xf)<10)
		sbuf[1] = ((CLKTUN>>7)&0xf)+'0';
	else
		sbuf[1] = (((CLKTUN>>7)&0xf)-10)+'a';
	sbuf[3] = ((CLKTUN>>4)&0x1) + '0';
	if(((CLKTUN)&0xf)<10)
		sbuf[4] = ((CLKTUN)&0xf)+'0';
	else
		sbuf[4] = (((CLKTUN)&0xf)-10)+'a';
	winSetResid(winItem(handle,TEST_TUNING_ID),strToResId(sbuf));
	winSetResid(winItem(handle,TEST_GSENSOR_ID),strToResId(gSensorGetName()));
	winSetResid(winItem(handle,TEST_LCD_ID),strToResId(LcdGetName()));
	winSetResid(winItem(handle,TEST_SENSOR_ID),strToResId(SensorGetName()));
	u32 sd_cap = hal_sdCapacity();	// sectors
	static char sdStr[10];
	if(sd_cap > 512*1024*2)
	{
		u32 gb = sd_cap/(1024*2);	//MB
		if(gb >= 10*1024)
		{
			sdStr[0] = (gb>>10)/10+'0';
			sdStr[1] = (gb>>10)%10+'0';
			sdStr[2] = '.';
			sdStr[3] = (gb/100)%10+'0';
			sdStr[4]='G';
			sdStr[5]='B';
			sdStr[6]=0;
		}
		else
		{
			sdStr[0] = (gb>>10)%10+'0';
			sdStr[1] = '.';
			sdStr[2] = (gb/100)%10+'0';
			sdStr[3]='G';
			sdStr[4]='B';
			sdStr[5]=0;
		}
		winSetResid(winItem(handle,TEST_SD_ID),strToResId(sdStr));
	}
	else
	{
		u32 mb = sd_cap/2;	// KB
		if(mb >= 100*1024)
		{
			sdStr[0] = (mb>>10)/100+'0';
			sdStr[1] = (mb>>10)/10+'0';
			sdStr[2] = (mb/100)%10+'0';
			sdStr[3]='M';
			sdStr[4]='B';
			sdStr[5]=0;
		}
		else if(mb >= 10)
		{
			sdStr[0] = (mb>>10)/10+'0';
			sdStr[1] = (mb/100)%10+'0';
			sdStr[2]='M';
			sdStr[3]='B';
			sdStr[4]=0;
		}
		else 
		{
			sdStr[0] = (mb/100)%10+'0';
			sdStr[1]='M';
			sdStr[2]='B';
			sdStr[3]=0;
		}
		winSetResid(winItem(handle,TEST_SD_ID),strToResId(sdStr));
	}
	return 0;
}
static int selfTestCloseWin(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("selfTest Close Win!!!\n");
	ax32xx_auadcEnable(0);
	hal_auadcMemUninit();
	return 0;
}
static int selfTestWinChildClose(winHandle handle,uint32 parameNum,uint32* parame)
{
	deg_Printf("selfTest WinChild Close!!!\n");
	return 0;
}
static int selfTestSysMsg1S(winHandle handle,uint32 parameNum,uint32* parame)
{
	return 0;
}
extern uint32 getKeyADCvalue(void);
extern int getCurBatteryADC(void);
static int selfTestSysMsg500MS(winHandle handle,uint32 parameNum,uint32* parame)
{
	int i;
	short volume;
	INT32U addr,len;
	short * buff;
	
	static char adcKeyStr[25]="keyADC value:";
	intToString(&adcKeyStr[13],11,getKeyADCvalue());
	winSetResid(winItem(handle,TEST_ADCKEY_VAL_ID),strToResId(adcKeyStr));

	volume = 0;
	buff = (short *)hal_auadcBufferGet(&addr,&len,NULL,NULL);
	if(buff)
	{
		for (i=0;i<len/2;i++)
		{
			if (buff[i]<0)
			{
				buff[i] = (~buff[i]+1)&(0x0000ffff) ;
			}
			if (i==0)
				volume = buff[i];
			else if ( volume < buff[i])
				volume = buff[i];
		}
		if (volume > 30000)//>90db
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId("))))))))"));
		else if (volume > 10000)	//80db
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId(")))))))"));
		else if (volume > 3000)	//70db
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId("))))))"));
		else if (volume > 2000)		//65db
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId(")))))"));
		else if (volume > 1000)		//60db
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId("))))"));
		else if (volume > 300)		//50db
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId(")))"));
		else if (volume > 100)		//40db
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId("))"));
		else if (volume > 10)		//20db
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId(")"));
		else 
			winSetResid(winItem(handle,TEST_MIC_ID),strToResId("-")); //<20db
		hal_auadcBufferRelease(buff);
	}
	else 
		winSetResid(winItem(handle,TEST_MIC_ID),strToResId("?"));
	
	static char batteryADCStr[15];
	if(SysCtrl.usb == USB_STAT_NULL)
	{
		batteryADCStr[0]='0'+SysCtrl.battery;
		batteryADCStr[1]='-';
		intToString(&batteryADCStr[2],13,getCurBatteryADC());
		winSetResid(winItem(handle,TEST_BATTERY_ID),strToResId(batteryADCStr));
	}
	else
		winSetResid(winItem(handle,TEST_BATTERY_ID),strToResId("charging"));

	static char usensorStr[6];
	if(USENSOR_STAT_NULL == SysCtrl.usensor)
	{
		winSetResid(winItem(handle,TEST_USENSOR_TIP_ID),strToResId("usensor:"));
		winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("offline"));
	}
	else
	{
		if(!uvcisp.exfilesize)
		{
			winSetResid(winItem(handle,TEST_USENSOR_TIP_ID),strToResId("usensor work:"));
			if(USENSOR_SHOW_ON== SysCtrl.usensor||USENSOR_STAT_IN== SysCtrl.usensor)
			{
				if(!api_backrec_status())
				{	
					usensorStr[0] = '0';
					usensorStr[1] = '0';
					usensorStr[2] = usb_enum_phase_get()/10 +'0';
					usensorStr[3] = usb_enum_phase_get()%10 +'0';;
					usensorStr[4]=0;
					if(0 == usb_enum_phase_get())
						winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("..."));	
					else
						winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId(usensorStr));
				}
				else
					winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("OK"));
			}
			else if(USENSOR_STAT_UNSTABLE== SysCtrl.usensor)
				winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("unstable"));	
			else if(USENSOR_SHOW_OFF== SysCtrl.usensor)
				winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("show off"));
		}
		else
		{
			winSetResid(winItem(handle,TEST_USENSOR_TIP_ID),strToResId("usensor eeprom:"));
			if(USENSOR_SHOW_ON== SysCtrl.usensor||USENSOR_STAT_IN== SysCtrl.usensor)
			{
				if(usb_enum_phase_get()<11)
				{	
					usensorStr[0] = '0';
					usensorStr[1] = '0';
					usensorStr[2] = usb_enum_phase_get()/10 +'0';
					usensorStr[3] = usb_enum_phase_get()%10 +'0';;
					usensorStr[4]=0;
					if(0 == usb_enum_phase_get())
						winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("..."));	
					else
						winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId(usensorStr));
				}
				else
					winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("OK"));
			}
			else if(USENSOR_STAT_UNSTABLE== SysCtrl.usensor)
				winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("unstable"));
			else if(USENSOR_SHOW_OFF== SysCtrl.usensor)
				winSetResid(winItem(handle,TEST_USENSOR_ID),strToResId("show off"));
		}
	}
	return 0;
}


msgDealInfor selfTestMsgDeal[]=
{
	{SYS_OPEN_WINDOW,selfTestOpenWin},
	{SYS_CLOSE_WINDOW,selfTestCloseWin},
	{SYS_CHILE_COLSE,selfTestWinChildClose},
	{KEY_EVENT_PRINTER_EN,selfTestKeyMsgOk},
	{KEY_EVENT_UP,selfTestKeyMsgUp},
	{KEY_EVENT_DOWN,selfTestKeyMsgDown},
	{KEY_EVENT_MENU,selfTestKeyMsgMenu},
	{KEY_EVENT_MODE,selfTestKeyMsgMode},
	{SYS_EVENT_SDC,selfTestSysMsgSD},
	{SYS_EVENT_USB,selfTestSysMsgUSB},
	{SYS_EVENT_BAT,selfTestSysMsgBattery},
	{SYS_EVENT_1S,selfTestSysMsg1S},
	{SYS_EVENT_500MS,selfTestSysMsg500MS},
	{EVENT_MAX,NULL},
};

WINDOW(selfTestWindow,selfTestMsgDeal,selfTestWin)



