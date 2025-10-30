/****************************************************************************
**
**                              DEVICE
** *   **             THE APPOTECH MULTIMEDIA PROCESSOR
**** **                  DEVICE CMOS-SENSOR HEADER
*** ***
**  * **               (C) COPYRIGHT 2016 BUILDWIN
**      **
**         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
**
* File Name   : cmos_sensor.c
* Author      : Mark.Douglas
* Version     : V100
* Date        : 09/22/2016
* Description : This file is CMOS-SENSOR device file
*
* History     :
* 2016-09-22  :
*      <1>.This is created by mark,set version as v100.
*      <2>.Add basic functions & information
******************************************************************************/
#include "cmos_sensor.h"
#include "../../ax32_platform_demo/application.h"

static INT8U devWRId, devRDId, devAddrW, devDataW;
// static _Sensor_Header_ *p_SensorHeader = NULL;
// volatile u8 senser_select;	// 0: select senser 1 , 1: select senser 2
INT8U senser_select = 0;
Sensor_Op_t *devSensorOp = NULL;
static Sensor_Op_t *devSensor1Op = NULL;
// static Sensor_Op_t *devSensor2Op=NULL;

ALIGNED(32)
Sensor_Op_t devSensor_Cmd;
// ALIGNED(32) Sensor_Op_t devSensor2_Cmd;
Sensor_Init_t *devSensorInit = NULL;
static Sensor_Init_t *devSensor1Init = NULL;
// static Sensor_Init_t *devSensor2Init=NULL;
ALIGNED(32)
Sensor_Init_t devSensor_Ident;
// ALIGNED(32) Sensor_Init_t devSensor2_Ident;

extern int _res_sensor_header_item_start;
extern int _res_sensor_header_len;

static u8 sen_mode = 0;

u8 get_sen_mode()

{
	return sen_mode;
}

void set_sen_mode(u8 mode)
{
	sen_mode = mode;
}
SENSOR_HEADER_SECTION const _Sensor_Header_ SensorHeader = {
	.header_items_addr = (u32)&_res_sensor_header_item_start,
	.header_items_total_size = (u32)&_res_sensor_header_len, //- sizeof(_Sensor_Header_)),
	.header_item_size = sizeof(Sensor_Init_t),
};

const Sensor_Op_t test_img =
	{
		.typ = CSI_TYPE_COLOR_BAR, // test color
		.pixelw = 640,
		.pixelh = 480,
		.hsyn = 1,
		.vsyn = 1,
		.colrarray = 0, // 0:_RGRG_ 1:_GRGR_,2:_BGBG_,3:_GBGB_
		.AVDD = 0x16,
		.DVDD = 0x06,
		.VDDIO = 0x10,
		.rotate_adapt = {0},
		.hvb_adapt = {48000000, 2000, 0, 0, 0xff},
		.mclk = 16000000, // 19000000
		.pclk_fir_en = 0,
		.pclk_inv_en = 0,
		.csi_tun = 0,
#if HAL_CFG_ISP_SUPPORT
		.isp_all_mod = 0, //_ISP_AUTO_<<_YGAMA_POS_,//_ISP_MODE_DEFAULT_
		//.hgrm_adapt = {256,0,8,240,480,800,1040,180,360,640,680,0x5a96a555,0x1a96a,0x1000000,0x0},//0x5a96a555,0x1a96a//0x2000000,0x0//0x0a815000,0x5
		.ae_adapt = {.exp_adapt = {4, {24, 40, 60, 80, 80, 80, 100, 136}, 3, 195 * 256, 12, 4, 2048, 0x4142},																				  // 28->36
					 .hgrm_adapt = {256, 160, 320, 960, 1120, 200, 400, 640, 680, 0x55555555, 0x55555555, 0x55555555, 0x01, {8, 9, 10, 11, 12, 13, 14, 15}, {8, 9, 10, 11, 12, 13, 14, 15}}}, // 0.5,2,2
		.blc_adapt = {-4, -6, -6, -4},
		.ddc_adapt = {2, 1, 0, 0, 16, {8, 8, 8, 8, 8, 8, 8, 8}, {8, 8, 8, 8, 8, 8, 8, 8}, 1, {2, 2, 2, 2, 2, 2, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0}, {5, 8, 12, 15, 20, 25, 30}, 0},
		.awb_adapt = {0x03, 188, 620, 3, 2, 0xa, 0xc0, 0, 3, 0, 3, 0, 0, {(348 << 20) | (256 << 10) | (370 << 0), (368 << 20) | (256 << 10) | (350 << 0), (465 << 20) | (256 << 10) | (225 << 0), (370 << 20) | (256 << 10) | (385 << 0), (370 << 20) | (256 << 10) | (385 << 0)}, 0, {0}, {0}, {0}, 0}, // manual:0xff
		//.ccm_adapt = {0x38,0x00,0x04,0x04,0x44,-0x4,0x04,-0x4,0x40,0x00,0x00,0x00},
		.ccm_adapt = {{0x100, 0x00, 0x00,
					   0x00, 0x100, 0x00,
					   0x00, 0x00, 0x100},
					  0x0c,
					  0xc,
					  0xc},
		//.ygama_adapt = {NULL,0,{11,14,17,17,17,17,17,17},0,0,0xff,144,208,96,144},
		.ygama_adapt = {0, {0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0xff, 144, 208, 96, 144},
		.rgbgama_adapt = {0, {1, 1, 1, 1, 1, 1, 1, 1}, {20, 12, 12, 12, 12, 12, 12, 12}, 0, 0xff, 0, 0xff, 0, 0xff, 64, 224, 4000}, // 2
		.ch_adapt = {1, 1, {1, 1, 1, 1, 1, 1}, {320, 192, 320, 256, 384, 128}, {64, 64, 192, 128, 256, 0}, {12, 6, 6, 12, 6, 6}, {6, 12, 6, 6, 12, 6}, {6, 6, 12, 6, 6, 12}, {2, 4, 6, 8, 12, 16, 20, 24, 28, 28, 28, 28, 28, 28, 28, 28, 28}, {32, 32, 32, 32, 32, 32, 32, 32}},
		.vde_adapt = {0x80, 0x80, 0x80, 0x80, {72, 72, 72, 72, 72, 72, 72, 72, 72}},

		.cfd_adapt = {4, 0xb0, 0x20, 1, 1, 2, 0xff, 0},
		.md_adapt = {40, 40, 1, 1, 1280, 180, 720},
		.p_fun_adapt = {NULL, NULL, NULL},
#endif
};

#if HAL_CFG_ISP_SUPPORT
const Sensor_Init_t null_init = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, gama, sizeof(gama)};
// const Sensor_Init_t null2_init={0x00,0x00,0x00,0x00,0x00,0x00,gama,sizeof(gama)};
#else
const Sensor_Init_t null_init = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*,gama,sizeof(gama)*/};
// const Sensor_Init_t null2_init={0x00,0x00,0x00,0x00,0x00,0x00/*,gama,sizeof(gama)*/};
#endif

/*******************************************************************************
 * Function Name  : sensor_iic_enable
 * Description    : enable senor iic for write/read
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_enable(void)
{
	hal_iic0Init(); // initial iic0
#if 0				//(2 == SENSOR_NUM)
	gsensor_iic_enable();	// senser2 use iic1
#endif
}
/*******************************************************************************
 * Function Name  : sensor_iic_disable
 * Description    : disable senor iic for write/read
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_disable(void)
{
}
/*******************************************************************************
 * Function Name  : sensor_iic_write
 * Description    : senor iic  write
 * Input          : INT8U *data : data & addr
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensor_iic_write(INT8U *data)
{
	INT16U addr;
	if (1) // 0==senser_select)
	{
		if (devAddrW == 2)
		{
			addr = (data[0] << 8) | data[1];
			hal_iic016bitAddrWrite(devWRId, addr, &data[2], devDataW);
		}
		else
		{
			hal_iic08bitAddrWrite(devWRId, data[0], &data[1], devDataW);
		}
	}
#if HAL_CFG_EN_IIC1
	else
	{
		//==iic 1==
		if (devAddrW == 2)
		{
			addr = (data[0] << 8) | data[1];
			hal_iic116bitAddrWrite(devWRId, addr, &data[2], devDataW);
		}
		else
		{
			hal_iic18bitAddrWrite(devWRId, data[0], &data[1], devDataW);
		}
	}
#endif
}
/*******************************************************************************
 * Function Name  : sensor_iic_read
 * Description    : senor iic  read
 * Input          : INT8U *data : data & addr
 * Output         : none
 * Return         : none
 *******************************************************************************/
INT32U sensor_iic_read(INT8U *data)
{
	INT32U temp = 0;
	INT16U addr;

	if (1) //(0==senser_select)
	{
		if (devAddrW == 2)
		{
			addr = (data[0] << 8) | data[1];
			hal_iic016bitAddrRead(devWRId, addr, (u8 *)&temp, devDataW);
		}
		else
		{
			hal_iic08bitAddrRead(devWRId, data[0], (u8 *)&temp, devDataW);
		}
	}
#if HAL_CFG_EN_IIC1
	else
	{
		if (devAddrW == 2)
		{
			addr = (data[0] << 8) | data[1];
			hal_iic116bitAddrRead(devWRId, addr, (u8 *)&temp, devDataW);
		}
		else
		{
			hal_iic18bitAddrRead(devWRId, data[0], (u8 *)&temp, devDataW);
		}
	}
#endif
	return temp;
}
/*******************************************************************************
* Function Name  : sensor_iic_info
* Description    : senor iic  slave set
* Input          : INT8U wid : write id
					  INT8U rid  : read id
					  INT8U awidth : addr width
					  INT8U dwidth : data width
* Output         : none
* Return         : none
*******************************************************************************/
void sensor_iic_info(INT8U wid, INT8U rid, INT8U awidth, INT8U dwidth)
{
	devWRId = wid;
	devRDId = rid;
	devAddrW = awidth;
	devDataW = dwidth;
}
/*******************************************************************************
* Function Name  : sensorCheckId
* Description    : senor check id
* Input          : sensor_adpt *p_sensor_cmd : sensor op
* Output         : none
* Return         : 1 : check ok
					  -1: check fail
*******************************************************************************/
// u8 senid=0;
static int sensorCheckId(Sensor_Init_t *p_sensor_ident)
{
	INT8U u8Buf[3];
	INT32U id = 0;

	u8Buf[0] = p_sensor_ident->id_reg;
	if (p_sensor_ident->addr_num == 2)
	{
		u8Buf[0] = p_sensor_ident->id_reg >> 8;
		u8Buf[1] = p_sensor_ident->id_reg;
	}

	sensor_iic_info(p_sensor_ident->w_cmd, p_sensor_ident->r_cmd, p_sensor_ident->addr_num, p_sensor_ident->data_num);
	id = sensor_iic_read(u8Buf);
	deg_Printf("sen1 id =%d,p_sensor_ident->id=%d\n", id, p_sensor_ident->id);
	uart_Printf("senid1: %x %x %x %x\n", p_sensor_ident->id, id, p_sensor_ident->w_cmd, p_sensor_ident->r_cmd);

	if (id == p_sensor_ident->id)
		return 1;
	else
		return -1;
}

#if 0
static int sensor2CheckId(Sensor_Init_t *p_sensor_ident)
{
	INT8U u8Buf[4];
	INT32U id= 0;
	u8Buf[0] = p_sensor_ident->id_reg;
	if(p_sensor_ident->addr_num == 2)
	{
		u8Buf[0] = p_sensor_ident->id_reg>>8;
		u8Buf[1] = p_sensor_ident->id_reg;
	}

	sensor_iic_info(p_sensor_ident->w_cmd,p_sensor_ident->r_cmd,p_sensor_ident->addr_num,p_sensor_ident->data_num);
	id = sensor_iic_read(u8Buf);

	deg_Printf("sen2 id =%x,p_sensor_ident->id=%d\n",id,p_sensor_ident->id);
	uart_Printf("senid2: %x %x %x %x\n",p_sensor_ident->id,id,p_sensor_ident->w_cmd,p_sensor_ident->r_cmd);
	if(id == p_sensor_ident->id)
		return 1;
	else
		return -1;
}
#endif

static Sensor_Op_t *sensor_adpt_load(Sensor_Init_t *p_sen_init, u8 *init_buf)
{
	if (p_sen_init->sensor_struct_addr)
	{
		u32 sen_struct_flash_adr = SEN_RES_FLASH_ADDR(p_sen_init->sensor_struct_addr);
		hal_spiFlashRead(sen_struct_flash_adr, (u32)&devSensor_Cmd, p_sen_init->sensor_struct_size);
#if 1
		if (p_sen_init->sensor_init_tab_adr)
		{
			u32 init_tab_flash_adr = SEN_RES_FLASH_ADDR(p_sen_init->sensor_init_tab_adr);
			hal_spiFlashRead(init_tab_flash_adr, (u32)init_buf, p_sen_init->sensor_init_tab_size);
			return &devSensor_Cmd;
		}
#else
		if (1 == get_sen_mode())
		{
			deg_Printf("--------------------after---------------%d\n", get_sen_mode());
			if (p_sen_init->sensor_init_tab_adr2)
			{
				u32 init_tab_flash_adr = SEN_RES_FLASH_ADDR(p_sen_init->sensor_init_tab_adr2);
				hal_spiFlashRead(init_tab_flash_adr, (u32)init_buf, p_sen_init->sensor_init_tab_size2);
				return &devSensor_Cmd;
			}
		}
		else
		{
			deg_Printf("--------------------front---------------%d\n", get_sen_mode());
			if (p_sen_init->sensor_init_tab_adr)
			{
				u32 init_tab_flash_adr = SEN_RES_FLASH_ADDR(p_sen_init->sensor_init_tab_adr);
				hal_spiFlashRead(init_tab_flash_adr, (u32)init_buf, p_sen_init->sensor_init_tab_size);
				return &devSensor_Cmd;
			}
		}
#endif
	}
	debg("sensor_adpt_load err %x %x\n", p_sen_init->sensor_struct_addr, p_sen_init->sensor_init_tab_adr);
	return NULL;
}

#if 0
static Sensor_Op_t * sensor2_adpt_load(Sensor_Init_t *p_sen_init,u8 *init_buf){
	if(p_sen_init->sensor_struct_addr){
		u32  sen_struct_flash_adr = SEN_RES_FLASH_ADDR(p_sen_init->sensor_struct_addr);
		hal_spiFlashRead(sen_struct_flash_adr,(u32)&devSensor2_Cmd,p_sen_init->sensor_struct_size);
		if(p_sen_init->sensor_init_tab_adr){
			u32  init_tab_flash_adr = SEN_RES_FLASH_ADDR(p_sen_init->sensor_init_tab_adr);
			hal_spiFlashRead(init_tab_flash_adr,(u32)init_buf,p_sen_init->sensor_init_tab_size);
			return &devSensor2_Cmd;
		}
	}
	debg("sensor_adpt_load2 err %x %x\n",p_sen_init->sensor_struct_addr,p_sen_init->sensor_init_tab_adr);
	return NULL;
}
#endif

/*******************************************************************************
 * Function Name  : sensorAutoCheck
 * Description    : auto check,find support sensor in initial table
 * Input          : none
 * Output         : none
 * Return         : sensor_adpt *p_sensor_cmd : support sensor
 *******************************************************************************/

static Sensor_Op_t *sensorAutoCheck(u8 *init_buf)
{
	INT8U i;
	_Sensor_Header_ *p_SensorHeader;

	Sensor_Init_t devSensorInitTable[64];
	Sensor_Op_t *devSensor_Struct = NULL;

	p_SensorHeader = (_Sensor_Header_ *)hal_sysMemMalloc(sizeof(_Sensor_Header_), 64);
	hal_spiFlashRead(SEN_RES_FLASH_ADDR(&SensorHeader), (u32)p_SensorHeader, sizeof(_Sensor_Header_));
	int num = p_SensorHeader->header_items_total_size / p_SensorHeader->header_item_size;
	hal_spiFlashRead(SEN_RES_FLASH_ADDR(p_SensorHeader->header_items_addr), (u32)devSensorInitTable, p_SensorHeader->header_items_total_size);
	uart_Printf("_res_sensor_tab =%x ", p_SensorHeader->header_items_addr);
	deg_Printf("_res_sensor_tab_len =%d\n", num);
	hal_sysMemFree(p_SensorHeader);

	for (i = 0; i < num; i++)
	{
		if (sensorCheckId(&(devSensorInitTable[i])) >= 0)
		{
			deg_Printf("sen id =%x\n", devSensorInitTable[i].id);
			memcpy(&devSensor_Ident, &devSensorInitTable[i], sizeof(Sensor_Init_t));
			devSensor1Init = &devSensor_Ident;
			devSensor_Struct = sensor_adpt_load(devSensor1Init, init_buf);
			break;
		}
	}
	if (devSensor_Struct == NULL)
	{
		deg_Printf("sen id Er: unkown!\n");
		devSensor1Init = (Sensor_Init_t *)&null_init;
		return ((Sensor_Op_t *)&test_img); // index 0 is test only
	}
	return devSensor_Struct;
}

#if 0 //(2 == SENSOR_NUM)
static Sensor_Op_t * sensor2AutoCheck(u8 *init_buf){
	INT8U i;
	Sensor_Init_t devSensorInitTable[64];
	Sensor_Op_t * devSensor_Struct=NULL;
		
	if(NULL==p_SensorHeader)
	{
		p_SensorHeader = (_Sensor_Header_ *)hal_sysMemMalloc(sizeof(_Sensor_Header_),64);
	}
	hal_spiFlashRead(SEN_RES_FLASH_ADDR(&SensorHeader),(u32)p_SensorHeader,sizeof(_Sensor_Header_));
	int num = p_SensorHeader->header_items_total_size / p_SensorHeader->header_item_size;
	hal_spiFlashRead(SEN_RES_FLASH_ADDR(p_SensorHeader->header_items_addr),(u32)devSensorInitTable,p_SensorHeader->header_items_total_size);

	deg_Printf("_res_sensor_tab_len2 =%d\n",num);

	for(i=0;i<num;i++)
	{
		if(sensor2CheckId(&(devSensorInitTable[i]))>=0)
		{
			//deg_Printf("sen2 id =%x\n",devSensorInitTable[i].id);
			memcpy(&devSensor2_Ident, &devSensorInitTable[i],sizeof(Sensor_Init_t));
			devSensor2Init = &devSensor2_Ident;
			devSensor_Struct = sensor2_adpt_load(devSensor2Init,init_buf);
			break;
		}
	}
	if(devSensor_Struct == NULL)
	{
		deg_Printf("sen2 id Er: unkown!\n");
		devSensor2Init = (Sensor_Init_t *)&null2_init;
		return ((Sensor_Op_t *)&test_img); // index 0 is test only
	}
	else
	{
		deg_Printf("sen2 id:0x%x\n",devSensor2Init->id);
	}
	
	return devSensor_Struct;
}
#endif

/*******************************************************************************
 * Function Name  : SensorGetName
 * Description    : get Sensor name
 * Input          :
 * Output         : none
 * Return         : char *
 *******************************************************************************/
char *SensorGetName(void)
{
	if (devSensorInit == NULL)
		return ((char *)("NULL"));

	return (char *)devSensorInit->sensor_name;
}

// ALIGNED(32) static u8 initbuf[CMOS_INIT_LEN_MAX];

/*******************************************************************************
 * Function Name  : sensorInit
 * Description    : initial cmos sensor
 * Input          : none
 * Output         : none
 * Return         : none
 *******************************************************************************/
void sensorInit(void)
{
	u32 i;
	// senser_select=0;
	u8 *initbuf;

	initbuf = (u8 *)hal_sysMemMalloc(CMOS_INIT_LEN_MAX, 64);
	if (initbuf == NULL)
	{
		return;
	}
#if 0
        ax32xx_sysLDOSet(SYS_LDO_LSEN,0,0); 
        ax32xx_sysLDOSet(SYS_LDO_HSEN,0,0);
        ax32xx_csiIOConfigUinit();
        hal_gpioInit(GPIO_PF,GPIO_PIN10,GPIO_OUTPUT,GPIO_PULL_FLOATING);//PF11 sensor power
        if(senser_select == 0)
                hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);
        else
                hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_HIGH);
        XOSTimeDly(500);
        
        ax32xx_sysLDOSet(SYS_LDO_LSEN,SYS_VOL_V1_8,1);
        ax32xx_sysLDOSet(SYS_LDO_HSEN,SYS_VOL_V3_0,1);
        //ax32xx_csiIOConfig();
        XOSTimeDly(200);
#endif
	// hal_gpioInit(SEN1_PWDN_CH,SEN1_PWDN_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	// hal_gpioWrite(SEN1_PWDN_CH,SEN1_PWDN_PIN,GPIO_HIGH);
	//  hal_gpioInit(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	//  hal_gpioWrite(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_HIGH);
#if 0 //(2 == SENSOR_NUM)//0//(2 == SENSOR_NUM)
	hal_gpioInit(SEN2_PWDN_CH,SEN2_PWDN_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(SEN2_PWDN_CH,SEN2_PWDN_PIN,GPIO_HIGH);
	hal_gpioInit(SEN2_RESET_CH,SEN2_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(SEN2_RESET_CH,SEN2_RESET_PIN,GPIO_HIGH);
#endif

	hal_csiMCLKSet(16000000); // default mclk for id check

	sensor_iic_enable();

#if (1 == SENSOR_NUM)
	hal_gpioInit(SEN1_PWDN_CH, SEN1_PWDN_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(SEN1_PWDN_CH, SEN1_PWDN_PIN, GPIO_HIGH);
	hal_gpioInit(SEN1_PWDN_CH, SEN1_PWDN_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(SEN1_PWDN_CH, SEN1_PWDN_PIN, GPIO_LOW);
	// hal_gpioInit(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	// hal_gpioWrite(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_HIGH);
	// hal_sysDelayMS(50);
	// hal_gpioWrite(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_LOW);
	// hal_sysDelayMS(100);
	// hal_gpioWrite(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_HIGH);
	// hal_sysDelayMS(50);

	if (devSensorOp == NULL)
		devSensorOp = sensorAutoCheck(initbuf);

	if (devSensorOp)
	{
		ax32xx_sysLDOSet(SYS_LDO_LSEN, devSensorOp->DVDD, 1);
		ax32xx_sysCpuMsDelay(1);
		ax32xx_sysLDOSet(SYS_LDO_HSEN, devSensorOp->VDDIO, 1);
		ax32xx_sysCpuMsDelay(1);
	}

	hal_csiMCLKSet(devSensorOp->mclk);
	devSensorInit = devSensor1Init;
	if (devSensorInit)
	{
		sensor_iic_info(devSensorInit->w_cmd, devSensorInit->r_cmd, devSensorInit->addr_num, devSensorInit->data_num);
		if (devSensorInit->sensor_init_tab_adr != NULL)
		{
			for (i = 0;; i += devSensorInit->addr_num + devSensorInit->data_num)
			{
				if ((initbuf[i] == 0xFF) && (initbuf[i + 1] == 0xFF))
					break;
				sensor_iic_write(&initbuf[i]);

				if (i == 0)
					hal_sysDelayMS(10);
			}
		}
		hal_csiRegister(devSensorOp, devSensorInit);
	}

	if (devSensorOp)
	{
		sensor_frame_rate_adapt(0, devSensorOp->hvb_adapt.fps);
	}

#else // 2 cam
#if 0
	//==senser 2 init==
	hal_gpioInit(SEN2_PWDN_CH,SEN2_PWDN_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(SEN2_PWDN_CH,SEN2_PWDN_PIN,GPIO_LOW);
	hal_gpioInit(SEN2_RESET_CH,SEN2_RESET_PIN,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	hal_gpioWrite(SEN2_RESET_CH,SEN2_RESET_PIN,GPIO_HIGH);
	hal_sysDelayMS(50);
	hal_gpioWrite(SEN2_RESET_CH,SEN2_RESET_PIN,GPIO_LOW);
	hal_sysDelayMS(100);
	hal_gpioWrite(SEN2_RESET_CH,SEN2_RESET_PIN,GPIO_HIGH);
	hal_sysDelayMS(50);
#else
/*	hal_dacHPSet(1,HP_VDD_2_8V);
	hal_sysDelayMS(50);

	hal_dacHPSet(0,0);
	hal_sysDelayMS(50);

	hal_dacHPSet(1,HP_VDD_2_8V);
	hal_sysDelayMS(50);  */
#endif
	if (senser_select == 0)
	{
#if 1
		// hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);//PF11 sensor power
		// hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_HIGH);
		// hal_sysDelayMS(50);

		hal_gpioInit(GPIO_PF, GPIO_PIN10, GPIO_OUTPUT, GPIO_PULL_FLOATING); // PF10 sensor reset
		// hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_LOW);
		// hal_sysDelayMS(50);
		hal_gpioWrite(GPIO_PF, GPIO_PIN10, GPIO_HIGH);
		// hal_sysDelayMS(50);
		// hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_LOW);
		hal_sysDelayMS(20);
#else
		hal_gpioInit(GPIO_PF, GPIO_PIN11, GPIO_OUTPUT, GPIO_PULL_FLOATING); // PF11 sensor power
		hal_gpioWrite(GPIO_PF, GPIO_PIN11, GPIO_LOW);
		hal_sysDelayMS(50);

		hal_gpioInit(GPIO_PF, GPIO_PIN10, GPIO_OUTPUT, GPIO_PULL_FLOATING); // PF10 sensor reset
		hal_gpioWrite(GPIO_PF, GPIO_PIN10, GPIO_HIGH);
		hal_sysDelayMS(50);
		hal_gpioWrite(GPIO_PF, GPIO_PIN10, GPIO_LOW);
		hal_sysDelayMS(50);
		hal_gpioWrite(GPIO_PF, GPIO_PIN10, GPIO_HIGH);
		hal_sysDelayMS(20);
#endif
	}
#if 0
	else if(senser_select == 1)
	{
#if 0
		hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);//PF11 sensor power
		hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);
		hal_sysDelayMS(50);
		
		hal_gpioInit(GPIO_PF,GPIO_PIN10,GPIO_OUTPUT,GPIO_PULL_FLOATING);//PF10 sensor reset
		hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_HIGH);
		hal_sysDelayMS(50);
		hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_LOW);
		hal_sysDelayMS(50);
		hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_HIGH);
		hal_sysDelayMS(20);
#else
		hal_gpioInit(GPIO_PF,GPIO_PIN11,GPIO_OUTPUT,GPIO_PULL_FLOATING);//PF11 sensor power
		hal_gpioWrite(GPIO_PF,GPIO_PIN11,GPIO_LOW);
		hal_sysDelayMS(50);
		
		hal_gpioInit(GPIO_PF,GPIO_PIN10,GPIO_OUTPUT,GPIO_PULL_FLOATING);//PF10 sensor reset
		//hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_HIGH);
		//hal_sysDelayMS(50);
		hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_LOW);
		//hal_sysDelayMS(50);
		//hal_gpioWrite(GPIO_PF,GPIO_PIN10,GPIO_HIGH);
		hal_sysDelayMS(20);
#endif
	}
#endif
#if 0
	senser_select=1;

	if(NULL==devSensor2Op)
		devSensor2Op = sensor2AutoCheck(initbuf);
	
	devSensorOp=devSensor2Op;
	devSensorInit = devSensor2Init;
	if(devSensorOp)
	{
		ax32xx_sysLDOSet(SYS_LDO_LSEN,devSensorOp->DVDD,1); 
		ax32xx_sysCpuMsDelay(1);
		ax32xx_sysLDOSet(SYS_LDO_HSEN,devSensorOp->VDDIO,1);
		ax32xx_sysCpuMsDelay(1);
		hal_csiMCLKSet(devSensorOp->mclk);
	}

	if(devSensorInit)
	{
		sensor_iic_info(devSensorInit->w_cmd,devSensorInit->r_cmd,devSensorInit->addr_num,devSensorInit->data_num);
		if(devSensorInit->sensor_init_tab_adr!=NULL)
		{
			for(i=0;;i+=devSensorInit->addr_num+devSensorInit->data_num)
			{
				if((initbuf[i]==0xFF)&&(initbuf[i+1]==0xFF))
					break;

				sensor_iic_write(&initbuf[i]);

				if(i==0)
					hal_sysDelayMS(10);
			}
		}
		hal_csiRegister(devSensorOp,devSensorInit);
	}
#endif
#if 0
	hal_gpioWrite(SEN2_PWDN_CH,SEN2_PWDN_PIN,GPIO_HIGH);
	hal_sysDelayMS(10);
	//===sensor1 init==
	hal_gpioWrite(SEN1_PWDN_CH,SEN1_PWDN_PIN,GPIO_LOW);
	hal_gpioWrite(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_HIGH);
	hal_sysDelayMS(30);
	hal_gpioWrite(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_LOW);
	hal_sysDelayMS(50);
	hal_gpioWrite(SEN1_RESET_CH,SEN1_RESET_PIN,GPIO_HIGH);
	hal_sysDelayMS(50);
#else
/*	hal_dacHPSet(0,0);
	hal_sysDelayMS(50);

	hal_dacHPSet(1,HP_VDD_2_8V);
	hal_sysDelayMS(50);

	hal_dacHPSet(0,0);
	hal_sysDelayMS(50);
*/
#endif
	// senser_select=0;

#if 1
	if (devSensor1Op == NULL)
	{
		devSensor1Op = sensorAutoCheck(initbuf);
	}
	else
	{
		devSensor1Op = sensorAutoCheck(initbuf);
	}
#endif
	devSensorOp = sensorAutoCheck(initbuf);
	deg_Printf("check sensor id end\n");
	devSensorOp = devSensor1Op;
	devSensorInit = devSensor1Init;
	if (devSensorOp)
	{
		ax32xx_sysLDOSet(SYS_LDO_LSEN, devSensorOp->DVDD, 1);
		ax32xx_sysCpuMsDelay(20);
		ax32xx_sysLDOSet(SYS_LDO_HSEN, devSensorOp->VDDIO, 1);
		ax32xx_sysCpuMsDelay(20);
		hal_csiMCLKSet(devSensorOp->mclk);

		// if(devSensorInit)
		// {
		sensor_iic_info(devSensorInit->w_cmd, devSensorInit->r_cmd, devSensorInit->addr_num, devSensorInit->data_num);
		if (devSensorInit->sensor_init_tab_adr != NULL)
		{
			for (i = 0;; i += devSensorInit->addr_num + devSensorInit->data_num)
			{
				if ((initbuf[i] == 0xFF) && (initbuf[i + 1] == 0xFF))
					break;
				sensor_iic_write(&initbuf[i]);

				if (i == 0)
					hal_sysDelayMS(10);
			}
		}
		hal_csiRegister(devSensorOp, devSensorInit);
	}
	// }

	// if(devSensorOp)
	// {
#if HAL_CFG_ISP_SUPPORT
	sensor_frame_rate_adapt(0, devSensorOp->hvb_adapt.fps);
#endif
	// }
	sensor_rotate(!senser_select);

#endif
#if HAL_CFG_ISP_SUPPORT
	isp_init();
#endif

	hal_sysMemFree(initbuf);
}
/*******************************************************************************
 * Function Name  : uart_to_iic
 * Description    :
 * Output         : None
 * Return         :
 *******************************************************************************/
INT8U *uart_to_iic(u8 *p)
{
	u8 iicbuf[6];
	u8 *seek;

	if (NULL != (seek = str_seek(p, "-iic -w:")))
	{
		iicbuf[0] = (char_2_hex(seek[0]) << 4) | (char_2_hex(seek[1]) << 0);
		iicbuf[1] = (char_2_hex(seek[3]) << 4) | (char_2_hex(seek[4]) << 0);
		iicbuf[2] = (char_2_hex(seek[6]) << 4) | (char_2_hex(seek[7]) << 0);
		sensor_iic_write(iicbuf);
		debg(" -ok\n");
		if (NULL != (seek = str_seek(seek, "-iic -w:")))
		{
			iicbuf[0] = (char_2_hex(seek[0]) << 4) | (char_2_hex(seek[1]) << 0);
			iicbuf[1] = (char_2_hex(seek[3]) << 4) | (char_2_hex(seek[4]) << 0);
			iicbuf[2] = (char_2_hex(seek[6]) << 4) | (char_2_hex(seek[7]) << 0);
			sensor_iic_write(iicbuf);
			debg(" -ok\n");
		}
	}

	if (NULL != (seek = str_seek(p, "-iic -r:")))
	{
		iicbuf[0] = (char_2_hex(seek[0]) << 4) | (char_2_hex(seek[1]) << 0);
		iicbuf[1] = (char_2_hex(seek[3]) << 4) | (char_2_hex(seek[4]) << 0);
		iicbuf[2] = (char_2_hex(seek[6]) << 4) | (char_2_hex(seek[7]) << 0);
		debg(" -ok >%x \n", sensor_iic_read(iicbuf));
		if (NULL != (seek = str_seek(seek, "-iic -r:")))
		{
			iicbuf[0] = (char_2_hex(seek[0]) << 4) | (char_2_hex(seek[1]) << 0);
			iicbuf[1] = (char_2_hex(seek[3]) << 4) | (char_2_hex(seek[4]) << 0);
			debg(" -ok >%x \n", sensor_iic_read(iicbuf));
		}
	}

	return p;
}

u16 sensor_get_w()
{
	if (NULL == devSensorOp)
	{
		return 0;
	}
	else
	{
		return devSensorOp->pixelw;
	}
}

u16 sensor_get_h()
{
	if (NULL == devSensorOp)
	{
		return 0;
	}
	else
	{
		return devSensorOp->pixelh;
	}
}
//-------------------------------uart isp debug-----------------------------------------------
#if CMOS_USART_ONLINE_DBG > 0

#define SENSOR_DBG_NUM 2
#define SENSOR_DBG_SIZE 256
static INT8U sensorDBGBuffer[SENSOR_DBG_NUM][SENSOR_DBG_SIZE]; // __attribute__ ((section("._urx_buf_"))); //512

static INT16U revCnt;
static INT8U revTimeout, revBuf, revRdy;

/*******************************************************************************
 * Function Name   : sensorDBGService
 * Description	  : sensor on line debug process
 * Input		      : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGIOCtrl(u8 *p)
{

	uart_PutStr(p);
	uart_to_iic(p);
	uart_to_isp(p);
	if (NULL != str_seek(p, "-h:"))
	{
		debg("\n");
		// debg("-isp -ccm:0,(36,fa,00,02,4e,00,04,f8,3e,00,00,00)\n");
		debg("-isp -ccm:0,(36,fe,fa,02,4a,fe,06,fa,48,00,00,00)\n");
		debg("-isp -lsc:0,(f9,0280,0168,10,10,10,10,10,10)\n");
		debg("-isp -awb:0,(10,10,f0,0a,20,10)\n");
		debg("-isp -bhcs:0,(80,80,80,40)\n");
		debg("-isp -gain:0,(00,00,00)\n");
		debg("-isp -ygamma:ff,00,ff,00,04\n");
		debg("-isp -whdr:ff,00,ff,00,04\n");
		debg("-isp -rgbgma:ff,(00,ff,00,ff,00,ff)\n");
		debg("-isp -lc:ff,(00,ff,00,ff,00,ff)\n");
		debg("-isp -acutance:ff\n");
		debg("-iic -w:00,00\n");
		debg("-iic -r:00,00\n");
		debg("-csi -cfg:00\n");
		debg("-isp -hgrm:ff\n");
		debg("-isp -DDC:0,(40,40,30,08)\n");
		debg("-save -f\n");
	}
}
/*******************************************************************************
 * Function Name  : sensorDBGService
 * Description	  : sensor on line debug initial
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGInit(void)
{
	revTimeout = 0;
	revBuf = 0;
	revCnt = 0;
	revRdy = 0;

	hal_uartRXIsrRegister(sensorDBGRecv);
}
/*******************************************************************************
 * Function Name  : sesnorDBGService
 * Description	  : sensor on line debug service.user should not call this function in isr
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sesnorDBGService(void)
{
	int i, idx;

	for (i = 0; i < 4; i++)
	{
		idx = revRdy & (3 << (4 - i));
		if (idx)
		{
			sensorDBGIOCtrl(sensorDBGBuffer[idx]);
			revRdy &= ~(3 << (4 - i));
		}
	}
}
/*******************************************************************************
 * Function Name  : sensorDBGService
 * Description	  : sensor on line debug timeout check
 * Input		  : none
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGTimeout(void)
{
	if (revCnt)
	{
		revTimeout++;
		if (revTimeout > 200)
		{
			sensorDBGBuffer[revBuf][revCnt] = '\0';
			revRdy = (revRdy << 2) | revBuf;
			revBuf++;
			if (revBuf >= SENSOR_DBG_NUM)
				revBuf = 0;
			revCnt = 0;
			revTimeout = 0;
		}
	}
}
/*******************************************************************************
 * Function Name  : sensorDBGRecv
 * Description	  : sensor on line debug recivce
 * Input		  : u8 data : data
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void sensorDBGRecv(u8 data)
{
	revTimeout = 0;

	sensorDBGBuffer[revBuf][revCnt++] = data;
	if (revCnt >= (SENSOR_DBG_SIZE - 1))
	{
		sensorDBGBuffer[revBuf][revCnt] = 0;
		revRdy = (revRdy << 2) | revBuf;
		revBuf++;
		if (revBuf >= SENSOR_DBG_NUM)
			revBuf = 0;
		revCnt = 0;
	}
}

#endif // CMOS_USART_ONLINE_DBG

/**************************************************************************************************
 * 							usb online debg
 ***************************************************************************************************/

/*******************************************************************************
 * Function Name  : Sensor_DebugRegister
 * Description	  : sensor on line debug recivce
 * Input		      :
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void Sensor_DebugRegister(u8 *pbdata, u32 trans_len, u32 param_3B)
{
	// debg("Sensor_DebugRegister\n");
	// debg("%x%x",pbdata[0],pbdata[1]);
	u8 u8AddrLength = param_3B & 0xff;
	u8 u8DataLength = (param_3B & 0xff00) >> 8;
	// debg("%x,%x",u8AddrLength,u8DataLength);

	// sensor_iic_write(pbdata);

	if (u8AddrLength == 2)
	{
		INT16U addr;
		addr = pbdata[0];
		addr = (addr << 8) | pbdata[1];
		hal_iic016bitAddrWrite(devWRId, addr, &pbdata[2], u8DataLength);
	}
	else
	{
		hal_iic08bitAddrWrite(devWRId, pbdata[0], &pbdata[1], u8DataLength);
	}
}

/*******************************************************************************
 * Function Name  : usb_cut_raw
 * Description	  : isp tool onlining cuts raw and saves to sd card
 * Input		      :
 * Output		  : None
 * Return		  : None
 *******************************************************************************/

void usb_cut_raw(u8 *pbdata, u32 trans_len, u32 param_3B)
{

#if CMOS_USB_ONLINE_DBG
	debg(__func__);
//	int startPos = param_3B >> 8;
#if (_DEBG_CUT_SDR_DATA_ == _DEBG_CUT_CSI_RAW_)

	void sdram_data_capture_task(u8 * fname);
	sdram_data_capture_task(pbdata);

	debg("usb cut raw finish\n");
#endif
#endif
}

/*******************************************************************************
 * Function Name  : Sensor_common_Get
 * Description	  : sensor on line get sensor ID and so on
 * Input		      :
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void Sensor_common_Get(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG
	debg(__func__);
	static u32 i = 0;
	hal_Sensor_common_Get(pbdata, trans_len, param_3B);
	if (!i)
	{
		extern int dispLayerInit(INT8U layer);
		dispLayerInit(0);
		i++;
	}
#endif
}

/*******************************************************************************
 * Function Name  : Sensor_common_Set
 * Description	  : sensor on line get sensor ID
 * Input		      :
 * Output		  : None
 * Return		  : None
 *******************************************************************************/
void Sensor_common_Set(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG
	debg(__func__);
	hal_Sensor_common_Set(pbdata, trans_len, param_3B);
	/*
	int startPos = param_3B;
	debg("Sensor_set\n");
	debg("\n");
	debg("startPos:%d len:%d 3B:%d\n",startPos,trans_len,param_3B);

	switch(startPos){
		case offsetof(COMMON,exp_gain):		CopyToMember(&common_adapt,offsetof(COMMON,exp_gain),
													sizeof(common_adapt.exp_gain),pbdata,startPos,trans_len);
													break;
		case offsetof(COMMON,gain_max):		CopyToMember(&common_adapt,offsetof(COMMON,gain_max),
													sizeof(common_adapt.gain_max),pbdata,startPos,trans_len);
											_Sensor_Adpt_ *csiSensor = hal_csiAdptGet();
											csiSensor->ae_adapt.exp_adapt.gain_max = common_adapt.gain_max;
													break;
		case offsetof(COMMON,mclk):			CopyToMember(&common_adapt,offsetof(COMMON,mclk),
													sizeof(common_adapt.mclk),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,mclk_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,pclk):
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,pclk),
													sizeof(common_adapt.hvb.pclk),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,pclk_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,v_len):
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,v_len),
													sizeof(common_adapt.hvb.v_len),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,v_len_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,down_fps_mode):
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,down_fps_mode),
													sizeof(common_adapt.hvb.down_fps_mode),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,down_fps_mode_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,fps):
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,fps),
													sizeof(common_adapt.hvb.fps),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,fps_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,hvb) + offsetof(Hvb_Adapt,frequency):
											CopyToMember(&common_adapt,offsetof(COMMON,hvb)+ offsetof(Hvb_Adapt,frequency),
													sizeof(common_adapt.hvb.frequency),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,frequency_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;

		case offsetof(COMMON,hsyn):			CopyToMember(&common_adapt,offsetof(COMMON,hsyn),
													sizeof(common_adapt.hsyn),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,hsyn_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,vsyn):			CopyToMember(&common_adapt,offsetof(COMMON,vsyn),
													sizeof(common_adapt.vsyn),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,vsyn_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,rduline):		CopyToMember(&common_adapt,offsetof(COMMON,rduline),
													sizeof(common_adapt.rduline),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,rduline_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,colrarray):	CopyToMember(&common_adapt,offsetof(COMMON,colrarray),
													sizeof(common_adapt.colrarray),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,colrarray_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,pclk_fir_en):	CopyToMember(&common_adapt,offsetof(COMMON,pclk_fir_en),
													sizeof(common_adapt.pclk_fir_en),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,pclk_fir_en_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,pclk_inv_en):	CopyToMember(&common_adapt,offsetof(COMMON,pclk_inv_en),
													sizeof(common_adapt.pclk_inv_en),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,pclk_inv_en_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,csi_tun):		CopyToMember(&common_adapt,offsetof(COMMON,csi_tun),
													sizeof(common_adapt.csi_tun),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,csi_tun_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,exp_gain_en):	CopyToMember(&common_adapt,offsetof(COMMON,exp_gain_en),
													sizeof(common_adapt.exp_gain_en),pbdata,startPos,trans_len);
													break;
		case offsetof(COMMON,blk_en):		CopyToMember(&common_adapt,offsetof(COMMON,blk_en),
													sizeof(common_adapt.blk_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_BLC_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_BLC_POS_)) == common_adapt.blk_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_BLC_POS_,common_adapt.blk_en);
													break;
		case offsetof(COMMON,lsc_en):		CopyToMember(&common_adapt,offsetof(COMMON,lsc_en),
													sizeof(common_adapt.lsc_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_LSC_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_LSC_POS_)) == common_adapt.lsc_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_LSC_POS_,common_adapt.lsc_en);
													break;
		case offsetof(COMMON,ddc_en):		CopyToMember(&common_adapt,offsetof(COMMON,ddc_en),
													sizeof(common_adapt.ddc_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_DDC_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_DDC_POS_)) == common_adapt.ddc_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_DDC_POS_,common_adapt.ddc_en);
													break;
		case offsetof(COMMON,awb_en):		CopyToMember(&common_adapt,offsetof(COMMON,awb_en),
													sizeof(common_adapt.awb_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_AWB_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_)) == common_adapt.awb_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_AWB_POS_,common_adapt.awb_en);
													break;
		case offsetof(COMMON,ccm_en):		CopyToMember(&common_adapt,offsetof(COMMON,ccm_en),
													sizeof(common_adapt.ccm_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_CCM_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_CCM_POS_)) == common_adapt.ccm_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_CCM_POS_,common_adapt.ccm_en);
													break;
		case offsetof(COMMON,dgain_en):		CopyToMember(&common_adapt,offsetof(COMMON,dgain_en),
													sizeof(common_adapt.dgain_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_DGAIN_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_DGAIN_POS_)) == common_adapt.dgain_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_DGAIN_POS_,common_adapt.dgain_en);
													break;
		case offsetof(COMMON,ygama_en):		CopyToMember(&common_adapt,offsetof(COMMON,ygama_en),
													sizeof(common_adapt.ygama_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_YGAMA_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_YGAMA_POS_)) == common_adapt.ygama_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_YGAMA_POS_,common_adapt.ygama_en);
													break;
		case offsetof(COMMON,rgb_gama_en):	CopyToMember(&common_adapt,offsetof(COMMON,rgb_gama_en),
													sizeof(common_adapt.rgb_gama_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_RGB_GAMA_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_RGB_GAMA_POS_)) == common_adapt.rgb_gama_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_RGB_GAMA_POS_,common_adapt.rgb_gama_en);
													break;
		case offsetof(COMMON,ch_en):		CopyToMember(&common_adapt,offsetof(COMMON,ch_en),
													sizeof(common_adapt.ch_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_CH_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_CH_POS_)) == common_adapt.ch_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_CH_POS_,common_adapt.ch_en);
													break;
		case offsetof(COMMON,vde_en):		CopyToMember(&common_adapt,offsetof(COMMON,vde_en),
													sizeof(common_adapt.vde_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_VDE_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_VDE_POS_)) == common_adapt.vde_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_VDE_POS_,common_adapt.vde_en);
													break;
		case offsetof(COMMON,ee_en):		CopyToMember(&common_adapt,offsetof(COMMON,ee_en),
													sizeof(common_adapt.ee_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_EE_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_EE_POS_)) == common_adapt.ee_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_EE_POS_,common_adapt.ee_en);
													break;
		case offsetof(COMMON,cfd_en):		CopyToMember(&common_adapt,offsetof(COMMON,cfd_en),
													sizeof(common_adapt.cfd_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_CFD_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_CFD_POS_)) == common_adapt.cfd_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_CFD_POS_,common_adapt.cfd_en);
													break;
		case offsetof(COMMON,saj_en):		CopyToMember(&common_adapt,offsetof(COMMON,saj_en),
													sizeof(common_adapt.saj_en),pbdata,startPos,trans_len);
											_SET_ISP_MODE_(isp_module_state,_SAJ_POS_,!((_GET_ISP_MODE_(*isp.p_all_mod,_SAJ_POS_)) == common_adapt.saj_en));
											_SET_ISP_MODE_(*isp.p_all_mod,_SAJ_POS_,common_adapt.saj_en);
													break;
		case offsetof(COMMON,AVDD):			CopyToMember(&common_adapt,offsetof(COMMON,AVDD),
													sizeof(common_adapt.AVDD),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,AVDD_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,DVDD):			CopyToMember(&common_adapt,offsetof(COMMON,DVDD),
													sizeof(common_adapt.DVDD),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,DVDD_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,VDDIO):		CopyToMember(&common_adapt,offsetof(COMMON,VDDIO),
													sizeof(common_adapt.VDDIO),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,VDDIO_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,pclk_fir_class):	CopyToMember(&common_adapt,offsetof(COMMON,pclk_fir_class),
													sizeof(common_adapt.pclk_fir_class),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,pclk_fir_en_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;
		case offsetof(COMMON,rotate):		CopyToMember(&common_adapt,offsetof(COMMON,rotate),
													sizeof(common_adapt.rotate),pbdata,startPos,trans_len);
											_SET_BIT_(commom_mode,rotate_pos,1);
											_SET_BIT_(commom_mode,common_mode_max,1);
													break;

		default:							debg("do not have this module\n");
											break;

	}

	//double regs were used anytime during the vsyn signal is valid,so you shoud change these regs at the vsyn signal is invalid,
	//then these changed double regs will be valid at vsyn signal is valid next time.
	//tell soft_exp that someone(module) was modified
	_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);

	debg("isp_module_state:%x isp.p_all_mod:%x\n",isp_module_state,*isp.p_all_mod);
//	debg("blk_en:%d\n",common_adapt.blk_en);
//	debg("lsc_en:%d\n",common_adapt.lsc_en);
//	debg("ddc_en:%d\n",common_adapt.ddc_en);
//	debg("awb_en:%d\n",common_adapt.awb_en);
//	debg("ccm_en:%d\n",common_adapt.ccm_en);
//	debg("dgain_en:%d\n",common_adapt.dgain_en);
//	debg("ygama_en:%d\n",common_adapt.ygama_en);
//	debg("rgb_gama_en:%d\n",common_adapt.rgb_gama_en);
//	debg("ch_en:%d\n",common_adapt.ch_en);
//	debg("vde_en:%d\n",common_adapt.vde_en);
//	debg("ee_en:%d\n",common_adapt.ee_en);
//	debg("cfd_en:%d\n",common_adapt.cfd_en);
//	debg("saj_en:%d\n",common_adapt.saj_en);
//	debg("vddio:%d\n",common_adapt.VDDIO);
//	debg("exp_gain_en:%d\n",common_adapt.exp_gain_en);
//	debg("exp_gain:%d\n",common_adapt.exp_gain);
	*/
#endif
}

/*******************************************************************************
 * Function Name  : isp_debug_write
 * Description    : isp online debug.
 * Input          :
 * Output         : none
 * Return         : none
 *******************************************************************************/
void isp_debug_write(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG
	debg(__func__);
	hal_isp_debug_write(pbdata, trans_len, param_3B);
	/*	int isp_mode = param_3B & 0xff;
		int startPos = param_3B >> 8;
		int i;
		debg("\n");
		debg("isp_mode:%x len:%d 3B:%x\n",isp_mode,trans_len,param_3B);
		switch(isp_mode){
			case _AE_POS_		:	debg("write ae\n");
									AE *ae_p = (AE *)pbdata;
									debg("%d\n",ae_p->exp_adapt.gain_max);
									devSensor_Cmd.ae_adapt.exp_adapt.gain_max = ae_p->exp_adapt.gain_max;
									memcpy(&devSensor_Cmd.ae_adapt.exp_adapt.exp_tag,&(ae_p->exp_adapt.exp_tag),sizeof(ae_p->exp_adapt.exp_tag));
									break;
			case _BLC_POS_  	: 	debg("wirte blc\n");
									BLC *blc_p = (BLC *)pbdata;
									devSensor_Cmd.blc_adapt.blkl_r = blc_p->blkl_r;
									devSensor_Cmd.blc_adapt.blkl_gr = blc_p->blkl_gr;
									devSensor_Cmd.blc_adapt.blkl_gb = blc_p->blkl_gb;
									devSensor_Cmd.blc_adapt.blkl_b = blc_p->blkl_b;
									//debg("%d %d %d %d\n",devSensor_Cmd.blc_adapt.blkl_r,devSensor_Cmd.blc_adapt.blkl_gr,devSensor_Cmd.blc_adapt.blkl_gb,devSensor_Cmd.blc_adapt.blkl_b);
									if(_GET_ISP_MODE_(*isp.p_all_mod,_BLC_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _LSC_POS_       : 	debg("wirte lsc:\n");
									CopyToMember(lsc_tab, 0, sizeof(lsc_tab), pbdata, startPos, trans_len);

									for(i = startPos/2;i<(((startPos+trans_len)/2)<572?((startPos+trans_len)/2):572);i++){
										debg("  ");
										uart_Put_dec(lsc_tab[i]);
									}
									if(_GET_ISP_MODE_(*isp.p_all_mod,_LSC_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _DDC_POS_       : 	debg("write ddc\n");
									memcpy(&(devSensor_Cmd.ddc_adapt),pbdata,sizeof(devSensor_Cmd.ddc_adapt));
									devSensor_Cmd.ddc_adapt.indx_table[0] = devSensor_Cmd.ddc_adapt.ddc_class;
									devSensor_Cmd.ddc_adapt.indx_table[1] = clip((devSensor_Cmd.ddc_adapt.ddc_class-2),0,5);
									debg("%d\n",devSensor_Cmd.ddc_adapt.hot_num);
									if(_GET_ISP_MODE_(*isp.p_all_mod,_DDC_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;
			case _AWB_POS_       : 	debg("wirte awb:\n");
									awb_update(pbdata,trans_len,param_3B);
									debg("seg_mode:%d\n",devSensor_Cmd.awb_adapt.seg_mode);
									//debg("ymin:%d\n",devSensor_Cmd.awb_adapt.ymin);
									//debg("ymax:%d\n",devSensor_Cmd.awb_adapt.ymax);
									//debg("cb_th:%d\n",devSensor_Cmd.awb_adapt.cb_th[0]);

									for(i =0;i<128;i++){
										debg("  %d",devSensor_Cmd.awb_adapt.awb_tab[i]);
									}
									debg("\n");
									_SET_ISP_MODE_(isp_module_state,_AWB_POS_,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									break;

			case _CCM_POS_       : 	debg("write ccm\n");
									CCM *ccm_p = (CCM *)pbdata;
									memcpy(&(devSensor_Cmd.ccm_adapt.ccm),ccm_p->ccm,sizeof(ccm_p->ccm));
									debg("%d %d %d \n",devSensor_Cmd.ccm_adapt.ccm[0],devSensor_Cmd.ccm_adapt.ccm[1],devSensor_Cmd.ccm_adapt.ccm[2]);
									debg("%d %d %d \n",devSensor_Cmd.ccm_adapt.ccm[3],devSensor_Cmd.ccm_adapt.ccm[4],devSensor_Cmd.ccm_adapt.ccm[5]);
									debg("%d %d %d \n",devSensor_Cmd.ccm_adapt.ccm[6],devSensor_Cmd.ccm_adapt.ccm[7],devSensor_Cmd.ccm_adapt.ccm[8]);
									_SET_ISP_MODE_(isp_module_state,_CCM_POS_,1);
									_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									break;

			case _DGAIN_POS_ 	 :	debg("write dgain\n");
									//	memcpy(&devSensor_Cmd.rgbdgain_adapt,pbdata,sizeof(devSensor_Cmd.rgbdgain_adapt));
									if(_GET_ISP_MODE_(*isp.p_all_mod,_DGAIN_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _YGAMA_POS_     : 	debg("wirte ygama\n");
									//ygama_update(pbdata,trans_len,param_3B);
									if(_GET_ISP_MODE_(*isp.p_all_mod,_YGAMA_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _RGB_GAMA_POS_  : 	debg("write rgb_gamma\n");
									//	memcpy(&(devSensor_Cmd.rgbgama_adapt),pbdata,sizeof(devSensor_Cmd.rgbgama_adapt));
									if(_GET_ISP_MODE_(*isp.p_all_mod,_RGB_GAMA_POS_) == _ISP_EN_){
										_SET_ISP_MODE_(isp_module_state,isp_mode,1);
										_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
									}
									break;

			case _CH_POS_		 :	debg("write ch\n");
									CH *ch_p = (CH *)pbdata;
									memcpy(&devSensor_Cmd.ch_adapt.enhence,&ch_p->enhence,sizeof(ch_p->enhence));
									memcpy(&devSensor_Cmd.ch_adapt.r_rate,&ch_p->r_rate,sizeof(ch_p->r_rate));
									memcpy(&devSensor_Cmd.ch_adapt.g_rate,&ch_p->g_rate,sizeof(ch_p->g_rate));
									memcpy(&devSensor_Cmd.ch_adapt.b_rate,&ch_p->b_rate,sizeof(ch_p->b_rate));*/
	/*
	debg("en:%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.enhence[0],devSensor_Cmd.ch_adapt.enhence[1],devSensor_Cmd.ch_adapt.enhence[2]
	,devSensor_Cmd.ch_adapt.enhence[3],devSensor_Cmd.ch_adapt.enhence[4],devSensor_Cmd.ch_adapt.enhence[5]);
	debg("ra:%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.r_rate[0],devSensor_Cmd.ch_adapt.r_rate[1],devSensor_Cmd.ch_adapt.r_rate[2]
	,devSensor_Cmd.ch_adapt.r_rate[3],devSensor_Cmd.ch_adapt.r_rate[4],devSensor_Cmd.ch_adapt.r_rate[5]);
	debg("ga:%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.g_rate[0],devSensor_Cmd.ch_adapt.g_rate[1],devSensor_Cmd.ch_adapt.g_rate[2]
	,devSensor_Cmd.ch_adapt.g_rate[3],devSensor_Cmd.ch_adapt.g_rate[4],devSensor_Cmd.ch_adapt.g_rate[5]);
	debg("ba:%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.b_rate[0],devSensor_Cmd.ch_adapt.b_rate[1],devSensor_Cmd.ch_adapt.b_rate[2]
	,devSensor_Cmd.ch_adapt.b_rate[3],devSensor_Cmd.ch_adapt.b_rate[4],devSensor_Cmd.ch_adapt.b_rate[5]);
	debg("%d %d %d %d %d %d\n",devSensor_Cmd.ch_adapt.rate[0],devSensor_Cmd.ch_adapt.rate[1],devSensor_Cmd.ch_adapt.rate[2]
	,devSensor_Cmd.ch_adapt.rate[3],devSensor_Cmd.ch_adapt.rate[4],devSensor_Cmd.ch_adapt.rate[5],devSensor_Cmd.ch_adapt.rate[6]
	,devSensor_Cmd.ch_adapt.rate[7]);*/

	/*			if(_GET_ISP_MODE_(*isp.p_all_mod,_CH_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;

case _EE_POS_        : 	debg("write ee\n");
				EE *ee_p = (EE *)pbdata;
				devSensor_Cmd.ee_adapt.ee_class = ee_p->ee_class;
				debg("ee_class:%d\n",devSensor_Cmd.ee_adapt.ee_class);
				if(_GET_ISP_MODE_(*isp.p_all_mod,_EE_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;
case _VDE_POS_      : 	debg("write vde\n");
				VDE *vde_p = (VDE *)pbdata;
				devSensor_Cmd.vde_adapt.contra = vde_p->contra;
				devSensor_Cmd.vde_adapt.bright_k = vde_p->bright_k;
				devSensor_Cmd.vde_adapt.bright_oft = vde_p->bright_oft;
				devSensor_Cmd.vde_adapt.hue = vde_p->hue;
				memcpy(&(devSensor_Cmd.vde_adapt.sat_rate),vde_p->sat_rate,sizeof(vde_p->sat_rate));
				if(_GET_ISP_MODE_(*isp.p_all_mod,_VDE_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;

case _CFD_POS_       :// memcpy(&(devSensor_Cmd.cfd_adapt),pbdata,sizeof(devSensor_Cmd.cfd_adapt));

				if(_GET_ISP_MODE_(*isp.p_all_mod,_CFD_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;

case _SAJ_POS_		:	debg("write saj\n");
				SAJ *saj_p = (SAJ *)pbdata;
				memcpy(&(devSensor_Cmd.saj_adapt.sat_rate),saj_p->sat_rate,sizeof(saj_p->sat_rate));
				debg("%d %d %d %d %d %d %d %d\n",devSensor_Cmd.saj_adapt.sat_rate[0],devSensor_Cmd.saj_adapt.sat_rate[1],devSensor_Cmd.saj_adapt.sat_rate[2]
				,devSensor_Cmd.saj_adapt.sat_rate[3],devSensor_Cmd.saj_adapt.sat_rate[4],devSensor_Cmd.saj_adapt.sat_rate[5],devSensor_Cmd.saj_adapt.sat_rate[6]
				,devSensor_Cmd.saj_adapt.sat_rate[7]);
				if(_GET_ISP_MODE_(*isp.p_all_mod,_SAJ_POS_) == _ISP_EN_){
					_SET_ISP_MODE_(isp_module_state,isp_mode,1);
					_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
				}
				break;


default :break;
}
//	tell the main that who was changed
//	debg("sta:%x\n",isp_module_state);
//	tell the main modules was changed
//	_SET_ISP_MODE_(*isp.p_all_mod,_OL_DB_POS,_ISP_EN_);
debg("isp write out\n");*/
#endif
}

/*******************************************************************************
 * Function Name  : isp_debug_read
 * Description    : isp online read
 * Input          :
 * Output         : none
 * Return         : none
 *******************************************************************************/
void isp_debug_read(u8 *pbdata, u32 trans_len, u32 param_3B)
{
#if CMOS_USB_ONLINE_DBG
	debg(__func__);
	hal_isp_debug_read(pbdata, trans_len, param_3B);
#endif
}

/*******************************************************************************
 * Function Name  : cmos_spec_color
 * Description    : set spec color
 * Input          : index 0: normal color 1:red color,2 green color ,3 blue color , 4 B&W color , 5 old time color
 * Output         : none
 * Return         : none
 *******************************************************************************/
void cmos_spec_color(u8 index)
{
	deg_Printf("set spec color%d\n", index);

	if (0x48 == devSensorInit->id) // 1280 == devSensorOp->pixelw)	// 720P  63p
	{
#if 0
		if(1 == index)
		{
			/*isp.p_ccm->ccm[0] = 0x100;
			isp.p_ccm->ccm[1] = 0x0;
			isp.p_ccm->ccm[2] = 0x0;
			isp.p_ccm->ccm[3] = 0x80;
			isp.p_ccm->ccm[4] = 0x80;
			isp.p_ccm->ccm[5] = 0x0;
			isp.p_ccm->ccm[6] = 0x80;
			isp.p_ccm->ccm[7] = 0x0;
			isp.p_ccm->ccm[8] = 0x80;*/
			isp.p_ccm->ccm[0] = 0x128;
			isp.p_ccm->ccm[1] = 0x0;
			isp.p_ccm->ccm[2] = 0x0;
			isp.p_ccm->ccm[3] = 0x55;
			isp.p_ccm->ccm[4] = 0x110;
			isp.p_ccm->ccm[5] = 0x0;
			isp.p_ccm->ccm[6] = 0x55;
			isp.p_ccm->ccm[7] = 0x0;
			isp.p_ccm->ccm[8] = 0x110;
		}
		else if(2 == index)
		{
			isp.p_ccm->ccm[0] = 0x110;
			isp.p_ccm->ccm[1] = 0x55;
			isp.p_ccm->ccm[2] = 0x0;
			isp.p_ccm->ccm[3] = 0x0;
			isp.p_ccm->ccm[4] = 0x128;
			isp.p_ccm->ccm[5] = 0x0;
			isp.p_ccm->ccm[6] = 0x0;
			isp.p_ccm->ccm[7] = 0x55;
			isp.p_ccm->ccm[8] = 0x110;
		}
		else if(3 == index)
		{
			isp.p_ccm->ccm[0] = 0x110;
			isp.p_ccm->ccm[1] = 0x0;
			isp.p_ccm->ccm[2] = 0x55;
			isp.p_ccm->ccm[3] = 0x0;
			isp.p_ccm->ccm[4] = 0x110;
			isp.p_ccm->ccm[5] = 0x55;
			isp.p_ccm->ccm[6] = 0x0;
			isp.p_ccm->ccm[7] = 0x0;
			isp.p_ccm->ccm[8] = 0x128;
		}
		else if(4 == index) // B&W
		{
			isp.p_ccm->ccm[0] = 0x55;
			isp.p_ccm->ccm[1] = 0x55;
			isp.p_ccm->ccm[2] = 0x55;
			isp.p_ccm->ccm[3] = 0x55;
			isp.p_ccm->ccm[4] = 0x55;
			isp.p_ccm->ccm[5] = 0x55;
			isp.p_ccm->ccm[6] = 0x55;
			isp.p_ccm->ccm[7] = 0x55;
			isp.p_ccm->ccm[8] = 0x55;
		}
		else if(5 == index) // old time
		{
			isp.p_ccm->ccm[0] = 0x80;
			isp.p_ccm->ccm[1] = 0x80;
			isp.p_ccm->ccm[2] = 0x0;
			isp.p_ccm->ccm[3] = 0x80;
			isp.p_ccm->ccm[4] = 0x80;
			isp.p_ccm->ccm[5] = 0x0;
			isp.p_ccm->ccm[6] = 0x80;
			isp.p_ccm->ccm[7] = 0x0;
			isp.p_ccm->ccm[8] = 0x80;
		}
		else	// normal
		{
			isp.p_ccm->ccm[0] = 0x100;
			isp.p_ccm->ccm[1] = 0x0;
			isp.p_ccm->ccm[2] = 0x0;
			isp.p_ccm->ccm[3] = 0x0;
			isp.p_ccm->ccm[4] = 0x100;
			isp.p_ccm->ccm[5] = 0x0;
			isp.p_ccm->ccm[6] = 0x0;
			isp.p_ccm->ccm[7] = 0x0;
			isp.p_ccm->ccm[8] = 0x100;
		}
		isp_CCM_cfg( isp.p_ccm->ccm,isp.p_ccm->s41,isp.p_ccm->s42,isp.p_ccm->s43);
#endif
	}
	else // VGA
	{

		if ((0x9b == devSensorInit->id) || (0xa0 == devSensorInit->id)) // 0308 , 0309
		{
			u8 temp[4];
			if (devSensorInit == devSensor1Init) // sensor 1, IIC0
			{
				if (1 == index)
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x00; //
					sensor_iic_write(temp);
					temp[0] = 0xBB; // CR
					temp[1] = 0x60; //
					sensor_iic_write(temp);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					sensor_iic_write(temp);
				}
				else if (2 == index)
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x80; //
					sensor_iic_write(temp);
					temp[0] = 0xBB; // CR
					temp[1] = 0x80; //
					sensor_iic_write(temp);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					sensor_iic_write(temp);
				}
				else if (3 == index)
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x70; //
					sensor_iic_write(temp);
					temp[0] = 0xBB; // CR
					temp[1] = 0xc0; //
					sensor_iic_write(temp);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					sensor_iic_write(temp);
				}
				else if (4 == index) // B&W
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x00; //
					sensor_iic_write(temp);
					temp[0] = 0xBB; // CR
					temp[1] = 0x00; //
					sensor_iic_write(temp);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					sensor_iic_write(temp);
				}
				else if (5 == index) // old time
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x90; //
					sensor_iic_write(temp);
					temp[0] = 0xBB; // CR
					temp[1] = 0x30; //
					sensor_iic_write(temp);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					sensor_iic_write(temp);
				}
				else // normal
				{
					temp[0] = 0x23; // special effect
					temp[1] = 0x00; // disable
					sensor_iic_write(temp);
				}
			}
#if HAL_CFG_EN_IIC1
			else // sensor 2, IIC1
			{
				gsensor_iic_enable();
				if (1 == index)
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x00; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0xBB; // CR
					temp[1] = 0x60; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
				}
				else if (2 == index)
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x80; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0xBB; // CR
					temp[1] = 0x80; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
				}
				else if (3 == index)
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x70; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0xBB; // CR
					temp[1] = 0xc0; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
				}
				else if (4 == index) // B&W
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x00; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0xBB; // CR
					temp[1] = 0x00; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
				}
				else if (5 == index) // old time
				{
					temp[0] = 0xBA; // CB
					temp[1] = 0x90; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0xBB; // CR
					temp[1] = 0x30; //
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
					temp[0] = 0x23; // special effect
					temp[1] = 0x02; // enable
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
				}
				else // normal
				{
					temp[0] = 0x23; // special effect
					temp[1] = 0x00; // disable
					hal_iic18bitAddrWrite(devSensorInit->w_cmd, temp[0], &temp[1], devSensorInit->data_num);
				}
				gsensor_iic_disable();
			}
#endif
		}
		else if (0x3a == devSensorInit->id) // FB3A03
		{
			u8 temp[4];
			if (1 == index)
			{					// RED
				temp[0] = 0x70; //  special effect
				temp[1] = 0x0B; //
				sensor_iic_write(temp);
				temp[0] = 0x69; //
				temp[1] = 0x20; //
				sensor_iic_write(temp);
				temp[0] = 0x68; //
				temp[1] = 0xC0; //
				sensor_iic_write(temp);
				temp[0] = 0x67; //
				temp[1] = 0x80; //
				sensor_iic_write(temp);
				temp[0] = 0xB4; //
				temp[1] = 0x91; //
				sensor_iic_write(temp);
				temp[0] = 0x56; //
				temp[1] = 0x40; //
				sensor_iic_write(temp);
			}
			else if (2 == index)
			{					// BLUE
				temp[0] = 0x70; //  special effect
				temp[1] = 0x0B; //
				sensor_iic_write(temp);
				temp[0] = 0x69; //
				temp[1] = 0x20; //
				sensor_iic_write(temp);
				temp[0] = 0x68; //
				temp[1] = 0x60; //
				sensor_iic_write(temp);
				temp[0] = 0x67; //
				temp[1] = 0xE0; //
				sensor_iic_write(temp);
				temp[0] = 0xB4; //
				temp[1] = 0x91; //
				sensor_iic_write(temp);
				temp[0] = 0x56; //
				temp[1] = 0x40; //
				sensor_iic_write(temp);
			}
			else if (3 == index)
			{					// GREEN
				temp[0] = 0x70; //  special effect
				temp[1] = 0x0B; //
				sensor_iic_write(temp);
				temp[0] = 0x69; //
				temp[1] = 0x20; //
				sensor_iic_write(temp);
				temp[0] = 0x68; //
				temp[1] = 0x70; //
				sensor_iic_write(temp);
				temp[0] = 0x67; //
				temp[1] = 0x60; //
				sensor_iic_write(temp);
				temp[0] = 0xB4; //
				temp[1] = 0x91; //
				sensor_iic_write(temp);
				temp[0] = 0x56; //
				temp[1] = 0x40; //
				sensor_iic_write(temp);
			}
			else if (4 == index) // B&W
			{
				temp[0] = 0x70; //  special effect
				temp[1] = 0x0B; //
				sensor_iic_write(temp);
				temp[0] = 0x69; //
				temp[1] = 0x20; //
				sensor_iic_write(temp);
				temp[0] = 0x68; //
				temp[1] = 0x80; //
				sensor_iic_write(temp);
				temp[0] = 0x67; //
				temp[1] = 0x80; //
				sensor_iic_write(temp);
				temp[0] = 0xB4; //
				temp[1] = 0x91; //
				sensor_iic_write(temp);
				temp[0] = 0x56; //
				temp[1] = 0x40; //
				sensor_iic_write(temp);
			}
			else if (5 == index) // old time
			{
				temp[0] = 0x70; //  special effect
				temp[1] = 0x0B; //
				sensor_iic_write(temp);
				temp[0] = 0x69; //
				temp[1] = 0x20; //
				sensor_iic_write(temp);
				temp[0] = 0x68; //
				temp[1] = 0xA0; //
				sensor_iic_write(temp);
				temp[0] = 0x67; //
				temp[1] = 0x60; //
				sensor_iic_write(temp);
				temp[0] = 0xB4; //
				temp[1] = 0x91; //
				sensor_iic_write(temp);
				temp[0] = 0x56; //
				temp[1] = 0x40; //
				sensor_iic_write(temp);
			}
			else // normal
			{
				temp[0] = 0x70; // CB
				temp[1] = 0x0f; //
				sensor_iic_write(temp);
				temp[0] = 0x69; //
				temp[1] = 0x00; //
				sensor_iic_write(temp);
				temp[0] = 0x68; //
				temp[1] = 0x80; //
				sensor_iic_write(temp);
				temp[0] = 0x67; //
				temp[1] = 0x80; //
				sensor_iic_write(temp);
				temp[0] = 0xB4; // CB
				temp[1] = 0xF1; //
				sensor_iic_write(temp);
				temp[0] = 0x56; //
				temp[1] = 0x40; //
				sensor_iic_write(temp);
			}
		}
		else if (0x46 == devSensorInit->id) // SC030
		{
			u8 temp[4];
			if (1 == index) //  red
			{
				temp[0] = 0xf0; // page select
				temp[1] = 0x00;
				sensor_iic_write(temp);
				temp[0] = 0xb0; // manual awb
				temp[1] = 0x40; //
				sensor_iic_write(temp);

				temp[0] = 0xc8; // blue
				temp[1] = 0x70;
				sensor_iic_write(temp);

				temp[0] = 0xc9; // red
				temp[1] = 0xd0;
				sensor_iic_write(temp);
			}
			else if (2 == index) // green
			{
				temp[0] = 0xf0; // page select
				temp[1] = 0x00;
				sensor_iic_write(temp);
				temp[0] = 0xb0; // manual awb
				temp[1] = 0x40; //
				sensor_iic_write(temp);

				temp[0] = 0xc8; // blue
				temp[1] = 0x20;
				sensor_iic_write(temp);

				sensor_iic_write(temp);
				temp[0] = 0xc9; // red
				temp[1] = 0x20;
				sensor_iic_write(temp);
			}
			else if (3 == index) // blue
			{
				temp[0] = 0xf0; // page select
				temp[1] = 0x00;
				sensor_iic_write(temp);
				temp[0] = 0xb0; // manual awb
				temp[1] = 0x40; //
				sensor_iic_write(temp);

				temp[0] = 0xc8; // blue
				temp[1] = 0xd0;
				sensor_iic_write(temp);

				temp[0] = 0xc9; // red
				temp[1] = 0x20;
				sensor_iic_write(temp);
			}
			else if (4 == index) // old time
			{
				temp[0] = 0xf0; // page select
				temp[1] = 0x00;
				sensor_iic_write(temp);
				temp[0] = 0xb0; // manual awb
				temp[1] = 0x40; //
				sensor_iic_write(temp);

				temp[0] = 0xc8; // blue
				temp[1] = 0x20;
				sensor_iic_write(temp);

				temp[0] = 0xc9; // red
				temp[1] = 0xd0;
				sensor_iic_write(temp);

				temp[0] = 0x9f;
				temp[1] = 0xa3;
				sensor_iic_write(temp);
			}
			else if (5 == index) //	B&W
			{
				temp[0] = 0xf0; // page select
				temp[1] = 0x00;
				sensor_iic_write(temp);
				temp[0] = 0xb0; // auto awb  enable
				temp[1] = 0x41; //
				sensor_iic_write(temp);
				temp[0] = 0xe6; //
				temp[1] = 0x00; //
				sensor_iic_write(temp);
				temp[0] = 0x9f;
				temp[1] = 0xb3;
				sensor_iic_write(temp);
			}
			else if (6 == index) //	sketch
			{
				temp[0] = 0xf0; // page select
				temp[1] = 0x00;
				sensor_iic_write(temp);
				temp[0] = 0xe6; //
				temp[1] = 0xc0; //
				sensor_iic_write(temp);
				temp[0] = 0xb0; // auto awb  enable
				temp[1] = 0x41; //
				sensor_iic_write(temp);

				temp[0] = 0x9f;
				temp[1] = 0xa3;
				sensor_iic_write(temp);
			}
			else // normal
			{
				temp[0] = 0xf0; // page select
				temp[1] = 0x00;
				sensor_iic_write(temp);
				temp[0] = 0xb0; // auto awb  enable
				temp[1] = 0x41; //
				sensor_iic_write(temp);

				temp[0] = 0xe6; //
				temp[1] = 0x00; //
				sensor_iic_write(temp);
				temp[0] = 0x9f;
				temp[1] = 0xa3;
				sensor_iic_write(temp);
			}
		}

		else if (0x9d == devSensorInit->id) // GC0328
		{
			u8 temp[4];
			if (1 == index) //  old
			{
				temp[0] = 0x43;
				temp[1] = 0x02;
				sensor_iic_write(temp);
				temp[0] = 0xDA;
				temp[1] = 0xD0;
				sensor_iic_write(temp);
				temp[0] = 0xDB;
				temp[1] = 0x28;
				sensor_iic_write(temp);
			}
			else if (2 == index) // green
			{
				temp[0] = 0x43;
				temp[1] = 0x02;
				sensor_iic_write(temp);
				temp[0] = 0xDA;
				temp[1] = 0xC0;
				sensor_iic_write(temp);
				temp[0] = 0xDB;
				temp[1] = 0xC0;
				sensor_iic_write(temp);
			}
			else if (3 == index) // blue
			{
				temp[0] = 0x43;
				temp[1] = 0x02;
				sensor_iic_write(temp);
				temp[0] = 0xDA;
				temp[1] = 0x50;
				sensor_iic_write(temp);
				temp[0] = 0xDB;
				temp[1] = 0xE0;
				sensor_iic_write(temp);
			}

			else if (4 == index) // red
			{
				temp[0] = 0x43; //
				temp[1] = 0x02; //
				sensor_iic_write(temp);
				temp[0] = 0xDA; //
				temp[1] = 0x20; //
				sensor_iic_write(temp);
				temp[0] = 0xDB; //
				temp[1] = 0x60; //
				sensor_iic_write(temp);
			}
			else if (5 == index) // B&W
			{
				temp[0] = 0x43; // CB
				temp[1] = 0x02; //
				sensor_iic_write(temp);
				temp[0] = 0xDA; // CR
				temp[1] = 0x00; //
				sensor_iic_write(temp);
				temp[0] = 0xDB; // special effect
				temp[1] = 0x00; // enable
				sensor_iic_write(temp);
			}

			else // normal
			{
				temp[0] = 0x43; // CB
				temp[1] = 0x00; //
				sensor_iic_write(temp);
				temp[0] = 0xDA; // CR
				temp[1] = 0x00; //
				sensor_iic_write(temp);
				temp[0] = 0xDB; // special effect
				temp[1] = 0x00; // enable
				sensor_iic_write(temp);
			}
		}
		else if (0xa6 == devSensorInit->id) // 20a6
		{
			u8 temp[4];
			if (1 == index) // red
			{
				temp[0] = 0x0b; // uv output
				temp[1] = 0x22; //
				sensor_iic_write(temp);
				temp[0] = 0x1d;
				temp[1] = 0xe0;
				sensor_iic_write(temp);
				temp[0] = 0x1c;
				temp[1] = 0x60;
				sensor_iic_write(temp);
			}
			else if (2 == index)
			{
				temp[0] = 0x0b; // uv output
				temp[1] = 0x22; //
				sensor_iic_write(temp);
				temp[0] = 0x1d;
				temp[1] = 0x60;
				sensor_iic_write(temp);
				temp[0] = 0x1c;
				temp[1] = 0x60;
				sensor_iic_write(temp);
			}
			else if (3 == index) // blue
			{
				temp[0] = 0x0b; // uv output
				temp[1] = 0x22; //
				sensor_iic_write(temp);
				temp[0] = 0x1d;
				temp[1] = 0x60;
				sensor_iic_write(temp);
				temp[0] = 0x1c;
				temp[1] = 0xa0;
				sensor_iic_write(temp);
			}
			else if (4 == index) // B&W
			{
				temp[0] = 0x0b; // uv output
				temp[1] = 0x22; //
				sensor_iic_write(temp);
				sensor_iic_write(temp);
				temp[0] = 0x1d;
				temp[1] = 0x80;
				sensor_iic_write(temp);
				temp[0] = 0x1c;
				temp[1] = 0x80;
				sensor_iic_write(temp);
			}
			else if (5 == index) // old time
			{
				temp[0] = 0x0b; // uv output
				temp[1] = 0x22; //
				sensor_iic_write(temp);
				temp[0] = 0x1d;
				temp[1] = 0x90;
				sensor_iic_write(temp);
				temp[0] = 0x1c;
				temp[1] = 0x50;
				sensor_iic_write(temp);
			}
			else // normal
			{
				temp[0] = 0x0b; // uv output
				temp[1] = 0x02; //
				sensor_iic_write(temp);
			}
		}
	}
}

/*******************************************************************************
 * Function Name  : cmos_sensor_pin
 * Description    : set sensor pin state
 * Input          : pin 0: input 1:output
 * Output         : none
 * Return         : none
 *******************************************************************************/
void cmos_sensor_pin(u8 pin)
{
	/*
	#if (2 == SENSOR_NUM)

		u8 temp[4];

		if(1280 == devSensorOp->pixelw)	// 720P
		{
			if(0x62 == devSensorInit->id)	// H62
			{
				deg_Printf("cmos_sensor_pin:h62 %d\n",pin);
				if(devSensor1Op == devSensorOp)	// iic0
				{
					if(0 == pin)
					{
						temp[0] = 0x1d;
						temp[1] = 0x00;
						sensor_iic_write(temp);
						temp[0] = 0x1e;
						temp[1] = 0x00;
						sensor_iic_write(temp);
					}
					else
					{
						temp[0] = 0x1d;
						temp[1] = 0xFF;
						sensor_iic_write(temp);
						temp[0] = 0x1e;
						temp[1] = 0x1F;
						sensor_iic_write(temp);
					}
				}
				else
				{
					gsensor_iic_enable();
					if(0 == pin)
					{
						temp[0] = 0x1d;
						temp[1] = 0x00;
						hal_iic18bitAddrWrite(devSensorInit->w_cmd,temp[0],&temp[1],devSensorInit->data_num);
						temp[0] = 0x1e;
						temp[1] = 0x00;
						hal_iic18bitAddrWrite(devSensorInit->w_cmd,temp[0],&temp[1],devSensorInit->data_num);
					}
					else
					{
						temp[0] = 0x1d;
						temp[1] = 0xFF;
						hal_iic18bitAddrWrite(devSensorInit->w_cmd,temp[0],&temp[1],devSensorInit->data_num);
						temp[0] = 0x1e;
						temp[1] = 0x1F;
						hal_iic18bitAddrWrite(devSensorInit->w_cmd,temp[0],&temp[1],devSensorInit->data_num);
					}
					gsensor_iic_disable();
				}
			}
		}
		else // vga
		{
			if(0xa0 == devSensorInit->id)	// 0309
			{
				deg_Printf("cmos_sensor_pin:0309 %d\n",pin);
				if(devSensor1Op == devSensorOp)	// iic0
				{
					if(0 == pin)
					{
						temp[0] = 0xFE;
						temp[1] = 0x00;
						sensor_iic_write(temp);
						temp[0] = 0x25;
						temp[1] = 0x00;
						sensor_iic_write(temp);
					}
					else
					{
						temp[0] = 0xFE;
						temp[1] = 0x00;
						sensor_iic_write(temp);
						temp[0] = 0x25;
						temp[1] = 0xFF;
						sensor_iic_write(temp);
					}
				}
				else
				{
					gsensor_iic_enable();
					if(0 == pin)
					{
						temp[0] = 0xFE;
						temp[1] = 0x00;
						hal_iic18bitAddrWrite(devSensorInit->w_cmd,temp[0],&temp[1],devSensorInit->data_num);
						temp[0] = 0x25;
						temp[1] = 0x00;
						hal_iic18bitAddrWrite(devSensorInit->w_cmd,temp[0],&temp[1],devSensorInit->data_num);
					}
					else
					{
						temp[0] = 0xFE;
						temp[1] = 0x00;
						hal_iic18bitAddrWrite(devSensorInit->w_cmd,temp[0],&temp[1],devSensorInit->data_num);
						temp[0] = 0x25;
						temp[1] = 0xFF;
						hal_iic18bitAddrWrite(devSensorInit->w_cmd,temp[0],&temp[1],devSensorInit->data_num);
					}
					gsensor_iic_disable();
				}
			}

		}
	#endif
	*/
}

void sensor_rotate(u8 r)
{
	unsigned char buf[2];
#if 0
	 if(senser_select == 1)
    {
		buf[0] = 0x4a;
        buf[1] = 0x04;
    }
    else if(senser_select == 0)
    {
		buf[0] = 0x4a;
        buf[1] = 0x0C;//0x04
    }
	sensor_iic_write(buf);
#else
	if (0x9b == devSensorInit->id) // 0308
	{
		buf[0] = 0x14; // 0',180"
		buf[1] = 0x10 | (r << 0);
		sensor_iic_write(buf);
	}
	else if (0x3a == devSensorInit->id) // BF3a03
	{
		buf[0] = 0x1e;
		if (r)
		{
			// 后置
			buf[1] = 0x40;
			sensor_iic_write(buf);
		}
		else
		{
			// 前置
			buf[1] = 0x60;
			sensor_iic_write(buf);
		}
		deg_Printf("%d %s\n", r, __func__);
	}
	else if (0x46 == devSensorInit->id) // BF3a03
	{
		buf[0] = 0xf0;
		buf[1] = 0x32;
		sensor_iic_write(buf);
		buf[0] = 0x21;
		if (r)
		{
			// 后置
			buf[1] = 0x66; //[2:1] 1:morror ;0:disable	[6:5] 1:flip ;0:disable
			sensor_iic_write(buf);

			buf[0] = 0xe0;
			buf[1] = 0x08;
			sensor_iic_write(buf);
			buf[0] = 0xf0;
			buf[1] = 0x35;
			sensor_iic_write(buf);
		}
		else
		{
			// 前置
			buf[1] = 0x60; //[2:1] 1:morror ;0:disable	[6:5] 1:flip ;0:disable
			sensor_iic_write(buf);

			buf[0] = 0xe0;
			buf[1] = 0x04;
			sensor_iic_write(buf);
			buf[0] = 0xf0;
			buf[1] = 0x18;
			sensor_iic_write(buf);
		}
		deg_Printf("%d %s\n", r, __func__);
	}
	else if (0xa6 == devSensorInit->id) // bf20a6
	{
		buf[0] = 0x4a;
		if (r)
		{
			// 后置
			buf[1] = 0x08; // 0x00
			sensor_iic_write(buf);
		}
		else
		{
			// 前置
			buf[1] = 0x0c; // 0x08
			sensor_iic_write(buf);
		}
	}
#endif
}

#if 1
void sensorInit_table(void)
{
	devSensorOp = NULL;
	sensorInit();
}

void sensor_change()
{
#if 1
	deg_Printf("sensorChange start\n");

	hal_csiEnable(0);
	dispLayerUninit(DISP_LAYER_VIDEO);

	if (senser_select)
	{
		senser_select = 0;
		/*hal_dacHPSet(1,HP_VDD_2_8V);
		hal_sysDelayMS(50);

		hal_dacHPSet(0,0);
		hal_sysDelayMS(50);

		hal_dacHPSet(1,HP_VDD_2_8V);
		hal_sysDelayMS(50);*/
		sensorInit_table();
		deg_Printf("------------00000--------\n");
	}
	else
	{
		senser_select = 1;

		/*hal_dacHPSet(0,0);
		hal_sysDelayMS(50);
		hal_dacHPSet(1,HP_VDD_2_8V);
		hal_sysDelayMS(50);
		hal_dacHPSet(0,0);
		hal_sysDelayMS(50);*/
		sensorInit_table();
		deg_Printf("-------------11111----------\n");
	}

#if 1
	hal_csiEnable(1);
	XOSTimeDly(300); // wait AE ok
	dispLayerInit(DISP_LAYER_VIDEO);
	deg_Printf("change end\n");
#else
	hal_csiMCLKSet(devSensorOp->mclk);
	hal_csiRegister(devSensorOp, devSensorInit);
	hal_mjpegSrcCSI();
	ax32xx_csi_only_FrameSet();

	SysCtrl.crop_level = 0;
	dispLayerSetFrontCrop(SysCtrl.crop_level);

	hal_lcdPreviewStart(devSensorOp->pixelw, devSensorOp->pixelh);
	// hal_custom_frame_reset_sensor_szie();
	XOSTimeDly(200);
	hal_csiEnable(1);
#endif
#endif
}
#else
//=====sensor A ,sensor B change select======
void sensor_change()
{
#if (2 == SENSOR_NUM)
	cmos_spec_color(0); // normal color

	if (devSensor2Init->id == devSensor1Init->id) // the same sensor type
	{
		deg_Printf("same sensor type id=%x\n", devSensor1Init->id);
		hal_csiEnable(0);
		cmos_sensor_pin(0);

		if (devSensor1Op == devSensorOp)
		{
			hal_gpioWrite(SEN1_PWDN_CH, SEN1_PWDN_PIN, GPIO_HIGH);
			XOSTimeDly(2);
			hal_gpioWrite(SEN2_PWDN_CH, SEN2_PWDN_PIN, GPIO_LOW);

			devSensorOp = devSensor2Op;
			devSensorInit = devSensor2Init;
			senser_select = 1;
			hal_csiRegister(devSensorOp, devSensorInit);
			sensor_iic_info(devSensorInit->w_cmd, devSensorInit->r_cmd, devSensorInit->addr_num, devSensorInit->data_num);
			sensor_frame_rate_adapt(0, devSensorOp->hvb_adapt.fps);
			if (0x54 == devSensor1Init->id)
			{
				hal_senser_rotate(1);
				ax32xx_csiPrioritySet(1);
			}
		}
		else
		{
			hal_gpioWrite(SEN2_PWDN_CH, SEN2_PWDN_PIN, GPIO_HIGH);
			XOSTimeDly(2);
			hal_gpioWrite(SEN1_PWDN_CH, SEN1_PWDN_PIN, GPIO_LOW);
			devSensorOp = devSensor1Op;
			devSensorInit = devSensor1Init;
			senser_select = 0;
			hal_csiRegister(devSensorOp, devSensorInit);
			sensor_iic_info(devSensorInit->w_cmd, devSensorInit->r_cmd, devSensorInit->addr_num, devSensorInit->data_num);
			sensor_frame_rate_adapt(0, devSensorOp->hvb_adapt.fps);
			if (0x54 == devSensor1Init->id)
			{
				hal_senser_rotate(3);
				ax32xx_csiPrioritySet(2);
			}
		}

		cmos_sensor_pin(1);
		XOSTimeDly(200);
		hal_csiEnable(1);

		if (videoRecordFrameEnGet())
		{
			// disable frame show
			videoRecordCmdSet(CMD_COM_FRAME, 0);
			hal_custom_frame_add_enable(0);
		}
	}
	else
	{
		int ret;
		deg_Printf("dif sensor type\n");
		hal_csiEnable(0);

		cmos_sensor_pin(0);
		if (devSensor1Op == devSensorOp)
		{
			hal_gpioWrite(SEN1_PWDN_CH, SEN1_PWDN_PIN, GPIO_HIGH);
			XOSTimeDly(1);
			hal_gpioWrite(SEN2_PWDN_CH, SEN2_PWDN_PIN, GPIO_LOW);

			devSensorOp = devSensor2Op;
			devSensorInit = devSensor2Init;
			senser_select = 1;
			sensor_iic_info(devSensorInit->w_cmd, devSensorInit->r_cmd, devSensorInit->addr_num, devSensorInit->data_num);
			isp_switch_senser_set_gain_exp();
			deg_Printf("sensor2 work\n");
		}
		else
		{
			hal_gpioWrite(SEN2_PWDN_CH, SEN2_PWDN_PIN, GPIO_HIGH);
			XOSTimeDly(1);
			hal_gpioWrite(SEN1_PWDN_CH, SEN1_PWDN_PIN, GPIO_LOW);

			devSensorOp = devSensor1Op;
			devSensorInit = devSensor1Init;
			senser_select = 0;
			sensor_iic_info(devSensorInit->w_cmd, devSensorInit->r_cmd, devSensorInit->addr_num, devSensorInit->data_num);
			isp_switch_senser_set_gain_exp();
			deg_Printf("sensor1 work\n");
		}
		sensor_frame_rate_adapt(0, devSensorOp->hvb_adapt.fps);
		cmos_sensor_pin(1);
		hal_csiMCLKSet(devSensorOp->mclk);
		hal_csiRegister(devSensorOp, devSensorInit);
		hal_mjpegSrcCSI();
		isp_init();
		ax32xx_csi_only_FrameSet();

		SysCtrl.crop_level = 0;
		dispLayerSetFrontCrop(SysCtrl.crop_level, 1);

		hal_lcdPreviewStart(devSensorOp->pixelw, devSensorOp->pixelh);
		// hal_custom_frame_reset_sensor_szie();
		XOSTimeDly(200);
		hal_csiEnable(1);

		if (videoRecordFrameEnGet())
		{
			// disable frame show
			videoRecordCmdSet(CMD_COM_FRAME, 0);
			hal_custom_frame_add_enable(0);
		}
	}

#endif
}
#endif

#if 0
void sensor_stanby()
{
	hal_gpioWrite(SEN1_PWDN_CH,SEN1_PWDN_PIN,GPIO_HIGH);	//save power
#if (2 == SENSOR_NUM)
	hal_gpioWrite(SEN2_PWDN_CH,SEN2_PWDN_PIN,GPIO_HIGH);
#endif
}

void sensor_resume()
{
#if (2 == SENSOR_NUM)
	if(devSensor1Op == devSensorOp)
	{
		hal_gpioWrite(SEN1_PWDN_CH,SEN1_PWDN_PIN,GPIO_LOW);
	}
	else
	{
		hal_gpioWrite(SEN2_PWDN_CH,SEN2_PWDN_PIN,GPIO_LOW);
	}
#else
	hal_gpioWrite(SEN1_PWDN_CH,SEN1_PWDN_PIN,GPIO_LOW);
#endif
	deg_Printf("sensor resume\n");
}
#endif
