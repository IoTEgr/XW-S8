#include "cmos_sensor.h"


#if DEV_SENSOR_IT03A1 > 0


SENSOR_INIT_SECTION const unsigned char IT03A1InitTable[]=
{    
0x00, 0x00,             // global set : page #0                                                                                          
0x08, 0x01,	//soft reset                                                                                        
0x09, 0x02,	//pad output enable, soft power down                                                                
0x10, 0x44,                                                                                                     
0x21, 0x00,	//03 //mclk divider 04 for 48Mhz, vh mirror                                                         
0x30, 0x00,	//vga mode                
//0x31, HBLANK_BASE >> 8,  //hb [11:8]
//0x32, HBLANK_BASE & 0xff,//hb [7:0] 
//0x33, VBLANK_BASE >> 8,  //vb [11:8]
//0x34, VBLANK_BASE & 0xff,//vb [7:0] 
0x60, 0x13,	//vsync active low, hsync active high, pclk rising(catch falling)                                   
0x68, 0x03,	//y-cb-y-cr order                                                                                   
0x70, 0x91,	//windowing                                                                                                                                                                                       
0x80, 0x21,	//11 //memory                                                                                       
0x81, 0x05,	//04                                                                                                                                                                                                                                                                
0x00, 0x01,             // analog timing : page #1                                                                                          
0x10, 0x11,                                                                                                     
0x11, 0x05,                                                                                                     
0x20, 0xa1,                                                                                                     
0x30, 0x94,                                                                                                     
0x31, 0x00,                                                                                                     
0x40, 0x0e,	//vddio 1.8v 0e, vddio 2.8v 0d                                                                                                                                                                                                                                                                     
0x00, 0x02,    // pixel timing : page #2                                                                                                 
0x10, 0x80,                                                                                                     
0x11, 0x0b,                                                                                                     
0x12, 0x90,                                                                                                     
0x20, 0xd2,                                                                                                                                                                                                                                                                                             
0x00, 0x08,	//ispp pre processing  // ispp : page #08                                                                             
0x10, 0x03,	//global digital gain control                                                                       
0x80, 0x01,	//blc control                                                                                       
0x81, 0x00,	//10 //fixed blc offset    
0xe8, 0x04,                                                                                                                                                                                                                                                           
0x00, 0x0a,	//ispp lsc    // ispp : page #0a                                                                                      
0x10, 0x01,	//lsc control                                                                                       
0x28, 0x00,                                                                                                     
0x29, 0x01,                                                                                                     
0x2a, 0x00,                                                                                                     
0x2b, 0x00,                                                                                                     
0x2c, 0x00,                                                                                                     
0x2d, 0x00,                                                                                                     
0x30, 0x22,                                                                                                     
0x31, 0x21,                                                                                                                                                                                                                                                                                     
0x00, 0x0c,	//ispp dpc     // ispp : page #0c                                                                                     
0x10, 0x01,	//dpc control                                                                                       
0x11, 0x11,	//dpc option                                                                                        
0x18, 0x08,	//dpc threshold                                                                                     
0x19, 0x04,
0x80, 0x01,	//nr control                                                                                        
0x81, 0x98,	//33 //nr option                                                                                    
0x88, 0x06,	//04 04 //nr threshold                                                                              
0x89, 0x00,	//08 04                                                                                             
0x8a, 0x00,	//10 10                                                                                             
0x8b, 0x05,	//05                                                                                                
0x8c, 0x05,	//08                                                                                                
0xe0, 0x10,	//outdoor to normal point                                                                           
0xe1, 0x30,	//40 //normal to dark point                                                                                                                                                                                                                                                               
0x00, 0x0e,	//ispp color  // ispp : page #0e                                                                                       
0x11, 0x68,	//intoption                                                                                         
0x90, 0x23,	//33 //37 //33 //b3 //a3 //color matrix a                                                           
0x91, 0x29,	//35 //3f //45 //46 //2b                                                                            
0x92, 0xd7,	//cf //ca //c9 //c8 //c8                                                                            
0x93, 0x00,	//fc //f7 //f2 //f2 //0d                                                                            
0x94, 0x10,	//0e //12 //0b //0a //07                                                                            
0x95, 0x26,	//32 //32 //33 //34 //33                                                                            
0x96, 0x0a,	//00 //fc //02 //02 //06                                                                            
0x97, 0xf3,	//f3 //f1 //f5 //ee //fb                                                                            
0x98, 0xd7,	//d1 //d1 //d0 //d7 //d7                                                                            
0x99, 0x36,	//3c //3e //3b //3b //2e                                                                            
0xa1, 0x03,	//color matrix b                                                                                    
0xa2, 0x00,                                                                                                     
0xa3, 0xfd,                                                                                                     
0xa4, 0xfc,                                                                                                     
0xa5, 0x00,                                                                                                     
0xa6, 0x04,                                                                                                     
0xa7, 0xf0,                                                                                                     
0xa8, 0x00,                                                                                                     
0xa9, 0x10,                                                                                                                                                                                                                                                                                    
0x00, 0x10,	//ispp gamma     // ispp : page #10                                                                                    
0x10, 0x01,	//gamma control                                                                                     
0x20, 0x00,                                                                                                     
0x21, 0x04,	//08                                                                                                
0x22, 0x0b,	//12                                                                                                
0x23, 0x24,	//2a                                                                                                
0x24, 0x49,	//4d                                                                                                
0x25, 0x66,	//68                                                                                                
0x26, 0x7c,	//7e                                                                                                
0x27, 0x8d,	//8f                                                                                                
0x28, 0x9c,	//9d                                                                                                
0x29, 0xaa,	//aa                                                                                                
0x2a, 0xb6,	//b6                                                                                                
0x2b, 0xcb,	//ca                                                                                                
0x2c, 0xdd,	//dc                                                                                                
0x2d, 0xef,	//ef                                                                                                
0x2e, 0xff,	//ff                                                                                                                                                                                                                                                                               
0x00, 0x12,	//ispp ee    // ispp : page #12                                                                                        
0x20, 0x1b,	//18 12 //p edge gain                                                                               
0x21, 0x20,	//28 1c //n edge gain                                                                               
0x22, 0x11,	//pn edge coring                                                                                    
0x23, 0x7f,	//p edge clip                                                                                       
0x24, 0x7f,	//n edge clip                                                                                       
0x29, 0x00,	//10 //edge value start                                                                             
0x2a, 0x8f,	//88                                                                                                
0x2e, 0x34,	//new edge suppression                                                                              
0x2f, 0x10,                                                                                                     
0x50, 0x82,	//82 //h edge control                                                                               
0x51, 0x80,                                                                                                     
0x52, 0xc9,                                                                                                     
0x53, 0x80,                                                                                                     
0x54, 0x33,                                                                                                     
0x60, 0x11,	//1b //09 //color cbcr coring                                                                       
0x61, 0x00,                                                                                                     
0x80, 0x01,                                                                                                     
0x8e, 0x32,//new	//30 //color suppression                                                                      
0x8f, 0x80,                                                                                                     
0x90, 0x11,	//10 //contrast control                                                                             
0x91, 0x00,	//08 //brightness control                                                                           
0x92, 0xf0,	//y clip                                                                                            
0x93, 0x00,                                                                                                     
0x98, 0x1a,	//1f //1f //upper b color saturation                                                                
0x99, 0x1c,	//1f //1f //upper r color saturation                                                                
0x9a, 0x19,	//10 //1f //lower b color saturation                                                                
0x9b, 0x1c,	//1f //1f //lower r color saturation                                                                                                                                                                                                                                                          
0x00, 0x18,	//ispp aeg control   // ispp : page #18                                                                                
0x10, 0x80,	//aegc control                                                                                      
0x18, 0x00,	//ae lock range                                                                                     
0x20, 0x7d,	//step                                                                                              
0x21, 0x04,	//max shutter                                                                                                                                                                                                      
0x40, 0x00,	//pixel shutter                                                                                     
0x41, 0x01,                                                                                                     
0x50, 0x02,	//analog max gain                                                                                   
0x51, 0x38,	//max gain                                                                                          
0x52, 0x08,	//min gain                                                                                          
0x61, 0x00,	//dark offset                                                                                       
0x80, 0x80,	//60 //70 //80 //50 //ae target                                                                     
0x82, 0x80,	//60 //70 //80 //50                                                                                 
0xa0, 0x00,	//00 //ae weight                                                                                                                                                                                                                                                          
0x00, 0x1a,	//ispp awb control // ispp : page #1a                                                                                   
0x10, 0x00,                                                                                                     
0x20, 0x80,	//80 //7e //cb level                                                                                
0x21, 0x80,	//81 //7e //80 //cr level                                                                           
0x30, 0xf0,	//a8 //a8 //ff //ff //cr top                                                                        
0x31, 0x88,	//98 //00 //00 //80 //cr bottom                                                                     
0x32, 0xc8,	//c0 //ff //ff //e0 //cb top                                                                        
0x33, 0x70,	//b0 //b0 //00 //00 //cb bottom                                                                     
0x40, 0x04,	//47 //8e //23 //outdoor start                                                                      
0x41, 0x08,	//47 //8e //23 //outdoor end                                                                        
0x42, 0xa8,	//b2 //outdoor fixed cr gain                                                                        
0x43, 0xb0,	//aa //outdoor fixed cb gain                                                                        
0x50, 0x89,	//to high boundary cr                                                                               
0x51, 0xde,	//to high bounadry cb                                                                               
0x52, 0x89,	//to low boundary cr                                                                                
0x53, 0xde,	//to low boundary cb                                                                                
0x60, 0xea,                                                                                                     
0x61, 0xb0,                                                                                                     
0x80, 0xa0,	//white pixel domain                                                                                
0x81, 0x40,                                                                                                     
0x82, 0xa0,                                                                                                     
0x83, 0x40,                                                                                                     
0x84, 0xa0,                                                                                                     
0x85, 0x40,                                                                                                     
0x86, 0xf0,	//c0                                                                                                
0x87, 0x20,                                                                                                     
0x88, 0x05,	//                                                                                                  
0x89, 0x30,                                                                                                     
0x8d, 0xac,	//                                                                                                  
0x8e, 0x28,                                                                                                     
0x8f, 0xff,                                                                                                                                                                                                                                                                                                
0x00, 0x00,      // operation enable                                                                                                
0x20, 0x01,                                                                                                                                                                                                                                                                                                                                    
0x00, 0x1a,	//ispp awb control	                                                                                
0x60, 0xea,                                                                                                     
0x61, 0xb0,                                                                                                     
0x10, 0xb3,
0x00, 0x00,
-1,-1};

static void IT03A1_rotate(u32 r)
{
	unsigned char buf[2];
	buf[0] = 0x21;
	buf[1] = 0x00|(r<<0);
	
//	sensor_iic_enable();
//	sensor_iic_info();		
	sensor_iic_write(buf);

//  sensor_iic_disable();		
}

static void IT03A1_hvblank(u32 h,u32 v)
{
	u32 i;
	const u8 t[][2] = {
		{0x31,h >> 8},//HB[11:8]
		{0x32,h & 0xff},//HB[7:0]
		{0x33,v >> 8},//VB[11:8]
		{0x34,v & 0xff},//VB[7:0]
	};
//	sensor_iic_enable();
//	sensor_iic_info();		
	for(i=0;i<4;i++)
		sensor_iic_write((u8 *)&t[i][0]);
//  sensor_iic_disable();			
//	return (h<<16)|v;
}

SENSOR_OP_SECTION const Sensor_Op_t it03a1_cmd= 
{
	.rotate_adapt = {0},
	.hvb_adapt = {0xa7,0x07,0xa7,0x07},
	. mclk = 24000000,
	.p_fun_adapt = {IT03A1_rotate,IT03A1_hvblank},
};

SENSOR_HEADER_ITEM_SECTION const Sensor_Init_t it03a1_init=
{
	.sensor_struct_addr =	(u32 *)&it03a1_cmd,   
	.sensor_struct_size = sizeof(Sensor_Op_t),
	.sensor_init_tab_adr =	(u32 *)IT03A1InitTable,     
	.sensor_init_tab_size = sizeof(IT03A1InitTable),
	.id               =	0x63,                 
	.w_cmd            =	0x86,                 
	.r_cmd            =	0x87,                 
	.addr_num         =	0x01,                 
	.data_num         =	0x01,                 
	.id_reg           =	0x01,                 

};



#endif


