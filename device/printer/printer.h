#ifndef  _PRINTER_H
#define  _PRINTER_H
#include "../../hal/inc/hal.h"   
#include "../../xos/xos.h"
//#include "../openImage/printer_print.h"
#include "../openImage/openImage.h"
#include "../openPrinter/openPrinter.h"
#include "../masfImage/masfprint.h"


void printer_init();

//==ret : 0 is ok , 8: no paper , 9: tm hot , 10: w h err
//==level    0~4  max~min
//==print mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery 0~4
u8 printer_print(u8* buf,u16 w,u16 h,s16 level,u8 print_mode,u8 batteryValue);


//==ret: 0 :is ok , 1: mem err ,2: mem err , 3: jpg dec err , 8: no paper ,9 : tm hot
//==fd : jpeg file ==
//==level:  0~4   max~min
//==print_mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery : 0~4
u8 printer_jpeg(int fd,s16 level,u8 print_mode,u8 batteryValue);


//==ret: 0 :is ok , 1: mem err ,2: mem err , 3: jpg dec err , 8: no paper ,9 : tm hot ,10: w ,h err
//==jpeg: jpeg buf ==
//==level:  0~4   max~min
//==print_mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery : 0~4
u8 printer_jpeg_buf(u8 *jpeg,s16 level,u8 print_mode,u8 batteryValue,u32 jpg_size);

int printer_app_buf(u8 *buf,int height);

u8 printer_dot_matrix_handle(u8 *ybuf,u16 w, u16 h);


extern void PRINTER_LATCH_HIGH();  		
extern void PRINTER_LATCH_LOW();
extern void PRINTER_CLK_HIGH();
extern void PRINTER_CLK_LOW();
extern void PRINTER_DAT_LOW();
extern void PRINTER_DAT_HIGH();  	
extern int  TIMER_REST();						
extern void TIMER_STOP();
extern u32  TIMERTICKCOUNT();//		ax32xx_timer3TickCount()
extern void MOTO_WORK(u8 x,u8 y,u8 z,u8 w);
extern void PRINTER_STB_HIGH();
extern void PRINTER_STB_LOW();
extern void PRINTER_STB_HIGH();
extern void PRINTER_STB_LOW();
extern void PRINTER_STB2_HIGH();
extern void PRINTER_STB2_LOW();
extern int  open_printer_tm_get();
extern u32  printer_get_paper();

extern void PRINTER_DAT_LOW_MASF();
extern void PRINTER_DAT_HIGH_MASF();
extern void printer_delay_masf(u32 n);
extern s32 printer_tem_get_masf();
extern void btcomService_masf(void);

#endif

