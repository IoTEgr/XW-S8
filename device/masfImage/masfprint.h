#ifndef SPI_PRINT_H
#define SPI_PRINT_H
#include "../printer/printer.h"
#include "masfImage.h"
// #include "assert.h"
#include "../../ax32_platform_demo/application.h"

S32 open_image_print_masf(u8* buf,u16 w,u16 h,u16 batteryValue_mv,u32 paper_feed_to_cutter_line, u32 label_feed_to_cutter_line);
void print_density_set( u8 value);
s32 open_printer_bluetooth_print_masf(u16 batteryValue_mv,u32 paper_feed_to_cutter_line, u32 label_feed_to_cutter_line);
#endif