#include"msp430f5438.h"
#include"TimerA1.h"
#include"delay.h"

//DATA�ڶ���ΪP10.1
#define DATA_IN   P10DIR &= ~BIT1
#define DATA_OUT  P10DIR |= BIT1
#define DATA_SET  P10OUT |= BIT1
#define DATA_CLR  P10OUT &= ~BIT1
#define DATA_R    P10IN & BIT1

unsigned int ERROR=1;
unsigned char RH_data_H_temp,RH_data_L_temp,T_data_H_temp,T_data_L_temp;
unsigned char checkdata_temp,checkdata;
unsigned char RH_data_H,RH_data_L,T_data_H,T_data_L;
unsigned char COMdata,temp;

void DHT11_init(void)
{
  P10REN  |= BIT1;//ʹ������������
  DATA_IN;
  DATA_SET;       //�����Ϊ����
}

void  COM(void)
{
    unsigned char i,j;
    for(i=0;i<8;i++)	   
    {        
        ERROR=2;
        while((!(DATA_R))&&(ERROR++));
        
        if(ERROR==1)                  //��ʱû����Ӧ������
          break;
        
        for(j=0;j<10;j++)
        {
          delayus();
          delayus();          
          delayus();          
          delayus();
        }
        temp=0;
        if(DATA_R)
          temp=1;
        ERROR=2;
        while((DATA_R)&&(ERROR++));
        //��ʱ������forѭ��		  
        if(ERROR==1)break;
        //�ж�����λ��0����1	 
                
        COMdata<<=1;
        COMdata|=temp; 
     }
}


//--------------------------------
//-----ʪ�ȶ�ȡ�ӳ��� ------------
//--------------------------------
//----���±�����Ϊȫ�ֱ���--------
//----�¶ȸ�8λ== T_data_H------
//----�¶ȵ�8λ== T_data_L------
//----ʪ�ȸ�8λ== RH_data_H-----
//----ʪ�ȵ�8λ== RH_data_L-----
//----У�� 8λ == checkdata-----
//----��������ӳ�������----------
//---- delayms();, delayus();, COM(); 
//--------------------------------

void DHT11_measure(void)
{
  char i; 
  if (TimeBase % 1000 != 0)
    return;
  //��������18ms 
    DATA_OUT;
    DATA_CLR;
    delayms(18);
    while(CountDown);
    DATA_IN;
    DATA_SET;
  
  //������������������ ������ʱ20~40us
    for (i=0;i<10;i++)
    {
      delayus();
      delayus();
      delayus();
      delayus();
    }
  //������Ϊ���� �жϴӻ���Ӧ�ź� 
 
  //�жϴӻ��Ƿ��е͵�ƽ��Ӧ�ź� �粻��Ӧ����������Ӧ����������	  
   if(DATA_R)
   {
     ERROR = 1;
     return;
   }
   while(!(DATA_R))		 
   {
       ERROR=2;
       //�жϴӻ��Ƿ񷢳� 80us �ĵ͵�ƽ��Ӧ�ź��Ƿ����	 
       while((!(DATA_R))&&ERROR++);
       if(ERROR==1)break;
       ERROR=2;
     //�жϴӻ��Ƿ񷢳� 80us �ĸߵ�ƽ���緢����������ݽ���״̬
       while((DATA_R)&&ERROR++);
       if(ERROR==1)break;
     //���ݽ���״̬		 
       COM();
       if(ERROR==1)break;
       RH_data_H_temp=COMdata;
       COM();
       if(ERROR==1)break;
       RH_data_L_temp=COMdata; 
       COM();
       if(ERROR==1)break;
       T_data_H_temp=COMdata;
       COM();
       if(ERROR==1)break;
       T_data_L_temp=COMdata;
       COM();
       if(ERROR==1)break;
       checkdata_temp=COMdata;
       break;
    }
    
    DATA_IN;
    DATA_SET;

    if (ERROR==1)
      return;
//����У�� 
 
   temp=(T_data_H_temp+T_data_L_temp+RH_data_H_temp+RH_data_L_temp);
   if(temp==checkdata_temp)
   {
        RH_data_H = RH_data_H_temp;
        RH_data_L = RH_data_L_temp;
        T_data_H  = T_data_H_temp;
        T_data_L  = T_data_L_temp;
        checkdata = checkdata_temp;
        ERROR = 0;          
   }
      
   //�ͷ�����
   DATA_IN;
   DATA_SET;
}
