#include"msp430f5438.h"

unsigned int CountDown = 0;

//׼ȷ��ʱ1us(�������������������ж�)
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

//��ڲ���:
//  n:��ʱ��ms��
//���ڲ���:
//  �������(0Ϊ�޴�1Ϊ��ǰ�к���������ʱ)
//����ʾ��(����ʽ):
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