/****************************************************************************
        ***           ***                      MAXLIB-GRAPHIC
       ** **        ** **
      **  **       **   **    THE MAXLIB FOR IMAGE SHOW PROCESS
     **    **     **    **                    
    **     **    **      **           MAX COMMON MANAGEMENT
   **       **  **       **
  **         ** **        **           (C) COPYRIGHT 2016 MAX
**           ***           **    

* File Name   : common.c
* Author      : Mark.Douglas 
* Version     : V0100
* Date        : 05/25/2016
* Description : This file for maxlib common managemant
*               
* History     :
* 2016-05-25  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/
#include "misc.h"

#define NUM_STR_LEN           16
static char publicString[NUM_STR_LEN];

/*******************************************************************************
* Function Name  : CountToString
* Description    : change count to string type
* Input          : char *string : destination
*                    u16_t count1 : count 1
                     u16_t count2 : count 2
* Output         : string
* Return         : 0
*******************************************************************************/
s32_t CountToString(char *string,u16_t count1,u16_t count2)
{
	string[0] = (count1/100)%10+'0';
	string[1] = (count1/10)%10+'0';
	string[2] = (count1%10)+'0';

	string[3] = '/';

	string[4] = (count2/100)%10+'0';
	string[5] = (count2/10)%10+'0';
	string[6] = (count2%10)+'0';	

	return 0;
}
/*******************************************************************************
* Function Name  : stringInster
* Description    : string inster
* Input          : char *des : destination
                      char *src : source
*                    u32_t offset : offset
                     u32_t len : length
* Output         : des
* Return         : 0
*******************************************************************************/
s32_t stringInster(char *des,char *src,u32_t offset,u32_t len)
{
	u32_t count;
	count = 0;
	des+=offset;
	while(*src)
	{
		*des++ = *src++;
		if(len)
		{
			count++;
			if(count>=len)
				break;
		}
	}
	return 0;	
}
/*******************************************************************************
* Function Name  : strTransform
* Description    : string transform
* Input          : char *str : string
                     u32_t len : length
* Output         : des
* Return         : 0
*******************************************************************************/
void strTransform(char *str,u32_t len)
{
	char temp;
	
	if(str == (void *)0)
		return ;
	if(len <= 1)
		return ;
	
	temp = str[0];
	str[0] = str[len-1];
	str[len-1] = temp;
	strTransform(&str[1],len-2);
}
/*******************************************************************************
* Function Name  : HexToString
* Description    : string hex
* Input          : u32_t value : value
                     u32_t lenght : length
* Output         : des
* Return         : char *
*******************************************************************************/
char *HexToString(u32_t value,u8_t lenght)
{
    char *string;
    u8_t i,temp;
    
    string = publicString;
    
    for(i=0;i<lenght;i++)
    {
        temp = (value>>((7-i)*4))&0x0f;
        if(temp<10)
           string[i] = '0'+temp;
        else
           string[i] = 'A'+temp-10;   
        
    }
    string[i] = 0;
    
    return string;    
}

/*******************************************************************************
* Function Name  : NumberToString
* Description    : string number
* Input          : u32_t value : value
                     u32_t lenght : length
* Output         : des
* Return         : char *
*******************************************************************************/
char *NumberToString(u32_t value,u8_t lenght)
{
	char *string;
	u8_t i;
	u32_t tempvalue;
	
	string = publicString;

    tempvalue = value;
	for(i=0;i<NUM_STR_LEN-1;i++)
	{
		if(tempvalue == 0)
		   break;
		string[i] = '0'+tempvalue%10;
		tempvalue = tempvalue/10;
	}

	for(;i<lenght;i++)
	{
		string[i] = '0';
	}

	string[i] = 0;
	
	strTransform(string,i);

	return string;
}
