#include "../inc/ax32xx.h"
//TODO:arrange
/*
struct bu_buffer_s {
    u8 *pw,*end;
    u8  buffer[64];
};
static struct bu_buffer_s bu_buffer __attribute__ ((section(".sram_comm")));
//fifo for btuart rx dma
static u8 bu_fifo[64] __attribute__ ((section(".sram_comm")));
static u32 bu_fifo_end_addr = (u32)&bu_fifo[sizeof(bu_fifo)];

void bu_kick_dma_rx(bool enable)
{
    if(enable) 
        BUSTA |= (1<<0);
    else 
        BUSTA &=~(1<<0);
}

void bu_putc(const char c)
{
    BUDATA = c;
    while(!(BUSTA & (1<<2)));
}

void bu_isr(void)
{
    u32 busta = BUSTA;
    if(busta & (1<<1))
    {
        //clean pending,and stop rx dma
        BUSTA &=~(3<<0);
        debg("btuart overflow\n");
        debg("%s\n",bu_fifo);
    }
    if(busta & (1<<2))
    {
        BUSTA &=~(1<<2);
    }
    if(busta & (1<<3))
    {
        BUSTA &=~(1<<3);
    }
}

void bu_init(u32 boudrate)
{
    //TX:PD14(GROUP4),RX:PD15(GROUP5)
    PMAPCON0 &=~0xFF00; 
    PMAPCON0 |= 0x4500;
	PDFMAP   |= BIT(14)|BIT(15); 
	PDDIR    |= BIT(15); 
	PDDIR    &=~BIT(14);
    
    BUBAUD     = (APB_CLK / boudrate) - 1;//buad rate
    BUDATA     = 0;
    
    BUDMARXADR = ((u32)bu_fifo);//fifo addr

    BULOOPCNT  = (0<<3)|//overflow limit
                 (2<<0);//rx ram size
                 
    BUCON      = (0<<7)|//stop bit select
                 (0<<6)|//tx 9th bit value
                 (0<<5)|//9bit mode en
                 (1<<4)|//btuart en
                 (0<<3)|//tx int en (normal and dma mode)
                 (0<<2)|//rx (normal) int en
                 (1<<1)|//rx dma overflow int en
                 (1<<0);//auto dma en
                 
    BUSTA      = (0<<5)|//rx 9th bit value
                 (0<<3)|//rx done(normal) flag
                 (0<<2)|//tx done flag
                 (0<<1)|//rx dma overflow flag
                 (0<<0);//rx dma kick
    
    fp_hal_btuart_isr = bu_isr;
    SetIntSrc(_BUART_INT_);
}

void bu_test(void)
{
    bu_init(115200);
    bu_putc('A');
    debgreg(BUCPUPOUT);
    
    bu_buffer.pw  = &bu_buffer.buffer[0];
    bu_buffer.end = &bu_buffer.buffer[sizeof(bu_buffer.buffer)];
    my_memset(bu_buffer.buffer,0,sizeof(bu_buffer.buffer));
    
    bu_kick_dma_rx(true);
    bu_putc('B');
    
    u32 idel_cnt = 50;
    
    while(1)
    {
        //detect per 10ms
        Delay_MS(10);
        
        if(++idel_cnt == 0)
            idel_cnt = 50;
            
        u32 rdcnt = 0;
        u32 rxcnt = BUDMACNT;
        u32 read_addr = BUCPUPOUT;
        
        if(rxcnt) //FIFO有数据
        {
            //检查FIFO是否需要LOOP
            rxcnt = min(bu_fifo_end_addr - read_addr,rxcnt);
            //检查buffer可用空间
            rdcnt = min(bu_buffer.end - bu_buffer.pw,rxcnt);
            //搬运数据
            my_memcpy(bu_buffer.pw,(u8 *)read_addr,rdcnt);
                    
            BUMINUS = rdcnt - 1;
            bu_buffer.pw += rdcnt;
            if(bu_buffer.pw == bu_buffer.end)
            {
                //buffer full
                debg("btuart buffer full\n");
                //reset bu_buffer
                bu_buffer.pw  = &bu_buffer.buffer[0];
            }
            
            idel_cnt = 0;
        }
        
        //认为传输结束
        if(idel_cnt == 49)
        {
            debgbuf(bu_buffer.buffer,bu_buffer.pw - &bu_buffer.buffer[0]);
            //reset bu_buffer
            bu_buffer.pw  = &bu_buffer.buffer[0];
            debgreg(BUCPUPOUT);
        }
    }
}
*/



