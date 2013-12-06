#include "msp430f5438.h"

unsigned int Hall_Result;

//��ڲ�������
//���ڲ���: ADC12ת��ֵ(unsigned int 0~4095)
unsigned int Hall_convert()
{
  Hall_Result = 0;
  
  ADC12CTL0 &=~ADC12ENC;                //��������ת��
  while(ADC12CTL1 & ADC12BUSY);
  
  ADC12MCTL1&=~ADC12EOS;                
  ADC12MCTL2|= ADC12EOS;                //���ý���λ
  
  ADC12CTL0 |= ADC12ENC;
  
  ADC12CTL0 |= ADC12SC;                 //��ʼ�´�ת��������������
  
  while(!Hall_Result);
  
  ADC12CTL0 &=~ADC12ENC;
  while(ADC12CTL1 & ADC12BUSY);         //�ȴ�ת�����
  
  ADC12MCTL1|= ADC12EOS;                
  ADC12MCTL2&=~ADC12EOS;                //���ý���λ
  
  ADC12CTL0 |= ADC12ENC;

  ADC12CTL0 |= ADC12SC;                 //�ؿ�֮ǰת��
  return Hall_Result;  
}