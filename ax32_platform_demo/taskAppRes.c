#include"uiWin.h"

enum
{
	APP_STRING1_ID=0,
	APP_STRING2_ID,
	APP_STRING3_ID,
	APP_STRING4_ID,
	APP_QRCODE_ID,
	APP_PRINTER_TIPS_BK_ID,
	APP_PRINTER_TIPS_HEAD_ID,
	APP_PRINTER_TIPS_STR_ID,
	APP_PRINTER_TIPS_CHARGING_NPRINT_ID,
	APP_PRINTER_TIPS_PLZ_CHARG,
	APP_PRINTER_TIPS_PIC_ERR,
	APP_MAX_ID
};

static widgetCreateInfor AppWin[] =
{
	createFrameWin(Rx(0),Ry(0), Rw(320),Rh(240),R_COLOR_TRANSFER,WIN_ABS_POS),

	//createImageIcon(APP_QRCODE_ID,Rx(95), Ry(110), Rw(129),Rh(129), R_ICON_MAX,ALIGNMENT_CENTER),
	
	createStringIcon(APP_STRING1_ID, Rx(0), Ry(5), Rw(320),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING2_ID, Rx(3), Ry(30), Rw(317),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING3_ID, Rx(0), Ry(50), Rw(320),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),
	createStringIcon(APP_STRING4_ID, Rx(3), Ry(75), Rw(317),Rh(20),strToResId(" "),ALIGNMENT_LEFT, R_COLOR_BLACK,0),

	//===tips===
	createRect(APP_PRINTER_TIPS_BK_ID,Rx(70),Ry(50),Rw(180),Rh(140),R_COLOR_TBLACK),
	createStringIcon(APP_PRINTER_TIPS_HEAD_ID,Rx(70),Ry(50), Rw(180),Rh(45),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(APP_PRINTER_TIPS_STR_ID,Rx(70),Ry(95), Rw(180),Rh(95),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(APP_PRINTER_TIPS_CHARGING_NPRINT_ID,Rx(70),Ry(95), Rw(180),Rh(95),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	//createStringIcon(APP_PRINTER_TIPS_PIC_ERR,Rx(70),Ry(50), Rw(180),Rh(45),strToResId(" "),ALIGNMENT_CENTER, R_COLOR_WHITE,0),
	createStringIcon(APP_PRINTER_TIPS_PLZ_CHARG,Rx(70),Ry(142), Rw(180),Rh(32),R_ID_STR_TIPS_PRINTING,ALIGNMENT_CENTER, R_COLOR_WHITE,0),

	widgetEnd(),
};


#if 0
static void AppQRShow(winHandle handle)
{

/*	if(R_ID_STR_LAN_SCHINESE == configGet(CONFIG_ID_LANGUAGE))
	{
		winSetVisible(winItem(handle,APP_QRCODE_EN_ID),false);
		winSetVisible(winItem(handle,APP_QRCODE_SC_ID),true);
	}
	else
	{
		winSetVisible(winItem(handle,APP_QRCODE_SC_ID),false);
		winSetVisible(winItem(handle,APP_QRCODE_EN_ID),true);
	}

	winSetResid(winItem(handle,APP_QRCODE_ID), R_ID_ICON_MTQRIN);
	winSetVisible(winItem(handle,APP_QRCODE_ID),true);
	drawUIService(true);
*/
}
#endif

static void AppStrShow(winHandle handle)
{
	winSetResid(winItem(handle,APP_STRING1_ID), R_ID_STR_APP_TIPS1);
	winSetResid(winItem(handle,APP_STRING2_ID), R_ID_STR_APP_TIPS2);
	winSetResid(winItem(handle,APP_STRING3_ID), R_ID_STR_APP_TIPS3);
	winSetResid(winItem(handle,APP_STRING4_ID), R_ID_STR_APP_TIPS4);

	winSetVisible(winItem(handle,APP_STRING1_ID),true);
	winSetVisible(winItem(handle,APP_STRING2_ID),true);
	winSetVisible(winItem(handle,APP_STRING3_ID),true);
	winSetVisible(winItem(handle,APP_STRING4_ID),true);
}

static void AppPrinterTipsShow(winHandle handle,bool show,u32 head_str_id,u32 tips_str_id)
{

	if(true==show)
	{
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_BK_ID),true);
		
		winSetResid(winItem(handle,APP_PRINTER_TIPS_HEAD_ID),head_str_id);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_HEAD_ID),true);
		
		winSetResid(winItem(handle,APP_PRINTER_TIPS_STR_ID),tips_str_id);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_STR_ID),true);
			
		winSetResid(winItem(handle,APP_PRINTER_TIPS_CHARGING_NPRINT_ID),tips_str_id);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_CHARGING_NPRINT_ID),true);



		if(tips_str_id==R_ID_STR_TIP_KEEPQ)
		{
			winSetResid(winItem(handle,APP_PRINTER_TIPS_PLZ_CHARG),R_ID_STR_TIP_PLZ_CHARG);
			winSetVisible(winItem(handle,APP_PRINTER_TIPS_PLZ_CHARG),true);

		}
	}
	else
	{
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_BK_ID),false);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_HEAD_ID),false);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_STR_ID),false);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_CHARGING_NPRINT_ID),false);
		winSetVisible(winItem(handle,APP_PRINTER_TIPS_PLZ_CHARG),false);
		//winSetVisible(winItem(handle,APP_PRINTER_TIPS_PIC_ERR),false);
	}
}


