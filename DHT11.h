#ifndef DHT11_H_
#define DHT11_H_
extern void DHT11_init(void);
extern void DHT11_measure(void);
extern unsigned char RH_data_H,RH_data_L,T_data_H,T_data_L;
#endif