#include"msp430f5438.h"

unsigned long MPY_16x16u(unsigned int a, unsigned int b)
{
  unsigned long temp;
  
  _DINT();
  MPY32CTL0 = MPYDLYWRTEN;   //延迟到所有结果寄存器准备好
    
  MPY = a;                             // Load first operand -unsigned mult
  OP2 = b;                             // Load second operand

  _EINT();
  
  temp = RESHI;
  return ((temp << 16) + RESLO);
}

//remain unimplemented DO NOT USE
unsigned long long MPY_32x32u(unsigned long a, unsigned long b)
{
  unsigned long long temp=0;

  _DINT();  
  MPY32CTL0 = MPYDLYWRTEN;   //延迟到所有结果寄存器准备好
//  MPY32CTL0 |= OP1_32+ OP2_32;//选择32位模式

  
  MPY32L = a &  0xFFFF;
  MPY32H = a >> 16;
  OP2L   = b &  0xFFFF;
  OP2H   = b >> 16;
  
  _EINT();
  __no_operation();
  return temp;
}