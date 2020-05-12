/* P2 - Function Generator
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020 */

#include "msp.h"
#include "spi.h"
#include "delay.h"
#include "waveforms.h"
#include "keypad.h"
#include "LCD.h"
#include "fungen.h"

void main(void) {
    /* character to store keypad input */
    char c;

    /* initialize duty cycle to 50% for square wave */
    uint8_t duty_cycle = DEFAULT_DUTY_CYCLE;

    /* stop watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    /* run all init functions for the function generator */
    fungen_init();

    while(1) {
        /* get input from keypad */
        c = keypad_get_key();

        /* if 1-5 were pressed, update the frequency */
        if (c == '1') {
            fungen_update_freq(FREQ_100_HZ);
        }
        else if (c == '2') {
            fungen_update_freq(FREQ_200_HZ);
        }
        else if (c == '3') {
            fungen_update_freq(FREQ_300_HZ);
        }
        else if (c == '4') {
            fungen_update_freq(FREQ_400_HZ);
        }
        else if (c == '5') {
            fungen_update_freq(FREQ_500_HZ);
        }

        /* if 7-9 are pressed, set the global to point to the
         * correct wave table */
        else if (c == '7') {
            wave_table = square_table;
        }
        else if (c == '8') {
            wave_table = sine_table;
        }
        else if (c == '9') {
            wave_table = saw_table;
        }

        /* the '*' key decreases the duty cycle by 10% */
        else if (c == '*') {
            duty_cycle = decrease_duty_cycle(duty_cycle);
            populate_square_table(DEFAULT_VOLTAGE, duty_cycle);
            delay_us(10000); /* delay to avoid button bounce */
        }

        /* the '0' key increases the duty cycle by 10% */
        else if (c == '0') {
            duty_cycle = increase_duty_cycle(duty_cycle);
            populate_square_table(DEFAULT_VOLTAGE, duty_cycle);
            delay_us(10000); /* delay to avoid button bounce */
        }

        /* the '#' key sets the duty cycle to default */
        else if (c == '#') {
            duty_cycle = DEFAULT_DUTY_CYCLE;
            populate_square_table(DEFAULT_VOLTAGE, duty_cycle);
        }

        /* write out the current information to the LCD */
        fungen_print_wave(duty_cycle);
    }
}
