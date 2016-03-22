/* 
 * File:   i2c_fw_master.h
 * Author: dinusha
 *
 * Created on October 20, 2015, 10:33 PM
 */

#ifndef I2C_FW_MASTER_H
#define	I2C_FW_MASTER_H

#ifdef	__cplusplus
extern "C" {
#endif
    #define _XTAL_FREQ      8000000 // 8MHz Clock
    
    void i2c_fw_init();
    void i2c_fw_start();
    void i2c_fw_stop();
    void i2c_fw_repstart();
    unsigned char i2c_fw_read_status();
    void i2c_fw_send_ack();
    void i2c_fw_send_nack();
    void i2c_fw_write_byte(unsigned char data);
    unsigned char i2c_fw_read_byte();
    unsigned char i2c_fw_write_and_read(unsigned char addr, unsigned char reg, unsigned char data_len, unsigned char *data);
    unsigned char i2c_fw_write_only(unsigned char addr, unsigned char reg, unsigned char value);
    

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_FW_MASTER_H */

