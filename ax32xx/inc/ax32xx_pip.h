#ifndef AX32XX_PIP_H
#define AX32XX_PIP_H

enum pip_src_m {
    PIP_SRC_JPG = 0,
    PIP_SRC_ISP = 1,
};
/*******************************************************************************
* Function Name  : ax32xx_pipConfig
* Description    : pip config
* Input          : u8 top_src: top layer source. 0:JPGB,1:ISP
*                  u16 x : top layer strat-x relative to bottom layer
*                  u16 y : top layer strat-y relative to bottom layer
*                  u16 jwidth : when top layer is jpeg
*                  u16 jheight: when top layer is jpeg
*                  bool enable: pip enable
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_pipConfig(u8 top_src,u16 x,u16 y,u16 jwidth,u16 jheight,bool enable);
/*******************************************************************************
* Function Name  : ax32xx_pipEnable
* Description    : pip enable
* Input          : bool enable: pip enable
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_pipEnable(bool enable);
/*******************************************************************************
* Function Name  : ax32xx_pipCounterReset
* Description    : reset internal counter of pip
* Input          : u8 src
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_pipCounterReset(u8 src);
/*******************************************************************************
* Function Name  : ax32xx_pipGetTopLayer
* Description    : get source of pip top-layer
* Input          : none
* Output         : none
* Return         : 0:jpeg;1:isp
*******************************************************************************/
u8 ax32xx_pipGetTopLayer(void);
/*******************************************************************************
* Function Name  : ax32xx_pipGetBotLayer
* Description    : get source of pip top-layer
* Input          : none
* Output         : none
* Return         : 0:jpeg;1:isp
*******************************************************************************/
u8 ax32xx_pipGetBotLayer(void);
#endif
