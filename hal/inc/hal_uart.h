/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         UART HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_uart.h
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

#ifndef HAL_UART_H
#define HAL_UART0_H


void uart_PutChar_n(u8 c);
void uart_PutStr(u8 *p_str) ;
void uart_Put_hex(DWORD dwHex, BOOL bMode);
void uart_Put_udec(DWORD dwDec);
void uart_Put_dec(long lDec);
void uart_Printf(const char *pszStr, ...);
void uart_PrintfBuf(void *pBuf, int iDataLen);


#if HAL_CFG_EN_DBG
#define deg(...)              uart_Printf(__VA_ARGS__)
#define deg_Printf(...)       uart_Printf(__VA_ARGS__)
#define deg_PrintfBuf(p,i)    uart_PrintfBuf(p,i)
#define my_printf(...)		  uart_Printf(__VA_ARGS__)
#define debgbuf(p,i)          uart_PrintfBuf(p,i)
#define debgchar(c)		      uart_PutChar_n(c)
#define deg_Register(x)	      uart_Printf(#x"=%X\n",x)
#define debgreg(x)			  uart_Printf(#x"=%X\n",x)
#define MARK()				  uart_Printf("%d\n",__LINE__)
#define ENTER()			      uart_Printf("[ENTER] %s\n",__func__)
#define LEAVE()			      uart_Printf("[EXIT] %s\n",__func__)

#else
#define  deg(...)             
#define  deg_Printf(...)      
#define  deg_PrintfBuf(p,i)   
#define  my_printf(...)		   
#define  debgbuf(p,i)         
#define  debgchar(c)			  
#define  deg_Register(x)		
#define  debgreg(x)			   
#define  MARK()				
#define  ENTER()				  
#define  LEAVE()				 

#endif


/*******************************************************************************
* Function Name  : hal_uartInit
* Description    : hal layer.uart initial
* Input          : none
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uartInit(void);
/*******************************************************************************
* Function Name  : hal_uartSendData
* Description    : hal layer.uart  send data
* Input          : u8 data : data
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uartSendData(u8 data);
/*******************************************************************************
* Function Name  : hal_uartRXIsrRegister
* Description    : hal layer.uart  rx ISR register
* Input          : void (*isr)(u8 data) : isr
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uartRXIsrRegister(void (*isr)(u8 data));
/*******************************************************************************
* Function Name  : hal_uartIOShare
* Description    : hal layer.uart io share flag
* Input          : none
* Output         : None
* Return         : u8 : 0->current is uart using,1->other using
*******************************************************************************/
u8 hal_uartIOShare(void);
/*******************************************************************************
* Function Name  : hal_uartPrintString
* Description    : uart printf string
* Input          : 
*                  
* Output         : None
* Return         : None
*******************************************************************************/
void hal_uartPrintString(char *string);
void liukongCTLTimeout1(void);
u8 Bt_Control_Led_OnOff(u8 onoff);

void hal_uart1Init(void);

void hal_uart1RXIsrRegister(void (*isr)(u8 data));














#endif
