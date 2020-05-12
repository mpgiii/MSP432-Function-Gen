/* waveforms.c
 * Written by Connor McKee and Michael Georgariou
 * CPE 316 - Spring 2020
 *
 * - Has function to begin reading wave table being pointed to by wave_table
 *   and outputting it to a DAC in an interrupt handler
 * - Maintains a few wave tables to be used by a MSP432 function generator.
 * - Resolution can be increased by adjusting WAVE_TABLE_SIZE in waveforms.h.
 * - Tables can be added easily -- just populate another table of size
 *   WAVE_TABLE_SIZE. Whichever table currently being pointed to by the
 *   wave_table pointer will be the one being outputted to a DAC. */

#include "msp.h"
#include "spi.h"
#include "waveforms.h"

#include <math.h>

/* pointer to current wave table being outputted to the DAC. Set this to the
 * wave you want to be outputting (header file declares this as extern, so it
 * can be updated wherever waveforms.h is included) */
uint16_t* wave_table;

/* wave tables populated later of size WAVE_TABLE_SIZE */
uint16_t square_table[WAVE_TABLE_SIZE] = {0};
uint16_t sine_table[WAVE_TABLE_SIZE] = {0};
uint16_t saw_table[WAVE_TABLE_SIZE] = {0};


/* setup_timer
 * Sets up an interrupt to go off WAVE_TABLE_SIZE times per period.
 * Frequency determined by value passed in */
void setup_timer(uint16_t freq) {
    /* enable interrupts for CCR0 */
    TIMER_A0 -> CTL = (TIMER_A_CTL_TASSEL_2 | TIMER_A_CTL_MC_1);

    /* run the timer until our period is up */
    TIMER_A0 -> CCTL[0] = TIMER_A_CCTLN_CCIE;

    /* set timer to go off every 1/64th of our period */
    switch(freq) {
        case FREQ_100_HZ :
            TIMER_A0 -> CCR[0] = TIMER_PERIOD_100_HZ/WAVE_TABLE_SIZE;
            break;
        case FREQ_200_HZ :
            TIMER_A0 -> CCR[0] = TIMER_PERIOD_200_HZ/WAVE_TABLE_SIZE;
            break;
        case FREQ_300_HZ :
            TIMER_A0 -> CCR[0] = TIMER_PERIOD_300_HZ/WAVE_TABLE_SIZE;
            break;
        case FREQ_400_HZ :
            TIMER_A0 -> CCR[0] = TIMER_PERIOD_400_HZ/WAVE_TABLE_SIZE;
            break;
        case FREQ_500_HZ :
            TIMER_A0 -> CCR[0] = TIMER_PERIOD_500_HZ/WAVE_TABLE_SIZE;
            break;
        default :
            TIMER_A0 -> CCR[0] = TIMER_PERIOD_100_HZ/WAVE_TABLE_SIZE;
            break;
    }


    /* ISR for CCR0 */
    NVIC -> ISER[0] = 1 << (TA0_0_IRQn & 31);

    /* enable interrupts */
    __enable_irq();
}


/* TA0_0_IRQHandler
 * interrupt handler which sends next table value to DAC */
void TA0_0_IRQHandler() {
    static int index = 0;

    TIMER_A0 -> CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;

    /* output the current lookup table voltage to the DAC */
    send_to_DAC(wave_table[index++]);

    if (index >= WAVE_TABLE_SIZE) {
        index = 0;
    }
}


/* populate_square_table
 * populates global square wave table.
 * ground will always be at 0, and the "high" value will be the voltage
 * passed in.
 * note: duty_cycle is a integer from 0-100, representing percentage
 * for duty_cycle */
void populate_square_table(uint8_t voltage, uint8_t duty_cycle) {
    int i;
    int offset = (duty_cycle * WAVE_TABLE_SIZE) / 100;

    for (i = 0; i < offset; i++) {
        square_table[i] = voltage * DAC_MULTIPLIER;
    }
    for (; i < WAVE_TABLE_SIZE; i++) {
        square_table[i] = 0;
    }
}

/* populate_saw_table
 * populates global sawtooth wave table.
 * ground will always be at 0, and the "high" value will be the voltage
 * passed in. */
void populate_saw_table(uint8_t voltage) {
    int i;
    int res = 0;

    for (i = 0; i < WAVE_TABLE_SIZE; i++) {
        saw_table[i] = res;
        res += (voltage * DAC_MULTIPLIER) / WAVE_TABLE_SIZE;
    }
}

/* populate_sine_table
 * populates global sine wave table.
 * ground will always be at 0, and the "high" value will be the voltage
 * passed in. */
void populate_sine_table(uint8_t voltage) {
    int i;
    int res = 0;
    int amplitude = (voltage * DAC_MULTIPLIER) / 2;

    for (i = 0; i < WAVE_TABLE_SIZE; i++) {
        res = (amplitude * sin((i * 2*PI) / WAVE_TABLE_SIZE)) + amplitude;
        sine_table[i] = res;
    }
}
