#include "../inc/ax32xx.h"
//TODO:arrange

///////////////////// PIC IN PIC //////////////////
/**
 * \note pip获取底层图像的起始地址(bottom pic buffer addr)，
 *       对底层图像buffer中{x,y,width,height}区域进行保护，不允许
 *       底层图像数据写入该区域。
 *       pip对顶层图像不做任何限制。
 */
#define R_PIP_CON       PICCON
#define R_PIP_COOR      PICCOOR
#define R_PIP_JSIZE     PICJPGSIZE

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
void ax32xx_pipConfig(u8 top_src,u16 x,u16 y,u16 jwidth,u16 jheight,bool enable)
{
    R_PIP_CON |= (1<<2);
    R_PIP_CON  = (0<<2)|//clear register
                 (top_src<<1)|//top image,0:jpeg1,1:isp
                 (enable<<0);//enable
             
    R_PIP_COOR = ((y&~1)<<16)|//y start
                 ((x&~3)<<0); //x start
              
    if(top_src == PIP_SRC_JPG)
        R_PIP_JSIZE = (jheight<<16)|
                      (jwidth<<0);
    else
        R_PIP_JSIZE = 0;
}
/*******************************************************************************
* Function Name  : ax32xx_pipEnable
* Description    : pip enable
* Input          : bool enable: pip enable
* Output         : None
* Return         : none
*******************************************************************************/
void ax32xx_pipEnable(bool enable)
{
    if(enable)
        R_PIP_CON |= (1<<0);
    else
        R_PIP_CON &=~(1<<0);
}
/*******************************************************************************
* Function Name  : ax32xx_pipCounterReset
* Description    : reset internal counter of pip
* Input          : u8 src
* Output         : none
* Return         : none
*******************************************************************************/
void ax32xx_pipCounterReset(u8 src)
{
    //if 'src' is bottom,reset pip
    if(src == (((R_PIP_CON >> 1) ^ 1) & 1))
    {
        R_PIP_CON |= (1<<2);
    }
}
/*******************************************************************************
* Function Name  : ax32xx_pipGetTopLayer
* Description    : get source of pip top-layer
* Input          : none
* Output         : none
* Return         : 0:jpeg;1:isp
*******************************************************************************/
u8 ax32xx_pipGetTopLayer(void)
{
    return (R_PIP_CON >> 1) & 1;
}
/*******************************************************************************
* Function Name  : ax32xx_pipGetBotLayer
* Description    : get source of pip top-layer
* Input          : none
* Output         : none
* Return         : 0:jpeg;1:isp
*******************************************************************************/
u8 ax32xx_pipGetBotLayer(void)
{
    return ((R_PIP_CON >> 1) ^ 1) & 1;
}
//void pip_print_all_register(void)
//{
//    debgreg(R_PIP_CON);
//    debgreg(R_PIP_COOR);
//    debgreg(R_PIP_JSIZE);
//}

