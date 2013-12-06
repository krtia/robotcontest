#include "msp430f5438.h"
#include "Global.h"
#ifdef GP2Y0A02_Used_
/*
需要和ADC12.c配合使用
*/
#define   Num_of_Results   1000
  
volatile unsigned int results[2][Num_of_Results];
                                            // Needs to be global in this
                                            // example. Otherwise, the
                                            // compiler removes it because it
                                            // is not used for anything.

unsigned char DataReady;
double dist1,dist2,dist3;

//入口参数：aver1,aver2,分别为2个红外测距的转换值(0~4095)
//出口参数：无
//处理结果存放在dist1,dist2两个全局变量中
//dist3用于调试
//此函数暂时没有完成

void GP2Y0A02_DistCompute(double aver1,double aver2)
{
  double volt1,volt2;
  double dao=1/aver1;
  
    volt1 = (aver1*3.3)/4095;       //3.3v参考电压下信号电压值
    
    if (volt1>2.221)
      dist1 = 20.0-(volt1-2.221)*23.9;
    else if (volt1 >2.006)
      dist1 = 25.0-(volt1-2.006)*23.3;
    else if (volt1 >1.768)
      dist1 = 30.0-(volt1-1.768)*21;
    else if (volt1 >1.546)
      dist1 = 35.0-(volt1-1.546)*22.5;    
    else if (volt1 >1.360)
      dist1 = 40.0-(volt1-1.360)*26.9;
    else if (volt1 >0.87)
      dist1 = 70.0-(volt1-0.87)*6;

    volt2 = (aver2*3.3)/4095;       //3.3v参考电压下信号电压值
    
    if (volt2>2.221)
      dist2 = 20.0-(volt2-2.221)*23.9;
    else if (volt2 >2.006)
      dist2 = 25.0-(volt2-2.006)*23.3;
    else if (volt2 >1.768)
      dist2 = 30.0-(volt2-1.768)*21;
    else if (volt2 >1.546)
      dist2 = 35.0-(volt2-1.546)*22.5;    
    else if (volt2 >1.360)
      dist2 = 40.0-(volt2-1.360)*26.9;
    else if (volt2 >0.87)
      dist2 = 70.0-(volt2-0.87)*6;
    
    dist3=841010014232.0*dao*dao*dao-1808548607.43*dao*dao+1319702.47663*dao-289.751141638; //参数还没调
    __no_operation();
}

//500次取值去掉最大最小各100个
//需要加入卡尔曼滤波

void GP2Y0A02_DataProcess()
{
  unsigned char j;
  unsigned int i,shift,maxi,mini;
  unsigned int max,min;
  unsigned long sum1=0, sum2=0;
  
  if (DataReady == 1)
    shift = 0;
  else shift = 500;

  for (j=0;j<100;j++)    //先尝试去掉100个最大、100个最小,再取平均
  {
    max = 0;
    min = 65535;
    for (i=shift; i<500+shift; i++)
    {
      if ((min>results[0][i])&&(results[0][i]!=0))
      {
        min = results[0][i];
        mini = i;
      }
      if ((max<results[0][i])&&(results[0][i]!=0))
      {
        max = results[0][i];
        maxi = i;
      }
    }
    results[0][mini]=0;
    results[0][maxi]=0;
  }
/*
  for (j=0;j<100;j++)
  {
    max = 0;
    min = 65535;
    for (i=shift; i<500+shift; i++)
    {
      if ((min>results[i][1])&&(results[i]!=0))
      {
        min = results[i][1];
        mini = i;
      }
      if ((max<results[i][1])&&(results[i]!=0))
      {
        max = results[i][1];
        maxi = i;
      }
    }
    results[mini][1]=0;
    results[maxi][1]=0;
  }*/
  
  for (i=shift; i<500+shift; i++)
  {
    sum1  +=results[0][i];
    sum2  +=results[1][i];
  }
  
  GP2Y0A02_DistCompute((double)sum1/300,(double)sum2/300);
  DataReady ++;
}

//作为调试参数使用
//sum为500*2000即一百万次采样结果的总合
//可将断电设置在sum=0;一行观察每次sum的变化
void GP2Y0A02_Calibration()
{
  static unsigned long long sum=0;
  static unsigned int cnt=0;
  unsigned int i,shift;
  
  if (DataReady == 1)
    shift = 0;
  else shift = 500;
  
  for (i=shift; i<500+shift; i++)
  {
    sum  +=results[i][0];
  }
  cnt++;
  if (cnt==2000)
  {
    cnt=0;
    sum=0;
  }
  DataReady++;
}
#endif