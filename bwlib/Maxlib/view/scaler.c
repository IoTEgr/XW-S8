/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **              MAX ROURCE ICON MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : scaler.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib resource icon managemant
*               
* History     :
* 2016-09-08  : 
*      <1>.This is created by mark,set version as v0101.
*      <2>.Add basic functions.
******************************************************************************/
#include "./../max.h"
#include "./../maxlib.h"


#if  MAXLIB_SCALER_USE > 0

#define  SCALER_NODE_NUM    CFG_SCALER_NONE_NUM

#define SCALER_CTR_EN      0x00000001
#define SCALER_CTR_FLAG    0x00000002
#define RGB565_R(c)   (c&0xf800)
#define RGB565_G(c)   (c&0x07e0)
#define RGB565_B(c)   (c&0x001f)
#define RGB565_M(r,g,b)  ((r&0xf800)|(g&0x07e0)|(b&0x001f))
typedef struct ScalerFifo_S
{
    u32_t ctrl;
    Scaler_Para_T in;
    Scaler_Para_T out;
    u32_t swidth;
    u32_t sheigth;
    u32_t dwidth;
    u32_t dheigth;
    u32_t i;
    u32_t j;
    u32_t sxr;
    u32_t syr;
    u32_t xr;
    u32_t yr;
    color_t *des;
    color_t *src;
    color_t *sdata;
    color_t *ddata;
    u32_t size;
}ScalerFifo_T;

typedef struct Scaler_Node_S
{
	u32_t image;
	u32_t mem;
	s16_t owidth;
	s16_t oheight;

    u16_t scaler;
	u16_t rev;
}Scaler_Node_T;

static ScalerFifo_T ScalerCtr;
static Scaler_Node_T scalerNode[SCALER_NODE_NUM];

s32_t max_scaler_main(Scaler_Para_T *out,Scaler_Para_T *in);
/*****************************************************************************
* Function Name  : scalerInit
* Description    : scaler initial
* Output        : 
* Return         : success

******************************************************************************/
s32_t scalerInit(void)
{
	s32_t i;

	for(i=0;i<SCALER_NODE_NUM;i++)
	{
		scalerNode[i].image = 0;
		scalerNode[i].mem = 0;
	}

	return 0;
}
/*****************************************************************************
* Function Name  : scalerUninit
* Description    : scaler uninitial
* Output        : 
* Return         : success

******************************************************************************/
s32_t scalerUninit(void)
{
	s32_t i;

	for(i=0;i<SCALER_NODE_NUM;i++)
	{
		scalerNode[i].image = 0;
		if(scalerNode[i].mem)
			max_mem_free((void *)scalerNode[i].mem);
		scalerNode[i].mem = 0;
	}
	return 0;
}
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
u32_t scalerStart(u32_t image,s16_t *x,s16_t *y, u16_t *sw,u16_t *sh,u16_t scaler)
{
	u16_t dw,dh,i,node;
	Scaler_Para_T out,in;

	if(image == 0 || scaler == 0)
		return 0;
	if(scaler == 100)
		return image;
	in.data = (u8_t *)viewGetRawDate(image,VIEW_CTRL_ZOOM);
	if(in.data == NULL)
		return 0;
	node = SCALER_NODE_NUM;
	for(i=0;i<SCALER_NODE_NUM;i++)
	{
		if(scalerNode[i].image == 0)
		{
			if(node == SCALER_NODE_NUM)
				node = i;
			continue;
		}
		if(scalerNode[i].image == image)
			break;
	}
	if(i<SCALER_NODE_NUM)
	{
		if(scalerNode[i].scaler == scaler)
		{
			*x += (*sw-scalerNode[i].owidth)/2;
			*y += (*sh-scalerNode[i].oheight)/2;
			*sw = scalerNode[i].owidth;
			*sh = scalerNode[i].oheight;
			return scalerNode[i].mem;
		}
		max_mem_free((void *)scalerNode[i].mem);
		node = i;		
	}
	else
	{
		scalerNode[node].image = image;
	}

	dw = (*sw)*scaler/100;
	dh = (*sh)*scaler/100;
	scalerNode[node].owidth = dw;
	scalerNode[node].oheight= dh;
	scalerNode[node].mem = (u32_t)max_mem_malloc(dw*dh*2);
	scalerNode[node].scaler = scaler;

	out.data = (u8_t *)scalerNode[node].mem;
	out.width  = dw;
	out.heigth= dh;

	
	in.width = *sw;
	in.heigth=*sh;
	max_scaler_main(&out,&in);

	*x += (*sw-scalerNode[node].owidth)/2;
	*y += (*sh-scalerNode[node].oheight)/2;
	*sw = scalerNode[node].owidth;
	*sh = scalerNode[node].oheight;

	return scalerNode[node].mem;
}
/*******************************************************************************
* Function Name  : rotateExit
* Description    : release rotate buffer
* Input          :  u32_t image : image buffer
* Output        : 
* Return         : 0

******************************************************************************/
s32_t scalerStop(u32_t image)
{
	int i;

	for(i=0;i<SCALER_NODE_NUM;i++)
	{
		if(scalerNode[i].mem == 0)
			continue;
		if(scalerNode[i].image == image)
		{
			max_mem_free((void *)scalerNode[i].mem);
			scalerNode[i].mem = 0;
			break;
		}
	}

	return 0;
}
s32_t max_scaler_main(Scaler_Para_T *out,Scaler_Para_T *in)
{
	u16_t color1;
#define SHIRT_STEP   4
    if(ScalerCtr.ctrl & SCALER_CTR_EN)
        return -1;
    ScalerCtr.ctrl |= SCALER_CTR_EN;    
	ScalerCtr.swidth  = in->width;
	ScalerCtr.sheigth = in->heigth;
	ScalerCtr.dwidth  = out->width;
	ScalerCtr.dheigth = out->heigth;
    ScalerCtr.sdata = (color_t *)in->data;
	ScalerCtr.ddata = (color_t *)out->data;
    
    if(ScalerCtr.swidth == ScalerCtr.dwidth && ScalerCtr.sheigth == ScalerCtr.dheigth)  // no need scaler.only copy data
    {
        
        for(ScalerCtr.i=0;ScalerCtr.i<ScalerCtr.sheigth;ScalerCtr.i++)
        {
            for(ScalerCtr.j=0;ScalerCtr.j<ScalerCtr.swidth;ScalerCtr.j++)
            {
                 ScalerCtr.ddata[ScalerCtr.j] = ScalerCtr.sdata[ScalerCtr.j]; 
            }
            ScalerCtr.ddata += ScalerCtr.dwidth;
            ScalerCtr.sdata += ScalerCtr.swidth;
        }
        ScalerCtr.ctrl &= ~SCALER_CTR_EN;
        return 1;
    }

    ScalerCtr.ctrl |= SCALER_CTR_FLAG;
    
	ScalerCtr.sxr = (ScalerCtr.swidth<<SHIRT_STEP)/ScalerCtr.dwidth;
	ScalerCtr.syr = (ScalerCtr.sheigth<<SHIRT_STEP)/ScalerCtr.dheigth;

	ScalerCtr.yr = 0;
    ScalerCtr.des = ScalerCtr.ddata;
	for(ScalerCtr.j=0;ScalerCtr.j<ScalerCtr.dheigth;ScalerCtr.j++)
	{
	   
	   ScalerCtr.src = ScalerCtr.sdata +(ScalerCtr.yr>>SHIRT_STEP)*ScalerCtr.swidth;
	   ScalerCtr.xr = 0;
	   color1 = 0;
       for(ScalerCtr.i=0;ScalerCtr.i<ScalerCtr.dwidth;ScalerCtr.i++)
	   {
#if 0
		   color2 =  ScalerCtr.src[ScalerCtr.xr>>SHIRT_STEP];
		   ScalerCtr.des[ScalerCtr.i] = RGB565_M((RGB565_R(color1)>>1)+(RGB565_R(color2)>>1),\
			                            (RGB565_G(color1)>>1)+(RGB565_G(color2)>>1),\
			                            (RGB565_B(color1)>>1)+(RGB565_B(color2)>>1));
           color1 = color2;
#else
           ScalerCtr.des[ScalerCtr.i] = ScalerCtr.src[ScalerCtr.xr>>SHIRT_STEP];
#endif
		   ScalerCtr.xr+=ScalerCtr.sxr;
	   }
	   ScalerCtr.yr+=ScalerCtr.syr;
	   ScalerCtr.des+=ScalerCtr.dwidth;
	}
    ScalerCtr.ctrl &= ~SCALER_CTR_EN;
    color1 = color1;
	return 1;
}
#if 0

int scaler2(Scaler_Para_T *out,Scaler_Para_T *in)
{
	u16_t p0,p1,p2,p3;
    u16_t *des,*src;
	u16_t i,j,x,y;
	u32_t xr,yr;
	s32_t ex,ey;
	float sy,sx,u,v,pm3,pm2,pm1,pm0;

	des = (u16_t *)out->data;
	src = (u16_t *)in->data;
	xr = (in->width<<16)/out->width;
	yr = (in->heigth<<16)/out->heigth;
    ex = -(1<<15)+(xr>>1);
	ey = -(1<<15)+(yr>>1);

	for(i=0;i<out->heigth;i++)
	{
		sy = (i+0.499999)*in->heigth/out->heigth-0.5;
		for(j=0;j<out->width;j++)
		{
			sx = (j+0.499999)*in->width/out->width-0.5;
			x = (u16_t)sx;
			y = (u16_t)sy;
			if(x>=in->width || y>in->heigth)
				p0 = 0;
			else
				p0 = *(u16_t *)(src +x +y*in->width);

			if((x+1)>=in->width || y>in->heigth)
				p1 = 0;
			else
				p1 = *(u16_t *)(src +x +1+y*in->width);
			if((x)>=in->width || (y+1)>in->heigth)
				p2 = 0;
			else
				p2 = *(u16_t *)(src +x+(y+1)*in->width);
			if((x+1)>=in->width || (y+1)>in->heigth)
				p3 = 0;
			else
				p3 = *(u16_t *)(src +x +1+(y+1)*in->width);

            u = sx-x;
			v = sy-y;
			pm3 = u*v;
			pm2 = u*(1-v);
			pm1 = v*(1-u);
			pm0 = (1-u)*(1-v);

			*(des+j+i*out->width) = RGB565_M((u16_t)(RGB565_R(p0)*pm0+RGB565_R(p1)*pm1+RGB565_R(p2)*pm2+RGB565_R(p3)*pm3),\
				                             (u16_t)(RGB565_G(p0)*pm0+RGB565_G(p1)*pm1+RGB565_G(p2)*pm2+RGB565_G(p3)*pm3),\
                                             (u16_t)(RGB565_B(p0)*pm0+RGB565_B(p1)*pm1+RGB565_B(p2)*pm2+RGB565_B(p3)*pm3));
		}
	}
}
#endif














#endif
