#include"Global.h"
#ifdef StepMotor_Used_
#ifndef StepMotor_H_
extern unsigned char StepDir[2];
extern unsigned int StepCnt[2];
extern void StepMotor_init();
extern char StepMotor_set(unsigned int SM_Step,unsigned char SM_Dir,unsigned char Num);
#define StepMotor_interval 3      //��������ٶȶ��壬3Ϊ��󣬿��Ը�Ϊ�����������ٶ�
#endif
#endif