#ifndef MPU6050_H_
#define MPU6050_H_
#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define MPU6050_Address 0xD0        //设备地址
#define omegaX_offset -13
#define accelZ_offset -2249
extern void MPU6050_init(void);
extern signed int MPU6050_GetAX();
extern signed int MPU6050_GetAY();
extern signed int MPU6050_GetAZ();
extern signed int MPU6050_GetGX();
extern signed int MPU6050_GetGY();
extern signed int MPU6050_GetGZ();
#endif 
