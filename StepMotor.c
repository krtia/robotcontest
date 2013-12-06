#include"msp430f5438.h"

unsigned char StepDir[2];
unsigned int StepCnt[2];
//P3�ڿ��������������(P3.0~P3.3,P3.4~P3.7)
void StepMotor_init()
{
    P3DIR = 0xFF;
    P3OUT = 0x00;
}

//��ڲ���:SM_step,ת���Ĳ���
//         SM_Dir, ת���ķ���
//         Num,���ƵĲ�������ı��
//���ڲ���:������(1Ϊ�õ���Ѿ���ռ�ã�0Ϊ���óɹ�)
//�����������ʵ����TimerA1�жϺ�����
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