#include <xc.h>
#include "timer.h"
#define  _XTAL_FREQ 16000000
void configurazione(void);
unsigned char numeri [] = {
    0b11001111,
    0b10001000,
    0b01101101,
    0b10101101,
    0b10101010,
    0b10100111,
    0b11100111,
    0b10001100,
    0b11101111,
    0b10101110
};

void main(void) {
    configurazione();
    PORTC = 0b10000000;
    for (unsigned char i = 0; i < 11; i++) {
        PORTD = numeri[i];
        for (unsigned char c = 0; c < 50; c++) {
            __delay_ms(10);
        }
    }

}

void configurazione(void) {
    LATA = 0;
    TRISA = 0b11000000;

    LATB = 0;
    TRISB = 0xFF;

    LATC = 0;
    TRISC = 0x00;

    LATD = 0;
    TRISD = 0x00;

    CMCON = 0x07;
    UCONbits.USBEN = 0;
    UCFGbits.UTRDIS = 0;
}