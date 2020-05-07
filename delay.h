#ifndef DELAY_H
#define DELAY_H
#include "msp.h"

#define FREQ_1_5_MHZ 1
#define FREQ_3_MHZ 3
#define FREQ_6_MHZ 6
#define FREQ_12_MHZ 12
#define FREQ_24_MHZ 24
#define FREQ_48_MHZ 48

void set_DCO(uint32_t MHz);
void delay_us(uint32_t us);
void delay_ms(uint32_t delay);
#endif
