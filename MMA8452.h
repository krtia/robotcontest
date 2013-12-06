#ifndef MMA8452_H_
#define MMA8452_H_
#define	ACCEL_XOUT_H	0x01
#define	ACCEL_XOUT_L	0x02
#define	ACCEL_YOUT_H	0x03
#define	ACCEL_YOUT_L	0x04
#define	ACCEL_ZOUT_H	0x05
#define	ACCEL_ZOUT_L	0x06
#define MMA8452_Address 0x38        //…Ë±∏µÿ÷∑
extern void MMA8452_init(void);
extern signed int MMA8452_GetAX();
extern signed int MMA8452_GetAY();
extern signed int MMA8452_GetAZ();
#endif 
