#include "gsensor.h"

#if 0
#define  DA280_IC      1   // 0: is not DA280 , 1: is DA280

#define  DA380_SLAVE_ADDR    0x4e

#define  DA380_ID                0x13
 
#define  DA380_MICRO_MOTION    80
                                                       
#define NSA_REG_SPI_I2C                 0x00
#define NSA_REG_WHO_AM_I                0x01
#define NSA_REG_ACC_X_LSB               0x02
#define NSA_REG_ACC_X_MSB               0x03
#define NSA_REG_ACC_Y_LSB               0x04
#define NSA_REG_ACC_Y_MSB               0x05
#define NSA_REG_ACC_Z_LSB               0x06
#define NSA_REG_ACC_Z_MSB               0x07
#define NSA_REG_MOTION_FLAG				0x09 
#define NSA_REG_G_RANGE                 0x0f
#define NSA_REG_ODR_AXIS_DISABLE        0x10
#define NSA_REG_POWERMODE_BW            0x11
#define NSA_REG_SWAP_POLARITY           0x12
#define NSA_REG_FIFO_CTRL               0x14
#define NSA_REG_INTERRUPT_SETTINGS1     0x16
#define NSA_REG_INTERRUPT_SETTINGS2     0x17
#define NSA_REG_INTERRUPT_MAPPING1      0x19
#define NSA_REG_INTERRUPT_MAPPING2      0x1a
#define NSA_REG_INTERRUPT_MAPPING3      0x1b
#define NSA_REG_INT_PIN_CONFIG          0x20
#define NSA_REG_INT_LATCH               0x21
#define NSA_REG_ACTIVE_DURATION         0x27
#define NSA_REG_ACTIVE_THRESHOLD        0x28
#define NSA_REG_TAP_DURATION            0x2A
#define NSA_REG_TAP_THRESHOLD           0x2B
#define NSA_REG_CUSTOM_OFFSET_X         0x38
#define NSA_REG_CUSTOM_OFFSET_Y         0x39
#define NSA_REG_CUSTOM_OFFSET_Z         0x3a
#define NSA_REG_ENGINEERING_MODE        0x7f
#define NSA_REG_SENSITIVITY_TRIM_X      0x80
#define NSA_REG_SENSITIVITY_TRIM_Y      0x81
#define NSA_REG_SENSITIVITY_TRIM_Z      0x82
#define NSA_REG_COARSE_OFFSET_TRIM_X    0x83
#define NSA_REG_COARSE_OFFSET_TRIM_Y    0x84
#define NSA_REG_COARSE_OFFSET_TRIM_Z    0x85
#define NSA_REG_FINE_OFFSET_TRIM_X      0x86
#define NSA_REG_FINE_OFFSET_TRIM_Y      0x87
#define NSA_REG_FINE_OFFSET_TRIM_Z      0x88
#define NSA_REG_SENS_COMP               0x8c
#define NSA_REG_PULL_UP               	0x8f
#define NSA_REG_SENS_COARSE_TRIM        0xd1


const static unsigned char  da380_wakeup_level_table[G_LEVEL_MAX] = {0,40,40,40};
const static unsigned char  da380_active_level_table[G_LEVEL_MAX] = {0,100,60,40};
static int activeTime=0;

static int DA380_Init(void)
{
	unsigned char  i,wakeup,temp;

	gsensor_iic_enable();
    for(i=0;i<5;i++)
    {
		gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_WHO_AM_I,&temp);
		if(temp==DA380_ID)
			break;			
    }
	if(i>=5)
	{
		gsensor_iic_disable();
		return -1;
	}
	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_MOTION_FLAG,&temp);  // reset int status
	if(temp&0x04)
		wakeup = 1;
	else
		wakeup = 0;
	
	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_SPI_I2C,0x24);	// reset
	hal_sysDelayMS(5); // delay

	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_G_RANGE,0x02);	// reslution .02 : 10G

    gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_POWERMODE_BW,0x4e);  // low power mode,62HZ

	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_ODR_AXIS_DISABLE,0x06); // disable x,y,z.62HZ

	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INT_LATCH,0x86); // reset all int,int latch all the time

	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_ENGINEERING_MODE,0x83);
	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_ENGINEERING_MODE,0x69);
	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_ENGINEERING_MODE,0xbd);
	#if (1 == DA280_IC)
	//==add by ticru==
	u8 pull_up_state;
	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_PULL_UP,&pull_up_state);  // read pull up
	pull_up_state &= ~0x02;		// bit 1 set 0 will not pull up IIC
	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_PULL_UP,pull_up_state);  // write pull up
	//==end add by ticru==
	#endif
	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_SWAP_POLARITY,0x00);

	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INTERRUPT_SETTINGS1,0x00);  // diable all int	

	gsensor_iic_disable();
	activeTime = -8000;
	return wakeup;
}



static void DA380_SetMovementInt(unsigned char level,unsigned char active)
{
	unsigned char temp;
	
	gsensor_iic_enable();
    gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_MOTION_FLAG,&temp);
	if(temp&0x04)
		activeTime = XOSTimeGet();
	if(level == 0)
	{
		gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_POWERMODE_BW,0x4e);
        gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INT_PIN_CONFIG,0x01|(active&1));
        gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INTERRUPT_MAPPING1,0x00);  // diable int mapping
        gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INTERRUPT_MAPPING2,0x00);
		gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INTERRUPT_MAPPING3,0x00);
		
        gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INTERRUPT_SETTINGS1,0x00);  // diable all int		
		gsensor_iic_disable();

		return ;
	}
	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_POWERMODE_BW,0x0e);
    
    gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INTERRUPT_SETTINGS1,0x07);  // mapping active to int	
    gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_ACTIVE_DURATION,0x03); 
	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_ACTIVE_THRESHOLD,level);   // set active level

	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INT_PIN_CONFIG,0x00|(active&1));

	gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INTERRUPT_MAPPING1,0x04);  // diable all int	
	
	gsensor_iic_disable();
        
}
static int DA380_GetInt(void)
{
    unsigned char value,sts=0;

	if((activeTime+8000)>XOSTimeGet())  // int status wait 5-second
		return 0;
	gsensor_iic_enable();	

	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_MOTION_FLAG,&value);  // reset int status
	if(value&0x04)
		sts = 1;
	else
		sts = 0;
    if(sts)
    {
		activeTime = XOSTimeGet();
    }
//		gsensor_iic_write(DA380_SLAVE_ADDR,NSA_REG_INT_LATCH,0x8f); // reset all int,int latch all the time
	gsensor_iic_disable();

	return sts;

    return 0;
}

static int DA380_GetXYZ(int *x,int *y,int *z)
{
	short x_axis,y_axis,z_axis;
	unsigned char temp;
	
	gsensor_iic_enable();
	
	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_ACC_X_MSB,&temp);
	x_axis = temp;
	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_ACC_X_LSB,&temp);
	x_axis = (x_axis<<8)|temp;

	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_ACC_Y_MSB,&temp);
	y_axis = temp;
	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_ACC_Y_LSB,&temp);
	y_axis = (y_axis<<8)|temp;

	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_ACC_Z_MSB,&temp);
	z_axis = temp;
	gsensor_iic_read(DA380_SLAVE_ADDR,NSA_REG_ACC_Z_LSB,&temp);
	z_axis = (z_axis<<8)|temp;

	gsensor_iic_disable();

	if(x)
		*x = x_axis>>4;
	if(y)
		*y = y_axis>>4;
	if(z)
		*z = z_axis>>4;

	

    return 0;
}

static int DA380_MircoMotionCheck(int level)
{
	int x,y,z;
	int dx,dy,dz;

	if(level==0)
		return 0;

	DA380_GetXYZ(&x,&y,&z);

	hal_sysDelayMS(20);
	DA380_GetXYZ(&dx,&dy,&dz);

	if((abs(dx-x)>DA380_MICRO_MOTION)||\
		(abs(dy-y)>DA380_MICRO_MOTION)||\
		(abs(dz-z)>DA380_MICRO_MOTION))
		return 1;
	else
		return 0;
}


static int DA380_IOCtrl(int handle,int op,int para)
{
	int level;
    switch(op)
    {
		case G_IOCTRL_DISABLE : DA380_SetMovementInt(0,1);break;
		case G_IOCTRL_WAKEUPSET : 
			   level = da380_wakeup_level_table[para];
			   DA380_SetMovementInt(level,1);
			   break;
		case G_IOCTRL_WAKEUPGET :
			   return 0;
			   break;
		case G_IOCTRL_ACTIVESET:
               
			   level = da380_active_level_table[para];
			   DA380_SetMovementInt(level,1);
			   break;
		case G_IOCTRL_ACTIVEGET:
			   
			   return DA380_GetInt();
			   break;
		case G_IOCTRL_ACTIVECHK:
			   return DA380_MircoMotionCheck(para);
			   break;
    }

	return 0;
}



G_Sensor_OP_T da380=
{
	.name = "DA380",
	.init = DA380_Init,
	.readxyz = DA380_GetXYZ,
	.ioctrl = DA380_IOCtrl,
};

#endif

