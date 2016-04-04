//
//  TWI.c
//  testing-I2C
//
//  Created by Ted Myers on 4/4/16.
//  Copyright © 2016 Ted Myers. All rights reserved.
//

#include "TWI.h"

// possibly: have these functions return a bool
// make them all check a status register, and program
// reports an error if the bool is false

// Generate TWI start signal (Master only)
void TWIStart(void)
{
    // TWI Control Register (p.231)
    TWCR |= (1<<TWINT) |   // Start the TWI/Clear the TWINT flag
            (1<<TWSTA) |   // Checks if bus is open, writes START when available
            (1<<TWEN);      // enables TWI operation and activates the TWI interface
    
    // Wait until TWI has finished its current job
    // Replace this with interrupt based routine next
    while ((TWCR & (1<<TWINT)) == 0);
}

// Generate TWI stop signalm (Master only)
void TWIStop(void)
{
    // TWI Control Register (p.231)
    TWCR |= (1<<TWINT) |   // Start the TWI/Clear the TWINT flag
            (1<<TWSTO) |   // generate a STOP condition on the TWI
            (1<<TWEN);      // enables TWI operation and activates the TWI interface
}

// write a byte to TWI
void TWIWrite(uint8_t data_byte)
{
    // TWI Data Register (p.232)
    TWDR = data_byte;
    TWCR =  (1<<TWINT) |   // Start the TWI/Clear the TWINT flag
            (1<<TWEN);     // enables TWI operation and activates the TWI interface
    
    // wait until TWI has finished its current job
    while ((TWCR & (1<<TWINT)) == 0);
}

// Read a byte form TWI, with or without an ACK
uint8_t TWIRead(bool ack_signal)
{
    TWCR =  (1<<TWINT) |            // Start the TWI/Clear the TWINT flag
            (1<<TWEN)  |            // enables TWI operation and activates the TWI
            (ack_signal<<TWEA);      // Generate the acknowledge pulse (or not)
    
    // wait until TWI has finished its current job
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

uint8_t TWIGetStatus(void)
{
    uint8_t status;
    // mask the status
    status = TWSR & 0xF8;
    return status;
}