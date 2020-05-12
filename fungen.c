/* fungen.c
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * For use with the MSP432. */

#include "msp.h"
#include "spi.h"
#include "delay.h"
#include "waveforms.h"
#include "keypad.h"
#include "LCD.h"
#include "fungen.h"

#include <stdio.h>

/* fungen_init
 * initializes all components necessary for the function generator, including
 * populating the wave tables at the default voltage, setting the DCO to
 * 24 MHz, initializing all used peripherals, turning on interrupts for the
 * output, and printing out the default LCD screen. */
void fungen_init() {
    /* initialize wave tables */
    populate_square_table(DEFAULT_VOLTAGE, DEFAULT_DUTY_CYCLE);
    populate_saw_table(DEFAULT_VOLTAGE);
    populate_sine_table(DEFAULT_VOLTAGE);

    /* set the default wave to be the square wave (why not) */
    wave_table = square_table;

    /* set the DCO to run at 24 MHz */
    set_DCO(FREQ_24_MHZ);

    /* initialize our peripherals */
    SPI_init();
    keypad_init();
    LCD_init();

    /* start the output at 0 volts */
    send_to_DAC(0);

    /* initialize the timer to run at 100 Hz */
    setup_timer(FREQ_100_HZ);

    /* write our LCD beginning stuff */
    LCD_go_to_line(1);
    LCD_write_string("100 Hz   -- Freq");
    LCD_go_to_line(2);
    LCD_write_string("50% PWM  -- Wave");
    LCD_turn_off_cursor();
}

/* fungen_update_freq
 * sets the timer to be the correct frequency, and writes the frequency to the
 * first line of the LCD */
void fungen_update_freq(uint16_t freq) {
    LCD_go_to_line(1);

    switch(freq) {
        case FREQ_100_HZ :
            setup_timer(FREQ_100_HZ);
            LCD_write_string("100");
            break;
        case FREQ_200_HZ :
            setup_timer(FREQ_200_HZ);
            LCD_write_string("200");
            break;
        case FREQ_300_HZ :
            setup_timer(FREQ_300_HZ);
            LCD_write_string("300");
            break;
        case FREQ_400_HZ :
            setup_timer(FREQ_400_HZ);
            LCD_write_string("400");
            break;
        case FREQ_500_HZ :
            setup_timer(FREQ_500_HZ);
            LCD_write_string("500");
            break;
    }
}

/* fungen_print_wave
 * writes the current waveform to the second line of the LCD */
void fungen_print_wave(uint8_t duty_cycle) {
    /* character buffer to write lines */
    char buffer[20];

    /* go to the second line */
    LCD_go_to_line(2);

    /* write out current waveform */
    if (wave_table == square_table) {
        sprintf(buffer, "%2d%% PWM  ", duty_cycle);
        LCD_write_string(buffer);
    }
    else if (wave_table == sine_table) {
        LCD_write_string("Sine    ");
    }
    else { /* wave_table == saw_table */
        LCD_write_string("Sawtooth");
    }

    /* reset cursor position for main function */
    LCD_go_to_line(1);
}

/* increase_duty_cycle
 * increase the duty cycle by DUTY_CYCLE_INCREMENT,
 * maxing out at MAX_DUTY_CYCLE */
uint8_t increase_duty_cycle(uint8_t duty_cycle) {

    duty_cycle += DUTY_CYCLE_INCREMENT;

    if (duty_cycle >= MAX_DUTY_CYCLE) {
        duty_cycle = MAX_DUTY_CYCLE;
    }

    return duty_cycle;
}

/* decrease_duty_cycle
 * decrease the duty cycle by DUTY_CYCLE_INCREMENT,
 * minimum being at MIN_DUTY_CYCLE */
uint8_t decrease_duty_cycle(uint8_t duty_cycle) {

    duty_cycle -= DUTY_CYCLE_INCREMENT;

    if (duty_cycle <= MIN_DUTY_CYCLE) {
        duty_cycle = MIN_DUTY_CYCLE;
    }

    return duty_cycle;
}
