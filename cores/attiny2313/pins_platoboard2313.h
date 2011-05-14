/*
  pins_platoboard2313.h - pin definitions for Wiring on the PlatoBoard

  This file is a part of the PlatoBoard, 
  http://www.appliedplatonics.com/platoboard/

  Based on a file from Arduino

  Copyright (c) 2007 David A. Mellis

  Portions copyright (c) 2011 Applied Platonics, insofar as tables of
  constants are copyrightable.

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

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $

  modified 2011-04-28 for attiny2313, Applied Platonics
  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 14-108-2009 for attiny45 Saposoft

  2011-05-10 jbm Small tweaks to TIMER defs for PlatoBoard/ATTiny2313
*/

#ifndef pins_platoboard2313_h
#define pins_platoboard2313_h

#include <avr/pgmspace.h>

#define NOT_A_PIN 0
#define NOT_A_PORT 0

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4

//changed it to uint16_t to uint8_t
extern const uint8_t PROGMEM port_to_mode_PGM[];
extern const uint8_t PROGMEM port_to_input_PGM[];
extern const uint8_t PROGMEM port_to_output_PGM[];

extern const uint8_t PROGMEM digital_pin_to_port_PGM[];
// extern const uint8_t PROGMEM digital_pin_to_bit_PGM[];
extern const uint8_t PROGMEM digital_pin_to_bit_mask_PGM[];
extern const uint8_t PROGMEM digital_pin_to_timer_PGM[];

// Get the bit location within the hardware port of the given virtual pin.
// This comes from the pins_*.c file for the active board configuration.
// 
// These perform slightly better as macros compared to inline functions
//
#define digitalPinToPort(P) ( pgm_read_byte( digital_pin_to_port_PGM + (P) ) )
#define digitalPinToBitMask(P) ( pgm_read_byte( digital_pin_to_bit_mask_PGM + (P) ) )
#define digitalPinToTimer(P) ( pgm_read_byte( digital_pin_to_timer_PGM + (P) ) )
#define analogInPinToBit(P) (P)
// in the following lines modified pgm_read_word in pgm_read_byte, word doesn't work on attiny45
#define portOutputRegister(P) ( (volatile uint8_t *)( pgm_read_byte( port_to_output_PGM + (P))) )
#define portInputRegister(P) ( (volatile uint8_t *)( pgm_read_byte( port_to_input_PGM + (P))) )
#define portModeRegister(P) ( (volatile uint8_t *)( pgm_read_byte( port_to_mode_PGM + (P))) )

#endif // ndef pins_platoboard2313_h
