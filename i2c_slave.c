#include <xc.h>
#include <stdio.h>

#include "i2c_slave.h"

#define I2CSLAVEADD 0x40
#define I2CBUFFER   4

unsigned char SSP_bufIndex;
unsigned char SSPBuffer[I2CBUFFER] = {0, 0, 0, 0};


void init_i2cslave()
{
    SSPEN   = 0;
    //RC3 and RC4 as inputs - SCL, SDA
    TRISCbits.TRISC6 = 1;
    TRISCbits.TRISC4 = 1;

    SSPEN   = 1;

    SSPIF   = 0;
    //7 bit slave mode
    SSPCON  = 0b00110110;
    //Setting Address
    SSPADD  = (I2CSLAVEADD << 1);
    SSPSTAT = 0;
    SSPSTATbits.SMP = 1;
    SSPBUF  = 0;
    WCOL    = 0;
    SSPOV   = 0;
    CKP     = 1;
    PIE1bits.SSPIE = 1;
    //Peripheral interrupt enable
    PEIE    = 1;
    //Global interrupt enable
    GIE     = 1;
}

void handle_i2c()
{
//unsigned char SSP_bufIndex;
unsigned char BUF_temp;
static unsigned char *byte_ptr;
static unsigned char register_index;


SSPIF = 0; // Clear Interrupt Flag

BUF_temp = SSPBUF; // Clear BF flag

if (SSPSTATbits.S) // If Start bit was detected
    {
    //printf("Im at start bit\r\n");
    if (D_nA == 0) // If received byte is my address
    {
        //printf("Im at address detected\r\n");
        SSP_bufIndex = 0; // Initialize byte count
    } else // Reading or writing data
    {
        //printf("XXXXXXXXXXXXXXXXXXXXX\r\n");
        if (R_nW) // If Master is reading
        {
            //printf("Im at reading\r\n");
            if (SSP_bufIndex == 0) {
                // This is the first byte in a Read request..
                // Point to the beginning of our data buffer
                byte_ptr = SSPBuffer;
                //if(BUF_temp == 0x22){
                    //TRISBbits.TRISB2 = 0;
                    //PORTBbits.RB2 = 1;
                    //SSPBUF = 0x11;
                //}
                
            }
            SSPBUF = 0x11;
            //SSPBUF = *byte_ptr++; // pointer math is a little faster than SSP_bufIndex dereference
            if (ACKSTAT == 0) {
                // Master signaled that the message is over
            }
        } else // Master is writing
        {
            //printf("Im at writing\r\n");
            if (SSP_bufIndex == 0) { // First byte in a Write command
                //registerIndex = BUF_temp;
                //printf("Im at writing %x\r\n", BUF_temp);
                if(BUF_temp == 0x55){
                    
                    TRISBbits.TRISB2 = 0;
                    PORTBbits.RB2 = 1;
                }
            }
            else if (SSP_bufIndex == 1) {
                SSPBuffer[register_index] = BUF_temp;
            } else
            {
                // Extra byte(s) in the Write command, so ignore them
            }
        }
        SSP_bufIndex++;
    }
}
if (WCOL | SSPOV) // If Write Collision Error or Overflow
{
    printf("WCOL|SSPOV Collision\r\n");
    init_i2cslave(); // Reset I2C module on error condition
}
CKP = 1; // Ensure SCL is released

}

/*
 * movlw	b'01010000'		; RB4 and RB6 as inputs - SDA, SCL
	movwf	TRISB
 ; Configuring I2C Peripheral
	banksel	PIR1
	;clrf	PIR1
	bcf		PIR1, SSPIF

	;movlw	b'00110110'
	movlw	b'00110110'
			;-----0110		; 7 bit slave mode
	banksel	SSPCON
	movwf	SSPCON

	movlw	DEVICE_ADDR
	banksel	SSPADD			; Setting Address
	movwf	SSPADD

	banksel	SSPSTAT
	clrf	SSPSTAT

	banksel	SSPBUF
	clrf	SSPBUF

	;Enabling Interrupts
	banksel	PIE1
	bsf		PIE1, SSPIE

	banksel	INTCON
	bsf		INTCON, PEIE	; Peripheral interrupt enable
	bsf		INTCON, GIE		; Global interrupt enable
 *
 * =========================================================================
 * SSP_Handler
	banksel	SSPSTAT
	movf	SSPSTAT, w
	andlw	b'00101101'
	banksel	temp
	movwf	temp

State1
	banksel	temp
	movf	temp, w
	xorlw	b'00001001'
	btfss	STATUS, Z
	goto	State2
	banksel	SSPBUF
	movf	SSPBUF, w
	banksel	buffer
	movwf	buffer

	return

State2
	banksel	temp
	movf	temp, w
	xorlw	b'00101001'
	btfss	STATUS, Z
	goto	State3
	banksel	SSPBUF
	movf	SSPBUF, w
	banksel	buffer
	movwf	buffer
	return


State3
	banksel	temp
	movf	temp, w
	andlw	b'00101100'
	xorlw	b'00001101'
	btfss	STATUS, Z
	goto	State4

	return


State4
	banksel	temp
	movf	temp, w
	xorlw	b'00101100'
	btfss	STATUS, Z
	goto	State5

	return


State5

	return
 */
