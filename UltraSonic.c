#include"msp430f5438.h"
#include"UCS.h"
#include"TimerA1.h"
#include"delay.h"

volatile unsigned long risingTime,fallingTime;
volatile unsigned int  risingCap,fallingCap;
volatile unsigned long sonicDist,sonicTime;

//P8.6捕获模式
//CCR1
void UltraSonic_init()
{
    P8DIR|= BIT5;
    P8OUT&=~BIT5;
      
    P8DIR &=~BIT6;           // P8.6 input
    P8SEL |= BIT6;	     // P8.6 options select
    
    TA1CCTL1 = CM_3+CCIS_1+CAP+SCS;                    // CCR1 capture mode P8.6

    TA1CCTL1 |= CCIE;
}

//P10.2发送脉冲
void UltraSonic_Tx()
{
    P8OUT  |= BIT5;
    delay_us(10);
    P8OUT  &=~BIT5;
}

// Timer_A3 Interrupt Vector (TAIV) handler
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_ISR(void)
{
  switch(TA1IV)
  {
    case  0: break;                          // No interrupt
    case  2:                                 // CCR1 used
      if (TA1CCTL1 & CCI)
      {
        risingTime  = TimeBase;
        risingCap   = TA1CCR1;
      }
      else
      {
        fallingTime = TimeBase;
        fallingCap  = TA1CCR1;
        sonicTime   = (((fallingTime-risingTime)*4000+fallingCap)-risingCap);
        sonicDist   = sonicTime*34/800;
      }
      break;
    case  4: break;                          // CCR2 not used
    case  6: break;                          // reserved
    case  8: break;                          // reserved
    case 10: break;                          // reserved
    case 12: break;                          // reserved
    case 14: break;                          // overflow
         
    default: break; 
  }
}
