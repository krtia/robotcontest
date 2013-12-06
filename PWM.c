#include "msp430x54x.h"
#include "PWM.h"
//初始化PWM
//P8.1|--> PWM_1
//P8.2|--> PWM_2
//P8.3|--> PWM_3
//P8.4|--> PWM_4
//默认占空比50%
//SMCLK=XT2/4=4MHz,频率4KHz
void PWM_init(void)
{
	P8DIR |= BIT1+BIT2+BIT3+BIT4;           // P2.2 and P2.3 output
	P8SEL |= BIT1+BIT2+BIT3+BIT4;			// P1.2 and P2.3 options select
	TA0CCR0 = 1000-1;						// PWM Period
	TA0CCTL1 = OUTMOD_7;                    // CCR1 reset/set
	TA0CCTL2 = OUTMOD_7;                    // CCR2 reset/set
	TA0CCTL3 = OUTMOD_7;                    // CCR3 reset/set
	TA0CCTL4 = OUTMOD_7;                    // CCR4 reset/set
	TA0CTL = TASSEL_2 + MC_1 + TACLR;	// SMCLK, up mode, clear TAR
	PWM_1(500);
	PWM_2(500);
	PWM_3(500);
	PWM_4(500);
}
//调节占空比
//参数  n 为0~1000的整数，占空比为  n/10 %
void PWM_1(unsigned int n)
{
	TA0CCR1 = n;                            // CCR1 PWM duty cycle
}
void PWM_2(unsigned int n)
{
	TA0CCR2 = n;                            // CCR2 PWM duty cycle
}
void PWM_3(unsigned int n)
{
	TA0CCR3 = n;                            // CCR3 PWM duty cycle
}
void PWM_4(unsigned int n)
{
	TA0CCR4 = n;                            // CCR4 PWM duty cycle
}