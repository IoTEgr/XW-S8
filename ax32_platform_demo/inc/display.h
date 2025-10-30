/****************************************************************************
**
 **                              DISPLAY
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  DISPLAY
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : display.h
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : 
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#ifndef  DISPLAY_H
     #define  DISPLAY_H




enum
{
	DISP_LAYER_VIDEO=0,
	DISP_LAYER_OSD0,
	DISP_LAYER_OSD1,

	DISP_LAYER_MAX
};

enum
{
    DISP_PIP_FRONT = 0,
    DISP_PIP_BACK,
    DISP_PIP_BACK_RIGHTTOP,
    DISP_PIP_FRONT_RIGHTTOP,
    DISP_PIP_FRONT_LEFT_BACK_RIGHT,
    DISP_PIP_FRONT_RIGHT_BACK_LEFT,
    DISP_PIP_MAX,
    DISP_PIP_DISABLE,
};
	 
//-----------------rotate table
#define  VIDEO_ROTATE_NORMAL      0
#define  VIDEO_ROTATE_VMIRROR    1
#define  VIDEO_ROTATE_HMIRROR    2
#define  VIDEO_ROTATE_180          3
#define  VIDEO_ROTATE_90           4
#define  VIDEO_ROTATE_270          5
#define  VIDEO_ROTATE_DEFAULT      0xff
//-----------------ratio table
#define  VIDEO_RATIO_4_3           0
#define  VIDEO_RATIO_16_9          1
#define  VIDEO_RATIO_16_10         2
#define  VIDEO_RATIO_DEFAULT       0xff // default,do not change ratio


/*******************************************************************************
* Function Name  : dispLayerInit
* Description    : display layer initiail
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerInit(INT8U layer);
/*******************************************************************************
* Function Name  : dispLayerunInit
* Description    : display layer uninitiail
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerUninit(INT8U layer);
/*******************************************************************************
* Function Name  : dispLayerResolution
* Description    : get display layer resolution
* Input          : INT8U layer : display layer index
                      INT16U *width : width 
                      INT16U *height : height
* Output         : none                                            
* Return         :int 0:success
*******************************************************************************/
int dispLayerGetResolution(INT8U layer,INT16U *width,INT16U *height);
/*******************************************************************************
* Function Name  : dispLayerSetResolution
* Description    : set display layer resolution
* Input          : INT8U layer : display layer index
                      INT16U *width : width 
                      INT16U *height : height
* Output         : none                                            
* Return         :int 0:success
*******************************************************************************/
int dispLayerSetResolution(INT8U layer,INT16U width,INT16U height);
/*******************************************************************************
* Function Name  : dispLayerSetRotate
* Description    : set display layer raote,only used in video layer
* Input          : INT8U layer : display layer index
                      INT8U rotate:roate degree
* Output         : none                                            
* Return         :int 0:success
*******************************************************************************/
int dispLayerSetRotate(INT8U layer,INT8U rotate);
/*******************************************************************************
* Function Name  : dispRatioResolutionCalaulate
* Description    : calculate resolution of ratio
* Input          : INT8U ratio : ratio .4:3,16:9,16:10
                       INT16U *width : width 
                      INT16U *height : height
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispRatioResolutionCalculate(INT8U ratio,INT16U *width,INT16U *height);
/*******************************************************************************
* Function Name  : dispLayerSetRatio
* Description    : display set video ratio,only used on video layer
* Input          : INT8U layer : layer index,video,osd0,osd1
                      INT8U ratio : ratio .4:3,16:9,16:10
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerSetRatio(INT8U layer,INT8U ratio);
/*******************************************************************************
* Function Name  : dispFrameShow
* Description    : display show for video,osd0,osd1 set buffer to show,can not used in csi preview
* Input          : INT8U layer : layer index,video,osd0,osd1
                      INT32U addr: buffer to show
                      INT16U width:buffer width
                      INT16U height:buffer height
                      INT8U ratio : show ratio
                      INT8U rotate: rotate
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerShow(INT8U layer,INT32U addr,INT16U width,INT16U height,INT8U rotate);
/*******************************************************************************
* Function Name  : dispLayerGetFrame
* Description    : display get osd0,osd1 frame buffer
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         :0
*******************************************************************************/
void *dispLayerGetFrame(INT8U layer);
/*******************************************************************************
* Function Name  : dispLayerSetFrame
* Description    : display show for osd0,osd1
* Input          : INT8U layer : layer index,video,osd0,osd1
                      INT32U addr: buffer to show
* Output         : none                                            
* Return         :0
*******************************************************************************/
int dispLayerSetFrame(INT8U layer,u32 addr);
/*******************************************************************************
* Function Name  : dispLayerSetResolution
* Description    : set Pic-in-Pic mode
* Input          : INT8U mode : mode index
* Output         : none                                            
* Return         : int 0:success
*******************************************************************************/
int dispLayerSetPIPMode(INT8U mode);
/*******************************************************************************
* Function Name  : dispLayerFreeFrame
* Description    : display get osd0,osd1 frame buffer
* Input          : INT8U layer : layer index,video,osd0,osd1
* Output         : none                                            
* Return         : 0
*******************************************************************************/
s32 dispLayerFreeFrame(INT8U layer,void * frame);


/*******************************************************************************
* Function Name  : dispLayerSetFrontCrop
* Description    : set Pic-in-Pic mode
* Input          : INT8U level :  scale level
                    :mjpg_crop: 0 recode crop image , 1 recode sensor image
* Output         : none                                            
* Return         : int 0:success
*******************************************************************************/
int dispLayerSetFrontCrop(INT8U level,u8 mjpg_crop);


void *dispLayerGetFreeFrame(INT8U layer);




#endif



