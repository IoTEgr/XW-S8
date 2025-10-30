#ifndef __USERINTERFACE_H_
#define __USERINTERFACE_H_


typedef void (*actionType)(u8 *, u8 *, u8);
typedef bool (*actionRun)(void *, u8 *);

typedef struct UserInterface  
{
	u8 cnt;
	u8 action;
	u8 *preFrameBuf;
	actionType actionFunc;
	actionRun run;
} UserInterface;

extern UserInterface changeUserInterface;
extern uint8 jump_process_type;

enum{
	UPPER2BOTTOM,
	BOTTOM2UPPER,
	LEFT2RIGHT,
	RIGHT2LEFT,
	LEFTBOTTOM2RIGHTUPPER,
	LEFTUPPER2RIGHTBOTTOM,
	CIRCLE_INSIDE2OUTSIDE,
	CIRCLE_OUTSIDE2INSIDE,
	SQUARE_INSIDE2OUTSIDE,
	SQUARE_OUTSIDE2INSIDE,
};

enum
{
	JUMP_PROCESS_TYPE_NONE,
	CIRCLE_OPEN,
	CIRCLE_CLOSE,
};

extern void animationInit(UserInterface *objName);
extern bool animationRun(void *objName, u8 *srcBuf);
extern void animationUnInit(UserInterface *objName);

extern void type_function_circle(INT32U buff);



#define ANIMATION_CNT (/*10*/6)


#define ANIMATION(name, action)\
{\
	UserInterface name = {ANIMATION_CNT, action, NULL, NULL, animationRun};\
	animationInit(&name);\
}


int CLAHE (u8* pImage,u32 uiXRes,u32 uiYRes,u8 Min,u8 Max,u32 uiNrX,u32 uiNrY,u32 uiNrBins,float fCliplimit);
void BayerDithering(u8* ybuf,u16 w, u16 h);
void FloydSteinbergDithering(u8* ybuf,u16 w, u16 h,u8 thred);

void y_process(u8*buf,u32 size);


void warpHandle(u8* ybuf,u8*uvbuf,u16 buf_w,u16 buf_h, u16 *r);
void trilateral_16_window(u8* ybuf,u16 *uvbuf,u16 w,u16 h);
void stream_10_window(u8* ybuf,u16 *uvbuf,u16 w,u16 h);
void rismatic_multiwindow(u8* ybuf,u16 *uvbuf,u16 w,u16 h);
void hexagon_multiwindow(u8* ybuf,u16 *uvbuf,u16 w,u16 h);
#endif
  
