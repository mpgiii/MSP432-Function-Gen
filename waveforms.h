/*
 * waveforms.h
 *
 *  Created on: May 3, 2020
 *      Author: mpgii
 */

#ifndef WAVEFORMS_H_
#define WAVEFORMS_H_

/* define pi. self explanatory */
#define PI 3.14159265358979323846

/* use DAC_MULTIPLIER to turn a voltage into the input to the DAC.
 * Calculated using the max 12 bit number divided by the DAC input high
 * voltage. */
#define DAC_MULTIPLIER 1241

/* definitions of different frequencies */
#define FREQ_100_HZ 100
#define FREQ_200_HZ 200
#define FREQ_300_HZ 300
#define FREQ_400_HZ 400
#define FREQ_500_HZ 500

/* Running at 16 MHz, one cycle is 0.833333 us.
 * Trying to get a frequency of 50 Hz. */
#define TIMER_PERIOD_100_HZ 121000
#define TIMER_PERIOD_200_HZ 121000
#define TIMER_PERIOD_300_HZ 121000
#define TIMER_PERIOD_400_HZ 121000
#define TIMER_PERIOD_500_HZ 121000

/* bigger wave table size => higher resolution */
#define WAVE_TABLE_SIZE 128

/* function prototypes */
void generate_square_wave(int voltage, int duty_cycle);
void generate_triangle_wave(int voltage);

void setup_timer();
void TA0_0_IRQHandler();

/* table which is populated by waveforms.c */
extern int wave_table[WAVE_TABLE_SIZE];

#endif /* WAVEFORMS_H_ */
