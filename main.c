#include <xc.h>
#include <stdio.h>
//#include <stdlib.h>


#include "uart.h"
#include "ds18b20.h"
#include "dht22.h"
#include "i2c_fw_master.h"
#include "bmp180.h"
#include "wl_tx.h"
//#include "i2c_slave.h"

#define _XTAL_FREQ      8000000 // 8MHz Clock

//http://picguides.com/beginner/config.php
#pragma config BOREN = OFF, MCLRE = ON, CPD = OFF, FOSC = INTRCIO, WDTE = OFF, CP = OFF, PWRTE = ON, IESO=OFF, FCMEN = OFF
//_CP_OFF & _CPD_OFF & _BOR_OFF & _MCLRE_ON & _WDT_OFF & _PWRTE_ON & _INTRC_OSC_NOCLKOUT & _FCMEN_OFF & _IESO_OFF

unsigned char data[48] = {0};
unsigned char scale = 0;

void pic_init()
{
    //Setting Oscillator
    OSCCON = 0b01110000;    //8Mhz Crystal, OSTS - Intrnal Oscillator, HFINTOSC - High Frequency Oscillator(1), LFINTOSC - Low Freq. Oscillator(0), Internal Oscillator is used for Clock

    ANSEL   = 0;
    ANSELH  = 0;
    //Disabling the A2D converter
    ADCON0bits.ADON = 0;
    //CCP1CON = 0;
    TRISCbits.TRISC6 = 0;
    PORTCbits.RC6 = 0;
}

void init_tx_interrupt()
{
    //Enabling Timer 1 interrupt
    T1CONbits.TMR1ON  = 1;
    T1CONbits.T1CKPS0 = 1; // ~ 131ms
    T1CONbits.T1CKPS1 = 1;
    
    PIE1bits.TMR1IE  = 1;
    INTCONbits.PEIE  = 1;
    INTCONbits.GIE   = 1;
}

void interrupt isr()
{
    //handle_i2c();
    
    //PORTCbits.RC6 = 1;
    if(PIR1bits.TMR1IF){
        scale++;
        //Send data in 15s interval
        if( scale == 20 ){
            tx_send_data(40,data);
            scale = 0;
        }
        
        // Re-enabling interrupt
        PIR1bits.TMR1IF = 0;
    }
}


int main(int argc, char** argv)
{
    //char buff[2] = {5, 15};
    
    struct ds18b20 t1 = {
        {0x28, 0x69, 0x55, 0x5d, 0x5, 0, 0, 0x7c},
        0,
        0,
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        0
    };
    
//    struct ds18b20 t2 = {
//        {0x28, 0x6a, 0xf4, 0x8f, 0x6, 0, 0, 0x39},
//        0,
//        0,
//        {0, 0, 0, 0, 0, 0, 0, 0, 0},
//        0
//    };

    struct dht22 d1 = {0};
    
    struct bmp180_t dev = {0};
    
    unsigned short ut;
    unsigned long  up;
    
    //unsigned char data[8] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    

    /*
     * id 1 = 28
       id 2 = 6a
       id 3 = f4
       id 4 = 8f
       id 5 = 6
       id 6 = 0
       id 7 = 0
       id 8 = 39*/

    /*
    id 1 = 28
    id 2 = 69
    id 3 = 55
    id 4 = 5d
    id 5 = 5
    id 6 = 0
    id 7 = 0
    id 8 = 7c

     */

    pic_init();
    uart_init();
    ds18b20_init();
    dht22_init();
    i2c_fw_init();
    tx_init();
    
    dev.addr = 0xEF;
    dev.chipid = 0x55;
    //dev.oss_setting = 0;
    //dev.sw_oversample = 0;
    
    bmp180_init(&dev);
    bmp180_read_params(&dev);
    
//    printf("Param AC1 = %d\r\n", dev.params.ac1);
//    printf("Param AC2 = %d\r\n", dev.params.ac2);
//    printf("Param AC3 = %d\r\n", dev.params.ac3);
//    printf("Param AC4 = %d\r\n", dev.params.ac4);
//    printf("Param AC5 = %d\r\n", dev.params.ac5);
//    printf("Param AC6 = %d\r\n", dev.params.ac6);
//    
//    printf("Param B1 = %d\r\n", dev.params.b1);
//    printf("Param B2 = %d\r\n", dev.params.b2);
//    
//    printf("Param MB = %d\r\n", dev.params.mb);
//    printf("Param MC = %d\r\n", dev.params.mc);
//    printf("Param MD = %d\r\n", dev.params.md);

    init_tx_interrupt();

    while(1){
        
        
        ut = bmp180_get_ut(&dev);
        up = bmp180_get_up(&dev);
        
        //ds18b20_read_rom(&t1);
        //printf("======== Sensor 1 ========\r\n");
        ds18b20_start_convert(&t1);
        ds18b20_read_scratch(&t1);
        
        data[0] = (t1.tempi >> 8) & 0xFF;
        data[1] = (t1.tempi) & 0xFF;
        
        data[2] = (t1.tempd >> 8) & 0xFF;
        data[3] = (t1.tempd) & 0xFF;

        __delay_ms(200);

        //printf("======== Sensor 2 ========\r\n");
        //ds18b20_start_convert(&t2);
        //ds18b20_read_scratch(&t2);
        
        // Reusing the previous struct due to variable memory limitation in this PIC
        t1.romcode[0]    = 0x28;
        t1.romcode[1]    = 0x6a;
        t1.romcode[2]    = 0xf4;
        t1.romcode[3]    = 0x8f;
        t1.romcode[4]    = 0x6;
        t1.romcode[5]    = 0;
        t1.romcode[6]    = 0;
        t1.romcode[7]    = 0x39;
        
        t1.scratchpad[0] = 0;
        t1.scratchpad[1] = 0;
        t1.scratchpad[2] = 0;
        t1.scratchpad[3] = 0;
        t1.scratchpad[4] = 0;
        t1.scratchpad[5] = 0;
        t1.scratchpad[6] = 0;
        t1.scratchpad[7] = 0;
        t1.scratchpad[8] = 0;
        
        
        ds18b20_start_convert(&t1);
        ds18b20_read_scratch(&t1);
        
        data[4] = (t1.tempi >> 8) & 0xFF;
        data[5] = (t1.tempi) & 0xFF;
        
        data[6] = (t1.tempd >> 8) & 0xFF;
        data[7] = (t1.tempd) & 0xFF;

        __delay_ms(200);
        
        t1.romcode[0]    = 0x28;
        t1.romcode[1]    = 0x69;
        t1.romcode[2]    = 0x55;
        t1.romcode[3]    = 0x5d;
        t1.romcode[4]    = 0x5;
        t1.romcode[5]    = 0;
        t1.romcode[6]    = 0;
        t1.romcode[7]    = 0x7c;
        
        t1.scratchpad[0] = 0;
        t1.scratchpad[1] = 0;
        t1.scratchpad[2] = 0;
        t1.scratchpad[3] = 0;
        t1.scratchpad[4] = 0;
        t1.scratchpad[5] = 0;
        t1.scratchpad[6] = 0;
        t1.scratchpad[7] = 0;
        t1.scratchpad[8] = 0;

        //printf("===========================\r\n");
        dht22_read(&d1);
        
        
        //TRISCbits.TRISC1 = 0;
        //PORTCbits.RC1 = 0;
        //bmp180_ac1, bmp180_ac2, bmp180_ac3, bmp180_ac4, bmp180_ac5, bmp180_ac6, bmp180_b1, bmp180_b2, bmp180_mb, bmp180_mc, bmp180_md
        //dev.params.ac1, dev.params.ac2, dev.params.ac3, dev.params.ac4, dev.params.ac5, dev.params.ac6, dev.params.b1, dev.params.b2, dev.params.mb, dev.params.mc, dev.params.md
//        printf("READINGS_START\r\n");
//        printf("ow1_tempi, ow1_tempd, ow2_tempi, ow2_tempd, dht22_temp, dht22_humidity\r\n");
//        printf("%d, %d, %d, %d, %d, %d\r\n", t1.tempi, t1.tempd, t2.tempi, t2.tempd, ((d1.ti << 8)  + d1.td), ((d1.rhi << 8) + d1.rhd));
//        printf("bmp180_ac1, bmp180_ac2");
//        printf(", bmp180_ac3, bmp180_ac4");
//        printf(", bmp180_ac5, bmp180_ac6");
//        printf(", bmp180_b1, bmp180_b2");
//        printf(", bmp180_mb, bmp180_mc");
//        printf(", bmp180_md, bmp180_ut, bmp180_up\r\n");
//        printf("%d, %d", dev.params.ac1, dev.params.ac2);
//        printf(", %d, %d", dev.params.ac3, dev.params.ac4);
//        printf(", %d, %d", dev.params.ac5, dev.params.ac6);
//        printf(", %d, %d", dev.params.b1, dev.params.b2);
//        printf(", %d, %d", dev.params.mb, dev.params.mc);
//        printf(", %d, %d, %ld\r\n", dev.params.md, ut, up);
//        printf("READINGS_END\r\n");

       //tx_start();
        
        // Sending high byte first
//        data[0] = (t1.tempi >> 8) & 0xFF;
//        data[1] = (t1.tempi) & 0xFF;
//        
//        data[2] = (t1.tempd >> 8) & 0xFF;
//        data[3] = (t1.tempd) & 0xFF;
//        
//        data[4] = (t2.tempi >> 8) & 0xFF;
//        data[5] = (t2.tempi) & 0xFF;
//        
//        data[6] = (t2.tempd >> 8) & 0xFF;
//        data[7] = (t2.tempd) & 0xFF;
        
        data[8] = d1.ti;
        data[9] = d1.td;
        
        data[10] = d1.rhi;
        data[11] = d1.rhd;
        
        data[12] = (dev.params.ac1 >> 8) & 0xFF;
        data[13] = (dev.params.ac1) & 0xFF;
        
        data[14] = (dev.params.ac2 >> 8) & 0xFF;
        data[15] = (dev.params.ac2) & 0xFF;
        
        data[16] = (dev.params.ac3 >> 8) & 0xFF;
        data[17] = (dev.params.ac3) & 0xFF;
        
        data[18] = (dev.params.ac4 >> 8) & 0xFF;
        data[19] = (dev.params.ac4) & 0xFF;
        
        data[20] = (dev.params.ac5 >> 8) & 0xFF;
        data[21] = (dev.params.ac5) & 0xFF;
        
        data[22] = (dev.params.ac6 >> 8) & 0xFF;
        data[23] = (dev.params.ac6) & 0xFF;
        
        data[24] = (dev.params.b1 >> 8) & 0xFF;
        data[25] = (dev.params.b1) & 0xFF;
        
        data[26] = (dev.params.b2 >> 8) & 0xFF;
        data[27] = (dev.params.b2) & 0xFF;
        
        data[28] = (dev.params.mb >> 8) & 0xFF;
        data[29] = (dev.params.mb) & 0xFF;
        
        data[30] = (dev.params.mc >> 8) & 0xFF;
        data[31] = (dev.params.mc) & 0xFF;
        
        data[32] = (dev.params.md >> 8) & 0xFF;
        data[33] = (dev.params.md) & 0xFF;
        
        data[34] = (ut >> 8) & 0xFF;
        data[35] = (ut) & 0xFF;
        
        data[36] = (up >> 24) & 0xFF;
        data[37] = (up >> 16) & 0xFF;
        data[38] = (up >> 8) & 0xFF;
        data[39] = (up) & 0xFF;
        
        //tx_send_data(40,data);
        //Wait for 1s before starting a new cycle
        __delay_ms(1000);
        
        //After stabilizing and initial readings are taken, enable transmission if timer 1 interrupt is already not set only
        //if(!PIE1bits.TMR1IE)
            //init_tx_interrupt();
    }

    //return (EXIT_SUCCESS);
    return (0);
}
