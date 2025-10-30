
#ifndef __ASSEMBLER__
//#define SFR_AT *(volatile unsigned long*)
#define SFR_AT *(volatile __sfr unsigned long*)
#define SFR_16_AT *(volatile unsigned short int*)
#define SFR_8_AT *(volatile unsigned char*)
#else
#define SFR_AT
#endif


#ifndef _THUNDERSE_SFR_
#define _THUNDERSE_SFR_

//#define SFR_BASE 0x00000000 
//#define SFR_OFFSET 4

#define MAX_SPRS_PER_GRP_BITS (11)

/* Base addresses for the groups */
#define SPRGROUP_SYS    (0  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_01     (1  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_02     (2  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_DC     (3  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_IC     (4  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_MAC    (5  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_DBG    (6  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PCU    (7  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_08     (8  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_PIC    (9  << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_10     (10 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_11     (11 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_12     (12 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_13     (13 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_14     (14 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_15     (15 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR0   (16 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR1   (17 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR2   (18 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR3   (19 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR4   (20 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR5   (21 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR6   (22 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR7   (23 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR8   (24 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR9   (25 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR10  (26 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR11  (27 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR12  (28 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR13  (29 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR14  (30 << MAX_SPRS_PER_GRP_BITS)
#define SPRGROUP_SFR15  (31 << MAX_SPRS_PER_GRP_BITS)

/* System control and status group */
#define SPR_VR      SFR_AT  (SPRGROUP_SYS + (0x0 << 2))  
#define SPR_NPC     SFR_AT  (SPRGROUP_SYS + (0x5 << 2))  
#define SPR_SR      SFR_AT  (SPRGROUP_SYS + (0x6 << 2))  
#define SPR_PPC     SFR_AT  (SPRGROUP_SYS + (0x7 << 2))  

#define SPR_EPCR    SFR_AT  (SPRGROUP_SYS + (0x10 << 2))  
#define SPR_EPCR1   SFR_AT  (SPRGROUP_SYS + (0x11 << 2))  
#define SPR_EPCR2   SFR_AT  (SPRGROUP_SYS + (0x12 << 2))  

#define SPR_EVBA    SFR_AT  (SPRGROUP_SYS + (0x14 << 2))  

#define SPR_EEAR    SFR_AT  (SPRGROUP_SYS + (0x20 << 2))

#define SPR_ESR     SFR_AT  (SPRGROUP_SYS + (0x30 << 2))
#define SPR_ESR1    SFR_AT  (SPRGROUP_SYS + (0x31 << 2))
#define SPR_ESR2    SFR_AT  (SPRGROUP_SYS + (0x32 << 2))

//  SPR_GPR .SPR_SGPR
#define SPR_GPR00   SFR_AT  (SPRGROUP_SYS + (0x40<< 2))
#define SPR_GPR01   SFR_AT  (SPRGROUP_SYS + (0x41<< 2))
#define SPR_GPR02   SFR_AT  (SPRGROUP_SYS + (0x42<< 2))
#define SPR_GPR03   SFR_AT  (SPRGROUP_SYS + (0x43<< 2))
#define SPR_GPR04   SFR_AT  (SPRGROUP_SYS + (0x44<< 2))
#define SPR_GPR05   SFR_AT  (SPRGROUP_SYS + (0x45<< 2))
#define SPR_GPR06   SFR_AT  (SPRGROUP_SYS + (0x46<< 2))
#define SPR_GPR07   SFR_AT  (SPRGROUP_SYS + (0x47<< 2))
#define SPR_GPR08   SFR_AT  (SPRGROUP_SYS + (0x48<< 2))
#define SPR_GPR09   SFR_AT  (SPRGROUP_SYS + (0x49<< 2))
#define SPR_GPR10   SFR_AT  (SPRGROUP_SYS + (0x4a<< 2))
#define SPR_GPR11   SFR_AT  (SPRGROUP_SYS + (0x4b<< 2))
#define SPR_GPR12   SFR_AT  (SPRGROUP_SYS + (0x4c<< 2))
#define SPR_GPR13   SFR_AT  (SPRGROUP_SYS + (0x4d<< 2))
#define SPR_GPR14   SFR_AT  (SPRGROUP_SYS + (0x4e<< 2))
#define SPR_GPR15   SFR_AT  (SPRGROUP_SYS + (0x4f<< 2))
#define SPR_GPR16   SFR_AT  (SPRGROUP_SYS + (0x50<< 2))
#define SPR_GPR17   SFR_AT  (SPRGROUP_SYS + (0x51<< 2))
#define SPR_GPR18   SFR_AT  (SPRGROUP_SYS + (0x52<< 2))
#define SPR_GPR19   SFR_AT  (SPRGROUP_SYS + (0x53<< 2))
#define SPR_GPR20   SFR_AT  (SPRGROUP_SYS + (0x54<< 2))
#define SPR_GPR21   SFR_AT  (SPRGROUP_SYS + (0x55<< 2))
#define SPR_GPR22   SFR_AT  (SPRGROUP_SYS + (0x56<< 2))
#define SPR_GPR23   SFR_AT  (SPRGROUP_SYS + (0x57<< 2))
#define SPR_GPR24   SFR_AT  (SPRGROUP_SYS + (0x58<< 2))
#define SPR_GPR25   SFR_AT  (SPRGROUP_SYS + (0x59<< 2))
#define SPR_GPR26   SFR_AT  (SPRGROUP_SYS + (0x5a<< 2))
#define SPR_GPR27   SFR_AT  (SPRGROUP_SYS + (0x5b<< 2))
#define SPR_GPR28   SFR_AT  (SPRGROUP_SYS + (0x5c<< 2))
#define SPR_GPR29   SFR_AT  (SPRGROUP_SYS + (0x5d<< 2))
#define SPR_GPR30   SFR_AT  (SPRGROUP_SYS + (0x5e<< 2))
#define SPR_GPR31   SFR_AT  (SPRGROUP_SYS + (0x5f<< 2))

// SPR_SGPR
#define SPR_SGPR02  SFR_AT  (SPRGROUP_SYS + (0x62<< 2))
#define SPR_SGPR03  SFR_AT  (SPRGROUP_SYS + (0x63<< 2))
#define SPR_SGPR04  SFR_AT  (SPRGROUP_SYS + (0x64<< 2))
#define SPR_SGPR05  SFR_AT  (SPRGROUP_SYS + (0x65<< 2))
#define SPR_SGPR06  SFR_AT  (SPRGROUP_SYS + (0x66<< 2))
#define SPR_SGPR07  SFR_AT  (SPRGROUP_SYS + (0x67<< 2))
#define SPR_SGPR08  SFR_AT  (SPRGROUP_SYS + (0x68<< 2))
#define SPR_SGPR09  SFR_AT  (SPRGROUP_SYS + (0x69<< 2))
#define SPR_SGPR10  SFR_AT  (SPRGROUP_SYS + (0x6a<< 2))
#define SPR_SGPR11  SFR_AT  (SPRGROUP_SYS + (0x6b<< 2))
#define SPR_SGPR12  SFR_AT  (SPRGROUP_SYS + (0x6c<< 2))
#define SPR_SGPR13  SFR_AT  (SPRGROUP_SYS + (0x6d<< 2))
#define SPR_SGPR15  SFR_AT  (SPRGROUP_SYS + (0x6f<< 2))
#define SPR_SGPR17  SFR_AT  (SPRGROUP_SYS + (0x71<< 2))
#define SPR_SGPR19  SFR_AT  (SPRGROUP_SYS + (0x73<< 2))
#define SPR_SGPR21  SFR_AT  (SPRGROUP_SYS + (0x75<< 2))
#define SPR_SGPR23  SFR_AT  (SPRGROUP_SYS + (0x77<< 2))
#define SPR_SGPR25  SFR_AT  (SPRGROUP_SYS + (0x79<< 2))
#define SPR_SGPR27  SFR_AT  (SPRGROUP_SYS + (0x7b<< 2))
#define SPR_SGPR29  SFR_AT  (SPRGROUP_SYS + (0x7d<< 2))
#define SPR_SGPR31  SFR_AT  (SPRGROUP_SYS + (0x7f<< 2))

/* Reserved 01 group */
#define SPR_01_TEST SFR_AT  (SPRGROUP_01 + (0x00<< 2))

/* Reserved 02 group */
#define SPR_02_TEST SFR_AT  (SPRGROUP_02 + (0x00<< 2))

/* Data cache group */
#define SPR_DCCR    SFR_AT  (SPRGROUP_DC + (0x0 << 2))
#define SPR_DCBIR   SFR_AT  (SPRGROUP_DC + (0x1 << 2))
#define SPR_DCAIR   SFR_AT  (SPRGROUP_DC + (0x2 << 2))
#define SPR_DCBWR   SFR_AT  (SPRGROUP_DC + (0x3 << 2))
#define SPR_DCAWR   SFR_AT  (SPRGROUP_DC + (0x4 << 2))
#define SPR_DCBFR   SFR_AT  (SPRGROUP_DC + (0x5 << 2))
#define SPR_DCAFR   SFR_AT  (SPRGROUP_DC + (0x6 << 2))
#define SPR_DCCNT   SFR_AT  (SPRGROUP_DC + (0x7 << 2))

/* Instruction cache group */
#define SPR_ICCR    SFR_AT  (SPRGROUP_IC + (0x0 << 2))
#define SPR_ICBIR   SFR_AT  (SPRGROUP_IC + (0x1 << 2))
#define SPR_ICAIR   SFR_AT  (SPRGROUP_IC + (0x2 << 2))
#define SPR_ICCNT   SFR_AT  (SPRGROUP_IC + (0x3 << 2))

/* MAC group */
#define SPR_MACLO   SFR_AT  (SPRGROUP_MAC + (0x0 << 2))
#define SPR_MACHI   SFR_AT  (SPRGROUP_MAC + (0x1 << 2))

/* DBG group */
#define SPR_BPOINT0 SFR_AT  (SPRGROUP_DBG + (0x0 << 2))
#define SPR_BPOINT1 SFR_AT  (SPRGROUP_DBG + (0x1 << 2))
#define SPR_BPOINT2 SFR_AT  (SPRGROUP_DBG + (0x2 << 2))
#define SPR_BPOINT3 SFR_AT  (SPRGROUP_DBG + (0x3 << 2))

#define SPR_LSUBP0  SFR_AT  (SPRGROUP_DBG + (0x4 << 2))
#define SPR_LSUBP1  SFR_AT  (SPRGROUP_DBG + (0x5 << 2))
#define SPR_LSUBP2  SFR_AT  (SPRGROUP_DBG + (0x6 << 2))
#define SPR_LSUBP3  SFR_AT  (SPRGROUP_DBG + (0x7 << 2))
#define SPR_LSUCFG0 SFR_AT  (SPRGROUP_DBG + (0x8 << 2)) 
#define SPR_LSUCFG1 SFR_AT  (SPRGROUP_DBG + (0x9 << 2)) 
#define SPR_LSUCFG2 SFR_AT  (SPRGROUP_DBG + (0xA << 2)) 
#define SPR_LSUCFG3 SFR_AT  (SPRGROUP_DBG + (0xB << 2)) 
#define SPR_LSUCFG4 SFR_AT  (SPRGROUP_DBG + (0xC << 2)) 
#define SPR_SPRBP0  SFR_AT  (SPRGROUP_DBG + (0xD << 2))
#define SPR_SPRBP1  SFR_AT  (SPRGROUP_DBG + (0xE << 2))
#define SPR_SPRBP2  SFR_AT  (SPRGROUP_DBG + (0xF << 2))

#define SPR_SPRBP3  SFR_AT  (SPRGROUP_DBG + (0x10 << 2))
#define SPR_SPRCFG0 SFR_AT  (SPRGROUP_DBG + (0x11 << 2)) 
#define SPR_SPRCFG1 SFR_AT  (SPRGROUP_DBG + (0x12 << 2)) 
#define SPR_SPRCFG2 SFR_AT  (SPRGROUP_DBG + (0x13 << 2)) 
#define SPR_SPRCFG3 SFR_AT  (SPRGROUP_DBG + (0x14 << 2)) 
#define SPR_BPFLAG  SFR_AT  (SPRGROUP_DBG + (0x15 << 2)) 
#define SPR_BPEPC   SFR_AT  (SPRGROUP_DBG + (0x16 << 2)) 
#define SPR_BPADDR  SFR_AT  (SPRGROUP_DBG + (0x17 << 2)) 
#define SPR_BPWDATA SFR_AT  (SPRGROUP_DBG + (0x18 << 2)) 

#define SPR_BERRSTA SFR_AT  (SPRGROUP_DBG + (0x20 << 2))

/* PCU group */
#define SPR_PCUCON0 SFR_AT  (SPRGROUP_PCU + (0x0 << 2))
#define SPR_PCUCON1 SFR_AT  (SPRGROUP_PCU + (0x1 << 2))
#define SPR_PCUCON2 SFR_AT  (SPRGROUP_PCU + (0x2 << 2))
#define SPR_PCUCON3 SFR_AT  (SPRGROUP_PCU + (0x3 << 2))
#define SPR_PCUCNT0 SFR_AT  (SPRGROUP_PCU + (0x10 << 2))
#define SPR_PCUCNT1 SFR_AT  (SPRGROUP_PCU + (0x11 << 2))
#define SPR_PCUCNT2 SFR_AT  (SPRGROUP_PCU + (0x12 << 2))
#define SPR_PCUCNT3 SFR_AT  (SPRGROUP_PCU + (0x13 << 2))

/* Reserved 08 group */
#define SPR_08_TEST SFR_AT  (SPRGROUP_08 + (0x00<< 2))

/* PIC group */
#define SPR_PICMR   SFR_AT  (SPRGROUP_PIC + (0x0 << 2))
#define SPR_PICPR   SFR_AT  (SPRGROUP_PIC + (0x1 << 2))
#define SPR_PICSR   SFR_AT  (SPRGROUP_PIC + (0x2 << 2))
#define SPR_PICVR   SFR_AT  (SPRGROUP_PIC + (0x3 << 2))
#define SPR_PICER   SFR_AT  (SPRGROUP_PIC + (0x4 << 2))
#define SPR_PICCR   SFR_AT  (SPRGROUP_PIC + (0x5 << 2))

/* Reserved 10 group */
#define SPR_10_TEST SFR_AT  (SPRGROUP_10 + (0x00<< 2))

/* Reserved 11 group */
#define SPR_11_TEST SFR_AT  (SPRGROUP_11 + (0x00<< 2))

/* Reserved 12 group */
#define SPR_12_TEST SFR_AT  (SPRGROUP_12 + (0x00<< 2))

/* Reserved 13 group */
#define SPR_13_TEST SFR_AT  (SPRGROUP_13 + (0x00<< 2))

/* Reserved 14 group */
#define SPR_14_TEST SFR_AT  (SPRGROUP_14 + (0x00<< 2))

/* Reserved 15 group */
#define SPR_15_TEST SFR_AT  (SPRGROUP_15 + (0x00<< 2))

// Define the register address constants, word address

/////////////////////////////////////////////////////////
// Group 0
/////////////////////////////////////////////////////////
#define SFR_0_TEST             SFR_AT (SPRGROUP_SFR0 + (0x000 <<2))
//.................... 0x00x .........................
#define PORTA             SFR_AT (SPRGROUP_SFR0 + (0x000<<2))
#define PORTB             SFR_AT (SPRGROUP_SFR0 + (0x001<<2))
#define PORTD             SFR_AT (SPRGROUP_SFR0 + (0x002<<2))
#define PORTE             SFR_AT (SPRGROUP_SFR0 + (0x003<<2))
#define PORTF             SFR_AT (SPRGROUP_SFR0 + (0x004<<2))
#define PORTG             SFR_AT (SPRGROUP_SFR0 + (0x005<<2))
#define PORTH             SFR_AT (SPRGROUP_SFR0 + (0x006<<2))
#define PORTJ             SFR_AT (SPRGROUP_SFR0 + (0x007<<2))

//.................... 0x01x .........................
#define PADIR             SFR_AT (SPRGROUP_SFR0 + (0x010<<2))
#define PBDIR             SFR_AT (SPRGROUP_SFR0 + (0x011<<2))
#define PDDIR             SFR_AT (SPRGROUP_SFR0 + (0x012<<2))
#define PEDIR             SFR_AT (SPRGROUP_SFR0 + (0x013<<2))
#define PFDIR             SFR_AT (SPRGROUP_SFR0 + (0x014<<2))
#define PGDIR             SFR_AT (SPRGROUP_SFR0 + (0x015<<2))
#define PHDIR             SFR_AT (SPRGROUP_SFR0 + (0x016<<2))
#define PJDIR             SFR_AT (SPRGROUP_SFR0 + (0x017<<2))

//.................... 0x02x .........................
#define PAPU              SFR_AT (SPRGROUP_SFR0 + (0x020<<2))
#define PBPU              SFR_AT (SPRGROUP_SFR0 + (0x021<<2))
#define PDPU              SFR_AT (SPRGROUP_SFR0 + (0x022<<2))
#define PEPU              SFR_AT (SPRGROUP_SFR0 + (0x023<<2))
#define PFPU              SFR_AT (SPRGROUP_SFR0 + (0x024<<2))
#define PGPU              SFR_AT (SPRGROUP_SFR0 + (0x025<<2))
#define PHPU              SFR_AT (SPRGROUP_SFR0 + (0x026<<2))
#define PJPU              SFR_AT (SPRGROUP_SFR0 + (0x027<<2))

//.................... 0x03x .........................
#define PAPD              SFR_AT (SPRGROUP_SFR0 + (0x030<<2))
#define PBPD              SFR_AT (SPRGROUP_SFR0 + (0x031<<2))
#define PDPD              SFR_AT (SPRGROUP_SFR0 + (0x032<<2))
#define PEPD              SFR_AT (SPRGROUP_SFR0 + (0x033<<2))
#define PFPD              SFR_AT (SPRGROUP_SFR0 + (0x034<<2))
#define PGPD              SFR_AT (SPRGROUP_SFR0 + (0x035<<2))
#define PHPD              SFR_AT (SPRGROUP_SFR0 + (0x036<<2))
#define PJPD              SFR_AT (SPRGROUP_SFR0 + (0x037<<2))

//.................... 0x04x .........................
#define PAFMAP            SFR_AT (SPRGROUP_SFR0 + (0x040<<2))
#define PBFMAP            SFR_AT (SPRGROUP_SFR0 + (0x041<<2))
#define PDFMAP            SFR_AT (SPRGROUP_SFR0 + (0x042<<2))
#define PEFMAP            SFR_AT (SPRGROUP_SFR0 + (0x043<<2))
#define PFFMAP            SFR_AT (SPRGROUP_SFR0 + (0x044<<2))
#define PGFMAP            SFR_AT (SPRGROUP_SFR0 + (0x045<<2))
#define PHFMAP            SFR_AT (SPRGROUP_SFR0 + (0x046<<2))
#define PJFMAP            SFR_AT (SPRGROUP_SFR0 + (0x047<<2))

//.................... 0x05x .........................
#define PADRV             SFR_AT (SPRGROUP_SFR0 + (0x050<<2))
#define PBDRV             SFR_AT (SPRGROUP_SFR0 + (0x051<<2))
#define PDDRV             SFR_AT (SPRGROUP_SFR0 + (0x052<<2))
#define PEDRV             SFR_AT (SPRGROUP_SFR0 + (0x053<<2))
#define PFDRV             SFR_AT (SPRGROUP_SFR0 + (0x054<<2))
#define PGDRV             SFR_AT (SPRGROUP_SFR0 + (0x055<<2))
#define PHDRV             SFR_AT (SPRGROUP_SFR0 + (0x056<<2))
#define PJDRV             SFR_AT (SPRGROUP_SFR0 + (0x057<<2))

//.................... 0x06x .........................
#define PANHL             SFR_AT (SPRGROUP_SFR0 + (0x060<<2))
#define PBNHL             SFR_AT (SPRGROUP_SFR0 + (0x061<<2))
#define PDNHL             SFR_AT (SPRGROUP_SFR0 + (0x062<<2))
#define PENHL             SFR_AT (SPRGROUP_SFR0 + (0x063<<2))
#define PFNHL             SFR_AT (SPRGROUP_SFR0 + (0x064<<2))
#define PGNHL             SFR_AT (SPRGROUP_SFR0 + (0x065<<2))
#define PHNHL             SFR_AT (SPRGROUP_SFR0 + (0x066<<2))
#define PJNHL             SFR_AT (SPRGROUP_SFR0 + (0x067<<2))

//.................... 0x07x .........................
#define PAPULED           SFR_AT (SPRGROUP_SFR0 + (0x070<<2))
#define PBPULED           SFR_AT (SPRGROUP_SFR0 + (0x071<<2))
#define PDPULED           SFR_AT (SPRGROUP_SFR0 + (0x072<<2))
#define PEPULED           SFR_AT (SPRGROUP_SFR0 + (0x073<<2))
#define PFPULED           SFR_AT (SPRGROUP_SFR0 + (0x074<<2))
#define PGPULED           SFR_AT (SPRGROUP_SFR0 + (0x075<<2))
#define PHPULED           SFR_AT (SPRGROUP_SFR0 + (0x076<<2))
#define PJPULED           SFR_AT (SPRGROUP_SFR0 + (0x077<<2))

//.................... 0x08x .........................
#define PAPDLED           SFR_AT (SPRGROUP_SFR0 + (0x080<<2))
#define PBPDLED           SFR_AT (SPRGROUP_SFR0 + (0x081<<2))
#define PDPDLED           SFR_AT (SPRGROUP_SFR0 + (0x082<<2))
#define PEPDLED           SFR_AT (SPRGROUP_SFR0 + (0x083<<2))
#define PFPDLED           SFR_AT (SPRGROUP_SFR0 + (0x084<<2))
#define PGPDLED           SFR_AT (SPRGROUP_SFR0 + (0x085<<2))
#define PHPDLED           SFR_AT (SPRGROUP_SFR0 + (0x086<<2))
#define PJPDLED           SFR_AT (SPRGROUP_SFR0 + (0x087<<2))

//.................... 0x09x .........................
#define PADRVH            SFR_AT (SPRGROUP_SFR0 + (0x090<<2))
#define PBDRVH            SFR_AT (SPRGROUP_SFR0 + (0x091<<2))
#define PDDRVH            SFR_AT (SPRGROUP_SFR0 + (0x092<<2))
#define PEDRVH            SFR_AT (SPRGROUP_SFR0 + (0x093<<2))
#define PFDRVH            SFR_AT (SPRGROUP_SFR0 + (0x094<<2))
#define PGDRVH            SFR_AT (SPRGROUP_SFR0 + (0x095<<2))
#define PHDRVH            SFR_AT (SPRGROUP_SFR0 + (0x096<<2))
#define PJDRVH            SFR_AT (SPRGROUP_SFR0 + (0x097<<2))

//.................... 0x0ax .........................
#define PSRAMIOCON0		SFR_AT (SPRGROUP_SFR0 + (0x0a0 <<2))
#define PSRAMIOCON1		SFR_AT (SPRGROUP_SFR0 + (0x0a1 <<2))
#define PSRAMIOCON2		SFR_AT (SPRGROUP_SFR0 + (0x0a2 <<2))
#define PSRAMIOCON3		SFR_AT (SPRGROUP_SFR0 + (0x0a3 <<2))
#define PSRAMIOCON4		SFR_AT (SPRGROUP_SFR0 + (0x0a4 <<2))
#define PSRAMIOCON5		SFR_AT (SPRGROUP_SFR0 + (0x0a5 <<2))
#define PSRAMIO				SFR_AT (SPRGROUP_SFR0 + (0x0a6 <<2))


//.................... 0x0bx .........................
//.................... 0x0cx .........................
//.................... 0x0dx .........................
#define PMAPCON0          SFR_AT (SPRGROUP_SFR0 + (0x0d0<<2))
#define PMAPCON1          SFR_AT (SPRGROUP_SFR0 + (0x0d1<<2))
#define PMAPCON2          SFR_AT (SPRGROUP_SFR0 + (0x0d2<<2))
#define PMAPCON3          SFR_AT (SPRGROUP_SFR0 + (0x0d3<<2))
#define PMAPCON4          SFR_AT (SPRGROUP_SFR0 + (0x0d4<<2))

//.................... 0x0ex .........................
#define PADIE             SFR_AT (SPRGROUP_SFR0 + (0x0e0<<2))
#define PBDIE             SFR_AT (SPRGROUP_SFR0 + (0x0e1<<2))
#define PDDIE             SFR_AT (SPRGROUP_SFR0 + (0x0e2<<2))
#define PEDIE             SFR_AT (SPRGROUP_SFR0 + (0x0e3<<2))
#define PFDIE             SFR_AT (SPRGROUP_SFR0 + (0x0e4<<2))
#define PGDIE             SFR_AT (SPRGROUP_SFR0 + (0x0e5<<2))
#define PHDIE             SFR_AT (SPRGROUP_SFR0 + (0x0e6<<2))
#define PJDIE             SFR_AT (SPRGROUP_SFR0 + (0x0e7<<2))

//.................... 0x0fx .........................
#define PWKEN             SFR_AT (SPRGROUP_SFR0 + (0x0f0<<2))
#define PWKEDG            SFR_AT (SPRGROUP_SFR0 + (0x0f1<<2))
#define PWKLEV            SFR_AT (SPRGROUP_SFR0 + (0x0f2<<2))
#define PWKPND            SFR_AT (SPRGROUP_SFR0 + (0x0f3<<2))

/////////////////////////////////////////////////////////
// Group 1
/////////////////////////////////////////////////////////
#define SFR_1_TEST             SFR_AT (SPRGROUP_SFR1 + (0x000 <<2))
//.................... 0x00x .........................
#define CHIP_ID           SFR_AT (SPRGROUP_SFR1 + (0x000<<2))
#define TESTDATA          SFR_AT (SPRGROUP_SFR1 + (0x001<<2))
#define TESTMODE          SFR_AT (SPRGROUP_SFR1 + (0x002<<2))
#define WDTCON            SFR_AT (SPRGROUP_SFR1 + (0x003<<2))
#define MCLKCON           SFR_AT (SPRGROUP_SFR1 + (0x004<<2))
#define IO1D1CON          SFR_AT (SPRGROUP_SFR1 + (0x005<<2))
#define DLLCON            SFR_AT (SPRGROUP_SFR1 + (0x00E<<2))
#define DLLCON1           SFR_AT (SPRGROUP_SFR1 + (0x00F<<2))

//.................... 0x01x .........................
#define PCON0             SFR_AT (SPRGROUP_SFR1 + (0x010<<2))
#define PCON1             SFR_AT (SPRGROUP_SFR1 + (0x011<<2))
#define CLKCON0           SFR_AT (SPRGROUP_SFR1 + (0x012<<2))
#define CLKCON1           SFR_AT (SPRGROUP_SFR1 + (0x013<<2))
#define CLKCON2           SFR_AT (SPRGROUP_SFR1 + (0x014<<2))
#define CLKTUN            SFR_AT (SPRGROUP_SFR1 + (0x015<<2))
#define PLL0CON           SFR_AT (SPRGROUP_SFR1 + (0x016<<2))
#define PLL0DIV           SFR_AT (SPRGROUP_SFR1 + (0x017<<2))
#define PLL1CON           SFR_AT (SPRGROUP_SFR1 + (0x018<<2))
#define PLL1DIV           SFR_AT (SPRGROUP_SFR1 + (0x019<<2))
#define LDOCON            SFR_AT (SPRGROUP_SFR1 + (0x01A<<2))
#define DACACON0          SFR_AT (SPRGROUP_SFR1 + (0x01B<<2))
#define DACACON1          SFR_AT (SPRGROUP_SFR1 + (0x01C<<2))
#define DACACON2          SFR_AT (SPRGROUP_SFR1 + (0x01D<<2))
#define SPMODE            SFR_AT (SPRGROUP_SFR1 + (0x01E<<2))
#define LCDTCON           SFR_AT (SPRGROUP_SFR1 + (0x01F<<2))

//.................... 0x02x .........................
#define RSTCON            SFR_AT (SPRGROUP_SFR1 + (0x020<<2))
#define SDRACON0          SFR_AT (SPRGROUP_SFR1 + (0x021<<2))
#define PRAMCCON0         SFR_AT (SPRGROUP_SFR1 + (0x021<<2))	// psram equ sdram
#define SDRACON1          SFR_AT (SPRGROUP_SFR1 + (0x022<<2))
#define SDRACON2          SFR_AT (SPRGROUP_SFR1 + (0x023<<2))
#define SDRDLL0           SFR_AT (SPRGROUP_SFR1 + (0x024<<2))

//.................... 0x03x .........................
#define LFSRCON           SFR_AT (SPRGROUP_SFR1 + (0x030<<2))
#define LFSRFIFO          SFR_AT (SPRGROUP_SFR1 + (0x031<<2))
#define LFSRPOLY          SFR_AT (SPRGROUP_SFR1 + (0x032<<2))
#define CRCFIFO           SFR_AT (SPRGROUP_SFR1 + (0x033<<2))
#define CRCRES            SFR_AT (SPRGROUP_SFR1 + (0x034<<2))
#define BE2LE             SFR_AT (SPRGROUP_SFR1 + (0x035<<2))
#define HFWBE2LE          SFR_AT (SPRGROUP_SFR1 + (0x036<<2))
#define LVDCON            SFR_AT (SPRGROUP_SFR1 + (0x037<<2))
#define RTCCON            SFR_AT (SPRGROUP_SFR1 + (0x038<<2))
#define RTCCPND           SFR_AT (SPRGROUP_SFR1 + (0x039<<2))
#define RTCSECCNT         SFR_AT (SPRGROUP_SFR1 + (0x03a<<2))
#define RTCDATA           SFR_AT (SPRGROUP_SFR1 + (0x03b<<2))

//.................... 0x04x .........................
#define MCP0CON            SFR_AT (SPRGROUP_SFR1 + (0x040<<2))
#define MCP0SCADDR         SFR_AT (SPRGROUP_SFR1 + (0x041<<2))
#define MCP0TGADDR         SFR_AT (SPRGROUP_SFR1 + (0x042<<2))
#define MCP0LENGTH         SFR_AT (SPRGROUP_SFR1 + (0x043<<2))
#define MCP1CON            SFR_AT (SPRGROUP_SFR1 + (0x044<<2))
#define MCP1SCADDR         SFR_AT (SPRGROUP_SFR1 + (0x045<<2))
#define MCP1TGADDR         SFR_AT (SPRGROUP_SFR1 + (0x046<<2))
#define MCP1LENGTH         SFR_AT (SPRGROUP_SFR1 + (0x047<<2))
#define MCP2CON            SFR_AT (SPRGROUP_SFR1 + (0x048<<2))
#define MCP2SCADDR         SFR_AT (SPRGROUP_SFR1 + (0x049<<2))
#define MCP2TGADDR         SFR_AT (SPRGROUP_SFR1 + (0x04a<<2))
#define MCP2LENGTH         SFR_AT (SPRGROUP_SFR1 + (0x04b<<2))
#define MON_MEM_EN         SFR_AT (SPRGROUP_SFR1 + (0x04c<<2))
#define MON_MEM_PND        SFR_AT (SPRGROUP_SFR1 + (0x04d<<2))
//.................... 0x05x .........................
#define MBISTCON0         SFR_AT (SPRGROUP_SFR1 + (0x050<<2))
#define MBISTCON1         SFR_AT (SPRGROUP_SFR1 + (0x051<<2))
#define MBISTADDRST       SFR_AT (SPRGROUP_SFR1 + (0x052<<2))
#define MBISTADDREND      SFR_AT (SPRGROUP_SFR1 + (0x053<<2))
#define MBISTBMASK        SFR_AT (SPRGROUP_SFR1 + (0x054<<2))
#define MBISTCRCRES       SFR_AT (SPRGROUP_SFR1 + (0x055<<2))
#define MBISTCRCEXP       SFR_AT (SPRGROUP_SFR1 + (0x056<<2))

//...................0x06x...............................
#define PICCON            SFR_AT (SPRGROUP_SFR1 + (0x060<<2))
#define PICCOOR           SFR_AT (SPRGROUP_SFR1 + (0x061<<2))
#define PICJPGSIZE        SFR_AT (SPRGROUP_SFR1 + (0x062<<2))
#define ROTATECON         SFR_AT (SPRGROUP_SFR1 + (0x063<<2))     
#define ROTSRCYADR        SFR_AT (SPRGROUP_SFR1 + (0x064<<2))     
#define ROTSRCUVADR       SFR_AT (SPRGROUP_SFR1 + (0x065<<2))  
#define ROTTGYADR         SFR_AT (SPRGROUP_SFR1 + (0x066<<2))   
#define ROTTGUVADR        SFR_AT (SPRGROUP_SFR1 + (0x067<<2))  
#define ROTIMGSIZE        SFR_AT (SPRGROUP_SFR1 + (0x068<<2)) 
#define ROTADDRSTRI       SFR_AT (SPRGROUP_SFR1 + (0x069<<2)) 
#define ROTATESTA         SFR_AT (SPRGROUP_SFR1 + (0x06a<<2)) 

/////////////////////////////////////////////////////////
// Group 2
/////////////////////////////////////////////////////////
#define SFR_2_TEST             SFR_AT (SPRGROUP_SFR2 + (0x000 <<2))
//.................... 0x00x .........................
#define SPIFGCON          SFR_AT (SPRGROUP_SFR2 + (0x000<<2))
#define SPIFFACON         SFR_AT (SPRGROUP_SFR2 + (0x001<<2))
#define SPIFPACON         SFR_AT (SPRGROUP_SFR2 + (0x002<<2))
#define SPIFDCON          SFR_AT (SPRGROUP_SFR2 + (0x003<<2))

#define SPI1CON           SFR_AT (SPRGROUP_SFR2 + (0x004<<2))
#define SPI1BUF           SFR_AT (SPRGROUP_SFR2 + (0x005<<2))
#define SPI1BAUD          SFR_AT (SPRGROUP_SFR2 + (0x006<<2))
#define SPI1DMACNT        SFR_AT (SPRGROUP_SFR2 + (0x007<<2))
#define SPI1DMAADR        SFR_AT (SPRGROUP_SFR2 + (0x008<<2))

#define EMICON            SFR_AT (SPRGROUP_SFR2 + (0x009<<2))
#define EMIHEADER         SFR_AT (SPRGROUP_SFR2 + (0x00a<<2))
#define EMIBAUD           SFR_AT (SPRGROUP_SFR2 + (0x00b<<2))
#define EMIPND            SFR_AT (SPRGROUP_SFR2 + (0x00c<<2))
#define EMISTA            SFR_AT (SPRGROUP_SFR2 + (0x00d<<2))
#define EMIDMACNT         SFR_AT (SPRGROUP_SFR2 + (0x00e<<2))
#define EMIDMAADR         SFR_AT (SPRGROUP_SFR2 + (0x00f<<2))

//.................... 0x01x .........................
#define BUCON             SFR_AT (SPRGROUP_SFR2 + (0x010<<2))
#define BUBAUD            SFR_AT (SPRGROUP_SFR2 + (0x011<<2))
#define BUDATA            SFR_AT (SPRGROUP_SFR2 + (0x012<<2))
#define BUSTA             SFR_AT (SPRGROUP_SFR2 + (0x013<<2))
#define BUDMATXADR        SFR_AT (SPRGROUP_SFR2 + (0x014<<2))
#define BUDMARXADR        SFR_AT (SPRGROUP_SFR2 + (0x015<<2))
#define BUCPUPOUT         SFR_AT (SPRGROUP_SFR2 + (0x016<<2))
#define BUDMACNT          SFR_AT (SPRGROUP_SFR2 + (0x017<<2))
#define BUDMATXCNT        SFR_AT (SPRGROUP_SFR2 + (0x018<<2))
#define BULOOPCNT         SFR_AT (SPRGROUP_SFR2 + (0x019<<2))
#define BUMINUS           SFR_AT (SPRGROUP_SFR2 + (0x01a<<2))

//.................... 0x02x .........................
#define UART0CON          SFR_AT (SPRGROUP_SFR2 + (0x020<<2))
#define UART0BAUD         SFR_AT (SPRGROUP_SFR2 + (0x021<<2))
#define UART0DAT          SFR_AT (SPRGROUP_SFR2 + (0x022<<2))
#define UART0PND          SFR_AT (SPRGROUP_SFR2 + (0x023<<2))
#define IIC0BAUD          SFR_AT (SPRGROUP_SFR2 + (0x024<<2))
#define IIC0CON           SFR_AT (SPRGROUP_SFR2 + (0x025<<2))
#define IIC0RDATA         SFR_AT (SPRGROUP_SFR2 + (0x026<<2))
#define IIC0TDATA         SFR_AT (SPRGROUP_SFR2 + (0x027<<2))
#define IIC0CMD           SFR_AT (SPRGROUP_SFR2 + (0x028<<2))
#define IIC0STA           SFR_AT (SPRGROUP_SFR2 + (0x029<<2))

//.................... 0x03x .........................
//#define IRCON0            SFR_AT (SPRGROUP_SFR2 + (0x030<<2))
//#define IRCON1            SFR_AT (SPRGROUP_SFR2 + (0x031<<2))
//#define IRDATA            SFR_AT (SPRGROUP_SFR2 + (0x032<<2))
#define IIC1BAUD          SFR_AT (SPRGROUP_SFR2 + (0x033<<2))
#define IIC1CON           SFR_AT (SPRGROUP_SFR2 + (0x034<<2))
#define IIC1RDATA         SFR_AT (SPRGROUP_SFR2 + (0x035<<2))
#define IIC1TDATA         SFR_AT (SPRGROUP_SFR2 + (0x036<<2))
#define IIC1CMD           SFR_AT (SPRGROUP_SFR2 + (0x037<<2))
#define IIC1STA           SFR_AT (SPRGROUP_SFR2 + (0x038<<2))

#define AGCPWR            SFR_AT (SPRGROUP_SFR2 + (0x03a<<2))

//.................... 0x04x .........................
#define AUADCCFG          SFR_AT (SPRGROUP_SFR2 + (0x040<<2))
#define AUADCBAUD         SFR_AT (SPRGROUP_SFR2 + (0x041<<2))
#define AUADCBADR         SFR_AT (SPRGROUP_SFR2 + (0x042<<2))
#define AUADCDCNT         SFR_AT (SPRGROUP_SFR2 + (0x043<<2))
#define AUADCGAIN         SFR_AT (SPRGROUP_SFR2 + (0x044<<2))
#define AUADCSTA          SFR_AT (SPRGROUP_SFR2 + (0x045<<2))
#define AGCCFG            SFR_AT (SPRGROUP_SFR2 + (0x046<<2))
#define AGCLVL            SFR_AT (SPRGROUP_SFR2 + (0x047<<2))
#define AGCDRC            SFR_AT (SPRGROUP_SFR2 + (0x048<<2))
#define AGCTEST           SFR_AT (SPRGROUP_SFR2 + (0x049<<2))

#define SDRMTPR           SFR_AT (SPRGROUP_SFR2 + (0x04a<<2))
#define SDRMTCNT          SFR_AT (SPRGROUP_SFR2 + (0x04b<<2))
#define SDRMTCON          SFR_AT (SPRGROUP_SFR2 + (0x04c<<2))
#define SDRBZCNT          SFR_AT (SPRGROUP_SFR2 + (0x04d<<2))
#define SDRRVDCNT         SFR_AT (SPRGROUP_SFR2 + (0x04e<<2))
#define SDRWVDCNT         SFR_AT (SPRGROUP_SFR2 + (0x04f<<2))

//.................... 0x05x .........................
#define DACOBCON          SFR_AT (SPRGROUP_SFR2 + (0x050<<2))
#define DACOBSIZE         SFR_AT (SPRGROUP_SFR2 + (0x051<<2))
#define DACOBSTADR        SFR_AT (SPRGROUP_SFR2 + (0x052<<2))
#define DACOBSYNC         SFR_AT (SPRGROUP_SFR2 + (0x053<<2))
#define DACOBSTA          SFR_AT (SPRGROUP_SFR2 + (0x054<<2))
#define DACFIFOSPD        SFR_AT (SPRGROUP_SFR2 + (0x055<<2))
#define DACFIFOCNT        SFR_AT (SPRGROUP_SFR2 + (0x056<<2))
#define DACADR            SFR_AT (SPRGROUP_SFR2 + (0x057<<2))
#define DACDAT            SFR_AT (SPRGROUP_SFR2 + (0x058<<2))
#define ADCCON            SFR_AT (SPRGROUP_SFR2 + (0x05c<<2))
#define ADCBAUD           SFR_AT (SPRGROUP_SFR2 + (0x05d<<2))
#define ADCDAT            SFR_AT (SPRGROUP_SFR2 + (0x05e<<2))

//.................... 0x06x .........................
#define CH0VCNT         SFR_AT (SPRGROUP_SFR2 + (0x060<<2))
#define CH1VCNT         SFR_AT (SPRGROUP_SFR2 + (0x061<<2))
#define CH2VCNT         SFR_AT (SPRGROUP_SFR2 + (0x062<<2))
#define CH3VCNT         SFR_AT (SPRGROUP_SFR2 + (0x063<<2))
#define CH4VCNT         SFR_AT (SPRGROUP_SFR2 + (0x064<<2))
#define CH5VCNT         SFR_AT (SPRGROUP_SFR2 + (0x065<<2))
#define CH6VCNT         SFR_AT (SPRGROUP_SFR2 + (0x066<<2))
#define CH7VCNT         SFR_AT (SPRGROUP_SFR2 + (0x067<<2))
#define CH8VCNT         SFR_AT (SPRGROUP_SFR2 + (0x068<<2))
#define CH9VCNT         SFR_AT (SPRGROUP_SFR2 + (0x069<<2))
#define CH10VCNT        SFR_AT (SPRGROUP_SFR2 + (0x06a<<2))
#define CH11VCNT        SFR_AT (SPRGROUP_SFR2 + (0x06b<<2))
#define CH12VCNT        SFR_AT (SPRGROUP_SFR2 + (0x06c<<2))
#define CH13VCNT        SFR_AT (SPRGROUP_SFR2 + (0x06d<<2))
#define CH14VCNT        SFR_AT (SPRGROUP_SFR2 + (0x06e<<2))
#define CH15VCNT        SFR_AT (SPRGROUP_SFR2 + (0x06f<<2))

//.................... 0x07x .........................
#define CH0BWCON         SFR_AT (SPRGROUP_SFR2 + (0x070<<2))
#define CH1BWCON         SFR_AT (SPRGROUP_SFR2 + (0x071<<2))
#define CH2BWCON         SFR_AT (SPRGROUP_SFR2 + (0x072<<2))
#define CH3BWCON         SFR_AT (SPRGROUP_SFR2 + (0x073<<2))
#define CH4BWCON         SFR_AT (SPRGROUP_SFR2 + (0x074<<2))
#define CH5BWCON         SFR_AT (SPRGROUP_SFR2 + (0x075<<2))
#define CH6BWCON         SFR_AT (SPRGROUP_SFR2 + (0x076<<2))
#define CH7BWCON         SFR_AT (SPRGROUP_SFR2 + (0x077<<2))
#define CH8BWCON         SFR_AT (SPRGROUP_SFR2 + (0x078<<2))
#define CH9BWCON         SFR_AT (SPRGROUP_SFR2 + (0x079<<2))
#define CH10BWCON        SFR_AT (SPRGROUP_SFR2 + (0x07a<<2))
#define CH11BWCON        SFR_AT (SPRGROUP_SFR2 + (0x07b<<2))
#define CH12BWCON        SFR_AT (SPRGROUP_SFR2 + (0x07c<<2))
#define CH13BWCON        SFR_AT (SPRGROUP_SFR2 + (0x07d<<2))
#define CH14BWCON        SFR_AT (SPRGROUP_SFR2 + (0x07e<<2))
#define CH15BWCON        SFR_AT (SPRGROUP_SFR2 + (0x07f<<2))

//.................... 0x08x .........................
#define CHBWCON0         SFR_AT (SPRGROUP_SFR2 + (0x080<<2))
#define CHBWCON1         SFR_AT (SPRGROUP_SFR2 + (0x081<<2))
#define CPUBWCON         SFR_AT (SPRGROUP_SFR2 + (0x082<<2))
#define LSWBWCON         SFR_AT (SPRGROUP_SFR2 + (0x083<<2))
#define LSRBWCON         SFR_AT (SPRGROUP_SFR2 + (0x084<<2))
#define HSWBWCON         SFR_AT (SPRGROUP_SFR2 + (0x085<<2))
#define HSRBWCON         SFR_AT (SPRGROUP_SFR2 + (0x086<<2))
#define HSLSBWCON        SFR_AT (SPRGROUP_SFR2 + (0x087<<2))
#define CHHPRI           SFR_AT (SPRGROUP_SFR2 + (0x088<<2))
#define CHDNCON0         SFR_AT (SPRGROUP_SFR2 + (0x089<<2))
#define CHDNCON1         SFR_AT (SPRGROUP_SFR2 + (0x08a<<2))
#define CHDNCON2         SFR_AT (SPRGROUP_SFR2 + (0x08b<<2))
#define CHDNCON3         SFR_AT (SPRGROUP_SFR2 + (0x08c<<2))
#define CHDNCON4         SFR_AT (SPRGROUP_SFR2 + (0x08d<<2))
#define CHDNCON5         SFR_AT (SPRGROUP_SFR2 + (0x08e<<2))
#define CHDMACNCL        SFR_AT (SPRGROUP_SFR2 + (0x08f<<2))

//.................... 0x09x .........................
#define CHFBEN           SFR_AT (SPRGROUP_SFR2 + (0x090<<2))
#define SPIFACT          SFR_AT (SPRGROUP_SFR2 + (0x091<<2))
#define SPIFSTA          SFR_AT (SPRGROUP_SFR2 + (0x092<<2))
#define SPIFBAUD         SFR_AT (SPRGROUP_SFR2 + (0x093<<2))
#define SPIFDBUF         SFR_AT (SPRGROUP_SFR2 + (0x094<<2))
#define SPIFDCNT         SFR_AT (SPRGROUP_SFR2 + (0x095<<2))
#define SPIFBADR         SFR_AT (SPRGROUP_SFR2 + (0x096<<2))
#define SPIFSADR         SFR_AT (SPRGROUP_SFR2 + (0x097<<2))
#define SPIFCRC          SFR_AT (SPRGROUP_SFR2 + (0x098<<2))
#define SPIFPOLY         SFR_AT (SPRGROUP_SFR2 + (0x099<<2))
#define SPIFLFSR         SFR_AT (SPRGROUP_SFR2 + (0x09a<<2))
#define SPIFPAMSK        SFR_AT (SPRGROUP_SFR2 + (0x09b<<2))
#define SPIFFAMSK        SFR_AT (SPRGROUP_SFR2 + (0x09c<<2))
#define SPIFFACMD        SFR_AT (SPRGROUP_SFR2 + (0x09d<<2))
#define SPIFPACMD        SFR_AT (SPRGROUP_SFR2 + (0x09e<<2))
#define SPIFDCMD         SFR_AT (SPRGROUP_SFR2 + (0x09f<<2))

//.................... 0x0Ax .........................
#define  NOC_REG0       SFR_AT (SPRGROUP_SFR2 + (0x0A0<<2)) 
#define  NOC_REG1       SFR_AT (SPRGROUP_SFR2 + (0x0A1<<2)) 
#define  NOC_REG2       SFR_AT (SPRGROUP_SFR2 + (0x0A2<<2)) 
#define  NOC_REG3       SFR_AT (SPRGROUP_SFR2 + (0x0A3<<2)) 
#define  NOC_REG4       SFR_AT (SPRGROUP_SFR2 + (0x0A4<<2)) 
#define  NOC_REG5       SFR_AT (SPRGROUP_SFR2 + (0x0A5<<2)) 
#define  NOC_REG6       SFR_AT (SPRGROUP_SFR2 + (0x0A6<<2)) 
#define  NOC_REG7       SFR_AT (SPRGROUP_SFR2 + (0x0A7<<2)) 
#define  NOC_REG8       SFR_AT (SPRGROUP_SFR2 + (0x0A8<<2)) 
#define  NOC_REG9       SFR_AT (SPRGROUP_SFR2 + (0x0A9<<2)) 
#define  NOC_REG10      SFR_AT (SPRGROUP_SFR2 + (0x0AA<<2)) 
#define  NOC_REG11      SFR_AT (SPRGROUP_SFR2 + (0x0AB<<2)) 
#define  NOC_REG12      SFR_AT (SPRGROUP_SFR2 + (0x0AC<<2)) 
#define  NOC_REG13      SFR_AT (SPRGROUP_SFR2 + (0x0AD<<2)) 
#define  NOC_REG14      SFR_AT (SPRGROUP_SFR2 + (0x0AE<<2)) 
#define  NOC_REG15      SFR_AT (SPRGROUP_SFR2 + (0x0AF<<2)) 
#define  NOC_REG16      SFR_AT (SPRGROUP_SFR2 + (0x0B0<<2)) 
#define  NOC_REG17      SFR_AT (SPRGROUP_SFR2 + (0x0B1<<2)) 
#define  NOC_REG18      SFR_AT (SPRGROUP_SFR2 + (0x0B2<<2)) 
#define  NOC_REG19      SFR_AT (SPRGROUP_SFR2 + (0x0B3<<2)) 
#define  NOC_REG20      SFR_AT (SPRGROUP_SFR2 + (0x0B4<<2)) 
#define  NOC_REG21      SFR_AT (SPRGROUP_SFR2 + (0x0B5<<2)) 
#define  NOC_REG22      SFR_AT (SPRGROUP_SFR2 + (0x0B6<<2)) 
#define  NOC_REG23      SFR_AT (SPRGROUP_SFR2 + (0x0B7<<2)) 
#define  NOC_REG24      SFR_AT (SPRGROUP_SFR2 + (0x0B8<<2)) 
#define  NOC_REG25      SFR_AT (SPRGROUP_SFR2 + (0x0B9<<2)) 
#define  NOC_REG26      SFR_AT (SPRGROUP_SFR2 + (0x0BA<<2)) 
#define  NOC_REG27      SFR_AT (SPRGROUP_SFR2 + (0x0BB<<2)) 
#define  NOC_REG28      SFR_AT (SPRGROUP_SFR2 + (0x0BC<<2)) 

/////////////////////////////////////////////////////////
// Group 3
/////////////////////////////////////////////////////////
#define SFR_3_TEST             SFR_AT (SPRGROUP_SFR3 + (0x000 <<2))
//.................... 0x00x .........................
#define TMR0CON           SFR_AT (SPRGROUP_SFR3 + (0x000<<2))
#define TMR0CNT           SFR_AT (SPRGROUP_SFR3 + (0x001<<2))
#define TMR0PR            SFR_AT (SPRGROUP_SFR3 + (0x002<<2))
#define TMR0PWM           SFR_AT (SPRGROUP_SFR3 + (0x003<<2))
#define TMR1CON           SFR_AT (SPRGROUP_SFR3 + (0x004<<2))
#define TMR1CNT           SFR_AT (SPRGROUP_SFR3 + (0x005<<2))
#define TMR1PR            SFR_AT (SPRGROUP_SFR3 + (0x006<<2))
#define TMR1PWM           SFR_AT (SPRGROUP_SFR3 + (0x007<<2))
#define TMR2CON           SFR_AT (SPRGROUP_SFR3 + (0x008<<2))
#define TMR2CNT           SFR_AT (SPRGROUP_SFR3 + (0x009<<2))
#define TMR2PR            SFR_AT (SPRGROUP_SFR3 + (0x00a<<2))
#define TMR2PWM           SFR_AT (SPRGROUP_SFR3 + (0x00b<<2))
#define TMR3CON           SFR_AT (SPRGROUP_SFR3 + (0x00c<<2))
#define TMR3CNT           SFR_AT (SPRGROUP_SFR3 + (0x00d<<2))
#define TMR3PR            SFR_AT (SPRGROUP_SFR3 + (0x00e<<2))
#define TMR3PWM           SFR_AT (SPRGROUP_SFR3 + (0x00f<<2))

//.................... 0x01x .........................
#define USB20CON0         SFR_AT (SPRGROUP_SFR3 + (0x010<<2))
#define USB20CON1         SFR_AT (SPRGROUP_SFR3 + (0x011<<2))
#define USB20CON2         SFR_AT (SPRGROUP_SFR3 + (0x012<<2))
#define USB20E0ADR        SFR_AT (SPRGROUP_SFR3 + (0x013<<2))
#define USB20E1TADR       SFR_AT (SPRGROUP_SFR3 + (0x014<<2))
#define USB20E1RADR       SFR_AT (SPRGROUP_SFR3 + (0x015<<2))
#define USB20E2TADR       SFR_AT (SPRGROUP_SFR3 + (0x016<<2))
#define USB20E2RADR       SFR_AT (SPRGROUP_SFR3 + (0x017<<2))
#define USB20E3TADR       SFR_AT (SPRGROUP_SFR3 + (0x018<<2))
#define USB20E3RADR       SFR_AT (SPRGROUP_SFR3 + (0x019<<2))
#define USB20E4TADR       SFR_AT (SPRGROUP_SFR3 + (0x01a<<2))
#define USB20E4RADR       SFR_AT (SPRGROUP_SFR3 + (0x01b<<2))
#define USB20TST0         SFR_AT (SPRGROUP_SFR3 + (0x01c<<2))
#define USB20TST1         SFR_AT (SPRGROUP_SFR3 + (0x01d<<2))
#define USB20TST_OUT      SFR_AT (SPRGROUP_SFR3 + (0x01e<<2))

//.................... 0x02x .........................
#define SDCON0            SFR_AT (SPRGROUP_SFR3 + (0x020<<2))
#define SDCON1            SFR_AT (SPRGROUP_SFR3 + (0x021<<2))
#define SDBAUD            SFR_AT (SPRGROUP_SFR3 + (0x022<<2))
#define SDCADR            SFR_AT (SPRGROUP_SFR3 + (0x023<<2))
#define SDDADR            SFR_AT (SPRGROUP_SFR3 + (0x024<<2))
#define SDDCNT            SFR_AT (SPRGROUP_SFR3 + (0x025<<2))
#define SDIDX             SFR_AT (SPRGROUP_SFR3 + (0x026<<2))
#define SDARG             SFR_AT (SPRGROUP_SFR3 + (0x027<<2))

#define TMR0CPND          SFR_AT (SPRGROUP_SFR3 + (0x02a<<2))
#define TMR1CPND          SFR_AT (SPRGROUP_SFR3 + (0x02b<<2))
#define TMR2CPND          SFR_AT (SPRGROUP_SFR3 + (0x02c<<2))
#define TMR3CPND          SFR_AT (SPRGROUP_SFR3 + (0x02d<<2))

//.................... 0x03x .........................
#define SDRCON            SFR_AT (SPRGROUP_SFR3 + (0x030<<2))
#define SDRHCMD           SFR_AT (SPRGROUP_SFR3 + (0x031<<2))
#define SDRTM             SFR_AT (SPRGROUP_SFR3 + (0x032<<2))
#define SDRMRS            SFR_AT (SPRGROUP_SFR3 + (0x033<<2))

#define SD1CON0           SFR_AT (SPRGROUP_SFR3 + (0x037<<2))
#define SD1CON1           SFR_AT (SPRGROUP_SFR3 + (0x038<<2))
#define SD1BAUD           SFR_AT (SPRGROUP_SFR3 + (0x039<<2))
#define SD1CADR           SFR_AT (SPRGROUP_SFR3 + (0x03a<<2))
#define SD1DADR           SFR_AT (SPRGROUP_SFR3 + (0x03b<<2))
#define SD1DCNT           SFR_AT (SPRGROUP_SFR3 + (0x03c<<2))
#define SD1IDX            SFR_AT (SPRGROUP_SFR3 + (0x03d<<2))
#define SD1ARG            SFR_AT (SPRGROUP_SFR3 + (0x03e<<2))

//.................... 0x04x .........................
#define TTCTL             SFR_AT (SPRGROUP_SFR3 + (0x040<<2))
#define TTPR              SFR_AT (SPRGROUP_SFR3 + (0x041<<2))
#define TTCNT             SFR_AT (SPRGROUP_SFR3 + (0x042<<2))

//.................... 0x05x .........................
#define PFDATA            SFR_AT (SPRGROUP_SFR3 + (0x050<<2))
#define PFCON             SFR_AT (SPRGROUP_SFR3 + (0x051<<2))
#define PFBAUD            SFR_AT (SPRGROUP_SFR3 + (0x052<<2))

//.................... 0x06x .........................
#define USB20E5TADR       SFR_AT (SPRGROUP_SFR3 + (0x060<<2))
#define USB20E5RADR       SFR_AT (SPRGROUP_SFR3 + (0x061<<2))
#define USB20E6TADR       SFR_AT (SPRGROUP_SFR3 + (0x062<<2))
#define USB20E6RADR       SFR_AT (SPRGROUP_SFR3 + (0x063<<2))

/////////////////////////////////////////////////////////
// Group 4
/////////////////////////////////////////////////////////
#define SFR_4_TEST             SFR_AT (SPRGROUP_SFR4 + (0x000 <<2))
//.................... 0x00x .........................
#define USB20_FAddr         SFR_AT (SPRGROUP_SFR4 + (0x000<<2))
#define USB20_Power         SFR_AT (SPRGROUP_SFR4 + (0x001<<2))
#define USB20_IntrInl       SFR_AT (SPRGROUP_SFR4 + (0x002<<2))
#define USB20_IntrInh       SFR_AT (SPRGROUP_SFR4 + (0x003<<2))
#define USB20_IntrOutl      SFR_AT (SPRGROUP_SFR4 + (0x004<<2))
#define USB20_IntrOuth      SFR_AT (SPRGROUP_SFR4 + (0x005<<2))
#define USB20_IntrInel      SFR_AT (SPRGROUP_SFR4 + (0x006<<2))
#define USB20_IntrIneh      SFR_AT (SPRGROUP_SFR4 + (0x007<<2))
#define USB20_IntrOutel     SFR_AT (SPRGROUP_SFR4 + (0x008<<2))
#define USB20_IntrOuteh     SFR_AT (SPRGROUP_SFR4 + (0x009<<2))
#define USB20_IntrUSB       SFR_AT (SPRGROUP_SFR4 + (0x00A<<2))
#define USB20_IntrUSBE      SFR_AT (SPRGROUP_SFR4 + (0x00B<<2))
#define USB20_Frame1        SFR_AT (SPRGROUP_SFR4 + (0x00C<<2))
#define USB20_Frame2        SFR_AT (SPRGROUP_SFR4 + (0x00D<<2))
#define USB20_Index         SFR_AT (SPRGROUP_SFR4 + (0x00E<<2))
#define USB20_Testmode      SFR_AT (SPRGROUP_SFR4 + (0x00F<<2))

//.................... 0x01x .........................
#define USB20_InMaxPl       SFR_AT (SPRGROUP_SFR4 + (0x010<<2))
#define USB20_InMaxPh       SFR_AT (SPRGROUP_SFR4 + (0x011<<2))
#define USB20_CSR0          SFR_AT (SPRGROUP_SFR4 + (0x012<<2))
#define USB20_CSR01         SFR_AT (SPRGROUP_SFR4 + (0x013<<2))
#define USB20_InCSR1        SFR_AT (SPRGROUP_SFR4 + (0x012<<2))
#define USB20_InCSR2        SFR_AT (SPRGROUP_SFR4 + (0x013<<2))
#define USB20_OutMaxPl      SFR_AT (SPRGROUP_SFR4 + (0x014<<2))
#define USB20_OutMaxPh      SFR_AT (SPRGROUP_SFR4 + (0x015<<2))
#define USB20_OutCSR1       SFR_AT (SPRGROUP_SFR4 + (0x016<<2))
#define USB20_OutCSR2       SFR_AT (SPRGROUP_SFR4 + (0x017<<2))
#define USB20_Count0        SFR_AT (SPRGROUP_SFR4 + (0x018<<2))
#define USB20_OutCount1     SFR_AT (SPRGROUP_SFR4 + (0x018<<2))
#define USB20_OutCount2     SFR_AT (SPRGROUP_SFR4 + (0x019<<2))
#define USB20_TxType        SFR_AT (SPRGROUP_SFR4 + (0x01A<<2))
#define USB20_NAKLimit0     SFR_AT (SPRGROUP_SFR4 + (0x01B<<2))
#define USB20_TxInterVal    SFR_AT (SPRGROUP_SFR4 + (0x01B<<2))
#define USB20_RxType        SFR_AT (SPRGROUP_SFR4 + (0x01C<<2))
#define USB20_RxInterVal    SFR_AT (SPRGROUP_SFR4 + (0x01D<<2))   

//.................... 0x02x .........................
#define USB20_FIFO0         SFR_AT (SPRGROUP_SFR4 + (0x020<<2))   /*  FIFOs for Endpoints 0 */
#define USB20_FIFO1         SFR_AT (SPRGROUP_SFR4 + (0x024<<2))   /*  FIFOs for Endpoints 1 */
#define USB20_FIFO2         SFR_AT (SPRGROUP_SFR4 + (0x028<<2))   /*  FIFOs for Endpoints 2 */
#define USB20_FIFO3         SFR_AT (SPRGROUP_SFR4 + (0x02C<<2))   /*  FIFOs for Endpoints 3 */
//              Reserved for USB SIE

//.................... 0x03x .........................
#define USB20_FIFO4         SFR_AT (SPRGROUP_SFR4 + (0x030<<2))   /*  FIFOs for Endpoints 4 */
#define USB20_FIFO5         SFR_AT (SPRGROUP_SFR4 + (0x034<<2))   /*  FIFOs for Endpoints 5 */
#define USB20_FIFO6         SFR_AT (SPRGROUP_SFR4 + (0x038<<2))   /*  FIFOs for Endpoints 6 */

//              Reserved for USB SIE

//.................... 0x04x .........................
//              Reserved for USB SIE

//.................... 0x05x .........................
//              Reserved for USB SIE

//.................... 0x06x .........................
//              Reserved for USB SIE
#define USB20_Devctl        SFR_AT (SPRGROUP_SFR4 + (0x060<<2))

//.................... 0x07x .........................
//              Reserved for USB SIE


/////////////////////////////////////////////////////////
// Group 5
/////////////////////////////////////////////////////////
#define SFR_5_TEST             SFR_AT (SPRGROUP_SFR5 + (0x000 <<2))
//.................... 0x00x .........................
#define LCDCTRL       SFR_AT (SPRGROUP_SFR5 + (0x000<<2))
#define LCDSTA        SFR_AT (SPRGROUP_SFR5 + (0x001<<2))
#define LCDPREL       SFR_AT (SPRGROUP_SFR5 + (0x002<<2))
#define LCDSCON       SFR_AT (SPRGROUP_SFR5 + (0x003<<2))
#define LCDDOE        SFR_AT (SPRGROUP_SFR5 + (0x004<<2))
#define LCDVSRP       SFR_AT (SPRGROUP_SFR5 + (0x005<<2))
#define LCDHSRP       SFR_AT (SPRGROUP_SFR5 + (0x006<<2))
#define LCDDERP       SFR_AT (SPRGROUP_SFR5 + (0x007<<2))
#define LCDVSFP       SFR_AT (SPRGROUP_SFR5 + (0x008<<2))
#define LCDHSFP       SFR_AT (SPRGROUP_SFR5 + (0x009<<2))
#define LCDDEFP       SFR_AT (SPRGROUP_SFR5 + (0x00a<<2))
#define LCDDESL       SFR_AT (SPRGROUP_SFR5 + (0x00b<<2))
#define LCDDEEL       SFR_AT (SPRGROUP_SFR5 + (0x00c<<2))
#define LCDFSTX       SFR_AT (SPRGROUP_SFR5 + (0x00d<<2))
#define LCDFSTY       SFR_AT (SPRGROUP_SFR5 + (0x00e<<2))

//.................... 0x01x .........................
#define LCDFRMW       SFR_AT (SPRGROUP_SFR5 + (0x010<<2))
#define LCDFRMH       SFR_AT (SPRGROUP_SFR5 + (0x011<<2))
#define LCDWINX       SFR_AT (SPRGROUP_SFR5 + (0x012<<2))
#define LCDWINY       SFR_AT (SPRGROUP_SFR5 + (0x013<<2))
#define LCDCST        SFR_AT (SPRGROUP_SFR5 + (0x014<<2))
#define LCDDNUM       SFR_AT (SPRGROUP_SFR5 + (0x015<<2))
#define LCDCMD        SFR_AT (SPRGROUP_SFR5 + (0x016<<2))
#define LCDDMOD       SFR_AT (SPRGROUP_SFR5 + (0x017<<2))
#define LCDENDL       SFR_AT (SPRGROUP_SFR5 + (0x018<<2))
#define LCDBLKL       SFR_AT (SPRGROUP_SFR5 + (0x019<<2))
#define LCDRGBORD     SFR_AT (SPRGROUP_SFR5 + (0x01a<<2))
#define LCDDMOD1      SFR_AT (SPRGROUP_SFR5 + (0x01b<<2))
#define LCDSPIBAUD    SFR_AT (SPRGROUP_SFR5 + (0x01c<<2))
#define LCDSPICON     SFR_AT (SPRGROUP_SFR5 + (0x01d<<2))
#define LCDSPI_WAIT   SFR_AT (SPRGROUP_SFR5 + (0x01e<<2))
//.................... 0x04x .........................

/////////////////////////////////////////////////////////
// Group 6
/////////////////////////////////////////////////////////
#define SFR_6_TEST             SFR_AT (SPRGROUP_SFR6 + (0x000 <<2))
//.................... 0x00x .........................
/////////////////////////////////////////////////////////
#define CSIMODE         SFR_AT (SPRGROUP_SFR6 + (0x000<<2))
#define ISPMODE         SFR_AT (SPRGROUP_SFR6 + (0x001<<2))
#define ISPCTRL         SFR_AT (SPRGROUP_SFR6 + (0x002<<2))
#define CSI_INTR        SFR_AT (SPRGROUP_SFR6 + (0x003<<2))
#define CSI_I2L_RUVBADR SFR_AT (SPRGROUP_SFR6 + (0x004<<2))
#define CSI_INTEN       SFR_AT (SPRGROUP_SFR6 + (0x005<<2))
#define CSI_DMA_MODE    SFR_AT (SPRGROUP_SFR6 + (0x006<<2))
#define CSI_IMAGE_SIZE  SFR_AT (SPRGROUP_SFR6 + (0x007<<2))
#define CSI_TEST_BLANK  SFR_AT (SPRGROUP_SFR6 + (0x008<<2))
#define CSI_TEST_SIZE   SFR_AT (SPRGROUP_SFR6 + (0x009<<2))
#define CSI_I2J_HCROP   SFR_AT (SPRGROUP_SFR6 + (0x00a<<2))
#define CSI_I2J_VCROP   SFR_AT (SPRGROUP_SFR6 + (0x00b<<2))
#define CSI_I2L_HCROP   SFR_AT (SPRGROUP_SFR6 + (0x00c<<2))
#define CSI_I2L_VCROP   SFR_AT (SPRGROUP_SFR6 + (0x00d<<2))
#define CSI_I2W_HCROP   SFR_AT (SPRGROUP_SFR6 + (0x00e<<2))
#define CSI_I2W_VCROP   SFR_AT (SPRGROUP_SFR6 + (0x00f<<2))

//.................... 0x01x .........................
#define CSI_I2W_YBADR   SFR_AT (SPRGROUP_SFR6 + (0x010<<2))
#define CSI_I2W_UVBADR  SFR_AT (SPRGROUP_SFR6 + (0x011<<2))
#define CSI_I2W_LBC     SFR_AT (SPRGROUP_SFR6 + (0x012<<2))
#define CSI_I2W_LBS     SFR_AT (SPRGROUP_SFR6 + (0x013<<2))
#define CSI_I2J_YBADR   SFR_AT (SPRGROUP_SFR6 + (0x014<<2))
#define CSI_I2J_UVBADR  SFR_AT (SPRGROUP_SFR6 + (0x015<<2))
#define CSI_I2J_LBC     SFR_AT (SPRGROUP_SFR6 + (0x016<<2))
#define CSI_I2J_LBS     SFR_AT (SPRGROUP_SFR6 + (0x017<<2))
#define CSI_I2L_YBADR   SFR_AT (SPRGROUP_SFR6 + (0x018<<2))
#define CSI_I2L_UVBADR  SFR_AT (SPRGROUP_SFR6 + (0x019<<2))
#define CSI_I2L_STRD    SFR_AT (SPRGROUP_SFR6 + (0x01a<<2))
#define CSI_UVC_LADR    SFR_AT (SPRGROUP_SFR6 + (0x01b<<2))
#define CSI_DVP_BADR    SFR_AT (SPRGROUP_SFR6 + (0x01c<<2))
#define CSI_STAT_BADR   SFR_AT (SPRGROUP_SFR6 + (0x01d<<2))
#define CSI_SIZE_STAT   SFR_AT (SPRGROUP_SFR6 + (0x01e<<2))
#define CSI_I2L_RYBADR  SFR_AT (SPRGROUP_SFR6 + (0x01f<<2))

//.................... 0x02x .........................
#define ISP_BLC_PARA0   SFR_AT (SPRGROUP_SFR6 + (0x020<<2))
#define ISP_BLC_PARA1   SFR_AT (SPRGROUP_SFR6 + (0x021<<2))
#define ISP_DPC_PARA0   SFR_AT (SPRGROUP_SFR6 + (0x022<<2))
#define ISP_DPC_PARA1   SFR_AT (SPRGROUP_SFR6 + (0x023<<2))
#define ISP_DPC_PARA2   SFR_AT (SPRGROUP_SFR6 + (0x024<<2))
#define ISP_DPC_PARA3   SFR_AT (SPRGROUP_SFR6 + (0x025<<2))
#define ISP_DPC_PARA4   SFR_AT (SPRGROUP_SFR6 + (0x026<<2))
#define ISP_AWB_PARA0   SFR_AT (SPRGROUP_SFR6 + (0x027<<2))
#define ISP_AWB_GAIN    SFR_AT (SPRGROUP_SFR6 + (0x028<<2))
#define ISP_AE_WINX01   SFR_AT (SPRGROUP_SFR6 + (0x029<<2))
#define ISP_AE_WINX23   SFR_AT (SPRGROUP_SFR6 + (0x02a<<2))
#define ISP_AE_WINY01   SFR_AT (SPRGROUP_SFR6 + (0x02b<<2))
#define ISP_AE_WINY23   SFR_AT (SPRGROUP_SFR6 + (0x02c<<2))
#define ISP_AE_WEIGHT0  SFR_AT (SPRGROUP_SFR6 + (0x02d<<2))
#define ISP_AE_WEIGHT1  SFR_AT (SPRGROUP_SFR6 + (0x02e<<2))
#define ISP_AE_WEIGHT2  SFR_AT (SPRGROUP_SFR6 + (0x02f<<2))

//.................... 0x03x .........................
#define ISP_AE_WEIGHT3  SFR_AT (SPRGROUP_SFR6 + (0x030<<2))
#define ISP_WP_PARA0    SFR_AT (SPRGROUP_SFR6 + (0x031<<2))
#define ISP_WP_PARA1    SFR_AT (SPRGROUP_SFR6 + (0x032<<2))
#define ISP_WP_PARA2    SFR_AT (SPRGROUP_SFR6 + (0x033<<2))
#define ISP_WP_PARA3    SFR_AT (SPRGROUP_SFR6 + (0x034<<2))
#define ISP_WP_PARA4    SFR_AT (SPRGROUP_SFR6 + (0x035<<2))
#define ISP_WP_PARA5    SFR_AT (SPRGROUP_SFR6 + (0x036<<2))
#define ISP_WP_PARA6    SFR_AT (SPRGROUP_SFR6 + (0x037<<2))
#define ISP_WP_PARA7    SFR_AT (SPRGROUP_SFR6 + (0x038<<2))
#define ISP_CCM_PARA0   SFR_AT (SPRGROUP_SFR6 + (0x039<<2))
#define ISP_CCM_PARA1   SFR_AT (SPRGROUP_SFR6 + (0x03a<<2))
#define ISP_CCM_PARA2   SFR_AT (SPRGROUP_SFR6 + (0x03b<<2))
#define ISP_CCM_PARA3   SFR_AT (SPRGROUP_SFR6 + (0x03c<<2))
#define ISP_DGAIN_PARA0 SFR_AT (SPRGROUP_SFR6 + (0x03d<<2))
#define ISP_DGAIN_PARA1 SFR_AT (SPRGROUP_SFR6 + (0x03e<<2))
#define ISP_DGAIN_PARA2 SFR_AT (SPRGROUP_SFR6 + (0x03f<<2))

//.................... 0x04x .........................
#define ISP_SHARP_PARA  SFR_AT (SPRGROUP_SFR6 + (0x040<<2))
#define ISP_SHARP_EEM0  SFR_AT (SPRGROUP_SFR6 + (0x041<<2))
#define ISP_SHARP_EEM1  SFR_AT (SPRGROUP_SFR6 + (0x042<<2))
#define ISP_SHARP_EEM2  SFR_AT (SPRGROUP_SFR6 + (0x043<<2))
#define ISP_SHARP_EEM3  SFR_AT (SPRGROUP_SFR6 + (0x044<<2))
#define ISP_SHARP_EEM4  SFR_AT (SPRGROUP_SFR6 + (0x045<<2))
#define ISP_SHARP_DNM0  SFR_AT (SPRGROUP_SFR6 + (0x046<<2))
#define ISP_SHARP_DNM1  SFR_AT (SPRGROUP_SFR6 + (0x047<<2))
#define ISP_SHARP_DNM2  SFR_AT (SPRGROUP_SFR6 + (0x048<<2))
#define ISP_SHARP_DNM3  SFR_AT (SPRGROUP_SFR6 + (0x049<<2))
#define ISP_SHARP_DNM4  SFR_AT (SPRGROUP_SFR6 + (0x04a<<2))
#define ISP_CCF_PARA    SFR_AT (SPRGROUP_SFR6 + (0x04b<<2))
#define ISP_CCF_CDM     SFR_AT (SPRGROUP_SFR6 + (0x04c<<2))
#define ISP_CH_PARA0    SFR_AT (SPRGROUP_SFR6 + (0x04d<<2))
#define ISP_CH_PARA1    SFR_AT (SPRGROUP_SFR6 + (0x04e<<2))
#define ISP_CH_PARA2    SFR_AT (SPRGROUP_SFR6 + (0x04f<<2))
                                                         
//.................... 0x05x .........................
#define ISP_CH_PARA3    SFR_AT (SPRGROUP_SFR6 + (0x050<<2))
#define ISP_CH_PARA4    SFR_AT (SPRGROUP_SFR6 + (0x051<<2))
#define ISP_CH_PARA5    SFR_AT (SPRGROUP_SFR6 + (0x052<<2))
#define ISP_CH_PARA6    SFR_AT (SPRGROUP_SFR6 + (0x053<<2))
#define ISP_CH_PARA7    SFR_AT (SPRGROUP_SFR6 + (0x054<<2))
#define ISP_CH_PARA8    SFR_AT (SPRGROUP_SFR6 + (0x055<<2))
#define ISP_CH_PARA9    SFR_AT (SPRGROUP_SFR6 + (0x056<<2))
#define ISP_VDE_PARA0   SFR_AT (SPRGROUP_SFR6 + (0x057<<2))
#define ISP_VDE_PARA1   SFR_AT (SPRGROUP_SFR6 + (0x058<<2))
#define ISP_VDE_PARA2   SFR_AT (SPRGROUP_SFR6 + (0x059<<2))
#define ISP_SAJ_PARA0   SFR_AT (SPRGROUP_SFR6 + (0x05a<<2))
#define ISP_SAJ_PARA1   SFR_AT (SPRGROUP_SFR6 + (0x05b<<2))
#define ISP_SAJ_PARA2   SFR_AT (SPRGROUP_SFR6 + (0x05c<<2))
#define ISP_MD_PARA0    SFR_AT (SPRGROUP_SFR6 + (0x05d<<2))
#define ISP_MD_PARA1    SFR_AT (SPRGROUP_SFR6 + (0x05e<<2))
#define ISP_MD_PARA2    SFR_AT (SPRGROUP_SFR6 + (0x05f<<2))

//.................... 0x06x .........................
#define ISP_LS_SIZE     SFR_AT (SPRGROUP_SFR6 + (0x060<<2))
#define ISP_LS_HSTP     SFR_AT (SPRGROUP_SFR6 + (0x061<<2))
#define ISP_LS_VSTP     SFR_AT (SPRGROUP_SFR6 + (0x062<<2))
#define ISP_LS_HTAB0    SFR_AT (SPRGROUP_SFR6 + (0x063<<2))
#define ISP_LS_HTAB1    SFR_AT (SPRGROUP_SFR6 + (0x064<<2))
#define ISP_LS_HTAB2    SFR_AT (SPRGROUP_SFR6 + (0x065<<2))
#define ISP_LS_HTAB3    SFR_AT (SPRGROUP_SFR6 + (0x066<<2))
#define ISP_LS_HTAB4    SFR_AT (SPRGROUP_SFR6 + (0x067<<2))
#define ISP_LS_HTAB5    SFR_AT (SPRGROUP_SFR6 + (0x068<<2))
#define ISP_LS_HTAB6    SFR_AT (SPRGROUP_SFR6 + (0x069<<2))
#define ISP_LS_HTAB7    SFR_AT (SPRGROUP_SFR6 + (0x06a<<2))
#define ISP_LS_HTAB8    SFR_AT (SPRGROUP_SFR6 + (0x06b<<2))
#define ISP_LS_HTAB9    SFR_AT (SPRGROUP_SFR6 + (0x06c<<2))
#define ISP_LS_HTAB10   SFR_AT (SPRGROUP_SFR6 + (0x06d<<2))
#define ISP_LS_HTAB11   SFR_AT (SPRGROUP_SFR6 + (0x06e<<2))
#define ISP_LS_HTAB12   SFR_AT (SPRGROUP_SFR6 + (0x06f<<2))

//.................... 0x07x .........................
#define ISP_LS_HTAB13   SFR_AT (SPRGROUP_SFR6 + (0x070<<2))
#define ISP_LS_HTAB14   SFR_AT (SPRGROUP_SFR6 + (0x071<<2))
#define ISP_LS_HTAB15   SFR_AT (SPRGROUP_SFR6 + (0x072<<2))
#define ISP_LS_VTAB0    SFR_AT (SPRGROUP_SFR6 + (0x073<<2))
#define ISP_LS_VTAB1    SFR_AT (SPRGROUP_SFR6 + (0x074<<2))
#define ISP_LS_VTAB2    SFR_AT (SPRGROUP_SFR6 + (0x075<<2))
#define ISP_LS_VTAB3    SFR_AT (SPRGROUP_SFR6 + (0x076<<2))
#define ISP_LS_VTAB4    SFR_AT (SPRGROUP_SFR6 + (0x077<<2))
#define ISP_LS_VTAB5    SFR_AT (SPRGROUP_SFR6 + (0x078<<2))
#define ISP_LS_VTAB6    SFR_AT (SPRGROUP_SFR6 + (0x079<<2))
#define ISP_LS_VTAB7    SFR_AT (SPRGROUP_SFR6 + (0x07a<<2))

#define FRAME_CTRL    	 SFR_AT (SPRGROUP_SFR6 + (0x07b<<2))
#define FRAME_SIZE    	 SFR_AT (SPRGROUP_SFR6 + (0x07c<<2))
#define FRAME_COORD     SFR_AT (SPRGROUP_SFR6 + (0x07d<<2))
#define FRAME_Y_THD     SFR_AT (SPRGROUP_SFR6 + (0x07e<<2))
#define FRAME_UV_THD    SFR_AT (SPRGROUP_SFR6 + (0x07f<<2))
#define FRAME_Y_ADDR    SFR_AT (SPRGROUP_SFR6 + (0x080<<2))
#define FRAME_Y_TABLE_ADDR    	 SFR_AT (SPRGROUP_SFR6 + (0x081<<2))
#define FRAME_UV_ADDR    	 	 SFR_AT (SPRGROUP_SFR6 + (0x082<<2))
#define FRAME_UV_TABLE_ADDR    SFR_AT (SPRGROUP_SFR6 + (0x083<<2))
#define FRAME_GRAM_BASE_ADDR    SFR_AT (SPRGROUP_SFR6 + (0x084<<2))
#define FRAME_STATUS    SFR_AT (SPRGROUP_SFR6 + (0x085<<2))


////////////////////////////////////////////////////////
// Group 7
/////////////////////////////////////////////////////////
#define SFR_7_TEST        SFR_AT (SPRGROUP_SFR7 + (0x000 <<2))
//.................... 0x00x .........................
#define PRE_JPCON1        SFR_AT (SPRGROUP_SFR7 + (0x000<<2))
#define PRE_JPCON2        SFR_AT (SPRGROUP_SFR7 + (0x001<<2))
#define PRE_JPCON3        SFR_AT (SPRGROUP_SFR7 + (0x002<<2))
#define PRE_JPSIZE0       SFR_AT (SPRGROUP_SFR7 + (0x003<<2))
#define PRE_JPOBAS        SFR_AT (SPRGROUP_SFR7 + (0x004<<2))
#define PRE_JPOYADR       SFR_AT (SPRGROUP_SFR7 + (0x004<<2)) // same address with JPOBAS

#define PRE_JPOEND        SFR_AT (SPRGROUP_SFR7 + (0x005<<2))
#define PRE_JPOUVADR      SFR_AT (SPRGROUP_SFR7 + (0x005<<2)) // same address with JPOEND
#define PRE_JPOPTR        SFR_AT (SPRGROUP_SFR7 + (0x006<<2))
#define PRE_JPOLTA        SFR_AT (SPRGROUP_SFR7 + (0x007<<2))
#define PRE_JPIBAS        SFR_AT (SPRGROUP_SFR7 + (0x008<<2))
#define PRE_JPIYADR       SFR_AT (SPRGROUP_SFR7 + (0x008<<2)) // same address with JPIBAS

#define PRE_JPIEND        SFR_AT (SPRGROUP_SFR7 + (0x009<<2))
#define PRE_JPIUVADR      SFR_AT (SPRGROUP_SFR7 + (0x009<<2)) // same address with JPIEND

#define PRE_JPIPTR        SFR_AT (SPRGROUP_SFR7 + (0x00a<<2))
#define PRE_JPUBIT        SFR_AT (SPRGROUP_SFR7 + (0x00b<<2))
#define PRE_JPHCNT        SFR_AT (SPRGROUP_SFR7 + (0x00c<<2))
#define PRE_JPDRI         SFR_AT (SPRGROUP_SFR7 + (0x00d<<2))
#define PRE_JPBSHADDR     SFR_AT (SPRGROUP_SFR7 + (0x00e<<2))
#define PRE_JPINTCON      SFR_AT (SPRGROUP_SFR7 + (0x00f<<2))
//.................... 0x01x .........................
#define PRE_JPFLG         SFR_AT (SPRGROUP_SFR7 + (0x010<<2))
#define PRE_JPLS          SFR_AT (SPRGROUP_SFR7 + (0x011<<2))
#define PRE_JPES          SFR_AT (SPRGROUP_SFR7 + (0x012<<2))
#define PRE_JPCOM         SFR_AT (SPRGROUP_SFR7 + (0x013<<2))
#define PRE_JPTINF        SFR_AT (SPRGROUP_SFR7 + (0x014<<2))
#define PRE_JPT0SIZE      SFR_AT (SPRGROUP_SFR7 + (0x015<<2))
#define PRE_JPT1SIZE      SFR_AT (SPRGROUP_SFR7 + (0x016<<2))
#define PRE_JPT2SIZE      SFR_AT (SPRGROUP_SFR7 + (0x017<<2))
#define PRE_JPT0POS       SFR_AT (SPRGROUP_SFR7 + (0x018<<2))
#define PRE_JPT1POS       SFR_AT (SPRGROUP_SFR7 + (0x019<<2))
#define PRE_JPT2POS       SFR_AT (SPRGROUP_SFR7 + (0x01a<<2))
#define PRE_JPT0BASE      SFR_AT (SPRGROUP_SFR7 + (0x01b<<2))
#define PRE_JPT1BASE      SFR_AT (SPRGROUP_SFR7 + (0x01c<<2))
#define PRE_JPT2BASE      SFR_AT (SPRGROUP_SFR7 + (0x01d<<2))
#define PRE_JPSIZE1       SFR_AT (SPRGROUP_SFR7 + (0x01e<<2))
#define PRE_JPXSCALE      SFR_AT (SPRGROUP_SFR7 + (0x01f<<2))

//.................... 0x02x .........................
#define PRE_JPYSCALE      SFR_AT (SPRGROUP_SFR7 + (0x020 <<2))
#define PRE_JPBSCCOEF0    SFR_AT (SPRGROUP_SFR7 + (0x021 <<2))
#define PRE_JPBSCCOEF1    SFR_AT (SPRGROUP_SFR7 + (0x022 <<2))
#define PRE_JPBSCCOEF2    SFR_AT (SPRGROUP_SFR7 + (0x023 <<2))
#define PRE_JPBSCCOEF3    SFR_AT (SPRGROUP_SFR7 + (0x024 <<2))
#define PRE_JPBSCCOEF4    SFR_AT (SPRGROUP_SFR7 + (0x025 <<2))
#define PRE_JPBSCCOEF5    SFR_AT (SPRGROUP_SFR7 + (0x026 <<2))
#define PRE_JPBSCCOEF6    SFR_AT (SPRGROUP_SFR7 + (0x027 <<2))
#define PRE_JPBSCCOEF7    SFR_AT (SPRGROUP_SFR7 + (0x028 <<2))
#define PRE_JPELINECNT    SFR_AT (SPRGROUP_SFR7 + (0x029 <<2))
#define PRE_JPDCXCROP     SFR_AT (SPRGROUP_SFR7 + (0x02a <<2))
#define PRE_JPDCYCROP     SFR_AT (SPRGROUP_SFR7 + (0x02b <<2))
#define PRE_JPFLG_CLR     SFR_AT (SPRGROUP_SFR7 + (0x02c <<2))
#define PRE_JPPAUSECNT    SFR_AT (SPRGROUP_SFR7 + (0x02d <<2))
#define PRE_JPDCYBASE     SFR_AT (SPRGROUP_SFR7 + (0x02e <<2))
#define PRE_JPDCUVBASE    SFR_AT (SPRGROUP_SFR7 + (0x02f <<2))

//.................... 0x03x .........................
#define PRE_JPDMAOUTX32   SFR_AT (SPRGROUP_SFR7 + (0x030 <<2))
#define PRE_JPBSCCOEF10       SFR_AT (SPRGROUP_SFR7 + (0x031 <<2))
#define PRE_JPBSCCOEF11       SFR_AT (SPRGROUP_SFR7 + (0x032 <<2))
#define PRE_JPBSCCOEF12       SFR_AT (SPRGROUP_SFR7 + (0x033 <<2))
#define PRE_JPBSCCOEF13       SFR_AT (SPRGROUP_SFR7 + (0x034 <<2))
#define PRE_JPBSCCOEF14       SFR_AT (SPRGROUP_SFR7 + (0x035 <<2))
#define PRE_JPBSCCOEF15       SFR_AT (SPRGROUP_SFR7 + (0x036 <<2))
#define PRE_JPBSCCOEF16       SFR_AT (SPRGROUP_SFR7 + (0x037 <<2))
#define PRE_JPBSCCOEF17       SFR_AT (SPRGROUP_SFR7 + (0x038 <<2))
#define PRE_LINE_JPBSCCOEF0   SFR_AT (SPRGROUP_SFR7 + (0x039 <<2))
#define PRE_LINE_JPBSCCOEF1   SFR_AT (SPRGROUP_SFR7 + (0x03a <<2))
#define PRE_LINE_JPBSCCOEF2   SFR_AT (SPRGROUP_SFR7 + (0x03b <<2))
#define PRE_LINE_JPBSCCOEF3   SFR_AT (SPRGROUP_SFR7 + (0x03c <<2))
#define PRE_LINE_JPBSCCOEF4   SFR_AT (SPRGROUP_SFR7 + (0x03d <<2))
#define PRE_LINE_JPBSCCOEF5   SFR_AT (SPRGROUP_SFR7 + (0x03e <<2))
#define PRE_LINE_JPBSCCOEF6   SFR_AT (SPRGROUP_SFR7 + (0x03f <<2))
//.................... 0x04x .........................
#define PRE_LINE_JPBSCCOEF7   SFR_AT (SPRGROUP_SFR7 + (0x040 <<2))
#define PRE_UVJPBSCCOEF0      SFR_AT (SPRGROUP_SFR7 + (0x041 <<2))
#define PRE_UVJPBSCCOEF1      SFR_AT (SPRGROUP_SFR7 + (0x042 <<2))
#define PRE_UVJPBSCCOEF2      SFR_AT (SPRGROUP_SFR7 + (0x043 <<2))
#define PRE_UVJPBSCCOEF3      SFR_AT (SPRGROUP_SFR7 + (0x044 <<2))
#define PRE_UVJPBSCCOEF4      SFR_AT (SPRGROUP_SFR7 + (0x045 <<2))
#define PRE_UVJPBSCCOEF5      SFR_AT (SPRGROUP_SFR7 + (0x046 <<2))
#define PRE_UVJPBSCCOEF6      SFR_AT (SPRGROUP_SFR7 + (0x047 <<2))
#define PRE_UVJPBSCCOEF7      SFR_AT (SPRGROUP_SFR7 + (0x048 <<2))
#define PRE_UVJPBSCCOEF10     SFR_AT (SPRGROUP_SFR7 + (0x049 <<2))
#define PRE_UVJPBSCCOEF11     SFR_AT (SPRGROUP_SFR7 + (0x04a <<2))
#define PRE_UVJPBSCCOEF12     SFR_AT (SPRGROUP_SFR7 + (0x04b <<2))
#define PRE_UVJPBSCCOEF13     SFR_AT (SPRGROUP_SFR7 + (0x04c <<2))
#define PRE_UVJPBSCCOEF14     SFR_AT (SPRGROUP_SFR7 + (0x04d <<2))
#define PRE_UVJPBSCCOEF15     SFR_AT (SPRGROUP_SFR7 + (0x04e <<2))
#define PRE_UVJPBSCCOEF16     SFR_AT (SPRGROUP_SFR7 + (0x04f <<2))
//.................... 0x05x .........................
#define PRE_UVJPBSCCOEF17     SFR_AT (SPRGROUP_SFR7 + (0x050 <<2))
#define PRE_DCTIMER           SFR_AT (SPRGROUP_SFR7 + (0x051 <<2))

#define PRE_JPT3SIZE      SFR_AT (SPRGROUP_SFR7 + (0x053<<2))
#define PRE_JPT3POS       SFR_AT (SPRGROUP_SFR7 + (0x054<<2))
#define PRE_JPT3BASE      SFR_AT (SPRGROUP_SFR7 + (0x055<<2))


/////////////////////////////////////////////////////////
// Group 8
/////////////////////////////////////////////////////////
#define SFR_8_TEST             SFR_AT (SPRGROUP_SFR8 + (0x000 <<2))
//.................... 0x00x .........................
#define BACK_JPCON1        SFR_AT (SPRGROUP_SFR8 + (0x000<<2))
#define BACK_JPCON2        SFR_AT (SPRGROUP_SFR8 + (0x001<<2))
#define BACK_JPCON3        SFR_AT (SPRGROUP_SFR8 + (0x002<<2))
#define BACK_JPSIZE0       SFR_AT (SPRGROUP_SFR8 + (0x003<<2))
#define BACK_JPOBAS        SFR_AT (SPRGROUP_SFR8 + (0x004<<2))
#define BACK_JPOYADR       SFR_AT (SPRGROUP_SFR8 + (0x004<<2)) // same address with JPOBAS

#define BACK_JPOEND        SFR_AT (SPRGROUP_SFR8 + (0x005<<2))
#define BACK_JPOUVADR      SFR_AT (SPRGROUP_SFR8 + (0x005<<2)) // same address with JPOEND
#define BACK_JPOPTR        SFR_AT (SPRGROUP_SFR8 + (0x006<<2))
#define BACK_JPOLTA        SFR_AT (SPRGROUP_SFR8 + (0x007<<2))
#define BACK_JPIBAS        SFR_AT (SPRGROUP_SFR8 + (0x008<<2))
#define BACK_JPIYADR       SFR_AT (SPRGROUP_SFR8 + (0x008<<2)) // same address with JPIBAS

#define BACK_JPIEND        SFR_AT (SPRGROUP_SFR8 + (0x009<<2))
#define BACK_JPIUVADR      SFR_AT (SPRGROUP_SFR8 + (0x009<<2)) // same address with JPIEND

#define BACK_JPIPTR        SFR_AT (SPRGROUP_SFR8 + (0x00a<<2))
#define BACK_JPUBIT        SFR_AT (SPRGROUP_SFR8 + (0x00b<<2))
#define BACK_JPHCNT        SFR_AT (SPRGROUP_SFR8 + (0x00c<<2))
#define BACK_JPDRI         SFR_AT (SPRGROUP_SFR8 + (0x00d<<2))
#define BACK_JPBSHADDR     SFR_AT (SPRGROUP_SFR8 + (0x00e<<2))
#define BACK_JPINTCON      SFR_AT (SPRGROUP_SFR8 + (0x00f<<2))
//.................... 0x01x .........................
#define BACK_JPFLG         SFR_AT (SPRGROUP_SFR8 + (0x010<<2))
#define BACK_JPLS          SFR_AT (SPRGROUP_SFR8 + (0x011<<2))
#define BACK_JPES          SFR_AT (SPRGROUP_SFR8 + (0x012<<2))
#define BACK_JPCOM         SFR_AT (SPRGROUP_SFR8 + (0x013<<2))
#define BACK_JPTINF        SFR_AT (SPRGROUP_SFR8 + (0x014<<2))
#define BACK_JPT0SIZE      SFR_AT (SPRGROUP_SFR8 + (0x015<<2))
#define BACK_JPT1SIZE      SFR_AT (SPRGROUP_SFR8 + (0x016<<2))
#define BACK_JPT2SIZE      SFR_AT (SPRGROUP_SFR8 + (0x017<<2))
#define BACK_JPT0POS       SFR_AT (SPRGROUP_SFR8 + (0x018<<2))
#define BACK_JPT1POS       SFR_AT (SPRGROUP_SFR8 + (0x019<<2))
#define BACK_JPT2POS       SFR_AT (SPRGROUP_SFR8 + (0x01a<<2))
#define BACK_JPT0BASE      SFR_AT (SPRGROUP_SFR8 + (0x01b<<2))
#define BACK_JPT1BASE      SFR_AT (SPRGROUP_SFR8 + (0x01c<<2))
#define BACK_JPT2BASE      SFR_AT (SPRGROUP_SFR8 + (0x01d<<2))
#define BACK_JPSIZE1       SFR_AT (SPRGROUP_SFR8 + (0x01e<<2))
#define BACK_JPXSCALE      SFR_AT (SPRGROUP_SFR8 + (0x01f<<2))

//.................... 0x02x .........................
#define BACK_JPYSCALE      SFR_AT (SPRGROUP_SFR8 + (0x020 <<2))
#define BACK_JPBSCCOEF0    SFR_AT (SPRGROUP_SFR8 + (0x021 <<2))
#define BACK_JPBSCCOEF1    SFR_AT (SPRGROUP_SFR8 + (0x022 <<2))
#define BACK_JPBSCCOEF2    SFR_AT (SPRGROUP_SFR8 + (0x023 <<2))
#define BACK_JPBSCCOEF3    SFR_AT (SPRGROUP_SFR8 + (0x024 <<2))
#define BACK_JPBSCCOEF4    SFR_AT (SPRGROUP_SFR8 + (0x025 <<2))
#define BACK_JPBSCCOEF5    SFR_AT (SPRGROUP_SFR8 + (0x026 <<2))
#define BACK_JPBSCCOEF6    SFR_AT (SPRGROUP_SFR8 + (0x027 <<2))
#define BACK_JPBSCCOEF7    SFR_AT (SPRGROUP_SFR8 + (0x028 <<2))
#define BACK_JPELINECNT    SFR_AT (SPRGROUP_SFR8 + (0x029 <<2))
#define BACK_JPDCXCROP     SFR_AT (SPRGROUP_SFR8 + (0x02a <<2))
#define BACK_JPDCYCROP     SFR_AT (SPRGROUP_SFR8 + (0x02b <<2))
#define BACK_JPFLG_CLR     SFR_AT (SPRGROUP_SFR8 + (0x02c <<2))
#define BACK_JPPAUSECNT    SFR_AT (SPRGROUP_SFR8 + (0x02d <<2))
#define BACK_JPDCYBASE     SFR_AT (SPRGROUP_SFR8 + (0x02e <<2))
#define BACK_JPDCUVBASE    SFR_AT (SPRGROUP_SFR8 + (0x02f <<2))

//.................... 0x03x .........................
#define BACK_JPDMAOUTX32       SFR_AT (SPRGROUP_SFR8 + (0x030 <<2))
#define BACK_JPBSCCOEF10       SFR_AT (SPRGROUP_SFR8 + (0x031 <<2))
#define BACK_JPBSCCOEF11       SFR_AT (SPRGROUP_SFR8 + (0x032 <<2))
#define BACK_JPBSCCOEF12       SFR_AT (SPRGROUP_SFR8 + (0x033 <<2))
#define BACK_JPBSCCOEF13       SFR_AT (SPRGROUP_SFR8 + (0x034 <<2))
#define BACK_JPBSCCOEF14       SFR_AT (SPRGROUP_SFR8 + (0x035 <<2))
#define BACK_JPBSCCOEF15       SFR_AT (SPRGROUP_SFR8 + (0x036 <<2))
#define BACK_JPBSCCOEF16       SFR_AT (SPRGROUP_SFR8 + (0x037 <<2))
#define BACK_JPBSCCOEF17       SFR_AT (SPRGROUP_SFR8 + (0x038 <<2))
#define BACK_LINE_JPBSCCOEF0   SFR_AT (SPRGROUP_SFR8 + (0x039 <<2))
#define BACK_LINE_JPBSCCOEF1   SFR_AT (SPRGROUP_SFR8 + (0x03a <<2))
#define BACK_LINE_JPBSCCOEF2   SFR_AT (SPRGROUP_SFR8 + (0x03b <<2))
#define BACK_LINE_JPBSCCOEF3   SFR_AT (SPRGROUP_SFR8 + (0x03c <<2))
#define BACK_LINE_JPBSCCOEF4   SFR_AT (SPRGROUP_SFR8 + (0x03d <<2))
#define BACK_LINE_JPBSCCOEF5   SFR_AT (SPRGROUP_SFR8 + (0x03e <<2))
#define BACK_LINE_JPBSCCOEF6   SFR_AT (SPRGROUP_SFR8 + (0x03f <<2))
//.................... 0x04x .........................
#define BACK_LINE_JPBSCCOEF7   SFR_AT (SPRGROUP_SFR8 + (0x040 <<2))
#define BACK_UVJPBSCCOEF0      SFR_AT (SPRGROUP_SFR8 + (0x041 <<2))
#define BACK_UVJPBSCCOEF1      SFR_AT (SPRGROUP_SFR8 + (0x042 <<2))
#define BACK_UVJPBSCCOEF2      SFR_AT (SPRGROUP_SFR8 + (0x043 <<2))
#define BACK_UVJPBSCCOEF3      SFR_AT (SPRGROUP_SFR8 + (0x044 <<2))
#define BACK_UVJPBSCCOEF4      SFR_AT (SPRGROUP_SFR8 + (0x045 <<2))
#define BACK_UVJPBSCCOEF5      SFR_AT (SPRGROUP_SFR8 + (0x046 <<2))
#define BACK_UVJPBSCCOEF6      SFR_AT (SPRGROUP_SFR8 + (0x047 <<2))
#define BACK_UVJPBSCCOEF7      SFR_AT (SPRGROUP_SFR8 + (0x048 <<2))
#define BACK_UVJPBSCCOEF10     SFR_AT (SPRGROUP_SFR8 + (0x049 <<2))
#define BACK_UVJPBSCCOEF11     SFR_AT (SPRGROUP_SFR8 + (0x04a <<2))
#define BACK_UVJPBSCCOEF12     SFR_AT (SPRGROUP_SFR8 + (0x04b <<2))
#define BACK_UVJPBSCCOEF13     SFR_AT (SPRGROUP_SFR8 + (0x04c <<2))
#define BACK_UVJPBSCCOEF14     SFR_AT (SPRGROUP_SFR8 + (0x04d <<2))
#define BACK_UVJPBSCCOEF15     SFR_AT (SPRGROUP_SFR8 + (0x04e <<2))
#define BACK_UVJPBSCCOEF16     SFR_AT (SPRGROUP_SFR8 + (0x04f <<2))
//.................... 0x05x .........................
#define BACK_UVJPBSCCOEF17     SFR_AT (SPRGROUP_SFR8 + (0x050 <<2))
#define BACK_DCTIMER           SFR_AT (SPRGROUP_SFR8 + (0x051 <<2))
#define BACK_DEBLOCK           SFR_AT (SPRGROUP_SFR8 + (0x052 <<2))



/////////////////////////////////////////////////////////
//// Group 9
///////////////////////////////////////////////////////////
#define SFR_9_TEST             SFR_AT (SPRGROUP_SFR9 + (0x000 <<2))
//.................... 0x000 .........................
#define DE_FRAME_CTRL              SFR_AT (SPRGROUP_SFR9 + (0x000<<2))
#define DE_CRITICAL_CTRL           SFR_AT (SPRGROUP_SFR9 + (0x001<<2))
#define DE_INT_CTRL                SFR_AT (SPRGROUP_SFR9 + (0x002<<2))
#define DE_STATUS                  SFR_AT (SPRGROUP_SFR9 + (0x003<<2))
#define DE_SCREEN_CFG              SFR_AT (SPRGROUP_SFR9 + (0x004<<2))
#define DE_BK_CFG                  SFR_AT (SPRGROUP_SFR9 + (0x005<<2))
#define DE_DISPLAY_CTRL            SFR_AT (SPRGROUP_SFR9 + (0x006<<2))
#define DE_CSC_BCS_CFG             SFR_AT (SPRGROUP_SFR9 + (0x007<<2))
#define DE_VIDEO_SIZE_CFG          SFR_AT (SPRGROUP_SFR9 + (0x008<<2))
#define DE_VIDEO_COOR_CFG          SFR_AT (SPRGROUP_SFR9 + (0x009<<2))
#define DE_VIDEO_Y_FB_CFG          SFR_AT (SPRGROUP_SFR9 + (0x00a<<2))
#define DE_VIDEO_Y_STRIDE_CFG      SFR_AT (SPRGROUP_SFR9 + (0x00b<<2))
#define DE_VIDEO_UV_FB_CFG         SFR_AT (SPRGROUP_SFR9 + (0x00c<<2))
#define DE_VIDEO_UV_STRIDE_CFG     SFR_AT (SPRGROUP_SFR9 + (0x00d<<2))
#define DE_VIDEO_SCALE_COF_CFG     SFR_AT (SPRGROUP_SFR9 + (0x00e<<2))
#define DE_OSD0_SIZE_CFG           SFR_AT (SPRGROUP_SFR9 + (0x00f<<2))
//.................... 0x010 .........................
#define DE_OSD0_COOR_CFG           SFR_AT (SPRGROUP_SFR9 + (0x010<<2))
#define DE_OSD0_FB_CFG             SFR_AT (SPRGROUP_SFR9 + (0x011<<2))
#define DE_OSD0_STRIDE_CFG         SFR_AT (SPRGROUP_SFR9 + (0x012<<2))
#define DE_OSD1_SIZE_CFG           SFR_AT (SPRGROUP_SFR9 + (0x013<<2))
#define DE_OSD1_COOR_CFG           SFR_AT (SPRGROUP_SFR9 + (0x014<<2))
#define DE_OSD1_FB_CFG             SFR_AT (SPRGROUP_SFR9 + (0x015<<2))
#define DE_OSD1_STRIDE_CFG         SFR_AT (SPRGROUP_SFR9 + (0x016<<2))
#define DE_OSD_ALPHA_CFG           SFR_AT (SPRGROUP_SFR9 + (0x017<<2))
#define DE_CP_TARGET_CFG           SFR_AT (SPRGROUP_SFR9 + (0x018<<2))
#define DE_SATCOEF0_CFG            SFR_AT (SPRGROUP_SFR9 + (0x019<<2))
#define DE_SATCOEF1_CFG            SFR_AT (SPRGROUP_SFR9 + (0x01a<<2))
#define DE_CCMC1_CFG               SFR_AT (SPRGROUP_SFR9 + (0x01b<<2))
#define DE_CCMC2_CFG               SFR_AT (SPRGROUP_SFR9 + (0x01c<<2))
#define DE_CCMC3_CFG               SFR_AT (SPRGROUP_SFR9 + (0x01d<<2))
#define DE_CCMOFFSET_CFG           SFR_AT (SPRGROUP_SFR9 + (0x01e<<2))
#define DE_VIDEO_Y_CUR_FB          SFR_AT (SPRGROUP_SFR9 + (0x01f<<2))
//.................... 0x020 .........................
#define DE_YWTABLE0L_CFG           SFR_AT (SPRGROUP_SFR9 + (0x020<<2))
#define DE_YWTABLE0H_CFG           SFR_AT (SPRGROUP_SFR9 + (0x021<<2))
#define DE_YWTABLE1L_CFG           SFR_AT (SPRGROUP_SFR9 + (0x022<<2))
#define DE_YWTABLE1H_CFG           SFR_AT (SPRGROUP_SFR9 + (0x023<<2))
#define DE_YWTABLE2L_CFG           SFR_AT (SPRGROUP_SFR9 + (0x024<<2))
#define DE_YWTABLE2H_CFG           SFR_AT (SPRGROUP_SFR9 + (0x025<<2))
#define DE_YWTABLE3L_CFG           SFR_AT (SPRGROUP_SFR9 + (0x026<<2))
#define DE_YWTABLE3H_CFG           SFR_AT (SPRGROUP_SFR9 + (0x027<<2))
#define DE_YWTABLE4L_CFG           SFR_AT (SPRGROUP_SFR9 + (0x028<<2))
#define DE_YWTABLE4H_CFG           SFR_AT (SPRGROUP_SFR9 + (0x029<<2))
#define DE_YWTABLE5L_CFG           SFR_AT (SPRGROUP_SFR9 + (0x02a<<2))
#define DE_YWTABLE5H_CFG           SFR_AT (SPRGROUP_SFR9 + (0x02b<<2))
#define DE_YWTABLE6L_CFG           SFR_AT (SPRGROUP_SFR9 + (0x02c<<2))
#define DE_YWTABLE6H_CFG           SFR_AT (SPRGROUP_SFR9 + (0x02d<<2))
#define DE_YWTABLE7L_CFG           SFR_AT (SPRGROUP_SFR9 + (0x02e<<2))
#define DE_YWTABLE7H_CFG           SFR_AT (SPRGROUP_SFR9 + (0x02f<<2))
//.................... 0x030 .........................
#define DE_YHTABLE0_CFG            SFR_AT (SPRGROUP_SFR9 + (0x030<<2))
#define DE_YHTABLE1_CFG            SFR_AT (SPRGROUP_SFR9 + (0x031<<2))
#define DE_YHTABLE2_CFG            SFR_AT (SPRGROUP_SFR9 + (0x032<<2))
#define DE_YHTABLE3_CFG            SFR_AT (SPRGROUP_SFR9 + (0x033<<2))
#define DE_YHTABLE4_CFG            SFR_AT (SPRGROUP_SFR9 + (0x034<<2))
#define DE_YHTABLE5_CFG            SFR_AT (SPRGROUP_SFR9 + (0x035<<2))
#define DE_YHTABLE6_CFG            SFR_AT (SPRGROUP_SFR9 + (0x036<<2))
#define DE_YHTABLE7_CFG            SFR_AT (SPRGROUP_SFR9 + (0x037<<2))
#define DE_UVWTABLE0L_CFG          SFR_AT (SPRGROUP_SFR9 + (0x038<<2))
#define DE_UVWTABLE0H_CFG          SFR_AT (SPRGROUP_SFR9 + (0x039<<2))
#define DE_UVWTABLE1L_CFG          SFR_AT (SPRGROUP_SFR9 + (0x03a<<2))
#define DE_UVWTABLE1H_CFG          SFR_AT (SPRGROUP_SFR9 + (0x03b<<2))
#define DE_UVWTABLE2L_CFG          SFR_AT (SPRGROUP_SFR9 + (0x03c<<2))
#define DE_UVWTABLE2H_CFG          SFR_AT (SPRGROUP_SFR9 + (0x03d<<2))
#define DE_UVWTABLE3L_CFG          SFR_AT (SPRGROUP_SFR9 + (0x03e<<2))
#define DE_UVWTABLE3H_CFG          SFR_AT (SPRGROUP_SFR9 + (0x03f<<2))
//.................... 0x040 .........................
#define DE_UVWTABLE4L_CFG          SFR_AT (SPRGROUP_SFR9 + (0x040<<2))
#define DE_UVWTABLE4H_CFG          SFR_AT (SPRGROUP_SFR9 + (0x041<<2))
#define DE_UVWTABLE5L_CFG          SFR_AT (SPRGROUP_SFR9 + (0x042<<2))
#define DE_UVWTABLE5H_CFG          SFR_AT (SPRGROUP_SFR9 + (0x043<<2))
#define DE_UVWTABLE6L_CFG          SFR_AT (SPRGROUP_SFR9 + (0x044<<2))
#define DE_UVWTABLE6H_CFG          SFR_AT (SPRGROUP_SFR9 + (0x045<<2))
#define DE_UVWTABLE7L_CFG          SFR_AT (SPRGROUP_SFR9 + (0x046<<2))
#define DE_UVWTABLE7H_CFG          SFR_AT (SPRGROUP_SFR9 + (0x047<<2))
#define DE_WSTEP_CFG               SFR_AT (SPRGROUP_SFR9 + (0x048<<2))
#define DE_HSTEP_CFG               SFR_AT (SPRGROUP_SFR9 + (0x049<<2))
#define DE_SCA_SRCSIZE_CFG         SFR_AT (SPRGROUP_SFR9 + (0x04a<<2))
#define DE_SCA_DSTSIZE_CFG         SFR_AT (SPRGROUP_SFR9 + (0x04b<<2))

/////////////////////////////////////////////////////////
// Group 10
/////////////////////////////////////////////////////////
#define SFR_10_TEST             SFR_AT (SPRGROUP_SFR10 + (0x000<<2))
//.................... 0x000 .........................
#define OSDSRCADR               SFR_AT (SPRGROUP_SFR10 + (0x000<<2))
#define OSDSRCSZ                SFR_AT (SPRGROUP_SFR10 + (0x001<<2))
#define OSDDSTADR               SFR_AT (SPRGROUP_SFR10 + (0x002<<2))
#define OSDDSTSZ                SFR_AT (SPRGROUP_SFR10 + (0x003<<2))
#define OSDDSTMSZ               SFR_AT (SPRGROUP_SFR10 + (0x004<<2))
#define OSDCMPPND               SFR_AT (SPRGROUP_SFR10 + (0x005<<2))
#define OSDCMPCON               SFR_AT (SPRGROUP_SFR10 + (0x006<<2))
//.................... 0x010 .........................
#define PMUCON                  SFR_AT (SPRGROUP_SFR10 + (0x010<<2)) 
#define PMUCON1                 SFR_AT (SPRGROUP_SFR10 + (0x011<<2)) 
#define PMUFSBAUD               SFR_AT (SPRGROUP_SFR10 + (0x012<<2)) 
#define PMUFLAG                 SFR_AT (SPRGROUP_SFR10 + (0x013<<2)) 
#define PMUREFVOL               SFR_AT (SPRGROUP_SFR10 + (0x014<<2)) 
#define PMUDBGVAL               SFR_AT (SPRGROUP_SFR10 + (0x015<<2)) 
#define PMUDBGHW                SFR_AT (SPRGROUP_SFR10 + (0x016<<2)) 
#define PMUDBGLW                SFR_AT (SPRGROUP_SFR10 + (0x017<<2)) 
#define PMUDBGPROT              SFR_AT (SPRGROUP_SFR10 + (0x018<<2)) 
#define PMUBATVOL               SFR_AT (SPRGROUP_SFR10 + (0x019<<2)) 
#define PMUBATHW                SFR_AT (SPRGROUP_SFR10 + (0x01a<<2)) 
#define PMUBATLW                SFR_AT (SPRGROUP_SFR10 + (0x01b<<2)) 
#define PMUBATPROT              SFR_AT (SPRGROUP_SFR10 + (0x01c<<2)) 
#define PMUADCCON               SFR_AT (SPRGROUP_SFR10 + (0x01d<<2)) 
#define PMUADCDAT               SFR_AT (SPRGROUP_SFR10 + (0x01e<<2)) 
//.................... 0x020 .........................
#define CSI_PHY_TIMER0          SFR_AT (SPRGROUP_SFR10 + (0x020<<2))
#define CSI_PHY_TIMER1          SFR_AT (SPRGROUP_SFR10 + (0x021<<2))
#define CSI_PHY_TIMER2          SFR_AT (SPRGROUP_SFR10 + (0x022<<2))
#define CSI_PHY_TIMER3          SFR_AT (SPRGROUP_SFR10 + (0x023<<2))
#define CSI_PHY_TIMER4          SFR_AT (SPRGROUP_SFR10 + (0x024<<2))
#define CSI_PHY_TIMER5          SFR_AT (SPRGROUP_SFR10 + (0x025<<2))
#define CSI_PHY_CTL             SFR_AT (SPRGROUP_SFR10 + (0x026<<2))
#define CSI_PHY_LANEC_CTL       SFR_AT (SPRGROUP_SFR10 + (0x027<<2))
#define CSI_PHY_LANE0_CTL       SFR_AT (SPRGROUP_SFR10 + (0x028<<2))
#define CSI_PHY_LANE1_CTL       SFR_AT (SPRGROUP_SFR10 + (0x029<<2))
#define CSI_PHY_LANE0_STA       SFR_AT (SPRGROUP_SFR10 + (0x02a<<2))
#define CSI_PHY_LANE1_STA       SFR_AT (SPRGROUP_SFR10 + (0x02b<<2))

/////////////////////////////////////////////////////////
// Group 11
/////////////////////////////////////////////////////////
#define SFR_11_TEST             SFR_AT (SPRGROUP_SFR11 + (0x000 <<2))

//.................... 0x00x .........................
#define USB11CON0               SFR_AT (SPRGROUP_SFR11 + (0x000<<2))
#define USB11CON1               SFR_AT (SPRGROUP_SFR11 + (0x001<<2))
#define USB11CON2               SFR_AT (SPRGROUP_SFR11 + (0x002<<2))
#define USB11CON3               SFR_AT (SPRGROUP_SFR11 + (0x003<<2))
#define USB11CON4               SFR_AT (SPRGROUP_SFR11 + (0x004<<2))
#define USB11DPDM               SFR_AT (SPRGROUP_SFR11 + (0x005<<2))  
#define USB11E0ADR              SFR_AT (SPRGROUP_SFR11 + (0x006<<2))
#define USB11E1TADR             SFR_AT (SPRGROUP_SFR11 + (0x007<<2))
#define USB11E1RADR             SFR_AT (SPRGROUP_SFR11 + (0x008<<2))
#define USB11E2TADR             SFR_AT (SPRGROUP_SFR11 + (0x009<<2))
#define USB11E2RADR             SFR_AT (SPRGROUP_SFR11 + (0x00a<<2))
#define USB11E3TADR             SFR_AT (SPRGROUP_SFR11 + (0x00b<<2))
#define USB11E3RADR             SFR_AT (SPRGROUP_SFR11 + (0x00c<<2))
#define USB11DATA               SFR_AT (SPRGROUP_SFR11 + (0x00d<<2))

//.................... 0x01x .........................
#define USB11MCPCON             SFR_AT (SPRGROUP_SFR11 + (0x010<<2))
#define USB11MCPPND             SFR_AT (SPRGROUP_SFR11 + (0x011<<2))
#define USB11MCPSCADR           SFR_AT (SPRGROUP_SFR11 + (0x012<<2))
#define USB11MCPTGADR           SFR_AT (SPRGROUP_SFR11 + (0x013<<2))
#define USB11MCPLENGTH          SFR_AT (SPRGROUP_SFR11 + (0x014<<2))

/////////////////////////////////////////////////////////
// Group 12 (reserve for mipi controller)
/////////////////////////////////////////////////////////
#define SFR_12_TEST             SFR_AT (SPRGROUP_SFR12 + (0x000 <<2))

//.................... 0x00x .........................
#define CSI_VERSION               SFR_AT (SPRGROUP_SFR12 + (0x000<<2))
#define CSI_N_LANES               SFR_AT (SPRGROUP_SFR12 + (0x001<<2))    
#define CSI_CSI2_RESETN           SFR_AT (SPRGROUP_SFR12 + (0x002<<2))    
#define CSI_INT_ST_MAIN           SFR_AT (SPRGROUP_SFR12 + (0x003<<2))    
#define CSI_HOST_DATA_IDS_1       SFR_AT (SPRGROUP_SFR12 + (0x004<<2))
#define CSI_PHY_SHUTDOWNZ         SFR_AT (SPRGROUP_SFR12 + (0x005<<2))    
#define CSI_DPHY_RSTZ             SFR_AT (SPRGROUP_SFR12 + (0x006<<2))    
#define CSI_PHY_RX                SFR_AT (SPRGROUP_SFR12 + (0x007<<2))    
#define CSI_PHY_STOPSTATE         SFR_AT (SPRGROUP_SFR12 + (0x008<<2))    
#define CSI_PHY_TEST_CTRL0        SFR_AT (SPRGROUP_SFR12 + (0x009<<2))    
#define CSI_PHY_TEST_CTRL1        SFR_AT (SPRGROUP_SFR12 + (0x00a<<2))    
#define CSI_IPI_MODE              SFR_AT (SPRGROUP_SFR12 + (0x00b<<2))   
#define CSI_IPI_VCID              SFR_AT (SPRGROUP_SFR12 + (0x00c<<2))    
#define CSI_IPI_DATA_TYPE         SFR_AT (SPRGROUP_SFR12 + (0x00d<<2))  
#define CSI_IPI_HSA_TIME          SFR_AT (SPRGROUP_SFR12 + (0x00e<<2))       
#define CSI_IPI_HBP_TIME          SFR_AT (SPRGROUP_SFR12 + (0x00f<<2))     
#define CSI_IPI_HSD_TIME          SFR_AT (SPRGROUP_SFR12 + (0x010<<2))         
#define CSI_IPI_HLINE_TIME        SFR_AT (SPRGROUP_SFR12 + (0x011<<2))      
#define CSI_IPI_VSA_LINES         SFR_AT (SPRGROUP_SFR12 + (0x012<<2))        
#define CSI_IPI_VBP_LINES         SFR_AT (SPRGROUP_SFR12 + (0x013<<2))       
#define CSI_IPI_VFP_LINES         SFR_AT (SPRGROUP_SFR12 + (0x014<<2))      
#define CSI_IPI_VACTIVE_LINES     SFR_AT (SPRGROUP_SFR12 + (0x015<<2))      
#define CSI_IPI_MEM_FLUSH         SFR_AT (SPRGROUP_SFR12 + (0x016<<2))       
#define CSI_PHY_CAL               SFR_AT (SPRGROUP_SFR12 + (0x017<<2))      
#define CSI_INT_ST_PHY_FATAL      SFR_AT (SPRGROUP_SFR12 + (0x018<<2))      
#define CSI_INT_MSK_PHY_FATAL     SFR_AT (SPRGROUP_SFR12 + (0x019<<2))       
#define CSI_INT_FORCE_PHY_FATAL   SFR_AT (SPRGROUP_SFR12 + (0x01a<<2))     
#define CSI_INT_ST_PKT_FATAL      SFR_AT (SPRGROUP_SFR12 + (0x01b<<2))    
#define CSI_INT_MSK_PKT_FATAL     SFR_AT (SPRGROUP_SFR12 + (0x01c<<2))      
#define CSI_INT_FORCE_PKT_FATAL   SFR_AT (SPRGROUP_SFR12 + (0x01d<<2))     
#define CSI_INT_ST_FRAME_FATAL    SFR_AT (SPRGROUP_SFR12 + (0x01e<<2))   
#define CSI_INT_MSK_FRAME_FATAL   SFR_AT (SPRGROUP_SFR12 + (0x01f<<2))    
#define CSI_INT_FORCE_FRAME_FATAL SFR_AT (SPRGROUP_SFR12 + (0x020<<2)) 
#define CSI_INT_ST_PHY            SFR_AT (SPRGROUP_SFR12 + (0x021<<2))     
#define CSI_INT_MSK_PHY           SFR_AT (SPRGROUP_SFR12 + (0x022<<2))     
#define CSI_INT_FORCE_PHY         SFR_AT (SPRGROUP_SFR12 + (0x023<<2))       
#define CSI_INT_ST_PKT            SFR_AT (SPRGROUP_SFR12 + (0x024<<2))          
#define CSI_INT_MSK_PKT           SFR_AT (SPRGROUP_SFR12 + (0x025<<2))             
#define CSI_INT_FORCE_PKT         SFR_AT (SPRGROUP_SFR12 + (0x026<<2))         
#define CSI_INT_ST_LINE           SFR_AT (SPRGROUP_SFR12 + (0x027<<2))
#define CSI_INT_MSK_LINE          SFR_AT (SPRGROUP_SFR12 + (0x028<<2))
#define CSI_INT_FORCE_LINE        SFR_AT (SPRGROUP_SFR12 + (0x029<<2))
#define CSI_INT_ST_IPI            SFR_AT (SPRGROUP_SFR12 + (0x02a<<2))          
#define CSI_INT_MSK_IPI           SFR_AT (SPRGROUP_SFR12 + (0x02b<<2))            
#define CSI_INT_FORCE_IPI         SFR_AT (SPRGROUP_SFR12 + (0x02c<<2))          

/////////////////////////////////////////////////////////
// Group 13
/////////////////////////////////////////////////////////
#define SFR_13_TEST             SFR_AT (SPRGROUP_SFR13 + (0x000 <<2))
//.................... 0x000 .........................

/////////////////////////////////////////////////////////
// Group 14
/////////////////////////////////////////////////////////
#define SFR_14_TEST             SFR_AT (SPRGROUP_SFR14 + (0x000 <<2))
//.................... 0x000 .........................
#if 1
//==LT==
#define SFR_BANDWIDTH_MONa0  	SFR_AT (SPRGROUP_SFR14 + (0x000 <<2))
#define SFR_BANDWIDTH_MONa1  	SFR_AT (SPRGROUP_SFR14 + (0x001 <<2))
#define SFR_BANDWIDTH_MONa2  	SFR_AT (SPRGROUP_SFR14 + (0x002 <<2))
#define SFR_BANDWIDTH_MONa3  	SFR_AT (SPRGROUP_SFR14 + (0x003 <<2))
#else
//==SE==
#define SFR_BANDWIDTH_MONa0  	SFR_AT (SPRGROUP_SFR15 + (0x000 <<2))
#define SFR_BANDWIDTH_MONa1  	SFR_AT (SPRGROUP_SFR15 + (0x001 <<2))
#define SFR_BANDWIDTH_MONa2  	SFR_AT (SPRGROUP_SFR15 + (0x002 <<2))
#define SFR_BANDWIDTH_MONa3  	SFR_AT (SPRGROUP_SFR15 + (0x003 <<2))
#define SFR_BANDWIDTH_MONa4  	SFR_AT (SPRGROUP_SFR15 + (0x004 <<2))
#define SFR_BANDWIDTH_MONa5  	SFR_AT (SPRGROUP_SFR15 + (0x005 <<2))
#define SFR_BANDWIDTH_MONa6  	SFR_AT (SPRGROUP_SFR15 + (0x006 <<2))
#define SFR_BANDWIDTH_MONa7  	SFR_AT (SPRGROUP_SFR15 + (0x007 <<2))
#define SFR_BANDWIDTH_MONa8  	SFR_AT (SPRGROUP_SFR15 + (0x008 <<2))
#define SFR_BANDWIDTH_MONa9  	SFR_AT (SPRGROUP_SFR15 + (0x009 <<2))
#define SFR_BANDWIDTH_MONaa  	SFR_AT (SPRGROUP_SFR15 + (0x00a <<2))
#define SFR_BANDWIDTH_MONab  	SFR_AT (SPRGROUP_SFR15 + (0x00b <<2))
#define SFR_BANDWIDTH_MONac  	SFR_AT (SPRGROUP_SFR15 + (0x00c <<2))
#define SFR_BANDWIDTH_MONad  	SFR_AT (SPRGROUP_SFR15 + (0x00d <<2))
#define SFR_BANDWIDTH_MONae  	SFR_AT (SPRGROUP_SFR15 + (0x00e <<2))
#define SFR_BANDWIDTH_MONaf  	SFR_AT (SPRGROUP_SFR15 + (0x00f <<2))
#endif

/////////////////////////////////////////////////////////
// Group 15
/////////////////////////////////////////////////////////
#define SFR_15_TEST             SFR_AT (SPRGROUP_SFR15 + (0x000 <<2))

#define PMC_CTRL_REG             SFR_AT (SPRGROUP_SFR15 + (0x000 <<2))
#define PMC_CFG_REG              SFR_AT (SPRGROUP_SFR15 + (0x001 <<2))
//#define                          SFR_AT (SPRGROUP_SFR15 + (0x002 <<2))
#define PMC_WAIT_REG             SFR_AT (SPRGROUP_SFR15 + (0x003 <<2))
#define PMC_SERVICE_REG          SFR_AT (SPRGROUP_SFR15 + (0x004 <<2))
#define PMC_RFIFO_CTRL           SFR_AT (SPRGROUP_SFR15 + (0x005 <<2))
#define PMC_DLL_LOCK_CTL         SFR_AT (SPRGROUP_SFR15 + (0x006 <<2))
#define PMC_DELAY_CTRL           SFR_AT (SPRGROUP_SFR15 + (0x007 <<2))
#define PMC_RE_INIT              SFR_AT (SPRGROUP_SFR15 + (0x008 <<2))
#define PMC_LOW_POWER_CTRL       SFR_AT (SPRGROUP_SFR15 + (0x009 <<2))
#define PMC_TPU                  SFR_AT (SPRGROUP_SFR15 + (0x00a <<2))
#define PMC_TRST                 SFR_AT (SPRGROUP_SFR15 + (0x00b <<2))
#define PMC_TCEM                 SFR_AT (SPRGROUP_SFR15 + (0x00c <<2))
#define PMC_THS_INT              SFR_AT (SPRGROUP_SFR15 + (0x00d <<2))
#define PMC_TDPD_INIT            SFR_AT (SPRGROUP_SFR15 + (0x00e <<2))
#define PMC_MR_INIT              SFR_AT (SPRGROUP_SFR15 + (0x00f <<2))

#define PMC_MR0                  SFR_AT (SPRGROUP_SFR15 + (0x010 <<2))
#define PMC_MR1                  SFR_AT (SPRGROUP_SFR15 + (0x011 <<2))
#define PMC_MR2                  SFR_AT (SPRGROUP_SFR15 + (0x012 <<2))
#define PMC_MR3                  SFR_AT (SPRGROUP_SFR15 + (0x013 <<2))
#define PMC_MR4                  SFR_AT (SPRGROUP_SFR15 + (0x014 <<2))
#define PMC_MR8                  SFR_AT (SPRGROUP_SFR15 + (0x015 <<2))
#define PMC_COMMAND_TIMING       SFR_AT (SPRGROUP_SFR15 + (0x016 <<2))
#define PMC_RW_LATENCY           SFR_AT (SPRGROUP_SFR15 + (0x017 <<2))
#define PMC_INITIAL_STATUS       SFR_AT (SPRGROUP_SFR15 + (0x018 <<2))
#define PMC_IDLE_WAIT            SFR_AT (SPRGROUP_SFR15 + (0x019 <<2))
#define PMC_THS                  SFR_AT (SPRGROUP_SFR15 + (0x01a <<2))
#define PMC_TXPHS                SFR_AT (SPRGROUP_SFR15 + (0x01b <<2))
#define PMC_TXHS                 SFR_AT (SPRGROUP_SFR15 + (0x01c <<2))
#define PMC_TDPD                 SFR_AT (SPRGROUP_SFR15 + (0x01d <<2))
#define PMC_TXPDPD               SFR_AT (SPRGROUP_SFR15 + (0x01e <<2))
#define PMC_TXDPD                SFR_AT (SPRGROUP_SFR15 + (0x01f <<2))

#define PMC_TCQLZ                SFR_AT (SPRGROUP_SFR15 + (0x020 <<2))
#define PMC_CSN_DUMMY            SFR_AT (SPRGROUP_SFR15 + (0x021 <<2))
#define PMC_RWDS_CTRL            SFR_AT (SPRGROUP_SFR15 + (0x022 <<2))
#define PMC_MEMORY_RESET         SFR_AT (SPRGROUP_SFR15 + (0x023 <<2))
#define PMC_TRP                  SFR_AT (SPRGROUP_SFR15 + (0x024 <<2))
#define PMC_TRWR                 SFR_AT (SPRGROUP_SFR15 + (0x025 <<2))
#define PMC_DLL_DELAY_CNT        SFR_AT (SPRGROUP_SFR15 + (0x026 <<2))
#define PMC_MAIN_FSM_STATE       SFR_AT (SPRGROUP_SFR15 + (0x027 <<2))
#define PMC_DELAY_ADJUST         SFR_AT (SPRGROUP_SFR15 + (0x028 <<2))
#define PMC_AP_SPECIAL           SFR_AT (SPRGROUP_SFR15 + (0x029 <<2))
#define PMC_INTR_STATUS          SFR_AT (SPRGROUP_SFR15 + (0x02a <<2))
#define PMC_INTR_CTRL            SFR_AT (SPRGROUP_SFR15 + (0x02b <<2))
#define PMC_INTR_TEST            SFR_AT (SPRGROUP_SFR15 + (0x02c <<2))
#define PMC_READ_TIMEOUT         SFR_AT (SPRGROUP_SFR15 + (0x02d <<2))
#define PMC_MR_DIE               SFR_AT (SPRGROUP_SFR15 + (0x02e <<2))
//#define SFR_BANDWIDTH_MON2f      SFR_AT (SPRGROUP_SFR15 + (0x02f <<2))
//
//#define SFR_BANDWIDTH_MON30      SFR_AT (SPRGROUP_SFR15 + (0x030 <<2))
//#define SFR_BANDWIDTH_MON31      SFR_AT (SPRGROUP_SFR15 + (0x031 <<2))
//#define SFR_BANDWIDTH_MON32      SFR_AT (SPRGROUP_SFR15 + (0x032 <<2))
//#define SFR_BANDWIDTH_MON33      SFR_AT (SPRGROUP_SFR15 + (0x033 <<2))
#define PMC_BIST_CTRL            SFR_AT (SPRGROUP_SFR15 + (0x034 <<2))
#define PMC_BIST_UDP             SFR_AT (SPRGROUP_SFR15 + (0x035 <<2))
#define PMC_BIST_LFSR_SEED       SFR_AT (SPRGROUP_SFR15 + (0x036 <<2))
#define PMC_BIST_ADDR            SFR_AT (SPRGROUP_SFR15 + (0x037 <<2))
#define PMC_BIST_STATUS          SFR_AT (SPRGROUP_SFR15 + (0x038 <<2))
#define PMC_BIST_DATA_BACK       SFR_AT (SPRGROUP_SFR15 + (0x039 <<2))
#define PMC_BIST_BIT_ERR0        SFR_AT (SPRGROUP_SFR15 + (0x03a <<2))
#define PMC_BIST_BIT_ERR1        SFR_AT (SPRGROUP_SFR15 + (0x03b <<2))
#define PMC_BIST_BIT_ERR2        SFR_AT (SPRGROUP_SFR15 + (0x03c <<2))
#define PMC_BIST_BIT_ERR3        SFR_AT (SPRGROUP_SFR15 + (0x03d <<2))
#define PMC_BIST_BIT_ERR4        SFR_AT (SPRGROUP_SFR15 + (0x03e <<2))
#define PMC_BIST_BIT_ERR5        SFR_AT (SPRGROUP_SFR15 + (0x03f <<2))

#define PMC_BIST_ERR6            SFR_AT (SPRGROUP_SFR15 + (0x040 <<2))
#define PMC_BIST_ERR7            SFR_AT (SPRGROUP_SFR15 + (0x041 <<2))






//.................... 0x000 .........................

//............................  SIM_END .........................
#define SIMEND            (*(volatile unsigned long*) 0x3ffc = 0xdeaddead)
#define SIMERR            (*(volatile unsigned long*) 0x3ffc = 0xdeadeeee)
#define SIMMON            (*(volatile unsigned long*) 0x3ffc = 0xacdc0000)

//------------------------------------------------------------------------------
// DAC Internal SFR Address Defines              
//------------------------------------------------------------------------------
#define DACCFG                 0
#define DACSTA                 1
#define DACCLR                 2
#define DACVOL                 3
#define DACVCON                4
#define TRIMCON1               5
#define TRREGL                 6
#define TRREGR                 7
#define DACLRMIX0              8
#define DACLRMIX1              9
#define DACEQGAIN0             10
#define DACEQGAIN1             11
#define DACEQGAIN2             12
#define DACEQGAIN3             13
#define DACEQGAIN4             14
#define EQCOEFFADR             15
#define EQCOEFF                16

//------------------------------------------------------------------------------
// IRTCC Internal SFR command defines
//------------------------------------------------------------------------------
#define IRTRTC_RD_CMD          0xe0
#define IRTRTC_WR_CMD          0xf0

#define IRTALM_RD_CMD          0x50
#define IRTALM_WR_CMD          0x51

#define IRTRAM_RD_CMD          0x52
#define IRTRAM_WR_CMD          0x53

#define IRTCON0_RD_CMD         0x54
#define IRTCON0_WR_CMD         0x55

#define IRTCON1_RD_CMD         0x56
#define IRTCON1_WR_CMD         0x57

#define IRTCON2_RD_CMD         0x58
#define IRTCON2_WR_CMD         0x59

#define IRTCON3_RD_CMD         0x5a
#define IRTCON3_WR_CMD         0x5b

#define IRTCON4_RD_CMD         0x40
#define IRTCON4_WR_CMD         0x41

#define IRTCON5_RD_CMD         0x42
#define IRTCON5_WR_CMD         0x43

#define IRTCON6_RD_CMD         0x44
#define IRTCON6_WR_CMD         0x45

#define IRTCON7_RD_CMD         0x46 
#define IRTCON7_WR_CMD         0x47

#define IRTCON8_RD_CMD         0x48 
#define IRTCON8_WR_CMD         0x49

#define IRTCON9_RD_CMD         0x4a 
#define IRTCON9_WR_CMD         0x4b

#define IRTCON10_RD_CMD        0x30 
#define IRTCON10_WR_CMD        0x31

#define IRTCDIVL_RD_CMD        0x32 
#define IRTCDIVL_WR_CMD        0x33 

#define IRTCDIVH_RD_CMD        0x34 
#define IRTCDIVH_WR_CMD        0x35 

#endif
