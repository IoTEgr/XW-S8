#include"uiWin.h"

enum
{
	DATETIME_BATTERY_ID=0,
	DATETIME_YEAR_ID,
	DATETIME_MONTH_ID,
	DATETIME_DAY_ID,
	DATETIME_HOUR_ID,
	DATETIME_MIN_ID,
	DATETIME_SEC_ID,
	
	DATETIME_DELI1_ID,
	DATETIME_DELI2_ID,
	DATETIME_DELI3_ID,
	DATETIME_DELI4_ID,
	
};

static widgetCreateInfor dateTimeWin[] =
{
	createFrameWin(Rx(70),Ry(42), Rw(180),Rh(142),R_COLOR_GRAY2/*R_COLOR_TRANSFER*/,WIN_ABS_POS),
	//createImageIcon(DATETIME_BATTERY_ID,Rx(320-33),         Ry(0),      Rw(33),Rh(32), R_ID_ICON_MTBATTERY3,ALIGNMENT_CENTER),
	createStringIcon(DATETIME_YEAR_ID,   Rx(26),            Ry(45),    Rw(48),Rh(25),strToResId("0000"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_MONTH_ID,  Rx(26+48+16),      Ry(45),    Rw(24),Rh(25),strToResId("00"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_DAY_ID,    Rx(26+48+16+24+16),Ry(45),    Rw(24),Rh(25),strToResId("00"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_HOUR_ID,   Rx(38),            Ry(45+25), Rw(24),Rh(25),strToResId("00"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_MIN_ID,    Rx(38+24+16),      Ry(45+25), Rw(24),Rh(25),strToResId("00"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_SEC_ID,    Rx(38+24+16+24+16),Ry(45+25), Rw(24),Rh(25),strToResId("00"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_DELI1_ID,  Rx(26+48),         Ry(45),    Rw(16),Rh(25),strToResId("-"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_DELI2_ID,  Rx(26+48+16+24),   Ry(45),    Rw(16),Rh(25),strToResId("-"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_DELI3_ID,  Rx(38+24),         Ry(45+25), Rw(16),Rh(25),strToResId(":"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
	createStringIcon(DATETIME_DELI4_ID,  Rx(38+24+16+24),   Ry(45+25), Rw(16),Rh(25),strToResId(":"),ALIGNMENT_CENTER,R_COLOR_WHITE,0),
    //createStringIcon(FORMAT_YES_STR_ID,Rx(0),Ry(140-100), Rw(220),Rh(40),R_ID_STR_COM_OK,ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	widgetEnd(),
};


#define DATE_TIME_ITEM_MAX      6
#define DATE_TIME_STR_LEN       10
static u8 dateTimeStr[6][DATE_TIME_STR_LEN];
static uint32 dateTimeData[6];
static unsigned char daysPerMon[12]={31,28,31,30,31,30,31,31,30,31,30,31};


static void dateTimeIncreace(uint32 num)
{
	if(num>=6)
		return;
	dateTimeData[num]+=1;
	if(num==0&&dateTimeData[num]>2050)
		dateTimeData[num]=1990;
	if(num==1&&dateTimeData[num]>12)
		dateTimeData[num]=1;
	if(num==2&&dateTimeData[num]>daysPerMon[dateTimeData[num-1]-1])
		dateTimeData[num]=1;
	if(num==3&&dateTimeData[num]>23)
		dateTimeData[num]=0;
	if(num==4&&dateTimeData[num]>59)
		dateTimeData[num]=0;
	if(num==5&&dateTimeData[num]>59)
		dateTimeData[num]=0;
}

static void dateTimeDecreace(uint32 num)
{
	if(num>=6)
		return;
	dateTimeData[num]-=1;
	if(num==0&&dateTimeData[num]<1990)
		dateTimeData[num]=2050;
	if(num==1&&dateTimeData[num]==0)
		dateTimeData[num]=12;
	if(num==2&&dateTimeData[num]==0)
		dateTimeData[num]=daysPerMon[dateTimeData[num-1]-1];
	if(num==3&&dateTimeData[num]>23)
		dateTimeData[num]=23;
	if(num==4&&dateTimeData[num]>59)
		dateTimeData[num]=59;
	if(num==5&&dateTimeData[num]>59)
		dateTimeData[num]=59;
}

static void dateTimeTimeStr()
{
	dateTimeStr[0][0] = (dateTimeData[0]/1000)%10+'0';
	dateTimeStr[0][1] = (dateTimeData[0]/100)%10+'0';
	dateTimeStr[0][2] = (dateTimeData[0]/10)%10+'0';
	dateTimeStr[0][3] = dateTimeData[0]%10+'0';
	dateTimeStr[0][4] = '\0';

	dateTimeStr[1][0] = (dateTimeData[1]/10)%10+'0';
	dateTimeStr[1][1] = dateTimeData[1]%10+'0';
	dateTimeStr[1][2] = '\0';

	dateTimeStr[2][0] = (dateTimeData[2]/10)%10+'0';
	dateTimeStr[2][1] = dateTimeData[2]%10+'0';
	dateTimeStr[2][2] = '\0';

	dateTimeStr[3][0] = (dateTimeData[3]/10)%10+'0';
	dateTimeStr[3][1] = dateTimeData[3]%10+'0';
	dateTimeStr[3][2] = '\0';

	dateTimeStr[4][0] = (dateTimeData[4]/10)%10+'0';
	dateTimeStr[4][1] = dateTimeData[4]%10+'0';
	dateTimeStr[4][2] = '\0';

	dateTimeStr[5][0] = (dateTimeData[5]/10)%10+'0';
	dateTimeStr[5][1] = dateTimeData[5]%10+'0';
	dateTimeStr[5][2] = '\0';

}



static void settingDateTimeBaterryShow(winHandle handle)
{
	return ;
	//deg_Printf("*************SysCtrl.battery %d  ***\n",SysCtrl.battery);
	if(SysCtrl.usb != USB_STAT_NULL)
	{
		SysCtrl.bat_charge_idx++;
		if(SysCtrl.bat_charge_idx>3)
		{
			SysCtrl.bat_charge_idx=0;
		}
		winSetResid(winItem(handle,DATETIME_BATTERY_ID),R_ID_ICON_MTBATTERY1+SysCtrl.bat_charge_idx);
	}
	else if(SysCtrl.battery == BATTERY_STAT_4)
		winSetResid(winItem(handle,DATETIME_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	else if(SysCtrl.battery == BATTERY_STAT_3)
		winSetResid(winItem(handle,DATETIME_BATTERY_ID),R_ID_ICON_MTBATTERY3);
	else if(SysCtrl.battery == BATTERY_STAT_2)
		winSetResid(winItem(handle,DATETIME_BATTERY_ID),R_ID_ICON_MTBATTERY_LOW2);
	else if(SysCtrl.battery == BATTERY_STAT_1)
		winSetResid(winItem(handle,DATETIME_BATTERY_ID),R_ID_ICON_MTBATTERY1);
	else if(SysCtrl.battery == BATTERY_STAT_0)
		winSetResid(winItem(handle,DATETIME_BATTERY_ID),R_ID_ICON_MTBATTERY0);
	else
		winSetResid(winItem(handle,DATETIME_BATTERY_ID),R_ID_ICON_MTBATTERY4);
	winSetVisible(winItem(handle,DATETIME_BATTERY_ID),true);
}

static void settingDateTimeSelShow(winHandle handle,u8 sel_idx)
{
	u8 i;
	dateTimeTimeStr();
	for(i=0;i<DATE_TIME_ITEM_MAX;i++)
	{
		winSetStrInfor(winItem(handle,DATETIME_YEAR_ID+i),0,ALIGNMENT_CENTER,R_COLOR_WHITE);
	}
	winSetStrInfor(winItem(handle,DATETIME_YEAR_ID+sel_idx),0,ALIGNMENT_CENTER,R_COLOR_YELLOW);

	for(i=0;i<DATE_TIME_ITEM_MAX;i++)
	{
		winSetResid(winItem(handle,DATETIME_YEAR_ID+i),strToResId(dateTimeStr[i]));
	}
}


