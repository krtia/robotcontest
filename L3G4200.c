#include "msp430x54x.h"
#include "L3G4200.h"
#include "IIC.h"//…Ë±∏µÿ÷∑0xD2
void L3G4200_init()
{
   IIC_write(0xD2,CTRL_REG1, 0xcf);   //
   IIC_write(0xD2,CTRL_REG2, 0x00);   //
   IIC_write(0xD2,CTRL_REG3, 0x08);   //
   IIC_write(0xD2,CTRL_REG4, 0x30);  //+-2000dps
   IIC_write(0xD2,CTRL_REG5, 0x10);//enable FIFO
}
signed int L3G4200_GetX()
{
  union
  {
    int x;
    unsigned char val[2];
  };
  val[0]=IIC_read(0xD2,OUT_X_L);
  val[1]=IIC_read(0xD2,OUT_X_H);
  return x;
}
signed int L3G4200_GetY()
{  
  union
  {
    int y;
    unsigned char val[2];
  };
  val[0]=IIC_read(0xD2,OUT_Y_L);
  val[1]=IIC_read(0xD2,OUT_Y_H);
  return y;
}
signed int L3G4200_GetZ()
{
  union
  {
    int z;
    unsigned char val[2];
  };
  val[0]=IIC_read(0xD2,OUT_Z_L);
  val[1]=IIC_read(0xD2,OUT_Z_H);
  return z;
}