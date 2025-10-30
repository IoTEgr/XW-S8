/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         RTC DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_rtc.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN RTC HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#ifndef AX32XX_RTC_H
    #define AX32XX_RTC_H


#define  WKI_0      0
#define  WKI_1      1



/*******************************************************************************
* Function Name  : ax32xx_rtcIRQHandler
* Description	 : rtc irq handler
* Input 		 : 
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_rtcWriteByte
* Description	 : rtc write byte
* Input 		 : u8 byte : byte value
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcWriteByte(u8 byte);
/*******************************************************************************
* Function Name  : ax32xx_rtcReadByte
* Description	 : rtc read byte
* Input 		 : 
* Output		 : None
* Return		 : u8
*******************************************************************************/
u8 ax32xx_rtcReadByte(void);
/*******************************************************************************
* Function Name  : ax32xx_rtcRamRead
* Description	 : rtc read ram
* Input 		 : u8 cmd : ram addr
                    u8 *buffer : buffer
                    u8 len : length
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcRamRead(u8 addr,u8 *buffer,u8 len);
/*******************************************************************************
* Function Name  : ax32xx_rtcRamWrite
* Description	 : rtc write ram
* Input 		 : u8 cmd : ram addr
                    u8 *buffer : buffer
                    u8 len : length
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcRamWrite(u8 addr,u8 *buffer,u8 len);
/*******************************************************************************
* Function Name  : ax32xx_rtcDataRead
* Description	 : rtc read data
* Input 		 : u8 cmd : cmd
                    u8 *buffer : buffer
                    u8 len : length
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcDataRead(u8 cmd,u8 *buffer,u8 len);
/*******************************************************************************
* Function Name  : ax32xx_rtcDataWrite
* Description	 : rtc write data
* Input 		 : u8 cmd : cmd
                    u8 *buffer : buffer
                    u8 len : length
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcDataWrite(u8 cmd,u8 *buffer,u8 len);
/*******************************************************************************
* Function Name  : ax32xx_rtcInit
* Description	 : rtc initial
* Input 		 : 
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcInit(void);
/*******************************************************************************
* Function Name  : ax32xx_rtcSencodEnable
* Description	 : rtc enable second irq
* Input 		 :      u8 en : 1-enable
                        void (*callback)(void) : call back
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcSencodEnable(u8 en,void (*callback)(void));
/*******************************************************************************
* Function Name  : ax32xx_rtcAlamEnable
* Description	 : rtc enable alam irq
* Input 		 :      u8 en : 1-enable
                        void (*callback)(void) : call back
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcAlamEnable(u8 en,void (*callback)(void));
/*******************************************************************************
* Function Name  : ax32xx_rtcAlamSet
* Description	 : rtc enable alam time
* Input 		 :      u32 value alam time in second
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcAlamSet(u32 value);
/*******************************************************************************
* Function Name  : rtc_alarm_weakup_reset
* Description	 : rtc enable alam time
* Input 		 : second:     poweron after second
* Output		 : None
* Return		 : none
*******************************************************************************/
void rtc_alarm_weakup_reset(u32 second) ;
/*******************************************************************************
* Function Name  : ax32xx_rtcGet
* Description	 : rtc get second value
* Input 		 :      
* Output		 : None
* Return		 : u32 : second
*******************************************************************************/
u32 ax32xx_rtcGet(void);
/*******************************************************************************
* Function Name  : ax32xx_rtcSet
* Description	 : rtc set value
* Input 		 :      u32 : second
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_rtcSet(u32 value);

/*******************************************************************************
* Function Name  : ax32xx_WKOEnable
* Description	 : wko enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKOEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_VDDGSENEnable
* Description	 : VDD_GSEN enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_VDDGSENEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_WKIEnable
* Description	 : WKI input enable 
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKI0InputEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_WKIEnable
* Description	 : WKI input enable 
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKI1InputEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_WKI1Read
* Description	 : WKI 1 read
* Input 		 :      
* Output		 : None
* Return		 : 0-low,1-high
*******************************************************************************/
u8 ax32xx_WKI1Read(void);
/*******************************************************************************
* Function Name  : ax32xx_WKI0Read
* Description	 : WKI 0 read
* Input 		 :      
* Output		 : None
* Return		 : 0-low,1-high
*******************************************************************************/
u8 ax32xx_WKI0Read(void);
/*******************************************************************************
* Function Name  : ax32xx_WKI1WakeupEnable
* Description	 : WKI 1 wakeup enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKI1WakeupEnable(u8 en);
void ax32xx_WKI1WakeupTriger(u8 en); //1: rising triger, 0: level triger
/*******************************************************************************
* Function Name  : ax32xx_rtcBatDecEnable
* Description	 :  rtc BATTERY dectection enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_rtcBatDecEnable(u8 en);
/*******************************************************************************
* Function Name  : ax32xx_WKI1CleanPending
* Description	 : WKI 1 clean wakeup pending
* Input 		 : None
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKI1CleanPending(void);
/*******************************************************************************
* Function Name  : ax32xx_rtcSenHVEnable
* Description	 :  ax32xx_rtcSenHVEnable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_rtcSenHVEnable(u8 en);





#endif

