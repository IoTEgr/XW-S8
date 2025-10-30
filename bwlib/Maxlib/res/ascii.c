#include "R.h"



extern const unsigned char *ascii_default_table[];
//extern const unsigned char *ascii_num2_table[];
extern const unsigned char *ascii_num3_table[];
//extern const unsigned char *ascii_num4_table[];
//extern const unsigned char *ascii_num5_table[];

const unsigned char *ascii_draw_get(char c,unsigned char *width,unsigned char *heigth,unsigned char font)
{
   const unsigned char *table;
   unsigned char index;
   if(c<32 || c == 34 || c>126)
      return ((void *)0);
   if(c<34)
      index = c-32;
   else
      index = c-33;
   font&=0x0f;
   /*if(font == R_FONT_NUM2)
      table = ascii_num2_table[index];   
   else if(font == R_FONT_NUM3)
   	  table = ascii_num3_table[index];
   else if(font == R_FONT_NUM4)
   	  table = ascii_num4_table[index];
//   else if(font == R_FONT_NUM5)
//   	  table = ascii_num5_table[index];
   else*/
   	if(font == R_FONT_NUM3)
   	  table = ascii_num3_table[index];
	else
      table = ascii_default_table[index];   
   
   if(width)
      *width = table[0];
   if(heigth)
      *heigth = table[1];
   return &table[2];    
}