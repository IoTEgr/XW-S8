// #include "../../ax32xx/inc/ax32xx.h"
#include "printer.h"
#include "../../ax32_platform_demo/application.h"
#include "../../multimedia/interface/userInterface.h"
#include "../../multimedia/btcom/inc/btcom_inner.h"
#include "../../multimedia/btcom/inc/btcom.h"
#include "../../multimedia/btcom/inc/btcom_stream.h"
#include "../../multimedia/btcom/inc/btcom_user.h"

#define PRINT_TIMER TIMER3
#define PRINTER_W 384
#define PRINTER_GRAY_STEP 32   // 64  // 128 ?��?��?y ��?o��
#define LINE_TIME_FIX_EN 1	   // 0: close line point time fix  ,1: open line point time fix
#define HALF_LINE_PRINTER_EN 0 //  0: max one line hot , 1: max half line hot
#define GRAY_PARTS_OF_LINE 1
#define DOT_PARTS_OF_LINE 2
#define USE_PW_IO_CH 1 // control printer power CH ; 1:use IO , 0: use vddcore
#define PRINTER_GRAY_TIME 36
#define STEP_TIMES 10

// #define TIME_OUT 1000*10//20
// static s32 p_tm_hot_time=0;	//recode pre tm hot time
// printer power
#if USE_PW_IO_CH
#define PRINTER_POWER_CH GPIO_PB
#define PRINTER_POWER_PIN GPIO_PIN6
#endif

static u8 stream_printer_run(u16 w, u16 h, s16 level, u8 batteryValue);
static u32 get_battery_time(u8 batteryValue);
static u8 get_dot_fix_time(s32 *hot_time, u16 point_cnt, u16 *fix_time);
static u8 get_temp_and_fix_time(s32 *hot_time, u16 point_cnt, u16 *fix_time);

//==moto==
#define MOTO_AIN1_CH GPIO_PE
#define MOTO_AIN1_PIN GPIO_PIN0
#define MOTO_AIN2_CH GPIO_PF
#define MOTO_AIN2_PIN GPIO_PIN10
#define MOTO_BIN2_CH GPIO_PF
#define MOTO_BIN2_PIN GPIO_PIN11
#define MOTO_BIN1_CH GPIO_PA
#define MOTO_BIN1_PIN GPIO_PIN3

#define MOTO_AIN1_HIGH()                            \
	{                                               \
		GPIO_DATA(MOTO_AIN1_CH) |= (MOTO_AIN1_PIN); \
	}
#define MOTO_AIN1_LOW()                              \
	{                                                \
		GPIO_DATA(MOTO_AIN1_CH) &= ~(MOTO_AIN1_PIN); \
	}
#define MOTO_AIN2_HIGH()                            \
	{                                               \
		GPIO_DATA(MOTO_AIN2_CH) |= (MOTO_AIN2_PIN); \
	}
#define MOTO_AIN2_LOW()                              \
	{                                                \
		GPIO_DATA(MOTO_AIN2_CH) &= ~(MOTO_AIN2_PIN); \
	}
#define MOTO_BIN1_HIGH()                            \
	{                                               \
		GPIO_DATA(MOTO_BIN1_CH) |= (MOTO_BIN1_PIN); \
	}
#define MOTO_BIN1_LOW()                              \
	{                                                \
		GPIO_DATA(MOTO_BIN1_CH) &= ~(MOTO_BIN1_PIN); \
	}
#define MOTO_BIN2_HIGH()                            \
	{                                               \
		GPIO_DATA(MOTO_BIN2_CH) |= (MOTO_BIN2_PIN); \
	}
#define MOTO_BIN2_LOW()                              \
	{                                                \
		GPIO_DATA(MOTO_BIN2_CH) &= ~(MOTO_BIN2_PIN); \
	}

//==printer==  // 92 ��
#define PRINTER_STB_NUM 1		//  1 pin , 2pin
#define PRINTER_DATA_CH GPIO_PE // pf6
#define PRINTER_DATA_PIN GPIO_PIN15
#define PRINTER_CLK_CH GPIO_PE
#define PRINTER_CLK_PIN GPIO_PIN13
#define PRINTER_LATCH_CH GPIO_PB
#define PRINTER_LATCH_PIN GPIO_PIN5
#define PRINTER_TM_CH GPIO_PA
#define PRINTER_TM_PIN GPIO_PIN6
#define PRINTER_PSENSOR_CH GPIO_PA
#define PRINTER_PSENSOR_PIN GPIO_PIN7

#if (1 == PRINTER_STB_NUM)
#define PRINTER_STB_CH GPIO_PA
#define PRINTER_STB_PIN GPIO_PIN9
void PRINTER_STB_HIGH(void)
{
	GPIO_DATA(PRINTER_STB_CH) |= (PRINTER_STB_PIN);
}
void PRINTER_STB_LOW()
{
	GPIO_DATA(PRINTER_STB_CH) &= ~(PRINTER_STB_PIN);
}
#else
#define PRINTER_STB_CH GPIO_PF
#define PRINTER_STB_PIN GPIO_PIN9
#define PRINTER_STB2_CH GPIO_PD
#define PRINTER_STB2_PIN GPIO_PIN13
#define PRINTER_STB2_HIGH()                               \
	{                                                     \
		GPIO_DATA(PRINTER_STB2_CH) |= (PRINTER_STB2_PIN); \
	}
#define PRINTER_STB2_LOW()                                 \
	{                                                      \
		GPIO_DATA(PRINTER_STB2_CH) &= ~(PRINTER_STB2_PIN); \
	}

void PRINTER_STB_HIGH()
{
	GPIO_DATA(PRINTER_STB_CH) |= (PRINTER_STB_PIN);
	GPIO_DATA(PRINTER_STB2_CH) |= (PRINTER_STB2_PIN);
}
void PRINTER_STB_LOW()
{
	GPIO_DATA(PRINTER_STB_CH) &= ~(PRINTER_STB_PIN);
	GPIO_DATA(PRINTER_STB2_CH) &= ~(PRINTER_STB2_PIN);
}
#endif

void PRINTER_LATCH_HIGH()
{
	GPIO_DATA(PRINTER_LATCH_CH) |= (PRINTER_LATCH_PIN);
}
void PRINTER_LATCH_LOW()
{
	GPIO_DATA(PRINTER_LATCH_CH) &= ~(PRINTER_LATCH_PIN);
}
void PRINTER_CLK_HIGH()
{
	GPIO_DATA(PRINTER_CLK_CH) |= (PRINTER_CLK_PIN);
}
void PRINTER_CLK_LOW()
{
	GPIO_DATA(PRINTER_CLK_CH) &= ~(PRINTER_CLK_PIN);
}
void PRINTER_DAT_LOW()
{
	GPIO_DATA(PRINTER_DATA_CH) &= ~(PRINTER_DATA_PIN);
}
void PRINTER_DAT_HIGH()
{
	GPIO_DATA(PRINTER_DATA_CH) |= (PRINTER_DATA_PIN);
}
void PRINTER_DAT_LOW_MASF()
{
	// PORTE&=~(PRINTER_DATA_PIN_);
	// PORTE&=~(PRINTER_CLK_PIN_);
	PORTE &= ~(PRINTER_CLK_PIN | PRINTER_DATA_PIN);
	PORTE |= (PRINTER_CLK_PIN);
}
void PRINTER_DAT_HIGH_MASF()
{
	PORTE |= (PRINTER_DATA_PIN);
	PORTE &= ~(PRINTER_CLK_PIN);
	PORTE |= (PRINTER_CLK_PIN);
}

void printer_delay_masf(u32 n)
{
	volatile u32 i = (20 * 25 * n);
	while (i--)
	{
		asm("l.nop");
		// if(SysCtrl.stream_print)
		// btcomService();
		// deg_Printf("F");
	}
}

#define DATA_SIZE 81
u16 tem_adc_values[DATA_SIZE] = { // 10KΩ
	931, 926, 922, 917, 912,
	907, 901, 896, 890, 884,
	878, 872, 865, 859, 852,
	845, 838, 831, 823, 816,
	808, 800, 792, 784, 776,
	767, 759, 750, 741, 732,
	723, 714, 705, 695, 686,
	676, 667, 657, 647, 638,
	628, 618, 608, 599, 589,
	579, 569, 559, 550, 540,
	530, 520, 511, 501, 492,
	482, 473, 464, 455, 446,
	437, 428, 419, 410, 402,
	393, 385, 377, 369, 361,
	353, 345, 338, 330, 323,
	316, 309, 302, 295, 288,
	282};
//
// u16 tem_adc_values[DATA_SIZE] = {//30KΩ
// 	789, 779, 769, 759, 749,
// 	738, 728, 717, 706, 695,
// 	684, 673, 662, 650, 639,
// 	627, 616, 604, 592, 581,
// 	569, 557, 546, 534, 523,
// 	512, 500, 489, 478, 467,
// 	456, 445, 434, 424, 413,
// 	403, 393, 383, 373, 364,
// 	354, 345, 336, 327, 318,
// 	310, 301, 293, 285, 278,
// 	270, 263, 255, 248, 241,
// 	235, 228, 222, 215, 209,
// 	203, 198, 192, 187, 181,
// 	176, 171, 167, 162, 157,
// 	153, 149, 144, 140, 136,
// 	133, 129, 125, 122, 118,
// 	115
// };
s32 printer_tem_get_masf()
{
	u32 value_adc = 0;
	u8 i;
	for (i = 0; i < 20; i++)
	{
		value_adc += printer_tm_adc();
	}
	value_adc /= 20;
	for (i = 0; i < DATA_SIZE; i++)
	{
		if (tem_adc_values[i] <= value_adc)
		{
			return i;
		}
	}
	return 81;
}

#define OP_MOTO_AIN1(x)                                                                        \
	{                                                                                          \
		GPIO_DATA(MOTO_AIN1_CH) = ((GPIO_DATA(MOTO_AIN1_CH) & (~MOTO_AIN1_PIN)) | (x << (0))); \
	}
#define OP_MOTO_AIN2(x)                                                                         \
	{                                                                                           \
		GPIO_DATA(MOTO_AIN2_CH) = ((GPIO_DATA(MOTO_AIN2_CH) & (~MOTO_AIN2_PIN)) | (x << (10))); \
	}
#define OP_MOTO_BIN1(x)                                                                        \
	{                                                                                          \
		GPIO_DATA(MOTO_BIN1_CH) = ((GPIO_DATA(MOTO_BIN1_CH) & (~MOTO_BIN1_PIN)) | (x << (3))); \
	}
#define OP_MOTO_BIN2(x)                                                                         \
	{                                                                                           \
		GPIO_DATA(MOTO_BIN2_CH) = ((GPIO_DATA(MOTO_BIN2_CH) & (~MOTO_BIN2_PIN)) | (x << (11))); \
	}
int TIMER_REST()
{
	int Ret = ax32xx_printTimerStart(PRINT_TIMER, 0xFFFF, 0);
	return Ret;
}

u32 TIMERTICKCOUNT()
{
	u32 Ret = ax32xx_timer3TickCount();
	return Ret;
}
void TIMER_STOP()
{
	ax32xx_timerStop(PRINT_TIMER);
}
void MOTO_WORK(u8 x, u8 y, u8 z, u8 w)
{
	OP_MOTO_AIN1(x);
	OP_MOTO_AIN2(y);
	OP_MOTO_BIN1(z);
	OP_MOTO_BIN2(w);
}

int printer_tm_adc(void)
{
	int tm = hal_adcGetChannel(ADC_CH_PA6);
	return tm;
}

static void printer_set_int(u8 en);

static void printer_delay(u32 n)
{
	volatile u32 i = 20 * n;
	while (i--)
	{
		asm("l.nop");
		// if(SysCtrl.stream_print)
		// btcomService();
		// deg_Printf("F");
	}
}

#if 1
//==n: 25 is 1ms==
//==n: 50 is 2ms==
static void printer_moto_delay(u32 n)
{
	printer_delay(25 * n);
}
#else
static void printer_moto_delay(u32 n)
{
	volatile u32 i = 500 * n;
	while (i--)
	{
		asm("l.nop");
	}
}
#endif

/*openPrinter.c*/

void openPrinter_timer_print_rest()
{
	ax32xx_printTimerStart(PRINT_TIMER, 0xffff, NULL);
	return;
}

void openPrinter_timer_print_stop()
{
	ax32xx_timerStop(PRINT_TIMER);
	return;
}

u32 openPrinter_timer_print_tickcount_get()
{
	ax32xx_timer3TickCount();
	return;
}

int open_printer_tm_get()
{
#define R_LOW 10000 // 10K
	int tm = 0;
	tm = open_printer_tm(hal_adcGetChannel(ADC_CH_PA6), 1024, R_LOW);
	return tm;
}

static int tm_count = 0;
static int tmSum = 0;
void open_printer_tm_reset(void)
{
	tm_count = 0;
	tmSum = 0;
}
/*openPrinter.c*/

void printer_init()
{
	//==close printer power==
	//==PB6  PB5 handle clean USB setting==
	USB11CON0 = 0;
	USB11CON1 = 0;
	USB11CON0 |= (1 << 6);			 // control by soft
	USB11CON1 &= ~(BIT(4) | BIT(6)); // disable dp,dm 120K pullup
	USB11CON1 &= ~(BIT(7) | BIT(5)); // disable dp,dm 15k pulldown
	//==end PB6 handle clean USB setting==

	// hal_gpioInit(GPIO_PB,GPIO_PIN6,GPIO_OUTPUT,GPIO_PULL_FLOATING);
	// hal_gpioWrite(GPIO_PB,GPIO_PIN6,GPIO_LOW);
	// int cuu = 0;
	// while(1)
	// {
	//	ax32xx_wdtClear();
	//	int a = hal_gpioRead(GPIO_PB, GPIO_PIN6);
	//	cuu++;
	//	if(cuu > 2000)
	//	break;
	//	deg_Printf("---%d--\n", a);
	// }
	//==moto==
	// LDOCON &= ~(1<<12);//vddcore �ص�
	hal_gpioInit(PRINTER_POWER_CH, PRINTER_POWER_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_POWER_CH, PRINTER_POWER_PIN, GPIO_LOW);

	hal_gpioInit(MOTO_AIN1_CH, MOTO_AIN1_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN1_CH, MOTO_AIN1_PIN, GPIO_LOW);
	// ������PF10 �����������
	hal_gpioInit(MOTO_AIN2_CH, MOTO_AIN2_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN2_CH, MOTO_AIN2_PIN, GPIO_LOW);

	hal_gpioInit(MOTO_BIN1_CH, MOTO_BIN1_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN1_CH, MOTO_BIN1_PIN, GPIO_LOW);
	hal_gpioInit(MOTO_BIN2_CH, MOTO_BIN2_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN2_CH, MOTO_BIN2_PIN, GPIO_LOW);
	//==printer==
	hal_gpioInit(PRINTER_PSENSOR_CH, PRINTER_PSENSOR_PIN, GPIO_INPUT, GPIO_PULL_FLOATING);

	hal_gpioInit(PRINTER_DATA_CH, PRINTER_DATA_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_DATA_CH, PRINTER_DATA_PIN, GPIO_LOW);
	hal_gpioInit(PRINTER_CLK_CH, PRINTER_CLK_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_CLK_CH, PRINTER_CLK_PIN, GPIO_LOW);
	hal_gpioInit(PRINTER_LATCH_CH, PRINTER_LATCH_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_LATCH_CH, PRINTER_LATCH_PIN, GPIO_HIGH);

	hal_gpioInit(PRINTER_STB_CH, PRINTER_STB_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH, PRINTER_STB_PIN, GPIO_LOW);

#if (1 == PRINTER_STB_NUM)
	hal_gpioInit(PRINTER_STB_CH, PRINTER_STB_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH, PRINTER_STB_PIN, GPIO_LOW);
	deg_Printf("printer stb 1pin\n");
#else
	hal_gpioInit(PRINTER_STB_CH, PRINTER_STB_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB_CH, PRINTER_STB_PIN, GPIO_HIGH);
	hal_gpioInit(PRINTER_STB2_CH, PRINTER_STB2_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_STB2_CH, PRINTER_STB2_PIN, GPIO_HIGH);
	deg_Printf("printer stb 2pin\n");
#endif

	deg_Printf("printer_init\n");
}

//==return 0: have paper , 1: have not paper==
u32 printer_get_paper()
{
	// return 0;
	SysCtrl.paper_check = 0;
	u32 ps = hal_gpioRead(GPIO_PA, GPIO_PIN7);
	SysCtrl.paper_check = ps;
	// deg_Printf("ps=%d\n",ps);
	return ps;
}

//===tm val===
// 20C = 37K  pull down, 2.54V,val = 789
// 30C = 24K  pull down, 2.26V,val = 701
// 40C = 15K  pull down, 1.90V,val = 589
// 50C = 10K  pull down, 1.57V,val = 487
// 60C = 7.5K pull down, 1.34V,val = 415
// 70C = 5.2K pull down, 1.05V,val = 325
extern int printer_tm_flag;
extern int usb_prinr;
static u16 temprature = 0;
static s32 printer_get_tm() // ע��������Ϊ�����˲�Ҫ�������
{
	s32 tm = hal_adcGetChannel(ADC_CH_PA6);
	temprature = tm;

	if (printer_tm_flag)
	{
		tm = 20;
	}
	if (hal_wki1Read())
	{
		tm = 20;
	}

	if (tm < 250) // 85C overhot
	{
		// deg_Printf("[%d]<250 \n",tm);
		return 0;
	}
	else if (tm < 310) // very hot , 75C :ad:310
	{
		// deg_Printf("[%d]<310 \n",tm);
		return 1;
	}
	else if (tm < 391) // 65C
	{
		// deg_Printf("[%d]<391 \n",tm);
		return 2;
	}
	else if (tm < 485) // 55C
	{
		return 3;
	}
	else if (tm < 578) // 45C
	{
		return 4;
	}
	else if (tm < 682) // 35C
	{
		return 5;
	}
	else if (tm < 768) // 25C
	{
		return 6;
	}
	else if (tm < 877) // 10C
	{
		return 7;
	}
	else if (tm >= 928) // very cool ,0C
	{
		return 8;
	}

	return 0;
}
#if !USE_PW_IO_CH
// vddcore  --SLEEP
void SLEEP(u8 arg)
{
	if (arg)
	{
		LDOCON |= (1 << 12);
		ax32xx_sysLDOSet(SYS_LDO_LSEN, SYS_VOL_V2_8, 1);
	}
	else
	{
		LDOCON &= ~(1 << 12);
	}
}
#endif
//==en 0: disable , 1: enable===
static void printer_moto_enable(u8 en)
{
	//==init moto pin==

	hal_gpioWrite(MOTO_AIN1_CH, MOTO_AIN1_PIN, GPIO_LOW);
	hal_gpioWrite(MOTO_AIN2_CH, MOTO_AIN2_PIN, GPIO_LOW);
	hal_gpioWrite(MOTO_BIN1_CH, MOTO_BIN1_PIN, GPIO_LOW);
	hal_gpioWrite(MOTO_BIN2_CH, MOTO_BIN2_PIN, GPIO_LOW);

	if (en)
	{
#if USE_PW_IO_CH
		hal_gpioWrite(PRINTER_POWER_CH, PRINTER_POWER_PIN, GPIO_HIGH);
#else
		SLEEP(1);
#endif
		printer_moto_delay(20);
	}
	else
	{
#if USE_PW_IO_CH
		hal_gpioWrite(PRINTER_POWER_CH, PRINTER_POWER_PIN, GPIO_LOW);
#else
		SLEEP(0);
#endif
	}
}

/*
//==default==
//dir  0 : forward , 1: reserve===
#define MOTO_TIME  50//50
static s32 printer_moto_move_step(u8 dir)
{
	static u8 half_step = 0;

	if(0 == dir)
	{
		if(0 == half_step)
		{
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			half_step = 1;

		}
		else
		{
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();

			half_step = 0;
		}

	}
	else
	{
		if(0 == half_step)
		{
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_LOW();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			half_step = 1;

		}
		else
		{
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_LOW();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_LOW();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_LOW();
			printer_moto_delay(MOTO_TIME);
			MOTO_AIN1_HIGH();
			MOTO_AIN2_HIGH();
			MOTO_BIN1_HIGH();
			MOTO_BIN2_HIGH();
			half_step = 0;
		}
	}
}
*/

const s32 gstep_hot_time_arr[32] =
	{
		15, 15, 15, 15, 25, 25, 25, 25,
		35, 35, 35, 35, 45, 45, 45, 45,
		55, 55, 55, 55, 65, 65, 65, 65,
		75, 75, 75, 75, 85, 85, 85, 85};
// const s32 gstep_hot_time_arr[32]=
// 	{
// 	 	 5,  5,  5,  5, 13, 13, 13, 13,
// 		21, 21, 21, 21, 29, 29, 29, 29,
// 		37, 37, 37, 37, 45, 45, 45, 45,
// 		53, 53, 53, 53, 61, 61, 61, 61
// 	};
//==half step==

#define MOTO_TIME 50 // 9

// dir  0 : moto forward , 1: moto reserve===
// mode 0 : move 1 line ,1: use hot_time move
// hot_time : mode 1 use it
// gsetp : mode 1 use it
// line_fix_time : mode 1 use it
// ret : 0 : move finish , 1: not finish
// 1-2相   ATP2XSP1RTNEN3
static s32 printer_moto_move_step(u8 dir, u8 mode, u32 hot_time, u8 gstep, u32 line_fix_time, u32 moto_speed)
{
	static u8 half_step = 0;
	s32 ret = 0;
	if (0 == mode)
	{
		if (0 == dir)
		{
			if (0 == half_step)
			{
				MOTO_AIN1_HIGH();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_HIGH();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				half_step = 1;
			}
			else
			{
				MOTO_AIN1_HIGH();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_HIGH();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				half_step = 0;
			}
		}
		else
		{
			if (0 == half_step)
			{

				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_HIGH();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_LOW();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				half_step = 1;
			}
			else
			{
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_LOW();
				MOTO_BIN2_HIGH();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_HIGH();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_LOW();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				printer_moto_delay(moto_speed * 2);
				MOTO_AIN1_HIGH();
				MOTO_AIN2_HIGH();
				MOTO_BIN1_HIGH();
				MOTO_BIN2_LOW();
				half_step = 0;
			}
		}
		return ret;
	}
	else
	{
		static u8 move_finish = 0, mstep = 0;
		static s32 all_hot_time, moto_delay_time, move_time;
		if (0 == gstep) // start
		{
#if GRAY_PARTS_OF_LINE > 1
			all_hot_time = hot_time * PRINTER_GRAY_STEP * 2;
#else
			all_hot_time = hot_time * PRINTER_GRAY_STEP;
#endif
			moto_delay_time = all_hot_time / 4;

			if (moto_delay_time < MOTO_TIME * moto_speed /*23*/) // MOTO_TIME*25 is 1ms ,50 is 2ms
			{
				moto_delay_time = MOTO_TIME * moto_speed /*23*/;
			}

			// hot_time=((all_hot_time*gstep_hot_time_arr[gstep])/1024);
			move_finish = 0;
			mstep = 0;
			move_time = hot_time;
		}
		else
		{
			// hot_time=((all_hot_time*gstep_hot_time_arr[gstep])/1024);
			move_time = move_time + hot_time;
			if (move_time >= moto_delay_time)
			{
				mstep++;
				if (mstep > 3)
				{
					mstep = 4;
					move_finish = 1;
					if (half_step)
					{
						half_step = 0;
					}
					else
					{
						half_step = 1;
					}
				}
				move_time -= moto_delay_time;
			}
		}
		if (0 == dir)
		{
			if (0 == half_step)
			{
				if (0 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time + line_fix_time);
				}
				else if (1 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time + line_fix_time);
				}
				else if (2 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
				else if (3 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
			}
			else
			{
				if (0 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
				else if (1 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
				else if (2 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
				else if (3 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time + line_fix_time);
				}
			}
		}
		else
		{
			if (0 == half_step)
			{
				if (0 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time + line_fix_time);
				}
				else if (1 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
				else if (2 == mstep)
				{
					MOTO_AIN1_LOW();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
				else if (3 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
			}
			else
			{
				if (0 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_LOW();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
				else if (1 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_HIGH();
					printer_delay(hot_time + line_fix_time);
				}
				else if (2 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_LOW();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time + line_fix_time);
				}
				else if (3 == mstep)
				{
					MOTO_AIN1_HIGH();
					MOTO_AIN2_HIGH();
					MOTO_BIN1_HIGH();
					MOTO_BIN2_LOW();
					printer_delay(hot_time + line_fix_time);
				}
			}
		}

		if (move_finish)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
}

/*
//==one step==
//dir  0 : forward , 1: reserve===
#define MOTO_TIME  100//50
static s32 printer_moto_move_step(u8 dir)
{
	if(0 == dir)
	{
		MOTO_AIN1_HIGH();
		MOTO_AIN2_LOW();
		MOTO_BIN1_LOW();
		MOTO_BIN2_HIGH();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_LOW();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_LOW();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_LOW();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_LOW();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_LOW();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_LOW();
		MOTO_BIN2_HIGH();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_HIGH();
	}
	else
	{
		MOTO_AIN1_LOW();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_LOW();
		MOTO_BIN2_HIGH();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_LOW();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_LOW();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_LOW();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_LOW();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_LOW();
		MOTO_BIN1_LOW();
		MOTO_BIN2_HIGH();
		printer_moto_delay(MOTO_TIME);
		MOTO_AIN1_HIGH();
		MOTO_AIN2_HIGH();
		MOTO_BIN1_HIGH();
		MOTO_BIN2_HIGH();
	}
}
*/

static void printer_strobe(u32 n)
{
	// u16 counter = n;
	// u8 flag = 0;
#if (1 == PRINTER_STB_NUM)

	PRINTER_STB_HIGH();
	printer_delay(n);
	PRINTER_STB_LOW();

#else

	PRINTER_STB_LOW();
	PRINTER_STB2_LOW();
	printer_delay(n);
	PRINTER_STB_HIGH();
	PRINTER_STB2_HIGH();

#endif
}
u8 is_printting = 0;
//==mode : 0 hot n time , 1 hot and moto move==
//==gstep : mode 1 use it==
//==line_fix_time : mode 1 use it==
//==ret : 0 : is moto move line finish , 1: moto moving
static s32 printer_strobe_moto_move(u8 mode, u32 n, u8 gstep, u32 line_fix_time, u32 moto_speed)
{
	s32 ret = 0;
#if (1 == PRINTER_STB_NUM)
	PRINTER_STB_HIGH();
	if (0 == mode)
		printer_delay(n);
	else
	{
		ret = printer_moto_move_step(1, 1, n, gstep, line_fix_time, moto_speed);
	}

	PRINTER_STB_LOW();
#else
	PRINTER_STB_HIGH();
	PRINTER_STB2_HIGH();
	if (0 == mode)
		printer_delay(n);
	else
	{
		ret = printer_moto_move_step(1, 1, n, gstep, line_fix_time, moto_speed);
	}
	PRINTER_STB_LOW();
	PRINTER_STB2_LOW();
#endif
	return ret;
}

static u8 print_grayLocal_align_width(u8 *data, int idx, int times, u16 gray_tab[PRINTER_GRAY_STEP], int k, u32 hot_time, u16 line_fix[PRINTER_W + 1], u32 moto_speed, u8 ret)
{
	int i /*,k*/;
	s16 pnum;

	// for(k=0; k<PRINTER_GRAY_STEP; k++)
	{
		pnum = 0;
		for (i = 0; i < PRINTER_W; i++)
		{
			if (*(data + i) <= gray_tab[k])
			{
				if ((i % times) == idx)
				{
					PRINTER_DAT_HIGH();
					pnum++;
				}
				else
				{
					PRINTER_DAT_LOW();
				}
			}
			else
			{
				PRINTER_DAT_LOW();
			}
			PRINTER_CLK_HIGH();
			PRINTER_CLK_LOW();
		}
		PRINTER_LATCH_LOW();
		PRINTER_LATCH_HIGH();
		if (ret)
			ret = printer_strobe_moto_move(1, hot_time, k, line_fix[pnum], moto_speed);
		else
			ret = printer_strobe_moto_move(0, hot_time, k, line_fix[pnum], moto_speed);
	}

	return ret;
}
static u8 print_grayLocal_align_height(u8 *data, int idx, int times, u16 w, u16 h, u16 gray_tab[PRINTER_GRAY_STEP], int k, u32 hot_time, u16 line_fix[PRINTER_W + 1], u32 moto_speed, u8 ret)
{
	u32 len;
	s32 offset;
	int /*k,*/ i;
	s16 pnum;

	len = w * h;
	// for(k=0; k<PRINTER_GRAY_STEP; k++)
	{
		pnum = 0;
		offset = len;
		for (i = 0; i < PRINTER_W; i++)
		{
			offset -= w;
			if (*(data + offset) <= gray_tab[k])
			{
				if ((i % times) == idx)
				{
					PRINTER_DAT_HIGH();
					pnum++;
				}
				else
				{
					PRINTER_DAT_LOW();
				}
			}
			else
			{
				PRINTER_DAT_LOW();
			}
			PRINTER_CLK_HIGH();
			PRINTER_CLK_LOW();
		}
		PRINTER_LATCH_LOW();
		PRINTER_LATCH_HIGH();
		PRINTER_DAT_LOW();
		if (ret)
			ret = printer_strobe_moto_move(1, hot_time, k, line_fix[pnum], moto_speed);
		else
			ret = printer_strobe_moto_move(0, hot_time, k, line_fix[pnum], moto_speed);
	}

	return ret;
}
static u8 print_dot_align_width(u8 *data, int idx, int times, u32 hot_time, u16 line_fix[PRINTER_W + 1])
{
	int i;
	s16 pnum;

	for (i = 0; i < PRINTER_W; i++)
	{
		pnum = 0;
		if (*(data + i) <= 128)
		{
			if ((i % times) == idx)
			{
				PRINTER_DAT_HIGH();
				pnum++;
			}
			else
			{
				PRINTER_DAT_LOW();
			}
		}
		else
		{
			PRINTER_DAT_LOW();
		}
		PRINTER_CLK_HIGH();
		PRINTER_CLK_LOW();
	}
	PRINTER_LATCH_LOW();
	PRINTER_LATCH_HIGH();

	printer_strobe(hot_time + line_fix[pnum]);

	return 0;
}
static u8 print_dot_align_height(u8 *data, int idx, int times, u16 w, u16 h, u32 hot_time, u16 line_fix[PRINTER_W + 1])
{
	int i;
	u32 offset;
	s16 pnum = 0;

	offset = w * h;
	for (i = 0; i < h; i++)
	{
		offset -= w;
		if (*(data + offset) <= 128)
		{
			if ((i % times) == idx)
			{
				PRINTER_DAT_HIGH();
				pnum++;
			}
			else
			{
				PRINTER_DAT_LOW();
			}
		}
		else
		{
			PRINTER_DAT_LOW();
		}
		PRINTER_CLK_HIGH();
		PRINTER_CLK_LOW();
	}
	PRINTER_LATCH_LOW();
	PRINTER_LATCH_HIGH();
	printer_strobe(hot_time + line_fix[pnum]);

	return 0;
}

static int tm_get()
{
	int tm = 0, tmchange = 0;
	int i;
	tm = open_printer_tm_get(hal_adcGetChannel(ADC_CH_PA6), 1023, 10000);
	return tm;
}

//==ret : 0 is ok , 8: no paper , 9: tm hot , 10: w h err 15 low bat  ,11 Hardware Initialization err ,12 Authorization verification err
//==level    0~4  max~min
//==print mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery 0~4
u8 pi = 0;
// static u8 stream_printe_init=1;

int getCurBatteryADC(void);

// preheat_tem 预热到的温度（单位：摄氏度）
static u8 open_printer_preheat(s32 preheat_tem)
{
	u8 data_arr[384] = {0};
	u16 i;
	if (open_printer_tm_get() < preheat_tem)
	{
		memset(&data_arr[0], 0x00, 384);
		PRINTER_DAT_HIGH();
		for (i = 0; i < 384; i++)
		{
			PRINTER_CLK_HIGH();
			PRINTER_CLK_LOW();
		}
		PRINTER_LATCH_LOW();
		PRINTER_LATCH_HIGH();
		while (open_printer_tm_get() < preheat_tem)
		{
			PRINTER_STB_HIGH();
			printer_delay(5);
			PRINTER_STB_LOW();
			printer_delay(1);
		}
		PRINTER_STB_LOW();
		PRINTER_DAT_LOW();
		for (i = 0; i < 384; i++)
		{
			PRINTER_CLK_HIGH();
			PRINTER_CLK_LOW();
		}
		PRINTER_LATCH_LOW();
		PRINTER_LATCH_HIGH();
	}
}

// 打印函数--已授权
u8 Authorization_LocalPrint(u8 *buf, u16 w, u16 h, s16 level, u8 print_mode, u8 batteryValue)
{
#define PRINT_GRAY_LEVEL 0
	u8 ret = 0, tm = 0;
	u16 batteryValue_mv = getCurBatteryADC() * 19470 / 2046;
	if (SysCtrl.battery <= 2)
	{
		ret = 15;
		goto PRINTER_END;
	}
	pi++;

#if (2 == SENSOR_NUM)
	printer_init();
#endif
	deg_Printf("after  printer_init!\n");
	deg_Printf("=========================[%d]==========================\n", pi);
	deg("w=%d,h=%d,level=%d,print_mode=%d,open_image_credit_check(void)=%d\n", w, h, level, print_mode, open_image_credit_check());
	int CurBatteryADC = getCurBatteryADC();

	deg_Printf("batteryValue=%d\n", batteryValue);
	deg_Printf(" tm_get()=%d,CurBatteryADC=%d,batteryValue_mv=%d(mv)\n", tm_get(), CurBatteryADC, batteryValue_mv);
	// ��ӡ
	hal_gpioInit(MOTO_BIN1_CH, MOTO_BIN1_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN1_CH, MOTO_BIN1_PIN, GPIO_LOW);
	hal_gpioInit(MOTO_BIN2_CH, MOTO_BIN2_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_BIN2_CH, MOTO_BIN2_PIN, GPIO_LOW);
	ax32xx_gpioSFRSet(GPIO_MAP_UARTTX0, UART0_POS_NONE);
	hal_gpioInit(PRINTER_PSENSOR_CH, PRINTER_PSENSOR_PIN, GPIO_INPUT, GPIO_PULL_FLOATING);
	printer_moto_enable(1);
	hal_gpioInit(MOTO_AIN2_CH, MOTO_AIN2_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN2_CH, MOTO_AIN2_PIN, GPIO_LOW);
	printer_delay(500);
	if (printer_get_paper()) // check paper
	{
		ret = 8;
		goto PRINTER_END;
	}
	deg_Printf("after  check paper!\n");
	tm = printer_get_tm(); // check tm
	if (1 == tm)		   // hot
	{
		ret = 9;
		goto PRINTER_END;
	}
	//==set battery==
	deg_Printf("batteryValue =%d,???4-(0),3-(4),2-(8),1-(12),0-(16)\n", batteryValue);
	is_printting = 1;
#if 1
	u8 printer_level_arr[7] = {70, 80, 90, 100, 105, 110, 115};
	u8 printer_level = 3;
	deg_Printf("SysCtrl.printer_level =%d\n", SysCtrl.printer_level);
	if (SysCtrl.printer_level <= 4605)
		printer_level = 0;
	else if (SysCtrl.printer_level <= 5418)
		printer_level = 1;
	else if (SysCtrl.printer_level <= 6375)
		printer_level = 2;
	else if (SysCtrl.printer_level <= 7500)
		printer_level = 3;
	else if (SysCtrl.printer_level <= 8625)
		printer_level = 4;
	else if (SysCtrl.printer_level <= 9918)
		printer_level = 5;
	else if (SysCtrl.printer_level <= 11406)
		printer_level = 6;
	open_printer_preheat(25);
	print_density_set(printer_level_arr[printer_level]);

	u32 paper_feed_to_cutter_line = 10 * 8;
	u32 label_feed_to_cutter_line = 10 * 8;
	ret = open_image_print_masf(buf, w, h, batteryValue_mv, paper_feed_to_cutter_line, label_feed_to_cutter_line);

#else
	printer_moto_enable(1);
	open_printer_set_paper_type(1);
	open_printer_set_contrast(1);
	open_printer_set_clarity(1);

	if (1 == print_mode)
		open_printer_set_darkness(6 + level);
	else
		open_printer_set_darkness(25 + level);

	if (1 == print_mode) // DOT_MATRIX
	{
		u8 *p = buf;

		if (SysCtrl.printer_near_far == 1)
		{

			deg_Printf("print the near image\n");
#if PRINT_GRAY_LEVEL == 0
			open_printer_set_contrast(1);
#else
			open_image_gray_level(0, buf, w, h, 16);
#endif
		}
		else if (SysCtrl.printer_near_far == 2)
		{

			deg_Printf("print the middle image\n");
#if PRINT_GRAY_LEVEL == 0
			open_printer_set_contrast(3);

#else
			open_image_gray_level(0, buf, w, h, 8);
#endif
		}
		else if (SysCtrl.printer_near_far == 3)
		{
			deg_Printf("print the far image\n");

#if PRINT_GRAY_LEVEL == 0
			open_printer_set_contrast(4);
#else
			open_image_gray_level(0, buf, w, h, 32);
#endif
		}
		ret = open_printer_print(buf, w, h, batteryValue_mv);
	}
	else // GRAY LEVEL
	{
		//		u8 *p = buf;

		if (SysCtrl.printer_near_far == 1)
		{
			//			open_image_handle_close(0, 32, buf,w,h);
			deg_Printf("print the near image\n");
#if PRINT_GRAY_LEVEL == 0
			open_printer_set_clarity(2);
#else
			open_image_gray_level(0, buf, w, h, 16);
#endif
		}
		else if (SysCtrl.printer_near_far == 2)
		{
			deg_Printf("print the middle image\n");
#if PRINT_GRAY_LEVEL == 0
			open_printer_set_clarity(3);
#else
			open_image_gray_level(0, buf, w, h, 8);
#endif
		}
		else if (SysCtrl.printer_near_far == 3)
		{
			deg_Printf("print the far image\n");
#if PRINT_GRAY_LEVEL == 0
			open_printer_set_clarity(4);
#else
			open_image_gray_level(0, buf, w, h, 32);
#endif
		}

		ret = open_printer_print(buf, w, h, batteryValue_mv);
	}
#endif

PRINTER_END:
	printer_moto_enable(0);
	hal_adcInit();
	hal_gpioInit(GPIO_PE, GPIO_PIN0, GPIO_INPUT, GPIO_PULL_FLOATING);
	ax32xx_adcEnable(ADC_CH_PE0, 1);
	// re init uart,when need  用作TX  debug
	hal_uartInit();
	deg_Printf("tm_get()=%d\n", tm_get());
	is_printting = 0;
	return ret;
}

// 打印函数--未授权
u8 unAuthorized_LocalPrint(u8 *buf, u16 w, u16 h, s16 level, u8 print_mode, u8 batteryValue)
{
	s32 tm;
	s32 tm_hot_time = 0;
	//	u32 tm_fix = 0;
	u32 tm_battery = 0;
	u32 all_hot_time, delay_time;
	s16 i, j, k;
	u8 ret = 0;
	u32 moto_speed;
	int idx = 0;
	moto_speed = 23; // 23 Bt_Get_Printer_Moto_Speed();
	if (SysCtrl.battery <= 2)
	{
		ret = 15;
		goto PRINTER_END;
	}

	deg_Printf("=========================[%d]==========================\n", pi);
	deg("w=%d,h=%d,level=%d,print_mode=%d\n", w, h, level, print_mode);

	u8 *y_buf = buf;
	y_process(y_buf, w * h);

	// init
	stream_printer_init();
	hal_gpioInit(PRINTER_PSENSOR_CH, PRINTER_PSENSOR_PIN, GPIO_INPUT, GPIO_PULL_FLOATING);
	printer_moto_enable(1);
	printer_delay(500);

	if (printer_get_paper()) // check paper
	{
		ret = 8;
		goto PRINTER_END;
	}
	deg_Printf("after  check paper!\n");

	tm = printer_get_tm(); // check tm

	if (1 == tm) // hot
	{
		ret = 9;
		goto PRINTER_END;
	}

	//==set battery==
	tm_battery = get_battery_time(batteryValue);

	is_printting = 1;
	pi++;

	if (1 == print_mode) // DOT_MATRIX
	{
		u8 *p = y_buf; // buf
		u16 line_fix_time[PRINTER_W / DOT_PARTS_OF_LINE + 1] = {0};
		u16 line_point_cnt;
		printer_moto_enable(1);

#if LINE_TIME_FIX_EN
		line_point_cnt = PRINTER_W / DOT_PARTS_OF_LINE;
		get_dot_fix_time(&tm_hot_time, line_point_cnt, line_fix_time);
#endif

		level = (level / 70);
		if (level > 130)
			level = (level * 6 / 10) / 2;
		else
			level = (level * 5 / 10) / 2;
#if (!DENSITY_PULS)
		all_hot_time = (16 + tm_battery + tm_hot_time + level /*level*7*/) * 10;
#else
		all_hot_time = (16 + tm_battery + tm_hot_time + level /*level*7*/) * 12; // there are 7 levels , but max is same
#endif
		if (all_hot_time >= (16 + tm_battery + tm_hot_time + level /*7*7*/) * 12)
		{
			delay_time = 0;
		}
		else
		{
			delay_time = (16 + tm_battery + tm_hot_time + level /*7*7*/) * 12 - all_hot_time;
		}

		//==first move 2 line==
		printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
		printer_delay(delay_time + all_hot_time + 50);
		printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
		printer_delay(delay_time + all_hot_time + 50);

		if (PRINTER_W == w)
		{
			for (j = 0; j < h; j++)
			{
				printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
				for (idx = 0; idx < DOT_PARTS_OF_LINE; idx++)
				{
					print_dot_align_width(p, idx, DOT_PARTS_OF_LINE, all_hot_time, line_fix_time);
				}

				hal_wdtClear();
				if (printer_get_paper()) // no paper
				{
					ret = 8;
					goto PRINTER_END;
				}
				if (hal_wki1Read())
				{
					ret = 12;
					goto PRINTER_END;
				}
				p += w;
			}
		}
		else if (PRINTER_W == h)
		{
			u32 len;
			u32 offset;
			len = w * h;
			for (j = 0; j < w; j++)
			{
				p = y_buf + j; // buf
				printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);

				for (k = 0; k < 3; k++)
				{
					for (idx = 0; idx < DOT_PARTS_OF_LINE; idx++)
					{
						print_dot_align_height(p, idx, DOT_PARTS_OF_LINE, w, h, all_hot_time, line_fix_time);
					}
				}
				hal_wdtClear();
				if (printer_get_paper()) // no paper
				{
					ret = 8;
					goto PRINTER_END;
				}
				if (hal_wki1Read())
				{
					ret = 12;
					goto PRINTER_END;
				}
			}
		}
		else
		{
			ret = 10;
			goto PRINTER_END;
		}
	}
	else // GRAY LEVEL
	{
		s16 /*i,j,k,*/ pnum;
		u16 line_fix_time[PRINTER_W / GRAY_PARTS_OF_LINE + 1] = {0};
		s16 gray_step = 256 / PRINTER_GRAY_STEP;
		u8 *p = y_buf; // buf
		printer_moto_enable(1);
		u16 gray_tab[PRINTER_GRAY_STEP];
#if 0
		for(k=0;k<PRINTER_GRAY_STEP;k++)
		{
			gray_tab[k]=k*gray_step;
		}
#else
		u8 val = PRINTER_GRAY_STEP / 4;
		u8 tol = 0;
		u8 step1 = 8, step2 = 6, step3 = 6, step4 = 12;
		for (k = 0; k < val; k++) // 0~9
		{
			gray_tab[k] = (k + 1) * step1;
		}
		tol = val * step1;				//
		for (k = val; k < val * 2; k++) // 10~PRINTER_GRAY_STEP-10
		{
			gray_tab[k] = tol + (k - val + 1) * step2; //
		}
		tol = tol + val * step2;			//
		for (k = val * 2; k < val * 3; k++) //
		{
			gray_tab[k] = tol + (k - val * 2 + 1) * step3; //
		}
		tol = tol + val * step3;					  //
		for (k = val * 3; k < PRINTER_GRAY_STEP; k++) // PRINTER_GRAY_STEP-10~PRINTER_GRAY_STEP
		{
			gray_tab[k] = tol + (k - val * 3 + 1) * step4; //
		}
#endif
		u16 line_point_cnt, line_fix_step;
		// u16 line_fix_time[PRINTER_W+1];

		line_point_cnt = PRINTER_W / GRAY_PARTS_OF_LINE;
		get_temp_and_fix_time(&tm_hot_time, line_point_cnt, line_fix_time);

#undef PRINTER_GRAY_TIME
#ifdef PRINTER_3_7V
#define PRINTER_GRAY_TIME 16 // 16//25//16//32//64//64//96//192//?��?��?����?����??
#if LINE_TIME_FIX_EN
#if HALF_LINE_PRINTER_EN
		line_point_cnt = PRINTER_W / 2;
#else
		line_point_cnt = PRINTER_W;
#endif

#endif
#else
#define PRINTER_GRAY_TIME 48
#endif

		//== 5或以上会比原来更浓
		// if(level>3)
		//	level+=2;

#ifdef PRINTER_3_7V // ÷70   大于130x0.6   ，其余x0.5
		level = (level / 70);
		if (level > 130)
			level = level * 6 / 10;
		else
			level = level * 5 / 10;
#if (!DENSITY_PULS)
		all_hot_time = (PRINTER_GRAY_TIME + tm_hot_time * 2 + level /*level*15*/ + tm_battery) * 8 / 10;
#else
		all_hot_time = (PRINTER_GRAY_TIME + tm_hot_time + level /*level*15*/ + tm_battery) * 18 / 10; // there are 7 levels , but max is same
#endif
		if (all_hot_time >= (PRINTER_GRAY_TIME + tm_hot_time + level /*7*15*/ + tm_battery) * 18 / 10)
		{
			delay_time = 0;
		}
		else
		{
			delay_time = (PRINTER_GRAY_TIME + tm_hot_time + level /*7*11*/ + tm_battery) * 18 / 10 - all_hot_time;
		}

#else
		all_hot_time = (PRINTER_GRAY_TIME + tm_hot_time + level * 7 + tm_battery) / 2; // there are 7 levels , but max is same

		if (all_hot_time >= (PRINTER_GRAY_TIME + tm_hot_time + 7 * 7 + tm_battery) / 2)
		{
			delay_time = 0;
		}
		else
		{
			delay_time = (PRINTER_GRAY_TIME + tm_hot_time * 2 + 7 * 7 + tm_battery) / 2 - all_hot_time;
		}
#endif

		//==first move 2 line==
		printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
		printer_delay((delay_time + all_hot_time) * PRINTER_GRAY_STEP + 50);
		printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
		printer_delay((delay_time + all_hot_time) * PRINTER_GRAY_STEP + 50);

		if (PRINTER_W == w)
		{
			for (j = 0; j < h; j++)
			{
				ret = 1;
#if 1
				int count_step;
				for (count_step = 0; count_step < /*STEP_TIMES*/ PRINTER_GRAY_STEP; count_step++)
				{
					for (idx = 0; idx < GRAY_PARTS_OF_LINE; idx++)
					{
						ret = print_grayLocal_align_width(p, idx, GRAY_PARTS_OF_LINE, gray_tab, count_step, all_hot_time, line_fix_time, moto_speed, ret);
					}
				}
/*for (idx = 0; idx < GRAY_PARTS_OF_LINE; idx++)
{
	ret = print_gray_align_width(p,idx,GRAY_PARTS_OF_LINE,gray_tab,all_hot_time,line_fix_time,moto_speed,ret);
}*/
#else
				for (idx = 0; idx < GRAY_PARTS_OF_LINE; idx++)
				{
					ret = print_gray_align_width(p, idx, GRAY_PARTS_OF_LINE, gray_tab, all_hot_time, line_fix_time, moto_speed, ret);
				}
#endif
				p += w;
				// wait moto move finish
				while (ret)
				{
					ret = printer_moto_move_step(1, 1, all_hot_time, k, 0, moto_speed);
				}
				hal_wdtClear();
				if (printer_get_paper()) // no paper
				{
					ret = 8;
					goto PRINTER_END;
				}
				if (hal_wki1Read())
				{
					ret = 12;
					goto PRINTER_END;
				}
			}
		}
		else if (PRINTER_W == h)
		{
			int count_while = 0;
			for (j = 0; j < w; j++)
			{
				p = y_buf + j; // buf+j
				ret = 1;
#if 1
				int count_step;
				for (count_step = 0; count_step < /*STEP_TIMES*/ PRINTER_GRAY_STEP; count_step++)
				{
					for (idx = 0; idx < GRAY_PARTS_OF_LINE; idx++)
					{
						ret = print_grayLocal_align_height(p, idx, GRAY_PARTS_OF_LINE, w, h, gray_tab, count_step, all_hot_time, line_fix_time, moto_speed, ret);
					}
				}
/*for (idx = 0; idx < GRAY_PARTS_OF_LINE; idx++)
{
	ret = print_gray_align_height(p,idx,GRAY_PARTS_OF_LINE,w,h,gray_tab,all_hot_time,line_fix_time,moto_speed,ret);
}*/
#else
				for (idx = 0; idx < GRAY_PARTS_OF_LINE; idx++)
				{
					ret = print_gray_align_height(p, idx, GRAY_PARTS_OF_LINE, w, h, gray_tab, all_hot_time, line_fix_time, moto_speed, ret);
				}
#endif
				// wait moto move finish
				while (ret)
				{
					ret = printer_moto_move_step(1, 1, all_hot_time, k, 0, moto_speed);
					count_while++;
				}
				deg_Printf("count_while=%d\n", count_while);
				hal_wdtClear();
				if (printer_get_paper()) // no paper
				{
					ret = 8;
					goto PRINTER_END;
				}
				if (hal_wki1Read())
				{
					ret = 12;
					goto PRINTER_END;
				}
			}
		}
		else
		{
			ret = 10;
			goto PRINTER_END;
		}
	}

	//==set paper align==
	if (print_mode == 1)
	{
		for (i = 0; i < 80; i++)
		{
			printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
			printer_moto_delay(200);
			printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
			printer_moto_delay(200);
		}
	}
	else
	{
		for (i = 0; i < 80; i++) //?����?�䨰��???���?������DD
		{
			printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
			printer_delay((delay_time + all_hot_time + 50) * PRINTER_GRAY_STEP);
			printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
			printer_delay((delay_time + all_hot_time + 50) * PRINTER_GRAY_STEP);
		}
	}

PRINTER_END:
	// ��ԭ

	// re init ,for adc - bat detective

	printer_moto_enable(0);
	hal_adcInit();
	hal_gpioInit(GPIO_PE, GPIO_PIN0, GPIO_INPUT, GPIO_PULL_FLOATING);
	ax32xx_adcEnable(ADC_CH_PE0, 1);
	// re init uart,when need  用作TX  debug
	hal_uartInit();
	// stream_printe_init=1;

	// hal_sysDelayMS(10);
	deg("T-T-T---%d-- show ret[%d]\n", temprature, ret);
	deg("total time[%d] tm_hot_time[%d] level[%d] tm_battery[%d]\n", (PRINTER_GRAY_TIME + tm_hot_time + level * 7 + tm_battery) / 2, tm_hot_time, level, tm_battery);
	deg("??? tm_battery=%d,tm_hot_time=%d,level=%d level*7=%d all_hot_time:%d time:%d\n", tm_battery, tm_hot_time, level, level * 7, all_hot_time, ((delay_time + all_hot_time + 50) * PRINTER_GRAY_STEP)); // h = 384

	// PA3���������
	// hal_gpioInit(MOTO_BIN1_CH,MOTO_BIN1_PIN,GPIO_INPUT,GPIO_PULL_FLOATING);
	is_printting = 0;

	return ret;
}

u8 printer_print(u8 *buf, u16 w, u16 h, s16 level, u8 print_mode, u8 batteryValue)
{
	u8 ret = 0;
	if (SysCtrl.credit_flag) // 已验证
	{
		// level = level/1000*4;
		// deg_Printf("Local level = %d\n",level);
		ret = Authorization_LocalPrint(buf, w, h, level, print_mode, batteryValue);
	}
	else // 未验证
	{
		ret = unAuthorized_LocalPrint(buf, w, h, level, print_mode, batteryValue);
	}
	hal_gpioInit(GPIO_PF, GPIO_PIN11, GPIO_INPUT, GPIO_PULL_FLOATING);
	hal_gpioInit(GPIO_PE, GPIO_PIN0, GPIO_INPUT, GPIO_PULL_FLOATING);
	return ret;
}

//==en 0: is close , 1 : is open==
static void printer_set_int(u8 en)
{
	static u8 csi, jpga, jpgb, lcdc, de, rotate, emi, usb1, usb2, timer0, timer1, timer2, timer3;

	if (0 == en)
	{
		if (SPR_PICMR & BIT(IRQ_CSI))
		{
			deg_Printf("IRQ_CSI en\n");
			csi = 1;
		}
		else
		{
			csi = 0;
		}
		if (SPR_PICMR & BIT(IRQ_JPGA))
		{
			deg_Printf("IRQ_JPGA en\n");
			jpga = 1;
		}
		else
		{
			jpga = 0;
		}
		if (SPR_PICMR & BIT(IRQ_JPGB))
		{
			deg_Printf("IRQ_JPGB en\n");
			jpgb = 1;
		}
		else
		{
			jpgb = 0;
		}
		if (SPR_PICMR & BIT(IRQ_LCDC))
		{
			deg_Printf("IRQ_LCDC en\n");
			lcdc = 1;
		}
		else
		{
			lcdc = 0;
		}
		if (SPR_PICMR & BIT(IRQ_DE))
		{
			deg_Printf("IRQ_DE en\n");
			de = 1;
		}
		else
		{
			de = 0;
		}
		if (SPR_PICMR & BIT(IRQ_ROTATE))
		{
			deg_Printf("IRQ_ROTATE en\n");
			rotate = 1;
		}
		else
		{
			rotate = 0;
		}
		if (SPR_PICMR & BIT(IRQ_EMI))
		{
			deg_Printf("IRQ_EMI en\n");
			emi = 1;
		}
		else
		{
			emi = 0;
		}
		if (SPR_PICMR & BIT(IRQ_USB11))
		{
			deg_Printf("IRQ_USB11 en\n");
			usb1 = 1;
		}
		else
		{
			usb1 = 0;
		}

		if (SPR_PICMR & BIT(IRQ_USB20))
		{
			deg_Printf("IRQ_USB20 en\n");
			usb2 = 1;
		}
		else
		{
			usb2 = 0;
		}
		/*if(SPR_PICMR&BIT(IRQ_TIMER0))
		{
			deg_Printf("IRQ_TIMER0 en\n");
			timer0 = 1;
		}
		else
		{
			timer0 = 0;
		}*/
		if (SPR_PICMR & BIT(IRQ_TIMER1))
		{
			deg_Printf("IRQ_TIMER1 en\n");
			timer1 = 1;
		}
		else
		{
			timer1 = 0;
		}
		if (SPR_PICMR & BIT(IRQ_TIMER2))
		{
			deg_Printf("IRQ_TIMER2 en\n");
			timer2 = 1;
		}
		else
		{
			timer2 = 0;
		}
		// if(SPR_PICMR&BIT(IRQ_TIMER3))
		// {
		// 	deg_Printf("IRQ_TIMER3 en\n");
		// 	timer3 = 1;
		// }
		// else
		// {
		// 	timer3 = 0;
		// }

		ax32xx_intEnable(IRQ_CSI, 0);
		ax32xx_intEnable(IRQ_JPGA, 0);
		ax32xx_intEnable(IRQ_JPGB, 0);
		ax32xx_intEnable(IRQ_LCDC, 0);
		ax32xx_intEnable(IRQ_DE, 0);
		ax32xx_intEnable(IRQ_ROTATE, 0);
		ax32xx_intEnable(IRQ_EMI, 0);
		ax32xx_intEnable(IRQ_USB11, 0);
		ax32xx_intEnable(IRQ_USB20, 0);
		// ax32xx_intEnable(IRQ_TIMER0,0);
		ax32xx_intEnable(IRQ_TIMER1, 0);
		ax32xx_intEnable(IRQ_TIMER2, 0);
		ax32xx_intEnable(IRQ_TIMER3, 0);
	}
	else
	{
		if (csi)
			ax32xx_intEnable(IRQ_CSI, 1);
		if (jpga)
			ax32xx_intEnable(IRQ_JPGA, 1);
		if (jpgb)
			ax32xx_intEnable(IRQ_JPGB, 1);
		if (lcdc)
			ax32xx_intEnable(IRQ_LCDC, 1);
		if (de)
			ax32xx_intEnable(IRQ_DE, 1);
		if (rotate)
			ax32xx_intEnable(IRQ_ROTATE, 1);
		if (emi)
			ax32xx_intEnable(IRQ_EMI, 1);
		if (usb1)
			ax32xx_intEnable(IRQ_USB11, 1);
		if (usb2)
			ax32xx_intEnable(IRQ_USB20, 1);
		// if(timer0)
		// ax32xx_intEnable(IRQ_TIMER0,1);
		if (timer1)
			ax32xx_intEnable(IRQ_TIMER1, 1);
		if (timer2)
			ax32xx_intEnable(IRQ_TIMER2, 1);
		if (timer3)
			ax32xx_intEnable(IRQ_TIMER3, 1);
	}
}

//==ret : 0 is ok , other err==
//==scale jpg to printer width height==
//==src_jpg: input jpg==
static u8 printer_jpg_to_printer_wh(u8 *src_jpg, u32 *dst_ybuf, u16 *dst_w, u16 *dst_h, u32 jpg_size)
{
	u8 *yuvbuf = NULL;
	u32 ybuf_size, yuvbuf_size;
	u8 *enjpg_buf = NULL;
	u32 enjpg_w = 0, enjpg_h = 0;
	u32 en_ybuf_size;
	u32 en_yuvbuf_size;
	u8 *en_yuvbuf = NULL;
	u16 src_width, src_height, l_width, l_heigth;
	u8 ret, need_encode;
	u32 point = 0;

	INT32S ret_jpg;
	INT8U *thumbBuffer_old = NULL, *tempBuff = NULL;

	ret = 0;
	need_encode = 0;
	*dst_ybuf = 0;
	*dst_w = 0;
	*dst_h = 0;

#if 0
	while(hal_BackRecDecodeStatusCheck()||!ax32xx_csiScalerCheck());

	if(api_backrec_status())
	{
		backrec_adpt_set(0);
	}
#else
	while (!ax32xx_csiScalerCheck())
		;
#endif
	hal_lcdSetPIPEnable(0);
#if 0
	//==check jpg w h==
	hal_mjpegHeaderParse(src_jpg,NULL);
#else
	ret_jpg = 0;
	ret_jpg = hal_mjpegHeaderParse(src_jpg, &thumbBuffer_old);
	if (ret_jpg == -10)
	{
		INT8U app[12];

		point = jpg_size - sizeof app;
		memcpy(app, src_jpg + point, sizeof app);

		if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
		{
			u32 thumbOffset;
			thumbOffset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];

			src_jpg = src_jpg + thumbOffset;
			ret = hal_mjpegHeaderParse(src_jpg, NULL);
		}
		else
		{
			tempBuff = src_jpg;
			src_jpg = thumbBuffer_old;
			deg("Decode thumb\n");
			ret = hal_mjpegHeaderParse(src_jpg, NULL);
		}
	}
	if (ret_jpg < 0)
	{
		deg_Printf("headparse fail .ret_jpg:%d\n", ret_jpg);
		ret_jpg = -1;
	}
	else
	{
		deg_Printf("printer_wh ret_jpg:%d\n", ret_jpg);
	}
#endif
	hal_mjpegDecodeGetResolution(&src_width, &src_height);
	hal_mjpeglDecodeGetResolution(&l_width, &l_heigth);
	deg_Printf("printer_wh:src_width:%d ,src_height:%d ,l_width:%d ,l_height:%d\n", src_width, src_height, l_width, l_heigth);
	if (/*l_width < 4032 && */ src_height >= 384)
	{
		if ((src_width == PRINTER_W) || (src_height == PRINTER_W))
		{
			*dst_w = src_width;
			*dst_h = src_height;
		}
		else if ((src_width > PRINTER_W) && (src_height > PRINTER_W))
		{
			if (src_width >= src_height)
			{
				if (src_width == 4032)
				{
					*dst_w = ((src_width * PRINTER_W) / 2270) & (~0x1f);
				}
				else
					*dst_w = ((src_width * PRINTER_W) / src_height) & (~0x1f);
				*dst_h = PRINTER_W;
			}
			else
			{
				*dst_w = PRINTER_W;
				*dst_h = (src_height * PRINTER_W) / src_width;
			}
		}
		else if ((src_width > PRINTER_W) || (src_height > PRINTER_W))
		{
			if (src_width > PRINTER_W)
			{
				*dst_w = PRINTER_W;
				*dst_h = (src_height * PRINTER_W) / src_width;
			}
			else
			{
				*dst_w = ((src_width * PRINTER_W) / src_height) & (~0x1f);
				*dst_h = PRINTER_W;
			}
		}
		else
		{
			need_encode = 1;
			*dst_w = src_width;
			*dst_h = src_height;
			if (src_width >= src_height)
			{
				enjpg_w = ((src_width * PRINTER_W) / src_height) & (~0x1f);
				enjpg_h = PRINTER_W;
			}
			else
			{
				enjpg_w = PRINTER_W;
				enjpg_h = (src_height * PRINTER_W) / src_width;
			}
		}
	}
	else
	{
		*dst_w = 672;
		*dst_h = 384;
	}
	//==decode==
	ybuf_size = (*dst_w) * (*dst_h);
	yuvbuf_size = (ybuf_size * 3) / 2;
	yuvbuf = (u8 *)hal_sysMemMalloc(yuvbuf_size, 32);
	/*deg_Printf("\n\n");
	hal_sysMemPrint();
	deg_Printf("\n\n");*/

	if (NULL == yuvbuf)
	{
		ret = 1;
		goto PRINTER_SCALE_JPG;
	}
	deg_Printf("*dst_w:%d,*dst_h:%d,ybuf_size=%d,yuvbuf_size=%d\n", *dst_w, *dst_h, ybuf_size, yuvbuf_size);

	*dst_ybuf = (u32)yuvbuf;

	if (/*l_width < 4032 &&*/ src_height >= 384)
	{
		if (hal_mjpegDecodeNoIsr(src_jpg, (u8 *)(yuvbuf), (u8 *)(yuvbuf + ybuf_size), *dst_w, *dst_h) < 0)
		{
			ret = 3;
			goto PRINTER_SCALE_JPG;
		}

		if (!jpeg1_decode_check())
		{
			ret = 4;
			goto PRINTER_SCALE_JPG;
		}
		ax32xx_sysDcacheFlush((u32)yuvbuf, yuvbuf_size);
		deg_Printf("pass decode:%d\n", need_encode);

		if (need_encode)
		{
			enjpg_buf = (u8 *)hal_sysMemMalloc(100 * 1024, 32); // i guess no over 100KB
			if (NULL == enjpg_buf)
			{
				ret = 2;
				goto PRINTER_SCALE_JPG;
			}

			deg_Printf("enjpg_w:%d,enjpg_h:%d\n", enjpg_w, enjpg_h);

			//==encode==
			ax32xx_mjpB_Ctl_init(0, JPEG_Q_75, *dst_w, *dst_h, enjpg_w, enjpg_h);
			ax32xx_mjpB_Linebuf_cfg((u8 *)(yuvbuf), (u8 *)(yuvbuf + ybuf_size));
			ax32xx_mjpB_dma_cfg((u32)enjpg_buf, (u32)(enjpg_buf + 100 * 1024));
			hal_watermarkEnable(0, 0);
			ax32xx_mjpB_on_2();
			if (ax32xx_mjpegB_encode_check() < 0)
			{
				ret = 5;
				goto PRINTER_ENCODE_ERR;
			}
			ax32xx_sysDcacheFlush((u32)enjpg_buf, 100 * 1024);
			deg_Printf("pass encode\n");

			//==decode==
			en_ybuf_size = enjpg_w * enjpg_h;
			en_yuvbuf_size = en_ybuf_size * 3 / 2;
			en_yuvbuf = (u8 *)hal_sysMemMalloc(en_yuvbuf_size, 32);
			if (NULL == en_yuvbuf)
			{
				ret = 1;
				goto PRINTER_ENCODE_ERR;
			}

			if (hal_mjpegDecodeNoIsr(enjpg_buf, (u8 *)(en_yuvbuf), (u8 *)(en_yuvbuf + en_ybuf_size), enjpg_w, enjpg_h) < 0)
			{
				ret = 3;
				goto PRINTER_ENCODE_ERR;
			}

			if (!jpeg1_decode_check())
			{
				ret = 4;
				goto PRINTER_ENCODE_ERR;
			}
			ax32xx_sysDcacheFlush((u32)en_yuvbuf, en_yuvbuf_size);
			deg_Printf("pass decode\n");

			*dst_w = enjpg_w;
			*dst_h = enjpg_h;
			*dst_ybuf = (u32)en_yuvbuf;
		PRINTER_ENCODE_ERR:

			if (yuvbuf)
			{
				hal_sysMemFree(yuvbuf);
				yuvbuf = NULL;
			}
		}
	}
	else
	{
		ret = decodePic(src_jpg, yuvbuf, yuvbuf + ybuf_size, *dst_w, *dst_h, jpg_size);
		if (ret)
		{
			deg_Printf("decodePic ret:%d dst_w:%d dst_h:%d\n", ret, *dst_w, *dst_h);
			ret = 0;
		}
		else
		{
			deg_Printf("decode Pic fail !\n");
			if (yuvbuf)
			{
				hal_sysMemFree(yuvbuf);
				yuvbuf = NULL;
			}
			ret = -1;
		}
	}
PRINTER_SCALE_JPG:

	if (need_encode)
	{
		if (enjpg_buf)
		{
			hal_sysMemFree(enjpg_buf);
			enjpg_buf = NULL;
		}
	}
	(void)tempBuff;

	return ret;
}

u8 printer_dot_matrix_handle(u8 *ybuf, u16 w, u16 h)
{
	int ret = -1;

	deg_Printf("dot_matrix ybuf w:%d,w:%d\n", w, h);

#if 0	
	char *name1;
	int fHandle1;
	fHandle1 = task_image_createfile(VIDEO_CH_A,&name1);

	write(fHandle1,(void *)ybuf,w*h*3/2);
	deg_Printf("111dicodePic save :%x Size:%x name:%d\n",ybuf,w*h*3/2,fHandle1);
	deamon_fsSizeModify(-1,fs_size(fHandle1));
	close(fHandle1);
#endif

#if 1
	if ((384 == w) && (672 == h))
	{
		ret = CLAHE(ybuf, w, h, 0, 255, w / 48, h / 84, 64, 4.0f); // 2.0f
	}
	else if ((672 == w) && (384 == h))
	{
		/*hal_sysMemPrint();
		deg_Printf("----------w:%d h:%d %d %d %d \n",w,h,w/84,h/48,64);
		deg_Printf("count :%d\n",sizeof(u32)*64*(h/48)*(w/84));*/
		ret = CLAHE(ybuf, w, h, 0, 255, w / 84, h / 48, 64, 4.0f); // 2.0f
	}
	else if ((384 == w) && (216 == h))
	{
		ret = CLAHE(ybuf, w, h, 0, 255, w / 48, h / 27, 64, 4.0f); // 2.0f
	}
	else if ((216 == w) && (384 == h))
	{
		ret = CLAHE(ybuf, w, h, 0, 255, w / 27, h / 48, 64, 4.0f); // 2.0f
	}
	else if ((320 == w) && (180 == h))
	{
		ret = CLAHE(ybuf, w, h, 0, 255, w / 32, h / 18, 64, 4.0f); // 2.0f
	}
	else if ((320 == w) && (240 == h))
	{
		ret = CLAHE(ybuf, w, h, 0, 255, w / 32, h / 24, 64, 4.0f); // 2.0f
	}
	else if ((384 == h) && (512 == w))
	{
		ret = CLAHE(ybuf, w, h, 0, 255, w / 64, h / 48, 64, 4.0f);
	}
#else
	ret = 0;
#endif
	//?��?�C????????43?o??64?��?-?????3?��?????o?????-?????��?????2?��???��?�C?????��?3?????��???????????3??	//???????????��?��??????
	//?��?�C?????????????��???��?��???��?��???????????��?3???��?��???��???��???��???��???��???????2?o?��?o?��???��???��???��???��???�C???��???��???��?????????��?o??
	//?��?1?????��???????????�C?��?��?-???????��?????��???????��?��?��???��?3256?��???��???????��?��???????1???��ram?��?3?????��???��?2???a?????????��?�C???????????��?��?a?��???????�C?��?��?????????????��
	//?��?�C?????��???????��???��???????��???????��?????��?��?a?��??ram?��???��?3

#if 0	
			char *name;
			int fHandle;
			fHandle = task_image_createfile(VIDEO_CH_A,&name);
		
			write(fHandle,(void *)ybuf,w*h*3/2);
			deg_Printf("111dicodePic save :%x Size:%x name:%d\n",ybuf,w*h*3/2,fHandle);
			deamon_fsSizeModify(-1,fs_size(fHandle));
			close(fHandle);
#endif

	deg_Printf("matrix show ret:%d\n", ret);
	if (0 == ret)
	{
		FloydSteinbergDithering(ybuf, w, h, 0);
	}
	else
	{
		deg_Printf("clahe err:%d\n", ret);
		ret = 1;
	}

	return ret;
}

//==ret: 0 :is ok , 1: mem err ,2: mem err , 3: jpg dec err , 8: no paper ,9 : tm hot
//==fd : jpeg file ==
//==level:  0~4   max~min
//==print_mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery : 0~4
#if SDRAM_SIZE == SDRAM_SIZE_2M
#define ALLOC_JPEG_SIZE 100 * 1024
#else
#define ALLOC_JPEG_SIZE 500 * 1024
#endif
u8 printer_jpeg(int fd, s16 level, u8 print_mode, u8 batteryValue)
{
	u32 ybuf /*,y_buf_size*/, res;
	s32 jpg_size;
	u16 printer_w, printer_h;
	u8 *jpg_y_buf = NULL;
	u8 *jpeg = NULL;
	u8 ret;

	jpg_size = fs_size(fd);
	deg_Printf("jpg_size=%d\n", jpg_size);

	if (jpg_size < 512) // too mall , jpg file not ok
	{
		if (jpeg)
		{
			hal_sysMemFree(jpeg);
		}
		ret = 2;
		goto PRINTER_END;
	}
	/*deg_Printf("\n\n");
	hal_sysMemPrint();
	deg_Printf("\n\n");*/
	//----------Printer read thumb-------

	// if(jpg_size > (hal_sysMemRemain()+64))		//fsSize large than remain space
	{
		jpeg = (INT8U *)hal_sysMemMalloc(ALLOC_JPEG_SIZE, 64); // malloc space
		read(fd, jpeg, ALLOC_JPEG_SIZE);
		ax32xx_sysDcacheFlush((u32)jpeg, ALLOC_JPEG_SIZE);
	}

	res = hal_mjpegHeaderParse(jpeg, NULL);
	deg_Printf("printer jpg:check res[%d]\n", res);
	if (res == -10 || res == 0)
	{

		INT8U app[12];
		lseek(fd, jpg_size - sizeof(app), 0);
		read(fd, app, sizeof(app));
		if (app[4] == 'J' && app[5] == 'R' && app[6] == 'X')
		{
			u32 thumbOffset;
			thumbOffset = app[8] << 24 | app[9] << 16 | app[10] << 8 | app[11];
			deg_Printf("thumbOffset=0x%x \n", thumbOffset);
			lseek(fd, thumbOffset, 0);
			read(fd, jpeg, ALLOC_JPEG_SIZE);
			ax32xx_sysDcacheFlush((u32)jpeg, ALLOC_JPEG_SIZE);
			deg_Printf("Decode thumb\n");
			deg_Printf("read jpg data : jpgSize:%d thumbSize:%d\n", jpg_size, jpg_size - thumbOffset);
		}
		else
		{

			if ((hal_sysMemRemain() - 672 * 384 * 3 / 2 - 23 * 1024 + ALLOC_JPEG_SIZE) > jpg_size)
			{
				deg_Printf("outside pic print\n");
				deg_Printf("---[%d]---", (hal_sysMemRemain() - 672 * 384 * 3 / 2 - 23 * 1024 + ALLOC_JPEG_SIZE));
				hal_sysMemFree(jpeg);
				jpeg = (INT8U *)hal_sysMemMalloc(jpg_size, 64);
				if (jpeg == NULL)
					goto PRINTER_END;
				lseek(fd, 0, 0);
				read(fd, jpeg, jpg_size);
				ax32xx_sysDcacheFlush((u32)jpeg, jpg_size);

				/*deg_Printf("\n\n");
				hal_sysMemPrint();
				deg_Printf("\n\n");*/
			}
			else
			{
				deg_Printf("---[%d]---", (hal_sysMemRemain() - 672 * 384 * 3 / 2 - 23 * 1024 + ALLOC_JPEG_SIZE));
				deg_Printf("alloc fail!! \noutdside pic Size %d > %d:remainSize\n", jpg_size, hal_sysMemRemain());
				ret = 2;
				goto PRINTER_END;
			}
		}
	}
	else
	{
		deg_Printf("res else!!\n");
		ret = 1;
		goto PRINTER_END;
	}

	//-------------------------------

	ret = printer_jpg_to_printer_wh(jpeg, &ybuf, &printer_w, &printer_h, jpg_size);
	jpg_y_buf = (u8 *)ybuf;
	deg_Printf("printer_w=%d,printer_h=%d,ret=%d\n", printer_w, printer_h, ret);
	if (0 == ret)
	{
		if (1 == print_mode)
		{
			printer_dot_matrix_handle(jpg_y_buf, printer_w, printer_h);
		}
		// printer_set_int(0);
		u8 i = 0; // 解压data
		u8 a[5] = {3, 4, 8, 16, 32};
		ret = printer_print((u8 *)jpg_y_buf, printer_w, printer_h, level, print_mode, batteryValue);
	}

PRINTER_END:

#if 0
	if
(outStr)
	{
		hal_sysMemFree(outStr);
	}
#endif
	if (jpg_y_buf)
	{
		hal_sysMemFree(jpg_y_buf);
	}

	if (jpeg)
	{
		hal_sysMemFree(jpeg);
	}

	dispLayerSetPIPMode(DISP_PIP_FRONT);
	deg_Printf("ret=%d\n", ret);
	return ret;
}

//==ret: 0 :is ok , 1: mem err ,2: mem err , 3: jpg dec err , 8: no paper ,9 : tm hot ,10: w ,h err
//==jpeg: jpeg buf ==
//==level:  0~4   max~min
//==print_mode : 0: GRAY_LEVEL,  1: DOT_MATRIX
//==battery : 0~4

u8 printer_jpeg_buf(u8 *jpeg, s16 level, u8 print_mode, u8 batteryValue, u32 jpg_size)
{
	u32 ybuf /*,y_buf_size*/;
	u16 printer_w, printer_h;
	u8 *jpg_y_buf;
	u8 ret;

	if (!SysCtrl.stream_print)
	{
		jpg_y_buf = NULL;
		ret = printer_jpg_to_printer_wh(jpeg, &ybuf, &printer_w, &printer_h, jpg_size);
		jpg_y_buf = (u8 *)ybuf;
		deg_Printf("printer_w=%d,printer_h=%d,ret=%d\n", printer_w, printer_h, ret);

		if (0 == ret)
		{
			if (1 == print_mode)
			{
				printer_dot_matrix_handle(jpg_y_buf, printer_w, printer_h);
			}

			printer_set_int(0);
			ret = printer_print((u8 *)jpg_y_buf, printer_w, printer_h, level, print_mode, batteryValue);
			printer_set_int(1);
		}
	}
	else
	{

		u16 w, h;
		w = 0; //
		h = 0;
		printer_set_int(0);
		ret = stream_printer_run(w, h, level, batteryValue);
		printer_set_int(1);
	}

	deg_Printf("_%s_ stream_print:%d\n", __func__, SysCtrl.stream_print);
PRINTER_2_END:

	if (jpg_y_buf)
	{
		hal_sysMemFree(jpg_y_buf);
	}

	dispLayerSetPIPMode(DISP_PIP_FRONT);
	deg(" in printer_jpeg_buf: ret=%d\n", ret);
	return ret;
}

u8 Flash_Photo_Print(u8 *jpeg, u16 level, u8 print_mode, u8 batteryValue)
{

	u8 ret;
	u16 dst_w, dst_h, src_width, src_height;
	u8 *jpg_y_buf, jpg_uv_buf;

#if PIC_16_9
	dst_w = 672;
	dst_h = 384;
#else
#if defined(RESOLUTION_720P)
	dst_w = 672;
#else
	dst_w = 512;
#endif
	dst_h = 384;
#endif
	deg_Printf("Flash_Photo_Print :111 show reamin space:%d\n", hal_sysMemRemain());
	jpg_y_buf = hal_sysMemMalloc(dst_w * dst_h * 3 / 2, 64);

	ret = decodePic(jpeg, jpg_y_buf, jpg_y_buf + dst_w * dst_h, dst_w, dst_h, 0);
	if (ret)
	{
		deg_Printf("decode Pic success dst_w[%d] dst_h[%d]\n", dst_w, dst_h);
		ret = 0;
	}
	else
	{
		deg_Printf("decode Pic fail !\n");
		return -1;
	}

	deg_Printf("Flash_Photo_Print :222 show reamin space:%d\n", hal_sysMemRemain());
	if (0 == ret)
	{
		if (1 == print_mode)
		{
			printer_dot_matrix_handle(jpg_y_buf, dst_w, dst_h);
		}

		printer_set_int(0);
		ret = printer_print((u8 *)jpg_y_buf, dst_w, dst_h, level, print_mode, batteryValue);
		deg_Printf("read flash photo print ret:%d\n", ret);
		printer_set_int(1);
	}

	if (jpg_y_buf)
		hal_sysMemFree(jpg_y_buf);
	return ret;
}

static void printer_get_tempture(s32 *hot_time)
{
#if 0
	//使用行数来做补偿判断
	u32 line = printer_get_line();
#define DEFINE_HEAT 75
	if		(line <=50)	{*hot_time = DEFINE_HEAT;}
	else if	(line <=100){*hot_time = DEFINE_HEAT-7;}
	else if	(line <=150){*hot_time = DEFINE_HEAT-14;}
	else if	(line <=200){*hot_time = DEFINE_HEAT-21;}
	else if	(line <=250){*hot_time = DEFINE_HEAT-28;}
	else if	(line <=300){*hot_time = DEFINE_HEAT-35;}
	else if	(line <=350){*hot_time = DEFINE_HEAT-42;}
	else if	(line <=400){*hot_time = DEFINE_HEAT-49;}
	else if	(line <=450){*hot_time = DEFINE_HEAT-56;}
	else if	(line <=500){*hot_time = DEFINE_HEAT-63;}
	else if	(line <=550){*hot_time = DEFINE_HEAT-70;}
	else if	(line <=600){*hot_time = DEFINE_HEAT-75;}
	
	else 				{*hot_time = DEFINE_HEAT-75;}

#else

	//	s32 tm = hal_adcGetChannel(ADC_CH_PA6);
	u32 tm = Bt_Stream_getADtm(); // printer_getADtm();
	if (!tm)
		tm = (u32)hal_adcGetChannel(ADC_CH_PA6);
	// 25摄氏度~75摄氏度
	//			AD值				--		摄氏度
	if (tm <= 250)
	{
		*hot_time = 0;
	} // 85 过热
	else if (tm <= 316)
	{
		*hot_time = 1;
	} // 75
	else if (tm <= 323)
	{
		*hot_time = 1;
	}
	else if (tm <= 321)
	{
		*hot_time = 2;
	}
	else if (tm <= 326)
	{
		*hot_time = 3;
	}
	else if (tm <= 329)
	{
		*hot_time = 4;
	}
	else if (tm <= 333)
	{
		*hot_time = 5;
	}
	else if (tm <= 340)
	{
		*hot_time = 6;
	}
	else if (tm <= 343)
	{
		*hot_time = 7;
	}
	else if (tm <= 347)
	{
		*hot_time = 8;
	} // 70
	else if (tm <= 353)
	{
		*hot_time = 9;
	}
	else if (tm <= 356)
	{
		*hot_time = 10;
	}
	else if (tm <= 359)
	{
		*hot_time = 11;
	}
	else if (tm <= 365)
	{
		*hot_time = 12;
	}
	else if (tm <= 368)
	{
		*hot_time = 13;
	}
	else if (tm <= 371)
	{
		*hot_time = 14;
	}
	else if (tm <= 377)
	{
		*hot_time = 15;
	} // 65
	else if (tm <= 382)
	{
		*hot_time = 16;
	}
	else if (tm <= 387)
	{
		*hot_time = 17;
	}
	else if (tm <= 397)
	{
		*hot_time = 18;
	}
	else if (tm <= 402)
	{
		*hot_time = 19;
	}
	else if (tm <= 407)
	{
		*hot_time = 20;
	}
	else if (tm <= 416)
	{
		*hot_time = 21;
	}
	else if (tm <= 421)
	{
		*hot_time = 22;
	}
	else if (tm <= 426)
	{
		*hot_time = 23;
	} // 60
	else if (tm <= 435)
	{
		*hot_time = 24;
	}
	else if (tm <= 440)
	{
		*hot_time = 25;
	}
	else if (tm <= 444)
	{
		*hot_time = 26;
	}
	else if (tm <= 452)
	{
		*hot_time = 27;
	}
	else if (tm <= 456)
	{
		*hot_time = 28;
	}
	else if (tm <= 461)
	{
		*hot_time = 29;
	}
	else if (tm <= 469)
	{
		*hot_time = 30;
	} // 55
	else if (tm <= 462)
	{
		*hot_time = 31;
	}
	else if (tm <= 485)
	{
		*hot_time = 32;
	}
	else if (tm <= 493)
	{
		*hot_time = 33;
	}
	else if (tm <= 497)
	{
		*hot_time = 34;
	}
	else if (tm <= 501)
	{
		*hot_time = 35;
	}
	else if (tm <= 509)
	{
		*hot_time = 36;
	}
	else if (tm <= 513)
	{
		*hot_time = 37;
	}
	else if (tm <= 517)
	{
		*hot_time = 38;
	} // 50
	else if (tm <= 524)
	{
		*hot_time = 39;
	}
	else if (tm <= 527)
	{
		*hot_time = 40;
	}
	else if (tm <= 531)
	{
		*hot_time = 41;
	}
	else if (tm <= 538)
	{
		*hot_time = 42;
	}
	else if (tm <= 541)
	{
		*hot_time = 43;
	}
	else if (tm <= 545)
	{
		*hot_time = 44;
	}
	else if (tm <= 564)
	{
		*hot_time = 45;
	} // 45
	else if (tm <= 570)
	{
		*hot_time = 46;
	}
	else if (tm <= 576)
	{
		*hot_time = 47;
	}
	else if (tm <= 588)
	{
		*hot_time = 48;
	}
	else if (tm <= 594)
	{
		*hot_time = 49;
	}
	else if (tm <= 599)
	{
		*hot_time = 50;
	}
	else if (tm <= 619)
	{
		*hot_time = 51;
	}
	else if (tm <= 623)
	{
		*hot_time = 52;
	}
	else if (tm <= 628)
	{
		*hot_time = 53;
	} // 40
	else if (tm <= 637)
	{
		*hot_time = 54;
	}
	else if (tm <= 641)
	{
		*hot_time = 55;
	}
	else if (tm <= 646)
	{
		*hot_time = 56;
	}
	else if (tm <= 662)
	{
		*hot_time = 57;
	}
	else if (tm <= 668)
	{
		*hot_time = 58;
	}
	else if (tm <= 673)
	{
		*hot_time = 59;
	}
	else if (tm <= 683)
	{
		*hot_time = 60;
	} // 35
	else if (tm <= 687)
	{
		*hot_time = 61;
	}
	else if (tm <= 690)
	{
		*hot_time = 62;
	}
	else if (tm <= 696)
	{
		*hot_time = 63;
	}
	else if (tm <= 699)
	{
		*hot_time = 64;
	}
	else if (tm <= 703)
	{
		*hot_time = 65;
	}
	else if (tm <= 708)
	{
		*hot_time = 66;
	}
	else if (tm <= 715)
	{
		*hot_time = 67;
	}
	else if (tm <= 723)
	{
		*hot_time = 68;
	} // 30
	else if (tm <= 733)
	{
		*hot_time = 69;
	}
	else if (tm <= 737)
	{
		*hot_time = 70;
	}
	else if (tm <= 742)
	{
		*hot_time = 71;
	}
	else if (tm <= 751)
	{
		*hot_time = 72;
	}
	else if (tm <= 755)
	{
		*hot_time = 73;
	}
	else if (tm <= 759)
	{
		*hot_time = 74;
	}
	else if (tm <= 768)
	{
		*hot_time = 75;
	} // 25
	else if (tm <= 772)
	{
		*hot_time = 76;
	}
	else if (tm <= 776)
	{
		*hot_time = 77;
	}
	else if (tm <= 784)
	{
		*hot_time = 78;
	}
	else if (tm <= 788)
	{
		*hot_time = 79;
	}
	else if (tm <= 793)
	{
		*hot_time = 80;
	}
	else if (tm <= 801)
	{
		*hot_time = 81;
	}
	else if (tm <= 805)
	{
		*hot_time = 82;
	}
	else if (tm <= 809)
	{
		*hot_time = 83;
	} // 20
	else if (tm <= 817)
	{
		*hot_time = 84;
	}
	else if (tm <= 820)
	{
		*hot_time = 85;
	}
	else if (tm <= 823)
	{
		*hot_time = 86;
	}
	else if (tm <= 831)
	{
		*hot_time = 87;
	}
	else if (tm <= 835)
	{
		*hot_time = 88;
	}
	else if (tm <= 838)
	{
		*hot_time = 89;
	}
	else if (tm <= 845)
	{
		*hot_time = 90;
	} // 15
	else if (tm <= 848)
	{
		*hot_time = 91;
	}
	else if (tm <= 852)
	{
		*hot_time = 92;
	}
	else if (tm <= 858)
	{
		*hot_time = 93;
	}
	else if (tm <= 861)
	{
		*hot_time = 94;
	}
	else if (tm <= 864)
	{
		*hot_time = 95;
	}
	else if (tm <= 871)
	{
		*hot_time = 96;
	}
	else if (tm <= 873)
	{
		*hot_time = 97;
	}
	else if (tm <= 877)
	{
		*hot_time = 98;
	} // 10
	else if (tm <= 883)
	{
		*hot_time = 99;
	}
	else if (tm <= 890)
	{
		*hot_time = 100;
	}
	else if (tm <= 893)
	{
		*hot_time = 101;
	}
	else if (tm <= 896)
	{
		*hot_time = 102;
	}
	else if (tm <= 898)
	{
		*hot_time = 103;
	}
	else if (tm <= 901)
	{
		*hot_time = 104;
	}
	else if (tm <= 907)
	{
		*hot_time = 105;
	} // 5
	else
		*hot_time = 108; // below 5

#endif
}

static u8 get_temp_time(u32 *hot_time)
{
	s32 tm;

	tm = printer_get_tm(); // check tm
	if (tm <= 1)		   // hot
	{
		if (hal_wki1Read())
		{
			*hot_time = 0;
			return 12;
		}
		*hot_time = 0;
		return 9;
	}
	else if (2 == tm)
	{
		*hot_time = 1;
	}
	else if (3 == tm)
	{
		*hot_time = 4;
	}
	else if (4 == tm)
	{
		*hot_time = 8;
	}
	else if (5 == tm)
	{
		*hot_time = 12;
	}
	else if (6 == tm)
	{
		*hot_time = 16;
	}
	else
	{
		*hot_time = 18;
	}

	return 0;
}

static u32 get_battery_time(u8 batteryValue)
{
	u32 tm_battery;

	if (batteryValue >= 4)
	{
		tm_battery = 0;
	}
	else if (3 == batteryValue)
	{
		tm_battery = 4;
	}
	else if (2 == batteryValue)
	{
		tm_battery = 8;
	}
	else if (1 == batteryValue)
	{
		tm_battery = 12;
	}
	else // if(0 == batteryValue)
	{
		tm_battery = 16;
	}

	return tm_battery;
}

static u8 get_dot_fix_time(s32 *hot_time, u16 point_cnt, u16 *fix_time)
{
	u8 start_fix[/*DOT_PARTS_OF_LINE*/] = {64, 64, 64, 64, 64, 64, 64, 64}; //{46,46,46,46,46,46,46,46};
	u16 fix_step = 1;
	int i;

#if 1

	s32 tm;

	tm = printer_get_tm();
	if (tm == 0)
	{
		*hot_time = 0;
		fix_step = 65;
	}
	else if (tm <= 2)
	{
		*hot_time = 1;
		fix_step = 65;
	}
	else if (3 == tm)
	{
		*hot_time = 12; // 8;
		fix_step = 60;
	}
	else if (4 == tm)
	{
		*hot_time = 24; // 16;
		fix_step = 55;
	}
	else if (5 == tm)
	{
		*hot_time = 36; // 24;
		fix_step = 50;
	}
	else if (6 == tm)
	{
		*hot_time = 48; // 32;
		fix_step = 45;
	}
	else
	{
		*hot_time = 40;
		fix_step = 40;
	}

	fix_step /= GRAY_PARTS_OF_LINE;

#endif

	for (i = 0; i <= point_cnt; i++)
	{
		if (i > start_fix[DOT_PARTS_OF_LINE - 1])
		{
			fix_time[i] = (i - start_fix[DOT_PARTS_OF_LINE - 1]) * 10 /*30*/ / fix_step;
		}
		else
		{
			fix_time[i] = 0;
		}
	}

	return 0;
}

static u8 get_temp_and_fix_time(s32 *hot_time, u16 point_cnt, u16 *fix_time)
{
	u8 start_fix[/*GRAY_PARTS_OF_LINE*/] = {24, 24, 24, 24};
	s32 tm;
	u16 fix_step;
	int i;
	// 40-65
	// tm = printer_get_tm();
	printer_get_tempture(&(*hot_time));
	/*if(*hot_time <=5)//75-70
	{
		fix_step = 70;
	}else */
	if (*hot_time <= 10) // 70-65
	{
		fix_step = 67; // 70;
	}
	else if (*hot_time <= 15) // 65-60
	{
		fix_step = 65; // 65;
	}
	else if (*hot_time <= 20) // 60-55
	{
		fix_step = 63; // 63;
	}
	else if (*hot_time <= 25) // 55-50
	{
		fix_step = 60; // 60;
	}
	else if (*hot_time <= 30) // 50-45
	{
		fix_step = 57; // 57;
	}
	else if (*hot_time <= 35) // 45-40
	{
		fix_step = 55;
	}
	else if (*hot_time <= 40) // 40-35
	{
		fix_step = 53;
	}
	else if (*hot_time <= 45) // 35-30
	{
		fix_step = 50;
	}
	else if (*hot_time <= 50) // 30-25
	{
		fix_step = 47;
	}
	else if (*hot_time <= 55) // 25-20
	{
		fix_step = 45;
	}
	else if (*hot_time <= 60) // 20-15
	{
		fix_step = 43;
	}
	else if (*hot_time <= 65) // 15-10
	{
		fix_step = 40;
	}
	else if (*hot_time <= 70) // 10-5
	{
		fix_step = 40;
	}
	else
	{
		fix_step = 37;
	}

	fix_step /= GRAY_PARTS_OF_LINE;
	for (i = 0; i <= point_cnt; i++)
	{
		if (i > start_fix[GRAY_PARTS_OF_LINE - 1])
		{
			fix_time[i] = (i - start_fix[GRAY_PARTS_OF_LINE - 1]) * 10 / fix_step;
		}
		else
		{
			fix_time[i] = 0;
		}
	}

	return 0;
}

void stream_printer_init(void)
{
	//==PB6  PB5 handle clean USB setting==
	USB11CON0 = 0;
	USB11CON1 = 0;
	USB11CON0 |= (1 << 6);			 // control by soft
	USB11CON1 &= ~(BIT(4) | BIT(6)); // disable dp,dm 120K pullup
	USB11CON1 &= ~(BIT(7) | BIT(5)); // disable dp,dm 15k pulldown

	// power
	hal_gpioInit(PRINTER_POWER_CH, PRINTER_POWER_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_POWER_CH, PRINTER_POWER_PIN, GPIO_LOW);

	// moto
	hal_gpioInit(MOTO_AIN1_CH, MOTO_AIN1_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioInit(MOTO_AIN2_CH, MOTO_AIN2_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioInit(MOTO_BIN1_CH, MOTO_BIN1_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioInit(MOTO_BIN2_CH, MOTO_BIN2_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(MOTO_AIN1_CH, MOTO_AIN1_PIN, GPIO_LOW);
	hal_gpioWrite(MOTO_AIN2_CH, MOTO_AIN2_PIN, GPIO_LOW);
	hal_gpioWrite(MOTO_BIN1_CH, MOTO_BIN1_PIN, GPIO_LOW);
	hal_gpioWrite(MOTO_BIN2_CH, MOTO_BIN2_PIN, GPIO_LOW);

	// data/clk/latch/stb
	hal_gpioInit(PRINTER_DATA_CH, PRINTER_DATA_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioInit(PRINTER_CLK_CH, PRINTER_CLK_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioInit(PRINTER_LATCH_CH, PRINTER_LATCH_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioInit(PRINTER_STB_CH, PRINTER_STB_PIN, GPIO_OUTPUT, GPIO_PULL_FLOATING);
	hal_gpioWrite(PRINTER_DATA_CH, PRINTER_DATA_PIN, GPIO_LOW);
	hal_gpioWrite(PRINTER_CLK_CH, PRINTER_CLK_PIN, GPIO_LOW);
	hal_gpioWrite(PRINTER_LATCH_CH, PRINTER_LATCH_PIN, GPIO_HIGH);
	hal_gpioWrite(PRINTER_STB_CH, PRINTER_STB_PIN, GPIO_LOW);
}

static u8 print_dot_line(u8 *data, int idx, int times, u8 k, u32 hot_time, u16 line_fix[PRINTER_W + 1], u8 ret)
{
	int i, j;
	s16 pnum;
	for (j = 0; j < 10 /*20*/; j++)
	{
		for (i = 0; i < PRINTER_W; i++)
		{
			pnum = 0;
			btcomService();
			if (*(data + i) == 1) //(*(data+i) <= 128)
			{
				if ((i % times) == idx)
				{
					PRINTER_DAT_HIGH();
					pnum++;
				}
				else
				{
					PRINTER_DAT_LOW();
				}
			}
			else
			{
				PRINTER_DAT_LOW();
			}
			PRINTER_CLK_HIGH();
			PRINTER_CLK_LOW();
		}
		PRINTER_LATCH_LOW();
		PRINTER_LATCH_HIGH();
		printer_strobe(hot_time + line_fix[pnum]);
		/*if(ret)
			ret =  printer_strobe_moto_move(1,hot_time,k,line_fix[pnum]);
		else
			ret = printer_strobe_moto_move(0,hot_time,k,line_fix[pnum]);*/
	}
	return ret;
}

static u8 stream_print_dot(u8 *buf, u16 w, u16 h, u8 k, u32 hot_time, u16 *line_fix, u32 moto_speed)
{
	u8 ret = 0;
	int idx = 0;
	u16 j;
	u8 *p = buf;
	// deg_Printf(" SPD\n");
	for (j = 0; j < h; j++)
	{
		printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
		// ret =0;
		for (idx = 0; idx < DOT_PARTS_OF_LINE; idx++)
		{
			ret = print_dot_line(p, idx, DOT_PARTS_OF_LINE, k, hot_time, line_fix, ret);
		}
		p += w;

		hal_wdtClear();

		// wait moto move finish
		/*printer_moto_move_step(1,0,0,0,0,moto_speed);
		while(ret)
		{
			ret=printer_moto_move_step(1,1,hot_time,k,0,moto_speed);
		}*/
		if (printer_get_paper()) // no paper
		{
			ret = 8;
			return ret;
		}
	}

	return ret;
}

static u8 print_gray_line(u8 *data, int idx, int times, u16 *gray_tab, u32 hot_time, u16 line_fix[PRINTER_W + 1], u32 moto_speed, u8 ret)
{
	int i, k;
	s16 pnum;
	u32 tm_ad = 0;
	u32 r_val = 1;
	static u32 befAdc = 0;
	u32 adcVaule = 0;

	befAdc = hal_adcGetChannel(ADC_CH_PA6);

	for (k = 0; k < PRINTER_GRAY_STEP; k++)
	{
		if (k % 4 == 0)
			btcomService();
		pnum = 0;
		for (i = 0; i < PRINTER_W; i++)
		{
			if (i % 4 == 0)
			{
				r_val = Bt_Stream_Adjtm((u32)&befAdc, i, PRINTER_W, r_val);
			}

			if (*(data + i) <= gray_tab[k])
			{
				if ((i % times) == idx)
				{
					PRINTER_DAT_HIGH();
					pnum++;
				}
				else
				{
					PRINTER_DAT_LOW();
				}
			}
			else
			{
				PRINTER_DAT_LOW();
			}
			PRINTER_CLK_HIGH();
			PRINTER_CLK_LOW();
		}
		PRINTER_LATCH_LOW();
		PRINTER_LATCH_HIGH();
		if (ret)
			ret = printer_strobe_moto_move(1, hot_time, k, line_fix[pnum], moto_speed);
		else
			ret = printer_strobe_moto_move(0, hot_time, k, line_fix[pnum], moto_speed);
	}

	return ret;
}

static u8 stream_print_gray(u8 *buf, u16 w, u16 h, u16 *gray_tab, u8 k, u32 hot_time, u16 *line_fix, s16 level, u32 tm_battery, u32 moto_speed)
{
	u8 ret = 0;
	u16 j, line_point_cnt;
	s32 tm_hot_time, all_hot_time;
	u8 *p = buf;
	int idx = 0;
	for (j = 0; j < h; j++)
	{
		btcomService();
		if (j % 3 == 0) // 每x行重新检测温度,减少浓度突变,更平滑
		{
			line_point_cnt = PRINTER_W / GRAY_PARTS_OF_LINE;
			get_temp_and_fix_time(&tm_hot_time, line_point_cnt, line_fix);

			Bt_Stream_HottimeAdj(tm_hot_time, 5, 8, 8);
#if (!DENSITY_PULS)
			all_hot_time = (PRINTER_GRAY_TIME + tm_hot_time + level /*level*27*/ + tm_battery) * 12 / 10;
#else
			all_hot_time = (PRINTER_GRAY_TIME + tm_hot_time + level /*level*27*/ + tm_battery) * 14 / 10;
#endif
			hot_time = all_hot_time;
		}
		ret = 1;
		for (idx = 0; idx < GRAY_PARTS_OF_LINE; idx++)
		{
			ret = print_gray_line(p, idx, GRAY_PARTS_OF_LINE, gray_tab, hot_time, line_fix, moto_speed, ret);
		}
		p += w;

		// wait moto move finish
		while (ret)
		{
			ret = printer_moto_move_step(1, 1, hot_time, k, 0, moto_speed);
		}

		hal_wdtClear();
		if (printer_get_paper()) // no paper
		{
			SysCtrl.paper_check = 1;
			ret = 8;
			return ret;
		}
	}
	return ret;
}

// static u16 decode_count1 =0 ;
// static u16 decode_count2 =0 ;
// static u8 Flag_stream=0;

extern u32 gBluetoothPictureHeight;
extern u32 gBluetoothGrayPrint;
extern u32 gBluetoothDotPrint;
extern u32 gBluetoothGraySegPrint;
extern u32 gBluetoothMotorFeed;
extern u32 gBluetoothDataPack;

#define ALIGNMENT 64

void *align_pointer(void *ptr, int alignment)
{
	int original_address = (int)ptr;
	int aligned_address = (original_address + alignment - 1) & ~(alignment - 1);
	return (void *)aligned_address;
}

u8 *op_outStr = NULL;
u8 *op_outStr1 = NULL;
u8 *op_outStr2 = NULL;
u8 *op_outStrAddr = NULL;
u8 *op_outStr1Addr = NULL;
u8 *op_outStr2Addr = NULL;
u8 gHeight = 0;

void open_printer_mallocSpcaec_free(void)
{

	if (op_outStrAddr != NULL)
	{
		hal_sysMemFree((u8 *)op_outStrAddr);
		op_outStrAddr = NULL;
	}
	if (op_outStr1Addr != NULL)
	{
		hal_sysMemFree((u8 *)op_outStr1Addr);
		op_outStr1Addr = NULL;
	}

	if (op_outStr2Addr != NULL)
	{
		hal_sysMemFree((u8 *)op_outStr2Addr);
		op_outStr2Addr = NULL;
	}
}
void btcomService_masf(void)
{
	btcomService();
}

// 0 解压错误、超时
// 1 取到数据

int open_printer_bluetooth_rx(void)
{
	int i, j, ret = 0;
	u8 *buff = NULL;
	u32 size;
	//	static u16 unZipLenLast = 0;
	Bt_BufManage_T *Manage;
#if PRINT_DEBUG_LOG == 1
//	deg_Printf("bluetooth_rx Start\n");
#endif
	u32 tickTime = XOSTimeGet();
	Manage = Get_btdataBufManage();

OPEN_PRINTER_BLUETOOTH_RX_RETRY:
	while (1)
	{
		ax32xx_wdtClear();
		if (get_flow_ctrl())
		{
			if (Manage->Btbuf_QueueRemain >= (18))
			{
				Bt_Flow_Ctrl(0);
				set_flow_ctrl(0);
			}
		}
		else
		{
			if (Manage->Btbuf_QueueRemain < (15))
			{
				Bt_Flow_Ctrl(1);
				set_flow_ctrl(1);
			}
		}
		btcomService();
		ret = Bt_Stream_Out(Manage, &buff, &size, &tickTime); // 取数据
		//		deg_Printf("Bt_Stream_Out ret %d,\n\n",ret);
		if (ret == 3 || ret == 4) // 队列中无数据 4:超时
		{
			ret = -1; // 返回值关系到打印提示,有需要请自行设置
			break;
		}
		else if (ret == 5)
		{
			ret = 0;
			continue;
		}
		else
		{
			tickTime = XOSTimeGet(); // 1: 取到了数据 3
			ret = 1;
			break;
		}
	}
	//	printer_delay(100);

	if (ret == 1)
	{
		//		deg_Printf("gary printe mode \n");
		if (SysCtrl.printer_print_mode == 0) // gary printe mode
		{
			u8 h1 = 0;
			u16 zipLen, unZipLen;
			u8 *str = buff;

			zipLen = (str[3] << 8) | str[2];
			unZipLen = (str[1] << 8) | str[0];

			//			deg_Printf("[P %d]:zipLen=%d,unZipLen=%d\n",zip_num,zipLen,unZipLen);
			// zip_num++;
			if ((zipLen == 0) || (unZipLen == 0))
			{
				// Bt_Stream_Free(&Manage->vid_s);
				Manage->Btbuf_QueueRemain++;
				goto OPEN_PRINTER_BLUETOOTH_RX_RETRY;
			}
			if (unZipLen)
			{
				//				if(unZipLen > unZipLenLast)
				//				{
				//					if(op_outStr)hal_sysMemFree(op_outStr);
				//					if(op_outStr1)hal_sysMemFree(op_outStr1);
				//					op_outStr = hal_sysMemMalloc(unZipLen,64);
				//					op_outStr1 = hal_sysMemMalloc(unZipLen * 2,64);
				//					unZipLenLast = unZipLen;
				//				}
			}

			//			fHandle = open(/*name*/fname, FA_WRITE | FA_READ);
			//			lseek(fHandle,fs_size(fHandle),0);
			//			write(fHandle,(void *)buff,zipLen+4);
			//			close(fHandle);
			//			char *a_arr=" op_outStr1";
			//			static u64 aaaaa=0;
			//			aaaaa++;
			//			deg_Printf("buff=%x,op_outStr=%x\n",buff,op_outStr);
			//
			//			if(zipLen>32)
			//			{
			//				deg_PrintfBuf(buff+4,16);
			//				deg_Printf("\n");
			//				deg_PrintfBuf(buff+4+zipLen-16,16);
			//			}
			//			else	deg_PrintfBuf(buff,zipLen);
			//			deg_Printf("\n");
			//			deg_Printf("buff=%x,op_outStr=%x\n",buff,op_outStr);
			ret = Bt_Stream_Decompress(Manage, (u32)buff, op_outStr, op_outStr1, &h1); // 解压缩

			//			deg_Printf("Bt_Stream_Decompress finish  ret=%d\n",ret);

			//			fHandle = open(/*name*/fname, FA_WRITE | FA_READ);
			//			lseek(fHandle,fs_size(fHandle),0);
			//			write(fHandle,(void *)a_arr,10);
			//			write(fHandle,(void *)&aaaaa,8);
			//			write(fHandle,(void *)a_arr,11);
			//			write(fHandle,(void *)op_outStr1,h1*PRINTER_W);
			//			close(fHandle);

			if (ret)
				return 0; // 解压缩失败则跳过
			gHeight = h1;
			// deg_Printf("gHeight=%d\n",gHeight);
			// ret = stream_print_gray((u8 *)outStr1,PRINTER_W,h1,gray_tab,k,all_hot_time,line_fix_time,level,tm_battery,moto_speed);
			Manage->Btbuf_QueueRemain++;
			//			deg_Printf("Manage->Btbuf_QueueRemain=%d\n",Manage->Btbuf_QueueRemain);
			return 1;
		}
		else
		{ // dot mode
			//			memset(op_outStr2,0x00,1024 * 12);
			// op_outStr2 = (u8 *)hal_sysMemMalloc(1024 * 10, 64);
			gHeight = (u16)(size / 48); // 一行原始数据是48byte
			u8 *data = buff;
			u32 pic_office = 0;
			for (i = 0; i < size; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (data[i] & (0x01 << j))
					{
						*(op_outStr2 + pic_office) = 0;
					}
					else
					{
						*(op_outStr2 + pic_office) = 255;
					}
					pic_office++;
				}
			}
			// ret=stream_print_dot((u8 *)outStr2,PRINTER_W,h1,k,all_hot_time,line_fix_time,moto_speed);
			// deg_Printf("_[%d]_[%x] %d\n",size,buff,ret);
			// Bt_Stream_Free(&Manage->vid_s);
			Manage->Btbuf_QueueRemain++;
			return 1;
		}
	}
	else
	{
		return 0;
	}
}

u8 Authorization_AppPrint(u16 w, u16 h, s16 level, u8 batteryValue)
{
	u16 line_fix_time[PRINTER_W / GRAY_PARTS_OF_LINE + 1] = {0};
	u16 line_point_cnt, i, j, k;
	u16 batteryValue_mv = getCurBatteryADC() * 19470 / 2046;
	u8 *outStr = NULL, *outStr1 = NULL, *outStr2 = NULL;
	u32 delay_time, tickTime = XOSTimeGet();
	u32 out_len = 0, tm_battery /*,tm_hot_time*/, all_hot_time, moto_speed;
	s32 tm_hot_time;
	u8 ret = 0;
	s8 Printe_CountLoadPara = 0; // reload printdensity paramate

#if 0
	open_printer_set_darkness(4+level);
	open_printer_set_paper_type(1);
	open_printer_set_contrast(1);
	open_printer_set_clarity(4);
#endif
	Bt_Flow_Ctrl(1);
	set_flow_ctrl(1);

	// init
	stream_printer_init();
	hal_gpioInit(PRINTER_PSENSOR_CH, PRINTER_PSENSOR_PIN, GPIO_INPUT, GPIO_PULL_FLOATING);
	printer_moto_enable(1);

	// 过温检测
	get_temp_time(&tm_hot_time);
	if (tm_hot_time == 0)
	{
		goto PRINTER_3_END;
	}

	if (printer_get_paper()) // no paper
	{
		SysCtrl.paper_check = 1;
		goto PRINTER_3_END;
	}
	if (hal_wki1Read())
	{
		ret = 12;
		goto PRINTER_3_END;
	}
	if (SysCtrl.printer_print_mode == 0) // gary printe mode
	{
		if (op_outStr == NULL)
		{
			op_outStrAddr = (u8 *)hal_sysMemMalloc(12 * 1024, 64);
			op_outStr = (u8 *)align_pointer(op_outStrAddr, ALIGNMENT);
		}
		if (op_outStr1 == NULL)
		{
			op_outStr1Addr = (u8 *)hal_sysMemMalloc(12 * 1024, 64);
			op_outStr1 = (u8 *)align_pointer(op_outStr1Addr, ALIGNMENT);
		}
	}
	else
	{

		if (op_outStr2 == NULL) // 扩点，1个byte变成8个byte
		{
			op_outStr2Addr = (u8 *)hal_sysMemMalloc(1024 * 12, 64);
			op_outStr2 = (u8 *)align_pointer(op_outStr2Addr, ALIGNMENT);
		}
	}

	u8 printer_level_arr[7] = {70, 80, 90, 100, 105, 110, 115}; //{90,100,110,120,125,130,135};//{70,80,90,100,105,110,115};
	u8 printer_level = 3;
	deg_Printf("SysCtrl.printer_level =%d\n", SysCtrl.printer_level);
	if (SysCtrl.printer_level <= 4605)
		printer_level = 0;
	else if (SysCtrl.printer_level <= 5418)
		printer_level = 1;
	else if (SysCtrl.printer_level <= 6375)
		printer_level = 2;
	else if (SysCtrl.printer_level <= 7500)
		printer_level = 3;
	else if (SysCtrl.printer_level <= 8625)
		printer_level = 4;
	else if (SysCtrl.printer_level <= 9918)
		printer_level = 5;
	else if (SysCtrl.printer_level <= 11406)
		printer_level = 6;
	//	open_printer_preheat(25);
	print_density_set(printer_level_arr[printer_level]);

	//	ret = open_printer_bluetooth_print(batteryValue_mv);
	//	open_printer_bluetooth_print_masf(batteryValue_mv);
	u32 paper_feed_to_cutter_line = 10 * 8;
	u32 label_feed_to_cutter_line = 14 * 8;
	ret = open_printer_bluetooth_print_masf(batteryValue_mv, paper_feed_to_cutter_line, label_feed_to_cutter_line);

PRINTER_3_END:
	SysCtrl.stream_print = 0; // 流打印置位
	Bt_Flow_Ctrl(1);		  // 清除已缓存的数据时，需要先打开流控，让app暂停发送

	open_printer_mallocSpcaec_free();
	Bt_Get_Printer_State(NULL, 0);

	// before clean data ,enable the flow ctrl

	printer_delay(2000);
	// clean ISR's Bt data
	CleanISR_Btbuf();
	// clean queue's Bt data
	Bt_Stream_Clean_Useless();

	Bt_Get_Printer_State(NULL, 0);
	// SysCtrl.stream_print = 0;	//流打印置位
	Bt_Stream_HottimeRest();

	// printer power down
	printer_moto_enable(0);

	// reinit bat
	hal_adcInit();
	hal_gpioInit(MOTO_AIN1_CH, MOTO_AIN1_PIN, GPIO_INPUT, GPIO_PULL_FLOATING);
	ax32xx_adcEnable(ADC_CH_PE0, 1);
// reinit uart
#if HAL_CFG_EN_DBG
	hal_uartInit();
#endif
	deg_Printf("--%s finish\n", __func__);
	deg_Printf("T-T-T show ret[%d] delaytime[%d]\n", ret, delay_time);
	deg_Printf("all_hot_time[%d] level[%d] \n", all_hot_time, level);
	deg_Printf("tm_battery=%d,tm_hot_time=%d,gBluetoothPictureHeight=%d \n", tm_battery, tm_hot_time, gBluetoothPictureHeight); // h = 384
	// deg_Printf("decode time 1:%d 2:%d\n",decode_count1,decode_count2);
	//  close(fHandle);

	deg_Printf("gBluetoothPictureHeight=%d\ngBluetoothGrayPrint=%d\ngBluetoothDotPrint=%d\ngBluetoothGraySegPrint=%d\ngBluetoothMotorFeed=%d \ngBluetoothDataPack=%d\n",
			   gBluetoothPictureHeight, gBluetoothGrayPrint, gBluetoothDotPrint, gBluetoothGraySegPrint, gBluetoothMotorFeed, gBluetoothDataPack); // h = 384
	dispLayerSetPIPMode(DISP_PIP_FRONT);
	Bt_Stream_setADtm(0);
	return ret;
}

u8 unAuthorized_AppPrint(u16 w, u16 h, s16 level, u8 batteryValue)
{
	u16 line_fix_time[PRINTER_W / GRAY_PARTS_OF_LINE + 1] = {0};
	u16 line_point_cnt, i, j, k;
	u8 *outStr = NULL, *outStr1 = NULL, *outStr2 = NULL;
	u32 delay_time, tickTime = XOSTimeGet();
	u32 out_len = 0, tm_battery /*,tm_hot_time*/, all_hot_time, moto_speed, oldmoto_speed;
	s32 tm_hot_time;
	u8 ret = 0;
	s8 Printe_CountLoadPara = 0; // reload printdensity paramate
#define PRINTER_GRAY_TIME 48

	u16 gray_tab[PRINTER_GRAY_STEP];
	s16 gray_step = 256 / PRINTER_GRAY_STEP;
#if 1
	for (k = 0; k < PRINTER_GRAY_STEP; k++)
	{
		gray_tab[k] = k * gray_step;
	}
#else
	u8 val = PRINTER_GRAY_STEP / 4;
	u8 tol = 0;
	u8 step1 = 8, step2 = 6, step3 = 6, step4 = 12;
	for (k = 0; k < val; k++) // 0~9
	{
		gray_tab[k] = (k + 1) * step1;
	}
	tol = val * step1;				//
	for (k = val; k < val * 2; k++) // 10~PRINTER_GRAY_STEP-10
	{
		gray_tab[k] = tol + (k - val + 1) * step2; //
	}
	tol = tol + val * step2;			//
	for (k = val * 2; k < val * 3; k++) //
	{
		gray_tab[k] = tol + (k - val * 2 + 1) * step3; //
	}
	tol = tol + val * step3;					  //
	for (k = val * 3; k < PRINTER_GRAY_STEP; k++) // PRINTER_GRAY_STEP-10~PRINTER_GRAY_STEP
	{
		gray_tab[k] = tol + (k - val * 3 + 1) * step4; //
	}
#endif
	deg_Printf("in func:%s.\n", __func__);

	moto_speed = oldmoto_speed = Bt_Get_Printer_Moto_Speed();
	Bt_Stream_setADtm(hal_adcGetChannel(ADC_CH_PA6));

	// init
	stream_printer_init();
	hal_gpioInit(PRINTER_PSENSOR_CH, PRINTER_PSENSOR_PIN, GPIO_INPUT, GPIO_PULL_FLOATING);
	printer_moto_enable(1);

	// 过温检测
	get_temp_time(&tm_hot_time);
	if (tm_hot_time == 0)
	{
		deg_Printf("tm_hot_time GO.stop_print:%d\n", SysCtrl.stop_print);
		goto UNARI_APP_END;
	}
	if (printer_get_paper()) // no paper
	{
		deg_Printf("paper GO\n");
		SysCtrl.paper_check = 1;
		goto UNARI_APP_END;
	}
	if (hal_wki1Read())
	{
		deg_Printf("wki GO\n");
		ret = 12;
		goto UNARI_APP_END;
	}
	// hot time across to battery
	tm_battery = get_battery_time(batteryValue);

	// moto move
	printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
	printer_delay((delay_time + all_hot_time + 50) * PRINTER_GRAY_STEP);
	printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
	printer_delay((delay_time + all_hot_time + 50) * PRINTER_GRAY_STEP);

	if (SysCtrl.printer_print_mode == 0)
	{
		level = (level / 40) - 100;
	}
	else
	{
		level = (level / 70);
		if (level > 130)
			level = level * 7 / 10;
		else
			level = level * 6 / 10;
	}
	// level = 0;
	deg_Printf("S show level:%d aht[%d]\n", level, (PRINTER_GRAY_TIME + tm_hot_time + level /*level*27*/ + tm_battery) * 12 / 10);
	// SysCtrl.paper_check=1;
	// goto UNARI_APP_END;
	if (SysCtrl.printer_print_mode == 0) // gary printe mode
	{
		// deg_Printf("before  op_outStr=0x%x,op_outStr1=0x%x\n",op_outStr,op_outStr1);
		if (op_outStr == NULL)
		{
			op_outStrAddr = (u8 *)hal_sysMemMalloc(12 * 1024, 64);
			// op_outStr  = (u8*)((u32)op_outStrAddr+0x3f)&(~0x3f);
			op_outStr = (u8 *)align_pointer(op_outStrAddr, ALIGNMENT);
		}
		if (op_outStr1 == NULL)
		{
			op_outStr1Addr = (u8 *)hal_sysMemMalloc(12 * 1024, 64);
			// op_outStr1 = (op_outStr1Addr+0x3f)&(~0x3f);
			op_outStr1 = (u8 *)align_pointer(op_outStr1Addr, ALIGNMENT);
		}
		// deg_Printf("after op_outStrAddr=0x%x,op_outStr1Addr=0x%x\n",op_outStrAddr,op_outStr1Addr);
		// deg_Printf("after op_outStr=0x%x,op_outStr1=0x%x\n",op_outStr,op_outStr1);
	}
	else
	{

		// deg_Printf("before	op_outStr2=0x%x\n",op_outStr2);
		if (op_outStr2 == NULL) // 扩点，1个byte变成8个byte
		{
			op_outStr2Addr = (u8 *)hal_sysMemMalloc(1024 * 12, 64);
			op_outStr2 = (u8 *)align_pointer(op_outStr2Addr, ALIGNMENT);
		}

		// deg_Printf("after op_outStr2Addr=0x%x\n",op_outStr2Addr);
		// deg_Printf("after op_outStr2=0x%x\n",op_outStr2);
	}

	// deg_Printf("after op_outStr=0x%x,op_outStr1=0x%x\n",op_outStr,op_outStr1);

	// stream print
	do
	{
		u32 buff, size;
		s32 syn_cnt, syn_cnt_next, addr;
		Bt_BufManage_T *Manage;
		ax32xx_wdtClear();
		// 设置参数
		if (SysCtrl.printer_print_mode == 0)
		{ // gray mode

			// 	//moto_speed = Bt_Get_Printer_Moto_Speed();
			// 	line_point_cnt=PRINTER_W/GRAY_PARTS_OF_LINE;
			// 	get_temp_and_fix_time(&tm_hot_time, line_point_cnt, line_fix_time);

			// Bt_Stream_HottimeAdj(tm_hot_time,3,5,5);

			// all_hot_time = (PRINTER_GRAY_TIME+tm_hot_time+level/*level*27*/+tm_battery)*9/10;	//there are 7 levels , but max is same
		}
		else
		{ // dot mode
			line_point_cnt = PRINTER_W / DOT_PARTS_OF_LINE;
			get_dot_fix_time(&tm_hot_time, line_point_cnt, line_fix_time);

#if (!DENSITY_PULS)
			all_hot_time = (PRINTER_GRAY_TIME + 80 + tm_hot_time * 2 + level /*level*15*/ + tm_battery) * 16 / 10;
#else
			all_hot_time = (PRINTER_GRAY_TIME + 80 + tm_hot_time * 2 + level /*level*15*/ + tm_battery) * 18 / 10; // there are 7 levels , but max is same
#endif

			if (all_hot_time >= (PRINTER_GRAY_TIME + 80 + tm_hot_time * 2 + level /*7*15*/ + tm_battery) * 18 / 10)
				delay_time = 0;
			else
				delay_time = (PRINTER_GRAY_TIME + 80 + tm_hot_time * 2 + level /*7*15*/ + tm_battery) * 18 / 10 - all_hot_time;
		}

		if (tm_hot_time == 0) // 到达报警温度85C,停止打印并通知app停止发送数据
		{
			deg_Printf("tm_hot_time2 GO\n");
			goto UNARI_APP_END;
		}
		// 取流
		Manage = Get_btdataBufManage();

		// 流控
		if (get_flow_ctrl()) // 若流控已启动,待空间资源足够时,解除流控,继续接收数据
		{
			if (Manage->Btbuf_QueueRemain >= (BT_BUFFER_NUM * 7 / 10))
			{
				// if(get_remainFreme() > (NUM_OF_FRAME*8/10))
				{
					Bt_Flow_Ctrl(0);
					set_flow_ctrl(0);
				}
			}
		}

		btcomService();
		// 取数据
		ret = Bt_Stream_Out(Manage, &buff, &size, &tickTime);
		if (ret == 3 || ret == 4)
		{			 // 1: 取到了数据 3:队列中无数据 ; 4:超时 5: 轮询中
			ret = 0; // 返回值关系到打印提示,有需要请自行设置
			break;
		}
		else if (ret == 5)
		{
			if (oldmoto_speed == moto_speed)
				moto_speed = moto_speed + 10;
			if (moto_speed > 100)
				moto_speed = 100;
			continue;
		}
		else
			tickTime = XOSTimeGet();

		oldmoto_speed = moto_speed;

		if (SysCtrl.printer_print_mode == 0) // gary printe mode
		{
			u8 h1 = 0;
			u16 zipLen, unZipLen;
			u8 *str = buff;
			zipLen = (str[3] << 8) | str[2];
			unZipLen = (str[1] << 8) | str[0];
			// deg_Printf("the len == %d,%d\n",zipLen,unZipLen);

			if ((!unZipLen) || (!zipLen))
			{ // 遇到空数据,跳过
				deg_Printf("unZipLen == 0\n");
				Manage->Btbuf_QueueRemain++;
				continue;
			}

			// 解压缩
			ret = Bt_Stream_Decompress(Manage, buff, op_outStr, op_outStr1, &h1);
			if (ret)
				continue; // 解压缩失败则跳过

			ret = stream_print_gray((u8 *)op_outStr1, PRINTER_W, h1, gray_tab, k, all_hot_time, line_fix_time, level, tm_battery, moto_speed);
			Manage->Btbuf_QueueRemain++;
			// deg_Printf("aht[%d]-s[%d]-t[%d] ad[%d]\n",all_hot_time,moto_speed,printer_get_tm(),hal_adcGetChannel(ADC_CH_PA6));
		}
		else
		{ // dot mode

			// 扩点，1个byte变成8个byte

			u8 h1 = size / 48; // 一行原始数据是48byte
			u8 *data = buff;
			u32 pic_office = 0;
			for (i = 0; i < size; i++)
			{
				for (j = 0; j < 8; j++)
				{
					if (data[i] & (0x01 << j))
					{
						*(op_outStr2 + pic_office) = 1;
					}
					else
					{
						*(op_outStr2 + pic_office) = 0;
					}
					pic_office++;
					// btcomService();
				}
			}
			ret = stream_print_dot((u8 *)op_outStr2, PRINTER_W, h1, k, all_hot_time, line_fix_time, moto_speed);
			deg_Printf("_[%d]_[%x] %d\n", size, buff, ret);
			Manage->Btbuf_QueueRemain++;
		}
		if (ret)
			goto UNARI_APP_END;

	} while (1);
	// moto move
	for (i = 0; i < 70; i++)
	{
		printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
		printer_delay(1500);
		printer_moto_move_step(1, 0, 0, 0, 0, moto_speed);
		printer_delay(1500);
	}

UNARI_APP_END:
	SysCtrl.stream_print = 0; // 流打印置位

	open_printer_mallocSpcaec_free();
	Bt_Get_Printer_State(NULL, 0);
	// before clean data ,enable the flow ctrl
	Bt_Flow_Ctrl(1); // 清除已缓存的数据时，需要先打开流控，让app暂停发送

	printer_delay(2000);
	// clean ISR's Bt data
	CleanISR_Btbuf();
	// clean queue's Bt data
	Bt_Stream_Clean_Useless();

	Bt_Stream_HottimeRest();

	// printer power down
	printer_moto_enable(0);

	// reinit bat
	hal_adcInit();
	hal_gpioInit(MOTO_AIN1_CH, MOTO_AIN1_PIN, GPIO_INPUT, GPIO_PULL_FLOATING);
	ax32xx_adcEnable(ADC_CH_PE0, 1);
// reinit uart
#if HAL_CFG_EN_DBG
	hal_uartInit();
#endif

	deg_Printf("--%s finish\n", __func__);
	deg_Printf("T-T-T show ret[%d] delaytime[%d]\n", ret, delay_time);
	deg_Printf("all_hot_time[%d] level[%d] \n", all_hot_time, level);
	deg_Printf("tm_battery=%d,tm_hot_time=%d \n", tm_battery, tm_hot_time); // h = 384
	// deg_Printf("decode time 1:%d 2:%d\n",decode_count1,decode_count2);
	//  close(fHandle);

	if (outStr)
	{
		hal_sysMemFree(outStr);
	}
	if (outStr1)
	{
		hal_sysMemFree(outStr1);
	}
	if (outStr2)
	{
		hal_sysMemFree(outStr2);
	}
	dispLayerSetPIPMode(DISP_PIP_FRONT);
	Bt_Stream_setADtm(0);
	return ret;
}

static u8 stream_printer_run(u16 w, u16 h, s16 level, u8 batteryValue)
{
	u8 ret = 0;
	deg_Printf("Local level = %d\n", level);
	if (SysCtrl.credit_flag) // 已验证
	{
		level = level / 1000 * 3;
		// deg_Printf("APP change:%d\n",level);
		ret = Authorization_AppPrint(w, h, level, batteryValue);
	}
	else // 未验证
	{
		ret = unAuthorized_AppPrint(w, h, level, batteryValue);
	}

	return ret;
}
