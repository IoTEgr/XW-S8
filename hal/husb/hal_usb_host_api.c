
#include "../inc/hal.h"

u8 usb_type;
UDEV u_dev;

volatile u8 usb_host_type; 

/*
typedef struct
{
	u8 inquiry[40];
	u8 maxlun;
	u32 cap;
	u32 slen;
	u8 online;
	BOOL(*fp_udisk_actech)(void);
	BOOL(*fp_udisk_state)(void);
	BOOL(*fp_udisk_rd)(BYTE *pDataBuf, DWORD dwLBA, DWORD dwLBANum);
	BOOL(*fp_udisk_wr)(BYTE *pDataBuf, DWORD dwLBA, DWORD dwLBANum);
	
}DISKHAL;
0，初始化usb为typeA设备后，插入中断自动检测设备，并枚举,枚举成功后自动发送消息。
1.接受到__USBDEV_INSERT__消息
2.读取MSC结构体参数，例如容量，在线状态等，注册f：fp_udisk_actech，fp_udisk_state，fp_udisk_rd，fp_udisk_wr到文件系统
3.使用文件系统读写文件。

*/
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
void delay_5ms(u32 n)
{
	volatile u32 i=5000*n;
	while(i--)
	{
		//asm("nop");
	}
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
void usb_host_fun_reg(void)
{
	fp_usb_isr = ax32xx_usb_hostIRQHanlder;	
	fp_host_connect_hal = usb_host_enum;
	fp_host_remove_hal = usb_host_remove;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
void hal_husb_init_typeA(void)
{
	if(u_dev.pudisk || u_dev.puvc)
		return ;
	delay_5ms(500);
	u_dev.pudisk = NULL;
	u_dev.puvc= NULL;

	usb_host_fun_reg();
	usb_phyreset_as_typea();
	usb_host_reset();
	ax32xx_intEnable(IRQ_USB20,1);	
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
void usb_host_speed_connect(void)
{
	usb_type = HIGH_SPEED;
	
	if(usb_type == HIGH_SPEED)
		USB20_Power = 0x28;
	else
		USB20_Power = 0x08;

	delay_5ms(20);


	if(usb_type == HIGH_SPEED)
		USB20_Power = 0x20;
	else
		USB20_Power = 0x00;
	delay_5ms(20);

}

/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
extern bool host_enum_dev(UDEV *udev);
u8 usb_host_enum(void)
{
	
	deg("[ enum usb dev start ]\n");
	//delay_5ms(1000); //before enum delay
	u_dev.pudisk = NULL;
	u_dev.puvc = NULL;
	usb_stack_init();//stack init
	usb_host_speed_connect(); //usb1.1 or 2.0 connect
	//husb_ep0isr();//usb ep0 isr fist start
	husb_kick_ep0isr();
	return 0;
}

//unlink usensor, unlinck uvc dev has lincking, usb host unlinck
void uvc_host_dev_detech(void)
{
	u_dev.pudisk = NULL; 
	u_dev.puvc= NULL;		
}

	

bool usensor_online(void)
{
	if(u_dev.puvc)
		return true;
	else
		return false;
}


/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern void dispLayerPIPOnlyFront(void);
//extern int layout_astern_exit(void);
extern void reflush_usensor_check(void);
u8 usb_host_remove(void)
{
	ax32xx_intEnable(IRQ_USB20,0);
	usensor_detach();
	dispLayerPIPOnlyFront();
	//layout_astern_exit(); //osd off back line
	uvc_host_dev_detech();
	//USB20CON0 = 0;
	//hal_husb_init_typeA();
	reflush_usensor_check();
	usb_host_type = 0;
	USB20CON0 = 0;
	debg("usb 2.0 remove ok\n");
	
	return true;
}
/*******************************************************************************
* Function Name  : uart_Init
* Description    : initialize the uart
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb11_host_fun_reg(void)
{
	fp_usb11_isr = ax32xx_usb11_hostIRQHanlder;	
	fp_host11_connect_hal = usb11_host_enum;
	fp_host11_remove_hal = usb11_host_remove;
}
void hal_husb11_init_typeA(void)
{
	if(u_dev.pudisk || u_dev.puvc)
		return ;
	delay_5ms(500);
	u_dev.pudisk = NULL;
	u_dev.puvc= NULL;

	usb11_host_fun_reg();
	usb_phyreset_as_typea11();
	usb11_host_reset();
	
	//debg("DevCtl:%x\n",usb11_registers_read(DevCtl));
	debg("USB11CON0: %x, USB11CON1:%x, USB11CON2:%x,USB11CON3:%x \n",USB11CON0,USB11CON1,USB11CON2,USB11CON3);
	ax32xx_intEnable(IRQ_USB11,1);	
}
u8 usb11_host_enum(void)
{
	
	deg("[ enum usb1.1 dev start ]\n");
	//delay_5ms(2000); //before enum delay
	u_dev.pudisk = NULL;
	u_dev.puvc = NULL;
	//delay_5ms(1000); 
	usb_stack_init();//stack init
	usb11_registers_write(Power,0x08);
	//debg("Power:%x\n",usb11_registers_read(Power));
	delay_5ms(20);
	//power:0x58
	usb11_registers_write(Power,0x00);
	//debg("Power:%x\n",usb11_registers_read(Power));
	delay_5ms(20);
	//usb_host_speed_connect(); //usb1.1 or 2.0 connect
	//husb_ep0isr();//usb ep0 isr fist start
	husb_kick_ep0isr();
	return 0;
}
u8 usb11_host_remove(void)
{
	ax32xx_intEnable(IRQ_USB11,0);
	usensor_detach();
	dispLayerPIPOnlyFront();
//	layout_astern_exit(); //osd off back line
	//delay_5ms(800);
	//usensor_lgc_detech();
	//usensor_infs_detech();
	uvc_host_dev_detech();
	//USB20CON0 = 0;
	//hal_husb_init_typeA();
	reflush_usensor_check();
	usb_host_type = 0;
	USB11CON0 = USB11CON1 =USB11CON2 = USB11CON3 = 0;
	debg("usb 1.1 remove ok\n");
	
	return true;
}
u8 usb_host_detect(void)
{
#if 1
	s32 ret1,ret2;
	ret1 = usb20_host_check();
	ret2 = usb11_host_check();
	if(ret1>0)
	{
		usb_host_type = 2;
		return 2; //usb 2.0 host first
	}
	if(ret2 > 0)
	{
		usb_host_type = 1;
		return 1; // usb1.1 host
	}
	usb_host_type = 0;
	return 0;
#else
	usb_host_type = 2;
	return 2; //usb 2.0 host first
	
#endif
}
void usb_host_type_set(u8 type)
{
	usb_host_type = type;
}
