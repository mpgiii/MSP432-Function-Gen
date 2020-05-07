/*
 * spi.h
 *
 *  Created on: Apr 29, 2020
 *      Author: mpgii
 */

#ifndef SPI_H_
#define SPI_H_

/* constant definitions */
#define CS_PORT P4
#define CS_BIT  BIT1

#define SPI_PORT P1
#define CLK_BIT BIT5
#define MOSI_BIT BIT6

/* DAC config bit definitions */
#define EN_BIT   BIT7  /* if enabled, ignore this command. */
#define BUF_BIT  BIT6  /* if enabled, buffered input */
#define GAIN_BIT BIT5  /* if enabled, Vo = Vref * D/4096
                        * if disabled, 2(Vo = 2Vref * D/4096) */
#define SHDN_BIT BIT4  /* if enabled, active mode operation. if disabled, no
                        * output is available. */

/* function prototypes */
void SPI_init(void);
void send_to_DAC(uint16_t voltage);

#endif /* SPI_H_ */
