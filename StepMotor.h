#include"Global.h"
#ifdef StepMotor_Used_
#ifndef StepMotor_H_
extern unsigned char StepDir[2];
extern unsigned int StepCnt[2];
extern void StepMotor_init();
extern char StepMotor_set(unsigned int SM_Step,unsigned char SM_Dir,unsigned char Num);
#define StepMotor_interval 3      //步进电机速度定义，3为最大，可以改为变量来控制速度
#endif
#endif