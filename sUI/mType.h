#ifndef  __MTYPE_H
#define  __MTYPE_H

#include "../hal/inc/hal.h"   
#include "debug.h"
#include "string.h"
#include "../ax32xx/inc/typedef.h"


/*typedef   char  int8;
typedef   unsigned char  uint8;
typedef   short  int16;
typedef   unsigned short  uint16;
typedef   int       int32;
typedef   unsigned int      uint32;
typedef   signed long long    int64;
typedef   unsigned long long  uint64;
typedef enum
{
	false=0,
	true=!false
}bool;*/
#define  NULL          ((void *)0)


#define _ALIGN(n)

#define  toSmall(n)       ( ((n)>='A'&&(n)<='Z')?((n)-'A'+'a'):(n))
#define  toBig(n)           ( ((n)>='a'&&(n)<='z')?((n)-'a'+'A'):(n))

#define MakeTag(a,b,c,d) ((a)|((b)<<8)|((c)<<16)|((uint32)(d)<<24))

#define  MAX(a,b)       ((a)>(b)?(a):(b))
#define  MIN(a,b)        ((a)>(b)?(b):(a))
#define  CLIP(x,a,b)     MIN(MAX(x,a),b)

#define ARRY_LEN(a)    (sizeof(a)/sizeof(a[0]))

#endif

