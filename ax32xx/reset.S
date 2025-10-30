#include <or1k-asm.h>
    .section .vector,"ax"
    
    .macro     jmp  lbl
    .align     8
    OR1K_DELAYED_NOP(
        OR1K_INST(l.j           \lbl)
    )
    .endm
    
    .global _start 
    .org    0x0000
    jmp   _start   
    .org    0x0010
    jmp   _start
    .org    0x0020
    jmp   _except_buserr 
    .org    0x0030
    jmp   _except_dpf   //NMI interrupt:PMU & WDT
    .org    0x0040
    jmp   _except_ipf
    .org    0x0050
    jmp   _except_lpint   //LP extemal interrupt
    .org    0x0060
    jmp   _except_align
    .org    0x0070
    jmp   _except_instruction
    .org    0x0080
    jmp   _except_hpint   //HP extemal interrupt
    .org    0x0090
    jmp   _except_dtlbmiss
    .org    0x00a0
    jmp   _except_itlbmiss
    .org    0x00b0
    jmp   _except_range
    .org    0x00c0
    jmp   _except_syscall
    .org    0x00d0
    jmp   _except_float
    .org    0x00e0
    jmp   _except_trap
    .org    0x00f0
    jmp   _except_breakp    //breakpoint interrupt
    
#define   SFAT_VER	0x00020000
    .org    0x0100
    .word       SFAT_VER
    .ascii      "SFAT"
	
    .org    0x0110
    jmp   cb_mem_write
    jmp   cb_mem_read
	
    .org    0x0120    
    .word   cb_FIFO2mem
    .word   cb_mem2FIFO
	


    .org    0x0140
    .word   Sensor_DebugRegister
    .word   Sensor_common_Get
	.word   Sensor_common_Set
    .word   isp_debug_write
    .word   isp_debug_read
	.word	usb_cut_raw

	
    .org    0x0160
    .word   lcd_debug_write
    .word   lcd_debug_read
    .word   lcdreg_debug_toggle
    .word   lcdreg_debug_write

_except_buserr:
    l.addi  r3,r0,0x20
    l.j     _except
_except_dpf:
    l.addi  r3,r0,0x30
    l.j     _except
_except_ipf:
    l.addi  r3,r0,0x40
    l.j     _except
_except_align:
    l.addi  r3,r0,0x60
    l.j     _except
_except_instruction:
    l.addi  r3,r0,0x70
    l.j     _except
_except_dtlbmiss:
    l.addi  r3,r0,0x90
    l.j     _except
_except_itlbmiss:
    l.addi  r3,r0,0xa0
    l.j     _except
_except_range: 
    l.addi  r3,r0,0xb0
    l.j     _except
_except_syscall: 
    l.addi  r3,r0,0xc0
    l.j     _except
_except_float:
    l.addi  r3,r0,0xd0
    l.j     _except
_except_trap:
    l.addi  r3,r0,0xe0
    l.j     _except
_except_breakp:
    l.addi  r3,r0,0xf0
    l.j     _except
        
_except:
    l.movhi r2,hi(exception)
    l.ori   r2,r2,lo(exception)
    l.jalr  r2
    l.rfe
    l.nop
    
_except_lpint:
	/* reserved stack space, and cover red-zone */
    l.addi r1, r1, -(80+(32*4)) 
    /* push all GPRs that callee do not saved */
    l.sw 0(r1), r2
    l.sw 4(r1), r3
    l.sw 8(r1), r4
    l.sw 12(r1), r5
    l.sw 16(r1), r6
    l.sw 20(r1), r7
    l.sw 24(r1), r8
    l.sw 28(r1), r9
    l.sw 32(r1), r11
    l.sw 36(r1), r12
    l.sw 40(r1), r13
    l.sw 44(r1), r15
    l.sw 48(r1), r17
    l.sw 52(r1), r19
    l.sw 56(r1), r21
    l.sw 60(r1), r23
    l.sw 64(r1), r25
    l.sw 68(r1), r27
    l.sw 72(r1), r29
    l.sw 76(r1), r31

    /* save EPCR */
    l.mfspr r9, r0, 32
    l.sw 80(r1), r9 
    /* save ESR */
    l.mfspr r9, r0, 64
    l.sw 84(r1), r9 

    /* slow interrupt service routine */
    l.jal slow_isr//////
    l.nop//////

    /* pop EPCR */
    l.lwz r9, 80(r1)
    l.mtspr r0, r9, 32
    /* pop ESR */
    l.lwz r9, 84(r1)
    l.mtspr r0, r9, 64

    /* Pop all saved GPRs */
    l.lwz r2, 0(r1)
    l.lwz r3, 4(r1)
    l.lwz r4, 8(r1)
    l.lwz r5, 12(r1)
    l.lwz r6, 16(r1)
    l.lwz r7, 20(r1)
    l.lwz r8, 24(r1)
    l.lwz r9, 28(r1)
    l.lwz r11, 32(r1)
    l.lwz r12, 36(r1)
    l.lwz r13, 40(r1)
    l.lwz r15, 44(r1)
    l.lwz r17, 48(r1)
    l.lwz r19, 52(r1)
    l.lwz r21, 56(r1)
    l.lwz r23, 60(r1)
    l.lwz r25, 64(r1)
    l.lwz r27, 68(r1)
    l.lwz r29, 72(r1)
    l.lwz r31, 76(r1)

    l.addi r1, r1, (80+(32*4)) /* release stack space */
    l.rfe
    l.nop

_except_hpint:
#if 1
	/* reserved stack space, and cover red-zone */
    l.addi r1, r1, -(8+(32*4)) 
    /* push all GPRs that callee do not saved */
    /* the stack does not need when shadow GPRs are implemented */
    l.sw 0(r1), r9

    /* save EPCR */
    l.mfspr r9, r0, 32
    l.sw 4(r1), r9 
    /* save ESR */
    l.mfspr r9, r0, 64
    l.sw 8(r1), r9 

    /* slow interrupt service routine */
    l.jal fast_isr 
    l.nop

    /* pop EPCR */
    l.lwz r9, 4(r1)
    l.mtspr r0, r9, 32
    /* pop ESR */
    l.lwz r9, 8(r1)
    l.mtspr r0, r9, 64

    /* Pop all saved GPRs */
    l.lwz r9, 0(r1)

    l.addi r1, r1, (8+(32*4)) /* release stack space */
    l.rfe
    l.nop
#endif

_sim_end:
    l.movhi r4,0xdead
    l.ori   r4,r4,0xdead
    l.movhi r3,0x4
    l.ori   r3,r3,0x3ffc
    l.sw 0(r3),r4
    l.nop
    l.nop
    l.nop


_start:
#if 1
    l.ori r3, r9, 0
    l.jal _step_in
	l.nop   1
#endif
    l.movhi r1,hi(__stack)
    l.ori   r1,r1,lo(__stack)
    l.movhi r2,hi(exception_init)
    l.ori   r2,r2,lo(exception_init)
    l.jalr  r2
    l.nop   1
    l.movhi r2,hi(main)
    l.ori   r2,r2,lo(main)
    l.jalr  r2
    l.nop   1
    
#if 1
    l.jal _step_out
	l.nop   1
#endif

.global  _step_in
    
_step_in:
    l.movhi r4, hi(_step_data)    
	l.ori r4,r4,lo(_step_data)
    l.addi  r1,r1,-(4)
    l.sw 0(r1), r3

    l.or r3,r0,r1
    l.srli r3,r3,8
    l.slli r3,r3,8
    l.sw 0(r3),r1
    l.addi r5,r3,(0x100)
  
 cpy:   
    l.lwz r6, 0(r3)
    l.sw 0(r4), r6
    
    l.addi  r3,r3,4
    l.addi  r4,r4,4

    l.sfeq r3,r5
    l.bnf cpy    
    l.nop   1
    l.jalr  r9


.global  _step_out

_step_out:
    l.movhi r4, hi(_step_data)
    l.ori r4,r4,lo(_step_data)      
    l.lwz r3, 0(r4)
    l.lwz r7, 0(r4)
    l.srli r3,r3,8
    l.slli r3,r3,8
    l.addi r5,r3,(0x100)
    
cpy_back:   
    l.lwz r6, 0(r4)
    l.sw 0(r3), r6
    
    l.addi  r3,r3,4
    l.addi  r4,r4,4

    l.sfeq r3,r5
    l.bnf cpy_back    
    l.nop   1

    l.ori   r1,r7,0
    l.lwz r9, 0(r1)
    l.addi  r1,r1,4
    
    l.jr  r9
    l.ori   r11,r0,2


.global  _step_data

    .section .vector.data
_step_data:
    .space 0x100,0x00 
