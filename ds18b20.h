/* 
 * File:   ds18b20.h
 * Author: dinusha
 *
 * Created on October 6, 2015, 10:02 PM
 */

#ifndef DS18B20_H
#define	DS18B20_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
//#include <stdint.h>

#define DS18B20_9BIT    0
#define DS18B20_10BIT   1
#define DS18B20_11BIT   2
#define DS18B20_12BIT   3  

struct ds18b20{
        unsigned char romcode [8];
        signed int tempi;
        unsigned short tempd;
        unsigned char scratchpad[9];
        unsigned char config;
        //unsigned char crc;
};

void ds18b20_init();
void ds18b20_start_convert(struct ds18b20 *dev);
void ds18b20_read_rom(struct ds18b20 *dev);
void ds18b20_read_scratch(struct ds18b20 *dev);
void ds18b20_write_scratch(struct ds18b20 *dev);
void ds18b20_copy_scratch();
void ds18b20_recall_eeprom();
void ds18b20_set_resolution(struct ds18b20 *dev, unsigned char res);


#ifdef	__cplusplus
}
#endif

#endif	/* DS18B20_H */

