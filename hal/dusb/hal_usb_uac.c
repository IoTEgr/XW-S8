
/******************** (C) COPYRIGHT 2013 BUILDWIN ************************
* File Name          : uart.c
* Author             : pengtao_zhang
* Version            : V0719a
* Date               : 07/19/2013 ;mm/dd/yyyy
* Description        : This file provides all the uart firmware functions.
***************************************************************************/
//#include "../../mcu/mcu_include.h"
#include "../inc/hal.h"

//bool (*fp_uac_unit_ctl_hal)(u8 val, u8 rqu, u8 len);
bool (*fp_UacHandleToStreaming)(u8 val, u8 rqu, u8 len);

u8  UacMuteControl;
u8  UacVolCtrlSettings[8];
u8 const UacVolCtrlSettings_def[8] =	
{
	//UAC_GET_CUR
	0x00,0x0b,
	//UAC_GET_RES
	0x00,0x01,	
	//UAC_GET_MIN
	0x00,0xe5,	
	//UAC_GET_MAX
	0x00,0x15,	
};

u8 UacSampFreq[3];
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uac_epx_cfg(void)
{
	USB20_Index = UAC_EPX;		
	USB20_InMaxPl  = 0x00;
	USB20_OutMaxPl = 0x00;
	USB20_InMaxPh  = 0x04;
	USB20_OutMaxPh = 0x04;
	USB20_OutCSR1 = 0x00;
	USB20_OutCSR2 = 0x60;
	USB20_InCSR1 = 0x48;
	USB20_InCSR2 = 0x60;
	USB20_Index = 0;

	memcpy(UacVolCtrlSettings,(u8*)UacVolCtrlSettings_def,sizeof(UacVolCtrlSettings_def));
	UacSampFreq[0] = (u8)(__PCM_SAMPL__>>0);
	UacSampFreq[1] = (u8)(__PCM_SAMPL__>>8);
	UacSampFreq[2] = (u8)(__PCM_SAMPL__>>16);
	
//extern void (*fp_uac_off_hal)(void);
//extern bool (*fp_uac_on_hal)(void);

	//fp_uac_on_hal = NULL;
	//fp_uac_off_hal = NULL;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/


bool uac_unit_ctl_hal(u8 val, u8 rqu, u8 len)
{
	//UAC_FU_MUTE_CONTROL
	if(val == UAC_FU_MUTE_CONTROL){
		if(rqu == UAC_GET_CUR){
			(*fp_usb_ep0_tx)((u8* )&UacMuteControl,1);
		}
		else if(rqu == UAC_SET_CUR){
			usb_dev.ep0_state = RECEIVING_STATE;
			USB20_CSR0 = BIT(6);           //清除RxRdy		
		}
		else{
			debg("unkown val %x:\n",val);	
		}
	}
	
	//UAC_FU_VOLUME_CONTROL
	else if(val == UAC_FU_VOLUME_CONTROL){
		if(rqu == UAC_GET_CUR){
			(*fp_usb_ep0_tx)(&UacVolCtrlSettings[0],2);
		}
		else if(rqu == UAC_GET_RES){
			(*fp_usb_ep0_tx)(&UacVolCtrlSettings[2],2);
		}
		else if(rqu == UAC_GET_MIN){
			(*fp_usb_ep0_tx)(&UacVolCtrlSettings[4],2);
		}
		else if(rqu == UAC_GET_MAX){
			(*fp_usb_ep0_tx)(&UacVolCtrlSettings[6],2);
		}	
		else if(rqu == UAC_SET_CUR){
			//
			usb_dev.ep0_state = RECEIVING_STATE;
			USB20_CSR0 = BIT(6);           //清除RxRdy		
		}
		else if(rqu == UAC_SET_MIN){
			//
			usb_dev.ep0_state = RECEIVING_STATE;
			USB20_CSR0 = BIT(6);           //清除RxRdy		
		}
		else if(rqu == UAC_SET_MAX){
			//
			usb_dev.ep0_state = RECEIVING_STATE;
			USB20_CSR0 = BIT(6);           //清除RxRdy		
		}
		else if(rqu == UAC_SET_RES){
			//
			usb_dev.ep0_state = RECEIVING_STATE;
			USB20_CSR0 = BIT(6);           //清除RxRdy		
		}
		else{
			debg("unkown val %x:\n",val);
			return false; 	
		}	
	}
	return true;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
BOOL UacReceiveSetSamplingFreqCallback(u8* rxbuf)
{
	u8 i;
	for(i = 0; i < 3; i++){
		UacSampFreq[i] = rxbuf[i];
	}
	debgbuf(UacSampFreq,3);
	return true;	
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
bool UacHandleToStreaming(u8 val, u8 rqu, u8 len)
{
		pSetReqCallback = NULL;
		if (rqu == UAC_SET_CUR){
			if (val== UAC_EP_SAMPLING_FREQ_CONTROL)
			{
				if (len == 3)
				{
					pSetReqCallback = UacReceiveSetSamplingFreqCallback;
					usb_dev.ep0_state = RECEIVING_STATE;
					USB20_CSR0 = BIT(6);           //清除RxRdy	
				}
			}
		}
		if (rqu == UAC_GET_CUR)
		{
			if (val == UAC_EP_SAMPLING_FREQ_CONTROL)
			{
				(*fp_usb_ep0_tx)(UacSampFreq,2);
			}
		}
	return true;
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
volatile u8 uaconflg;
void uac_start(void)
{
//#if _UN_ASURE
	//ledsta = RECORD;
	u32 frq = (UacSampFreq[2] << 16) + (UacSampFreq[1] << 8) + (UacSampFreq[0] << 0);
	debg("adc frq:%x\n",frq);
	//agc_actech_to_usb(frq);
	//agc_on(1,syscfg.agc_en);
	hal_auadcStartForUAC(frq,100);
	uaconflg = true;
	//uac_isr_hal();
//#endif
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void uac_stop(void)
{
	ax32xx_auadcEnable(0);
	uaconflg = false;
#if _UN_ASURE
	//ledsta = IDLE;
	agc_off();
	pcmflg = false;
#endif
}












