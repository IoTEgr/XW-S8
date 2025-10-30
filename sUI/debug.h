#ifndef __DEBUG_H
#define __DEBUG_H
#include "stdio.h"


#define DEBUG_LEVEL_NONE                    0
#define DEBUG_LEVEL_NORMAL               1
#define DEBUG_LEVEL_WARNING             2
#define DEBUG_LEVEL_ERROR                   3


#define DEBUG_LEVEL    DEBUG_LEVEL_ERROR


extern void uart_Printf(const char *pszStr, ...);
#if(DEBUG_LEVEL>=DEBUG_LEVEL_ERROR)
#define debug_err(fmt,...)    deg_Printf("[ERR]:%s,%d  "fmt"",__FUNCTION__,__LINE__,##__VA_ARGS__)//uart_Printf("[ERR]:%s,%d  "fmt"",__FUNCTION__,__LINE__,##__VA_ARGS__)
#define debug_wrn(fmt,...)    deg_Printf("[WRN]:%s,%d  "fmt"",__FUNCTION__,__LINE__,##__VA_ARGS__)//uart_Printf("[WRN]:%s,%d  "fmt"",__FUNCTION__,__LINE__,##__VA_ARGS__)
#define debug_msg(fmt,...)    deg_Printf(fmt,##__VA_ARGS__)//uart_Printf(fmt,##__VA_ARGS__)
#elif(DEBUG_LEVEL>=DEBUG_LEVEL_WARNING)
#define debug_err(fmt,...)      
#define debug_wrn(fmt,...)    deg_Printf("[WRN]:%s,%d  "fmt"",__FUNCTION__,__LINE__,##__VA_ARGS__)//uart_Printf("[WRN]:%s,%d  "fmt"",__FUNCTION__,__LINE__,##__VA_ARGS__)
#define debug_msg(fmt,...)    deg_Printf(fmt,##__VA_ARGS__)//uart_Printf(fmt,##__VA_ARGS__)
#elif(DEBUG_LEVEL>=DEBUG_LEVEL_NORMAL)
#define debug_err(fmt,...)      
#define debug_wrn(fmt,...)    
#define debug_msg(fmt,...)    deg_Printf(fmt,##__VA_ARGS__)//uart_Printf(fmt,##__VA_ARGS__)
#else
#define debug_err(fmt,...)      
#define debug_wrn(fmt,...)    
#define debug_msg(fmt,...)   
#endif




#endif


