#include"msp430f5438.h"
#include"Global.h"

void TimerB0_init()
{
#ifdef PhotoelectricEncoder_Used_
    TB0CCTL0 |= CCIE;                 //TB0CCR0 允许中断
#endif
    TB0CCR0 = 4000-1;                 //设置延时时间(4MHz/4000=1KHz)
    TB0CTL |= TBSSEL_2;               //SMCLK
    TB0CTL |= MC_1;
}
