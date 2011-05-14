/*
  Tone.cpp - Implements the Tone.cpp interface

  Original interface by Brett Hagman; this is a total rewrite to make
  it fit on the ATTiny2313.

  Copyright (c) 2011 Applied Platonics.

  This file is a part of the PlatoBoard, 
  http://www.appliedplatonics.com/platoboard/

  Distributed under the terms of the GPL.
*/

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include "wiring.h"
#include "pins_platoboard2313.h"

#ifndef sbi
#define sbi(p,n) (p) |= 1 << (n)
#endif

#ifndef cbi
#define cbi(p,n) (p) &= ~(1 << (n))
#endif

#define USE_NEW_TONE 1

// XXX PLATOBOARD Only supports one tone at a time, on Timer0; while
// in use, one cannot do PWM on pins 5 and 9.

// timerx_toggle_count:
//  > 0 - duration specified
//  = 0 - stopped
//  < 0 - infinitely (until stop() method called, or new play() called)

volatile long timer0_toggle_count;
volatile uint8_t *timer0_pin_port;
volatile uint8_t timer0_pin_mask;

#if USE_NEW_TONE
uint16_t real_ocr;
uint16_t ocr_left;
#endif

#define TONE_PIN_UNUSED 255

volatile uint8_t pinUsed = TONE_PIN_UNUSED;

// Set up the pin for tone, marking resources as used.
static int8_t claimPin(uint8_t _pin) 
{
  uint8_t pu_copy = pinUsed;

  if (pu_copy != TONE_PIN_UNUSED && pu_copy != _pin)
    return -1; // Already in use

  pinUsed = _pin;

  return 0; // returns timer number, which is always 0 on 2313
}


#if USE_NEW_TONE

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
  claimPin(_pin);

  if (pinUsed != _pin) 
    return; // We hit a race condition, and lost.

  timer0_pin_port = portOutputRegister(digitalPinToPort(_pin));
  timer0_pin_mask = digitalPinToBitMask(_pin);

  // Set the pinMode as OUTPUT // XXX TODO Make idempotent
  pinMode(_pin, OUTPUT);

  // Running on timer0, so must use fixed prescaler of 64x, lest we
  // mess up delay(), millis(), etc.
  real_ocr = F_CPU/frequency/64/2-1;

  if (real_ocr <= 255) {
    OCR0A = real_ocr; // Good for 244Hz to 62kHz
    ocr_left = 0;
  } else {
    OCR0A = 255;
    ocr_left = real_ocr;
  }
  
  // Calculate the toggle count
  if (duration > 0) {
    timer0_toggle_count = 2 * frequency * duration / 1000;
  } else {
    timer0_toggle_count = -1;
  }

  // Enable the interrupt
  sbi(TIMSK, OCIE0A);
}
#else

// frequency (in hertz) and duration (in milliseconds).

void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
  uint8_t prescalarbits = 0b001;
  long toggle_count = 0;
  uint32_t ocr = 0;

  claimPin(_pin);

  if (pinUsed != _pin) 
    return; // We hit a race condition, and lost.


  timer0_pin_port = portOutputRegister(digitalPinToPort(_pin));
  timer0_pin_mask = digitalPinToBitMask(_pin);

  // Set the pinMode as OUTPUT // XXX TODO Make idempotent
  pinMode(_pin, OUTPUT);


  // Compute OCR: OCR = F_CPU / freq / prescaler / 2 - 1
  ocr = F_CPU / frequency / 2;

  // We need to get ocr into an 8b quantity; to do this, we walk up
  // the prescaler settings.  The first step is CLK/8, with the
  // prescaler bits set to 0b001.  The next step is CLK/64, with the
  // bits set to 0b010.  The next step is CLK/256, then CLK/1024.
  //
  // Division by 8 is equivalent to a rightward shift of 3; division
  // by 4 is equivalent to a right-shift of 2.
  //
  // Divisions stack, which we take advantage of below:
  //    8, 64, 256, 1024 = 8, 8*8, 8*8*4, 8*8*4*4
  //

  uint8_t shift_steps[] = { 0, 3, 3, 2, 2 };
  for (prescalarbits = 1; ocr > (255+1) && prescalarbits < 0b101; prescalarbits++) {
    ocr = ocr >> shift_steps[prescalarbits];    
  }
  ocr -= 1;

  // XXX TODO Allow for ocr > 255 by adding the appropriate
  // epicyclical variables.

  // Set the OCR for the given timer,
  OCR0A = ocr;

  TCCR0B = prescalarbits;

  // Calculate the toggle count
  if (duration > 0) {
    timer0_toggle_count = 2 * frequency * duration / 1000;
  } else {
    timer0_toggle_count = -1;
  }

  // Enable the interrupt
  sbi(TIMSK, OCIE0A);
}
#endif

// Turn off tone on pin _pin
void noTone(uint8_t _pin)
{
  cbi(TIMSK, OCIE0A);
  pinUsed = TONE_PIN_UNUSED;

  digitalWrite(_pin, 0);
}

// Actual Interrupt handler
ISR(TIMER0_COMPA_vect)
{
  if (ocr_left < 15) {
    if (real_ocr > 255) {
      OCR0A = 255;
      ocr_left = real_ocr;
    }

    if (timer0_toggle_count != 0)  {
      // toggle the pin
      *timer0_pin_port ^= timer0_pin_mask;
    
      if (timer0_toggle_count > 0)
	timer0_toggle_count--;

    } else {
      cbi(TIMSK, OCIE0A);
    }
  } else {
    if (real_ocr > 255) {
      ocr_left -= 255;
      if (ocr_left < 255) {
	OCR0A = ocr_left;
	ocr_left = 0;
      } 
      
    }      
  }
}
