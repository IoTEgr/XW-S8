/****************************************************************************
        ***           ***                      MAXLIB-GRAPHC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **              MAX ROURCE ICON MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : Rotate.c
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


#if MAXLIB_ROTATE_USE > 0

typedef struct Rotate_Node_S
{
	u32_t mem;
	
	u32_t tcolor;
	
	u16_t degree;
	u16_t rev;

    u32_t image;
	u16_t width;
	u16_t height;

	
}Rotate_Node_T;

#define  ROTATE_NODE_MAX          CFG_ROTATE_NODE_NUM

static Rotate_Node_T  rotateIcon[ROTATE_NODE_MAX];

u16_t *max_rotate_start(u16_t *src_buff,u16_t *w,u16_t *h,u16_t angle,u16_t tcolor);
/*******************************************************************************
* Function Name  : rotateInit
* Description    : rotate initial
* Input          :  
* Output        : 
* Return         : success

******************************************************************************/
s32_t rotateInit(void)
{
	s32_t i;

	for(i=0;i<ROTATE_NODE_MAX;i++)
	{
		rotateIcon[i].mem = 0;
		rotateIcon[i].image = 0;
	}

	return 0;
}
/*******************************************************************************
* Function Name  : rotateUninit
* Description    : rotate uninitial
* Output        : 
* Return         : success

******************************************************************************/
s32_t rotateUninit(void)
{
	s32_t i;

	for(i=0;i<ROTATE_NODE_MAX;i++)
	{
		if(rotateIcon[i].mem)
			max_mem_free((void *)rotateIcon[i].mem);
		rotateIcon[i].mem = 0;
	}

	return 0;
}
/*******************************************************************************
* Function Name  : rotateStart
* Description    : rotate image
* Input          :  u32_t image : image buffer
                       s16_t *x : image draw x
                       s16_t *y : image draw y
                       s16_t *w: image width
                       s16_t *h: image height
                       u16_t degree: rotate angle
                       u32_t *tcolor : color
* Output        : 
* Return         : void *  0 : fail

******************************************************************************/
u32_t rotateStart(u32_t image,s16_t *x,s16_t *y,u16_t *w,u16_t *h,u16_t degree,u32_t *tcolor)
{
	s32_t i,node,idata;
	u16_t tw,th;

    node = ROTATE_NODE_MAX;
	for(i=0;i<ROTATE_NODE_MAX;i++)
	{
		if(rotateIcon[i].mem == 0)
		{
			if(node== ROTATE_NODE_MAX)
				node = i;
			continue;
		}
		if(rotateIcon[i].image == image)
			break;
	}

	if(i<ROTATE_NODE_MAX)
	{
		node = i;
		if(rotateIcon[i].degree == degree)
		{
			tw = *w;
			th = *h;
			*w = rotateIcon[i].width;
			*h = rotateIcon[i].height;
			*x-=(*w-tw)>>1;
			*y-=(*h-th)>>1;
			*tcolor = rotateIcon[i].tcolor;
			return rotateIcon[i].mem;
		}
		max_mem_free((void *)rotateIcon[i].mem);
	}
	else
	{
		if(node == ROTATE_NODE_MAX) // no more rotate node
			return 0;
	}

	rotateIcon[node].image = image;
	rotateIcon[node].degree = degree;
	idata = viewGetRawDate(image,VIEW_CTRL_ROTATE);//R_getData(viewGetResId(image));
	if(R_TRANSFER_COLOR_CHECK(*tcolor))
	    rotateIcon[node].tcolor = *tcolor;
	else
	{
		*tcolor = rotateIcon[node].tcolor = R_TRANSFER_COLOR_MAKE(R_COLOR_TRANSFER);
	}
    tw = *w;
	th = *h;
	rotateIcon[node].mem = (u32_t)max_rotate_start((u16_t *)idata,w,h,degree,R_TRANSFER_COLOR_GET(rotateIcon[node].tcolor));
	rotateIcon[node].width = *w;
	rotateIcon[node].height = *h;
    *x-=(*w-tw)>>1;
	*y-=(*h-th)>>1;
	
    return rotateIcon[node].mem;
	
}
/*******************************************************************************
* Function Name  : rotateExit
* Description    : release rotate buffer
* Input          :  u32_t image : image buffer
* Output        : 
* Return         : 0

******************************************************************************/
s32_t rotateStop(u32_t image)
{
	int i;

	for(i=0;i<ROTATE_NODE_MAX;i++)
	{
		if(rotateIcon[i].mem == 0)
			continue;
		if(rotateIcon[i].image == image)
		{
			max_mem_free((void *)rotateIcon[i].mem);
			rotateIcon[i].mem = 0;
			break;
		}
	}

	return 0;
}
/*******************************************************************************
* Function Name  : rotateGetData
* Description    :  rotate get data
* Input          :  u32_t image : image 
* Output        : 
* Return         : 0

******************************************************************************/
u32_t rotateGetData(u32_t image)
{
	int i;

	for(i=0;i<ROTATE_NODE_MAX;i++)
	{
		if(rotateIcon[i].mem == 0)
			continue;
		if(rotateIcon[i].image == image)
		{
			return rotateIcon[i].mem;
		}
	}

	return 0;	
}
u16_t *max_rotate_start(u16_t *src_buff,u16_t *w,u16_t *h,u16_t angle,u16_t tcolor)
{
	color_t *des_buff;
    u16_t width,height;
	u16_t desw,desh;
	s32_t ltopx,ltopy,lbomx,lbomy,rtopx,rtopy,rbomx,rbomy;
	s32_t tempw,temph;
	s32_t i,j,x,y;
	s16_t cosw,cosh,sinw,sinh,sini,cosi,sin,cos;

	width = *w;
	height = *h;

	tempw = width/2;
	temph = height/2;

	ltopx = ((-tempw)*math_cos(angle)+(temph)*math_sin(angle))/100;
	ltopy = ((tempw)*math_sin(angle)+(temph)*math_cos(angle))/100;
	rtopx = ((tempw)*math_cos(angle)+(temph)*math_sin(angle))/100;
	rtopy = ((-tempw)*math_sin(angle)+(temph)*math_cos(angle))/100;
	lbomx = ((-tempw)*math_cos(angle)+(-temph)*math_sin(angle))/100;
	lbomy = ((tempw)*math_sin(angle)+(-temph)*math_cos(angle))/100;
	rbomx = ((tempw)*math_cos(angle)+(-temph)*math_sin(angle))/100;
	rbomy = ((-tempw)*math_sin(angle)+(-temph)*math_cos(angle))/100;

	desw = math_max(math_abs(rbomx-ltopx),math_abs(rtopx-lbomx));
	desh = math_max(math_abs(rbomy-ltopy),math_abs(rtopy-lbomy));

	des_buff = (color_t *)max_mem_malloc(desw*desh*2); 

	if (!des_buff) {
		//DBG_PRINT("malloc des_buff fail!!\r\n");
		return 0;
	}
    angle = 360-angle;
	sin = math_sin(angle);
	cos = math_cos(angle);
	cosw = (desw/2)*cos;
	sinw = (desw/2)*sin;
	cosh = (desh/2)*cos;
	sinh = (desh/2)*sin;
	
	for (i = 0;i < (desh * desw);i++)
         des_buff[i] = tcolor;

	for(i=0;i<desh;i++)
	{
		 sini = i*sin;
		 cosi = i*cos;
		 for(j=0;j<desw;j++)
		 {
		 	 x = ((j*cos - cosw)+(sinh-sini))/100;
			 y = ((cosh-cosi)-(j*sin-sinw))/100;
			 
		 	 //x = ((j-desw/2)*cos_table[360-angle]+(desh/2-i)*sin_table[360-angle])/100;
			// y = ((desh/2-i)*cos_table[360-angle]-(j-desw/2)*sin_table[360-angle])/100;

			 if(x>tempw || x<(-tempw) || y>temph || y<(-temph))
			 	continue;

			 x = x+width/2;
			 y = math_abs(y-height/2);
			 if((y*width+x)>width*height)
			 {
				  continue;
			 }
			 *(des_buff+i*desw+j) = (color_t)*(src_buff+y*width+x);
		 }
	}

	*w = desh;
	*h = desw;
	
	return ((u16_t*)des_buff);

}
void max_rotate_end(u16_t *buff)
{
	max_mem_free(buff);
}





















#endif

