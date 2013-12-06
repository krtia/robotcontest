#include "msp430x54x.h"
#include "UCS.h"
#include "MMA8452.h"
#include "IIC.h"
void MMA8452_init(void)
{
  IIC_write(MMA8452_Address,0x2A,0x05);   //50Hz ODR 800Hz��ʱ1.25ms������ģʽ�������ٶ�ȡģʽ�ء������豸
  IIC_write(MMA8452_Address,0x2B,0x02);   //�Լ�ء������λ�ء�˯�߷�ʽ������ģʽ���Զ�˯�߹ء�ת��ģʽ���߷ֱ���
  delay_ms(10);
}
signed int MMA8452_GetAX()
{
  union
  {
  signed int ax;
  unsigned char n[2];
  };
  n[0]=IIC_read(MMA8452_Address,ACCEL_XOUT_L);
  n[1]=IIC_read(MMA8452_Address,ACCEL_XOUT_H);
  return ax;
}
signed int MMA8452_GetAY()
{
  union
  {
  signed int ay;
  unsigned char n[2];
  };
  n[0]=IIC_read(MMA8452_Address,ACCEL_YOUT_L);
  n[1]=IIC_read(MMA8452_Address,ACCEL_YOUT_H);
  return ay;
}
signed int MMA8452_GetAZ()
{
  union
  {
  signed int az;
  unsigned char n[2];
  };
  n[0]=IIC_read(MMA8452_Address,ACCEL_ZOUT_L);
  n[1]=IIC_read(MMA8452_Address,ACCEL_ZOUT_H);
  return az;
}
