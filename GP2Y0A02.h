#include"Global.h"
#ifdef  GP2Y0A02_Used_
#ifndef GP2Y0A02_H_
#define GP2Y0A02_H_
extern volatile unsigned int results[][1000];
extern unsigned char DataReady;
extern double dist1,dist2;

extern void GP2Y0A02_DistCompute(double aver1,double aver2);
extern void GP2Y0A02_DataProcess();
#endif
#endif