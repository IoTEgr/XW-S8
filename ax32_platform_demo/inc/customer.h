/****************************************************************************
**
 **                              CUSTOMER
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  CUSTOMER HEADER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : customer.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is customer define 
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  CUSTOMER_H
    #define  CUSTOMER_H



#define MXWAPP_PRINTER_SUPPORT 1//HAL_MXWAPP_PRINTER_SUPPORT also open 
//#define USE_BOARD_J589//Q6
#define USE_BOARD_J586//Q5

#define S5_PRODECT 	0
#define S6_PRODECT	1

#define PRODUCT_NUM S5_PRODECT
#define DENSITY_PULS 1	//printer density control:1: density +2 , 0: density orig



#define DOUBLE_CAM

#define PRINTER_CUS_TEST 0

#define  USE_DEFAULT_ENGLISH

#define HALL_EN 1 // 0: no use , 1: use hall detect

#define  SYSTEM_VERSION     "329X_V1.0.0"  //----user configure
#define  BOARD_TYPE_BWV1          0
#define  BOARD_TYPE_FPGA          1

#define RECORD_MODE_DEFAULT       0  //  default mode,720P+VGA doule sensor.
#define RECORD_MODE_SIGNAL        1  //  only 720P
#define RECORD_MODE_AUTO          2  //  720P first.if VGA in ,creating B folder
#define BT_MODE                   1  //  0:MP3 mode    1:BT mode 

//=====Printer Attribute================
#define 	MAIN_MENU_UI	1			//choose MainMenu UI . 1: USE jpg (cusromer Ver)	 , 0:USE bmp (orig Ver)
#define     RESOLUTION		0   		//choose configuration ;such as resolution setting ... . 1: 720P  , 0: VGA
#define 	VOLTAGE			1 			//choose printer voltage . 1: 3.7V  , 0: 7.4V
#define 	PIC_16_9	(RESOLUTION)&0	//choose use VGA Sensor take photo 16:9.  	1:enable	, 0:disable
#define 	BAT_UDATE_RTIME	1			//choose bat level update real time . 1:enable update	,0:disable . {Description: when disable , -->
										//--> bat level only reduce not increase.bat detect only update when get a lower value.}
#define 	ENABLE_FLASH_PHOTO	1	//choose enable flash storage photo. 1:enable	,0:disable
#define		WRITE_CARD_TEST	0			//only for test
#if(1== RESOLUTION)
#define RESOLUTION_720P
#else
#define RESOLUTION_VGA
#endif

#if (1==VOLTAGE)
#define  PRINTER_3_7V
#else
#define PRINTER_7_4V    
#endif

#if(1 == BAT_UDATE_RTIME)
#define BAT_UPDATE_RTIME_EN
#endif

#if (1 ==ENABLE_FLASH_PHOTO)
#define FLASH_CAPACITY  (4*1024*1024)			//  flash capacity-- 4/8M
#define FLASH_UDISK_JPG_SIZE 		(10*1024*6)	//  must 40KB align	
#define PHOTO_2_FLASH_W	320		//task photo on flash W
#define PHOTO_2_FLASH_H	240		//					  H

#endif


#define  RECORD_MODE       RECORD_MODE_SIGNAL 


#define  SYSTEM_BOARD      BOARD_TYPE_BWV1//BOARD_TYPE_FPGA// BOARD_TYPE_BWV1     //----user configure


#define  VIDEO_RATIO       VIDEO_RATIO_DEFAULT     //----user configure


#define  VIDEO_ROTATE      VIDEO_ROTATE_DEFAULT   //----user configure


#define  BAT_SAVE_POWER     0 // second.when battery powered and no key event.0,disable power save


#define  SDC_NULL_TIMEOUT   2 // second .sdc null dialog showing time. 0,disable sdc null showing


//#define  POWER_ON_RECORD     1 // power on record if power_key power on.0:disable


#define  TAKE_PHOTO_MODE     0  // take photo from A and B once if B channel is valided .0:only A OR B


#define  MOTION_DEC_TIME    20 // second.when motion dection no active recording this time will stop recording


#define  KEYSOUND_VOLUME    100 // key & take photo sound volume [0-100]


#define  AVI_TYPE_ODML       1  // AVI FILE TYPE .1-open dml,0-std


#define  FILE_SYNC_WRITE    1  // write sync for avi file


#define  FILE_SYNC_DELETE   0  // delete sync 

#define  FILE_SIZE_PRE_MALLOC	0 // 0: file is not pre malloc , 1 : file is pre malloc(FILE MAX SIZE before REC)

#define  VERSION_LOAD_AUTO     0   // auto load version from resource


#define  DATETIME_LOAD_AUTO    1  // auto load default time from resource


#define  KEYSOUND_LOAD_AUTO    1  // auto load key sound from resource

#define  USENSOR_PWRCTRL_EN    1  // if enable,control usensor power by PE1;if disable,usensor power always power-on
								 // AX3282S define 1, AX3291A define 0
#define  USENSOR_AUDIO_ON      0 // back record audio volume on, use front record audio
#define  KEY_NUM			   5

#define  LCD_CROP_SCALE      1 //lcd crop scale show image ,  0 : is disable , 1 : is enable 
#define  LCD_PLAY_SCALE      1 //LCD show updown black for 4:3 LCD ,  0 : is disable , 1 : is enable 
#define  LCD_PLAY_JPEG_SCALE   0 //suport play jpeg file ,scale up or down ,  0 : is disable , 1 : is enable 


#define  USENSOR_RESOLUTION      0 //usensor recording resolution. 1: 640*480; 2: 1280:720; 3:1280:960

#define  SMALL_PANEL_SUPPORT		0//support lcd panel smaller than 320X240,remove sub menu on the right side && remove current time show on top bar and so on
									  //enable it when in need
#define  IR_MENU_EN            0  // 0: not IR control , 1 IR control


#define  ICON_FRAME_NUM			22//20  //resource frame num
#define MP3_SHOW_MOVE       1//1:use mp3 move , 0:disable

//����ͷSENSOR_NUMҲҪΪ2 ����DOUBLE_CAM�����Ƶ�˫��ͷ
#define  SENSOR_NUM    2 // 1: is one sensor , 2: is two sensor

#define  mjpeg_lcd_cut    0   //   0:   16:9   1: 4:3 

#define  REDUCE_2_5M  2500 // set for sd remain space
#define	REDUCE_60M	6000

#define  USE_EXTERNAL_DAC_PA	0// 0 : use ic dclass , 1: use external pa

#define NOT_COVER_FILE 0 //do not cover old file 

#define SUPPORT_SELF_TEST HAL_CFG_SELF_TEST

#if (1==USE_EXTERNAL_DAC_PA)
//==need to set ax32xx_dac.c  ax32xx_dacInit(void)
//==R_DAC_CTRL1 |= (1<<9);//Class AB
#define   DAC_PA_MUTE_CH  GPIO_PA
#define   DAC_PA_MUTE_PIN GPIO_PIN9
#define   DAC_PA_MUTE_INIT() {hal_gpioInit(DAC_PA_MUTE_CH,DAC_PA_MUTE_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);}
#define   DAC_PA_MUTE_OFF()  {hal_gpioWrite(DAC_PA_MUTE_CH,DAC_PA_MUTE_PIN,GPIO_HIGH);}	// have sound
#define   DAC_PA_MUTE_ON()  {hal_gpioWrite(DAC_PA_MUTE_CH,DAC_PA_MUTE_PIN,GPIO_LOW);}	// no sound
#else
//==need to set ax32xx_dac.c  ax32xx_dacInit(void)
//==R_DAC_CTRL1 &=~(1<<9);  //Class D==
#define   DAC_PA_MUTE_INIT() 
#define   DAC_PA_MUTE_OFF() 
#define   DAC_PA_MUTE_ON() 
#endif


#define  SEN1_RESET_CH	 GPIO_PB
#define  SEN1_RESET_PIN  GPIO_PIN6
#define  SEN1_PWDN_CH	 GPIO_PD
#define  SEN1_PWDN_PIN	 GPIO_PIN14

#if (2==SENSOR_NUM)
//#define SEN1_RESET_CH  
//#define SEN1_RESET_PIN 
#define  SEN2_RESET_CH	 GPIO_PD
#define  SEN2_RESET_PIN  GPIO_PIN4
#define  SEN2_PWDN_CH	 GPIO_PF //vdd-hp
#define  SEN2_PWDN_PIN	 GPIO_PIN0
#endif




//======watermark===
#define WATER_CHAR_GAP 0  //watermark character gap
#define ST_PIXEL_W    16	//res pic's w
#define ST_PIXEL_H	  32	//res pic's h

#define WATERMAKE_SET_X_POS 10	//watermark X pos

#if defined(RESOLUTION_VGA)
#define WATERMAKE_SET_Y_POS	448	//watermark Y pos
#elif defined(RESOLUTION_720P)
#if PIC_16_9
#define WATERMAKE_SET_Y_POS	328	//watermark Y pos
#else
#define WATERMAKE_SET_Y_POS	688	
#endif
#endif

//=========


//---------file dir-----------------------
#define  FILEDIR_VIDEO  "VIDEO/"              //----user configure
#define  FILEDIR_PHOTO  "PHOTO/"              //----user configure
#define  FILEDIR_AUDIO  "AUDIO/"              //----user configure
#if RECORD_MODE == RECORD_MODE_SIGNAL
#define  FILEDIR_VIDEOA "VIDEO/"
#define  FILEDIR_VIDEOB "VIDEO/"
#else
#define  FILEDIR_VIDEOA "VIDEOA/"
#define  FILEDIR_VIDEOB "VIDEOB/"
#endif

#define  FILEDIR_MP3	 "MP3/"

#endif




