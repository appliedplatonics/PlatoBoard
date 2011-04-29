/*
  TinySerial.cpp - Hardware serial library for Wiring on the ATTiny2313
  Copyright (c) 2011 Applied Platonics.

  Distributed under the terms of the GPL.

  2011-04-29 jbm First revision
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "wiring.h"
#include "wiring_private.h"

#include <avr/pgmspace.h>
#include "TinySerial.h"

#define RX_BUFFER_SIZE 16

struct ring_buffer
{
  unsigned char buffer[RX_BUFFER_SIZE];
  int head;
  int tail;
};

static ring_buffer _rx_buffer  =  { { 0 }, 0, 0 };
static ring_buffer *rx_buffer = &_rx_buffer;

inline void store_char(unsigned char c)
{
  int i = (unsigned int)(rx_buffer->head + 1) % RX_BUFFER_SIZE;

  // if we should be storing the received character into the location
  // just before the tail (meaning that the head would advance to the
  // current location of the tail), we're about to overflow the buffer
  // and so we don't write the character or advance the head.
  if (i != rx_buffer->tail) {
    rx_buffer->buffer[rx_buffer->head] = c;
    rx_buffer->head = i;
  }
}


ISR(USART_RX_vect)
{
  unsigned char c = RXB;
  store_char(c);
}

// Constructors ////////////////////////////////////////////////////////////////

TinySerial::TinySerial()
{
}


// Public Methods //////////////////////////////////////////////////////////////

void TinySerial::beginFromTable(uint16_t ubrr)
{
  sbi(UCSRA, U2X);
  UBRRH = 0xff & (ubrr >> 8);
  UBRRL = 0xff & ubrr;

  sbi(UCSRB, RXEN);
  sbi(UCSRB, TXEN);
  sbi(UCSRB, RXCIE);
  cbi(UCSRB, TXCIE);

  /* 8 data bits, 1 stop bit */
  UCSRC = (1 << UCSZ1) | (1 << UCSZ0);
}

void TinySerial::begin(long baud)
{
  return beginFromTable( F_CPU/8/baud - 1 );
}

void TinySerial::end()
{
  cbi(UCSRB, RXEN);
  cbi(UCSRB, TXEN);
  cbi(UCSRB, RXCIE);
}

int TinySerial::available(void)
{
  return (unsigned int)(RX_BUFFER_SIZE + rx_buffer->head - rx_buffer->tail) % RX_BUFFER_SIZE;
}

int TinySerial::peek(void)
{
  if (rx_buffer->head == rx_buffer->tail) {
    return -1;
  } else {
    return rx_buffer->buffer[rx_buffer->tail];
  }
}

int TinySerial::read(void)
{
  // if the head isn't ahead of the tail, we don't have any characters
  if (rx_buffer->head == rx_buffer->tail) {
    return -1;
  } else {
    unsigned char c = rx_buffer->buffer[rx_buffer->tail];
    rx_buffer->tail = (unsigned int)(rx_buffer->tail + 1) % RX_BUFFER_SIZE;
    return c;
  }
}

void TinySerial::flush()
{
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // don't reverse this or there may be problems if the RX interrupt
  // occurs after reading the value of rx_buffer_head but before writing
  // the value to rx_buffer_tail; the previous value of rx_buffer_head
  // may be written to rx_buffer_tail, making it appear as if the buffer
  // were full, not empty.
  rx_buffer->head = rx_buffer->tail;
}

void TinySerial::write(uint8_t c)
{
  while (!(UCSRA & (1<<UDRE)))
    ;

  TXB = c;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TinySerial Serial = TinySerial();


