#include "gsensor.h"

#if 0
#define  GMA301_SLAVE_ADDR     0x30
#define  GMA301_ID                0x55
 
#define  GMA301_MICRO_MOTION    256

const static unsigned char  gma301_wakeup_level_table[G_LEVEL_MAX] = {0,80,80,50};
const static unsigned char  gma301_active_level_table[G_LEVEL_MAX] = {0,110,80,50};
//=============================================================================
static int GMA301_Init(void)
{
	unsigned char  wakeup,i,data;
 
    gsensor_iic_enable();
	

	for(i=0;i<5;i++)
	{
		gsensor_iic_read(GMA301_SLAVE_ADDR,0x12,&data);
		if(data==GMA301_ID)  // id error
		{
			break;
		}
	}
	if(i>=5)
	{
		gsensor_iic_disable();
		return -1;
	}
	gsensor_iic_read(GMA301_SLAVE_ADDR,0x1C,&data);   // enable int status read
     
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1D,&data);  // read int status
    if((data&0x20) == 0x20)
		wakeup = 1;
	else
		wakeup = 0;
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x21,0x52);  // reset 
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x02);  // step -0 ,enable data write
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x12);  // step -1 ,download OTP
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x02);  // step -2 ,download OTP
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x82);  // step -3 ,download OTP
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x02);  // step -4 ,download OTP
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x1F,0x28);  // enable osc
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x0C,0x8F);  // enable x,y,z high speed 
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x06);  // start g-sensor convert

	gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x02);  // step -0 ,enable data write
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x11,0x05|2);  // active high ,enable
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x38,0xFF);  // set micro motion throd
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x39,0x30); // set micro motion
    
//    gsensor_iic_write(GMA301_SLAVE_ADDR,0x0F,0x00);
    
//    gsensor_iic_write(GMA301_SLAVE_ADDR,0x0E,0x00);
    
//    gsensor_iic_write(GMA301_SLAVE_ADDR,0x1F,0x28);
    
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x12,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x13,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x14,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x15,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x16,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x17,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x18,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x19,&data);
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x21,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x00,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1F,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x0C,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x11,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x38,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x39,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x0F,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1F,&data);
    
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x1F,0x38);   // enable micro motion
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1C,&data);   // clear int
     
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1D,&data);  // clear int
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x0E,0x00);   // disable all int

    gsensor_iic_disable();
	return wakeup;
    
}

static void GMA301_SetMovementInt(unsigned char level,unsigned char active)
{
    unsigned char  data;

	gsensor_iic_enable();

	if(level == 0)
	{
		gsensor_iic_write(GMA301_SLAVE_ADDR,0x11,0x04|((active&1)<<1));   

		gsensor_iic_write(GMA301_SLAVE_ADDR,0x39,0xff);	

		gsensor_iic_write(GMA301_SLAVE_ADDR,0x1F,0x00);
		
		gsensor_iic_read(GMA301_SLAVE_ADDR,0x1C,&data);  // clear int
    
        gsensor_iic_read(GMA301_SLAVE_ADDR,0x1D,&data);  // clear int		
        
		gsensor_iic_write(GMA301_SLAVE_ADDR,0x0E,0x00);   // disable all int

		gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x02);   // standby
		/*while(1)
		{
			data=1;
			gsensor_iic_read(GMA301_SLAVE_ADDR,0x0E,&data);  // clear int		
			if(data==0)
				break;
		}*/
		
//        gsensor_iic_write(GMA301_SLAVE_ADDR,0x21,0x52);  // reset 
        		
		gsensor_iic_disable();

		return ;
	}
	
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x0F,0x00);   
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x0E,0x00);   // disable all int

    gsensor_iic_write(GMA301_SLAVE_ADDR,0x1F,0x28);  // enable micro motion
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1C,&data);  // clear int
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1D,&data);  // clear int
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x11,0x05|((active&1)<<1));   // enable active low
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x38,0xff);
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x39,level);			
        
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x12,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x13,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x14,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x15,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x16,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x17,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x18,&data);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x19,&data);
    
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x1f,0x38);   //enable mircro motion 
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1C,&data);  // clear int
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1D,&data);  // clear int
	
    gsensor_iic_write(GMA301_SLAVE_ADDR,0x0e,0x1c);  // enable sensor int

	gsensor_iic_write(GMA301_SLAVE_ADDR,0x00,0x06);   // enable sensor active

	gsensor_iic_disable();
        
}
/*
static void GMA301_ClearDataFlag(void)
{
	unsigned char  data;

	gsensor_iic_enable();
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1C,&data);
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1D,&data); 
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x12,&data);
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x13,&data); 

	gsensor_iic_disable();
}
*/
static int GMA301_GetInt(void)
{
    unsigned char value,sts=0;

	gsensor_iic_enable();	

	gsensor_iic_read(GMA301_SLAVE_ADDR,0x1C,&value);  // clear int
	if(value == 0xaa)
	    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1D,&value);
	else
	{
		gsensor_iic_disable();
		return 0;
	}
    if((value&0x20))
      sts = 1;
    else
      sts = 0;
	  
//	GMA301_ClearDataFlag();

	gsensor_iic_disable();

	return sts;/**/

    return 0;
}

static int GMA301_GetXYZ(int *x,int *y,int *z)
{
    short x_axis,y_axis,z_axis;
    unsigned char data;

	gsensor_iic_enable();

	gsensor_iic_read(GMA301_SLAVE_ADDR,0x1C,&data);   // enable int status read
     
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x1D,&data);  // read int status	
    
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x14,&data);
	x_axis = data;
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x15,&data);
	x_axis = data|(x_axis<<8);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x16,&data);
	y_axis = data;
   gsensor_iic_read(GMA301_SLAVE_ADDR,0x17,&data);
	y_axis = data|(y_axis<<8);
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x18,&data);
	z_axis = data;
    gsensor_iic_read(GMA301_SLAVE_ADDR,0x19,&data);
	z_axis = data|(z_axis<<8);
          
    if(x)
      *x = x_axis&0x1ff;
    if(y)
      *y = y_axis&0x1ff;
    if(z)
      *z = z_axis&0x1ff;   

	gsensor_iic_disable();
	  
	return 0;
}
static int GMA301_MircoMotionCheck(int level)
{
	int x,y,z;
	int dx,dy,dz;

	if(level==0)
		return 0;

	GMA301_GetXYZ(&x,&y,&z);

	hal_sysDelayMS(20);
	GMA301_GetXYZ(&dx,&dy,&dz);

	if((abs(dx-x)>GMA301_MICRO_MOTION)||\
		(abs(dy-y)>GMA301_MICRO_MOTION)||\
		(abs(dz-z)>GMA301_MICRO_MOTION))
		return 1;
	else
		return 0;
}
static int GMA301_IOCtrl(int handle,int op,int para)
{
	int level;
    switch(op)
    {
		case G_IOCTRL_DISABLE : GMA301_SetMovementInt(0,1);break;
		case G_IOCTRL_WAKEUPSET : 
			   level = gma301_wakeup_level_table[para];
			   GMA301_SetMovementInt(level,1);
			   break;
		case G_IOCTRL_WAKEUPGET :
			   return 0;
			   break;
		case G_IOCTRL_ACTIVESET:
               
			   level = gma301_active_level_table[para];
			   if(level == 0)
			   {
			   	   GMA301_SetMovementInt(120,1); // mark modify,20180323.if wakeup is on and active is turned off,it will restart
			   	   break;
			   }
			   GMA301_SetMovementInt(level,1);
			   break;
		case G_IOCTRL_ACTIVEGET:
			   
			   return GMA301_GetInt();
			   break;
		case G_IOCTRL_ACTIVECHK:
			   return GMA301_MircoMotionCheck(para);
			   break;
    }

	return 0;
}


G_Sensor_OP_T gma301=
{
	.name = "GMA301",
	.init = GMA301_Init,
	.readxyz = GMA301_GetXYZ,
	.ioctrl = GMA301_IOCtrl,
};

#endif

