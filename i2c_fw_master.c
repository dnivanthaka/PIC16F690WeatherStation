#include <xc.h>

#include "i2c_fw_master.h"

#define I2C_SDA      TRISBbits.TRISB4
#define I2C_SDA_DATA PORTBbits.RB4
#define I2C_SCL      TRISBbits.TRISB6


void i2c_fw_init()
{   
    I2C_SDA = 1;
    I2C_SCL = 1;
    
    SSPCON = 0b00101011;
    
    //PIE1bits.SSPIE = 1;
    SSPIF = 0;
    //Peripheral interrupt enable
    //PEIE    = 1;
    //Global interrupt enable
    //GIE     = 1;
}

void i2c_fw_start()
{
    di();
    //Start with both lines high
    I2C_SDA = 1;
    I2C_SCL = 1;
    //Pulling SDA low while SCL is high
    I2C_SDA = 0;
    __delay_us(1);
    //Pulling SCL also low
    I2C_SCL = 0;
    ei();
}

void i2c_fw_stop()
{
    di();
    //Start with SCL high and SDA low
    I2C_SDA = 0;
    I2C_SCL = 1;
    __delay_us(1);
    //Pulling SDA high
    I2C_SDA = 1;
    __delay_us(1);
    ei();
}

void i2c_fw_repstart()
{
    di();
    //Start with both lines low
    I2C_SDA = 1;
    I2C_SCL = 1;

    //I2C_SCL = 1;
    __delay_us(1);
    I2C_SDA = 0;
    __delay_us(1);
    I2C_SCL = 0;
    __delay_us(1);
    
    //I2C_SDA = 1;
    //__delay_us(1);
    ei();
}

unsigned char i2c_fw_read_status()
{
    unsigned char ret = 1;
    
    di();
    
    I2C_SDA = 1;
    I2C_SCL = 1;
    //__delay_us(1);
    ret = I2C_SDA_DATA;
    __delay_us(1);
    I2C_SCL = 0;
    ei();
    return ret;
}

void i2c_fw_send_ack()
{
    di();
    //Start with SCL & SDA low
    I2C_SDA = 1;
    I2C_SCL = 0;

    //I2C_SDA = 0;
    //__delay_us(1);
    I2C_SCL = 1;
    __delay_us(1);
    I2C_SCL = 0;
    ei();    
}

void i2c_fw_send_nack()
{
    di();
    //Start with SCL & SDA low
    //I2C_SDA = 1;
    I2C_SCL = 0;

    I2C_SDA = 0;
    //__delay_us(1);
    I2C_SCL = 1;
    __delay_us(1);
    I2C_SCL = 0;
    ei();
}

void i2c_fw_write_byte(unsigned char data)
{
    unsigned char i;

    di();
    
    //Start with SCL & SDA low
    I2C_SDA = 0;
    I2C_SCL = 0;

    for(i=0;i<8;i++){
        if(data & (1 << 7))
              I2C_SDA = 1;

        I2C_SCL = 1; 
        __delay_us(1);

        I2C_SCL = 0;
        I2C_SDA = 0;

        data = data << 1;
    }

    //9th bit ACK
    //I2C_SCL = 1;
    ei();    
}

unsigned char i2c_fw_read_byte()
{
    unsigned char i, data = 0;

    di();
    
    //Start with SCL low & SDA high (input)
    I2C_SCL = 0;
    I2C_SDA = 1;
    

    for(i=0;i<8;i++){
        I2C_SCL = 1;
        //__delay_us(1);
        if(I2C_SDA_DATA)
                data = data | (1 << (7 - i));

        I2C_SCL = 0;
        //data = data<<1;        
    }

    ei();
    
    return data;
}

unsigned char i2c_fw_write_and_read(
unsigned char addr, unsigned char reg, unsigned char data_len, unsigned char *data)
{
    unsigned char i;
    //i2c_fw_init();
    i2c_fw_start();
    // Writing to device
    i2c_fw_write_byte((addr<<1) & 0xFE);
    // Reading ACK
    i2c_fw_read_status();
    // Writing Register Address
    i2c_fw_write_byte(reg);
    // Reading ACK
    i2c_fw_read_status();
    // Repeated start
    i2c_fw_repstart();
    // Reading from device
    i2c_fw_write_byte((addr<<1) | 0x01);
    // Reading ACK
    i2c_fw_read_status();
    
    // Reading Data
    for(i=0;i<data_len;i++){
        *(data + i) = i2c_fw_read_byte();
    }
    i2c_fw_send_ack();
    //printf("data = %x\r\n", data);
    
    
    
    //i2c_fw_write_byte(0b11111111);
    i2c_fw_stop();
    
    return 0;
}

unsigned char i2c_fw_write_only(unsigned char addr, unsigned char reg, unsigned char value)
{
    i2c_fw_start();
    // Writing to device
    i2c_fw_write_byte((addr<<1) & 0xFE);
    // Reading ACK
    i2c_fw_read_status();
    // Writing Register Address
    i2c_fw_write_byte(reg);
    // Reading ACK
    i2c_fw_read_status();
	// Writing Register Value
	i2c_fw_write_byte(value);
    // Reading ACK
    i2c_fw_read_status();
    i2c_fw_stop();
    
    return 0;
}
