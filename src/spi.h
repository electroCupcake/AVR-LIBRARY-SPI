/*
 * spi.h
 *
 *  Created on: Oct 20, 2015
 *      Author: jconvertino
 *

    Copyright (C) 2015 John Convertino

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef SPI_H_
#define SPI_H_

#include <inttypes.h>

//modes
#define SLAVE_MODE  0
#define MASTER_MODE 1
#define SPI_MODE_0	0 //sample rising, setup falling, clk normally low
#define SPI_MODE_1  1 //sample falling, setup rising, clk normally low
#define SPI_MODE_2	2 //sample falling, setup rising, clk normally high
#define SPI_MODE_3  3 //sample rising, setup falling, clk normally high
#define MSB_FIRST   0
#define LSB_FIRST   1

//shift value
#define SPI_MODE_LOC CPHA
#define SPI_MODE_MSK 0xD3

//SCK dividers
/* LVL_0 = F_CPU/4
 * LVL_1 = F_CPU/16
 * LVL_2 = F_CPU/64
 * LVL_4 = F_CPU/128
 *
 * The above is all at NORMAL_SPD, setting to DOUBLE_SPD will
 * cut each of the divisors in half (aka F_CPU/4 is now: F_CPU/2).
 */
#define FOSC_LVL_0  0
#define FOSC_LVL_1  1
#define FOSC_LVL_2  2
#define FOSC_LVL_3 	3
#define NORMAL_SPD	0
#define DOUBLE_SPD 	1

//will default to MSB first, ISR interrupt disabled, and SPI_MODE_0
void initSPI(uint8_t mode, uint8_t clkRate, uint8_t clkDouble);

void spiSettings(uint8_t mode, uint8_t endian);

uint8_t spiTrans(uint8_t data);

void spiSlaveLow(volatile uint8_t *pinPort, uint8_t pinNum);

void spiSlaveHigh(volatile uint8_t *pinPort, uint8_t pinNum);

void spiIsr_Enabled();

void spiIsr_Disabled();

#endif /* SPI_H_ */
