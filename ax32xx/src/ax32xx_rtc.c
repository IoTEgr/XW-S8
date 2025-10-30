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
* File Name   : ax32xx_rtc.c
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
#include "../inc/ax32xx.h"

#define RTC_RD_RTCCNT    IRTRTC_RD_CMD
#define RTC_WR_RTCCNT    IRTRTC_WR_CMD
#define RTC_RD_RTCALM    IRTALM_RD_CMD
#define RTC_WR_RTCALM    IRTALM_WR_CMD
#define RTC_RD_RTCRAM    IRTRAM_RD_CMD
#define RTC_WR_RTCRAM    IRTRAM_WR_CMD
#define RTC_RD_IRTCON0   IRTCON0_RD_CMD
#define RTC_WR_IRTCON0   IRTCON0_WR_CMD
#define RTC_RD_IRTCON1   IRTCON1_RD_CMD
#define RTC_WR_IRTCON1   IRTCON1_WR_CMD
#define RTC_RD_IRTCON2   IRTCON2_RD_CMD
#define RTC_WR_IRTCON2   IRTCON2_WR_CMD
#define RTC_RD_IRTCON3   IRTCON3_RD_CMD
#define RTC_WR_IRTCON3   IRTCON3_WR_CMD
#define RTC_RD_IRTCON4   IRTCON4_RD_CMD
#define RTC_WR_IRTCON4   IRTCON4_WR_CMD
#define RTC_RD_IRTCON5   IRTCON5_RD_CMD
#define RTC_WR_IRTCON5   IRTCON5_WR_CMD
#define RTC_RD_IRTCON6   IRTCON6_RD_CMD
#define RTC_WR_IRTCON6   IRTCON6_WR_CMD
#define RTC_RD_IRTCON7   IRTCON7_RD_CMD
#define RTC_WR_IRTCON7   IRTCON7_WR_CMD
#define RTC_RD_IRTCON8   IRTCON8_RD_CMD
#define RTC_WR_IRTCON8   IRTCON8_WR_CMD
#define RTC_RD_IRTCON9   IRTCON9_RD_CMD
#define RTC_WR_IRTCON9   IRTCON9_WR_CMD
#define RTC_RD_IRTCON10   IRTCON10_RD_CMD
#define RTC_WR_IRTCON10   IRTCON10_WR_CMD
#define RTC_RD_IRTCDIVL   IRTCDIVL_RD_CMD
#define RTC_WR_IRTCDIVL   IRTCDIVL_WR_CMD
#define RTC_RD_IRTCDIVH   IRTCDIVH_RD_CMD
#define RTC_WR_IRTCDIVH   IRTCDIVH_WR_CMD


#define  R_SYS_CTRL0        PCON0
#define  R_RTC_CTRL         RTCCON
#define  R_RTC_DATA         RTCDATA
#define  R_RTC_STATUS      RTCCPND

void (*rtcAlamISR)(void);
void (*rtcSecondISR)(void);
/*******************************************************************************
* Function Name  : ax32xx_rtcWriteByte
* Description	 : rtc write byte
* Input 		 : u8 byte : byte value
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcWriteByte(u8 byte)
{
	volatile int timeout = 0x8000;
	R_RTC_DATA = byte;
	while(R_RTC_CTRL & (1<<3))
	{
		timeout--;
		if(timeout==0)
			break;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_rtcReadByte
* Description	 : rtc read byte
* Input 		 : 
* Output		 : None
* Return		 : u8
*******************************************************************************/
u8 ax32xx_rtcReadByte(void)
{
	u8 temp;
	volatile int timeout = 0x8000;
	
	R_RTC_DATA = 0;
	while(R_RTC_CTRL & (1<<3))
	{
		timeout--;
		if(timeout==0)
			break;
	}
	temp = R_RTC_DATA;
	
	return temp;		
}
/*******************************************************************************
* Function Name  : ax32xx_rtcRamRead
* Description	 : rtc read ram
* Input 		 : u8 cmd : ram addr
                    u8 *buffer : buffer
                    u8 len : length
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcRamRead(u8 addr,u8 *buffer,u8 len)
{
	int i;

	R_RTC_CTRL |= (1<<8);        //CE ENABLE 

	ax32xx_rtcWriteByte(RTC_RD_RTCRAM); // command
	ax32xx_rtcWriteByte(addr); // addr
	for(i=0;i<len;i++)
	    buffer[i] = ax32xx_rtcReadByte();
    R_RTC_CTRL &= ~(1<<8);       //CE DISABLE 
}
/*******************************************************************************
* Function Name  : ax32xx_rtcRamWrite
* Description	 : rtc write ram
* Input 		 : u8 cmd : ram addr
                    u8 *buffer : buffer
                    u8 len : length
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcRamWrite(u8 addr,u8 *buffer,u8 len)
{
	int i;

	R_RTC_CTRL |= (1<<8);        //CE ENABLE 

	ax32xx_rtcWriteByte(RTC_WR_RTCRAM); // command
	ax32xx_rtcWriteByte(addr); // addr
	for(i=0;i<len;i++)
	    ax32xx_rtcWriteByte(buffer[i]);
    R_RTC_CTRL &= ~(1<<8);       //CE DISABLE 	
}
/*******************************************************************************
* Function Name  : ax32xx_rtcDataRead
* Description	 : rtc read data
* Input 		 : u8 cmd : cmd
                    u8 *buffer : buffer
                    u8 len : length
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcDataRead(u8 cmd,u8 *buffer,u8 len)
{
	int i;
	
	R_RTC_CTRL |= (1<<8);        //CE ENABLE 

	ax32xx_rtcWriteByte(cmd);	
    for(i=0;i<len;i++)
    {
		buffer[i] = ax32xx_rtcReadByte();
    }
	R_RTC_CTRL &= ~(1<<8);       //CE DISABLE 
}
/*******************************************************************************
* Function Name  : ax32xx_rtcDataWrite
* Description	 : rtc write data
* Input 		 : u8 cmd : cmd
                    u8 *buffer : buffer
                    u8 len : length
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcDataWrite(u8 cmd,u8 *buffer,u8 len)
{
	int i;
	
	R_RTC_CTRL |= (1<<8);        //CE ENABLE 

	ax32xx_rtcWriteByte(cmd);	
    for(i=0;i<len;i++)
    {
		ax32xx_rtcWriteByte(buffer[i]);
    }
	R_RTC_CTRL &= ~(1<<8);       //CE DISABLE 	
}
/*******************************************************************************
* Function Name  : ax32xx_rtcInit
* Description	 : rtc initial
* Input 		 : 
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcInit(void)
{
	u8 buffer[4];
	u8 use_rc;
	ax32xx_rtcRamRead(8,(u8 *)&use_rc,1);  

	ax32xx_sysClkSet(SYS_CLK_RTC,1);
	R_RTC_CTRL = (0<<21)|//WKI1 input state
                 (0<<20)|//WKI0 input state
                 (0<<19)|//WKO input state
                 (0<<18)|//WK reset state
                 (0<<17)|//RTC POR bit state
                 (0<<16)|//weakup-pin weak-up reset enable
                 (0<<15)|//power down reset en
                 (0<<14)|//test en
                 (0<<13)|//irtc sleep weak-up en
                 (0<<12)|//rtc-por en
                 (0<<11)|//timer ie
                 (0<<10)|//alarm ie
                 (1<<9)| //second ie
                 (0<<8)| //w/r cs
                 (1<<6)| //w/r baudrate,0:DIV2,1:DIV64,2:DIV128,3:DIV256
                 (0<<4)| //power dowm reset time
                 (0<<3)| //w/r done-pending only-read
                 (0<<2)| //timer pending
                 (0<<1)| //alarm pending
                 (0<<0); //second pending

	R_RTC_STATUS = (1<<2)|(1<<1)|(1<<0); //clean all irq flash

    ax32xx_rtcDataRead(RTC_RD_IRTCON5,buffer,1);
    buffer[0] |= (7<<5); 
    ax32xx_rtcDataWrite(RTC_WR_IRTCON5,buffer,1);//clean weak-up pending

	if(0xAA == use_rc)	// have 32k xosc
	{
    	buffer[0] = BIT(0);  // enable rtc,battery
    	ax32xx_rtcDataWrite(RTC_WR_IRTCON0,buffer,1);
		uart_Printf("XOSC\n"); 
	}
	else
	{
	    u16 rc128k_div; 
	    u8 rc128k_ref;
	    u8 rc128k_trim;
	    u32 rc128k = ((*(u32 *)0x4fac)& 0xffffff)>>1; 	
	    rc128k_div = (rc128k & 0x3ff)?(rc128k & 0x3ff)+63500:64000;
	    rc128k_ref = (rc128k & 0x1fc00)?(rc128k & 0x1fc00)>>10:0x40;
	    rc128k_trim = (rc128k & 0x7e0000)?(rc128k & 0x7e0000)>>17:0x20;
		uart_Printf("RC128k=0x%x\n",rc128k); 

	    buffer[0] = (rc128k_div&0xff); 
	    ax32xx_rtcDataWrite(RTC_WR_IRTCDIVL,buffer,1); 
	    buffer[0] = (rc128k_div&0xff00)>>8; 
	    ax32xx_rtcDataWrite(RTC_WR_IRTCDIVH,buffer,1); 
	    
	    buffer[0] = (rc128k_trim<<1)|(1<<0); 
	    ax32xx_rtcDataWrite(RTC_WR_IRTCON7,buffer,1);//rtc128k enable[0]; rtc128trim[1:6]

	    buffer[0] = rc128k_ref&0x7f;
	    ax32xx_rtcDataWrite(RTC_WR_IRTCON8,buffer,1);//rtc128k enable[0]; rtc128trim[1:6]

	    //ax32xx_rtcDataWrite(RTC_RD_IRTCON0,buffer,1); 
	    buffer[0] |= (1<<4)|1|(1<<5);; 
	    ax32xx_rtcDataWrite(RTC_WR_IRTCON0,buffer,1);//rtc128k divide to 2Hz

	}

	rtcAlamISR = NULL;
	rtcSecondISR = NULL;

	ax32xx_intEnable(IRQ_RTC_WDT,1);
}
/*******************************************************************************
* Function Name  : ax32xx_rtcIRQHandler
* Description	 : rtc irq handler
* Input 		 : 
* Output		 : None
* Return		 : none
*******************************************************************************/
extern void hal_speed_callback(void);
extern void api_backsta_linking(void);
extern void api_usensor_1S_pollmonitor(void);
extern u32 hal_jB_fcnt_get(void);
void ax32xx_rtcIRQHandler(void)
{
	if(R_RTC_CTRL & BIT(0)) // second
	{
		R_RTC_STATUS |= BIT(0);
		//hal_speed_callback();
		//debg("1");
		//hal_auadc_cnt();
		//hal_jA_fcnt_get();
		//hal_jB_fcnt_get();
		//api_backsta_linking();
		//api_usensor_1S_pollmonitor();
		//agc_gain_mnt();
		//pt_javgsize();
		if(rtcSecondISR)
		{
			rtcSecondISR();	
		}
	}
	if(R_RTC_CTRL & BIT(1))//alam
	{
		R_RTC_STATUS |= BIT(1);
		if(rtcAlamISR)
		{
			rtcAlamISR();	
		}
	}	
	if(R_RTC_CTRL & BIT(2)){//rtctime 
		R_RTC_STATUS |= BIT(2);
	}		
}
/*******************************************************************************
* Function Name  : ax32xx_rtcSencodEnable
* Description	 : rtc enable second irq
* Input 		 :      u8 en : 1-enable
                        void (*callback)(void) : call back
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcSencodEnable(u8 en,void (*callback)(void))
{
	if(en)
	{
		R_RTC_STATUS |= BIT(0);
		R_RTC_CTRL |= (1<<9);
        
		rtcSecondISR = callback;
	}
	else
	{		
		R_RTC_CTRL &= ~(1<<9);
		R_RTC_STATUS |= BIT(0);
	}
}
/*******************************************************************************
* Function Name  : ax32xx_rtcAlamEnable
* Description	 : rtc enable alam irq
* Input 		 :      u8 en : 1-enable
                        void (*callback)(void) : call back
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcAlamEnable(u8 en,void (*callback)(void))
{
	u8 temp=0;
	if(en)
	{
		R_RTC_STATUS |= BIT(1);
		R_RTC_CTRL |= (1<<10);
        
		rtcAlamISR = callback;

	    //enable alarm weakup enable 
	    ax32xx_rtcDataRead(RTC_RD_IRTCON2,&temp,1); 
	    temp |= (1<<5)|(1<<0); 
	    ax32xx_rtcDataWrite(RTC_WR_IRTCON2,&temp,1);    

//	    ax32xx_WKOEnable(0); 
	}
	else
	{	
		ax32xx_rtcDataRead(RTC_RD_IRTCON2,&temp,1); 
	    temp &= ~((1<<5)|(1<<0)); 
		ax32xx_rtcDataWrite(RTC_WR_IRTCON2,&temp,1); 
		R_RTC_CTRL &= ~(1<<10);
		R_RTC_STATUS |= BIT(1);
	}	
}
/*******************************************************************************
* Function Name  : ax32xx_rtcAlamSet
* Description	 : rtc enable alam time
* Input 		 :      u32 value alam time in second
* Output		 : None
* Return		 : none
*******************************************************************************/
void ax32xx_rtcAlamSet(u32 value)
{
	u8 buffer[4];

	buffer[0] = (value>>24)&0xff;
	buffer[1] = (value>>16)&0xff;
	buffer[2] = (value>>8)&0xff;
	buffer[3] = value&0xff;
	ax32xx_rtcDataWrite(RTC_WR_RTCALM, buffer,4);	
}

/*******************************************************************************
* Function Name  : rtc_alarm_weakup_reset
* Description	 : rtc enable alam time
* Input 		 : second:     poweron after second
* Output		 : None
* Return		 : none
*******************************************************************************/
/*void rtc_alarm_weakup_reset(u32 second) 
{ 
    u8 temp; 
    ax32xx_rtcAlamSet(ax32xx_rtcGet() + second); 
     
    //clean alarm pending 
    temp = 0; 
    ax32xx_rtcDataWrite(RTC_WR_IRTCON2,&temp,1); 
     
    //enable alarm weakup enable 
    ax32xx_rtcDataRead(RTC_RD_IRTCON2,&temp,1); 
    temp |= (1<<5)|(1<<0); 
    ax32xx_rtcDataWrite(RTC_WR_IRTCON2,&temp,1);    

    ax32xx_WKOEnable(0); 
}

*/
/*******************************************************************************
* Function Name  : ax32xx_rtcGet
* Description	 : rtc get second value
* Input 		 :      
* Output		 : None
* Return		 : u32 : second
*******************************************************************************/
u32 ax32xx_rtcGet(void)
{
	u8 buffer[4];
    u32 value;
	
	ax32xx_rtcDataRead(RTC_RD_RTCCNT,buffer,4);

	value = buffer[0];
	value = (value<<8)+buffer[1];
	value = (value<<8)+buffer[2];
	value = (value<<8)+buffer[3];

	return value;
}
/*******************************************************************************
* Function Name  : ax32xx_rtcSet
* Description	 : rtc set value
* Input 		 :      u32 : second
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_rtcSet(u32 value)
{
	u8 buffer[4];

	buffer[0] = (value>>24)&0xff;
	buffer[1] = (value>>16)&0xff;
	buffer[2] = (value>>8)&0xff;
	buffer[3] = value&0xff;
	ax32xx_rtcDataWrite(RTC_WR_RTCCNT, buffer,4);
}
/*******************************************************************************
* Function Name  : ax32xx_WKOEnable
* Description	 : wko enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKOEnable(u8 en)
{
	u8 temp;
	R_RTC_CTRL |= (1<<6);
    if(en)
    {
#if SDRAM_PSRAM_SEL
        temp = (0<<4)|//weakup reset cpu en
               (0<<3)|//wko output select pull-up/down
               (1<<2)|//wko output data
               (1<<1)|//wko output en
               (0<<0);//wko input en
#else
		temp = (0<<4)|//weakup reset cpu en
			   (1<<3)|//wko output select pull-up/down
			   (1<<2)|//wko output data
			   (1<<1)|//wko output en
			   (0<<0);//wko input en
#endif
    }
    else
    {
        temp = (0<<4)|//weakup reset cpu en
               (0<<3)|//wko output select pull-up/down
               (0<<2)|//wko output data
               (1<<1)|//wko output en
               (0<<0);//wko input en
    }
    
	ax32xx_rtcDataWrite(RTC_WR_IRTCON4,&temp,1);
}
/*******************************************************************************
* Function Name  : ax32xx_VDDGSENEnable
* Description	 : VDD_GSEN enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_VDDGSENEnable(u8 en)
{
	u8 temp;
    
	ax32xx_rtcDataRead(RTC_RD_IRTCON0,&temp,1);	
    if(en)
    {
		temp |= (1<<6);
    }
    else
    {
        temp &=~(1<<6);
    }
    ax32xx_rtcDataWrite(RTC_WR_IRTCON0,&temp,1);

	ax32xx_sysCpuMsDelay(10);  // delay
}
/*******************************************************************************
* Function Name  : ax32xx_WKI0InputEnable
* Description	 : WKI input enable 
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKI0InputEnable(u8 en)
{
	u8 temp;

	temp = 0;
	ax32xx_rtcDataRead(RTC_RD_IRTCON3,&temp,1);
	if(en)
	{			
		temp |= (1<<0);//|(1<<2)|(1<<5);
	}
	else
	{
		temp &= ~(1<<0);
	}

    ax32xx_rtcDataWrite(RTC_WR_IRTCON3,&temp,1);

}
/*******************************************************************************
* Function Name  : ax32xx_WKIEnable
* Description	 : WKI input enable 
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKI1InputEnable(u8 en)
{
	u8 temp;

	temp = 0;
	ax32xx_rtcDataRead(RTC_RD_IRTCON3,&temp,1);
	if(en)
	{
//		temp &= ~(1<<4);
		temp |= (1<<3);//|(1<<2)|(1<<5);		
	}
	else
	{
//		temp |= (1<<4);
		temp &= ~(5<<3);//|(1<<2)|(1<<5);		
	}
	ax32xx_rtcDataWrite(RTC_WR_IRTCON3,&temp,1);


}

/*******************************************************************************
* Function Name  : ax32xx_WKI1Read
* Description	 : WKI 1 read
* Input 		 :      
* Output		 : None
* Return		 : 0-low,1-high
*******************************************************************************/
u8 ax32xx_WKI1Read(void)
{
	if(R_RTC_CTRL & (1<<21))
		return 1;
	else
		return 0;
}
/*******************************************************************************
* Function Name  : ax32xx_WKI0Read
* Description	 : WKI 0 read
* Input 		 :      
* Output		 : None
* Return		 : 0-low,1-high
*******************************************************************************/
u8 ax32xx_WKI0Read(void)
{
	if(R_RTC_CTRL & (1<<20))
		return 1;
	else
		return 0;

}
/*******************************************************************************
* Function Name  : ax32xx_WKI1WakeupEnable
* Description	 : WKI 1 wakeup enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKI1WakeupEnable(u8 en)
{
	u8 temp;
    R_RTC_CTRL |= (1<<6);
    if(en)
    {
		temp = (7<<5);
		ax32xx_rtcDataWrite(RTC_WR_IRTCON5,&temp,1);//clean weak-up pending
        
		temp = (0<<5)|(0<<4);
		ax32xx_rtcDataWrite(RTC_WR_IRTCON6,&temp,1);//wki1 level triger
		
	    ax32xx_rtcDataRead(RTC_RD_IRTCON2,&temp,1);
		temp |= (1<<3);
		ax32xx_rtcDataWrite(RTC_WR_IRTCON2,&temp,1);//enable wki1 weak-up,when weak-up,WKO will output High
		//R_RTC_CTRL |= (1<<16);                 //weak-up reset cpu
		
		ax32xx_rtcDataRead(RTC_RD_IRTCON4,&temp,1);
		temp |= (1<<4);
		ax32xx_rtcDataWrite(RTC_WR_IRTCON4,&temp,1);//weak-up reset cpu
		temp = (1<<3);
		ax32xx_rtcDataWrite(RTC_WR_IRTCON3,&temp,1);//enable wki1 input
    }
	else
	{
		ax32xx_rtcDataRead(RTC_RD_IRTCON3,&temp,1);
		temp &= ~(7<<3);
        ax32xx_rtcDataWrite(RTC_WR_IRTCON3,&temp,1);
		
		ax32xx_rtcDataRead(RTC_RD_IRTCON5,&temp,1);
		temp |= (7<<5);
		ax32xx_rtcDataWrite(RTC_WR_IRTCON5,&temp,1);//clean weak-up pending
	
		ax32xx_rtcDataRead(RTC_RD_IRTCON2,&temp,1);
		temp &= ~(1<<3); 
	    ax32xx_rtcDataWrite(RTC_WR_IRTCON2,&temp,1);//disable wki0/wki1/wko weak-up   	   
	}
	 
}
void ax32xx_WKI1WakeupTriger(u8 en)
{
	u8 temp;
	if(en)
	{
		temp = (0<<5)|(1<<4);
		ax32xx_rtcDataWrite(RTC_WR_IRTCON6,&temp,1);//wki1 wakeup rising edge triger
	}
	else
	{
		temp = (0<<5)|(0<<4);
		ax32xx_rtcDataWrite(RTC_WR_IRTCON6,&temp,1);//wki1 level triger
	}
}
/*******************************************************************************
* Function Name  : ax32xx_WKI1CleanPending
* Description	 : WKI 1 clean wakeup pending
* Input 		 : None
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_WKI1CleanPending(void)
{
    u8 temp = (7<<5);
    ax32xx_rtcDataWrite(RTC_WR_IRTCON5,&temp,1);//clean all weak-up pending
}
/*******************************************************************************
* Function Name  : ax32xx_rtcBatDecEnable
* Description	 :  rtc BATTERY dectection enable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_rtcBatDecEnable(u8 en)
{
	u8 temp;
	ax32xx_rtcDataRead(RTC_RD_IRTCON0,&temp,1);
	if(en)
		temp |= 1<<5;
	else 
		temp &= ~(1<<5);
	ax32xx_rtcDataWrite(RTC_WR_IRTCON0,&temp,1);

	ax32xx_sysCpuMsDelay(10);  // delay
}
/*******************************************************************************
* Function Name  : ax32xx_rtcSenHVEnable
* Description	 :  ax32xx_rtcSenHVEnable
* Input 		 :      u8 en : 0->disable,1->enable
* Output		 : None
* Return		 : 
*******************************************************************************/
void ax32xx_rtcSenHVEnable(u8 en)
{
	u8 temp;
	ax32xx_rtcDataRead(RTC_RD_IRTCON0,&temp,1);
	if(en)
		temp |= 1<<7;
	else 
		temp &= ~(1<<7);
	ax32xx_rtcDataWrite(RTC_WR_IRTCON0,&temp,1);

	ax32xx_sysCpuMsDelay(10);  // delay
}


