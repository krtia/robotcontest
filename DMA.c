#include"msp430f5438.h"

//由于430DMA传送中CPU关断，所以DMA意义不大
unsigned int DMA_DST;                       //传送目标变量
void DMA_init()
{
    // Setup DMA0
  DMACTL0 = DMA0TSEL_24;                    // 选择ADC12IFG触发
  DMACTL4 = DMARMWDIS;                      // Read-modify-write disable
  DMA0CTL &= ~DMAIFG;
  DMA0CTL = DMADT_5+DMAEN+DMADSTINCR_1+DMASRCINCR_1+DMAIE; // DT_4重复单个传输，目标和源地址不自动增减，使能DMA及中断
  DMA0SZ = 2;                               // DMA0 size = 1
//  __data16_write_addr((unsigned short) &DMA0SA,(unsigned long) &ADC12MEM0);
  DMA0SA = 0x0720u;
                                            // ... from ADC12MEM0
//  __data16_write_addr((unsigned short) &DMA0DA,(unsigned long) &DMA_DST);
  DMA0DA = (unsigned short) &DMA_DST;
                                            // ... to destination in RAM
}

#pragma vector = DMA_VECTOR
__interrupt void DMA_ISR(void)
{
  switch(DMAIV)
  {
  case 0: break;
  case 2:                                 // DMA0IFG = DMA Channel 0
    
    break;
  case 4: break;                          // DMA1IFG = DMA Channel 1
  case 6: break;                          // DMA2IFG = DMA Channel 2
  default: break;
  }
}