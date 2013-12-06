#include<msp430f5438.h>
#include"Global.h"
#include"GP2Y0A02.h"
#include"Hall.h"

//�������ͨ��ʱ������ģʽΪ�ظ���ͨ��������ADC12MCTLn�Ĵ�����ע�����ý���λADC12EOS��������Ҫ�����ж�
//io������Ϊģ�������An
//��ʼ����Ϊ�ظ�����A0,A1����ͨ��(�Ӻ�����)��ͬʱA2��ʼ��(�ɽӻ���)
void ADC12_init()
{
  ADC12CTL0 |= ADC12SHT0_3+ ADC12MSC+ ADC12ON;  //���ò���ʱ��Ϊ32��adcʱ�����ڣ���β���������adc
  ADC12CTL1 |= ADC12SHP+ ADC12SSEL_3+ ADC12CONSEQ_3;    //ʹ�ܲ���ʱ�ӣ�ѡ��ʱ��ԴΪSMCLK, ����Ϊ�ظ�����ģʽ
  ADC12CTL2 |= ADC12RES_2;  //���÷ֱ���Ϊ12λ
  
  P6SEL |= BIT0;
  ADC12MCTL0|= ADC12SREF_0+ ADC12INCH_0;//����+-�ο���ѹ�ֱ�ΪVcc��Vss�� ͨ��ΪA0
  ADC12IE   |= ADC12IE0;                    //ʹ���ж�
  
  P6SEL |= BIT1;
  ADC12MCTL1|= ADC12SREF_0+ ADC12INCH_1;//����+-�ο���ѹ�ֱ�ΪVcc��Vss�� ͨ��ΪA1
  ADC12IE   |= ADC12IE1;                    //ʹ���ж�

  P6SEL |= BIT2;
  ADC12MCTL2|= ADC12SREF_0+ ADC12INCH_2;//����+-�ο���ѹ�ֱ�ΪVcc��Vss�� ͨ��ΪA2
  ADC12IE   |= ADC12IE2;                    //ʹ���ж�
  
  ADC12MCTL1|= ADC12EOS;                //���ý���λ
    
  ADC12CTL0 |= ADC12ENC;    //ʹ��ת��
}


#pragma vector=ADC12_VECTOR
__interrupt void ADC12ISR (void)
{
#ifdef GP2Y0A02_Used_
  static unsigned int index = 0;
#endif

  switch(ADC12IV)
  {
  case  0: break;                           // Vector  0:  No interrupt
  case  2: break;                           // Vector  2:  ADC overflow
  case  4: break;                           // Vector  4:  ADC timing overflow
  case  6:                                  // Vector  6:  ADC12IFG0
#ifdef GP2Y0A02_Used_
    results[0][index] = ADC12MEM0;             // Move results    
#endif
    break;
  case  8:                                  // Vector  8:  ADC12IFG1
#ifdef GP2Y0A02_Used_    
    results[1][index] = ADC12MEM1;             // Move results    
    index++;                                // Increment results index, modulo; Set Breakpoint1 here
    
    if (index % 500 == 0)
    {
      DataReady ++;                         //������ģʽ��DataReady = 0,2(δ׼����)�� =1(ǰ500��׼����)�� =3(��500��)
      if (DataReady == 4)
        DataReady = 0;
    }
    if (index == 1000)
    {
      index = 0;
    }
#endif    
    break;                           
  case 10:                                  // Vector 10:  ADC12IFG2
    Hall_Result = ADC12MEM2;
    break;
  case 12: break;                           // Vector 12:  ADC12IFG3
  case 14: break;                           // Vector 14:  ADC12IFG4
  case 16: break;                           // Vector 16:  ADC12IFG5
  case 18: break;                           // Vector 18:  ADC12IFG6
  case 20: break;                           // Vector 20:  ADC12IFG7
  case 22: break;                           // Vector 22:  ADC12IFG8
  case 24: break;                           // Vector 24:  ADC12IFG9
  case 26: break;                           // Vector 26:  ADC12IFG10
  case 28: break;                           // Vector 28:  ADC12IFG11
  case 30: break;                           // Vector 30:  ADC12IFG12
  case 32: break;                           // Vector 32:  ADC12IFG13
  case 34: break;                           // Vector 34:  ADC12IFG14
  default: break; 
  }
}