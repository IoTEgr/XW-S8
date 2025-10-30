/****************************************************************************
**
 **                         BUILDWIN AX32XX MCU LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                       AX32XX INTRRUPUT
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : ax32xx_int.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN AX32XX HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/ax32xx.h"

#define __except        SECTION(".vector.text")
#define __except_data   SECTION(".vector.data")

extern int _exception_vma;

//__except_data static char str0[] = {"[exception_init]\r\n"};
__except_data static char str1[] = {"[exception]\r\n"};
__except_data static char str2[] = {"[except reset]\r\n"};
__except_data static char str3[] = {"\xff\xff\xff\x00\xff\xff\xff"};
__except_data static char str4[] = {"[low power]\r\n"};
__except_data static char str5[] = {"[WDT RESET]\r\n"};


__except_data u32 chip_vision;
__except_data u32 sdr_tune_source;

#define USE_PRESAVE_TUNE				0 //是否保存第一次tune的值到irtc ram，以后读ram的值进行配置，加快开机
#define DLL_TUN							0
#define CLK_TUN 						1
//#define SDRAM_TUNE_SOURCE               CLK_TUN  //only METALFIX B1 can use DLL_TUN
#define SDRTUN_DEBG						1

#define CLKTUN_CONST                    (BIT(6)|BIT(13)|BIT(5)|BIT(12))
#define CLKTUN_OUTPUT_POS               7
#define CLKTUN_INPUT_POS                0



#define DLLTUN_CH1_POS                  25
#define DLLTUN_CH2_POS					21
#define DLLTUN_CONST					((0xf<<DLLTUN_CH1_POS)|(0xf<<DLLTUN_CH2_POS))

//#define RTC_RD_RTCRAM    IRTRAM_RD_CMD
//#define RTC_WR_RTCRAM    IRTRAM_WR_CMD
#define HAL_TUNEVALUE_ADDR  16           // RTC RAM ADDR
#define HAL_CRCCHECK_ADDR   20           // RTC RAM ADDR
////////////////////////////////////////////////////////////////////////////////

#define WTD_DIS()    WDTCON = 0x5500;

#define PSRAM_INT_ENABLE    1   	// 0: disable psram irq, 1: enalbe psram irq
#define exact_delay(r1,r2,t) asm(\
  "l.movhi   %0,  hi(%2)\n"\
  "l.ori     %0, %0, lo(%2)\n"\
  "l.addi    %1, r0, 0\n"\
  "0:l.addi  %1,%1,1\n"\
  "l.sfeq    %0,%1\n"\
  "l.bnf     0b\n"\
  :"=r"(r1),"=r"(r2):"i"(t)\
  );
  
  
  
#if (0==SDRAM_PSRAM_SEL)
__except static void exception_delay(u32 cnt)
{
	int i, j;
	exact_delay(i,j,cnt);
   // while(cnt--) asm("l.nop");
}
#else
__except void exception_delay(u32 cnt)
{
    while(cnt--) asm("l.nop");
}
#endif


__except void _rtcRamWrite(u8 addr,u8 *buffer,u8 len)
{
	int i;

	RTCCON |= (1<<8)|(1<<6);        //CE ENABLE 
	RTCDATA = IRTRAM_WR_CMD;
	while(RTCCON & (1<<3));
	//_rtcWriteByte(RTC_WR_RTCRAM); // command
	RTCDATA = addr;
	while(RTCCON & (1<<3));
	//_rtcWriteByte(addr); // addr
	for(i=0;i<len;i++)
	{
		RTCDATA = buffer[i];
		while(RTCCON & (1<<3));
		// _rtcWriteByte(buffer[i]);
	}
	   
    RTCCON &= ~(1<<8);       //CE DISABLE 	
}
__except void _rtcRamRead(u8 addr,u8 *buffer,u8 len)
{
	int i;

	RTCCON |= (1<<8)|(1<<6);        //CE ENABLE 
	RTCDATA = IRTRAM_RD_CMD;
	while(RTCCON & (1<<3));
	//_rtcWriteByte(IRTRAM_RD_CMD); // command
	RTCDATA = addr;
	while(RTCCON & (1<<3));
	//_rtcWriteByte(addr); // addr
	for(i=0;i<len;i++){
		RTCDATA = 0;
		while(RTCCON & (1<<3));
		buffer[i] = RTCDATA;
	   // buffer[i] = _rtcReadByte();
	}
    RTCCON &= ~(1<<8);       //CE DISABLE 
}
__except static u8 exception_irtc_read(u8 cmd)
{
    RTCCON |= BIT(8)|BIT(6);
    RTCDATA = cmd;
    while(RTCCON & BIT(3));
    RTCDATA = 0;
    while(RTCCON & BIT(3));
    RTCCON &= ~BIT(8);
    return RTCDATA & 0xff;
}

__except static void exception_irtc_write(u8 cmd,u8 data)
{
	RTCCON |= BIT(8)|BIT(6);
	RTCDATA = cmd;
	while(RTCCON & BIT(3));
	RTCDATA = data;
	while(RTCCON & BIT(3));
	RTCCON &= ~BIT(8);
}
__except void my_putchar(char c)
{
#if SDRTUN_DEBG
	if('\n'== c)
	{
		UART0DAT = 0x0D;
		while((UART0PND & 0x2)==0);
		UART0PND |= BIT(1);  
		UART0DAT = 0x0A;
		while((UART0PND & 0x2)==0);
		UART0PND |= BIT(1);  
	}
	else{
		UART0DAT = c&0xff;
		while((UART0PND & 0x2)==0);
		 UART0PND |= BIT(1);  
		
	}
#endif
}

#define exception_putchar  my_putchar

__except void exception_uart_puts(const char * str)
{
#if CFG_MCU_DBG_EN
    while(*str)
    {
        UART0DAT = *str++;
        while((UART0PND & 0x2)==0);
        UART0PND = (1<<5)|(1<<4);
    }
#endif
}
__except void exception_putword (u32 w) 
{
#if CFG_MCU_DBG_EN
	__except_data static u8 hex[16] = "0123456789ABCDEF";
	int i;
	u8 out=0;
	exception_putchar('0');
	exception_putchar('x');
	for (i=8; i; --i) 
	{
		if(out||((w>>28)&0xF)||(i==1))
		{
			out = 1;
			exception_putchar(hex[(w>>28)&0xF]);	
		}
		w <<= 4;
	}
#endif
}

__except  void my_memset(void *dst, u8 val, int cnt)
{
	u8 *pDstTemp = (u8 *)dst;
	while (cnt--)
	{
		*pDstTemp++ = val;
	}
}
#if 0
__except void auto_flash_init(u32 u32SPIBaud,u8 bus_mode)
{
	u8 dummy =0;
	u8 dummy_en = 0;
    //u32 spif_clk = spi0_clk_config();
    //spi0_pin_config(bus_mode);
    *(u32 *)0x1000  = 0xaabbccd1;
//    SPIFGCON  = 0;
//	SPIFFACON = 0;
    SPIFGCON  = (1<<24)|//cs min high time
                (1<<20)|//cs min hold time
                (1<<16)|//cs min setup time
                (0<<15)|//spi_csp mode select
                (1<<14)|//spi_csf mode select
                (0<<13)|//spi_csp en(for psram)
                (1<<12)|//spi_csf en(for flash)
                (0<<10)|//cs special mode en(auto mode)
                ((bus_mode & 3)<<8)| //data bus width(manual mode)
                (0<<7)| //transfer dir(manual mode)
                (0<<5)| //LFSR enc/dec en
                (0<<4)| //crc16 enable
                (0<<3)| //sample edge,0:raise,1:fall
                (1<<2)| //spi_clk idle polarity
                (1<<1)| //auto mode en ---------------
                (1<<0); //spif en
                if(bus_mode==1)
					dummy =1,dummy_en = 1;
				else if (bus_mode==3)
					dummy =3,dummy_en = 1;
				else
					dummy = 0,dummy_en = 0;
    SPIFFACON = (dummy<<20)|//dummy bytes count(auto mode)--------
                (2<<18)|//addr bytes count(auto mode)--------
                (0<<14)|//mode bus width(auto mode)	√
                (0<<12)|//cmd bus width(auto mode)	√
                (0<<10)|//addr bus width(auto mode) √
                ((bus_mode & 3)<<8)| //data bus width(auto mode) √
                (dummy_en<<6)| //dummy phase enable(auto mode)---------
                (0<<5)| //mode phase enable(auto mode)------
                (1<<4)| //cmd phase enable(auto mode)------
                (0<<2)| //dma device en			DMA模式
                (1<<1)| //cpu data en			数据模式
                (1<<0); //cpu instruction en  	指令模式
  
    SPIFFAMSK = 0x01fffff;//address mask(flash,auto mode)掩码--------
    SPIFPAMSK = 0xfffffff;//address mask(psram,auto mode)

	 *(u32 *)0x1000  = 0xaabbccd2;
	if(bus_mode == 1)
	{
		SPIFFACMD &= ~(0xfff);
		SPIFFACMD = (0<<16)|//mode value(flash,auto mode)---------
					(0x02<<8)| //write cmd(flash,auto mode)----------
					(0x3b<<0); //read cmd(flash,auto mode)----------
	}
	else if(bus_mode == 3)
	{
		SPIFFACMD &= ~(0xfff); 
		SPIFFACMD = (0<<16)|//mode value(flash,auto mode)---------
					(0x02<<8)| //write cmd(flash,auto mode)----------
					(0x6b<<0); //read cmd(flash,auto mode)----------
	}
	else
	{
		SPIFFACMD &= ~(0xfff);
		SPIFFACMD = (0<<16)|//mode value(flash,auto mode)---------
					(0x02<<8)| //write cmd(flash,auto mode)----------
					(0x03<<0); //read cmd(flash,auto mode)----------
	}

//    SPIFPACMD = (0<<16)|//mode value(psram,auto mode)
//                (0<<8)| //write cmd(psram,auto mode)
//                (0<<0); //read cmd(psram,auto mode)
//                
//    SPIFDCMD  = (0<<16)|//mode value(dma device,auto mode)
//                (0<<8)| //write cmd(dma device,auto mode)
//                (0<<0); //read cmd(dma device,auto mode)
}


__except void memcpy_flash2psram(void *dst, void *src, int cnt)
{
	MCP0CON = BIT(3);   
    MCP0CON = BIT(0); 	 
	
	MCP0SCADDR = (u32)src;
	MCP0TGADDR = (u32)dst;
	MCP0LENGTH = cnt-1;
    
	while(1){
		if(MCP0CON & BIT(1)){
			MCP0CON |= BIT(3);//clear dma_copy pengding
			break;
		}	
	}
}

typedef void (*func) (uint32_t addr, uint32_t secn, uint32_t secs, uint32_t cmd); 
__except void flash_to_psram(void)
{
#if 1

    PMAPCON1 |= (3<<20);
    PAFMAP   |= /*(1<<11)|(1<<10)|(1<<8)|(1<<9)|*/(1<<6)|(1<<7);
    //PADIR    &=~((1<<11)|(1<<10));       
    PADIR    |= /*(1<<9)|(1<<8)|*/(1<<6)|(1<<7);
    PAPU     |= /*(1<<9)|(1<<8)|*/(1<<6)|(1<<7);
    
    PEFMAP |= (1<<0);//psram_cs 
    PEDIR  &= ~(1<<0);

    func p1= 0x001012d0;
    func p2= 0x00101a50;
    uint32_t i;
    for(i=0;i<0x345;i++){
        WDTCON = 0x500;
        p1(0x4000,7+i,1,0);
        WDTCON = 0x500;
        p2(0x4000,(0x4000000>>9)+i,1,0);
        WDTCON = 0x500;
    }
#endif
#if 0
    uint8_t bus_mode = 2;
    SPIFGCON  |= (1<<1); //auto mode en ---------------
    SPIFFACON = (0<<20)|//dummy bytes count(auto mode)--------
                (2<<18)|//addr bytes count(auto mode)--------
                (0<<14)|//mode bus width(auto mode)	√
                (0<<12)|//cmd bus width(auto mode)	√
                (0<<10)|//addr bus width(auto mode) √
                ((bus_mode & 3)<<8)| //data bus width(auto mode) √
                (0<<6)| //dummy phase enable(auto mode)---------
                (0<<5)| //mode phase enable(auto mode)------
                (1<<4)| //cmd phase enable(auto mode)------
                (1<<2)| //dma device en			DMA模式
                (0<<1)| //cpu data en			数据模式
                (0<<0); //cpu instruction en  	指令模式
    SPIFFAMSK = 0x01fffff;//address mask(flash,auto mode)掩码--------
    SPIFFACMD = (0<<16)|//mode value(flash,auto mode)---------
                (0x02<<8)| //write cmd(flash,auto mode)----------
                (0x03<<0); //read cmd(flash,auto mode)----------
                
    SPIFPACON = (0<<20)|//dummy bytes count(auto mode)--------
                (2<<18)|//addr bytes count(auto mode)--------
                (0<<14)|//mode bus width(auto mode)	√
                (0<<12)|//cmd bus width(auto mode)	√
                (0<<10)|//addr bus width(auto mode) √
                ((bus_mode & 3)<<8)| //data bus width(auto mode) √
                (0<<6)| //dummy phase enable(auto mode)---------
                (0<<5)| //mode phase enable(auto mode)------
                (1<<4); //cmd phase enable(auto mode)------

                
    SPIFPAMSK = 0x01fffff;//address mask(flash,auto mode)掩码--------
    SPIFPACMD = (0<<16)|//mode value(flash,auto mode)---------
                (0x02<<8)| //write cmd(flash,auto mode)----------
                (0x03<<0); //read cmd(flash,auto mode)----------
                
    memcpy_flash2psram(0x4000000, 0x6000000+(7<<9), (0x345<<9));
#endif    
}
#endif
typedef void (*func1) (uint32_t addr, uint8 *s1, uint8_t s2); 
__except_data static u32 vbg_param;
__except void exception_init(void)
{
//    UART0DAT = 'e';
//    while((UART0PND & 0x2)==0);
//    UART0PND |= BIT(1);  
    
	__intvct_adr__((u32)&_exception_vma);

    //LVD电压1.8V
    LVDCON = (1<<6)|(1<<5)|(0<<0);
//    WDTCON = 0x500;
//-----------------PMU-------------
//sys:144M,pmu_ADC_baud:1M~2M
//open battery detech:IRTCON0&BIT(5)
//---------------------------------
#if 0
    u8 rtc0 = 0;
    rtc0 = exception_irtc_read(IRTCON0_RD_CMD);
    if((rtc0 & (1<<5)) == 0)
        exception_irtc_write(IRTCON0_WR_CMD,rtc0|(1<<5));
//	exception_delay(10);	
	PMUREFVOL = vbg_param;//*(u32 *)(0x5000-140+0x30 + 4) & 0x3ff;//0x32a;//efuse读值？
	PMUCON  = 71<<2;//(((APB_CLK/(2*baudrate) - 1) & 0x7F)<<2);	//2M,根据系统时钟修改。
	PMUFSBAUD = 24*2*(((71) & 0x7F)+1);//fpga: baudrate*1/32;芯片：baudrate*1/24;
	PMUCON1 =(2<<5)/*|(2<<3)*/|(5<<0);	
	//PMUCON1 =(5<<0);	
	PMUBATHW  = 0x1388;//4.6v
	PMUBATLW  = 0x0d48;//3.0//3.4
	PMUBATPROT= 0x0dac;//0x0e10;//3.5
	PMUCON  |= (1<<1)|(1<<0);
#endif
//    flash_to_psram();
//    SPIFGCON  |= (1<<1); //auto mode en ---------------
//    SPIFFACON = (0<<20)|//dummy bytes count(auto mode)--------
//                (2<<18)|//addr bytes count(auto mode)--------
//                (0<<14)|//mode bus width(auto mode)	√
//                (0<<12)|//cmd bus width(auto mode)	√
//                (0<<10)|//addr bus width(auto mode) √
//                ((0 & 3)<<8)| //data bus width(auto mode) √
//                (0<<6)| //dummy phase enable(auto mode)---------
//                (0<<5)| //mode phase enable(auto mode)------
//                (1<<4)| //cmd phase enable(auto mode)------
//                (0<<2)| //dma device en			DMA模式
//                (1<<1)| //cpu data en			数据模式
//                (1<<0); //cpu instruction en  	指令模式
                
//    SPIFFAMSK = 0x01fffff;//address mask(flash,auto mode)掩码--------
//    SPIFFACMD = (0<<16)|//mode value(flash,auto mode)---------
//                (0x02<<8)| //write cmd(flash,auto mode)----------
//                (0x03<<0); //read cmd(flash,auto mode)----------

#if 0
    func1 p1= 0x001019b0;
    uint8 cmd[1];
    cmd[0]= 0x35;
    p1(0x105,cmd,NULL);

    SPIFPACON = ((3-1)<<20)|//dummy bytes count(auto mode)
                ((3-1)<<18)|//addr bytes count(auto mode)
                (0<<14)|//mode bus width(auto mode)
                (3<<12)|//cmd bus width(auto mode)
                (3<<10)|//addr bus width(auto mode)
                (3<<8)| //data bus width(auto mode)
                (1<<6)| //dummy phase enable(auto mode)
                (0<<5)| //mode phase enable(auto mode)
                (1<<4); //cmd phase enable(auto mode)

//    SPIFPACMD = (0<<16)|//mode value(psram,auto mode)
//                (0<<8)| //write cmd(psram,auto mode)
//                (0<<0); //read cmd(psram,auto mode)

    SPIFGCON  |= (1<<1);//auto mode en --------------- 
    SPIFFACON |= (1<<1)| //cpu data en
                 (1<<0); //cpu instruction en
#endif             

 
    //uart
#if AX32_PLATFORM == FPGA
    PMAPCON0 = (PMAPCON0 & ~0x3ff)|(9<<5)|(6<<0);
    PBFMAP   |= BIT(4)|BIT(0);
    PBDIR    |= BIT(4);
    PBDIR    &= ~BIT(0);
#elif AX32_PLATFORM == AX3292  //uart tx pe1
    PMAPCON0 = (PMAPCON0 & ~0x3ff)|(24<<5);
    PEFMAP   |= BIT(1);
    PEDIR    &= ~BIT(1);
#else   //uart tx pa7
    PMAPCON0 = (PMAPCON0 & ~0x3ff)|(6<<5);
    PAFMAP   |= BIT(7);
    PADIR    &= ~BIT(7);
#endif
    UART0BAUD = APB_CLK / UART_BAUDRATE -1;   //baud rate
    UART0PND |= BIT(5)|BIT(4);			  //清除标志位
    UART0CON |= BIT(4);					  //使能UART0
}

__except u32 ax32xx_vddrtcCalculate(u32 Artc,u32 Abg)
{
    u32 Vrtc = 0;
    /*if (0)//(batchk_param)
    {
        Vrtc = Artc * batchk_param * 1000 / Abg / 1024 * 2;
    }
    else*/
    {
        Vrtc = (Artc * vbg_param) / Abg * 2;//vbg_param
	  //Vrtc = (Artc * 800) / (vbg_param) * 2;
    }
    
    return Vrtc;
}


//#define combo_delay(t)  do {volatile int i=(t)*10; while(i--)  WTD_DIS(); } while(0) //1us  96M
//#define delay(n)    combo_delay(n)
////////////////////////////////////////////////////////////////////////////////



#if  (0==SDRAM_PSRAM_SEL)
__except void dllcon_init (void)
{
	
	//int i,j;
	if(chip_vision == THUNDERSE_A1 )  
	{
		LDOCON |= BIT(28);
		//DLLCON |= (BIT(10) | BIT(9)) ;    // LDOS sel ldo vol 11
		DLLCON |= BIT(11);            // enable ldoen
		exception_delay(2400);          //(6*2400+5)*6.94ns(144M)=100us
		DLLCON |= BIT(2); //SEL VREF=VBG
		DLLCON |= BIT(1); //vcdlref=vtv
		
		DLLCON |= BIT(19);  //enable DLLEN 
		DLLCON |= (BIT(15) | BIT(14) | BIT(13) | BIT(5) );  //dll_freqs_12v 200M,111,  lowspeed
		DLLCON |= BIT(12);  //ICPSEL 1:dcbias 0: selfbias,sel  dcbias
		
		
		//DLLCON |= BIT(17);  //DLL_FORCEALL
		DLLCON |= BIT(16);  //DLL_Forceout 
		DLLCON |= BIT(4);  //DLL START
		exception_delay(2400);  //(6*2400+5)*6.94ns(144M)=100us

		//exact_delay(i,j,240000);          //(6*2400+5)*6.94ns(144M)=10ms
		//while((DLLCON & BIT(0))==0);
		// DLLCON = 0;
	}
	else if(chip_vision ==THUNDERSE_B1)
	{
		LDOCON |= BIT(28);  //enable VREF_EN
		DLLCON |= BIT(2); //REFSEL 0: iref 1:vref
		// delay(3) ;  //delay 100ns (500ns)
		//combo_delay(1);  
		
		//DLLCON |= BIT(1); //VTBIAS 0:V TO IV BIAS 1: V TO V BIAS
		
		 //DLLCON |= BIT(10); // LDOS
		 //DLLCON |= BIT(9);  // ldos sel ldo vol
		DLLCON |= BIT(11);   // enable ldoen
		
		exception_delay(47); //delay 100us (161us)(107US)
		//  combo_delay(100); 
		
		DLLCON |= BIT(19);  //enable DLLEN 
		exception_delay(10);
		
		DLLCON |= BIT(15);  //dll_freqs_12v 200M,110
		DLLCON |= BIT(14);  //dll_freqs_12v 200M,110
		//DLLCON |= BIT(13);  //dll_freqs_12v 200M,110
		DLLCON |= BIT(5);  //LOWSPEED
		
		DLLCON |= BIT(12);  //ICPSEL 1:dcbias 0: selfbias
		
		DLLCON &=~(BIT(7) | BIT(6)); //loops00:16phase;01:12phase;10:10phase;11:8phase
		//DLLCON |= BIT(6);
		//DLLCON |= BIT(7);
		
		DLLCON |= BIT(17); //DLL_FORCEALL
		DLLCON |= BIT(16); //DLL_Forceout 
		
		//DLLCON |= SDRITUN | SCOTUN;
		
		//DLLCON |= BIT(3);  //vpullsel 0:pull to 0.8 1:pull to 1.2
		
		//DLLCON |= BIT(1); //VTBIAS 0:V TO IV BIAS 1: V TO V BIAS
		
		exception_delay(47);
		DLLCON |= BIT(4);  //DLL START
		
		exception_delay(10);   //delay 100ns
		DLLCON |= BIT(18);  //flocken
		exception_delay(2400) ; //delay 100us->200us 107us  
		DLLCON &= ~BIT(18); //flocken DISABLE
		
		//delay(100000);
		DLLCON |= BIT(8); //dll_lockdet
		exception_delay(2400) ;
		while((DLLCON & BIT(0))==0);
		my_putchar('2');
	}
else
	return;
	
}

#define SDRAM_CMD_READ_WRITE 0x84
#define SDRAM_CMD_PRE_ALL    6
#define SDRAM_CMD_CBR        3
#define SDRAM_CMD_MRS        2
#define SDRAM_CMD_NOP        1
#define issue_cmd(cmd)   SDRHCMD = cmd|BIT(4)
#define wait_SDRC_OK() while((SDRHCMD&(1<<6)) == 0)

__except uint32_t sdram_init (void)
{

   int i;
#if (SDRAM_SIZE_2M==SDRAM_SIZE)
	   SDRCON = 0x180|(1<<5);//param[0];
#else
	   SDRCON = 0x181|(1<<5);//param[0];
#endif

    SDRTM  = 0x2297fbf5;//param[1];

    asm("l.nop");
    asm("l.nop");
    asm("l.nop");
    
    SDRCON |= 0x80;
    exception_delay(200);
    //combo_delay(200);  //minimum 200us, should be 400us

    issue_cmd (SDRAM_CMD_NOP);
    wait_SDRC_OK ();

    issue_cmd (SDRAM_CMD_PRE_ALL);
    wait_SDRC_OK ();

    for (i=8; i; --i) {
        issue_cmd (SDRAM_CMD_CBR);
        wait_SDRC_OK ();
    }

    SDRMRS = 0x32;//param[2];   //CAS latency should be parameter
    issue_cmd (SDRAM_CMD_MRS);
    wait_SDRC_OK ();

  return 16;
}
#define  R_ADC_CTRL            ADCCON
#define  R_ADC_BAUDRATE      ADCBAUD
#define  R_ADC_DATA            ADCDAT
#define  R_SYS_CLK      PCON0

#define  R_SYS_CLK1     PCON1

#define  HAL_CFG_ADC_BAUDRATE       1000000UL
/*
__except u16 adcRead(void)
{
	volatile int i=0x8ffff;
	u16 value;
//	if(R_ADC_CTRL&(ADC_EN))
	 while(R_ADC_CTRL&ADC_EOC)
	 {
	 	i--;
		if(i==0)
		{

			break;
		}
	 }
	value = R_ADC_DATA&0x3ff;

	return value;
}
__except void adcConverStart(void)
{
	R_ADC_CTRL |= ADC_GO;	 // start one change
}
__except void adcSetBaudrate(u32 baudrate)
{
	R_ADC_BAUDRATE = (APB_CLK/(2*baudrate)- 1) & 0x7F;	
}
*/
__except void sysClkSet(u8 clk_type,u8 en)
{
    u32 CLKSFR = (u32)&R_SYS_CLK;
    u8 dev_id = clk_type;
    
    if(clk_type > 31)
    {
        CLKSFR = (u32)&R_SYS_CLK1;
        dev_id = clk_type - 32;
    }
	if(en)
		SFR_AT CLKSFR &=~(1<<dev_id);
	else
		SFR_AT CLKSFR |= (1<<dev_id);
}
/*
__except void adcEnable(u8 ch,u8 en)
{
	en&=1;
	if(ch>ADC_CH_BGOP)
		return ;
	if(en)
	{
		sysClkSet(SYS_CLK_ADC,1);
		R_ADC_CTRL  = (ADC_EN)|(ch);

	}
	else
		R_ADC_CTRL  &= ~(ADC_EN);
	
}

__except s32 adcGetChannel(u8 ch)
{
	adcEnable(ch,1);
	adcSetBaudrate(HAL_CFG_ADC_BAUDRATE);
	adcConverStart();
	return adcRead();
}
*/

/* 嘗試非常簡易的檢驗方法 */
////////////////////////////////////////////////////////////////////////////////
/* 嘗試非常簡易的檢驗方法 */
#define ADR(a) (((a) & adrmsk) + 0x02000000)
#define ADDR0 0x55555554
#define DATA0 0x33CCCC33
#define ADDR1 0xAAAAAAA8
#define DATA1 0xAA5555AA
#define ADDR2 0xCCCCCCCC
#define DATA2 0x99666699
#define WORD_WRITE(p,i) *(volatile uint32_t *) ADR(ADDR ## p) = DATA ## p
#define HALF_WRITE(p,i) *(volatile uint16_t *) (ADR(ADDR ## p) + i*2) = DATA ## p >> (i*16)
#define BYTE_WRITE(p,i) *(volatile uint8_t  *) (ADR(ADDR ## p) + i) = DATA ## p >> (i*8)
#define WORD_READ(p,i)  (*(volatile uint32_t *) ADR(ADDR ## p))
#define HALF_READ(p,i) ((*(volatile uint16_t *) (ADR(ADDR ## p) + i*2)) << (i*16))
#define BYTE_READ(p,i) ((*(volatile uint8_t  *) (ADR(ADDR ## p) + i)) << (i*8))
__except  bool sdram_tuning_check_2(uint8_t sdrsize_shf)
{
  uint32_t adrmsk = (1<<sdrsize_shf) - 1;
  uint32_t data0,data2;

  BYTE_WRITE (0,0);
  HALF_WRITE (1,0);
  BYTE_WRITE (0,1);
  WORD_WRITE (2,0);
  BYTE_WRITE (0,2);
  HALF_WRITE (1,1);
  BYTE_WRITE (0,3);

  data2  = BYTE_READ (2,0);
  data0  = HALF_READ (0,0);
  data2 += BYTE_READ (2,1);
  if      (WORD_READ (1,0) != DATA1) return false;
  data2 += BYTE_READ (2,2);
  if (     HALF_READ (0,1) + data0 != DATA0) return false;
  if (     BYTE_READ (2,3) + data2 != DATA2) return false;

 
  
  return true;
}
  

#define b_bic_orr(r,c,s)        r = ((r) &~(c))|(s)     
__except_data u32 SDRTUN2_CON;
#define UNUSED					__attribute__((unused))
__except_data  u32 tuning_test_data[] = {
    //0x392B944D, 0x4B16C719, 0xD9F72F51, 0xB35C8D7C, 0x0FDF8A0B, 0x5004BA88, 0x60CF8AEE, 0x50FE04B9, 
    //0x36AC5174, 0xFE72698C, 0x6191F295, 0x4FCFAB39, 0xE962B343, 0x355F4C50, 0x26015E00, 0x105B8A35, 
    //0xD3D9209F, 0xBAB9DDC5, 0x9B19F3E9, 0x2A1B0AA1, 0xDDF26143, 0x12D2E044, 0x54B4A12E, 0x08B629A0, 
    //0x91D472A0, 0x79ED30BA, 0x2D73FF91, 0x65DAA782, 0x1F79DE34, 0x412243A8, 0x6115F79F, 0x2709CB25, 
    0xFDCFB6F6, 0x82DC7E0C, 0x64711906, 0x94466C03, 0x02FF4741, 0xA28D8DF9, 0xB8311948, 0x5D73D025, 
    0x9BE168E0, 0xD85EA796, 0xFD90593D, 0xF12C429F, 0x3EF4FC5F, 0xF0C616BB, 0x2AE37380, 0xA8886B3A, 
    0xAD4736F6, 0xC517CF51, 0x618E6B88, 0x40D5DA4C, 0x3012C3F1, 0x0CF1FB8D, 0x465AC24D, 0x55EC889F, 
    0x611D1DBB, 0x876BEF10, 0xF54E57AE, 0x6F5E319F, 0x9A415B44, 0x847B25A1, 0xD21EB67C, 0xB9AF496C,
};
__except_data  u32 tuning_test_addr[] = {
   // 0x026ACC74, 0x023B26C4, 0x027C5B84, 0x023E2C1C, 0x023B5654, 0x027F537C, 0x02508F9C, 0x02232F44, 
   // 0x0275D2D0, 0x027D97FC, 0x02656A68, 0x02591464, 0x02684250, 0x0268B960, 0x027A7E2C, 0x020023E8, 
    //0x0275B4FC, 0x026245A8, 0x0232F294, 0x026648EC, 0x0235C43C, 0x0205224C, 0x02229B4C, 0x02565FE8, 
    //0x022D5F84, 0x02106AD8, 0x024045D8, 0x02010780, 0x02519074, 0x0205B72C, 0x0266D718, 0x025683E0, 
    0x0264BC04, 0x0277B338, 0x02784EE8, 0x02273F3C, 0x0210483C, 0x027741AC, 0x020A2AA4, 0x026C2228, 
    0x027261C0, 0x023CC00C, 0x02233EBC, 0x021F6194, 0x02673A34, 0x02232C68, 0x025D1644, 0x023FC228, 
    0x02287868, 0x02601B70, 0x0244062C, 0x021978AC, 0x02468EB0, 0x025BF570, 0x02694D20, 0x02260C74, 
    0x026A0190, 0x02779854, 0x02585E78, 0x02140590, 0x027C8088, 0x023CE83C, 0x020E64C0, 0x021EC9DC, 
};

#define ARRAY_NUM(array)		(sizeof(array)/sizeof(array[0]))
typedef struct tune_s {
    struct {u8 index,cnt;} value[5];//仅保存5组值,依据经验设置,虽然有局限性
    u8 remain; //value[]中剩余可用的元素数量
    u8 cur;    //当前可用的value[]的index
} tune_t;
typedef struct slope_s {
    u8 start;
    u8 cnt; 
    s8 slope; 
	u8 max_con;
} slope_t;
//#if SDRAM_TUNE_SOURCE == CLK_TUN
__except_data u8 tune_tab_1_clk[] =  //out x  b11_7_tab

{ // BIT(12) is set
  // backwards
//  0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
//  0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
   0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
  0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
  // backwards
  0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
  0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
//  0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
//  0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
};
__except_data u8 tune_tab_2_clk[]  =   //IN Y  b4_0_tab

{ // BIT(5) is set
  // backwards
//  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
//  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
  0x2F, 0x2E, 0x2D, 0x2C, 0x2B, 0x2A, 0x29, 0x28,
  0x27, 0x26, 0x25, 0x24, 0x23, 0x22, 0x21, 0x20,
  // backwards
  0x3F, 0x3E, 0x3D, 0x3C, 0x3B, 0x3A, 0x39, 0x38,
  0x37, 0x36, 0x35, 0x34, 0x33, 0x32, 0x31, 0x30,
//  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
//  0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
};

//#else
__except_data u8 tune_tab_1_dll[] =  //16 phase ：22.5/（0~360）

{ 
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};
__except_data u8 tune_tab_2_dll[]  =   //16 phase 22.5/（0~360）
{ 
  
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

//#endif

__except_data tune_t tune_by;//仅包含b11_7的tune信息 tune_b11_7
__except_data tune_t tune_values[ARRAY_NUM(tune_tab_1_clk)];//包含所有的信息



__except void _sdr_tuning2_conifg_set(u32 sdrconfig)
{
    SDRTUN2_CON = sdrconfig;
}

/*__except static bool sdram_tuning_check_byte(u8 * dst)
{
    u32 i = 0; 
  //  _dcache_enable(false);
    for(i = 0;i < 0x100;i++)
    {
        dst[i] = (i&0xff);
    }
    
   // delay(1);
   // _dcache_enable(true);
    for(i = 0;i < 0x100;i++)
    {
        if(dst[i] != (i&0xff))
        {
            return false;
        }
    }
	// _dcache_enable(false);
    return true;
}*/

__except static bool sdram_tuning_check_word(u32 * dst)
{
    u32 i = 0;
    
  //  _dcache_enable(false);
    for(i = 0;i < ARRAY_NUM(tuning_test_data);i++)
    {
        dst[i] = tuning_test_data[i];
    }
    
    //delay(1);
  //  _dcache_enable(true);
    for(i = 0;i < ARRAY_NUM(tuning_test_data);i++)
    {
        if(dst[i] != tuning_test_data[i])
        {
            return false;
        }
    }
	for(i = 0;i < ARRAY_NUM(tuning_test_data);i++)
    {
        dst[i] = tuning_test_data[i];
    }
    
    //delay(1);
   // _dcache_enable(true);
    for(i = 0;i < ARRAY_NUM(tuning_test_data);i++)
    {
        if(dst[i] != tuning_test_data[i])
        {
            return false;
        }
    }
	
    
    return true;
}
__except static void tune_values_init(void)
{
    u32 i;
    for(i = 0;i<ARRAY_NUM(tune_values);i++)
    {
        tune_values[i].remain = ARRAY_NUM(tune_values[0].value);
        tune_values[i].cur = 0;
        my_memset(tune_values[i].value,0,sizeof(tune_values[0].value));
    }

    tune_by.remain = ARRAY_NUM(tune_by.value);
    tune_by.cur = 0;
    my_memset(tune_by.value,0,sizeof(tune_by.value));
}

__except static bool sdram_tuning_check_1(void)
{
    //__except_data static u32 byte_num = 0;
   // byte_num++;
    //byte test
	 
   /* if(!sdram_tuning_check_byte((u8 *)(tuning_test_addr[byte_num % ARRAY_NUM(tuning_test_addr)])
                              ))
    {
        return false;
    }*/
    
    //word test
    u32 i = 0;
    for(i = 0;i < ARRAY_NUM(tuning_test_addr);i++)
    {
        if(!sdram_tuning_check_word((u32 *)(tuning_test_addr[i])))
            return false;
    }
	
	/*if(!sdram_tuning_check_byte((u8 *)(tuning_test_addr[byte_num % ARRAY_NUM(tuning_test_addr)])
                              ))
    {
        return false;
    }*/
	
    return true;
}
__except static void sdram_tuning_check_init(void)
{
    if(!(SDRACON1 & (1<<29)))
    {
		
        u32 i;
        u32 temp;
        for(i = 0;i<ARRAY_NUM(tuning_test_addr);i++)
        {
            temp = tuning_test_addr[i] & 0xFF1FFFFF; //确保在2M范围内
            if((temp & 0xFFFFF) > (0x100000 - 10 * 1024))
                temp -= 10 * 1024; //确保不能太靠近边界
			tuning_test_addr[i] = temp;
            
            //if(temp > (0x2000000 + 2 * 1024 * 1024 - 10 * 1024))
             
        }
    }
}
__except static u32 tab_next_index(u32 tab_size,u32 start,s32 interval)
{
    s32 t = ((s32)start + interval) % tab_size;
    if(t < 0)
        return tab_size + t;
    else
        return t;
}

__except static u8 tab_next_value(const u8 *tab,u32 tab_size,u32 start,s32 interval)
{
    return tab[tab_next_index(tab_size,start,interval)];
}

//搜索tune到的值中cnt最大的
__except static void tune_values_max(const u8 *tab,u32 tab_size,tune_t *tune,u8 *index,u8 *cnt)
{
    if(!tune->cur)
        return;
        
    u32 i;
    u8  max_index = 0,max_cnt = 0;

#define _tab_next(start,interval)  tab_next_value(tab,tab_size,start,interval)
    if(tune->cur > 1) //至少要有两段
    {
        u8  f_slice_index = tune->value[0].index; //first slice index
        u8  f_slice_cnt   = tune->value[0].cnt;
        u8  l_slice_index = tune->value[tune->cur - 1].index;
        u8  l_slice_cnt   = tune->value[tune->cur - 1].cnt;
        u8  next = _tab_next(l_slice_index,l_slice_cnt);
        
        if(next == tab[f_slice_index])//检查首尾是否是连在一起的
        {
            max_index = l_slice_index;
            max_cnt   = l_slice_cnt + f_slice_cnt;
        }
    }
#undef _tab_next

    for(i = 0;i<tune->cur;i++)
    {
        if(tune->value[i].cnt > max_cnt)
        {
            max_cnt   = tune->value[i].cnt;
            max_index = tune->value[i].index;//在tab中的index
        }
    }
    
    if(index) *index = max_index;
    if(cnt) *cnt = max_cnt;
}

__except static int tune_values_select(u8 *p_by_id,u8 *p_bx_id)
{
   // __except_data static u32 re = 2;
    
    if(!tune_by.cur)
    {
       
		
        return -1; //报错,让外部重新再tuning一次
    }
    
   // if(re)
    //    re--;
    
    // b_bic_orr(WDTCON,0,(5<<16));
    u8 max_index[2],max_cnt[2];
    u8 middle_index = 0,middle_indexs_cnt = 0;
    s8 middle_indexs[5];
    u8 min_cnt = (SDRTUN2_CON >> 8) & 0xff;
    if(!min_cnt) min_cnt = 14;
    
    middle_indexs[0] = 0;
    middle_indexs[1] = -1;
    middle_indexs[2] = 1;
    middle_indexs[3] = -2;
    middle_indexs[4] = 2;
    //1.选出tune_tab_1中最连续的区间
	if(sdr_tune_source == CLK_TUN)
		tune_values_max(tune_tab_1_clk,ARRAY_NUM(tune_tab_1_clk),&tune_by,&max_index[0],&max_cnt[0]);
	else
		tune_values_max(tune_tab_1_dll,ARRAY_NUM(tune_tab_1_dll),&tune_by,&max_index[0],&max_cnt[0]);
    if(max_cnt[0] < min_cnt)
    {
        return -1; //报错,让外部重新再tuning一次
    }


    //2.找到该区间的中部
	if(sdr_tune_source == CLK_TUN)
		middle_index = tab_next_index(ARRAY_NUM(tune_tab_1_clk),max_index[0],max_cnt[0]/2);  //OUTPUT
	else
		middle_index = tab_next_index(ARRAY_NUM(tune_tab_1_dll),max_index[0],max_cnt[0]/2);  //OUTPUT
    if(max_cnt[0] > 10)
    {
        middle_indexs_cnt = 5;
    }
    else if(max_cnt[0] > 5) //[10,5)
    {
        middle_indexs_cnt = 3;
    }
    else //[5,0)
    {
        middle_indexs_cnt = 1;
    }

	
	
    
    //3.从中部选出bit[4:0]最连续的
   
    u8 id_temp,cnt_temp,by_id;
    u32 i;
    max_index[1] = max_cnt[1] = 0;
    for(i = 0;i<middle_indexs_cnt;i++)
    {
		if(sdr_tune_source == CLK_TUN){
			by_id = tab_next_index(ARRAY_NUM(tune_tab_1_clk),middle_index,middle_indexs[i]);
			tune_values_max(tune_tab_2_clk,ARRAY_NUM(tune_tab_2_clk),
							&tune_values[by_id],&id_temp,&cnt_temp);
		}else{
			by_id = tab_next_index(ARRAY_NUM(tune_tab_1_dll),middle_index,middle_indexs[i]);
			tune_values_max(tune_tab_2_dll,ARRAY_NUM(tune_tab_2_dll),
							&tune_values[by_id],&id_temp,&cnt_temp);
		}
			
        if(cnt_temp > max_cnt[1])
        {
            max_cnt[1]   = cnt_temp; //最佳的tune value在b4_0_tab中的连续数目
            max_index[1] = id_temp;  //最佳的tune value在b4_0_tab中的起始index
            max_index[0] = by_id; //最佳的tune value在b11_7_tab中的index
        }
    }
    if(sdr_tune_source == CLK_TUN)
		middle_index = tab_next_index(ARRAY_NUM(tune_tab_2_clk),max_index[1],max_cnt[1]/2);  //INPUT
	else
		middle_index = tab_next_index(ARRAY_NUM(tune_tab_2_dll),max_index[1],max_cnt[1]/2);  //INPUT
    

    
    if(p_by_id) *p_by_id = max_index[0];
    if(p_bx_id)  *p_bx_id  = middle_index;
    
    return 0;
}



typedef  void (* func_init_pll )(void *);
#define clr_wdt()    WDTCON = 0x5003d
#define soft_delay(t) do {volatile int i=(t)*2; while(i--); clr_wdt(); } while(0)    //10us RC 2M


__except bool check_if_xosc_exist(void)
{	
    u32 i;
	CLKCON0 |= BIT(24); //Timer 32kHz signal source sel : 0:from RC2M; 1:xosc32k 
	TMR0CON = (7<<8) ;
	TMR0PR = 32768/2 -1; 
	TMR0CNT = 0; 
	TMR0CON |= BIT(0); //enable and work on the Counter Mode

	for(i = 0; i < 0xffff; i++){
		if(TMR0CNT > 16){
			//test_dbg_putstr("xosc!\n");
            for(i = 0; i < 20; i++){
                TMR0CNT = 0;
                soft_delay(14000);         //50ms RC 2.4M
                if(TMR0CNT > 600){
                    //test_dbg_putstr("check if xosc ok!\n");
                    return true;	
                }	
            }
			return true;	
		}	
	}
	//test_dbg_putstr("no xosc!\n");
	return false;
  /**/  
}
__except bool init_sys_clk(void){
	bool if_xosc_exist = check_if_xosc_exist();
    u32 rc10M_turn = ((*(u32 *)0x4fa8)& 0xfffc00)>>10; 	
	u8 use_rc = 0;

    if(if_xosc_exist)
	{
        func_init_pll init_pll1 = 0x00104424; 
        u16 param[4]; //= {132000000/32768, (132000000%32768)*2, 0,  264000000/132000000-1};
        //prevent complier from using memcpy
        param[0] = COMBO_CLOCK/32768;
        param[1] = (COMBO_CLOCK%32768)*2;
        param[2] = 0;
        param[3] = (COMBO_CLOCK*2)/COMBO_CLOCK-1;
       
        init_pll1(param);  
        //test_dbg_putstr ("32k\r\n"); 
        
        PLL1DIV = ((200000000/32768)<<16)|((200000000%32768)*2);
        PLL1CON = 0x1323;
        SDRACON0 = 0x3ff;
		#if (SDRAM_SIZE_2M == SDRAM_SIZE)
		SDRACON1 &= ~(1<<29);
		#else
		SDRACON1 |= (1<<29);
		#endif
        CLKCON1 = (CLKCON1 &~ (7<<5))|(1<<5);   //ddr pll divide
        CLKCON0 = (CLKCON0 &~ (1<<22))|(1<<23);  //0:ddrpll 1:syspll

	   	{//print only for test 
	        UART0BAUD = COMBO_CLOCK/115200 -1;
	        while((UART0PND & 0x2)==0);
	        UART0PND |= BIT(1);  
	        UART0DAT = 'b'; 
	   	}

		//======write flag to rtc ram====
		{
			volatile int timeout = 0x8000;
			use_rc = 0xAA;

			RTCCON |= (1<<8);		 //CE ENABLE 
			timeout = 0x8000;
			RTCDATA = IRTRAM_WR_CMD;	// W CMD
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			timeout = 0x8000;
			RTCDATA = 8;				//W addr
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			timeout = 0x8000;
			RTCDATA = use_rc;		//W data
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			RTCCON &= ~(1<<8);		 //CE DISABLE	
		}

    }
	else
	{
        if(rc10M_turn == 0)
        {
            //rc10M_turn = 10000;
            //===NO 32K XOS,  IC not RC10M check ,can't run!!!
            while(1);
        }
 
        CLKCON2 |= (1<<16);  //RC10M enable, delay 100us
        CLKCON0 |= BIT(3);   //RC select RC10M;rc sel:00:rc2M ,10:RC10M
        soft_delay(1);
        CLKCON0 &= ~(BIT(1)|BIT(0)); //system clk choose RC     

        PLL0DIV  = (COMBO_CLOCK/1000/rc10M_turn)<<16;
        PLL0DIV  |= (COMBO_CLOCK%rc10M_turn)*65536/rc10M_turn;
        PLL0CON = 0x10e7;
        CLKCON1 = (COMBO_CLOCK*2)/COMBO_CLOCK-1;
        // µÈ´ýÖÁÉÙ1mS 
        soft_delay (100);
        // Ïµ½y•rçŠÄRCÇÐ“Qµ½PLL, CLKCON0[1:0]=·Öîl£º0b11£»²»·Öîl£º0x10
        CLKCON0 |= ((COMBO_CLOCK*2)/COMBO_CLOCK-1)?3:2;
        //test_dbg_putstr ("rc10M_turn\r\n");  
        
        PLL1DIV  = (200000/rc10M_turn)<<16;
        PLL1DIV  |= (200000%rc10M_turn)*65536/rc10M_turn;
        PLL1CON = 0x13e7;
        SDRACON0 = 0x3ff;
        CLKCON1 = (CLKCON1 &~ (7<<5))|(1<<5);   //ddr pll divide
        CLKCON0 = (CLKCON0 &~ (1<<22))|(1<<23);  //0:ddrpll 1:syspll

		{//print only for test 
	        UART0BAUD = COMBO_CLOCK/115200 -1;
	        while((UART0PND & 0x2)==0);
	        UART0PND |= BIT(1);  
	        UART0DAT = 'c'; 
		}
		//======write flag to rtc ram====
		{
			volatile int timeout = 0x8000;

			RTCCON |= (1<<8);		 //CE ENABLE 
			timeout = 0x8000;
			RTCDATA = IRTRAM_WR_CMD;	// W CMD
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			timeout = 0x8000;
			RTCDATA = 8;				//W addr
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			timeout = 0x8000;
			RTCDATA = use_rc;		//W data
			while(RTCCON & (1<<3))
			{
				timeout--;
				if(timeout==0)
					break;
			}
			RTCCON &= ~(1<<8);		 //CE DISABLE	
		}

    }
    
}


#define CRC8_ALPHA       0x31     //x^8+x^5+x^4+1
__except u32 crc_cal(u32 data)
{
	u8 temp = 0;
	u8 crc8 = CRC8_ALPHA;
	u32 crc_reg = 0;
	u8 i = 0, j = 0;
	for(i = 0;i < 4; i++)
	{
		temp = (u8)((data>>(i<<3))&0xff); //从低字节开始进行crc计算
		/*数据往左移了8位，需要计算8次*/
		for(j = 8; j>0;--j)
		{	
			WTD_DIS();
			if(temp & 0x80)  //判断最高位是否为1
				temp = (temp<<1)^crc8;
			else
				temp = temp<<1;
		}
		crc_reg |= (temp<<(i<<3));  //将计算的crc值存回crc_reg中
	}
	return crc_reg;
}


__except void sdram_clock_tune(void)  //clktun 
{
	init_sys_clk();

	LDOCON = (LDOCON & ~(0x03<<13))|((SYS_VOL_V1_30&0x03)<<13);  //LDO 1.2V; 0:1.0V, 1: 1.2V, 2: 1.25V, 3: 1.3V
	exception_delay(20);
//	uint32_t mid;

	WTD_DIS();
	LDOCON |=(1<<15);//adkey bandgap enable11
//	u16 vbg_set = 0xf;
	//efuse
	u8 vbg_trim=(((*(u32 *)(0x5000-140+0x30 + 8))>>24)& 0x1f);
	if(vbg_trim != 0)
	{
		LDOCON = (LDOCON & ~(0x1f<<16))|((vbg_trim)<<16);
		//vbg_set = vbg_trim;
	}	
	u8 vbg_ofs = (*(u32 *)(0x5000-140+0x30 + 4) & 0x7f);
	if(vbg_ofs != 0)
	{
		//vbg_param = vbg_ofs +737;
		if(vbg_trim != 0)
		{
			vbg_param = (vbg_trim*31)/10+755;
		}
		else
		{
			vbg_param =800;
		}
	}
	else
	{
		vbg_param =800;
	}

	//LDOCON |= (0xF<<16);//bandgap voltage select (3<<12)
#if 1 //DLL打开前打开后测量1次	
	//	u16 vbg_adc0,vbg_adc1;
	//	vbg_adc0 = adcGetChannel(11);//adc0
#endif	
	
//-------------read chip id
	chip_vision = CHIP_ID &0xffff0000;
	
	if(chip_vision == THUNDERSE_A1 )  //B2 do not use dll
	{
		sdr_tune_source = CLK_TUN;
		dllcon_init();
	}
	if(chip_vision == THUNDERSE_B1 )  //B2 do not use dll
	{
		sdr_tune_source = DLL_TUN;
		dllcon_init();
	}	
	else if(chip_vision == THUNDERSE_B2 ) 
	{	
		sdr_tune_source = CLK_TUN;
		DLLCON = 0;
	}
	//delay(200);
	exception_delay(200);
	SPR_DCCR &= ~BIT(0);
	
	/*************read presave tune value*******/
#if USE_PRESAVE_TUNE
	u32 pre_save_tunevalue = 0;
	u32 crc_check = 0;

	_rtcRamRead(HAL_TUNEVALUE_ADDR,(u8 *)&pre_save_tunevalue,4);
	_rtcRamRead(HAL_CRCCHECK_ADDR,(u8 *)&crc_check,4);
	

	//pre_save_tunevalue = 0;
	if(pre_save_tunevalue)
	{
		if(crc_check == crc_cal(pre_save_tunevalue))
		{
			if(sdr_tune_source == CLK_TUN){
				CLKTUN = pre_save_tunevalue;  //11_7: OUTPUT, 4_0: INPUT 
			}else{
				DLLCON = pre_save_tunevalue;
			}
			my_putchar('b');
			goto TUNE_END;
		}
		//my_putchar('d');
	}
#endif
    u32 cnty  = 0,tuny = 0xffffffff;
    u32 cntx   = 0,tunx  = 0xffffffff;
    u32 cnty_temp = 0;
    u32 i = 0,
        j = 0;
	
		
	SDRTUN2_CON = 0;	
	//_sdr_tuning2_conifg_set(0);
	
	
	WTD_DIS();

	
    uint32_t tune_cnt = 16;
    sdram_tuning_check_init();
	uint32_t sdrsize_shf = 23;  //8M 
	 if(!(SDRACON1 & (1<<29)))
		 sdrsize_shf =21;
		 
	uint32_t i_max,j_max;
	if(sdr_tune_source == CLK_TUN)
	{
		i_max = ARRAY_NUM(tune_tab_1_clk);
		j_max = ARRAY_NUM(tune_tab_2_clk); 
	}
	else{
		i_max = ARRAY_NUM(tune_tab_1_dll);
		j_max = ARRAY_NUM(tune_tab_2_dll);
	}
begain: //如果select()报错,将可能重新开始tune

if(sdr_tune_source == CLK_TUN)
	CLKTUN |= CLKTUN_CONST;
else
	CLKTUN &= ~CLKTUN_CONST;

    
	sdram_init();
	
    tune_values_init();
  //  WDTCON = 8 & 7;
	
	my_putchar('\n');


    for(
      //  clktun_const = (1<<12)|(1<<6)|(1<<5),
		//clktun_const = (1<<12) | (1<<13) | (1<<6) | (1<<5),
        cnty = 0,    
        tuny = 0xffffffff, 
        i = 0;
        i < i_max;
        i++)
	{
		
		//sdram_init();
        for(cntx = 0,
            tunx = 0xffffffff,
            cnty_temp = 0, //用于给[11:7]的计数做依据
            j = 0;
            j < j_max;
            j++)
        {
			if(sdr_tune_source == CLK_TUN)
			{
				//clktun_const = (1<<12) | (1<<13) | (1<<6) | (1<<5);
				CLKTUN = CLKTUN_CONST | (tune_tab_1_clk[i] << CLKTUN_OUTPUT_POS) | (tune_tab_2_clk[j]<<CLKTUN_INPUT_POS);
			}else{
				DLLCON &= ~DLLTUN_CONST;
				DLLCON |= (tune_tab_1_dll[i]<<DLLTUN_CH1_POS)|(tune_tab_2_dll[j]<<DLLTUN_CH2_POS);
			}
			WTD_DIS();
			exception_delay(5);
			 
            //delay(5);
			sdram_init();
			
	
			if(sdram_tuning_check_2 (sdrsize_shf)&&sdram_tuning_check_1())
            {
				
				my_putchar('1');
				my_putchar(' ');
                if(tunx == 0xffffffff)
                {
                    tunx = CLKTUN;
                    if(tune_values[i].remain)
                    {
                        //store (i,j)
                        tune_values[i].value[tune_values[i].cur].index = j;
                    }
                }
				
                cnty_temp++; //表示小循环tune到过值
                cntx++;
            }
            else
            {
				my_putchar('0');
				my_putchar(' ');
                if(cntx != 0)
                {
                    //store clktun value and cnt
                    tune_values[i].value[tune_values[i].cur].cnt = cntx;
                    tune_values[i].cur++;
                    tune_values[i].remain--;
                }
                
                tunx = 0xffffffff;
                cntx = 0;
            }
            WTD_DIS();
			
			
          // b_bic_orr(WDTCON,0,(5<<16));  //clear wdt
        }

        //补充对末尾的检查
        if(cntx != 0)
        {
            cnty_temp++;
            //store bit [4:0] clktun value and cnt
            tune_values[i].value[tune_values[i].cur].cnt = cntx;
            tune_values[i].cur++;
            tune_values[i].remain--;
        }
        
        if(cnty_temp) //表示前边的小循环有tune到过正确的值
        {
            if(tuny == 0xffffffff)
            {
                tuny = CLKTUN;
                if(tune_by.remain)
                {
                    //store i
                    tune_by.value[tune_by.cur].index = i;
                }
            }
            cnty++;
			
        }
        else
        {
            if(cnty != 0)
            {
                //store [11:7] clktun value and cnt
                tune_by.value[tune_by.cur].cnt = cnty;
                tune_by.cur++;
                tune_by.remain--;
            }
            
            tuny = 0xffffffff;
            cnty = 0;
        }
        
       // if(sdr_tuning2_hook)
        //    sdr_tuning2_hook(i,ARRAY_NUM(b11_7_tab));
		my_putchar('\n');	 
    }
    
    if(cnty != 0)
    {
        //store [11:7] clktun value and cnt
        tune_by.value[tune_by.cur].cnt = cnty;
        tune_by.cur++;
        tune_by.remain--;
    }
    
    //_dcache_enable(false);  //dcache must be disable,or enter main will go to exception.

   // tune_values_debg();
    
    //select best tuning value
    u8 by_id = 0,bx_id = 0, save_tunevale = 0;
    if((tune_values_select(&by_id,&bx_id)))
    {
		if(tune_cnt){
			tune_cnt--;
			goto begain;
		}
    }else{
		save_tunevale = 1;  //0: not save tune value, 1: save tune value
	}

	if(sdr_tune_source == CLK_TUN){
		 CLKTUN = CLKTUN_CONST | (tune_tab_1_clk[by_id]<<7) | (tune_tab_2_clk[bx_id]<<0);  //11_7: OUTPUT, 4_0: INPUT 
	}else{
		DLLCON &= ~DLLTUN_CONST;
		DLLCON |= (tune_tab_1_dll[by_id]<<DLLTUN_CH1_POS)|(tune_tab_2_dll[bx_id]<<DLLTUN_CH2_POS);
	}
	//u32 tunevalue = 0, tune_crc = 0;
#if USE_PRESAVE_TUNE
	pre_save_tunevalue = 0;
	crc_check = 0;
	if(save_tunevale)
	{
		if(sdr_tune_source == CLK_TUN){
			pre_save_tunevalue = CLKTUN;  //11_7: OUTPUT, 4_0: INPUT 
		}else{
			pre_save_tunevalue = DLLCON;
		}
		crc_check = crc_cal(pre_save_tunevalue);

	}	
	_rtcRamWrite(HAL_TUNEVALUE_ADDR,(u8 *)&pre_save_tunevalue,4); //save tune value
	_rtcRamWrite(HAL_CRCCHECK_ADDR,(u8 *)&crc_check,4); //save tune value
TUNE_END:	
#endif
	exception_delay(100);
	WTD_DIS();
	//delay(100);
	sdram_init();
	
	exception_delay(100);
	 WTD_DIS();
	//delay(100);
#if 1 //DLL打开前打开后测量1次		
	//	vbg_adc1 = adcGetChannel(11);//adc0
	//	vbg_param = vbg_adc1 * vbg_param / vbg_adc0;
	//	vbg_param_adc = vbg_param *1024/3300;

#endif	

}
	
#else

#define PSRAM_TEST_EN  	0	// 0 : disable psram  test , 1: enable psram test
#define PSRAM_TYPE_NUM_MAX	 4

#define PSRAM_WB_BURST64
//#define WB_VAR_LATENCY		// enable can up 3~5% speed
#define AP_VAR_READ			// read fast can up 3~5% speed

#define PSRAM_FRQ_133M		0
#define PSRAM_FRQ_166M		1
#define PSRAM_FRQ_200M		2
#define PSRAM_FRQ_225M		3
#define PSRAM_FRQ_230M		4
#define PSRAM_FRQ_233M		5
#define PSRAM_FRQ_250M		6

#define PSRAM_FRQ_SEL 		PSRAM_FRQ_200M


#define clr_wdt()    WDTCON = 0x5003d
#define soft_delay(t) do {volatile int i=(t)*2; while(i--); clr_wdt(); } while(0)    //10us RC 2M

__except u8 check_if_xosc_exist(void)
{	
    u32 i;
	CLKCON0 |= BIT(24); //Timer 32kHz signal source sel : 0:from RC2M; 1:xosc32k 
	TMR0CON = (7<<8) ;
	TMR0PR = 32768/2 -1; 
	TMR0CNT = 0; 
	TMR0CON |= BIT(0); //enable and work on the Counter Mode

	for(i = 0; i < 0xffff; i++){
		if(TMR0CNT > 16){
			//test_dbg_putstr("xosc!\n");
            for(i = 0; i < 20; i++){
                TMR0CNT = 0;
                soft_delay(14000);         //50ms RC 2.4M
                if(TMR0CNT > 600){
                    //test_dbg_putstr("check if xosc ok!\n");
                    return 1;	
                }
            }
			return 1;
		}
	}
	//test_dbg_putstr("no xosc!\n");
	return 0;
}


//////////////////
// Initialize SYSPLL
//////////////////
__except static void psram_init_syspll(u8 xosc_exist)
{
  ////////////////////
  // Local Variables
  ////////////////////
  unsigned int PLL0_BIASS ;
  unsigned int PLL0_VCOS  ;
  unsigned int PLL0_LPFS  ;
  unsigned int PLL0_DUPT  ;
  unsigned int PLL0_RIS   ;
  unsigned int PLL0_DEN   ;
  unsigned int PLL0_AEN   ;
  unsigned int PLL0_INT   ;
  unsigned int PLL0_FRAC  ;
  
  /////////////////////////////
  // 230MHz ~ 340MHz
  /////////////////////////////
  PLL0_VCOS = 1; 
  PLL0_RIS = 0; 
  PLL0_DUPT = 1; 
  PLL0_DEN = 1; 
  PLL0_AEN = 1; 
  PLL0_BIASS = 2;

  if(xosc_exist)
  {
      PLL0_LPFS = 0; 		// 32k input
//	  PLL0_INT = 4394; 
//	  PLL0_FRAC = 34816; 
	  PLL0_INT = (PLL_CLK/2)/32768;		// 288M pll 
	  PLL0_FRAC = ((PLL_CLK/2)%32768)*2;
  }
  else
  {
	  //==RC128K tirm==
	  u8 rtcset;
	  u16 rc128k_div; 
	  u8 rc128k_trim;
	  u32 rc128k = ((*(u32 *)0x4fac)& 0xffffff)>>1;   
	  rc128k_div = rc128k & 0xFFFF;
	  rc128k_trim = (rc128k & 0x7e0000)?(rc128k & 0x7e0000)>>17:0x20;
	  
	  if(0==rc128k_div)
	  {
		  //===EFUSE NO 128K tuning, can't run!!!
		  //while(1);
		  rc128k_div=0xF8D0;	  // default val
		  rc128k_trim=0x18; 	  // default val 
	  }
	  
	  rtcset = (rc128k_div&0xff); 
	  exception_irtc_write(IRTCDIVL_WR_CMD,rtcset); 
	  rtcset = (rc128k_div&0xff00)>>8; 
	  exception_irtc_write(IRTCDIVH_WR_CMD,rtcset); 
	  rtcset=rc128k_trim<<1|BIT(0); 				  // 128K trim set 
	  exception_irtc_write(IRTCON8_WR_CMD,rtcset);	  // open 128K RC
	  CLKCON0 |= BIT(27);							  // xosc sel rc128K 
	  rtcset = exception_irtc_read(IRTCON0_RD_CMD);   
	  rtcset|=BIT(5)|BIT(4)|BIT(0);				  // xosc enable 
	  exception_irtc_write(IRTCON0_WR_CMD,rtcset);	  
	  //==end RC128K tirm==

  	  PLL0_LPFS = 1; 		// 128K input
	  PLL0_INT  = ((PLL_CLK/2)/1000/128);
	  PLL0_FRAC  = ((PLL_CLK/2)/1000%128)*65536/128;
  }

  PLL0DIV = (PLL0_INT<<16) | 
  PLL0_FRAC; 
  PLL0CON = (PLL0_VCOS<<8) | 
  	(PLL0_BIASS <<11) |
  (PLL0_LPFS<<6) | 
  (PLL0_RIS <<2) | 
  PLL0_AEN | 
  (PLL0_DEN <<1) | 
  (PLL0_DUPT<<5);
  
}

//////////////////
//Initialize DDRPLL
//////////////////
__except static void psram_init_ddrpll(u8 xosc_exist)
{
	////////////////////
	// Local Variables
	////////////////////
	unsigned int PLL1_BIASS ;
	unsigned int PLL1_VCOS  ;
	unsigned int PLL1_LPFS  ;
	unsigned int PLL1_DUPT  ;
	unsigned int PLL1_RIS   ;
	unsigned int PLL1_DEN   ;
	unsigned int PLL1_AEN   ;
	unsigned int PLL1_INT   ;
	unsigned int PLL1_FRAC  ;
	u32 pll1_clk;
	
	PLL1_RIS = 0; 
	PLL1_DUPT = 1; 
	PLL1_DEN = 1; 
	PLL1_AEN = 1; 
	PLL1_BIASS = 2;

	if(PSRAM_FRQ_133M==PSRAM_FRQ_SEL)
	{
		pll1_clk = 133000000;
		PLL1_VCOS = 4;
	}
	else if(PSRAM_FRQ_166M==PSRAM_FRQ_SEL)
	{
		pll1_clk = 166000000;
		PLL1_VCOS = 5;
	}
	else if(PSRAM_FRQ_200M==PSRAM_FRQ_SEL)
	{
		pll1_clk = 200000000;
		PLL1_VCOS = 6;
	}
	else if(PSRAM_FRQ_225M==PSRAM_FRQ_SEL)
	{
		pll1_clk = 225000000;
		PLL1_VCOS = 6;
	}
	else if(PSRAM_FRQ_230M==PSRAM_FRQ_SEL)
	{
		pll1_clk = 230000000;
		PLL1_VCOS = 6;
	}
	else if(PSRAM_FRQ_233M==PSRAM_FRQ_SEL)
	{
		pll1_clk = 233000000;
		PLL1_VCOS = 6;
	}
	else if(PSRAM_FRQ_250M==PSRAM_FRQ_SEL)
	{
		pll1_clk = 250000000;
		PLL1_VCOS = 7;
	}
	else
	{
		pll1_clk = 200000000;
		PLL1_VCOS = 6;
	}

	if(xosc_exist)
	{
	   	PLL1_LPFS = 0; 		// 32k input
	   	PLL1_INT = ((pll1_clk/2)/32768);		// 200M pll 
	   	PLL1_FRAC = (((pll1_clk/2)%32768)*2);
	}
	else
	{
		PLL1_LPFS = 1; 		// 128k input
	  	PLL1_INT = ((pll1_clk/2)/1000)/128;					// 200M pll 
	  	PLL1_FRAC = (((pll1_clk/2)/1000)%128)*65536/128;
	}

	PLL1DIV = (PLL1_INT<<16) | 
	PLL1_FRAC; 
	PLL1CON = (PLL1_VCOS<<8) | 
	(PLL1_BIASS << 11) |
	(PLL1_LPFS<<6) | 
	(PLL1_RIS <<2) | 
	PLL1_AEN | 
	(PLL1_DEN <<1) | 
	(PLL1_DUPT<<5);

}

//////////////////
//Initialize system clock
//////////////////
__except static void psram_gen_clk()
{
  ////////////////////
  // Local Variables
  ////////////////////
  unsigned int  SPLL_DIV  ;
  unsigned int  SDRPLL_DIV;
  unsigned int  SCSEL     ;
  ////////////////////
  // system clock
  ////////////////////
  SCSEL   = 3;								// 3: system clk output after pll div 
  SDRPLL_DIV = 0;
  SPLL_DIV=(COMBO_CLOCK*2)/COMBO_CLOCK-1;	// system pll div clock 
  CLKCON1 = (CLKCON1&~(0xFF))|(SDRPLL_DIV<<5)|SPLL_DIV;
  CLKCON0 = (CLKCON0&~(BIT(1)|BIT(0)))/*|BIT(23)*/|SCSEL;
  exception_delay(500);
}

__except static void psram_clk_init() 
{
  u8 xosc_exist	= check_if_xosc_exist();
  u8 use_rc = 0;
  ////////Initialize SYSPLL
  psram_init_syspll(xosc_exist);

  ////////Initialize DDRPLL
  psram_init_ddrpll(xosc_exist);

  ////////system clock
  psram_gen_clk();

//======write flag to rtc ram====
	if(xosc_exist)
	{
		use_rc=0xAA;
	}
	volatile int timeout = 0x8000;

	RTCCON |= (1<<8);		 //CE ENABLE 
	timeout = 0x8000;
	RTCDATA = IRTRAM_WR_CMD;	// W CMD
	while(RTCCON & (1<<3))
	{
		timeout--;
		if(timeout==0)
			break;
	}
	timeout = 0x8000;
	RTCDATA = 8;				//W addr
	while(RTCCON & (1<<3))
	{
		timeout--;
		if(timeout==0)
			break;
	}
	timeout = 0x8000;
	RTCDATA = use_rc;			//W data
	while(RTCCON & (1<<3))
	{
		timeout--;
		if(timeout==0)
			break;
	}
	RTCCON &= ~(1<<8);		 	//CE DISABLE	
}

//==idx define:===
//==0:APS3208K_OKUD:4MB==
//==1:APS6408L_OBX:8MB==
//==2:W955D8MBKX:4MB==
//==3:W956D8NKR:8MB==
__except static void psram_type_clk_cfg(u8 idx,u8 frq)
{
	u8 ret = 0;
	if(0==idx)
	{
//		PMC_DLL_LOCK_CTL = 0x2b2b2b05;
		PMC_CFG_REG 	 = 0x80000001;//relase reset and select pram type

		//PMC_TPU 		 = 0x01407531;//tVCS for wb
		PMC_TPU 		 = 0x0140752b;//tVCS for wb
		PMC_TRST		 = 0x00000191;//tRPH for wb
		//PMC_TCEM		 = 0x00000313;//tCSM for wb
		PMC_TCEM		 = 0x0000024B;//tCSM for wb
		PMC_THS_INT 	 = 0x00030d40;
		PMC_TDPD_INIT	 = 0x000186a0;
		PMC_THS 		 = 0x00007536;
		PMC_TXPHS		 = 0x00000010;
		PMC_TXHS		 = 0x00007534;
		PMC_TDPD		 = 0x000186a6;
		PMC_TXPDPD		 = 0x00000010;
		PMC_TXDPD		 = 0x00007534;

		if(PSRAM_FRQ_133M==frq)
		{
			PMC_COMMAND_TIMING = 0x0004000b;//[25:16]TCPH=0x4
//			PMC_RW_LATENCY   = 0x0101000c;
			PMC_RW_LATENCY   = 0x01010008;//[21:16]WL=0+1 [5:0]RL=8
			PMC_IDLE_WAIT 	 = 0x80030d40;
			#ifdef AP_VAR_READ
				PMC_MR_INIT  = 0x00000013;//MR4[7:6]WL=00  MR0[4:2]RL=100 MR0[5]=0 select variable r_latency
			#else
				PMC_MR_INIT  = 0x00000033;//MR4[7:6]WL=00  MR0[4:2]RL=100 MR0[5]=1 select fix r_latency
			#endif
		}
		else if(PSRAM_FRQ_166M==frq)
		{
			PMC_COMMAND_TIMING = 0x0004000b;//[25:16]TCPH=0x4
//			PMC_RW_LATENCY	 = 0x0103000c;
			PMC_RW_LATENCY	 = 0x0103000a;//[21:16]WL=2+1 [5:0]RL=10
			PMC_IDLE_WAIT 	 = 0x80030d40;
			#ifdef AP_VAR_READ
				PMC_MR_INIT 	 = 0x00008017;//MR4[7:6]WL=10  MR0[4:2]RL=101 MR0[5]=0 select variable r_latency
			#else
				PMC_MR_INIT 	 = 0x00008037;//MR4[7:6]WL=10  MR0[4:2]RL=101 MR0[5]=1 select fix r_latency
			#endif
		}
		else if(PSRAM_FRQ_200M==frq)
		{
/*
			PMC_COMMAND_TIMING = 0x0006000b;//[25:16]TCPH=0x6
//			PMC_RW_LATENCY	 = 0x0103000c;
			PMC_RW_LATENCY	 = 0x0103000c;//[21:16]WL=2+1 [5:0]RL=12
			PMC_IDLE_WAIT 	 = 0x80030d40;
			#ifdef AP_VAR_READ
				PMC_MR_INIT 	 = 0x0000801b;//MR4[7:6]WL=10  MR0[4:2]RL=110 MR0[5]=0 select variable r_latency
			#else
				PMC_MR_INIT 	 = 0x0000803b;//MR4[7:6]WL=10  MR0[4:2]RL=110 MR0[5]=1 select fix r_latency
			#endif
*/

			PMC_COMMAND_TIMING = 0x0008000b;//[25:16]TCPH=0x8
			PMC_RW_LATENCY	 = 0x0103000c;//[21:16]WL=2+1 [5:0]RL=12
			PMC_IDLE_WAIT	 = 0x80030d40;
			#ifdef AP_VAR_READ
				PMC_MR_INIT 	 = 0x00008019;//MR4[7:6]WL=10  MR0[4:2]RL=110 MR0[5]=0 select variable r_latency
			#else
				PMC_MR_INIT 	 = 0x00008039;//MR4[7:6]WL=10  MR0[4:2]RL=110 MR0[5]=1 select fix r_latency
			#endif

		}
		else if(PSRAM_FRQ_233M==frq)
		{
			PMC_COMMAND_TIMING = 0x0008000b;//[25:16]TCPH=0x8
//			PMC_RW_LATENCY	 = 0x0103000c;
			PMC_RW_LATENCY	 = 0x0103000e;//[21:16]WL=2+1 [5:0]RL=14
			PMC_IDLE_WAIT 	 = 0x80030d40;
			#ifdef AP_VAR_READ
				PMC_MR_INIT 	 = 0x0000801f;//MR4[7:6]WL=10  MR0[4:2]RL=111 MR0[5]=0 select variable r_latency
			#else
				PMC_MR_INIT 	 = 0x0000803f;//MR4[7:6]WL=10  MR0[4:2]RL=111 MR0[5]=1 select fix r_latency
			#endif
		}
		else	// default 200M 
		{
			PMC_COMMAND_TIMING = 0x0006000b;//[25:16]TCPH=0x6
//			PMC_RW_LATENCY	 = 0x0103000c;
			PMC_RW_LATENCY	 = 0x0103000c;//[21:16]WL=2+1 [5:0]RL=12
			PMC_IDLE_WAIT 	 = 0x80030d40;
			#ifdef AP_VAR_READ
				PMC_MR_INIT 	 = 0x0000801b;//MR4[7:6]WL=10  MR0[4:2]RL=110 MR0[5]=0 select variable r_latency
			#else
				PMC_MR_INIT 	 = 0x0000803b;//MR4[7:6]WL=10  MR0[4:2]RL=110 MR0[5]=1 select fix r_latency
			#endif
		}

		PMC_SERVICE_REG = 0xf<<16|0x3<<0;	// write faster than read 

		PMC_CSN_DUMMY	 = 0x0000408a;//CSNDUMMY[8:6]WCMD_DUMMY=010
		//PMC_CSN_DUMMY    = 0x00001087;//DQS_GATING_START[14:12]=1 CSNDUMMY[8:6]WCMD_DUMMY=010 RCMD_DUMMY[3:0]=1

		PMC_TCQLZ		 = 0x01070007;
		#ifdef AP_VAR_READ
		  	//PMC_AP_SPECIAL   = 0x000000d0;//AP_SPECIAL[0]FIX_RD_LATENCY=0
			PMC_AP_SPECIAL   = 0x00000000;//AP_SPECIAL[0]FIX_RD_LATENCY=0
		#else
		  	//PMC_AP_SPECIAL   = 0x000000d1;//AP_SPECIAL[0]FIX_RD_LATENCY=1
			PMC_AP_SPECIAL   = 0x00000001;//AP_SPECIAL[0]FIX_RD_LATENCY=1
		#endif
		PMC_TRP 		 = 0x000000c9;
		PMC_TRWR		 = 0x00000005;
		
		PMC_CTRL_REG	 = 0x00000001;
		while(PMC_INITIAL_STATUS != 0x03);
/*
		exception_delay(100);
		PMC_DELAY_CTRL = (0x3 << 21)| // RDLL1_DELAY
		                 (0x0 << 20)| // RDLL1_DELAY_WR
		                 //(0x14 << 11)| // RDLL0_DELAY
		                 (0xFF << 11)| // RDLL0_DELAY
		                 (0x0 << 10)| // RDLL0_DELAY_WR
		                 (0x11 << 1)| // WDLL_DELAY
		                 (0x0 << 0); // WDLL_DELAY_WR
		PMC_DELAY_CTRL |= BIT(10);	// RDLL0_DELAY_WR
		exception_delay(100);
		
		exception_putword(PMC_DELAY_CTRL);
		exception_putchar('\n');
*/
	}
	else if(1==idx)
	{
//		PMC_DLL_LOCK_CTL = 0x2b2b2b05;
		PMC_CFG_REG      = 0x80000003;//relase reset and select pram type

		PMC_TPU          = 0x0000752b;// tVCS for wb
		PMC_TRST         = 0x752b0191;//tRPH for wb
		PMC_TCEM         = 0x00000633;//tCSM for wb
		PMC_THS_INT      = 0x00030d18;
		PMC_TDPD_INIT    = 0x0001868c;
		PMC_THS          = 0x00007530;
		PMC_TXPHS        = 0x00000010;
		PMC_TXHS         = 0x0000752f;
		PMC_TDPD         = 0x00018692;
		PMC_TXPDPD       = 0x00000010;
		PMC_TXDPD        = 0x00007533;

		PMC_IDLE_WAIT	 = 0x80030d40;

		if(PSRAM_FRQ_133M==frq)
		{
		  	PMC_COMMAND_TIMING = 0x0004000c;//[25:16]TCPH=0x2
//		  	PMC_RW_LATENCY   = 0x0105000c;
		  	PMC_RW_LATENCY   = 0x0105000a;//[21:16]WL=5 [5:0]RL=10
			#ifdef AP_VAR_READ
		    	PMC_MR_INIT  = 0x00074008;//MR4[7:5]WL=010  MR0[4:2]RL=010 MR0[5]=0 select variable r_latency MR0[1:0]=00
			#else
		    	PMC_MR_INIT  = 0x00074025;//MR4[7:6]WL=010  MR0[4:2]RL=010 MR0[5]=1 select fix r_latency MR0[1:0]=00
			#endif
		}
		else if(PSRAM_FRQ_166M==frq)
		{
		  	PMC_COMMAND_TIMING = 0x0004000c;//[25:16]TCPH=0x3
//			PMC_RW_LATENCY	 = 0x0106000c;
			PMC_RW_LATENCY	 = 0x0106000c;//[21:16]WL=6 [5:0]RL=12
			#ifdef AP_VAR_READ
		    	PMC_MR_INIT  = 0x0007c00c;//MR4[7:5]WL=110  MR0[4:2]RL=011 MR0[5]=0 select variable r_latency MR0[1:0]=00
			#else
		    	PMC_MR_INIT  = 0x0007c02c;//MR4[7:6]WL=110  MR0[4:2]RL=011 MR0[5]=1 select fix r_latency MR0[1:0]=00
			#endif
		}
		else if(PSRAM_FRQ_200M==frq)
		{
		  	PMC_COMMAND_TIMING = 0x0004000c;//[25:16]TCPH=0x4
//			PMC_RW_LATENCY	 = 0x0107000c;
			PMC_RW_LATENCY	 = 0x0107000e;//[21:16]WL=7 [5:0]RL=14
			#ifdef AP_VAR_READ
		    	PMC_MR_INIT  = 0x00072010;//MR4[7:5]WL=001  MR0[4:2]RL=100 MR0[5]=0 select variable r_latency MR0[1:0]=00
			#else
		    	PMC_MR_INIT  = 0x00072030;//MR4[7:6]WL=001  MR0[4:2]RL=100 MR0[5]=1 select fix r_latency MR0[1:0]=00
			#endif
		}
		else if(PSRAM_FRQ_225M==frq)
		{
		  	PMC_COMMAND_TIMING = 0x0008000c;//[25:16]TCPH=0x7
//			PMC_RW_LATENCY	 = 0x0108000c;
			PMC_RW_LATENCY	 = 0x01080010;//[21:16]WL=8 [5:0]RL=16
			#ifdef AP_VAR_READ
		    	PMC_MR_INIT  = 0x0007a014;//MR4[7:5]WL=101  MR0[4:2]RL=101 MR0[5]=0 select variable r_latency MR0[1:0]=00
			#else
		    	PMC_MR_INIT  = 0x0007a034;//MR4[7:6]WL=101  MR0[4:2]RL=101 MR0[5]=1 select fix r_latency MR0[1:0]=00
			#endif
		}
		else if(PSRAM_FRQ_250M==frq)
		{
		  	PMC_COMMAND_TIMING = 0x0008000c;//[25:16]TCPH=0x7
//			PMC_RW_LATENCY	 = 0x0108000c;
			PMC_RW_LATENCY	 = 0x01080010;//[21:16]WL=8 [5:0]RL=16
			#ifdef AP_VAR_READ
		    	PMC_MR_INIT  = 0x0007a014;//MR4[7:5]WL=101  MR0[4:2]RL=101 MR0[5]=0 select variable r_latency MR0[1:0]=00
			#else
		    	PMC_MR_INIT  = 0x0007a034;//MR4[7:6]WL=101  MR0[4:2]RL=101 MR0[5]=1 select fix r_latency MR0[1:0]=00
			#endif
		}
		else	// default  200M  frq 
		{
		  	PMC_COMMAND_TIMING = 0x0004000c;//[25:16]TCPH=0x4
//			PMC_RW_LATENCY	 = 0x0107000c;
			PMC_RW_LATENCY	 = 0x0107000e;//[21:16]WL=7 [5:0]RL=14
			#ifdef AP_VAR_READ
		    	PMC_MR_INIT  = 0x00072010;//MR4[7:5]WL=001  MR0[4:2]RL=100 MR0[5]=0 select variable r_latency MR0[1:0]=00
			#else
		    	PMC_MR_INIT  = 0x00072030;//MR4[7:6]WL=001  MR0[4:2]RL=100 MR0[5]=1 select fix r_latency MR0[1:0]=00
			#endif
		}

		PMC_SERVICE_REG = 0xf<<16|0x3<<0;	// write faster than read 

		//PMC_CSN_DUMMY    = 0x0000408a;//CSNDUMMY[8:6]WCMD_DUMMY=010
		PMC_TCQLZ        = 0x01080008;

		#ifdef AP_VAR_READ
		  	PMC_AP_SPECIAL   = 0x000000d0;//AP_SPECIAL[0]FIX_RD_LATENCY=0
		#else
		  	PMC_AP_SPECIAL   = 0x000000d1;//AP_SPECIAL[0]FIX_RD_LATENCY=1
		#endif

		PMC_TRWR         = 0x00000001;
		PMC_TRP          = 0x000000c9;
		
		PMC_CTRL_REG     = 0x00000001;
		while(PMC_INITIAL_STATUS != 0x03);

	}
	else if(2==idx)
	{
		 PMC_MEMORY_RESET = BIT(16);
		 exception_delay(500);
		 PMC_MEMORY_RESET = 0x0;
		
//		 PMC_DLL_LOCK_CTL = 0x2b2b2b05;
		 PMC_CFG_REG		= 0x8000000b;//relase reset and select pram type
		 PMC_RE_INIT	  = 0x7;
		 exception_delay(1000);  // 1000 is 50us 

//	     if(PSRAM_FRQ_166M==frq)
		 if(1)
	     {
			 PMC_TPU	   = 0x00006141;// tVCS for wb
			 PMC_TRST		  = 0x00000043;//tRPH for wb
			 PMC_TCEM		  = 0x0000028b;//tCSM for wb
			 PMC_THS			= 0x00000005;// tTHS for wb
			 PMC_TXPHS		  = 0x0000000e;// tXPHS for wb
			 PMC_TXHS		  = 0x00002d71;//txHS for wb
			 PMC_TDPD		  = 0x00000682;//tDPDIN for wb
			 PMC_TXPDPD 	  = 0x00000025;//txPDPD for wb
			 PMC_TXDPD		  = 0x0000616a;//txDPD
			 PMC_COMMAND_TIMING = 0x0005000a;//[25:16]tCPH=tRWR=4=((36/6.024)-2)
			 PMC_RW_LATENCY   = 0x000c000c;//[21:16]WL=6*2=0xc

			 #ifdef PSRAM_WB_BURST64
   				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT	  = 0x00408f15;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0001;[1,0]=01=64B;[3]VL=0=Variable Latency
   				#else
					PMC_MR_INIT	  = 0x00408f1d;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0001;[1,0]=01=64B;[3]VL=1=Fixed Latency
   				#endif
			 #else
   				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT	  = 0x00408f17;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0001;[1,0]=11=32B;[3]VL=0=Variable Latency
   				#else
					PMC_MR_INIT	  = 0x00408f1f;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0001;[1,0]=11=32B;[3]VL=1=Fixed Latency
   				#endif
			 #endif

		     PMC_TCQLZ		= 0x01060006;

			#ifdef WB_VAR_LATENCY
				PMC_RWDS_CTRL	 = 0x00000018;
			#else
				PMC_RWDS_CTRL	 = 0x00000019;
			#endif
	     }

	   	 PMC_AP_SPECIAL	= 0x00000001;
	     PMC_TRP		= 0x00000022;
	     PMC_TRWR		= 0x00000003;//[7:0]tRWR=4

	     PMC_CTRL_REG 	= 0x00000001;
	     while(PMC_INITIAL_STATUS != 0x0f);

	}
	else if(3==idx)
	{
		//PRAMC controller initial 
//		PMC_DLL_LOCK_CTL = 0x2b2b2b05;
		PMC_CFG_REG      = 0x80000017;//relase reset and select pram type
		PMC_RE_INIT	  = 0x7;
		exception_delay(1000);  // 1000 is 50us 
		
		PMC_TPU          = 0x00007531;// tVCS for wb
		PMC_TRST         = 0x00000051;//tRPH for wb
		PMC_TCEM         = 0x00000313;//tCSM for wb
		PMC_THS          = 0x0000025e;// tTHS for wb
		PMC_TXPHS        = 0x00000010;// tXPHS for wb
		PMC_TXHS         = 0x00004e30;//txHS for wb
		PMC_TDPD         = 0x0000025e;//tDPDIN for wb
		PMC_TXPDPD       = 0x0000002c;//txPDPD for wb
		PMC_TXDPD        = 0x0000755c;//txDPD

		if(PSRAM_FRQ_166M==frq)
		{
			PMC_COMMAND_TIMING = 0x00040003;
			//[25:16]tCPH=tRWR=4=((36/6.024)-2)   
			PMC_RW_LATENCY   = 0x000c000e;
			//[21:16]WL=6*2=0xc   
			#ifdef PSRAM_WB_BURST64
		   		#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f15;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0001;[8,1,0]=101=64B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f1d;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0001;[8,1,0]=101=64B;[3]VL=1=Fixed Latency
				#endif
			#else
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f17;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0001;[8,1,0]=111=32B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT 	 = 0xff418f1f;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0001;[8,1,0]=111=32B;[3]VL=1=Fixed Latency
				#endif
			#endif
			PMC_TRWR		 = 0x00000004;//[7:0]tRWR=4
		}
		else if(PSRAM_FRQ_200M==frq)
		{
			PMC_COMMAND_TIMING = 0x00050003;
			//[25:16]tCPH=tRWR=5=((35/5)-2)
			PMC_RW_LATENCY   = 0x000e000e;
			//[21:16]WL=7*2=0xe
			#ifdef PSRAM_WB_BURST64
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f25;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0010;[8,1,0]=101=64B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f2d;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0010;[8,1,0]=101=64B;[3]VL=1=Fixed Latency
				#endif
			#else
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f27;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0010;[8,1,0]=111=32B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f2f;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0010;[8,1,0]=111=32B;[3]VL=1=Fixed Latency
				#endif
			#endif
			PMC_TRWR		 = 0x00000005;//[7:0]tRWR=5
		}
		else if(PSRAM_FRQ_230M==frq)
		{
			PMC_COMMAND_TIMING = 0x00070003;
			//[25:16]tCPH=tRWR=7=((35/4.3478)-2)  
			PMC_RW_LATENCY   = 0x0014000e;
			//[21:16]WL=10*2=0x14	
			#ifdef PSRAM_WB_BURST64
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f55;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0101;[8,1,0]=101=64B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f5d;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0101;[8,1,0]=101=64B;[3]VL=1=Fixed Latency
				#endif
			#else
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f57;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0101;[8,1,0]=111=32B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f5f;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0101;[8,1,0]=111=32B;[3]VL=1=Fixed Latency
				#endif
			#endif
			PMC_TRWR		 = 0x00000007;//[7:0]tRWR=7

		}
		else if(PSRAM_FRQ_250M==frq)
		{
			PMC_COMMAND_TIMING = 0x00070003;
			//[25:16]tCPH=tRWR=7=((35/4)-2)	
			PMC_RW_LATENCY	 = 0x0014000e;
			//[21:16]WL=10*2=0x14  
			#ifdef PSRAM_WB_BURST64
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f55;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0101;[8,1,0]=101=64B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f5d;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0101;[8,1,0]=101=64B;[3]VL=1=Fixed Latency			
				#endif
			#else
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f57;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0101;[8,1,0]=111=32B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f5f;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0101;[8,1,0]=111=32B;[3]VL=1=Fixed Latency
				#endif
			#endif
			PMC_TRWR		 = 0x00000007;//[7:0]tRWR=7

		}
		else	//default 200M 
		{
			PMC_COMMAND_TIMING = 0x00050003;
			//[25:16]tCPH=tRWR=5=((35/5)-2)
			PMC_RW_LATENCY   = 0x000e000e;
			//[21:16]WL=7*2=0xe
			#ifdef PSRAM_WB_BURST64
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f25;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0010;[8,1,0]=101=64B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f2d;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0010;[8,1,0]=101=64B;[3]VL=1=Fixed Latency
				#endif
			#else
				#ifdef WB_VAR_LATENCY
					PMC_MR_INIT = 0xff418f27;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0010;[8,1,0]=111=32B;[3]VL=0=Variable Latency
				#else
					PMC_MR_INIT = 0xff418f2f;//MR_INIT[7:4]=CR0[7:4]=InitialLatency=0010;[8,1,0]=111=32B;[3]VL=1=Fixed Latency
				#endif
			#endif
			PMC_TRWR		 = 0x00000005;//[7:0]tRWR=5
		}


		PMC_TCQLZ		 = 0x01050005;
		PMC_AP_SPECIAL	 = 0x000000d2;
		PMC_TRP 		 = 0x00000029;
		
		PMC_CTRL_REG	 = 0x00000001;//enable psram controller

		#ifdef WB_VAR_LATENCY
		PMC_RWDS_CTRL	 = 0x00000016;
		#else
		PMC_RWDS_CTRL	 = 0x00000017;
		#endif

		while(PMC_INITIAL_STATUS != 0x0f);
	}
	else 
	{
		
	}

};


#if (1==PSRAM_TEST_EN)
__except static void dma_cpy_unwait_start(void *dst, void *src, int cnt)
{
	MCP0CON = BIT(3);
	MCP0CON = BIT(0); 	//en
	
	MCP0SCADDR = (u32)src;
	MCP0TGADDR = (u32)dst;
	MCP0LENGTH = cnt-1;
}

__except static void dma_cpy_unwait_done()
{
	while((MCP0CON & BIT(1)) == 0);	 //wait for complete pending
	MCP0CON = 0;
}

#define PSRAM_TEST_DATA_LEN   0x1000
__except static void psram_test(void)
{
	u32 i,ret;
	u32 psram_addr=0x02000000;
	volatile u32 *ptr=(u32 *)psram_addr;
	volatile u16 *ptr16bit=(u8 *)psram_addr;
	volatile u8 *ptr8bit=(u8 *)psram_addr;

	ret=0;

	//diasable wdt
	WDTCON &=0xFF0FF;
	WDTCON |=0x00500;

	exception_putchar('o');
	exception_putchar('r');
	exception_putchar('d');
	exception_putchar('e');
	exception_putchar('r');
	exception_putchar(':');
	exception_putchar('\n');

	//==psram check 8bit==
	for(i=0;i<PSRAM_TEST_DATA_LEN;i++)
	{
		ptr8bit[i]=i;
		ptr8bit[i+PSRAM_TEST_DATA_LEN]=i;
	}

	for(i=0;i<PSRAM_TEST_DATA_LEN;i++)
	{
		if(ptr8bit[i]!=(u8)i)
		{
			ret=1;
			break;
		}

		if(ptr8bit[i+PSRAM_TEST_DATA_LEN]!=(u8)i)
		{
			ret=2;
			break;
		}
	}

	//==psram check 16bit==
	for(i=0;i<PSRAM_TEST_DATA_LEN/2;i++)
	{
		ptr16bit[i]=i;
		ptr16bit[i+PSRAM_TEST_DATA_LEN/2]=i;
	}

	for(i=0;i<PSRAM_TEST_DATA_LEN/2;i++)
	{
		if(ptr16bit[i]!=(u16)i)
		{
			ret=3;
			break;
		}

		if(ptr16bit[i+PSRAM_TEST_DATA_LEN/2]!=(u16)i)
		{
			ret=4;
			break;
		}
	}

	//==psram check 32bit==
	if(0==ret)
	{
		for(i=0;i<PSRAM_TEST_DATA_LEN/4;i++)
		{
			ptr[i]=i;
			ptr[i+PSRAM_TEST_DATA_LEN/4]=i;
		}

		for(i=0;i<PSRAM_TEST_DATA_LEN/4;i++)
		{
			if(ptr[i]!=i)
			{
				ret=5;
				break;
			}

			if(ptr[i+PSRAM_TEST_DATA_LEN/4]!=i)
			{
				ret=6;
				break;
			}
		}
	}

	if(ret)
	{
		exception_putchar('e');
		exception_putchar('r');
		exception_putchar('r');
		exception_putchar(':');
		exception_putchar('\n');
		exception_putword(ret);
		exception_putchar('\n');
		exception_putword(i);
		exception_putchar('\n');

		if(1==ret)
		{
			exception_putword(ptr8bit[i]);
		}
		else if(2==ret)
		{
			exception_putword(ptr8bit[i+PSRAM_TEST_DATA_LEN]);
		}
		else if(3==ret)
		{
			exception_putword(ptr16bit[i]);
		}
		else if(4==ret)
		{
			exception_putword(ptr16bit[i+PSRAM_TEST_DATA_LEN/2]);
		}
		else if(5==ret)
		{
			exception_putword(ptr[i]);
		}
		else if(6==ret)
		{
			exception_putword(ptr[i+PSRAM_TEST_DATA_LEN]);
		}

		exception_putchar('\n');
		while(1)
		{
			WTD_DIS();
		}
	}

	exception_putchar('b');
	exception_putchar('o');
	exception_putchar('t');
	exception_putchar('h');
	exception_putchar(':');
	exception_putchar('\n');

	//==init data==
	for(i=0;i<PSRAM_TEST_DATA_LEN/4;i++)
	{
		ptr8bit[i]=(u8)i;
	}

	//==dma & cpu both handle==
	dma_cpy_unwait_start(ptr8bit+PSRAM_TEST_DATA_LEN/4,ptr8bit,PSRAM_TEST_DATA_LEN/4);

	for(i=0;i<PSRAM_TEST_DATA_LEN/4;i++)
	{
		ptr8bit[i+PSRAM_TEST_DATA_LEN*3/4]=(u8)i;
	}

	dma_cpy_unwait_done();


	//==check data==
	for(i=0;i<PSRAM_TEST_DATA_LEN/4;i++)
	{
		if(ptr8bit[i+PSRAM_TEST_DATA_LEN*3/4]!=(u8)i)
		{
			ret=1;
			break;
		}
	}

	if(0==ret)
	{
		for(i=0;i<PSRAM_TEST_DATA_LEN/4;i++)
		{
			if(ptr8bit[i]!=(u8)i)
			{
				ret=2;
				break;
			}
		}
	}

	if(0==ret)
	{
		for(i=0;i<PSRAM_TEST_DATA_LEN/4;i++)
		{
			if(ptr8bit[i+PSRAM_TEST_DATA_LEN/4]!=(u8)i)
			{
				ret=3;
				break;
			}
		}
	}

	//==debug==
	if(ret)
	{
		exception_putchar('e');
		exception_putchar('r');
		exception_putchar('r');
		exception_putchar(':');
		exception_putchar('\n');
		exception_putword(ret);
		exception_putchar('\n');
		exception_putword(i);
		exception_putchar('\n');

		if(1==ret)
		{
			exception_putword(ptr8bit[i+PSRAM_TEST_DATA_LEN*3/4]);
		}
		else if(2==ret)
		{
			exception_putword(ptr8bit[i]);
		}
		else if(3==ret)
		{
			exception_putword(ptr8bit[i+PSRAM_TEST_DATA_LEN/4]);
			/*
			for(;i<PSRAM_TEST_DATA_LEN/4;i++)
			{
				exception_putword(ptr8bit[i+PSRAM_TEST_DATA_LEN/4]);
			}
			*/
		}

		exception_putchar('\n');
		while(1)
		{
			WTD_DIS();
		}
	}

}
#endif

__except void psram_io_drive()
{
   //SSEL[1:0] 1.8V  00=reserved 01=50ohm 10=35ohm 11=reserved(<35ohm ???)
   //DQ0
   PSRAMIOCON2 |=  BIT(14);//SSEL[1]
   PSRAMIOCON2 |=  BIT(0); //SSEL[0]
   PSRAMIOCON0 |=  BIT(14);//ST
   //DQ1
   PSRAMIOCON2 |=  BIT(15);//SSEL[1]
   PSRAMIOCON2 |=  BIT(1); //SSEL[0]
   PSRAMIOCON0 |=  BIT(15);//ST
   //DQ2
   PSRAMIOCON2 |=  BIT(16);//SSEL[1]
   PSRAMIOCON2 |=  BIT(2); //SSEL[0]
   PSRAMIOCON0 |=  BIT(16);//ST
   //DQ3
   PSRAMIOCON2 |=  BIT(17);//SSEL[1]
   PSRAMIOCON2 |=  BIT(3); //SSEL[0]
   PSRAMIOCON0 |=  BIT(17);//ST
   //DQ4
   PSRAMIOCON2 |=  BIT(18);//SSEL[1]
   PSRAMIOCON2 |=  BIT(4); //SSEL[0]
   PSRAMIOCON0 |=  BIT(18);//ST
   //DQ5
   PSRAMIOCON2 |=  BIT(19);//SSEL[1]
   PSRAMIOCON2 |=  BIT(5); //SSEL[0]
   PSRAMIOCON0 |=  BIT(19);//ST
   //DQ6
   PSRAMIOCON2 |=  BIT(20);//SSEL[1]
   PSRAMIOCON2 |=  BIT(6); //SSEL[0]
   PSRAMIOCON0 |=  BIT(20);//ST
   //DQ7
   PSRAMIOCON2 |=  BIT(21);//SSEL[1]
   PSRAMIOCON2 |=  BIT(7); //SSEL[0]
   PSRAMIOCON0 |=  BIT(21);//ST
   //DQS
   PSRAMIOCON2 |=  BIT(22);//SSEL[1]
   PSRAMIOCON2 |=  BIT(8); //SSEL[0]
   PSRAMIOCON0 |=  BIT(22);//ST
   //DM
   PSRAMIOCON2 |=  BIT(23);//SSEL[1]
   PSRAMIOCON2 |=  BIT(9);//SSEL[0]
   PSRAMIOCON0 |=  BIT(23);//ST
   //CSN
   PSRAMIOCON2 |=  BIT(24);//SSEL[1]
   PSRAMIOCON2 |=  BIT(10);//SSEL[0]
   PSRAMIOCON0 |=  BIT(24);//ST
   //CK
   PSRAMIOCON2 |=  BIT(25);//SSEL[1]
   PSRAMIOCON2 |=  BIT(11);//SSEL[0]
   PSRAMIOCON0 |=  BIT(25);//ST
   //CKN
   PSRAMIOCON2 |=  BIT(26);//SSEL[1]
   PSRAMIOCON2 |=  BIT(12);//SSEL[0]
   PSRAMIOCON0 |=  BIT(26);//ST
}
#define PSRAM_TUN_MAX    64
#define PSRAM_TUN_STEP   (PSRAM_TUN_MAX/32)
__except_data u32 tuneData[32];
__except void tuneDataInit(void)
{
	u8 i;
	for(i=0;i<32;i++)
		tuneData[i]=0;
}
__except void setTuneDataBit32(u8 x, u8 y)
{
	tuneData[x] = tuneData[x] | (1 << y);
}
__except void setTuneDataBit(u8 x, u8 y)
{
	setTuneDataBit32(x,y);
}
__except u8 getTuneDataBit32(u8 x, u8 y)
{
	if (tuneData[x] & (1 << y))
		return 1;
	return 0;
}

#define EX_REGION  0
#define EY_REGION  0
__except bool tuneDataReginMatch32(u8 x, u8 y, u8 regin)
{
	u8 xEnd, yEnd, i, j;
	xEnd = x + regin+EX_REGION;
	yEnd = y + regin+EY_REGION;
	if (getTuneDataBit32(xEnd-1, yEnd-1) == 0)
		return false;
	if (getTuneDataBit32(x, y) == 0)
		return false;
	if (getTuneDataBit32(x, yEnd-1) == 0)
		return false;
	if (getTuneDataBit32(xEnd-1, y) == 0)
		return false;
	
	for (i = x; i<xEnd; i++)
	{
		for (j = y; j<yEnd; j++)
		{
			if (getTuneDataBit32(i, j) == 0)
				return false;
		}
	}
	return true;
}

__except bool getTuneDataInfor(u8 *x, u8 *y, u8 regin)
{
	s8 x1, y1, xEnd, yEnd;
	xEnd = 32 - (regin+EX_REGION);
	yEnd = 32 - (regin+EY_REGION);
	
	for (y1 = 0; y1 <= yEnd; y1 += 1)
	{
		for (x1 = 0; x1 <= xEnd; x1 += 1)
		{
			if (tuneDataReginMatch32(x1, y1, regin))
			{
				*x = x1 + ((regin+EX_REGION) >> 1);
				*y = y1 + ((regin+EY_REGION) >> 1);
				return true;
			}
		}
	}
	return false;
}

__except int psram_rd_wr_test(u8 x,u8 y,u8 psram_size)
{
	#define TEST_DATA 0x5a5aa5a5
	#define TEST_LEN (1024*8)
	#define TEST_STEP_4MB (0x400000/TEST_LEN/4)
	#define TEST_STEP_8MB (0x800000/TEST_LEN/4)
	#define TEST_ADDR 0x02000000		// psram addr 
	volatile u32 *ptr=(u32 *)TEST_ADDR;
	int i;
	u32 data=TEST_DATA+x+(y<<5);
	if(4==psram_size)
	{
		for(i=0;i<TEST_LEN;i++)
		{
			*ptr=data;
			ptr+=TEST_STEP_4MB;
		}
		ptr=(u32 *)TEST_ADDR;
		for(i=0;i<TEST_LEN;i++)
		{
			if(*ptr!=data)
				return -1;
			ptr+=TEST_STEP_4MB;
		}
	}
	else
	{
		for(i=0;i<TEST_LEN;i++)
		{
			*ptr=data;
			ptr+=TEST_STEP_8MB;
		}
		ptr=(u32 *)TEST_ADDR;
		for(i=0;i<TEST_LEN;i++)
		{
			if(*ptr!=data)
				return -1;
			ptr+=TEST_STEP_8MB;
		}
	}
	return 0;
}
__except void psram_tunning(u8 psram_size)
{
	s16 x, y,min,max;
	u8 bx, by;
	x=(PMC_DLL_DELAY_CNT>>0)&0xff;
	y=(PMC_DLL_DELAY_CNT>>8)&0xff;
	exception_putchar('P');
	exception_putchar('S');
	exception_putchar('R');
	exception_putchar('M');
	exception_putchar(' ');
	exception_putchar('T');
	exception_putchar('U');
	exception_putchar('N');
	exception_putchar(',');
	exception_putchar('x');
	exception_putchar(':');
	exception_putword(x);
	exception_putchar(',');
	exception_putchar('y');
	exception_putchar(':');
	exception_putword(y);
	exception_putchar('\n');
	
	for(min=x;min>=0;min--)
	{
		PMC_DELAY_CTRL = (PMC_DELAY_CTRL & ~((0xff<<1)|(0xff<<11))) | ((min)<<1) | ((y)<<11)|(1<<10)|(1<<0);	
		exception_delay(5);		

		WTD_DIS();
		if(0!=psram_rd_wr_test(min,y,psram_size))
		{
			min++;
			break;
		}
	}

	for(max=x;max<255;max++)
	{
		PMC_DELAY_CTRL = (PMC_DELAY_CTRL & ~((0xff<<1)|(0xff<<11))) | ((max)<<1) | ((y)<<11)|(1<<10)|(1<<0);	
		exception_delay(5);		
		WTD_DIS();
		if(0!=psram_rd_wr_test(max,y,psram_size))
		{
			max--;
			break;
		}
	}

	bx=(min+max)>>1;
	for(min=y;min>=0;min--)
	{
		PMC_DELAY_CTRL = (PMC_DELAY_CTRL & ~((0xff<<1)|(0xff<<11))) | ((bx)<<1) | ((min)<<11)|(1<<10)|(1<<0);	
		exception_delay(5);		
		WTD_DIS();
		if(0!=psram_rd_wr_test(bx,min,psram_size))
		{
			min++;
			break;
		}
	}

	for(max=y;max<255;max++)
	{
		PMC_DELAY_CTRL = (PMC_DELAY_CTRL & ~((0xff<<1)|(0xff<<11))) | ((bx)<<1) | ((max)<<11)|(1<<10)|(1<<0);	
		exception_delay(5);		
		WTD_DIS();
		if(0!=psram_rd_wr_test(bx,max,psram_size))
		{
			max--;
			break;
		}
	}

	by=(min+max)>>1;
	PMC_DELAY_CTRL = (PMC_DELAY_CTRL & ~((0xff<<1)|(0xff<<11))) | ((bx)<<1) | ((by)<<11)|(1<<10)|(1<<0);	
	exception_delay(5);
	exception_putchar('T');
	exception_putchar('U');
	exception_putchar('N');
	exception_putchar(':');
	exception_putword(bx);
	exception_putchar(',');
	exception_putword(by);
	exception_putchar('\n');
}

__except void psram_init(void)
{
	u8 psram_size=8;
	//uart
#if AX32_PLATFORM == FPGA
	PMAPCON0 = (PMAPCON0 & ~0x3ff)|(9<<5)|(6<<0);
	PBFMAP	 |= BIT(4)|BIT(0);
	PBDIR	 |= BIT(4);
	PBDIR	 &= ~BIT(0);
#elif AX32_PLATFORM == AX3292  //uart tx pe1
	PMAPCON0 = (PMAPCON0 & ~0x3ff)|(24<<5);
	PEFMAP	 |= BIT(1);
	PEDIR	 &= ~BIT(1);
#else   //uart tx pa7
	PMAPCON0 = (PMAPCON0 & ~0x3ff)|(6<<5);
	PAFMAP	 |= BIT(7);
	PADIR	 &= ~BIT(7);
#endif
	UART0BAUD = APB_CLK / UART_BAUDRATE -1;   //baud rate
	UART0PND |= BIT(5)|BIT(4);
	UART0CON |= BIT(4);

	//==enable core ldo==
	LDOCON = (LDOCON & ~(0x03<<13))|((SYS_VOL_V1_25&0x03)<<13);  //LDO 1.2V; 0:1.0V, 1: 1.2V, 2: 1.25V, 3: 1.3V
	exception_delay(20);
	//==enable psram ldo==
	LDOCON = (LDOCON & ~(0x1F<<23))|((SYS_VOL_V2_0&0x1F)<<23);	// LDO 1.8V 
	LDOCON |= BIT(22);
	exception_delay(500);	// need > 1MS , cpu is 2M , 1.5MS 

	WTD_DIS();
	psram_clk_init();	// after here , cpu is 144M 


	//==vbg trim==
	u8 vbg_trim=(((*(u32 *)(0x5000-140+0x30 + 8))>>24)& 0x1f);
	if(vbg_trim != 0){
		LDOCON = (LDOCON & ~(0x1f<<16))|((vbg_trim)<<16);
	}	
	u8 vbg_ofs = (*(u32 *)(0x5000-140+0x30 + 4) & 0x7f);
	if(vbg_ofs != 0){
		vbg_param = vbg_ofs +737;
	}
	else{
		vbg_param =800;
	}

	psram_io_drive();

	//====find psram type====
	s32 i;
	u32 psram_vid,psram_did;
	for(i=0;i<PSRAM_TYPE_NUM_MAX;i++)
	{
		WTD_DIS();

		PRAMCCON0 &= ~BIT(0);// pramc soft reset 
		exception_delay(100);	// 1000 is 50us 

		PRAMCCON0 |= BIT(0);// release pramc soft reset 
		exception_delay(1000);	// 1000 is 50us 
		
		u32 cnt = 0;
		u32 r_dat = 0;
		u32 wdll_delay_init;
		u32 rdll0_delay_init;
		u32 rdll1_delay_init;
		while(((r_dat & 0x80808000)!= 0x00000000) || cnt==0)
		{
		  r_dat = PMC_DLL_LOCK_CTL;
		  while((r_dat & 0x80808000)!= 0x00000000)
		  {
			  if((r_dat & 0x00008000) == 0x00008000)
				{wdll_delay_init = ((r_dat>>8)&0x7) + 1;}
			  else
				{wdll_delay_init = ((r_dat>>8)&0x7);}
			  if((r_dat & 0x00800000) == 0x00800000)
				{rdll0_delay_init = ((r_dat>>16)&0x7) + 1;}
			  else
				{rdll0_delay_init = ((r_dat>>16)&0x7);}
			  if((r_dat & 0x80000000) == 0x80000000)
				{rdll1_delay_init = ((r_dat>>24)&0x7) + 1;}
			  else
				{rdll1_delay_init = ((r_dat>>24)&0x7);}
			  PMC_DLL_LOCK_CTL = (((r_dat>>27)&0x1f)<<27) | 
								 (rdll1_delay_init<<24)   |
								 (((r_dat>>19)&0x1f)<<19) |
								 (rdll0_delay_init<<16)   |
								 (((r_dat>>11)&0x1f)<<11) |
								 (wdll_delay_init<<8)	  |
								 (((r_dat>>0)&0xff)<<0)   ;
			  r_dat = PMC_DLL_LOCK_CTL;
			  r_dat = PMC_DLL_LOCK_CTL;
		  }
		  exception_delay(10000);
		  r_dat = PMC_DLL_LOCK_CTL;
		  if((r_dat & 0x80808000)!= 0x00000000)
		  {
			  if((r_dat & 0x00008000) == 0x00008000)
				{wdll_delay_init = ((r_dat>>8)&0x7) + 1;}
			  else
				{wdll_delay_init = ((r_dat>>8)&0x7);}
			  if((r_dat & 0x00800000) == 0x00800000)
				{rdll0_delay_init = ((r_dat>>16)&0x7) + 1;}
			  else
				{rdll0_delay_init = ((r_dat>>16)&0x7);}
			  if((r_dat & 0x80000000) == 0x80000000)
				{rdll1_delay_init = ((r_dat>>24)&0x7) + 1;}
			  else
				{rdll1_delay_init = ((r_dat>>24)&0x7);}
			  PMC_DLL_LOCK_CTL = (((r_dat>>27)&0x1f)<<27) | 
								 (rdll1_delay_init<<24)   |
								 (((r_dat>>19)&0x1f)<<19) |
								 (rdll0_delay_init<<16)   |
								 (((r_dat>>11)&0x1f)<<11) |
								 (wdll_delay_init<<8)	  |
								 (((r_dat>>0)&0xff)<<0)   ;
			  r_dat = PMC_DLL_LOCK_CTL;
		  }
		  cnt = 1;
		}


		exception_putchar('P');
		exception_putchar('S');
		exception_putchar('R');
		exception_putchar('M');
		exception_putchar(':');
		exception_putchar('\n');
		exception_putword(i);
		exception_putchar('\n');
		psram_type_clk_cfg(i,PSRAM_FRQ_SEL);

		psram_vid=PMC_MR0;
		exception_putword(psram_vid);
		exception_putchar('\n');
		
		psram_vid=PMC_MR1;
		exception_putword(psram_vid);
		exception_putchar('\n');

		psram_did=PMC_MR2;
		exception_putword(psram_did);
		exception_putchar('\n');

		if(0==i)
		{
			//==0:APS3208K_OKUD:4MB==
			if(((0xC98d&0x1F)==(psram_vid&0x1F))&&((0x00C9&0x18)==(psram_did&0x18)))
			{
				break;
			}
		}
		else if(1==i)
		{
			//==1:APS6408L_OBX:8MB==
			if(((0x938d&0x1F)==(psram_vid&0x1F))&&((0xA093&0x18)==(psram_did&0x18)))
			{
				break;
			}
		}
		else if(2==i)
		{
			//==2:W955D8MBKX:4MB==
			
		}
		else if(3==i)
		{
			//==3:W956D8NKR:8MB==
			
		}
	}

	if(i==PSRAM_TYPE_NUM_MAX)
	{
		exception_putchar('e');
		exception_putchar('r');
		exception_putchar('r');
		exception_putchar('\n');
	}
	else
	{
		if((0==i)||(2==i))	// 4MB psram
		{
			psram_size=4;
			exception_putchar('4');
			exception_putchar('M');
			exception_putchar('B');
			exception_putchar('\n');
		}
		else
		{
			psram_size=8;
			exception_putchar('8');
			exception_putchar('M');
			exception_putchar('B');
			exception_putchar('\n');
		}
		exception_putchar('p');
		exception_putchar('a');
		exception_putchar('s');
		exception_putchar('s');
		exception_putchar('\n');
	}
	//====end find psram type====

//	psram_tunning(psram_size);
	
	#if (1==PSRAM_TEST_EN)
	psram_test();
	#endif


#if PSRAM_INT_ENABLE
	PMC_INTR_CTRL &= ~(BIT(13)|BIT(14)); // enalbe memory read timeout int
#endif

}
#endif

__except void exception(u32 except_id)
{
    //CLEAN WTD
    WDTCON = (5<<16)|(5<<0);
    
    exception_uart_puts(str1);
    str3[3] = (u8)except_id;
	exception_uart_puts(str3);
	exception_uart_puts(str3);
	exception_uart_puts(str3);	
    if(0x30 == (except_id&0xff) )
		exception_uart_puts(str5);
	//检测VDDRTC
    int i = 3;
    u8 rtc0 = 0;
    u32 Abg = 0,Artc = 0,Vrtc = 0;
    
    rtc0 = exception_irtc_read(IRTCON0_RD_CMD);
    if((rtc0 & (1<<5)) == 0)
        exception_irtc_write(IRTCON0_WR_CMD,rtc0|(1<<5));
    
    PCON0   &=~(1<<29);
    ADCCON  = (1<<4)|(9);
    ADCBAUD = (APB_CLK/(2*1000000) - 1) & 0x7F;
    
    while(i--)
    {
        ADCCON  = (1<<4)|(9);
        exception_delay(10);
        ADCCON |= (1<<7);
        while(ADCCON & (1<<6));
        Artc += ADCDAT & 0x3ff;
        
        ADCCON  = (1<<4)|(11);
        exception_delay(10);
        ADCCON |= (1<<7);
        while(ADCCON & (1<<6));
        Abg += ADCDAT & 0x3ff;
    }

    Vrtc = ax32xx_vddrtcCalculate(Artc,Abg);
    
    //u8 temp[6];
    //temp[0] = '0' + Vrtc/1000%10;
    //temp[1] = '0' + Vrtc/100%10;
    //temp[2] = '0' + Vrtc/10%10;
    //temp[3] = '0' + Vrtc%10;
    //temp[4] = ',';
    //temp[5] = 0;
    //exception_uart_puts((const char *)temp);
    
    if(Vrtc > 3700)
    {
        goto RESET;
    }
	exception_uart_puts(str4);	
    //如果VDDRTC过低
    //关闭时钟
    PCON0 |= (0xfff << 20)|(0xff << 9);
    
    //关闭后录电源
    PEFMAP &=~(1<<1);
    PEDIR  &=~(1<<1);
    PORTE  &=~(1<<1);
    
    //关闭LCD背光
    PAFMAP &=~(1<<8);
    PADIR  &=~(1<<8);
    PORTA  &=~(1<<8);
    
    //关闭SENSOR电源
   // LDOCON &=~((1<<15)|(1<<7)|(1<<3));
     LDOCON &=~((1<<21)|(1<<12)|(1<<5));
    //关闭电源
    while(1)
    {
        PAFMAP &=~(3<<6);
        PADIR  &=~(3<<6);
        PORTA  &=~(3<<6); //gsensor i2c output low
        
        //close gsensor power
        //exception_irtc_write(IRTCON0_WR_CMD,(1<<4)|(1<<1));//寄存器值修改，IRTCON4 bit5?
        exception_irtc_write(IRTCON5_WR_CMD,(7<<5));//clean wko/wki0/wki1 weakup pending
        exception_irtc_write(IRTCON4_WR_CMD,(1<<1));//wko output 0
    }
RESET:
	while(1);
    //看门狗复位
    WDTCON = 0;
    while(1)
    {
        exception_uart_puts(str2);
    }
}

//触发exception
__except void exception_trigger(void)
{
    asm(".ascii \"01234\"");
}

void (*fp_usb_isr)(void);
void (*fp_usb11_isr)(void);

/*******************************************************************************
* Function Name  : ax32xx_intHandler
* Description    : int handler,default handler
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_intHandler(void)
{
	
}
/*******************************************************************************
* Function Name  : fast_isr
* Description    : isr,cpu vector
* Input          : none
* Output         : None
* Return         : 
*******************************************************************************/
void fast_isr(void)
{
	switch((mfspr(SPR_PICVR) >> 16)& 0x1f)
	{
		case IRQ_CSI    : ax32xx_csiIRQHandler();break;
		case IRQ_AUADC  : ax32xx_auadcIRQHandler();break;
		case IRQ_JPGB   : ax32xx_mjpegBIRQHandler();break;
		case IRQ_UART0  : ax32xx_uart0IRQHandler();break;
		case IRQ_BTUART  : ax32xx_uart1IRQHandler();break;
		case IRQ_LCDC   : ax32xx_lcdIRQHandler();break;	
		case IRQ_DE     : ax32xx_deIRQHandler();break;	
		case IRQ_EMI    : ax32xx_emiIRQHandler();break;	
		case IRQ_DAC    : ax32xx_dacIRQHandler();break;	
		case IRQ_TIMER0 : ax32xx_timer0IRQHandler();break;
		case IRQ_TIMER1 : ax32xx_timer1IRQHandler();break;
		case IRQ_TIMER2 : ax32xx_timer2IRQHandler();break;
		case IRQ_TIMER3 : ax32xx_timer3IRQHandler();break;
		case IRQ_GPIO   : ax32xx_gpioIRQHandler();break;	
		case IRQ_USB20  : (*fp_usb_isr)();break;	
		case IRQ_RTC_WDT: ax32xx_rtcIRQHandler();break;	
		case IRQ_USB11  : (*fp_usb11_isr)();break;	
		default:          ax32xx_intHandler();break;
	}
}
/*******************************************************************************
* Function Name  : slow_isr
* Description    : isr ,cpu vector
* Input          : none
* Output         : none
* Return         : none
*******************************************************************************/
void slow_isr(void)
{
	switch(mfspr(SPR_PICVR)& 0x01f)
	{
		case IRQ_TIMER0 : ax32xx_timer0IRQHandler();break;
		case IRQ_TIMER1 : ax32xx_timer1IRQHandler();break;
		case IRQ_TIMER2 : ax32xx_timer2IRQHandler();break;
		case IRQ_TIMER3 : ax32xx_timer3IRQHandler();break;
		case IRQ_LCDC   : ax32xx_lcdIRQHandler();break;	
		case IRQ_RTC_WDT: ax32xx_rtcIRQHandler();break;	
		case IRQ_DAC    : ax32xx_dacIRQHandler();break;	
		case IRQ_GPIO   : ax32xx_gpioIRQHandler();break;	
		case IRQ_UART0  : ax32xx_uart0IRQHandler();break;
		case IRQ_CSI    : ax32xx_csiIRQHandler();break;
        case IRQ_USB20  : (*fp_usb_isr)();break;
		case IRQ_JPGA	: ax32xx_mjpegAIRQHandler();break;
		case IRQ_AUADC  : ax32xx_auadcIRQHandler();break;
		case IRQ_DE     : ax32xx_deIRQHandler();break;	
		case IRQ_JPGB   : ax32xx_mjpegBIRQHandler();break;
        case IRQ_EMI    : ax32xx_emiIRQHandler();break;
        case IRQ_ROTATE : ax32xx_rotateIRQHandler();break;
        case IRQ_OSDCMP : ax32xx_osdcmpIRQHandler();break;
        case IRQ_USB11  : (*fp_usb11_isr)();break;
        case IRQ_BTUART :ax32xx_uart1IRQHandler();break;
        case IRQ_SPI0   :
        case IRQ_SDC0   :
        case IRQ_SDC1   :
        case IRQ_SPI1   :
        case IRQ_LVD    :
        case IRQ_IIC0_IIC1 :
        case IRQ_MCP0   :
       // case IRQ_BTUART :
        case IRQ_MCP1   :
        case IRQ_MCP2   :
        case IRQ_PMU    :
        case IRQ_USB11MCP :
        case IRQ_TT_MIPI:
		default:          ax32xx_intHandler();break;
	}
}
/*******************************************************************************
* Function Name  : ax32xx_intEnable
* Description    : int enable set
* Input          : u8 irqNum : irq index,in IRQ_IDX_E
                   u8 en     : 1->enable,0->disable
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_intEnable(u8 irqNum,u8 en)
{
    if(en)
		__sprset(SPR_PICMR,irqNum);
	else
		__sprclr(SPR_PICMR,irqNum);
}

/*******************************************************************************
* Function Name  : ax32xx_int_priority
* Description    : int enable set
* Input          : u8 irqNum : irq index,in IRQ_IDX_E
                   u8 level     : 1->high priority,0->low priority
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_int_priority(u8 irqNum,u8 level)
{
    if(level)
		__sprset(SPR_PICPR,irqNum);
	else
		__sprclr(SPR_PICPR,irqNum);
}


/*******************************************************************************
* Function Name  : ax32xx_intInit
* Description    : int initial
* Input          : none
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_intInit(void)
{
	ax32xx_intEnable(IRQ_TIMER0,0);
	ax32xx_intEnable(IRQ_CSI,0);	
	ax32xx_intEnable(IRQ_AUADC,0);	
	ax32xx_intEnable(IRQ_JPGB,0);	
	ax32xx_intEnable(IRQ_UART0,0);	
	ax32xx_intEnable(IRQ_LCDC,0);	
	ax32xx_intEnable(IRQ_DE,0);	
	ax32xx_intEnable(IRQ_DAC,0);	
	ax32xx_intEnable(IRQ_TIMER0,0);	
	ax32xx_intEnable(IRQ_TIMER1,0);	
	ax32xx_intEnable(IRQ_TIMER2,0);	
	ax32xx_intEnable(IRQ_TIMER3,0);	
	ax32xx_intEnable(IRQ_GPIO,0);	
	ax32xx_intEnable(IRQ_USB20,0);	
	ax32xx_intEnable(IRQ_USB11,0);	
	ax32xx_intEnable(IRQ_RTC_WDT,0);	
	SPR_PICCR |= BIT(1); //HPSEN use hpr need HPSEN
}

