#include "msp430x54x.h"
#include "UCS.h"
#include "MPU6050.h"
#include "IIC.h"
void MPU6050_init(void)
{
  IIC_init();
  IIC_write(MPU6050_Address,PWR_MGMT_1, 0x00);	
  delay_ms(10);
  //DEVICE_RESET_0+SLEEP_0+CYCLE_0+TEMP_DIS_0+CLKSEL_0
  //����λ���ϵ��Զ���λ�������ߡ�ʹ���¶ȴ�������ʹ���ڲ�8MHz����
  IIC_write(MPU6050_Address,SMPLRT_DIV, 0x07);//��������=����������ٶ�/(1+7)=125Hz
  delay_ms(10);
  IIC_write(MPU6050_Address,CONFIG, 0x06);//�ر�FSYNC��������,���ʱ��
  delay_ms(10);
  IIC_write(MPU6050_Address,GYRO_CONFIG, 0x18);//���Լ�⣬FS_SEL_3��2000��/s
  delay_ms(10);
  IIC_write(MPU6050_Address,ACCEL_CONFIG, 0x01);//���Լ�⣬AFS_SEL_2:2���������ٶȣ���ֹƵ��5Hz
  delay_ms(100);
}
signed int MPU6050_GetAX()
{
  union
  {
  signed int ax;
  unsigned char n[2];
  };
  n[0]=IIC_read(MPU6050_Address,ACCEL_XOUT_L);
  n[1]=IIC_read(MPU6050_Address,ACCEL_XOUT_H);
  return ax;
}
signed int MPU6050_GetAY()
{
  union
  {
  signed int ay;
  unsigned char n[2];
  };
  n[0]=IIC_read(MPU6050_Address,ACCEL_YOUT_L);
  n[1]=IIC_read(MPU6050_Address,ACCEL_YOUT_H);
  return ay;
}
signed int MPU6050_GetAZ()
{
  union
  {
  signed int az;
  unsigned char n[2];
  };
  n[0]=IIC_read(MPU6050_Address,ACCEL_ZOUT_L);
  n[1]=IIC_read(MPU6050_Address,ACCEL_ZOUT_H);
  return az;
}
signed int MPU6050_GetGX()
{
  union
  {
  signed int gx;
  unsigned char n[2];
  };
  n[0]=IIC_read(MPU6050_Address,GYRO_XOUT_L);
  n[1]=IIC_read(MPU6050_Address,GYRO_XOUT_H);
  return gx;
}
signed int MPU6050_GetGY()
{
  union
  {
  signed int gy;
  unsigned char n[2];
  };
  n[0]=IIC_read(MPU6050_Address,GYRO_YOUT_L);
  n[1]=IIC_read(MPU6050_Address,GYRO_YOUT_H);
  return gy;
}
signed int MPU6050_GetGZ()
{
  union
  {
  signed int gz;
  unsigned char n[2];
  };
  n[0]=IIC_read(MPU6050_Address,GYRO_ZOUT_L);
  n[1]=IIC_read(MPU6050_Address,GYRO_ZOUT_H);
  return gz;
}