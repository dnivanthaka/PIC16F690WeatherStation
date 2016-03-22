#include <xc.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//#include <stdint.h>

#include "onewire.h"

//volatile unsigned char *port;
//volatile unsigned char *tris;
//unsigned char pin;

#define ONEWIRE_PORT PORTCbits.RC0
#define ONEWIRE_TRIS TRISCbits.TRISC0


void ow_init()
{
    // Pulling pin high
    //PORTBbits.RB4 = 0;
    //TRISBbits.TRISB4 = 1;
    ONEWIRE_PORT = 0;
    ONEWIRE_TRIS = 1;
}

unsigned char ow_reset()
{
    unsigned char ret = 0;

    di();

    // Pulling pin low
    //PORTBbits.RB4 = 0;
    //TRISBbits.TRISB4 = 0;
    ONEWIRE_PORT = 0;
    ONEWIRE_TRIS = 0;

    __delay_us(490);
    // Pulling up
    //TRISBbits.TRISB4 = 1;
    ONEWIRE_TRIS = 1;
    // Checking for response
    __delay_us(60);
    //if( PORTBbits.RB4 == 0 ){
    if( ONEWIRE_PORT == 0 ){
        //printf("DS18b20 responded\r\n");
        ret = 1;
    }else{
        //printf("ONEWIRE ERROR: Device didnt respond\r\n");
        ret = 0;
    }
    
    ei();
    __delay_us(450);

    return ret;
}

unsigned char ow_read_pin()
{
    //return PORTBbits.RB4;
    return ONEWIRE_PORT;
}

void ow_write_byte(unsigned char data)
{
    unsigned char i;

    di();
    for(i=0;i<8;i++){
        //TRISBbits.TRISB4 = 0;
        ONEWIRE_TRIS = 0;
        __delay_us(15);
        if( data & 1 ){
            //TRISBbits.TRISB4 = 1;
            ONEWIRE_TRIS = 1;
        }
        __delay_us(45);
        //TRISBbits.TRISB4 = 1;
        ONEWIRE_TRIS = 1;
        // Recovery time
        __delay_us(1);
        data = data >> 1;
    }
    ei();
}

unsigned char ow_readbit()
{
    unsigned char data = 0;
    
    di();
    //TRISBbits.TRISB4 = 0;
    ONEWIRE_TRIS = 0;
    __delay_us(1);
    //TRISBbits.TRISB4 = 1;
    ONEWIRE_TRIS = 1;
    __delay_us(5);

   //if(PORTBbits.RB4 == 1)
   if(ONEWIRE_PORT == 1)
       data = 1;
    
    __delay_us(55);
    ei();

    return data;
}

unsigned char ow_read_byte()
{
    unsigned char i;
    unsigned char data = 0;
    
    di();
    for(i=0;i<8;i++){
        data >>= 1;
        //TRISBbits.TRISB4 = 0;
        ONEWIRE_TRIS = 0;
        __delay_us(1);
        //TRISBbits.TRISB4 = 1;
        ONEWIRE_TRIS = 1;
        __delay_us(5);

       //if(PORTBbits.RB4 == 1)
       if(ONEWIRE_PORT == 1)
        data = data | 0x80;

        __delay_us(55);
     }
    ei();

    return data;
}

// Calculate the CRC8 of the byte value provided with the current
// global 'crc8' value.
// Returns current global crc8 value
//
/*
unsigned char docrc8(unsigned char value)
{
   // See Application Note 27

   // TEST BUILD
   crc8 = dscrc_table[crc8 ^ value];
   return crc8;
}
*/
