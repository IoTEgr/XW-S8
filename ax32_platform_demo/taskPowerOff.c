#include "application.h"


#define POWER_UPDATE_BAT_NUM 7
static u32 poweroff_update_bat_tab[POWER_UPDATE_BAT_NUM]={
	
	R_ID_STR_COM_LEVEL_1,
	R_ID_STR_COM_LEVEL_2,
	R_ID_STR_COM_LEVEL_3,
	R_ID_STR_COM_LEVEL_4,
	R_ID_STR_COM_LEVEL_5,
	R_ID_STR_COM_LEVEL_6,
	R_ID_STR_COM_LEVEL_7
};

int uninit(void)
{
	int ret=0,temp;
	
	hal_wdtClear();

    boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_BKLIGHT,0); // back light off
    boardIoctrl(SysCtrl.bfd_led,IOCTRL_LED_NO0,0); // led off
	boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0); // ir led off


	///DAC_PA_MUTE_ON();

	//==for lcd draw black==
	drawUIService(0);	
	hal_osdEnable(OSD0,1);
	ax32xx_sysCpuMsDelay(50);
	//==end for lcd draw black==

	boardIoctrl(SysCtrl.bfd_lcd,IOCTRL_LCD_LCMOFF,0);
	ax32xx_sysCpuMsDelay(10);


	hal_custom_frame_unit();
    deg("system power off\n");
/*
    if(SysCtrl.battery != 0)
    {
	    while(1) // wait power release
	    {
	        ret = boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
			if((ret<0) ||(temp==0))
				break;
	    }
    }
*/

//	boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWEROFF,0);
//	temp = configValue2Int(CONFIG_ID_PARKMODE);
//    boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_STABLE,temp); // wait gsensor stable	

//    boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_SETPARK,temp); // gsensor park mode set.do not care gsensor.

//    boardIoctrl(SysCtrl.bfd_usensor,IOCTRL_USB_POWER,0); // usensor off

	//if(SysCtrl.battery != 0)
	 {
		 while(1) // wait power release
		 {
			// ret = boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
			 s32 adcKey = hal_adcGetChannel(ADC_CH_PB1);
				deg_Printf("the adc key == %d\n",adcKey);
			 if((ret<0) ||(adcKey>900))
				 break;
		 }
	 }
	
    hal_timerStop(TIMER0); 
	hal_sysUninit();


//  XOSTimeDly(100);
//	hal_timerStop(TIMER0); 

	boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);

	if(temp)
	{
#if 0	
		//deg_Printf("during system off, usb dc in, restart system \n");
		//ax32xx_WKI1WakeupTriger(1); //wki wakeup rising trigger
		//ax32xx_wkiCleanPending();
		//hal_vddWKOEnable(0);
		SysCtrl.usb = USB_STAT_DCIN;
		hal_sysReset(); // reset
		while(1);
		//	hal_vddWKOEnable(1);
		//	goto POWER_OFF_DCIN;
#endif		
	}
	else
	{
		hal_wkiWakeup(1);
		ax32xx_WKI1WakeupTriger(1); //wki wakeup rising trigger
		ax32xx_wkiCleanPending();
		hal_vddWKOEnable(0);
		ax32xx_sysCpuMsDelay(50);
	}
/*
#if 0
	ax32xx_wkiCleanPending();
	hal_wkiWakeup(0);
	hal_vddWKOEnable(0);
	ax32xx_sysCpuMsDelay(50);
#else
	ax32xx_WKI1WakeupTriger(1); 
	ax32xx_wkiCleanPending();
	hal_vddWKOEnable(0);
	ax32xx_sysCpuMsDelay(50);
	//deg("are you ok?\n");	
#endif
*/
	u8 count = 0;
	while(1)
	{
		boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);

		if(temp){
			 ret = boardIoctrl(SysCtrl.bfd_key,IOCTRL_KEY_POWER,(INT32U )&temp);
			 if(!temp){
				count++;
				if(count > 10){
					deg("rest ok\n");	
					hal_sysReset(); // reset
					while(1);
				}
			 }else{
				 deg("poweroff\n");	
				 hal_wkiWakeup(1);
				 ax32xx_WKI1WakeupTriger(1); //wki wakeup rising trigger
				 ax32xx_wkiCleanPending();
				 hal_vddWKOEnable(0);
				 ax32xx_sysCpuMsDelay(50);
			 }
		}
//		boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_LOCK,(INT32U)&temp);
//		if(hal_rtcAlarmStatusGet(1))
//			hal_sysReset();
//		temp = 0;
//		boardIoctrl(SysCtrl.bfd_usb,IOCTRL_USB_CHECK,(INT32U)&temp);
//		if(temp && (SysCtrl.usb==USB_STAT_NULL))
//		{
//			SysCtrl.usb = USB_STAT_DCIN;
//			hal_sysReset(); // reset
		//	hal_vddWKOEnable(1);
		//	goto POWER_OFF_DCIN;
//		}
		//XOSTimeDly(1000);
		//deg_Printf(".");
		//hal_vddWKOEnable(0);
       // ax32xx_wkiCleanPending();
		ax32xx_sysCpuMsDelay(50);
	}
	return 0; // makesure the program won't be here
}

extern int usb_bat_flag;
void taskPowerOffOpen(uint32 arg)
{
	if(usb_bat_flag == 0)
	{
		layout_logo_show(0,0,0); 
    	layout_logo_show(1,1,0);  // power off.music en,wait music end
	}
	hal_dacSetVolume(0);
    winUninit();
	configSet(CONFIG_ID_BAT_OLD,poweroff_update_bat_tab[SysCtrl.battery]);
	if(SysCtrl.usb != USB_STAT_NULL)
		configSet(CONFIG_ID_BAT_CHECK_FLAG,R_ID_STR_COM_ON);

    userConfigSave();
	XOSTimeDly(1000);
	uninit();
}

sysTask taskPowerOff=
{
	"Power Off",
	/*NULL*/0,
	taskPowerOffOpen,
	NULL,
	NULL,
};


