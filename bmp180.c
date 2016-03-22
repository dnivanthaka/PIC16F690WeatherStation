#include <xc.h>
//#include <stdio.h>


#include "bmp180.h"
#include "i2c_fw_master.h"

unsigned char bmp180_init(struct bmp180_t *device)
{
//    i2c_fw_init();
//    //i2c_fw_write_byte(0b11111111);
//    i2c_fw_stop();
    unsigned char data;
    i2c_fw_write_and_read((device->addr>>1), 0xD0, 1, &data);
    //printf("data = %x\r\n", data);
    
    if(data = device->chipid){
        //printf("BMP180 Device Detected!!!\r\n");
    }
    
    return 0;
}

//void bmp180_init_defaults()
//{
//
//}

unsigned char bmp180_read_params(struct bmp180_t *device)
{
    unsigned char i, j = 0;
    unsigned char data[22];
    
    for(i=0xAA;i<=0xBF;i++){
        i2c_fw_write_and_read((device->addr>>1), i, 1, &data[j]);
        //printf("Param value %d - %x\r\n", j, data[j]);
        j++;
    }
    //00000000
    device->params.ac1 = ((signed short)data[0] << 8) | data[1];
    device->params.ac2 = ((signed short)data[2] << 8) | data[3];
    device->params.ac3 = ((signed short)data[4] << 8) | data[5];
    device->params.ac4 = ((unsigned short)data[6] << 8) | data[7];
    device->params.ac5 = ((unsigned short)data[8] << 8) | data[9];
    device->params.ac6 = ((unsigned short)data[10] << 8) | data[11];
    
    device->params.b1  = ((signed short)data[12] << 8) | data[13];
    device->params.b2  = ((signed short)data[14] << 8) | data[15];
    
    device->params.mb  = ((signed short)data[16] << 8) | data[17];
    device->params.mc  = ((signed short)data[18] << 8) | data[19];
    device->params.md  = ((signed short)data[20] << 8) | data[21];
    
//    printf("Param AC1 = %d\r\n", device->params.ac1);
//    printf("Param AC2 = %d\r\n", device->params.ac2);
//    printf("Param AC3 = %d\r\n", device->params.ac3);
//    printf("Param AC4 = %d\r\n", device->params.ac4);
//    printf("Param AC5 = %d\r\n", device->params.ac5);
//    printf("Param AC6 = %d\r\n", device->params.ac6);
//    
//    printf("Param B1 = %d\r\n", device->params.b1);
//    printf("Param B2 = %d\r\n", device->params.b2);
//    
//    printf("Param MB = %d\r\n", device->params.mb);
//    printf("Param MC = %d\r\n", device->params.mc);
//    printf("Param MD = %d\r\n", device->params.md);
        
    
    return 0;
}


unsigned short bmp180_get_ut(struct bmp180_t *device)
{
    unsigned char data[2];
    
    i2c_fw_write_only((device->addr>>1), 0xF4, 0x2E);
    __delay_ms(5);
    i2c_fw_write_and_read((device->addr>>1), 0xF6, 1, &data[0]);
    i2c_fw_write_and_read((device->addr>>1), 0xF7, 1, &data[1]);
    
    //printf("UT = %d\r\n", data[0] << 8 | data[1]);
    
    return ((unsigned short)data[0] << 8 | data[1]);
}

unsigned long bmp180_get_up(struct bmp180_t *device)
{
    unsigned char data[3];
    unsigned long tmp = 0;
    
    i2c_fw_write_only((device->addr>>1), 0xF4, (0x34 | (device->oss_setting<<6)));
    __delay_ms(3);
    i2c_fw_write_and_read((device->addr>>1), 0xF6, 1, &data[0]);
    i2c_fw_write_and_read((device->addr>>1), 0xF7, 1, &data[1]);
    i2c_fw_write_and_read((device->addr>>1), 0xF8, 1, &data[2]);
    
    tmp = (unsigned long)data[0] << 16 | (unsigned long)data[1] << 8;
    //tmp2 = (unsigned long)data[1] << 8;
    //tmp3 = (unsigned long)data[2];
    
    //printf("UP = %ld\r\n", (unsigned long)(tmp | data[2]) >> (8 - device->oss_setting));
    
    return (unsigned long)(tmp | data[2]) >> (8 - device->oss_setting);
}