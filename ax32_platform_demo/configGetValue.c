/****************************************************************************
**
 **                              MENU 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MENU SETTING
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : menu_user_setting.c
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

INT32U configValue2Int(INT8U config_id)
{
	INT32U value=0;
	INT32U temp_value=0;
	temp_value = configGet(CONFIG_ID_PRINTER_DENSITY_L)*100+configGet(CONFIG_ID_PRINTER_DENSITY_H);
	switch(configGet(config_id))
	{
// lanuage		
		case R_ID_STR_LAN_CZECH : value = LANUAGE_CZECH;break;
		case R_ID_STR_LAN_DUTCH: value = LANUAGE_DUTCH;break;
		case R_ID_STR_LAN_FRECH: value = LANUAGE_FRENCH;break;
		case R_ID_STR_LAN_GERMAN: value = LANUAGE_GERMAN;break;
		case R_ID_STR_LAN_HEBREW: value = LANUAGE_HEBREW;break;
		case R_ID_STR_LAN_ITALIAN: value = LANUAGE_ITALIAN;break;
		case R_ID_STR_LAN_JAPANESE: value = LANUAGE_JAPANESE;break;
		case R_ID_STR_LAN_KOERA: value = LANUAGE_KOERA;break;
		case R_ID_STR_LAN_POLISH: value = LANUAGE_POLISH;break;
		case R_ID_STR_LAN_PORTUGUESE: value = LANUAGE_PORTUGUESE;break;
		case R_ID_STR_LAN_RUSSIAN: value = LANUAGE_RUSSIAN;break;
		case R_ID_STR_LAN_SCHINESE: value = LANUAGE_SCHINESE;break;
		case R_ID_STR_LAN_SPANISH: value = LANUAGE_SPANISH;break;
		case R_ID_STR_LAN_TAI: value = LANUAGE_TAI;break;
		case R_ID_STR_LAN_TCHINESE: value = LANUAGE_TCHINESE;break;
		case R_ID_STR_LAN_TURKEY: value = LANUAGE_TURKEY;break;
		//case R_ID_STR_LAN_UKRAINIAN: value = LANUAGE_UKRAINIAN;break;
        case R_ID_STR_LAN_ENGLISH: value = LANUAGE_ENGLISH;break;
// common
		case R_ID_STR_COM_ALWAYSON:value = 2;break;
        case R_ID_STR_COM_ON:
		case R_ID_STR_COM_OK:
		case R_ID_STR_COM_YES:value = 1;break;
		case R_ID_STR_COM_OFF:
		case R_ID_STR_COM_CANCEL:
		case R_ID_STR_COM_NO:value = 0;break;	   
// time		
		case R_ID_STR_TIM_2SEC : value = 2;break;
		case R_ID_STR_TIM_3SEC : value = 3;break;
		case R_ID_STR_TIM_5SEC : value = 5;break;
		case R_ID_STR_TIM_10SEC : value = 10;break;
		case R_ID_STR_TIM_30SEC : value = 30;break;
		case R_ID_STR_TIM_1MIN : value = 1*60;break;
		case R_ID_STR_TIM_2MIN : value = 2*60;break;
		case R_ID_STR_TIM_3MIN : value = 3*60;break;
		case R_ID_STR_TIM_5MIN : value = 5*60;break;
		case R_ID_STR_TIM_10MIN: value = 10*60;break;
		case R_ID_STR_TIM_60MIN: value = 60*60;break;

// resolution
		case R_ID_STR_RES_480FHD: value = (720<<16)|480;break;
        case R_ID_STR_RES_1024P: value = (1280<<16)|1024;break;
		case R_ID_STR_RES_1080P: value = (1440<<16)|1080;break;
		case R_ID_STR_RES_FHD:
		case R_ID_STR_RES_1080P_SHORT:	
		#if defined(RESOLUTION_VGA)
		case R_ID_STR_RES_1080FHD: value = (1920<<16)|1440;break;
		#elif defined(RESOLUTION_720P)
		case R_ID_STR_RES_1080FHD: value = (1920<<16)|1080;break;
		#endif
		case R_ID_STR_RES_HD:
		case R_ID_STR_RES_720P_SHORT:
		#if defined(RESOLUTION_VGA)
		case R_ID_STR_RES_720P : value = (1280<<16)|960;break;
		#elif defined(RESOLUTION_720P)
		case R_ID_STR_RES_720P : value = (1280<<16)|720;break;
		#endif
		case R_ID_STR_RES_240P :
		case R_ID_STR_RES_QVGA : value = (320<<16)|240;break;
		case R_ID_STR_RES_480P :
		case R_ID_STR_RES_VGA   : value = (640<<16)|480;break;
		
		#if defined(RESOLUTION_VGA)
		case R_ID_STR_RES_1M    : value = (1280<<16)|960;break;
		case R_ID_STR_RES_2M    : value = (1920<<16)|1440;break;
		case R_ID_STR_RES_3M    : value = (2048<<16)|1536;break;//(2560<<16)|1440; 1920
		case R_ID_STR_RES_5M    : value = (2688<<16)|2304;break;//(3200<<16)|1800
		case R_ID_STR_RES_8M    : value = (3200<<16)|2400;break;//3800<<16)|2160
		case R_ID_STR_RES_10M    : value = (3648<<16)|2732;break;	// MAX width is 4032 3800<<16)|2160
		case R_ID_STR_RES_12M    : value = (4000<<16)|3000;break;	// MAX width is 4032 4032<<16)|2880
		case R_ID_STR_RES_20M    : value = (5120<<16)|4000;break;	// MAX width is 4032
		case R_ID_STR_RES_48M    : value = (8000<<16)|6000;break;	// MAX width is 4032

		#elif defined(RESOLUTION_720P)
		
		case R_ID_STR_RES_1M    : value = (1344<<16)|752;break;
		case R_ID_STR_RES_2M    : value = (1920<<16)|1080;break;
		//case R_ID_STR_RES_3M    : value = (2048<<16)|1152;break;//(2560<<16)|1440; 1920
		case R_ID_STR_RES_3M    : value = (2400<<16)|1350;break;
		//case R_ID_STR_RES_5M    : value = (2592<<16)|1458;break;//(3200<<16)|1800
		case R_ID_STR_RES_5M    : value = (3200<<16)|1800;break;
		case R_ID_STR_RES_8M    : value = (4000<<16)|2250;break;//3800<<16)|2160
		case R_ID_STR_RES_10M    : value = (4288<<16)|2412;break;	// MAX width is 4032 3800<<16)|2160
		case R_ID_STR_RES_12M    : value = (4800<<16)|2700;break;	// MAX width is 4032 4032<<16)|2880
		case R_ID_STR_RES_20M    : value = (6080<<16)|3430;break;	// MAX width is 4032
		case R_ID_STR_RES_48M    : value = (9344<<16)|5280;break;	// MAX width is 4032

		#endif
// level
		case R_ID_STR_TIP_NEAR:
        case R_ID_STR_COM_LOW : value = 1;break;
		case R_ID_STR_TIP_MIDDLE:
		case R_ID_STR_COM_MIDDLE: value = 2;break;
		case R_ID_STR_TIP_FAR:
		case R_ID_STR_COM_HIGH: value = 3;break;
// photo num
		case R_ID_STR_PHOTO_NUM_3 : value = 3;break;
		case R_ID_STR_PHOTO_NUM_5: value = 5;break;
// frq 
        case R_ID_STR_COM_50HZ: value = 0;break;
		case R_ID_STR_COM_60HZ: value = 1;break;
// IR
       // case R_ID_STR_IR_AUTO: value = 2;break;
        case CFG_COM_LIGHTING_FORM: value = 2;break;

//isp 
    // white blance
        case R_ID_STR_ISP_AUTO: value = 0;break;
		case R_ID_STR_ISP_SUNLIGHT: value = 1;break;
		case R_ID_STR_ISP_CLOUDY: value = 2;break;
		case R_ID_STR_ISP_TUNGSTEN: value = 3;break;
		case R_ID_STR_ISP_FLUORESCENT: value = 4;break;
    //  ev
        case R_ID_STR_COM_N2_0: value = 1;break;
		case R_ID_STR_COM_N1_0: value = 2;break;
		case R_ID_STR_COM_P0_0: value = 3;break;
		case R_ID_STR_COM_P1_0: value = 4;break;
		case R_ID_STR_COM_P2_0: value = 5;break;
		case R_ID_STR_COM_P3_0: value = 6;break;
		case R_ID_STR_COM_P4_0: value = 7;break;
	//	ev
		case R_ID_STR_COM_LEVEL_1: //value = /*0*/((configGet(CONFIG_ID_PRINTER_DENSITY)*85/100)*85/100)*85/100;
			value = ((temp_value*85/100)*85/100)*85/100;
			break;
		case R_ID_STR_COM_LEVEL_2: //value = /*1*/(configGet(CONFIG_ID_PRINTER_DENSITY)*85/100)*85/100;
			value = (temp_value*85/100)*85/100;
			break;
		case R_ID_STR_COM_LEVEL_3: //value = /*2*/configGet(CONFIG_ID_PRINTER_DENSITY)*85/100;
			value = temp_value*85/100;
			//deg_Printf("value[%d]\n",value);
			break;
		case R_ID_STR_COM_LEVEL_4:// value = /*5*/configGet(CONFIG_ID_PRINTER_DENSITY)*100/100;
			value = temp_value;
			break;
		case R_ID_STR_COM_LEVEL_5: //value = /*4*/configGet(CONFIG_ID_PRINTER_DENSITY)*115/100;
			value = temp_value*115/100;
			break;
		case R_ID_STR_COM_LEVEL_6:// value = /*5*/(configGet(CONFIG_ID_PRINTER_DENSITY)*115/100)*115/100;
			value = (temp_value*115/100)*115/100;
			break;
		case R_ID_STR_COM_LEVEL_7: //value = /*6*/((configGet(CONFIG_ID_PRINTER_DENSITY)*115/100)*115/100)*115/100;
			value = ((temp_value*115/100)*115/100)*115/100;
			break;

		
		//  priht mode
		case R_ID_STR_SET_PRINT_DOT: value = 1;break;
        case R_ID_STR_SET_PRINT_GRAY: value = 0;break;
		
		default:
			break;
	}
	
	return value;
}



