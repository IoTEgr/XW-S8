/******************** (C) COPYRIGHT 2013 BUILDWIN ************************
* File Name          : uart.c
* Author             : pengtao_zhang
* Version            : V0719a
* Date               : 07/19/2013 ;mm/dd/yyyy
* Description        : This file provides all the uart firmware functions.
***************************************************************************/
//#include "../../mcu/mcu_include.h"
#include "../inc/hal.h"
#include "hal_usb_dev_enum.h"
#include "hal_usb_msc.h"
#include "../../device/cmos_sensor/cmos_sensor.h"

//bool (*fp_rbc_process)(void);
//bool (*fp_usb_bulk_tx)(u8 epx, u32 adr, u16 len);
//bool (*fp_usb_bulk_rx)(u8 epx, u32 adr, u16 len);

const u8 device_inquiry_data[36] =
{
	0x00,	// Peripheral Device Type: direct access devices
	0x80,	// Removable: UFD is removable
	0x04,	// ANSI version
	0x02, 	// Response Data Format: compliance with UFI  
	0x1f,	// Additional Length (Number of unsigned chars following this one): 31, totally 36 unsigned chars
	0x00, 0x00, 0x00,	// reserved
	'B', 'u', 'i', 'l', 'd', 'w', 'i', 'n', 'M', 'e', 'd', 'i', 'a', '-',				//BuildwinMedia-Player  can't be change , updata tools need it
	'P', 'l', 'a', 'y', 'e', 'r', ' ', ' ', ' ', ' ', '1', '.', '0', '0'				//len full

};
#define scsi_bulk_clr_rx()		USB20_OutCSR1 = BIT(4);           //Flush FIFO, Clear RxRdy//USB20_OutCSR1 = 0;
#define SENSE_ERROR_CODE		0x70
#define	REQUESTDATA_SIZE		0x12
#define INQUIRY_SIZE  sizeof(device_inquiry_data)
#define FORMTCAPACITY_SIZE		0x0C
#define DISKCAPACITY_SIZE		0x08
#define SECTOR_SIZE				0x200
#define MODE_SENSE10_LEN	0x08


typedef  struct _SENSE_CODE {
	unsigned char	SenseKey;
	unsigned char	ASC;
//	unsigned char	ASCQ;
} SENSE_CODE;

const u8  MscSenseCode[][2] = {
	/*NO_SENSE			*/ 			{0x00, 0x00},
	/*INVALID_FIELD_IN_COMMAND	*/ 	{0x05, 0x24},
	/*NEW_MEDIUM_ARRIVEL		*/ 	{0x06, 0x28},
	/*WRITE_PROTECTED		*/ 		{0x07, 0x27},
	/*MEDIUM_NOT_PRESENT		*/ 	{0x02, 0x3A},
	/*DATA_PHASE_ERROR  */			{0x04, 0x4b},
};

typedef struct {
//	unsigned long		Signature;
	u32		CbwTag;
	u32		CbwTrxLength;
	u32		CbwFlag;
	u32		CbwLun;
//	unsigned char		CbwCbLen;

	u32		OpCode;
	u32		SubOpCode;
	u32		Address;
	u32		SubEx;
	u32		Length;
	u32		Residue;
    u32		SubEx1;
    u32		SubEx2;
    u32		SubEx3;
    u32     Func1;
    u32     DataAddr;
    u32     Func2;
    u32     Param;
	//unsigned long		TrxLen;
//	unsigned char		Page;
} MSC_CMD;

typedef struct
{
	u8 pc_move;
	u8 epxin;
	u8 epxout;
	u8 uspeed;
	
	u8 bstall;
	u8 sense;
	u8 cswsta;
	u8 *pcsw;
	u8 *pcbw;
	u8 *ptxbuf;
	u8 *prxbuf;
}SCSI;

//----------------------------------------------------------------------------------------------------
static const u16 crc16table[256] = 
{  
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,  
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,  
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,  
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,  
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,  
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,  
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,  
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,  
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,  
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,  
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,  
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,  
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,  
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,  
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,  
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,  
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,  
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,  
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,  
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,  
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,  
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,  
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,  
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,  
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,  
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,  
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,  
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,  
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,  
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,  
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,  
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78  
};
//----------------------------------------------------------------------------------------------------
u16 std_fcs16(register void *pData,register u32 Len)
{
	u16 Crc = 0xFFFF;
	while(Len-- > 0)
	{
		Crc = (Crc >> 8) ^ crc16table[(Crc ^ (*(u8*)pData)) & 0xff];
		pData = (u8*)pData + 1;
	}
	return Crc;
}

MSC_CMD MscCmd;
SCSI scsi; 

//u8 msc_txbuf[512+16] __attribute__ ((section("._msc_txbuf")));;
//u8 msc_rxbuf[512+16] __attribute__ ((section("._msc_rxbuf")));;

bool (*fp_usb_bulk_tx)(u8 epx, u32 adr, u16 len);
bool (*fp_usb_bulk_rx)(u8 epx, u32 adr, u16 len);
s32 (*fp_disk_rd)(void *pDataBuf, u32 dwLBA, u32 dwLBANum);
s32 (*fp_disk_wr)(void *pDataBuf, u32 dwLBA, u32 dwLBANum);
s32 (*fp_disk_online)(void);
u32 (*fp_disk_cap)();
void (*fp_disk_free)(void);
void (*fp_disk_busy)(void);
s32 (*fp_disk_stop)(void);
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void msc_epx_cfg(void)
{
	USB20_Index = __MSC_EPX__;		
	USB20_InMaxPl = 0;
	USB20_InMaxPh = 0x04;
	USB20_OutMaxPl = 0;
	USB20_OutMaxPh = 0x04;
	
	USB20_InCSR2 = 0;
	USB20_InCSR1 |= BIT(6) | BIT(3);
	USB20_OutCSR2 = 0;
	USB20_OutCSR1 |= BIT(7) | BIT(4);
	
	scsi.ptxbuf = _msc_txfifo_; //480
	scsi.prxbuf = _msc_rxfifo_; //280
	scsi.pcsw = _msc_txfifo_;
	scsi.pcbw = _msc_rxfifo_;
	scsi.epxin = __MSC_EPIN__;        
	scsi.epxout = __MSC_EPOUT__;        
	__MSCTADR__ = (u32)scsi.ptxbuf ;
    __MSCRADR__ = (u32)scsi.prxbuf ;

	scsi.pc_move = false; 

}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		

* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
void ScsiStallAll(void)
{
	usb_dev.ep1_pipe_stall = 1;
	usb_stall_ep(__MSC_EPOUT__);
	usb_stall_ep(__MSC_EPIN__);
	
	while(usb_dev.ep1_pipe_stall) {
        if(usb_dev.ep1_rx_stall == 0) {
            usb_stall_ep(__MSC_EPOUT__);
		}
        if(usb_dev.ep1_tx_stall == 0) {
    		usb_stall_ep(__MSC_EPIN__);
		}
    }
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		

* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
bool ScsiStallIn(u8 stall)
{
	volatile s32 i;
	
    if (stall) 
	{
		//debg("- stall in\n");
        usb_stall_ep(__MSC_EPIN__);
		//tmout_reset();
		i=0;
        while (usb_dev.ep1_tx_stall)
		{
			i++;
			if(i>=20000)
			//if(tmout_occur(2000))
			{
				debg("- stall in to\n");
				break;
			}		
		}
			
    }
	return true;

}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		

* @details
* - 该函数怎么用
* @note
* 该函数使用过程中注意什么
* @attention
* 该函数使用过程中特别注意的事项
*/
/*-----------------------------------------------------------------------------------*/
bool ScsiStallout(stall)
{

    if (stall) {
		debg("- stall out\n");
        usb_stall_ep(__MSC_EPOUT__);
		//tmout_reset();
        while (usb_dev.ep1_rx_stall){
			/*if(tmout_occur(2000)){
				break;
			}*/
		}
    }
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/

u8 dev_online(u8 lun)
{
	return (*fp_disk_online)();

}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
u32 dev_cap(u8 lun)
{
	return (*fp_disk_cap)();
}

void dev_stop(void)
{
	(*fp_disk_stop)();
}


/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void dev_busy(void)
{
	(*fp_disk_busy)();
}


/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void dev_free(void)
{
	(*fp_disk_free)();
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void mscSet_Status(u8 status)
{
	scsi.sense = status;
	if(status){
		scsi.cswsta = 1;	
	}
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
void Host_In_Data(u8 *ptxbuf, u32 len)
{
	u8 eptx = scsi.epxin;
	
	if (!scsi.bstall) {
		if (MscCmd.CbwTrxLength > len) {
			MscCmd.Residue = MscCmd.CbwTrxLength - len;
			(*fp_usb_bulk_tx)(eptx,(u32)ptxbuf,len);
			scsi.bstall = 1;
			
		} else {
			(*fp_usb_bulk_tx)(eptx,(u32)ptxbuf,MscCmd.CbwTrxLength);
		}
		mscSet_Status(NO_SENSE);
	}	
	else {
		MscCmd.Residue = MscCmd.CbwTrxLength;
	}
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_ReadCapacity(void)
{
	u32 cap;
	u8 *ptxbuf = scsi.ptxbuf;
	
	if(!dev_online(MscCmd.CbwLun))
	{
		mscSet_Status(MEDIUM_NOT_PRESENT);
		scsi.bstall = 1;	
	}
	
	if(!(cap = dev_cap(MscCmd.CbwLun)))
	{
		mscSet_Status(MEDIUM_NOT_PRESENT);
		scsi.bstall = 1;	
	}	
	
	cap -= 1;
	ptxbuf[0] = (u8)(cap >> 24);
	ptxbuf[1] = (u8)(cap >> 16);
	ptxbuf[2] = (u8)(cap >> 8);
	ptxbuf[3] = (u8)(cap >> 0);

	ptxbuf[4] = 0;
	ptxbuf[5] = 0;
	ptxbuf[6] = 0x02;
	ptxbuf[7] = 0x00;
	
	Host_In_Data(ptxbuf,8);
	
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_ReadFormatCapacity(void)
{
	u32 cap;
	u8 *ptxbuf = scsi.ptxbuf;

	if(!dev_online(MscCmd.CbwLun)){
		mscSet_Status(MEDIUM_NOT_PRESENT);
		scsi.bstall = 1;	
	}
	
	if(!(cap = dev_cap(MscCmd.CbwLun))){
		mscSet_Status(MEDIUM_NOT_PRESENT);
		scsi.bstall = 1;	
	}	
	cap -= 1;
	
	ptxbuf[0] = 0x00;
	ptxbuf[1] = 0x00;
	ptxbuf[2] = 0x00;
	ptxbuf[3] = 0x08;	

	ptxbuf[4] = (u8)(cap >> 24);
	ptxbuf[5] = (u8)(cap >> 16);
	ptxbuf[6] = (u8)(cap >> 8);
	ptxbuf[7] = (u8)(cap >> 0);
	
	ptxbuf[8] = 0x02;
	ptxbuf[9] = 0x00;
	ptxbuf[10] = 0x02;
	ptxbuf[11] = 0x00;
	
	//debg("-bstall:%x\n",scsi.bstall);
	Host_In_Data(ptxbuf,12);

	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_RequestSense(void)
{
	u8 *ptxbuf = scsi.ptxbuf;
	u8 * pSenseCode = (u8*)&MscSenseCode[scsi.sense];
	
	memset(ptxbuf, 0, REQUESTDATA_SIZE);
	
	ptxbuf[0] = SENSE_ERROR_CODE;		// error code
	ptxbuf[2] = pSenseCode[0];			// sense key
	ptxbuf[7] = REQUESTDATA_SIZE - 8;	// Additional Sense data length
	ptxbuf[12] = pSenseCode[1];			// Additional Sense Code
	ptxbuf[13] = 0;  					//MscStatusCode.ASCQ;Additional Sense Code Qualifier
	
	Host_In_Data(ptxbuf,REQUESTDATA_SIZE);

	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_Inquiry(void)
{
	
	u8 *ptxbuf = scsi.ptxbuf;
	memcpy(ptxbuf, (u8*)device_inquiry_data, INQUIRY_SIZE);
	Host_In_Data(ptxbuf,INQUIRY_SIZE);
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_ModeSense6(void)
{
	u8 *ptxbuf = scsi.ptxbuf;
	
	u32 dwTxdata;
	
	if(!dev_online(MscCmd.CbwLun)){
		mscSet_Status(MEDIUM_NOT_PRESENT);
		scsi.bstall = 1;	
	}
	
	dwTxdata = 0x03000000L;
	
	/*
	if(dev_wp(MscCmd.CbwLun)) {
		dwTxdata |= 0x8000;
	}
	 */
	ptxbuf[0] = (u8)(dwTxdata >> 24);
	ptxbuf[1] = (u8)(dwTxdata >> 16);
	ptxbuf[2] = (u8)(dwTxdata >> 8);
	ptxbuf[3] = (u8)(dwTxdata >> 0);
	
	Host_In_Data(ptxbuf,4);
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_TestUnitReady(void)
{
	//ledsta = IDLE;
	
	dev_free();
	if(!dev_online(MscCmd.CbwLun))
	{
		mscSet_Status(MEDIUM_NOT_PRESENT);
	}
	else
	{
		mscSet_Status(NO_SENSE);			
	}
	
	if(scsi.pc_move)
	{
		mscSet_Status(MEDIUM_NOT_PRESENT);	
	}
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_StartStopUnit(void)
{
	if(0x0200 == MscCmd.Address)
	{
		//usb stop
		dev_stop();
		scsi.pc_move = true;
		debg("dev moved...\n");
	}
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_PreventAllowMediumRemoval(void)
{
	if((MscCmd.Address >> 8) == 0x01){
		mscSet_Status(INVALID_FIELD_IN_COMMAND);	
	}
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_Verify(void)
{
	//ledsta = RECORD;
	//LED闪灯
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
typedef void (*L1_func)(uint32_t *p1,uint32_t *p2);
typedef void (*l2_func) (uint32_t, uint32_t, uint32_t);

void cb_FIFO2mem (uint8_t *buf, uint32_t len) {
	uint32_t to;
	do {
		// 計算長度
		to = (len>512)?512:len;
        usb_bulk_rx(scsi.epxout,(u32)scsi.prxbuf,to);
        memcpy(buf,scsi.prxbuf,to);
        buf += to;
        __MSCRADR__ = (u32)scsi.prxbuf; //复位地址
		len -= to;
        
	} while (len);
   USB20_OutCSR1 = BIT(4);           //Flush FIFO, Clear RxRdy(接收下一个包)
}

void cb_mem2FIFO (uint8_t *buf, uint32_t len) {
	uint32_t to;
	do {
		// 計算長度
		to = (len>512)?512:len;
        memcpy(scsi.ptxbuf,buf,to);
        buf += to;
        usb_bulk_tx(scsi.epxout,(u32)scsi.ptxbuf,to);
		len -= to;
	} while (len);
}

void cb_mem_read (void) {

    if(0xffffffff == MscCmd.DataAddr) MscCmd.DataAddr = (uint32)scsi.ptxbuf;
	// 先L2_func再讀內存
	if (MscCmd.Func2 + 1) {
		l2_func L2_func = (l2_func)MscCmd.Func2;
        L2_func (MscCmd.DataAddr, MscCmd.Residue, MscCmd.Param);
	}
    cb_mem2FIFO ((uint8_t *)MscCmd.DataAddr, MscCmd.Residue);
    //MscCmd.Residue = 0;  
}
          
void cb_mem_write (void) {

    //debg(__func__);
    //debg("\r\nMscCmd.DataAddr = %x",MscCmd.DataAddr);
    //debg("\r\nMscCmd.Residue = %x",MscCmd.Residue);
    //debg("\r\nMscCmd.Func2  = %x",MscCmd.Func2 );
    if(0xffffffff == MscCmd.DataAddr) MscCmd.DataAddr = (uint32)scsi.prxbuf;

    cb_FIFO2mem ((uint8_t *)MscCmd.DataAddr, MscCmd.Residue);
    
	if (MscCmd.Func2 + 1) {
        //debg("\r\nMscCmd.Func2  = %x",MscCmd.Func2 );
		l2_func L2_func = (l2_func)MscCmd.Func2;
        L2_func (MscCmd.DataAddr, MscCmd.Residue, MscCmd.Param);
	}
	//MscCmd.Residue = 0;
}

void cb_mem_rwex (uint32_t cpu) {
    //debg(__func__);
	if (0x80 == MscCmd.CbwFlag)	// 傳輸方向
		cb_mem_read ();
	else
		cb_mem_write ();
}

void mscCmd_ufmod(void)
{

    //debg(__func__);
    //debg("\r\nMscCmd.Func1 = %x",MscCmd.Func1);
    MscCmd.Residue = MscCmd.CbwTrxLength;
    L1_func l1_func = (L1_func)MscCmd.Func1;
    l1_func((u32 *)&MscCmd,(u32 *)&scsi);
    MscCmd.Residue = 0;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/

bool sent_csw(void)
{
	u8 eptx = scsi.epxin;
	u8 *ptxbuf = scsi.pcsw;
	//u32 dwTxdata;
	
	//USB_BulkInWait_Rdy();
	
	ptxbuf[0] = 0x55;
	ptxbuf[1] = 0x53;
	ptxbuf[2] = 0x42;
	ptxbuf[3] = 0x53;
	
	ptxbuf[4] = (u8)(MscCmd.CbwTag >> 0);
	ptxbuf[5] = (u8)(MscCmd.CbwTag >> 8);
	ptxbuf[6] = (u8)(MscCmd.CbwTag >> 16);
	ptxbuf[7] = (u8)(MscCmd.CbwTag >> 24);

	ptxbuf[8] =  (u8)(MscCmd.Residue >> 0);
	ptxbuf[9] =  (u8)(MscCmd.Residue >> 8);
	ptxbuf[10] = (u8)(MscCmd.Residue >> 16);
	ptxbuf[11] = (u8)(MscCmd.Residue >> 24);

	ptxbuf[12] = scsi.cswsta ? 1 : 0;	
	(*fp_usb_bulk_tx)(eptx, (u32)ptxbuf, 13);
	
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool mscCmd_Read(void)
{
	//debg("mscCmd_Read\n");
	u8 *ptxbuf = scsi.ptxbuf;
	u32 lba = MscCmd.Address;
	u32 sec = MscCmd.Length ;
	MscCmd.Residue = MscCmd.CbwTrxLength;
	
	//设备在线？
	if(!dev_online(MscCmd.CbwLun)){
		mscSet_Status(MEDIUM_NOT_PRESENT);
		scsi.bstall = 1;
		return false;
	}
	dev_busy();
	//debg("r-");
	//ledsta = RECORD;
	do{

		if((*fp_disk_rd)(_msc_txbuf_,lba,1)<0)
		{
			break;	
		}
		ax32xx_mcpy_gram2ufifo(ptxbuf,_msc_txbuf_,512);
		if(!(*fp_usb_bulk_tx)(scsi.epxin,(u32)ptxbuf,512)){
			break;	
		}
		
		
		MscCmd.Residue -= 512;
		lba++;
	}while(--sec);
	
	return true;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
extern volatile u8 test;
bool mscCmd_Write(void)
{
	//debg("mscCmd_Write\n");
	u8 *prxbuf = scsi.prxbuf;
	u32 lba = MscCmd.Address;
	u32 sec = MscCmd.Length ;
	MscCmd.Residue = MscCmd.CbwTrxLength;
	
	//设备在线？
	if(!dev_online(MscCmd.CbwLun)){
		mscSet_Status(MEDIUM_NOT_PRESENT);
		scsi.bstall = 1;
		return false;
	}
	
	dev_busy();	
	//ledsta = RECORD;
	do
	{	
		(*fp_usb_bulk_rx)(scsi.epxout,(u32)prxbuf,512);
		ax32xx_mcpy_gram2ufifo(_msc_rxbuf_,prxbuf,512);
		if((*fp_disk_wr)(_msc_rxbuf_,lba,1)<0)
		{
			__MSCRADR__ = (u32)scsi.prxbuf; //复位地址
			break;	
		}
		__MSCRADR__ = (u32)scsi.prxbuf; //复位地址
		//debg("lba:%x\n",lba);
		MscCmd.Residue -= 512;
		lba++;
	}while(--sec);	

	USB20_OutCSR1 = BIT(4);           //Flush FIFO, Clear RxRdy(接收下一个包)
	return  true;
}
extern void XOSTimeDly(INT32U dely);
void cbw_update(void)
{	
	ax32xx_wdtEnable(0);//return mask close WDT
	MscCmd.Residue = 0;
	sent_csw();
	//deg_Printf("usb update\n");//while(1);
/*
	hal_auadcStop();
	hal_dacPlayStop();
	ax32xx_csiEnable(0);*/
/*	
	void auadc_Stop(void);
	void audac_Stop(void);
	audac_Stop();
	auadc_Stop();
	csi_Stop();
 */   
	
    USB20CON0 &= ~0x1;	//release sie reset
	USB20CON0 &= ~0x4;	//release usb ph reset
    USB20CON0 &= ~(BIT(18)|BIT(19));

	USB11CON1 |= (1<<3);                      //RESET	
	while((USB11CON1 &BIT(3))==0); //wait for usb1.1 reset release
	
	XOSTimeDly(500); 
	XOSTimeDly(500); 

	__LGIE_DIS__();
	__HGIE_DIS__();

	ax32xx_intEnable(IRQ_CSI,0);
	ax32xx_intEnable(IRQ_AUADC,0);	
	ax32xx_intEnable(IRQ_JPGB,0);	
	ax32xx_intEnable(IRQ_UART0,0);	
	ax32xx_intEnable(IRQ_LCDC,0);	//
	ax32xx_intEnable(IRQ_DE,0);	    //
	ax32xx_intEnable(IRQ_DAC,0);	
	ax32xx_intEnable(IRQ_TIMER0,0);	
	ax32xx_intEnable(IRQ_TIMER1,0);	
	ax32xx_intEnable(IRQ_TIMER2,0);	
	ax32xx_intEnable(IRQ_TIMER3,0);	
	ax32xx_intEnable(IRQ_GPIO,0);	
	ax32xx_intEnable(IRQ_USB20,0);	
	ax32xx_intEnable(IRQ_USB11,0);	
	ax32xx_intEnable(IRQ_RTC_WDT,0);	

	//lcd back light off
	//hal_gpioEPullSet(GPIO_PA,GPIO_PIN8,GPIO_PULLE_DOWN);
	//hal_gpioWrite(GPIO_PA,GPIO_PIN8,GPIO_LOW);

	//__intvct_adr__(0x100000);
    __intvct_mask();
	//void dcache_close(); 
    //dcache_close();
}

static void cbw_connect_write_sensor_i2c_reg(void)
{
//	u32 i2c_addr, i2c_value;
	u8 temp_buf[4];
	u8 *prxbuf = scsi.pcbw;
//	i2c_addr = (prxbuf[17]<<24)|(prxbuf[18]<<16)|(prxbuf[19]<<8)|(prxbuf[20]);
//	i2c_value = (prxbuf[21]<<24)|(prxbuf[22]<<16)|(prxbuf[23]<<8)|(prxbuf[24]);
	//deg_Printf("W i2c_addr=0x%x,i2c_value=0x%x\n",i2c_addr,i2c_value);

	if(1 == devSensorInit->addr_num)
	{
		temp_buf[0] = prxbuf[20];
		if(1 == devSensorInit->data_num)
		{
			temp_buf[1] = prxbuf[24];
		}
		else
		{
			temp_buf[1] = prxbuf[23];
			temp_buf[2] = prxbuf[24];
		}
	}
	else
	{
		temp_buf[0] = prxbuf[19];
		temp_buf[1] = prxbuf[20];
		if(1 == devSensorInit->data_num)
		{
			temp_buf[2] = prxbuf[24];
		}
		else
		{
			temp_buf[2] = prxbuf[23];
			temp_buf[3] = prxbuf[24];
		}
	}

	sensor_iic_write(temp_buf);
}

static void cbw_connect_read_sensor_i2c_reg(void)
{
	u32 /*i2c_addr,*/ i2c_value;
	u8 temp_buf[4];
	u8 *prxbuf = scsi.pcbw;
	
	u8 eptx = scsi.epxout;
	u8 *ptxbuf = scsi.pcsw;
//	i2c_addr = (prxbuf[17]<<24)|(prxbuf[18]<<16)|(prxbuf[19]<<8)|(prxbuf[20]);
	
	if(1 == devSensorInit->addr_num)
	{
		temp_buf[0] = prxbuf[20];
	}
	else
	{
		temp_buf[0] = prxbuf[19];
		temp_buf[1] = prxbuf[20];
	}
	//i2c_value = Sensor_ReadRegister(temp_buf,&SensorInfo);
	i2c_value = sensor_iic_read(temp_buf);
	//deg_Printf("R i2c_addr=0x%x,i2c_value=0x%x\n",i2c_addr,i2c_value);

	ptxbuf[0] = 0xcb;
	ptxbuf[1] = 0xf2;
	ptxbuf[2] = (u8)((i2c_value>>24) & 0xff);
	ptxbuf[3] = (u8)((i2c_value>>16) & 0xff);
	ptxbuf[4] = (u8)((i2c_value>>8) & 0xff);
	ptxbuf[5] = (u8)(i2c_value & 0xff);
	(*fp_usb_bulk_tx)(eptx,(u32)ptxbuf,6);
	MscCmd.Residue=0;
	scsi.cswsta=0;
	return /*true*/;

}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/

/*-----------------------------------------------------------------------------------*/
bool scsi_cmd_analysis (void)
{
	MscCmd.Residue = 0;
	scsi.bstall = 0;
	scsi.cswsta = 0;
	//u8 temp[256];
	
	//no data 
	if(MscCmd.CbwTrxLength == 0)
	{
	
		scsi_bulk_clr_rx();
		if (MscCmd.OpCode == 0x00) {
			mscCmd_TestUnitReady(); 
		} else if (MscCmd.OpCode == 0x1B) {
			mscCmd_StartStopUnit();
		} else if (MscCmd.OpCode == 0x1E) {
			mscCmd_PreventAllowMediumRemoval();
		} else if (MscCmd.OpCode == 0x2F) {
			mscCmd_Verify();
		} else if (MscCmd.OpCode == 0xCD) {
			mscCmd_ufmod();
		} else if (MscCmd.OpCode == 0xDA) {
			cbw_update();
			return false;
		}
		else if(MscCmd.OpCode == 0xCB) {	   // 0xcb is pc tools interface
			u8 *prxbuf = scsi.pcbw;
			//deg_Printf("MscCmd.SubOpCode=0x%x\n",MscCmd.SubOpCode);
			//deg_Printf("prxbuf[15]=0x%x,prxbuf[16]=0x%x,prxbuf[17]=0x%x,prxbuf[18]=0x%x\n",prxbuf[15],prxbuf[16],prxbuf[17],prxbuf[18]);
		   if(0xf0 == MscCmd.SubOpCode)	   //updata pc time to RTC
		   {
			   deg_Printf("updata rtc\n");
			   u32 time = (prxbuf[19]<<24) | (prxbuf[20]<<16) | (prxbuf[21]<<8) | prxbuf[22];
			   deg_Printf("time = 0x%x\n",time);
			   hal_rtcSecondSet(time);
		   }
		   else if(0xf1==MscCmd.SubOpCode) 	// write sensor i2c reg
		   {
			   cbw_connect_write_sensor_i2c_reg();
		   }
		}
		else 
		{
			mscSet_Status(INVALID_FIELD_IN_COMMAND);
			scsi.bstall = 1;
		}
		ScsiStallIn(scsi.bstall);
	}

	//data in
	else if(0x80 == MscCmd.CbwFlag){
		scsi_bulk_clr_rx();
		if(MscCmd.OpCode == 0x28) {
			mscCmd_Read();
		} else if(MscCmd.OpCode == 0x03) {
			mscCmd_RequestSense();
		} else if (MscCmd.OpCode == 0x12) {
			mscCmd_Inquiry();
		} else if (MscCmd.OpCode == 0x1A) {
			mscCmd_ModeSense6();
		} else if (MscCmd.OpCode == 0x23) {
			mscCmd_ReadFormatCapacity();
		} else if (MscCmd.OpCode == 0x25) {
			mscCmd_ReadCapacity();
		} else if (MscCmd.OpCode == 0xCD) {
			mscCmd_ufmod();
		}
		else if(MscCmd.OpCode == 0xCB) 
		{	   // 0xcb is pc tools interface
			// u8 *prxbufTemp = scsi.pcbw;
			// deg_Printf("[data in] Op=0x%x\n",MscCmd.SubOpCode);
			// deg_Printf("[data in] prxbuf[15]=0x%x,prxbuf[16]=0x%x,prxbuf[17]=0x%x,prxbuf[18]=0x%x\n",prxbufTemp[15],prxbufTemp[16],prxbufTemp[17],prxbufTemp[18]);
			u16 crc;
			u16 len;
			deg_Printf("[masf] MscCmd.opcode == 0xcb\n");
			if(0xf2==MscCmd.SubOpCode) 				   // read sensor i2c reg
		    {
			   cbw_connect_read_sensor_i2c_reg();
		    }
		    else if(0xf6 == MscCmd.SubOpCode)
		    {
				//subcmd
				u8 *prxbuf = scsi.pcbw;
				deg_Printf("16:%x 17:%x 18:%x 19:%x 20:%x 21:%x 22:%x\n",prxbuf[16],prxbuf[17],prxbuf[18],prxbuf[19],prxbuf[20],prxbuf[21],prxbuf[22]);
			
				masf_cbf6_process_datain(prxbuf[17], scsi.epxout, scsi.pcsw);
				MscCmd.Residue=0;
				scsi.cswsta=0;
		   }
		}
		else
		{
			MscCmd.Residue = MscCmd.CbwTrxLength;
			mscSet_Status(INVALID_FIELD_IN_COMMAND);
			scsi.bstall = 1;
		}
		
		ScsiStallIn(scsi.bstall);
	}
	//data out
	else{
		if(MscCmd.OpCode == 0xCB && MscCmd.SubOpCode == 0xf6){
			u8 *prxbufTemp = scsi.pcbw;
			masf_cbf6_process_dataout(prxbufTemp[17], scsi.epxout, scsi.prxbuf);

			__MSCRADR__ = (u32)scsi.prxbuf;
            USB20_OutCSR1 = BIT(4);  
		}
		else
		{
			if ((MscCmd.OpCode != 0x2A && MscCmd.OpCode != 0xCB))
			{  
				scsi_bulk_clr_rx();
			}
			if (MscCmd.OpCode == 0x2A)
			{
				mscCmd_Write();
			} else if (MscCmd.OpCode == 0xCD) {
				mscCmd_ufmod();
			} else {
				MscCmd.Residue = MscCmd.CbwTrxLength;
				mscSet_Status(INVALID_FIELD_IN_COMMAND);
				scsi.bstall = 1;
			}
			ScsiStallout(scsi.bstall);
		}
	}

	sent_csw();
	return true;
}

/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/

#define __LSB__(a,b,c,d) (((u32)d << 24) | ((u32)c << 16) | ((u32)b << 8) | ((u32)a << 0))
#define __MSB__(a,b,c,d) (((u32)a << 24) | ((u32)b << 16) | ((u32)c << 8) | ((u32)d << 0))
bool get_cbw(void)
{
	u8 *prxbuf = scsi.pcbw;
	
	//if(!((read_usb_reg(RxCount2) == 0x00) && (read_usb_reg(RxCount1) == 0x1f)))
	//	return false;
		
	
	//debgbuf(scsi.prxbuf,31);
	//debgbuf(scsi.ptxbuf,31);
	
	//decode cbw
	if(!((prxbuf[0] == 0x55) && (prxbuf[1] == 0x53) && (prxbuf[2] == 0x42) && (prxbuf[3] == 0x43)))
		return false;	
		
	//MscCmd.CbwTag
	
	MscCmd.CbwTag = __LSB__(prxbuf[4],prxbuf[5],prxbuf[6],prxbuf[7]);
	
	//MscCmd.CbwTrxLength
	MscCmd.CbwTrxLength = __LSB__(prxbuf[8],prxbuf[9],prxbuf[10],prxbuf[11]);
	
	//MscCmd.CbwFlag
	MscCmd.CbwFlag = prxbuf[12];
	
	//Lun
	MscCmd.CbwLun = prxbuf[13];
	
	//MscCmd.OpCode
	MscCmd.OpCode = prxbuf[15];
	
	//MscCmd.SubOpCode
	MscCmd.SubOpCode = prxbuf[16];
	
	//MscCmd.Address
	MscCmd.Address = __MSB__(prxbuf[17],prxbuf[18],prxbuf[19],prxbuf[20]);
	
	//MscCmd.SubEx
	MscCmd.SubEx = prxbuf[21];
	
	//MscCmd.Length
	MscCmd.Length = __MSB__(0,0,prxbuf[22],prxbuf[23]);
	//MscCmd.SubEx1
	MscCmd.SubEx1 = prxbuf[24];

	//MscCmd.SubEx2
	MscCmd.SubEx2 = __MSB__(prxbuf[25],prxbuf[26],prxbuf[27],prxbuf[28]);

	//MscCmd.SubEx3
	MscCmd.SubEx3 = __MSB__(0,0,prxbuf[29],prxbuf[30]);

    MscCmd.Func1 = __LSB__(prxbuf[16],prxbuf[17],prxbuf[18],prxbuf[19]);
    MscCmd.DataAddr = __LSB__(prxbuf[20],prxbuf[21],prxbuf[22],prxbuf[23]);
    MscCmd.Func2 = __LSB__(prxbuf[24],prxbuf[25],prxbuf[26],prxbuf[27]);
    MscCmd.Param = __MSB__(0,prxbuf[30],prxbuf[29],prxbuf[28]);
	//debg(".OpCode:%x,\n",MscCmd.OpCode);//debgbuf(prxbuf, 31);
	
	
	return true;
}
#undef __LSB__
#undef __MSB__

/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
bool rbc_rec_pkg(void)
{
	
	ax32xx_intEnable(IRQ_USB20,0);					//USBIE_DIS();
	USB20_Index = (scsi.epxin  & 0x7f);               //选用EP1做为 Mass
	
	u8 ret = false;
	if(USB20_OutCSR1 & BIT(0))
	{
		if(!((USB20_OutCount2  == 0x00) && (USB20_OutCount1  == 0x1f)))
			ret =  false;
		else
			ret =  true;
	}	
	ax32xx_intEnable(IRQ_USB20,1);	//USBIE_EN();
	return ret;
}
/*-----------------------------------------------------------------------------------*/
/**
* @brief  		
* @param[in]	 
* @retval		
* @details
* @note
* @attention
*/
/*-----------------------------------------------------------------------------------*/
BOOL rbc_process(void)
{
	//if(read_usb_reg(RxCSR1) & BIT(0)){
	if(rbc_rec_pkg()){
		if(get_cbw()){
			return scsi_cmd_analysis();
		}
		else{
			scsi_bulk_clr_rx();
			//ScsiStallAll();	
		}
	}
	
	return true;
}




















