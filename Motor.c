#include "msp430f5438.h"
#include "PWM.h"

#define MOTORDIR  P9DIR
#define MOTOROUT  P9OUT

#define IN1 BIT4
#define IN2 BIT5
#define IN3 BIT6
#define IN4 BIT7
#define IN5 BIT0
#define IN6 BIT1
#define IN7 BIT2
#define IN8 BIT3

volatile unsigned char L_dir=0,R_dir=0;

void Motor_init()
{
  PWM_init();
  MOTORDIR |= 0xff;
  MOTOROUT =  0;
}

//PWM调整马达速率函数
//入口参数：l1,l2,r1,r2为-1000~1000的signed int 值
//负数代表反转

void Motor_config(signed int l1, signed int l2, signed int r1, signed int r2)
{
  if(l1 >= 0)
  {
    PWM_1((unsigned)l1);
    MOTOROUT &=~IN5;
    MOTOROUT |= IN6;
  }
  else
  {
    PWM_1((unsigned)-l1);    
    MOTOROUT |= IN5;
    MOTOROUT &=~IN6;    
  }
  
  if(l2 >= 0)
  {
    PWM_2((unsigned)l2);
    MOTOROUT |= IN7;
    MOTOROUT &=~IN8;
    _DINT();
    L_dir = 0;
    _EINT();
  }
  else
  {
    PWM_2((unsigned)-l2);
    MOTOROUT &=~IN7;
    MOTOROUT |= IN8;    
    _DINT();
    L_dir = 1;
    _EINT();    
  }
  
  if(r1 >= 0)
  {
    PWM_3((unsigned)r1);
    MOTOROUT |= IN4;
    MOTOROUT &=~IN3;
  }
  else
  {
    PWM_3((unsigned)-r1);
    MOTOROUT &=~IN4;
    MOTOROUT |= IN3;    
  } 
  
  if(r2 >= 0)
  {
    PWM_4((unsigned)r2);
    MOTOROUT &=~IN2;
    MOTOROUT |= IN1;        
    _DINT();
    R_dir = 0;
    _EINT();    
  }
  else
  {
    PWM_4((unsigned)-r2);
    MOTOROUT |= IN2;
    MOTOROUT &=~IN1; 
    _DINT();
    R_dir = 1;
    _EINT();    
  }
}

void Motor_brake(unsigned char brake)
{
  if (brake | BIT0)
  {
    MOTOROUT  |= IN5;
    MOTOROUT  |= IN6;
  }
  if (brake | BIT1)
  {
    MOTOROUT  |= IN7;
    MOTOROUT  |= IN8;
  }
  if (brake | BIT2)
  {
    MOTOROUT  |= IN1;
    MOTOROUT  |= IN2;
  }
  if (brake | BIT3)
  {
    MOTOROUT  |= IN3;
    MOTOROUT  |= IN4;
  }  
}
