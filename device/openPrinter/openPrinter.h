#ifndef  _OPENPRINTER_H
#define  _OPENPRINTER_H
#include "../../hal/inc/hal.h"   
//---------------------------------------------------------------------------
#define  PRINTER_W					384
/*
 * 返回: 温度： -20 ~ 70，单位摄氏度
*/
extern int open_printer_tm_get();
/*
 * buf: 图片数据
 * w: 	图片宽度 点
 * h: 	图片高度 点
 * batteryValue:  当前电池电压 单位mV
 * 返回值：状态
*/
extern int open_printer_print(u8* buf,u16 w,u16 h,u16 batteryValue);
extern int open_printer_bluetooth_print(u16 batteryValuemV);
extern int open_printer_bluetooth_print_dot(u8 *Buf, u32 w, u32 h, u16 batteryValue);
extern int open_printer_bluetooth_print_gray(u8 *Buf, u32 w, u32 h, u16 batteryValue);
extern void open_printer_bluetooth_print_end(void);
/*
 * PictrueType：设置图片类型，0：人像，1：风景   
*/
extern void open_printer_set_paper_type(u8 PaperType);
/*
 * Darkness：当前设置蓝牙点阵打印的黑度，     0 - 255，默认值为100
*/
extern void open_printer_set_darkness_btgray(u8 Darkness);
/*
 * Darkness：当前设置的蓝牙灰度打印的黑度，     0 - 255，默认值为100
*/
extern void open_printer_set_darkness_btdot(u8 Darkness);
/*
 * Darkness：当前设置的本地图片打印黑度，     0 - 255，默认值为100
*/
extern void open_printer_set_darkness(u8 Darkness);
/*
 * 设置打印清晰度：
 *  1 - 5
*/
extern void open_printer_set_clarity(u8 type);
/*
 * 返回支持的当前最大的色阶等级
 * 32,16,8,4,2
*/
extern int open_printer_get_graylevel(void);
/*
 * 返回是否校验成功
 * 0：未校验， 1：已校验成功
*/
extern  int open_printer_get_credit(void);
/*
 * 设置图片对比度类型：
 *  0 - 4
 *  0：原图
 *  1：调整对比度到基础饱和
 *  2：基础饱和对比基础上，增加99.5%点的对比权重
 *  3：基础饱和对比基础上，增加15%的对比度
 *  4：自动矩阵对比度（计算需要3 - 6秒）
*/
extern void open_printer_set_contrast(u8 Type);
/*
 * 计算打印头温度
 *  Adc：TM对应ADC检测的数据
 *  AdcMax：对应ADC检测的最大值，10位ADC对应为 1024
 *  nR100：对应TM检测下拉电阻的欧姆电阻，单位欧姆，例如10K为 10000
*/
extern int open_printer_tm(int Adc,int AdcMax,int nR100);//计算打印头温度
//---------------------------------------------------------------------------
#endif