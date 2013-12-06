#include"msp430f5438.h"
#include"Global.h"

#ifdef RotaryEncoder_Used_
//unsigned int A_before,A_now,A_T,A_interval;
//unsigned int B_before,B_now,B_T,B_interval;
volatile signed long pulseCnt;

//P4.1,P4.2捕获,P9.6,P9.7IO口判断电平(输入模式，下拉电阻)
//CM_0不捕获，CM_1上升沿,CM_2下降沿,CM_3都捕获
void RotaryEncoder_init()
{
    P4DIR &= ~(BIT1+ BIT2);
    P4SEL |=  BIT1+ BIT2;
    
    P9DIR &= ~(BIT6+ BIT7);
    P9REN |= BIT6+BIT7;
    P9OUT &= ~(BIT6+ BIT7);
    
    TB0CCTL1 = CM_3+CCIS_0+CAP+SCS;                    // CCR1 capture mode P4.1
    TB0CCTL2 = CM_3+CCIS_0+CAP+SCS;                    // CCR1 capture mode P4.2
    TB0CCTL1 |= CCIE;
    TB0CCTL2 |= CCIE;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void RotaryEncoder_ISR()
{
  unsigned char PhaseA,PhaseB;
  PhaseA  = P9IN & BIT6;
  PhaseB  = P9IN & BIT7;
  switch(TB0IV)
  {
    case  0: break;                          // No interrupt
    case  2:                                 // CCR1 used
      if ((PhaseA && PhaseB)|((!PhaseA)&&(!PhaseB)))
      {
        pulseCnt++;
      }
      else
      {
        pulseCnt--;
      }
      break;
    case  4:                                 // CCR2 used
      if ((PhaseA&&PhaseB)|((!PhaseA)&&(!PhaseB)))
      {
        pulseCnt--;
      }
      else
      {
        pulseCnt++;      
      }
      break;
    case  6: break;                          // reserved
    case  8: break;                          // reserved
    case 10: break;                          // reserved
    case 12: break;                          // reserved
    case 14: break;                          // overflow
         
    default: break;     
  }
}
/*
#pragma vector = TIMER0_B0_VECTOR
__interrupt void TimerB0_ISR()
{
  L_interval++;
  if (L_interval  > 200)
    L_T=0;
  R_interval++;
  if (R_interval  > 200)
    R_T=0;  
}*/

#endif