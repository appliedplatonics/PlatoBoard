/* -*- mode: C; c-basic-offset: 2; indent-tabs-mode: nil -*- */
/*
  wiring_analog.c - analog input and output

  Copyright (c) 2005-2006 David A. Mellis

  Portions copyright (c) 2011 Applied Platonics.

  This file is a part of the PlatoBoard, 
  http://www.appliedplatonics.com/platoboard/


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

  Id: wiring.c 248 2007-02-03 15:36:30Z mellis

  Modified 28-08-2009 for attiny84 R.Wiersma
  Modified 14-108-2009 for attiny45 Saposoft

  2011-05-11 jbm Ported to ATtiny2313 by Applied Platonics
*/

#include "wiring_private.h"
#include "pins_platoboard2313.h"

uint8_t analog_reference = DEFAULT;

void analogReference(uint8_t mode)
{
  // Doesn't actually do anything... remove from code, force
  // compile-time error?
}

int analogRead(uint8_t pin)
{
  return 0;
}

// Right now, PWM output only works on the pins with
// hardware support.  These are defined in the appropriate
// pins_*.c file.  For the rest of the pins, we default
// to digital output.
void analogWrite(uint8_t pin, int val)
{
  // We need to make sure the PWM output is enabled for those pins
  // that support it, as we turn it off when digitally reading or
  // writing with them.  Also, make sure the pin is in output mode
  // for consistenty with Wiring, which doesn't require a pinMode
  // call for the analog output pins.
  pinMode(pin, OUTPUT);

  switch(digitalPinToTimer(pin)) {
  case TIMER0A: 
    OCR0A = val;
    sbi(TCCR0A, COM0A1);
    break;
  case TIMER0B:
    OCR0B = val;
    sbi(TCCR0A, COM0B1);
    break;

  case TIMER1A:
    OCR1A = val;
    sbi(TCCR1A, COM0A1);
    break;
  case TIMER1B:
    OCR1B = val;
    sbi(TCCR1A, COM0B1);
    break;
  default:
    digitalWrite(pin, val >= 128);
  }
}
