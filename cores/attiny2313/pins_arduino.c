/*
  pins_arduino.c - pin definitions for the Arduino board
  Part of Arduino / Wiring Lite

  Copyright (c) 2005 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: pins_arduino.c 565 2009-03-25 10:50:00Z dmellis $

  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 09-10-2009 for attiny45 A.Saporetti
*/

#include <avr/io.h>
#include "wiring_private.h"
#include "pins_arduino.h"

/* XXX TODO PLATOBOARD */


// On the Arduino board, digital pins are also used
// for the analog output (software PWM).  Analog input
// pins are a separate set.

// On the PlatoBoard's 2313 socket, there are only digital pins, which
// are used for both digital and analog output.  The 2313 has no
// analog pins, sadly.

// ATMEL ATTINY 2313 / PLATOBOARD
//
//                  +-\/-+
//           /RST  1|    |20  VCC
//      (D 0) PD0  2|    |19  PB7 (D 14)
//      (D 1) PD1  3|    |18  PB6 (D 13)
//           XTAL  4|    |17  PB5 (D 12)
//           XTAL  5|    |16  PB4 (D 11) (PWM+)
//      (D 2) PD2  6|    |15  PB3 (D 10) (PWM+)
//      (D 3) PD3  7|    |14  PB2 (D  9) (PWM+)
//      (D 4) PD4  8|    |13  PB1 (D  8)
//(PWM+)(D 5) PD5  9|    |12  PB0 (D  7)
//            GND 10|    |11  PD6 (D  6)
//                  +----+
//
// (PWM+ indicates the additional PWM pins on the ATmega168.)


#define PA 0 // Unused, but is the port underlying the XTAL
#define PB 1
#define PD 2

// these arrays map port names (e.g. port B) to the
// appropriate addresses for various functions (e.g. reading
// and writing)
const uint8_t PROGMEM port_to_mode_PGM[] = {
	NOT_A_PORT,
	&DDRB,
	&DDRD,
};

const uint8_t PROGMEM port_to_output_PGM[] = {
	NOT_A_PORT,
	&PORTB,
	&PORTD,
};

const uint8_t PROGMEM port_to_input_PGM[] = {
	NOT_A_PIN,
	&PINB,
	&PIND
};

const uint8_t PROGMEM digital_pin_to_port_PGM[] = {

  // 7 PD entries
  PD, PD, PD, PD, 
  PD, PD, PD,

  // 8 PBs
  PB, PB, PB, PB,
  PB, PB, PB, PB,

  // zero Js.
};

const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[] = {
  _BV(0), _BV(1), _BV(2), _BV(3),
  _BV(4), _BV(5), _BV(6),

  _BV(0), _BV(1), _BV(2), _BV(3),
  _BV(4), _BV(5), _BV(6), _BV(7),
};

const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
  NOT_ON_TIMER, // 0
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  TIMER0B, // 5: OC0B
  NOT_ON_TIMER,

  NOT_ON_TIMER, // 7
  NOT_ON_TIMER,
  TIMER0A, // 9: OC0A
  TIMER1A, // 10: OC1A
  TIMER1B, // 11: OC1B
  NOT_ON_TIMER,
  NOT_ON_TIMER,
  NOT_ON_TIMER, // 14
};
