#include <xc.h>
#include <stdio.h>
//#include <stdlib.h>

#include "uart.h"

void uart_init()
{
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB5 = 1;
    //SPBRG = 0x19;       //9600 baud rate
    //TXEN = 1;
    //BRGH = 1;
    //SPEN = 1;
    //CREN = 1;
    TXREG = 0;
    RCREG = 0;
    //SPBRG = 0x40;       //20Mhz clock
    BRG16 = 0;
    BRGH  = 1;
    SYNC  = 0;
    //SPBRG = 25;     //19200
    //SPBRG = 51;       //9600
    SPBRG = 207;       //2400
    
    TXSTA = 0x24;
    RCSTA = 0x90;
}

void putchar(char data)
{
    while( !TXIF )
        continue;
    TXREG = data;
}

char getchar()
{
    while( !RCIF )
        continue;
    return RCREG;
}

