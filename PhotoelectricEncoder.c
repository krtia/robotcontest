#include"msp430f5438.h"
#include"Global.h"
#include"Motor.h"
#include"UART.h"

#ifdef PhotoelectricEncoder_Used_
#define Filter 20

unsigned int L_before,L_now,L_interval;
unsigned int R_before,R_now,R_interval;
unsigned long L_T,L_T_prev,R_T,R_T_prev;
signed long L_cnt,R_cnt,L_speed,R_speed;

extern int flag;

void PhotoelectricEncoder_init()
{
    P4DIR &= ~(BIT1+ BIT2);
    P4SEL |=  BIT1+ BIT2;
    
    TB0CCTL1 = CM_1+CCIS_0+CAP+SCS;                    // CCR1 capture mode P4.1
    TB0CCTL2 = CM_1+CCIS_0+CAP+SCS;                    // CCR1 capture mode P4.2
    TB0CCTL1 |= CCIE;
    TB0CCTL2 |= CCIE;
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void PhotoelectricEncoder_ISR()
{
  switch(TB0IV)
  {
    case  0: break;                          // No interrupt
    case  2:                                 // CCR1 used
      L_before= L_now;
      L_now   = TB0CCR1;
      L_T =(L_T_prev)*(100-Filter)/100+(L_interval*20+((signed)L_now-(signed)L_before)/200)*Filter/100;   //转一圈所需要的毫秒数,带滤波

      L_T_prev  = L_T;
      L_speed = (signed long)(60000/L_T);    //rpm,后期可*100以更精确

      if (L_dir)
      {
        L_cnt--;
        L_speed = -L_speed;
      }
      else
        L_cnt++;
      
      L_interval=0;
      if (TB0CCTL1 & COV)
      {
        UART_sendstr(UCA1,"OVERFLOW!");
        TB0CCTL1 &= ~COV;
      }
      
      if (flag == 5)
      {
        while(1);
      }
      break;
    case  4:                                 // CCR2 used
      R_before= R_now;
      R_now   = TB0CCR2;
      R_T =(R_T_prev)*(100-Filter)/100+(R_interval*20+((signed)R_now-(signed)R_before)/200)*Filter/100;   //转一圈所需要的毫秒数,带滤波

      R_T_prev  = R_T;
      R_speed = (unsigned int)(60000/R_T);

      if (R_dir)
      {
        R_cnt--;
        R_speed = -R_speed;
      }
      else
        R_cnt++;
      
      R_interval=0;
      if (TB0CCTL2 & COV)
      {
        UART_sendstr(UCA1,"OVERFLOW!");
        TB0CCTL2 &= ~COV;
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

#pragma vector = TIMER0_B0_VECTOR
__interrupt void TimerB0_ISR()
{
  L_interval++;
  if (L_interval  > 100)
  {
    L_T=65535;
    L_speed=0;
  }
  R_interval++;
  if (R_interval  > 100)
  {
    R_T=65535;
    R_speed=0;
  }
}

#endif