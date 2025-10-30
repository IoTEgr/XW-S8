#ifndef  _MASF_IMAGE_H
#define  _MASF_IMAGE_H
#include "../../hal/inc/hal.h"

#include "../../xos/xos.h"

#include "../openImage/openImage.h"

#include "../../ax32xx/inc/ax329x.h"

extern int masf_image_credit_check(void);//sg 调用此函数检查授权

void masf_cbf6_process_datain(u8 t, u8 epxout, u8 *_ptxbuf);
void masf_cbf6_process_dataout(u8 t, u8 epxout, u8 *_prxbuf);
#endif  //_MASF_IMAGE_H