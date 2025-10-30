#ifndef  SCALER_H
#define SCALER_H

//#include "typedef.h"

typedef struct Scaler_Para_S
{
	unsigned short int width;
	unsigned short int heigth;

	unsigned char *data;
}Scaler_Para_T;





/*****************************************************************************
* Function Name  : scalerInit
* Description    : scaler initial
* Output        : 
* Return         : success

******************************************************************************/
s32_t scalerInit(void);
/*****************************************************************************
* Function Name  : scalerUninit
* Description    : scaler uninitial
* Output        : 
* Return         : success

******************************************************************************/
s32_t scalerUninit(void);
/*****************************************************************************
* Function Name  : scalerStart
* Description    : scaler start
* Input           : u32_t image : image
                       s16_t *x      : image x;
                       s16_t *y      : image y;
                       u16_t *sw    : image width;
                       u16_t *sh    : image height;
                       u16_t scaler : scaler
* Output        : 
* Return         : success

******************************************************************************/
u32_t scalerStart(u32_t image,s16_t *x,s16_t *y, u16_t *sw,u16_t *sh,u16_t scaler);
/*******************************************************************************
* Function Name  : rotateExit
* Description    : release rotate buffer
* Input          :  u32_t image : image buffer
* Output        : 
* Return         : 0

******************************************************************************/
s32_t scalerStop(u32_t image);



#endif