#include "msp430f5438.h"

//RTC不能用

typedef struct{
  unsigned char sec;
  unsigned char min;
  unsigned char hour;
  unsigned char dow;
  unsigned char day;
  unsigned char mon;
  unsigned int  year;
}RTC_setTypeDef;

volatile unsigned char presentSec,presentMin,presentHour,presentDow,presentDay,presentMon;
volatile unsigned int presentYear;

void RTC_init()
{
  RTCCTL01 &= ~RTCHOLD;  //解除hold
  RTCCTL01 |= RTCMODE;   //RTC模式
  RTCCTL01 |= RTCRDYIE;  //读取中断使能  
}

unsigned char RTC_set(RTC_setTypeDef *now)
{
  if ((RTCCTL01&0x1000u))  //如果在更新时期
  {
    return 1;
  }
  else
  {
    RTCSEC  = now->sec;
    RTCMIN  = now->min;
    RTCHOUR = now->hour;
    RTCDOW  = now->dow;
    RTCDAY  = now->day;
    RTCMON  = now->mon;
    RTCYEARH= (now->year)>>8;
    RTCYEARL= now->year;
    return 0;
  }
}

#pragma vector = RTC_VECTOR
__interrupt void RTC_ISR(void)
{
  switch(RTCIV)
  {
    case 0: break;                          // No interrupts
    case 2:                                 // RTCRDYIFG
      presentSec  = RTCSEC;
      presentMin  = RTCMIN;
      presentHour = RTCHOUR;
      presentDow  = RTCDOW;
      presentDay  = RTCDAY;
      presentMon  = RTCMON;
//      presentYear = ((RTCYEARH)<<8) | RTCYEARL;
      break;                          
    case 4: break;                          // RTCEVIFG
    case 6: break;                          // RTCAIFG
    case 8: break;                          // RT0PSIFG
    case 10: break;                         // RT1PSIFG
    case 12: break;                         // Reserved
    case 14: break;                         // Reserved
    case 16: break;                         // Reserved
    default: break;
  }
}