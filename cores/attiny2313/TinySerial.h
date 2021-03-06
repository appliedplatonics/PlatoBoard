/* -*- mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*- */                                          /*
   TinySerial.cpp - Hardware serial library for Wiring on the ATTiny2313
   Copyright (c) 2011 Applied Platonics.

   This file is a part of the PlatoBoard, 
   http://www.appliedplatonics.com/platoboard/

   Distributed under the terms of the GPL.

   2011-04-29 jbm First revision
                                                                                                           */

#ifndef TinySerial_h
#define TinySerial_h

#include <inttypes.h>
#include "Stream.h"

#include <avr/io.h>

#ifndef _AVR_IOTN2313_H_
#error "TinySerial.h: Not compiling for the ATTiny2313, or screwed-up includes."
#endif

class TinySerial : public Stream
{
public:
  TinySerial();
  void beginFromTable(uint16_t);
  void begin(long);
  void end();
  virtual int available(void);
  virtual int peek(void);
  virtual int read(void);
  virtual void flush(void);
  virtual void write(uint8_t);
  using Print::write; // pull in write(str) and write(buf, size) from Print
  using Print::writePgm; // pull in write(str) and write(buf, size) from Print
};

extern TinySerial Serial;

#endif // ndef TinySerial_h
