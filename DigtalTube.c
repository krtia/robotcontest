#include"msp430f5438.h"
#include"Global.h"
#ifdef DigtalTube_Used_
unsigned int Display_Number[5];
unsigned char Digtal[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF};

//����ɨ�������TimerA1.c��
void DigtalTube_init()
{
  P7DIR |= BIT4+ BIT5+ BIT6+ BIT7;  //������һ���������
  P7OUT &= ~(BIT4+ BIT5+ BIT6+ BIT7);
  P9DIR = 0xFF;                     //������һλ��
  P9OUT = 0xFF;
}
/*
Number��ʽ:��һλ��ʾС���㣬4��ʾ��С���㣬0��1��2��3��ʾС�����ڵ�1��2��3��4���������
�� Number=00000, ����ʾΪ 0.000, Number=41234, ��ʾΪ1234
*/
void DigtalTube_set(unsigned int Number)
{ 
  Display_Number[0] =  Number/10000;
  Display_Number[1] =  Number/1000-Display_Number[0]*10;
  Display_Number[2] =  Number/100-Display_Number[0]*100-Display_Number[1]*10;
  Display_Number[3] =  Number/10-Display_Number[0]*1000-Display_Number[1]*100-Display_Number[2]*10;
  Display_Number[4] =  Number-Display_Number[0]*10000-Display_Number[1]*1000-Display_Number[2]*100-Display_Number[3]*10;
}
#endif