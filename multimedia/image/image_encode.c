/****************************************************************************
**
 **                              MULTIMEDIA
  ** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
   **** **                  MULTIMEDIA IMAGE ENCODE
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **   
* File Name   : image_encode.c
* Author      : Mark.Douglas 
* Version     : V100
* Date        : 09/22/2016
* Description : This file is image encode file
*               
* History     : 
* 2016-09-22  : 
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "../media.h"
#include "../multimedia.h"
#include "../../ax32_platform_demo/inc/display.h"
#include "../../ax32_platform_demo/application.h" 
#include "../watermark/image_watermark.h"
#include "../imageL/imageL_encode.h"


#if MEDIA_CFG_IMAGE_ENCODE_EN >0

//===EXIF format ===
#if (1 == JPEG_EXIF)

#define  IFD0_DE_CNT  0xa
#define  CHILD_IFD_DE_CNT  0x22

typedef struct 
{
	u16  tag;
	u16  type;	// 1:u8bit ,2:8bit,3:u16bit,4:u32bit,5:u64bits,6:s8bit,7:8bit,8:s16bit,9:s32bit,10:s64bit,11:float,12:double
	u16  count[2]; 
	u16  value_offset[2];
}EXIF_IFD0_DE;

typedef struct 
{
	u16  soi;
	u16  app1;
	u16  app1_size;
	u16  flag[2];
	u16  flag_v;
	u16  byte_order;
	u16  bo_flag;
	u16  ifd0_offset[2];
	u16  ifd0_cnt;
	EXIF_IFD0_DE ifd0_de[IFD0_DE_CNT];
	u16  de_end[2];
	u8 de0_buf[0xC];
	u8 de1_buf[0xA];
	u16 de3_buf[4];
	u16 de4_buf[4];
	u8 de6_buf[0xc];
	u8 de7_buf[0x14];
	u16 child_ifd_cnt;
	EXIF_IFD0_DE child_de[CHILD_IFD_DE_CNT];
	u16 child_de_end[2];
	u16 child_de0_buf[4];
	u16 child_de1_buf[4];
	u8 child_de5_buf[0x14];
	u8 child_de6_buf[0x14];
	u16 child_de8_buf[4];
	u16 child_de9_buf[4];
	u16 child_de10_buf[4];
	u16 child_de11_buf[4];
	u16 child_de15_buf[4];
	u16 child_de16_buf[4];
	u16 child_de27_buf[4];
}EXIF_JPEG_HEAD;


extern INT32U configValue2Int(INT8U config_id);
int jpeg_exif_head_init(u8 *head_buf,u16 width,u16 height,u8* datatime)
{
	u32 offset;
	u32* temp;
	int res = 0;
	EXIF_JPEG_HEAD *jpeg_exif = (EXIF_JPEG_HEAD *)head_buf;
	//==init head==
	jpeg_exif->soi = 0xD8FF;
	jpeg_exif->app1 = 0xE1FF;
	jpeg_exif->app1_size = 0;
	jpeg_exif->flag[0] = 0x7845;	 // "Exif"
	jpeg_exif->flag[1] = 0x6669;
	jpeg_exif->flag_v = 0;
	jpeg_exif->byte_order = 0x4949;  //"II"
	jpeg_exif->bo_flag = 0x2A;	
	jpeg_exif->ifd0_offset[0] = 0x08;
	jpeg_exif->ifd0_cnt = IFD0_DE_CNT;
	jpeg_exif->de_end[0]=0x0304;
	jpeg_exif->child_ifd_cnt = CHILD_IFD_DE_CNT;
	jpeg_exif->child_de_end[0]=0x0;

	//==init de0==
	jpeg_exif->ifd0_de[0].tag = 0x010F;	//make
	jpeg_exif->ifd0_de[0].type = 0x02;
	jpeg_exif->ifd0_de[0].count[0] = 0x0c;
	offset = sizeof(EXIF_IFD0_DE)*IFD0_DE_CNT+jpeg_exif->ifd0_offset[0]+0x6;
	jpeg_exif->ifd0_de[0].value_offset[0] = offset&0xffff;
	strcpy(jpeg_exif->de0_buf,"JXL");
	//==init de1==
	jpeg_exif->ifd0_de[1].tag = 0x0110;	//model
	jpeg_exif->ifd0_de[1].type = 0x02;
	jpeg_exif->ifd0_de[1].count[0]=0x0a;
	offset = jpeg_exif->ifd0_de[0].value_offset[0]+jpeg_exif->ifd0_de[0].count[0];
	jpeg_exif->ifd0_de[1].value_offset[0] = offset&0xffff;
	strcpy(jpeg_exif->de1_buf,"JXL");
	//==init de2==
	jpeg_exif->ifd0_de[2].tag = 0x0112;	//orientation
	jpeg_exif->ifd0_de[2].type = 0x03;
	jpeg_exif->ifd0_de[2].count[0]= 0x01;
	jpeg_exif->ifd0_de[2].value_offset[0] = 0x1;
	//==init de3==
	jpeg_exif->ifd0_de[3].tag = 0x011a;	//x resolution
	jpeg_exif->ifd0_de[3].type = 0x05;
	jpeg_exif->ifd0_de[3].count[0]=0x01;
	offset = jpeg_exif->ifd0_de[1].value_offset[0]+jpeg_exif->ifd0_de[1].count[0];
	jpeg_exif->ifd0_de[3].value_offset[0] = offset&0xffff;
	jpeg_exif->de3_buf[0]=0x48;			// 72 dpi
	jpeg_exif->de3_buf[2]=0x01;
	//==init de4==
	jpeg_exif->ifd0_de[4].tag = 0x011b;	//y resolution
	jpeg_exif->ifd0_de[4].type = 0x05;
	jpeg_exif->ifd0_de[4].count[0] = 0x01;
	offset = jpeg_exif->ifd0_de[3].value_offset[0]+0x8;
	jpeg_exif->ifd0_de[4].value_offset[0] = offset&0xffff;
	jpeg_exif->de4_buf[0]=0x48;			// 72 dpi
	jpeg_exif->de4_buf[2]=0x01;
	//==init de5==
	jpeg_exif->ifd0_de[5].tag = 0x0128;	//resolution unit
	jpeg_exif->ifd0_de[5].type = 0x03;
	jpeg_exif->ifd0_de[5].count[0]=0x01;
	jpeg_exif->ifd0_de[5].value_offset[0]=0x02;
	//==init de6==
	jpeg_exif->ifd0_de[6].tag = 0x0131;	//software
	jpeg_exif->ifd0_de[6].type = 0x02;
	jpeg_exif->ifd0_de[6].count[0] = 0x0c;
	offset = jpeg_exif->ifd0_de[4].value_offset[0]+0x8;
	jpeg_exif->ifd0_de[6].value_offset[0] = offset&0xffff;
	strcpy(jpeg_exif->de6_buf,"V233-00-01");
	//==init de7==
	jpeg_exif->ifd0_de[7].tag = 0x0132;	//date time
	jpeg_exif->ifd0_de[7].type = 0x02;
	jpeg_exif->ifd0_de[7].count[0]=0x14;
	offset = jpeg_exif->ifd0_de[6].value_offset[0]+jpeg_exif->ifd0_de[6].count[0];
	jpeg_exif->ifd0_de[7].value_offset[0] = offset&0xffff;
	strcpy(jpeg_exif->de7_buf,datatime);
	//==init de8==
	jpeg_exif->ifd0_de[8].tag = 0x0213;	//ycbcr positioning
	jpeg_exif->ifd0_de[8].type = 0x03;
	jpeg_exif->ifd0_de[8].count[0]=0x01;
 	jpeg_exif->ifd0_de[8].value_offset[0] = 0x02;
	//==init de9==
	jpeg_exif->ifd0_de[9].tag = 0x8769;	//Exif child ifd offset
	jpeg_exif->ifd0_de[9].type = 0x04;
	jpeg_exif->ifd0_de[9].count[0]=0x01;
	offset = jpeg_exif->ifd0_de[7].value_offset[0]+jpeg_exif->ifd0_de[7].count[0];
	jpeg_exif->ifd0_de[9].value_offset[0] = offset&0xffff;
	//==init child de0==
	jpeg_exif->child_de[0].tag = 0x829a;//exposure time
	jpeg_exif->child_de[0].type = 0x05;
	jpeg_exif->child_de[0].count[0]=0x01;
	offset = jpeg_exif->ifd0_de[9].value_offset[0]+sizeof(EXIF_IFD0_DE)*CHILD_IFD_DE_CNT+0x6;
	jpeg_exif->child_de[0].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de0_buf[0]=0x0a;	// 1/99 s		(child_de0_buf[0]/child_de0_buf[2])		
	jpeg_exif->child_de0_buf[2]=0x3e1;
	//==init child de1==
	jpeg_exif->child_de[1].tag = 0x829D;	//Fnumber
	jpeg_exif->child_de[1].type = 0x05;
	jpeg_exif->child_de[1].count[0]=0x01;
	offset = jpeg_exif->child_de[0].value_offset[0]+0x8;
	jpeg_exif->child_de[1].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de1_buf[0]=0x20;	// f/3.2		(child_de0_buf[0]/child_de0_buf[2])	
	jpeg_exif->child_de1_buf[2]=0x0a;
	//==init child de2==
	jpeg_exif->child_de[2].tag = 0x8822;	// exposure program
	jpeg_exif->child_de[2].type = 0x03;
	jpeg_exif->child_de[2].count[0]=0x01;
	jpeg_exif->child_de[2].value_offset[0]=0x2;
	//==init child de3==
	jpeg_exif->child_de[3].tag = 0x8827;	// ISO speedRatings
	jpeg_exif->child_de[3].type = 0x03;
	jpeg_exif->child_de[3].count[0]=0x01;
	jpeg_exif->child_de[3].value_offset[0]=0x64;//ISO-100						
	//==init child de4==
	jpeg_exif->child_de[4].tag = 0x9000;	// ExifVersion
	jpeg_exif->child_de[4].type = 0x07;
	jpeg_exif->child_de[4].count[0]=0x04;						
	strcpy(&jpeg_exif->child_de[4].value_offset[0],"02");
	strcpy(&jpeg_exif->child_de[4].value_offset[1],"20");
	//==init child de5==
	jpeg_exif->child_de[5].tag = 0x9003;	// DateTimeOriginal
	jpeg_exif->child_de[5].type = 0x02;
	jpeg_exif->child_de[5].count[0]=0x14;
	offset = jpeg_exif->child_de[1].value_offset[0]+0x8;
	jpeg_exif->child_de[5].value_offset[0] = offset&0xffff;
	strcpy(jpeg_exif->child_de5_buf,datatime);
	//==init child de6==
	jpeg_exif->child_de[6].tag = 0x9004;	// DateTimeDigitized
	jpeg_exif->child_de[6].type = 0x02;
	jpeg_exif->child_de[6].count[0]=0x14;
	offset = jpeg_exif->child_de[5].value_offset[0]+jpeg_exif->child_de[5].count[0];
	jpeg_exif->child_de[6].value_offset[0] = offset&0xffff;
	strcpy(jpeg_exif->child_de6_buf,datatime);
	//==init child de7==
	jpeg_exif->child_de[7].tag = 0x9101;	// Compornents Configuration
	jpeg_exif->child_de[7].type = 0x07;
	jpeg_exif->child_de[7].count[0]=0x4;
	offset = 0x00030201;
	jpeg_exif->child_de[7].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de[7].value_offset[1] = (offset>>16)&0xffff;
	//==init child de8==
	jpeg_exif->child_de[8].tag = 0x9102;	// Compressed bitsperpixel
	jpeg_exif->child_de[8].type = 0x05;
	jpeg_exif->child_de[8].count[0]=0x01;
	offset = jpeg_exif->child_de[6].value_offset[0]+jpeg_exif->child_de[6].count[0];
	jpeg_exif->child_de[8].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de8_buf[0]=0x02;
	jpeg_exif->child_de8_buf[2]=0x01;
	//==init child de9==
	jpeg_exif->child_de[9].tag = 0x9203;	// brightness value
	jpeg_exif->child_de[9].type = 0x0a;
	jpeg_exif->child_de[9].count[0]=0x01;
	offset = jpeg_exif->child_de[8].value_offset[0]+0x8;
	jpeg_exif->child_de[9].value_offset[0] = offset&0xffff;
	offset = 0xFFFFEC78;					// -5      ((int)0xFFFFEC78/(int)0x3E8)
	jpeg_exif->child_de9_buf[0]=offset&0xffff;	
	jpeg_exif->child_de9_buf[1]=(offset>>16)&0xffff;
	offset = 0x3E8;						
	jpeg_exif->child_de9_buf[2]=offset&0xffff;
	jpeg_exif->child_de9_buf[3]=(offset>>16)&0xffff;
	//==init child de10==
	jpeg_exif->child_de[10].tag = 0x9204;// exposure bias value
	jpeg_exif->child_de[10].type = 0x0a;
	jpeg_exif->child_de[10].count[0]=0x01;
	offset = jpeg_exif->child_de[9].value_offset[0]+0x8;
	jpeg_exif->child_de[10].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de10_buf[0]=0x0a;	// +1 bias      (*(int*)child_de10_buf[0]/*(int*)child_de10_buf[2])
	jpeg_exif->child_de10_buf[2]=0x0a;
	//==init child de11==
	jpeg_exif->child_de[11].tag = 0x9205;// max aperture value
	jpeg_exif->child_de[11].type = 0x05;
	jpeg_exif->child_de[11].count[0]=0x01;
	offset = jpeg_exif->child_de[10].value_offset[0]+0x8;
	jpeg_exif->child_de[11].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de11_buf[0]=0x3;	// 1.5      (*(int*)child_de11_buf[0]/*(int*)child_de11_buf[2])
	jpeg_exif->child_de11_buf[2]=0x2;
	//==init child de12==
	jpeg_exif->child_de[12].tag = 0x9207;// Metering mode
	jpeg_exif->child_de[12].type = 0x03;
	jpeg_exif->child_de[12].count[0]=0x01;
	jpeg_exif->child_de[12].value_offset[0]=0x04;// 0: unknow ,1: avd ,2: cent ,3:one point,4: more point ,5:more place , 6: apart, 255: other
	//==init child de13==
	jpeg_exif->child_de[13].tag = 0x9208;// light source
	jpeg_exif->child_de[13].type = 0x03;
	jpeg_exif->child_de[13].count[0]=0x01;
	jpeg_exif->child_de[13].value_offset[0]=0x0;// 0: unknow ,1:sun ,2:led ,3:tungsten,10:flash,17:stand A, 18: stand B, 18: stand C,255: other
	//==init child de14==
	jpeg_exif->child_de[14].tag = 0x9209;// flash
	jpeg_exif->child_de[14].type = 0x03;
	jpeg_exif->child_de[14].count[0]=0x01;
	jpeg_exif->child_de[14].value_offset[0]=0x0;// 0: unflash ,1:flash ,5:flash & unreflector 7:flash &reflector
	//==init child de15==
	jpeg_exif->child_de[15].tag = 0x920a;// focal length
	jpeg_exif->child_de[15].type = 0x05;
	jpeg_exif->child_de[15].count[0]=0x01;
	offset = jpeg_exif->child_de[11].value_offset[0]+0x8;
	jpeg_exif->child_de[15].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de15_buf[0]=0x52;	// 7mm      (*(int*)child_de15_buf[0]/*(int*)child_de15_buf[2])
	jpeg_exif->child_de15_buf[2]=0x0B;
	//==init child de16==
	jpeg_exif->child_de[16].tag = 0x9214;
	jpeg_exif->child_de[16].type = 0x03;
	jpeg_exif->child_de[16].count[0]=0x04;
	offset = jpeg_exif->child_de[15].value_offset[0]+0x8;
	jpeg_exif->child_de[16].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de16_buf[0] = 0x510;
	jpeg_exif->child_de16_buf[1] = 0x3cc;
	jpeg_exif->child_de16_buf[2] = 0xa20;
	jpeg_exif->child_de16_buf[3] = 0x798;
	//==init child de17==
	jpeg_exif->child_de[17].tag = 0xa000;//flash pix version
	jpeg_exif->child_de[17].type = 0x07;
	jpeg_exif->child_de[17].count[0]=0x04;
	strcpy(&jpeg_exif->child_de[17].value_offset[0],"01");
	strcpy(&jpeg_exif->child_de[17].value_offset[1],"00");
	//==init child de18==
	jpeg_exif->child_de[18].tag = 0xa001;//color space
	jpeg_exif->child_de[18].type = 0x03;
	jpeg_exif->child_de[18].count[0]=0x01;
	jpeg_exif->child_de[18].value_offset[0]=0x01;// 1:sRGB				
	//==init child de19==
	jpeg_exif->child_de[19].tag = 0xa002;//exif image width
	jpeg_exif->child_de[19].type = 0x04;
	jpeg_exif->child_de[19].count[0]=0x01;
	jpeg_exif->child_de[19].value_offset[0]=width;
	//==init child de20==
	jpeg_exif->child_de[20].tag = 0xa003;//exif image height
	jpeg_exif->child_de[20].type = 0x04;
	jpeg_exif->child_de[20].count[0]=0x01;
	jpeg_exif->child_de[20].value_offset[0]=height;
	//==init child de21==
	jpeg_exif->child_de[21].tag = 0xa005;//exif interoperability offset
	jpeg_exif->child_de[21].type = 0x04;
	jpeg_exif->child_de[21].count[0]=0x01;
	jpeg_exif->child_de[21].value_offset[0]=0x2e6;
	//==init child de22==
	jpeg_exif->child_de[22].tag = 0xa300;//file source
	jpeg_exif->child_de[22].type = 0x07;
	jpeg_exif->child_de[22].count[0]=0x01;
	jpeg_exif->child_de[22].value_offset[0]=0x03;
	//==init child de23==
	jpeg_exif->child_de[23].tag = 0xa301;//scene type
	jpeg_exif->child_de[23].type = 0x07;
	jpeg_exif->child_de[23].count[0]=0x01;
	jpeg_exif->child_de[23].value_offset[0]=0x01;
	//==init child de24==
	jpeg_exif->child_de[24].tag = 0xa401;//custom rendered
	jpeg_exif->child_de[24].type = 0x03;
	jpeg_exif->child_de[24].count[0]=0x01;
	jpeg_exif->child_de[24].value_offset[0]=0x0;	// 0:normal process ,1:custom process
	//==init child de25==
	jpeg_exif->child_de[25].tag = 0xa402;//exposure mode
	jpeg_exif->child_de[25].type = 0x03;
	jpeg_exif->child_de[25].count[0]=0x01;
	jpeg_exif->child_de[25].value_offset[0]=0x0;	// 0:auto exposure ,1:manual exposure ,2:auto bracket 
	//==init child de26==
	jpeg_exif->child_de[26].tag = 0xa403;//while balance mode
	jpeg_exif->child_de[26].type = 0x03;
	jpeg_exif->child_de[26].count[0]=0x01;
	jpeg_exif->child_de[26].value_offset[0]=0x0;	// 0:auto white balance ,1:manual white balance
	//==init child de27==
	jpeg_exif->child_de[27].tag = 0xa404;//digital zoom ratio
	jpeg_exif->child_de[27].type = 0x05;
	jpeg_exif->child_de[27].count[0]=0x01;
	offset = jpeg_exif->child_de[16].value_offset[0]+0x8;
	jpeg_exif->child_de[27].value_offset[0] = offset&0xffff;
	jpeg_exif->child_de27_buf[0] = 0x0;	// 0 	(*(int*)child_de27_buf[0]/*(int*)child_de27_buf[2])			
	jpeg_exif->child_de27_buf[2] = 0x1;
	//==init child de28==
	jpeg_exif->child_de[28].tag = 0xa405;//35mm film equiv focal length
	jpeg_exif->child_de[28].type = 0x03;
	jpeg_exif->child_de[28].count[0]=0x01;
	jpeg_exif->child_de[28].value_offset[0]=0x0;
	//==init child de29==
	jpeg_exif->child_de[29].tag = 0xa406;//scene capture type
	jpeg_exif->child_de[29].type = 0x03;
	jpeg_exif->child_de[29].count[0]=0x01;
	jpeg_exif->child_de[29].value_offset[0]=0x0;// 0:standard 1: landscape 2: portrait 3:night scene
	//==init child de30==
	jpeg_exif->child_de[30].tag = 0xa407;//gain control
	jpeg_exif->child_de[30].type = 0x03;
	jpeg_exif->child_de[30].count[0]=0x01;
	jpeg_exif->child_de[30].value_offset[0]=0x0;// 0:none ,1:low gain up 2:high gain up 3:low gain down ,4: low gain down
	//==init child de31==
	jpeg_exif->child_de[31].tag = 0xa408;//contrast
	jpeg_exif->child_de[31].type = 0x03;
	jpeg_exif->child_de[31].count[0]=0x01;
	jpeg_exif->child_de[31].value_offset[0]=0x0;// 0:normal ,1:soft ,2:hard
	//==init child de32==
	jpeg_exif->child_de[32].tag = 0xa409;//saturation
	jpeg_exif->child_de[32].type = 0x03;
	jpeg_exif->child_de[32].count[0]=0x01;
	jpeg_exif->child_de[32].value_offset[0]=0x0;// 0:normal ,1:low saturation ,2:high saturation
	//==init child de33==
	jpeg_exif->child_de[33].tag = 0xa40a;//sharpness
	jpeg_exif->child_de[33].type = 0x03;
	jpeg_exif->child_de[33].count[0]=0x01;
	jpeg_exif->child_de[33].value_offset[0]=0x0;// 0:normal ,1:soft ,2:hard


	u16 app_size = (jpeg_exif->child_de[27].value_offset[0]+0x8+0xA);
	jpeg_exif->app1_size = (app_size>>8|(app_size&0xff)<<8);
//	deg_Printf("jpeg_exif.app1_size=0x%x\n",jpeg_exif->app1_size);
//	deg_Printf("sizeof(jpeg_exif)=0x%x,sizeof(ifd0_de)=0x%x,sizeof(jpeg_exif.child_de)=0x%x\n",sizeof(jpeg_exif),sizeof(jpeg_exif->ifd0_de),sizeof(jpeg_exif->child_de));

	return res;
}
#endif
//===end EXIF format===

//------for watermark
#if 0
#define SM_PIC_ST_NUM 10  //small pic timestemp num
#define ST_PIXEL_W    16
#define ST_PIXEL_H	  32

#define WATERMAKE_SET_X_POS 10
#define WATERMAKE_SET_Y_POS	448

static u16 id_w_st,id_h_st;
static u8* small_pic_id_buf[SM_PIC_ST_NUM];
static u16 bk_w_sm,bk_h_sm;

static u32 small_pic_id[SM_PIC_ST_NUM]={RES_NUM_0,RES_NUM_1,RES_NUM_2,RES_NUM_3,RES_NUM_4,
								 RES_NUM_5,RES_NUM_6,RES_NUM_7,RES_NUM_8,RES_NUM_9,
								 /*RES_NUM_SLASH,RES_NUM_COLON,*/
								};
#endif

//-----

extern void videoRecordImageWatermark(INT16U width,INT16U height,INT8U en);


/*******************************************************************************
* Function Name  : imageEncodeInit
* Description	 : image encode initial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0
*******************************************************************************/
int imageEncodeInit(void)
{
/*	
	csi_isp_cfg();
		
//	csi_frameend_isr_register(csi_lframe_done);
	csi_on();
*/
	return 0;
}
/*******************************************************************************
* Function Name  : imageEncodeUninit
* Description	 : image encode uninitial
* Input 		 : none
* Output		 : none 										   
* Return		 : int 0 success
*******************************************************************************/
int imageEncodeUninit(void)
{
//	csi_off();
	return 0;
}

#if 0
void small_pic_show_wm(u8* ydst_buf,u16 pos_x,u16 pos_y,char time_str)
{
	
	if(time_str == '/')
		SysCtrl.timestemp_idx =0;
	else if(time_str == ':')
		SysCtrl.timestemp_idx =0;
	else if(time_str == ' ')
		SysCtrl.timestemp_idx =0;
	else if(time_str == '1')
		SysCtrl.timestemp_idx =1;
	else if(time_str == '2')
		SysCtrl.timestemp_idx =2;
	else if(time_str == '3')
		SysCtrl.timestemp_idx =3;
	else if(time_str == '4')
		SysCtrl.timestemp_idx =4;
	else if(time_str == '5')
		SysCtrl.timestemp_idx =5;
	else if(time_str == '6')
		SysCtrl.timestemp_idx =6;
	else if(time_str == '7')
		SysCtrl.timestemp_idx =7;
	else if(time_str == '8')
		SysCtrl.timestemp_idx =8;
	else if(time_str == '9')
		SysCtrl.timestemp_idx =9;
	else if(time_str == '0')
		SysCtrl.timestemp_idx =0;
		


	//if(p_lcd_buffer_st)
	{
		u16 lcd_w,lcd_h;
		//small_pic_id_buf
		yuv420_draw_buf(ydst_buf,640,480,pos_x,pos_y,id_w_st,id_h_st,small_pic_id_buf[SysCtrl.timestemp_idx],id_w_st,id_h_st,YUV_ALPHA_Y,YUV_ALPHA_UV);
		//#endif
		ax32xx_sysDcacheWback((u32)ydst_buf,640*480*3/2);
		//ax32xx_sysDcacheFlush((u32)ydst_buf,640*480*3/2);
	}

}
#endif

/*******************************************************************************
* Function Name  : imageEncodeStart
* Description	 : take a photo
* Input 		 : FHANDLE fileHanle : file handle for write photo ram data to the file
                 : INT16U image_width : image width
                 : INT16U image_height:image height
                 : INT8U image_q qaulity,refence to __42__,__50__,__62__,__75__,__81__
                 : INT8U timestramp : time stramp.0->disable,
* Output		 : none 										   
* Return		 : int : 0 success
                                -1 fail
*******************************************************************************/

int imageEncodeStart(FHANDLE fileHanle,INT16U image_width,INT16U image_height,INT8U image_q,INT8U timestramp,INT8U frame_enable)
{
    INT32S res;
    INT32U addr,size,timeout,allsize,thumber_size;
	void *buff;//,*tbuff;
	s32 sync, sync_next;
	u16 csi_w,csi_h;
	u32 *thumber_buff = NULL;
	

	
    hal_mjpegEncodeResolutionImage(image_width,image_height,PIC_16_9);

	/*res = w_test_open(fileHanle, 1 << 13);
	if (res < 0)
	{
		watermark_buf_bmp2yuv_free();
		hal_mjpegEncodeStop();
		deg_Printf("image encode : w_test_open\n");
		return -1;
	}*/
		
	
	if(hal_mjpegMemInit(1)<0)
	{
		watermark_buf_bmp2yuv_free();
		hal_mjpegEncodeStop();   //uninit mjpegEncCtrl buffer
		deg_Printf("image encode : memory malloc fail\n");
		return -1;
	}
	

	{
		hal_csiResolutionGet(&csi_w,&csi_h);
		hal_mjpegPhotoStart(csi_w,csi_h,image_q,0,frame_enable,0); //crop ,use csi width & height ,encode a small jpeg
	}


    timeout = XOSTimeGet();
 	allsize = 0;
    while(1)
    {
        buff = hal_mjpegRawBufferGet(buff,&addr,&size,&sync, &sync_next);//hal_mjpegRawDataGet(&tbuff,&addr,&size);   // get jpeg frame addr & jpeg size.a total frame contains some of small buffers.here is the total size & start buffer addr
		if(buff)
	    {

			thumber_buff = buff;
			hal_mjpegRawDataSet(buff);
            res = 0;
			break;
	    }
		if((timeout+2000)<XOSTimeGet())
		{
			deg_Printf("image encode : timeout 2-second.\n");
			res = -1;
			break;
		}
    }
    
	
    if(timestramp)
	    videoRecordImageWatermark(image_width,image_height,0);  // disable	

	mjpegEncCtrl.uvbuffer = mjpegEncCtrl.ybuffer+mjpegEncCtrl.csi_width*mjpegEncCtrl.csi_height;
	system_audio_play(RES_MUSIC_TAKE_PHOTO,1,0);
	//if((SysCtrl.crop_level!=0) && (frame_enable!=0)) 
	{
		ax32xx_sysDcacheWback((u32)thumber_buff,size);
		ax32xx_csiEnable(1);
		dispLayerSetPIPMode(DISP_PIP_DISABLE);
		XOSTimeDly(20);
		u32 ticks=XOSTimeGet();
		Image_ARG_T arg;
		arg.target.width = mjpegEncCtrl.csi_width;
		arg.target.height = mjpegEncCtrl.csi_height;
		arg.yout  = (u8*)mjpegEncCtrl.ybuffer;
		arg.uvout = (u8*)mjpegEncCtrl.uvbuffer;
		arg.media.type = MEDIA_SRC_RAM;
		arg.media.src.buff = (u32)thumber_buff;//thumbNode;
		arg.wait = 1;  // wait decode end
		if(imageDecodeStart(&arg)<0)
		{
			deg("jpg decode fail\n");
		}
		else
		{
			ticks=XOSTimeGet()-ticks;
			deg("jpg decode ok,ticks=%d\n",ticks);
			u16 dec_width,dec_height;
			imageDecodeGetResolution(&dec_width,&dec_height);
			deg("jpg decode:w=%d,h=%d\n",dec_width,dec_height);
		}
		hal_csiEnable(1);
		dispLayerSetPIPMode(SysCtrl.pip_mode);
		
		//==end decode jpg==
	}
	
	
#if 1
#if 1
	//==when crop ,and frame ... at the same time 

	deg_Printf("before ybuf[%x] uvbuf[%x]\n",mjpegEncCtrl.ybuffer,mjpegEncCtrl.uvbuffer);

	{
		if(mjpegEncCtrl.frame_enable==1)
		{
				hal_custom_frame_add_mjpeg((u8*)mjpegEncCtrl.ybuffer);
		}
		
		if(2==SysCtrl.photo_mode_switch)	// effect mode
		{
				yuv420_effect_handle((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer,mjpegEncCtrl.csi_width,mjpegEncCtrl.csi_height);
		}
		else if(0==SysCtrl.photo_mode_switch)
		{
			if((7==SysCtrl.photo_mode_idx)||(8==SysCtrl.photo_mode_idx))//==pencil effect==
			{
				u8 color;
				if(7==SysCtrl.photo_mode_idx)
				{
					color=0;
				}
				else
				{
					color=1;
				}
					yuv420_pencil_sketch((u8*)mjpegEncCtrl.ybuffer,(u8*)mjpegEncCtrl.uvbuffer,mjpegEncCtrl.csi_width,mjpegEncCtrl.csi_height,color);
			}
		}
		deg_Printf("after ybuf[%x] uvbuf[%x]\n",mjpegEncCtrl.ybuffer,mjpegEncCtrl.uvbuffer);

	if(timestramp)
	watermark_bmp2yuv_draw((u8*)mjpegEncCtrl.ybuffer,WATERMAKE_SET_X_POS,WATERMAKE_SET_Y_POS,WATER_CHAR_GAP);
	
	}


#endif


	ax32xx_sysDcacheWback(mjpegEncCtrl.ybuffer,mjpegEncCtrl.csi_width*mjpegEncCtrl.csi_height*3/2);

//------------------------------------------------------
#if 0
	hal_mjpegPhotoStart(csi_w,csi_h,image_q,0,frame_enable,1);


	thumber_buff = NULL;

    timeout = XOSTimeGet();
 	allsize = 0;
    while(1)
    {
        buff = hal_mjpegRawBufferGet(buff,&addr,&size,&sync, &sync_next);//hal_mjpegRawDataGet(&tbuff,&addr,&size);   // get jpeg frame addr & jpeg size.a total frame contains some of small buffers.here is the total size & start buffer addr
		if(buff)
	    {
			thumber_buff = buff;
			thumber_size = size;
			hal_mjpegRawDataSet(buff);
            res = 0;
			break;
	    }
		if((timeout+2000)<XOSTimeGet())
		{
			deg_Printf("image encode : timeout 2-second.\n");
			res = -1;
			break;
		}
    }



	{
		ax32xx_csiEnable(1);
		dispLayerSetPIPMode(DISP_PIP_DISABLE);
		XOSTimeDly(20);
		u32 ticks=XOSTimeGet();
		Image_ARG_T arg;
		arg.target.width = mjpegEncCtrl.csi_width;
		arg.target.height = mjpegEncCtrl.csi_height;
		arg.yout  = (u8*)mjpegEncCtrl.ybuffer;
		arg.uvout = (u8*)mjpegEncCtrl.uvbuffer;
		arg.media.type = MEDIA_SRC_RAM;
		arg.media.src.buff = (u32)mjpegEncCtrl.mjpbuf;//thumbNode;
		arg.wait = 1;  // wait decode end
		if(imageDecodeStart(&arg)<0)
		{
			deg("jpg decode fail\n");
		}
		else
		{
			ticks=XOSTimeGet()-ticks;
			deg("jpg decode ok,ticks=%d\n",ticks);
			u16 dec_width,dec_height;
			imageDecodeGetResolution(&dec_width,&dec_height);
			deg("jpg decode:w=%d,h=%d\n",dec_width,dec_height);
		}
		hal_csiEnable(1);
		dispLayerSetPIPMode(SysCtrl.pip_mode);

		//==end decode jpg==
	}

	ax32xx_sysDcacheWback(mjpegEncCtrl.ybuffer,mjpegEncCtrl.csi_width*mjpegEncCtrl.csi_height*3/2);

#endif
//------------------------------------------------------
	
	hal_mjpegPhotoStart(image_width,image_height,image_q,0,frame_enable,1); //didn't crop ,use orig

	//ax32xx_mjpeg_manual_on();
	//ax32xx_intEnable(IRQ_JPGA,1); // enable jpegirq
			

    timeout = XOSTimeGet();
 	allsize = 0;
    while(1)
    {
        buff = hal_mjpegRawBufferGet(buff,&addr,&size,&sync, &sync_next);//hal_mjpegRawDataGet(&tbuff,&addr,&size);   // get jpeg frame addr & jpeg size.a total frame contains some of small buffers.here is the total size & start buffer addr
		if(buff)
	    {

			res =write(fileHanle,(void *)addr,(UINT)size);
			deg_Printf("the addr == %x,show size:%d\n",addr,size);
			if(res<0)
			{
				deg_Printf("image encode : error\n");
				break;
			}
			hal_mjpegRawDataSet(buff);
            res = 0;
			break;
	    }
		if((timeout+2000)<XOSTimeGet())
		{
			deg_Printf("image encode : timeout 2-second.\n");
			res = -1;
			break;
		}
    }

	
#endif
	//int Qthum_ret;
	//Qthum_ret = hal_mjpegStitchQuadThumb(fileHanle,(u8*)thumber_buff, thumber_size);
	//deg_Printf("Qthum_ret:%d\n",Qthum_ret);
	/*
	int fHandle;
	char *name;
	fHandle = task_image_createfile(VIDEO_CH_A,&name);
	write(fHandle,(void *)thumber_buff,(UINT)thumber_size);
	close(fHandle);
	*/
	watermark_buf_bmp2yuv_free();

	hal_mjpegEncodeStop();   //uninit mjpegEncCtrl buffer
	//w_test_close();	
	(void)allsize;
	thumber_buff = NULL;
	return res;
}
extern int mjpB_actech_as_photo(u16 win_w, u16 win_h, u8 quality, u8 frame_enable);
extern bool hal_mjpB_usensor_fram_rd(u8 **p, u32 *len, u32 *id,s32 *u_sync, s32 *u_sync_next);
extern bool hal_mjpB_usensor_fram_free(void);
extern void api_mjp_avi_stop(void);
#if 0
int imageEncodeStartB(FHANDLE fileHanle,INT16U image_width,INT16U image_height,INT8U image_q,INT8U timestramp,INT8U frame_enable)
{
#if 0
    INT32S res;
	INT32U len,id,timeout;
	s32 u_sync, u_sync_next;
	u8 *pbuf;
	debg(">>>Take back photo\n");
	videoRecordImageWatermark(image_width,image_height,timestramp);
	if(mjpB_actech_as_photo(image_width,image_height,image_q,frame_enable)<0)
		{
			videor_print("take photo: mjpB start fail.\n");
			return -1;
		}
	
    timeout = XOSTimeGet();
    while(1)
    {
		if(hal_mjpB_usensor_fram_rd(&pbuf, &len, &id,&u_sync, &u_sync_next)){
		//hal_mjpB_usensor_fram_free();
		//debg(">");
		}
		if((pbuf)&&(len))
	    {
			//while(tbuff)
			{
			//tbuff = hal_mjpegRawBufferGet(tbuff,&addr,&size);

				res =write(fileHanle,(void *)pbuf,len);
				if(res<0)
				{
					deg_Printf("image encode B: error\n");
					break;
				}
			}
			hal_mjpB_usensor_fram_free();
            res = 0;
			break;
	    }
		if((timeout+2000)<XOSTimeGet())
		{
			deg_Printf("image encodeB : timeout 2-second.\n");
			res = -1;
			break;
		}
    }
    
	api_mjp_avi_stop();
    if(timestramp)
	    videoRecordImageWatermark(image_width,image_height,0);  // disable
    
	return res;
#else
	return 0;
#endif
}
#endif


#endif









