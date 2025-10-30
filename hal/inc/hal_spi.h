/****************************************************************************
**
 **                         BUILDWIN HAL LAYER
  ** *   **                     THE APPOTECH HAL
   **** **                         SPI HAL LAYER DRIVER
  *** ***
 **  * **               (C) COPYRIGHT 2016 BUILDWIN 
**      **                         
         **         BuildWin SZ LTD.CO  ; VIDEO PROJECT TEAM
          **       
* File Name   : hal_spi.h
* Author      : Mark.Douglas 
* Version     : V0200
* Date        : 05/25/2016
* Description : This file for BUILDWIN TIMER HARDWARE LAYER.
*               
*               
* History     :
* 2017-02-27  : 
*      <1>.This is created by mark,set version as v0100.
*      <2>.Add basic functions.
******************************************************************************/

#ifndef HAL_SPI_H
#define HAL_SPI_H
#include "../../ax32_platform_demo/inc/customer.h"





//----------------------spi flash command table-------------------------------
#define		SF_WRITE_ENABLE	       		0x06
#define		SF_WRITE_DISABLE	   		0x04
#define		SF_READ_STATUS	   			0x05
#define		SF_WRITE_STATUS	   			0x01
#define		SF_READ_DATA	   			0x03				// <= SF_SIZE
#define		SF_WRITE_DATA	   			0x02				// 256 BYTE
#define		SF_ERASE_BLOCK	   			0xD8				// 64K
#define		SF_ERASE_SECTOR	   			0x20				// 4k
#define		SF_READ_ID	       			0x9F				// 3 BYTE
#define		SF_ERASE_CHIP	   			0xC7
#define		SF_POWER_DOWN	   			0xB9
#define		SF_RELEASE_POWER_DOWN	   	0xAB




#define   SF_PAGE_SIZE      256
#define   SF_SECTOR_SIZE   4096
#define   SF_BLOCK_SIZE    65536
#define   SF_STATUS_TIMEOUT   (8000*2000)  // cpu loop



/*******************************************************************************
* Function Name  : hal_spiIOShare
* Description	 : share spi io for other application using
* Input 		 : 
* Output		 : None
* Return		 : 
*******************************************************************************/
u8 hal_spiIOShare(void);
/*******************************************************************************
* Function Name  : hal_spiInit
* Description	 : spi initial
* Input 		 : none
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiInit(void);
/*******************************************************************************
* Function Name  : hal_spiSendByte
* Description	 : spi send one byte data
* Input 		 : u8 : byte
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiSendByte(u8 byte);
/*******************************************************************************
* Function Name  : hal_spiSend
* Description	 : spi send data using DMA
* Input 		 : u32 addr : buffer addr
                    u32 len   : data length
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiSend(u32 addr,u32 len);
/*******************************************************************************
* Function Name  : hal_spiRecvByte
* Description	 : spi recv one byte data
* Input 		 : none
* Output		 : None
* Return		 : u8 : 
*******************************************************************************/
u8 hal_spiRecvByte(void);
/*******************************************************************************
* Function Name  : hal_spiRecv
* Description	 : spi recv data using DMA
* Input 		 : u32 addr : buffer addr
                    u32 len   : data length
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiRecv(u32 addr,u32 len);
/*******************************************************************************
* Function Name  : hal_spiFlashReadID
* Description	 : spi flash read id
* Input 		 : none
* Output		 : None
* Return		 : u32 : id
*******************************************************************************/
u32 hal_spiFlashReadID(void);
/*******************************************************************************
* Function Name  : hal_spiFlashWriteEnable
* Description	 : spi flash write enable
* Input 		 : none
* Output		 : None
* Return		 : none
*******************************************************************************/
void hal_spiFlashWriteEnable(void);
/*******************************************************************************
* Function Name  : hal_spiFlashWait
* Description	 : wait spi flash idle
* Input 		 : none
* Output		 : None
* Return		 : s32 : >=0 success 
                                  <   timeout
*******************************************************************************/
s32 hal_spiFlashWait(void);
/*******************************************************************************
* Function Name  : hal_spiFlashReadPage
* Description	 : read spi flash one page
* Input 		 : u32 addr : spi flash addr in byte
                    u32 buffer : buffer addr
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashReadPage(u32 addr,u32 buffer);
/*******************************************************************************
* Function Name  : hal_spiFlashRead
* Description	 : read spi flash
* Input 		 : u32 addr : spi flash addr in byte
                    u32 buffer : buffer addr
                    u32 len   :  data length
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashRead(u32 addr,u32 buffer,u32 len);
/*******************************************************************************
* Function Name  : hal_spiFlashWritePage
* Description	 : write spi flash one page
* Input 		 : u32 addr : spi flash addr in byte
                    u32 buffer : buffer addr
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashWritePage(u32 addr,u32 buffer);
/*******************************************************************************
* Function Name  : hal_spiFlashWrite
* Description	 : write spi flash
* Input 		 : u32 addr : spi flash addr in byte
                    u32 buffer : buffer addr
                    u32 len   : data length
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashWrite(u32 addr,u32 buffer,u32 len);
/*******************************************************************************
* Function Name  : hal_spiFlashEraseSector
* Description	 : erase spi flash one sector
* Input 		 : u32 blockAddr : sector addr aglin in byte
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashEraseSector(u32 sectorAddr);
/*******************************************************************************
* Function Name  : hal_spiFlashEraseBlock
* Description	 : erase spi flash one block
* Input 		 : u32 blockAddr : block addr aglin in byte
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashEraseBlock(u32 blockAddr);
/*******************************************************************************
* Function Name  : hal_spiFlashEraseChip
* Description	 : erase spi flash all chip
* Input 		 : 
* Output		 : None
* Return		 : s32 : >=0 success 
*******************************************************************************/
s32 hal_spiFlashEraseChip(void);

#if ENABLE_FLASH_PHOTO
//===============use spi flash for save photo api==================
//==udisk: head+jpg0+jpg1+jpg2+jpg3...


	
#define FLASH_UDISK_LOOP_NUM	9999

void spi_udisk_init(u32 flash_cap);
//==return :udisk jpg num ,after format is 0==
u16 spi_udisk_jpg_num();
//==return :udisk first jpg num , loop 0~99999==
u32 spi_udisk_first_jpg_num();
//==return: 0 is ok, other is err==
u8 spi_udisk_write_jpg(u8 *jpg,u32 size);
//==return: 0 is ok, other is err==
u8 spi_udisk_del_jpg(u32 index);
//==return index jpg size===
u32 spi_udisk_read_jpg_size(u32 index);
//==return :0 is ok,other is err==
u8 spi_udisk_read_jpg(u32 index,u8 *jpg);
//==return : pos ==
u16 spi_udisk_get_list_idx(u32 index);
//==format udisk==
//==input : flash capacity===
void spi_udisk_format(u32 flash_cap);

u16 spi_udisk_jpg_free_num();

#endif



#endif

