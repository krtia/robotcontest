#include "msp430f5438.h"

unsigned int Hall_Result;

//入口参数：无
//出口参数: ADC12转换值(unsigned int 0~4095)
unsigned int Hall_convert()
{
  Hall_Result = 0;
  
  ADC12CTL0 &=~ADC12ENC;                //结束当次转换
  while(ADC12CTL1 & ADC12BUSY);
  
  ADC12MCTL1&=~ADC12EOS;                
  ADC12MCTL2|= ADC12EOS;                //设置结束位
  
  ADC12CTL0 |= ADC12ENC;
  
  ADC12CTL0 |= ADC12SC;                 //开始下次转换（包含霍尔）
  
  while(!Hall_Result);
  
  ADC12CTL0 &=~ADC12ENC;
  while(ADC12CTL1 & ADC12BUSY);         //等待转换完成
  
  ADC12MCTL1|= ADC12EOS;                
  ADC12MCTL2&=~ADC12EOS;                //设置结束位
  
  ADC12CTL0 |= ADC12ENC;

  ADC12CTL0 |= ADC12SC;                 //重开之前转换
  return Hall_Result;  
}