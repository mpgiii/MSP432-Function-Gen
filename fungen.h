/* fungen.h
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020 */

#ifndef FUNGEN_H_
#define FUNGEN_H_

/* define constant values */
#define DEFAULT_DUTY_CYCLE 50
#define DEFAULT_VOLTAGE 3

#define DUTY_CYCLE_INCREMENT 10
#define MAX_DUTY_CYCLE 90
#define MIN_DUTY_CYCLE 10

#define LINE_SIZE 20

/* function prototypes */
void fungen_init();
void fungen_update_freq(uint16_t freq);
void fungen_print_wave(uint8_t duty_cycle);
uint8_t increase_duty_cycle(uint8_t duty_cycle);
uint8_t decrease_duty_cycle(uint8_t duty_cycle);

#endif /* FUNGEN_H_ */
