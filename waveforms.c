#include "msp.h"
#include "spi.h"
#include "waveforms.h"

/*
 * waveforms.c
 *
 *  Created on: May 3, 2020
 *      Author: Michael Georgariou and Connor McKee
 */

#include <math.h>

int index = 0;
int* wave_table;

int square_table[WAVE_TABLE_SIZE] = {0};
int sine_table[WAVE_TABLE_SIZE] = {0};
int saw_table[WAVE_TABLE_SIZE] = {0};

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


/* interrupt handler which sends next table value to DAC */
void TA0_0_IRQHandler() {
    TIMER_A0 -> CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;

    /* output the current lookup table voltage to the DAC */
    send_to_DAC(wave_table[index++]);

    if (index >= WAVE_TABLE_SIZE) {
        index = 0;
    }
}


/* populates global square wave table..
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

/* populates global sawtooth wave table.
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

/* populates global sine wave table.
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
