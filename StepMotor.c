#include"msp430f5438.h"

unsigned char StepDir[2];
unsigned int StepCnt[2];
//P3口控制两个步进电机(P3.0~P3.3,P3.4~P3.7)
void StepMotor_init()
{
    P3DIR = 0xFF;
    P3OUT = 0x00;
}

//入口参数:SM_step,转动的步数
//         SM_Dir, 转动的方向
//         Num,控制的步进电机的编号
//出口参数:错误码(1为该电机已经被占用，0为设置成功)
//步进电机具体实现在TimerA1中断函数中
char StepMotor_set(unsigned int SM_Step,unsigned char SM_Dir,unsigned char Num)
{
    if (StepCnt[Num] > 0)
        return 1;
    _DINT();
    StepCnt[Num] = SM_Step;
    StepDir[Num] = SM_Dir;
    _EINT();
    return 0;
}