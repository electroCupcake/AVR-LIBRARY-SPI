/*
 * spi.c
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

#include <avr/io.h>
#include <avr/common.h>
#include <avr/interrupt.h>

#include "spi.h"

void initSpi(uint8_t mode, uint8_t clkRate, uint8_t clkDouble)
{
	uint8_t tmpSREG = 0;

	tmpSREG = SREG;
	cli();

	SPCR = 0;

	switch(mode)
	{
		case MASTER_MODE:
			SPCR = clkRate;
			SPSR = clkDouble;
			DDRB |= (1 << DDB5) | (1 << DDB3) | (1 << DDB2); //output SCK, MOSI, SS
			DDRB &= ~(1 << DDB4); //input MISO
			SPCR |= (1 << MSTR) | (1 << SPE);
			break;
		case SLAVE_MODE:
		default:
			DDRB |= (1 << DDB4); //output on MISO
			DDRB &= ~(1 << DDB5) & ~(1 << DDB3) & ~(1 << DDB2); //input MOSI, SCK, SS
			SPCR |= (1 << SPE);
			break;
	}

	SREG = tmpSREG;
}

void spiSettings(uint8_t mode, uint8_t endian)
{
	SPCR |= (mode << SPI_MODE_LOC) | (endian << DORD);
	SPCR &= SPI_MODE_MSK | (mode << SPI_MODE_LOC) | (endian << DORD);
}

uint8_t spiTrans(uint8_t data)
{
	uint8_t tmp_SREG = SREG;
	cli();

	SPDR = data;

	//better idea needed to deal with collisions
	if(SPSR & (1 << WCOL))
	{
		SREG = tmp_SREG;
		return data;
	}

	while((SPSR & (1 << SPIF)) == 0);

	SREG = tmp_SREG;
	return SPDR;
}

void spiSlaveLow(volatile uint8_t *pinPort, uint8_t pinNum)
{
	*pinPort &= ~(1 << pinNum);
}

void spiSlaveHigh(volatile uint8_t *pinPort, uint8_t pinNum)
{
	*pinPort |= (1 << pinNum);
}

void spiIsr_Enabled()
{
	SPCR |= (1 << SPIE);
}

void spiIsr_Disabled()
{
	SPCR &= ~(1 << SPIE);
}
