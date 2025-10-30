/* spr_defs.h -- Defines OR1K architecture specific special-purpose registers
   Copyright (C) 1999 Damjan Lampret, lampret@opencores.org

This file is part of OpenRISC 1000 Architectural Simulator.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

/* This file is also used by microkernel test bench. Among
others it is also used in assembly file(s). */

/* Definition of special-purpose registers (SPRs) 

#define MAX_GRPS (32)
#define MAX_SPRS_PER_GRP_BITS (11)
#define MAX_SPRS_PER_GRP (1 << MAX_SPRS_PER_GRP_BITS)
#define MAX_SPRS (0x10000)

*/

/*
#define mtspr(spr,value) \
asm("l.mtspr\t\t%0,%1,0": : "r" ((unsigned long)spr), "r" ((unsigned long)value));
*/

//spr set or clr bit define
//#define __sprset(spr,n)    do{long val; val = mfspr(spr); val |=  (1 << n); mtspr(spr,val);}while(0)
//#define __sprclr(spr,n)    do{long val; val = mfspr(spr); val &= ~(1 << n); mtspr(spr,val);}while(0)
#define __sprset(spr,n)      spr |= BIT(n)//mtspr(spr,mfspr(spr) | (1 << n))
#define __sprclr(spr,n)      spr &= ~BIT(n)//mtspr(spr,mfspr(spr) & ~(1 << n))
    
#define __LGIE_DIS__()       SPR_SR &= ~BIT(1)//__sprclr(SPR_SR,2)
#define __LGIE_EN__()        SPR_SR |= BIT(1)//__sprset(SPR_SR,2)
#define __HGIE_DIS__()       SPR_SR &= ~BIT(2)
#define __HGIE_EN__()        SPR_SR |= BIT(2)
#define __intvct_mask()      SPR_SR &= ~BIT(14)//__sprclr(SPR_SR,14)
#define __intvct_sram()      SPR_SR |= BIT(14)//__sprset(SPR_SR,14)

#define SPR_SR_EPH 0x4000; 
#define __intvct_adr__(x)    do{SPR_EVBA = x; SPR_SR |= SPR_SR_EPH;}while(0)    

#define mfspr(spr)           (spr)
#define mtspr(spr,val)       do{spr = val;}while(0)
/*
#define __gie_dis() do{u32 val; val = mfspr(SPR_SR); val &= ~(1<<2); mtspr(SPR_SR,val);}while(0)
#define __gie_en()  do{u32 val; val = mfspr(SPR_SR); val |= (1<<2); mtspr(SPR_SR,val);}while(0)
    
#define __intvct_sram()  do{u32 val; val = mfspr(SPR_SR); val |= (1<<14); mtspr(SPR_SR,val);}while(0)
*/
