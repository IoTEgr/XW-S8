#include "application.h"

extern void winUpdateAllResId(void);
int configLanguage(void)
{
	static uint32 lastSetting=0;
	R_loadResource((void *)User_String_Table,R_STR_MAX);// load string table
	if(0==configValue2Int(CONFIG_ID_LANGUAGE))
	{
		fontSetLanguage(/*R_ID_STR_LAN_ENGLISH*/LANUAGE_ENGLISH);
	}
	else
	{
		fontSetLanguage(configValue2Int(CONFIG_ID_LANGUAGE));
		if(configGet(CONFIG_ID_LANGUAGE)!=lastSetting)
		{
			lastSetting=configGet(CONFIG_ID_LANGUAGE);
		}
	}
	return 0;
}
void configSys(uint32 configId)
{

	INT32U value = configValue2Int(configId);
    switch(configId)
	{
		case CONFIG_ID_KEYSOUND:
			  SysCtrl.f_keysound = value;break;	   
		case CONFIG_ID_RESOLUTION:
			   videoRecordCmdSet(CMD_COM_RESOLUTIONN,value);
			   task_video_record_caltime();
			   break;
#if 0
		case CONFIG_ID_MOTIONDECTION:
			   if(value)
			   	   hal_mdEnable(1);
			   else
			   	   hal_mdEnable(0);
			   break;
#endif
/*
		case CONFIG_ID_PARKMODE:
			   if(SysCtrl.bfd_gsensor<0)
			   {
			   }
			   break;
*/
		case CONFIG_ID_AUDIOREC:
			   videoRecordCmdSet(CMD_COM_AUDIOEN,value);
			   break;
	    case CONFIG_ID_TIMESTAMP	: 
			   videoRecordCmdSet(CMD_COM_TIMESTRAMP,value);break;
	    case CONFIG_ID_LOOPTIME : 
			   SysCtrl.t_mrec = value;
			  if(value == 0)
		      {
			      videoRecordCmdSet(CMD_COM_LOOPREC,0);
				  videoRecordCmdSet(CMD_COM_LOOPTIME,10*60);
				  SysCtrl.t_mrec = 10*60;
		      }
			  else
			  {
				  videoRecordCmdSet(CMD_COM_LOOPREC,1);
				  videoRecordCmdSet(CMD_COM_LOOPTIME,value);
			  }
			  break;
	    case CONFIG_ID_LANGUAGE:	
			  configLanguage();  
			  break;
/*
		case CONFIG_ID_GSENSOR:
			  if(SysCtrl.bfd_gsensor>=0)
			      boardIoctrl(SysCtrl.bfd_gsensor,IOCTRL_GSE_SETLOCK,(INT32U)value);
			  break;
*/
#if HAL_CFG_ISP_SUPPORT
	    case CONFIG_ID_EV:
			  sensor_compensate_exp_set(value);
			  break;
		case CONFIG_ID_WBLANCE:
			  awb_scene_mod_set(value);
			  break;
#endif
		case CONFIG_ID_FREQUNCY:
			  //sensor_frame_rate_adapt(value,25);
			  break;
		case CONFIG_ID_IR_LED:
			  //boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,value);
			  //deg_Printf("ir val=%d\n",value);
			  /*
			  SysCtrl.ir_setting=value;
			  if(2==SysCtrl.ir_setting)
			  {
				  boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,1);
			  }
			  else
			  {
				  boardIoctrl(SysCtrl.bfd_ir,IOCTRL_IR_SET,0);
			  }
			  */
			  break;
		case CONFIG_ID_PRINTER_EN:
			  SysCtrl.printer_en = configValue2Int(CONFIG_ID_PRINTER_EN);//configGet(CONFIG_ID_PRINTER_EN);
			  //deg_Printf("SysCtrl.printer_en=%d\n",SysCtrl.printer_en);
		  	  break;
		case CONFIG_ID_PRINTER_DENSITY:
			// SysCtrl.printer_level = configGet(CONFIG_ID_PRINTER_DENSITY);
			  SysCtrl.printer_level=configValue2Int(CONFIG_ID_PRINTER_DENSITY);
			  deg_Printf("SysCtrl.printer_level=%d\n",SysCtrl.printer_level);
			  break;
		case CONFIG_ID_PRINTER_MODE:
			  SysCtrl.printer_print_mode=configValue2Int(CONFIG_ID_PRINTER_MODE);
			  //deg_Printf("SysCtrl.printer_print_mode=%d\n",SysCtrl.printer_print_mode);
			  break;
	  	case CONFIG_ID_PRINTER_NEARFAR:
		  	 SysCtrl.printer_near_far=configValue2Int(CONFIG_ID_PRINTER_NEARFAR);	  
		   	 break;
	  	case CONFIG_ID_VOLUME:
			  SysCtrl.volume_level=configGet(CONFIG_ID_VOLUME);
			  //deg_Printf("SysCtrl.printer_level=%d\n",SysCtrl.printer_level);
			  break;
			
		default:
		  	  break;
	}	/**/


}



void configSystem(void)
{
	uint32 i;
	for(i=0;i<CONFIG_ID_MAX;i++)
		configSys(i);
}



