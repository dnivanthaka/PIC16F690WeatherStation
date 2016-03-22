#include <xc.h>

#include "wl_tx.h"

#define WIRE_TX      PORTCbits.RC7
#define WIRE_TX_TRIS TRISCbits.TRISC7
#define WIRE_RX      PORTCbits.RC1

#define DEVICE_ID    0x01

void sendbit(unsigned char bt)
{
    //Pulse duty encoded
    WIRE_TX = 1;
    __delay_us(80);
    WIRE_TX = 0;
    __delay_us(100);
    if(bt > 0)
        __delay_us(50);
}

void sendbyte(unsigned char data)
{
    unsigned char i;
    
    for(i=0;i<8;i++){
        
        //sendbit((data & 0x80));
        //data = data << 1;
        
        sendbit((data & 0x80));
        data = data << 1;
        
        //sendbit((data & (1 << (7 - i))));
        
        //__delay_us(1);
    }
    
    //sendbit((data & 1));
    
    //__delay_us(250);
}


//void tx_send(void)
//{
//    unsigned char i;
//    //sendbit(1);
//    //sendbit(0);
//    //sendbit(1);
//    //sendbit(0);
//    //sendbyte(0x77);
//    for(i=0;i<14;i++){
//        sendbit(0);
//    }
//    
//    sendbit(1);
//    
//    __delay_us(10);
//    
//    sendbyte(0x55);
//    //sendbyte(0xFF);
//    sendbyte(0x77);
//    sendbyte(0xAA);
//    
//    // Sending stop bits
//    sendbit(0);
//    sendbit(0);
//    //sendbyte(0xFF);
//    
//    __delay_us(10);
//    
//    
//}

unsigned char tx_send_data(unsigned char length, unsigned char *data)
{
    unsigned char i, checksum = 0;

    for(i=0;i<14;i++){
        sendbit(0);
    }
    
    sendbit(1);
    
    __delay_us(10);
    
    sendbyte(0x55);
    sendbyte(DEVICE_ID);
    sendbyte(length);
    sendbyte(0x77);
    
    for(i=0;i<length;i++){
        sendbyte( *(data + i) );
        //checksum += *(data + i);
    }
    
    //sendbyte(checksum);
    
    // Sending stop bits
    sendbit(0);
    sendbit(0);
    
    __delay_us(100);
    
    return 0;
}

void tx_init()
{
    WIRE_TX_TRIS = 0;
    WIRE_TX      = 0;
}