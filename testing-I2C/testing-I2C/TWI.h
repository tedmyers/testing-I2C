//
//  TWI.h
//  testing-I2C
//
//  Created by Ted Myers on 4/4/16.
//  Copyright © 2016 Ted Myers. All rights reserved.
//

#ifndef TWI_h
#define TWI_h

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

// possibly: have these functions return a bool
// make them all check a status register, and program
// reports an error if the bool is false

// Generate TWI start signal (Master only)
void TWIStart(void);

// Generate TWI stop signalm (Master only)
void TWIStop(void);

// write a byte to TWI
void TWIWrite(uint8_t data_byte);

// Read a byte form TWI, with or without an ACK
uint8_t TWIRead(bool ack_signal);

uint8_t TWIGetStatus(void);

#endif /* TWI_h */
