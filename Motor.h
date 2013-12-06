#ifndef MOTOR_H_
#define MOTOR_H_
extern void Motor_init();
extern void Motor_config(signed int l1, signed int l2, signed int r1, signed int r2);
extern void Motor_brake(unsigned char);
extern volatile unsigned char L_dir,R_dir;
#endif