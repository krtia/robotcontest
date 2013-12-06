#ifndef PID_H_
#define PID_H_
#define Accuracy 100            //С������ڵڼ�λ����10,100,1000,����ֵ�����ֵ����Ҫ�����������

typedef struct
{
    #define AUTOMATIC	1
    #define MANUAL	0
    #define DIRECT      0
    #define REVERSE     1
  
    signed long kp;                  // * (P)roportional Tuning Parameter
    signed long ki;                  // * (I)ntegral Tuning Parameter
    signed long kd;                  // * (D)erivative Tuning Parameter

    unsigned char controllerDirection;

    signed long *myInput;   
    signed long myOutput;   
    signed long mySetpoint; 
    
    signed long lastInput;		  
    unsigned long lastTime;
    signed long ITerm;
    
    unsigned int SampleTime;
    signed long outMin, outMax;

    unsigned char inAuto;
}PID_struct;

unsigned char PID_compute(PID_struct *pid);
void PID_setTunings(PID_struct *pid, unsigned long Kp, unsigned long Ki, unsigned long Kd);
void PID_setSampleTime(PID_struct *pid, unsigned int NewSampleTime);
void PID_setOutputLimits(PID_struct *pid, signed long Min, signed long Max);
void PID_setMode(PID_struct *pid, unsigned char Mode);
void PID_setControllerDirection(PID_struct *pid, unsigned char Direction);
void PID_init(PID_struct *pid);
#endif