/* 
 * File:   bmp180.h
 * Author: dinusha
 *
 * Created on November 1, 2015, 12:59 PM
 */

#ifndef BMP180_H
#define	BMP180_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    struct bmp180_calib_t {
        signed short    ac1;
        signed short    ac2;
        signed short    ac3;
        unsigned short  ac4;
        unsigned short  ac5;
        unsigned short  ac6;
        signed short    b1;
        signed short    b2;
        signed short    mb;
        signed short    mc;
        signed short    md;
    };
    
    struct bmp180_t {
        struct bmp180_calib_t params;
        unsigned char   addr;
        unsigned char   pmode;
        unsigned char   chipid;
        //unsigned short  number_of_samples;
        unsigned char    oss_setting;
        //signed short    sw_oversample;
    };

    unsigned char bmp180_init(struct bmp180_t *device);
    unsigned char bmp180_read_params(struct bmp180_t *device);
    unsigned short bmp180_get_ut(struct bmp180_t *device);
    unsigned long bmp180_get_up(struct bmp180_t *device);
    


#ifdef	__cplusplus
}
#endif

#endif	/* BMP180_H */

