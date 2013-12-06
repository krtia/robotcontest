#ifndef Kalman_H_
#define Kalman_H_
struct Kalman
{
	double X,P;
}
extern void Kalman_D1_updata(Kalman* val,double Z,double R);
#endif