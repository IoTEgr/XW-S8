#include "../inc/ax32xx.h"
//TODO:arrange
/*
static void soft_crc32 (u32 poly, u32 val, u32 *crc) {
	int i;

	for (i=0; i<32; ++i) {
		if (((val<<31)^(*crc))&0x80000000) *crc = (*crc<<1) ^ poly;
		else *crc <<= 1;
		val >>= 1;
	}
}

static bool crc_test(void)
{
    ENTER();
    bool res = false;
    
	uint32_t crc32 = 0xffffffff;
	soft_crc32 (0x4c11db7, 0x80000000, &crc32);
	soft_crc32 (0x4c11db7, 0xaa000000, &crc32);
	
	LFSRCON |= BIT(2);
    CRCRES  = 0xffffffff;
	CRCFIFO = 0x80000000;
	CRCFIFO = 0xaa000000;
    delay(100);
	if (crc32 == (CRCRES & 0xffffffff))
        res = true;
        
    debgreg(crc32);
    debgreg(CRCRES);
    
    LEAVE();
    debgreg(res);
    return res;
}
*/