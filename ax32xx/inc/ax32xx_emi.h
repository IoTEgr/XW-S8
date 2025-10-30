#ifndef AX32XX_EMI_H
#define AX32XX_EMI_H

enum {
    EMI_MASTER = 0,
    EMI_SLAVE  = 1,
};

extern void (*fp_emi_done)(void);

/*******************************************************************************
* Function Name  : ax32xx_emiPinConfig
* Description    : emi init
* Input          : u8 mode : select MASTER/SLAVE
*                  u32 baud: baud rate
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_emiInit(u8 mode,u32 baud);
/*******************************************************************************
* Function Name  : ax32xx_emiISRRegister
* Description    : register irq service
* Input          : void (*isr)(u8 header)
*                  header for rx
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_emiISRRegister(void (*isr)(u32 header));
/*******************************************************************************
* Function Name  : ax32xx_emiIRQHandler
* Description    : emi irs
* Input          : None
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_emiIRQHandler(void);
/*******************************************************************************
* Function Name  : ax32xx_emiKick
* Description    : if emi is master,this will kick tx
*                  if emi is slave,this will kick rx
* Input          : u8 header : only for tx,4bit header will send to slave
*                  u8 * buffer : src buffer for tx,dest buffer for rx
*                  u32 cnt : for tx,means how many bytes to tx
*                            for rx,means how many bytes to rx
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_emiKick(u32 header,u8 * buffer,u32 cnt);
/*******************************************************************************
* Function Name  : ax32xx_emiCheckBusy
* Description    : check emi is busy or not
* Input          : None
* Output         : None
* Return         : return true if busy
*******************************************************************************/
bool ax32xx_emiCheckBusy(void);
/*******************************************************************************
* Function Name  : ax32xx_emiCheckRXError
* Description    : check emi rx is error or not
* Input          : None
* Output         : None
* Return         : return true if error
*******************************************************************************/
bool ax32xx_emiCheckRXError(void);
#endif
