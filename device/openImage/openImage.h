#ifndef  _OPENIMAGE_H
#define  _OPENIMAGE_H
#include "../../hal/inc/hal.h"   
extern void open_image_gray_level(int Type,u8 *image, int width, int height,int Level);
extern void openimage_credit_write(unsigned char KeyBuf[256]);
extern void openimage_id_get(unsigned char IDBuf[256]);
extern int  open_image_credit_check(void);
extern void open_image_white_add(u8 *image, int width, int height,int Add);
extern void open_image_contrast_basic(unsigned char *image, int width, int height);
extern void open_image_contrast_basic_level(int Level, unsigned char *image, int width, int height);
extern void open_image_contrast_add(unsigned char *image, int width, int height, int contrast);
extern void open_image_contrast_auto_matrix(unsigned char *image, int width, int height);
extern void open_image_handle(int Type, int Level, u8 *image, int width, int height);
/*
extern void open_image_handle_close(int Type, int Level,u8 *image, int width, int height);
extern void open_image_handle_far(int Type, int Level,u8 *image, int width, int height);
extern void open_image_handle_middle(int Type, int Level,u8 *image, int width, int height);
extern void open_image_handle_ex(int Type, int Level, int Add, u8 *image, int width, int height);
extern void open_image_handle_camera(int Type, int Level, u8 *image, int width, int height);
extern void open_image_handle_picture(int Type, int Level, u8 *image, int width, int height);
 * */
#endif