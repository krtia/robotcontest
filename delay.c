#include"msp430f5438.h"

unsigned int CountDown = 0;

//准确延时1us(包括进出函数，开关中断)
void delayus()
{
    _DINT();
    __no_operation();
    __no_operation();    
    __no_operation();    
    __no_operation();
    __no_operation();
    __no_operation();    
    __no_operation();
    _EINT();
}

//入口参数:
//  n:延时的ms数
//出口参数:
//  错误参数(0为无错，1为当前有函数调用延时)
//调用示例(阻塞式):
//  while(delayms(5));
//  while(CountDown);
unsigned char delayms(unsigned int n)
{
    if (CountDown != 0)
      return 1;
    else
    {
      CountDown = n;
      return 0;
    }
}