#include "msp.h"
#include "spi.h"

/*
 * spi.c
 *
 *  Created on: Apr 29, 2020
 *      Author: mpgii
 */


void SPI_init(void) {

    /* first reset the EUSCI */
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_SWRST;

    /* next, set bits to do the following:
     * polarity as active low
     * enable synchronous mode
     * use the SMCLK as the clock source
     * set the MSB first selector
     * set as the master */
    EUSCI_B0 -> CTLW0 |= EUSCI_B_CTLW0_SWRST
                     |  EUSCI_B_CTLW0_CKPL
                     |  EUSCI_B_CTLW0_SYNC
                     |  EUSCI_B_CTLW0_UCSSEL_2
                     |  EUSCI_B_CTLW0_MSB
                     |  EUSCI_B_CTLW0_MST;

    /* run the SMCLK at full speed (do not bitscale it) */
    /* TODO: this may not be needed */
    EUSCI_B0 -> BRW = 0x01;

    /* set the CS bit as an output */
    CS_PORT -> SEL0 &= ~CS_BIT;
    CS_PORT -> SEL1 &= ~CS_BIT;
    CS_PORT -> DIR |= CS_BIT;
    CS_PORT -> OUT |= CS_BIT;

    /* set the appropriate SPI port bits to run in primary mode */
    SPI_PORT -> SEL0 |= (CLK_BIT | MOSI_BIT);
    SPI_PORT -> SEL1 &= ~(CLK_BIT | MOSI_BIT);

    /* and set these as outputs */
    SPI_PORT -> DIR |= (CLK_BIT | MOSI_BIT);
    SPI_PORT -> OUT |= (CLK_BIT | MOSI_BIT);

    /* clear reset */
    EUSCI_B0 -> CTLW0 &= ~(EUSCI_B_CTLW0_SWRST);

    return;
}

void send_to_DAC(uint16_t output) {
    uint8_t low;
    uint8_t high;

    /* get the lower 8 bits of the input and store it in low */
    low = output & 0x0F;

    /* get the higher 8 bits and store it in high */
    high = (output >> 8) & 0x0F;

    /* to send a command to the DAC, the highest 4 bits should be:
     * 0, BUF, ~GA, and ~SHDN. See header file for explanation for each bit */
    high &= ~(EN_BIT | BUF_BIT);
    high |= GAIN_BIT | SHDN_BIT;

    /* reset the CS bit low */
    CS_PORT -> OUT &= ~(CS_BIT);

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* transfer the high 8 bits to the DAC */
    EUSCI_B0 -> TXBUF = high;

    /* wait until transmit buffer is empty */
    while (!(EUSCI_B0 -> IFG & EUSCI_B_IFG_TXIFG));

    /* send the low 8 bits */
    EUSCI_B0 -> TXBUF = low;

    /* wait for transmit to complete */
    while(!(EUSCI_B0 -> IFG & EUSCI_B_IFG_RXIFG));

    /* set the CS bit high */
    CS_PORT -> OUT |= CS_BIT;

    return;
}
