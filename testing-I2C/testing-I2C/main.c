#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#include "TWI.h"

#define RED_PIN     PD3
#define GREEN_PIN   PD6
#define BLUE_PIN    PD5
#define ROT_A       PC0
#define ROT_B       PC1
#define BUTTON      PC3
#define LED_DDR     DDRD
#define LED_PORT    PORTD

#define START       0x08
#define MT_SLA_ACK  0x18
#define MT_DATA_ACK 0x28

#define ERROR   1
#define SUCCESS 0

/******************************
 
 testing-I2C: A program to learn about and 
 use the I2C communication protocol
 
 Useful information and tutorial at the following link:
 http://www.embedds.com/programming-avr-i2c-interface/
 (this program based off of examples provided there)
 
 
 I2C allows control of up to 128 devices
 
 Uses two pins for communication:
 -SCK (System Clock)
 -SDA (System Data)
 
 These pins are pulled high, and are set low to communicate.
 This allows easier communication by devices at slightly different
 logic levels (e.g. Atmega328 @ 5V vs. Raspberry Pi @ 3.3V)
 
 Parts of I2C communication:
 START  signal
 Address Packet     |   Seven bits, lead by data direction bit + acknowledge bit
 Data Packet        |   Eight data bits + acknowledge bit
 STOP signal
 
 START and STOP signals are always generated by the master.
 These are generated by pulling SDA low while SCL is high
 
 Acknowledge bit:
 Receiver always needs to confirm data received by pulling SDA
 low (ACK) or leave high (NACK) to indicate no data received.
 
 I2C can send multiple bytes in a single packet.
 This will be useful to send color data later on to the light
 strip controller. We will need to send three bytes of data,
 eight bits for each color brightness level.
 
 Note: AVR uses the term TWI (two-wire interface) to refer
 to I2C due to copyright issues. It is the same otherwise
 
 
 ******************************/
 
void initIO(void)
{
    // initialize LED
    LED_DDR |= (1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN);
    
    // initialize TWI pins as outputs/inputs? (may be done in HW later)
}

#define MASTER_SENDER_MODE  1
#define SLAVE_RECEIVER_MODE 2

uint8_t initTWI(uint8_t mode)
{
    switch (mode)
    {
        case MASTER_SENDER_MODE:
            // TWI Status Register (p.231)
            TWSR &= ~(1<<TWPS0) | ~(1<<TWPS1); // PrescalarValue = 1
            
            // TWI Bit Rate Register (p.230)
            TWBR = 0x01;
            
            // TWI Control Register (p.231)
            TWCR |= (1<<TWEN); // enable TWI
            
            return SUCCESS;
            
            // Question: is the F_CPU here already prescaled or not?
            // Could change the answer: 8MHz or 1MHz
            
            /*  p.213
             -----
             F_SCL   = F_CPU /
             (16 + 2*TWBR*PrescalarValue)
             = 8000000 /
             (16 + 2*1*1
             = 444.444 kHz?
             */
            
        case SLAVE_RECEIVER_MODE:
            // TWI Status Register (p.231)
            TWSR &= ~(1<<TWPS0) | ~(1<<TWPS1); // PrescalarValue = 1
            
            // TWI Bit Rate Register (p.230)
            TWBR = 0x01;
            
            // TWI Control Register (p.231)
            TWCR |= (1<<TWEN) | (1<<TWEA); // enable TWI
            
            // Address register
            TWAR |= (1<<TWGCE); // enable recognition of general call
            
            return SUCCESS;
            
        default:
            return ERROR;
            
            
    }
    
}



uint8_t sendColorsTWI(uint8_t red, uint8_t green, uint8_t blue)
{
    TWIStart();
    
    if (TWIGetStatus() != START )           return ERROR;
    
    TWIWrite(red);
    if (TWIGetStatus() != MT_SLA_ACK )      return ERROR;
    
    TWIWrite(green);
    if (TWIGetStatus() != MT_DATA_ACK )     return ERROR;
    
    TWIWrite(blue);
    if (TWIGetStatus() != MT_DATA_ACK )     return ERROR;
    
    TWIStop();
    
    return SUCCESS;
}

uint8_t receiveColorsTWI(uint8_t *red, uint8_t *green, uint8_t *blue)
{
    TWIStart();
    
    if (TWIGetStatus() != START )           return ERROR;
    
    *red = TWIRead(true); // read w/ACK
    if (TWIGetStatus() != MT_SLA_ACK )      return ERROR;
    
    *green = TWIRead(true); // read w/ACK
    if (TWIGetStatus() != MT_DATA_ACK )      return ERROR;
    
    *blue = TWIRead(true); // read w/ACK
    if (TWIGetStatus() != MT_DATA_ACK )      return ERROR;
    
}

int main(void) {
    
	initIO();
    initTWI();
    
    uint8_t delay_amount = 20;
    uint8_t red_data, green_data, blue_data;
    
    while (1) {
    
        // set true if Master
        #ifdef true
        for (;;)
        {
            // fade all colors up
            int i;
            for (i = 0; i < 256; i++)
            {
                sendColorsTWI(i,i,i);
                _delay_ms(delay_amount);
            }
            // fade all colors down
            int j;
            for (j = 255; j >= 0; j--)
            {
                sendColorsTWI(i,i,i);
                _delay_ms(delay_amount);
            }
        }
        #endif
        
        
        #ifdef false
        
        for (;;)
        {
            receiveColorsTWI(&red_data, &green_data, &blue_data)
        }
        
        // set false if Slave
        
        #endif
    

	}

	return 0; // never reached
}
