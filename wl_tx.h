/* 
 * File:   wireless433.h
 * Author: dinusha
 *
 * Created on November 3, 2015, 8:52 PM
 */

#ifndef WIRELESS433_H
#define	WIRELESS433_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    #define _XTAL_FREQ      8000000 // 8MHz Clock

    void sendbit(unsigned char bt);
    void sendbyte(unsigned char data);
    //void tx_send();
    unsigned char tx_send_data(unsigned char length, unsigned char *data);
    void tx_init();
    
    //static unsigned char data[16] = {0};


#ifdef	__cplusplus
}
#endif

#endif	/* WIRELESS433_H */

