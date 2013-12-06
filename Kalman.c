#include <math.h>
#include "Kalman.h"
void Kalman_updata(Kalman* val,double Z,double R)
{
	double X_mid=val->X;
	double P_mid=val->P;
	double Kg=(R*R)/(P_mid*P_mid+R*R);
	val->X = X_mid+Kg*(Z-X_mid);
	val->P = sqrt((1-Kg)*R*R);
}