/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                  THE APPOTECH HAL
   **** **                 UART HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_uart.c
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN UART HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "../inc/hal.h"
#include "application.h"
#include "../../device/printer/printer.h"

static void (*hal_uartCallback)(u8 data);
static void (*hal_uart1Callback)(u8 data);

static u8 halUartShareFlag=0;
extern void ax32xx_uart1IOCfg(u8 en);

extern u8 tfdebg_buf[16*1024];
extern u32 tfdebg_num;
extern void logChar(char c);
void hal_uartIOShareCheck(void);

#if 1//def USE_BT_CONTROL_LED
static u8 tmpledoff[4] = {0x01,0x52,0x01,0x00};
static u8 tmpledon[4]  = {0x01,0x52,0x01,0x01};
u8 Bt_Control_Led_OnOff(u8 onoff)
{
#if 1
	int i;
	hal_uartIOShareCheck();
	
	if(onoff)
	{
		deg_Printf("led onm1111\n");
		for(i=0;i<4;i++)
		{
			hal_uartSendData(tmpledon[i]);
		}
		/*hal_uartSendData(0x01);
		hal_uartSendData(0x52);
		hal_uartSendData(0x01);
		hal_uartSendData(0x01);*/
	}
	else
	{	
		deg_Printf("led offm1111\n");
		for(i=0;i<4;i++)
		{
			hal_uartSendData(tmpledoff[i]);
		}
		/*hal_uartSendData(0x01);
		hal_uartSendData(0x52);
		hal_uartSendData(0x01);
		hal_uartSendData(0x00);*/
	}
#endif
	return 0;
}
#endif

/*******************************************************************************
* Function Name  : hal_uartIRQHandler
* Description    : hal layer.uart rx irq handler
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
static void hal_uartIRQHandler(u8 data)
{
	if(hal_uartCallback)
		hal_uartCallback(data);
}
/*******************************************************************************
* Function Name  : hal_BtuartIRQHandler
* Description    : hal layer.uart rx irq handler
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uart1IRQHandler(u8 data)
{
	if(hal_uart1Callback)
	{
		hal_uart1Callback(data);
	}

	//hal_btuartSendData(data);
}


/*******************************************************************************
* Function Name  : hal_uartIOShare
* Description    : hal layer.uart io share flag
* Input          : none
* Output         : None
* Return         : u8 : 0->current is uart using,1->other using
*******************************************************************************/
u8 hal_uartIOShare(void)
{
	u8 temp;
	
#if HAL_CFG_UART == UART0	
	ax32xx_uart0IOCfg(0);
#else

#endif

	temp = halUartShareFlag;

	halUartShareFlag = 1;

	return temp;
}
/*******************************************************************************
* Function Name  : hal_uartIOShareCheck
* Description    : hal layer.uart io share flag check
* Input          : none
* Output         : None
* Return         : 
*******************************************************************************/
void hal_uartIOShareCheck(void)
{
	if(halUartShareFlag)
	{
	#if HAL_CFG_UART == UART0	
		ax32xx_uart0IOCfg(1);
	    halUartShareFlag = 0;
	#else

	#endif
		
	}
}
/*******************************************************************************
* Function Name  : hal_uartInit
* Description    : hal layer.uart initial
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uartInit(void)
{
#if HAL_CFG_UART == UART0
    ax32xx_uart0IOCfg(1);
	ax32xx_uart0Init(HAL_CFG_UART0_BAUDRATE,hal_uartIRQHandler);
	halUartShareFlag = 0;
	uart_Printf("init uart0 finish \n");
#else

#endif
}

void hal_uart1SendData(u8 data)
{
	ax32xx_uart1SendByte(data);
}

/*******************************************************************************
* Function Name  : hal_uartSendData
* Description    : hal layer.uart  send data
* Input          : u8 data : data
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uartSendData(u8 data)
{
#if (HAL_CFG_UART == UART0)//	&& CFG_MCU_PTDBG_EN
	ax32xx_uart0SendByte(data);
#endif	
#if CFG_MCU_TFDBG_EN
	tfdebg_buf[tfdebg_num] = data;
	tfdebg_num = (tfdebg_num<sizeof(tfdebg_buf)-1)?(tfdebg_num+1):0;
#endif
}
/*******************************************************************************
* Function Name  : hal_uartRXIsrRegister
* Description    : hal layer.uart  rx ISR register
* Input          : void (*isr)(u8 data) : isr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uartRXIsrRegister(void (*isr)(u8 data))
{
	hal_uartCallback = isr;
}

/*******************************************************************************
* Function Name  : hal_BTuartRXIsrRegister
* Description    : hal layer.uart  rx ISR register
* Input          : void (*isr)(u8 data) : isr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uart1RXIsrRegister(void (*isr)(u8 data))
{

	hal_uart1Callback = isr;
}

/*******************************************************************************
* Function Name  : uart_PutChar_n
* Description    : '\n' denote newline
* Input          : c:character
* Output         : None
* Return         : None
*******************************************************************************/
void uart_PutChar_n(u8 c)
{	
	
   	if('\n' == c)
	{
		hal_uartSendData(0x0D);
		hal_uartSendData(0x0A);
	}
	else
	{
       	 	hal_uartSendData(c);
	}
}

/*******************************************************************************
* Function Name  : uart_PutStr
* Description    : uart output strings
* Input          : p_str:strings pointer
* Output         : None
* Return         : None
*******************************************************************************/
void uart_PutStr(u8 *p_str)     
{
    while(*p_str)
        uart_PutChar_n(*p_str++);
}

/*******************************************************************************
* Function Name  : uart_Put_hex
* Description    : uart output  use hex number
* Input          : dwHex: unsigned  long number
*                  bMode: 0: small letter, 1:capital letter
* Output         : None
* Return         : None
*******************************************************************************/
void uart_Put_hex(DWORD dwHex, BOOL bMode)
{
    BYTE HexTable[16] = {'0','1','2','3','4','5','6','7','8','9',bMode?'A':'a'};
    BYTE aHex[8] = {0};
    int i;

    for (i=11; i<16; i++)
    {
        HexTable[i] = HexTable[i-1] + 1;
    }

    i = 8;
    do
    {
        aHex[--i] = dwHex & 0xf;
        dwHex >>= 4;
    } while (dwHex);

    while (i < 8)
    {
        hal_uartSendData(HexTable[aHex[i++]]);
    }
}

/*******************************************************************************
* Function Name  : uart_Put_udec
* Description    : uart output  use unsigned decimal number
* Input          : dwDec:  unsigned  long number
* Output         : None
* Return         : None
*******************************************************************************/
void uart_Put_udec(DWORD dwDec)
{
    BYTE aDec[10] = {0};
    int i = 10;

    do
    {
       aDec[--i] = '0' + dwDec % 10;
       dwDec /= 10;
    } while (dwDec);

    while (i < 10)
    {
       hal_uartSendData(aDec[i++]);
    }
}

/*******************************************************************************
* Function Name  : uart_Put_dec
* Description    : uart output  use signed decimal number
* Input          : dwDec:  signed  long number
* Output         : None
* Return         : None
*******************************************************************************/
void uart_Put_dec(long lDec)
{
    BYTE aDec[10] = {0};
    int i = 10;

    if (lDec < 0)
    {
        hal_uartSendData('-');
        lDec = ~(lDec - 1);
    }

    do
    {
       aDec[--i] = '0' + lDec % 10;
       lDec /= 10;
    } while (lDec);

    while (i < 10)
    {
       hal_uartSendData(aDec[i++]);
    }
}

/*******************************************************************************
* Function Name  : uart_Printf
* Description    : uart output character,type:
*                  %d,%i:signed long decimal number
*                   %u  :unsigned long decimal number
*                   %x  :unsigned  long hex number(small letter)
*                   %X  :unsigned  long hex number(capital letter)
*                   %c  :character
*                   %s  :character string
* Input          :  *pszStr: char type pointer
* Output         : None
* Return         : None
*******************************************************************************/
void uart_Printf(const char *pszStr, ...)
{
    va_list arglist;
	va_start(arglist, pszStr);

	hal_uartIOShareCheck();

//	return;

	//if((bI2CBusy_Flag == 0)&&(bSFBusy_Flag == 0))
	{
		 while ('\0' != *pszStr)
		 {
			 if ('%' != *pszStr)
			 {
				  uart_PutChar_n(*pszStr);
			 }
			 else
			 {
				  switch (*(++pszStr))
				  {
				  case '\0':
					   uart_PutChar_n('%');
					   pszStr--;
					   break;

				  case 'd':
				  case 'i':
					   uart_Put_dec(va_arg(arglist, long));
					   break;

				  case 'u':
					   uart_Put_udec(va_arg(arglist, DWORD));
					   break;

				  case 'x':
					   uart_Put_hex(va_arg(arglist, DWORD), FALSE);
					   break;

				  case 'X':
					   uart_Put_hex(va_arg(arglist, DWORD), TRUE);
					   break;

				  case 'c':
					   uart_PutChar_n(va_arg(arglist, int));
					   break;

				  case 's':
					   uart_PutStr(va_arg(arglist, BYTE *));
					   break;

				  default:                                                        
					   uart_PutChar_n('@');
					   break;
				  }
			 }
			 pszStr++;
		 }
	}
}

/*******************************************************************************
* Function Name  : uart_PrintfBuf
* Description    : uart output character use hex number(capital letter)
* Input          : *pBuf   :output character pointer
*                  iDataLen:character length
* Output         : None
* Return         : None
*******************************************************************************/
void uart_PrintfBuf(void *pBuf, int iDataLen)
{
	int i;
	u8 *pTempBuf = (BYTE *)pBuf;
	//uart_Printf("pBuffAddr = 0x%X", (int)pBuf);
	for (i=0; i<iDataLen; i++)
	{
		if (0 == i%32)
		{
			uart_PutChar_n('\n');
		}
		if(pTempBuf[i] < 0x10)
			uart_PutChar_n('0');
		uart_Put_hex(pTempBuf[i],1);
		uart_PutChar_n(' ');
	}
	uart_PutChar_n('\n');
}
/*******************************************************************************
* Function Name  : hal_uartPrintString
* Description    : uart printf string
* Input          : 
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uartPrintString(char *string)
{
	while(*string)
	{
		ax32xx_uart0SendByte(*string);
		string++;
	}
}

static u8 recvbuf[80];
static int recv_len = 0;
static void rxRecv(u8 data)
{
	if (recv_len >= sizeof(recvbuf) - 1) {
		recv_len = 0;
	}
	recvbuf[recv_len++] = data;
	recvbuf[recv_len] = 0;
}
void uart1Test(void)
{
	static bool inited = false;
	static uint32 lastOut = 0;

	// init
	if (inited == false)
	{
		//==PB5 PB6 handle clean USB setting==
#if 0
		USB11CON0=0;
		USB11CON1=0;
		USB11CON0 |= (1<<6);//control by soft
		USB11CON1 &= ~(BIT(4)|BIT(6));	//disable dp,dm 120K pullup
		USB11CON1 &= ~(BIT(7)|BIT(5));	//disable dp,dm 15k pulldown
#endif
		//==endPB5  PB6 handle clean USB setting==
		ax32xx_uart1IOCfg(1);
		ax32xx_uart1Init(UART1_BAUDRATE,hal_uart1IRQHandler);
		hal_uart1RXIsrRegister(rxRecv);
		inited = true;
	}

	// send every 5s
	uint32 tick = XOSTimeGet();
	if (tick - lastOut >= 2000) {
		HAL_RTC_T *time = hal_rtcTimeGet();
		char *strTime = hal_rtcTime2String(time);
		deg_Printf("SEND:%s\n",strTime);

		int i;
		for (i = 0; i < strlen(strTime); i++) {
			hal_uart1SendData(strTime[i]);
		}
		hal_uart1SendData('\r');
		hal_uart1SendData('\n');

		lastOut = tick;
	}

	// print received message
	if (recv_len > 0 && recvbuf[recv_len - 1] == '|')
	{
		deg_Printf("RECV:%s\n",recvbuf);
		recv_len = 0;
	}
}

void hal_uart1Init(void)
{
	ax32xx_uart1IOCfg(1);
	ax32xx_uart1Init(UART1_BAUDRATE,hal_uart1IRQHandler);
}


