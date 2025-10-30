/*
 * usb_device_mass.h
 *
 *  Created on: 2014Äê3ÔÂ17ÈÕ
 *      Author: jingcunxu
 */

#ifndef USB_MSC_H_
#define USB_MSC_H_

// UFI COMMAND DEFINE
#define TEST_UNIT_READY                                        0x00
#define REWIND                                                 0x01
#define REZERO_UNIT                                            0x01
#define REQUEST_SENSE                                          0x03
#define FORMAT                                                 0x04
#define FORMAT_UNIT                                            0x04
#define READ_BLOCK_LIMITS                                      0x05
#define INITIALIZE_ELEMENT_STATUS                              0x07
#define REASSIGN_BLOCKS                                        0x07
#define GET_MESSAGE_06                                        0x08
#define READ_06                                               0x08
#define RECEIVE                                                0x08
#define PRINT                                                  0x0A
#define SEND_MESSAGE_06                                       0x0A
#define SEND_06                                               0x0A
#define WRITE_06                                              0x0A
#define SEEK_06                                               0x0B
#define SLEW_AND_PRINT                                         0x0B
#define READ_REVERSE                                           0x0F
#define SYNCHRONIZE_BUFFER                                     0x10
#define WRITE_FILEMARKS                                        0x10
#define SPACE                                                  0x11
#define INQUIRY                                                0x12
#define VERIFY_06                                             0x13
#define RECOVER_BUFFERED_DATA                                  0x14
#define MODE_SELECT_06                                        0x15
#define RESERVE                                                0x16
#define RESERVE_UNIT                                           0x16
#define RELEASE                                                0x17
#define RELEASE_UNIT                                           0x17
#define COPY                                                   0x18
#define ERASE                                                  0x19
#define MODE_SENSE_06                                         0x1A
#define LOAD_UNLOAD                                            0x1B
#define SCAN                                                   0x1B
#define STOP_PRINT                                             0x1B
#define STOP_START_UNIT                                        0x1B
//+=============================================================================+
//|           D - DIRECT ACCESS DEVICE                       Device Column Key  |
//|           .T - SEQUENTIAL ACCESS DEVICE                  M = Mandatory      |
//|           . L - PRINTER DEVICE                           O = Optional       |
//|           .  P - PROCESSOR DEVICE                        V = Vendor Specific|
//|           .  .W - WRITE ONCE READ MULTIPLE DEVICE        R = Reserved       |
//|           .  . R - READ ONLY (CD-ROM) DEVICE                                |
//|           .  .  S - SCANNER DEVICE                                          |
//|           .  .  .O - OPTICAL MEMORY DEVICE                                  |
//|           .  .  . M - MEDIA CHANGER DEVICE                                  |
//|           .  .  .  C - COMMUNICATION DEVICE                                 |
//|           .  .  .  .                                                        |
//|        OP DTLPWRSOMC Description                                            |
//|----------+----------+-------------------------------------------------------|
//|        1C OOOOOOOOOO RECEIVE DIAGNOSTIC RESULTS                             |
#define RECEIVE_DIAGNOSTIC_RESULTS                             0x1C
#define SEND_DIAGNOSTIC                                        0x1D
#define PREVENT_ALLOW_MEDIUM_REMOVAL                           0x1E
#define READ_FORMAT_CAPACITY                                   0x23
#define SET_WINDOW                                             0x24
#define GET_WINDOW                                             0x25
#define READ_CAPACITY                                          0x25
#define READ_CD_ROM_CAPACITY                                   0x25
#define SCSI_CAPACITY_SECTOR_COUNT_MSB  2
#define SCSI_CAPACITY_SECTOR_COUNT_LSB  3
#define SCSI_CAPACITY_SECTOR_SIZE_MSB  6
#define SCSI_CAPACITY_SECTOR_SIZE_LSB  7
#define GET_MESSAGE_10                                        0x28
#define READ_10                                               0x28
#define READ_GENERATION                                        0x29
#define SEND_MESSAGE_10                                       0x2A
#define SEND_10                                               0x2A
#define WRITE_10                                              0x2A
#define LOCATE                                                 0x2B
#define POSITION_TO_ELEMENT                                    0x2B
#define SEEK_10                                               0x2B
#define ERASE_10                                              0x2C
#define READ_UPDATED_BLOCK                                     0x2D
#define WRITE_AND_VERIFY_10                                   0x2E
#define VERIFY_10                                             0x2F
#define SEARCH_DATA_HIGH_10                                   0x30
#define OBJECT_POSITION                                        0x31
#define SEARCH_DATA_EQUAL_10                                  0x31
#define SEARCH_DATA_LOW_10                                    0x32
#define SET_LIMITS_10                                         0x33
#define GET_DATA_BUFFER_STATUS                                 0x34
#define PRE_FETCH                                              0x34
#define READ_POSITION                                          0x34
#define SYNCHRONIZE_CACHE                                      0x35
#define LOCK_UNLOCK_CACHE                                      0x36
#define READ_DEFECT_DATA_10                                   0x37
#define MEDIUM_SCAN                                            0x38
#define COMPARE                                                0x39
#define COPY_AND_VERIFY                                        0x3A
#define WRITE_BUFFER                                           0x3B
#define READ_BUFFER                                            0x3C
#define UPDATE_BLOCK                                           0x3D
#define READ_LONG                                              0x3E
#define WRITE_LONG                                             0x3F


#define CHANGE_DEFINITION                                      0x40
#define WRITE_SAME                                             0x41
#define READ_SUB_CHANNEL                                       0x42
#define READ_TOC                                               0x43
#define READ_HEADER                                            0x44
#define PLAY_AUDIO_10                                         0x45
#define PLAY_AUDIO_MSF                                         0x47
#define PLAY_AUDIO_TRACK_INDEX                                 0x48
#define PLAY_TRACK_RELATIVE_10                                0x49
#define PAUSE_RESUME                                           0x4B
#define LOG_SELECT                                             0x4C
#define LOG_SENSE                                              0x4D
#define MODE_SELECT_10                                        0x55
#define MODE_SENSE_10                                         0x5A

#define MOVE_MEDIUM                                            0xA5
#define PLAY_AUDIO_12                                         0xA5
#define EXCHANGE_MEDIUM                                        0xA6
#define GET_MESSAGE_12                                        0xA8
#define READ_12                                               0xA8
#define PLAY_TRACK_RELATIVE_12                                0xA9
#define SEND_MESSAGE_12                                       0xAA
#define WRITE_12                                              0xAA
#define ERASE_12                                              0xAC
#define WRITE_AND_VERIFY_12                                   0xAE
#define VERIFY_12                                             0xAF
#define SEARCH_DATA_HIGH_12                                   0xB0
#define SEARCH_DATA_EQUAL_12                                  0xB1
#define SEARCH_DATA_LOW_12                                    0xB2
#define SET_LIMITS_12                                         0xB3
#define REQUEST_VOLUME_ELEMENT_ADDRESS                         0xB5
#define SEND_VOLUME_TAG                                        0xB6
#define READ_DEFECT_DATA_12                                   0xB7
#define READ_ELEMENT_STATUS                                   0xB8

/* Sense Key Definition */
#define SENSE_KEY_NO_SENSE					0x00
#define SENSE_KEY_RECOVERED_ERROR			0x01
#define SENSE_KEY_NOT_READY					0x02
#define SENSE_KEY_MEDIUM_ERROR				0x03
#define SENSE_KEY_HARDWARE_ERROR			0x04
#define SENSE_KEY_ILLEGAL_REQUEST			0x05
#define SENSE_KEY_UNIT_ATTENTION			0x06
#define SENSE_KEY_DATA_PROTECT				0x07
#define SENSE_KEY_BLANK_CHECK				0x08
#define SENSE_KEY_VENDOR_SPECIFIC			0x09
#define SENSE_KEY_ABORTED_COMMAND			0x0B
#define SENSE_KEY_VOLUME_OVERFLOW			0x0D
#define SENSE_KEY_MIS_COMPARE				0x0E


/* Additional Sense code definition*/
#define ASC_NO_ADDITIONAL_SENSE_INFORMATION		0x00
#define ASC_RECOVERED_DATA_WITH_RETRIES			0x17
#define ASC_RECOVERED_DATA_WITH_ECC				0x18
#define ASC_MEDIUM_PRESENT						0x3A
#define ASC_LOGICAL_DRIVE_NOT_READY_BEING_READY	0x04
#define ASC_LOGICAL_DRIVE_NOT_READY_FMT_IN_PRGS	0x04
#define ASC_NO_REFERENCE_POSITION_FOUND			0x06
#define ASC_NO_SEEK_COMPLETE					0x02
#define ASC_WRITE_FAULT							0x03
#define ASC_ID_CRC_ERROR						0x10
#define ASC_UNRECOVERED_READ_ERROR				0x11
#define ASC_ADDRESS_MARK_NOT_FOUND_FOR_ID_FIELD	0x12
#define ASC_RECORDED_ENTITY_NOT_FOUND			0x14
#define ASC_INCOMPATIBLE_MEDIUM_INSTALLED		0x30
#define ASC_CANNOT_READ_MEDIUM_INCOMPATIBLE_FMT	0x30
#define ASC_CANNOT_READ_MEDIUM_UNKNOWN_FORMAT	0x30
#define ASC_FORMAT_COMMAND_FAILED				0x31
#define ASC_INVALID_COMMAND_OPERATION_CODE		0x20
#define ASC_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE	0x21
#define ASC_INVALID_FIELD_IN_COMMAND_PACKET		0x24
#define ASC_LOGICAL_UNIT_NOT_SUPPORTED			0x25
#define ASC_INVALID_FIELD_IN_PARAMETER_LIST		0x26
#define ASC_MEDIUM_REMOVAL_PREVENTED			0x53
#define ASC_NOT_READY_TO_READY_TRANSIT_MDI_CHNG	0x28
#define ASC_POWER_ON_OR_BUS_DEVICE_RESET		0x29
#define ASC_WRITE_PROTECTED_MEDIA				0x27
#define ASC_OVERLAPPED_COMMAND_ATTEMPTED		0x4E

/* Definition of additional sense code qualifier*/
/* Additional Sense code definition */
#define ASCQ_NO_ADDITIONAL_SENSE_INFORMATION		0x00
#define ASCQ_RECOVERED_DATA_WITH_RETRIES			0x01
#define ASCQ_RECOVERED_DATA_WITH_ECC				0x00
#define ASCQ_MEDIUM_PRESENT							0x00
#define ASCQ_LOGICAL_DRIVE_NOT_READY_BEING_READY	0x01
#define ASCQ_LOGICAL_DRIVE_NOT_READY_FMT_IN_PRGS	0x04
#define ASCQ_NO_REFERENCE_POSITION_FOUND			0x00
#define ASCQ_NO_SEEK_COMPLETE						0x00
#define ASCQ_WRITE_FAULT							0x00
#define ASCQ_ID_CRC_ERROR							0x00
#define ASCQ_UNRECOVERED_READ_ERROR					0x00
#define ASCQ_ADDRESS_MARK_NOT_FOUND_FOR_ID_FIELD	0x00
#define ASCQ_RECORDED_ENTITY_NOT_FOUND				0x00
#define ASCQ_INCOMPATIBLE_MEDIUM_INSTALLED			0x00
#define ASCQ_CANNOT_READ_MEDIUM_INCOMPATIBLE_FMT	0x02
#define ASCQ_CANNOT_READ_MEDIUM_UNKNOWN_FORMAT		0x01
#define ASCQ_FORMAT_COMMAND_FAILED					0x01
#define ASCQ_INVALID_COMMAND_OPERATION_CODE			0x00
#define ASCQ_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE		0x00
#define ASCQ_INVALID_FIELD_IN_COMMAND_PACKET		0x00
#define ASCQ_LOGICAL_UNIT_NOT_SUPPORTED				0x00
#define ASCQ_INVALID_FIELD_IN_PARAMETER_LIST		0x00
#define ASCQ_MEDIUM_REMOVAL_PREVENTED				0x02
#define ASCQ_NOT_READY_TO_READY_TRANSIT_MDI_CHNG	0x00
#define ASCQ_POWER_ON_OR_BUS_DEVICE_RESET			0x00
#define ASCQ_WRITE_PROTECTED_MEDIA					0x00
#define ASCQ_OVERLAPPED_COMMAND_ATTEMPTED			0x00

//void rbc_process(void);
BOOL rbc_process(void);
void STALL_INEP(u8 bEp);
void STALL_OUTEP(u8 bEp);

//extern u8 rx_fifo[512];
//extern u8 tx_fifo[512];


//#define 	MAS_EPX			EP1
//#define		MAS_FIFO		EP1_FIFO
//#define 	MAS_PKG_SZ		512//EP2_BUF_SIZE	

#define		NO_SENSE					0
#define		INVALID_FIELD_IN_COMMAND	1
#define		NEW_MEDIUM_ARRIVEL			2
#define		WRITE_PROTECTED				3
//#define		PASSWORD_ERROR				4
//#define		USERNAME_ERROR				5
//#define		USER_PROHIBITTED			6
#define		MEDIUM_NOT_PRESENT			4  //7
#define		DATA_PHASE_ERROR			5  //8


BOOL rbc_process(void);
void msc_epx_cfg(void);
//extern bool (* fp_rbc_process)(void);
//extern void (* fp_msc_epx_cfg)(void);
//extern bool (*fp_usb_bulk_tx)(u8 epx, u32 adr, u16 len);
//extern bool (*fp_usb_bulk_rx)(u8 epx, u32 adr, u16 len);

extern const u8 UDISK_CONFIG_DESC_DATA[];


#define 	__MSC_EPX__			1
#define 	__MSC_EPIN__		0x81
#define 	__MSC_EPOUT__		1
#define 	__MSC_LUN__ 		1
#define 	__MSC_SIZE__		512
//#define 	__MSCTADR__		USB20E3TADR
//#define 	__MSCRADR__		USB20E3RADR
//bool (*fp_rbc_process)(void);
extern bool (*fp_usb_bulk_tx)(u8 epx, u32 adr, u16 len);
extern bool (*fp_usb_bulk_rx)(u8 epx, u32 adr, u16 len);
extern s32 (*fp_disk_rd)(void *pDataBuf, u32 dwLBA, u32 dwLBANum);
extern s32 (*fp_disk_wr)(void *pDataBuf, u32 dwLBA, u32 dwLBANum);
extern s32 (*fp_disk_online)(void);
extern u32 (*fp_disk_cap)();
extern void (*fp_disk_free)(void);
extern void (*fp_disk_busy)(void);
extern s32 (*fp_disk_stop)(void);

#endif /* USB_DEVICE_MASS_H_ */
