/* waveforms.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020 */

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

/* periods for given frequency, running at 24 MHz */
#define TIMER_PERIOD_100_HZ 240000
#define TIMER_PERIOD_200_HZ 120000
#define TIMER_PERIOD_300_HZ 80000
#define TIMER_PERIOD_400_HZ 60000
#define TIMER_PERIOD_500_HZ 48000

/* bigger wave table size => higher resolution */
#define WAVE_TABLE_SIZE 128

/* function prototypes */
void populate_square_table(uint8_t voltage, uint8_t duty_cycle);
void populate_saw_table(uint8_t voltage);
void populate_sine_table(uint8_t voltage);

void setup_timer(uint16_t freq);
void TA0_0_IRQHandler();


/* pointer to table currently being sent to the DAC */
extern uint16_t* wave_table;

/* tables which are populated by waveforms.c */
extern uint16_t square_table[WAVE_TABLE_SIZE];
extern uint16_t sine_table[WAVE_TABLE_SIZE];
extern uint16_t saw_table[WAVE_TABLE_SIZE];

#endif /* WAVEFORMS_H_ */
