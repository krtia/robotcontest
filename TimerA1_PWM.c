#include "msp430f5438.h"
#include "Global.h"
#ifdef TimerA1_PWM_Used_

void TimerA1_PWM_1(unsigned int n)
{
	TA1CCR1 = n;                            // CCR1 PWM duty cycle
}

void TimerA1_PWM_2(unsigned int n)
{
	TA1CCR2 = n;                            // CCR1 PWM duty cycle
}

//初始化PWM
//P8.6|--> TimerA1_PWM_1
//P7.3|--> TimerA1_PWM_2

//默认占空比50%
//SMCLK=XT2/4=4MHz,频率4KHz
void TimerA1_PWM_init()
{
    P8DIR |= BIT6;           // P8.6 output
    P8SEL |= BIT6;	     // P8.6 options select
    P7DIR |= BIT3;           // P7.3 output
    P7SEL |= BIT3;	     // P7.3 options select
    
    TA1CCTL1 = OUTMOD_7;                    // CCR1 reset/set
    TA1CCTL2 = OUTMOD_7;                    // CCR2 reset/set

    TimerA1_PWM_1(2000);
    TimerA1_PWM_2(2000);
}
#endif