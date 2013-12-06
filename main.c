#include"msp430f5438.h"
#include"Global.h"

#include "msp430_math.h"
#include"WDT.h"
#include"UCS.h"
#include"TimerA1.h"
#include"TimerB0.h"
#include"delay.h"
//#include"adc12.h"
//#include"RTC.h"
//#include"DMA.h"
//#include"MPY.h"
//#include"DigtalTube.h"
//#include"I2C.h"
//#include"LCD12864.h"
//#include"DHT11.h"
//#include"TimerA1_PWM.h"
//#include"GP2Y0A02.h"
//#include"UltraSonic.h"
//#include"Hall.h"

#include"Motor.h"

#include"PhotoelectricEncoder.h"
//#include"RotaryEncoder.h"
#include"Traction.h"

#include"UART.h"
//#include"StepMotor.h"

//#include "IIC.h"
//#include"HMC5883.h"
//#include"L3G4200.h"
//#include"MMA8452.h"
//#include"MPU6050.h"

#include"PID.h"

volatile unsigned char distState;
int j,i=500,flag=1,k=1,tx;
unsigned long nowTime,lastDist=0xFFFF,startTime;
//gned long theta=0;
PID_struct Motor_L,Motor_R;
/*
PID_struct Turn;
signed long Setpoint = 100;
signed char dir;
signed int speed;
signed int omegaX,omegaY,omegaZ;
signed int accelX,accelY,accelZ;
signed long intAccelX,intAccelY,intAccelZ;
signed int RA,GA,SA,EA;
signed long thetaX;
PID_struct qiao;*/

unsigned char command;

/*
double getAngle(void)
{
    signed int dirX,dirY,dirZ;  
    double theta;
    dirX=(HMC5883_Get_x()-141)*100/78;
    dirY=HMC5883_Get_y()-0;//没有使用
    dirZ=HMC5883_Get_z()-1319;
    if (dirX!=0)
    {
      theta=atan((double)dirZ/(double)dirX);
      if (dirX<0)
        theta+=3.141;
      else if (dirZ<0)
        theta+=6.283;
    }
    else
    {
      if (dirZ>0)
        theta=1.570;
      else
        theta=4.712;
    }
    return theta;
}*/

signed int abs(signed int a)
{
  if (a<0) return -a;
  else return a;
}

int main( void )
{
  WDT_init();
  
  UCS_init();
  
  TimerA1_init();
  
  TimerB0_init();
  
//  ADC12_init();
  
//  RTC_init();
  
//  DMA_init();
  
//  DigtalTube_init();
  
//  I2C_Master_init();
  
//  DHT11_init();  
  
//  TimerA1_PWM_init();
  
  
//  UltraSonic_init();

  UART_init(UCA1,9600);
  
  Motor_init();
  
  PhotoelectricEncoder_init();
  
//  RotaryEncoder_init();
  
  Traction_init();
  
//  StepMotor_init();
  
//  HMC5883_init();
  
//  IIC_init();
  
//  L3G4200_init();
  
//  MMA8452_init();
  
//  MPU6050_init();
  
//扫描版避障
  /*  
  P1DIR &=~BIT4;
  P1DIR &=~BIT5;
  P1DIR &=~BIT6;
  P1DIR &=~BIT7;
  */
//中断版避障  
  /*
  P1IES |= BIT4;
  P1IES |= BIT5;
  P1IES |= BIT6;
  P1IES |= BIT7;
  P1IFG=0;
  P1IE |=BIT4+BIT5+BIT6+BIT7;  
  */
  
  _EINT();

/*  
  RTC_setTypeDef ref;
  ref.year = 1970;
  ref.dow  = 3;
  
  while(RTC_set(&ref));
*/  
  

//  ADC12CTL0 |= ADC12SC;
//  UCB0IFG |= UCTXIFG;
  
//  TimerA1_PWM_2(3000);

//  while(StepMotor_set(5000,1,0));

//刹车测试
/*
  P4DIR |= BIT7;
  P4OUT &= ~BIT7;  
  Motor_config(1000,1000,-1000,-1000);
  while(TimeBase!=500);
  Motor_brake(0x0F);
  P4OUT |= BIT7;
  while(!(L_speed==0) || !(R_speed ==0));
  P4OUT &=~BIT7;
*/

//PID init
/*  
  Motor_L.myOutput = 0;
  Motor_L.myInput = &L_speed;
  Motor_L.mySetpoint = 200;
  Motor_L.inAuto = 1;
  PID_setOutputLimits(&Motor_L, 0, (signed long)1000*Accuracy);
  Motor_L.SampleTime = 10;
  PID_setControllerDirection(&Motor_L, 0);
  PID_setTunings(&Motor_L, 2100, 3800, 70);//kp2100,ki4000
  if (TimeBase>Motor_L.SampleTime)
    Motor_L.lastTime = TimeBase-Motor_L.SampleTime;
  else
    Motor_L.lastTime = 0;
  
  Motor_R=Motor_L;
  Motor_R.mySetpoint = 201;
  Motor_R.myInput = &R_speed;
  
  Motor_config(600,600,600,600);
  while(TimeBase!=500);
*/  
/*
  qiao.myOutput = 0;
  qiao.myInput = &thetaX;
  qiao.mySetpoint = 0;
  qiao.inAuto = 1;
  PID_setOutputLimits(&qiao, (signed long)-800, (signed long)800);
  qiao.SampleTime = 10;
  PID_setControllerDirection(&qiao, 0);
  PID_setTunings(&qiao, 80, 20, 20);//kp2100,ki4000
  if (TimeBase>qiao.SampleTime)
    qiao.lastTime = TimeBase-qiao.SampleTime;
  else
    qiao.lastTime = 0;*/
  
//  while(UCA1_GET_CHAR(&command));
//  startTime = TimeBase;
  Motor_config(600,600,600,600);
  
  while(1)
  {
    
    
      if (nowTime != TimeBase)
      {
        nowTime = TimeBase;     //每一毫秒运行一次
        if((xianshi+250>TimeBase)||(xianshi == 0))
        {
          if ((!(P1IN & BIT1))&&(P1IN & BIT0))
            Motor_config(700,700,700,700);
          else if((P1IN & BIT1)&&((P1IN & BIT0)))
            Motor_config(1000,1000,0,0);
          else if ((!(P1IN & BIT1))&&(!(P1IN & BIT0)))
            Motor_config(500,500,1000,1000);
          else
            Motor_config(500,500,500,500);
        }
/*        if ((nowTime % 50 == 0)&&(nowTime < startTime+5000))
        {
//          accelX=(accelX+MMA8452_GetAX())/2L;
//          accelY=(accelY+MMA8452_GetAY())/2L;
//          accelZ=(accelZ+MMA8452_GetAZ())/2L;
          intAccelX += accelX;
          intAccelY += accelY;
          intAccelZ += accelZ;
          omegaX=L3G4200_GetX();
          omegaY=L3G4200_GetY();
          omegaZ=L3G4200_GetZ();
          RA = RA + (signed long)omegaX*2000/32768;//角速度积分，定点精度两位小数
          GA = (signed int)(acos((double)accelZ/16384)/3.14159*18000);
          if (accelY < 0)
            GA=-GA;
          EA = (GA-RA)/3; //尝试/3衰减
          if (EA > 30)
            EA=30;
          else if (EA < -30)
            EA=-30;
          SA = RA + EA;
          thetaX = SA;
          
          UART_sendint(UCA1, (unsigned)(omegaX+32768));
          UART_sendstr(UCA1, " ");
          UART_sendint(UCA1, (unsigned)(omegaY+32768));
          UART_sendstr(UCA1, " ");
          UART_sendint(UCA1, (unsigned)(omegaZ+32768));
          UART_sendstr(UCA1, " ");          
          
          UART_sendint(UCA1, (unsigned)(accelX+32768));
          UART_sendstr(UCA1, " ");
          UART_sendint(UCA1, (unsigned)(accelY+32768));
          UART_sendstr(UCA1, " ");
          UART_sendint(UCA1, (unsigned)(accelZ+32768));
          UART_sendstr(UCA1, " ");          */
//          UART_sendint(UCA1, (unsigned)(RA+32768));
//          UART_sendstr(UCA1, " ");          
//          UART_sendint(UCA1, (unsigned)(GA+32768));
//          UART_sendstr(UCA1, " ");                    
//          UART_sendint(UCA1, (unsigned)(SA+32768));
//          UART_sendstr(UCA1, " ");                    

//          PID_compute(&qiao);
          
/*          if((qiao.myOutput < 500)&&(qiao.myOutput > -500))
            Motor_brake(0xFF);
          else
          {
            if (qiao.myOutput>0)
              Motor_config((qiao.myOutput-500)/5+500,(qiao.myOutput-500)/5+500,(qiao.myOutput-500)/5+500,(qiao.myOutput-500)/5+500);
            else
              Motor_config((qiao.myOutput+500)/5-500,(qiao.myOutput+500)/5-500,(qiao.myOutput+500)/5-500,(qiao.myOutput+500)/5-500);
          }*/
//          Motor_config(qiao.myOutput,qiao.myOutput,qiao.myOutput,qiao.myOutput);
        }
//角速度L3G4200控制跷跷板
/*        
        if (nowTime % 10 == 0)//&&(nowTime < startTime+5000))
        {
          
//          UART_sendint(UCA0, (unsigned)(omegaY+32768));
//          UART_sendstr(UCA0, " ");
          
          PID_compute(&qiao);
          
//         UART_sendint(UCA0, (unsigned)(qiao.myOutput+32768));
//          UART_sendstr(UCA0, " ");
          
          if((qiao.myOutput < 500)&&(qiao.myOutput > -500))
            Motor_brake(0xFF);
          else
          {
            if (qiao.myOutput>0)
              Motor_config((qiao.myOutput-500)/5+500,(qiao.myOutput-500)/5+500,(qiao.myOutput-500)/5+500,(qiao.myOutput-500)/5+500);
            else
              Motor_config((qiao.myOutput+500)/5-500,(qiao.myOutput+500)/5-500,(qiao.myOutput+500)/5-500,(qiao.myOutput+500)/5-500);
          }
        }*/
        /*
        else if (nowTime > 5000+startTime)
        {
          Motor_config(0,0,0,0);
          while(UCA0_GET_CHAR(&command));
          startTime = TimeBase;
          Motor_config(700,700,700,700);
        }        */
//电子罗盘（固定朝向）        
/*
        if (nowTime % 20 == 0)
        {
          theta = (signed long)(getAngle()*1000);
          if (!UCA1_GET_CHAR(&command))
          {
            Setpoint = (command-48)*1571;
          }
          
          if(abs(theta - Setpoint)>50)
          {
            if (theta <Setpoint)
            {
              if ((Setpoint-theta)<3140)
                dir = 1;//counter-clockwise
              else
                dir = -1;
            }
            else
            {
              if ((theta-Setpoint)<3140)
                dir = -1;
              else
                dir = 1;              
            }
            if(abs(theta-Setpoint)<3140)
              speed = (signed int)(abs(theta-Setpoint));
            else
              speed = (signed int)((6280-abs(theta-Setpoint)));
            speed = speed/8+600;
            if (speed >1000) speed =1000;
            Motor_config(-speed*dir,-speed*dir,speed*dir,speed*dir);            
          }
          else
            Motor_config(0,0,0,0);   
        }*/
//电子罗盘（转圈返回角度）
        /*
        if ((nowTime % 20 == 0)&&(nowTime > 50)&&(nowTime < startTime+10000))
        {

          UART_sendint(UCA1, (unsigned)(dirX+5000));
          UART_sendstr(UCA1, " ");
          UART_sendint(UCA1, (unsigned)(dirZ+5000));
          UART_sendstr(UCA1, " ");
          UART_sendint(UCA1, (unsigned)(getAngle()*1000));
          UART_sendstr(UCA1, " ");
          _NOP();
        }
        else if (nowTime > 10000+startTime)
        {
          Motor_config(0,0,0,0);
          while(UCA1_GET_CHAR(&command));
          startTime = TimeBase;
          Motor_config(-700,-700,700,700);
        }*/

//避障(while循环最小10ms)    
/*            
        if ((nowTime % 20 ==0)&&(!(distState & BIT0)))
        {
          distState = ~P1IN;
          distState &= 0xF0;
          if (distState | 0x00)
          {
            if ((distState & 0x10)&&(distState & 0x40))
              Motor_config(-500,-500,700,700);
            else if ((distState & 0x40)&&(distState & 0x80))            
              Motor_config(0,0,700,700);
            else if (distState & 0x40)
              Motor_config(-700,-700,700,700);
            else if (distState & 0x80)
              Motor_config(700,700,700,700);
//            else Motor_config(0,0,0,0);
          }
          else
            Motor_config(850,850,475,475);
        }
        
        if (nowTime % 50 == 0)
        {
          lastDist = sonicDist;
          UltraSonic_Tx();
          if (lastDist < 50)
          {
            Motor_config(-700,-700,700,700);
            distState |= BIT0;
          }
          else
            distState &=~BIT0;
        }*/
//      }

//PID
/*    
    if((!PID_compute(&Motor_L))||(!PID_compute(&Motor_R)))
    {
      Motor_config(Motor_L.myOutput,Motor_L.myOutput,Motor_R.myOutput,Motor_R.myOutput);
      tx=Motor_L.myOutput;
    }
*/    
//PID
    
//加减速、正反

    /*
    nowtime = TimeBase;
    if((nowtime % 1000 == 0)
       &&(k!=nowtime)) 
    {
      k=nowtime;
      j=(TimeBase/1000)%12;
      if(0<=j&&j<=5) i=500+j*100;
      else if(6<=j&&j<=10) i=1500-j*100;
      else 
      {
        i=0;
        flag++;
      }
      if (flag)
        Motor_config(i,i,i,i);
      else Motor_config(-i,-i,-i,-i);
//      flag ^=1;

    }*/

//超声波测试    
/*    if (TimeBase %1000==0)
    {
      UltraSonic_Tx();
      while(TimeBase %1000==0);
    }
    
    if (sonicDist!=0)
      sonicDist=0;*/
//红外测试
//   if (DataReady % 2 == 1)
//      GP2Y0A02_DataProcess();
//PWM
//    WDT_PWM1();

  }
}
