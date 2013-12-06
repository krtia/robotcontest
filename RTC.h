#ifndef RTC_H_
#define RTC_H_

typedef struct{
  unsigned char sec;
  unsigned char min;
  unsigned char hour;
  unsigned char dow;
  unsigned char day;
  unsigned char mon;
  unsigned int  year;
}RTC_setTypeDef;

extern void RTC_init();
extern unsigned char RTC_set(RTC_setTypeDef *);

extern volatile unsigned char presentSec,presentMin,presentHour,presentDow,presentDay,presentMon;
extern volatile unsigned int presentYear;

#endif