/****************************************************************************
**
 **                              RESOURCE 
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  RESOURCE FONT
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : font.c
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
#include "application.h"

#define  FONT_DATA                RES_RESFONT
#define  FONT_INDEX              RES_RESFONTIDX


typedef struct Font_Data_S
{
	INT16U width;
	INT16U height;
	INT32U offset;
}Font_Data_T;

typedef struct Font_Idx_S
{
	INT32U index;
	INT32U offset;
}Font_Idx_T;
typedef struct Font_Str_S
{
	INT16U width;
	INT16U height;
	INT16U number;
	INT16U offset;
}Font_Str_T;

typedef struct Font_Ctrl_S
{
	INT8U lanMax;
	INT8U strMax;
	INT8U curLan;
	INT8U curStr;

    INT32U curChar;
	INT32U charMax;

	INT32U curOffset;

	Font_Str_T strInfo;
	Font_Data_T charInfo;

	INT32U addrData;
	INT32U addrIndex;

	INT16U strCache[128];
#if HAL_CFG_EN_LINE == 0
    INT8U  charCache[1024];
#endif
}Font_Ctrl_T;

static INT8U CH_INV_W = 0;

static Font_Ctrl_T fontCtrl;

/*******************************************************************************
* Function Name  : fontInit
* Description    :  font initial
* Input          : 
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int fontInit(void)
{
	int addr;

	fontCtrl.curStr = 0xff;
	fontCtrl.curLan = 0xff;
	fontCtrl.strMax = 0;
	fontCtrl.curChar= 0xffffffff;

	addr = nv_open(FONT_DATA);
	if(addr <0)
	{
		deg_Printf("font : initial fail.can not find data\n");
		return -1;
	}
	fontCtrl.addrData = addr;
	nv_read(addr,&fontCtrl.charMax,4);  // get counter of char
	
	addr = nv_open(FONT_INDEX);
	if(addr<0)
	{
		deg_Printf("font : initial fail.can not find index\n");
		return -1;
	}
	fontCtrl.addrIndex = addr;
    nv_read(fontCtrl.addrIndex,&addr,4); 
	if((addr&0x0000ffff) != 0x0000584d)
	{
		deg_Printf("font : initial fail.index error\n");
		return -2;
	}
    fontCtrl.lanMax = (addr>>24)&0xff;
	CH_INV_W = (addr>>16)&0xff;
	deg_Printf("font : initial ok.max lan =%d,max char %d,CH_INV_W=%d\n",fontCtrl.lanMax,fontCtrl.charMax,CH_INV_W);
    return fontCtrl.lanMax;	
}
/*******************************************************************************
* Function Name  : fontSetLanguage
* Description    :  set current lanaguage
* Input          : INT8U num : lan index 
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int fontSetLanguage(INT8U num)
{
	Font_Idx_T index;
	
	if(num>=fontCtrl.lanMax)
		return -1;

	if(num == fontCtrl.curLan)
		return 0;

	nv_read(fontCtrl.addrIndex+(num+1)*8,&index,8); 
	if(num!=(index.index&0xff))
		return -1;
	fontCtrl.curOffset = index.offset;
	fontCtrl.curLan = num;
	fontCtrl.strMax = (index.index>>8)&0xff;
// all cache is invliad	
	fontCtrl.curStr = 0xff;
	fontCtrl.curChar= 0xffffffff;
    deg_Printf("font : language = %d,str max = %d\n",num,fontCtrl.strMax);
	return fontCtrl.curLan;
}
/*******************************************************************************
* Function Name  : fontGetCharData
* Description    :  get char data
* Input          : INT8U num : char offset 
                      INT8U *buffer : dest buffer
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
static int fontGetCharData(INT32U num,INT8U *buffer)
{
	int temp;
	if(num>=fontCtrl.charMax)
		return -1;

	if(num != fontCtrl.curChar)
	{
		nv_read(fontCtrl.addrData+(num+1)*8,&fontCtrl.charInfo,8);
	}
    temp = fontCtrl.charInfo.height*((fontCtrl.charInfo.width+7)>>3);
	if(temp&0x0f)
		temp = (temp&(~0x0f))+0x10;
    nv_read(fontCtrl.addrData+fontCtrl.charInfo.offset,buffer,temp); // read data

	return 0;
}
/*******************************************************************************
* Function Name  : fontGetStringInfo
* Description    :  get string info 
* Input          : INT8U num : string index
                      INT16U *width:string width
                      INT16U *height:string height
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int fontGetStringInfo(INT8U num,INT16U *width,INT16U *height)
{
	if(num>=fontCtrl.strMax)
		return -1;
	if(num!=fontCtrl.curStr)
	{
		nv_read(fontCtrl.addrIndex+fontCtrl.curOffset+(num+1)*8,&fontCtrl.strInfo,8);
	}

	if(width)
		*width = fontCtrl.strInfo.width;
	if(height)
		*height = fontCtrl.strInfo.height;

	return 0;
}
/*******************************************************************************
* Function Name  : fontGetString
* Description    :  get string data 
* Input          : INT8U num : string index
                      INT16U *buffer: dest buffer
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
static int fontGetString(INT8U num,INT16U *buffer)
{
	int temp;
	if(num>=fontCtrl.strMax)
		return -1;
	if(num!=fontCtrl.curStr)
	{
		nv_read(fontCtrl.addrIndex+fontCtrl.curOffset+(num+1)*8,&fontCtrl.strInfo,8);
	}
    temp = fontCtrl.strInfo.number<<1;
	if(temp&0x0f)
		temp = (temp&(~0x0f))+0x10;
	nv_read(fontCtrl.addrIndex+fontCtrl.curOffset+fontCtrl.strInfo.offset,buffer,temp);

	return fontCtrl.strInfo.number;
}
/*******************************************************************************
* Function Name  : fontDrawChar
* Description    :  draw char 
* Input          : INT8U *dest : destination buffer
                      INT8U *charsrc :char buffer
                      INT16S x    : x position
                      INT16S y    : y position
                      INT16U char_w: char width
                      INT16U char_h: char height
                      INT16U width : buffer size
                      INT8U color : color
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
static int fontDrawChar(INT8U *dest,INT8U *charsrc,INT16S x,INT16S y,INT16U char_w,INT16U char_h,INT16U dest_w,INT8U color)
{
	INT8U i,j,temp,mask;
	INT8U *line;

	if(charsrc == NULL || dest == NULL)
		return -1;
		
	line = dest+(x)+((y*dest_w));

	for(i=0;i<char_h;i++)
	{
	    for(j=0;j<(char_w+7)/8;j++)
		{
	         temp = *charsrc++;
			 for(mask=0;mask<8;mask++)
			 {
				 if(temp&(0x80>>mask))
				 {
					 *line++ = color;
				 }
				 else
				 {
					 *line++=0xfd;
				 }
			 }
		 }
		 y++;
		 line =dest+(x)+((y*dest_w));
		 hal_sysDcacheWBack((u32)dest+(x)+(((y-1)*dest_w)),(char_w+7)/8);
	}
	return 0;  
}
/*******************************************************************************
* Function Name  : fontDrawString
* Description    :  draw string 
* Input          : INT8U *dest : destination buffer
                      INT8U num  : string index
                      INT16S x    : x position
                      INT16S y    : y position
                      INT16U width : buffer size
                      INT8U color : color
* Output         : none                                            
* Return         : int : 
*******************************************************************************/
int fontDrawString(INT8U *dest,INT8U num,INT16S x,INT16S y,INT16U width,INT8U color)
{
	INT8U charcnt,i;
	INT16U lastCharW;
#if HAL_CFG_EN_LINE
    INT8U *charCache = halSysMemLineCache;
#else
    INT8U *charCache = fontCtrl.charCache;
#endif

	if(fontGetString(num,fontCtrl.strCache)<0)
		return -1;
	charcnt = fontCtrl.strInfo.number;
    if(charcnt>128)
		charcnt = 128;

    lastCharW = 0;
	for(i=0;i<charcnt;i++)
	{
		if(fontGetCharData(fontCtrl.strCache[i]&0x7fff,charCache)<0)
		{
			x+=fontCtrl.charInfo.width;  
			continue;
		}
		if((x+fontCtrl.charInfo.width)>width)
			break;
		
		//fontDrawChar(dest,charCache,x,y,fontCtrl.charInfo.width,fontCtrl.charInfo.height,width,color);
        if(fontCtrl.strCache[i]&0x8000)
        {
			fontCtrl.charInfo.height>>=2;
			//if(fontCtrl.charInfo.height>8)
			//	fontCtrl.charInfo.height = 8;
			fontDrawChar(dest,charCache,x/*-lastCharW*/,y,fontCtrl.charInfo.width,fontCtrl.charInfo.height,width,color);
			fontCtrl.charInfo.width = 0;
        }
		else
		{
			fontDrawChar(dest,charCache,x,y,fontCtrl.charInfo.width,fontCtrl.charInfo.height,width,color);
			lastCharW = fontCtrl.charInfo.width;
			x+=fontCtrl.charInfo.width/*+CH_INV_W*/; 
		}
		

	}
	(void)lastCharW;

	return 0;
}


INT32U resGetString(INT8U id,INT16U *width,INT16U *height)
{
	int temp;
	if(id>=fontCtrl.strMax)
		return 0;
	if(id!=fontCtrl.curStr)
	{
		nv_read(fontCtrl.addrIndex+fontCtrl.curOffset+(id+1)*8,&fontCtrl.strInfo,8);
		fontCtrl.curStr=id;
	}
	if(fontCtrl.strInfo.number>128)
		fontCtrl.strInfo.number=128;
    	temp = fontCtrl.strInfo.number<<1;
	if(temp&0x0f)
		temp = (temp&(~0x0f))+0x10;
	nv_read(fontCtrl.addrIndex+fontCtrl.curOffset+fontCtrl.strInfo.offset,fontCtrl.strCache,temp);
	if(width)
		*width = fontCtrl.strInfo.width;
	if(height)
		*height = fontCtrl.strInfo.height;
	return fontCtrl.strInfo.number;
}
INT8U* resGetChar(INT8U id,INT8U num,INT16U *width,INT16U *height,uint8* special)
{
#if HAL_CFG_EN_LINE
    INT8U *charCache = halSysMemLineCache;
#else
    INT8U *charCache = fontCtrl.charCache;
#endif
	if(id>=fontCtrl.strMax)
		return NULL;
	if(id!=fontCtrl.curStr)
	{
		resGetString(id,NULL,NULL);
	}
	if(num>=fontCtrl.strInfo.number)
		return NULL;
	if(fontGetCharData(fontCtrl.strCache[num]&0x3fff,charCache)<0)
		return NULL;
	if(fontCtrl.strCache[num]&0x8000)
	{
		if(width)
			*width = fontCtrl.charInfo.width;
		if(height)
			*height = (fontCtrl.charInfo.height>>2)+1;
		if(fontCtrl.strCache[num]&0x4000)
		{
			if(special)
				*special=2;
			if(height)
				*height = fontCtrl.charInfo.height;
		}
		else
		{
			if(special)
				*special=1;
			if(height)
				*height = (fontCtrl.charInfo.height>>2)+1;
		}
	}
	else
	{
		if(width)
			*width = fontCtrl.charInfo.width;
		if(height)
			*height = fontCtrl.charInfo.height;
		if(special)
			*special=0;
	}
	return charCache;
}





