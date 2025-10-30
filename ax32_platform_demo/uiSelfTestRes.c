#include"uiWin.h"

enum
{
	TEST_ADCKEY_VAL_ID=0,
	TEST_GSENSOR_ID,
	TEST_SENSOR_ID,
	TEST_LCD_ID,
	TEST_MIC_ID,
	TEST_TUNING_ID,
	TEST_BATTERY_ID,
	TEST_USENSOR_TIP_ID,
	TEST_USENSOR_ID,
	TEST_SD_ID,
	TEST_CHIP_ID
};
static widgetCreateInfor selfTestWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TBLACK,WIN_ABS_POS),
	createStringIcon(TEST_ADCKEY_VAL_ID,Rx(0),Ry(0), Rw(240),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(INVALID_WIDGET_ID,Rx(0),Ry(25), Rw(100),Rh(25),strToResId("Gsensor:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_GSENSOR_ID,Rx(150),Ry(25), Rw(100),Rh(25),strToResId("Gsensor:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(INVALID_WIDGET_ID,Rx(0),Ry(50), Rw(100),Rh(25),strToResId("sensor:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_SENSOR_ID,Rx(150),Ry(50), Rw(100),Rh(25),strToResId("null"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(INVALID_WIDGET_ID,Rx(0),Ry(75), Rw(100),Rh(25),strToResId("lcd:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_LCD_ID,Rx(150),Ry(75), Rw(100),Rh(25),strToResId(" null"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(INVALID_WIDGET_ID,Rx(0),Ry(100), Rw(100),Rh(25),strToResId("mic:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_MIC_ID,Rx(150),Ry(100), Rw(100),Rh(25),strToResId("  "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(INVALID_WIDGET_ID,Rx(0),Ry(125), Rw(100),Rh(25),strToResId("tuning:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_TUNING_ID,Rx(150),Ry(125), Rw(100),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(INVALID_WIDGET_ID,Rx(0),Ry(150), Rw(100),Rh(25),strToResId("battery:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_BATTERY_ID,Rx(150),Ry(150), Rw(100),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_USENSOR_TIP_ID,Rx(0),Ry(175), Rw(100),Rh(25),strToResId("usensor:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_USENSOR_ID,Rx(150),Ry(175), Rw(100),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(INVALID_WIDGET_ID,Rx(0),Ry(200), Rw(100),Rh(25),strToResId("sd:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(TEST_SD_ID,Rx(150),Ry(200), Rw(100),Rh(25),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
	createStringIcon(INVALID_WIDGET_ID,Rx(0),Ry(225), Rw(100),Rh(25),strToResId("chip:"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
#if (AX32_PLATFORM == FPGA)
	createStringIcon(TEST_CHIP_ID,Rx(150),Ry(225), Rw(100),Rh(25),strToResId("FPGA"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
#elif  (AX32_PLATFORM == AX3282S)
	createStringIcon(TEST_CHIP_ID,Rx(150),Ry(225), Rw(100),Rh(25),strToResId("AX3282S"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
#elif  (AX32_PLATFORM == AX3291A)
	createStringIcon(TEST_CHIP_ID,Rx(150),Ry(225), Rw(100),Rh(25),strToResId("AX3291A"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
#else
	createStringIcon(TEST_CHIP_ID,Rx(150),Ry(225), Rw(100),Rh(25),strToResId("???"),ALIGNMENT_LEFT, R_COLOR_WHITE,0),
#endif
	widgetEnd(),
};



