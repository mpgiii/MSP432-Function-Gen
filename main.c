#include "msp.h"
#include "spi.h"
#include "delay.h"
#include "waveforms.h"
#include "keypad.h"

/**
 * P2 main.c
 */

#define DEFAULT_DUTY_CYCLE 50
#define DEFAULT_VOLTAGE 3

#define DUTY_CYCLE_INCREMENT 10
#define MAX_DUTY_CYCLE 90
#define MIN_DUTY_CYCLE 10

void main(void)
{
    /* character to store keypad input */
    char c;

    /* initialize duty cycle to 50% for square wave */
    uint8_t duty_cycle = DEFAULT_DUTY_CYCLE;

    /* stop watchdog timer */
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;

    /* initialize wave tables */
    populate_square_table(DEFAULT_VOLTAGE, duty_cycle);
    populate_saw_table(DEFAULT_VOLTAGE);
    populate_sine_table(DEFAULT_VOLTAGE);

    /* set the default wave to be the square wave (why not) */
    wave_table = square_table;

    /* set the DCO to run at 12 MHz */
    set_DCO(FREQ_12_MHZ);

    /* initialize the SPI */
    SPI_init();

    /* initialize the keypad */
    keypad_init();

    /* start the output at 0 volts */
    send_to_DAC(0);

    /* initialize the timer to run at 100 Hz */
    setup_timer(FREQ_100_HZ);

    while(1) {
        /* get input from keypad */
        c = keypad_get_key();

        /* depending on which key is pressed, do something */
        switch(c) {
            /* when 1-5 are pressed, set the frequency of the output to
             * 100-500 Hz, respectively */
            case '1' :
                setup_timer(FREQ_100_HZ);
                break;
            case '2' :
                setup_timer(FREQ_200_HZ);
                break;
            case '3' :
                setup_timer(FREQ_300_HZ);
                break;
            case '4' :
                setup_timer(FREQ_400_HZ);
                break;
            case '5' :
                setup_timer(FREQ_500_HZ);
                break;

            /* when 7-9 are pressed, set the output waveform to
             * square, sine, and sawtooth, respectively */
            case '7' :
                wave_table = square_table;
                break;
            case '8' :
                wave_table = sine_table;
                break;
            case '9' :
                wave_table = saw_table;
                break;

            /* the '*' key decreases the duty cycle by 10%, down to a minimum
             * of 10% (only for square wave, obviously) */
            case '*' :
                duty_cycle -= DUTY_CYCLE_INCREMENT;
                if (duty_cycle <= MIN_DUTY_CYCLE) {
                    duty_cycle = MIN_DUTY_CYCLE;
                }
                populate_square_table(DEFAULT_VOLTAGE, duty_cycle);
                delay_us(100000); /* delay to avoid button bounce */
                break;

            /* the '0' key increases the duty cycle by 10%, up to a maximum
             * of 90% (only for square wave, obviously) */
            case '0' :
                duty_cycle += DUTY_CYCLE_INCREMENT;
                if (duty_cycle >= MAX_DUTY_CYCLE) {
                    duty_cycle = MAX_DUTY_CYCLE;
                }
                populate_square_table(DEFAULT_VOLTAGE, duty_cycle);
                delay_us(100000); /* delay to avoid button bounce */
                break;

            /* the '#' key sets the duty cycle to 50% */
            case '#' :
                duty_cycle = DEFAULT_DUTY_CYCLE;
                populate_square_table(DEFAULT_VOLTAGE, duty_cycle);
                break;
        }
    }
}
