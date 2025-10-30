#include "cmos_sensor.h"

#if DEV_SENSOR_SIV100B >0

SENSOR_INIT_SECTION const unsigned char SIV100BInitTable[]=
{
	0X3, 0X04, 
	0X4, 0x0, //rotate
	0X5, 0X7,	
	0X6, 0X0,		
	0X7, 0X0,		
	0X8, 0X0,		
	0X9, 0X0,		
	0Xa, 0X0,		
	0Xb, 0X0,		
	0Xc, 0X0,		
	0Xd, 0X0,		
	0Xe, 0X0,		
	0Xf, 0X0,
	0X10, 0X13,		
	0X11, 0X4, 	
	0X12, 0Xa, 	
	0X13, 0X1f,		
	0X14, 0X17,		
	0X15, 0X0, 	
	0X16, 0X89,		
	0X17, 0X60,		
	0X18, 0X16,		
	0X19, 0X0, 	
	0X1a, 0X0, 	
	0X1b, 0X90,		
	0X1c, 0X0, 	
	0X1d, 0X0, 	
	0X1e, 0X0, 	
	0X1f, 0X52,	
	
	//0X20, ((HBLANK_BASE>>8)<<4)|(VBLANK_BASE>>8), //[9:8] 60hz
	//0X21, HBLANK_BASE & 0xff, //hb[7:0] 60zh
	//0X22, VBLANK_BASE & 0xff, //vb[7:0] 60hz
	//0X23, ((HBLANK_BASE>>8)<<4)|(VBLANK_BASE>>8),	//50hz
	//0X24, HBLANK_BASE & 0xff,	//50hz
	//0X25, VBLANK_BASE & 0xff,	//50hz
	0X33, 0X5,		
	0X34, 0X7D,		
	0X35, 0X7D,		
	0X36, 0X7d,		
	0X37, 0X7d,	
	0X38, 0X0, 	
	0X39, 0X0, 	
	0X3a, 0X0, 	
	0X3b, 0X0, 	
	0X3c, 0X0, 	
	0X3d, 0X0, 	
	0X3e, 0X0, 	
	0X3f, 0X0, 	
	0X40, 0X80,		
	0x41, 0x78,
	0X42, 0X7f,		
	0X43, 0Xc0,		
	0X44, 0X48,		
	0X45, 0X28,		
	0X46, 0X8, 	
	0X47, 0X15,		
	0X48, 0X14,		
	0X49, 0Xc, 	
	0X4a, 0X42,		
	0X4b, 0Xc4,		
	0X4c, 0X3c,		
	0X4d, 0X50,		
	0X4e, 0X17,		
	0X4f, 0X8a,		
	0X50, 0X94,
	0X51, 0X60,		
	0X52, 0X90,		
	0X53, 0X70,
	0X5a, 0X0, 	
	0X5b, 0X2, 	
	0X5c, 0X33,		
	0X5d, 0Xf8,		
	0X5e, 0X0, 	
	0X5f, 0X0, 	
	0X60, 0Xc3,		
	0X61, 0X88,		
	0X62, 0X1, 	
	0X63, 0X80,		
	0X64, 0X80,		
	0X65, 0Xd2,		
	0X66, 0X83,		
	0X67, 0Xd6,		
	0X68, 0X80,		
	0X69, 0X8a,		
	0X6a, 0X70,		
	0X6b, 0X90,		
	0X6c, 0X70,		
	0X6d, 0X88,		
	0X6e, 0X77,		
	0X6f, 0X46,		
	0X70, 0Xd8,		
	0X71, 0X40,	
	0X72, 0X5, 	
	0X73, 0X2, 	
	0X74, 0Xc, 
	0X75, 0X10, 		
	0X76, 0X60, 		
	0X77, 0Xaf,		
	0X78, 0X8d,		
	0X79, 0X0, 	
	0X7a, 0Xb5,	
	0X7b, 0Xa3,		
	0X7c, 0X80,		
	0X7d, 0X0, 	
	0X7e, 0X0, 	
	0X7f, 0X41,		
	0X80, 0Xaf,		
	0X81, 0X19,		
	0X82, 0X3d,		
	0X83, 0X0, 	
	0X84, 0Xff,		
	0X85, 0X0, 	
	0X86, 0Xa1,		
	0X87, 0X00, 
	0X88, 0X24,		
	0X89, 0Xf, 	
	0X8a, 0X80,		
	0X8b, 0X80,		
	0X8c, 0X80,		
	0X8d, 0X80,		
	0X8e, 0X0, 	
	0X8f, 0X0, 	
	0X90, 0X0, 	
	0X91, 0X0, 	
	0X92, 0X44,		
	0X93, 0X10,		
	0X94, 0X20,		
	0X95, 0X18,		
	0X96, 0X10,		
	0X97, 0X20,		
	0X98, 0X20,		
	0X99, 0X18,		
	0X9a, 0X50,		
	0X9b, 0X0, 	
	0X9c, 0Xd, 	
	0X9d, 0Xd, 	
	0X9e, 0Xd, 	
	0X9f, 0Xd, 	
	0Xa0, 0X10,		
	0Xa1, 0X10,		
	0Xa2, 0X10,		
	0Xa3, 0X10,		
	0Xa4, 0Xcc,		
	0Xa5, 0Xcc,		
	0Xa6, 0Xcc,		
	0Xa7, 0Xcb,		
	0Xa8, 0Xa9,		
	0Xa9, 0X43,
	0Xaa, 0X45,	
	0Xab, 0X11,		
	0Xac, 0X13,		
	0Xad, 0X0, 	
	0Xae, 0X0, 	
	0Xaf, 0X98,		
	0Xb0, 0X90,
	0Xb1, 0X0, 	
	0Xb2, 0Xb, 	
	0Xb3, 0X16,		
	0Xb4, 0X2a,		
	0Xb5, 0X4a,		
	0Xb6, 0X64,		
	0Xb7, 0X7b,		
	0Xb8, 0X8f,		
	0Xb9, 0Xa0,		
	0Xba, 0Xae,		
	0Xbb, 0Xbb,		
	0Xbc, 0Xd0,		
	0Xbd, 0Xe2,		
	0Xbe, 0Xf2,		
	0Xbf, 0Xf9,		
	0Xc0, 0Xff,
	0Xc1, 0X3a,		
	0Xc2, 0Xcb,		
	0Xc3, 0Xfb,		
	0Xc4, 0X10,		
	0Xc5, 0X21,		
	0Xc6, 0X10,		
	0Xc7, 0Xf3,		
	0Xc8, 0Xbd,		
	0Xc9, 0X50,		
	0Xca, 0X90,		
	0Xcf, 0X20,		
	0Xd0, 0X20,		
	0Xd1, 0X28,		
	0Xd2, 0X86,		
	0Xd3, 0X0, 
	0Xcb, 0X40,		
	0Xcc, 0X28,		
	0Xcd, 0X6,	
	0Xce, 0X6,
	0Xd4, 0X12,		
	0Xd5, 0X16,	
	0Xd6, 0X18,	
	0Xd7, 0X04,
	0Xd8, 0X0, 	
	0Xd9, 0X0, 	
	0Xda, 0X0, 	
	0Xdb, 0Xff,		
	0Xdc, 0X0, 	
	0Xdd, 0Xff,		
	0Xde, 0X0, 	
	0Xdf, 0Xff,		
	0Xe0, 0X0, 	
	0Xe1, 0X2a,		
	0Xe2, 0X8b,
	0Xe3, 0X0, 	
	0Xe4, 0X0, 	
	0Xe5, 0X0, 	
	0Xe6, 0X44,		
	0Xf0, 0X24,		
	0Xf1, 0X0, 	
	0Xf2, 0X80,		
	0Xf3, 0X0, 	
	0Xf4, 0Xe0,		
	0x03,0xc5,
	0x7a,0xa8,
	0x7b,0xb0,
	0x7c,0x80,
	0x40,0x80,
	-1,-1
};
static void SIV100B_rotate(u32 r)
{
	unsigned char buf[2];
	buf[0] = 0x04;

	buf[1] = (r<<0);
//	sensor_iic_enable();
//	sensor_iic_info();		
	sensor_iic_write(buf);	

//	 sensor_iic_diable();	

}

static void SIV100B_hvblank(u32 h,u32 v)
{

	u32 i;
	const u8 t[][2] = {
		{0X20, ((h>>8)<<4)|(v>>8)},
		{0X21, h & 0xff},
		{0X22, v & 0xff},
	};

//	sensor_iic_enable();
//	sensor_iic_info();	

	for(i=0;i<6;i++)
		sensor_iic_write((u8 *)&t[i][0]);
//	 sensor_iic_diable();			
//	return (h<<16)|v;
}
SENSOR_OP_SECTION const Sensor_Op_t siv100b_cmd= 
{
	.typ = CSI_TYPE_YUV422,
	.pixelw = 640,
	.pixelh= 480,
	.hsyn = 1,
	.vsyn = 1,
	.rduline = 0,//
	.colrarray = 2, //YCBYCR
	.rotate_adapt = {0},
	.hvb_adapt = {0x6a,0x20,0x6a,0x20},
	. mclk = 24000000,
	.p_fun_adapt = {SIV100B_rotate,SIV100B_hvblank,NULL},
};


SENSOR_HEADER_ITEM_SECTION const Sensor_Init_t siv100b_init=
{
	.sensor_struct_addr =	(u32 *)&siv100b_cmd,
	.sensor_struct_size = sizeof(Sensor_Op_t),
	.sensor_init_tab_adr =	(u32 *)SIV100BInitTable,     
	.sensor_init_tab_size = sizeof(SIV100BInitTable),
	.id               =	0x0c,               
	.w_cmd            =	0x66,               
	.r_cmd            =	0x67,               
	.addr_num         =	0x01,               
	.data_num         =	0x01,               
	.id_reg           =	0x01,               
};


#endif

