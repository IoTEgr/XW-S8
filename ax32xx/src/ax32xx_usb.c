/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         USB DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_usb.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN USB HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"

#define TESTXOSC12	0


static void (*ax32xxUSBIRQ[USB_IRQ_MAX])(void);
static volatile u16 usbIsrCnt=0;

//void (*fp_enum_epx_cfg)(u8 devtyp); //enum init pfun
//void (*fp_msc_epx_cfg)(void);
//void (*fp_uac_epx_cfg)(void);
//void (*fp_uvc_epx_cfg)(void);

//extern void tmout_reset(void);
//extern bool tmout_occur(u32 tout);
#define combo_delay(t)  do {int i=(t)*2; while(i--) asm("l.nop"); } while(0)

const u32 ExTADR[6] = {(u32)&USB20E0ADR, (u32)&USB20E1TADR,(u32)&USB20E2TADR,(u32)&USB20E3TADR,(u32)&USB20E4TADR,(u32)&USB20E5TADR};
const u32 ExRADR[6] = {(u32)&USB20E0ADR, (u32)&USB20E1RADR,(u32)&USB20E2RADR,(u32)&USB20E3RADR,(u32)&USB20E4RADR,(u32)&USB20E5RADR};
//#define __U20ExTADR__(x)  *((volatile __sfr u32 *)(ExTADR[x]))
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/*void write_usb_reg(uint8_t addr, uint8_t reg_data)
{
    USBCON4 = (reg_data<<8) |
              (1<<7) | //kst
              addr;
	while((USB20CON0 & 0x02)==0x00);
}*/

/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*uint8_t read_usb_reg(uint8_t addr)
{
    USBCON4 = (1<<7) | //kst
              (1<<6) | //read
              addr;
	while((USB20CON0 & 0x02)==0x00);
	return USBDATA;	
}*/

/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
******************************************************************************
void usb_dev_switch(void)
{
	if(syscfg.devtyp == MIN_DV){
		if(syscfg.usbmod == __USBNC__){
			
			syscfg.usbtypdev++;
			if(syscfg.usbtypdev >= USBDEV_NUM)
				syscfg.usbtypdev = 0;;
			debg("usb swt:%d\n",syscfg.usbtypdev);
			usb_init();				
		}
	}
}*/	



/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
volatile u8 mscbusy;

bool _usb_bulk_tx(u8 epx, u32 adr, u16 len,u16 blkszie)
{	
	//debg("usb_bulk_tx\n");
	mscbusy = true;
//	ClrIntSrc(_USB_INT_);//USBIE_DIS();
	ax32xx_intEnable(IRQ_USB20,0);
	
	u8 ret = true;
	u8 tepx = USB20_Index; //保存idex
    u16 txlen;
    u32 tx_addr = adr;//EP1_ADDR;
	
	
	USB20_Index = (epx & 0x7f);               //选用EP1做为 Mass
	
	volatile int i = 0x2ffff;
	//debg("u-i:%x\n",epx);
    while (len) {
        txlen = (len >= blkszie) ? blkszie : len;
		
		//*((u32 *)(&USB20E1TADR + 2 * (epx-1))) = tx_addr;
		__MSCTADR__ = adr;
		//USB20E3TADR = adr;
		//__U20ExTADR__(epx) = adr;
		
		USB20CON1 = BIT(13 + epx) | txlen;
		USB20_InCSR1 |= BIT(0);
        len -= txlen;
        tx_addr += txlen;
			
        i = 0x2fffff;
		while (USB20_InCSR1 & BIT(0))
		{			
			i--;
			if(i<=0)
			{
				//debg("msc btx tmo\n");
				ret = false;
				break;
			}
		}
    }
	__MSCTADR__ = (u32)adr; //复位地址
	//USB20E3TADR = (u32)adr; //复位地址
	//__U20ExTADR__(epx) = adr;
	USB20_Index=tepx;
	
//	SetIntSrc(_USB_INT_);//USBIE_EN();
	ax32xx_intEnable(IRQ_USB20,1);
	
	mscbusy = false;
	return ret;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool _usb_bulk_rx(u8 epx, u32 adr, u16 len,u16 blkszie)
{	
	//debg("usb_bulk_rx\n");
//	ClrIntSrc(_USB_INT_);					//USBIE_DIS();
	ax32xx_intEnable(IRQ_USB20,0);
	u8 ret = true;
	u8 cnt = 0;
	u8 tepx = USB20_Index; //保存idex
	u8 error = false;
    u16 rxlen;
    u32 rx_addr = adr;//EP1_ADDR;

	USB20_Index = (epx & 0x7f);               //选用EP1做为 Mass
	
	//debg("u-i:%x\n",epx);
	volatile int i = 0xffffff;
    while (!error && len) 
	{
        rxlen = (len >= blkszie) ? blkszie : len;
		
        __MSCRADR__ = rx_addr;
		//USB20E3RADR = rx_addr;
		USB20_OutCSR1 = BIT(4);           //Flush FIFO, Clear RxRdy(接收下一个包)

        i = 0x2fffff;
		while (!error && !(USB20_OutCSR1 & BIT(0)))
		{
		    i--;
			if(i<=0)
			{
				ret = false;
				break;
			}		
		}
		
		cnt++;
		 
        len -= rxlen;
        rx_addr += rxlen;
    }

	USB20_Index = tepx;
//	SetIntSrc(_USB_INT_);//USBIE_EN();
	ax32xx_intEnable(IRQ_USB20,1);
	return ret;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool _usb_iso_kick(u8 epx, u32 adr, u16 len)
{
//	ClrIntSrc(_USB_INT_);					//USBIE_DIS();	
	ax32xx_intEnable(IRQ_USB20,0);
	u8 tepx = USB20_Index; //保存idex
	//debg(":%x,%x,%d\n",epx,adr,len);
	USB20_Index = (epx & 0x7f);               //选用EP1做为 Mass
	
	//*((u32 *)(&USB20E1TADR + 2 * (epx-1))) = adr;
	if(2 == epx)
		USB20E2TADR = adr;
	if(3 == epx)
		USB20E3TADR = adr;
	if(4 == epx)
		USB20E4TADR = adr;
	//USB20E4TADR = adr;
	USB20CON1 = BIT(13 + epx) | len;
	
//	if(USB20_InCSR1 & BIT(1))
//		debg(">:%x\n",USB20_InCSR1);	
	//if(USB20_InCSR1&0x80)
	//	USB20_InCSR1 &=~0x80;
	USB20_InCSR1 |= BIT(7);
	if(USB20_InCSR1 & BIT(2)){
		USB20_InCSR1 &= ~BIT(2);
		//debg(">:%x\n",USB20_InCSR1);
	}
	USB20_InCSR1 |= BIT(0);//set bit txpktrdy
	
	/*volatile u32 tmout = 0xfffff;
	while (1){
		if(!(--tmout)){
			//debg("iso tout\n");
			break;
		}
		if(!(USB20_InCSR1 & 0x03)){
			break;
		}
		
	};*/
	 
	USB20_Index = tepx;
//	SetIntSrc(_USB_INT_);//USBIE_EN()
    ax32xx_intEnable(IRQ_USB20,1);
	return true;
}
bool _iso_wait_txdown(u8 epx)
{
		volatile u32 tmout = 0xfffff;
		u8 tepx = USB20_Index; //保存idex
		USB20_Index = (epx & 0x7f);  
		while (1){
			if(!(--tmout)){
				debg("iso tout\n");
				USB20_Index = tepx;	
				return false;
			}
			if(!(USB20_InCSR1 & BIT(0))){
				USB20_Index = tepx;
				return true;
			}
		};//waitting iso paket
		
		
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool usb_iso_tx(u8 epx, u32 adr, u16 len)
{
//	ClrIntSrc(_USB_INT_);					//USBIE_DIS();	
	ax32xx_intEnable(IRQ_USB20,0);
	u8 tepx = USB20_Index; //保存idex
	//debg(":%x,%x,%d\n",epx,adr,len);
	USB20_Index = (epx & 0x7f);               //选用EP1做为 Mass
	
	//*((u32 *)(&USB20E1TADR + 2 * (epx-1))) = adr;
	USB20E4TADR = adr;
	USB20CON1 = BIT(13 + epx) | len;
		
	if(USB20_InCSR1&0x80)
		USB20_InCSR1 &=~0x80;
	USB20_InCSR1 &=~0x04;
	USB20_InCSR1 |= 0x01;//set bit txpktrdy
	
	volatile u32 tmout = 0xfffff;
	while (1){
		if(!(--tmout)){
			//debg("iso tout\n");
			break;
		}
		if(!(USB20_InCSR1 & 0x03)){
			break;
		}		
	};
	 
	USB20_Index = tepx;
//	SetIntSrc(_USB_INT_);//USBIE_EN()
	ax32xx_intEnable(IRQ_USB20,1);
	return true;
}


/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void usb_burst_reset(void)
{	 
		//初始化SIE寄存器
	USB20_Power = 0x60;
	USB20_FAddr = 0x00;
	USB20_Testmode = 0x00;
	USB20_Index = 0;
	
	//usb sie中断使能
	USB20_IntrUSBE  = USB_RESET | USB_SUSPEND | USB_SOF;
	USB20_IntrInel = USB_INEP_INT(0)|USB_OUTEP_INT(3);//USB_INEP_INT(0)|USB_INEP_INT(1)|USB_INEP_INT(2)|USB_OUTEP_INT(3);
	USB20_IntrOutel = 0;;//USB_OUTEP_INT(1)|USB_OUTEP_INT(2)|USB_OUTEP_INT(3);
	usbIsrCnt = 0;
/*
	//msc init
	(*fp_enum_epx_cfg)(0);
	(*fp_msc_epx_cfg)();
	(*fp_uvc_epx_cfg)();
	(*fp_uac_epx_cfg)();

*/	
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void usb_host_reset(void)
{
	USB20_IntrInel |= BIT(0);	//ep0 isr en;
	//USB20_IntrUSBE &=~BIT(2);  //when usb 2.0 line 10m,disable "Babble Error int"
	USB20_IntrUSBE |= 
					 BIT(4) //connect	isr en	
					|BIT(5);//disconnect isr en
	USB20_Devctl=0x03;

	//ep0 cfg
	(USB20_FAddr) = 0;
	
	/*
	(USB20_Index) = 1;		// set ep1 IN/OUT max packet length, 512 bytes
	(USB20_InMaxPl) = 0;
	(USB20_OutMaxPl) = 0;
	(USB20_InMaxPh) = 0x02;
	(USB20_OutMaxPh) = 0x02;

	(USB20_Index) = 2;		// set ep2 IN/OUT max packet length, 512 bytes
	(USB20_InMaxPl) = 0;
	(USB20_OutMaxPl) = 0;
	(USB20_InMaxPh) = 2;
	(USB20_OutMaxPh) = 2;//

	(USB20_Index) = 3;		// set ep2 IN/OUT max packet length, 512 bytes
	(USB20_InMaxPl) = 0;
	(USB20_OutMaxPl) = 0;
	(USB20_InMaxPh) = 1;
	(USB20_OutMaxPh) = 1;//
	
	//debg("USB20_Index:%x\n",USB20_Index);
	 */
}
/*******************************************************************************
* Function Name  : timer_Timer1_Stop
* Description    : timer1 stop
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/		
void usb_phyreset_as_typeb(void)
{
	USB20CON0   = 0x21307400;
	CLKCON2 &= ~BIT(16);
	combo_delay (100);
	
	debg("Usb1.1 and Usb2.0 Init\r\n");
	#if TESTXOSC12
		PFDIE   &= ~(3<<8);  // die pf8,pf9 for xosc12M
		CLKCON2 |= 1<<3;     // xosc12M en
		USB20CON0 |= (1<<26);  // sel test clk in from pa13
	#else
		CLKCON2 |= BIT(16);
		//PMAPCON0 = 0;
		//PMAPCON1 &= ~(0xF00);
		//PADIR &= ~BIT(2);
		CLKCON0 = (CLKCON0 & ~(BIT(14)|BIT(15))) | (BIT(16)|BIT(14));
	#endif

#if TESTXOSC12
	//PADIR   |= 1<<13;   // PA13 input test clock
	//USB20CON0 |= 1<<9 ;   // test clk en
#endif	
 //////////////////////////////////////////////////////
   
	USB20CON0 |= BIT(16); //type B device
	combo_delay (1);
	USB20CON0 &= ~(BIT(17)                   //PULL R control by SIE
			   |BIT(22)|BIT(23)            //USB PLL SEL
			   |BIT(24)|BIT(25)            //USB  OSC
			   |BIT(11)
               |BIT(10)|BIT(20)|BIT(21)) ;                  //no down stream port
    USB20CON0 |= BIT(12)|BIT(28);
    //USB20CON0 |= BIT(15)|BIT(14)|BIT(13);
	
	//复位USB PHY
	// 使能USB PHY OSC, USBCONUSB20CON0等待1uS穩定
	USB20CON0 |= 0x80;	// OSC enable
	combo_delay (1);
	// 使能USB PHY PLL, USBCONUSB20CON0等待200uS穩定
	USB20CON0 |= 0x40;	// PLL enable
	combo_delay (1000);
	// 釋放USB SIE復位,USBCON0[0]=1
	USB20CON0 |= 0x1;	//release sie reset
	combo_delay (1);
	// 釋放USB PHY復位,USBCON0[2]=1,等待300uS使ClkTun穩定 modi @20131216
	USB20CON0 |= 0x4;	//release phy reset
	combo_delay (100);
}
/*******************************************************************************
* Function Name  : usb_phyreset_as_typea
* Description    : usb type set
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
extern void XOSTimeDly(INT32U dely);	
void usb_phyreset_as_typea(void)
{
	//disable upll, uosc ,fix usb hot insert bug
	USB20CON0 = 0;
	XOSTimeDly(100); 
#if 1  
	USB20CON0 |= 0x80;	//osc enable
	combo_delay(10);
	USB20CON0 |= 0x40;	//pll enable
	combo_delay(10);
	USB20CON0 |= 0x6400;//vbusvalid adevice downstream_port
	combo_delay(10);
	USB20CON0 |= 0x1;	//release sie reset
	combo_delay(10);
	USB20CON0 |= 0x4;	//release phy reset
	combo_delay(10);
#endif
	// initial USB host phy
	debg("husb:<init as usb typA...>\n");
	XOSTimeDly(1000); 
	#if TESTXOSC12
		PFDIE   &= ~(3<<8);  // die pf8,pf9 for xosc12M
		CLKCON2 |= 1<<3;     // xosc12M en
		USB20CON0 |= (1<<26);  // sel test clk in from pa13
	#else
/*
		CLKCON2 |= BIT(16);
		PMAPCON0 = 0;
		PMAPCON1 &= ~(0xF00);
		PADIR &= ~BIT(2);
		CLKCON0 = (CLKCON0 & ~(BIT(14)|BIT(15))) | (BIT(16)|BIT(14));
*/
	#endif
	USB20CON0 &= ~(BIT(22)|BIT(23) //pll div2
				|BIT(21)|BIT(20)
				);
	USB20CON0 |= BIT(22)
				|BIT(18) |BIT(19) //10k pulldown
				|BIT(13)			//VBUS
				//|BIT(9)			//VBUS
				;
				
	USB20CON0 &= ~(BIT(17)			//PULL R control by SIE
				| BIT(16)			//type A
				| BIT(24) | BIT(25))	//USB OSC
				;
#if 0
	USB20CON0 |= 0x80;	//osc enable
	combo_delay(10);
	USB20CON0 |= 0x40;	//pll enable
	combo_delay(10);
	USB20CON0 |= 0x6400;//vbusvalid adevice downstream_port
	combo_delay(10);
	USB20CON0 |= 0x1;	//release sie reset
	combo_delay(10);
	USB20CON0 |= 0x4;	//release phy reset
	combo_delay(10);
#endif

	//usb_type = HIGH_SPEED;//

	//if(usb_type == HIGH_SPEED)
	USB20_Power = 0x70; // Software Connection Control Enable, High-Speed Enable, High-Speed Mode Enable
	//else
	//	USB20_Power = 0x58;

	//USB20_Devctl=0x03;

	//reset_int();//SFR INIT
	//deg("usb host action\n");
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
volatile u16 sofcnt;
void usb_insert(void)
{
#if __UN_ASURE__
	usbsta = USB_ACTECH;
	
	write_usb_reg(IntrUSBE,/*USB_SOF|*/USB_RESET|USB_SUSPEND);
	usb_msg = USB_INSERT;
	msg_crtl_put(&usb_msg);
	
	sofcnt++;
#endif
}



/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void usb_check_hal(void)
{
#if __UN_ASURE__
	if(USB_ACTECH == usbsta){
		if(!dcin_detect()){
			usbsta = USB_DETACH;
			
			//发送拔出消息
			usb_msg = USB_REMOVE;
			msg_crtl_put(&usb_msg);				
		}		
	}
	return;
#endif 
}

/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
static volatile u16 u32softcnt;
u32 get_u32softcnt(void)
{
	return u32softcnt;
}
/*******************************************************************************
* Function Name  : ax32xx_usbIRQHanlder
* Description    : USB irq handler
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_usbIRQHanlder(void)
{
	
	usbIsrCnt++;
	//debg("uisr\n");
    u8 intrusb = USB20_IntrUSB;//& read_usb_reg(IntrUSBE);
    //u8 introut1 = read_usb_reg(IntrRx1);
    u8 intrin1 = USB20_IntrInl;
	//debg(".%x,%x\n",intrin1,USB20_IntrUSB);
    if (intrusb & BIT(2)) 
	{
       // debg("USB -reset\n");
        usb_burst_reset();
		if(ax32xxUSBIRQ[USB_IRQ_RESET])
			ax32xxUSBIRQ[USB_IRQ_RESET]();
    }
	else{
		
        if (intrusb & BIT(0)) {
            //USB Suspend
            USB20_IntrUSBE = (BIT(2) | BIT(1) | BIT(3));  //Enable Resume int
            debg("USB -suspend\n");
			//usb_init();
        } else if (intrusb & BIT(1)) {
            //USB Resume
            USB20_IntrUSBE = BIT(2) | BIT(3);  //Only Reset
            debg("USB -resume\n");
			
        } else if (intrusb & BIT(3)) {
            //USB sof
            USB20_IntrUSBE = BIT(2) | BIT(3);  //Only Reset //Only Reset
			//debg("- USB sof \n");
			u32softcnt++;
		
			usb_insert();
        }		
	}
    
    if (intrin1 & BIT(0)) {
		if(ax32xxUSBIRQ[USB_IRQ_EP0])
			ax32xxUSBIRQ[USB_IRQ_EP0]();
		
    }
    if (intrin1 & BIT(4)) {
		//debg("uvc_isr\n");
    }	
}
/*******************************************************************************
* Function Name  : ax32xx_usbCallbackRegister
* Description    : USB CALLBACK REGISTER
* Input          : u8 type : USB_IRQ_E
                      void (*callback)(void) : callback
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_usbCallbackRegister(u8 type,void (*callback)(void))
{
	if(type<USB_IRQ_MAX)
	{
		ax32xxUSBIRQ[type] = callback;
	}
}


u8 (*fp_epout_isr_hal)(void);
u8 (*fp_host_connect_hal)(void);
u8 (*fp_host_remove_hal)(void);
u8 (*fp11_host_connect_hal)(void);
u8 (*fp11_host_remove_hal)(void);
extern bool (*fp_mjpBcodec_linking)(void);
extern bool api_backrec_status(void);
extern void usb_stack_init(void);
extern void husb_kick_ep0isr(void);
extern void husb_ep0isr(void);
void ax32xx_usb_hostIRQHanlder(void)
{
#if 0
	bool transta = api_backrec_status();
    u8 intrusb = USB20_IntrUSB;//& read_usb_reg(IntrUSBE);
    u8 intrin1 = USB20_IntrInl;
	USB20_IntrInh;//u8 intrinh = USB20_IntrInh;
	u8 introul = USB20_IntrOutl;
	USB20_IntrOuth;//u8 introuh = USB20_IntrOuth;
    //u8 introut1 = read_usb_reg(IntrRx1);
  /*  u8 intrin1 = USB20_IntrInl;
	u8 intrinh = USB20_IntrInh;
	u8 introul = USB20_IntrOutl;
	u8 introuh = USB20_IntrOuth;*/
	//debg("usb_host\n");
	//debg(".%x,%x,%x,%x,%x\n",intrusb,intrin1,intrinh,introul,introuh);
	
    if (introul){ //USB epxout
		if(fp_epout_isr_hal)
			(*fp_epout_isr_hal)();
    }
	
	//insert
	if(intrusb & BIT(4)){
		debg("husb: <insert>\n");
		(*fp_host_connect_hal)();
		return;
	}
	
	//remove
	if(intrusb & BIT(5)){
		debg("husb: <remove>\n");
		(*fp_host_remove_hal)();
		
		return;
			
	}
	
	//USB bable err
    if (intrusb & BIT(2)){ 
		
		if(transta){ //epx tran state
			debg("ERR:-bable <reset usb dev>\n");
			(*fp_host_remove_hal)();
		}else{ //ep0 enum state
			if(fp_mjpBcodec_linking) //usensor had acteched ok but 1st mjp decode is not ok
			{
				debg("ERR:-bable <reset usb dev>\n");
				(*fp_host_remove_hal)();
			}else{
				debg("ERR:-bable <enum retry >\n");
				usb_stack_init();
				husb_kick_ep0isr();//husb_ep0isr();
			}
		}
		return;
    }
	
	if(intrin1 & BIT(0)){
		husb_ep0isr();	
	}	
#endif
}

s32 usb_device_check(void)
{
	u32 tmp0,tmp1;
	#define _TEST_JK_MOD0_	0
	REG32(USB20CON0) = 0;
	REG32(USB20CON0) |= (1<<17);//control by soft
	REG32(USB20CON0) |= ((1<<18)|(1<<19));//enable dp,dm 10K pdown
	REG32(USB20CON0) &= ~((1<<21)|(1<<20)); //disable dp,dm 120K pullup
	ax32xx_sysCpuMsDelay(60);
	tmp0 = (REG32(USB20CON0) >> 30) & 0x03 ;
	debg("<<<J,K :%x\n",tmp0);
	
	REG32(USB20CON0) |= (1<<17);//control by soft
	REG32(USB20CON0) &= ~((1<<18)|(1<<19));//disable dp,dm 10K pdown
	REG32(USB20CON0) |= ((1<<21)|(1<<20)); //enable dp,dm 120K pullup
	ax32xx_sysCpuMsDelay(60);
	tmp1 = (REG32(USB20CON0) >> 30) & 0x03 ; 
	
	debg(">>>J,K :%x\n",tmp1);
	if(_TEST_JK_MOD0_ == (tmp0 | tmp1))// && (usbIsrCnt)
	{
         return 0;
	}
	
	return -1;


}
/*******************************************************************************
* Function Name  : ax32xx_usbPhyPllEnable
* Description    : enable usb phy pll(96M/120M/240M/480M)
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ax32xx_usbPhyPllEnable(void)
{
	USB20CON0 &= ~(BIT(24) | BIT(25));	//USB OSC
	USB20CON0 |= 0x80;	//osc enable
	combo_delay(10);
	USB20CON0 |= 0x40;	//pll enable
	combo_delay(10);
	USB20CON0 |= 0x4;	//release phy reset
	combo_delay(10);
}
/*******************************************************************************
* Function Name  : usb11_registers_write
* Description    : usb11_registers_write
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 (*fp_host11_connect_hal)(void);
u8 (*fp_host11_remove_hal)(void);
//usb1.1 SIE registers write
void usb11_registers_write(u8 addr, u8 reg_data)
{
	USB11CON4 = (reg_data<<8)|     //write data
				(1<<7)|            //kick
				(0<<6)|            //write
				(addr&0x3f);			   //reg addr
	//debg("USB11CON0:%x\n",USB11CON0);
	//while((USB11CON0 & 0x02)==0x00){;}//wait done
}
/*******************************************************************************
* Function Name  : usb11_registers_read
* Description    : usb11_registers_read
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//usb1.1 SIE registers write
u8 usb11_registers_read(u8 addr)
{
	USB11CON4 = (1<<7)|            //kick
				(1<<6)|            //read
				(addr&0x3f);			   //reg addr
	//debg("USB11CON0:%x\n",USB11CON0);
	//while((USB11CON0 & 0x02)==0x00){;}  //wait done
	return USB11DATA;              //read data
}
/*******************************************************************************
* Function Name  : usb_memcpy
* Description    : usb_memcpy
* Input          : dir:1:gram->usb11_ram, 0: usb11_ram->gram;
* Output         : None
* Return         : None
*******************************************************************************/
void usb_memcpy(uint8_t dir,uint32_t dst,uint32_t src,uint32_t len)
{
	 USB11MCPCON |= BIT(0);
	 USB11MCPCON &= ~(1<<2);
	 USB11MCPCON |= (dir<<2);
	 USB11MCPSCADR = src;
	 USB11MCPTGADR = dst;
	 USB11MCPLENGTH = len -1; 
	 while(!(USB11MCPPND & BIT(0)));
// INT_STR (" wait\r\n");
}
/*******************************************************************************
* Function Name  : usb_phyreset_as_typea11
* Description    : usb_phyreset_as_typea11
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_phyreset_as_typea11(void)
{
#if 0
	//disable upll, uosc ,fix usb hot insert bug
	USB11CON0 = USB11CON1 =USB11CON2 = USB11CON3 = 0;

	
	XOSTimeDly(200); 
	debg("husb:<init as usb 1.0 typA...>\n");
	PCON1 &= ~BIT(0);
	//CLKCON1 |= BIT(26);
	//CLKCON2 |= BIT(27); //DDLCLK 
	CLKCON2 &= ~BIT(27); //SYSCLK 
	CLKCON2 &= ~(BIT(26)|BIT(25)|BIT(24));
    CLKCON2 |= (((PLL_CLK/48000000)-1)<<24);
//	CLKCON2 |= (1<<24); //96M/(1+1) = 48M
	USB11CON1 |= (1<<3);                      //RESET	
	while((USB11CON1 &BIT(3))==0); //wait for usb1.1 reset release
	
	/* {USBTM,DPPSEL,PHYEN,PHYSPDSEL,PHYSPD,RNW,RWDONE,RWKICK} */
	USB11CON0 = 0xf0;//0xF0; 	"IC 建议改成F0"
	//USB11CON2 [18:8]EPSIZE, [7:4]EP3TXVAL~EP0TXVAL
	USB11CON2 = 0x00;
	USB11CON1 &= ~(BIT(4)|BIT(6));	//15k pullup
	//USB11CON1 &= ~(BIT(7)|BIT(5));	//15k pulldown
	USB11CON1 |= BIT(7)|BIT(5)//15k pulldown
			  |BIT(2)|BIT(1)|BIT(0);		//VBUS
	USB11CON3 |= (1<<12); //sof mask
	USB11CON3 &= ~BIT(11);   //CID : 0 : host 1 : device	
	
	//usb11_registers_write(DevCtl,0x03); 
	
	// debg("USB11CON0:%x, USB11CON1:%x, USB11CON2:%x, USB11CON3:%x\n",USB11CON0,USB11CON1,USB11CON2,USB11CON3);
	//debg("DevCtl:%x\n",usb11_registers_read(DevCtl));
#endif
}
/*******************************************************************************
* Function Name  : usb11_host_reset
* Description    : usb11_host_reset
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb11_host_reset(void)
{
#if 0
	usb11_registers_write(IntrTx1E,BIT(0));//ep0 isr en;
	usb11_registers_write(IntrUSBE,BIT(4)|BIT(5));//connect	isr en//disconnect isr en
	usb11_registers_write(DevCtl,0x03); 
	//usb11_registers_write(IntrUSBE,usb11_registers_read(IntrUSBE)&(~BIT(2)));//connect	isr en//disconnect isr en
	//USB20_IntrUSBE &=~BIT(2);  //babble err int disable
	usb11_registers_write(FAddr,0);
	//debg("FAddr:%d\n",usb11_registers_read(FAddr));
	//debg("IntrTx1E:%x\n",usb11_registers_read(IntrTx1E));
	//debg("IntrTx1E:%x\n",usb11_registers_read(IntrUSBE));
	USB11CON0 |= BIT(5);   //usb phy power on
#endif
}

void ax32xx_usb11_hostIRQHanlder(void)
{
#if 0
	//debg("*");
	bool transta = api_backrec_status();
	u8 intrusb = usb11_registers_read(IntrUSB);//& read_usb_reg(IntrUSBE);
    u8 introul = usb11_registers_read(IntrRx1);
    u8 intrin1 = usb11_registers_read(IntrTx1);
  /*  u8 intrin1 = USB20_IntrInl;
	u8 intrinh = USB20_IntrInh;
	u8 introul = USB20_IntrOutl;
	u8 introuh = USB20_IntrOuth;*/
	//debg("[usbhost11]\n");
	//debg("[.%x,%x,%x]\n",intrusb,introul,intrin1);
	
    if (introul){ //USB epxout
		if(fp11_epout_isr_hal)
			(*fp11_epout_isr_hal)();
    }
	
	//insert
	if(intrusb & BIT(4)){
		USB11CON0 &=~BIT(6);
		debg("husb: <insert>\n");
		(*fp_host11_connect_hal)();
		return;
	}
	
	//remove
	if(intrusb & BIT(5)){
		debg("husb: <remove>\n");
		(*fp_host11_remove_hal)();
		
		return;
			
	}
	
	//USB bable err
    if (intrusb & BIT(2)){ 
		
		if(transta){ //epx tran state
			debg("ERR:-bable <reset usb dev>\n");
			(*fp_host11_remove_hal)();
		}else{ //ep0 enum state
			if(fp_mjpBcodec_linking) //usensor had acteched ok but 1st mjp decode is not ok
			{
				debg("ERR:-bable <reset usb dev>\n");
				(*fp_host11_remove_hal)();
			}else{
				debg("ERR:-bable <enum retry >\n");
				usb_stack_init();
				husb_kick_ep0isr();//husb_ep0isr();
			}
		}
		return;
    }
	
	if(intrin1 & BIT(0)){
		husb_ep0isr();	
	}	
#endif

}
s32 usb20_host_check(void)
{
	u32 tmp0, tmp1;
	#define _TEST_JK_MOD0_	0
	//debg("usb20_host_check\n");
	REG32(USB20CON0)=0;
	REG32(USB20CON0) |= (1<<17);//control by soft
	REG32(USB20CON0) |= ((1<<18)|(1<<19));//enable dp,dm 10K pdown
	REG32(USB20CON0) &= ~((1<<21)|(1<<20)); //disable dp,dm 120K pullup
	ax32xx_sysCpuMsDelay(60);
	tmp0 = (REG32(USB20CON0) >> 30) & 0x03 ;
	//debg("<<<J,K :%x\n",tmp0);
	
	REG32(USB20CON0) |= (1<<17);//control by soft
	REG32(USB20CON0) &= ~((1<<18)|(1<<19));//disable dp,dm 10K pdown
	REG32(USB20CON0) |= ((1<<21)|(1<<20)); //enable dp,dm 120K pullup
	ax32xx_sysCpuMsDelay(60);
	tmp1 = (REG32(USB20CON0) >> 30) & 0x03 ; 
	
	//debg(">>>J,K :%x\n",tmp1);
	if(_TEST_JK_MOD0_ != (tmp0 & tmp1))// && (usbIsrCnt)
	{
		debg("**********USB 2.0 HOST Detected********\n");
         return 1;
	}
	
	return 0;
	#undef _TEST_JK_MOD0_

}
extern INT32U XOSTimeGet(void);
s32 usb11HostCheckNoWait(void)
{
#if 0
	static u32 usbCheckStat=0;
	static u32 lastTime=0;
	static u32 tmp0=0, tmp1=0;
if(usbCheckStat==1)
	goto USB_CHECK_STATE1;
if(usbCheckStat==2)
	goto USB_CHECK_STATE2;

	#define _TEST_JK_MOD0_	2  //Dp=1 , Dm = 0, full speed
	#define _TEST_JK_MOD1_	1  //Dp=1 , Dm = 0, low speed
	USB11CON0=0;
	USB11CON1=0;
	USB11CON0 |= (1<<6);//control by soft
	USB11CON1 &= ~(BIT(4)|BIT(6));	//disable dp,dm 120K pullup
	USB11CON1 |= (BIT(7)|BIT(5));	//enable dp,dm 15k pulldown
	usbCheckStat=1;
	tmp0=0;
	tmp1=0;
	lastTime=XOSTimeGet();
USB_CHECK_STATE1:	
	if(XOSTimeGet()<lastTime)
	{
		lastTime=XOSTimeGet();
		return -1;
	}
	if(XOSTimeGet()-lastTime<60)
		return -1;
	//ax32xx_sysCpuMsDelay(60);
	tmp0 = (REG32(USB11CON0) >> 8) & 0x03 ;
	//debg("<<<J,K :%x\n",tmp0);
	
	USB11CON0 |= (1<<6);//control by soft
	USB11CON1 |= (BIT(4)|BIT(6));	//enable dp,dm 120K pullup
	USB11CON1 &= ~(BIT(7)|BIT(5));	//disable dp,dm 15k pulldown
	usbCheckStat=2;
	lastTime=XOSTimeGet();
USB_CHECK_STATE2:	
	if(XOSTimeGet()<lastTime)
	{
		lastTime=XOSTimeGet();
		return -1;
	}
	if(XOSTimeGet()-lastTime<60)
		return -1;
	//ax32xx_sysCpuMsDelay(60);
	tmp1 = (REG32(USB11CON0) >> 8) & 0x03 ; 
	usbCheckStat=0;
	//debg(">>>J,K :%x\n",tmp1);
	if((_TEST_JK_MOD0_ == (tmp0 & tmp1))/*||(_TEST_JK_MOD1_ == (tmp0 & tmp1))*/)// && (usbIsrCnt)
	{
		debg("**********USB 1.1 HOST Detected********\n");
         return 1;
	}
	return 0;
	#undef _TEST_JK_MOD0_ 
	#undef _TEST_JK_MOD1_
#else
	return 0;
#endif
}

s32 usb11_host_check(void)
{
#if 0
//	debg("usb11_host_check\n");
	u32 tmp0, tmp1;
	#define _TEST_JK_MOD0_	2  //Dp=1 , Dm = 0, full speed
	#define _TEST_JK_MOD1_	1  //Dp=1 , Dm = 0, low speed
	USB11CON0=0;
	USB11CON1=0;
	USB11CON0 |= (1<<6);//control by soft
	USB11CON1 &= ~(BIT(4)|BIT(6));	//disable dp,dm 120K pullup
	USB11CON1 |= (BIT(7)|BIT(5));	//enable dp,dm 15k pulldown
	
	ax32xx_sysCpuMsDelay(60);
	tmp0 = (REG32(USB11CON0) >> 8) & 0x03 ;
	//debg("<<<J,K :%x\n",tmp0);
	
	USB11CON0 |= (1<<6);//control by soft
	USB11CON1 |= (BIT(4)|BIT(6));	//enable dp,dm 120K pullup
	USB11CON1 &= ~(BIT(7)|BIT(5));	//disable dp,dm 15k pulldown
	ax32xx_sysCpuMsDelay(60);
	tmp1 = (REG32(USB11CON0) >> 8) & 0x03 ; 
	
	//debg(">>>J,K :%x\n",tmp1);
	if((_TEST_JK_MOD0_ == (tmp0 & tmp1))/*||(_TEST_JK_MOD1_ == (tmp0 & tmp1))*/)// && (usbIsrCnt)
	{
		debg("**********USB 1.1 HOST Detected********\n");
         return 1;
	}
	
	return 0;
	#undef _TEST_JK_MOD0_ 
	#undef _TEST_JK_MOD1_
#else
	return 0;
#endif
}
void usb20_phyreset_as_filter(void)
{
	USB20CON0 &= ~(BIT(22)|BIT(23)            //USB PLL SEL
			   |BIT(24)|BIT(25));            //USB  OSC
	// 使能USB PHY OSC, USBCON0[7]=1,等待1uS穩定
	USB20CON0 |= 0x80;	// OSC enable
	combo_delay (1);
	// 使能USB PHY PLL, USBCON0[6]=1,等待200uS穩定
	USB20CON0 |= 0x40;	// PLL enable
	combo_delay (1000);
	// 釋放USB SIE復位,USBCON0[0]=1
	// 釋放USB PHY復位,USBCON0[2]=1,等待300uS使ClkTun穩定 modi @20131216
	USB20CON0 |= 0x4;	//release phy reset
	combo_delay (100);
}
