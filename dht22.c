#include <xc.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>

#include "dht22.h"

#define DHT22_TRIS TRISCbits.TRISC1
#define DHT22_PORT PORTCbits.RC1


void dht22_init()
{
    // Making sensor pin as input
    //TRISBbits.TRISB5 = 1;
    //PORTBbits.RB5    = 0;
    DHT22_TRIS = 1;
    DHT22_PORT = 0;

    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.T0SE = 0;
    OPTION_REGbits.PSA  = 0;
    OPTION_REGbits.PS0  = 0;
    OPTION_REGbits.PS1  = 0;
    OPTION_REGbits.PS2  = 0;

    // 1s delay as per datasheet
    __delay_ms(1000);
}

unsigned char dht22_read(struct dht22 *s)
{
    unsigned char i, p;
    unsigned char *ptr = &(s->rhi);
    
    di();
    // Sending start condition
    //PORTBbits.RB5    = 0;
    //TRISBbits.TRISB5 = 0;
    DHT22_PORT = 0;
    DHT22_TRIS = 0;

    __delay_ms(2);

    //PORTBbits.RB5    = 1;
    //TRISBbits.TRISB5 = 0;
    DHT22_PORT = 1;
    DHT22_TRIS = 0;

    //PORTBbits.RB5    = 0;
    //TRISBbits.TRISB5 = 1;
    DHT22_PORT = 0;
    DHT22_TRIS = 1;
    __delay_us(40);

    // Wait for sensor response
    //while(PORTBbits.RB5);
    while(DHT22_PORT);
    // Discard 1st bit
    //while(! PORTBbits.RB5);
    while(! DHT22_PORT);
    //while(PORTBbits.RB5);
    while(DHT22_PORT);

    //__delay_us(75);
    // Reading data
    for(i=0;i<5;i++){
        for(p=0;p<8;p++){
        //p = i / 8;
            ptr[ i ] <<= 1;
            //while(! PORTBbits.RB5);
            while(! DHT22_PORT);
            TMR0 = 0;
            //while(PORTBbits.RB5);
            while(DHT22_PORT);
            if(TMR0 > 40){
                ptr[ i ] |= 1;
            }
        }
    }

    ei();

    // Verify data
    if( (unsigned char)(s->rhd + s->rhi + s->td + s->ti) != s->csum ){
        //printf("Invalid data\r\n");
    }

    int temp     = (s->ti << 8)  + s->td;
    int humidity = (s->rhi << 8) + s->rhd;

    //printf("Temp. = %dC\r\n",  temp);
    //printf("Humid = %d%%\r\n",  humidity);
    //printf("csum = %d\r\n",  s->csum);
    //printf("act = %d\r\n", (unsigned char)(s->rhd + s->rhi + s->td + s->ti));
    //printf("val = %d\r\n", i / 8);
    return 0;
}

