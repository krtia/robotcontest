#include"msp430f5438.h"

//该版本I2C没有经过调试，不要使用

unsigned char RXData,TXData;
void I2C_Master_init()
{
    // Configure pins
    P3SEL     |= BIT1 + BIT2;

    UCB0CTL1 |= UCSWRST;	            //Software reset enabled
    UCB0CTL0 |= UCMODE_3  + UCMST + UCSYNC;//I2C mode, Master mode, sync
    UCB0CTL1 |= UCSSEL_1;                  //ACLK
    
//    UCB0BRW    = 0x0077; 		    // baud rate = SMCLK / 119
    UCB0I2CSA  = 0x48;		            //slave address
    UCB0CTL1  &=~UCSWRST;
    
    UCB0IE    |= UCTXIE;
}
/*
void I2C_Slave_init()
{
    P3SEL     |= BIT1 + BIT2;               // Assign I2C pins to USCI_B0
    UCB0CTL1  |= UCSWRST;                   // Enable SW reset
    UCB0CTL0  = UCMODE_3 + UCSYNC;          // I2C Slave, synchronous mode
    UCB0I2COA = 0x48;                       // Own Address is 48h
    UCB0CTL1  &= ~UCSWRST;                  // Clear SW reset, resume operation
    UCB0IE    |= UCRXIE+UCTXIE;             // Enable TX interrupt
                                            
}*/

// USCI_B0 Data ISR
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(UCB0IV)
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4:                                 // Vector  4: NACKIFG
    UCB0IFG &=  ~UCNACKIFG;
    break;  
  case  6:                                 // Vector  6: STTIFG
    UCB0TXBUF = TXData;
    break;
  case  8: break;                           // Vector  8: STPIFG
  case 10:                                  // Vector 10: RXIFG
    RXData = UCB0RXBUF;                     // Get RX data
    TXData = RXData;
    break;
  case 12:                                  // Vector 12: TXIFG
    UCB0TXBUF = ++TXData;
  default: break; 
  }
}
