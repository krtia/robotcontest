#include"msp430f5438.h"
#include"TimerA1.h"
#include"delay.h"

//DATA口定义为P10.1
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
  P10REN  |= BIT1;//使能上下拉电阻
  DATA_IN;
  DATA_SET;       //电阻改为上拉
}

void  COM(void)
{
    unsigned char i,j;
    for(i=0;i<8;i++)	   
    {        
        ERROR=2;
        while((!(DATA_R))&&(ERROR++));
        
        if(ERROR==1)                  //超时没有响应则跳出
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
        //超时则跳出for循环		  
        if(ERROR==1)break;
        //判断数据位是0还是1	 
                
        COMdata<<=1;
        COMdata|=temp; 
     }
}


//--------------------------------
//-----湿度读取子程序 ------------
//--------------------------------
//----以下变量均为全局变量--------
//----温度高8位== T_data_H------
//----温度低8位== T_data_L------
//----湿度高8位== RH_data_H-----
//----湿度低8位== RH_data_L-----
//----校验 8位 == checkdata-----
//----调用相关子程序如下----------
//---- delayms();, delayus();, COM(); 
//--------------------------------

void DHT11_measure(void)
{
  char i; 
  if (TimeBase % 1000 != 0)
    return;
  //主机拉低18ms 
    DATA_OUT;
    DATA_CLR;
    delayms(18);
    while(CountDown);
    DATA_IN;
    DATA_SET;
  
  //总线由上拉电阻拉高 主机延时20~40us
    for (i=0;i<10;i++)
    {
      delayus();
      delayus();
      delayus();
      delayus();
    }
  //主机设为输入 判断从机响应信号 
 
  //判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行	  
   if(DATA_R)
   {
     ERROR = 1;
     return;
   }
   while(!(DATA_R))		 
   {
       ERROR=2;
       //判断从机是否发出 80us 的低电平响应信号是否结束	 
       while((!(DATA_R))&&ERROR++);
       if(ERROR==1)break;
       ERROR=2;
     //判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
       while((DATA_R)&&ERROR++);
       if(ERROR==1)break;
     //数据接收状态		 
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
//数据校验 
 
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
      
   //释放总线
   DATA_IN;
   DATA_SET;
}
