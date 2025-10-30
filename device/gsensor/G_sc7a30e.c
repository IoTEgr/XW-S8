#include "gsensor.h"

#if 0
#define  SC7A30E_DEFAULT_IIC_PULL_UP   0 	// 0: close 50K IIC pull up , 1: open 50K IIC pull up

#define  SC7A30E_SLAVE_ADDR     0x3a
#define  SC7A30E_ID                0x11

#define  SC7A30E_MICRO_MOTION    0x40

const static unsigned char sc7a30e_wakeup_level_table[G_LEVEL_MAX] = {0,100,50,10};
const static unsigned char  sc7a30e_active_level_table[G_LEVEL_MAX] = {0,0x7e,0x7a,0x50};
static int activeTime=0;

static int SC7A30E_Init(void)
{
     unsigned char temp,i,wakeup;
     gsensor_iic_enable();

	 for(i=0;i<3;i++)
	 {
         gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x0f,&temp);
		 if(temp == SC7A30E_ID)
		 	break;
	 }
     if(i>=3)
     {
	 	 gsensor_iic_disable();
		 return -1;
     }
	 gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x39,&temp);
	 if(temp&0x40)
         wakeup = 1;
	 else
	 	wakeup = 0;
	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x22,0x00);
	 
	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x24,0x80);
	 
	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x21,0x0d);
	 
	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x20,0x5f);
	 
	 gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x26,&temp);

	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x25,0);	
	 
	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x21,0x09);
	 
	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x23,0x80);

	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x38,0x00);

	 gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x3a,0x7f);//set threshold

	 gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x57,&temp);

	 #if (0 == SC7A30E_DEFAULT_IIC_PULL_UP)
	 if(!(temp&0x04))	// if open otp iic pull up
	 {
	 	gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x1e,0x05);
		temp|=0x04;	
		gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x57,temp);// close otp iic pull up
		gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x1e,0x15);
	 }
	 #else
	 if(temp & 0x04)	//if not open otp iic pull up
	 {
	 	gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x1e,0x05);
		temp &= (~0x04);	
		gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x57,temp);	// open otp iic pull up
		gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x1e,0x15);		// set otp default open IIC pull up 
	 }
	 #endif

     activeTime = -8000;
     gsensor_iic_disable();
	return wakeup;
}

static void SC7A30E_SetInt(unsigned char level,unsigned char active)
{
	 unsigned char temp;

	 gsensor_iic_enable();
    
     if(level == 0)
     {
	 	   
	 	   
	 	   gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x38,0x00);

		   gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x3a,0x7f);//set threshold
		   
           gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x39,&temp);
		//   gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x20,0x00);
     }
	 else
	 { 	 
	 	  
		  gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x25,(!active)<<1);	 // int  active
		 
		  gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x3a,level);
		 
		  gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x3b,0x03);
		 
		  gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x3c,0x7f);
		 
		  gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x38,0x15);
		 
		  gsensor_iic_write(SC7A30E_SLAVE_ADDR,0x22,0x80);

		  gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x39,&temp);
	 }

     if(temp&0x40)
     {
	 	activeTime = XOSTimeGet();
     }
	 gsensor_iic_disable();
}

static int SC7A30E_GetInt(void)
{
	unsigned char temp,wakeup;

	if((activeTime+8000)>XOSTimeGet())  // int status wait 5-second
		return 0;
	
	gsensor_iic_enable();
    gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x39,&temp);
	if(temp&0x40)
	{
		 activeTime = XOSTimeGet();
         wakeup = 1;
	}
	else
	{
	 	activeTime = 0;
	 	wakeup = 0;
	}
  
	gsensor_iic_disable();

	 return wakeup;
}
static int SC7A30E_GetXYZ(int *x,int *y,int *z)
{
	short x_axis,y_axis,z_axis;
	unsigned char temp;
	
	gsensor_iic_enable();
	gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x29,&temp);
	x_axis = (temp&0x80)?(0xff00|temp): temp;
//	gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x28,&temp);
//	x_axis = (x_axis<<8)|temp;

	gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x2b,&temp);
	y_axis = (temp&0x80)?(0xff00|temp): temp;
//	gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x2a,&temp);
//	y_axis = (y_axis<<8)|temp;

	gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x2d,&temp);
	z_axis = (temp&0x80)?(0xff00|temp): temp;
//	gsensor_iic_read(SC7A30E_SLAVE_ADDR,0x2c,&temp);
//	z_axis = (z_axis<<8)|temp;

	if(x)
		*x = x_axis;
	if(y)
		*y = y_axis;
	if(z)
		*z = z_axis;

	gsensor_iic_disable();

	return 0;
}
static int SC7A30E_MircoMotionCheck(int level)
{
	int x,y,z;
	int dx,dy,dz;

    if((activeTime+8000)>XOSTimeGet()) 
		return 1;
	if(level==0)
		return 0;
	SC7A30E_GetXYZ(&x,&y,&z);

	hal_sysDelayMS(20);
	SC7A30E_GetXYZ(&dx,&dy,&dz);

	if((abs(dx-x)>SC7A30E_MICRO_MOTION)||\
		(abs(dy-y)>SC7A30E_MICRO_MOTION)||\
		(abs(dz-z)>SC7A30E_MICRO_MOTION))
		return 1;
	else
		return 0;
}
static int SC7A30E_IOCtrl(int handle,int op,int para)
{
	int level;
    switch(op)
    {
		case G_IOCTRL_DISABLE : SC7A30E_SetInt(0,1);break;
		case G_IOCTRL_WAKEUPSET : 
			   level = sc7a30e_wakeup_level_table[para];
			   SC7A30E_SetInt(level,1);
			   break;
		case G_IOCTRL_WAKEUPGET :
			   return 0;
			   break;
		case G_IOCTRL_ACTIVESET:
               
			   level = sc7a30e_active_level_table[para];
			   SC7A30E_SetInt(level,1);
			   break;
		case G_IOCTRL_ACTIVEGET:
			   
			   return SC7A30E_GetInt();
			   break;
		case G_IOCTRL_ACTIVECHK:
			   return SC7A30E_MircoMotionCheck(para);
			   break;
    }

	return 0;
}


G_Sensor_OP_T sc7a30e=
{
	.name = "SC7A30E",
	.init = SC7A30E_Init,
	.readxyz = SC7A30E_GetXYZ,
	.ioctrl = SC7A30E_IOCtrl,
};

#endif

