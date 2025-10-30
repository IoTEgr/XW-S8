#include "ax32xx_cfg.h"

MEMORY
        {
            boot     : ORIGIN = 0x01FFFC00, LENGTH = 0x0000200
            ram_boot : ORIGIN = 0x00000000, LENGTH = 20K
            ram_user : ORIGIN = 0x00000000, LENGTH = 20K
            usbfifo  : ORIGIN = 0x00008000, LENGTH = 0x0001000
            sdram    : ORIGIN = 0x02000000, LENGTH = SDRAM_SIZE
            exsdram  : ORIGIN = 0x00000000, LENGTH = 0x00800000
        }


SECTIONS
{
        /* bootsec */
        .bootsec : AT(0) 
        {
            KEEP(*(.bootsec))
        } > boot
        
        .exception : AT((LOADADDR(.bootsec) + SIZEOF(.bootsec) + 0x1FF) & 0xfffffe00)
        {
_exception_vma = .;
            *(.vector)
            *(.vector.text)
            *(.vector.data)
        } > ram_user

        /* RAM AREA2: mask variable use + code use + buf use*/
        .ram (NOLOAD): 
        {
__sram_start = .;
            /* mask rom use var area, must be careful len = __MASK_VARLEN__*/
            *(.msk_var_use) 
            *(.sdc_buf)
            /* buff if need run in sram */
            *(.sram_comm)
        } > ram_user
__sram_end = .;

        .fifo (NOLOAD): 
        {
__ufifo_start = .;
            *(.sram_Usbfifo)
        } > usbfifo
__ufifo_end = .;

        /* user code in sdram */
        .text 0x2000000 : AT((LOADADDR(.exception) + SIZEOF(.exception) + 0x1FF) & 0xfffffe00)
        {
_text_start = .;
            *(.text*)
            *(.rodata*) 
            *(.data*)
        } > sdram
_text_end = .;

        /* bss in sdram */
        .bss ALIGN(4) (NOLOAD):  
        {
__bss_start = .;
            *(.bss*)
            *(COMMON)
            *(.buff.clmt)
            *(.big_buffer*)
            *(._urx_buf_)
        } > sdram

_sdram_remian_addr = ALIGN(64) + 0;
        
        .before_load 0x2000 : AT((LOADADDR(.text) + SIZEOF(.text) + 0x1FF) & 0xfffffe00)
        {
_before_load_vma = .;
            *(.before_load.entry)
            *(.before_load)
        } > ram_boot

        .lcd_resource 0x0000000 : AT((LOADADDR(.before_load) + SIZEOF(.before_load) + 0xFF) & 0xffffff00)
		{
			*(.lcd_res.header)
				
			_res_lcd_struct_start = .;
			*(.lcd_res.struct)
			_res_lcd_struct_end = .;	
			*(.lcd_res.init_tab)
				 
			*(.lcd_res.gamma_tab)
				
			*(.lcd_res.contra_tab)
			
		} > exsdram
		.sensor_resource 0x0000000 : AT(LOADADDR(.lcd_resource) + SIZEOF(.lcd_resource)){
			*(.sensor_res.header)
				
			_res_sensor_header_item_start = .;		
			*(.sensor_res.header.items)
			_res_sensor_header_item_end = .;
				
			*(.sensor_res.isp_tab)
				
			*(.sensor_res.struct)
				
			*(.sensor_res.init_tab)
				
		}> exsdram
        
        .eh_frame (NOLOAD):
        {
            *(.eh_frame)
        } > exsdram
        
        _exception_len = ((SIZEOF(.exception) + 511) / 512);
        _exception_sec = (LOADADDR(.exception) / 512);
        
        /*start lba address for boot_loader upload in SPI-FLASH */    
        _text_sec = (LOADADDR(.text) / 512);
        
        /*code lba length */
        _text_len = (((_text_end - _text_start)+0x1ff) / 512);
        
        _before_load_len = ((SIZEOF(.before_load) + 511) / 512);
        _before_load_sec = (LOADADDR(.before_load) / 512);

        __stack_size     = 4096 + 1024;
        __sdram_remain_size = (ORIGIN(sdram) + LENGTH(sdram) - _sdram_remian_addr) - __stack_size;
        ASSERT(((ORIGIN(sdram) + LENGTH(sdram) - _sdram_remian_addr) >= __stack_size),"No memroy for stack")
        
        __bss_end = _sdram_remian_addr + __sdram_remain_size;
        
		/*addr of resource in spiflash,uint:byte*/
		_lcd_resource_start_addr = LOADADDR(.lcd_resource); 
		_lcd_resource_size = _res_lcd_struct_end - _res_lcd_struct_start;
		_sensor_resource_start_addr = LOADADDR(.sensor_resource);
		_res_sensor_header_len = _res_sensor_header_item_end - _res_sensor_header_item_start;
		
		
		  

}
    /*PROVIDE(__stack = 0xfd000 - 4); if d-cache is not use*/
    /*PROVIDE(__stack = 0x2000 - 4);if d-cache use*/
    PROVIDE(__stack = ORIGIN(sdram) + LENGTH(sdram) - 4)/*stack in sdram */;
    
