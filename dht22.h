/*
 * File:   dht22.h
 * Author: dinusha
 *
 * Created on February 20, 2015, 7:20 PM
 */

#ifndef DHT22_H
#define	DHT22_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
//#include <stdbool.h>

#define _XTAL_FREQ      8000000 // 8MHz Clock
#define DHT22_OK        0
#define DHT22_TIMEOUT   1
#define DHT22_INVALID   2

struct dht22 {
    unsigned char rhi, rhd, ti, td, csum;
    //short rh, tm;
    //unsigned char csum;
};

//#define DHT22PIN  RB4
//#define DHT22TRIS TRISB

void dht22_init();
unsigned char dht22_read(struct dht22 *);
//bool dht22_verify(struct dht22 *);


#ifdef	__cplusplus
}
#endif

#endif	/* DHT22_H */



